# Kurzweil PC2 76-Key MIDI Controller

This repository is for a **Kurzweil PC2 76-key keybed converted into a standalone MIDI controller using one Arduino Mega 2560 and a custom shield**.

The hardware and firmware in this repository are specifically for the PC2 keybed, its original pitch/mod wheel assembly, the two illuminated octave-shift buttons located directly above the pitch and mod wheels (**SW4 = Octave Down, SW5 = Octave Up**), and a 1/4-inch sustain-pedal input.

## Implemented functions

- All 76 PC2 keys with velocity
- Original Bass and Treble keybed ribbons
- Pitch wheel
- Mod wheel
- Octave down / normal / octave up using the original illuminated buttons above the wheels
- SW4 = Octave Down; SW5 = Octave Up
- Original SW4/SW5 button LEDs
- 1/4-inch TS sustain pedal
- Automatic sustain-pedal polarity detection at power-up
- PC2 PRESSR/aftertouch routed to A3 for future use

Aftertouch MIDI output is intentionally not enabled yet because the original pressure strip is still being evaluated for consistent mechanical response.

## Final Arduino Mega 2560 pinout

| Function | Mega pin(s) |
|---|---|
| J8 Bass ribbon | D22-D37 |
| J9 Treble pins 1-12 | D38-D49 |
| J9 Treble pins 13-20 | D2-D9 |
| Octave Down button | D10 |
| Octave Up button | D11 |
| Octave Down LED | D12 |
| Octave Up LED | D13 |
| Pitch | A0 |
| Mod | A1 |
| Sustain | A2 |
| Aftertouch / PRESSR | A3 |

### Reserved interfaces

- D14-D15: Serial3
- D16-D17: Serial2
- D18-D19: Serial1
- D20-D21: I2C
- D50-D53: SPI
- A4-A15: available

D0/D1 remain the primary MIDI serial interface.

## Keybed scanner settings

- 76 keys
- First MIDI note: 28
- Last MIDI note: 103
- Velocity timing range: 1800-80000 us
- Matrix settling delay: 3 us

The Bass and Treble halves of the PC2 keybed use opposite BR/MK contact order. The included `models/kurzweil_pc2/pins.h` already accounts for this. Do not globally reverse contact order in `states.cpp`.

## Controls

### Pitch
PC2 wheel connector pin 2 -> A0.

### Mod
PC2 wheel connector pin 3 -> A1. The measured PC2 mod-wheel output is approximately 0.008-3.6 V, so the firmware maps its useful ADC range to the full MIDI CC1 range 0-127.

### Sustain
1/4-inch TS jack:

- TIP -> A2
- SLEEVE -> GND

The firmware learns pedal polarity at startup. **Leave the pedal released while powering on or resetting the Mega.**

### SW4 / SW5 octave buttons and LEDs

The two original illuminated buttons located **just above the pitch and mod wheels** are used for octave shift:

- **SW4 = Octave Down** -> D10
- **SW5 = Octave Up** -> D11
- **SW4 LED = Octave Down indicator** -> D12 through a series resistor
- **SW5 LED = Octave Up indicator** -> D13 through a series resistor

With no octave shift selected, both LEDs are off. Pressing SW4 selects one octave down and lights the SW4 LED; pressing SW5 selects one octave up and lights the SW5 LED.

330 ohm or 470 ohm is recommended for the LED resistors.

## Required Arduino library

Install the **DIO2** library before compiling.

## Firmware

Open `keyboardscanner.ino` in the Arduino IDE, select **Arduino Mega or Mega 2560**, compile, and upload.

The current firmware sends standard MIDI at 31250 baud.

## Hardware documentation

See:

- [hardware/README.md](hardware/README.md)
- [models/kurzweil_pc2/README.md](models/kurzweil_pc2/README.md)

## License / attribution

The key-scanning state-machine foundation was originally released under GPLv3 by Daniel Moura. The PC2 matrix mapping, shield pinout, wheel calibration, octave controls, sustain implementation, and single-Mega integration in this repository are specific to this Kurzweil PC2 project.
