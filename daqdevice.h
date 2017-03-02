#ifndef DaqDevice_H
#define DaqDevice_H

#include <comedilib.h>
#include <vector>
#include <ctype.h>
#include "defines.h"


class DaqDevice
{
public:
    DaqDevice();
    DaqDevice(comedi_t* dev);
    void setDevice(comedi_t dev);
    int readData(int subdev,int channel,int range_idx,int aref);
    lsampl_t getRawAnalogInput(int channel);
    float getVolAnalogInput(int channel);
    int DAQcalibration(int subdev,int channel,int range_idx);
    int writeData(int subdev,int channel, int range_idx,int aref,double value);
    int setupCounter(int subdev,int channel,int initial_count);
    int readCounter(int subdev,int channel);
    int resetCounter(int subdev,int channel);

    lsampl_t getCounter(int channel);
    int writeDIO(int subdev,int channel,unsigned int bit);
private:
  comedi_t *_dev;
  comedi_polynomial_t* _converter_an_input;
  comedi_polynomial_t* _converter_an_output;
  lsampl_t* _an_input;
  lsampl_t _counter;
};

#endif // DaqDevice_H
