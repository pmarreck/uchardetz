[English](../README.md) | [العربية](README.ar.md) | [Azərbaycan](README.az.md) | [বাংলা](README.bn.md) | [Deutsch](README.de.md) | [Ελληνικά](README.el.md) | [Español](README.es.md) | [فارسی](README.fa.md) | [Français](README.fr.md) | [עברית](README.he.md) | [हिन्दी](README.hi.md) | [Magyar](README.hu.md) | [Italiano](README.it.md) | [日本語](README.ja.md) | [ភាសាខ្មែរ](README.km.md) | [한국어](README.ko.md) | [ਪੰਜਾਬੀ](README.pa.md) | [Polski](README.pl.md) | [پښتو](README.ps.md) | [Português (BR)](README.pt_br.md) | [Română](README.ro.md) | [Русский](README.ru.md) | [Kiswahili](README.sw.md) | [தமிழ்](README.ta.md) | [ไทย](README.th.md) | [Türkçe](README.tr.md) | [Українська](README.uk.md) | [اردو](README.ur.md) | [Tiếng Việt](README.vi.md) | [简体中文](README.zh_hans.md)

# uchardetz

د [uchardet](https://github.com/BYVoid/uchardet) یو فورک (د انکوډینګ کشف کولو کتابتون چې له Mozilla څخه پورټ شوی) چې د **Zig بیلډ سیسټم** ملاتړ لري.

## Zig بیلډ

دا فورک `build.zig` او `build.zig.zon` ورزیاتوي ترڅو کتابتون، شریک شوی کتابتون، CLI وسیله، او ټیسټونه ټول د Zig بیلډ سیسټم (0.15+) سره جوړ شي. دا د Zig پروژو کې د انحصار په توګه کارول یا د هر هدف لپاره کراس-کمپایل کول اسانه کوي چې Zig یې ملاتړ کوي.

### د Zig سره جوړول (د Nix له لارې)

```bash
# د پراختیا شیل ته ننوځئ (Zig 0.15 او macOS SDK برابروي)
nix develop

# ټول شیان جوړ کړئ (static lib + shared lib + CLI)
zig build

# CLI چل کړئ
zig build run -- somefile.txt
echo "some text" | ./zig-out/bin/uchardet

# د ټیسټ سویټ چل کړئ
zig build test
```

### مستقیم د Nix سره جوړول

```bash
nix build
# پایلې: result/bin/uchardet, result/lib/libuchardet.{a,dylib}, result/include/uchardet/uchardet.h
```

### د Zig انحصار په توګه کارول

خپل `build.zig.zon` ته ورزیات کړئ:

```zig
.dependencies = .{
    .uchardetz = .{
        .url = "https://github.com/pmarreck/uchardetz/archive/<commit>.tar.gz",
        .hash = "...",
    },
},
```

بیا خپل `build.zig` کې:

```zig
const uchardetz_dep = b.dependency("uchardetz", .{
    .target = target,
    .optimize = optimize,
});
your_module.linkLibrary(uchardetz_dep.artifact("uchardet"));
```

## پایلې

- **`libuchardet.a`** - ثابت کتابتون
- **`libuchardet.dylib`** (یا `.so`) - شریک شوی کتابتون
- **`uchardet`** - CLI وسیله
- **`include/uchardet/uchardet.h`** - عامه C هېډر

## د اپسټریم په پرتله سمونونه

اپسټریم ۵ ټیسټونه د "پېژندل شویو تاوتریخوالو" په توګه پرېښودل. دا فورک دا ټول سموي (60/60 تېر شوي، 0 بیرته تګ). هیڅ یو د کشف الګوریتم بنسټیز محدودیت نه و -- هر یو ته ټاکلی سمون ورکړل شو.

| ټیسټ | بیرته ورکوله | اصلي لامل | سمون |
|------|--------------|------------|-----|
| `da/iso-8859-1` | `ISO-8859-15` | یو شان د باور نمرې؛ ISO-8859-15 prober لومړی چلېده، مساوات یې ګټله | د ډنمارکي prober ترتیب بدل شو ترڅو ISO-8859-1 (زوړ، ډېر عام) مساوات وګټي |
| `da/iso-8859-15` | *(پرېښودل شوی)* | بایټ `0xA4` (د 8859-15 کې د یورو نښه، د 8859-1 کې د اسعارو نښه) په دواړو ماډلونو کې `SYM` ته نقشه شوی و، نو نمرې یو شان وې | په ISO-8859-1 نقشه کې `0xA4` د `ILL` په توګه نښه شو -- د اسعارو نښه (¤) په اصلي متن کې تقریباً هیڅکله نه کارول کېږي. کله چې `0xA4` راشي، د 8859-1 prober اوس پرېږدي، چې 8859-15 ته د ګټلو اجازه ورکوي |
| `es/iso-8859-15` | `ISO-8859-1` | د ډنمارکي په شان ورته `0xA4` ابهام | ورته `ILL` سمون د هسپانوي ISO-8859-1 نقشې ته پلي شو (همدارنګه مخکې له مخکې د فرانسوي او آلماني نقشو ته هم پلي شو) |
| `he/iso-8859-8` | `WINDOWS-1255` | **پخپله د ټیسټ ډاټا غلط وه.** دا منطقي ترتیب شوې عبراني درلودله (د کلمو په پایونو کې وروستي بڼې توري)، چې prober یې په سمه توګه د Windows-1255 په توګه پیژني. ISO-8859-8 یو *لیدنیز* انکوډینګ دی | د ټیسټ فایل د لیدنیز ترتیب شوې عبراني (د کرښو دننه بایټ-معکوس شوي برخې) سره بدل شو، چې د وروستي توري تحلیلګر ته روښانه لیدنیز ترتیب سیګنال ورکوي |
| `ja/utf-16le` | `unknown` | هیڅ BOM شتون نه درلود. ډیټیکټر یوازې د BOM (`FE FF` / `FF FE`) له لارې UTF-16 پېژنده. پرته له دې، ډاټا charset probers ته رسېده چې ټول یې له حد لاندې نمره ورکوله | یو هیوریستیک null-byte پیټرن ډیټیکټر ورزیات شو: لومړي 256 بایټونه د متناوب nulls لپاره سکین کوي. په جوړو بایټ ځایونو کې >20% nulls او په طاقو کې <5% = UTF-16BE؛ معکوس = UTF-16LE |
| `ja/utf-16be` | `unknown` | ورته پورتني | ورته هیوریستیک، معکوس null-byte پیټرن |

## اپسټریم

دا د [BYVoid/uchardet](https://github.com/BYVoid/uchardet) یو فورک دی. اصلي اپسټریم پروژه [freedesktop.org](https://www.freedesktop.org/wiki/Software/uchardet/) ته لېږدول شوې ده.

---
