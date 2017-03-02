//============================================================================
// Name        : TSAControl.cpp
// Author      : Haquang
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

// General
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
// RTAI
#include <rtai_posix.h>
#include <rtai_lxrt.h>
#include <comedilib.h>
// Boost
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>

// Local
#include "defines.h"
#include "motor.h"
#include "AdmittanceController.h"
#include "forcesensordh.h"

boost::asio::io_service t_cali_service;

using namespace std;
int cal_count = 0;

/* RTAI */

pthread_t motor_control_thread;
RT_TASK *maintask;
static RTIME period_ns = POSITION_CONTROL_PERIOD;
RTIME motor_control_period; /* requested timer period, in counts */

/* Global Variable */
vector<int>* v_time;
double deltaT = (double) POSITION_CONTROL_PERIOD / 1000000000;
double T = 100.0; //second;
double t = 0.0f;
bool stop;
int exectime;
double Kp,Ki,Kd;
Motor* motor;
Motor* user_motor;

double position;
double set_position;
RTIME exec_start, exec_end;

bool start = false;
AdmittanceController* admController;

//double mass = 0.000001f;
//double spring = 0.0001f;
//double damper = 0.00002f;

double mass = 0.0f;
double spring = 0.0f;
double damper = 0.0f;
double integral = 0.0f;


double stiffness = 0.0f; // Perceived Stiffness
double viscosity = 0.0f;

double proxy_mass;
/* Force sensor */
ForceSensorDH* forceSensor;
float samplingfrequency = 1000.0f;

bool collided = false;
/* Data Acquisition*/
vector<double> desPos;
vector<double> disp;
vector<double> extForce;
vector<double> desForce;
vector<double> tsaPos;

vector<double> v_Kp;
vector<double> v_Ki;
vector<double> v_Kd;
bool _notify = false;

fstream f_data;

/* Functions*/

void *motor_control_task(void *arg)
{
	int retval;
	RT_TASK *rt_task;

	/*  make this thread LXRT soft realtime */
	rt_task = rt_task_init_schmod(nam2num("MotorControl"), 1, 0, 0, SCHED_FIFO, 0xF);
	printf("THREAD INIT:name = %lu, address = %p.\n", rt_task, "MotorControl");
	mlockall(MCL_CURRENT | MCL_FUTURE);

	// makes task hard real time (default: soft)
	// uncomment the next line when developing : develop in soft real time mode
	rt_make_hard_real_time();

	motor_control_period = nano2count(period_ns);
	/* make task periodic, starting one cycle from now */
	retval = rt_task_make_periodic(rt_task,
			rt_get_time() + motor_control_period, motor_control_period);
	if (0 != retval) {
		if (-EINVAL == retval) {
			/* task structure is already in use */
			rt_printk("periodic task: task structure is invalid\n");
		} else {
			/* unknown error */
			rt_printk("periodic task: error starting task\n");
		}
		return 0;
	}
	exectime = 0.0f;
	exec_start = rt_get_cpu_time_ns();
	while (!stop){
		t+= deltaT;
		// Reading force from sensor
		forceSensor->readRawValue();
		double ext_force = forceSensor->getForceZ();
		if (abs(ext_force) >= FORCE_LIMIT){
			printf("WARNING! FORCE EXCEED LIMIT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
			start = false;
		}

		// PALPATION
		//#if PALPATION
		//		admController->runPalpation(ext_force);
		//#endif
		//
		//#if DRILL
		//		admController->runDrill(ext_force);
		//#endif
		//
		//#if STEP
		//		admController->runStep(ext_force);
		//#endif

		//motor->setPosition(admController->getVirtualMassPosition());

		double pos;
		double force;
		//		double  force = -10;
		if (start){

			// User
			//	double user_pos = -abs(10 * sin(2*M_PI * 0.2 *t));
			double user_pos = -12;
			user_motor->setUserPosition(user_pos);
			user_motor->setSpeed(-4);
			admController->setSpeed(4);
			user_motor->runUserMotor();

			/*Fixed Stiffness*/
#if ROBOT
			if (abs(ext_force) > EPSILON){
				if (!collided){
					collided = true;
					admController->setCollidePos(user_motor->getUserPosition());
				}
			} else {
				collided = false;
			}

			//			if (collided){
			admController->setCurPosition(user_motor->getUserPosition());
//			admController->runPalpation(ext_force);
			admController->runDrill(ext_force);
			//			} else {
			//				admController->setForce(0.0f);
			//			}
#else
			admController->setCurPosition(motor->getActualTransPosition());
			admController->runPalpation(ext_force);
#endif
			/* Force control */
			//			force = -abs(5 * sin(2*M_PI * 0.1 *t)); // (N)
			//if (force > 0)
			//	force = 0;
			//			force = -6;
			//			admController->setForce(force);
			//			admController->setExtForce(ext_force);

			admController->forceControl();

			/* Position control */
//			pos = 60;
			pos = admController->getVirtualMassPosition();
			motor->setPosition(pos);
			motor->run();


			/*DAQ */
			desPos.push_back(pos);
#if (!THETA_CONTROL)
			tsaPos.push_back(motor->getActualTransPosition());
#else
			tsaPos.push_back(motor->getActualPosition());

#endif
			extForce.push_back(forceSensor->getForceZ());
			desForce.push_back(admController->getDesignForce());
			disp.push_back(admController->getDisplacement());

		}


		rt_task_wait_period();
		//	exec_start = exec_end;
	}
	rt_make_soft_real_time();
	rt_task_delete(rt_task);

	return 0;
}


void reset(){
	stop = false;
	// reset time
	t = 0.0f;
	// reset data
	desPos.clear();
	disp.clear();
	extForce.clear();
	desForce.clear();

	v_Kp.clear();
	v_Ki.clear();
	v_Kd.clear();
	// reintialize thread

	if (!(motor_control_thread = rt_thread_create(motor_control_task, NULL, 10000))){
		printf("ERROR IN CREATING THREAD\n");
		exit(1);
	}
}
/*
 * Calibration function for force sensor
 */

void calibration(const boost::system::error_code& err,boost::asio::deadline_timer* t)
{
	if (err)
		return;
	if (!forceSensor->isCalibrateRequired()){
		printf("Finished calibration\n");
	} else {
		forceSensor->readRawValue();
		forceSensor->calibrate();
		cal_count++;
		t->async_wait(boost::bind(calibration,boost::asio::placeholders::error,t));
	}
}

/*
 * Flush data to file
 */
void flushToFile(string filename){

	if (f_data.is_open())
		f_data.close();

	f_data.open(filename.c_str(),std::fstream::out);

	f_data << stiffness << endl;

	f_data << "data = [";
	int i = 0;

	for (int i = 0; i < desPos.size();i++){
		f_data << desPos[i] << " " << tsaPos[i] << " " << extForce[i] << " " << desForce[i]<< " " << disp[i]  << endl;
	}
	f_data << "];";
	f_data.close();
}
void keyPress(){
	cout<<"Press <s> to stop motor."<<endl;
	cout<<"Press <r> to reset position."<<endl;
	cout<<"Press <f> flush data to file."<<endl;
	cout<<"Press <t> to toggle mode."<<endl;
	cout<<"Press <q> to exit the program."<<endl;

	cout<<"Press <l> to rotate clockwise."<<endl;
	cout<<"Press <k> to pause the motor."<<endl;
	cout<<"Press <j> to rotate counterclockwise."<<endl;


	char c;
	char lastKey;
	while (true) {
		cin >> lastKey;
		switch ( lastKey ) {
		case 's':
			cout << "Start Motor" << endl;
			//			stop = true;
			//			motor->disable();
			//			reset();
			t = 0;
			user_motor->enable();
			user_motor->calBias();
			motor->enable();
			motor->resetCalibrationMode();
			start = true;
			//			rt_thread_join(motor_control_thread);
			break;
		case 'r':
			cout << "Reset Motor!!!!: " << endl;
			motor->reset();
			//			admController->reset();
			//			reset();
			//			rt_thread_join(motor_control_thread);
			break;
		case 'f':
			start = false;

			cout << "Flushing data to file......" << endl;
			flushToFile("position.m");
			cout << "Done saving data to file ....." <<  endl;
			//			reset();
			user_motor->disable();
			user_motor->setCalibrationMode();
			motor->disable();
			motor->setCalibrationMode();
			break;

		case 't':
			if (motor->isCalibrationMode())
				motor->resetCalibrationMode();
			else
				motor->setCalibrationMode();
			break;

		case 'q':
			cout << "Exiting..." << endl;
			user_motor->setCalibrationMode();
			motor->setCalibrationMode();
			motor->disable();
			user_motor->disable();
			stop_rt_timer();
			rt_task_delete(maintask);

			printf("MASTER %p ENDS\n", maintask);
			boost::this_thread::interruption_point();
			exit(0);
			break;

		case 'j':
			//		motor->rotateClockwise();
			break;
		case 'k':
			motor->stop();
			break;
		case 'l':
			//		motor->rotateCounterClockwise();
			break;
		default:
			break;
		}
	}

}


int main(int argc, char *argv[]) {
	cout << "Starting....." << endl;

	/*
	 * Parsing arguments
	 */
	int arg = 1;
	while(arg < argc - 1)
	{
		if (!strcmp(argv[arg], "-PID")){
			Kp = (double)atof(argv[++arg]);
			cout << "Kp: " << Kp<< endl;

			Ki = (double)atof(argv[++arg]);
			cout << "Ki: " << Ki<< endl;

			Kd = (double)atof(argv[++arg]);
			cout << "Kd: " << Kd<< endl;
			arg++;
		} else if (!strcmp(argv[arg], "-AdmPara")){
			proxy_mass = (double)atof(argv[++arg]);
			cout << "mass:  " << proxy_mass << endl;

			spring = (double)atof(argv[++arg]);
			cout << "spring:  " << spring << endl;

			damper = (double)atof(argv[++arg]);
			cout << "damper:  " << damper << endl;

			integral = (double)atof(argv[++arg]);
			arg++;
			cout << "integral:  " << integral << endl;
		}  else if (!strcmp(argv[arg], "-stiffness")){
			stiffness = (double)atof(argv[++arg]);
			cout << "stiffness:  " << stiffness<< endl;
			arg++;
		} else if (!strcmp(argv[arg], "-damper")){
			viscosity = (double)atof(argv[++arg]);
			cout << "viscosity:  " << viscosity<< endl;
		} else {
			cerr << "Unrecognized option: " << argv[arg] << endl;
			exit(1);
		}
	}
	v_time = new vector<int>;
	cout << "Sampling time " << deltaT << " sec " << endl;

	/*RTAI*/
	maintask = rt_task_init(nam2num("MAIN"), 1, 0, 0);
	/* DAQ device*/
	comedi_t *device;
	/* Connect to DAQ board*/
	device = comedi_open(COMEDI_DEFAULT_DEVIVE);
	if(device == NULL){
		printf("Error connecting DAQ board\n");
		return -1;
	} else {
		printf("Connected to DAQ board: ");
		printf(comedi_get_board_name(device));
		printf("\n");
	}

	forceSensor = new ForceSensorDH(device,COMEDI_AN_IN_SUB,FIRST_INDEX_FORCE,AREF_DIFF);

	if (COMEDI_ERROR == forceSensor->DAQcalibration()){
		printf("Error calibrating DAQ board -Force Sensor\n");
	}
	forceSensor->setSampleFrequency(samplingfrequency);
	cout << "Calibration length" << forceSensor->getCalibrationLength();

	/* User */
	user_motor = new Motor(device,COMEDI_AN_OUT_SUB,COMEDI_AN_IN_SUB,COMEDI_DI_SUB1,COMEDI_COUNTER_SUB2,COMEDI_ADC_IN_0,COMEDI_DAC_OUT_1,COMEDI_DIO_OUT_2,COMEDI_COUNT_IN_0,COMEDI_RANGE);

	if ((COMEDI_ERROR == user_motor->DAQcalibration(COMEDI_AN_OUT_SUB,COMEDI_DAC_OUT_1,COMEDI_RANGE)) ||  // Analog output
			(COMEDI_ERROR == user_motor->DAQcalibration(COMEDI_AN_IN_SUB,COMEDI_ADC_IN_0,COMEDI_RANGE))	)// Analog input
	{
		printf("Error calibrating DAQ board - Analog output\n");
	}
	user_motor->setupCounter(COMEDI_COUNTER_SUB2,COMEDI_COUNT_IN_0,0);
	user_motor->resetCounter(COMEDI_COUNTER_SUB2,COMEDI_COUNT_IN_0);
	user_motor->setSamplingTime(deltaT);
	//	user_motor->enable();
	user_motor->setCalibrationMode();

	/*Timer for calibration*/

	boost::asio::deadline_timer timer_cali(t_cali_service, boost::posix_time::milliseconds(1000));
	timer_cali.async_wait(boost::bind(calibration,boost::asio::placeholders::error, &timer_cali));
	forceSensor->calibrationRequired();
	t_cali_service.run();
	while (forceSensor->isCalibrateRequired()){}

	/* Admittance controller configuration */
	admController = new AdmittanceController(device,COMEDI_AN_IN_SUB,COMEDI_ADC_IN_1,COMEDI_ADC_IN_2,COMEDI_RANGE);
	if ((COMEDI_ERROR == admController->DAQcalibration(COMEDI_AN_IN_SUB,COMEDI_ADC_IN_1,COMEDI_RANGE)) ||
			(COMEDI_ERROR == admController->DAQcalibration(COMEDI_AN_IN_SUB,COMEDI_ADC_IN_2,COMEDI_RANGE))) 	// Analog input
	{
		printf("Error calibrating DAQ board - Analog input for FSR sensor\n");
	}

	/* Motor Configuration */
#if SPEED_CONTROL
	motor = new Motor(device,COMEDI_AN_OUT_SUB,COMEDI_AN_IN_SUB,COMEDI_DI_SUB1,COMEDI_ADC_IN_7,COMEDI_DAC_OUT_0,COMEDI_DIO_OUT_1,COMEDI_RANGE);
#else
	motor = new Motor(device,COMEDI_AN_OUT_SUB,COMEDI_AN_IN_SUB,COMEDI_DI_SUB1,COMEDI_COUNTER_SUB,COMEDI_ADC_IN_7,COMEDI_DAC_OUT_0,COMEDI_DIO_OUT_1,COMEDI_COUNT_IN_0,COMEDI_RANGE);
#endif

	motor->setupCounter(COMEDI_COUNTER_SUB,COMEDI_COUNT_IN_0,0);
	if ((COMEDI_ERROR == motor->DAQcalibration(COMEDI_AN_OUT_SUB,COMEDI_DAC_OUT_0,COMEDI_RANGE)) || // Analog output
			(COMEDI_ERROR == motor->DAQcalibration(COMEDI_AN_IN_SUB,COMEDI_ADC_IN_7,COMEDI_RANGE))	)// Analog input
	{
		printf("Error calibrating DAQ board - Analog output\n");
	}

	// Motor
	motor->setSamplingTime(deltaT);
	motor->setPidParameters(Kp,Ki,Kd);
	motor->setupCounter(COMEDI_COUNTER_SUB,COMEDI_COUNT_IN_0,0);
	//	motor->loadScheduleGainParameters("PID.txt");
	//	motor->updatePidParameters(stiffness);
	motor->enable();

	// Admittance Control
	admController->setSamplingTime(deltaT);
	admController->setMassSpringDamperModel(mass,spring,damper,integral);
	admController->setPerceiveStiffness(stiffness,viscosity);
	admController->loadStiffnessRange("stiffness.txt");
	admController->setProxyMass(proxy_mass);
	// set realtime timer to run in pure periodic mode
	rt_set_periodic_mode();
	// start realtime timer and scheduler
	start_rt_timer(0);
	if (!(motor_control_thread = rt_thread_create(motor_control_task, NULL, 10000))){
		printf("ERROR IN CREATING THREAD\n");
		exit(1);
	}
	//	/* Thread for key press*/
	boost::thread key_thread(&keyPress);
	key_thread.join();

	return 0;
}
