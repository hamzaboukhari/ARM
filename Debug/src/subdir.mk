################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/a_IO.c \
../src/a_branch.c \
../src/a_data_process.c \
../src/a_data_transfer.c \
../src/a_linked_list.c \
../src/a_multiply.c \
../src/a_symTable.c \
../src/assemble.c \
../src/e_branch_methods.c \
../src/e_data_process_methods.c \
../src/e_data_transfer_methods.c \
../src/e_instruction.c \
../src/e_multiply_methods.c \
../src/emulate.c \
../src/utils.c 

OBJS += \
./src/a_IO.o \
./src/a_branch.o \
./src/a_data_process.o \
./src/a_data_transfer.o \
./src/a_linked_list.o \
./src/a_multiply.o \
./src/a_symTable.o \
./src/assemble.o \
./src/e_branch_methods.o \
./src/e_data_process_methods.o \
./src/e_data_transfer_methods.o \
./src/e_instruction.o \
./src/e_multiply_methods.o \
./src/emulate.o \
./src/utils.o 

C_DEPS += \
./src/a_IO.d \
./src/a_branch.d \
./src/a_data_process.d \
./src/a_data_transfer.d \
./src/a_linked_list.d \
./src/a_multiply.d \
./src/a_symTable.d \
./src/assemble.d \
./src/e_branch_methods.d \
./src/e_data_process_methods.d \
./src/e_data_transfer_methods.d \
./src/e_instruction.d \
./src/e_multiply_methods.d \
./src/emulate.d \
./src/utils.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross GCC Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


