# Kurzweil PC2 76-Key MIDI Controller

This repository is for a **Kurzweil PC2 76-key keybed converted into a standalone MIDI controller using one Arduino Mega 2560 and a custom shield**.

The hardware and firmware are specifically for the PC2 keybed, its original pitch/mod wheel assembly, the two illuminated switches directly above the wheels (**SW4 = Octave Down, SW5 = Octave Up**), and a 1/4-inch sustain-pedal input.

## Implemented functions

- All 76 PC2 keys with velocity
- Original Bass and Treble keybed ribbons
- Pitch wheel
- Mod wheel
- Octave down / normal / octave up on SW4/SW5
- Original SW4/SW5 LEDs as octave indicators
- 1/4-inch TS sustain pedal
- Automatic sustain-pedal polarity detection at power-up
- PC2 PRESSR/aftertouch routed to A3 for future use

Aftertouch MIDI output is intentionally not enabled yet because the original pressure strip is still being evaluated for consistent mechanical response.

## Downloading and opening the Arduino sketch

GitHub's downloaded ZIP adds a suffix to the repository folder name, so the firmware is intentionally kept in its own valid Arduino sketch folder:

```text
keyboardscanner/
    keyboardscanner.ino
    globals.h
    scanner.cpp
    states.cpp
    midi.cpp
    potentiometers.cpp
    velocity.h
    extensions.h
    models/kurzweil_pc2/...
```

After downloading and extracting the ZIP, open:

```text
keyboardscanner/keyboardscanner.ino
```

**Keep the entire `keyboardscanner` folder together. Do not move only the `.ino` file.**

In Arduino IDE select **Arduino Mega or Mega 2560**, install the **DIO2** library, then compile and upload.

## MIDI connection used for prototype testing

The successful dual-Mega prototype used the Mega's normal USB serial connection as a COM port and Bome as the serial-to-MIDI bridge:

- Serial speed: **31250 baud**
- Format: **8N1**
- Arduino Serial Monitor: **closed**
- Bome: configure the Mega COM port as a serial MIDI input

The stock Mega 2560 USB connection is a serial/COM device, not a native USB-MIDI device. The firmware sends standard MIDI bytes through `Serial` at 31250 baud, exactly as used in the prototype.

## Final Arduino Mega 2560 pinout

| Function | Mega pin(s) |
|---|---|
| J8 Bass ribbon | D22-D37 |
| J9 Treble pins 1-12 | D38-D49 |
| J9 Treble pins 13-20 | D2-D9 |
| SW4 / Octave Down button | D10 |
| SW5 / Octave Up button | D11 |
| SW4 / Octave Down LED | D12 |
| SW5 / Octave Up LED | D13 |
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

D0/D1 are used by `Serial`/UART0 and are shared with the Mega's USB-to-serial interface. Leave them unconnected when using the USB/COM-to-Bome MIDI path.

## Keybed scanner settings

- 76 keys
- First MIDI note: 28
- Last MIDI note: 103
- Bass section: notes 28-59
- Treble section: notes 60-103
- Velocity timing range: 1800-80000 us
- Velocity curve: linear 0-127 (prototype-verified baseline)
- Matrix settling delay: 3 us

Both Bass and Treble halves use the same physical contact order: **BR closes first, MK closes second**. Because `states.cpp` expects **SECOND contact first, FIRST contact second**, `pins.h` stores **MK first, BR second** for every key. This matches the verified dual-Mega prototype diagnostics.

## Controls

### Pitch

PC2 6-pin wheel connector pin 2 -> A0.

The pitch input is mapped as 14-bit MIDI Pitch Bend with a center dead zone around the wheel's mechanical center.

### Mod

PC2 6-pin wheel connector pin 3 -> A1.

The measured PC2 mod-wheel output is approximately 0.008-3.6 V. The successful prototype used ADC raw **2-736**, which the firmware maps to the full MIDI CC1 range 0-127.

### Sustain

1/4-inch TS jack:

- TIP -> A2
- SLEEVE -> GND

The firmware learns pedal polarity at startup. **Plug the pedal in and leave it released while powering on or resetting the Mega.**

### SW4 / SW5 octave buttons and LEDs

The two original illuminated buttons located **just above the pitch and mod wheels** are used for octave shift:

- **SW4 = Octave Down** -> D10
- **SW5 = Octave Up** -> D11
- **SW4 LED = Octave Down indicator** -> D12 through a series resistor
- **SW5 LED = Octave Up indicator** -> D13 through a series resistor

At normal octave both LEDs are off. Selecting octave down lights SW4; selecting octave up lights SW5. The code remembers the octave used for each Note On so the matching Note Off is sent to the same transposed note even if the octave is changed while a key is held.

330 ohm or 470 ohm is recommended for each LED series resistor. 1 kohm works but is visibly dimmer.

## Automated verification

The repository includes:

- a PC2-specific static mapping check in `keyboardscanner/tests/verify_pc2.py`
- a GitHub Actions build that compiles the actual sketch for **Arduino Mega 2560** with DIO2

These checks protect the 76-key mapping, contact order, shield pin assignments, velocity-table size, and basic compile compatibility.

## Hardware and personal PCB manufacturing

The custom Mega 2560 shield can be fabricated from the project's KiCad-exported Gerber and drill package. See the hardware guide for the manufacturing file list, ordering guidance, connector pinouts, and assembly precautions. The final shield is not yet physically verified.

See:

- [hardware/README.md](hardware/README.md)
- [keyboardscanner/models/kurzweil_pc2/README.md](keyboardscanner/models/kurzweil_pc2/README.md)

## License / attribution

The key-scanning state-machine foundation was originally released under GPLv3 by Daniel Moura. The PC2 matrix mapping, shield pinout, wheel calibration, octave controls, sustain implementation, and single-Mega integration in this repository are specific to this Kurzweil PC2 project.
