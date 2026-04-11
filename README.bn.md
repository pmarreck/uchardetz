[English](README.md) | [العربية](README.ar.md) | [Azərbaycan](README.az.md) | [বাংলা](README.bn.md) | [Deutsch](README.de.md) | [Ελληνικά](README.el.md) | [Español](README.es.md) | [فارسی](README.fa.md) | [Français](README.fr.md) | [עברית](README.he.md) | [हिन्दी](README.hi.md) | [Magyar](README.hu.md) | [Italiano](README.it.md) | [日本語](README.ja.md) | [ភាសាខ្មែរ](README.km.md) | [한국어](README.ko.md) | [ਪੰਜਾਬੀ](README.pa.md) | [Polski](README.pl.md) | [پښتو](README.ps.md) | [Português (BR)](README.pt_br.md) | [Română](README.ro.md) | [Русский](README.ru.md) | [Kiswahili](README.sw.md) | [தமிழ்](README.ta.md) | [ไทย](README.th.md) | [Türkçe](README.tr.md) | [Українська](README.uk.md) | [اردو](README.ur.md) | [Tiếng Việt](README.vi.md) | [简体中文](README.zh_hans.md)

# uchardetz

[uchardet](https://github.com/BYVoid/uchardet)-এর একটি ফর্ক (Mozilla থেকে পোর্ট করা একটি এনকোডিং শনাক্তকরণ লাইব্রেরি) — **Zig বিল্ড সিস্টেম** সমর্থনসহ।

## Zig বিল্ড

এই ফর্কটি `build.zig` এবং `build.zig.zon` যোগ করে, যাতে লাইব্রেরি, শেয়ার্ড লাইব্রেরি, CLI টুল এবং টেস্ট সবকিছু Zig বিল্ড সিস্টেম (0.15+) দিয়ে তৈরি করা যায়। এটি Zig প্রকল্পে ডিপেন্ডেন্সি হিসেবে ব্যবহার করা বা Zig সমর্থিত যেকোনো টার্গেটের জন্য ক্রস-কম্পাইল করা সহজ করে তোলে।

### Zig দিয়ে বিল্ড করা (Nix-এর মাধ্যমে)

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

### সরাসরি Nix দিয়ে বিল্ড করা

```bash
nix build
# Outputs: result/bin/uchardet, result/lib/libuchardet.{a,dylib}, result/include/uchardet/uchardet.h
```

### Zig ডিপেন্ডেন্সি হিসেবে ব্যবহার

আপনার `build.zig.zon`-এ যোগ করুন:

```zig
.dependencies = .{
    .uchardetz = .{
        .url = "https://github.com/pmarreck/uchardetz/archive/<commit>.tar.gz",
        .hash = "...",
    },
},
```

তারপর আপনার `build.zig`-এ:

```zig
const uchardetz_dep = b.dependency("uchardetz", .{
    .target = target,
    .optimize = optimize,
});
your_module.linkLibrary(uchardetz_dep.artifact("uchardet"));
```

## আউটপুট

- **`libuchardet.a`** - স্ট্যাটিক লাইব্রেরি
- **`libuchardet.dylib`** (বা `.so`) - শেয়ার্ড লাইব্রেরি
- **`uchardet`** - CLI টুল
- **`include/uchardet/uchardet.h`** - পাবলিক C হেডার

## আপস্ট্রিমের তুলনায় সংশোধনসমূহ

আপস্ট্রিম ৫টি টেস্ট "পরিচিত ত্রুটি" হিসেবে বাদ দিয়েছিল। এই ফর্ক সবগুলো ঠিক করে (৬০/৬০ পাস, ০ রিগ্রেশন)। কোনোটিই শনাক্তকরণ অ্যালগরিদমের মৌলিক সীমাবদ্ধতা ছিল না — প্রতিটির একটি নির্দিষ্ট সমাধান ছিল।

| টেস্ট | যা ফেরত দিত | মূল কারণ | সমাধান |
|------|--------------|------------|-----|
| `da/iso-8859-1` | `ISO-8859-15` | অভিন্ন আস্থা স্কোর; ISO-8859-15 prober আগে চলত, তাই সমতায় জিতত | ড্যানিশ prober ক্রম পাল্টানো হয়েছে যাতে ISO-8859-1 (পুরনো, বেশি প্রচলিত) সমতায় জেতে |
| `da/iso-8859-15` | *(বাদ দেওয়া হয়েছিল)* | বাইট `0xA4` (8859-15-তে ইউরো চিহ্ন, 8859-1-তে মুদ্রা চিহ্ন) উভয় মডেলে `SYM` হিসেবে ম্যাপ করা ছিল, তাই স্কোর একই হত | ISO-8859-1 ম্যাপে `0xA4`-কে `ILL` হিসেবে চিহ্নিত করা হয়েছে — মুদ্রা চিহ্ন (¤) বাস্তব টেক্সটে কার্যত কখনো ব্যবহার হয় না। `0xA4` দেখা গেলে 8859-1 prober এখন থেমে যায়, যাতে 8859-15 জেতে |
| `es/iso-8859-15` | `ISO-8859-1` | ড্যানিশের মতো একই `0xA4` অস্পষ্টতা | একই `ILL` সমাধান স্প্যানিশ ISO-8859-1 ম্যাপে প্রয়োগ করা হয়েছে (ফ্রেঞ্চ ও জার্মান ম্যাপেও সক্রিয়ভাবে প্রয়োগ করা হয়েছে) |
| `he/iso-8859-8` | `WINDOWS-1255` | **টেস্ট ডেটাই ভুল ছিল।** এতে যৌক্তিক ক্রমে সাজানো হিব্রু ছিল (শব্দের শেষে চূড়ান্ত-রূপের অক্ষর), যা prober সঠিকভাবে Windows-1255 হিসেবে শনাক্ত করে। ISO-8859-8 একটি *ভিজ্যুয়াল* এনকোডিং | টেস্ট ফাইল ভিজ্যুয়াল ক্রমে সাজানো হিব্রু দিয়ে প্রতিস্থাপন করা হয়েছে (লাইনের মধ্যে বাইট-বিপরীত ক্রম), যা চূড়ান্ত-অক্ষর বিশ্লেষককে স্পষ্ট ভিজ্যুয়াল-ক্রম সংকেত দেয় |
| `ja/utf-16le` | `unknown` | কোনো BOM নেই। ডিটেক্টর শুধুমাত্র BOM (`FE FF` / `FF FE`) দিয়ে UTF-16 চিনত। এটি ছাড়া ডেটা charset prober-গুলোতে পৌঁছাত যেগুলো সবই থ্রেশহোল্ডের নিচে স্কোর করত | একটি হিউরিস্টিক null-byte প্যাটার্ন ডিটেক্টর যোগ করা হয়েছে: প্রথম ২৫৬ বাইটে পর্যায়ক্রমিক null খোঁজে। জোড় অবস্থানে >২০% null এবং বিজোড়ে <৫% = UTF-16BE; উল্টোটা = UTF-16LE |
| `ja/utf-16be` | `unknown` | উপরের মতোই | একই হিউরিস্টিক, বিপরীত null-byte প্যাটার্ন |

## আপস্ট্রিম

এটি [BYVoid/uchardet](https://github.com/BYVoid/uchardet)-এর একটি ফর্ক। মূল আপস্ট্রিম প্রকল্প [freedesktop.org](https://www.freedesktop.org/wiki/Software/uchardet/)-এ স্থানান্তরিত হয়েছে।

---
