//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  John Marcello
//
//       LAB NAME:  Lab 10: Serial Communication Interface 
//
//      FILE NAME:  Lab10_p1_main.c
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
#include "adc.h"
#include "lcd1602.h"
#include "uart.h"


//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void run_lab_uarttest (void);

//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define buffer_size 8

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
  I2C_init();
  lcd1602_init();
  UART_init(115200);

  run_lab_uarttest();
  // enter your code here

 
 // Endless loop to prevent program from ending
 while (1);

} /* main */

char character;

uint8_t loopcntr = 0;

void run_lab_uarttest (void)
{
    char buffer[buffer_size]
    uint8_t position_idx = 0;

    do
    {
        character = UART_in_char();
        UART_out_char(character);

        if(character == '\r')
        {
            buffer[position_idx] = NULL;
        }

        else if (character = '\b') 
        {
            
        }

        else
        {
            buffer[position_idx] = character;
            position_idx++;
        }
        
    }
    while (character != '\r');

    lcd_clear();

    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    lcd_write_string("Program Done");
}