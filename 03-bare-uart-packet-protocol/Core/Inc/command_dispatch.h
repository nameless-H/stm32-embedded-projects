/*
 * command_dispatch.h
 *
 *  Created on: Jul 28, 2026
 *      Author: nameless
 */

#ifndef INC_COMMAND_DISPATCH_H_
#define INC_COMMAND_DISPATCH_H_

#include <stdint.h>
#include "packet_parser.h"

#define CMD_LED_ON    0x01
#define CMD_LED_OFF   0x02
#define CMD_LED_TOGGLE 0x03
#define CMD_PING      0x04
#define CMD_GET_STATUS 0x05
#define CMD_COUNT     0x06

typedef enum {
	RESP_ACK = 0x00,
	RESP_NACK_UNKNOWN_CMD = 0x01,
	RESP_NACK_BAD_PAYLOAD = 0x02,
} ResponseCode;

ResponseCode dispatch_command(PacketParser *p, uint8_t *resp_data, uint8_t *resp_len);

#endif /* INC_COMMAND_DISPATCH_H_ */
