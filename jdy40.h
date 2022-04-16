/*
 * Minimal w25qxx library
 *
 * Copyright (c) 2022 Lars Boegild Thomsen <lbthomsen@gmail.com>
 */

#ifndef JDY_40_H_
#define JDY_40_H_

#ifdef DEBUG
#define JDY40_DBG(...) printf(__VA_ARGS__);\
                     printf("\n");
#else
#define JDY40_DBG(...) ;
#endif

typedef struct {
	UART_HandleTypeDef *uartHandle;
	GPIO_TypeDef *cs_port;
	uint16_t cs_pin;
	GPIO_TypeDef *set_port;
	uint16_t set_pin;
} JDY40_HandleTypeDef;

typedef enum {
	JDY40_Ok,
    JDY40_Err,
    JDY40_Timeout
} JDY40_result_t;

typedef enum {
	JDY40_Idle,
	JDY40_Receiving
} JDY40_state_t;

JDY40_result_t jdy40_init(JDY40_HandleTypeDef *jdy40_init, UART_HandleTypeDef *uart, GPIO_TypeDef *cs_port, uint16_t cs_pin, GPIO_TypeDef *set_port, uint16_t set_pin);

#endif /* JDY_40_H_ */
