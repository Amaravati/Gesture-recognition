/*
 **
 ** pinmux_config.c source file generated on December 6, 2016 at 19:50:51.	
 **
 ** Copyright (C) 2000-2016 Analog Devices Inc., All Rights Reserved.
 **
 ** This file is generated automatically based upon the options selected in 
 ** the Pin Multiplexing configuration editor. Changes to the Pin Multiplexing
 ** configuration should be made by changing the appropriate options rather
 ** than editing this file.
 **
 ** Selected Peripherals
 ** --------------------
 ** PPI0 (FS1, FS2, CLK, D00, D01, D02, D03, D04, D05, D06, D07)
 ** TM0 (TMR1, TMR2)
 **
 ** GPIO (unavailable)
 ** ------------------
 ** PA06, PA07, PA12, PA13, PA14, PB00, PB01, PB02, PB03, PB04, PB05, PB06, PB07
 */

#include <sys/platform.h>
#include <stdint.h>

#define PPI0_FS1_PORTA_MUX  ((uint32_t) ((uint32_t) 0<<24))
#define PPI0_FS2_PORTA_MUX  ((uint32_t) ((uint32_t) 0<<26))
#define PPI0_CLK_PORTA_MUX  ((uint32_t) ((uint32_t) 0<<28))
#define PPI0_D00_PORTB_MUX  ((uint16_t) ((uint16_t) 0<<14))
#define PPI0_D01_PORTB_MUX  ((uint16_t) ((uint16_t) 0<<12))
#define PPI0_D02_PORTB_MUX  ((uint16_t) ((uint16_t) 0<<10))
#define PPI0_D03_PORTB_MUX  ((uint16_t) ((uint16_t) 0<<8))
#define PPI0_D04_PORTB_MUX  ((uint16_t) ((uint16_t) 0<<6))
#define PPI0_D05_PORTB_MUX  ((uint16_t) ((uint16_t) 0<<4))
#define PPI0_D06_PORTB_MUX  ((uint16_t) ((uint16_t) 0<<2))
#define PPI0_D07_PORTB_MUX  ((uint16_t) ((uint16_t) 0<<0))
#define TM0_TMR1_PORTA_MUX  ((uint16_t) ((uint16_t) 0<<12))
#define TM0_TMR2_PORTA_MUX  ((uint16_t) ((uint16_t) 0<<14))

#define PPI0_FS1_PORTA_FER  ((uint32_t) ((uint32_t) 1<<12))
#define PPI0_FS2_PORTA_FER  ((uint32_t) ((uint32_t) 1<<13))
#define PPI0_CLK_PORTA_FER  ((uint32_t) ((uint32_t) 1<<14))
#define PPI0_D00_PORTB_FER  ((uint16_t) ((uint16_t) 1<<7))
#define PPI0_D01_PORTB_FER  ((uint16_t) ((uint16_t) 1<<6))
#define PPI0_D02_PORTB_FER  ((uint16_t) ((uint16_t) 1<<5))
#define PPI0_D03_PORTB_FER  ((uint16_t) ((uint16_t) 1<<4))
#define PPI0_D04_PORTB_FER  ((uint16_t) ((uint16_t) 1<<3))
#define PPI0_D05_PORTB_FER  ((uint16_t) ((uint16_t) 1<<2))
#define PPI0_D06_PORTB_FER  ((uint16_t) ((uint16_t) 1<<1))
#define PPI0_D07_PORTB_FER  ((uint16_t) ((uint16_t) 1<<0))
#define TM0_TMR1_PORTA_FER  ((uint16_t) ((uint16_t) 1<<6))
#define TM0_TMR2_PORTA_FER  ((uint16_t) ((uint16_t) 1<<7))

int32_t adi_initpinmux(void);

/*
 * Initialize the Port Control MUX and FER Registers
 */
int32_t adi_initpinmux(void) {
    /* PORTx_MUX registers */
    *pREG_PORTA_MUX = PPI0_FS1_PORTA_MUX | PPI0_FS2_PORTA_MUX
     | PPI0_CLK_PORTA_MUX | TM0_TMR1_PORTA_MUX | TM0_TMR2_PORTA_MUX;
    *pREG_PORTB_MUX = PPI0_D00_PORTB_MUX | PPI0_D01_PORTB_MUX
     | PPI0_D02_PORTB_MUX | PPI0_D03_PORTB_MUX | PPI0_D04_PORTB_MUX
     | PPI0_D05_PORTB_MUX | PPI0_D06_PORTB_MUX | PPI0_D07_PORTB_MUX;

    /* PORTx_FER registers */
    *pREG_PORTA_FER = PPI0_FS1_PORTA_FER | PPI0_FS2_PORTA_FER
     | PPI0_CLK_PORTA_FER | TM0_TMR1_PORTA_FER | TM0_TMR2_PORTA_FER;
    *pREG_PORTB_FER = PPI0_D00_PORTB_FER | PPI0_D01_PORTB_FER
     | PPI0_D02_PORTB_FER | PPI0_D03_PORTB_FER | PPI0_D04_PORTB_FER
     | PPI0_D05_PORTB_FER | PPI0_D06_PORTB_FER | PPI0_D07_PORTB_FER;
    return 0;
}

