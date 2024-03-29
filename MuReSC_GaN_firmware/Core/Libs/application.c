/**
 ******************************************************************************
 * @file         application.c
 * @brief        This file provides code for the application
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 Marco Guerrini.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
#include "application.h"
#include "serial_parser.h"
#include "power_converter.h"
#include "feedback.h"
#include "main.h"
//#include <stdio.h>
#include <stdbool.h>
#include <string.h>

struct WaveformParams waveform = { 1250000, 40, 50, 20 };
//struct WaveformParams waveform = { 1250000, 20, 50 };
struct RgstrPrmHRTIM htimpar = { 4352, 96, 2054, 2272, 4230, 96, 2054, 2272,
		4230 };
bool danger = 0;

char *string[100];

void clParser(void) {
	int value = 0;
	/*
	 * SET
	 */
	if (serial_is_command("set", 0)) {
		/*
		 * FREQUENCY
		 */
		if (serial_is_command("freq", 1)) {
			if (serial_get_int(2, &value)) {
				sprintf(string, "Frequency: %d", value);
				serial_print(string);
				serial_nl();
				if (value <= 8e6)
					waveform.frequency = value;
				else {
					serial_print("Refused, not valid");
					serial_nl();
				}
			} else {
				serial_print("Syntax error");
				serial_nl();
			}
		}
		/*
		 * DUTY
		 */
		else if (serial_is_command("duty", 1)) {
			if (serial_get_int(2, &value)) {
				sprintf(string, "Duty: %d", value);
				serial_print(string);
				serial_nl();
				waveform.dutyCycle = value;
			} else {
				serial_print("Syntax error");
				serial_nl();
			}
		}
		/*
		 * DEAT TIME
		 */
		else if (serial_is_command("dt", 1)) {
			if (serial_get_int(2, &value)) {
				sprintf(string, "Dead time: %d ns", value);
				serial_print(string);
				serial_nl();
				if (value >= PC_MINIMUM_DT)
					waveform.deadTime = value;
				else {
					serial_print("Refused, not valid");
					serial_nl();
				}
			} else {
				serial_print("Syntax error");
				serial_nl();
			}
		}

		/*
		 * ADDITIONAL DEAT TIME
		 */
		else if (serial_is_command("adt", 1)) {
			if (serial_get_int(2, &value)) {
				sprintf(string, "Additional dead time: %d ns", value);
				serial_print(string);
				serial_nl();
				waveform.aDeadTime = value;
			} else {
				serial_print("Syntax error");
				serial_nl();
			}
		}
		/*
		 * CONTROL
		 */
		else if (serial_is_command("ctrl", 1)) {
			// Open loop control
			if (serial_is_command("ol", 2)) {
				pc_stop();
				fb_mode = OPEN_LOOP;
				serial_print(
						"Open loop mode, for safety the converter is stopped");
				serial_nl();
			}
			// On - Off
			else if (serial_is_command("oo", 2)) {
				fb_mode = ON_OFF;
				serial_print("On-Off mode");
				serial_nl();
			}

			// Advanced
			else if (serial_is_command("advanced", 2)) {
				// TODO
				serial_print("Not yet implemented");
				serial_nl();
			} else {
				serial_print("Syntax error");
				serial_nl();
			}
		}

		/*
		 * RESONANT MODE
		 */
		else if (serial_is_command("mode", 1)) {
			// mono resonant mode
			if (serial_is_command("mono", 2)) {
				pc_stop();
				pc_mode = MONO_RESONANT;
				pc_calculator_cmp_mono_resonant(&waveform, &htimpar);
				pc_update(&htimpar);
				serial_print(
						"Mono resonant mode, for safety the converter is stopped");
				serial_nl();
			}
			// multi resoant mode
			else if (serial_is_command("multi", 2)) {
				pc_stop();
				pc_mode = MULTI_RESONANT;
				pc_calculator_cmp_multi_resonant(&waveform, &htimpar);
				pc_update(&htimpar);
				serial_print(
						"Multi resonant mode, for safety the converter is stopped");
				serial_nl();
			}

			else {
				serial_print("Syntax error");
				serial_nl();
			}
		}

		/*
		 * Vout target
		 */
		else if (serial_is_command("vout", 1)) {
			if (serial_get_int(2, &value)) {

				fb_adc_out_target = value * FB_ADC_MAX_VALUE * FB_IN_RATIO
						/ FB_ADC_MAX_VOLTAGE / 1000 - fb_adc_cal_off_out;

				sprintf(string, "Vout set to: %d mV, Target: %d, Offset: %d",
						value, fb_adc_out_target, fb_adc_cal_off_out);
				serial_print(string);
				serial_nl();
			} else {
				serial_print("Syntax error");
				serial_nl();
			}
		}

		/*
		 * REGISTER
		 */
		else if (serial_is_command("reg", 1)) {
			if (danger) {
				// { 4352, 96, 2054, 2272, 4230, 96, 2054, 2272, 4230 };
				if (serial_get_int(2, &value)) {
					htimpar.period = value;
				} else {
					serial_print("Error getting period");
					serial_nl();
				}
				if (serial_get_int(3, &value)) {
					htimpar.A1 = value;
				} else {
					serial_print("Error getting A1");
					serial_nl();
				}
				if (serial_get_int(4, &value)) {
					htimpar.A2 = value;
				} else {
					serial_print("Error getting A2");
					serial_nl();
				}
				if (serial_get_int(5, &value)) {
					htimpar.B1 = value;
				} else {
					serial_print("Error getting B1");
					serial_nl();
				}
				if (serial_get_int(6, &value)) {
					htimpar.B2 = value;
				} else {
					serial_print("Error getting B2");
					serial_nl();
				}
				if (serial_get_int(7, &value)) {
					htimpar.C1 = value;
				} else {
					serial_print("Error getting C1");
					serial_nl();
				}
				if (serial_get_int(8, &value)) {
					htimpar.C2 = value;
				} else {
					serial_print("Error getting C2");
					serial_nl();
				}
				if (serial_get_int(9, &value)) {
					htimpar.D1 = value;
				} else {
					serial_print("Error getting D1");
					serial_nl();
				}
				if (serial_get_int(10, &value)) {
					htimpar.D2 = value;
				} else {
					serial_print("Error getting D2");
					serial_nl();
				}

			} else {
				serial_print("Enable danger mode");
				serial_nl();
			}
		} else {
			serial_print("Syntax error");
			serial_nl();
		}
	}
	/*
	 * GET
	 */
	else if (serial_is_command("get", 0)) {
		/*
		 * ALL
		 */
		if (serial_is_command("all", 1)) {
			sprintf(string, "F: %d Hz, DT: %d ns, ADT: %d Duty: %d%%",
					waveform.frequency, waveform.deadTime, waveform.aDeadTime, waveform.dutyCycle);
			serial_print(string);
			serial_nl();

			sprintf(string,
					"P: %d, A1: %d, A2: %d, B1: %d, B2: %d, C1: %d, C2: %d, D1: %d, D2: %d",
					htimpar.period, htimpar.A1, htimpar.A2, htimpar.B1,
					htimpar.B2, htimpar.C1, htimpar.C2, htimpar.D1, htimpar.D2);
			serial_print(string);
			serial_nl();
		}
		/*
		 * VIN
		 */
		else if (serial_is_command("vin", 1)) {
			//HAL_ADC_Start_IT(&hadc2);
			sprintf(string, "Vin = %d", fb_adc_in);
			serial_print(string);
			serial_nl();
		}
		/*
		 * VOUT
		 */
		else if (serial_is_command("vout", 1)) {
			//HAL_ADC_Start_IT(&hadc1);
			sprintf(string, "Vout = %d", fb_adc_out);
			serial_print(string);
			serial_nl();
		}
	}

	/*
	 * UPDATE
	 */
	else if (serial_is_command("update", 0)) {
		pc_stop();
		if (danger) {
			pc_update(&htimpar);
		} else {
			if (pc_mode == MONO_RESONANT) {
				pc_calculator_cmp_mono_resonant(&waveform, &htimpar);
			} else if (pc_mode == MULTI_RESONANT) {
				pc_calculator_cmp_multi_resonant(&waveform, &htimpar);
			}
			pc_update(&htimpar);
		}
		pc_start();
		serial_print("Update sent");
		serial_nl();
	}

	/*
	 * START
	 */
	else if (serial_is_command("start", 0)) {
		pc_start();
		serial_print("Converter started");
		serial_nl();
	}

	/*
	 * STOP
	 */
	else if (serial_is_command("stop", 0)) {
		pc_stop();
		serial_print("Converter stopped");
		serial_nl();
	}

	/*
	 * DANGER
	 */
	else if (serial_is_command("danger", 0)) {
		if (serial_is_command("enable", 1)) {
			danger = 1;
			serial_print("Danger mode enabled");
			serial_nl();
		} else if (serial_is_command("disable", 1)) {
			danger = 0;
			serial_print("Danger mode disabled");
			serial_nl();
		} else {
			serial_print("Syntax error");
			serial_nl();
		}
	}
	/*
	 * CALIBRATION
	 */
	else if (serial_is_command("calibrate", 0)) {
		if (serial_is_command("vin", 1)) {
			// calibration of vin procedure
			if (serial_get_int(2, &value)) {
				// calibrate here
				fb_adc_cal_off_in = value * FB_ADC_MAX_VALUE * FB_IN_RATIO
						/ FB_ADC_MAX_VOLTAGE / 1000 - fb_adc_in;

				sprintf(string,
						"Offset: %d, True: %d mv, Read: %d, Corrected: %d",
						fb_adc_cal_off_in, value, fb_adc_in,
						fb_adc_in + fb_adc_cal_off_in);
				serial_print(string);
				serial_nl();

			} else {
				serial_print("Error getting calibration Vin");
				serial_nl();
			}
		} else if (serial_is_command("vout", 1)) {
			// calibration of vout procedure
			if (serial_get_int(2, &value)) {
				// calibrate here
				fb_adc_cal_off_out = value * FB_ADC_MAX_VALUE * FB_OUT_RATIO
						/ FB_ADC_MAX_VOLTAGE / 1000 - fb_adc_out;

				sprintf(string,
						"Offset: %d, True: %d mv, Read: %d, Corrected: %d",
						fb_adc_cal_off_out, value, fb_adc_out,
						fb_adc_out + fb_adc_cal_off_out);
				serial_print(string);
				serial_nl();
			} else {
				serial_print("Error getting calibration Vout");
				serial_nl();
			}
		} else {
			serial_print("Syntax error");
			serial_nl();
		}
	}

	/*
	 * ELSE
	 */
	else {
		serial_print("Syntax error");
		serial_nl();
	}

}

void setup(void) {
	serial_parser_init();

	if (pc_mode == MONO_RESONANT) {
		pc_calculator_cmp_mono_resonant(&waveform, &htimpar);
	} else if (pc_mode == MULTI_RESONANT) {
		pc_calculator_cmp_multi_resonant(&waveform, &htimpar);
	}
	pc_update(&htimpar);

	feedback_init();
}

void loop(void) {
	serial_parser_worker(&clParser);

	if (HAL_GPIO_ReadPin(USER_BUTTON_GPIO_Port, USER_BUTTON_Pin)) {
		if (pc_output_status) {
			pc_stop();
		} else {
			pc_start();
		}
		HAL_Delay(2000);
//		pc_start();
//		HAL_Delay(1);
//		pc_stop();
	}

}
