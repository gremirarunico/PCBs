/**
 ******************************************************************************
 * @file         serial_parser.c
 * @brief        This file provides code serial management
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

#include "serial_parser.h"
#include "main.h"
#include <stdbool.h>
#include <string.h>

/**
 * Buffers
 */
char swap_buffer; // single char buffer, used by the serial
struct UartBuffer serial_buffer; // array of chars, used internally
bool command_ready = 0; // we are ready to parse our command and to execute it

/**
 * @fn void serial_parser_init(void)
 * @brief this function load the necessary things to run this library. You have to run once before while loop
 *
 * @pre
 * @post
 */
void serial_parser_init() {
	serial_buffer.index = 0;
	HAL_UART_Receive_IT(&hlpuart1, (uint8_t*) &swap_buffer, 1);
}

/**
 * @fn void serial_parser_worker(void(*)(void))
 * @brief this function do the not sync job, bust be put in the main loop
 *
 * @pre
 * @post
 * @param external_command_worker it's a void function, the function that manages the the actions
 */
void serial_parser_worker(void (*external_command_worker)(void)) {
	// We can parse the command
	if (command_ready) {
		external_command_worker();
		command_ready = 0;
	}
}

/**
 * @fn void serial_parser_received_command(void)
 * @brief decide what to do when a command is received
 *
 * @pre
 * @post
 */
void serial_parser_received_command(void) {
//	serial_print(serial_buffer.buffer);
	serial_buffer.index = 0;
	command_ready = 1;
}

/**
 * @fn void serial_buffer_overflow(void)
 * @brief This function manage what append in case of buffer overflow (simply prints an error and reset buffer)
 *
 * @pre
 * @post
 */
void serial_buffer_overflow(void) {
	serial_buffer.index = 0;
	serial_buffer.buffer[0] = '\0';

	serial_print("ERROR: buffer overflow, ignoring input...\t");
}

/**
 * @fn void serial_print(char*)
 * @brief Manage the serial output
 *
 * @pre
 * @post
 * @param outString
 */
void serial_print(char *outString) {
	//HAL_UART_Transmit_IT(&hlpuart1, outString, strlen(outString));
	HAL_UART_Transmit(&hlpuart1, (uint8_t*)outString, strlen(outString), 10);
}

bool serial_is_command(char *command, unsigned int position) {
	// Get the index beginning in the string of the command in the position position
	unsigned int startIndex = 0;
	for (unsigned int p = 0; p < position; p++) {
		// While I don't get the separator command and we are not running out of our array, increment
		while (serial_buffer.buffer[startIndex] != SP_SEPARATOR_COMMAND
				&& startIndex < SP_LOCAL_BUFFER_SIZE) {
			// If we get end of string we have no chance
			if (serial_buffer.buffer[startIndex] == '\0')
				return false;
			startIndex++;
		}
		// Go ahead of one (we are on the SEPARATOR)
		startIndex++;
	}

	// Check that the residual length of the string is sufficient to contain the command
	if (SP_LOCAL_BUFFER_SIZE < (startIndex + 1) + strlen(command))
		return false;

	// Check if the command is corresponding
	for (int i = 0; i < strlen(command); i++) {
		if (serial_buffer.buffer[startIndex + i] != command[i]) {
			return false;
		}
	}

	// If no check failed they must be correct
	return true;
}

/**
 * @fn void HAL_UART_RxCpltCallback(UART_HandleTypeDef*)
 * @brief Manage the interrupt, so it copy the buffer
 *
 * @pre
 * @post
 * @param huart
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

// If I received an end command character we have done, we can parse the command
	if (swap_buffer == SP_END_COMMAND) {
		serial_buffer.buffer[serial_buffer.index] = '\0';
		serial_parser_received_command();
	}
// otherwise we have to add it to our local buffer
	else {
		// We skip tabs or null chars or new-line in our buffer
		if (swap_buffer != '\r' && swap_buffer != '\0' && swap_buffer != '\n') {

			// If not buffer overflow
			if (serial_buffer.index < SP_LOCAL_BUFFER_SIZE) {
				// Save the char in buffer and increment index
				serial_buffer.buffer[serial_buffer.index] = swap_buffer;
				serial_buffer.index += 1;
			} else {
				// We are in overflow
				serial_buffer_overflow();
			}

		}
	}

// Restart listening
	HAL_UART_Receive_IT(&hlpuart1, (uint8_t*) &swap_buffer, 1);
}

