/*
 * packet_sender.c
 *
 *  Created on: Jul 29, 2026
 *      Author: nameless
 */

#include "packet_sender.h"
#include "stm32f401xc.h"
#include "crc8_update.h"

#define START_BYTE 0xAA
#define MAX_PAYLOAD_LEN 32

static void send_packet(const uint8_t *data, uint8_t len) {
	uint8_t crc = 0x00;

	while (!(USART1->SR & USART_SR_TXE)) {}
	USART1->DR = START_BYTE;

	while (!(USART1->SR & USART_SR_TXE)) {}
	USART1->DR = len;
	crc = crc8_update(crc, len);

	for (uint8_t i = 0; i < len; i++) {
		while (!(USART1->SR & USART_SR_TXE)) {}
		USART1->DR = data[i];
		crc = crc8_update(crc, data[i]);
	}

	while (!(USART1->SR & USART_SR_TXE)) {}
	USART1->DR = crc;
}

void send_response(ResponseCode code, const uint8_t *data, uint8_t len) {
    uint8_t packet[1 + MAX_PAYLOAD_LEN];
    packet[0] = (uint8_t)code;
    for (uint8_t i = 0; i < len; i++) packet[1+i] = data[i];
    send_packet(packet, 1 + len);
}
