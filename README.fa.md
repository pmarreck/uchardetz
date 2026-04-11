[English](README.md) | [العربية](README.ar.md) | [Azərbaycan](README.az.md) | [বাংলা](README.bn.md) | [Deutsch](README.de.md) | [Ελληνικά](README.el.md) | [Español](README.es.md) | [فارسی](README.fa.md) | [Français](README.fr.md) | [עברית](README.he.md) | [हिन्दी](README.hi.md) | [Magyar](README.hu.md) | [Italiano](README.it.md) | [日本語](README.ja.md) | [ភាសាខ្មែរ](README.km.md) | [한국어](README.ko.md) | [ਪੰਜਾਬੀ](README.pa.md) | [Polski](README.pl.md) | [پښتو](README.ps.md) | [Português (BR)](README.pt_br.md) | [Română](README.ro.md) | [Русский](README.ru.md) | [Kiswahili](README.sw.md) | [தமிழ்](README.ta.md) | [ไทย](README.th.md) | [Türkçe](README.tr.md) | [Українська](README.uk.md) | [اردو](README.ur.md) | [Tiếng Việt](README.vi.md) | [简体中文](README.zh_hans.md)

# uchardetz

یک فورک از [uchardet](https://github.com/BYVoid/uchardet) (کتابخانه تشخیص رمزگذاری که از موزیلا پورت شده) با پشتیبانی از **سیستم بیلد Zig**.

## بیلد با Zig

این فورک فایل‌های `build.zig` و `build.zig.zon` را اضافه می‌کند تا کتابخانه استاتیک، کتابخانه اشتراکی، ابزار خط فرمان و تست‌ها همگی با سیستم بیلد Zig (نسخه 0.15 به بالا) قابل ساخت باشند. این کار استفاده به عنوان وابستگی در پروژه‌های Zig و کراس‌کامپایل برای هر پلتفرمی که Zig پشتیبانی می‌کند را آسان می‌سازد.

### بیلد با Zig (از طریق Nix)

```bash
# ورود به شل توسعه (Zig 0.15 و SDK مک‌اواس را فراهم می‌کند)
nix develop

# ساخت همه چیز (کتابخانه استاتیک + کتابخانه اشتراکی + CLI)
zig build

# اجرای CLI
zig build run -- somefile.txt
echo "some text" | ./zig-out/bin/uchardet

# اجرای مجموعه تست‌ها
zig build test
```

### بیلد مستقیم با Nix

```bash
nix build
# Outputs: result/bin/uchardet, result/lib/libuchardet.{a,dylib}, result/include/uchardet/uchardet.h
```

### استفاده به عنوان وابستگی Zig

به فایل `build.zig.zon` خود اضافه کنید:

```zig
.dependencies = .{
    .uchardetz = .{
        .url = "https://github.com/pmarreck/uchardetz/archive/<commit>.tar.gz",
        .hash = "...",
    },
},
```

سپس در فایل `build.zig` خود:

```zig
const uchardetz_dep = b.dependency("uchardetz", .{
    .target = target,
    .optimize = optimize,
});
your_module.linkLibrary(uchardetz_dep.artifact("uchardet"));
```

## خروجی‌ها

- **`libuchardet.a`** — کتابخانه استاتیک
- **`libuchardet.dylib`** (یا `.so`) — کتابخانه اشتراکی
- **`uchardet`** — ابزار خط فرمان
- **`include/uchardet/uchardet.h`** — هدر عمومی C

## اصلاحات نسبت به پروژه اصلی

پروژه اصلی ۵ تست را به عنوان «خرابی‌های شناخته‌شده» رد می‌کرد. این فورک همه آن‌ها را اصلاح کرده است (۶۰ از ۶۰ موفق، بدون رگرسیون). هیچ‌کدام محدودیت بنیادین الگوریتم تشخیص نبودند — هر کدام راه‌حل مشخصی داشت.

| تست | نتیجه قبلی | علت ریشه‌ای | اصلاح |
|------|------------|-------------|-------|
| `da/iso-8859-1` | `ISO-8859-15` | امتیازهای اطمینان یکسان؛ prober مربوط به ISO-8859-15 زودتر اجرا می‌شد و در تساوی برنده بود | ترتیب proberهای دانمارکی تغییر داده شد تا ISO-8859-1 (قدیمی‌تر و رایج‌تر) در تساوی برنده شود |
| `da/iso-8859-15` | *(رد شده)* | بایت `0xA4` (نشان یورو در 8859-15، نشان ارز در 8859-1) در هر دو مدل به `SYM` نگاشت شده بود و امتیازهای یکسانی می‌گرفتند | `0xA4` در نقشه ISO-8859-1 به عنوان `ILL` علامت‌گذاری شد — نشان ارز (¤) عملاً هرگز در متن واقعی استفاده نمی‌شود. وقتی `0xA4` ظاهر شود، prober مربوط به 8859-1 اکنون متوقف می‌شود و 8859-15 برنده می‌گردد |
| `es/iso-8859-15` | `ISO-8859-1` | همان ابهام `0xA4` مانند دانمارکی | همان اصلاح `ILL` روی نقشه اسپانیایی ISO-8859-1 اعمال شد (همچنین به صورت پیشگیرانه روی نقشه‌های فرانسوی و آلمانی نیز اعمال گردید) |
| `he/iso-8859-8` | `WINDOWS-1255` | **خود داده‌های تست اشتباه بودند.** شامل عبری با ترتیب منطقی بودند (حروف شکل پایانی در انتهای کلمات)، که prober به درستی آن را Windows-1255 تشخیص می‌دهد. ISO-8859-8 یک رمزگذاری *بصری* است | فایل تست با عبری به ترتیب بصری جایگزین شد (توالی بایت‌ها درون خطوط معکوس شده)، که به تحلیلگر حروف پایانی سیگنال واضحی از ترتیب بصری می‌دهد |
| `ja/utf-16le` | `unknown` | BOM موجود نبود. آشکارساز فقط UTF-16 را از طریق BOM (`FE FF` / `FF FE`) تشخیص می‌داد. بدون آن، داده‌ها به proberهای رمزگذاری منتقل می‌شدند که همگی زیر آستانه امتیاز می‌گرفتند | یک آشکارساز اکتشافی الگوی بایت‌های نول اضافه شد: ۲۵۶ بایت اول را برای نول‌های متناوب بررسی می‌کند. بیش از ۲۰٪ نول در موقعیت‌های زوج با کمتر از ۵٪ در فرد = UTF-16BE؛ الگوی معکوس = UTF-16LE |
| `ja/utf-16be` | `unknown` | مانند بالا | همان اکتشاف، با الگوی معکوس بایت‌های نول |

## پروژه اصلی

این یک فورک از [BYVoid/uchardet](https://github.com/BYVoid/uchardet) است. پروژه اصلی رسمی به [freedesktop.org](https://www.freedesktop.org/wiki/Software/uchardet/) منتقل شده است.

---
