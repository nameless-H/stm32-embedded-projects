/*
 * ring_buffer.c
 *
 *  Created on: Jul 27, 2026
 *      Author: nameless
 */

#include "ring_buffer.h"

RingBuffer rx_buff = {.head = 0, .tail = 0};

void push(uint8_t byte) {
    uint8_t nextHead = (rx_buff.head + 1) % BUFFER_SIZE;
    if (nextHead != rx_buff.tail) {
        rx_buff.buffer[rx_buff.head] = byte;
        rx_buff.head = nextHead;
    }
}

bool pop(uint8_t *out) {
    if (rx_buff.head != rx_buff.tail) {
        *out = rx_buff.buffer[rx_buff.tail];
        rx_buff.tail = (rx_buff.tail + 1) % BUFFER_SIZE;
        return true;
    } else {
        return false;
    }
}



