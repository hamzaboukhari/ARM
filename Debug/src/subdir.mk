################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/assemble.c \
../src/branch_methods.c \
../src/data_process_methods.c \
../src/data_transfer_methods.c \
../src/emulate.c \
../src/instruction.c \
../src/multiply_methods.c \
../src/utils.c 

OBJS += \
./src/assemble.o \
./src/branch_methods.o \
./src/data_process_methods.o \
./src/data_transfer_methods.o \
./src/emulate.o \
./src/instruction.o \
./src/multiply_methods.o \
./src/utils.o 

C_DEPS += \
./src/assemble.d \
./src/branch_methods.d \
./src/data_process_methods.d \
./src/data_transfer_methods.d \
./src/emulate.d \
./src/instruction.d \
./src/multiply_methods.d \
./src/utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


