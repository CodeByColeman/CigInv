################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/main.c \
../src/stm32f4xx_hal_msp.c \
../src/stm32f4xx_it.c \
../src/syscalls.c \
../src/sysmem.c \
../src/system_stm32f4xx.c \
../src/vl53l0x_class.c 

OBJS += \
./src/main.o \
./src/stm32f4xx_hal_msp.o \
./src/stm32f4xx_it.o \
./src/syscalls.o \
./src/sysmem.o \
./src/system_stm32f4xx.o \
./src/vl53l0x_class.o 

C_DEPS += \
./src/main.d \
./src/stm32f4xx_hal_msp.d \
./src/stm32f4xx_it.d \
./src/syscalls.d \
./src/sysmem.d \
./src/system_stm32f4xx.d \
./src/vl53l0x_class.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32F4 -DSTM32F446RETx -DNUCLEO_F446RE -DDEBUG -DSTM32F446xx -DUSE_HAL_DRIVER -I"/home/coleman/VDW_Workspace1/ranging_2/HAL_Driver/Inc/Legacy" -I"/home/coleman/VDW_Workspace1/ranging_2/Utilities/STM32F4xx-Nucleo" -I"/home/coleman/VDW_Workspace1/ranging_2/inc" -I"/home/coleman/VDW_Workspace1/ranging_2/CMSIS/device" -I"/home/coleman/VDW_Workspace1/ranging_2/CMSIS/core" -I"/home/coleman/VDW_Workspace1/ranging_2/HAL_Driver/Inc" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


