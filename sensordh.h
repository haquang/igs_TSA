#ifndef SensorDH_H
#define SensorDH_H

#include <comedilib.h>
#include <vector>
#include <ctype.h>
#include "defines.h"
#include <iostream>
#include <string.h>

using namespace std;


class SensorDH
{
public:
	SensorDH();
	SensorDH(comedi_t* dev,const int subdev, unsigned short firstChannel, int reference);

	unsigned short SensorDH::numOfChannels();
	lsampl_t SensorDH::rawValue(unsigned short channel);
	int readData(int channel);
	float getVolAnalogInput(int channel);

	void SensorDH::setNumOfChannels(unsigned short num);
	int DAQcalibration();
	virtual unsigned short getDAQFirstChannel() const { return m_DAQfirstChannelIndex; };
	virtual lsampl_t getDAQMaxData() const { return m_DAQmaxdata; };
	virtual comedi_range* getDAQRangeStruct() const { return m_DAQrangeStruct; };
	virtual int getDAQReference() const { return m_DAQreference; };
	virtual void setCalibration(lsampl_t *calValue) {
		for (unsigned short c = 0; c < m_numOfChannels; ++c) m_calValue[c] = calValue[c];
		m_calibrate = true;
	};

	virtual void readRawValue() = 0;
	virtual void setRawValue(lsampl_t *rawValue) = 0;
	virtual void calibrate() = 0;
	virtual void close() = 0;

	~SensorDH();
protected:
	// variables to store raw and calibration related data
	unsigned short m_numOfChannels;
	lsampl_t *m_rawValue;
	lsampl_t *m_calValue;
	bool m_calibrate;
	comedi_polynomial_t* _converter_an_input;
	comedi_polynomial_t* _converter_an_output;
	lsampl_t* m_sample_torque;
	// data about the DAQ board that this device is connected to
	comedi_t *_dev;
	int _subdev;
	unsigned short m_DAQfirstChannelIndex;
	lsampl_t m_DAQmaxdata;
	comedi_range* m_DAQrangeStruct;
	int m_DAQreference;
};

#endif // SensorDH_H
