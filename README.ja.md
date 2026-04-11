[English](README.md) | [العربية](README.ar.md) | [Azərbaycan](README.az.md) | [বাংলা](README.bn.md) | [Deutsch](README.de.md) | [Ελληνικά](README.el.md) | [Español](README.es.md) | [فارسی](README.fa.md) | [Français](README.fr.md) | [עברית](README.he.md) | [हिन्दी](README.hi.md) | [Magyar](README.hu.md) | [Italiano](README.it.md) | [日本語](README.ja.md) | [ភាសាខ្មែរ](README.km.md) | [한국어](README.ko.md) | [ਪੰਜਾਬੀ](README.pa.md) | [Polski](README.pl.md) | [پښتو](README.ps.md) | [Português (BR)](README.pt_br.md) | [Română](README.ro.md) | [Русский](README.ru.md) | [Kiswahili](README.sw.md) | [தமிழ்](README.ta.md) | [ไทย](README.th.md) | [Türkçe](README.tr.md) | [Українська](README.uk.md) | [اردو](README.ur.md) | [Tiếng Việt](README.vi.md) | [简体中文](README.zh_hans.md)

# uchardetz

[uchardet](https://github.com/BYVoid/uchardet)（Mozillaから移植された文字エンコーディング検出ライブラリ）のフォークで、**Zigビルドシステム**をサポートしています。

## Zigビルド

このフォークでは `build.zig` と `build.zig.zon` を追加し、ライブラリ、共有ライブラリ、CLIツール、テストのすべてをZigビルドシステム（0.15以降）でビルドできるようにしました。これにより、Zigプロジェクトの依存関係として簡単に利用でき、Zigがサポートする任意のターゲットへのクロスコンパイルも可能です。

### Zigでのビルド（Nix経由）

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

### Nixで直接ビルド

```bash
nix build
# Outputs: result/bin/uchardet, result/lib/libuchardet.{a,dylib}, result/include/uchardet/uchardet.h
```

### Zig依存関係として使用する

`build.zig.zon` に以下を追加します：

```zig
.dependencies = .{
    .uchardetz = .{
        .url = "https://github.com/pmarreck/uchardetz/archive/<commit>.tar.gz",
        .hash = "...",
    },
},
```

次に `build.zig` で以下のように記述します：

```zig
const uchardetz_dep = b.dependency("uchardetz", .{
    .target = target,
    .optimize = optimize,
});
your_module.linkLibrary(uchardetz_dep.artifact("uchardet"));
```

## 出力物

- **`libuchardet.a`** – 静的ライブラリ
- **`libuchardet.dylib`**（または `.so`） – 共有ライブラリ
- **`uchardet`** – CLIツール
- **`include/uchardet/uchardet.h`** – パブリックCヘッダー

## upstreamからの修正

upstreamでは5つのテストが「既知の不具合」としてスキップされていました。このフォークではすべてを修正しています（60/60合格、リグレッション0件）。いずれも検出アルゴリズムの根本的な限界ではなく、それぞれに対象を絞った修正がありました。

| テスト | 返していた値 | 根本原因 | 修正内容 |
|--------|-------------|----------|----------|
| `da/iso-8859-1` | `ISO-8859-15` | 信頼度スコアが同一で、ISO-8859-15のproberが先に実行されたため、同点時に勝利していた | デンマーク語のprober順序を入れ替え、ISO-8859-1（より古く、より一般的）が同点時に勝つようにした |
| `da/iso-8859-15` | *（スキップ）* | バイト `0xA4`（8859-15ではユーロ記号、8859-1では通貨記号）が両モデルで `SYM` にマッピングされていたため、同一スコアになっていた | ISO-8859-1マップで `0xA4` を `ILL` としてマーク — 通貨記号（¤）は実際のテキストでほぼ使われない。`0xA4` が出現すると8859-1のproberが離脱し、8859-15が勝つようになった |
| `es/iso-8859-15` | `ISO-8859-1` | デンマーク語と同じ `0xA4` の曖昧性 | 同じ `ILL` 修正をスペイン語のISO-8859-1マップに適用（フランス語とドイツ語のマップにも予防的に適用） |
| `he/iso-8859-8` | `WINDOWS-1255` | **テストデータ自体が誤っていた。** 論理順序のヘブライ語（語末に終止形文字）が含まれており、proberはこれをWindows-1255として正しく識別する。ISO-8859-8は*視覚的*エンコーディングである | テストファイルを視覚的順序のヘブライ語（行内でバイト順を反転した文字列）に置き換え、終止形文字分析器に明確な視覚的順序のシグナルを与えた |
| `ja/utf-16le` | `unknown` | BOMが存在しなかった。検出器はBOM（`FE FF` / `FF FE`）経由でのみUTF-16を認識していた。BOMがない場合、データはすべてのcharset proberに流れ、いずれも閾値以下のスコアだった | ヒューリスティックなヌルバイトパターン検出器を追加：最初の256バイトを走査し、交互に現れるヌルを探す。偶数バイト位置で>20%のヌルかつ奇数位置で<5% = UTF-16BE、逆のパターン = UTF-16LE |
| `ja/utf-16be` | `unknown` | 上記と同じ | 同じヒューリスティック、逆のヌルバイトパターン |

## Upstream

これは [BYVoid/uchardet](https://github.com/BYVoid/uchardet) のフォークです。正式なupstreamプロジェクトは [freedesktop.org](https://www.freedesktop.org/wiki/Software/uchardet/) に移動しました。

---
