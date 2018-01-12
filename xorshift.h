/**
@file           xorshift.h
@author         <a href="https://github.com/AntaresLab">AntaresLab</a>
@version        1.0.1
@date           12-January-2018
@brief          This file consists random number generator interface.
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

#ifndef __XORSHIFT_H__
#define __XORSHIFT_H__

#include <stm8s.h>

/**
@defgroup xorshift Xorshift random number generator
@brief This module consists <a href="http://www.arklyffe.com/main/2010/08/29/xorshift-pseudorandom-number-generator/">16-bit xorshift random number generator</a>
@{
*/

void uint16_xorshift_init(uint16_t value);
uint16_t get_random_uint16();

///@}

#endif /* __XORSHIFT_H__ */