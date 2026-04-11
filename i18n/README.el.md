[English](../README.md) | [العربية](README.ar.md) | [Azərbaycan](README.az.md) | [বাংলা](README.bn.md) | [Deutsch](README.de.md) | [Ελληνικά](README.el.md) | [Español](README.es.md) | [فارسی](README.fa.md) | [Français](README.fr.md) | [עברית](README.he.md) | [हिन्दी](README.hi.md) | [Magyar](README.hu.md) | [Italiano](README.it.md) | [日本語](README.ja.md) | [ភាសាខ្មែរ](README.km.md) | [한국어](README.ko.md) | [ਪੰਜਾਬੀ](README.pa.md) | [Polski](README.pl.md) | [پښتو](README.ps.md) | [Português (BR)](README.pt_br.md) | [Română](README.ro.md) | [Русский](README.ru.md) | [Kiswahili](README.sw.md) | [தமிழ்](README.ta.md) | [ไทย](README.th.md) | [Türkçe](README.tr.md) | [Українська](README.uk.md) | [اردو](README.ur.md) | [Tiếng Việt](README.vi.md) | [简体中文](README.zh_hans.md)

# uchardetz

Ένα fork του [uchardet](https://github.com/BYVoid/uchardet) (βιβλιοθήκη ανίχνευσης κωδικοποίησης χαρακτήρων, μεταφερμένη από τον Mozilla) με υποστήριξη για το **σύστημα build του Zig**.

## Zig Build

Αυτό το fork προσθέτει τα αρχεία `build.zig` και `build.zig.zon`, ώστε η βιβλιοθήκη, η shared library, το εργαλείο CLI και τα tests να μπορούν να χτιστούν με το σύστημα build του Zig (0.15+). Αυτό διευκολύνει τη χρήση ως εξάρτηση σε projects Zig ή τη cross-compilation για οποιονδήποτε στόχο υποστηρίζει το Zig.

### Build με Zig (μέσω Nix)

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

### Build απευθείας με Nix

```bash
nix build
# Outputs: result/bin/uchardet, result/lib/libuchardet.{a,dylib}, result/include/uchardet/uchardet.h
```

### Χρήση ως εξάρτηση Zig

Προσθέστε στο `build.zig.zon` σας:

```zig
.dependencies = .{
    .uchardetz = .{
        .url = "https://github.com/pmarreck/uchardetz/archive/<commit>.tar.gz",
        .hash = "...",
    },
},
```

Στη συνέχεια, στο `build.zig` σας:

```zig
const uchardetz_dep = b.dependency("uchardetz", .{
    .target = target,
    .optimize = optimize,
});
your_module.linkLibrary(uchardetz_dep.artifact("uchardet"));
```

## Αποτελέσματα

- **`libuchardet.a`** - στατική βιβλιοθήκη
- **`libuchardet.dylib`** (ή `.so`) - shared library
- **`uchardet`** - εργαλείο CLI
- **`include/uchardet/uchardet.h`** - δημόσιο C header

## Διορθώσεις σε σχέση με το upstream

Το upstream παρέλειπε 5 tests ως "γνωστά προβληματικά". Αυτό το fork τα διορθώνει όλα (60/60 επιτυχημένα, 0 regressions). Κανένα δεν ήταν θεμελιώδης περιορισμός του αλγορίθμου ανίχνευσης -- καθένα είχε μια στοχευμένη λύση.

| Test | Επέστρεφε | Βασική αιτία | Διόρθωση |
|------|--------------|------------|-----|
| `da/iso-8859-1` | `ISO-8859-15` | Πανομοιότυπα σκορ εμπιστοσύνης· ο prober του ISO-8859-15 εκτελούνταν πρώτος και κέρδιζε στην ισοπαλία | Αντιστράφηκε η σειρά του δανέζικου prober ώστε το ISO-8859-1 (παλαιότερο, πιο διαδεδομένο) να κερδίζει στην ισοπαλία |
| `da/iso-8859-15` | *(παραλείφθηκε)* | Το byte `0xA4` (σύμβολο Euro στο 8859-15, σύμβολο νομίσματος στο 8859-1) ήταν αντιστοιχισμένο ως `SYM` και στα δύο μοντέλα, οπότε έπαιρναν ίδιο σκορ | Το `0xA4` σημειώθηκε ως `ILL` στον χάρτη ISO-8859-1 -- το σύμβολο νομίσματος (¤) ουσιαστικά δεν χρησιμοποιείται ποτέ σε πραγματικό κείμενο. Όταν εμφανίζεται το `0xA4`, ο prober του 8859-1 πλέον εγκαταλείπει, αφήνοντας το 8859-15 να κερδίσει |
| `es/iso-8859-15` | `ISO-8859-1` | Ίδια ασάφεια `0xA4` με τα δανέζικα | Ίδια διόρθωση `ILL` εφαρμόστηκε στον ισπανικό χάρτη ISO-8859-1 (εφαρμόστηκε επίσης προληπτικά στους γαλλικό και γερμανικό χάρτες) |
| `he/iso-8859-8` | `WINDOWS-1255` | **Τα ίδια τα δεδομένα του test ήταν λάθος.** Περιείχαν εβραϊκά σε λογική σειρά (γράμματα τελικής μορφής στο τέλος λέξεων), τα οποία ο prober αναγνωρίζει σωστά ως Windows-1255. Το ISO-8859-8 είναι *οπτική* κωδικοποίηση | Αντικαταστάθηκε το αρχείο test με εβραϊκά σε οπτική σειρά (αντεστραμμένες ακολουθίες bytes μέσα στις γραμμές), δίνοντας στον αναλυτή τελικών γραμμάτων ξεκάθαρο σήμα οπτικής σειράς |
| `ja/utf-16le` | `unknown` | Δεν υπήρχε BOM. Ο ανιχνευτής αναγνώριζε UTF-16 μόνο μέσω BOM (`FE FF` / `FF FE`). Χωρίς αυτό, τα δεδομένα περνούσαν στους charset probers που όλοι βαθμολογούσαν κάτω από το όριο | Προστέθηκε ευρετικός ανιχνευτής μοτίβων null-byte: σαρώνει τα πρώτα 256 bytes για εναλλασσόμενα nulls. >20% nulls σε ζυγές θέσεις byte με <5% σε μονές = UTF-16BE· το αντίστροφο = UTF-16LE |
| `ja/utf-16be` | `unknown` | Όπως παραπάνω | Ίδια ευρετική μέθοδος, αντίθετο μοτίβο null-byte |

## Upstream

Αυτό είναι fork του [BYVoid/uchardet](https://github.com/BYVoid/uchardet). Το κανονικό upstream project έχει μεταφερθεί στο [freedesktop.org](https://www.freedesktop.org/wiki/Software/uchardet/).

---
