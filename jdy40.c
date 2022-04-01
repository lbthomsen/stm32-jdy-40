/*
 * Minimal JDY-40 library
 *
 * Copyright (c) 2022 Lars Boegild Thomsen <lbthomsen@gmail.com>
 *
 */

#include "main.h"
#include "string.h"
#include "jdy40.h"

#ifdef DEBUG
#include "stdio.h"
#endif

/*
 * Internal functions
 */

/*
 * Public functions
 */
JDY40_result_t jdy40_init(JDY40_HandleTypeDef *jdy40, UART_HandleTypeDef *uart, GPIO_TypeDef *cs_port, uint16_t cs_pin, GPIO_TypeDef *set_port, uint16_t set_pin) {
	
	JDY40_DBG("JDY40 init");

	jdy40->uartHandle = uart;
	jdy40->cs_port = cs_port;
	jdy40->cs_pin = cs_pin;

	

}

/*
 * vim: ts=4 nowrap
 */
