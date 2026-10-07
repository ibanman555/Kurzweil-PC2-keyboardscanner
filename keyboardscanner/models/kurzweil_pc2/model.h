/*
Moura's Keyboard Scanner: turn you broken (or unused) keyboard in a MIDI controller
Copyright (C) 2017 Daniel Moura <oxesoft@gmail.com>

Kurzweil PC2 76-key configuration for the single Arduino Mega 2560 shield.
*/

#define KEYS_NUMBER 76
#define FIRST_KEY   28

// Verified key timing baseline from the PC2 prototype.
#define MIN_TIME_US 1800
#define MAX_TIME_US 80000

// The PC2 matrix requires a short settling delay after selecting each contact line.
#define SETTLING_TIME_MICROSECONDS 3

// Final shield controls.
#define PITCH_WHEEL_PIN       A0
#define MOD_WHEEL_PIN         A1
#define SUSTAIN_PEDAL_PIN     A2
#define AFTERTOUCH_PIN        A3

// Original illuminated buttons just above the pitch/mod wheels:
// SW4 = Octave Down, SW5 = Octave Up.
#define OCTAVE_DOWN_PIN       10  // SW4 button
#define OCTAVE_UP_PIN         11  // SW5 button
#define OCTAVE_DOWN_LED_PIN   12  // SW4 LED
#define OCTAVE_UP_LED_PIN     13  // SW5 LED

// J8 - Bass ribbon
#define BASS_MK0 22
#define BASS_T0  23
#define BASS_BR0 24
#define BASS_T1  25
#define BASS_MK1 26
#define BASS_T2  27
#define BASS_BR1 28
#define BASS_T3  29
#define BASS_MK2 30
#define BASS_T4  31
#define BASS_BR2 32
#define BASS_T5  33
#define BASS_MK3 34
#define BASS_T6  35
#define BASS_BR3 36
#define BASS_T7  37

// J9 - Treble ribbon
#define TREBLE_MK5  38
#define TREBLE_T0   39
#define TREBLE_BR5  40
#define TREBLE_T1   41
#define TREBLE_MK6  42
#define TREBLE_T2   43
#define TREBLE_BR6  44
#define TREBLE_T3   45
#define TREBLE_MK7  46
#define TREBLE_T4   47
#define TREBLE_BR7  48
#define TREBLE_T5   49
#define TREBLE_MK8   2
#define TREBLE_T6    3
#define TREBLE_BR8   4
#define TREBLE_T7    5
#define TREBLE_MK9   6
#define TREBLE_BR10  7
#define TREBLE_BR9   8
#define TREBLE_MK10  9
