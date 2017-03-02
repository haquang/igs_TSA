################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ATItools/dom.c \
../ATItools/events.c \
../ATItools/expatls.c \
../ATItools/ftconfig.c \
../ATItools/ftrt.c \
../ATItools/node.c \
../ATItools/stack.c \
../ATItools/xmlparse.c \
../ATItools/xmlrole.c \
../ATItools/xmltok.c \
../ATItools/xmltok_impl.c \
../ATItools/xmltok_ns.c 

O_SRCS += \
../ATItools/dom.o \
../ATItools/expatls.o \
../ATItools/ftconfig.o \
../ATItools/ftrt.o \
../ATItools/node.o \
../ATItools/stack.o \
../ATItools/xmlparse.o \
../ATItools/xmlrole.o \
../ATItools/xmltok.o 

OBJS += \
./ATItools/dom.o \
./ATItools/events.o \
./ATItools/expatls.o \
./ATItools/ftconfig.o \
./ATItools/ftrt.o \
./ATItools/node.o \
./ATItools/stack.o \
./ATItools/xmlparse.o \
./ATItools/xmlrole.o \
./ATItools/xmltok.o \
./ATItools/xmltok_impl.o \
./ATItools/xmltok_ns.o 

C_DEPS += \
./ATItools/dom.d \
./ATItools/events.d \
./ATItools/expatls.d \
./ATItools/ftconfig.d \
./ATItools/ftrt.d \
./ATItools/node.d \
./ATItools/stack.d \
./ATItools/xmlparse.d \
./ATItools/xmlrole.d \
./ATItools/xmltok.d \
./ATItools/xmltok_impl.d \
./ATItools/xmltok_ns.d 


# Each subdirectory must supply rules for building sources it contributes
ATItools/%.o: ../ATItools/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


