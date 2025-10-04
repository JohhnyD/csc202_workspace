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
//    This code uses the input devices on the launchpad development board.
//    The two input devices that are mainly used is the pushbuttons on the
//    launchpad and the launchpad development board. In addition, the keypad
//    is also used for inputs from a user.
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
void run_lab5_part2(void);
void run_lab5_part3(void);
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
  dipsw_init();
  seg7_init();
  lpsw_init();
  keypad_init();
  leds_init();
//Part 1 main
//-----------------------------------------------------------------------------
run_lab5_part1();
msec_delay(500);

//Part 2 main
//-----------------------------------------------------------------------------
run_lab5_part2();
msec_delay(500);

//Part 3 main
//-----------------------------------------------------------------------------
leds_enable();
//run_lab5_part3();
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
        bool display_is_on = false;
        int loopcntr1 = 0;
        seg7_off();
        while (loopcntr1 < 6)
        {
            if(is_pb_down(PB1_IDX))
            {
                if(display_is_on)
                {
                    seg7_off();
                    display_is_on = false;
                    loopcntr1++;
                        while(is_pb_down(PB1_IDX));
                        msec_delay(10);
                }
            else 
            {
                seg7_on(NUMBER3, SEG7_DIG0_ENABLE_IDX);
                display_is_on = true;
                loopcntr1++;
                msec_delay(10);
                while(is_pb_down(PB1_IDX))
                {
                    msec_delay(10);
                }
            }
            }
        }
        seg7_off();
        leds_off();
    }
//-----------------------------------------------------------------------------
//Part 2 Function:
//
  void run_lab5_part2(void)
    {
        typedef enum
        {
            GET_LOW,
            GET_HIGH,
            DISPLAY
        } dpswstate;

        dpswstate current_state;
        current_state = GET_LOW;
        uint8_t low_nibble = 0;
        uint8_t high_nibble = 0;
        uint8_t seg7_value = 0;
        uint8_t Display_value = 0;
        uint8_t loopcntr2 = 0;
        
        while(loopcntr2 < 3)
        {
            switch (current_state)
            {
                case (GET_LOW):
                low_nibble = dipsw_read();
                if(is_lpsw_down(LP_SW2_PORT))
                {
                    current_state = GET_HIGH;
                    msec_delay(10);
                    while (is_lpsw_down(LP_SW2_PORT))
                    msec_delay(10);
                }
            break;

            case (GET_HIGH):
                high_nibble = dipsw_read();
                if(is_lpsw_down(LP_SW2_PORT))
                {
                    current_state = DISPLAY;
                    msec_delay(10);
                    while (is_lpsw_down(LP_SW2_PORT))
                    msec_delay(10);
                }
            break;

            case DISPLAY:
                Display_value = ((high_nibble << 4));
                if (is_pb_down(PB1_IDX))
                {
                    seg7_on(Display_value, SEG7_DIG2_ENABLE_IDX);
                    msec_delay(10);
                    while (is_pb_down(PB1_IDX));
                    msec_delay(10);
                }
                else
                {
                    seg7_on(Display_value, SEG7_DIG0_ENABLE_IDX);
                }

                if(is_lpsw_down(LP_SW2_PORT))
                {
                    current_state = GET_LOW;
                    seg7_off();
                    loopcntr2++;
                    msec_delay(10);
                    while(is_lpsw_down(LP_SW2_PORT));
                    msec_delay(10);
                }
                break;
            }
        }
        
    }

//Part 3 Function:
//
void run_lab5_part3(void)
{
    
}

//FOR WHEN YOU PICK UP TOMORROW:
//Basics for this lab is creating flags in order to keep track of the states of the push button and switches (dipswitch)
//Things to remember:
//DIPswitch will be from msb - lsb so the first switch on the right is actually the LSB
//Need to remember what the flags do and do if else statments to know what the code should do
//ex: if If pushbutton 1 is not pressed ---> turn display on \\ else if pushbutton is pressed --->
//turn display off (unless if a number needs to be displayed)
//use bounce delay for switches because there is some interferance when the pushbutton is pressed that we need to account for
//