################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ATItools/sampleCode/ftconvert.c 

OBJS += \
./ATItools/sampleCode/ftconvert.o 

C_DEPS += \
./ATItools/sampleCode/ftconvert.d 


# Each subdirectory must supply rules for building sources it contributes
ATItools/sampleCode/%.o: ../ATItools/sampleCode/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


