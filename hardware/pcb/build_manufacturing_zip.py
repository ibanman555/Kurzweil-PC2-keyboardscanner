from pathlib import Path
import base64
import hashlib
import zipfile

ROOT = Path(__file__).resolve().parent
PARTS = ROOT / "manufacturing" / "parts"
OUT = ROOT / "PC2-Shield-Gerbers.zip"
EXPECTED_SHA256 = "bba3b698239ae15a7b6911c9d39496f48149648c18474a0976c9111cb7ca5255"

part_files = sorted(PARTS.glob("part*.b64"))
if not part_files:
    raise SystemExit("No PCB package parts found")

encoded = "".join(
    part.read_text(encoding="ascii").strip()
    for part in part_files
)

data = base64.b64decode(encoded, validate=True)
digest = hashlib.sha256(data).hexdigest()

if digest != EXPECTED_SHA256:
    raise SystemExit(
        f"PCB package SHA256 mismatch: expected {EXPECTED_SHA256}, got {digest}"
    )

OUT.write_bytes(data)

with zipfile.ZipFile(OUT, "r") as zf:
    bad = zf.testzip()
    if bad is not None:
        raise SystemExit(f"ZIP integrity check failed at {bad}")

print(f"Wrote {OUT}")
print(f"SHA256 {digest}")
