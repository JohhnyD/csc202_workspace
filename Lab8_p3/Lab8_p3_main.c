//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  John Marcello
//
//       LAB NAME:  Lab 8: Analog to Digital Converter
//
//      FILE NAME:  Lab8_p3_main.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This project runs on the LP_MSPM0G3507 LaunchPad board interfacing to
//    the CSC202 Expansion board.
//
//    This code adds on to part 2 of the lab by using intrrupts for push button
//    1 and pushbutton 2. Pushbutton 1 takes us out of the program while
//    pushbutton 2 takes the temerature reading of a room by initalizing
//    the ADC0 channel 5 for the sensor. The ADC0 value on the LCD displays
//    the value read off the the poteniometer and the temp is displayed on
//    line 2 of the LCD
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
void config_pb1_interrupt (void);
void GROUP1_IRQHandler(void);
void run_lab8_p3 (void);
void config_pb2_interrupt (void);
//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define potentiometerchannel7 7
#define temperaturechannel 5
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
  leds_init();
  leds_enable();
  I2C_init();
  lcd1602_init();
  ADC0_init(ADC12_MEMCTL_VRSEL_VDDA_VSSA);

  config_pb1_interrupt();
  config_pb2_interrupt();
  lcd_clear();

  run_lab8_p3();
 
  NVIC_DisableIRQ(GPIOB_INT_IRQn);
  NVIC_DisableIRQ(GPIOA_INT_IRQn);

 // Endless loop to prevent program from ending
 while (1);

} /* main */

bool done = false;
uint16_t ADC_value;

void run_lab8_p3 (void)
{

    while (!done)
    {
        ADC_value = ADC0_in(potentiometerchannel7);
        
        
        lcd_set_ddram_addr(LCD_LINE1_ADDR);
        lcd_write_string("ADC = ");
        lcd_set_ddram_addr(LCD_LINE1_ADDR + LCD_CHAR_POSITION_8);
        lcd_write_doublebyte(ADC_value);
        lcd_set_ddram_addr(LCD_LINE2_ADDR);
        lcd_write_string ("Temp = ");
        lcd_set_ddram_addr(LCD_LINE2_ADDR+ LCD_CHAR_POSITION_12);
        lcd_write_string("F");
        lcd_set_ddram_addr(LCD_LINE2_ADDR+ LCD_CHAR_POSITION_11);
        lcd_write_char (223);

        

        leds_off();

            for (uint8_t led_idx = LED_BAR_LD0_IDX; led_idx < ADC_value/455;led_idx++)
            {
                led_on(led_idx);
            }
        
        if (g_pb1_pressed)
        {
            done = true;
            g_pb1_pressed = false;
        }
        if (g_pb2_pressed)
        {
            uint32_t adc_result = 0;
            uint16_t celcius_reading = 0;

           
            adc_result = ADC0_in(temperaturechannel);

            celcius_reading = (uint16_t) thermistor_calc_temperature(adc_result);

            lcd_set_ddram_addr(LCD_LINE2_ADDR + LCD_CHAR_POSITION_6);
            lcd_write_doublebyte(celcius_reading*9/5+32);

            g_pb2_pressed = false;
        }
    }
    lcd_clear();
    leds_off();
    lcd_write_string("Program Stopped");
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

void config_pb2_interrupt (void)
{
    //set PB1 to rising edge (after inversion)
    GPIOA->POLARITY15_0 = GPIO_POLARITY15_0_DIO15_RISE;

    //Ensure bit is cleared
    GPIOA->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO15_CLR;

    //Unmask PB1 to allow interrupt 
    GPIOA->CPU_INT.IMASK = GPIO_CPU_INT_IMASK_DIO15_SET;

    NVIC_EnableIRQ(GPIOA_INT_IRQn);
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
