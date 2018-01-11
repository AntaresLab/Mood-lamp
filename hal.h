/**
@file           hal.h
@author         <a href="https://github.com/AntaresLab">AntaresLab</a>
@version        1.0.0
@date           06-January-2018
@brief          This file consists hardware-depended calls interface.
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

#ifndef __HAL_H__
#define __HAL_H__

#include <stm8s.h>

/**
@defgroup hal Hardware abstract layer
@brief This module consists hardware-depended calls
@details Uses external SPL functions calls:
@code
void GPIO_DeInit(GPIO_TypeDef* GPIOx);
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_Pin_TypeDef GPIO_Pin, GPIO_Mode_TypeDef GPIO_Mode);
void CLK_DeInit(void);
void CLK_SYSCLKConfig(CLK_Prescaler_TypeDef CLK_Prescaler);
void TIM1_DeInit(void);
void TIM1_TimeBaseInit(uint16_t TIM1_Prescaler, 
                       TIM1_CounterMode_TypeDef TIM1_CounterMode,
                       uint16_t TIM1_Period, uint8_t TIM1_RepetitionCounter);
void TIM1_Cmd(FunctionalState NewState);
void TIM1_CtrlPWMOutputs(FunctionalState NewState);
void TIM1_OC2Init(TIM1_OCMode_TypeDef TIM1_OCMode, 
                  TIM1_OutputState_TypeDef TIM1_OutputState, 
                  TIM1_OutputNState_TypeDef TIM1_OutputNState, 
                  uint16_t TIM1_Pulse, TIM1_OCPolarity_TypeDef TIM1_OCPolarity, 
                  TIM1_OCNPolarity_TypeDef TIM1_OCNPolarity, 
                  TIM1_OCIdleState_TypeDef TIM1_OCIdleState, 
                  TIM1_OCNIdleState_TypeDef TIM1_OCNIdleState);
void TIM1_OC3Init(TIM1_OCMode_TypeDef TIM1_OCMode, 
                  TIM1_OutputState_TypeDef TIM1_OutputState, 
                  TIM1_OutputNState_TypeDef TIM1_OutputNState, 
                  uint16_t TIM1_Pulse, TIM1_OCPolarity_TypeDef TIM1_OCPolarity, 
                  TIM1_OCNPolarity_TypeDef TIM1_OCNPolarity, 
                  TIM1_OCIdleState_TypeDef TIM1_OCIdleState, 
                  TIM1_OCNIdleState_TypeDef TIM1_OCNIdleState);
void TIM1_OC4Init(TIM1_OCMode_TypeDef TIM1_OCMode, 
                  TIM1_OutputState_TypeDef TIM1_OutputState, uint16_t TIM1_Pulse,
                  TIM1_OCPolarity_TypeDef TIM1_OCPolarity, 
                  TIM1_OCIdleState_TypeDef TIM1_OCIdleState);
void TIM1_OC2PreloadConfig(FunctionalState NewState);
void TIM1_OC3PreloadConfig(FunctionalState NewState);
void TIM1_OC4PreloadConfig(FunctionalState NewState);
void TIM1_SetCompare2(uint16_t Compare2);
void TIM1_SetCompare3(uint16_t Compare3);
void TIM1_SetCompare4(uint16_t Compare4);
void FLASH_Unlock(FLASH_MemType_TypeDef FLASH_MemType);
void FLASH_ProgramByte(uint32_t Address, uint8_t Data);
uint8_t FLASH_ReadByte(uint32_t Address);
@endcode
and u8, u16, u32 datatypes definitions
@{
*/

/**
@defgroup hal_gpio HAL GPIO
@ingroup hal
@brief Consists GPIO control functions
@{
*/

void gpio_init();

///@}

/**
@defgroup hal_clk HAL CLK
@ingroup hal
@brief Consists MCU clocking control functions
@{
*/

void clk_init();

///@}

/**
@defgroup hal_pwm HAL PWM
@ingroup hal
@brief Consists PWM timer control functions
@{
*/

void pwm_init();
void set_rgbw_output_value(u8 channel, u16 value);

///@}

/**
@defgroup hal_eeprom HAL EEPROM
@ingroup hal
@brief Consists EEPROM control functions
@{
*/

void eeprom_init();
u16 get_saved_xorshift_value();
void save_xorshift_value(u16 value);

///@}

///@}

#endif /* __HAL_H__ */