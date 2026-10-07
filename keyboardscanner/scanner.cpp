/*
Moura's Keyboard Scanner: turn you broken (or unused) keyboard in a MIDI controller
Copyright (C) 2017 Daniel Moura <oxesoft@gmail.com>

This code is originally hosted at https://github.com/oxesoft/keyboardscanner
*/

#include "globals.h"
#include <DIO2.h>

#define MODEL_PINS_DEF models/MODEL_NAME/pins.h

extern byte sustain_pedal_signal;

// Shared with midi.cpp so octave changes apply to all 76 keys.
int8_t octave_shift = 0;

// Learned at power-up. The sustain pedal must be released while powering on/resetting.
boolean sustain_pedal_released_state = HIGH;

#ifdef SETTLING_TIME_MICROSECONDS
#define SETTLING_TIME_DELAY delayMicroseconds(SETTLING_TIME_MICROSECONDS);
#else
#define SETTLING_TIME_DELAY
#endif

void octaveButtonsLoop()
{
    static bool downLastReading = HIGH;
    static bool downStable = HIGH;
    static unsigned long downChangeTime = 0;

    static bool upLastReading = HIGH;
    static bool upStable = HIGH;
    static unsigned long upChangeTime = 0;

    unsigned long now = millis();

    bool downReading = digitalRead(OCTAVE_DOWN_PIN);
    bool upReading   = digitalRead(OCTAVE_UP_PIN);

    if (downReading != downLastReading)
    {
        downLastReading = downReading;
        downChangeTime = now;
    }

    if ((now - downChangeTime) >= 25 && downReading != downStable)
    {
        downStable = downReading;
        if (downStable == LOW && octave_shift > -1)
        {
            octave_shift--;
        }
    }

    if (upReading != upLastReading)
    {
        upLastReading = upReading;
        upChangeTime = now;
    }

    if ((now - upChangeTime) >= 25 && upReading != upStable)
    {
        upStable = upReading;
        if (upStable == LOW && octave_shift < 1)
        {
            octave_shift++;
        }
    }

    // LEDs show the selected octave state. Both are off at normal pitch.
    digitalWrite(OCTAVE_DOWN_LED_PIN, octave_shift == -1 ? HIGH : LOW);
    digitalWrite(OCTAVE_UP_LED_PIN,   octave_shift ==  1 ? HIGH : LOW);
}

void scannerSetup()
{
    pinMode2(LED_BUILTIN, OUTPUT);
    digitalWrite2(LED_BUILTIN, LOW);

    #define PINS(output_pin, input_pin) \
        pinMode2(output_pin, OUTPUT); \
        digitalWrite2(output_pin, HIGH); \
        pinMode2(input_pin, INPUT_PULLUP);
    #include STR(MODEL_PINS_DEF)
    #undef PINS

    pinMode2(SUSTAIN_PEDAL_PIN, INPUT_PULLUP);
    delay(20);
    sustain_pedal_released_state = digitalRead2(SUSTAIN_PEDAL_PIN);

    pinMode(OCTAVE_DOWN_PIN, INPUT_PULLUP);
    pinMode(OCTAVE_UP_PIN, INPUT_PULLUP);

    pinMode(OCTAVE_DOWN_LED_PIN, OUTPUT);
    pinMode(OCTAVE_UP_LED_PIN, OUTPUT);
    digitalWrite(OCTAVE_DOWN_LED_PIN, LOW);
    digitalWrite(OCTAVE_UP_LED_PIN, LOW);
}

extern boolean matrix_signals[KEYS_NUMBER * 2];

void scannerLoop()
{
    boolean *s = matrix_signals;

    #define PINS(output_pin, input_pin) \
        digitalWrite2(output_pin, LOW); \
        SETTLING_TIME_DELAY \
        *(s++) = !digitalRead2(input_pin); \
        digitalWrite2(output_pin, HIGH);
    #include STR(MODEL_PINS_DEF)
    #undef PINS

    boolean pedal_raw = digitalRead2(SUSTAIN_PEDAL_PIN);
    sustain_pedal_signal =
        (pedal_raw == sustain_pedal_released_state) ? HIGH : LOW;

    octaveButtonsLoop();
}
