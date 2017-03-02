################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../AdmittanceController.cpp \
../daqdevice.cpp \
../forcesensordh.cpp \
../main.cpp \
../motor.cpp \
../sensordh.cpp 

O_SRCS += \
../AdmittanceController.o \
../daqdevice.o \
../forcesensordh.o \
../main.o \
../motor.o \
../sensordh.o 

OBJS += \
./AdmittanceController.o \
./daqdevice.o \
./forcesensordh.o \
./main.o \
./motor.o \
./sensordh.o 

CPP_DEPS += \
./AdmittanceController.d \
./daqdevice.d \
./forcesensordh.d \
./main.d \
./motor.d \
./sensordh.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/realtime/include -I"/home/haquang/workspace/rtai/TSAControl" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


