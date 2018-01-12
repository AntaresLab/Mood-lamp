/**
@file           mood_logic.c
@author         <a href="https://github.com/AntaresLab">AntaresLab</a>
@version        1.0.1
@date           12-January-2018
@brief          This file consists mood lamp logic.
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

#include "mood_logic.h"
#include "xorshift.h"
#include "hal.h"

/**
@addtogroup mood_lamp_logic
@{
*/

static uint16_t destination_color[] = {0, 0, 0};                                     ///< Color channels destination PWM values {R, G, B}
static uint16_t current_color[] = {0, 0, 0};                                         ///< Color channels current PWM values {R, G, B}

/**
@brief Mood lamp logic handler
@details This function handles one color changing step and destination color changig
@note This function should be called at regular time intervals 
@f$T=1000000T_{cycle}/(PWM_{max}+1)@f$ microseconds,
where @f$T_{cycle}@f$ - time of one full color flowing, seconds; 
@f$PWM_{max}@f$ - maximal color output PWM timer walue.
*/
void rgb_handle(){
  static uint8_t first_call = 1;                                                     // First function call flag
  uint16_t need_new_color = 1;                                                       // Destination color change flag
  for(uint8_t i = 0; i < 3; ++i){                                                    // Make one power step for every color channel
    if(current_color[i] < destination_color[i]){
      set_rgbw_output_value(i, ++current_color[i]);
      need_new_color = 0;
    }else if(current_color[i] > destination_color[i]){
      set_rgbw_output_value(i, --current_color[i]);
      need_new_color = 0;
    }
  }
  if(need_new_color){                                                           // If destination color was reached
    need_new_color = get_random_uint16() % (    RGB_ONE_COLOR_PROBABILITY +\
                                                RGB_TWO_COLORS_PROBABILITY +\
                                                RGB_THREE_COLORS_PROBABILITY +\
                                                RGB_ONE_COLOR_AND_RANDOM_PROBABILITY +\
                                                RGB_TWO_COLORS_AND_RANDOM_PROBABILITY); // Choose random color scheme for new destination color
    if(need_new_color < RGB_ONE_COLOR_PROBABILITY){                             // Only one random color channel full power color scheme
      for(uint8_t i = 0; i < 3; ++i){
        destination_color[i] = 0;
      }
      destination_color[get_random_uint16() % 3] = U16_MAX;
    }else if(need_new_color < ( RGB_ONE_COLOR_PROBABILITY +\
                                RGB_TWO_COLORS_PROBABILITY)){                   // Only two random color channels full power color scheme
      for(uint8_t i = 0; i < 3; ++i){
        destination_color[i] = U16_MAX;
      }
      destination_color[get_random_uint16() % 3] = 0;
    }else if(need_new_color < ( RGB_ONE_COLOR_PROBABILITY +\
                                RGB_TWO_COLORS_PROBABILITY +\
                                RGB_THREE_COLORS_PROBABILITY)){                 // Three (all) color channels full power color scheme
      for(uint8_t i = 0; i < 3; ++i){
        destination_color[i] = U16_MAX;
      }
    }else if(need_new_color < ( RGB_ONE_COLOR_PROBABILITY +\
                                RGB_TWO_COLORS_PROBABILITY +\
                                RGB_THREE_COLORS_PROBABILITY +\
                                RGB_ONE_COLOR_AND_RANDOM_PROBABILITY)){         // One random color channel full power, another color channels - random power color scheme
      for(uint8_t i = 0; i < 3; ++i){
        destination_color[i] = get_random_uint16();
      }
      destination_color[get_random_uint16() % 3] = U16_MAX;
    }else{                                                                      // One random color channel random power, another color channels - full power color scheme
      for(uint8_t i = 0; i < 3; ++i){
        destination_color[i] = U16_MAX;
      }
      destination_color[get_random_uint16() % 3] = get_random_uint16();
    }
    if(!first_call) for(uint32_t i = 0; i < 500000; ++i);                            // Delay if destination color was changed (for color flowing smoothing)
  }
  first_call = 0;                                                               // Next function call will not first
}

///@}