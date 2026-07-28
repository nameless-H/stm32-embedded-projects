/*
 * crc8.c
 *
 *  Created on: Jul 28, 2026
 *      Author: nameless
 */

#include "crc8_update.h"

uint8_t crc8_update(uint8_t crc, uint8_t byte) {
	crc ^= byte;
	for (uint8_t bit = 0; bit < 8; bit++) {
		if (crc & 0x80) {
			crc = (crc << 1) ^ 0x07;
		} else {
			crc <<= 1;
	    }
	}
	return crc;
}
