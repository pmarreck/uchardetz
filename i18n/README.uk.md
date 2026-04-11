[English](../README.md) | [العربية](README.ar.md) | [Azərbaycan](README.az.md) | [বাংলা](README.bn.md) | [Deutsch](README.de.md) | [Ελληνικά](README.el.md) | [Español](README.es.md) | [فارسی](README.fa.md) | [Français](README.fr.md) | [עברית](README.he.md) | [हिन्दी](README.hi.md) | [Magyar](README.hu.md) | [Italiano](README.it.md) | [日本語](README.ja.md) | [ភាសាខ្មែរ](README.km.md) | [한국어](README.ko.md) | [ਪੰਜਾਬੀ](README.pa.md) | [Polski](README.pl.md) | [پښتو](README.ps.md) | [Português (BR)](README.pt_br.md) | [Română](README.ro.md) | [Русский](README.ru.md) | [Kiswahili](README.sw.md) | [தமிழ்](README.ta.md) | [ไทย](README.th.md) | [Türkçe](README.tr.md) | [Українська](README.uk.md) | [اردو](README.ur.md) | [Tiếng Việt](README.vi.md) | [简体中文](README.zh_hans.md)

# uchardetz

Форк [uchardet](https://github.com/BYVoid/uchardet) (бібліотека визначення кодування, портована з Mozilla) з підтримкою **системи збірки Zig**.

## Збірка Zig

Цей форк додає `build.zig` та `build.zig.zon`, завдяки чому бібліотеку, динамічну бібліотеку, CLI-утиліту та тести можна зібрати за допомогою системи збірки Zig (0.15+). Це спрощує використання як залежності в проєктах на Zig та крос-компіляцію для будь-якої платформи, яку підтримує Zig.

### Збірка за допомогою Zig (через Nix)

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

### Збірка безпосередньо через Nix

```bash
nix build
# Outputs: result/bin/uchardet, result/lib/libuchardet.{a,dylib}, result/include/uchardet/uchardet.h
```

### Використання як залежності Zig

Додайте до вашого `build.zig.zon`:

```zig
.dependencies = .{
    .uchardetz = .{
        .url = "https://github.com/pmarreck/uchardetz/archive/<commit>.tar.gz",
        .hash = "...",
    },
},
```

Потім у вашому `build.zig`:

```zig
const uchardetz_dep = b.dependency("uchardetz", .{
    .target = target,
    .optimize = optimize,
});
your_module.linkLibrary(uchardetz_dep.artifact("uchardet"));
```

## Артефакти

- **`libuchardet.a`** -- статична бібліотека
- **`libuchardet.dylib`** (або `.so`) -- динамічна бібліотека
- **`uchardet`** -- CLI-утиліта
- **`include/uchardet/uchardet.h`** -- публічний C-заголовок

## Виправлення порівняно з upstream

Upstream пропускав 5 тестів як "відомо зламані". Цей форк виправляє їх усі (60/60 проходять, 0 регресій). Жодне з них не було фундаментальним обмеженням алгоритму визначення -- кожне мало цілеспрямоване виправлення.

| Тест | Повертав | Причина | Виправлення |
|------|----------|---------|-------------|
| `da/iso-8859-1` | `ISO-8859-15` | Ідентичні оцінки впевненості; prober ISO-8859-15 запускався першим і вигравав при рівності | Змінено порядок prober-ів для данської мови, щоб ISO-8859-1 (старіше, поширеніше) вигравало при рівності |
| `da/iso-8859-15` | *(пропущено)* | Байт `0xA4` (знак євро в 8859-15, знак валюти в 8859-1) відображався як `SYM` в обох моделях, тому вони оцінювалися однаково | Позначено `0xA4` як `ILL` у карті ISO-8859-1 -- знак валюти (¤) практично ніколи не зустрічається в реальних текстах. Коли з'являється `0xA4`, prober 8859-1 тепер відмовляється, дозволяючи 8859-15 перемогти |
| `es/iso-8859-15` | `ISO-8859-1` | Та сама неоднозначність `0xA4`, що й для данської | Те саме виправлення `ILL` застосовано до карти ISO-8859-1 для іспанської (також превентивно застосовано для французької та німецької карт) |
| `he/iso-8859-8` | `WINDOWS-1255` | **Самі тестові дані були неправильними.** Вони містили логічно впорядковану іврит (кінцеві форми літер наприкінці слів), що prober коректно розпізнає як Windows-1255. ISO-8859-8 -- це *візуальне* кодування | Замінено тестовий файл на візуально впорядковану іврит (байти у зворотному порядку в межах рядків), що дає аналізатору кінцевих літер чіткий сигнал візуального порядку |
| `ja/utf-16le` | `unknown` | BOM відсутній. Детектор розпізнавав UTF-16 лише через BOM (`FE FF` / `FF FE`). Без нього дані потрапляли до prober-ів кодувань, які всі давали оцінку нижче порогу | Додано евристичний детектор шаблонів нульових байтів: сканує перші 256 байтів на чергування нулів. >20% нулів на парних позиціях при <5% на непарних = UTF-16BE; зворотне -- UTF-16LE |
| `ja/utf-16be` | `unknown` | Те саме, що вище | Та сама евристика, протилежний шаблон нульових байтів |

## Upstream

Це форк [BYVoid/uchardet](https://github.com/BYVoid/uchardet). Канонічний upstream-проєкт переїхав на [freedesktop.org](https://www.freedesktop.org/wiki/Software/uchardet/).

---
