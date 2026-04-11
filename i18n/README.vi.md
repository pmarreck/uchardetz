[English](../README.md) | [العربية](README.ar.md) | [Azərbaycan](README.az.md) | [বাংলা](README.bn.md) | [Deutsch](README.de.md) | [Ελληνικά](README.el.md) | [Español](README.es.md) | [فارسی](README.fa.md) | [Français](README.fr.md) | [עברית](README.he.md) | [हिन्दी](README.hi.md) | [Magyar](README.hu.md) | [Italiano](README.it.md) | [日本語](README.ja.md) | [ភាសាខ្មែរ](README.km.md) | [한국어](README.ko.md) | [ਪੰਜਾਬੀ](README.pa.md) | [Polski](README.pl.md) | [پښتو](README.ps.md) | [Português (BR)](README.pt_br.md) | [Română](README.ro.md) | [Русский](README.ru.md) | [Kiswahili](README.sw.md) | [தமிழ்](README.ta.md) | [ไทย](README.th.md) | [Türkçe](README.tr.md) | [Українська](README.uk.md) | [اردو](README.ur.md) | [Tiếng Việt](README.vi.md) | [简体中文](README.zh_hans.md)

# uchardetz

Một bản fork của [uchardet](https://github.com/BYVoid/uchardet) (thư viện nhận diện mã hóa ký tự được chuyển từ Mozilla) với hỗ trợ **hệ thống build Zig**.

## Build bằng Zig

Bản fork này bổ sung `build.zig` và `build.zig.zon` để thư viện tĩnh, thư viện chia sẻ, công cụ CLI và bộ kiểm thử đều có thể được build bằng hệ thống build Zig (0.15+). Điều này giúp dễ dàng sử dụng như một dependency trong các dự án Zig hoặc biên dịch chéo cho bất kỳ nền tảng nào mà Zig hỗ trợ.

### Build bằng Zig (thông qua Nix)

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

### Build trực tiếp bằng Nix

```bash
nix build
# Outputs: result/bin/uchardet, result/lib/libuchardet.{a,dylib}, result/include/uchardet/uchardet.h
```

### Sử dụng như dependency của Zig

Thêm vào `build.zig.zon` của bạn:

```zig
.dependencies = .{
    .uchardetz = .{
        .url = "https://github.com/pmarreck/uchardetz/archive/<commit>.tar.gz",
        .hash = "...",
    },
},
```

Sau đó trong `build.zig` của bạn:

```zig
const uchardetz_dep = b.dependency("uchardetz", .{
    .target = target,
    .optimize = optimize,
});
your_module.linkLibrary(uchardetz_dep.artifact("uchardet"));
```

## Đầu ra

- **`libuchardet.a`** -- thư viện tĩnh
- **`libuchardet.dylib`** (hoặc `.so`) -- thư viện chia sẻ
- **`uchardet`** -- công cụ CLI
- **`include/uchardet/uchardet.h`** -- header C công khai

## Sửa lỗi so với upstream

Upstream đã bỏ qua 5 bài kiểm thử với trạng thái "lỗi đã biết". Bản fork này sửa tất cả (60/60 đạt, 0 hồi quy). Không có lỗi nào là hạn chế cơ bản của thuật toán nhận diện -- mỗi lỗi đều có bản sửa nhắm mục tiêu cụ thể.

| Bài kiểm thử | Trả về | Nguyên nhân gốc | Cách sửa |
|---------------|--------|-----------------|----------|
| `da/iso-8859-1` | `ISO-8859-15` | Điểm tin cậy giống hệt nhau; prober ISO-8859-15 chạy trước nên thắng khi hòa | Đổi thứ tự prober tiếng Đan Mạch để ISO-8859-1 (cũ hơn, phổ biến hơn) thắng khi hòa |
| `da/iso-8859-15` | *(bỏ qua)* | Byte `0xA4` (ký hiệu Euro trong 8859-15, ký hiệu tiền tệ trong 8859-1) được ánh xạ thành `SYM` trong cả hai mô hình nên điểm số giống nhau | Đánh dấu `0xA4` là `ILL` trong bảng ánh xạ ISO-8859-1 -- ký hiệu tiền tệ (¤) hầu như không bao giờ xuất hiện trong văn bản thực. Khi gặp `0xA4`, prober 8859-1 sẽ bỏ cuộc, nhường cho 8859-15 thắng |
| `es/iso-8859-15` | `ISO-8859-1` | Cùng sự nhập nhằng `0xA4` như tiếng Đan Mạch | Cùng bản sửa `ILL` áp dụng cho bảng ánh xạ ISO-8859-1 tiếng Tây Ban Nha (cũng áp dụng chủ động cho bảng tiếng Pháp và tiếng Đức) |
| `he/iso-8859-8` | `WINDOWS-1255` | **Chính dữ liệu kiểm thử bị sai.** Nó chứa tiếng Do Thái sắp xếp logic (chữ cái dạng cuối ở cuối từ), mà prober nhận diện đúng là Windows-1255. ISO-8859-8 là mã hóa *trực quan* | Thay thế tệp kiểm thử bằng tiếng Do Thái sắp xếp trực quan (các chuỗi byte đảo ngược trong từng dòng), cho bộ phân tích chữ cái cuối tín hiệu rõ ràng về thứ tự trực quan |
| `ja/utf-16le` | `unknown` | Không có BOM. Bộ nhận diện chỉ nhận ra UTF-16 qua BOM (`FE FF` / `FF FE`). Không có BOM, dữ liệu rơi xuống các charset prober và tất cả đều cho điểm dưới ngưỡng | Thêm bộ phát hiện mẫu byte null theo phương pháp heuristic: quét 256 byte đầu tiên tìm các byte null xen kẽ. >20% null ở vị trí byte chẵn với <5% ở lẻ = UTF-16BE; ngược lại = UTF-16LE |
| `ja/utf-16be` | `unknown` | Giống như trên | Cùng heuristic, mẫu byte null ngược lại |

## Upstream

Đây là bản fork của [BYVoid/uchardet](https://github.com/BYVoid/uchardet). Dự án upstream chính thức đã chuyển sang [freedesktop.org](https://www.freedesktop.org/wiki/Software/uchardet/).

---
