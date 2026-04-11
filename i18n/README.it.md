[English](../README.md) | [العربية](README.ar.md) | [Azərbaycan](README.az.md) | [বাংলা](README.bn.md) | [Deutsch](README.de.md) | [Ελληνικά](README.el.md) | [Español](README.es.md) | [فارسی](README.fa.md) | [Français](README.fr.md) | [עברית](README.he.md) | [हिन्दी](README.hi.md) | [Magyar](README.hu.md) | [Italiano](README.it.md) | [日本語](README.ja.md) | [ភាសាខ្មែរ](README.km.md) | [한국어](README.ko.md) | [ਪੰਜਾਬੀ](README.pa.md) | [Polski](README.pl.md) | [پښتو](README.ps.md) | [Português (BR)](README.pt_br.md) | [Română](README.ro.md) | [Русский](README.ru.md) | [Kiswahili](README.sw.md) | [தமிழ்](README.ta.md) | [ไทย](README.th.md) | [Türkçe](README.tr.md) | [Українська](README.uk.md) | [اردو](README.ur.md) | [Tiếng Việt](README.vi.md) | [简体中文](README.zh_hans.md)

# uchardetz

Un fork di [uchardet](https://github.com/BYVoid/uchardet) (una libreria di rilevamento della codifica derivata da Mozilla) con supporto al **sistema di build Zig**.

## Zig Build

Questo fork aggiunge `build.zig` e `build.zig.zon` in modo che la libreria, la libreria condivisa, lo strumento CLI e i test possano essere tutti compilati con il sistema di build Zig (0.15+), rendendo semplice l'utilizzo come dipendenza in progetti Zig o la cross-compilazione per qualsiasi target supportato da Zig.

### Compilazione con Zig (tramite Nix)

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

### Compilazione diretta con Nix

```bash
nix build
# Outputs: result/bin/uchardet, result/lib/libuchardet.{a,dylib}, result/include/uchardet/uchardet.h
```

### Utilizzo come dipendenza Zig

Aggiungi al tuo `build.zig.zon`:

```zig
.dependencies = .{
    .uchardetz = .{
        .url = "https://github.com/pmarreck/uchardetz/archive/<commit>.tar.gz",
        .hash = "...",
    },
},
```

Poi nel tuo `build.zig`:

```zig
const uchardetz_dep = b.dependency("uchardetz", .{
    .target = target,
    .optimize = optimize,
});
your_module.linkLibrary(uchardetz_dep.artifact("uchardet"));
```

## Output

- **`libuchardet.a`** – libreria statica
- **`libuchardet.dylib`** (o `.so`) – libreria condivisa
- **`uchardet`** – strumento CLI
- **`include/uchardet/uchardet.h`** – header C pubblico

## Correzioni rispetto all'upstream

L'upstream saltava 5 test come "notoriamente non funzionanti". Questo fork li corregge tutti (60/60 superati, 0 regressioni). Nessuno era una limitazione fondamentale dell'algoritmo di rilevamento: ognuno aveva una correzione mirata.

| Test | Risultato precedente | Causa principale | Correzione |
|------|---------------------|------------------|------------|
| `da/iso-8859-1` | `ISO-8859-15` | Punteggi di confidenza identici; il prober ISO-8859-15 veniva eseguito per primo, vincendo in caso di parità | Invertito l'ordine dei prober danesi in modo che ISO-8859-1 (più vecchio, più comune) vinca in caso di parità |
| `da/iso-8859-15` | *(saltato)* | Il byte `0xA4` (segno dell'Euro in 8859-15, segno di valuta in 8859-1) era mappato come `SYM` in entrambi i modelli, producendo punteggi identici | Marcato `0xA4` come `ILL` nella mappa ISO-8859-1 — il segno di valuta (¤) non viene praticamente mai usato nel testo reale. Quando appare `0xA4`, il prober 8859-1 ora si ritira, lasciando vincere 8859-15 |
| `es/iso-8859-15` | `ISO-8859-1` | Stessa ambiguità del `0xA4` del danese | Stessa correzione `ILL` applicata alla mappa spagnola ISO-8859-1 (applicata proattivamente anche alle mappe francese e tedesca) |
| `he/iso-8859-8` | `WINDOWS-1255` | **I dati di test stessi erano errati.** Contenevano ebraico in ordine logico (lettere finali alla fine delle parole), che il prober identifica correttamente come Windows-1255. ISO-8859-8 è una codifica *visuale* | Sostituito il file di test con ebraico in ordine visuale (sequenze di byte invertite all'interno delle righe), fornendo all'analizzatore di lettere finali un segnale chiaro di ordine visuale |
| `ja/utf-16le` | `unknown` | Nessun BOM presente. Il rilevatore riconosceva UTF-16 solo tramite BOM (`FE FF` / `FF FE`). Senza di esso, i dati passavano ai prober di charset che ottenevano tutti punteggi sotto la soglia | Aggiunto un rilevatore euristico di pattern di byte nulli: analizza i primi 256 byte cercando null alternati. >20% di null in posizioni pari con <5% in posizioni dispari = UTF-16BE; l'inverso = UTF-16LE |
| `ja/utf-16be` | `unknown` | Come sopra | Stessa euristica, pattern di byte nulli opposto |

## Upstream

Questo è un fork di [BYVoid/uchardet](https://github.com/BYVoid/uchardet). Il progetto upstream canonico si è spostato su [freedesktop.org](https://www.freedesktop.org/wiki/Software/uchardet/).

---
