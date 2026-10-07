/*
Moura's Keyboard Scanner: turn you broken (or unused) keyboard in a MIDI controller
Copyright (C) 2017 Daniel Moura <oxesoft@gmail.com>

This code is originally hosted at https://github.com/oxesoft/keyboardscanner
*/

#include "globals.h"

#define KEY_OFF      0
#define KEY_START    1
#define KEY_ON       2
#define KEY_RELEASED 3

byte          keys_state[KEYS_NUMBER] = {KEY_OFF};
unsigned long keys_time [KEYS_NUMBER] = {0};
byte          sustain_pedal_signal;
byte          sustain_pedal_signal_previous = HIGH;

static void handleKey(byte key, boolean upper, boolean lower)
{
    switch (keys_state[key])
    {
    case KEY_OFF:
        if (upper)
        {
            keys_state[key] = KEY_START;
            keys_time[key] = micros();
        }
        break;

    case KEY_START:
        if (!upper)
        {
            keys_state[key] = KEY_OFF;
            break;
        }
        if (lower)
        {
            keys_state[key] = KEY_ON;
            unsigned long time = micros() - keys_time[key];
            sendKeyEvent(0x90, key, time);
        }
        break;

    case KEY_ON:
        if (!lower)
        {
            keys_state[key] = KEY_RELEASED;
            keys_time[key] = micros();
        }
        break;

    case KEY_RELEASED:
        if (!upper)
        {
            keys_state[key] = KEY_OFF;
            unsigned long time = micros() - keys_time[key];
            sendKeyEvent(0x80, key, time);
        }
        break;
    }
}

boolean matrix_signals[KEYS_NUMBER * 2] = {LOW};

void statesLoop()
{
    boolean *signal = matrix_signals;

    for (byte key = 0; key < KEYS_NUMBER; key++)
    {
        // pins.h stores SECOND contact first, FIRST contact second.
        boolean lower = *(signal++);
        boolean upper = *(signal++);
        handleKey(key, upper, lower);
    }

    if (sustain_pedal_signal_previous != sustain_pedal_signal)
    {
        sendSustainPedalEvent(sustain_pedal_signal == LOW);
    }
    sustain_pedal_signal_previous = sustain_pedal_signal;
}
