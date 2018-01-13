/**
@file           mood_logic.h
@author         <a href="https://github.com/AntaresLab">AntaresLab</a>
@version        1.0.1
@date           12-January-2018
@brief          This file consists mood lamp logic parameters and interface.
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

#ifndef __MOOD_LOGIC_H__
#define __MOOD_LOGIC_H__

//#include <stm8s.h>

/**
@defgroup mood_lamp_logic Mood lamp logic
@brief This module consists mood lamp logic algorithmes
@{
*/

/**
@defgroup mood_lamp_parameters Mood lamp parameters
@ingroup mood_lamp_logic
@brief Consists shares of various color schemes in total value
@{
*/
#define RGB_ONE_COLOR_PROBABILITY               2                               ///< Only one random color channel full power color scheme share
#define RGB_TWO_COLORS_PROBABILITY              2                               ///< Only two random color channels full power color scheme share
#define RGB_THREE_COLORS_PROBABILITY            1                               ///< Three (all) color channels full power color scheme share
#define RGB_ONE_COLOR_AND_RANDOM_PROBABILITY    1                               ///< One random color channel full power, another color channels - random power color scheme share
#define RGB_TWO_COLORS_AND_RANDOM_PROBABILITY   1                               ///< One random color channel random power, another color channels - full power color scheme share
///@}

void rgb_handle();

///@}

#endif /* __MOOD_LOGIC_H__ */