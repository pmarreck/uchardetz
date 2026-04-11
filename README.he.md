[English](README.md) | [العربية](README.ar.md) | [Azərbaycan](README.az.md) | [বাংলা](README.bn.md) | [Deutsch](README.de.md) | [Ελληνικά](README.el.md) | [Español](README.es.md) | [فارسی](README.fa.md) | [Français](README.fr.md) | [עברית](README.he.md) | [हिन्दी](README.hi.md) | [Magyar](README.hu.md) | [Italiano](README.it.md) | [日本語](README.ja.md) | [ភាសាខ្មែរ](README.km.md) | [한국어](README.ko.md) | [ਪੰਜਾਬੀ](README.pa.md) | [Polski](README.pl.md) | [پښتو](README.ps.md) | [Português (BR)](README.pt_br.md) | [Română](README.ro.md) | [Русский](README.ru.md) | [Kiswahili](README.sw.md) | [தமிழ்](README.ta.md) | [ไทย](README.th.md) | [Türkçe](README.tr.md) | [Українська](README.uk.md) | [اردو](README.ur.md) | [Tiếng Việt](README.vi.md) | [简体中文](README.zh_hans.md)

# uchardetz

פורק של [uchardet](https://github.com/BYVoid/uchardet) (ספריית זיהוי קידודים שהועברה ממוזילה) עם תמיכה ב**מערכת הבנייה של Zig**.

## בנייה עם Zig

פורק זה מוסיף את `build.zig` ו-`build.zig.zon` כך שהספרייה הסטטית, הספרייה המשותפת, כלי שורת הפקודה והבדיקות ניתנים לבנייה באמצעות מערכת הבנייה של Zig (גרסה 0.15 ומעלה), מה שמקל על שימוש כתלות בפרויקטי Zig או קומפילציה צולבת לכל יעד ש-Zig תומך בו.

### בנייה עם Zig (דרך Nix)

```bash
# כניסה לסביבת הפיתוח (מספקת Zig 0.15 ו-SDK של macOS)
nix develop

# בניית הכול (ספרייה סטטית + ספרייה משותפת + CLI)
zig build

# הרצת ה-CLI
zig build run -- somefile.txt
echo "some text" | ./zig-out/bin/uchardet

# הרצת סט הבדיקות
zig build test
```

### בנייה ישירה עם Nix

```bash
nix build
# פלט: result/bin/uchardet, result/lib/libuchardet.{a,dylib}, result/include/uchardet/uchardet.h
```

### שימוש כתלות Zig

הוסיפו ל-`build.zig.zon` שלכם:

```zig
.dependencies = .{
    .uchardetz = .{
        .url = "https://github.com/pmarreck/uchardetz/archive/<commit>.tar.gz",
        .hash = "...",
    },
},
```

ואז ב-`build.zig` שלכם:

```zig
const uchardetz_dep = b.dependency("uchardetz", .{
    .target = target,
    .optimize = optimize,
});
your_module.linkLibrary(uchardetz_dep.artifact("uchardet"));
```

## פלטים

- **`libuchardet.a`** — ספרייה סטטית
- **`libuchardet.dylib`** (או `.so`) — ספרייה משותפת
- **`uchardet`** — כלי שורת פקודה
- **`include/uchardet/uchardet.h`** — כותרת C ציבורית

## תיקונים ביחס לפרויקט המקורי

הפרויקט המקורי דילג על 5 בדיקות וסימן אותן כ"תקלות ידועות". פורק זה מתקן את כולן (60/60 עוברות, 0 רגרסיות). אף אחת לא הייתה מגבלה מהותית של אלגוריתם הזיהוי — לכל אחת הייתה תיקון ממוקד.

| בדיקה | תוצאה קודמת | שורש הבעיה | תיקון |
|-------|-------------|------------|-------|
| `da/iso-8859-1` | `ISO-8859-15` | ציוני ביטחון זהים; ה-prober של ISO-8859-15 רץ ראשון וזכה בשוויון | שינוי סדר ה-probers הדניים כך ש-ISO-8859-1 (ישן יותר, נפוץ יותר) ינצח בשוויון |
| `da/iso-8859-15` | *(דולג)* | הבית `0xA4` (סימן אירו ב-8859-15, סימן מטבע ב-8859-1) מופה ל-`SYM` בשני המודלים, ולכן קיבלו ציון זהה | `0xA4` סומן כ-`ILL` במפת ISO-8859-1 — סימן המטבע (¤) כמעט אף פעם לא מופיע בטקסט אמיתי. כאשר `0xA4` מופיע, ה-prober של 8859-1 מוותר כעת, ומאפשר ל-8859-15 לנצח |
| `es/iso-8859-15` | `ISO-8859-1` | אותה עמימות `0xA4` כמו בדנית | אותו תיקון `ILL` הוחל על המפה הספרדית של ISO-8859-1 (הוחל גם באופן מונע על המפות הצרפתית והגרמנית) |
| `he/iso-8859-8` | `WINDOWS-1255` | **נתוני הבדיקה עצמם היו שגויים.** הם הכילו עברית בסדר לוגי (אותיות סופיות בסוף מילים), שה-prober מזהה בצדק כ-Windows-1255. ‏ISO-8859-8 הוא קידוד *ויזואלי* | קובץ הבדיקה הוחלף בעברית בסדר ויזואלי (רצפי בתים הפוכים בתוך שורות), מה שנותן למנתח האותיות הסופיות אות ברור של סדר ויזואלי |
| `ja/utf-16le` | `unknown` | ללא BOM. המזהה זיהה UTF-16 רק דרך BOM (‏`FE FF` / `FF FE`). בלעדיו, הנתונים הגיעו ל-probers של קידודים שכולם קיבלו ציון מתחת לסף | נוסף מזהה היוריסטי של דפוסי בתים אפסיים: סורק את 256 הבתים הראשונים לאיתור אפסים לסירוגין. מעל 20% אפסים במיקומים זוגיים עם פחות מ-5% באי-זוגיים = UTF-16BE; הדפוס ההפוך = UTF-16LE |
| `ja/utf-16be` | `unknown` | כנ"ל | אותה היוריסטיקה, דפוס בתים אפסיים הפוך |

## פרויקט מקורי

זהו פורק של [BYVoid/uchardet](https://github.com/BYVoid/uchardet). הפרויקט המקורי הקנוני עבר ל-[freedesktop.org](https://www.freedesktop.org/wiki/Software/uchardet/).

---
