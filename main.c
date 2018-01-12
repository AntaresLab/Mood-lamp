/**
@file           main.c
@author         <a href="https://github.com/AntaresLab">AntaresLab</a>
@version        1.0.1
@date           12-January-2018
@brief          This file consists initialization and main cycle.
@copyright      COPYRIGHT(c) 2018 Sergey Starovoitov aka AntaresLab (https://github.com/AntaresLab)

    This file is part of Mood_lamp.

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This software is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Mood_lamp.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
@mainpage This firmware realizes mood lamp logic on <a href="https://github.com/AntaresLab/RGBW_controller">AntaresLab RGBW controller board</a>.

There are only R, G and B board outputs used for this functional.

Board power-off detector is not used in this firmware.

On-board MCU is STM8S105K4.

Color outputs are: PC4 (R), PC3 (G), PC2 (B).

Another MCU pins are unused.

Firmware created in IAR STM8 3.10.1 IDE.

Optimization level: low.
*/

#include "hal.h"
#include "mood_logic.h"
#include "xorshift.h"

/**
@defgroup main Main module
@{
*/

/**
@brief Main function. 
@details Main function consists initialization commands and main cycle.
*/
void main(){
  gpio_init();                                                                  // GPIO initialization
  clk_init();                                                                   // 16MHz HSI initialization
  pwm_init();                                                                   // PWM timer initialization
  eeprom_init();                                                                // EEPROM memory initialization
  uint16_xorshift_init(get_saved_xorshift_value());                             // Xorshift random generator initialization
  save_xorshift_value(get_random_uint16());                                     // Xorshift random generator new state saving (for next power-on)
  eeprom_deinit();                                                              // EEPROM deinitialization for EEPROM data corrupting possibility exclision
  while(1){                                                                     // Main cycle
    rgb_handle();                                                               // Mood lamp logic handling
    for(uint16_t i = 0; i < 200; ++i);                                          // Color flow speed regulation
  }
}

///@}