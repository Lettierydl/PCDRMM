################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Arquivo.cpp \
../src/Dados.cpp \
../src/Solucao.cpp \
../src/Teste.cpp \
../src/main.cpp 

OBJS += \
./src/Arquivo.o \
./src/Dados.o \
./src/Solucao.o \
./src/Teste.o \
./src/main.o 

CPP_DEPS += \
./src/Arquivo.d \
./src/Dados.d \
./src/Solucao.d \
./src/Teste.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


