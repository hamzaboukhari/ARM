################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/IO.c \
../src/LinkedList.c \
../src/ass_branch.c \
../src/ass_data_process.c \
../src/ass_mutliply.c \
../src/assemble.c \
../src/data_transfer.c \
../src/symTable.c \
../src/utils.c 

OBJS += \
./src/IO.o \
./src/LinkedList.o \
./src/ass_branch.o \
./src/ass_data_process.o \
./src/ass_mutliply.o \
./src/assemble.o \
./src/data_transfer.o \
./src/symTable.o \
./src/utils.o 

C_DEPS += \
./src/IO.d \
./src/LinkedList.d \
./src/ass_branch.d \
./src/ass_data_process.d \
./src/ass_mutliply.d \
./src/assemble.d \
./src/data_transfer.d \
./src/symTable.d \
./src/utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


