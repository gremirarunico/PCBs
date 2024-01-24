/**
  ******************************************************************************
  * @file           : serial_parser.h
  * @brief          : Header for serial_parser.c file.
  *                   This file contains the common defines for serial management.
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


#ifndef LIBS_SERIAL_PARSER_H_
#define LIBS_SERIAL_PARSER_H_

#define SP_LOCAL_BUFFER_SIZE 20
#define SP_END_COMMAND '\r'
#define SP_SEPARATOR_COMMAND ' '

#include "main.h"
#include <stdbool.h>

extern UART_HandleTypeDef hlpuart1;
//extern struct UartBuffer serial_buffer;



/**
 * Buffer
 */
struct UartBuffer{
	char buffer[SP_LOCAL_BUFFER_SIZE];
	uint8_t index;
};

/**
 * @fn void serial_parser_init(void)
 * @brief this function load the necessary things to run this library. You have to run once before while loop
 *
 * @pre
 * @post
 */
void serial_parser_init(void);

/**
 * @fn void serial_paresr_worker(void)
 * @brief this function do the not sync job, bust be put in the main loop
 *
 * @pre
 * @post
 */
void serial_parser_worker(void (*external_command_worker)(void));

/**
 * @fn void serial_parser_received_command(void)
 * @brief
 *
 * @pre
 * @post
 */
void serial_parser_received_command(void);

/**
 * @fn void serial_buffer_overflow(void)
 * @brief
 *
 * @pre
 * @post
 */
void serial_buffer_overflow(void);

void serial_print(char *outString);

bool serial_is_command(char *command, unsigned int position);

/**
 * @fn void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef*, uint16_t)
 * @brief overwrite the __week function to manage the interruput of the serial
 *
 * @pre
 * @post
 * @param huart
 * @param Size
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t sieze);

#endif /* LIBS_SERIAL_PARSER_H_ */
