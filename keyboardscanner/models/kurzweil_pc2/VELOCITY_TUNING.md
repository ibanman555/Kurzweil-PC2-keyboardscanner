# PC2 velocity / playing-feel tuning

This guide explains how to change the velocity response of the Kurzweil PC2 conversion **without changing the verified keybed wiring or contact order**.

The current working baseline is:

```cpp
#define MIN_TIME_US 1800
#define MAX_TIME_US 80000
```

with a linear 128-entry `VELOCITY_CURVE` from 0 to 127.

These values are known to work with the assembled PC2. If you change the playing feel, keep the verified Bass/Treble contact ordering in `pins.h` exactly as it is.

## How velocity is calculated

Each PC2 key has two contacts. The firmware measures the time between the first and second contact closing.

- **Shorter time = faster key movement = higher velocity**
- **Longer time = slower key movement = lower velocity**

The measured time is converted in two stages:

1. `MIN_TIME_US` and `MAX_TIME_US` convert the physical contact time into a raw value from 0 to 127.
2. `VELOCITY_CURVE[]` converts that raw value into the final MIDI velocity.

The relevant code in `midi.cpp` is:

```cpp
unsigned long t = constrain(time, MIN_TIME_US, MAX_TIME_US);
t -= MIN_TIME_US;

unsigned long linear_velocity =
    127 - ((t * 127) / (MAX_TIME_US - MIN_TIME_US));

byte vel = VELOCITY_CURVE[linear_velocity];
```

For Note On messages, a calculated velocity of 0 is changed to 1 because MIDI Note On velocity 0 is normally interpreted as Note Off.

## The two things you can tune

### 1. Physical timing range

Edit:

```text
keyboardscanner/models/kurzweil_pc2/model.h
```

Current values:

```cpp
#define MIN_TIME_US 1800
#define MAX_TIME_US 80000
```

These define the physical key-speed range that maps to MIDI velocity 127 down to 1.

### 2. Velocity curve

Edit:

```text
keyboardscanner/velocity.h
```

The current table is linear:

```cpp
const byte VELOCITY_CURVE[] = {
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
...
112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127
};
```

The table has exactly 128 entries. The array index is the raw 0-127 velocity and the value stored at that index is the final MIDI velocity.

For example:

```text
VELOCITY_CURVE[32] = 32    -> raw 32 becomes MIDI velocity 32
VELOCITY_CURVE[64] = 80    -> raw 64 becomes MIDI velocity 80
VELOCITY_CURVE[96] = 110   -> raw 96 becomes MIDI velocity 110
```

## Recommended tuning order

Do **not** start by changing the curve.

First make sure the physical timing range is appropriate. Then shape the response with `velocity.h`.

Use this order:

1. Measure actual key timing.
2. Set `MIN_TIME_US`.
3. Set `MAX_TIME_US`.
4. Confirm you can reach the full useful dynamic range.
5. Only then change `VELOCITY_CURVE[]` for preference.

This keeps physical calibration separate from musical feel.

## Step 1 - measure actual key timing

The firmware already contains a timing diagnostic in:

```text
keyboardscanner/midi.cpp
```

Find:

```cpp
// #define DEBUG_VELOCITY_TIMES
// #define DEBUG_MIDI_MESSAGE
```

For a timing test, temporarily change them to:

```cpp
#define DEBUG_VELOCITY_TIMES
#define DEBUG_MIDI_MESSAGE
```

### Important if HIDUINO is installed

Do **not** run these debug modes through HIDUINO.

The debug output is ASCII text, not MIDI. HIDUINO expects MIDI bytes and will misinterpret the debug text.

For timing diagnostics, either:

- restore the normal Mega ATmega16U2 USB-serial firmware first, or
- use another serial/debug connection that does not pass through HIDUINO.

The repository includes:

```text
firmware/atmega16u2/usbserial_mega_16u2.hex
```

for restoring the normal USB serial interface.

### Serial baud rate during diagnostics

The normal PC2 sketch uses:

```cpp
Serial.begin(31250);
```

You may use a terminal that supports 31250 baud.

For easier debugging, you may temporarily change it to:

```cpp
Serial.begin(115200);
```

while the debug defines are enabled.

If you do this, restore it to:

```cpp
Serial.begin(31250);
```

before returning to normal MIDI operation.

### What the diagnostic output means

A key press prints something similar to:

```text
KEY_ON :37 TIME:12400 RAW:110 VEL:110
```

The fields are:

- `KEY_ON` - key press; use these values when calibrating attack velocity
- `37` - internal key index
- `TIME` - measured contact time in microseconds
- `RAW` - velocity after MIN/MAX timing conversion
- `VEL` - final velocity after `VELOCITY_CURVE[]`

You may also see `KEY_OFF` entries. Those describe release timing. For normal playing-feel calibration, concentrate on **KEY_ON**.

## Step 2 - collect useful playing data

Test both the Bass and Treble sections.

For each section, play approximately:

- 10-20 very hard notes
- 10-20 normal notes
- 10-20 very soft notes that you still want the keyboard to detect reliably

Do this on several keys, not only one key.

Write down the approximate timing ranges.

Example only:

```text
Hard playing:    2200-5000 us
Normal playing:  12000-35000 us
Soft playing:    50000-72000 us
```

Your actual PC2 may be different.

If Bass and Treble produce somewhat different times, that is not automatically a wiring problem. The current firmware uses one global timing range for all 76 keys, so choose values that work well across the whole keyboard.

## Step 3 - adjust MIN_TIME_US

`MIN_TIME_US` controls the hard-playing end.

Any measured time **at or below MIN_TIME_US** becomes raw velocity 127.

Current value:

```cpp
#define MIN_TIME_US 1800
```

### If it is too easy to hit velocity 127

Lower `MIN_TIME_US`.

Example:

```cpp
#define MIN_TIME_US 1400
```

This makes fewer strikes reach the 127 ceiling and generally lowers hard/mid velocities slightly.

### If you cannot reach high velocities

Raise `MIN_TIME_US`.

Example:

```cpp
#define MIN_TIME_US 2200
```

This makes the high end easier to reach.

### Recommended adjustment size

Change MIN in relatively small steps, such as:

```text
100-500 us at a time
```

Then retest.

Do not raise it so far that normal playing constantly clamps to 127.

## Step 4 - adjust MAX_TIME_US

`MAX_TIME_US` controls the soft-playing end.

Any measured time **at or above MAX_TIME_US** becomes raw velocity 0, which becomes MIDI velocity 1 for Note On.

Current value:

```cpp
#define MAX_TIME_US 80000
```

### If soft notes collapse to velocity 1 too easily

Raise `MAX_TIME_US`.

Example:

```cpp
#define MAX_TIME_US 90000
```

This gives slower key movement more room before reaching the minimum velocity.

### If soft playing is still too loud

Lower `MAX_TIME_US`.

Example:

```cpp
#define MAX_TIME_US 70000
```

This pushes slower strikes farther down the velocity range.

Be careful: if MAX is too low, many soft notes will clamp to velocity 1.

### Recommended adjustment size

Change MAX in steps such as:

```text
2000-5000 us at a time
```

Then retest.

## What the current 1800-80000 us range does

With the current linear curve, approximate results are:

| Contact time | Raw velocity | Final MIDI velocity |
|---:|---:|---:|
| 1800 us or faster | 127 | 127 |
| 3000 us | 126 | 126 |
| 5000 us | 122 | 122 |
| 10000 us | 114 | 114 |
| 20000 us | 98 | 98 |
| 30000 us | 82 | 82 |
| 40000 us | 65 | 65 |
| 50000 us | 49 | 49 |
| 60000 us | 33 | 33 |
| 70000 us | 17 | 17 |
| 80000 us or slower | 0 | 1 for Note On |

These numbers are useful when deciding whether the physical timing window itself needs adjustment.

## Step 5 - tune the velocity curve

Once MIN and MAX correctly cover your real playing range, use `velocity.h` to change how the keyboard feels.

### Linear - current baseline

The current curve is:

```text
raw 32  -> MIDI 32
raw 64  -> MIDI 64
raw 96  -> MIDI 96
raw 127 -> MIDI 127
```

This is the best baseline for physical calibration.

### Lighter / more sensitive feel

For a lighter feel, make the middle of the table **higher than the raw input**.

Example target points:

| Raw | Lighter output |
|---:|---:|
| 0 | 0 |
| 8 | 16 |
| 16 | 27 |
| 32 | 45 |
| 48 | 61 |
| 64 | 76 |
| 80 | 90 |
| 96 | 103 |
| 112 | 116 |
| 127 | 127 |

This makes normal playing produce stronger MIDI velocities without changing the physical key timing.

### Heavier / less sensitive feel

For a heavier feel, make the middle of the table **lower than the raw input**.

Example target points:

| Raw | Heavier output |
|---:|---:|
| 0 | 0 |
| 8 | 3 |
| 16 | 8 |
| 32 | 20 |
| 48 | 34 |
| 64 | 50 |
| 80 | 68 |
| 96 | 87 |
| 112 | 107 |
| 127 | 127 |

This requires a harder strike to produce high MIDI velocities.

## Generating a full 128-value curve

The easiest way to create a smooth curve is to generate all 128 entries rather than hand-drawing them.

This Python example creates a valid 128-entry table:

```python
gamma = 0.75  # less than 1.0 = lighter; greater than 1.0 = heavier

curve = [
    round(127 * ((i / 127) ** gamma))
    for i in range(128)
]

print(",".join(str(v) for v in curve))
```

Useful starting points:

```text
gamma = 1.00   linear/current
gamma = 0.85   slightly lighter
gamma = 0.75   clearly lighter
gamma = 1.15   slightly heavier
gamma = 1.35   clearly heavier
```

Paste the resulting 128 numbers into:

```text
keyboardscanner/velocity.h
```

inside:

```cpp
const byte VELOCITY_CURVE[] = {
    ...
};
```

## Rules for VELOCITY_CURVE

Keep all of these true:

- exactly **128 entries**
- each value must be from **0 through 127**
- first value must be **0**
- last value must be **127**
- values must never decrease as the index increases

Valid:

```text
0,0,1,1,2,3,4,5,...
```

Invalid:

```text
0,1,2,8,7,9,...
```

because the curve drops from 8 back to 7.

The repository's verification test checks these rules.

## Which setting should I change?

Use this as a quick guide:

| Symptom | Change first |
|---|---|
| Medium/hard notes hit 127 too easily | Lower `MIN_TIME_US` |
| Hardest playing cannot reach high velocity | Raise `MIN_TIME_US` |
| Very soft notes all become velocity 1 | Raise `MAX_TIME_US` |
| Soft playing is still too loud | Lower `MAX_TIME_US` |
| Full range is reachable, but keyboard feels too heavy | Use a lighter `VELOCITY_CURVE` |
| Full range is reachable, but keyboard feels too sensitive | Use a heavier `VELOCITY_CURVE` |
| Most playing occupies only a narrow velocity range | Measure TIME values and recalibrate MIN/MAX first |
| Bass and Treble feel different | Compare their measured TIME ranges before changing anything |

## A safe tuning workflow

A reliable way to tune the keyboard is:

1. Save a copy of the known-working `model.h` and `velocity.h`.
2. Measure KEY_ON timing values.
3. Adjust only `MIN_TIME_US` and `MAX_TIME_US` until hard/normal/soft strikes cover most of raw 1-127.
4. Return `velocity.h` to linear while doing timing calibration.
5. After the timing range feels correct, change only `velocity.h`.
6. Change one thing at a time.
7. Compile and upload.
8. Test several keys in both Bass and Treble sections.
9. Keep notes on each change so you can return to the previous value.
10. Once satisfied, disable both debug defines and restore `Serial.begin(31250)` before using HIDUINO/MIDI normally.

## What not to change for velocity feel

Do **not** change any of these simply to alter velocity response:

- J8/J9 pin assignments
- Bass/Treble contact order in `pins.h`
- `FIRST_KEY`
- `KEYS_NUMBER`
- `SETTLING_TIME_MICROSECONDS`
- octave button pins
- wheel or sustain pins

The Bass/Treble contact order is hardware-verified. Changing it can destroy the velocity measurement entirely; the earlier wrong treble contact order caused every Treble key to produce full velocity.

For normal playing-feel tuning, stay with:

```text
model.h      -> MIN_TIME_US / MAX_TIME_US
velocity.h   -> VELOCITY_CURVE
```

## Returning to the known-working baseline

If tuning goes badly, restore:

```cpp
#define MIN_TIME_US 1800
#define MAX_TIME_US 80000
```

and restore `keyboardscanner/velocity.h` to the linear sequence:

```text
0,1,2,3,...,125,126,127
```

That is the current known-working PC2 baseline.
