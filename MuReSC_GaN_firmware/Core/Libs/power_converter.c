#include "main.h"
#include <math.h>
#include <stdbool.h>
#include "power_converter.h"

bool pc_output_status = 0;

//struct WaveformParams waveform = { 13560000, 40, 50 };

//struct RgstrPrmHRTIM cmp = { 4352, 96, 2054, 2272, 4230, 96, 2054, 2272, 4230 };

void pc_set_cmps(struct RgstrPrmHRTIM *params) {
	// 1
	HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_B].CMP1xR = params->A1; //96;
	HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_B].CMP2xR = params->A2; //2054;

	//2
	HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].CMP1xR = params->B1; //2272;
	HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].CMP2xR = params->B2; //4230;

	//3
	HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].CMP1xR = params->C1; //96;
	HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].CMP2xR = params->C2; //2054;

	//4
	HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_C].CMP1xR = params->D1; //2272;
	HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_C].CMP2xR = params->D2; //4230;
}

void pc_start(void) {
	pc_output_status = 1;
	HAL_HRTIM_WaveformCounterStart(&hhrtim1,
			HRTIM_TIMERID_TIMER_A + HRTIM_TIMERID_TIMER_B
					+ HRTIM_TIMERID_TIMER_C + HRTIM_TIMERID_TIMER_D);
	HAL_HRTIM_WaveformOutputStart(&hhrtim1,
			HRTIM_OUTPUT_TA1 + HRTIM_OUTPUT_TA2 + HRTIM_OUTPUT_TB1
					+ HRTIM_OUTPUT_TB2 + HRTIM_OUTPUT_TC1 + HRTIM_OUTPUT_TC2
					+ HRTIM_OUTPUT_TD1 + HRTIM_OUTPUT_TD2);
}

void pc_stop(void) {
	pc_output_status = 0;
	HAL_HRTIM_WaveformOutputStop(&hhrtim1,
			HRTIM_OUTPUT_TA1 + HRTIM_OUTPUT_TA2 + HRTIM_OUTPUT_TB1
					+ HRTIM_OUTPUT_TB2 + HRTIM_OUTPUT_TC1 + HRTIM_OUTPUT_TC2
					+ HRTIM_OUTPUT_TD1 + HRTIM_OUTPUT_TD2);
	HAL_HRTIM_WaveformCounterStop(&hhrtim1,
			HRTIM_TIMERID_TIMER_A + HRTIM_TIMERID_TIMER_B
					+ HRTIM_TIMERID_TIMER_C + HRTIM_TIMERID_TIMER_D);
}

void pc_calculator_cmp_mono_resonant(struct WaveformParams *waveform,
	struct RgstrPrmHRTIM *params) {
	// Calculate frequency as CLOCK/Frequency and is the number of tick for the register
	unsigned int period = PC_HRTIM_EQ_CLK_FRQ / waveform->frequency;

	// Check if period is even, if not make it even
	if (period % 2 != 0) {
		period++;
	}

	unsigned int dt = ceil(PC_HRTIM_EQ_CLK_FRQ * waveform->deadTime / 1e9);

	params->B2 = PC_MINIMUM_COUNTER;
	params->A1 = PC_MINIMUM_COUNTER + dt;
	params->A2 = PC_MINIMUM_COUNTER + period / 2;
	params->B1 = PC_MINIMUM_COUNTER + period / 2 + dt;
	params->C1 = params->A1;
	params->C2 = params->A2;
	params->D1 = params->B1;
	params->D2 = params->B2;

	params->period = period;

}

void pc_update(struct RgstrPrmHRTIM *params) {
//	pc_stop();

	HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].PERxR = params->period;
	HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_B].PERxR = params->period;
	HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_C].PERxR = params->period;
	HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_D].PERxR = params->period;
	pc_set_cmps(params);

//	pc_start();

	//HRTIM1->sTimerxRegs[HRTIM_TIMERINDEX_TIMER_A].TIMxCR

	// Enable update on reset timer A
	//HAL_HRTIM_UpdateDisable
	//HAL_HRTIM_UpdateEnable(&hhrtim1,HRTIM_TIMERUPDATE_A+HRTIM_TIMERUPDATE_B+HRTIM_TIMERUPDATE_C+HRTIM_TIMERUPDATE_D);
}
