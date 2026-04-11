[English](../README.md) | [العربية](README.ar.md) | [Azərbaycan](README.az.md) | [বাংলা](README.bn.md) | [Deutsch](README.de.md) | [Ελληνικά](README.el.md) | [Español](README.es.md) | [فارسی](README.fa.md) | [Français](README.fr.md) | [עברית](README.he.md) | [हिन्दी](README.hi.md) | [Magyar](README.hu.md) | [Italiano](README.it.md) | [日本語](README.ja.md) | [ភាសាខ្មែរ](README.km.md) | [한국어](README.ko.md) | [ਪੰਜਾਬੀ](README.pa.md) | [Polski](README.pl.md) | [پښتو](README.ps.md) | [Português (BR)](README.pt_br.md) | [Română](README.ro.md) | [Русский](README.ru.md) | [Kiswahili](README.sw.md) | [தமிழ்](README.ta.md) | [ไทย](README.th.md) | [Türkçe](README.tr.md) | [Українська](README.uk.md) | [اردو](README.ur.md) | [Tiếng Việt](README.vi.md) | [简体中文](README.zh_hans.md)

# uchardetz

Fork ของ [uchardet](https://github.com/BYVoid/uchardet) (ไลบรารีตรวจจับการเข้ารหัสอักขระที่พอร์ตมาจาก Mozilla) พร้อมรองรับ**ระบบบิลด์ Zig**

## การบิลด์ด้วย Zig

Fork นี้เพิ่มไฟล์ `build.zig` และ `build.zig.zon` เพื่อให้สามารถบิลด์ไลบรารี, ไลบรารีแบบ shared, เครื่องมือ CLI และชุดทดสอบทั้งหมดด้วยระบบบิลด์ของ Zig (0.15+) ทำให้ใช้เป็น dependency ในโปรเจกต์ Zig หรือ cross-compile ไปยังทุกแพลตฟอร์มที่ Zig รองรับได้อย่างสะดวก

### การบิลด์ด้วย Zig (ผ่าน Nix)

```bash
# เข้า dev shell (ให้ Zig 0.15 และ macOS SDK)
nix develop

# บิลด์ทุกอย่าง (ไลบรารีแบบ static + shared + CLI)
zig build

# รัน CLI
zig build run -- somefile.txt
echo "some text" | ./zig-out/bin/uchardet

# รันชุดทดสอบ
zig build test
```

### การบิลด์ด้วย Nix โดยตรง

```bash
nix build
# ผลลัพธ์: result/bin/uchardet, result/lib/libuchardet.{a,dylib}, result/include/uchardet/uchardet.h
```

### การใช้เป็น dependency ของ Zig

เพิ่มในไฟล์ `build.zig.zon` ของคุณ:

```zig
.dependencies = .{
    .uchardetz = .{
        .url = "https://github.com/pmarreck/uchardetz/archive/<commit>.tar.gz",
        .hash = "...",
    },
},
```

จากนั้นในไฟล์ `build.zig` ของคุณ:

```zig
const uchardetz_dep = b.dependency("uchardetz", .{
    .target = target,
    .optimize = optimize,
});
your_module.linkLibrary(uchardetz_dep.artifact("uchardet"));
```

## ผลลัพธ์

- **`libuchardet.a`** — ไลบรารีแบบ static
- **`libuchardet.dylib`** (หรือ `.so`) — ไลบรารีแบบ shared
- **`uchardet`** — เครื่องมือ CLI
- **`include/uchardet/uchardet.h`** — ไฟล์ header C สาธารณะ

## การแก้ไขเหนือ upstream

Upstream ข้ามการทดสอบ 5 รายการโดยถือว่า "เป็นปัญหาที่ทราบอยู่แล้ว" Fork นี้แก้ไขทั้งหมด (ผ่าน 60/60, ไม่มี regression) ไม่มีรายการใดที่เป็นข้อจำกัดพื้นฐานของอัลกอริทึมตรวจจับ — แต่ละรายการมีการแก้ไขเฉพาะจุด

| การทดสอบ | ค่าที่คืน | สาเหตุ | การแก้ไข |
|----------|----------|--------|----------|
| `da/iso-8859-1` | `ISO-8859-15` | คะแนนความมั่นใจเท่ากัน; prober ของ ISO-8859-15 ทำงานก่อนจึงชนะเมื่อคะแนนเท่ากัน | สลับลำดับ prober ของภาษาเดนมาร์กเพื่อให้ ISO-8859-1 (เก่ากว่า, ใช้กันมากกว่า) ชนะเมื่อคะแนนเท่ากัน |
| `da/iso-8859-15` | *(ข้ามไป)* | ไบต์ `0xA4` (เครื่องหมายยูโรใน 8859-15, เครื่องหมายสกุลเงินใน 8859-1) ถูกแมปเป็น `SYM` ในทั้งสองโมเดล ทำให้คะแนนเท่ากัน | ทำเครื่องหมาย `0xA4` เป็น `ILL` ในแผนที่ ISO-8859-1 — เครื่องหมายสกุลเงิน (¤) แทบไม่มีการใช้ในข้อความจริง เมื่อพบ `0xA4` prober ของ 8859-1 จะถอนตัว ทำให้ 8859-15 ชนะ |
| `es/iso-8859-15` | `ISO-8859-1` | ความกำกวมของ `0xA4` เดียวกับภาษาเดนมาร์ก | ใช้การแก้ไข `ILL` เดียวกันกับแผนที่ ISO-8859-1 ของภาษาสเปน (รวมถึงใช้เชิงป้องกันกับแผนที่ภาษาฝรั่งเศสและเยอรมันด้วย) |
| `he/iso-8859-8` | `WINDOWS-1255` | **ข้อมูลทดสอบนั้นผิดเอง** มันมีภาษาฮีบรูแบบเรียงตามตรรกะ (ตัวอักษรรูปแบบท้ายคำอยู่ที่ปลายคำ) ซึ่ง prober ระบุได้ถูกต้องว่าเป็น Windows-1255 เพราะ ISO-8859-8 เป็นการเข้ารหัสแบบ *visual* | แทนที่ไฟล์ทดสอบด้วยภาษาฮีบรูแบบเรียงตาม visual order (ลำดับไบต์กลับด้านภายในบรรทัด) ทำให้ตัววิเคราะห์ตัวอักษรท้ายคำเห็นสัญญาณ visual order ชัดเจน |
| `ja/utf-16le` | `unknown` | ไม่มี BOM ตัวตรวจจับจำ UTF-16 ได้เฉพาะผ่าน BOM (`FE FF` / `FF FE`) เท่านั้น หากไม่มี ข้อมูลจะตกไปยัง charset prober ทุกตัวซึ่งทุกตัวให้คะแนนต่ำกว่าเกณฑ์ | เพิ่มตัวตรวจจับรูปแบบ null-byte แบบ heuristic: สแกน 256 ไบต์แรกหา null สลับตำแหน่ง >20% null ที่ตำแหน่งคู่และ <5% ที่ตำแหน่งคี่ = UTF-16BE; กลับกัน = UTF-16LE |
| `ja/utf-16be` | `unknown` | เหมือนข้างต้น | heuristic เดียวกัน, รูปแบบ null-byte ตรงข้าม |

## Upstream

นี่คือ fork ของ [BYVoid/uchardet](https://github.com/BYVoid/uchardet) โปรเจกต์ upstream อย่างเป็นทางการได้ย้ายไปที่ [freedesktop.org](https://www.freedesktop.org/wiki/Software/uchardet/)

---
