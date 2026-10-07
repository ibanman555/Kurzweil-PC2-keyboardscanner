from pathlib import Path
import re

ROOT = Path(__file__).resolve().parents[1]
MODEL = ROOT / "models" / "kurzweil_pc2" / "model.h"
PINS = ROOT / "models" / "kurzweil_pc2" / "pins.h"
VELOCITY = ROOT / "velocity.h"

model = MODEL.read_text()
pins = PINS.read_text()
velocity = VELOCITY.read_text()

def define_int(name):
    m = re.search(rf"^#define\s+{re.escape(name)}\s+(-?\d+)\b", model, re.M)
    assert m, f"missing integer define {name}"
    return int(m.group(1))

expected = {
    "KEYS_NUMBER": 76,
    "FIRST_KEY": 28,
    "MIN_TIME_US": 1800,
    "MAX_TIME_US": 80000,
    "SETTLING_TIME_MICROSECONDS": 3,
    "OCTAVE_DOWN_PIN": 10,
    "OCTAVE_UP_PIN": 11,
    "OCTAVE_DOWN_LED_PIN": 12,
    "OCTAVE_UP_LED_PIN": 13,
}
for name, value in expected.items():
    assert define_int(name) == value, f"{name} mismatch"

expected_signals = {
    "BASS_MK0":22,"BASS_T0":23,"BASS_BR0":24,"BASS_T1":25,
    "BASS_MK1":26,"BASS_T2":27,"BASS_BR1":28,"BASS_T3":29,
    "BASS_MK2":30,"BASS_T4":31,"BASS_BR2":32,"BASS_T5":33,
    "BASS_MK3":34,"BASS_T6":35,"BASS_BR3":36,"BASS_T7":37,
    "TREBLE_MK5":38,"TREBLE_T0":39,"TREBLE_BR5":40,"TREBLE_T1":41,
    "TREBLE_MK6":42,"TREBLE_T2":43,"TREBLE_BR6":44,"TREBLE_T3":45,
    "TREBLE_MK7":46,"TREBLE_T4":47,"TREBLE_BR7":48,"TREBLE_T5":49,
    "TREBLE_MK8":2,"TREBLE_T6":3,"TREBLE_BR8":4,"TREBLE_T7":5,
    "TREBLE_MK9":6,"TREBLE_BR10":7,"TREBLE_BR9":8,"TREBLE_MK10":9,
}
for name, value in expected_signals.items():
    assert define_int(name) == value, f"{name} expected D{value}"

pairs = re.findall(r"^PINS\(([^,]+),\s*([^)]+)\)", pins, re.M)
assert len(pairs) == 152, f"expected 152 contact entries, got {len(pairs)}"

expected_pairs = []
for group in range(4):
    for t in range(8):
        expected_pairs += [(f"BASS_MK{group}", f"BASS_T{t}"), (f"BASS_BR{group}", f"BASS_T{t}")]
for group in range(5, 10):
    for t in range(8):
        expected_pairs += [(f"TREBLE_MK{group}", f"TREBLE_T{t}"), (f"TREBLE_BR{group}", f"TREBLE_T{t}")]
for t in range(4):
    expected_pairs += [("TREBLE_MK10", f"TREBLE_T{t}"), ("TREBLE_BR10", f"TREBLE_T{t}")]

assert pairs == expected_pairs, "pins.h key/contact order differs from verified PC2 mapping"

numeric_pairs = [(expected_signals[o.strip()], expected_signals[i.strip()]) for o, i in pairs]
assert len(set(numeric_pairs)) == 152, "duplicate matrix contact pair found"

matrix_pins = set(expected_signals.values())
control_pins = {10, 11, 12, 13}
assert matrix_pins.isdisjoint(control_pins), "matrix/control digital pin collision"
assert not (matrix_pins & {14,15,16,17,18,19,20,21,50,51,52,53}), "reserved interface pin used by matrix"

body = velocity.split("{", 1)[1].split("}", 1)[0]
values = [int(v) for v in re.findall(r"\b\d+\b", body)]
assert len(values) == 128, f"velocity curve has {len(values)} entries, expected 128"
assert all(0 <= v <= 127 for v in values), "velocity value outside MIDI range"

print("PC2 static verification passed")


# Control and prototype-calibration invariants.
assert re.search(r"^#define\s+PITCH_WHEEL_PIN\s+A0\b", model, re.M)
assert re.search(r"^#define\s+MOD_WHEEL_PIN\s+A1\b", model, re.M)
assert re.search(r"^#define\s+SUSTAIN_PEDAL_PIN\s+A2\b", model, re.M)
assert re.search(r"^#define\s+AFTERTOUCH_PIN\s+A3\b", model, re.M)

pot_text = (ROOT / "potentiometers.cpp").read_text()
assert re.search(r"^#define\s+POTS_PB_CENTER_DEADZONE\s+12\b", pot_text, re.M)
assert re.search(r"^#define\s+MOD_MIN_RAW\s+2\b", pot_text, re.M)
assert re.search(r"^#define\s+MOD_MAX_RAW\s+736\b", pot_text, re.M)
assert "const uint16_t POTS_TYPES" in pot_text

scanner_text = (ROOT / "scanner.cpp").read_text()
assert "sustain_pedal_released_state = digitalRead2(SUSTAIN_PEDAL_PIN);" in scanner_text
assert "octave_shift > -1" in scanner_text and "octave_shift < 1" in scanner_text

midi_text = (ROOT / "midi.cpp").read_text()
assert "note_octave[key_index] = octave_shift;" in midi_text
assert "if (status_byte == 0x90 && vel == 0)" in midi_text

extensions_text = (ROOT / "extensions.h").read_text().strip()
assert extensions_text == "EXTENSION(potentiometers)", "unexpected extension enabled"

assert values == list(range(128)), "velocity curve differs from prototype-verified linear 0..127 baseline"

print("PC2 control/calibration verification passed")
