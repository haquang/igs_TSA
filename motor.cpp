/*
 * Motor.cpp
 *
 *  Created on: Aug 12, 2016
 *      Author: haquang
 */

#include "motor.h"

Motor::Motor() {
	// TODO Auto-generated constructor stub
	_integral = 0.0f;
}


Motor::Motor(comedi_t* dev, int subDevDAC,int subDevADC,int subDevDIN,int subDevCounter, int measureChannel,int controlChannel, int enableMotor,int counterChannel, int range):DaqDevice(dev) {
	_subDevDAC = subDevDAC;
	_subDevADC = subDevADC;
	_subDevDIN = subDevDIN;
	_subDevCounter = subDevCounter;
	_actSpeedChannel = measureChannel;
	_setValueChanel = controlChannel;
	_enableChannel = enableMotor;
	_counterChannel = counterChannel;
	_rangeIDX = range;

	comedi_dio_config(dev,_subDevDIN,_enableChannel,COMEDI_OUTPUT);
}


void Motor::setPidParameters(double Kp, double Ki, double Kd) {
	_Kp = Kp;
	_Ki = Ki;
	_Kd = Kd;
}

void Motor::setCalibrationMode(){
	_calibrating = true;
}

void Motor::resetCalibrationMode(){
	_calibrating = false;
	_actPosition = 0;
}

bool Motor::isCalibrationMode(){
	return _calibrating;
}

void Motor::run() {
	if (_calibrating)
		return;


#if (!THETA_CONTROL)
	// Update actual position
	double prv_position = _actPositionTrans;
	updateActualPositon();
	convertRotation2Translation();

	// Calculate set current

	_err = _desPosition - _actPositionTrans;
	_actSpeed = (_actPositionTrans - prv_position)/_deltaT;
	_integral += _err * _deltaT;
	_setCurent = _Kp * _err + _Ki * _integral - _Kd * _actSpeed;  // PI Controller
//
//	_prv_error = _err;
//
#else
	// Update actual position
		double prv_position = _theta;
		updateActualPositon();
//		convertRotation2Translation();
		_theta = (double) _actPosition / ENCODER_RESOLUTION * 2 * M_PI;
		// Calculate set current

		_err = _desPosition - _theta;
		_actSpeed = (_theta - prv_position)/_deltaT;
		_integral += _err * _deltaT;
		_setCurent = _Kp * _err + _Ki * _integral - _Kd * _actSpeed;  // PI Controller
	//
	//	_prv_error = _err;
	//
#endif

	if (_setCurent > MAX_CURRENT)
		_setCurent = MAX_CURRENT;
	else if (_setCurent < -MAX_CURRENT)
		_setCurent = -MAX_CURRENT;

	convertCurrent2Volt();
//	_setDACVolt = 0.0;
	if (COMEDI_ERROR == writeData(_subDevDAC,_setValueChanel,_rangeIDX,_aref,_setDACVolt)){
//			printf("Error writing to subdev %d at channel %d value %2.2f \n",_subDevDAC,_setValueChanel,_setSpeedVolt);
	}
}


Motor::~Motor() {
	// TODO Auto-generated destructor stub
}

int Motor::updateActualPositon() {
	if (COMEDI_OK == readCounter(_subDevCounter,_counterChannel))
		_actPosition = getCounter(_counterChannel);

//		printf("Counter: %d %d\n ",_counterChannel,_actPosition);
}

void Motor::convertVolt2Speed() {
	// Voltage Range : -4 -> 4 V
	// Speed Range : -10000 -> 10000 RPM
	_actSpeed = 2000 * _actSpeedVolt ;
}

void Motor::setSamplingTime(double dT) {
	_deltaT = dT; // convert to minute for intergration
	//_posconvertfactor = MOTOR_DIAMETER* _deltaT/60;
	_posconvertfactor = MOTOR_DIAMETER/ENCODER_RESOLUTION * M_PI /  GEAR_RATIO;
}

void Motor::disable() {

	_calibrating = true;
	if (COMEDI_ERROR == writeDIO(_subDevDIN,_enableChannel,false))
		cout << "ERROR disable";

	_setCurent = 0.0;
	// convert to DAC & send to controller
	convertCurrent2Volt();
	_setDACVolt = 0;
	if (COMEDI_ERROR == writeData(_subDevDAC,_setValueChanel,_rangeIDX,_aref,_setDACVolt)){
		printf("Error writing to subdev %d at channel %d value %2.2f \n",_subDevDAC,_setValueChanel,_setSpeedVolt);
	}
}



void Motor::convertCurrent2Volt(){
	_setDACVolt = 10 * _setCurent;
}

void Motor::setPosition(double pos) {

	_desPosition = pos;

	// Saturation
	if (_desPosition < -MAX_POS)
		_desPosition = -MAX_POS;
	else if (_desPosition > MAX_POS)
		_desPosition = MAX_POS;
}


void Motor::setCurrent(double current){
	_setCurent = current;
}

void Motor::enable() {
	if (COMEDI_ERROR == writeDIO(_subDevDIN,_enableChannel,true))
		cout << "ERROR enable";
}

double Motor::getActualPosition() {
	//		return _actPosition;
	return _theta;
}

double Motor::getActualTransPosition() {
	return _actPositionTrans;
}

double Motor::getDesignPositon() {

	return _desPosition;
}

void Motor::reset() {
	_static_position = 0.0;
	_state = RESET;
}

double Motor::getActualSpeed() {
	return _actSpeed;
}

void Motor::convertTranslation2Rotation() {
	// x = 5097 y
}


void Motor::stop() {
	printf("pause!");
	_desSpeed = 0;
	convertSpeed2Volt();
	// Write to DAC
	if (COMEDI_ERROR == writeData(_subDevDAC,_setValueChanel,_rangeIDX,_aref,_setSpeedVolt)){
		printf("Error writing to subdev %d at channel %d value %2.2f \n",_subDevDAC,_setValueChanel,_setSpeedVolt);
	}
}


void Motor::restartMotor(){
	_state = NORMAL;
}

double Motor::getDesignCurrent() {
	return _setCurent;
}

double Motor::getStaticPosition() {
	return _static_position;
}


/* Simulating user */

int Motor::updatePositionFromEscon(){

	// Read value from ADC
	if (COMEDI_ERROR == readData(_subDevADC,_actSpeedChannel,_rangeIDX,_aref)){
		return COMEDI_ERROR;
	} else {
		_actSpeedVolt = getVolAnalogInput(_actSpeedChannel);
	}
	// Convert voltage to Speed (RPM)
	convertVolt2Speed();

	// Integration of speed
	_userActPosition += _actSpeed * _posconvertfactor;


	return COMEDI_OK;

}

void Motor::runUserMotor(){
	if (_calibrating){
		disable();
		return;
	}

	enable();

	//updatePositionFromEscon();
	updateActualPositon();
	_actPosition = _actPosition - _bias_pos;
	// convert to translation

	_userActPosition = ((double)_actPosition) * _posconvertfactor;
	/*
	 * PID
	 */

	if (_desPosition - _userActPosition > 0){
		_desSpeed = 0;
	}


	if (_desSpeed > MAX_USER_SPEED)
		_desSpeed = MAX_USER_SPEED;
	else if (_desSpeed < -MAX_USER_SPEED)
		_desSpeed = -MAX_USER_SPEED;
	//	printf("des act: %6d  %6.5f\n",_actPosition,_userActPosition);

	convertSpeed2Volt();
	//	disable();
	if (COMEDI_ERROR == writeData(_subDevDAC,_setValueChanel,_rangeIDX,_aref,_setSpeedVolt)){
		//				printf("Error writing to subdev %d at channel %d value %2.2f \n",_subDevDAC,_setValueChanel,_setSpeedVolt);
	}
}

void Motor::setSpeed(double setSpeed){
	_desSpeed = setSpeed * (60*GEAR_RATIO) / (MOTOR_DIAMETER * M_PI) ;

}

void Motor::setUserPosition(double pos){
	_desPosition = pos;
}

void Motor::convertSpeed2Volt() {
	// Voltage Range : -10 -> 10 V
	// Speed Range : -8000 -> 8000 RPM
	_setSpeedVolt = -_desSpeed / 800;
}

double Motor::getUserPosition(){
	return _userActPosition;
}
void Motor::calBias(){
	if (COMEDI_OK == readCounter(_subDevCounter,_counterChannel))
		_actPosition = getCounter(_counterChannel);

	_bias_pos = _actPosition;
	_calibrating=  false;
	printf("bias position %5.5f \n",_bias_pos);
}
void Motor::userCalibrate(){
	_bias_pos += ((double)_actPosition) * _posconvertfactor;
}

void Motor::convertRotation2Translation() {
	// y = 0.0002 x
	double theta,X,deltaX,r;
	if (_actPosition < EPSILON_ROTATION)
		_actPositionTrans = 0;
	else{
		theta = abs((double) _actPosition / ENCODER_RESOLUTION) * 2 * M_PI;
		X = sqrt(pow(L0,2) - pow(theta,2) * pow(r0,2));
		if (abs(X) >= 0.001)
			r = r0 * sqrt(L0/X);
		else
			r = r0;

		_actPositionTrans = L0 - sqrt(pow(L0,2) - pow(theta,2) * pow(r,2));
	}
}



/* Crappy functions */
//
//Motor::Motor(comedi_t* dev, int subDevDAC,int subDevADC,int subDevDIN, int measureChannel,int controlChannel, int enableMotor, int range):DaqDevice(dev) {
//	_subDevDAC = subDevDAC;
//	_subDevADC = subDevADC;
//	_subDevDIN = subDevDIN;
//	_actSpeedChannel = measureChannel;
//	_setValueChanel = controlChannel;
//	_enableChannel = enableMotor;
//	_rangeIDX = range;
//
//	comedi_dio_config(dev,_subDevDIN,_enableChannel,COMEDI_OUTPUT);
//}
//
//void Motor::loadScheduleGainParameters(string filename) {
//	ifstream ifs(filename.c_str());
//	string line;
//	stringstream split;
//	double val;
//	if (ifs) {
//		getline(ifs,line);
//		while (!line.empty()){
//			split.clear();
//			split.str(line);
//			split << line;
//
//			split >> val;
//			_SG_Kp.push_back(val);
//
//			split >> val;
//			_SG_Ki.push_back(val);
//
//			split >> val;
//			_SG_Kd.push_back(val);
//
//			split >> val;
//			_SG_range.push_back(val);
//
//			getline(ifs,line);
//		}
//	}
//
//}
//
//
//void Motor::convertRotation2Translation() {
//	// y = 0.0002 x
//#if !TSA_MODEL
//	if (abs(_actPosition) < EPSILON_ROTATION)
//		_actPositionTrans = 0;
//	else
//		_actPositionTrans = A * _actPosition;
//#else
//	double theta,X,deltaX,r;
//	if (_actPosition < EPSILON_ROTATION)
//		_actPositionTrans = 0;
//	else{
//		theta = abs((double) _actPosition / ENCODER_RESOLUTION) * 2 * M_PI;
//		X = sqrt(pow(L0,2) - pow(theta,2) * pow(r0,2));
//		if (abs(X) >= 0.001)
//			r = r0 * sqrt(L0/X);
//		else
//			r = r0;
//
//		_actPositionTrans = L0 - sqrt(pow(L0,2) - pow(theta,2) * pow(r,2));
//	}
//
//#endif
//	if (_actPositionTrans > _static_position)
//		_static_position = _actPositionTrans;
//}
//
//void Motor::rotateClockwise() {
//	printf("rotating clockwise!");
//	_desSpeed = LOW_SPEED;
//	convertSpeed2Volt();
//	// Write to DAC
//	if (COMEDI_ERROR == writeData(_subDevDAC,_setValueChanel,_rangeIDX,_aref,_setSpeedVolt)){
//		printf("Error writing to subdev %d at channel %d value %2.2f \n",_subDevDAC,_setValueChanel,_setSpeedVolt);
//	}
//}
//
//void Motor::rotateCounterClockwise() {
//	printf("rotating counterclockwise!");
//	_desSpeed = -LOW_SPEED;
//	convertSpeed2Volt();
//	// Write to DAC
//	if (COMEDI_ERROR == writeData(_subDevDAC,_setValueChanel,_rangeIDX,_aref,_setSpeedVolt)){
//		printf("Error writing to subdev %d at channel %d value %2.2f \n",_subDevDAC,_setValueChanel,_setSpeedVolt);
//	}
//}
//
//
//void Motor::updatePidParameters(double val) {
//
//	// find position
//	int i;
//	for (i = 0; i < _SG_range.size();i++){
//		if (abs(val) <= _SG_range[i])
//			break;
//	}
//
//	// Set PID
//	if (i >= _SG_range.size())
//		i = _SG_range.size() - 1;
//
//	_Kp = _SG_Kp[i];
//	_Ki = _SG_Ki[i];
//	_Kd = _SG_Kd[i];
//
//	printf("Kp :%4.4f,Ki :%4.4f,Kd :%4.4f \n",_Kp,_Ki,_Kd);
//}
//
//void Motor::getPidParameters(double& Kp, double& Ki, double& Kd) {
//	Kp = _Kp;
//	Ki = _Ki;
//	Kd = _Kd;
//}
//
//void Motor::setState(MOTOR_STATE state) {
//	if (RESET != _state)
//		_state = state;
//
//	//	if (_state == REALEASE)
//	//		_static_position = _actPositionTrans;
//}
