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
#include "adc.h"
#include "clock.h"
#include "LaunchPad.h"
#include "lcd1602.h"
#include "ti/devices/msp/m0p/mspm0g350x.h"
#include "ti/devices/msp/peripherals/hw_adc12.h"
#include "ti/devices/msp/peripherals/hw_gpio.h"
#include "ti/devices/msp/peripherals/m0p/hw_cpuss.h"

//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void config_pb1_interrupt (void);
void config_pb2_interrupt (void);
void run_Lab9_p1 (void);


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
  motor0_pwm_enable();
  run_Lab9_p1();
 
 // Endless loop to prevent program from ending
 while (1);


} /* main */

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
                Keypad_value = keypad_scan();

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

                if (g_pb2_pressed)
                {
                    lcd_set_ddram_addr(LCD_LINE1_ADDR);
                    lcd_write_string("                ");
                    g_pb2_pressed = false;
                    current_state = BUZZER_IDLE;
                }
            break;
            

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

void motor0_set_pwm_freq(uint16_t pwm_frequency)
{
    uint32_t load_value = 200000;
    motor0_set_pwm_count(load_value/pwm_frequency);                  
}
