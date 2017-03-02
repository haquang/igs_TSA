/*
 * defines.h
 *
 *  Created on: Aug 12, 2016
 *      Author: haquang
 */

#define COMEDI_DEFAULT_DEVIVE "/dev/comedi0"   // Device ID for sensoray
#define COMEDI_ERROR -1
#define COMEDI_OK 0
#define COMEDI_RANGE 0
#define COMEDI_IN_CHAN_NUM 16
#define COMEDI_OUT_CHAN_NUM 4
#define COMEDI_COUNT_CHAN_NUM 2
// Sensoray Sub-channel
#define COMEDI_AN_IN_SUB 0
#define COMEDI_AN_OUT_SUB 1
#define COMEDI_DI_SUB1 2
#define COMEDI_DI_SUB2 3
#define COMEDI_DI_SUB3 4

#define COMEDI_COUNTER_SUB 11
#define COMEDI_COUNTER_SUB2 12

// Channel
#define COMEDI_DAC_OUT_0 0
#define COMEDI_DAC_OUT_1 1
#define COMEDI_DIO_OUT_1 0
#define COMEDI_DIO_OUT_2 1

#define COMEDI_ENC 1
#define COMEDI_ADC_IN_0 0
#define COMEDI_ADC_IN_1 1
#define COMEDI_ADC_IN_2 2
#define COMEDI_ADC_IN_3 3

#define COMEDI_ADC_IN_7 7


#define COMEDI_COUNT_IN_0 0
#define COMEDI_COUNT_IN_1 0

// Schedule
#define POSITION_CONTROL_PERIOD 100000 // ns


// Motor Controller
#define ROTATION 0
#define SPEED_CONTROL 0
#define CURRENT_CONTROL 0

#define SPEED_RANGE 1000 // rpm
#define INPUT_ADC_SPEED_RANGE 4 // 0 - 4V
#define OUTPUT_DAC_SPEED_RANGE 10 // 0 - 10V
#define ENCODER_RESOLUTION 1000
#define MAX_CURRENT 0.5f
#define MAX_SPEED 3000

#define EPSILON_ROTATION 2

#define MOTOR_CCW true		// Counter Clockwise Direction
#define MOTOR_CW false		// Clockwise direction
#define LOW_SPEED 100

#define KP_DEFAULT 0.0005
#define KI_DEFAULT 0.001

#define KD_DEFAULT 0.0125
#define CALIBRATING 0


// Admittance motion controller
#define SINGLE_SENSOR 1
#define SPRING_STIFFNESS 0.849
#define MUL_STIFFNESS 0
#define VIRTUAL_SPRING_LENGTH 0.1
#define PROXY 1
#define MAX_STIFFNESS 3

#define COMPRESSION_SPRING_LENGTH 35

#define VELOCITY_FILTER_SIZE 25

// Force sensor
#define ATI_CALIBRATION_FILE "./ATItools/FT11315.cal"
#define FIRST_INDEX_FORCE 16
#define EPSILON 0.1
#define FORCE_LEVEL 0.01
#define FORCE_DIFFER 0.005

#define FORCE_LIMIT 20

#define STIFFNESS_REGION_MARGIN 0.25


// Experiment Scenario

#define PALPATION 0
#define SUTURE 0
#define DRILL 1
#define STEP 0


#define FIRST_LAYER 3.0
#define BUFFER_ZONE 0.1
#define MID_LAYER FIRST_LAYER + BUFFER_ZONE

// Twisted String Actuator

#define TSA_MODEL 1

#define L0 35.45 // mm
#define r0 0.25  // mm
#define ENCODER_RESOLUTION 1024


// USER MOTOR
#define MOTOR_DIAMETER 12.19

#define KP 75.0
#define KD 1.0
#define GEAR_RATIO 4.8

#define MAX_USER_SPEED 60  // 30

// Force model

#define A 0.2741
#define B 4.6862
#define C -0.9294

#define ROBOT 1

//20170111
#define MAX_POS 120

#define THETA_CONTROL 1
