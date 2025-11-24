//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  John Marcello
//
//       LAB NAME:  Lab 10: Serial Communication Interface 
//
//      FILE NAME:  Lab10_p1_main.c
//
//  (first thing for part 2 is UART_write_string)                                   <-----------------------
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
#define buffer_size 10
#define buffer_size_max 11
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
  
  //run_lab_uarttest(); 
  

 // Endless loop to prevent program from ending
 while (1);

} /* main */

char character;

void run_lab_uarttest (void)
{
    char buffer[buffer_size];
    uint8_t position_idx = 0;

    do
    {
        character = UART_in_char();

        if(character == '\r')
        {
            buffer[position_idx] = NULL;
            UART_out_char(character);
        }

        else if (character == '\b' && position_idx !=0) 
        {
            position_idx--;
            UART_out_char(character);
        }

        else
        {
            if (position_idx < buffer_size_max)
            {
                buffer[position_idx] = character;
                UART_out_char(character);
                position_idx++;
            }
        }
        
    }
    while (character != '\r');

    lcd_clear();

    lcd_set_ddram_addr(LCD_LINE1_ADDR);
    lcd_write_string("Name:");
    lcd_set_ddram_addr(LCD_LINE1_ADDR + LCD_CHAR_POSITION_6);
    lcd_write_string(buffer);
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    lcd_write_string("Program Done");
}