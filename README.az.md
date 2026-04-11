[English](README.md) | [العربية](README.ar.md) | [Azərbaycan](README.az.md) | [বাংলা](README.bn.md) | [Deutsch](README.de.md) | [Ελληνικά](README.el.md) | [Español](README.es.md) | [فارسی](README.fa.md) | [Français](README.fr.md) | [עברית](README.he.md) | [हिन्दी](README.hi.md) | [Magyar](README.hu.md) | [Italiano](README.it.md) | [日本語](README.ja.md) | [ភាសាខ្មែរ](README.km.md) | [한국어](README.ko.md) | [ਪੰਜਾਬੀ](README.pa.md) | [Polski](README.pl.md) | [پښتو](README.ps.md) | [Português (BR)](README.pt_br.md) | [Română](README.ro.md) | [Русский](README.ru.md) | [Kiswahili](README.sw.md) | [தமிழ்](README.ta.md) | [ไทย](README.th.md) | [Türkçe](README.tr.md) | [Українська](README.uk.md) | [اردو](README.ur.md) | [Tiếng Việt](README.vi.md) | [简体中文](README.zh_hans.md)

# uchardetz

[uchardet](https://github.com/BYVoid/uchardet) layihəsinin fork-u (Mozilla-dan portlanmış kodlaşma aşkarlama kitabxanası) — **Zig qurma sistemi** dəstəyi ilə.

## Zig Qurması

Bu fork `build.zig` və `build.zig.zon` fayllarını əlavə edir ki, kitabxana, paylaşılan kitabxana, CLI aləti və testlərin hamısı Zig qurma sistemi (0.15+) ilə qurula bilsin. Bu, onu Zig layihələrində asılılıq kimi istifadə etməyi və ya Zig-in dəstəklədiyi istənilən hədəf üçün çarpaz kompilyasiya etməyi asanlaşdırır.

### Zig ilə qurmaq (Nix vasitəsilə)

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

### Birbaşa Nix ilə qurmaq

```bash
nix build
# Outputs: result/bin/uchardet, result/lib/libuchardet.{a,dylib}, result/include/uchardet/uchardet.h
```

### Zig asılılığı kimi istifadə

`build.zig.zon` faylınıza əlavə edin:

```zig
.dependencies = .{
    .uchardetz = .{
        .url = "https://github.com/pmarreck/uchardetz/archive/<commit>.tar.gz",
        .hash = "...",
    },
},
```

Sonra `build.zig` faylınızda:

```zig
const uchardetz_dep = b.dependency("uchardetz", .{
    .target = target,
    .optimize = optimize,
});
your_module.linkLibrary(uchardetz_dep.artifact("uchardet"));
```

## Çıxışlar

- **`libuchardet.a`** - statik kitabxana
- **`libuchardet.dylib`** (və ya `.so`) - paylaşılan kitabxana
- **`uchardet`** - CLI aləti
- **`include/uchardet/uchardet.h`** - ictimai C başlıq faylı

## Upstream-a nəzərən düzəlişlər

Upstream 5 testi "bilinən nasazlıq" kimi keçib. Bu fork onların hamısını düzəldir (60/60 uğurlu, 0 reqressiya). Heç biri aşkarlama alqoritminin əsas məhdudiyyəti deyildi — hər birinin hədəflənmiş həlli var idi.

| Test | Nə qaytarırdı | Kök səbəb | Düzəliş |
|------|--------------|------------|-----|
| `da/iso-8859-1` | `ISO-8859-15` | Eyni etibar balları; ISO-8859-15 prober birinci işləyib bərabərlikdə qalib gəlirdi | Danimarka prober sırası dəyişdirildi ki, ISO-8859-1 (daha köhnə, daha geniş yayılmış) bərabərlikdə qalib gəlsin |
| `da/iso-8859-15` | *(keçildi)* | `0xA4` baytı (8859-15-də Avro işarəsi, 8859-1-də valyuta işarəsi) hər iki modeldə `SYM` olaraq təyin olunmuşdu, buna görə eyni bal alırdılar | `0xA4` ISO-8859-1 xəritəsində `ILL` olaraq işarələndi — valyuta işarəsi (¤) real mətnlərdə demək olar ki, heç istifadə olunmur. `0xA4` rast gəlindikdə, 8859-1 prober indi dayanır və 8859-15-in qalib gəlməsinə imkan verir |
| `es/iso-8859-15` | `ISO-8859-1` | Danimarka ilə eyni `0xA4` qeyri-müəyyənliyi | Eyni `ILL` düzəlişi İspan ISO-8859-1 xəritəsinə tətbiq olundu (həmçinin Fransız və Alman xəritələrinə proaktiv olaraq tətbiq edildi) |
| `he/iso-8859-8` | `WINDOWS-1255` | **Test datası özü səhv idi.** Məntiqi sıralı İbranicə (söz sonlarında son formadakı hərflər) ehtiva edirdi ki, prober bunu düzgün olaraq Windows-1255 kimi müəyyən edir. ISO-8859-8 *vizual* kodlaşdırmadır | Test faylı vizual sıralı İbranicə ilə əvəz olundu (sətirlərdə bayt ardıcıllığı tərsinə çevrildi) ki, son hərf analizatoru vizual sıra siqnalını aydın alsın |
| `ja/utf-16le` | `unknown` | BOM yoxdur. Detektor UTF-16-nı yalnız BOM (`FE FF` / `FF FE`) vasitəsilə tanıyırdı. Onsuz, data həddi-aşağı bal alan charset prober-lərə düşürdü | Evristik null-bayt naxış detektoru əlavə olundu: ilk 256 baytda növbələnən null-lar axtarır. Cüt mövqelərdə >20% null, tək mövqelərdə <5% = UTF-16BE; əksi = UTF-16LE |
| `ja/utf-16be` | `unknown` | Yuxarıdakı ilə eyni | Eyni evristika, əks null-bayt naxışı |

## Upstream

Bu, [BYVoid/uchardet](https://github.com/BYVoid/uchardet) layihəsinin fork-udur. Kanonik upstream layihə [freedesktop.org](https://www.freedesktop.org/wiki/Software/uchardet/) ünvanına köçürülüb.

---
