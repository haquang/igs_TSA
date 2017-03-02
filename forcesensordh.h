/*
 * forcesensordh.h
 *
 *  Created on: Oct 24, 2016
 *      Author: haquang
 */

#ifndef FORCESENSORDH_H_
#define FORCESENSORDH_H_

#include <comedilib.h>

#include <vector>
#include "ATItools/ftconfig.h"
#include "sensordh.h"

using namespace std;

class ForceSensorDH : public SensorDH {

	float *m_volts;
	float *m_meanVolts;
	float *m_forceTorque;

	vector<double>  m_force;
	vector<double>  m_torque;

	float m_sampleFreq;

	Calibration *m_calibration;
	unsigned long m_calibrationCounter;
	unsigned long m_calibrationLength;

public:
	ForceSensorDH();

	ForceSensorDH(comedi_t* dev,const int subdev, unsigned short firstChannel, int reference);

	virtual ~ForceSensorDH();

	void setNumOfChannels(unsigned short num);
	void calibrationRequired() { m_calibrate = true; };
	bool isCalibrateRequired();
	float sample2Volts(const lsampl_t sample) const;
	const vector<double>  getForce() const { return m_force; };
	const vector<double>  getTorque() const { return m_torque; };
	void setSampleFrequency(float f);

	float getForceZ();
	int getCalibrationLength(){
		return m_calibrationLength;
	}
	void readRawValue();
	void setRawValue(lsampl_t *rawValue);
	void calibrate();
	void close() {};
};

#endif /* FORCESENSORDH_H_ */
