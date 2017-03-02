/*
 * forcesensordh.cpp
 *
 *  Created on: Oct 24, 2016
 *      Author: haquang
 */

#include "forcesensordh.h"

ForceSensorDH::ForceSensorDH() {
	// TODO Auto-generated constructor stub
	m_sampleFreq = 1000.0; // use 1kHz as default sample frequency
	m_calibrationCounter = 0;
	m_calibrationLength = (unsigned long) (2.0 * m_sampleFreq + 0.5); // wait 5 second to calibrate force sensor
	if (m_calibrationLength == 0) m_calibrationLength = 1;

	// load calibration file
	m_calibration = createCalibration(ATI_CALIBRATION_FILE, 1);
	if (m_calibration == NULL) cout << "ForceSensorDH::ForceSensorDH(): ERROR! No calibration file for force sensor." << endl;

	m_volts = NULL;
	m_meanVolts = NULL;
	m_forceTorque = NULL;
	setNumOfChannels(6);
}

ForceSensorDH::ForceSensorDH(comedi_t* dev,const int subdev, unsigned short firstChannel, int reference):SensorDH(dev,subdev,firstChannel,reference){

	m_sampleFreq = 1000.0; // use 1kHz as default sample frequency
	m_calibrationCounter = 0;
	m_calibrationLength = (unsigned long) (2.0 * m_sampleFreq + 0.5); // wait 5 second to calibrate force sensor
	if (m_calibrationLength == 0) m_calibrationLength = 1;

	printf("Calibration length: %d \n",m_calibrationLength);
	// load calibration file
	m_calibration = createCalibration(ATI_CALIBRATION_FILE, 1);
	if (m_calibration == NULL)
		cout << "ForceSensorDH::ForceSensorDH(): ERROR! No calibration file for force sensor." << endl;
	else
		cout << "ForceSensorDH::ForceSensorDH(): Calibration done." << endl;

	m_volts = NULL;
	m_meanVolts = NULL;
	m_forceTorque = NULL;
	setNumOfChannels(6);

//	m_DAQrangeStruct = comedi_get_range(_dev,_subdev, m_DAQfirstChannelIndex , 0 /*range +-10V*/);
//
//	printf("DAQ Range: %4.4f %4.4f \n " , m_DAQrangeStruct->max,m_DAQrangeStruct->min);
}


ForceSensorDH::~ForceSensorDH() {
	// TODO Auto-generated destructor stub
	destroyCalibration(m_calibration);

	delete [] m_volts;
	delete [] m_meanVolts;
	delete [] m_forceTorque;
}

float ForceSensorDH::sample2Volts(const lsampl_t sample) const {
	return (float)m_DAQrangeStruct->min/1e6 + (float)(m_DAQrangeStruct->max-m_DAQrangeStruct->min)/1e6 * (float)sample/(float)m_DAQmaxdata;
}

void ForceSensorDH::setNumOfChannels(unsigned short num){
	SensorDH::setNumOfChannels(num);

	delete [] m_volts;
	delete [] m_meanVolts;
	delete [] m_forceTorque;
	m_volts = new float[m_numOfChannels];
	m_meanVolts = new float[m_numOfChannels];
	m_forceTorque = new float[m_numOfChannels];
	memset(m_volts, 0, m_numOfChannels*sizeof(float));
	memset(m_meanVolts, 0, m_numOfChannels*sizeof(float));
	memset(m_forceTorque, 0, m_numOfChannels*sizeof(float));

	m_force.resize(m_numOfChannels, 0);
	m_torque.resize(m_numOfChannels, 0);
}

void ForceSensorDH::setSampleFrequency(float f){
	m_sampleFreq = f;
	m_calibrationLength = (unsigned long) (5.0 * m_sampleFreq + 0.5); // wait 5 second to calibrate force sensor
	if (m_calibrationLength == 0) m_calibrationLength = 1;
}

void ForceSensorDH::setRawValue(lsampl_t* rawValue) {
	//	lsampl_t rawReading;
	//	for (unsigned short c = 0; c < m_numOfChannels; ++c) {
	//		comedi_data_read(_dev, _subdev, c, 0, AREF_DIFF, &rawReading);
	//		m_rawValue[c] = rawReading;///rawValue[c];
	//		m_volts[c] = sample2Volts(m_rawValue[c]);
	//	}
	//
	//	// convert voltages to force/torque pair
	//	if (m_calibration != NULL)
	//		ConvertToFT(m_calibration, m_volts, m_forceTorque);
	//	else {
	//		for (unsigned short c = 0; c < m_numOfChannels; ++c)
	//			m_forceTorque[c] = m_volts[c];
	//	}
	//	m_force[0] = m_forceTorque[0];
	//	m_force[1] = m_forceTorque[1];
	//	m_force[2] = m_forceTorque[2];
	//	m_torque[0] = m_forceTorque[3];
	//	m_torque[1] = m_forceTorque[4];
	//	m_torque[2] = m_forceTorque[5];
}

void ForceSensorDH::readRawValue(){
	// read voltages at ANALOG IN by single shot
	// (only use this if this ANALOG IN device is NOT used as the trigger device)

	for (unsigned short c = 0; c < m_numOfChannels; ++c){
		if (COMEDI_ERROR == readData(c)){
			printf("Error reading channel %d \n" , c);
		} else {
			m_volts[c] = getVolAnalogInput(c);
//			printf("Channel %d volt %4.4f ",c,m_volts[c]);
		}
	}
	if (m_calibrate)
		return;
	// convert voltages to force/torque pair
	if (m_calibration != NULL)
		ConvertToFT(m_calibration, m_volts, m_forceTorque);
	else {
		for (unsigned short c = 0; c < m_numOfChannels; ++c)
			m_forceTorque[c] = m_volts[c];
	}
	m_force[0] = m_forceTorque[0];
	m_force[1] = m_forceTorque[1];
	m_force[2] = m_forceTorque[2];
//	m_torque[0] = m_forceTorque[3];
//	m_torque[1] = m_forceTorque[4];
//	m_torque[2] = m_forceTorque[5];

//	printf("Raw: %4.4f %4.4f %4.4f \n",m_forceTorque[0],m_forceTorque[1],m_forceTorque[2]);
}

bool ForceSensorDH::isCalibrateRequired(){
	return m_calibrate;
}

void ForceSensorDH::calibrate(){
	if (m_calibrationCounter < m_calibrationLength) {
		for (unsigned short c = 0; c < m_numOfChannels; ++c)
			m_meanVolts[c] += m_volts[c];

		++m_calibrationCounter;
	}
	else {
		for (unsigned long c = 0; c < m_numOfChannels; ++c){
			m_meanVolts[c] /= (float)m_calibrationLength;
			cout << "Mean volt at " << c << " is " << m_meanVolts[c] << endl;
		}
		if (m_calibration != NULL)
			Bias(m_calibration, m_meanVolts);

		m_calibrationCounter = 0;
		memset(m_meanVolts, 0, m_numOfChannels*sizeof(float));

		m_calibrate = false;

		cout << "ForceSensorDH::calibrate(): Force sensor has been calibrated."  << endl;
	}

}

float ForceSensorDH::getForceZ() {
	return m_force[2];
}
