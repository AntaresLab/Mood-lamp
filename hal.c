/**
@file           hal.c
@author         <a href="https://github.com/AntaresLab">AntaresLab</a>
@version        1.0.0
@date           06-January-2018
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
  GPIO_DeInit(GPIOC);
  GPIO_Init(GPIOC, GPIO_PIN_2, GPIO_MODE_OUT_PP_LOW_FAST);                      // Blue channel
  GPIO_Init(GPIOC, GPIO_PIN_3, GPIO_MODE_OUT_PP_LOW_FAST);                      // Green channel
  GPIO_Init(GPIOC, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_FAST);                      // Red channel
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
  CLK_DeInit();
  CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);                                      // Fcpu = Fclk
  CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);                                      // Fclk is HSI / 1
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
  TIM1_DeInit();
  TIM1_TimeBaseInit(0, TIM1_COUNTERMODE_UP, U16_MAX, 0);                        // Fpwm ~ 244Hz, 16 bit resolution
  TIM1_OC2Init(TIM1_OCMODE_PWM1, TIM1_OUTPUTSTATE_ENABLE,\
    TIM1_OUTPUTNSTATE_DISABLE, 0, TIM1_OCPOLARITY_HIGH, TIM1_OCNPOLARITY_HIGH,\
      TIM1_OCIDLESTATE_RESET, TIM1_OCNIDLESTATE_RESET);                         // OC2 - PWM mode 1 otput
  TIM1_OC2PreloadConfig(ENABLE);                                                // OC2 preload register enable
  TIM1_OC3Init(TIM1_OCMODE_PWM1, TIM1_OUTPUTSTATE_ENABLE,\
    TIM1_OUTPUTNSTATE_DISABLE, 0, TIM1_OCPOLARITY_HIGH, TIM1_OCNPOLARITY_HIGH,\
      TIM1_OCIDLESTATE_RESET, TIM1_OCNIDLESTATE_RESET);                         // OC3 - PWM mode 1 otput
  TIM1_OC3PreloadConfig(ENABLE);                                                // OC3 preload register enable
  TIM1_OC4Init(TIM1_OCMODE_PWM1, TIM1_OUTPUTSTATE_ENABLE, 0,\
    TIM1_OCPOLARITY_HIGH, TIM1_OCIDLESTATE_RESET);                              // OC4 - PWM mode 1 otput
  TIM1_OC4PreloadConfig(ENABLE);                                                // OC4 preload register enable
  TIM1_Cmd(ENABLE);                                                             // Start timer
  TIM1_CtrlPWMOutputs(ENABLE);                                                  // Connect timer PWM outputs to GPIOs
}

/**
@brief PWM level changing
@details Sets PWM level on selected channel. Uses the pseudoexponential (quadratic) PWM level addiction for 
apparent brightness linearization: @f$PWM=value^2/value_{max}@f$
@param[in] channel Channel number (0...2)
@param[in] value PWM channel value
@note If invalid channel number received, no any changes makes
*/
void set_rgbw_output_value(u8 channel, u16 value){
  switch(channel){
  case 0:                                                                       // Red channel
    TIM1_SetCompare4((((u32) value) * value) >> 16);
    break;
  case 1:                                                                       // Green channel
    TIM1_SetCompare3((((u32) value) * value) >> 16);
    break;
  case 2:                                                                       // Blue channel
    TIM1_SetCompare2((((u32) value) * value) >> 16);
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
  FLASH_Unlock(FLASH_MEMTYPE_DATA);                                             // Unlock EEPROM memory write protect
  u32 xorshift_address = FLASH_ReadByte(FLASH_DATA_START_PHYSICAL_ADDRESS);     // Random number generator initialization value address calculating
  xorshift_address <<= 8;
  xorshift_address += FLASH_ReadByte(FLASH_DATA_START_PHYSICAL_ADDRESS + 1);    // If address is out of EEPROM memory range
  if((xorshift_address >= (FLASH_DATA_END_PHYSICAL_ADDRESS - FLASH_DATA_START_PHYSICAL_ADDRESS)) || (xorshift_address < 2)){
    FLASH_ProgramByte(FLASH_DATA_START_PHYSICAL_ADDRESS, 0);                    // change addres to the first EEPROM memory cell after contained addres ones
    FLASH_ProgramByte(FLASH_DATA_START_PHYSICAL_ADDRESS + 1, 2);
  }
  if(xorshift_address & 0x00000001U){                                           // Because 2-byte variables are used, adress must have even value
    FLASH_ProgramByte(FLASH_DATA_START_PHYSICAL_ADDRESS + 1, xorshift_address & 0x000000FEU);
  }
}

/**
@brief Extracts random number generator initialization value from EEPROM memory
@return Random number generator initialization value
*/
u16 get_saved_xorshift_value(){
  u32 address = FLASH_ReadByte(FLASH_DATA_START_PHYSICAL_ADDRESS);              // Random number generator initialization value address calculating
  address <<= 8;
  address += FLASH_ReadByte(FLASH_DATA_START_PHYSICAL_ADDRESS + 1);
  address += FLASH_DATA_START_PHYSICAL_ADDRESS;
  u16 value = FLASH_ReadByte(address);                                          // High byte
  value <<= 8;
  value += FLASH_ReadByte(address + 1);                                         // Low byte
  return value;
}

/**
@brief Saves random number generator initialization value into EEPROM memory
@param[in] value New random number generator initialization value
*/
void save_xorshift_value(u16 value){
  u32 address = FLASH_ReadByte(FLASH_DATA_START_PHYSICAL_ADDRESS);              // Random number generator initialization value address calculating
  address <<= 8;
  address += FLASH_ReadByte(FLASH_DATA_START_PHYSICAL_ADDRESS + 1);
  address += FLASH_DATA_START_PHYSICAL_ADDRESS;
  FLASH_ProgramByte(address, value >> 8);
  FLASH_ProgramByte(address + 1, value);
  if(get_saved_xorshift_value() != value){                                      // If value wasn't saved correctly (EEPROM cells are damaged)
    address += 2;                                                               // calculate next random number generator initialization value address
    if(address > FLASH_DATA_END_PHYSICAL_ADDRESS){                              // If new addres is out of EEPROM memory range 
      address = FLASH_DATA_START_PHYSICAL_ADDRESS + 2;                          // change addres to the first EEPROM memory cell after contained addres ones
    }                                                                           // Save new address
    FLASH_ProgramByte(FLASH_DATA_START_PHYSICAL_ADDRESS, (address - FLASH_DATA_START_PHYSICAL_ADDRESS) >> 8);
    FLASH_ProgramByte(FLASH_DATA_START_PHYSICAL_ADDRESS + 1, address - FLASH_DATA_START_PHYSICAL_ADDRESS);
    FLASH_ProgramByte(address, value >> 8);                                     // Save value into the new EEPROM cells
    FLASH_ProgramByte(address + 1, value & 0x00FF);
  }
}

///@}