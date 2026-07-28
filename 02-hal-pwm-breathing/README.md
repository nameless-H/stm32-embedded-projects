# HAL PWM Breathing (STM32F401)

CubeMX/HAL rebuild of Project 1's PWM breathing effect, built for direct comparison
against the bare-metal register-level version. Same TIM2 Channel 2 PWM output on PA1,
same 84MHz PLL clock config, same breathing behavior — implemented via HAL calls
instead of raw register writes.

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

Clock config: 84MHz SYSCLK via PLL (HSI source, PLLM=8, PLLN=168, PLLP=4) — same
derivation as Project 1's bare-metal version.

---

## What This Demonstrates

- **CubeMX-generated peripheral config** — PWM Generation CH2 selected via CubeMX's
  pinout tool, auto-generates `MX_TIM2_Init()` and `HAL_TIM_MspPostInit()` for the
  GPIO alternate-function setup
- **Direct comparison against Project 1's bare-metal implementation** — traced every
  HAL call back to the equivalent register write done by hand in Project 1
  (`HAL_TIM_PWM_Init` → PSC/ARR, `HAL_TIM_PWM_ConfigChannel` → CCMR1/CCER,
  `__HAL_TIM_SET_COMPARE` → direct CCR2 write)
- **Gaps CubeMX's defaults left unfilled** — `HAL_TIM_PWM_Start()` is not
  auto-generated and had to be added manually; output-compare preload (OCPE) was left
  disabled by CubeMX's default `TIM_OC_InitTypeDef`, differing from the bare-metal
  version's explicit preload enable
- **Same breathing loop logic**, reimplemented using `__HAL_TIM_SET_COMPARE()` instead
  of a direct `TIM2->CCR2` write

---

## Register-to-HAL Mapping (for my own review)

| Bare-metal (Project 1) | HAL equivalent |
|---|---|
| `RCC->AHB1ENR \|= (1<<0)` | `__HAL_RCC_GPIOA_CLK_ENABLE()` |
| `RCC->APB1ENR \|= (1<<0)` | Handled inside `HAL_TIM_PWM_Init()` |
| `TIM2->PSC`, `TIM2->ARR` | `htim2.Init.Prescaler`, `htim2.Init.Period` |
| `TIM2->CCMR1` (PWM mode 1) | `sConfigOC.OCMode = TIM_OCMODE_PWM1` |
| `TIM2->CCER` (channel enable) | Set inside `HAL_TIM_PWM_ConfigChannel()` |
| `TIM2->CR1` (start) | `HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2)` |
| `TIM2->CCR2 = duty` | `__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, duty)` |