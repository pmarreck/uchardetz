[English](../README.md) | [العربية](README.ar.md) | [Azərbaycan](README.az.md) | [বাংলা](README.bn.md) | [Deutsch](README.de.md) | [Ελληνικά](README.el.md) | [Español](README.es.md) | [فارسی](README.fa.md) | [Français](README.fr.md) | [עברית](README.he.md) | [हिन्दी](README.hi.md) | [Magyar](README.hu.md) | [Italiano](README.it.md) | [日本語](README.ja.md) | [ភាសាខ្មែរ](README.km.md) | [한국어](README.ko.md) | [ਪੰਜਾਬੀ](README.pa.md) | [Polski](README.pl.md) | [پښتو](README.ps.md) | [Português (BR)](README.pt_br.md) | [Română](README.ro.md) | [Русский](README.ru.md) | [Kiswahili](README.sw.md) | [தமிழ்](README.ta.md) | [ไทย](README.th.md) | [Türkçe](README.tr.md) | [Українська](README.uk.md) | [اردو](README.ur.md) | [Tiếng Việt](README.vi.md) | [简体中文](README.zh_hans.md)

# uchardetz

Un fork al [uchardet](https://github.com/BYVoid/uchardet) (o biblioteca de detectare a codificarii portata din Mozilla) cu suport pentru **sistemul de build Zig**.

## Build cu Zig

Acest fork adauga `build.zig` si `build.zig.zon` astfel incat biblioteca statica, biblioteca partajata, unealta CLI si testele pot fi compilate cu sistemul de build Zig (0.15+). Acest lucru faciliteaza utilizarea ca dependinta in proiecte Zig sau compilarea incrucisata pentru orice tinta suportata de Zig.

### Compilare cu Zig (prin Nix)

```bash
# Intra in shell-ul de dezvoltare (ofera Zig 0.15 si macOS SDK)
nix develop

# Compileaza totul (lib statica + lib partajata + CLI)
zig build

# Ruleaza CLI
zig build run -- somefile.txt
echo "some text" | ./zig-out/bin/uchardet

# Ruleaza suita de teste
zig build test
```

### Compilare directa cu Nix

```bash
nix build
# Rezultate: result/bin/uchardet, result/lib/libuchardet.{a,dylib}, result/include/uchardet/uchardet.h
```

### Utilizare ca dependinta Zig

Adauga in `build.zig.zon`:

```zig
.dependencies = .{
    .uchardetz = .{
        .url = "https://github.com/pmarreck/uchardetz/archive/<commit>.tar.gz",
        .hash = "...",
    },
},
```

Apoi in `build.zig`:

```zig
const uchardetz_dep = b.dependency("uchardetz", .{
    .target = target,
    .optimize = optimize,
});
your_module.linkLibrary(uchardetz_dep.artifact("uchardet"));
```

## Rezultate

- **`libuchardet.a`** - biblioteca statica
- **`libuchardet.dylib`** (sau `.so`) - biblioteca partajata
- **`uchardet`** - unealta CLI
- **`include/uchardet/uchardet.h`** - header public C

## Corectii fata de upstream

Upstream-ul omitea 5 teste ca „defecte cunoscute". Acest fork le corecteaza pe toate (60/60 trecute, 0 regresii). Niciuna nu era o limitare fundamentala a algoritmului de detectare -- fiecare a avut o corectie tintita.

| Test | Returna | Cauza principala | Corectie |
|------|--------------|------------|-----|
| `da/iso-8859-1` | `ISO-8859-15` | Scoruri de incredere identice; prober-ul ISO-8859-15 rula primul, castigand egalitatea | S-a schimbat ordinea prober-ului danez astfel incat ISO-8859-1 (mai vechi, mai comun) sa castige egalitatea |
| `da/iso-8859-15` | *(omis)* | Byte-ul `0xA4` (semnul Euro in 8859-15, semnul monedei in 8859-1) era mapat ca `SYM` in ambele modele, deci scorurile erau identice | S-a marcat `0xA4` ca `ILL` in harta ISO-8859-1 -- semnul monedei (¤) nu este practic niciodata folosit in text real. Cand apare `0xA4`, prober-ul 8859-1 renunta acum, permitand castigul 8859-15 |
| `es/iso-8859-15` | `ISO-8859-1` | Aceeasi ambiguitate `0xA4` ca la daneza | Aceeasi corectie `ILL` aplicata hartii spaniole ISO-8859-1 (aplicata proactiv si hartilor franceze si germane) |
| `he/iso-8859-8` | `WINDOWS-1255` | **Datele de test in sine erau gresite.** Contineau ebraica in ordine logica (litere in forma finala la sfarsitul cuvintelor), pe care prober-ul o identifica corect ca Windows-1255. ISO-8859-8 este o codificare *vizuala* | S-a inlocuit fisierul de test cu ebraica in ordine vizuala (secvente de bytes inversate in cadrul liniilor), oferind analizorului de litere finale un semnal clar de ordine vizuala |
| `ja/utf-16le` | `unknown` | Fara BOM prezent. Detectorul recunostea UTF-16 doar prin BOM (`FE FF` / `FF FE`). Fara acesta, datele ajungeau la prober-ele de charset care scoteau scoruri sub prag | S-a adaugat un detector euristic de pattern null-byte: scaneaza primii 256 de bytes cautand null-uri alternante. >20% null-uri pe pozitii pare cu <5% pe impare = UTF-16BE; invers = UTF-16LE |
| `ja/utf-16be` | `unknown` | La fel ca mai sus | Aceeasi euristica, pattern de null-byte inversat |

## Upstream

Acesta este un fork al [BYVoid/uchardet](https://github.com/BYVoid/uchardet). Proiectul upstream canonic s-a mutat pe [freedesktop.org](https://www.freedesktop.org/wiki/Software/uchardet/).

---
