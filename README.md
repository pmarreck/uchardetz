# uchardetz

A fork of [uchardet](https://github.com/BYVoid/uchardet) (an encoding detector library ported from Mozilla) with **Zig build system** support.

## Zig Build

This fork adds `build.zig` and `build.zig.zon` so the library, shared library, CLI tool, and tests can all be built with the Zig build system (0.15+), making it easy to use as a dependency in Zig projects or to cross-compile for any target Zig supports.

### Building with Zig (via Nix)

```bash
# Enter the dev shell (provides Zig 0.15 and macOS SDK)
nix develop

# Build everything (static lib + shared lib + CLI)
zig build

# Run the CLI
zig build run -- somefile.txt
echo "some text" | ./zig-out/bin/uchardet

# Run the test suite
zig build test
```

### Building with Nix directly

```bash
nix build
# Outputs: result/bin/uchardet, result/lib/libuchardet.{a,dylib}, result/include/uchardet/uchardet.h
```

### Using as a Zig dependency

Add to your `build.zig.zon`:

```zig
.dependencies = .{
    .uchardetz = .{
        .url = "https://github.com/pmarreck/uchardetz/archive/<commit>.tar.gz",
        .hash = "...",
    },
},
```

Then in your `build.zig`:

```zig
const uchardetz_dep = b.dependency("uchardetz", .{
    .target = target,
    .optimize = optimize,
});
your_module.linkLibrary(uchardetz_dep.artifact("uchardet"));
```

## Outputs

- **`libuchardet.a`** - static library
- **`libuchardet.dylib`** (or `.so`) - shared library
- **`uchardet`** - CLI tool
- **`include/uchardet/uchardet.h`** - public C header

## Fixes over upstream

Upstream skipped 5 tests as "known broken." This fork fixes all of them (60/60 passing, 0 regressions). None were fundamental limitations of the detection algorithm -- each had a targeted fix.

| Test | Was returning | Root cause | Fix |
|------|--------------|------------|-----|
| `da/iso-8859-1` | `ISO-8859-15` | Identical confidence scores; ISO-8859-15 prober ran first, winning ties | Swapped Danish prober order so ISO-8859-1 (older, more common) wins ties |
| `da/iso-8859-15` | *(skipped)* | Byte `0xA4` (Euro sign in 8859-15, currency sign in 8859-1) mapped to `SYM` in both models, so they scored identically | Marked `0xA4` as `ILL` in the ISO-8859-1 map -- the currency sign (¤) is essentially never used in real text. When `0xA4` appears, the 8859-1 prober now bails, letting 8859-15 win |
| `es/iso-8859-15` | `ISO-8859-1` | Same `0xA4` ambiguity as Danish | Same `ILL` fix applied to the Spanish ISO-8859-1 map (also applied proactively to French and German maps) |
| `he/iso-8859-8` | `WINDOWS-1255` | **The test data itself was wrong.** It contained logically-ordered Hebrew (final-form letters at word endings), which the prober correctly identifies as Windows-1255. ISO-8859-8 is a *visual* encoding | Replaced test file with visually-ordered Hebrew (byte-reversed runs within lines), giving the final-letter analyzer a clear visual-order signal |
| `ja/utf-16le` | `unknown` | No BOM present. The detector only recognized UTF-16 via BOM (`FE FF` / `FF FE`). Without one, data fell through to charset probers that all scored below threshold | Added a heuristic null-byte pattern detector: scans the first 256 bytes for alternating nulls. >20% nulls at even byte positions with <5% at odd = UTF-16BE; the inverse = UTF-16LE |
| `ja/utf-16be` | `unknown` | Same as above | Same heuristic, opposite null-byte pattern |

## Upstream

This is a fork of [BYVoid/uchardet](https://github.com/BYVoid/uchardet). The canonical upstream project has moved to [freedesktop.org](https://www.freedesktop.org/wiki/Software/uchardet/).

---
