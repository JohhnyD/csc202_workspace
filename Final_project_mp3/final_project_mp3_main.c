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
#define SERIAL 1
#define REPEAT_SONG 2
#define RANDOM_SONG 3
#define VOLUME 2
#define END_PROGRAM 3
#define BUFFER_SIZE 1
#define MAX_BUFFER_LENGTH 2
#define NEWLINE "\r\n"
#define JOYSTICK_CHANNEL 7
#define JOYSTICK_UP 2600
#define JOYSTICK_DOWN 2400
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
    uint8_t menu_option = 0;
    uint8_t index    = 0;
    char buffer[MAX_BUFFER_LENGTH];
    bool done = false;

    lcd_clear(); 
    leds_disable();
    uint16_t ADC_value;

    while(!done)
        {
            ADC_value = ADC0_in(JOYSTICK_CHANNEL);
            
            if (ADC_value/455 < JOYSTICK_DOWN)
            {
                menu_option++;
            }

            if (ADC_value/455 > JOYSTICK_UP)
            {
                menu_option--;
            }

        }
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

