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

## Upstream

This is a fork of [BYVoid/uchardet](https://github.com/BYVoid/uchardet). The canonical upstream project has moved to [freedesktop.org](https://www.freedesktop.org/wiki/Software/uchardet/).

---
