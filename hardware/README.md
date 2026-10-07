# PC2 Mega 2560 shield

The final shield combines the complete Kurzweil PC2 keybed and controls onto one Arduino Mega 2560.

## J8 - Bass keybed ribbon

Pins 1-16 connect sequentially to Mega D22-D37.

## J9 - Treble keybed ribbon

Pins 1-12 connect to D38-D49. Pins 13-20 connect to D2-D9.

## J10 - SW4 / SW5 octave buttons and LEDs

J10 connects the two original illuminated buttons located **just above the PC2 pitch and mod wheels**. In this project they are used exclusively for octave shifting:

- **SW4 = Octave Down**
- **SW5 = Octave Up**

| J10 pin | PC2 control | Function | Mega pin |
|---:|---|---|---:|
| 1 | SW4 LED | Octave-down LED | D12 through series resistor |
| 2 | SW5 LED | Octave-up LED | D13 through series resistor |
| 3 | SW4 | Octave-down button | D10 |
| 4 | SW5 | Octave-up button | D11 |
| 5 | Common | Button/LED common | GND |

Use one current-limiting resistor per LED. 330 ohm or 470 ohm is recommended; 1 kohm works but is visibly dimmer. At normal octave both LEDs are off; selecting octave down lights SW4, and selecting octave up lights SW5.

## J11 - Wheels / pressure

| J11 pin | Function | Mega pin |
|---:|---|---:|
| 1 | +5 V reference | +5V |
| 2 | Pitch wheel | A0 |
| 3 | Mod wheel | A1 |
| 4 | Aftertouch / PRESSR | A3 |
| 5 | Analog ground | GND |
| 6 | NC | NC |

Pitch and mod are enabled. A3 is routed for PC2 channel aftertouch/pressure, but aftertouch MIDI output is intentionally not enabled yet because the original pressure strip is still being mechanically evaluated.

## J12 - Sustain pedal

1/4-inch TS jack:

- TIP -> A2
- SLEEVE -> GND

Firmware learns sustain-pedal polarity at power-up. Plug the pedal in and leave it released while powering on or resetting the Mega.

## Reserved interfaces

- D14-D15: Serial3
- D16-D17: Serial2
- D18-D19: Serial1
- D20-D21: I2C
- D50-D53: SPI
- A4-A15: unused analog pins

D0/D1 remain the primary serial/MIDI interface used by the sketch.
