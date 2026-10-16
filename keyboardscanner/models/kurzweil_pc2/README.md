# Kurzweil PC2 76-key model

This is the final hardware-verified Kurzweil PC2 76-key matrix used by the assembled single Arduino Mega 2560 shield.

The original PC2 keybed diodes remain in place. The scanner uses the sequential `PINS()` scan with a 3 microsecond settling delay.

## Contact order

The assembled PC2 keybed uses opposite contact order between the two ribbon sections:

- **Bass:** BR closes first, MK closes second.
- **Treble:** MK closes first, BR closes second.

`states.cpp` expects each key to be stored as **SECOND contact first, FIRST contact second**. Therefore:

- Bass entries are stored **MK first, BR second**.
- Treble entries are stored **BR first, MK second**.

This ordering is required for correct velocity timing. With the treble pair reversed, the first treble contact is already active when timing begins, collapsing the measured interval and forcing notes to MIDI velocity 127. The final assembled shield test confirmed the ordering above.

## MIDI range

- 76 keys total
- First MIDI note: 28
- Last MIDI note: 103
- Bass section: notes 28-59
- Treble section: notes 60-103

## Final shield connector mapping

### J8 - Bass ribbon

| J8 pin | Signal | Mega pin |
|---:|---|---:|
| 1 | MK0 | D22 |
| 2 | T0 | D23 |
| 3 | BR0 | D24 |
| 4 | T1 | D25 |
| 5 | MK1 | D26 |
| 6 | T2 | D27 |
| 7 | BR1 | D28 |
| 8 | T3 | D29 |
| 9 | MK2 | D30 |
| 10 | T4 | D31 |
| 11 | BR2 | D32 |
| 12 | T5 | D33 |
| 13 | MK3 | D34 |
| 14 | T6 | D35 |
| 15 | BR3 | D36 |
| 16 | T7 | D37 |

### J9 - Treble ribbon

| J9 pin | Signal | Mega pin |
|---:|---|---:|
| 1 | MK5 | D38 |
| 2 | T0 | D39 |
| 3 | BR5 | D40 |
| 4 | T1 | D41 |
| 5 | MK6 | D42 |
| 6 | T2 | D43 |
| 7 | BR6 | D44 |
| 8 | T3 | D45 |
| 9 | MK7 | D46 |
| 10 | T4 | D47 |
| 11 | BR7 | D48 |
| 12 | T5 | D49 |
| 13 | MK8 | D2 |
| 14 | T6 | D3 |
| 15 | BR8 | D4 |
| 16 | T7 | D5 |
| 17 | MK9 | D6 |
| 18 | BR10 | D7 |
| 19 | BR9 | D8 |
| 20 | MK10 | D9 |

The final four notes use MK10/BR10 with T0-T3.

## Verified control connector mapping

### J10 - Octave buttons and LEDs

| J10 pin | Physical function | Mega pin |
|---:|---|---:|
| 1 | SW5 / Octave Down LED | D12 through series resistor |
| 2 | SW4 / Octave Up LED | D13 through series resistor |
| 3 | SW4 / Octave Up button | D10 |
| 4 | SW5 / Octave Down button | D11 |
| 5 | Common | GND |

### J11 - Wheels / pressure

| J11 pin | Function | Mega pin |
|---:|---|---:|
| 1 | +5 V reference | +5V |
| 2 | Pitch wheel | A0 |
| 3 | Mod wheel | A1 |
| 4 | PRESSR / aftertouch | A3 |
| 5 | Analog ground | GND |
| 6 | NC | NC |

### J12 - Sustain

- Tip -> A2
- Sleeve -> GND

## Current velocity baseline

The assembled keyboard is currently working with:

- `MIN_TIME_US = 1800`
- `MAX_TIME_US = 80000`
- a linear 128-entry `VELOCITY_CURVE` from 0 to 127

The matrix/contact ordering above is now fixed by hardware verification. Future changes to playing feel should be made through the timing limits and/or `velocity.h`, not by changing Bass/Treble contact order.
