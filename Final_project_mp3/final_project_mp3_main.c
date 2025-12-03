//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  John & Vinny
//
//       LAB NAME:  Final Project MP3 Player
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
#include "LaunchPad.h"
#include "adc.h"
#include "clock.h"
#include "lcd1602.h"
#include "minecraft_melodies.h"
#include "pitches.h"
#include "uart.h"
#include <stdint.h>
#include <ti/devices/msp/msp.h>

//-----------------------------------------------------------------------------
// Define function prototypes used by the program
//-----------------------------------------------------------------------------
void run_final_project(void);
void display_menu(void);
void UART_output_string(const char *string);
void config_pb1_interrupt(void);
void config_pb2_interrupt(void);
void GROUP1_IRQHandler(void);
void motor0_set_pwm_freq(uint16_t pwm_frequency);
void play_note(uint16_t freq, uint16_t duration_ms, uint16_t note_spacing);
void play_moog_city(void);
//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------

#define BUFFER_SIZE 1
#define MAX_BUFFER_LENGTH 2
#define NEWLINE "\r\n"
#define JOYSTICK_CHANNEL 7
#define JOYSTICK_UP 3000
#define JOYSTICK_DOWN 2000
#define MSECJOYSTICK 250
#define motor0_set_freq 5
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
  I2C_init();
  lcd1602_init();
  UART_init(115200);
  leds_init();
  leds_enable();
  seg7_init();
  dipsw_init();
  ADC0_init(ADC12_MEMCTL_VRSEL_VDDA_VSSA);
  motor0_init();
  motor0_pwm_init(200000 / 50, 2000);
  config_pb1_interrupt();
  config_pb2_interrupt();

  run_final_project();

  NVIC_DisableIRQ(GPIOB_INT_IRQn);
  NVIC_DisableIRQ(GPIOA_INT_IRQn);
  while (1)
    ;

} /* main */

void run_final_project(void)
{
  lcd_clear();
  lcd_write_string("Welcome to the");
  lcd_set_ddram_addr(LCD_LINE2_ADDR);
  lcd_write_string("Jukebox!");
  msec_delay(2000);
  lcd_clear();
  lcd_write_string("Use Joystick");
  lcd_set_ddram_addr(LCD_LINE2_ADDR);
  lcd_write_string("+ pb2 to select");
  msec_delay(2000);

  uint8_t menu_options = 0;
  uint8_t play_mode    = 0;
  uint8_t index        = 0;
  bool    done         = false;

  lcd_clear();
  leds_disable();
  uint16_t ADC_value;
  lcd_clear();
  leds_disable();

  typedef enum
  {
    MAIN_MENU = 0,
    PLAYMODE,
    VOLUME,
    EXIT,
    SERIAL,
    RANDOM_SONG,
    REPEAT_SONG,
    EXIT_PLAYMODE,
  } main_menu_state_t;

  main_menu_state_t current_state;
  current_state = MAIN_MENU;

  while (!done)
  {
    switch (current_state)
    {
      case (MAIN_MENU):
        if (index < 1)
        {
          lcd_clear();
          lcd_write_string("Start Player?");
          index++;
        }

        ADC_value = ADC0_in(JOYSTICK_CHANNEL);

        if (ADC_value < JOYSTICK_DOWN)
        {
          current_state = VOLUME;
          msec_delay(MSECJOYSTICK);
          index = 0;
        }

        if (g_pb2_pressed)
        {
          msec_delay(500);
          index         = 0;
          g_pb2_pressed = false;
          current_state = PLAYMODE;
        }

        break;

      case (PLAYMODE):
        if (index < 1)
        {
          lcd_clear();
          lcd_write_string("Song Selection");
          index++;
        }

        ADC_value = ADC0_in(JOYSTICK_CHANNEL);

        if (ADC_value < JOYSTICK_DOWN)
        {
          current_state = RANDOM_SONG;
          msec_delay(MSECJOYSTICK);
          index = 0;
        }

        if (g_pb2_pressed)
        {
          current_state = SERIAL;
          g_pb2_pressed = false;
          index         = 0;
        }
        break;

      case (SERIAL):
        if (index < 1)
        {
          lcd_clear();
          lcd_write_string("Song title here");
          index++;
          // funtion here for song player
        }
        ADC_value = ADC0_in(JOYSTICK_CHANNEL);

        if (g_pb1_pressed)
        {
          // function here
          g_pb1_pressed = false;
          index         = 0;
        }

        if (g_pb2_pressed)
        {
          lcd_clear();
          play_moog_city();
          g_pb2_pressed = false;
          index         = 0;
        }

        break;

      case (RANDOM_SONG):
        if (index < 1)
        {
          lcd_clear();
          lcd_write_string("Random Song");
          index++;
        }

        ADC_value = ADC0_in(JOYSTICK_CHANNEL);
        // function goes here:

        if (ADC_value < JOYSTICK_DOWN)
        {
          current_state = REPEAT_SONG;
          msec_delay(MSECJOYSTICK);
          index = 0;
        }

        if (ADC_value > JOYSTICK_UP)
        {
          current_state = PLAYMODE;
          msec_delay(MSECJOYSTICK);
          index = 0;
        }

        if (g_pb1_pressed)
        {
          // function here
          g_pb1_pressed = false;
          index         = 0;
        }

        break;

      case (EXIT_PLAYMODE):
        if (index < 1)
        {
          lcd_clear();
          lcd_write_string("Exit Player?");
          index++;
        }
        ADC_value = ADC0_in(JOYSTICK_CHANNEL);
        // function goes here:

        if (ADC_value > JOYSTICK_UP)
        {
          current_state = REPEAT_SONG;
          msec_delay(MSECJOYSTICK);
          index = 0;
        }

        if (g_pb2_pressed)
        {
          lcd_clear();
          lcd_write_string("Going Back to");
          lcd_set_ddram_addr(LCD_LINE2_ADDR);
          lcd_write_string("Main Menu");
          msec_delay(1000);
          current_state = MAIN_MENU;
          g_pb2_pressed = false;
          index         = 0;
        }

        break;

      case (REPEAT_SONG):
        if (index < 1)
        {
          lcd_clear();
          lcd_write_string("Repeat Song");
          index++;
        }

        ADC_value = ADC0_in(JOYSTICK_CHANNEL);
        // funtion here:

        if (ADC_value > JOYSTICK_UP)
        {
          current_state = RANDOM_SONG;
          msec_delay(MSECJOYSTICK);
          index = 0;
        }

        if (g_pb2_pressed)
        {
          // function here:
          g_pb2_pressed = false;
          index         = 0;
        }

        if (ADC_value < JOYSTICK_DOWN)
        {
          current_state = EXIT_PLAYMODE;
          msec_delay(MSECJOYSTICK);
          index = 0;
        }

        break;

      case (VOLUME):
        if (index < 1)
        {
          lcd_clear();
          lcd_write_string("Change Volume?");
          index++;
        }

        ADC_value = ADC0_in(JOYSTICK_CHANNEL);

        if (ADC_value < JOYSTICK_DOWN)
        {
          current_state = EXIT;
          msec_delay(MSECJOYSTICK);
          index = 0;
        }

        if (ADC_value > JOYSTICK_UP)
        {
          current_state = MAIN_MENU;
          msec_delay(MSECJOYSTICK);
          index = 0;
        }

        if (g_pb2_pressed)
        {
          // function here:
          g_pb1_pressed = false;
          index         = 0;
        }
        break;

      case (EXIT):

        if (index < 1)
        {
          lcd_clear();
          lcd_write_string("Stop Program?");
          index++;
        }

        ADC_value = ADC0_in(JOYSTICK_CHANNEL);

        if (g_pb2_pressed)
        {
          lcd_clear();
          lcd_write_string("Bye-Bye!");
          done          = true;
          g_pb2_pressed = false;
        }

        if (ADC_value > JOYSTICK_UP)
        {
          current_state = VOLUME;
          msec_delay(MSECJOYSTICK);
          index = 0;
        }
        // delay.
        // turn of lcd.

        break;
    }
  }
}

void config_pb1_interrupt(void)
{
  // set PB1 to rising edge (after inversion)
  GPIOB->POLARITY31_16 = GPIO_POLARITY31_16_DIO18_RISE;

  // Ensure bit is cleared
  GPIOB->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO18_CLR;

  // Unmask PB1 to allow interrupt
  GPIOB->CPU_INT.IMASK = GPIO_CPU_INT_IMASK_DIO18_SET;

  NVIC_EnableIRQ(GPIOB_INT_IRQn);
} /* config_pb1_interrupt */

void config_pb2_interrupt(void)
{
  // set PB1 to rising edge (after inversion)
  GPIOA->POLARITY15_0 = GPIO_POLARITY15_0_DIO15_RISE;

  // Ensure bit is cleared
  GPIOA->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO15_CLR;

  // Unmask PB1 to allow interrupt
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
    switch (group_gpio_iidx)
    {
      case (CPUSS_INT_GROUP_IIDX_STAT_INT0): // GPIOA
        gpio_mis = GPIOA->CPU_INT.MIS;
        if ((gpio_mis & GPIO_CPU_INT_MIS_DIO15_MASK) ==
            GPIO_CPU_INT_MIS_DIO15_SET)
        {
          g_pb2_pressed       = true;
          GPIOA->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO15_CLR;
        } /* if */

        break;
      case (CPUSS_INT_GROUP_IIDX_STAT_INT1): // GPIOB
        gpio_mis = GPIOB->CPU_INT.MIS;
        if ((gpio_mis & GPIO_CPU_INT_MIS_DIO18_MASK) ==
            GPIO_CPU_INT_MIS_DIO18_SET)
        {
          g_pb1_pressed       = true;
          GPIOB->CPU_INT.ICLR = GPIO_CPU_INT_ICLR_DIO18_CLR;
        } /* if */
    }

  } while (group_gpio_iidx != 0);
}

void play_note(uint16_t freq, uint16_t duration_ms, uint16_t note_spacing)
{
  // Only enable the PWM if we have a frequency
  if (freq != 0)
  {
    motor0_set_pwm_freq(freq);
    motor0_set_pwm_dc(motor0_set_freq);
    motor0_pwm_enable();
    msec_delay(duration_ms);
    msec_delay(note_spacing);
    // add another msec for final project portion
    motor0_pwm_disable();
  } /* if */
} /* play_note */

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

void play_moog_city(void)
{
  led_on(LED_BAR_LD1_IDX);
  led_off(LED_BAR_LD2_IDX);
  lcd_set_ddram_addr(LCD_LINE1_ADDR + LCD_CHAR_POSITION_7);
  for (int i = 0; i < MOOG_CITY_LENGTH; i++)
  {
    lcd_write_string(moog_city[i].note);
    play_note(moog_city[i].freq, moog_city[i].duration,moog_city[i].note_spacing);
    lcd_clear();
  } /* for */
}