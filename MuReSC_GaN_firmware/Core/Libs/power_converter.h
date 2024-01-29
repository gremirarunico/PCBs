/*
 * power_converter.h
 *
 *  Created on: Jan 19, 2024
 *      Author: guerrinm
 */

#ifndef LIBS_POWER_CONVERTER_H_
#define LIBS_POWER_CONVERTER_H_

/**
 * @def PC_MINIMUM_COUNTER
 * @brief the minimum value the comparator counter can manage (it depends on the micro)
 *
 */
#define PC_MINIMUM_COUNTER 96

#include "main.h"

/*************************
 * @struct
 * @brief contains all the important informations about waveforms for the MuReSC converter
 *
 ************************/
struct WaveformParams{
	int frequency;
	int deadTime;
	int dutyCycle;
} ;

#endif /* LIBS_POWER_CONVERTER_H_ */
