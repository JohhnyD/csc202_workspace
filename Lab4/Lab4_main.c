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
//    This code tests the use of the LED bar and digital led components on the board
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

 //Part 1 function: Turns led on bar based on the value inputed
    void run_lab4_part1(void)
    {
       leds_on(0x3C);    //turns leds on 7 segment display from bit 5-2 
       void leds_off(void);
       msec_delay(1000);
    }

//Part_2 function:

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
    
}
//Part_3 function:
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
 
 void run_lab4_part4(void)
 {
    seg7_on(LETTERL ,SEG7_DIG0_ENABLE_IDX);
    msec_delay(5000);
    leds_off();
    msec_delay(500);
    
 }

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