[English](README.md) | [العربية](README.ar.md) | [Azərbaycan](README.az.md) | [বাংলা](README.bn.md) | [Deutsch](README.de.md) | [Ελληνικά](README.el.md) | [Español](README.es.md) | [فارسی](README.fa.md) | [Français](README.fr.md) | [עברית](README.he.md) | [हिन्दी](README.hi.md) | [Magyar](README.hu.md) | [Italiano](README.it.md) | [日本語](README.ja.md) | [ភាសាខ្មែរ](README.km.md) | [한국어](README.ko.md) | [ਪੰਜਾਬੀ](README.pa.md) | [Polski](README.pl.md) | [پښتو](README.ps.md) | [Português (BR)](README.pt_br.md) | [Română](README.ro.md) | [Русский](README.ru.md) | [Kiswahili](README.sw.md) | [தமிழ்](README.ta.md) | [ไทย](README.th.md) | [Türkçe](README.tr.md) | [Українська](README.uk.md) | [اردو](README.ur.md) | [Tiếng Việt](README.vi.md) | [简体中文](README.zh_hans.md)

# uchardetz

[uchardet](https://github.com/BYVoid/uchardet)（源自 Mozilla 的编码检测库）的一个分支，增加了 **Zig 构建系统**支持。

## Zig 构建

本分支添加了 `build.zig` 和 `build.zig.zon`，使得静态库、动态库、CLI 工具和测试都可以通过 Zig 构建系统（0.15+）来构建。这让在 Zig 项目中将其作为依赖使用或交叉编译到 Zig 支持的任意目标平台变得十分方便。

### 使用 Zig 构建（通过 Nix）

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

### 直接使用 Nix 构建

```bash
nix build
# Outputs: result/bin/uchardet, result/lib/libuchardet.{a,dylib}, result/include/uchardet/uchardet.h
```

### 作为 Zig 依赖使用

在你的 `build.zig.zon` 中添加：

```zig
.dependencies = .{
    .uchardetz = .{
        .url = "https://github.com/pmarreck/uchardetz/archive/<commit>.tar.gz",
        .hash = "...",
    },
},
```

然后在你的 `build.zig` 中：

```zig
const uchardetz_dep = b.dependency("uchardetz", .{
    .target = target,
    .optimize = optimize,
});
your_module.linkLibrary(uchardetz_dep.artifact("uchardet"));
```

## 构建产物

- **`libuchardet.a`** -- 静态库
- **`libuchardet.dylib`**（或 `.so`）-- 动态库
- **`uchardet`** -- CLI 工具
- **`include/uchardet/uchardet.h`** -- 公共 C 头文件

## 相对上游的修复

上游将 5 个测试标记为"已知故障"并跳过。本分支修复了所有这些问题（60/60 通过，0 个回归）。这些都不是检测算法的根本局限——每一个都有针对性的修复方案。

| 测试 | 原返回值 | 根本原因 | 修复方法 |
|------|---------|---------|---------|
| `da/iso-8859-1` | `ISO-8859-15` | 置信度评分完全相同；ISO-8859-15 prober 先执行，在平局时胜出 | 调换了丹麦语 prober 的顺序，使 ISO-8859-1（更早、更常用）在平局时胜出 |
| `da/iso-8859-15` | *（已跳过）* | 字节 `0xA4`（8859-15 中为欧元符号，8859-1 中为货币符号）在两个模型中均映射为 `SYM`，因此评分完全一致 | 在 ISO-8859-1 映射表中将 `0xA4` 标记为 `ILL`——货币符号（¤）在实际文本中几乎从未使用。当出现 `0xA4` 时，8859-1 prober 会立即放弃，让 8859-15 胜出 |
| `es/iso-8859-15` | `ISO-8859-1` | 与丹麦语相同的 `0xA4` 歧义 | 对西班牙语 ISO-8859-1 映射表应用了相同的 `ILL` 修复（同时预防性地应用于法语和德语映射表） |
| `he/iso-8859-8` | `WINDOWS-1255` | **测试数据本身有误。** 数据包含逻辑顺序的希伯来文（词尾使用终止形式字母），prober 正确地将其识别为 Windows-1255。ISO-8859-8 是一种*视觉*编码 | 将测试文件替换为视觉顺序的希伯来文（行内字节逆序排列），使词尾字母分析器能获得清晰的视觉顺序信号 |
| `ja/utf-16le` | `unknown` | 无 BOM。检测器仅通过 BOM（`FE FF` / `FF FE`）识别 UTF-16。没有 BOM 时，数据落入各 charset prober，但所有评分均低于阈值 | 添加了基于启发式的空字节模式检测器：扫描前 256 字节中交替出现的空字节。偶数位置空字节占比 >20% 且奇数位置 <5% = UTF-16BE；反之 = UTF-16LE |
| `ja/utf-16be` | `unknown` | 同上 | 相同的启发式方法，空字节模式相反 |

## 上游项目

本项目是 [BYVoid/uchardet](https://github.com/BYVoid/uchardet) 的分支。上游规范项目已迁移至 [freedesktop.org](https://www.freedesktop.org/wiki/Software/uchardet/)。

---
