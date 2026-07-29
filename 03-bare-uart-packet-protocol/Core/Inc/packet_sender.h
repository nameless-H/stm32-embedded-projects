/*
 * packet_sender.h
 *
 *  Created on: Jul 29, 2026
 *      Author: nameless
 */

#ifndef INC_PACKET_SENDER_H_
#define INC_PACKET_SENDER_H_

#include <stdint.h>
#include "command_dispatch.h"

void send_response(ResponseCode code, const uint8_t *data, uint8_t len);

#endif /* INC_PACKET_SENDER_H_ */
