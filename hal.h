/**
@file           hal.h
@author         <a href="https://github.com/AntaresLab">AntaresLab</a>
@version        1.0.1
@date           12-January-2018
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
@{
*/

/**
@defgroup hal_gpio HAL GPIO
@ingroup hal
@brief Consists GPIO control functions
@{
*/

#define HAL_GPIO_PINS_MASK                      0x1C

void gpio_init();

///@}

/**
@defgroup hal_clk HAL CLK
@ingroup hal
@brief Consists MCU clocking control functions
@{
*/

#define HAL_CLK_HSI_DIV_1_OUTPUT                0x00

void clk_init();

///@}

/**
@defgroup hal_pwm HAL PWM
@ingroup hal
@brief Consists PWM timer control functions
@{
*/

void pwm_init();
void set_rgbw_output_value(uint8_t channel, uint16_t value);

///@}

/**
@defgroup hal_eeprom HAL EEPROM
@ingroup hal
@brief Consists EEPROM control functions
@{
*/

#define HAL_EEPROM_UNBLOCK_CODE_1               ((uint8_t) 0xAE)
#define HAL_EEPROM_UNBLOCK_CODE_2               ((uint8_t) 0x56)
#define HAL_EEPROM_BLOCK_CODE                   ((uint8_t) 0x08)
#define HAL_EEPROM_START_ADDRESS                ((uint16_t) 0x4000)
#define HAL_EEPROM_END_ADDRESS                  ((uint16_t) 0x427F)
#define HAL_EEPROM_READ_BYTE(ADDRESS)           (*(PointerAttr uint8_t *) ((MemoryAddressCast) (ADDRESS)))
#define HAL_EEPROM_WRITE_BYTE(ADDRESS,DATA)     do{*(PointerAttr uint8_t*) ((MemoryAddressCast) (ADDRESS)) = (uint8_t)(DATA);}while(0)
#define HAL_EEPROM_READ_WORD(ADDRESS)           ((((uint16_t) HAL_EEPROM_READ_BYTE(ADDRESS)) << 8) + HAL_EEPROM_READ_BYTE((ADDRESS) + 1))
#define HAL_EEPROM_WRITE_WORD(ADDRESS,DATA)     do{HAL_EEPROM_WRITE_BYTE((ADDRESS), (DATA) >> 8);HAL_EEPROM_WRITE_BYTE((ADDRESS) + 1, (DATA));}while(0)

void eeprom_init();
uint16_t get_saved_xorshift_value();
void save_xorshift_value(uint16_t value);
void eeprom_deinit();

///@}

///@}

#endif /* __HAL_H__ */