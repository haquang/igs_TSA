/*
 * FSRSensor.h
 *
 *  Created on: Aug 17, 2016
 *      Author: haquang
 */

#ifndef ADMITTANCECONTROLLER_H_
#define ADMITTANCECONTROLLER_H_


#include <cmath>
#include "defines.h"
#include "daqdevice.h"
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <list>

using namespace std;

class AdmittanceController : public DaqDevice {

	double _ext_force = 0.0f;	// External Force
	double _err_force;
	double _prv_err_force;
	// Virtual Mass Parameters
	double _vm_position = 0.0f;	// Position
	double _vm_velocity = 0.0f;	// Velocity
	double _vm_acceleration = 0.0f;// Acceleration
	double _prv_vm_position = 0.0f;
	double _deltaT = 0.0f;
	double _cmd_current = 0.0f;
	bool _puncture = false;
	// Virtual Mass-Spring-Damper
	double _mass = 0.0f;
	double _spring = 0.0f;
	double _damper = 0.0f;
	double _integral = 0.0f;
	double _force_err_integral = 0.0f;
	// Perceive Stiffness
	double _m_stiffness = 0;
	double _stiffness;
	double _high_stiffness = 0.8;
	double _viscosity;
	double _des_force;
	double _deltaX;

	// Position
	double _proxy = 0;
	double _proxy_mass = 10.0;
	double _proxy_vel = 0;
	double _proxy_pos = COMPRESSION_SPRING_LENGTH;
	double _err_stiffness,_prv_err_stiffness;
	double _collide_position = 0.0f;
	// Actual Position
	double _position = 0.0f;
	double _prv_position = 0.0f;
	double _velocity = 0.0f;

	list<double> _velocity_filter;
	// for Sensoray/NI card
	int _subDevADC;
	int _rangeIDX = 0;
	int _forceChannelRight;
	int _forceChannelLeft;
	int _aref = AREF_GROUND;

	vector<double> _x_vect;
	vector<double> _stiffness_vect;					// GAIN-Scheduled proportional parameter
	vector<double> _mass_vect;
	vector<double> _damper_vect;

	bool _notify = false;
public:
	AdmittanceController();
	AdmittanceController(comedi_t* dev,int subDevADC,int forceChannelRight,int forceChannelLeft,int range);

	void solve();

	void setMassSpringDamperModel(double m,double k,double b,double i);
	void setPerceiveStiffness(double stiffness,double damper);
	void updatePerceiveStiffness();
	void setCurPosition(double pos);
	void updateProxyPosition();
	double getProxyPosition();
	void setSamplingTime(double dT);
	void setProxyMass(double mass);

	double reset();
	void run();
	void run(float extForce);
	double getVirtualMassPosition();
	double getCmdCurrent();
	double getForce();
	double getDesignForce();
	double getCurBound();
	double getPerceivedStiffness();

	double getStiffness();
	void forceControl();
	void setForce(double);
	void setExtForce(double f);
	void stiffnessControl();

	void loadStiffnessRange(string filename);
	void setCollidePos(double pos);

	double getDisplacement(){
		return _deltaX;
	}

	void setSpeed(double _velocity);

	// Scenario
	void runPalpation(float extForce);
	void runSuture(float extForce);
	void runDrill(float extForce);
	void runStep(float extForce);

	virtual ~AdmittanceController();
private:
	void readForce();
};

#endif /* ADMITTANCECONTROLLER_H_ */
