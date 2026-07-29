# STM32 Embedded Projects

Bare-metal and HAL STM32F401 firmware projects — register-level GPIO/timers/PWM,
UART packet protocol with CRC, and ongoing peripheral drivers, built as Stage 4
of a self-directed embedded systems roadmap targeting STM32/ARM Cortex-M firmware
engineering.

Companion repo: [arduino-embedded-fundamentals](link-to-that-repo) (Stage 3).

---

## Environment

- **Board:** STM32F401CCU6 Black Pill
- **Debugger:** ST-Link V2 clone (SWD)
- **IDE:** STM32CubeIDE + STM32CubeMX (standalone, decoupled since CubeIDE 2.0.0)
- **OS:** Fedora Workstation 44

---

## Projects

| # | Project | Concepts |
|---|---------|----------|
| 01 | [Bare-Metal GPIO, Timer & PWM](./01-bare-gpio-timer-pwm/) | RCC clock gating, MODER/BSRR, TIM2 interrupt-driven blink, PWM output-compare breathing, PLL clock reconfiguration to 84MHz with induced/diagnosed timing bug |
| 02 | [HAL PWM Breathing](./02-hal-pwm-breathing/) | CubeMX/HAL rebuild of Project 1 for direct register-vs-HAL comparison |
| 03 | [Bare-Metal UART Packet Protocol](./03-bare-uart-packet-protocol/) | USART register config, BRR baud rate math, interrupt-driven RX, ring buffer, framed packet parser FSM, incremental CRC-8, command dispatch |

---

## Roadmap Progress

- [x] Bare-metal GPIO + timer interrupts
- [x] PWM (bare-metal + HAL comparison)
- [x] PLL clock configuration
- [x] UART packet protocol + CRC validation
- [x] Command dispatch
- [ ] ADC + DMA multi-channel sampler
- [ ] PWM servo controller (UART command interface)
- [ ] I2C sensor dashboard
- [ ] SPI flash read/write

---

## Key Principles

- Bare-metal register-level implementation first, HAL rebuild second — every HAL
  call is traced back to the register writes it replaces before moving on
- Datasheet-first: RM0368 is the source of truth for every register/bit-field,
  not assumptions or copied snippets
- Deliberately induced and diagnosed real bugs (clock/timing mismatches, CRC
  corruption scenarios) rather than only building the happy path
