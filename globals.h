/*
Moura's Keyboard Scanner: turn you broken (or unused) keyboard in a MIDI controller
Copyright (C) 2017 Daniel Moura <oxesoft@gmail.com>

This code is originally hosted at https://github.com/oxesoft/keyboardscanner
*/

#ifndef GLOBALS_H
#define GLOBALS_H

#include <Arduino.h>

#define MODEL_NAME kurzweil_pc2

#define MODEL_HEADER_PATH models/MODEL_NAME/model.h
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#include STR(MODEL_HEADER_PATH)

void scannerSetup();
void scannerLoop();
void statesLoop();
void sendKeyEvent(byte status_byte, byte key_index, unsigned long time);
void sendSustainPedalEvent(boolean pressed);
void sendMidiEvent(byte status_byte, byte data1, byte data2);

#endif
