/*
 * packet_parser.c
 *
 *  Created on: Jul 28, 2026
 *      Author: nameless
 */

#include "packet_parser.h"
#include "crc8_update.h"

#define START_BYTE 0xAA

PacketParser parser = {.state = WAIT_START, .data_index = 0};

bool parser_feed (PacketParser *p, uint8_t byte) {
	switch (p->state) {
		case WAIT_START:
			if (byte == START_BYTE) {
				p->state = WAIT_LENGTH;
				p->running_crc = 0x00;
			}
			break;
		case WAIT_LENGTH:
			if (byte <= MAX_PAYLOAD_LEN) {
				p->length = byte;
				p->data_index = 0;
				p->running_crc = crc8_update(p->running_crc, byte);
				p->state = (p->length == 0) ? WAIT_CRC : READ_DATA;
			} else {
				p->state = WAIT_START;
			}
			break;

		case READ_DATA:
			p->data[p->data_index++] = byte;
			p->running_crc = crc8_update(p->running_crc, byte);

			if (p->data_index >= p->length) {
				p->state = WAIT_CRC;
			}
			break;

		case WAIT_CRC:
			p->crc = byte;

			p->state = WAIT_START;
			return (byte == p->running_crc);
	}

	return false;
}
