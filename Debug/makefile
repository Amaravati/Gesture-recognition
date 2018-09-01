################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := ${ComSpec}

-include ../makefile.init

RM := clean

# All of the sources participating in the build are defined here
-include sources.mk
-include system/startup_ldf/subdir.mk
-include system/pinmux/GeneratedSources/subdir.mk
-include system/drivers/device_class/video/queue_mgr/subdir.mk
-include system/drivers/device_class/video/subdir.mk
-include system/drivers/device_class/subdir.mk
-include system/BF707_BLIP2/drivers/sensor/omnivision/ov7692/subdir.mk
-include system/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(DSP_DEPS)),)
-include $(DSP_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LINKER_LOG_OUTPUTS += \
linker_log.xml \

OBJS := $(SRC_OBJS)
# Quote makefile variable : OBJS 
OBJS := $(subst \ ,@, $(OBJS))
OBJS := $(foreach var, $(OBJS), "$(subst @, ,$(var))")
# Quote makefile variable : C++_DEPS 
C++_DEPS := $(subst \ ,@, $(C++_DEPS))
C++_DEPS := $(foreach var, $(C++_DEPS), "$(subst @, ,$(var))")
# Quote makefile variable : C_DEPS 
C_DEPS := $(subst \ ,@, $(C_DEPS))
C_DEPS := $(foreach var, $(C_DEPS), "$(subst @, ,$(var))")
# Quote makefile variable : ASM_DEPS 
ASM_DEPS := $(subst \ ,@, $(ASM_DEPS))
ASM_DEPS := $(foreach var, $(ASM_DEPS), "$(subst @, ,$(var))")
# Quote makefile variable : CC_DEPS 
CC_DEPS := $(subst \ ,@, $(CC_DEPS))
CC_DEPS := $(foreach var, $(CC_DEPS), "$(subst @, ,$(var))")
# Quote makefile variable : DSP_DEPS 
DSP_DEPS := $(subst \ ,@, $(DSP_DEPS))
DSP_DEPS := $(foreach var, $(DSP_DEPS), "$(subst @, ,$(var))")
# Quote makefile variable : CPP_DEPS 
CPP_DEPS := $(subst \ ,@, $(CPP_DEPS))
CPP_DEPS := $(foreach var, $(CPP_DEPS), "$(subst @, ,$(var))")
# Quote makefile variable : S_DEPS 
S_DEPS := $(subst \ ,@, $(S_DEPS))
S_DEPS := $(foreach var, $(S_DEPS), "$(subst @, ,$(var))")
# Quote makefile variable : CXX_DEPS 
CXX_DEPS := $(subst \ ,@, $(CXX_DEPS))
CXX_DEPS := $(foreach var, $(CXX_DEPS), "$(subst @, ,$(var))")
# Quote makefile variable : C_UPPER_DEPS 
C_UPPER_DEPS := $(subst \ ,@, $(C_UPPER_DEPS))
C_UPPER_DEPS := $(foreach var, $(C_UPPER_DEPS), "$(subst @, ,$(var))")

# All Target
all: class_checkn_realtime_test2.dxe

# Tool invocations
class_checkn_realtime_test2.dxe: $(LDF_SRCS) $(SRC_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: CrossCore Blackfin Linker'
	ccblkfn.exe -proc ADSP-BF707 -si-revision any -TC:/Users/aamaravati3/ADI_work/class_checkn_realtime_test2/system/startup_ldf/app.ldf -no-mem -LC:/Users/aamaravati3/ADI_work/class_checkn_realtime_test2/Debug/system/startup_ldf -LC:/Analog\ Devices/SoftwareModules/Graphics2D-BF-Rel4.1.0/Lib-Production-CCES -LC:/Analog\ Devices/SoftwareModules/ImageProcessingToolbox-BF-Rel2.4.0/Lib-Production-CCES -flags-link -MDDEBUG,-MDCORE0 -add-debug-libpaths -no-utility-rom -flags-link libadi_graphics2d.dlb, -flags-link libadi_image_tool_box_cces_bf70x.dlb -o class_checkn_realtime_test2.dxe @input-file.txt $(LDF_SRCS)  -flags-link -od,.\.
	@echo 'Finished building target: $@'
	@echo ' '

linker_log.xml: class_checkn_realtime_test2.dxe $(LDF_SRCS) $(SRC_OBJS)

# Other Targets
clean:
	-$(RM) $(C++_DEPS)$(OBJS)$(C_DEPS)$(ASM_DEPS)$(CC_DEPS)$(DSP_DEPS)$(DXE)$(CPP_DEPS)$(LINKER_LOG_OUTPUTS)$(S_DEPS)$(CXX_DEPS)$(C_UPPER_DEPS) class_checkn_realtime_test2.dxe
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
