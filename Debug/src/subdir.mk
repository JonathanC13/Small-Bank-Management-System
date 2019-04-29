################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/BankManagementSystemController.cpp \
../src/BankManagementSystemModel.cpp \
../src/BankManagementSystemUDPcontroller.cpp \
../src/BankManagementSystemView.cpp \
../src/SmallBankManagementSystem.cpp 

OBJS += \
./src/BankManagementSystemController.o \
./src/BankManagementSystemModel.o \
./src/BankManagementSystemUDPcontroller.o \
./src/BankManagementSystemView.o \
./src/SmallBankManagementSystem.o 

CPP_DEPS += \
./src/BankManagementSystemController.d \
./src/BankManagementSystemModel.d \
./src/BankManagementSystemUDPcontroller.d \
./src/BankManagementSystemView.d \
./src/SmallBankManagementSystem.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


