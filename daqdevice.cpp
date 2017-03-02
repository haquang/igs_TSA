#include "daqdevice.h"

DaqDevice::DaqDevice()
{

}
DaqDevice::DaqDevice(comedi_t* dev): _dev(dev){
	_an_input = new lsampl_t[COMEDI_IN_CHAN_NUM];
	_converter_an_input  = new comedi_polynomial_t[COMEDI_IN_CHAN_NUM];
	_converter_an_output = new comedi_polynomial_t[COMEDI_OUT_CHAN_NUM];

}

int DaqDevice::readData(int subdev,int channel,int range_idx,int aref){
	if (comedi_data_read(_dev, subdev, channel, range_idx, aref, &_an_input[channel]) < 0){
		return COMEDI_ERROR;
	} else {
		return COMEDI_OK;
	}
}

int DaqDevice::DAQcalibration(int subdev,int channel,int range_idx)
{
	int retval;
	int flags;
	comedi_calibration_t* parsed_calibration;
	comedi_polynomial_t* converter;
	comedi_conversion_direction options;

	if (COMEDI_AN_IN_SUB == subdev)
	{
		converter = &_converter_an_input[channel];
		options = COMEDI_TO_PHYSICAL;
	}

	else if (COMEDI_AN_OUT_SUB == subdev)
	{
		converter = &_converter_an_output[channel];
		options = COMEDI_FROM_PHYSICAL;
	}
	else
		return COMEDI_ERROR;

	flags = comedi_get_subdevice_flags(_dev, subdev);
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

		retval = comedi_get_softcal_converter(subdev, channel, range_idx,
				options, parsed_calibration, converter);

		comedi_cleanup_calibration(parsed_calibration);


		if(retval < 0)
		{
			return COMEDI_ERROR;
		}
	}else /* board uses hardware calibration */
	{
		retval = comedi_get_hardcal_converter(_dev, subdev, channel, range_idx,
				options, converter);
		if(retval < 0)
		{
			return COMEDI_ERROR;
		}
	}
	printf("Calibrate subdev %d at channel %d \n",subdev,channel);
	return COMEDI_OK;
}

lsampl_t DaqDevice::getRawAnalogInput(int channel)
{
	return(_an_input[channel]);
}

float DaqDevice::getVolAnalogInput(int channel)
{
	return  comedi_to_physical(_an_input[channel], &_converter_an_input[channel]);
}

int DaqDevice::writeData(int subdev,int channel, int range_idx,int aref,double value)
{
	lsampl_t raw;

	raw = comedi_from_physical(value,&_converter_an_output[channel]);
	//	printf("raw %d \n",raw);
	if (comedi_data_write(_dev, subdev, channel, range_idx, aref,raw) < 0)
	{
		return COMEDI_ERROR;
	} else {
		return COMEDI_OK;
	}
}

int DaqDevice::setupCounter(int subdev,int channel,int initial_count)
{
	int retval;
	lsampl_t counter_mode;

	int PFI_A;
	int PFI_B;
	int PFI_Z;

	if (subdev == 11){
		PFI_A = 8;
		PFI_B = 10;
		PFI_Z = 9;

	} else {
		PFI_A = 3;
		PFI_B = 11;
		PFI_Z = 4;
	}

	retval = comedi_reset(_dev, subdev);
	if(retval < 0) return retval;

	if (comedi_set_gate_source(_dev, subdev, channel, 0, NI_GPCT_DISABLED_GATE_SELECT) ||
			comedi_set_gate_source(_dev, subdev, channel, 1, NI_GPCT_DISABLED_GATE_SELECT))
		return COMEDI_ERROR;

	retval = comedi_set_other_source(_dev, subdev, 0, NI_GPCT_SOURCE_ENCODER_A, NI_GPCT_PFI_OTHER_SELECT(PFI_A));
	if (retval < 0)
		return retval;

	retval = comedi_set_other_source(_dev, subdev, 0, NI_GPCT_SOURCE_ENCODER_B, NI_GPCT_PFI_OTHER_SELECT(PFI_B));
	if (retval < 0)
		return retval;

	// Set counter mode
	counter_mode = (NI_GPCT_COUNTING_MODE_QUADRATURE_X1_BITS | NI_GPCT_COUNTING_DIRECTION_HW_UP_DOWN_BITS);

	retval = comedi_set_counter_mode(_dev, subdev, channel, counter_mode);
	if(retval < 0) return retval;

	/* set initial counter value by writing to channel 0.  The "load a" and "load b" registers can be
			set by writing to channels 1 and 2 respectively. */
	retval = comedi_data_write(_dev, subdev, channel, 0, 0, initial_count);

	retval = comedi_arm(_dev, subdev, NI_GPCT_ARM_IMMEDIATE);
	if(retval < 0) return retval;

	printf("Setup counter finished successfully\n");
	return 0;
}

int DaqDevice::readCounter(int subdev,int channel)
{
	//	printf("subchannel %d \n",subdev);
	if (comedi_data_read(_dev, subdev, channel, 0, 0, &_counter) < 0){
		return COMEDI_ERROR;
	} else {
		//		printf("subchannel %d %d \n",subdev,_counter);
		return COMEDI_OK;
	}
}

lsampl_t DaqDevice::getCounter(int channel)
{
	return(_counter);
}

int DaqDevice::resetCounter(int subdev,int channel)
{

	if (comedi_data_write(_dev, subdev, channel, 0, 0,0) < 0){
		return COMEDI_ERROR;
	} else {
		return COMEDI_OK;
	}
}


int DaqDevice::writeDIO(int subdev, int channel, unsigned int bit) {

	if (comedi_dio_write(_dev, subdev, channel, bit) < 0)
	{
		return COMEDI_ERROR;
	} else {
		return COMEDI_OK;
	}
}
