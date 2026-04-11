[English](../README.md) | [العربية](README.ar.md) | [Azərbaycan](README.az.md) | [বাংলা](README.bn.md) | [Deutsch](README.de.md) | [Ελληνικά](README.el.md) | [Español](README.es.md) | [فارسی](README.fa.md) | [Français](README.fr.md) | [עברית](README.he.md) | [हिन्दी](README.hi.md) | [Magyar](README.hu.md) | [Italiano](README.it.md) | [日本語](README.ja.md) | [ភាសាខ្មែរ](README.km.md) | [한국어](README.ko.md) | [ਪੰਜਾਬੀ](README.pa.md) | [Polski](README.pl.md) | [پښتو](README.ps.md) | [Português (BR)](README.pt_br.md) | [Română](README.ro.md) | [Русский](README.ru.md) | [Kiswahili](README.sw.md) | [தமிழ்](README.ta.md) | [ไทย](README.th.md) | [Türkçe](README.tr.md) | [Українська](README.uk.md) | [اردو](README.ur.md) | [Tiếng Việt](README.vi.md) | [简体中文](README.zh_hans.md)

# uchardetz

[uchardet](https://github.com/BYVoid/uchardet) (Mozilla ਤੋਂ ਪੋਰਟ ਕੀਤੀ ਇੱਕ ਇੰਕੋਡਿੰਗ ਖੋਜ ਲਾਇਬ੍ਰੇਰੀ) ਦਾ ਇੱਕ ਫੋਰਕ ਜਿਸ ਵਿੱਚ **Zig ਬਿਲਡ ਸਿਸਟਮ** ਦਾ ਸਮਰਥਨ ਸ਼ਾਮਲ ਹੈ।

## Zig ਬਿਲਡ

ਇਹ ਫੋਰਕ `build.zig` ਅਤੇ `build.zig.zon` ਸ਼ਾਮਲ ਕਰਦਾ ਹੈ ਤਾਂ ਜੋ ਲਾਇਬ੍ਰੇਰੀ, ਸ਼ੇਅਰਡ ਲਾਇਬ੍ਰੇਰੀ, CLI ਟੂਲ, ਅਤੇ ਟੈਸਟ ਸਭ Zig ਬਿਲਡ ਸਿਸਟਮ (0.15+) ਨਾਲ ਬਣਾਏ ਜਾ ਸਕਣ। ਇਸ ਨਾਲ Zig ਪ੍ਰੋਜੈਕਟਾਂ ਵਿੱਚ ਡਿਪੈਂਡੈਂਸੀ ਵਜੋਂ ਵਰਤਣਾ ਜਾਂ Zig ਦੁਆਰਾ ਸਮਰਥਿਤ ਕਿਸੇ ਵੀ ਟਾਰਗੇਟ ਲਈ ਕਰਾਸ-ਕੰਪਾਈਲ ਕਰਨਾ ਸੌਖਾ ਹੋ ਜਾਂਦਾ ਹੈ।

### Zig ਨਾਲ ਬਿਲਡ ਕਰਨਾ (Nix ਰਾਹੀਂ)

```bash
# ਡੈਵ ਸ਼ੈੱਲ ਵਿੱਚ ਦਾਖਲ ਹੋਵੋ (Zig 0.15 ਅਤੇ macOS SDK ਮੁਹੱਈਆ ਕਰਦਾ ਹੈ)
nix develop

# ਸਭ ਕੁਝ ਬਿਲਡ ਕਰੋ (static lib + shared lib + CLI)
zig build

# CLI ਚਲਾਓ
zig build run -- somefile.txt
echo "some text" | ./zig-out/bin/uchardet

# ਟੈਸਟ ਸੂਟ ਚਲਾਓ
zig build test
```

### Nix ਨਾਲ ਸਿੱਧੇ ਬਿਲਡ ਕਰਨਾ

```bash
nix build
# ਆਊਟਪੁੱਟ: result/bin/uchardet, result/lib/libuchardet.{a,dylib}, result/include/uchardet/uchardet.h
```

### Zig ਡਿਪੈਂਡੈਂਸੀ ਵਜੋਂ ਵਰਤਣਾ

ਆਪਣੀ `build.zig.zon` ਵਿੱਚ ਸ਼ਾਮਲ ਕਰੋ:

```zig
.dependencies = .{
    .uchardetz = .{
        .url = "https://github.com/pmarreck/uchardetz/archive/<commit>.tar.gz",
        .hash = "...",
    },
},
```

ਫਿਰ ਆਪਣੀ `build.zig` ਵਿੱਚ:

```zig
const uchardetz_dep = b.dependency("uchardetz", .{
    .target = target,
    .optimize = optimize,
});
your_module.linkLibrary(uchardetz_dep.artifact("uchardet"));
```

## ਆਊਟਪੁੱਟ

- **`libuchardet.a`** - ਸਟੈਟਿਕ ਲਾਇਬ੍ਰੇਰੀ
- **`libuchardet.dylib`** (ਜਾਂ `.so`) - ਸ਼ੇਅਰਡ ਲਾਇਬ੍ਰੇਰੀ
- **`uchardet`** - CLI ਟੂਲ
- **`include/uchardet/uchardet.h`** - ਪਬਲਿਕ C ਹੈਡਰ

## ਅੱਪਸਟ੍ਰੀਮ ਉੱਤੇ ਫਿਕਸ

ਅੱਪਸਟ੍ਰੀਮ ਨੇ 5 ਟੈਸਟ "ਜਾਣੇ-ਪਛਾਣੇ ਟੁੱਟੇ ਹੋਏ" ਵਜੋਂ ਛੱਡ ਦਿੱਤੇ ਸਨ। ਇਹ ਫੋਰਕ ਉਨ੍ਹਾਂ ਸਭ ਨੂੰ ਠੀਕ ਕਰਦਾ ਹੈ (60/60 ਪਾਸ, 0 ਰੀਗਰੈਸ਼ਨ)। ਇਨ੍ਹਾਂ ਵਿੱਚੋਂ ਕੋਈ ਵੀ ਖੋਜ ਐਲਗੋਰਿਦਮ ਦੀ ਬੁਨਿਆਦੀ ਸੀਮਾ ਨਹੀਂ ਸੀ -- ਹਰ ਇੱਕ ਦਾ ਨਿਸ਼ਾਨਾ ਫਿਕਸ ਸੀ।

| ਟੈਸਟ | ਵਾਪਸ ਕਰ ਰਿਹਾ ਸੀ | ਮੂਲ ਕਾਰਨ | ਫਿਕਸ |
|------|--------------|------------|-----|
| `da/iso-8859-1` | `ISO-8859-15` | ਇੱਕੋ ਜਿਹੇ ਕਾਨਫੀਡੈਂਸ ਸਕੋਰ; ISO-8859-15 prober ਪਹਿਲਾਂ ਚੱਲਿਆ, ਬਰਾਬਰੀ ਵਿੱਚ ਜਿੱਤ ਗਿਆ | ਡੈਨਿਸ਼ prober ਕ੍ਰਮ ਬਦਲਿਆ ਤਾਂ ਜੋ ISO-8859-1 (ਪੁਰਾਣਾ, ਵਧੇਰੇ ਆਮ) ਬਰਾਬਰੀ ਵਿੱਚ ਜਿੱਤੇ |
| `da/iso-8859-15` | *(ਛੱਡਿਆ ਗਿਆ)* | ਬਾਈਟ `0xA4` (8859-15 ਵਿੱਚ ਯੂਰੋ ਚਿੰਨ੍ਹ, 8859-1 ਵਿੱਚ ਕਰੰਸੀ ਚਿੰਨ੍ਹ) ਦੋਵਾਂ ਮਾਡਲਾਂ ਵਿੱਚ `SYM` ਨਾਲ ਮੈਪ ਸੀ, ਇਸ ਲਈ ਸਕੋਰ ਇੱਕੋ ਜਿਹੇ ਆਏ | ISO-8859-1 ਮੈਪ ਵਿੱਚ `0xA4` ਨੂੰ `ILL` ਮਾਰਕ ਕੀਤਾ -- ਕਰੰਸੀ ਚਿੰਨ੍ਹ (¤) ਅਸਲ ਟੈਕਸਟ ਵਿੱਚ ਲਗਭਗ ਕਦੇ ਵਰਤਿਆ ਨਹੀਂ ਜਾਂਦਾ। ਜਦੋਂ `0xA4` ਆਉਂਦਾ ਹੈ, 8859-1 prober ਹੁਣ ਛੱਡ ਦਿੰਦਾ ਹੈ, ਜਿਸ ਨਾਲ 8859-15 ਜਿੱਤਦਾ ਹੈ |
| `es/iso-8859-15` | `ISO-8859-1` | ਡੈਨਿਸ਼ ਵਾਂਗ ਹੀ `0xA4` ਦੀ ਅਸਪੱਸ਼ਟਤਾ | ਉਹੀ `ILL` ਫਿਕਸ ਸਪੈਨਿਸ਼ ISO-8859-1 ਮੈਪ ਤੇ ਲਾਗੂ ਕੀਤਾ (ਫ੍ਰੈਂਚ ਅਤੇ ਜਰਮਨ ਮੈਪਾਂ ਤੇ ਵੀ ਪਹਿਲਾਂ ਤੋਂ ਲਾਗੂ ਕੀਤਾ) |
| `he/iso-8859-8` | `WINDOWS-1255` | **ਟੈਸਟ ਡਾਟਾ ਆਪ ਗਲਤ ਸੀ।** ਇਸ ਵਿੱਚ ਤਰਕ-ਕ੍ਰਮ ਵਾਲੀ ਹਿਬਰੂ ਸੀ (ਸ਼ਬਦਾਂ ਦੇ ਅੰਤ ਵਿੱਚ ਫਾਈਨਲ-ਫਾਰਮ ਅੱਖਰ), ਜਿਸ ਨੂੰ prober ਸਹੀ ਤਰ੍ਹਾਂ Windows-1255 ਪਛਾਣਦਾ ਹੈ। ISO-8859-8 ਇੱਕ *ਵਿਜ਼ੁਅਲ* ਇੰਕੋਡਿੰਗ ਹੈ | ਟੈਸਟ ਫਾਈਲ ਨੂੰ ਵਿਜ਼ੁਅਲ-ਕ੍ਰਮ ਵਾਲੀ ਹਿਬਰੂ (ਲਾਈਨਾਂ ਵਿੱਚ ਬਾਈਟ-ਰਿਵਰਸ ਕੀਤੇ ਹਿੱਸੇ) ਨਾਲ ਬਦਲਿਆ, ਜਿਸ ਨਾਲ ਫਾਈਨਲ-ਲੈਟਰ ਐਨਾਲਾਈਜ਼ਰ ਨੂੰ ਸਪੱਸ਼ਟ ਵਿਜ਼ੁਅਲ-ਕ੍ਰਮ ਸਿਗਨਲ ਮਿਲਦਾ ਹੈ |
| `ja/utf-16le` | `unknown` | ਕੋਈ BOM ਮੌਜੂਦ ਨਹੀਂ ਸੀ। ਡਿਟੈਕਟਰ ਸਿਰਫ BOM (`FE FF` / `FF FE`) ਰਾਹੀਂ UTF-16 ਪਛਾਣਦਾ ਸੀ। ਇਸ ਤੋਂ ਬਿਨਾਂ, ਡਾਟਾ charset probers ਤੱਕ ਪਹੁੰਚ ਜਾਂਦਾ ਸੀ ਜੋ ਸਭ ਥ੍ਰੈਸ਼ਹੋਲਡ ਤੋਂ ਹੇਠਾਂ ਸਕੋਰ ਕਰਦੇ ਸਨ | ਇੱਕ ਹਿਊਰਿਸਟਿਕ null-byte ਪੈਟਰਨ ਡਿਟੈਕਟਰ ਸ਼ਾਮਲ ਕੀਤਾ: ਪਹਿਲੇ 256 ਬਾਈਟਾਂ ਵਿੱਚ ਬਦਲਵੇਂ nulls ਦੀ ਜਾਂਚ ਕਰਦਾ ਹੈ। ਸਮ ਬਾਈਟ ਸਥਿਤੀਆਂ ਤੇ >20% nulls ਅਤੇ ਵਿਸ਼ਮ ਤੇ <5% = UTF-16BE; ਉਲਟਾ = UTF-16LE |
| `ja/utf-16be` | `unknown` | ਉੱਪਰ ਵਾਂਗ ਹੀ | ਉਹੀ ਹਿਊਰਿਸਟਿਕ, ਉਲਟਾ null-byte ਪੈਟਰਨ |

## ਅੱਪਸਟ੍ਰੀਮ

ਇਹ [BYVoid/uchardet](https://github.com/BYVoid/uchardet) ਦਾ ਫੋਰਕ ਹੈ। ਮੂਲ ਅੱਪਸਟ੍ਰੀਮ ਪ੍ਰੋਜੈਕਟ [freedesktop.org](https://www.freedesktop.org/wiki/Software/uchardet/) ਤੇ ਚਲਾ ਗਿਆ ਹੈ।

---
