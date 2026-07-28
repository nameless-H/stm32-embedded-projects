# Bare-Metal GPIO, Timer & PWM (STM32F401)

Register-level STM32 firmware — no HAL, no CubeMX-generated init. Final implementation
is a PWM "breathing" LED effect on TIM2 Channel 2. Built incrementally: started with
an interrupt-driven blink (TIM2 update interrupt + BSRR toggle) to validate timer/GPIO
fundamentals, then extended to PWM output-compare mode for the breathing effect.
Includes a from-scratch PLL clock reconfiguration to 84MHz, with a deliberately induced
and diagnosed timing bug from the clock change.

---

## Hardware

| Component | Quantity | Connection |
|---|---|---|
| STM32F401CCU6 Black Pill | 1 | — |
| ST-Link V2 clone | 1 | SWD |
| LED | 1 | Anode → PA1 (via resistor), Cathode → GND |

---

## Build & Flash

Open in STM32CubeIDE via `File → Import → General → Existing Projects into Workspace`.
Select this folder, build, and flash via ST-Link (Run → Debug or Run).

---

## What This Demonstrates

- **Register-level GPIO** — RCC clock gating (`RCC->AHB1ENR`), MODER configuration,
  atomic pin control via BSRR instead of read-modify-write on ODR
- **Timer interrupts** — TIM2 configured via PSC/ARR for a precise 500ms period,
  NVIC-routed update interrupt, non-blocking LED toggle entirely in hardware/ISR
  (CPU free in `main()`'s loop)
- **PWM generation** — TIM2 Channel 2 in Output Compare PWM mode (CCMR1/CCER/CCR2),
  alternate function GPIO configuration (MODER + AFR), output-compare preload (OCPE)
  for glitch-free duty cycle updates, software-driven duty ramp for a breathing effect
- **PLL clock reconfiguration** — HSI (16MHz) → PLL-derived 84MHz SYSCLK, calculated
  PLLM/PLLN/PLLP by hand with the USB-clock (PLLQ) constraint considered even though
  unused, applied via STM32CubeMX's clock tree tool
- **Real debugging exercise** — after switching to 84MHz without recalculating TIM2's
  PSC, breathing speed visibly changed (~5x faster). Traced the cause to the
  STM32 APB-prescaler timer-clock doubling rule (timer clock = 2x APB clock when the
  APB prescaler is not /1), recalculated PSC from the actual 84MHz timer input, and
  restored correct timing

---

## Register Reference (for my own review)

| Peripheral | Registers touched |
|---|---|
| RCC | `AHB1ENR` (GPIOA), `APB1ENR` (TIM2) |
| GPIOA | `MODER`, `AFRL` |
| TIM2 | `PSC`, `ARR`, `CCMR1`, `CCER`, `CCR2`, `CR1`, `DIER`, `SR` |
| NVIC | `ISER0` |