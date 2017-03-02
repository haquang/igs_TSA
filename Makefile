CC = g++
CPPFLAGS = -Wall -g -std=gnu++11 -w -I/usr/realtime/include -O2 -fpermissive
LDFLAGS = -lm -lcomedi -lboost_system -lboost_thread -lncurses -pthread -L/usr/local/lib/
OBJS_ATI = ./ATItools/ftconfig.o ./ATItools/ftrt.o ./ATItools/dom.o ./ATItools/expatls.o ./ATItools/node.o ./ATItools/stack.o ./ATItools/xmlparse.o ./ATItools/xmlrole.o ./ATItools/xmltok.o


all: main.o daqdevice.o motor.o AdmittanceController.o sensordh.o forcesensordh.o
	${CC}   main.o daqdevice.o motor.o AdmittanceController.o sensordh.o forcesensordh.o ${LDFLAGS}  $(OBJS_ATI)  -o TSAControl

daqdevice.o: daqdevice.cpp daqdevice.h 
	${CC} -c ${CPPFLAGS} ${LDFLAGS} daqdevice.cpp

motor.o: motor.cpp motor.h
	${CC} -c ${CPPFLAGS} ${LDFLAGS} motor.cpp

AdmittanceController.o: AdmittanceController.cpp AdmittanceController.h
	${CC} -c ${CPPFLAGS} ${LDFLAGS} AdmittanceController.cpp
	
sensordh.o: sensordh.cpp sensordh.h 
	${CC} -c ${CPPFLAGS} ${LDFLAGS} sensordh.cpp
				
forcesensordh.o: forcesensordh.cpp forcesensordh.h
	${CC} -c ${CPPFLAGS} ${LDFLAGS} forcesensordh.cpp				
				
							
main.o: main.cpp daqdevice.h motor.h AdmittanceController.h sensordh.h forcesensordh.h
	${CC} -c ${CPPFLAGS} main.cpp 

clean:
	rm -rf TSAControl *.o *~

