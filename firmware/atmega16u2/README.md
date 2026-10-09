# ATmega16U2 USB firmware for the PC2 controller

This directory contains the USB-interface firmware used with this Kurzweil PC2 / Arduino Mega 2560 conversion.

These files are for a **Mega 2560 with an ATmega16U2 USB-interface MCU**. They are **not** the firmware for the main ATmega2560 that runs the PC2 keyboard-scanner sketch, and they must not be flashed to the main ATmega2560. They are also not intended for Mega-compatible boards that use CH340, FTDI, CP2102, or other fixed USB-to-serial chips.

## Included firmware

- **[ATmega_16u2_PC2.hex](ATmega_16u2_PC2.hex)** — custom build of the HIDUINO `arduino_midi` firmware for the ATmega16U2. Its USB product string has been changed to **Kurzweil PC2**, so the finished controller enumerates on the computer as a class-compliant USB-MIDI device with that name.
- **[usbserial_mega_16u2.hex](usbserial_mega_16u2.hex)** — Mega 2560 ATmega16U2 USB-serial restore image. Flash this if the board needs to be returned to its normal USB serial/COM-port behavior.

The PC2 sketch on the main ATmega2560 already sends raw MIDI bytes over UART0 at **31250 baud**. HIDUINO on the ATmega16U2 receives that MIDI stream and presents it to the host computer as USB MIDI.

## Recommended programmer: Olimex AVR-ISP-MK2

For the simplest repeatable way to install or restore the ATmega16U2 firmware, this project recommends the **Olimex AVR-ISP-MK2**.

- Manufacturer part number: **AVR-ISP-MK2**
- Mouser part number: **909-AVR-ISP-MK2**
- Mouser: https://www.mouser.com/ProductDetail/Olimex-Ltd/AVR-ISP-MK2

The Olimex programmer is compatible with AVR ISP programming and can be used with tools such as Atmel Studio and AVRDUDE.

### Important: use the ATmega16U2 ICSP header

The Mega 2560 has separate microcontrollers for the keyboard sketch and the USB interface. When flashing either HEX file in this directory, connect the programmer to the **2x3 ICSP header for the ATmega16U2 near the USB connector**.

**Do not use the main ATmega2560 ICSP header for these two files.**

Verify the cable's pin-1 orientation before powering or programming the board.

## Typical programming procedure

1. Upload the PC2 keyboard-scanner sketch to the main ATmega2560 first, while the normal USB-serial firmware is still installed, unless the main MCU will be programmed separately through ISP.
2. Disconnect power and connect the Olimex AVR-ISP-MK2 to the **ATmega16U2 ICSP header near the USB connector**.
3. In the programming software select **ATmega16U2** as the target device and **ISP** as the programming interface.
4. Program and verify **ATmega_16u2_PC2.hex** into the ATmega16U2 flash.
5. Reconnect the Mega's USB cable. The computer should enumerate the controller as a class-compliant USB-MIDI device named **Kurzweil PC2**.
6. If normal Arduino USB serial/COM-port operation is ever needed again, repeat the same ISP process using **usbserial_mega_16u2.hex**.

Once HIDUINO is installed, the normal Mega USB serial/COM interface is no longer available for uploading sketches to the main ATmega2560. Either restore `usbserial_mega_16u2.hex` temporarily or program the main ATmega2560 through its own ISP header.

## Debug-output warning

The current PC2 firmware contains some `Serial.print()` / `Serial.println()` statements only inside disabled compile-time debug sections. They are safe to leave in the source.

Do **not** enable `DEBUG_VELOCITY_TIMES` or `DEBUG_MIDI_MESSAGE` while HIDUINO is active. Debug text on UART0 would be mixed into the MIDI byte stream.

## Firmware validation

The supplied **ATmega_16u2_PC2.hex** is a valid Intel HEX image and contains the USB product descriptor **Kurzweil PC2**. Both supplied files are retained here so the USB interface can be changed between native USB MIDI and the normal Mega 2560 USB-serial interface without rebuilding either firmware.

## HIDUINO attribution

The USB-MIDI firmware is based on the open-source [HIDUINO project](https://github.com/ddiakopoulos/hiduino) by Dimitri Diakopoulos, which in turn uses Dean Camera's LUFA framework. HIDUINO is distributed under the MIT license; see the upstream project for source, build instructions, and licensing details.
