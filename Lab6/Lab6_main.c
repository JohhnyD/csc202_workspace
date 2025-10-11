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
//    This code introduces the use of the lcd display in order to display
//    integers, characters, a bit counter, and integrated use with a keypad
//    By using these we can better understand how the use of an lcd display
//    is beneficial at displaying information at a lower power consumption
//    cost
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

void wait_for_pb_release (uint8_t button);
void wait_for_pb_pressed (uint8_t button);
void run_lab6_p1 (void);
void run_lab6_p2 (void);
void run_lab6_p3 (void);
void run_lab6_p4 (void);

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
  dipsw_init();
  
  //Run lab part 1:

  lcd_write_string("Start Lab Part 1");
  msec_delay(2000);
  lcd_clear();
  run_lab6_p1();
  wait_for_pb_pressed(PB2_IDX);
  lcd_clear();
  lcd_write_string("Part 1 done");
  wait_for_pb_release(PB2_IDX);
  msec_delay(2000);
  lcd_set_ddram_addr(LCD_LINE2_ADDR);
  lcd_write_string("Press PB2");
  wait_for_pb_pressed(PB2_IDX);
  lcd_clear();
  wait_for_pb_release(PB2_IDX);
  lcd_clear();
  lcd_write_string("Start Lab Part 2");
  msec_delay(2000);
  lcd_clear();
  run_lab6_p2();
  lcd_set_ddram_addr(LCD_LINE2_ADDR);
  lcd_write_string("Press PB2");
  wait_for_pb_pressed(PB2_IDX);
  lcd_clear();
  wait_for_pb_release(PB2_IDX);
  lcd_write_string("Start Lab Part 3");
  msec_delay(2000);
  lcd_clear();
  run_lab6_p3();
  lcd_clear();
  lcd_write_string("Part 3 Done");
  msec_delay(2000);
  lcd_set_ddram_addr(LCD_LINE2_ADDR);
  lcd_write_string("Press PB2");
  wait_for_pb_pressed(PB2_IDX);
  lcd_clear();
  wait_for_pb_release(PB2_IDX);
  lcd_clear();
  lcd_write_string("Start Lab Part 4");
  msec_delay(2000);
  lcd_clear();
  run_lab6_p4();
  lcd_clear();
  lcd_write_string("Program Stopped");

 // Endless loop to prevent program from ending
 while (1);

} /* main */

//-----------------------------------------------------------------------------
//Run lab part 1 code and function:
//  Individually displays the alphabet based of of the Ascii code that
//  the lcd display can display
//  (I created an offset in order to let the compiler do the work for knowing
//   what ascii characters to display)
//-----------------------------------------------------------------------------

void run_lab6_p1 (void)
{
    uint8_t offset;

    for (offset = 'A'; offset <= 'Z'; offset++)
    {
        lcd_write_char(offset);
        if (offset == 'P')
        {
            lcd_set_ddram_addr(LCD_LINE2_ADDR);
        }
    }
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
//Run lab part 2 code and function: This function will display a 32 bit number,
// 16 bit number on line 2 of the lcd, and 8 bit number on line 1 while being  
// center definded. The user presees pushbutton 1 in order to display the next
// bit to be displayed
//-----------------------------------------------------------------------------

void run_lab6_p2 (void)
{
    uint32_t value_32bit = 1234567890;
    uint16_t value_16bit = 1234;
    uint8_t value_8bit = 123;

    lcd_set_ddram_addr(LCD_LINE1_ADDR + LCD_CHAR_POSITION_4);
    lcd_write_quadbyte(value_32bit);
    wait_for_pb_pressed(PB1_IDX);
    wait_for_pb_release(PB1_IDX);
    msec_delay(10);
    lcd_set_ddram_addr(LCD_LINE2_ADDR + LCD_CHAR_POSITION_6);
    lcd_write_doublebyte(value_16bit);
    wait_for_pb_pressed(PB1_IDX);
    lcd_clear();
    wait_for_pb_release(PB1_IDX);
    lcd_set_ddram_addr(LCD_LINE1_ADDR + LCD_CHAR_POSITION_8);
    lcd_write_byte(value_8bit);
    wait_for_pb_pressed(PB2_IDX);
    lcd_clear();
    lcd_write_string("Part 2 Done");
    wait_for_pb_release(PB2_IDX);
    msec_delay(2000);
}

//-----------------------------------------------------------------------------
//Run lab part 3 code and function: 
//  Creates a bit counter that counts from 100 down to 0 and then resets.
//  If the user pushes push button 1, it resets to 100 and if the user pushes 
//  pushbutton 2, it ends the program.
//-----------------------------------------------------------------------------

void run_lab6_p3 (void)
{
    uint8_t loop_counter = 0;
    uint8_t value_100counter = 100;
    bool done = false;
    
    

    while (!done)
    {
        lcd_set_ddram_addr(LCD_LINE1_ADDR + LCD_CHAR_POSITION_8);
        lcd_write_byte(value_100counter);
        msec_delay(200);
        
        if (value_100counter == 0)
        {
            value_100counter = 100;
        }
            else 
            {
                value_100counter--;
            }
        
        if (is_pb_down(PB2_IDX))
        {
            done = true;
        }

        if (is_pb_down(PB1_IDX))
        {
            value_100counter = 100;
        }
    }
}

//-----------------------------------------------------------------------------
//Run lab part 4 code and function: 
//  This function will detect when a key press is done on the keypad and
//  displays the character/integer on the lcd screen. 
//  It starts on line 1 and when its filled up goes to line 2 on the lcd.
//  This is done by using the hex to lcd function which takes the keypads 
//  hex value when a key is pressed and dispays it on the lcd. 
//  If pushbutton 1 is pressed is clears and resets the lcd. 
//  If pushbutton 2 is pressed it completes the program.
//-----------------------------------------------------------------------------

void run_lab6_p4 (void)
{
    uint8_t key_value = 0;
    uint8_t key_press_count = 0;
    bool done = false;

    while (!done) 
    {
        do 
        {
            key_value = keypad_scan();
        } 
        while (key_value == 0x10 && is_pb_up(PB2_IDX) && is_pb_up(PB1_IDX));

        key_press_count++;
        hex_to_lcd(key_value);
        msec_delay(10);
        wait_no_key_pressed();
        msec_delay(10);

        if (key_press_count == 16)
        {
            lcd_set_ddram_addr(LCD_LINE2_ADDR);
        }

        if (key_press_count == 33)
        {
            lcd_clear();
            key_press_count = 0;
            hex_to_lcd(key_value);
            key_press_count++;
            msec_delay(10);
            wait_no_key_pressed();
            msec_delay(10);
        }

        if (is_pb_down(PB2_IDX))
        {
            msec_delay(10);
            done = true;
        }

        if (is_pb_down(PB1_IDX))
        {
            lcd_clear();
            msec_delay(500);
            key_press_count = 0;
        }
        

    }

}