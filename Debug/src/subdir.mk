################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/adafruit_pn532.c 

OBJS += \
./src/adafruit_pn532.o 

C_DEPS += \
./src/adafruit_pn532.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32 -DSTM32L4 -DSTM32L476RGTx -DNUCLEO_L476RG -DDEBUG -DSTM32L476xx -DUSE_HAL_DRIVER -I"C:/Users/francisco/workspace_stm32/NucleoL476RG_I2C" -I"C:/Users/francisco/workspace_stm32/NucleoL476RG_I2C/inc" -I"C:/Users/francisco/workspace_stm32/AdafruitPN532/inc" -I"C:/Users/francisco/workspace_stm32/nucleo-l476rg_hal_lib" -I"C:/Users/francisco/workspace_stm32/nucleo-l476rg_hal_lib/CMSIS/core" -I"C:/Users/francisco/workspace_stm32/nucleo-l476rg_hal_lib/CMSIS/device" -I"C:/Users/francisco/workspace_stm32/nucleo-l476rg_hal_lib/HAL_Driver/Inc/Legacy" -I"C:/Users/francisco/workspace_stm32/nucleo-l476rg_hal_lib/HAL_Driver/Inc" -I"C:/Users/francisco/workspace_stm32/nucleo-l476rg_hal_lib/Utilities/STM32L4xx_Nucleo" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


