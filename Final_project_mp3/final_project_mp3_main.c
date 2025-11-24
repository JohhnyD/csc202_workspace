//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  John & Vinny
//
//       LAB NAME:  Final Project
//
//      FILE NAME:  final_project_mp3_main.c
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
#include "demo_melodies.h"
#include "pitches.h"
#include <stdint.h>

//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void run_final_project (void);
void display_menu (void);
void UART_output_string(const char *string);
//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define SERIAL '1'
#define REPEAT_SONG '2'
#define RANDOM_SONG '3'
#define VOLUME '4'
#define END_PROGRAM '5'
#define BUFFER_SIZE 1
#define MAX_BUFFER_LENGTH 2
#define NEWLINE "\r\n"
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
  leds_init();
  leds_enable();
  seg7_init();
  ADC0_init(ADC12_MEMCTL_VRSEL_VDDA_VSSA);
  motor0_init();
  motor0_pwm_init(200000/50, 2000);
  
  run_final_project();

 while (1);

} /* main */

void run_final_project (void)
{
    char menu_option = 0;
    uint8_t index    = 0;
    char buffer[MAX_BUFFER_LENGTH];
    bool done = false;

    lcd_clear(); 
    leds_disable();

    do
        {
            display_menu();
            menu_option = UART_in_char();
            UART_out_char(menu_option);

            switch (menu_option)
            {
                case SERIAL:
                    UART_output_string(NEWLINE);
                    UART_output_string("\nSerial Menu Selected");
                    //funtion here:
                    UART_output_string(NEWLINE);
                    UART_output_string(NEWLINE);

                    break;

                case REPEAT_SONG:
                    UART_output_string(NEWLINE);
                    UART_output_string("\nRepeat Song Menu Selected");
                    //funtion here:
                    UART_output_string(NEWLINE);
                    UART_output_string(NEWLINE);

                    break;

                case RANDOM_SONG:
                    UART_output_string(NEWLINE);
                    UART_output_string("\nRandom Song Menu Selected");
                    //funtion here:
                    UART_output_string(NEWLINE);
                    UART_output_string(NEWLINE);

                    break;
                
                case VOLUME:
                    UART_output_string(NEWLINE);
                    UART_output_string("\nVolume Chnage Menu Selected");
                    //funtion here:
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
    UART_output_string("  1. Serial (Song Selection)\r\n");
    UART_output_string("  2. Repeat Song\r\n");
    UART_output_string("  3. Play Random Song\r\n");
    UART_output_string("  4. Volume Change\r\n");
    UART_output_string("  5. End Program\r\n");
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