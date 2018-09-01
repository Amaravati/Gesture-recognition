/*
 ****************************************************************************
 Copyright (c) 2015 - Analog Devices Inc. All Rights Reserved.
 This software is proprietary & confidential to Analog Devices, Inc.
 and its licensors.
 ******************************************************************************

 Title: Frame Capture

 Description:
 Header file for Frame Capture (Class Driver) example for 
 Omnivision OV7692 sensor. 

 *****************************************************************************/

#ifndef __FRAMECAPTURE_CLASSDRIVER_H__
#define __FRAMECAPTURE_CLASSDRIVER_H__

/*=============  I N C L U D E S   =============*/
#include <adi_types.h>
#include <services/gpio/adi_gpio.h>
#include <drivers/sensor/omnivision/ov7692/adi_dev_ov7692.h>

/*=============  D E F I N E S   =============*/
/* Supported Video data display formats */
#define ADI_VIDEO_IN_RAW422_PROG  0
#define ADI_VIDEO_IN_RGB565       1
#define ADI_VIDEO_IN_Y            2
#define rws 30
#define ncs 40
#define matRows 30
#define matCols 40
#define nobyt rws*ncs
#define nmeas 1200
#define buflen 30

/* Number of frames to capture. */
#define NUM_FRAME_NUM_TO_CAPTURE buflen

/* Video Format. */
#define CAPTURE_VIDEO_FORMAT     ADI_DEV_VIDEO_FORMAT_640X480P

/* Video Data Format. */
#define CAPTURE_VIDEO_DATA_FORMAT ADI_VIDEO_IN_Y

/* Frame rate(fps). */
#define CAPTURE_VIDEO_FRAME_RATE 15

/* Aptina ASX340 Sensor specific macros. */
/* Output filename. */
#define CAPTURE_OUTPUT_FILENAME  "C:/Users/aamaravati3/Dropbox/images cap/ov7670_captureyby8fnewd.yuv"
#define CAPTURE_OUTPUT_FILENAME1  "C:/Users/aamaravati3/Dropbox/images cap/ov7670_captureyby8fnewo.yuv"

#define textname  "matrix.txt"


//#define CAPTURE_OUTPUT_FILENAME1  "../ov7670_capture.txt"

/* Timers for Sensor. */
#define SENSOR_TIMER             1

/* Port and pin for the Omnivision OV7692 sensor. */
#define SENSOR_GPIO_PORT         ADI_GPIO_PORT_A
#define SENSOR_GPIO_PIN          ADI_GPIO_PIN_5

/* Entry point for the sensor. */
#define SENSOR_ENTRY_POINT       &ADIDevVideoInOV7692


/* Video buffer Size - to hold up to RGB888 - 720p frame */
//#define VIDEO_BUF_SIZE           (1280u * 720 * 3)

#define VIDEO_BUF_SIZE           (1280u * 720 * 2)

/* Processor specific configuration */
/* ADSP-BF70x processor family */

/* CGU Device number. */
#define CGU_DEV_NUM              (0)
/* External input clock frequency in Hz */

//#define PROC_CLOCK_IN            (24000000u)

//#define PROC_CLOCK_IN            (24000000u)


/* Maximum core clock frequency in Hz */
//#define PROC_MAX_CORE_CLOCK      (384000000u)
//#define PROC_MAX_CORE_CLOCK        (120000000u)


/* Maximum system clock frequency in Hz */
//#define PROC_MAX_SYS_CLOCK       (100000000u)

//#define PROC_MAX_SYS_CLOCK     (216000000u)


#define MHZTOHZ       (1000000)

#define DF_DEFAULT    (0x0)
#define MSEL_DEFAULT  (0x10)
#define SSEL_DEFAULT  (0x8)
#define S1SEL_DEFAULT  (0x8)
#define CSEL_DEFAULT  (0x4)

//#define PROC_CLOCK_IN         (25 * MHZTOHZ)
//#define PROC_MAX_CORE_CLOCK      (500 * MHZTOHZ)
//#define PROC_MAX_SYS_CLOCK    (250 * MHZTOHZ)
//#define SCLK_MAX      (125 * MHZTOHZ)
//#define VCO_MIN       (72 * MHZTOHZ)


#define PROC_CLOCK_IN         (25 * MHZTOHZ)
#define PROC_MAX_CORE_CLOCK      (125 * MHZTOHZ)
#define PROC_MAX_SYS_CLOCK    (125 * MHZTOHZ)
#define PROC_MAX_SYS_CLOCK1    (125 * MHZTOHZ)
#define SCLK_MAX      (72 * MHZTOHZ)
#define SCLK1_MAX      (72 * MHZTOHZ)
#define VCO_MIN       (72 * MHZTOHZ)



#endif  /* __FRAMECAPTURE_CLASSDRIVER_H__ */


