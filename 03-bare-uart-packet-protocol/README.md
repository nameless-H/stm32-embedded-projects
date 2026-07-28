# Bare-Metal UART Packet Protocol (STM32F401)

Register-level USART1 implementation of a framed packet protocol with CRC-8
validation, ring-buffer-backed ISR reception, and an incremental parser FSM.
No HAL — RCC, GPIO alternate function, BRR baud rate math, and TXE/RXNE-gated
transmit/receive are all done by hand.

**Status: framing + CRC validation working. Command dispatch in progress.**

---

## Hardware

| Component | Quantity | Connection |
|---|---|---|
| STM32F401CCU6 Black Pill | 1 | — |
| ST-Link V2 clone | 1 | SWD |
| CP2102 USB-to-UART module | 1 | TX → PA10, RX → PA9, GND common |

---

## Protocol Spec

`[START_BYTE][LENGTH][DATA...][CRC]`
- `START_BYTE` = `0xAA`
- `LENGTH` = number of DATA bytes (0–`MAX_PAYLOAD_LEN`)
- `CRC` = CRC-8 (polynomial `0x07`), computed incrementally over `LENGTH + DATA`

Malformed packets (length exceeds max, or CRC mismatch) are silently discarded and
the parser resets to wait for the next start byte — no partial/corrupt data reaches
the application layer.

---

## Build & Flash

Open in STM32CubeIDE via `File → Import → General → Existing Projects into Workspace`.
Select this folder, build, and flash via ST-Link.

Clock config: 84MHz SYSCLK via PLL (same derivation as Project 1).

**PC-side testing:** Python + PySerial script (not included in this repo yet) sends
framed packets and verifies the echoed payload matches. Baud rate: 115200.

---

## What This Demonstrates

- **USART register-level config** — RCC clock gating (APB2 for USART1), GPIO
  alternate function setup (MODER + AFR, AF7) for PA9/PA10, BRR baud rate calculated
  by hand (mantissa/fraction split from `USARTDIV = f_PCLK / (16 × baud)`)
- **Interrupt-driven RX** — RXNEIE + NVIC-enabled USART1 interrupt, ISR only ever
  pushes into a ring buffer, never blocks or processes in interrupt context
- **Ring buffer (producer-consumer pattern)** — fixed-size circular buffer with
  head/tail indices, "sacrifice one slot" full-detection, `main()` polls via `pop()`
  fully decoupled from the ISR
- **Parser FSM** — `WAIT_START → WAIT_LENGTH → READ_DATA → WAIT_CRC`, resyncs to the
  next start byte on any framing/CRC failure rather than getting stuck
- **Incremental CRC-8** — computed byte-by-byte as the frame streams in, rather than
  buffering the whole frame first, matching how CRC is done in real UART/DMA hardware

---

## Register Reference (for my own review)

| Peripheral | Registers touched |
|---|---|
| RCC | `AHB1ENR` (GPIOA), `APB2ENR` (USART1) |
| GPIOA | `MODER`, `AFR[1]` |
| USART1 | `BRR`, `CR1`, `SR`, `DR` |
| NVIC | `USART1_IRQn` |