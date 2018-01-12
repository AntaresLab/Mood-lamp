/**
@file           hal.c
@author         <a href="https://github.com/AntaresLab">AntaresLab</a>
@version        1.0.1
@date           12-January-2018
@brief          This file consists hardware-depended functions.
@copyright      COPYRIGHT(c) 2018 Sergey Starovoitov aka AntaresLab (https://github.com/AntaresLab)

    This file is part of Mood_lamp.

    Mood_lamp is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Mood_lamp is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Mood_lamp.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "hal.h"

/**
@addtogroup hal_gpio
@{
*/

/**
@brief GPIO initialization
@details Initializes color PWM otput pins.
*/
void gpio_init(){
  GPIOC->DDR |= HAL_GPIO_PINS_MASK;                                             // Output pins are outputs
  GPIOC->CR1 |= HAL_GPIO_PINS_MASK;                                             // Push-pull
  GPIOC->CR2 |= HAL_GPIO_PINS_MASK;                                             // High-speed
}

///@}


/**
@addtogroup hal_clk
@{
*/

/**
@brief MCU clock initialization
@details Initializes MCU HSI as clock source with Fmcu = 16MHz
*/
void clk_init(){
  CLK->CKDIVR = HAL_CLK_HSI_DIV_1_OUTPUT;                                       // Fcpu = Fclk = HSI/1
}

///@}

/**
@addtogroup hal_pwm
@{
*/

/**
@brief PWM timer initialization
@details Initializes PWM timer in left-aligned mode with Fpwm ~ 244Hz and 16 bit resolution.
*/
void pwm_init(){
  TIM1->CCER1 = 0x10;                                                           // Timer channels 2, 3, 4 are on
  TIM1->CCER2 = 0x11;
  TIM1->CCMR2 = 0x68;                                                           // Channels are PWM mode 1 outputs, preload registers are on
  TIM1->CCMR3 = 0x68;
  TIM1->CCMR4 = 0x68;
  TIM1->CR1 |= 0x01;                                                            // Start timer
  TIM1->BKR |= 0x80;                                                            // Connect timer PWM outputs to GPIOs
}

/**
@brief PWM level changing
@details Sets PWM level on selected channel. Uses the pseudoexponential (quadratic) PWM level addiction for 
apparent brightness linearization: @f$PWM=value^2/value_{max}@f$
@param[in] channel Channel number (0...2)
@param[in] value PWM channel value
@note If invalid channel number received, no any changes makes
*/
void set_rgbw_output_value(uint8_t channel, uint16_t value){
  value = (((uint32_t) value) * value) >> 16;
  switch(channel){
  case 0:                                                                       // Red channel
    TIM1->CCR4H = (uint8_t) (value >> 8);
    TIM1->CCR4L = (uint8_t) value;
    break;
  case 1:                                                                       // Green channel
    TIM1->CCR3H = (uint8_t) (value >> 8);
    TIM1->CCR3L = (uint8_t) value;
    break;
  case 2:                                                                       // Blue channel
    TIM1->CCR2H = (uint8_t) (value >> 8);
    TIM1->CCR2L = (uint8_t) value;
    break;
  default:                                                                      // Everything else - do nothing
    break;
  }
}

///@}

/**
@addtogroup hal_eeprom
@{
*/

/**
@brief EEPROM memory initialization
@details EEPROM memory contains random number generator initialization value. First 
2 bytes of EEPROM contain addres of random number generator initialization value
relative to the beginning of the EEPROM memory. If memory cells contained random 
number generator initialization value, became damaged (can not be written correctly), 
random number generator initialization value will be written to the next memory 
cells. After that first 2 bytes of EEPROM will contain new addres of number generator 
initialization value relative to the beginning of the EEPROM memory.
*/
void eeprom_init(){
  FLASH->DUKR = HAL_EEPROM_UNBLOCK_CODE_1;                                      // Unlock EEPROM memory write protect
  FLASH->DUKR = HAL_EEPROM_UNBLOCK_CODE_2;
  if(((HAL_EEPROM_READ_WORD(HAL_EEPROM_START_ADDRESS) + HAL_EEPROM_START_ADDRESS)\
    >= HAL_EEPROM_END_ADDRESS) || ((HAL_EEPROM_READ_WORD(HAL_EEPROM_START_ADDRESS) +\
      HAL_EEPROM_START_ADDRESS) < (HAL_EEPROM_START_ADDRESS + 2))){             // If random number generator initialization value address is out of EEPROM memory range
    HAL_EEPROM_WRITE_WORD(HAL_EEPROM_START_ADDRESS, 0x0002);                    // change addres to the first EEPROM memory cell after contained addres ones
  }else if((HAL_EEPROM_READ_WORD(HAL_EEPROM_START_ADDRESS) +\
    HAL_EEPROM_START_ADDRESS) & 0x0001){                                        // Because 2-byte variables are used, adress must have even value
    HAL_EEPROM_WRITE_WORD(HAL_EEPROM_START_ADDRESS, HAL_EEPROM_READ_WORD(HAL_EEPROM_START_ADDRESS) & 0xFE);
  }
}

/**
@brief Extracts random number generator initialization value from EEPROM memory
@return Random number generator initialization value
*/
uint16_t get_saved_xorshift_value(){
  return HAL_EEPROM_READ_WORD(HAL_EEPROM_READ_WORD(HAL_EEPROM_START_ADDRESS) + HAL_EEPROM_START_ADDRESS);
}

/**
@brief Saves random number generator initialization value into EEPROM memory
@param[in] value New random number generator initialization value
*/
void save_xorshift_value(uint16_t value){
  HAL_EEPROM_WRITE_WORD(HAL_EEPROM_READ_WORD(HAL_EEPROM_START_ADDRESS) +\
    HAL_EEPROM_START_ADDRESS, value);
  if(HAL_EEPROM_READ_WORD(HAL_EEPROM_READ_WORD(HAL_EEPROM_START_ADDRESS) +\
    HAL_EEPROM_START_ADDRESS) != value){                                        // If value wasn't saved correctly (EEPROM cells are damaged)
    if((HAL_EEPROM_READ_WORD(HAL_EEPROM_READ_WORD(HAL_EEPROM_START_ADDRESS) +\
      HAL_EEPROM_START_ADDRESS)) < (HAL_EEPROM_END_ADDRESS - 1)){               // calculate next random number generator initialization value address, save it
      HAL_EEPROM_WRITE_WORD(HAL_EEPROM_START_ADDRESS,\
        HAL_EEPROM_READ_WORD(HAL_EEPROM_START_ADDRESS) + 2);
    }else{
      HAL_EEPROM_WRITE_WORD(HAL_EEPROM_START_ADDRESS, 0x0002);
    }
    HAL_EEPROM_WRITE_WORD(HAL_EEPROM_READ_WORD(HAL_EEPROM_START_ADDRESS) +\
      HAL_EEPROM_START_ADDRESS, value);                                         // and save value into the new EEPROM cells
  }
}

/**
@brief EEPROM memory deinitialization
@details Blocks EEPROM memory for write protection
*/
void eeprom_deinit(){
  FLASH->IAPSR &= ~HAL_EEPROM_BLOCK_CODE;
}

///@}