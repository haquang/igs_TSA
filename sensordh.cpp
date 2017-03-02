#include "sensordh.h"

SensorDH::SensorDH(){
	m_numOfChannels = 1;
	m_rawValue = new lsampl_t[m_numOfChannels];
	m_calValue = new lsampl_t[m_numOfChannels];
	m_calibrate = false;

	m_DAQfirstChannelIndex = 0;
	m_DAQmaxdata = 0;
	m_DAQreference = AREF_COMMON;
}

SensorDH::SensorDH(comedi_t* dev,const int subdev, unsigned short firstChannel, int reference) {
	m_numOfChannels = 1;
	m_rawValue = new lsampl_t[m_numOfChannels];
	m_calValue = new lsampl_t[m_numOfChannels];
	m_calibrate = false;
	m_DAQmaxdata = 0;

	_dev = dev;
	_subdev = subdev;
	m_DAQfirstChannelIndex = firstChannel;
	m_DAQreference = reference;
//	m_DAQmaxdata = comedi_get_maxdata(_dev, _subdev, m_DAQfirstChannelIndex);
}

unsigned short SensorDH::numOfChannels() {
	return m_numOfChannels;
}

void SensorDH::setNumOfChannels(unsigned short num) {
	m_numOfChannels = num;
	delete [] m_rawValue;
	delete [] m_calValue;
	m_rawValue = new lsampl_t[m_numOfChannels];
	m_calValue = new lsampl_t[m_numOfChannels];
	memset(m_rawValue, 0, m_numOfChannels*sizeof(lsampl_t));
	memset(m_calValue, 0, m_numOfChannels*sizeof(lsampl_t));
	m_sample_torque = new lsampl_t[m_numOfChannels];
}
int SensorDH::DAQcalibration()
{
	int retval;
	int flags;
	comedi_calibration_t* parsed_calibration;
	comedi_polynomial_t* converter;
	comedi_conversion_direction options;

	_converter_an_input  = new comedi_polynomial_t[m_numOfChannels];
	_converter_an_output = new comedi_polynomial_t[m_numOfChannels];
	for (int channel = 0; channel < m_numOfChannels;channel++){
		if (COMEDI_AN_IN_SUB == _subdev)
		{
			converter = &_converter_an_input[channel];
			options = COMEDI_TO_PHYSICAL;
		}

		else if (COMEDI_AN_OUT_SUB == _subdev)
		{
			converter = &_converter_an_output[channel];
			options = COMEDI_FROM_PHYSICAL;
		}
		else{

			return COMEDI_ERROR;
		}


		flags = comedi_get_subdevice_flags(_dev, _subdev);
		if(flags < 0)
		{
			return COMEDI_ERROR;
		}
		if(flags & SDF_SOFT_CALIBRATED) /* board uses software calibration */
		{
			char *calibration_file_path = comedi_get_default_calibration_path(_dev);

			/* parse a calibration file which was produced by the
		                comedi_soft_calibrate program */
			parsed_calibration = comedi_parse_calibration_file(calibration_file_path);
			if(parsed_calibration == NULL)
			{
				return COMEDI_ERROR;
			}

			/* get the comedi_polynomial_t for the subdevice/channel/range
		                we are interested in */

			retval = comedi_get_softcal_converter(_subdev, channel + m_DAQfirstChannelIndex, 0,
					options, parsed_calibration, converter);

			comedi_cleanup_calibration(parsed_calibration);


			if(retval < 0)
			{
				return COMEDI_ERROR;
			}
		}else /* board uses hardware calibration */
		{
			retval = comedi_get_hardcal_converter(_dev, _subdev, channel  + m_DAQfirstChannelIndex, 0,
					options, converter);
			if(retval < 0)
			{
				return COMEDI_ERROR;
			}
		}
		printf("Calibrate subdev %d at channel %d \n",_subdev,channel + m_DAQfirstChannelIndex);
	}

	return COMEDI_OK;
}


lsampl_t SensorDH::rawValue(unsigned short channel) {
	if (channel < m_numOfChannels)
		return m_rawValue[channel];
	else {
		cout << "SensorDH::rawValue: Used channel label (" << channel << ") is invalid. Max channel label is " << m_numOfChannels-1 << endl;
		return 0;
	}
};

int SensorDH::readData(int channel){
	if (comedi_data_read(_dev, _subdev, channel + m_DAQfirstChannelIndex, 0, AREF_DIFF, &m_sample_torque[channel]) < 0){
		return COMEDI_ERROR;
	} else {
		return COMEDI_OK;
	}
}

float SensorDH::getVolAnalogInput(int channel)
{
	return comedi_to_physical(m_sample_torque[channel], &_converter_an_input[channel]);
}

SensorDH::~SensorDH(){

}
