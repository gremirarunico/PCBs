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
 * @brief load the necessary things to run this library. You have to run once before while loop
 *
 * @pre
 * @post
 */
void serial_parser_init(void);

/**
 * @fn void serial_paresr_worker(void)
 * @brief do the not sync job, bust be put in the main loop
 *
 * @pre
 * @post
 */
void serial_parser_worker(void (*external_command_worker)(void));

/**
 * @fn void serial_parser_received_command(void)
 * @brief internal, to manage when a command is ready to be parsed, very fast (managed in the interrupt)
 *
 * @pre
 * @post
 */
void serial_parser_received_command(void);

/**
 * @fn void serial_buffer_overflow(void)
 * @brief this is an internal function to manage what you have to do if the internal buffer is going to overflow
 *
 * @pre
 * @post
 */
void serial_buffer_overflow(void);

/**
 * @fn void serial_print(char*)
 * @brief print in the serial (blocking)
 *
 * @pre
 * @post
 * @param outString
 */
void serial_print(char *outString);

/**
 * @fn bool serial_is_command(char*, unsigned int)
 * @brief check if in the posizion position there is the command command
 *
 * @pre
 * @post
 * @param command the command to check
 * @param position where you have to find it
 * @return true if the command is correct, false otherwise
 */
bool serial_is_command(char *command, unsigned int position);

/**
 * @fn bool serial_get_int(unsigned int, int*)
 * @brief get an int form the command line in the posizion position. Return true if everything is ok and store the value in valueReturn
 *
 * @pre
 * @post
 * @param position the position where you have to search for the int
 * @param valueReturn pointer where the command store the int found
 * @return bool, true if the conversion succeeded, false if it has failed
 */
bool serial_get_int(unsigned int position, int *valureReturn);

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
