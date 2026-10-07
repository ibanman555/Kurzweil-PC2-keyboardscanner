# Kurzweil PC2 keyboardscanner

This repository adapts Daniel Moura's \`keyboardscanner\` project for the **Kurzweil PC2 76-key keyboard** and a custom Arduino Mega 2560 shield.

The project has now moved from the two-Mega development prototype to the final **single Mega 2560** pinout used by the shield.

## Working functions

Prototype testing established working support for:

- all 76 keys with velocity
- original PC2 Bass and Treble keybed ribbons
- pitch wheel
- mod wheel
- octave down / normal / octave up
- the two original octave-button LEDs
- 1/4-inch TS sustain pedal with automatic power-on polarity detection

The shield also routes the PC2 \`PRESSR\` aftertouch signal to A3. The pressure strip has produced approximately 0.8-4.8 V in testing, but aftertouch MIDI output is not enabled yet because the original strip/key mechanics still need service/testing for consistent response.

## Final Mega 2560 pinout

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
| Aftertouch / PRESSR | A3 (routed, MIDI output not yet enabled) |

## Firmware baseline

- \`KEYS_NUMBER = 76\`
- \`FIRST_KEY = 28\`
- \`MIN_TIME_US = 1800\`
- \`MAX_TIME_US = 80000\`
- \`SETTLING_TIME_MICROSECONDS = 3\`

The PC2 Bass and Treble halves close their BR/MK contacts in opposite order. \`pins.h\` accounts for this explicitly; do not globally reverse contact order in \`states.cpp\`.

The pitch and mod wheel extension is enabled in \`extensions.h\`. The PC2 mod-wheel output does not reach a full 5 V, so its measured useful range is remapped to MIDI CC1 0-127.

The octave buttons shift the entire 76-key keyboard on the single-Mega build. The Note On octave is remembered per key so a Note Off is always sent to the matching transposed note even if the octave is changed while the key is held.

The sustain input automatically learns pedal polarity at startup. The pedal must be released during power-up/reset.

## Hardware

See [hardware/README.md](hardware/README.md) and [models/kurzweil_pc2/README.md](models/kurzweil_pc2/README.md) for connector and matrix wiring.

## Required Arduino library

Install the **DIO2** library used by the original scanner project.

## MIDI output

The sketch starts \`Serial\` at 31250 baud and sends standard MIDI bytes through the existing \`sendMidiEvent()\` path.

## Credits

Based on Daniel Moura's original keyboardscanner project:

https://github.com/oxesoft/keyboardscanner
