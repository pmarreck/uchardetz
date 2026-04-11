[English](README.md) | [العربية](README.ar.md) | [Azərbaycan](README.az.md) | [বাংলা](README.bn.md) | [Deutsch](README.de.md) | [Ελληνικά](README.el.md) | [Español](README.es.md) | [فارسی](README.fa.md) | [Français](README.fr.md) | [עברית](README.he.md) | [हिन्दी](README.hi.md) | [Magyar](README.hu.md) | [Italiano](README.it.md) | [日本語](README.ja.md) | [ភាសាខ្មែរ](README.km.md) | [한국어](README.ko.md) | [ਪੰਜਾਬੀ](README.pa.md) | [Polski](README.pl.md) | [پښتو](README.ps.md) | [Português (BR)](README.pt_br.md) | [Română](README.ro.md) | [Русский](README.ru.md) | [Kiswahili](README.sw.md) | [தமிழ்](README.ta.md) | [ไทย](README.th.md) | [Türkçe](README.tr.md) | [Українська](README.uk.md) | [اردو](README.ur.md) | [Tiếng Việt](README.vi.md) | [简体中文](README.zh_hans.md)

# uchardetz

Fork projektu [uchardet](https://github.com/BYVoid/uchardet) (biblioteka do wykrywania kodowania przeniesiona z Mozilli) z obsluga **systemu budowania Zig**.

## Budowanie w Zig

Ten fork dodaje pliki `build.zig` i `build.zig.zon`, dzieki czemu biblioteke statyczna, biblioteke wspoldzielona, narzedzie CLI i testy mozna zbudowac za pomoca systemu budowania Zig (0.15+). Ulatwia to wykorzystanie jako zaleznosc w projektach Zig lub kompilacje krzyzowa na dowolna platforme obslugiwana przez Zig.

### Budowanie za pomoca Zig (przez Nix)

```bash
# Wejdz do powloki deweloperskiej (zapewnia Zig 0.15 i macOS SDK)
nix develop

# Zbuduj wszystko (biblioteka statyczna + wspoldzielona + CLI)
zig build

# Uruchom CLI
zig build run -- somefile.txt
echo "some text" | ./zig-out/bin/uchardet

# Uruchom zestaw testow
zig build test
```

### Budowanie bezposrednio przez Nix

```bash
nix build
# Wyniki: result/bin/uchardet, result/lib/libuchardet.{a,dylib}, result/include/uchardet/uchardet.h
```

### Uzycie jako zaleznosc Zig

Dodaj do swojego `build.zig.zon`:

```zig
.dependencies = .{
    .uchardetz = .{
        .url = "https://github.com/pmarreck/uchardetz/archive/<commit>.tar.gz",
        .hash = "...",
    },
},
```

Nastepnie w swoim `build.zig`:

```zig
const uchardetz_dep = b.dependency("uchardetz", .{
    .target = target,
    .optimize = optimize,
});
your_module.linkLibrary(uchardetz_dep.artifact("uchardet"));
```

## Wyniki budowania

- **`libuchardet.a`** - biblioteka statyczna
- **`libuchardet.dylib`** (lub `.so`) - biblioteka wspoldzielona
- **`uchardet`** - narzedzie CLI
- **`include/uchardet/uchardet.h`** - publiczny naglowek C

## Poprawki wzgledem upstreamu

Upstream pominal 5 testow jako "znane bledy." Ten fork naprawia je wszystkie (60/60 przechodzi, 0 regresji). Zaden z nich nie byl fundamentalnym ograniczeniem algorytmu detekcji -- kazdy mial celowana poprawke.

| Test | Zwracal | Przyczyna | Poprawka |
|------|--------------|------------|-----|
| `da/iso-8859-1` | `ISO-8859-15` | Identyczne wyniki pewnosci; prober ISO-8859-15 uruchamial sie pierwszy, wygrywajac remisy | Zmieniono kolejnosc probera dunskiego, aby ISO-8859-1 (starszy, bardziej powszechny) wygrywal remisy |
| `da/iso-8859-15` | *(pominiety)* | Bajt `0xA4` (znak Euro w 8859-15, znak waluty w 8859-1) mapowany jako `SYM` w obu modelach, wiec wyniki byly identyczne | Oznaczono `0xA4` jako `ILL` w mapie ISO-8859-1 -- znak waluty (¤) praktycznie nie jest uzywany w rzeczywistym tekscie. Gdy pojawia sie `0xA4`, prober 8859-1 rezygnuje, pozwalajac 8859-15 wygrac |
| `es/iso-8859-15` | `ISO-8859-1` | Ta sama niejednoznacznosc `0xA4` co w dunskim | Ta sama poprawka `ILL` zastosowana do hiszpanskiej mapy ISO-8859-1 (rowniez proaktywnie zastosowana do map francuskiej i niemieckiej) |
| `he/iso-8859-8` | `WINDOWS-1255` | **Same dane testowe byly bledne.** Zawieraly hebrajski w kolejnosci logicznej (litery w formie koncowej na koncach slow), co prober poprawnie identyfikuje jako Windows-1255. ISO-8859-8 to kodowanie *wizualne* | Zastapiono plik testowy hebrajskim w kolejnosci wizualnej (odwrocone bajty w obrebie linii), co daje analizatorowi liter koncowych wyrazny sygnal kolejnosci wizualnej |
| `ja/utf-16le` | `unknown` | Brak BOM. Detektor rozpoznawal UTF-16 wylacznie przez BOM (`FE FF` / `FF FE`). Bez niego dane przechodzilydo proberow kodowania, ktore wszystkie osiagaly wynik ponizej progu | Dodano heurystyczny detektor wzorca bajtow null: skanuje pierwsze 256 bajtow w poszukiwaniu naprzemiennych nulli. >20% nulli na pozycjach parzystych przy <5% na nieparzystych = UTF-16BE; odwrotnie = UTF-16LE |
| `ja/utf-16be` | `unknown` | Jak wyzej | Ta sama heurystyka, odwrotny wzorzec bajtow null |

## Upstream

To jest fork [BYVoid/uchardet](https://github.com/BYVoid/uchardet). Kanoniczny projekt upstream przeniosl sie na [freedesktop.org](https://www.freedesktop.org/wiki/Software/uchardet/).

---
