# PC2 Mega 2560 shield

This hardware is a custom Arduino Mega 2560 shield designed for the Kurzweil PC2 76-key MIDI-controller conversion. It connects both keybed ribbons, the original pitch/mod/pressure board, the two illuminated octave buttons, and a 1/4-inch TS sustain jack to a single Mega.

## Personal PCB manufacturing

**[Download the PC2 Shield PCB manufacturing Gerber package](PC2%20Board.zip)**

## Bill of materials

- **[View the GitHub-formatted PC2 Shield BOM](PC2_Shield_BOM.md)** — concise, human-readable parts table with datasheet links.
- **[Open the full PC2 Shield BOM CSV](PC2_Shield_BOM.csv)** — CSV-format BOM for viewing or importing into a spreadsheet.
- **[Download the original Mouser XLS BOM](282323296.xls)** — original spreadsheet source.

The Markdown and CSV BOMs are for **one PC2 Shield**. The shield is designed for personal PCB fabrication. The original KiCad-exported manufacturing package supplied for this project is named **PC2 Board.zip** and contains these fabrication files:

| File | Purpose |
|---|---|
| `PC2 Shield-F_Cu.gbr` | Front copper |
| `PC2 Shield-B_Cu.gbr` | Back copper |
| `PC2 Shield-F_Mask.gbr` | Front solder mask |
| `PC2 Shield-B_Mask.gbr` | Back solder mask |
| `PC2 Shield-F_Silkscreen.gbr` | Front silkscreen |
| `PC2 Shield-B_Silkscreen.gbr` | Back silkscreen |
| `PC2 Shield-Edge_Cuts.gbr` | Board outline |
| `PC2 Shield-PTH.drl` | Plated drill holes |
| `PC2 Shield-NPTH.drl` | Non-plated drill holes |
| `PC2 Shield-job.gbrjob` | Gerber job metadata |

**Manufacturing instructions:** Upload the ZIP of Gerber and drill files to a PCB fabrication service. Review its Gerber preview for the board outline, holes, copper, silkscreen, and orientation before ordering. The package is for a **bare PCB**, not an assembled board: connectors, resistors, the sustain jack, and the Arduino Mega must be supplied and assembled separately.

**Status:** The single-Mega shield has been physically assembled in the PC2 and is working with all 76 keys and velocity, pitch, mod, sustain, octave buttons, and octave LEDs. The existing PCB does not require a respin for the verified functions. J10's LED conductors are intentionally non-intuitive: connector pin 1 is the SW5/Octave Down LED on D12, while connector pin 2 is the SW4/Octave Up LED on D13.

The final shield combines the complete Kurzweil PC2 keybed and controls onto one Arduino Mega 2560.

## J8 - Bass keybed ribbon

Pins 1-16 connect sequentially to Mega D22-D37.

## J9 - Treble keybed ribbon

Pins 1-12 connect to D38-D49. Pins 13-20 connect to D2-D9.

The physical treble contact order is **MK first, BR second**. The firmware intentionally stores each treble pair as **BR then MK** because `states.cpp` expects the second-closing contact first. This ordering has been verified on the assembled keyboard and is required for variable treble velocity.

## J10 - SW4 / SW5 octave buttons and LEDs

J10 connects the two original illuminated buttons located **just above the PC2 pitch and mod wheels**. In this project they are used exclusively for octave shifting:

Final assembled-shield testing confirmed that the **button inputs are D10/D11, while the LED conductors on J10 pins 1 and 2 are opposite the earlier labels**. The table below reflects the actual connector pin identity and the working Mega pin assignment.

- **SW4 = Octave Up**
- **SW5 = Octave Down**

| J10 pin | PC2 control | Function | Mega pin |
|---:|---|---|---:|
| 1 | SW5 LED | Octave-down LED | D12 through series resistor |
| 2 | SW4 LED | Octave-up LED | D13 through series resistor |
| 3 | SW4 | Octave-up button | D10 |
| 4 | SW5 | Octave-down button | D11 |
| 5 | Common | Button/LED common | GND |

Use one current-limiting resistor per LED. 330 ohm or 470 ohm is recommended; 1 kohm works but is visibly dimmer. At normal octave both LEDs are off; selecting octave up lights the physical SW4 LED through D13, and selecting octave down lights the physical SW5 LED through D12.

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

**J11 startup behavior:** A0 and A1 are analog inputs and would normally float when J11 is unplugged. The firmware performs a startup connection check using the Mega's internal pull-ups and disables any pitch/mod channel that still looks open, preventing false Pitch Bend or CC1 messages from an unconnected shield. Connect J11 before powering or resetting the Mega. If J11 is connected after startup, reset the Mega so the control inputs are detected again. Leave the pitch wheel released/centered during startup.

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
