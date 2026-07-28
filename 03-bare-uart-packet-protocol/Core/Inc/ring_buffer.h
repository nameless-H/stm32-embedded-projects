/*
 * ring_buffer.h
 *
 *  Created on: Jul 27, 2026
 *      Author: nameless
 */

#ifndef INC_RING_BUFFER_H_
#define INC_RING_BUFFER_H_

#include <stdint.h>
#include <stdbool.h>

#define BUFFER_SIZE 64

typedef struct {
    uint8_t buffer[BUFFER_SIZE];
    volatile uint8_t head;
    volatile uint8_t tail;
} RingBuffer;

extern RingBuffer rx_buff;   // declares that this variable exists somewhere, doesn't create it here

void push(uint8_t byte);
bool pop(uint8_t *out);

#endif /* INC_RING_BUFFER_H_ */
