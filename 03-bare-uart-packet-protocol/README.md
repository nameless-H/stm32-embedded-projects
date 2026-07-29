# Bare-Metal UART Packet Protocol (STM32F401)

Register-level USART1 implementation of a framed packet protocol with CRC-8
validation, ring-buffer-backed ISR reception, an incremental parser FSM, and
command dispatch with a structured, bidirectional response format. No HAL — RCC,
GPIO alternate function, BRR baud rate math, and TXE/RXNE-gated transmit/receive
are all done by hand.

---

## Hardware

| Component | Quantity | Connection |
|---|---|---|
| STM32F401CCU6 Black Pill | 1 | — |
| ST-Link V2 clone | 1 | SWD |
| CP2102 USB-to-UART module | 1 | TX → PA10, RX → PA9, GND common |
| LED | 1 | Anode → PA0 (via resistor), Cathode → GND |

---

## Protocol Spec

### Frame format (both directions)

`[START_BYTE][LENGTH][DATA...][CRC]`
- `START_BYTE` = `0xAA`
- `LENGTH` = number of DATA bytes (0–`MAX_PAYLOAD_LEN`)
- `CRC` = CRC-8 (polynomial `0x07`), computed incrementally over `LENGTH + DATA`

Malformed packets (length exceeds max, or CRC mismatch) are silently discarded and
the parser resets to wait for the next start byte.

### Command payload (host → device)

`DATA = [COMMAND_BYTE][ARGS...]`

| Command | Byte | Args |
|---|---|---|
| `CMD_LED_ON` | `0x01` | none |
| `CMD_LED_OFF` | `0x02` | none |
| `CMD_LED_TOGGLE` | `0x03` | none |
| `CMD_PING` | `0x04` | none |
| `CMD_GET_STATUS` | `0x05` | none |

### Response payload (device → host)

`DATA = [RESPONSE_CODE][RESPONSE_DATA...]`

| Response Code | Byte | Meaning |
|---|---|---|
| `RESP_ACK` | `0x00` | Command executed successfully |
| `RESP_NACK_UNKNOWN_CMD` | `0x01` | Command byte not recognized |
| `RESP_NACK_BAD_PAYLOAD` | `0x02` | Empty or malformed payload |

`RESPONSE_DATA` is empty for all commands except `CMD_GET_STATUS`, which returns
1 byte: current PA0 (LED) output state (`0x00`/`0x01`).

Every dispatched command produces exactly one response packet — response
construction is centralized in `dispatch_command()`/`main()`, not left to
individual command handlers, so no command path can silently fail to respond.

---

## Build & Flash

Open in STM32CubeIDE via `File → Import → General → Existing Projects into Workspace`.
Select this folder, build, and flash via ST-Link.

Clock config: 84MHz SYSCLK via PLL (same derivation as Project 1).

**PC-side testing:** `test_uart.py` (Python + PySerial) constructs and sends a
framed command packet, prints the decoded response. Baud rate: 115200.

---

## What This Demonstrates

- **USART register-level config** — RCC clock gating (APB2 for USART1), GPIO
  alternate function setup (MODER + AFR, AF7) for PA9/PA10, BRR baud rate calculated
  by hand (mantissa/fraction split from `USARTDIV = f_PCLK / (16 x baud)`)
- **Interrupt-driven RX** — RXNEIE + NVIC-enabled USART1 interrupt, ISR only ever
  pushes into a ring buffer, never blocks or processes in interrupt context
- **Ring buffer (producer-consumer pattern)** — fixed-size circular buffer,
  head/tail indices, "sacrifice one slot" full-detection, `main()` fully decoupled
  from the ISR via `pop()`
- **Parser FSM** — `WAIT_START → WAIT_LENGTH → READ_DATA → WAIT_CRC`, resyncs to
  the next start byte on any framing/CRC failure
- **Incremental CRC-8** — computed byte-by-byte as the frame streams in, matching
  how CRC is done in real UART/DMA hardware, rather than buffering the whole frame
  first
- **Command pattern dispatch** — function-pointer lookup table indexed by command
  byte, each handler taking payload in and writing response data out via output
  parameters, keeping the actual "send" call centralized in one place
- **Structured, bidirectional protocol** — same framing/CRC scheme used for both
  host→device commands and device→host responses, including a real hardware state
  query (`GET_STATUS` reading live GPIO state), not just command/ack

---

## Register Reference (for my own review)

| Peripheral | Registers touched |
|---|---|
| RCC | `AHB1ENR` (GPIOA), `APB2ENR` (USART1) |
| GPIOA | `MODER`, `AFR[1]`, `BSRR`, `ODR` |
| USART1 | `BRR`, `CR1`, `SR`, `DR` |
| NVIC | `USART1_IRQn` |