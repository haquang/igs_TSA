/*
 * Motor.h
 *
 *  Created on: Aug 12, 2016
 *      Author: haquang
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "defines.h"
#include "daqdevice.h"
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>

using namespace std;

enum MOTOR_STATE {
	NORMAL,
	REALEASE,
	RESET};

class Motor:public DaqDevice {

	double _desSpeed = 0.0f;  		// Design Speed
	double _actSpeed = 0.0f;		// Actual Speed
	double _desPosition = 0;		// Design position
	int _actPosition;	// Actual positon
	double _userActPosition = 0.0f;
	double _theta = 0.0f;

	double _desPositionTrans;
	double _actPositionTrans;
	double _actSpeedTran;
	double _static_position = 0; //

	double _setSpeedVolt;
	double _actSpeedVolt = 0.0f;
	double _setCurent = 0.0f;
	double _setDACVolt = 0.0f;


	double _err = 0;
	double _prv_error;
	double _integral;
	double _derivative;
	double _deltaT;
	double _posconvertfactor = 0.0f;
	double _setUserSpeed;
	// Controllers
	double _Kp=0.0f,_Ki=0.0f,_Kd = 0.0f;	// PID parameters
	vector<double> _SG_range;
	vector<double> _SG_Kp;					// GAIN-Scheduled proportional parameter
	vector<double> _SG_Ki;					// GAIN-Scheduled integral parameter
	vector<double> _SG_Kd;					// GAIN-Scheduled derivative parameter

	// for sensoray/ni card
	int _subDevADC;
	int _subDevDAC;
	int _subDevDIN;
	int _subDevCounter;
	int _rangeIDX = 0;
	int _actSpeedChannel;
	int _setValueChanel;
	int _enableChannel;
	int _counterChannel;
//	int _directionChannel;

	int _aref = AREF_GROUND;
	bool _calibrating = true;
	int _calibration_count = 0;
	int _bias_pos;
	MOTOR_STATE _state = NORMAL;

	bool _start = false;
public:
	Motor();
#if SPEED_CONTROL
	Motor(comedi_t* dev,int subDevDAC,int subDevADC,int subDevDIN,int subDevCounter,int measureChannel,int controlChannel,int enableMotor,int range);
#else
	Motor(comedi_t* dev,int subDevDAC,int subDevADC,int subDevDIN,int subDevCounter,int measureChannel,int controlChannel,int enableMotor,int counterChannel,int range);

#endif

	void setPidParameters(double Kp,double Ki, double Kd);
//	void updatePidParameters(double val);
//	void getPidParameters(double &Kp,double &Ki,double &Kd);

	void setPosition(double pos);
	void setSamplingTime(double dT);
	void setCurrent(double current);
	void run();

//	void rotateClockwise();
//	void rotateCounterClockwise();
	void stop();

	void enable();
	void reset();
	void disable();
	virtual ~Motor();
	void restartMotor();
	double getActualPosition();
	double getActualTransPosition();

	double getActualSpeed();
	double getDesignPositon();
	double getStaticPosition();

	double getDesignCurrent();
	void setCalibrationMode();
	void resetCalibrationMode();
	bool isCalibrationMode();
//	void loadScheduleGainParameters(string filename);

//	void setState(MOTOR_STATE state);

	// Function for user simulate
	void runUserMotor();
	void setSpeed(double setSpeed);
	int updatePositionFromEscon();
	void setUserPosition(double pos);
	double getUserPosition();
	void userCalibrate();
	void calBias();

private:
	int updateActualPositon();
	void convertVolt2Speed();
	void convertSpeed2Volt();
	void convertCurrent2Volt();
	void convertTranslation2Rotation();
	void convertRotation2Translation();

};

#endif /* MOTOR_H_ */
