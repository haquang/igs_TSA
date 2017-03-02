/*
 * FSRSensor.cpp
 *
 *  Created on: Aug 17, 2016
 *      Author: haquang
 */

#include "AdmittanceController.h"

AdmittanceController::AdmittanceController() {
	// TODO Auto-generated constructor stub

}

AdmittanceController::AdmittanceController(comedi_t* dev, int subDevADC, int forceChannelRight,int forceChannelLeft, int range) :DaqDevice(dev){
	_subDevADC = subDevADC;
	_forceChannelRight = forceChannelRight;
	_forceChannelLeft = forceChannelLeft;
	_rangeIDX = range;
}

double AdmittanceController::getForce() {
	return _ext_force;
}

void AdmittanceController::setPerceiveStiffness(double stiffness,double viscosity) {
	_stiffness = stiffness;
	_viscosity = viscosity;
}

void AdmittanceController::updatePerceiveStiffness() {
	for (int i = 0; i < _x_vect.size(); ++i) {
		if (abs(_position) <= abs(_x_vect[i])){
			_stiffness = _stiffness_vect[i];
			_proxy_mass = _mass_vect[i];
			_viscosity = _damper_vect[i];
			return;
		}
	}
	return;

}


double AdmittanceController::getProxyPosition(){
	return _proxy_pos;
}

void AdmittanceController::setCurPosition(double pos) {
	_prv_position = _position;
	_position = pos;

//	double v = (_position - _prv_position)/_deltaT;
//	_velocity_filter.push_front(v);
//
//	if (_velocity_filter.size() < VELOCITY_FILTER_SIZE)
//		_velocity = v;
//	else {
//		double sum = 0;
//
//		for (std::list<double>::iterator it=_velocity_filter.begin(); it != _velocity_filter.end(); ++it)
//			sum += *it;
//
//		_velocity = sum / VELOCITY_FILTER_SIZE;
//		_velocity_filter.pop_back();
//	}



}
void AdmittanceController::setSpeed(double vel) {
	_velocity = vel;
}

void AdmittanceController::forceControl() {

	_prv_err_force = _err_force;
	_err_force = _des_force - _ext_force;

	_prv_vm_position = _vm_position;

	_force_err_integral += _err_force;

	_vm_position = _spring * _err_force - _damper * (_prv_err_force - _err_force) + _integral * _force_err_integral;

	_vm_velocity = _vm_position - _prv_vm_position;
	// Saturation
	if (_vm_position < 0)
		_vm_position = 0;
	else if (_vm_position > MAX_POS){
		_vm_position = MAX_POS;

	}
}

void AdmittanceController::setExtForce(double extForce){
	_ext_force = extForce;
}

void AdmittanceController::setCollidePos(double pos) {
	_collide_position = pos;
}

void AdmittanceController::runPalpation(float extForce) {
	/*
	 *  fixed stiffness
	 */
	// update external force
	setExtForce(extForce);

	if (_position < _collide_position)
	{
		_deltaX = abs(_position - _collide_position);
		_des_force = -_stiffness * _deltaX;
	} else {
		_des_force = 0;
	}

}


void AdmittanceController::runDrill(float extForce) {

	setExtForce(extForce);

	if (_position < _collide_position)
	{
		_deltaX = abs(_position - _collide_position);

		if (_deltaX <= FIRST_LAYER)
			_des_force = -_high_stiffness * _deltaX-_viscosity*_velocity;
		else
			_des_force = -_stiffness * _deltaX - 2 * _viscosity*_velocity;

	} else {
		_des_force = 0;
	}
}

void AdmittanceController::runStep(float extForce) {

	setExtForce(extForce);

	double pos = abs(_position);
	if (pos == 0){
		_vm_position = _position;
	} else {
		if ((pos <= FIRST_LAYER)&& (false == _puncture))
			_des_force = -0.5 * pos - _viscosity * _velocity;
		else {
			_des_force = -_stiffness * pos -  _viscosity * _velocity;
			_puncture = true;
		}
	}

	if (_des_force > 0)
		_des_force = 0;
	// force control
	forceControl();
}



double AdmittanceController::getCmdCurrent(){
	return _cmd_current;
}

double AdmittanceController::getDesignForce(){
	return _des_force;
}

double AdmittanceController::getVirtualMassPosition() {
	return _vm_position;
}

void AdmittanceController::setSamplingTime(double dT) {
	_deltaT = dT;
}

AdmittanceController::~AdmittanceController() {
	// TODO Auto-generated destructor stub
}


void AdmittanceController::setForce(double force) {
	_des_force = force;
}













/*Crappy*/


void AdmittanceController::loadStiffnessRange(string filename) {
	ifstream ifs(filename.c_str());
	string line;
	stringstream split;
	double val;
	if (ifs) {
		getline(ifs,line);
		while (!line.empty()){
			split.clear();
			split.str(line);
			split << line;

			split >> val;
			_x_vect.push_back(val);

			split >> val;
			_stiffness_vect.push_back(val);

			split >> val;
			_mass_vect.push_back(val);

			split >> val;
			_damper_vect.push_back(val);

			getline(ifs,line);
		}
	}
	// Print

	for (int j = 0; j < _x_vect.size();j++){
		printf("Position :%4.4f,Stiffness :%4.4f \n",_x_vect[j],_stiffness_vect[j]);
	}

}


double AdmittanceController::getStiffness(){
	return _m_stiffness;
}

void AdmittanceController::stiffnessControl() {
	_prv_err_stiffness = _err_stiffness;
	_err_stiffness = _m_stiffness - _stiffness;
#if CURRENT_CONTROL
	_cmd_current = _spring * _err_stiffness + _damper * (_prv_err_stiffness - _err_stiffness);
#else
	_vm_position = _spring * _err_stiffness + _damper * (_prv_err_stiffness - _err_stiffness);
#endif
}

void AdmittanceController::run(float extForce) {
	// update external force
	float f = abs(extForce);
	if (f <= EPSILON)
		f = 0;

	_ext_force = f ;

	_prv_err_force = _err_force;

	//	if (0 <= _position)
#if PROXY
	double deltaPosition = abs(_position - _proxy_pos);
	if (deltaPosition <= COMPRESSION_SPRING_LENGTH)
		_des_force = -_stiffness * (COMPRESSION_SPRING_LENGTH - deltaPosition) - _viscosity * _vm_velocity/_deltaT;
	else {
		_vm_position = _position;
	}
#else
	_des_force = -_stiffness * (_position-_proxy);
#endif
	//	_des_force = -_stiffness * _position;
	//	else
	//		_des_force = 0;

	//	printf("des force: %4.3f \n",_des_force);
	_err_force = _des_force + _ext_force;

	// force control
	forceControl();

}

void AdmittanceController::run() {
	// update external force
	readForce();

	// solve derivative equation
	solve();
}



void AdmittanceController::readForce(){
#if SINGLE_SENSOR
	if ((COMEDI_ERROR == readData(_subDevADC,_forceChannelRight,_rangeIDX,_aref))){
		_ext_force = 0.0f;
	} else {
		_ext_force = getVolAnalogInput(_forceChannelRight);
		//				printf("%2.2f\n",_extForce);
	}
#else
	if ((COMEDI_ERROR == readData(_subDevADC,_forceChannelRight,_rangeIDX,_aref)) || (COMEDI_ERROR == readData(_subDevADC,_forceChannelLeft,_rangeIDX,_aref))){
		_extForce = 0.0f;
	} else {
		_extForce = getVolAnalogInput(_forceChannelRight) - getVolAnalogInput(_forceChannelLeft);
		//	printf("%2.2f\n",_extForce);
	}
#endif
}
void AdmittanceController::solve() {
	// calculate new acceleration
	_vm_acceleration = (-_err_force -  _vm_velocity * _damper -  _spring * _vm_position)/_mass;
	// Update velocity
	_vm_velocity = _vm_velocity + _deltaT * _vm_acceleration;
	// Update position
	_vm_position = _vm_position + _deltaT * _vm_velocity;
	//
	//	printf("ACC: %4.2f\n",_vm_acceleration);
	//	printf("VEL: %4.2f\n",_vm_velocity);
	printf("POS: %4.6f\n",_vm_position);

	// Saturation
	//	if (_vm_position < 0)
	//		_vm_position = 0;
	//	else if (_vm_position > MAX_POS)
	//		_vm_position = MAX_POS;
}

void AdmittanceController::setMassSpringDamperModel(double m, double k, double b,double i) {
	_mass = m;
	_spring = k;

	//	_damper = 2 * _mass *sqrt(_spring/_mass);
	_damper = b;
	_integral = i;
}

double AdmittanceController::reset() {
	_ext_force = 0.0f;
	_vm_position = 0.0f;
	_vm_velocity = 0.0f;
	_vm_acceleration = 0.0f;
}

void AdmittanceController::runSuture(float extForce) {

	float f = abs(extForce);
	if (f <= EPSILON)
		f = 0;

	_ext_force = f ;

	double deltaPosition = abs(_position - _proxy_pos);

	if ((_position <= FIRST_LAYER) && (false == _puncture))
		_des_force = -2.0 * (COMPRESSION_SPRING_LENGTH - deltaPosition);
	else{
		_des_force = -_stiffness * (COMPRESSION_SPRING_LENGTH - deltaPosition) - _viscosity * _vm_velocity;
		_puncture = true;
	}


	// force control
	forceControl();
}





double AdmittanceController::getPerceivedStiffness() {
	return _stiffness;
}

double AdmittanceController::getCurBound() {
	return _proxy;
}

void AdmittanceController::setProxyMass(double mass) {
	_proxy_mass = mass;
}

void AdmittanceController::updateProxyPosition(){
	double F, acc;
#if DRILL
	// Spring force on proxy
	double deltaPosition = abs(_position - _proxy_pos);
	F = _stiffness * (COMPRESSION_SPRING_LENGTH - deltaPosition) - _viscosity * _proxy_vel;
	// Update position
	acc = F/_proxy_mass;
	_proxy_vel += acc * _deltaT;
	_proxy_pos += _proxy_vel * _deltaT;
#else
	_proxy_pos = _proxy;
#endif
}
