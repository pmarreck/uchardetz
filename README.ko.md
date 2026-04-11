[English](README.md) | [العربية](README.ar.md) | [Azərbaycan](README.az.md) | [বাংলা](README.bn.md) | [Deutsch](README.de.md) | [Ελληνικά](README.el.md) | [Español](README.es.md) | [فارسی](README.fa.md) | [Français](README.fr.md) | [עברית](README.he.md) | [हिन्दी](README.hi.md) | [Magyar](README.hu.md) | [Italiano](README.it.md) | [日本語](README.ja.md) | [ភាសាខ្មែរ](README.km.md) | [한국어](README.ko.md) | [ਪੰਜਾਬੀ](README.pa.md) | [Polski](README.pl.md) | [پښتو](README.ps.md) | [Português (BR)](README.pt_br.md) | [Română](README.ro.md) | [Русский](README.ru.md) | [Kiswahili](README.sw.md) | [தமிழ்](README.ta.md) | [ไทย](README.th.md) | [Türkçe](README.tr.md) | [Українська](README.uk.md) | [اردو](README.ur.md) | [Tiếng Việt](README.vi.md) | [简体中文](README.zh_hans.md)

# uchardetz

[uchardet](https://github.com/BYVoid/uchardet)(Mozilla에서 포팅된 인코딩 감지 라이브러리)의 포크로, **Zig 빌드 시스템**을 지원합니다.

## Zig 빌드

이 포크는 `build.zig`와 `build.zig.zon`을 추가하여 라이브러리, 공유 라이브러리, CLI 도구, 테스트 모두를 Zig 빌드 시스템(0.15 이상)으로 빌드할 수 있게 합니다. 이를 통해 Zig 프로젝트의 의존성으로 쉽게 사용하거나 Zig가 지원하는 모든 대상으로 크로스 컴파일할 수 있습니다.

### Zig로 빌드하기 (Nix 경유)

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

### Nix로 직접 빌드하기

```bash
nix build
# Outputs: result/bin/uchardet, result/lib/libuchardet.{a,dylib}, result/include/uchardet/uchardet.h
```

### Zig 의존성으로 사용하기

`build.zig.zon`에 다음을 추가하세요:

```zig
.dependencies = .{
    .uchardetz = .{
        .url = "https://github.com/pmarreck/uchardetz/archive/<commit>.tar.gz",
        .hash = "...",
    },
},
```

그런 다음 `build.zig`에서:

```zig
const uchardetz_dep = b.dependency("uchardetz", .{
    .target = target,
    .optimize = optimize,
});
your_module.linkLibrary(uchardetz_dep.artifact("uchardet"));
```

## 출력물

- **`libuchardet.a`** – 정적 라이브러리
- **`libuchardet.dylib`** (또는 `.so`) – 공유 라이브러리
- **`uchardet`** – CLI 도구
- **`include/uchardet/uchardet.h`** – 공개 C 헤더

## upstream 대비 수정 사항

upstream에서는 5개의 테스트를 "알려진 결함"으로 건너뛰었습니다. 이 포크에서는 모두 수정했습니다 (60/60 통과, 회귀 0건). 어느 것도 감지 알고리즘의 근본적인 한계가 아니었으며, 각각 대상을 특정한 수정이 있었습니다.

| 테스트 | 반환하던 값 | 근본 원인 | 수정 내용 |
|--------|-----------|-----------|-----------|
| `da/iso-8859-1` | `ISO-8859-15` | 신뢰도 점수가 동일했고, ISO-8859-15 prober가 먼저 실행되어 동점 시 승리함 | 덴마크어 prober 순서를 교체하여 ISO-8859-1(더 오래되고 더 일반적인)이 동점 시 승리하도록 함 |
| `da/iso-8859-15` | *(건너뜀)* | 바이트 `0xA4`(8859-15에서는 유로 기호, 8859-1에서는 통화 기호)가 두 모델 모두에서 `SYM`으로 매핑되어 동일한 점수를 받음 | ISO-8859-1 맵에서 `0xA4`를 `ILL`로 표시 — 통화 기호(¤)는 실제 텍스트에서 사실상 사용되지 않음. `0xA4`가 나타나면 8859-1 prober가 이제 탈락하여 8859-15가 승리함 |
| `es/iso-8859-15` | `ISO-8859-1` | 덴마크어와 동일한 `0xA4` 모호성 | 동일한 `ILL` 수정을 스페인어 ISO-8859-1 맵에 적용 (프랑스어와 독일어 맵에도 사전 적용) |
| `he/iso-8859-8` | `WINDOWS-1255` | **테스트 데이터 자체가 잘못되었음.** 논리적 순서의 히브리어(단어 끝에 최종 형태 문자)가 포함되어 있었으며, prober는 이를 Windows-1255로 올바르게 식별함. ISO-8859-8은 *시각적* 인코딩임 | 테스트 파일을 시각적 순서의 히브리어(행 내에서 바이트 순서가 역전된 문자열)로 교체하여, 최종 문자 분석기에 명확한 시각적 순서 신호를 제공 |
| `ja/utf-16le` | `unknown` | BOM이 없었음. 감지기는 BOM(`FE FF` / `FF FE`)을 통해서만 UTF-16을 인식했음. BOM이 없으면 데이터가 charset prober들로 넘어갔고, 모두 임계값 이하의 점수를 받음 | 휴리스틱 null-byte 패턴 감지기 추가: 처음 256바이트를 스캔하여 교대로 나타나는 null을 탐지. 짝수 바이트 위치에서 >20% null이고 홀수 위치에서 <5%이면 = UTF-16BE; 반대이면 = UTF-16LE |
| `ja/utf-16be` | `unknown` | 위와 동일 | 동일한 휴리스틱, 반대 null-byte 패턴 |

## Upstream

이것은 [BYVoid/uchardet](https://github.com/BYVoid/uchardet)의 포크입니다. 정식 upstream 프로젝트는 [freedesktop.org](https://www.freedesktop.org/wiki/Software/uchardet/)으로 이전되었습니다.

---
