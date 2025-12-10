//*****************************************************************************
//*****************************    C Source Code    ***************************
//*****************************************************************************
//  DESIGNER NAME:  John Marcello & Vinny Vath
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
#include <stdlib.h>
#include <time.h>

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
void run_final_project_main(void);
void display_menu(void);
void config_pb1_interrupt(void);
void config_pb2_interrupt(void);
void GROUP1_IRQHandler(void);
void motor0_set_pwm_freq(uint16_t pwm_frequency);
void play_note(uint16_t freq, uint16_t duration_ms, uint16_t note_spacing);
void play_song_menu(void);
void play_moog_city(void);
void play_aria_math(void);
void play_living_mice(void);
void volume_change(void);
void play_wet_hands(void);
void play_strad(void);
void random_song(void);
void lcd_write_arrow(void);
//-----------------------------------------------------------------------------
// Define symbolic constants used by the program
//-----------------------------------------------------------------------------
#define JOYSTICK_CHANNEL 7
#define JOYSTICK_UP 3000
#define JOYSTICK_DOWN 2000
#define MSECJOYSTICK 250
#define motor0_set_freq 5
#define potentiometerchannel 1
//-----------------------------------------------------------------------------
// Define global variables and structures here.
// NOTE: when possible avoid using global variables
//-----------------------------------------------------------------------------
bool     g_pb1_pressed = false;
bool     g_pb2_pressed = false;
uint16_t volume_value  = 0;

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
  leds_disable();
  dipsw_init();
  ADC0_init(ADC12_MEMCTL_VRSEL_VDDA_VSSA);
  motor0_init();
  motor0_pwm_init(200000 / 50, 2000);
  config_pb1_interrupt();
  config_pb2_interrupt();

  run_final_project_main();

  NVIC_DisableIRQ(GPIOB_INT_IRQn);
  NVIC_DisableIRQ(GPIOA_INT_IRQn);
  while (1)
    ;

} /* main */

void run_final_project_main(void)
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

  uint8_t play_mode = 0;
  uint8_t index     = 0;
  bool    done      = false;

  lcd_clear();
  leds_disable();
  uint16_t ADC_value;
  lcd_clear();

  typedef enum
  {
    MAIN_MENU = 0,
    PLAYMODE,
    VOLUME,
    EXIT,
    SERIAL,
    RANDOM_SONG,
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
      {
        lcd_clear();
        play_song_menu();
        index         = 0;
        current_state = PLAYMODE;
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
          current_state = EXIT_PLAYMODE;
          msec_delay(MSECJOYSTICK);
          index = 0;
        }

        if (ADC_value > JOYSTICK_UP)
        {
          current_state = PLAYMODE;
          msec_delay(MSECJOYSTICK);
          index = 0;
        }

        if (g_pb2_pressed)
        {
          g_pb2_pressed = false;
          random_song();
          index = 0;
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
          current_state = RANDOM_SONG;
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

      case (VOLUME):
        if (index < 1)
        {
          lcd_clear();
          lcd_write_string("Change Volume?");
          index++;
        }

        ADC_value = ADC0_in(JOYSTICK_CHANNEL);

        if (g_pb2_pressed)
        {
          volume_change();
          index         = 0;
          current_state = MAIN_MENU;
          g_pb2_pressed = false;
        }

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

void play_note(uint16_t freq, uint16_t duration_ms,
               uint16_t note_spacing) // DtoAnolog could be so speaker voltage
                                      // could be changed)
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

void play_song_menu(void)
{
  typedef enum
  {
    MOOG_CITY = 0,
    ARIA_MATH,
    LIVING_MICE,
    WET_HANDS,
    STRAD,
    EXIT,
  } main_menu_state_t;

  main_menu_state_t current_state_song;
  current_state_song = MOOG_CITY;
  bool    complete   = false;
  uint8_t index      = 0;
  leds_disable();
  uint16_t ADC_value;

  lcd_clear();
  lcd_write_string("Press PB 1 to");
  lcd_set_ddram_addr(LCD_LINE2_ADDR);
  lcd_write_string("repeat song");
  msec_delay(2000);
  lcd_clear();
  lcd_write_string("Press PB 2 to");
  lcd_set_ddram_addr(LCD_LINE2_ADDR);
  lcd_write_string("Pause and Play");
  msec_delay(2000);

  while (!complete)
  {
    switch (current_state_song)
    {
      case (MOOG_CITY):
        if (index < 1)
        {
          lcd_clear();
          lcd_write_string("Moog City");
          lcd_write_arrow();
          index++;
        }

        ADC_value = ADC0_in(JOYSTICK_CHANNEL);

        if (ADC_value < JOYSTICK_DOWN)
        {
          current_state_song = ARIA_MATH;
          msec_delay(MSECJOYSTICK);
          index = 0;
        }

        if (g_pb2_pressed)
        {
          msec_delay(500);
          index         = 0;
          g_pb2_pressed = false;
          play_moog_city();
        }

        break;

      case (ARIA_MATH):
        if (index < 1)
        {
          lcd_clear();
          lcd_write_string("Aria Math");
          lcd_write_arrow();
          index++;
        }

        ADC_value = ADC0_in(JOYSTICK_CHANNEL);

        if (ADC_value < JOYSTICK_DOWN)
        {
          current_state_song = LIVING_MICE;
          msec_delay(MSECJOYSTICK);
          index = 0;
        }

        if (ADC_value > JOYSTICK_UP)
        {
          current_state_song = MOOG_CITY;
          msec_delay(MSECJOYSTICK);
          index = 0;
        }

        if (g_pb2_pressed)
        {
          msec_delay(500);
          index         = 0;
          g_pb2_pressed = false;
          play_aria_math();
        }

        break;

      case (LIVING_MICE):
        if (index < 1)
        {
          lcd_clear();
          lcd_write_string("Living Mice");
          lcd_write_arrow();
          index++;
        }

        ADC_value = ADC0_in(JOYSTICK_CHANNEL);

        if (ADC_value < JOYSTICK_DOWN)
        {
          current_state_song = WET_HANDS;
          msec_delay(MSECJOYSTICK);
          index = 0;
        }

        if (ADC_value > JOYSTICK_UP)
        {
          current_state_song = ARIA_MATH;
          msec_delay(MSECJOYSTICK);
          index = 0;
        }

        if (g_pb2_pressed)
        {
          msec_delay(500);
          index         = 0;
          g_pb2_pressed = false;
          play_living_mice();
        }

        break;

      case (WET_HANDS):
        if (index < 1)
        {
          lcd_clear();
          lcd_write_string("Wet Hands");
          lcd_write_arrow();
          index++;
        }

        ADC_value = ADC0_in(JOYSTICK_CHANNEL);

        if (ADC_value < JOYSTICK_DOWN)
        {
          current_state_song = STRAD;
          msec_delay(MSECJOYSTICK);
          index = 0;
        }

        if (ADC_value > JOYSTICK_UP)
        {
          current_state_song = LIVING_MICE;
          msec_delay(MSECJOYSTICK);
          index = 0;
        }

        if (g_pb2_pressed)
        {
          msec_delay(500);
          index         = 0;
          g_pb2_pressed = false;
          play_wet_hands();
          ;
        }

        break;

      case (STRAD):
        if (index < 1)
        {
          lcd_clear();
          lcd_write_string("Strad");
          lcd_write_arrow();
          index++;
        }

        ADC_value = ADC0_in(JOYSTICK_CHANNEL);

        if (ADC_value < JOYSTICK_DOWN)
        {
          current_state_song = EXIT;
          msec_delay(MSECJOYSTICK);
          index = 0;
        }

        if (ADC_value > JOYSTICK_UP)
        {
          current_state_song = WET_HANDS;
          msec_delay(MSECJOYSTICK);
          index = 0;
        }

        if (g_pb2_pressed)
        {
          msec_delay(500);
          index         = 0;
          g_pb2_pressed = false;
          play_strad();
        }

        break;

      case (EXIT):
        if (index < 1)
        {
          lcd_clear();
          lcd_write_string("Exit Song");
          lcd_set_ddram_addr(LCD_LINE2_ADDR);
          lcd_write_string("Slection?");
          index++;
        }

        ADC_value = ADC0_in(JOYSTICK_CHANNEL);

        if (ADC_value > JOYSTICK_UP)
        {
          current_state_song = STRAD;
          msec_delay(MSECJOYSTICK);
          index = 0;
        }

        if (g_pb2_pressed)
        {
          msec_delay(500);
          lcd_clear();
          lcd_write_string("Exiting Player");
          msec_delay(2000);
          index         = 0;
          g_pb2_pressed = false;
          complete      = true;
        }
    }
  }
}

void play_moog_city(void)
{
  lcd_clear();
  led_on(LED_BAR_LD1_IDX);
  led_off(LED_BAR_LD2_IDX);

  for (int i = 0; i < MOOG_CITY_LENGTH; i++)
  {
    lcd_write_string(moog_city[i].note);
    play_note(moog_city[i].freq, moog_city[i].duration,
              moog_city[i].note_spacing);
    lcd_clear();

    if (g_pb2_pressed)
    {
      msec_delay(200);
      uint8_t pause_value = 0;
      g_pb2_pressed       = false;
      lcd_write_string("Song Paused...");
      lcd_set_ddram_addr(LCD_LINE2_ADDR);
      lcd_write_string("Press PB2 2 play");
      while (pause_value < 1)
      {
        if (g_pb2_pressed)
        {
          lcd_clear();
          g_pb2_pressed = false;
          msec_delay(200);
          pause_value++;
        }
      }
    }

  } /* for */

  if (g_pb1_pressed)
  {
    g_pb1_pressed = false;
    msec_delay(200);
    play_moog_city();
  }
}

void play_aria_math(void)
{
  lcd_clear();
  led_on(LED_BAR_LD1_IDX);
  led_off(LED_BAR_LD2_IDX);
  for (int i = 0; i < ARIA_MATH_LENGTH; i++)
  {
    lcd_write_string(aria_math[i].note);
    play_note(aria_math[i].freq, aria_math[i].duration,
              aria_math[i].note_spacing);
    lcd_clear();

    if (g_pb2_pressed)
    {
      msec_delay(200);
      uint8_t pause_value = 0;
      g_pb2_pressed       = false;
      lcd_write_string("Song Paused...");
      lcd_set_ddram_addr(LCD_LINE2_ADDR);
      lcd_write_string("Press PB2 2 play");
      while (pause_value < 1)
      {
        if (g_pb2_pressed)
        {
          lcd_clear();
          g_pb2_pressed = false;
          msec_delay(200);
          pause_value++;
        }
      }
    }
  } /* for */

  if (g_pb1_pressed)
  {
    g_pb1_pressed = false;
    msec_delay(200);
    play_aria_math();
  }
}

void play_living_mice(void)
{
  lcd_clear();
  led_on(LED_BAR_LD1_IDX);
  led_off(LED_BAR_LD2_IDX);
  for (int i = 0; i < LIVING_MICE_LENGTH; i++)
  {
    lcd_write_string(living_mice[i].note);
    play_note(living_mice[i].freq, living_mice[i].duration,
              living_mice[i].note_spacing);
    lcd_clear();

    if (g_pb2_pressed)
    {
      msec_delay(200);
      uint8_t pause_value = 0;
      g_pb2_pressed       = false;
      lcd_write_string("Song Paused...");
      lcd_set_ddram_addr(LCD_LINE2_ADDR);
      lcd_write_string("Press PB2 2 play");
      while (pause_value < 1)
      {
        if (g_pb2_pressed)
        {
          lcd_clear();
          g_pb2_pressed = false;
          msec_delay(200);
          pause_value++;
        }
      }
    }
  } /* for */

  if (g_pb1_pressed)
  {
    g_pb1_pressed = false;
    msec_delay(200);
    play_living_mice();
  }
}

void play_wet_hands(void)
{
  lcd_clear();
  led_on(LED_BAR_LD1_IDX);
  led_off(LED_BAR_LD2_IDX);
  lcd_set_ddram_addr(LCD_LINE1_ADDR);
  for (int i = 0; i < WET_HANDS_LENGTH; i++)
  {
    lcd_write_string(wet_hands[i].note);
    play_note(wet_hands[i].freq, wet_hands[i].duration,
              wet_hands[i].note_spacing);
    lcd_clear();

    if (g_pb2_pressed)
    {
      msec_delay(200);
      uint8_t pause_value = 0;
      g_pb2_pressed       = false;
      lcd_write_string(
          "Song Paused..."); // If i had more time we could of put it into the
                             // sytick in order to have a varible that is just
                             // called stop and play music
      lcd_set_ddram_addr(LCD_LINE2_ADDR);
      lcd_write_string("Press PB2 2 play");
      while (pause_value < 1)
      {
        if (g_pb2_pressed)
        {
          lcd_clear();
          g_pb2_pressed = false;
          msec_delay(200);
          pause_value++;
        }
      }
    }

  } /* for */

  if (g_pb1_pressed)
  {
    g_pb1_pressed = false;
    msec_delay(200);
    play_wet_hands();
  }
}

void play_strad(void)
{
  lcd_clear();
  led_on(LED_BAR_LD1_IDX);
  led_off(LED_BAR_LD2_IDX);
  for (int i = 0; i < STRAD_LENGTH; i++)
  {
    lcd_write_string(strad[i].note);
    play_note(strad[i].freq, strad[i].duration, strad[i].note_spacing);
    lcd_clear();

    if (g_pb2_pressed)
    {
      msec_delay(200);
      uint8_t pause_value = 0;
      g_pb2_pressed       = false;
      lcd_write_string("Song Paused...");
      lcd_set_ddram_addr(LCD_LINE2_ADDR);
      lcd_write_string("Press PB2 2 play");
      while (pause_value < 1)
      {
        if (g_pb2_pressed)
        {
          lcd_clear();
          g_pb2_pressed = false;
          msec_delay(200);
          pause_value++;
        }
      }
    }
  } /* for */

  if (g_pb1_pressed)
  {
    g_pb1_pressed = false;
    msec_delay(200);
    play_strad();
  }
}

void volume_change(void)
{
  bool     done = false;
  uint16_t ADC_value;

  while (!done)
  {
    leds_enable();
    ADC_value = ADC0_in(JOYSTICK_CHANNEL);

    lcd_set_ddram_addr(LCD_LINE1_ADDR);
    lcd_write_string("Volume = ");
    lcd_set_ddram_addr(LCD_LINE1_ADDR + LCD_CHAR_POSITION_10);
    lcd_write_doublebyte(ADC_value);

    leds_off();

    for (uint8_t led_idx = LED_BAR_LD0_IDX; led_idx < ADC_value / 455;
         led_idx++)
    {
      led_on(led_idx);
    }

    if (g_pb1_pressed)
    {
      g_pb1_pressed = false;
      motor0_set_pwm_dc(ADC_value);
      lcd_clear();
      lcd_write_string("Volume Changed");
      lcd_set_ddram_addr(LCD_LINE2_ADDR);
      lcd_write_string("to:");
      lcd_set_ddram_addr(LCD_LINE2_ADDR + LCD_CHAR_POSITION_5);
      lcd_write_doublebyte(ADC_value);
      msec_delay(2000);
      lcd_clear();
      lcd_write_string("Exiting Volume");
      lcd_set_ddram_addr(LCD_LINE2_ADDR);
      lcd_write_string("Change");
      msec_delay(2000);
      leds_disable();
      done = true;
    }
  }
}

void random_song(void)
{
  // Seed the random number generator once
  srand(time(NULL));

  // Generate a random number between 1 and 100
  int num2 = (rand() % 5) + 1;

  if (num2 == 1)
  {
    play_moog_city();
  }

  if (num2 == 2)
  {
    play_aria_math();
  }

  if (num2 == 3)
  {
    play_living_mice();
  }

  if (num2 == 4)
  {
    play_wet_hands();
  }

  if (num2 == 5)
  {
    play_strad();
  }
}

void lcd_write_arrow(void)
{
  lcd_set_ddram_addr(LCD_CHAR_POSITION_13);
  lcd_write_string("<---");
}