[English](../README.md) | [العربية](README.ar.md) | [Azərbaycan](README.az.md) | [বাংলা](README.bn.md) | [Deutsch](README.de.md) | [Ελληνικά](README.el.md) | [Español](README.es.md) | [فارسی](README.fa.md) | [Français](README.fr.md) | [עברית](README.he.md) | [हिन्दी](README.hi.md) | [Magyar](README.hu.md) | [Italiano](README.it.md) | [日本語](README.ja.md) | [ភាសាខ្មែរ](README.km.md) | [한국어](README.ko.md) | [ਪੰਜਾਬੀ](README.pa.md) | [Polski](README.pl.md) | [پښتو](README.ps.md) | [Português (BR)](README.pt_br.md) | [Română](README.ro.md) | [Русский](README.ru.md) | [Kiswahili](README.sw.md) | [தமிழ்](README.ta.md) | [ไทย](README.th.md) | [Türkçe](README.tr.md) | [Українська](README.uk.md) | [اردو](README.ur.md) | [Tiếng Việt](README.vi.md) | [简体中文](README.zh_hans.md)

# uchardetz

Um fork do [uchardet](https://github.com/BYVoid/uchardet) (uma biblioteca de deteccao de codificacao portada do Mozilla) com suporte ao **sistema de build do Zig**.

## Build com Zig

Este fork adiciona `build.zig` e `build.zig.zon` para que a biblioteca estatica, a biblioteca compartilhada, a ferramenta CLI e os testes possam ser compilados com o sistema de build do Zig (0.15+). Isso facilita o uso como dependencia em projetos Zig ou a compilacao cruzada para qualquer plataforma suportada pelo Zig.

### Compilando com Zig (via Nix)

```bash
# Entre no shell de desenvolvimento (fornece Zig 0.15 e macOS SDK)
nix develop

# Compile tudo (lib estatica + lib compartilhada + CLI)
zig build

# Execute a CLI
zig build run -- somefile.txt
echo "some text" | ./zig-out/bin/uchardet

# Execute a suite de testes
zig build test
```

### Compilando diretamente com Nix

```bash
nix build
# Saidas: result/bin/uchardet, result/lib/libuchardet.{a,dylib}, result/include/uchardet/uchardet.h
```

### Usando como dependencia Zig

Adicione ao seu `build.zig.zon`:

```zig
.dependencies = .{
    .uchardetz = .{
        .url = "https://github.com/pmarreck/uchardetz/archive/<commit>.tar.gz",
        .hash = "...",
    },
},
```

Depois, no seu `build.zig`:

```zig
const uchardetz_dep = b.dependency("uchardetz", .{
    .target = target,
    .optimize = optimize,
});
your_module.linkLibrary(uchardetz_dep.artifact("uchardet"));
```

## Saidas

- **`libuchardet.a`** - biblioteca estatica
- **`libuchardet.dylib`** (ou `.so`) - biblioteca compartilhada
- **`uchardet`** - ferramenta CLI
- **`include/uchardet/uchardet.h`** - header publico C

## Correcoes em relacao ao upstream

O upstream ignorava 5 testes como "sabidamente quebrados". Este fork corrige todos eles (60/60 passando, 0 regressoes). Nenhum era uma limitacao fundamental do algoritmo de deteccao -- cada um teve uma correcao direcionada.

| Teste | Retornava | Causa raiz | Correcao |
|------|--------------|------------|-----|
| `da/iso-8859-1` | `ISO-8859-15` | Pontuacoes de confianca identicas; o prober ISO-8859-15 executava primeiro, vencendo empates | Invertida a ordem do prober dinamarques para que ISO-8859-1 (mais antigo, mais comum) venca empates |
| `da/iso-8859-15` | *(ignorado)* | O byte `0xA4` (sinal do Euro em 8859-15, sinal de moeda em 8859-1) era mapeado como `SYM` em ambos os modelos, resultando em pontuacoes identicas | Marcado `0xA4` como `ILL` no mapa ISO-8859-1 -- o sinal de moeda (¤) praticamente nunca e usado em texto real. Quando `0xA4` aparece, o prober 8859-1 agora desiste, permitindo que 8859-15 venca |
| `es/iso-8859-15` | `ISO-8859-1` | Mesma ambiguidade do `0xA4` que no dinamarques | Mesma correcao `ILL` aplicada ao mapa espanhol ISO-8859-1 (tambem aplicada proativamente aos mapas frances e alemao) |
| `he/iso-8859-8` | `WINDOWS-1255` | **Os proprios dados de teste estavam errados.** Continham hebraico em ordem logica (letras em forma final no fim das palavras), que o prober identifica corretamente como Windows-1255. ISO-8859-8 e uma codificacao *visual* | Substituido o arquivo de teste por hebraico em ordem visual (sequencias de bytes invertidas dentro das linhas), dando ao analisador de letras finais um sinal claro de ordem visual |
| `ja/utf-16le` | `unknown` | Sem BOM presente. O detector so reconhecia UTF-16 via BOM (`FE FF` / `FF FE`). Sem ele, os dados passavam para probers de charset que pontuavam abaixo do limiar | Adicionado um detector heuristico de padrao de bytes nulos: escaneia os primeiros 256 bytes procurando nulos alternados. >20% de nulos em posicoes pares com <5% em impares = UTF-16BE; o inverso = UTF-16LE |
| `ja/utf-16be` | `unknown` | Mesmo que acima | Mesma heuristica, padrao de bytes nulos invertido |

## Upstream

Este e um fork do [BYVoid/uchardet](https://github.com/BYVoid/uchardet). O projeto upstream canonico mudou-se para o [freedesktop.org](https://www.freedesktop.org/wiki/Software/uchardet/).

---
