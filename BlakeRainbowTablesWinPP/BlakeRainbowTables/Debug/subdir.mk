################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CreateRBT.c \
../Main.c \
../RedFun.c \
../SearchRBT.c \
../blake_ref.c \
../hashOperations.c \
../llist.c 

OBJS += \
./CreateRBT.o \
./Main.o \
./RedFun.o \
./SearchRBT.o \
./blake_ref.o \
./hashOperations.o \
./llist.o 

C_DEPS += \
./CreateRBT.d \
./Main.d \
./RedFun.d \
./SearchRBT.d \
./blake_ref.d \
./hashOperations.d \
./llist.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -std=c11 -D_FILE_OFFSET_BITS -O3 -Ofast -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


