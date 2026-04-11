[English](../README.md) | [العربية](README.ar.md) | [Azərbaycan](README.az.md) | [বাংলা](README.bn.md) | [Deutsch](README.de.md) | [Ελληνικά](README.el.md) | [Español](README.es.md) | [فارسی](README.fa.md) | [Français](README.fr.md) | [עברית](README.he.md) | [हिन्दी](README.hi.md) | [Magyar](README.hu.md) | [Italiano](README.it.md) | [日本語](README.ja.md) | [ភាសាខ្មែរ](README.km.md) | [한국어](README.ko.md) | [ਪੰਜਾਬੀ](README.pa.md) | [Polski](README.pl.md) | [پښتو](README.ps.md) | [Português (BR)](README.pt_br.md) | [Română](README.ro.md) | [Русский](README.ru.md) | [Kiswahili](README.sw.md) | [தமிழ்](README.ta.md) | [ไทย](README.th.md) | [Türkçe](README.tr.md) | [Українська](README.uk.md) | [اردو](README.ur.md) | [Tiếng Việt](README.vi.md) | [简体中文](README.zh_hans.md)

# uchardetz

[uchardet](https://github.com/BYVoid/uchardet) (Mozilla'dan aktarılmış bir karakter kodlaması algılama kütüphanesi) projesinin **Zig derleme sistemi** desteğiyle oluşturulmuş bir fork'u.

## Zig ile Derleme

Bu fork, `build.zig` ve `build.zig.zon` dosyalarını ekleyerek kütüphane, paylaşımlı kütüphane, CLI aracı ve testlerin tamamının Zig derleme sistemi (0.15+) ile derlenebilmesini sağlar. Bu sayede Zig projelerinde bağımlılık olarak kullanmak veya Zig'in desteklediği herhangi bir platform için çapraz derleme yapmak kolaylaşır.

### Zig ile Derleme (Nix üzerinden)

```bash
# Dev shell'e girin (Zig 0.15 ve macOS SDK sağlar)
nix develop

# Her şeyi derleyin (statik kütüphane + paylaşımlı kütüphane + CLI)
zig build

# CLI'ı çalıştırın
zig build run -- somefile.txt
echo "some text" | ./zig-out/bin/uchardet

# Test paketini çalıştırın
zig build test
```

### Doğrudan Nix ile Derleme

```bash
nix build
# Çıktılar: result/bin/uchardet, result/lib/libuchardet.{a,dylib}, result/include/uchardet/uchardet.h
```

### Zig Bağımlılığı Olarak Kullanma

`build.zig.zon` dosyanıza ekleyin:

```zig
.dependencies = .{
    .uchardetz = .{
        .url = "https://github.com/pmarreck/uchardetz/archive/<commit>.tar.gz",
        .hash = "...",
    },
},
```

Ardından `build.zig` dosyanızda:

```zig
const uchardetz_dep = b.dependency("uchardetz", .{
    .target = target,
    .optimize = optimize,
});
your_module.linkLibrary(uchardetz_dep.artifact("uchardet"));
```

## Çıktılar

- **`libuchardet.a`** — statik kütüphane
- **`libuchardet.dylib`** (veya `.so`) — paylaşımlı kütüphane
- **`uchardet`** — CLI aracı
- **`include/uchardet/uchardet.h`** — genel C başlık dosyası

## Upstream'e göre düzeltmeler

Upstream, 5 testi "bilinen bozuk" olarak atlıyordu. Bu fork hepsini düzeltti (60/60 geçiyor, 0 regresyon). Hiçbiri algılama algoritmasının temel bir sınırlaması değildi — her birinin hedefli bir düzeltmesi vardı.

| Test | Döndürüyordu | Kök neden | Düzeltme |
|------|-------------|-----------|----------|
| `da/iso-8859-1` | `ISO-8859-15` | Aynı güven puanları; ISO-8859-15 prober önce çalışarak beraberlikleri kazanıyordu | Danca prober sırası değiştirildi, böylece ISO-8859-1 (daha eski, daha yaygın) beraberlikleri kazanıyor |
| `da/iso-8859-15` | *(atlandı)* | `0xA4` baytı (8859-15'te Euro işareti, 8859-1'de para birimi işareti) her iki modelde de `SYM` olarak eşlenmiş, bu nedenle puanlar aynıydı | ISO-8859-1 haritasında `0xA4` `ILL` olarak işaretlendi — para birimi işareti (¤) gerçek metinlerde neredeyse hiç kullanılmaz. `0xA4` göründüğünde 8859-1 prober artık devreden çıkıyor ve 8859-15'in kazanmasına izin veriyor |
| `es/iso-8859-15` | `ISO-8859-1` | Danca ile aynı `0xA4` belirsizliği | Aynı `ILL` düzeltmesi İspanyolca ISO-8859-1 haritasına uygulandı (ayrıca önlem olarak Fransızca ve Almanca haritalara da uygulandı) |
| `he/iso-8859-8` | `WINDOWS-1255` | **Test verisinin kendisi hatalıydı.** Mantıksal sıralı İbranice (kelimelerin sonunda son-biçim harfler) içeriyordu; prober bunu doğru bir şekilde Windows-1255 olarak tanımlıyor. ISO-8859-8 bir *görsel* kodlamadır | Test dosyası görsel sıralı İbranice (satırlar içinde bayt-ters diziler) ile değiştirildi ve son-harf analizörüne net bir görsel-sıra sinyali verildi |
| `ja/utf-16le` | `unknown` | BOM yok. Algılayıcı UTF-16'yı yalnızca BOM (`FE FF` / `FF FE`) ile tanıyordu. BOM olmadan veri, hepsi eşik altında puan alan charset prober'lara düşüyordu | Sezgisel null-bayt desen algılayıcısı eklendi: ilk 256 baytı değişen null'lar için tarar. Çift bayt konumlarında >%20 null ve tek konumlarda <%5 = UTF-16BE; tersi = UTF-16LE |
| `ja/utf-16be` | `unknown` | Yukarıdaki ile aynı | Aynı sezgisel yöntem, ters null-bayt deseni |

## Upstream

Bu, [BYVoid/uchardet](https://github.com/BYVoid/uchardet) projesinin bir fork'udur. Kanonik upstream projesi [freedesktop.org](https://www.freedesktop.org/wiki/Software/uchardet/) adresine taşınmıştır.

---
