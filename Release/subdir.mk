################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CY8C9560A.c \
../I2CFunctions.c \
../MainFile.c \
../TCPComm.c 

OBJS += \
./CY8C9560A.o \
./I2CFunctions.o \
./MainFile.o \
./TCPComm.o 

C_DEPS += \
./CY8C9560A.d \
./I2CFunctions.d \
./MainFile.d \
./TCPComm.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


