# PC2 Shield PCB Manufacturing Files

This folder contains the fabrication output for the custom **Kurzweil PC2 -> Arduino Mega 2560 shield** used by this project.

## Download

[Download the complete PC2 Shield Gerber/drill package](PC2-Shield-Gerbers.zip)

The ZIP is provided for **personal manufacturing** of the shield and can be uploaded to a PCB fabrication service.

## Included manufacturing layers

The package contains the board-production files exported from the final PC2 shield design:

- Front copper
- Back copper
- Front solder mask
- Back solder mask
- Front silkscreen
- Back silkscreen
- Board edge cuts
- Plated through-hole drill file
- Non-plated through-hole drill file
- Gerber job file

The archive preserves the original manufacturing filenames and folder structure.

## What this board connects

The shield is designed to connect the PC2 hardware directly to one Arduino Mega 2560:

- J8: 16-pin Bass keybed ribbon
- J9: 20-pin Treble keybed ribbon
- J10: SW4/SW5 octave buttons and LEDs
- J11: pitch wheel, mod wheel, and PRESSR/aftertouch
- J12: 1/4-inch TS sustain pedal
- Arduino Mega 2560 shield headers

The exact signal-to-pin mapping is documented in [../README.md](../README.md) and in the firmware model under [../../keyboardscanner/models/kurzweil_pc2/](../../keyboardscanner/models/kurzweil_pc2/).

## Before ordering boards

These files are manufacturing outputs rather than editable KiCad source files. Before placing an order, confirm the PCB manufacturer's preview shows the expected board outline, copper, drill holes, solder mask, and silkscreen. Also verify connector orientation and component choices against the hardware documentation.

The Gerber ZIP included here is the package intended for fabrication; do not regenerate or rearrange individual files unless you are intentionally revising the PCB design.
