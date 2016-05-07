################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../LaserSnap.cpp \
../MessageController.cpp \
../Serials.cpp \
../main.cpp 

OBJS += \
./LaserSnap.o \
./MessageController.o \
./Serials.o \
./main.o 

CPP_DEPS += \
./LaserSnap.d \
./MessageController.d \
./Serials.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	arm-linux-g++ -I/home/fatsun/Library/OpenCV/install-opencv-2.4.12-s5p4418/include -I/home/fatsun/Library/OpenCV/install-opencv-2.4.12-s5p4418/include/opencv -I/home/fatsun/Library/OpenCV/install-opencv-2.4.12-s5p4418/include/opencv2 -O0 -fPIC -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


