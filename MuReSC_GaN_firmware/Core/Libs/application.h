/**
 ******************************************************************************
 * @file         application.h
 * @brief        Header for application.c file.
  *              This file contains the common defines for the application.
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

#ifndef LIBS_APPLICATION_H_
#define LIBS_APPLICATION_H_

#include "main.h"

extern UART_HandleTypeDef hlpuart1;
extern HRTIM_HandleTypeDef hhrtim1;

/**
 * @fn void setup(void)
 * @brief run one for the setup of additional peripheral not provided by CubeMX
 *
 * @pre
 * @post
 */
void setup(void);

/**
 * @fn void loop(void)
 * @brief the infinite loop (inside while(1) in the main)
 *
 * @pre
 * @post
 */
void loop(void);



#endif /* LIBS_APPLICATION_H_ */
