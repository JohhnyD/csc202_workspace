//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  John Marcello   
//
//       LAB NAME:  Lab 5:  Interfacing to Input Device
//
//      FILE NAME:  Lab5_main.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This project runs on the LP_MSPM0G3507 LaunchPad board interfacing to
//    the CSC202 Expansion board.
//
//    This code...
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
void run_lab5_part1(void);
//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
   #define NUMBER3 0x79
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

dipsw_init();
seg7_init();
run_lab5_part1();

// Endless loop to prevent program from ending
 while (1);
} /* main */ 

// End of main
//-----------------------------------------------------------------------------
 
 // Created functions for each part

//-----------------------------------------------------------------------------
//Part 1 function: This function will detect if push button 1 is pushed and when so will display a 3 on the 7-segment display.
//When it is pressed again it turns off the 7-segment display
    void run_lab5_part1(void)
    {
        int loopcntr = 0;
        while (loopcntr < 3)
        {
            if (is_pb_down(PB1_IDX))
            {
                msec_delay(150);
                seg7_off();
                loopcntr++;
            }
                else
                {
                 msec_delay(150);
                seg7_on(NUMBER3, SEG7_DIG0_ENABLE_IDX);
                }
        }
    }

//-----------------------------------------------------------------------------


//FOR WHEN YOU PICK UP TOMORROW:
//Basics for this lab is creating flags in order to keep track of the states of the push button and switches (dipswitch)
//Things to remember:
//DIPswitch will be from msb - lsb so the first switch on the right is actually the LSB
//Need to remember what the flags do and do if else statments to know what the code should do
//ex: if If pushbutton 1 is not pressed ---> turn display on \\ else if pushbutton is pressed --->
//turn display off (unless if a number needs to be displayed)
//use bounce delay for switches because there is some interferance when the pushbutton is pressed that we need to account for
//