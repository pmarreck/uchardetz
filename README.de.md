[English](README.md) | [العربية](README.ar.md) | [Azərbaycan](README.az.md) | [বাংলা](README.bn.md) | [Deutsch](README.de.md) | [Ελληνικά](README.el.md) | [Español](README.es.md) | [فارسی](README.fa.md) | [Français](README.fr.md) | [עברית](README.he.md) | [हिन्दी](README.hi.md) | [Magyar](README.hu.md) | [Italiano](README.it.md) | [日本語](README.ja.md) | [ភាសាខ្មែរ](README.km.md) | [한국어](README.ko.md) | [ਪੰਜਾਬੀ](README.pa.md) | [Polski](README.pl.md) | [پښتو](README.ps.md) | [Português (BR)](README.pt_br.md) | [Română](README.ro.md) | [Русский](README.ru.md) | [Kiswahili](README.sw.md) | [தமிழ்](README.ta.md) | [ไทย](README.th.md) | [Türkçe](README.tr.md) | [Українська](README.uk.md) | [اردو](README.ur.md) | [Tiếng Việt](README.vi.md) | [简体中文](README.zh_hans.md)

# uchardetz

Ein Fork von [uchardet](https://github.com/BYVoid/uchardet) (einer von Mozilla portierten Bibliothek zur Zeichenkodierungserkennung) mit Unterstützung für das **Zig-Build-System**.

## Zig-Build

Dieser Fork fügt `build.zig` und `build.zig.zon` hinzu, sodass die Bibliothek, die Shared Library, das CLI-Tool und die Tests alle mit dem Zig-Build-System (0.15+) gebaut werden können. Das macht es einfach, die Bibliothek als Abhängigkeit in Zig-Projekten zu verwenden oder für jedes von Zig unterstützte Ziel zu cross-kompilieren.

### Mit Zig bauen (über Nix)

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

### Direkt mit Nix bauen

```bash
nix build
# Outputs: result/bin/uchardet, result/lib/libuchardet.{a,dylib}, result/include/uchardet/uchardet.h
```

### Als Zig-Abhängigkeit verwenden

Füge Folgendes zu deiner `build.zig.zon` hinzu:

```zig
.dependencies = .{
    .uchardetz = .{
        .url = "https://github.com/pmarreck/uchardetz/archive/<commit>.tar.gz",
        .hash = "...",
    },
},
```

Dann in deiner `build.zig`:

```zig
const uchardetz_dep = b.dependency("uchardetz", .{
    .target = target,
    .optimize = optimize,
});
your_module.linkLibrary(uchardetz_dep.artifact("uchardet"));
```

## Ausgaben

- **`libuchardet.a`** - statische Bibliothek
- **`libuchardet.dylib`** (oder `.so`) - Shared Library
- **`uchardet`** - CLI-Tool
- **`include/uchardet/uchardet.h`** - öffentlicher C-Header

## Fehlerbehebungen gegenüber Upstream

Upstream hat 5 Tests als "bekanntermaßen fehlerhaft" übersprungen. Dieser Fork behebt alle (60/60 bestanden, 0 Regressionen). Keiner davon war eine grundsätzliche Einschränkung des Erkennungsalgorithmus -- jeder hatte eine gezielte Lösung.

| Test | Gab zurück | Ursache | Lösung |
|------|--------------|------------|-----|
| `da/iso-8859-1` | `ISO-8859-15` | Identische Konfidenzwerte; der ISO-8859-15-Prober lief zuerst und gewann bei Gleichstand | Reihenfolge des dänischen Probers getauscht, sodass ISO-8859-1 (älter, verbreiteter) bei Gleichstand gewinnt |
| `da/iso-8859-15` | *(übersprungen)* | Byte `0xA4` (Euro-Zeichen in 8859-15, Währungszeichen in 8859-1) war in beiden Modellen als `SYM` zugeordnet, daher identische Bewertung | `0xA4` in der ISO-8859-1-Map als `ILL` markiert -- das Währungszeichen (¤) wird in echtem Text praktisch nie verwendet. Wenn `0xA4` auftritt, bricht der 8859-1-Prober jetzt ab und lässt 8859-15 gewinnen |
| `es/iso-8859-15` | `ISO-8859-1` | Dieselbe `0xA4`-Mehrdeutigkeit wie bei Dänisch | Derselbe `ILL`-Fix auf die spanische ISO-8859-1-Map angewendet (ebenfalls proaktiv auf die französische und deutsche Map angewendet) |
| `he/iso-8859-8` | `WINDOWS-1255` | **Die Testdaten selbst waren falsch.** Sie enthielten logisch geordnetes Hebräisch (Endbuchstaben an Wortenden), was der Prober korrekt als Windows-1255 identifiziert. ISO-8859-8 ist eine *visuelle* Kodierung | Testdatei durch visuell geordnetes Hebräisch ersetzt (byte-umgekehrte Sequenzen innerhalb der Zeilen), was dem Endbuchstaben-Analysator ein eindeutiges visuelles Ordnungssignal gibt |
| `ja/utf-16le` | `unknown` | Kein BOM vorhanden. Der Detektor erkannte UTF-16 nur über BOM (`FE FF` / `FF FE`). Ohne BOM fielen die Daten an Charset-Prober durch, die alle unter dem Schwellenwert lagen | Heuristischer Null-Byte-Musterdetektor hinzugefügt: Untersucht die ersten 256 Bytes auf alternierende Nullen. >20% Nullen an geraden Byte-Positionen bei <5% an ungeraden = UTF-16BE; umgekehrt = UTF-16LE |
| `ja/utf-16be` | `unknown` | Wie oben | Dieselbe Heuristik, umgekehrtes Null-Byte-Muster |

## Upstream

Dies ist ein Fork von [BYVoid/uchardet](https://github.com/BYVoid/uchardet). Das kanonische Upstream-Projekt ist zu [freedesktop.org](https://www.freedesktop.org/wiki/Software/uchardet/) umgezogen.

---
