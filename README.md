# Airride Control System

A complete hardware and software suite for controlling vehicle air suspension with a 4-way solenoid valve.

## Project Overview

This repository contains two embedded projects plus shared utilities:
- `AirrideGui`  — ESP32-based touchscreen UI (TFT display)
- `AirrideController` — Arduino R4 Minima-based control firmware (solenoid + sensor management)
- `SharedLibraries` — common communication, CAN/serial, and utility libraries

Main capabilities:
- touchscreen control and visualization of suspension state
- pressure sensing and safety limit handling
- 4-way solenoid lift/lower control
- serial/CAN communication between GUI and controller

## Repository Structure

- `AirrideGui/`
  - `include/` and `lib/` UI modules
  - `src/main.cpp` application entry
  - `platformio.ini` ESP32 environment definitions

- `AirrideController/`
  - `include/` and `lib/` controller modules
  - `src/main.cpp` controller entry
  - `platformio.ini` Arduino R4 environments

- `SharedLibraries/`
  - reusable libs for `CanBus`, `CanMessage`, `Communication`, `ICanBus`, logging, etc.

## Hardware Architecture

UI Controller (`AirrideGui`)
- Board: ESP32-2432S028R (CYD)

System Controller (`AirrideController`)
- MCU: Arduino R4 Minima (AVR32)
- Shield: 4-Relay shield
- Solenoid drivers: 4 regulators/relays for 4-way control
- Pressure sensors: analog/digital input monitoring

## Features

- Touch-enabled runtime UI
- Real-time pressure and state monitoring
- Front/rear lift control
- Height presets, calibration, and safety limits
- Persistent settings via storage
- Log and debug output over serial

## Build & Upload (PlatformIO)

Requirements:
- VS Code with PlatformIO extension (recommended)
- PlatformIO CLI (`pip install platformio` optional)

Build:

```bash
cd AirrideGui
pio run

cd ../AirrideController
pio run
```

Upload:

```bash
cd AirrideGui
pio run -t upload

cd ../AirrideController
pio run -t upload
```

If specific board environments are needed, inspect `platformio.ini` for environment names.

## Communication Protocol

- GUI ↔ controller over CAN-Bus
- Shared protocol code in `SharedLibraries/Communication/`
- For message IDs and formats, review `SharedLibraries/CanMessage*/`


## Status

Basic functionality implemented and operational; ongoing work for:
- complete CAN integration
- auto-leveling and learning behavior
- tuning and hardware testing
- create custom hardware

---

**Built with:** PlatformIO, C++, TFT_eSPI, Arduino Framework

