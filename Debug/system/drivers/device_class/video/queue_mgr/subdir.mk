################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.4.0/Blackfin/lib/src/drivers/source/device_class/video/queue_mgr/adi_dev_video_queue.c 

SRC_OBJS += \
./system/drivers/device_class/video/queue_mgr/adi_dev_video_queue.doj 

C_DEPS += \
./system/drivers/device_class/video/queue_mgr/adi_dev_video_queue.d 


# Each subdirectory must supply rules for building sources it contributes
system/drivers/device_class/video/queue_mgr/adi_dev_video_queue.doj: C:/Analog\ Devices/CrossCore\ Embedded\ Studio\ 2.4.0/Blackfin/lib/src/drivers/source/device_class/video/queue_mgr/adi_dev_video_queue.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn.exe -c -file-attr ProjectName="class_checkn_realtime_test2" -proc ADSP-BF707 -flags-compiler --no_wrap_diagnostics -si-revision any -g -D_DEBUG -D__ADSPBF707_BLIP2__ -DENA_OV7692_SENSOR -DCORE0 -I"C:\Users\aamaravati3\ADI_work\class_checkn_realtime_test2\system" -I"C:\Analog Devices\CrossCore Embedded Studio 2.4.0/Blackfin/lib/src/drivers/source/device_class" -I"C:/Analog Devices/SoftwareModules/ADZS_BF707_BLIP2_Board-Rel1.0.0/Blackfin/include" -I"C:/Analog Devices/SoftwareModules/Graphics2D-BF-Rel4.1.0/Include" -I"C:/Analog Devices/SoftwareModules/ImageProcessingToolbox-BF-Rel2.4.0/Include" -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -no-utility-rom -gnu-style-dependencies -MD -Mo "system/drivers/device_class/video/queue_mgr/adi_dev_video_queue.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


