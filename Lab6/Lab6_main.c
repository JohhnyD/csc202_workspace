//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  John Marcello
//
//       LAB NAME:  Lab 6: Interfacing to Liquid Crystal Display (LCD)
//
//      FILE NAME:  lab6_main.c
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


void run_lab6_p1 (void);

//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------


// Define a structure to hold different data types

int main(void)
{
  // Configure the LaunchPad board
  clock_init_40mhz();
  launchpad_gpio_init();
  seg7_init();
  keypad_init();
  lpsw_init();
  I2C_init();
  lcd1602_init();
  
  //Run lab part 1:
  
  run_lab6_p1();
  

 // Endless loop to prevent program from ending
 while (1);

} /* main */

//-----------------------------------------------------------------------------
//Run lab part 1 code and function:
//-----------------------------------------------------------------------------
void run_lab6_p1 (void)
{
    uint8_t offset;

    for (offset = 'A'; offset< 'Z'; offset++)
    {
        lcd_write_char(offset);
        if (offset == 'P')
        {
            lcd_set_ddram_addr(LCD_LINE2_ADDR);
        }
    }
}
