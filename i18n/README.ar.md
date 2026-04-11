[English](../README.md) | [العربية](README.ar.md) | [Azərbaycan](README.az.md) | [বাংলা](README.bn.md) | [Deutsch](README.de.md) | [Ελληνικά](README.el.md) | [Español](README.es.md) | [فارسی](README.fa.md) | [Français](README.fr.md) | [עברית](README.he.md) | [हिन्दी](README.hi.md) | [Magyar](README.hu.md) | [Italiano](README.it.md) | [日本語](README.ja.md) | [ភាសាខ្មែរ](README.km.md) | [한국어](README.ko.md) | [ਪੰਜਾਬੀ](README.pa.md) | [Polski](README.pl.md) | [پښتو](README.ps.md) | [Português (BR)](README.pt_br.md) | [Română](README.ro.md) | [Русский](README.ru.md) | [Kiswahili](README.sw.md) | [தமிழ்](README.ta.md) | [ไทย](README.th.md) | [Türkçe](README.tr.md) | [Українська](README.uk.md) | [اردو](README.ur.md) | [Tiếng Việt](README.vi.md) | [简体中文](README.zh_hans.md)

# uchardetz

نسخة مُتفرّعة من [uchardet](https://github.com/BYVoid/uchardet) (مكتبة كشف الترميز المنقولة من Mozilla) مع دعم **نظام بناء Zig**.

## بناء Zig

تضيف هذه النسخة المتفرّعة ملفَّي `build.zig` و`build.zig.zon` بحيث يمكن بناء المكتبة الساكنة والمكتبة المشتركة وأداة سطر الأوامر والاختبارات جميعها باستخدام نظام بناء Zig (الإصدار 0.15 فأعلى)، مما يسهّل استخدامها كاعتمادية في مشاريع Zig أو التجميع المتقاطع لأي هدف يدعمه Zig.

### البناء باستخدام Zig (عبر Nix)

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

### البناء باستخدام Nix مباشرة

```bash
nix build
# Outputs: result/bin/uchardet, result/lib/libuchardet.{a,dylib}, result/include/uchardet/uchardet.h
```

### الاستخدام كاعتمادية Zig

أضف إلى ملف `build.zig.zon` الخاص بك:

```zig
.dependencies = .{
    .uchardetz = .{
        .url = "https://github.com/pmarreck/uchardetz/archive/<commit>.tar.gz",
        .hash = "...",
    },
},
```

ثم في ملف `build.zig` الخاص بك:

```zig
const uchardetz_dep = b.dependency("uchardetz", .{
    .target = target,
    .optimize = optimize,
});
your_module.linkLibrary(uchardetz_dep.artifact("uchardet"));
```

## المخرجات

- **`libuchardet.a`** - مكتبة ساكنة
- **`libuchardet.dylib`** (أو `.so`) - مكتبة مشتركة
- **`uchardet`** - أداة سطر الأوامر
- **`include/uchardet/uchardet.h`** - ترويسة C العامة

## الإصلاحات مقارنة بالمشروع الأصلي

تجاوز المشروع الأصلي 5 اختبارات باعتبارها "معطّلة معروفة". تُصلح هذه النسخة المتفرّعة جميعها (60/60 ناجحة، 0 تراجعات). لم تكن أيّ منها قصورًا جوهريًّا في خوارزمية الكشف — وإنما لكل منها إصلاح مستهدف.

| الاختبار | كان يُعيد | السبب الجذري | الإصلاح |
|------|--------------|------------|-----|
| `da/iso-8859-1` | `ISO-8859-15` | درجات ثقة متطابقة؛ prober الخاص بـ ISO-8859-15 عمل أولاً فربح عند التعادل | تم تبديل ترتيب prober الدنماركي بحيث يربح ISO-8859-1 (الأقدم والأكثر شيوعًا) عند التعادل |
| `da/iso-8859-15` | *(تم تجاوزه)* | البايت `0xA4` (رمز اليورو في 8859-15، ورمز العملة في 8859-1) مُعيَّن كـ `SYM` في كلا النموذجين، فحصلا على درجات متطابقة | تم تعليم `0xA4` كـ `ILL` في خريطة ISO-8859-1 — رمز العملة (¤) لا يُستخدم عمليًا في النصوص الحقيقية. عند ظهور `0xA4`، يتوقف prober الخاص بـ 8859-1 الآن، مما يتيح لـ 8859-15 الفوز |
| `es/iso-8859-15` | `ISO-8859-1` | نفس غموض `0xA4` كما في الدنماركية | نفس إصلاح `ILL` مطبَّق على خريطة ISO-8859-1 الإسبانية (مطبَّق أيضًا استباقيًا على الخرائط الفرنسية والألمانية) |
| `he/iso-8859-8` | `WINDOWS-1255` | **بيانات الاختبار نفسها كانت خاطئة.** احتوت على عبرية مرتّبة منطقيًا (حروف الشكل النهائي في نهايات الكلمات)، والتي يحددها prober بشكل صحيح كـ Windows-1255. ISO-8859-8 هو ترميز *بصري* | تم استبدال ملف الاختبار بعبرية مرتّبة بصريًا (سلاسل بايتات معكوسة داخل السطور)، مما يعطي محلل الحروف النهائية إشارة واضحة بالترتيب البصري |
| `ja/utf-16le` | `unknown` | لا يوجد BOM. كان الكاشف يتعرف على UTF-16 فقط عبر BOM ‏(`FE FF` / `FF FE`). بدونها، مرّت البيانات إلى probers الترميز التي سجّلت جميعها أقل من العتبة | تمت إضافة كاشف أنماط البايتات الفارغة: يفحص أول 256 بايت بحثًا عن بايتات فارغة متناوبة. أكثر من 20% بايتات فارغة في المواضع الزوجية مع أقل من 5% في الفردية = UTF-16BE؛ والعكس = UTF-16LE |
| `ja/utf-16be` | `unknown` | نفس ما سبق | نفس الأسلوب الاستدلالي، نمط بايتات فارغة معاكس |

## المشروع الأصلي

هذه نسخة متفرّعة من [BYVoid/uchardet](https://github.com/BYVoid/uchardet). انتقل المشروع الأصلي الرسمي إلى [freedesktop.org](https://www.freedesktop.org/wiki/Software/uchardet/).

---
