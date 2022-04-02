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

char buf[128];

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

	sprintf(&buf, "AT%s\r\n", cmd);

	JDY40_DBG("Sending command: %s", buf);

	HAL_UART_Transmit(jdy40->uartHandle, buf, strlen(buf) + 4, 100);

	sprintf(&buf, "");
	char ch;

	while (ch != 10) {
		HAL_UART_Receive(jdy40->uartHandle, &ch, 1, 10);
		sprintf(&buf, "%s%c", buf, ch);
	}

	JDY40_DBG("Received: %s", buf);

	return JDY40_Ok;
}

JDY40_result_t jdy40_send_command(JDY40_HandleTypeDef *jdy40) {

}

JDY40_result_t jdy40_receive_response(JDY40_HandleTypeDef *jdy40) {

}

void uart_rx_cb() {
	JDY40_DBG("JDY40 RX CB");

}

/*
 * Public functions
 */
JDY40_result_t jdy40_init(JDY40_HandleTypeDef *jdy40, UART_HandleTypeDef *uart, GPIO_TypeDef *cs_port, uint16_t cs_pin, GPIO_TypeDef *set_port, uint16_t set_pin) {
	
	JDY40_result_t result = JDY40_Ok;

	JDY40_DBG("JDY40 init");

	jdy40->uartHandle = uart;
	jdy40->cs_port = cs_port;
	jdy40->cs_pin = cs_pin;
	jdy40->set_port = set_port;
	jdy40->set_pin = set_pin;

	// Switch off for 1 ms
	jdy40_off(jdy40);
	HAL_Delay(1);
	jdy40_on(jdy40);
	HAL_Delay(1);

	jdy40_aton(jdy40);

	jdy40_send_receive(jdy40, "+BAUD");

	//buf[0] = 0;

	// Let us register the RX callback
	HAL_UART_RegisterCallback(uart, HAL_UART_RX_COMPLETE_CB_ID, uart_rx_cb);

	return result;

}

/*
 * vim: ts=4 nowrap
 */
