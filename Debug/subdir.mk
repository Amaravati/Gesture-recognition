################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../FrameCapture_ClassDriver.c \
../classifiers.c \
../gesture_samples.c \
../mc_tr.c \
../pca_info.c 

SRC_OBJS += \
./FrameCapture_ClassDriver.doj \
./classifiers.doj \
./gesture_samples.doj \
./mc_tr.doj \
./pca_info.doj 

C_DEPS += \
./FrameCapture_ClassDriver.d \
./classifiers.d \
./gesture_samples.d \
./mc_tr.d \
./pca_info.d 


# Each subdirectory must supply rules for building sources it contributes
FrameCapture_ClassDriver.doj: ../FrameCapture_ClassDriver.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn.exe -c -file-attr ProjectName="class_checkn_realtime_test2" -proc ADSP-BF707 -flags-compiler --no_wrap_diagnostics -si-revision any -g -D_DEBUG -D__ADSPBF707_BLIP2__ -DENA_OV7692_SENSOR -DCORE0 -I"C:\Users\aamaravati3\ADI_work\class_checkn_realtime_test2\system" -I"C:\Analog Devices\CrossCore Embedded Studio 2.4.0/Blackfin/lib/src/drivers/source/device_class" -I"C:/Analog Devices/SoftwareModules/ADZS_BF707_BLIP2_Board-Rel1.0.0/Blackfin/include" -I"C:/Analog Devices/SoftwareModules/Graphics2D-BF-Rel4.1.0/Include" -I"C:/Analog Devices/SoftwareModules/ImageProcessingToolbox-BF-Rel2.4.0/Include" -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -no-utility-rom -gnu-style-dependencies -MD -Mo "FrameCapture_ClassDriver.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

classifiers.doj: ../classifiers.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn.exe -c -file-attr ProjectName="class_checkn_realtime_test2" -proc ADSP-BF707 -flags-compiler --no_wrap_diagnostics -si-revision any -g -D_DEBUG -D__ADSPBF707_BLIP2__ -DENA_OV7692_SENSOR -DCORE0 -I"C:\Users\aamaravati3\ADI_work\class_checkn_realtime_test2\system" -I"C:\Analog Devices\CrossCore Embedded Studio 2.4.0/Blackfin/lib/src/drivers/source/device_class" -I"C:/Analog Devices/SoftwareModules/ADZS_BF707_BLIP2_Board-Rel1.0.0/Blackfin/include" -I"C:/Analog Devices/SoftwareModules/Graphics2D-BF-Rel4.1.0/Include" -I"C:/Analog Devices/SoftwareModules/ImageProcessingToolbox-BF-Rel2.4.0/Include" -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -no-utility-rom -gnu-style-dependencies -MD -Mo "classifiers.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

gesture_samples.doj: ../gesture_samples.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn.exe -c -file-attr ProjectName="class_checkn_realtime_test2" -proc ADSP-BF707 -flags-compiler --no_wrap_diagnostics -si-revision any -g -D_DEBUG -D__ADSPBF707_BLIP2__ -DENA_OV7692_SENSOR -DCORE0 -I"C:\Users\aamaravati3\ADI_work\class_checkn_realtime_test2\system" -I"C:\Analog Devices\CrossCore Embedded Studio 2.4.0/Blackfin/lib/src/drivers/source/device_class" -I"C:/Analog Devices/SoftwareModules/ADZS_BF707_BLIP2_Board-Rel1.0.0/Blackfin/include" -I"C:/Analog Devices/SoftwareModules/Graphics2D-BF-Rel4.1.0/Include" -I"C:/Analog Devices/SoftwareModules/ImageProcessingToolbox-BF-Rel2.4.0/Include" -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -no-utility-rom -gnu-style-dependencies -MD -Mo "gesture_samples.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

mc_tr.doj: ../mc_tr.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn.exe -c -file-attr ProjectName="class_checkn_realtime_test2" -proc ADSP-BF707 -flags-compiler --no_wrap_diagnostics -si-revision any -g -D_DEBUG -D__ADSPBF707_BLIP2__ -DENA_OV7692_SENSOR -DCORE0 -I"C:\Users\aamaravati3\ADI_work\class_checkn_realtime_test2\system" -I"C:\Analog Devices\CrossCore Embedded Studio 2.4.0/Blackfin/lib/src/drivers/source/device_class" -I"C:/Analog Devices/SoftwareModules/ADZS_BF707_BLIP2_Board-Rel1.0.0/Blackfin/include" -I"C:/Analog Devices/SoftwareModules/Graphics2D-BF-Rel4.1.0/Include" -I"C:/Analog Devices/SoftwareModules/ImageProcessingToolbox-BF-Rel2.4.0/Include" -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -no-utility-rom -gnu-style-dependencies -MD -Mo "mc_tr.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

pca_info.doj: ../pca_info.c
	@echo 'Building file: $<'
	@echo 'Invoking: CrossCore Blackfin C/C++ Compiler'
	ccblkfn.exe -c -file-attr ProjectName="class_checkn_realtime_test2" -proc ADSP-BF707 -flags-compiler --no_wrap_diagnostics -si-revision any -g -D_DEBUG -D__ADSPBF707_BLIP2__ -DENA_OV7692_SENSOR -DCORE0 -I"C:\Users\aamaravati3\ADI_work\class_checkn_realtime_test2\system" -I"C:\Analog Devices\CrossCore Embedded Studio 2.4.0/Blackfin/lib/src/drivers/source/device_class" -I"C:/Analog Devices/SoftwareModules/ADZS_BF707_BLIP2_Board-Rel1.0.0/Blackfin/include" -I"C:/Analog Devices/SoftwareModules/Graphics2D-BF-Rel4.1.0/Include" -I"C:/Analog Devices/SoftwareModules/ImageProcessingToolbox-BF-Rel2.4.0/Include" -structs-do-not-overlap -no-const-strings -no-multiline -warn-protos -double-size-32 -decls-strong -cplbs -no-utility-rom -gnu-style-dependencies -MD -Mo "pca_info.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


