/*
 * packet_parser.h
 *
 *  Created on: Jul 28, 2026
 *      Author: nameless
 */

#ifndef INC_PACKET_PARSER_H_
#define INC_PACKET_PARSER_H_

#include <stdint.h>
#include <stdbool.h>

#define MAX_PAYLOAD_LEN 32

typedef enum {
    WAIT_START,
    WAIT_LENGTH,
    READ_DATA,
    WAIT_CRC
} ParserState;

typedef struct {
	ParserState state;
    uint8_t length;
    uint8_t data[MAX_PAYLOAD_LEN];
    uint8_t crc;
    uint8_t data_index;
    uint8_t running_crc;
} PacketParser;

extern PacketParser parser;

bool parser_feed(PacketParser *p, uint8_t byte);

#endif /* INC_PACKET_PARSER_H_ */
