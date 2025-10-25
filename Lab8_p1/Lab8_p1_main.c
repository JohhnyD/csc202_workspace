//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  John
//
//       LAB NAME:  Lab 8: Analog to Digital Converter
//
//      FILE NAME:  Lab8_p1_main.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This project runs on the LP_MSPM0G3507 LaunchPad board interfacing to
//    the CSC202 Expansion board.
//
//    This code takes the ADC value of the channel of the photodiode sensor and
//    returns the value back of how much IR is being detected by the sensor
//
//*****************************************************************************
//*****************************************************************************

//-----------------------------------------------------------------------------
// Loads standard C include files
//-----------------------------------------------------------------------------
#include <stdio.h>

//-----------------------------------------------------------------------------
// Loads MSP launchpad board support macros and definitions
//-----------------------------------------------------------------------------
#include <ti/devices/msp/msp.h>
#include "adc.h"
#include "clock.h"
#include "LaunchPad.h"
#include "lcd1602.h"
#include "ti/devices/msp/m0p/mspm0g350x.h"
#include "ti/devices/msp/peripherals/hw_adc12.h"
#include "ti/devices/msp/peripherals/hw_gpio.h"
#include "ti/devices/msp/peripherals/m0p/hw_cpuss.h"

//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void run_lab8_p1 (void);
void config_pb1_interrupt (void);
void GROUP1_IRQHandler(void);
//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define photodiodechannel7 7
//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------
bool g_pb1_pressed = false;
bool g_pb2_pressed = false;

// Define a structure to hold different data types

int main(void)
{
  // Configure the LaunchPad board
  clock_init_40mhz();
  launchpad_gpio_init();
  dipsw_init();
  I2C_init();
  lcd1602_init();
  OPA0_init(6);
  OPA0_enable();
  ADC0_init(ADC12_MEMCTL_VRSEL_VDDA_VSSA);

  config_pb1_interrupt();

  run_lab8_p1();
 
 NVIC_DisableIRQ(GPIOB_INT_IRQn);
 // Endless loop to prevent program from ending
 while (1);

} /* main */

//-----------------------------------------------------------------------------
// run_lab8_p1 function:
//-----------------------------------------------------------------------------


bool done = false;
uint16_t ADC_value;
uint16_t ADC_threshold_value = 2500;

void run_lab8_p1 (void)
{
    while(!done)
    {
        ADC_value = ADC0_in(photodiodechannel7);

        if(ADC_value <= ADC_threshold_value)
        {
            msec_delay(500);
            lcd_clear();
            lcd_set_ddram_addr(LCD_LINE1_ADDR);
            lcd_write_string("Status: Dark");
            lcd_set_ddram_addr(LCD_LINE2_ADDR);
            lcd_write_string("ADC = ");
            lcd_set_ddram_addr(LCD_LINE2_ADDR + LCD_CHAR_POSITION_10);
            lcd_write_doublebyte(ADC_value);
        }

        else
        {
            msec_delay(500);
            lcd_clear();
            lcd_set_ddram_addr(LCD_LINE1_ADDR);
            lcd_write_string("Status: Light");
            lcd_set_ddram_addr(LCD_LINE2_ADDR);
            lcd_write_string("ADC = ");
            lcd_set_ddram_addr(LCD_LINE2_ADDR + LCD_CHAR_POSITION_10);
            lcd_write_doublebyte(ADC_value);
        }

        if (g_pb1_pressed)
        {
            done = true;
            g_pb1_pressed = false;
        }

    }
    lcd_clear();
    lcd_write_string("Program Stopped");
    msec_delay(2000);
}

void config_pb1_interrupt (void)
{
    //set PB1 to rising edge (after inversion)
    GPIOB->POLARITY31_16 = GPIO_POLARITY31_16_DIO18_RISE;

    //Ensure bit is cleared
    GPIOB->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO18_CLR;

    //Unmask PB1 to allow interrupt 
    GPIOB->CPU_INT.IMASK = GPIO_CPU_INT_IMASK_DIO18_SET;

    NVIC_EnableIRQ(GPIOB_INT_IRQn);
} /* config_pb1_interrupt */

void GROUP1_IRQHandler(void)
{
    uint32_t group_gpio_iidx;
    uint32_t gpio_mis;

    do
    {
        group_gpio_iidx = CPUSS->INT_GROUP[1].IIDX;
        switch(group_gpio_iidx)
        {
            case (CPUSS_INT_GROUP_IIDX_STAT_INT0): //GPIOA
                gpio_mis = GPIOA->CPU_INT.MIS;
                if((gpio_mis & GPIO_CPU_INT_MIS_DIO15_MASK) == GPIO_CPU_INT_MIS_DIO15_SET)
                {
                    g_pb2_pressed = true;
                    GPIOA->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO15_CLR;
                } /* if */

                break;
            case (CPUSS_INT_GROUP_IIDX_STAT_INT1): //GPIOB
                gpio_mis = GPIOB->CPU_INT.MIS;
                if((gpio_mis & GPIO_CPU_INT_MIS_DIO18_MASK) == GPIO_CPU_INT_MIS_DIO18_SET)
                {
                    g_pb1_pressed = true;
                    GPIOB->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO18_CLR;
                } /* if */
        }
        
    } while (group_gpio_iidx !=0);
}
