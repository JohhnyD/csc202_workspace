//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  John Marcello
//
//       LAB NAME:  Lab 9: Pulse Width Modulation
//
//      FILE NAME:  Lab9_p1_main.c
//
//-----------------------------------------------------------------------------
//
// DESCRIPTION:
//    This project runs on the LP_MSPM0G3507 LaunchPad board interfacing to
//    the CSC202 Expansion board.
//
//    This code tests the use of the passive buzzer and active buzzer. Mainly 
//    the code will show how to enable a specific buzzer or both on the 
//    launchpad development board. It also uses the LCD screen in order to
//    display what state the statemachine is in and on the second line
//    displays what frequency pwm is at and what the DC% is based off
//    of a user pressing a key on the keypad. Finally this code also uses
//    the use of the LED_BAR in order to show what buzzers are enabled. 
//    In addition how changing the dc% can affect how loud the buzzeers are
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
#include "adc.h"
#include "clock.h"
#include "LaunchPad.h"
#include "demo_melodies.h"
#include "lcd1602.h"
#include "ti/devices/msp/m0p/mspm0g350x.h"
#include "ti/devices/msp/peripherals/hw_adc12.h"
#include "ti/devices/msp/peripherals/hw_gpio.h"
#include "ti/devices/msp/peripherals/m0p/hw_cpuss.h"
#include <stdint.h>
#include "pitches.h"

//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void config_pb1_interrupt (void);
void config_pb2_interrupt (void);
void run_Lab9_p1 (void);
void play_note(uint16_t freq, uint16_t duration_ms);
//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define disable_active_buzzer
#define motor0_set_freq 50
//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------
bool g_pb1_pressed = false;
bool g_pb2_pressed = false;

// Define a structure to hold different data types

int main(void)
{
  // Configure the LaunchPad board
  clock_init_40mhz();
  launchpad_gpio_init();
  dipsw_init();
  leds_init();
  leds_enable();
  keypad_init();
  I2C_init();
  lcd1602_init();
  motor0_init();
  motor0_pwm_init(200000/50, 2000);
  
 
  // enter your code here
  config_pb2_interrupt();
  config_pb1_interrupt();
  motor0_pwm_enable();
  run_Lab9_p1();
 
 // Endless loop to prevent program from ending
 while (1);


} /* main */

//-----------------------------------------------------------------------------
// Run_lab9_p1 Function: This function defines a state machine in order to step
// through different states while using a passive buzzer and active buzzer.
// The first state is idle where both buzzers are idle and do not produce any
// sound. The second state enables the active buzzer and disables the passive
// buzzer. The third is the opposite where the passive buzzer is enabled while
// the active is disabled. Then both_buzzers as both of the buzzers active. The
// last state plays a melody while refering to the demo_melodies.h file. All
// states have a pushbutton interrupt in order to step through the state
// machine by pressing pushbutton 2. There is also a pushbutton 1 
// interrupt that exists out of the state machine and ends the program.
//-----------------------------------------------------------------------------

void run_Lab9_p1 (void)
{
typedef enum
  {
    BUZZER_IDLE = 0,
    ACTIVE_BUZZER,
    PASSIVE_BUZZER,
    BOTH_BUZZERS,
    PASSIVE_MELODY,
  } fsm_state_t;

  bool done = false;
  uint8_t DC_value = 0;
  uint8_t Keypad_value;

  fsm_state_t current_state;
  current_state = BUZZER_IDLE;

  while (!done)
  {
        switch(current_state)
        {
            case (BUZZER_IDLE):

                motor0_set_pwm_dc(DC_value);
                
                Keypad_value = keypad_scan();

                if(Keypad_value != 16)
                {
                    DC_value = ((Keypad_value*100)/15);
                    motor0_set_pwm_dc (DC_value);
                }
                lcd_set_ddram_addr(LCD_LINE1_ADDR);
                lcd_write_string("Buzzers Disabled");  
                lcd_set_ddram_addr(LCD_LINE2_ADDR);
                lcd_write_string("F=");
                lcd_set_ddram_addr(LCD_LINE2_ADDR + LCD_CHAR_POSITION_6);
                lcd_write_byte(motor0_set_freq);
                lcd_set_ddram_addr(LCD_LINE2_ADDR + LCD_CHAR_POSITION_10);
                lcd_write_string("DC=");
                lcd_set_ddram_addr(LCD_LINE2_ADDR + LCD_CHAR_POSITION_16);
                lcd_write_string("%");
                lcd_set_ddram_addr(LCD_LINE2_ADDR + LCD_CHAR_POSITION_13);
                lcd_write_byte(DC_value);
                led_on(LED_BAR_LD1_IDX);
                led_on(LED_BAR_LD2_IDX);
                

             if (g_pb2_pressed)
                {
                    lcd_set_ddram_addr(LCD_LINE1_ADDR);
                    lcd_write_string("                ");
                    g_pb2_pressed = false;
                    current_state = ACTIVE_BUZZER;
                }
            break;

            case (ACTIVE_BUZZER):
                Keypad_value = keypad_scan();

                if(Keypad_value != 16)
                {
                    DC_value = ((Keypad_value*100)/15);
                    motor0_set_pwm_dc (DC_value);
                }
                lcd_set_ddram_addr(LCD_LINE1_ADDR + LCD_CHAR_POSITION_2);
                lcd_write_string("Active Buzzer");
                lcd_set_ddram_addr(LCD_LINE2_ADDR);
                lcd_write_string("F=");
                lcd_set_ddram_addr(LCD_LINE2_ADDR + LCD_CHAR_POSITION_6);
                lcd_write_byte(motor0_set_freq);
                lcd_set_ddram_addr(LCD_LINE2_ADDR + LCD_CHAR_POSITION_16);
                lcd_write_string("%");
                lcd_set_ddram_addr(LCD_LINE2_ADDR + LCD_CHAR_POSITION_10);
                lcd_write_string("DC=");
                lcd_set_ddram_addr(LCD_LINE2_ADDR + LCD_CHAR_POSITION_13);
                lcd_write_byte(DC_value);
                led_on(LED_BAR_LD2_IDX);
                led_off(LED_BAR_LD1_IDX);
                if (g_pb2_pressed)
                {
                    lcd_set_ddram_addr(LCD_LINE1_ADDR);
                    lcd_write_string("                ");
                    g_pb2_pressed = false;
                    current_state = PASSIVE_BUZZER;
                }
            break;

            case (PASSIVE_BUZZER):
                Keypad_value = keypad_scan();

                if(Keypad_value != 16)
                {
                    DC_value = ((Keypad_value*100)/15);
                    motor0_set_pwm_dc (DC_value);
                }
                lcd_set_ddram_addr(LCD_LINE1_ADDR + LCD_CHAR_POSITION_2);
                lcd_write_string("Passive Buzzer");
                lcd_set_ddram_addr(LCD_LINE2_ADDR);
                lcd_write_string("F=");
                lcd_set_ddram_addr(LCD_LINE2_ADDR + LCD_CHAR_POSITION_6);
                lcd_write_byte(motor0_set_freq);
                lcd_set_ddram_addr(LCD_LINE2_ADDR + LCD_CHAR_POSITION_16);
                lcd_write_string("%");
                lcd_set_ddram_addr(LCD_LINE2_ADDR + LCD_CHAR_POSITION_10);
                lcd_write_string("DC=");
                lcd_set_ddram_addr(LCD_LINE2_ADDR + LCD_CHAR_POSITION_13);
                lcd_write_byte(DC_value);
                led_on(LED_BAR_LD1_IDX);
                led_off(LED_BAR_LD2_IDX);

                if (g_pb2_pressed)
                {
                    lcd_set_ddram_addr(LCD_LINE1_ADDR);
                    lcd_write_string("                ");
                    g_pb2_pressed = false;
                    current_state = BOTH_BUZZERS;
                }
            break; 

            case (BOTH_BUZZERS):
                Keypad_value = keypad_scan();

                if(Keypad_value != 16)
                {
                    DC_value = ((Keypad_value*100)/15);
                    motor0_set_pwm_dc (DC_value);
                }
                lcd_set_ddram_addr(LCD_LINE1_ADDR + LCD_CHAR_POSITION_3);
                lcd_write_string("Both Buzzers");
                lcd_set_ddram_addr(LCD_LINE2_ADDR);
                lcd_write_string("F=");
                lcd_set_ddram_addr(LCD_LINE2_ADDR + LCD_CHAR_POSITION_6);
                lcd_write_byte(motor0_set_freq);
                lcd_set_ddram_addr(LCD_LINE2_ADDR + LCD_CHAR_POSITION_16);
                lcd_write_string("%");
                lcd_set_ddram_addr(LCD_LINE2_ADDR + LCD_CHAR_POSITION_10);
                lcd_write_string("DC=");
                lcd_set_ddram_addr(LCD_LINE2_ADDR + LCD_CHAR_POSITION_13);
                lcd_write_byte(DC_value);
                led_off(LED_BAR_LD2_IDX);
                led_off(LED_BAR_LD1_IDX);


                if (g_pb2_pressed)
                {
                    lcd_set_ddram_addr(LCD_LINE1_ADDR);
                    lcd_write_string("                ");
                    g_pb2_pressed = false;
                    current_state = PASSIVE_MELODY;
                }
            break;

            case (PASSIVE_MELODY):
                
                lcd_set_ddram_addr(LCD_LINE1_ADDR + LCD_CHAR_POSITION_3);
                lcd_write_string("Play Melody");
                lcd_set_ddram_addr(LCD_LINE2_ADDR);
                lcd_write_string("F=");
                lcd_set_ddram_addr(LCD_LINE2_ADDR + LCD_CHAR_POSITION_6);
                lcd_write_byte(motor0_set_freq);
                lcd_set_ddram_addr(LCD_LINE2_ADDR + LCD_CHAR_POSITION_16);
                lcd_write_string("%");
                lcd_set_ddram_addr(LCD_LINE2_ADDR + LCD_CHAR_POSITION_10);
                lcd_write_string("DC=");
                lcd_set_ddram_addr(LCD_LINE2_ADDR + LCD_CHAR_POSITION_13);
                lcd_write_byte(DC_value);
                led_on(LED_BAR_LD1_IDX);
                led_off(LED_BAR_LD2_IDX);

                // Play the melody
                for (int i = 0; i < MARIO_LENGTH; i++)
                {
                    play_note(mario[i].freq, mario[i].duration);
                    msec_delay(100); // short pause between notes
                    lcd_set_ddram_addr(LCD_LINE2_ADDR + LCD_CHAR_POSITION_6);
                    lcd_write_byte(mario[i].freq);

                    lcd_set_ddram_addr(LCD_LINE2_ADDR + LCD_CHAR_POSITION_13);
                    lcd_write_byte(mario[i].duration);
                } /* for */

                if (g_pb2_pressed)
                {
                    lcd_set_ddram_addr(LCD_LINE1_ADDR);
                    lcd_write_string("                ");
                    g_pb2_pressed = false;
                    current_state = BUZZER_IDLE;
                }
            break;
            

        }
    if (g_pb1_pressed)
    {
        lcd_clear();
        lcd_write_string("Program Stopped");
        g_pb1_pressed = false;
        done = true;
    }
  }
}

void config_pb1_interrupt (void)
{
    //set PB1 to rising edge (after inversion)
    GPIOB->POLARITY31_16 = GPIO_POLARITY31_16_DIO18_RISE;

    //Ensure bit is cleared
    GPIOB->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO18_CLR;

    //Unmask PB1 to allow interrupt 
    GPIOB->CPU_INT.IMASK = GPIO_CPU_INT_IMASK_DIO18_SET;

    NVIC_EnableIRQ(GPIOB_INT_IRQn);
} /* config_pb1_interrupt */

void config_pb2_interrupt (void)
{
    //set PB1 to rising edge (after inversion)
    GPIOA->POLARITY15_0 = GPIO_POLARITY15_0_DIO15_RISE;

    //Ensure bit is cleared
    GPIOA->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO15_CLR;

    //Unmask PB1 to allow interrupt 
    GPIOA->CPU_INT.IMASK = GPIO_CPU_INT_IMASK_DIO15_SET;

    NVIC_EnableIRQ(GPIOA_INT_IRQn);
} /* config_pb1_interrupt */

void GROUP1_IRQHandler(void)
{
    uint32_t group_gpio_iidx;
    uint32_t gpio_mis;
    
    do
    {
        group_gpio_iidx = CPUSS->INT_GROUP[1].IIDX;
        switch(group_gpio_iidx)
        {
            case (CPUSS_INT_GROUP_IIDX_STAT_INT0): //GPIOA
                gpio_mis = GPIOA->CPU_INT.MIS;
                if((gpio_mis & GPIO_CPU_INT_MIS_DIO15_MASK) == GPIO_CPU_INT_MIS_DIO15_SET)
                {
                    g_pb2_pressed = true;
                    GPIOA->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO15_CLR;
                } /* if */

                break;
            case (CPUSS_INT_GROUP_IIDX_STAT_INT1): //GPIOB
                gpio_mis = GPIOB->CPU_INT.MIS;
                if((gpio_mis & GPIO_CPU_INT_MIS_DIO18_MASK) == GPIO_CPU_INT_MIS_DIO18_SET)
                {
                    g_pb1_pressed = true;
                    GPIOB->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO18_CLR;
                } /* if */
        }
        
    } while (group_gpio_iidx !=0);
}

//-----------------------------------------------------------------------------
// motor0set_pwm_freq will take the clock divider and clock scaler in order
// to set the pwm frequency of a user defined uint16_t varible. It then upfates
// the load value based on the new PWM frequency and updates it to the load
// register
//-----------------------------------------------------------------------------

void motor0_set_pwm_freq(uint16_t pwm_frequency)
{
// Determine what the timer clock divider and prescaler are
uint32_t tim_clk_divider = TIMA0->CLKDIV + 1;
uint32_t tim_clk_scaler = (TIMA0->COMMONREGS.CPS & GPTIMER_CPS_PCNT_MASK) + 1;
// Now determine what the timer clock frequency is
uint32_t tim_clk = get_bus_clock_freq() / (tim_clk_divider * tim_clk_scaler);
// Update the load value based on new PWM frequency
uint32_t load_value = (tim_clk / pwm_frequency);
// Disable timer and update the load register
motor0_pwm_disable();
TIMA0->COUNTERREGS.LOAD = (load_value - 1) & GPTIMER_LOAD_LD_MASK;
} /* */



void play_note(uint16_t freq, uint16_t duration_ms)
{
// Only enable the PWM if we have a frequency
if (freq != 0)
{
motor0_set_pwm_freq(freq);
motor0_set_pwm_dc(50);
motor0_pwm_enable();
msec_delay(duration_ms);
motor0_pwm_disable();
} /* if */
} /* play_note */