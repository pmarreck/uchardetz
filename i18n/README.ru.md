[English](../README.md) | [العربية](README.ar.md) | [Azərbaycan](README.az.md) | [বাংলা](README.bn.md) | [Deutsch](README.de.md) | [Ελληνικά](README.el.md) | [Español](README.es.md) | [فارسی](README.fa.md) | [Français](README.fr.md) | [עברית](README.he.md) | [हिन्दी](README.hi.md) | [Magyar](README.hu.md) | [Italiano](README.it.md) | [日本語](README.ja.md) | [ភាសាខ្មែរ](README.km.md) | [한국어](README.ko.md) | [ਪੰਜਾਬੀ](README.pa.md) | [Polski](README.pl.md) | [پښتو](README.ps.md) | [Português (BR)](README.pt_br.md) | [Română](README.ro.md) | [Русский](README.ru.md) | [Kiswahili](README.sw.md) | [தமிழ்](README.ta.md) | [ไทย](README.th.md) | [Türkçe](README.tr.md) | [Українська](README.uk.md) | [اردو](README.ur.md) | [Tiếng Việt](README.vi.md) | [简体中文](README.zh_hans.md)

# uchardetz

Форк [uchardet](https://github.com/BYVoid/uchardet) (библиотеки определения кодировки, портированной из Mozilla) с поддержкой **системы сборки Zig**.

## Сборка с помощью Zig

Этот форк добавляет `build.zig` и `build.zig.zon`, что позволяет собирать библиотеку, разделяемую библиотеку, CLI-утилиту и тесты с помощью системы сборки Zig (0.15+). Это упрощает использование в качестве зависимости в Zig-проектах и кросс-компиляцию под любую платформу, которую поддерживает Zig.

### Сборка с помощью Zig (через Nix)

```bash
# Войти в dev shell (предоставляет Zig 0.15 и macOS SDK)
nix develop

# Собрать всё (статическая библиотека + разделяемая библиотека + CLI)
zig build

# Запустить CLI
zig build run -- somefile.txt
echo "some text" | ./zig-out/bin/uchardet

# Запустить тесты
zig build test
```

### Сборка напрямую через Nix

```bash
nix build
# Результат: result/bin/uchardet, result/lib/libuchardet.{a,dylib}, result/include/uchardet/uchardet.h
```

### Использование в качестве зависимости Zig

Добавьте в ваш `build.zig.zon`:

```zig
.dependencies = .{
    .uchardetz = .{
        .url = "https://github.com/pmarreck/uchardetz/archive/<commit>.tar.gz",
        .hash = "...",
    },
},
```

Затем в вашем `build.zig`:

```zig
const uchardetz_dep = b.dependency("uchardetz", .{
    .target = target,
    .optimize = optimize,
});
your_module.linkLibrary(uchardetz_dep.artifact("uchardet"));
```

## Выходные артефакты

- **`libuchardet.a`** — статическая библиотека
- **`libuchardet.dylib`** (или `.so`) — разделяемая библиотека
- **`uchardet`** — CLI-утилита
- **`include/uchardet/uchardet.h`** — публичный C-заголовок

## Исправления по сравнению с upstream

Upstream пропускал 5 тестов как «заведомо нерабочие». Этот форк исправляет все (60 из 60 проходят, 0 регрессий). Ни одна из проблем не была фундаментальным ограничением алгоритма определения — для каждой нашлось точечное исправление.

| Тест | Возвращал | Причина | Исправление |
|------|-----------|---------|-------------|
| `da/iso-8859-1` | `ISO-8859-15` | Одинаковые показатели уверенности; prober ISO-8859-15 запускался первым и выигрывал при равном счёте | Изменён порядок prober-ов для датского, чтобы ISO-8859-1 (более старая и распространённая) побеждала при равном счёте |
| `da/iso-8859-15` | *(пропущен)* | Байт `0xA4` (знак евро в 8859-15, знак валюты в 8859-1) отображён как `SYM` в обеих моделях, поэтому оценки совпадали | Байт `0xA4` помечен как `ILL` в карте ISO-8859-1 — символ валюты (¤) практически не встречается в реальных текстах. Теперь при появлении `0xA4` prober 8859-1 отступает, позволяя 8859-15 победить |
| `es/iso-8859-15` | `ISO-8859-1` | Та же неоднозначность `0xA4`, что и для датского | То же исправление с `ILL` применено к испанской карте ISO-8859-1 (а также превентивно к французской и немецкой) |
| `he/iso-8859-8` | `WINDOWS-1255` | **Сами тестовые данные были неправильными.** Они содержали логически упорядоченный иврит (финальные формы букв в конце слов), который prober корректно определяет как Windows-1255. ISO-8859-8 — это *визуальная* кодировка | Тестовый файл заменён на визуально упорядоченный иврит (побайтово перевёрнутые фрагменты в строках), что даёт анализатору финальных букв чёткий сигнал визуального порядка |
| `ja/utf-16le` | `unknown` | BOM отсутствует. Детектор распознавал UTF-16 только по BOM (`FE FF` / `FF FE`). Без него данные проходили через все charset prober-ы, ни один из которых не набирал достаточный порог уверенности | Добавлен эвристический детектор паттерна нулевых байтов: сканирует первые 256 байт на предмет чередующихся нулей. >20% нулей на чётных позициях при <5% на нечётных = UTF-16BE; обратный паттерн = UTF-16LE |
| `ja/utf-16be` | `unknown` | То же, что и выше | Та же эвристика, обратный паттерн нулевых байтов |

## Upstream

Это форк [BYVoid/uchardet](https://github.com/BYVoid/uchardet). Каноническая версия upstream-проекта перенесена на [freedesktop.org](https://www.freedesktop.org/wiki/Software/uchardet/).

---
