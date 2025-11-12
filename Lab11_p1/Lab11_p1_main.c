//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  John Marcello
//
//       LAB NAME:  Lab 11: Serial Peripheral Interface
//
//      FILE NAME:  Lab11_p1_main
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
#include "spi.h"
#include "lcd1602.h"
#include "uart.h"


//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void UART_output_string(const char *string);
void run_lab11_p1 (void);
void display_menu (void);
//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define BUFFER_SIZE 1
#define MAX_BUFFER_LENGTH 2
#define NEWLINE "\r\n"
#define SET_DATA_TO_SEND '1'
#define SEND_DATA '2'
#define UPDATE_LEDS '3'
#define END_PROGRAM '4'

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
  spi1_init();

  run_lab11_p1();
  
  // Endless loop to prevent program from ending
  while (1);

} /* main */

void run_lab11_p1 (void)
{
    char menu_option = 0;
    uint8_t index    = 0;
    char buffer[MAX_BUFFER_LENGTH];
    bool done = false;

    lcd_clear();

    //inc_seg7(false);  
    leds_disable();

    do
        {
            display_menu();
            menu_option = UART_in_char();
            UART_out_char(menu_option);

            switch (menu_option)
            {
                case SET_DATA_TO_SEND:
                    UART_output_string(NEWLINE);
                    UART_output_string("\nSet Data to Send Menu Selected");
                    //inc_seg7(true);
                    UART_output_string(NEWLINE);
                    UART_output_string(NEWLINE);

                    break;

                case SEND_DATA:
                    UART_output_string(NEWLINE);
                    UART_output_string("\nSend Data Menu Selected");
                    //temp_reading();
                    UART_output_string(NEWLINE);
                    UART_output_string(NEWLINE);

                    break;

                case UPDATE_LEDS:
                    UART_output_string(NEWLINE);
                    UART_output_string("\nUpdate LEDs Menu Selected");
                    //seg7_off();
                    //leds_flash();
                    //inc_seg7(false);
                    UART_output_string(NEWLINE);
                    UART_output_string(NEWLINE);

                    break;
                
                case END_PROGRAM:
                    UART_output_string(NEWLINE);
                    UART_output_string("Thank you for using the program!");
                    lcd_clear();
                    leds_off();
                    seg7_off();
                    lcd_write_string("Program Stopped");
                    done = true;
                
                break;
            }
        }
    while(!done);
}

void display_menu (void)
{
    UART_output_string("MENU OPTIONS\r\n");
    UART_output_string("  1. Set Data to Send\r\n");
    UART_output_string("  2. Send Data\r\n");
    UART_output_string("  3. Update LEDs\r\n");
    UART_output_string("  4. End Program\r\n");
    UART_output_string("Enter your selection:");
}

void UART_output_string(const char *string)
{
    // for each character in string, write it to the UART module
    while (*string != '\0')
    {
        UART_out_char(*string++);
    } /* while */

} /* UART_write_string*/