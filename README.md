# Mood_lamp
This firmware allows to get the mood lamp effect (smooth flow of random colors) using an [AntaresLab RGBW_controller board](https://github.com/AntaresLab/RGBW_controller) and RGB LED strip.

Each color is generated using a random number generator, the state of which is updated and stored in non-volatile MCU memory each time power is turned on. Thus, each time power is turned on the color schemes will not be repeated. There are 5 color schemes in total:

* Only one random color constituent (R, G or B) full power color scheme (generates red, green or blue color at random)
* Only two random color constituents full power color scheme (generates yellow, turquoise or purple color at random)
* Three (all) color constituents full power color scheme (generates pseudowhite color)
* One random color constituent full power, another color constituents - random power color scheme (generates random color)
* One random color constituent random power, another color constituents - full power color scheme (generates random color)

Color scheme and parameters that define the color within the selected scheme are chosen randomly. The probability of choosing a particular scheme can be adjusted in a mood_logic.h file by adjusting the corresponding coefficients. When the power is turned on, a smooth transition from off state to random color occurs.

This firmware created to run on the AntaresLab RGBW_controller board. If you want to use it with another board or MCU, please change HAL functions and definitions in hal.h and hal.c files for your board or MCU and use the appropriate libraries and compiler.

I will be glad to any contribution to the development of my projects.

If you will find any mistakes or inaccuracies in my projects, please please notify me about that.

I will be glad, if you modify, impove or port my projects to other platforms, standards, compilers, boards, etc. Please notify me if you do this, and I will mention here about that.