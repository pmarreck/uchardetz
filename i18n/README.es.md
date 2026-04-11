[English](../README.md) | [العربية](README.ar.md) | [Azərbaycan](README.az.md) | [বাংলা](README.bn.md) | [Deutsch](README.de.md) | [Ελληνικά](README.el.md) | [Español](README.es.md) | [فارسی](README.fa.md) | [Français](README.fr.md) | [עברית](README.he.md) | [हिन्दी](README.hi.md) | [Magyar](README.hu.md) | [Italiano](README.it.md) | [日本語](README.ja.md) | [ភាសាខ្មែរ](README.km.md) | [한국어](README.ko.md) | [ਪੰਜਾਬੀ](README.pa.md) | [Polski](README.pl.md) | [پښتو](README.ps.md) | [Português (BR)](README.pt_br.md) | [Română](README.ro.md) | [Русский](README.ru.md) | [Kiswahili](README.sw.md) | [தமிழ்](README.ta.md) | [ไทย](README.th.md) | [Türkçe](README.tr.md) | [Українська](README.uk.md) | [اردو](README.ur.md) | [Tiếng Việt](README.vi.md) | [简体中文](README.zh_hans.md)

# uchardetz

Un fork de [uchardet](https://github.com/BYVoid/uchardet) (una biblioteca de detección de codificación portada desde Mozilla) con soporte para el **sistema de compilación Zig**.

## Compilación con Zig

Este fork añade `build.zig` y `build.zig.zon` para que la biblioteca estática, la biblioteca compartida, la herramienta CLI y los tests se puedan compilar con el sistema de compilación Zig (0.15+), facilitando su uso como dependencia en proyectos Zig o la compilación cruzada para cualquier plataforma que Zig soporte.

### Compilar con Zig (mediante Nix)

```bash
# Entrar al shell de desarrollo (proporciona Zig 0.15 y el SDK de macOS)
nix develop

# Compilar todo (biblioteca estática + biblioteca compartida + CLI)
zig build

# Ejecutar la CLI
zig build run -- somefile.txt
echo "some text" | ./zig-out/bin/uchardet

# Ejecutar la suite de tests
zig build test
```

### Compilar directamente con Nix

```bash
nix build
# Salida: result/bin/uchardet, result/lib/libuchardet.{a,dylib}, result/include/uchardet/uchardet.h
```

### Uso como dependencia de Zig

Añade a tu `build.zig.zon`:

```zig
.dependencies = .{
    .uchardetz = .{
        .url = "https://github.com/pmarreck/uchardetz/archive/<commit>.tar.gz",
        .hash = "...",
    },
},
```

Luego en tu `build.zig`:

```zig
const uchardetz_dep = b.dependency("uchardetz", .{
    .target = target,
    .optimize = optimize,
});
your_module.linkLibrary(uchardetz_dep.artifact("uchardet"));
```

## Artefactos generados

- **`libuchardet.a`** — biblioteca estática
- **`libuchardet.dylib`** (o `.so`) — biblioteca compartida
- **`uchardet`** — herramienta CLI
- **`include/uchardet/uchardet.h`** — cabecera pública en C

## Correcciones respecto al proyecto original

El proyecto original omitía 5 tests marcándolos como "fallos conocidos". Este fork los corrige todos (60/60 pasando, 0 regresiones). Ninguno era una limitación fundamental del algoritmo de detección — cada uno tenía una corrección específica.

| Test | Devolvía | Causa raíz | Corrección |
|------|----------|-------------|------------|
| `da/iso-8859-1` | `ISO-8859-15` | Puntuaciones de confianza idénticas; el prober de ISO-8859-15 se ejecutaba primero y ganaba en caso de empate | Se reordenaron los probers daneses para que ISO-8859-1 (más antiguo, más común) gane en empates |
| `da/iso-8859-15` | *(omitido)* | El byte `0xA4` (símbolo del euro en 8859-15, símbolo de moneda en 8859-1) estaba mapeado como `SYM` en ambos modelos, por lo que obtenían puntuaciones idénticas | Se marcó `0xA4` como `ILL` en el mapa ISO-8859-1 — el símbolo de moneda (¤) prácticamente nunca se usa en texto real. Cuando aparece `0xA4`, el prober de 8859-1 ahora aborta, permitiendo que 8859-15 gane |
| `es/iso-8859-15` | `ISO-8859-1` | La misma ambigüedad de `0xA4` que en danés | La misma corrección `ILL` aplicada al mapa español de ISO-8859-1 (también aplicada preventivamente a los mapas francés y alemán) |
| `he/iso-8859-8` | `WINDOWS-1255` | **Los datos del test estaban mal.** Contenían hebreo en orden lógico (letras en forma final al final de las palabras), que el prober identifica correctamente como Windows-1255. ISO-8859-8 es una codificación *visual* | Se reemplazó el archivo de test con hebreo en orden visual (secuencias de bytes invertidas dentro de las líneas), dando al analizador de letras finales una señal clara de orden visual |
| `ja/utf-16le` | `unknown` | Sin BOM presente. El detector solo reconocía UTF-16 mediante BOM (`FE FF` / `FF FE`). Sin ella, los datos pasaban a los probers de codificación que puntuaban por debajo del umbral | Se añadió un detector heurístico de patrones de bytes nulos: escanea los primeros 256 bytes buscando nulos alternantes. >20% de nulos en posiciones pares con <5% en impares = UTF-16BE; el patrón inverso = UTF-16LE |
| `ja/utf-16be` | `unknown` | Igual que el anterior | La misma heurística, con el patrón de bytes nulos inverso |

## Proyecto original

Este es un fork de [BYVoid/uchardet](https://github.com/BYVoid/uchardet). El proyecto original canónico se ha trasladado a [freedesktop.org](https://www.freedesktop.org/wiki/Software/uchardet/).

---
