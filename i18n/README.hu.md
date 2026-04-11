[English](../README.md) | [العربية](README.ar.md) | [Azərbaycan](README.az.md) | [বাংলা](README.bn.md) | [Deutsch](README.de.md) | [Ελληνικά](README.el.md) | [Español](README.es.md) | [فارسی](README.fa.md) | [Français](README.fr.md) | [עברית](README.he.md) | [हिन्दी](README.hi.md) | [Magyar](README.hu.md) | [Italiano](README.it.md) | [日本語](README.ja.md) | [ភាសាខ្មែរ](README.km.md) | [한국어](README.ko.md) | [ਪੰਜਾਬੀ](README.pa.md) | [Polski](README.pl.md) | [پښتو](README.ps.md) | [Português (BR)](README.pt_br.md) | [Română](README.ro.md) | [Русский](README.ru.md) | [Kiswahili](README.sw.md) | [தமிழ்](README.ta.md) | [ไทย](README.th.md) | [Türkçe](README.tr.md) | [Українська](README.uk.md) | [اردو](README.ur.md) | [Tiếng Việt](README.vi.md) | [简体中文](README.zh_hans.md)

# uchardetz

Az [uchardet](https://github.com/BYVoid/uchardet) (egy Mozillából portolt karakterkódolás-felismerő könyvtár) forkja, **Zig build rendszer** támogatással.

## Zig Build

Ez a fork hozzáadja a `build.zig` és `build.zig.zon` fájlokat, így a könyvtár, a megosztott könyvtár, a CLI eszköz és a tesztek mind a Zig build rendszerrel (0.15+) építhetők. Ezáltal egyszerűen használható függőségként Zig projektekben, vagy cross-kompilálható bármely Zig által támogatott platformra.

### Építés Ziggel (Nixen keresztül)

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

### Építés közvetlenül Nixszel

```bash
nix build
# Outputs: result/bin/uchardet, result/lib/libuchardet.{a,dylib}, result/include/uchardet/uchardet.h
```

### Használat Zig függőségként

Add hozzá a `build.zig.zon` fájlodhoz:

```zig
.dependencies = .{
    .uchardetz = .{
        .url = "https://github.com/pmarreck/uchardetz/archive/<commit>.tar.gz",
        .hash = "...",
    },
},
```

Majd a `build.zig` fájlodban:

```zig
const uchardetz_dep = b.dependency("uchardetz", .{
    .target = target,
    .optimize = optimize,
});
your_module.linkLibrary(uchardetz_dep.artifact("uchardet"));
```

## Kimenetek

- **`libuchardet.a`** – statikus könyvtár
- **`libuchardet.dylib`** (vagy `.so`) – megosztott könyvtár
- **`uchardet`** – CLI eszköz
- **`include/uchardet/uchardet.h`** – publikus C fejlécfájl

## Javítások az upstreamhez képest

Az upstream 5 tesztet kihagyott „ismerten hibásként". Ez a fork mindet javítja (60/60 sikeres, 0 regresszió). Egyik sem volt a felismerési algoritmus alapvető korlátja – mindegyikhez célzott javítás tartozott.

| Teszt | Visszaadott érték | Gyökérok | Javítás |
|-------|-------------------|----------|---------|
| `da/iso-8859-1` | `ISO-8859-15` | Azonos megbízhatósági pontszámok; az ISO-8859-15 prober futott először, ezért döntetlen esetén ő nyert | A dán prober sorrend megcserélése, hogy az ISO-8859-1 (régebbi, elterjedtebb) nyerjen döntetlen esetén |
| `da/iso-8859-15` | *(kihagyva)* | A `0xA4` bájt (euró jel a 8859-15-ben, pénznem jel a 8859-1-ben) mindkét modellben `SYM`-ként volt leképezve, így azonos pontszámot kaptak | A `0xA4` megjelölése `ILL`-ként az ISO-8859-1 térképben – a pénznem jel (¤) a gyakorlatban szinte soha nem fordul elő. Ha `0xA4` jelenik meg, a 8859-1 prober most kilép, és a 8859-15 nyer |
| `es/iso-8859-15` | `ISO-8859-1` | Ugyanaz a `0xA4` kétértelműség, mint a dánnál | Ugyanaz az `ILL` javítás a spanyol ISO-8859-1 térképre (proaktívan alkalmazva a francia és német térképekre is) |
| `he/iso-8859-8` | `WINDOWS-1255` | **Maga a tesztadat volt hibás.** Logikai sorrendű héber szöveget tartalmazott (szóvégi betűk a szavak végén), amelyet a prober helyesen Windows-1255-ként azonosít. Az ISO-8859-8 egy *vizuális* kódolás | A tesztfájl cseréje vizuálisan rendezett héber szövegre (bájt-fordított futamok a sorokon belül), így a szóvégi betű elemző egyértelmű vizuális sorrend jelet kap |
| `ja/utf-16le` | `unknown` | Nem volt BOM. A detektor csak BOM-on (`FE FF` / `FF FE`) keresztül ismerte fel az UTF-16-ot. Enélkül az adatok átestek a charset probereken, amelyek mind a küszöbérték alatt pontoztak | Heurisztikus null-bájt mintafelismerő hozzáadása: az első 256 bájtot vizsgálja váltakozó null-ok után. >20% null páros pozíciókban <5%-kal páratlan pozíciókban = UTF-16BE; fordítva = UTF-16LE |
| `ja/utf-16be` | `unknown` | Ugyanaz, mint fent | Ugyanaz a heurisztika, ellentétes null-bájt mintával |

## Upstream

Ez az [BYVoid/uchardet](https://github.com/BYVoid/uchardet) forkja. A kanonikus upstream projekt a [freedesktop.org](https://www.freedesktop.org/wiki/Software/uchardet/) oldalra költözött.

---
