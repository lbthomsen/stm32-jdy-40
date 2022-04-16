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
 *
 */

JDY40_HandleTypeDef *jdy40;
JDY40_state_t jdy40_state = JDY40_Idle;

char buf[128];
uint8_t rx_buf[2];
char rx_string[128];

/*
 * Internal functions
 */

inline void jdy40_on(JDY40_HandleTypeDef *jdy40) {
	HAL_GPIO_WritePin(jdy40->cs_port, jdy40->cs_pin, GPIO_PIN_RESET);
}

inline void jdy40_off(JDY40_HandleTypeDef *jdy40) {
	HAL_GPIO_WritePin(jdy40->cs_port, jdy40->cs_pin, GPIO_PIN_SET);
}

inline void jdy40_aton(JDY40_HandleTypeDef *jdy40) {
	HAL_GPIO_WritePin(jdy40->set_port, jdy40->set_pin, GPIO_PIN_RESET);
}

inline void jdy40_atoff(JDY40_HandleTypeDef *jdy40) {
	HAL_GPIO_WritePin(jdy40->set_port, jdy40->set_pin, GPIO_PIN_SET);
}

JDY40_result_t jdy40_send_receive(JDY40_HandleTypeDef *jdy40, char *cmd) {

	sprintf(buf, "AT%s\r\n", cmd);

	JDY40_DBG("Sending command: %s", buf);

	HAL_UART_Transmit(jdy40->uartHandle, (uint8_t *)buf, strlen(buf), 100);

//	buf[0] = (char)0;
//	uint8_t ch;
//
//	while (ch != 10) {
//		HAL_StatusTypeDef res = HAL_UART_Receive(jdy40->uartHandle, (uint8_t *)&ch, 1, 100);
//		if (res == HAL_OK) {
//			strncat(buf, (char *)&ch, 1);
//		}
//	}
//
//	JDY40_DBG("Received: %s", buf);

	return JDY40_Ok;
}

void uart_string(char *s) {
	JDY40_DBG("JDY40 string received: %s", rx_string);
	rx_string[0] = 0;
}

void uart_rx(uint8_t rx) {
	JDY40_DBG("JDY40 RX - jdy40 is 0x%02x (rx_string = %s)", rx, rx_string);
	switch (rx) {
	case 0:
	case 0x0d:
		break;
	case 0x0a:
		uart_string(rx_string);
		jdy40_state = JDY40_Idle;
		break;
	default:
		jdy40_state = JDY40_Receiving;
		strncat(rx_string, (char *)&rx, 1);
 	}
}

void uart_rx_complete_cb() {
	//JDY40_DBG("JDY40 COMP CB - jdy40 is 0x%02x (%c)", rx_buf[1], rx_buf[1]);
	uart_rx(rx_buf[1]);
}

void uart_rx_half_cb() {
	//JDY40_DBG("JDY40 HALF CB - jdy40 is 0x%02x (%c)", rx_buf[0], rx_buf[0]);
	uart_rx(rx_buf[0]);
}

void uart_rx_error_cb() {
	JDY40_DBG("JDY40 ER CB");

}

JDY40_result_t jdy40_waitfor(char *s, uint32_t timeout) {
	JDY40_result_t res = JDY40_Ok;

	uint32_t start = HAL_GetTick();


	return res;
}

/*
 * Public functions
 */
JDY40_result_t jdy40_init(JDY40_HandleTypeDef *jdy40_init, UART_HandleTypeDef *uart, GPIO_TypeDef *cs_port, uint16_t cs_pin, GPIO_TypeDef *set_port, uint16_t set_pin) {
	
	JDY40_result_t result = JDY40_Ok;

	JDY40_DBG("JDY40 init");

	jdy40 = jdy40_init;

	jdy40->uartHandle = uart;
	jdy40->cs_port = cs_port;
	jdy40->cs_pin = cs_pin;
	jdy40->set_port = set_port;
	jdy40->set_pin = set_pin;

	HAL_UART_RegisterCallback(jdy40->uartHandle, HAL_UART_RX_COMPLETE_CB_ID, uart_rx_complete_cb);
	HAL_UART_RegisterCallback(jdy40->uartHandle, HAL_UART_RX_HALFCOMPLETE_CB_ID, uart_rx_half_cb);
	HAL_UART_RegisterCallback(jdy40->uartHandle, HAL_UART_ERROR_CB_ID, uart_rx_error_cb);

	HAL_UART_Receive_DMA(jdy40->uartHandle, rx_buf, sizeof(rx_buf));

	jdy40_aton(jdy40);
	jdy40_on(jdy40);


	result = jdy40_waitfor("Wake", 1000);

	HAL_Delay(100);

	jdy40_send_receive(jdy40, "+BAUD");

	//buf[0] = 0;

	// Let us register the RX callback


	return result;

}

/*
 * vim: ts=4 nowrap
 */
