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
//    This code expands on knowledge that we learned about the lcd display
//    in the previous lab and also adds the use of an intrrupt. The intrrupt
//    allows the program to run multiple tasks at once without interfering
//    with the main code
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
void run_Lab7_p1 (void);
void run_Lab7_p2 (void);
void wait_for_pb_release (uint8_t button);
void wait_for_pb_pressed (uint8_t button);
void lcd_write_string_window(const char *string, uint8_t start_lcd_addr,uint8_t max_lcd_addr);
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
  dipsw_init();
  I2C_init();
  lcd1602_init();



  sys_tick_init(SYST_TICK_PERIOD_COUNT);
  run_Lab7_p1();
  run_Lab7_p2();
  sys_tick_disable();
  seg7_off();

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

//-----------------------------------------------------------------------------
// FPart 1 function: displays a scrolling message on the lcd screen
//-----------------------------------------------------------------------------

void run_Lab7_p1 (void)
{
    bool done = false;
    char message[] = "Microcontrollers are fun.";
    uint8_t LCD_Adress;

    while (!done)
    {

        for (LCD_Adress = 0x4F; (LCD_Adress > 0x40)&&(!done); LCD_Adress--)
        {
            lcd_clear();
            lcd_set_ddram_addr(LCD_Adress);
            lcd_write_string(message);
            msec_delay(200);

            if (is_pb_down(PB1_IDX))
            {
            done = true;
            }
        }

        uint8_t index = 0;
 
        while ((message [index] != '\0') &&(!done))
        {
            lcd_clear();
            lcd_set_ddram_addr(0x40);
            lcd_write_string(message + index);
            index++;
            msec_delay(200);

            if (is_pb_down(PB1_IDX))
            {
            done = true;
            }
            
        }
    }
    lcd_clear();
    lcd_write_string("Part 1 Done");
    msec_delay(2000);
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    lcd_write_string("Press PB2");
    wait_for_pb_pressed(PB2_IDX);
    lcd_clear();
    wait_for_pb_release(PB2_IDX);
    lcd_write_string("Start Part 2");
    msec_delay(2000);
    lcd_clear();
}

//-----------------------------------------------------------------------------
// Part 2 function: displays a scrolling message on the lcd screen that does
// not wrap to the first line on the LCD display
//-----------------------------------------------------------------------------

void run_Lab7_p2 (void)
{
    bool done = false;
    char message[] = "Microcontrollers are fun. I love programming in MSPM0+ assembly code!!!";
    uint8_t LCD_Adress;

    while (!done)
    {

        for (LCD_Adress = 0x4F; (LCD_Adress > 0x40)&&(!done); LCD_Adress--)
        {
            lcd_clear();
            lcd_set_ddram_addr(LCD_Adress);
            lcd_write_string_window(message,LCD_Adress,0x50);
            msec_delay(200);

            if (is_pb_down(PB1_IDX))
            {
            done = true;
            }
        }
        uint8_t index = 0;
 
        while ((message [index] != '\0') &&(!done))
        {
            lcd_clear();
            lcd_set_ddram_addr(0x40);
            lcd_write_string_window(message + index,0x40,0x50 );
            index++;
            msec_delay(200);

            if (is_pb_down(PB1_IDX))
            {
            done = true;
            }
            
        }
    }
    lcd_clear();
    lcd_write_string("Part 2 Done");
    msec_delay(2000);
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    lcd_write_string("Press PB2");
    wait_for_pb_pressed(PB2_IDX);
    lcd_clear();
    wait_for_pb_release(PB2_IDX);
    lcd_write_string("Start Part 3");
    msec_delay(2000);
    lcd_clear();
    
    }

//-----------------------------------------------------------------------------
//Pushbutton wait for pushbutton press function:
//-----------------------------------------------------------------------------

void wait_for_pb_pressed (uint8_t button)
{
    while (is_pb_up(button));
    msec_delay(10);
}

//-----------------------------------------------------------------------------
//Pushbutton wait for pushbutton release function:
//-----------------------------------------------------------------------------

void wait_for_pb_release (uint8_t button)
{
    while (is_pb_down(button));
    msec_delay(10);
}

//-----------------------------------------------------------------------------
//  Function for lcd_write_string_window: This function sets a specific
//  window on the lcd (line 2) so that a string is only displayed on the bottom
//  row of the lcd and does not wrap to the top row
//-----------------------------------------------------------------------------

void lcd_write_string_window(const char *string, uint8_t start_lcd_addr,uint8_t max_lcd_addr)
{
    uint8_t counter = start_lcd_addr;

    lcd_set_ddram_addr(start_lcd_addr);
    // for each character in string, write it to the LCD module
    while (*string != '\0' && max_lcd_addr > counter )
    {
        lcd_write_char(*string++);
        counter++;
    } /* while */

} /* lcd_write_string */