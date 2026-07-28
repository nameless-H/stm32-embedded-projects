/*
 * crc8.h
 *
 *  Created on: Jul 28, 2026
 *      Author: nameless
 */

#ifndef INC_CRC8_UPDATE_H_
#define INC_CRC8_UPDATE_H_

#include <stdint.h>

/**
 * @brief  Calculates CRC-8 (SMBus polynomial 0x07) over a given data buffer.
 * @param  data: Pointer to byte array
 * @param  len:  Number of bytes to process
 * @return Computed 8-bit CRC value
 */
uint8_t crc8_update(uint8_t crc, uint8_t byte);

#endif /* INC_CRC8_UPDATE_H_ */
