//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  TBD
//
//       LAB NAME:  TBD
//
//      FILE NAME:  main.c
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
void run_lab10_p2 (void);
void temp_reading (void);
void display_menu (void);
void UART_output_string(const char *string);
void inc_seg7 (bool enable_inc);
void leds_flash(void);
//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define BUFFER_SIZE 1
#define MAX_BUFFER_LENGTH 2
#define temperaturechannel 5
#define NEWLINE "\r\n"
#define SEG7_MENU_ITEM '1'
#define TEMP_MENU_ITEM '2'
#define LEDS_MENU_ITEM '3'
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
  leds_init();
  leds_enable();
  seg7_init();
  ADC0_init(ADC12_MEMCTL_VRSEL_VDDA_VSSA);
 
  run_lab10_p2();
  // enter your code here

 
 // Endless loop to prevent program from ending
 while (1);

} /* main */

//-----------------------------------------------------------------------------
// run_lab10_p2 function: This function defines the structure for a menu that
// is displayed in the serial console in CCS. It creates a menu in order for
// a user to select what action they want to perform. The three actions being
// incramenting a number on the 7-segment display, showing the current temp
// in the room, and the final main option is flashing the leds on the led-bar.
// The last option is to end the program. Menu options can be executed by
// the user typing numbers 1-4.
//-----------------------------------------------------------------------------

void run_lab10_p2 (void)
{
    char menu_option = 0;
    uint8_t index    = 0;
    char buffer[MAX_BUFFER_LENGTH];
    bool done = false;

    lcd_clear();

    inc_seg7(false);  
    leds_disable();

    do
        {
            display_menu();
            menu_option = UART_in_char();
            UART_out_char(menu_option);

            switch (menu_option)
            {
                case SEG7_MENU_ITEM:
                    UART_output_string(NEWLINE);
                    UART_output_string("\nIncrament Seven Segment Menu Selected");
                    inc_seg7(true);
                    UART_output_string(NEWLINE);
                    UART_output_string(NEWLINE);

                    break;

                case TEMP_MENU_ITEM:
                    UART_output_string(NEWLINE);
                    UART_output_string("\nDisplay Temprature Menu Selected");
                    temp_reading();
                    UART_output_string(NEWLINE);
                    UART_output_string(NEWLINE);

                    break;

                case LEDS_MENU_ITEM:
                    UART_output_string(NEWLINE);
                    UART_output_string("\nFlash LEDs Menu Selected");
                    seg7_off();
                    leds_flash();
                    inc_seg7(false);
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

// This is the main menu that is displayed in the serial console

void display_menu (void)
{
    UART_output_string("MENU OPTIONS\r\n");
    UART_output_string("  1. Incrament count on 7-segment display\r\n");
    UART_output_string("  2. Show Current Temperature\r\n");
    UART_output_string("  3. Flash LEDs 3 times\r\n");
    UART_output_string("  4. End Program\r\n");
    UART_output_string("Enter your selection:");
}

// This code will incrament the number on the 7-segment display and
// will start on 1 and add up to 9. Then reseting to 0 and restarting
// the count

void inc_seg7 (bool enable_inc)
{
    static uint8_t seg7_value = 0;

    if (enable_inc == true)
    {
        if (seg7_value == 9)
        {
             seg7_value = 0;
        }  
        else
            seg7_value++;
    }
    seg7_hex(seg7_value ,SEG7_DIG3_ENABLE_IDX);
}

// This function will read the current temperature in the room and
// display it on the LCD display

void temp_reading (void)
{
    lcd_clear();

    lcd_set_ddram_addr(LCD_LINE1_ADDR);
    lcd_write_string ("Temp = ");
    lcd_set_ddram_addr(LCD_LINE1_ADDR+ LCD_CHAR_POSITION_12);
    lcd_write_string("F");
    lcd_set_ddram_addr(LCD_LINE1_ADDR+ LCD_CHAR_POSITION_11);
    lcd_write_char (223);

    uint32_t adc_result = 0;
    uint16_t celcius_reading = 0;

           
    adc_result = ADC0_in(temperaturechannel);

    celcius_reading = (uint16_t) thermistor_calc_temperature(adc_result);

    lcd_set_ddram_addr(LCD_LINE1_ADDR + LCD_CHAR_POSITION_6);
    lcd_write_doublebyte(celcius_reading*9/5+32);
}

// This function flashes the leds on the ledbar while not flashing the 
// 7-segment display

void leds_flash(void)
{
    leds_off();
    leds_enable();
    uint8_t loopcntr = 0;

    while (loopcntr < 4)
    {
        leds_on(0xFF);
        msec_delay(800);
        leds_off();
        msec_delay(800);
       
        loopcntr++;
    }
    leds_disable();
}

// This function will output characters in string format in order
// to display text in the serial console.

void UART_output_string(const char *string)
{
    // for each character in string, write it to the UART module
    while (*string != '\0')
    {
        UART_out_char(*string++);
    } /* while */

} /* UART_write_string*/
