/*
 * command_dispatch.c
 *
 *  Created on: Jul 28, 2026
 *      Author: nameless
 */

#include <stddef.h>
#include "command_dispatch.h"
#include "stm32f401xc.h"

typedef ResponseCode (*CommandHandler)(const uint8_t *payload, uint8_t payload_len, uint8_t *resp_data, uint8_t *resp_len);

static ResponseCode handle_led_on(const uint8_t *payload, uint8_t len, uint8_t *resp_data, uint8_t *resp_len) {
	GPIOA->BSRR = GPIO_BSRR_BS0;
	*resp_len = 0;
	return RESP_ACK;
}

static ResponseCode handle_led_off(const uint8_t *payload, uint8_t len, uint8_t *resp_data, uint8_t *resp_len) {
	GPIOA->BSRR = GPIO_BSRR_BR0;
	*resp_len = 0;
	return RESP_ACK;
}

static ResponseCode handle_led_toggle(const uint8_t *payload, uint8_t len, uint8_t *resp_data, uint8_t *resp_len) {
	GPIOA->ODR ^= GPIO_ODR_OD0;
	*resp_len = 0;
	return RESP_ACK;
}

static ResponseCode handle_ping(const uint8_t *payload, uint8_t len, uint8_t *resp_data, uint8_t *resp_len) {
	*resp_len = 0;
	return RESP_ACK;
}

static ResponseCode handle_get_status(const uint8_t *payload, uint8_t len, uint8_t *resp_data, uint8_t *resp_len) {
	if (len != 0) {
		*resp_len = 0; return RESP_NACK_BAD_PAYLOAD;
	}
	resp_data[0] = (GPIOA->ODR & GPIO_ODR_OD0) ? 0x01 : 0x00;
	*resp_len = 1;
	return RESP_ACK;
}

static CommandHandler command_table[CMD_COUNT] = {
	NULL,             // 0x00 unused
	handle_led_on,    // 0x01
    handle_led_off,   // 0x02
	handle_led_toggle,// 0x03
    handle_ping,      // 0x04
	handle_get_status // 0x05
};

ResponseCode dispatch_command(PacketParser *p, uint8_t *resp_data, uint8_t *resp_len) {
	if (p->length == 0) {
		*resp_len = 0;
		return RESP_NACK_BAD_PAYLOAD;
	}

	uint8_t cmd = p->data[0];
	if (cmd >= CMD_COUNT || command_table[cmd] == NULL) {
		*resp_len = 0;
		return RESP_NACK_UNKNOWN_CMD;
	}

	return command_table[cmd](&p->data[1], p->length -1, resp_data, resp_len);
}
