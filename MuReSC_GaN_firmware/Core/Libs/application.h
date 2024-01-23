/**
 ******************************************************************************
 * @file         application.h
 * @brief        This file provides code serial management
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 Bologna Design.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#ifndef LIBS_APPLICATION_H_
#define LIBS_APPLICATION_H_

#include "main.h"

extern UART_HandleTypeDef hlpuart1;
extern HRTIM_HandleTypeDef hhrtim1;

void setup(void);

void loop(void);



#endif /* LIBS_APPLICATION_H_ */
