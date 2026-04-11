[English](../README.md) | [العربية](README.ar.md) | [Azərbaycan](README.az.md) | [বাংলা](README.bn.md) | [Deutsch](README.de.md) | [Ελληνικά](README.el.md) | [Español](README.es.md) | [فارسی](README.fa.md) | [Français](README.fr.md) | [עברית](README.he.md) | [हिन्दी](README.hi.md) | [Magyar](README.hu.md) | [Italiano](README.it.md) | [日本語](README.ja.md) | [ភាសាខ្មែរ](README.km.md) | [한국어](README.ko.md) | [ਪੰਜਾਬੀ](README.pa.md) | [Polski](README.pl.md) | [پښتو](README.ps.md) | [Português (BR)](README.pt_br.md) | [Română](README.ro.md) | [Русский](README.ru.md) | [Kiswahili](README.sw.md) | [தமிழ்](README.ta.md) | [ไทย](README.th.md) | [Türkçe](README.tr.md) | [Українська](README.uk.md) | [اردو](README.ur.md) | [Tiếng Việt](README.vi.md) | [简体中文](README.zh_hans.md)

# uchardetz

یہ [uchardet](https://github.com/BYVoid/uchardet) کا فورک ہے (Mozilla سے پورٹ کی گئی انکوڈنگ کا پتہ لگانے والی لائبریری) جس میں **Zig بلڈ سسٹم** کی سپورٹ شامل ہے۔

## Zig بلڈ

یہ فورک `build.zig` اور `build.zig.zon` شامل کرتا ہے تاکہ لائبریری، شیئرڈ لائبریری، CLI ٹول، اور ٹیسٹس سب Zig بلڈ سسٹم (0.15+) سے بنائے جا سکیں۔ اس سے Zig پروجیکٹس میں بطور ڈیپنڈنسی استعمال کرنا یا Zig کی سپورٹ کردہ کسی بھی ٹارگٹ کے لیے کراس کمپائل کرنا آسان ہو جاتا ہے۔

### Zig سے بلڈ (Nix کے ذریعے)

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

### براہ راست Nix سے بلڈ

```bash
nix build
# Outputs: result/bin/uchardet, result/lib/libuchardet.{a,dylib}, result/include/uchardet/uchardet.h
```

### Zig ڈیپنڈنسی کے طور پر استعمال

اپنے `build.zig.zon` میں شامل کریں:

```zig
.dependencies = .{
    .uchardetz = .{
        .url = "https://github.com/pmarreck/uchardetz/archive/<commit>.tar.gz",
        .hash = "...",
    },
},
```

پھر اپنے `build.zig` میں:

```zig
const uchardetz_dep = b.dependency("uchardetz", .{
    .target = target,
    .optimize = optimize,
});
your_module.linkLibrary(uchardetz_dep.artifact("uchardet"));
```

## آؤٹ پٹس

- **`libuchardet.a`** -- سٹیٹک لائبریری
- **`libuchardet.dylib`** (یا `.so`) -- شیئرڈ لائبریری
- **`uchardet`** -- CLI ٹول
- **`include/uchardet/uchardet.h`** -- پبلک C ہیڈر

## upstream کے مقابلے میں اصلاحات

upstream نے 5 ٹیسٹس کو "معلوم خرابی" کے طور پر چھوڑ دیا تھا۔ یہ فورک ان سب کو ٹھیک کرتا ہے (60/60 پاس، 0 ریگریشنز)۔ ان میں سے کوئی بھی ڈیٹیکشن الگورتھم کی بنیادی حد نہیں تھی -- ہر ایک کی ایک مخصوص اصلاح تھی۔

| ٹیسٹ | واپس کر رہا تھا | اصل وجہ | اصلاح |
|------|-----------------|---------|-------|
| `da/iso-8859-1` | `ISO-8859-15` | یکساں اعتماد اسکورز؛ ISO-8859-15 prober پہلے چلتا تھا اور برابری میں جیت جاتا تھا | ڈینش prober کی ترتیب بدلی تاکہ ISO-8859-1 (پرانا، زیادہ عام) برابری میں جیتے |
| `da/iso-8859-15` | *(چھوڑ دیا گیا)* | بائٹ `0xA4` (8859-15 میں یورو کا نشان، 8859-1 میں کرنسی کا نشان) دونوں ماڈلز میں `SYM` کے طور پر میپ تھا، اس لیے اسکور برابر آتے تھے | ISO-8859-1 میپ میں `0xA4` کو `ILL` کے طور پر نشان زد کیا -- کرنسی کا نشان (¤) اصل متن میں عملاً کبھی استعمال نہیں ہوتا۔ جب `0xA4` آتا ہے تو اب 8859-1 prober دستبردار ہو جاتا ہے، 8859-15 کو جیتنے دیتا ہے |
| `es/iso-8859-15` | `ISO-8859-1` | ڈینش والی ہی `0xA4` ابہام | وہی `ILL` اصلاح ہسپانوی ISO-8859-1 میپ پر لاگو کی گئی (فرانسیسی اور جرمن میپس پر بھی احتیاطاً لاگو کیا گیا) |
| `he/iso-8859-8` | `WINDOWS-1255` | **ٹیسٹ ڈیٹا خود غلط تھا۔** اس میں منطقی ترتیب والی عبرانی تھی (لفظ کے آخر میں آخری شکل کے حروف)، جسے prober درست طور پر Windows-1255 شناخت کرتا ہے۔ ISO-8859-8 ایک *بصری* انکوڈنگ ہے | ٹیسٹ فائل کو بصری ترتیب والی عبرانی سے بدلا (لائنوں کے اندر بائٹ ریورسڈ رنز)، جس سے فائنل لیٹر اینالائزر کو بصری ترتیب کا واضح سگنل ملتا ہے |
| `ja/utf-16le` | `unknown` | BOM موجود نہیں تھا۔ ڈیٹیکٹر صرف BOM (`FE FF` / `FF FE`) سے UTF-16 پہچانتا تھا۔ اس کے بغیر ڈیٹا charset probers تک پہنچتا تھا جن سب کا اسکور حد سے نیچے تھا | ایک ہیورسٹک نل بائٹ پیٹرن ڈیٹیکٹر شامل کیا: پہلے 256 بائٹس میں باری باری نلز اسکین کرتا ہے۔ جفت بائٹ پوزیشنز پر >20% نلز اور طاق پر <5% = UTF-16BE؛ الٹ = UTF-16LE |
| `ja/utf-16be` | `unknown` | اوپر والی ہی وجہ | وہی ہیورسٹک، الٹ نل بائٹ پیٹرن |

## Upstream

یہ [BYVoid/uchardet](https://github.com/BYVoid/uchardet) کا فورک ہے۔ کینونیکل upstream پروجیکٹ [freedesktop.org](https://www.freedesktop.org/wiki/Software/uchardet/) پر منتقل ہو چکا ہے۔

---
