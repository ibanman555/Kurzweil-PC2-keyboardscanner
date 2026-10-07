# Kurzweil PC2 76-key model

This model is the verified Kurzweil PC2 76-key matrix used by the single Arduino Mega 2560 shield.

The original PC2 keybed diodes remain in place. The scanner uses the normal sequential \`PINS()\` scan with a 3 microsecond settling delay.

## Contact order

The two halves of the PC2 keybed use opposite physical contact naming:

- Bass: BR closes first, MK closes second.
- Treble: MK closes first, BR closes second.

\`states.cpp\` expects each key to be stored as **SECOND contact first, FIRST contact second**, so \`pins.h\` deliberately uses different BR/MK ordering for the two halves.

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
