################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../classes/AABB.c \
../classes/Camera.c \
../classes/ComplexAABB.c \
../classes/GameObject.c \
../classes/Matrix.c \
../classes/ModelLoader.c \
../classes/Physics.c \
../classes/Shaders.c 

OBJS += \
./classes/AABB.o \
./classes/Camera.o \
./classes/ComplexAABB.o \
./classes/GameObject.o \
./classes/Matrix.o \
./classes/ModelLoader.o \
./classes/Physics.o \
./classes/Shaders.o 

C_DEPS += \
./classes/AABB.d \
./classes/Camera.d \
./classes/ComplexAABB.d \
./classes/GameObject.d \
./classes/Matrix.d \
./classes/ModelLoader.d \
./classes/Physics.d \
./classes/Shaders.d 


# Each subdirectory must supply rules for building sources it contributes
classes/%.o: ../classes/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


