//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  John Marcello   
//
//       LAB NAME:  Lab 4: Interfacing to Output Device
//
//      FILE NAME:  Lab4_main.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This project runs on the LP_MSPM0G3507 LaunchPad board interfacing to
//    the CSC202 Expansion board.
//
//    This code tests the use of the LED bar and 7-segment display on the launchpad development board.
//    By doing so we can better understand and display bit numbers and other information on the LEDS. In addition,
//    undertand how GPIO pins and the MUX works on the board.
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


//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void run_lab4_part1(void);
void run_lab4_part2(void);
void run_lab4_part3(void);
void run_lab4_part4(void);
void run_lab4_part5(void);
void run_lab4_part6(void);
//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
    #define LETTERL 0x38
    #define NUMBER4 0x66
    #define LETTERC 0x39    
    #define LETTERA 0x77
    #define LETTERF 0x71
    #define LETTERE 0x79
//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------


// Define a structure to hold different data types
//-----------------------------------------------------------------------------

// Main Code
//-----------------------------------------------------------------------------

int main(void)
{
  // Configure the LaunchPad board
  clock_init_40mhz();
  launchpad_gpio_init();

//Part 1 main
//-----------------------------------------------------------------------------
  leds_init();
  leds_enable();
  run_lab4_part1();
//-----------------------------------------------------------------------------
//Part2 main
//-----------------------------------------------------------------------------
  leds_init();
  leds_enable();
  run_lab4_part2();
  msec_delay(500);
//-----------------------------------------------------------------------------
//Part 3 main
//-----------------------------------------------------------------------------
  leds_init();
  leds_enable();
  run_lab4_part3();
  msec_delay(500);
  leds_off();
  leds_disable();
//-----------------------------------------------------------------------------  
//Part 4 main 
//-----------------------------------------------------------------------------
  seg7_init();
  run_lab4_part4();
  msec_delay(500);
//-----------------------------------------------------------------------------
//Part 5 main
//-----------------------------------------------------------------------------
  seg7_init();
  run_lab4_part5();
  msec_delay(500);
//-----------------------------------------------------------------------------
//Part 6 main
//-----------------------------------------------------------------------------
  seg7_init();
  run_lab4_part6();
  leds_off();
//-----------------------------------------------------------------------------
 
// Endless loop to prevent program from ending
 while (1);
} /* main */ 

// End of main
//-----------------------------------------------------------------------------
 
 // Created functions for each part

//-----------------------------------------------------------------------------
//Part 1 function: Turns led on bar based on the value inputed
    void run_lab4_part1(void)
    {
       leds_on(0x3C);    //turns leds on 7 segment display from bit 5-2 
       void leds_off(void);
       msec_delay(1000);
    }

//-----------------------------------------------------------------------------
//Part_2 function:
// This function will flash the leds on the ledbar from ledindex0 to ledindex7 (0-7 bits) and then loop for a total of 5 times
// with a 0.2 millisecond delay between each step

 void run_lab4_part2(void) 
{
    int loop_counter = 0;
    while (loop_counter < 5)
    {
        for (uint8_t led_idx = LED_BAR_LD0_IDX; led_idx <= LED_BAR_LD7_IDX; led_idx++)
        {
            led_on(led_idx);
            msec_delay(200);
            led_off(led_idx); 
        }
    for (uint8_t led_idx2 = LED_BAR_LD6_IDX; led_idx2 >= LED_BAR_LD1_IDX; led_idx2--)
    {
        led_on(led_idx2);
        msec_delay(200);
        led_off(led_idx2);
    }
    loop_counter++;
    }
    led_on(LED_BAR_LD0_IDX);
    msec_delay(200);
    led_off(LED_BAR_LD0_IDX);
}
//-----------------------------------------------------------------------------
//Part_3 function:
//This function displays a bit counter on the led bar graph that goes through all iterations of an 8-bit unsigned integer.
//It is then put in a loop to repeat the sequence two times

 void run_lab4_part3(void)
 {
    uint8_t loop_counterp3 = 0;
    uint8_t led_counter = 0;

    while (loop_counterp3 < 2)
    {
            if (led_counter ==255)
            {
                loop_counterp3++;
            }
        leds_on(led_counter++);
        msec_delay(100);
    }
 }
 
//-----------------------------------------------------------------------------
//Part 4 function:
//This function displays the letter L on the 7 segment display on the launchpad development board.
//To achieve this i created a symbolic constant for the correct 8-bit integer that would display the 
//letter L on the 7 segement display
 void run_lab4_part4(void)
 {
    seg7_on(LETTERL ,SEG7_DIG0_ENABLE_IDX);
    msec_delay(5000);
    leds_off();
    msec_delay(500);
    
 }

//-----------------------------------------------------------------------------
//Part 5 function
//This function displays the number 4 on the 7 segment display 4 times. To achieve this I made another symbolic constant
//that would display the number 4 and then it is put inside of a loop in order to repeat displaying the number 4 four times.
 void run_lab4_part5(void)
 {
    uint8_t loop_counterp5 = 0;

     while (loop_counterp5 < 4)
    {
            seg7_on(NUMBER4, SEG7_DIG2_ENABLE_IDX);
            msec_delay(3000);
            leds_off();
            msec_delay(2000);
            loop_counterp5++;
    }
 }

//-----------------------------------------------------------------------------
//Part 6 function:
//This function displays the word cafe for around 5 seconds. This is done by making symbolic constants for the letters C A F E and figuring
//what binary number coresponds to the 7 segment display that would display these letters. It is then put in a while loop in order to flash
//the 7-segment display really fast so all of the letters can be displayed at once. The reason you can't turn all the 7-segment digits on at
//once is because only one DIG can be turned on at a time, however if you turn them on and off fast enough you will not see the DIG technically
//turn off.
 void run_lab4_part6(void)
 {
    uint8_t loop_counterp6 = 0;

    while (loop_counterp6 <= 200)
    {
           seg7_on(LETTERC, SEG7_DIG0_ENABLE_IDX);
           msec_delay(5);
           seg7_on(LETTERA, SEG7_DIG1_ENABLE_IDX);
           msec_delay(5);
           seg7_on(LETTERF, SEG7_DIG2_ENABLE_IDX);
           msec_delay(5);
           seg7_on(LETTERE, SEG7_DIG3_ENABLE_IDX);
           msec_delay(5);
           loop_counterp6++;
    }
 }

//-----------------------------------------------------------------------------