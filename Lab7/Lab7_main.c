//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  John Marcello
//
//       LAB NAME:  Lab 7: Working with Interrupts
//
//      FILE NAME:  lab7_main.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This project runs on the LP_MSPM0G3507 LaunchPad board interfacing to
//    the CSC202 Expansion board.
//
//    This code ... *** COMPLETE THIS BASED ON LAB REQUIREMENTS ***
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
#include "clock.h"
#include "LaunchPad.h"
#include "lcd1602.h"


//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void SysTick_Handler(void);

//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define MSPM0_CLOCK_FREQUENCY                                            (40E6)
#define SYS_TICK_PERIOD                                              (10.25E-3)
#define SYST_TICK_PERIOD_COUNT        (SYS_TICK_PERIOD * MSPM0_CLOCK_FREQUENCY)

#define NUM_STATES                                                         (18)
//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------
const uint8_t seg7_letter_code[] = 
{
    0x6D, 0x00, 0x6D, 0x00, 0x6D, 0x00,  //S
    0x3F, 0x00, 0x3F, 0x00, 0x3F, 0x00,  //O
    0x6D, 0x00, 0x6D, 0x00, 0x6D, 0x00   //S
};

const uint8_t delay_count[] = 
{
    0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x18,    //dots
    0x24, 0x24, 0x24, 0x24, 0x24, 0x24,    //dash
    0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x30     //dots
};

// Define a structure to hold different data types

int main(void)
{
  // Configure the LaunchPad board
  clock_init_40mhz();
  launchpad_gpio_init();
  leds_init();
  leds_disable();
  seg7_init();
  I2C_init();
  lcd1602_init();
  dipsw_init();



  sys_tick_init(SYST_TICK_PERIOD_COUNT);
  SysTick_Handler();

  for(; ; );

 // Endless loop to prevent program from ending
 while (1);

} /* main */

//-----------------------------------------------------------------------------
// Function SystemTick Handler:
//-----------------------------------------------------------------------------

void SysTick_Handler(void)
{
    static uint16_t delay_time = 1;
    static uint16_t code_index = 0;

    delay_time--;
    if (delay_time == 0)
    {
        // delay time has expires so now move on to next letter to display
        seg7_on(seg7_letter_code[code_index], SEG7_DIG0_ENABLE_IDX);

        //get next delay time
        delay_time = delay_count[code_index];
        code_index++;

        if (code_index == NUM_STATES)
        {
            // we went through SOS message so restart index
            code_index = 0;
        } /*if*/
    }/*if*/
}/*SysTick_Handler*/

void Lab7_p1 (void)
{
    bool done = false;
    char message[] = "Microcontroller are fun.";
    uint8_t LCD_Adress;

    while (done!)
    {
        for (LCD_Adress = 0x4F; LCD_Adress > 0x40;)
        {
            lcd_clear();
            lcd_set_ddram_addr(LCD_LINE2_ADDR);
            lcd_write_string(message);
            delay(200);
        }

        uint8_t index = 0;
 
        while (message [index] != '\o')
        {
            lcd_clear();
            LCD_Adress = 0x40;
            lcd_write_string(message + index);
            msec_delay(200);
        }
        
    }
}