[English](README.md) | [العربية](README.ar.md) | [Azərbaycan](README.az.md) | [বাংলা](README.bn.md) | [Deutsch](README.de.md) | [Ελληνικά](README.el.md) | [Español](README.es.md) | [فارسی](README.fa.md) | [Français](README.fr.md) | [עברית](README.he.md) | [हिन्दी](README.hi.md) | [Magyar](README.hu.md) | [Italiano](README.it.md) | [日本語](README.ja.md) | [ភាសាខ្មែរ](README.km.md) | [한국어](README.ko.md) | [ਪੰਜਾਬੀ](README.pa.md) | [Polski](README.pl.md) | [پښتو](README.ps.md) | [Português (BR)](README.pt_br.md) | [Română](README.ro.md) | [Русский](README.ru.md) | [Kiswahili](README.sw.md) | [தமிழ்](README.ta.md) | [ไทย](README.th.md) | [Türkçe](README.tr.md) | [Українська](README.uk.md) | [اردو](README.ur.md) | [Tiếng Việt](README.vi.md) | [简体中文](README.zh_hans.md)

# uchardetz

ជា fork មួយរបស់ [uchardet](https://github.com/BYVoid/uchardet) (បណ្ណាល័យសម្រាប់រកឃើញការអ៊ិនកូដអក្សរ ដែលបានបម្លែងមកពី Mozilla) ដែលមានការគាំទ្រ**ប្រព័ន្ធ build Zig**។

## Zig Build

Fork នេះបន្ថែម `build.zig` និង `build.zig.zon` ដើម្បីឱ្យបណ្ណាល័យ បណ្ណាល័យរួមគ្នា ឧបករណ៍ CLI និងតេស្ត ទាំងអស់អាចត្រូវបាន build ដោយប្រព័ន្ធ build Zig (0.15+) ដែលធ្វើឱ្យវាងាយស្រួលក្នុងការប្រើជា dependency នៅក្នុងគម្រោង Zig ឬ cross-compile សម្រាប់ target ណាមួយដែល Zig គាំទ្រ។

### ការ build ជាមួយ Zig (តាមរយៈ Nix)

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

### ការ build ដោយផ្ទាល់ជាមួយ Nix

```bash
nix build
# Outputs: result/bin/uchardet, result/lib/libuchardet.{a,dylib}, result/include/uchardet/uchardet.h
```

### ការប្រើជា dependency របស់ Zig

បន្ថែមទៅក្នុង `build.zig.zon` របស់អ្នក៖

```zig
.dependencies = .{
    .uchardetz = .{
        .url = "https://github.com/pmarreck/uchardetz/archive/<commit>.tar.gz",
        .hash = "...",
    },
},
```

បន្ទាប់មកនៅក្នុង `build.zig` របស់អ្នក៖

```zig
const uchardetz_dep = b.dependency("uchardetz", .{
    .target = target,
    .optimize = optimize,
});
your_module.linkLibrary(uchardetz_dep.artifact("uchardet"));
```

## លទ្ធផល

- **`libuchardet.a`** – បណ្ណាល័យឋិតិវន្ត
- **`libuchardet.dylib`** (ឬ `.so`) – បណ្ណាល័យរួមគ្នា
- **`uchardet`** – ឧបករណ៍ CLI
- **`include/uchardet/uchardet.h`** – header C សាធារណៈ

## ការកែតម្រូវពី upstream

Upstream បានរំលងតេស្ត 5 ជា "ដឹងថាខូច"។ Fork នេះកែតម្រូវទាំងអស់ (60/60 ជោគជ័យ, 0 regression)។ គ្មានមួយណាជាដែនកំណត់មូលដ្ឋានរបស់ algorithm រកឃើញឡើយ — គ្រប់មួយមានការកែតម្រូវគោលដៅ។

| តេស្ត | តម្លៃដែលបានត្រឡប់ | មូលហេតុឫសគល់ | ការកែតម្រូវ |
|--------|-------------------|---------------|------------|
| `da/iso-8859-1` | `ISO-8859-15` | ពិន្ទុភាពជឿជាក់ដូចគ្នា; prober ISO-8859-15 ដំណើរការមុន ដូច្នេះឈ្នះក្នុងករណីស្មើ | ប្ដូរលំដាប់ prober ភាសាដាណឺម៉ាក ដើម្បីឱ្យ ISO-8859-1 (ចាស់ជាង ប្រើទូលំទូលាយជាង) ឈ្នះក្នុងករណីស្មើ |
| `da/iso-8859-15` | *(រំលង)* | Byte `0xA4` (សញ្ញាអឺរ៉ូក្នុង 8859-15, សញ្ញារូបិយប័ណ្ណក្នុង 8859-1) ត្រូវបានផ្គូផ្គងជា `SYM` នៅក្នុង model ទាំងពីរ ដូច្នេះពិន្ទុដូចគ្នា | សម្គាល់ `0xA4` ជា `ILL` នៅក្នុងផែនទី ISO-8859-1 — សញ្ញារូបិយប័ណ្ណ (¤) ស្ទើរតែមិនដែលត្រូវបានប្រើក្នុងអត្ថបទជាក់ស្ដែង។ នៅពេល `0xA4` លេចឡើង prober 8859-1 ឥឡូវដកខ្លួន ដើម្បីឱ្យ 8859-15 ឈ្នះ |
| `es/iso-8859-15` | `ISO-8859-1` | ភាពមិនច្បាស់ `0xA4` ដូចគ្នានឹងភាសាដាណឺម៉ាក | ការកែតម្រូវ `ILL` ដូចគ្នា បានអនុវត្តលើផែនទី ISO-8859-1 ភាសាអេស្ប៉ាញ (ក៏បានអនុវត្តជាមុនលើផែនទីភាសាបារាំង និងអាល្លឺម៉ង់ផងដែរ) |
| `he/iso-8859-8` | `WINDOWS-1255` | **ទិន្នន័យតេស្តខ្លួនឯងមានកំហុស។** វាមានអក្សរហេប្រូដែលរៀបតាមលំដាប់ឡូជិក (អក្សរទម្រង់ចុងនៅចុងពាក្យ) ដែល prober កំណត់ត្រឹមត្រូវថាជា Windows-1255។ ISO-8859-8 គឺជាការអ៊ិនកូដ*បែបមើលឃើញ* | ជំនួសឯកសារតេស្តដោយអក្សរហេប្រូដែលរៀបតាមលំដាប់មើលឃើញ (byte-reversed runs ក្នុងបន្ទាត់) ដើម្បីផ្ដល់ឱ្យឧបករណ៍វិភាគអក្សរចុងនូវសញ្ញាលំដាប់មើលឃើញច្បាស់លាស់ |
| `ja/utf-16le` | `unknown` | គ្មាន BOM។ ឧបករណ៍រកឃើញស្គាល់ UTF-16 តែតាមរយៈ BOM (`FE FF` / `FF FE`) ប៉ុណ្ណោះ។ បើគ្មាន ទិន្នន័យធ្លាក់ទៅ charset prober ទាំងអស់ដែលពិន្ទុទាំងអស់នៅក្រោមកម្រិតអប្បបរមា | បន្ថែមឧបករណ៍រកឃើញគំរូ null-byte បែប heuristic៖ ស្កេន 256 byte ដំបូងដើម្បីរកមើល null ប្រែប្រួល។ >20% null នៅទីតាំង byte គូ ជាមួយ <5% នៅសេស = UTF-16BE; ផ្ទុយមកវិញ = UTF-16LE |
| `ja/utf-16be` | `unknown` | ដូចខាងលើ | heuristic ដូចគ្នា គំរូ null-byte ផ្ទុយ |

## Upstream

នេះជា fork មួយរបស់ [BYVoid/uchardet](https://github.com/BYVoid/uchardet)។ គម្រោង upstream ផ្លូវការបានផ្លាស់ទៅ [freedesktop.org](https://www.freedesktop.org/wiki/Software/uchardet/)។

---
