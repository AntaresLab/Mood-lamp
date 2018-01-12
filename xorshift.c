/**
@file           xorshift.c
@author         <a href="https://github.com/AntaresLab">AntaresLab</a>
@version        1.0.1
@date           12-January-2018
@brief          This file consists random number generator.
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

#include "xorshift.h"

/**
@addtogroup xorshift
@{
*/

static uint16_t y16 = 1;                                                             ///< Xorshift random number generator base

/**
@brief Xorshift initialization
@details Initializes xorshift random number generator by specified value
@param[in] value Initializing value
@note If value is zero xorshift random number generator will be not initialized
*/
void uint16_xorshift_init(uint16_t value){
  if(value) y16 = value;
}

/**
@brief Random value getter
@details Calculates new random value by 
<a href="http://www.arklyffe.com/main/2010/08/29/xorshift-pseudorandom-number-generator/">16-bit xorshift method</a>:
@code
u16 get_random_u16(){
  static uint16_t y16 = 1;
  y16 ^= (y16 << 13);
  y16 ^= (y16 >> 9);
  return y16 ^= (y16 << 7);
}
@endcode
@return Calculated random value
*/
uint16_t get_random_uint16(){
  y16 ^= (y16 << 13);
  y16 ^= (y16 >> 9);
  return y16 ^= (y16 << 7);
}

///@}