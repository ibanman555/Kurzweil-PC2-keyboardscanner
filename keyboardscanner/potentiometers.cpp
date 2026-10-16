/*
Moura's Keyboard Scanner: turn you broken (or unused) keyboard in a MIDI controller
Copyright (C) 2017 Daniel Moura <oxesoft@gmail.com>

Kurzweil PC2 pitch/mod wheel calibration from the working prototype.
*/

#include "globals.h"

#define POTS_RESOLUTION_MICROSECONDS    5000
#define POTS_THRESHOLD_VALUE            8

// The original PC2 pitch potentiometer on the prototype does not return to
// exactly the same electrical center every time. A 96-count center dead zone
// was verified on the prototype to stop center jitter and force exact MIDI
// pitch-bend center (8192) when the wheel is released.
#define POTS_PB_CENTER_DEADZONE         96

#define POTS_NUMBER                     2

// A disconnected Mega analog input floats and can generate a continuous stream
// of false pitch/mod MIDI. At startup, briefly enable the AVR's internal pull-up
// and treat a channel that remains essentially at full scale as disconnected.
// PC2 controls should be connected before power-up/reset.
#define POTS_CONNECTION_MAX_RAW         1000
#define POTS_CONNECTION_SAMPLES         8
#define POTS_CONNECTION_SETTLE_MS       4

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
bool potConnected[POTS_NUMBER] = {false, false};
unsigned long lastReadingTime = 0;

static bool analogControlPresent(int pin)
{
    // With no connector attached, INPUT_PULLUP drives the open analog input
    // to approximately 1023. A connected PC2 potentiometer holds the pin below
    // that level. This probe is only performed during setup, before MIDI output.
    pinMode(pin, INPUT_PULLUP);
    delay(POTS_CONNECTION_SETTLE_MS);

    unsigned long total = 0;
    for (byte sample = 0; sample < POTS_CONNECTION_SAMPLES; sample++)
    {
        total += analogRead(pin);
    }

    // Disable the pull-up before normal analog operation.
    pinMode(pin, INPUT);
    digitalWrite(pin, LOW);
    delayMicroseconds(200);

    int average = total / POTS_CONNECTION_SAMPLES;
    return average < POTS_CONNECTION_MAX_RAW;
}

void potentiometersSetup()
{
    for (int i = 0; i < POTS_NUMBER; i++)
    {
        potConnected[i] = analogControlPresent(POTS_ANALOG_PINS[i]);

        if (!potConnected[i])
        {
            continue;
        }

        // Throw away one conversion after changing the pin configuration so
        // the ADC sample-and-hold has settled on the real control voltage.
        analogRead(POTS_ANALOG_PINS[i]);

        if (POTS_TYPES[i] == POT_TYPE_PITCHBEND)
        {
            midiValues[i] = 8192;
        }
        else
        {
            // Start from the actual mod-wheel position so simply powering up
            // does not create an artificial CC1 change.
            analogRawValues[i] = analogRead(POTS_ANALOG_PINS[i]);
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
        // Do not read an unconnected analog input. Floating A0/A1 values would
        // otherwise be interpreted as real pitch-bend or modulation movement.
        if (!potConnected[i])
        {
            continue;
        }

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
