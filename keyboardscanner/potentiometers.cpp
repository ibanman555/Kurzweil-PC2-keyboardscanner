/*
Moura's Keyboard Scanner: turn you broken (or unused) keyboard in a MIDI controller
Copyright (C) 2017 Daniel Moura <oxesoft@gmail.com>

Kurzweil PC2 pitch/mod wheel calibration from the working prototype.
*/

#include "globals.h"

#define POTS_RESOLUTION_MICROSECONDS 5000
#define POTS_THRESHOLD_VALUE         8
#define POTS_PB_CENTER_DEADZONE      12
#define POTS_NUMBER                  2

#define POT_TYPE_PITCHBEND 0xE000u
#define POT_TYPE_MODWHEEL  0xB001u

// PC2 mod-wheel output measured approximately 0.008 V to 3.6 V on the working prototype.
// The prototype calibration used raw ADC 2..736 and maps that measured range
// to the full MIDI CC1 range 0..127.
#define MOD_MIN_RAW 2
#define MOD_MAX_RAW 736

const int POTS_ANALOG_PINS[POTS_NUMBER] = {
    PITCH_WHEEL_PIN,
    MOD_WHEEL_PIN
};

const uint16_t POTS_TYPES[POTS_NUMBER] = {
    POT_TYPE_PITCHBEND,
    POT_TYPE_MODWHEEL
};

int analogRawValues[POTS_NUMBER] = {0};
int midiValues[POTS_NUMBER] = {0};
unsigned long lastReadingTime = 0;

void potentiometersSetup()
{
    for (int i = 0; i < POTS_NUMBER; i++)
    {
        if (POTS_TYPES[i] == POT_TYPE_PITCHBEND)
        {
            midiValues[i] = 8192;
        }
    }
}

void potentiometersLoop()
{
    unsigned long currentTime = micros();
    if (currentTime - lastReadingTime < POTS_RESOLUTION_MICROSECONDS)
    {
        return;
    }

    for (int i = 0; i < POTS_NUMBER; i++)
    {
        int raw = analogRead(POTS_ANALOG_PINS[i]);

        if (POTS_TYPES[i] == POT_TYPE_PITCHBEND)
        {
            const int CENTER = 512;
            int value;

            if (abs(raw - CENTER) <= POTS_PB_CENTER_DEADZONE)
            {
                value = 8192;
            }
            else if (raw < CENTER)
            {
                value = map(raw, 0, CENTER - POTS_PB_CENTER_DEADZONE - 1, 0, 8191);
            }
            else
            {
                value = map(raw, CENTER + POTS_PB_CENTER_DEADZONE + 1, 1023, 8192, 16383);
            }

            value = constrain(value, 0, 16383);
            if (midiValues[i] == value)
            {
                continue;
            }

            midiValues[i] = value;
            byte lsb = value & 0x7F;
            byte msb = value >> 7;
            sendMidiEvent(0xE0, lsb, msb);
        }
        else
        {
            int lastRaw = analogRawValues[i];
            if (abs(raw - lastRaw) < POTS_THRESHOLD_VALUE)
            {
                continue;
            }

            analogRawValues[i] = raw;
            int modRaw = constrain(raw, MOD_MIN_RAW, MOD_MAX_RAW);
            byte value = map(modRaw, MOD_MIN_RAW, MOD_MAX_RAW, 0, 127);
            sendMidiEvent(0xB0, 0x01, value);
        }
    }

    lastReadingTime = currentTime;
}
