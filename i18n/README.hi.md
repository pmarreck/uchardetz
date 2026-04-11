[English](../README.md) | [العربية](README.ar.md) | [Azərbaycan](README.az.md) | [বাংলা](README.bn.md) | [Deutsch](README.de.md) | [Ελληνικά](README.el.md) | [Español](README.es.md) | [فارسی](README.fa.md) | [Français](README.fr.md) | [עברית](README.he.md) | [हिन्दी](README.hi.md) | [Magyar](README.hu.md) | [Italiano](README.it.md) | [日本語](README.ja.md) | [ភាសាខ្មែរ](README.km.md) | [한국어](README.ko.md) | [ਪੰਜਾਬੀ](README.pa.md) | [Polski](README.pl.md) | [پښتو](README.ps.md) | [Português (BR)](README.pt_br.md) | [Română](README.ro.md) | [Русский](README.ru.md) | [Kiswahili](README.sw.md) | [தமிழ்](README.ta.md) | [ไทย](README.th.md) | [Türkçe](README.tr.md) | [Українська](README.uk.md) | [اردو](README.ur.md) | [Tiếng Việt](README.vi.md) | [简体中文](README.zh_hans.md)

# uchardetz

[uchardet](https://github.com/BYVoid/uchardet) का एक फ़ोर्क (Mozilla से पोर्ट की गई एक एन्कोडिंग डिटेक्शन लाइब्रेरी) जिसमें **Zig बिल्ड सिस्टम** का समर्थन जोड़ा गया है।

## Zig बिल्ड

यह फ़ोर्क `build.zig` और `build.zig.zon` जोड़ता है ताकि स्टैटिक लाइब्रेरी, शेयर्ड लाइब्रेरी, CLI टूल और टेस्ट सभी Zig बिल्ड सिस्टम (0.15+) से बनाए जा सकें। इससे इसे Zig प्रोजेक्ट्स में डिपेंडेंसी के रूप में उपयोग करना या Zig द्वारा समर्थित किसी भी लक्ष्य के लिए क्रॉस-कम्पाइल करना आसान हो जाता है।

### Zig से बिल्ड करना (Nix के माध्यम से)

```bash
# डेवलपमेंट शेल में प्रवेश करें (Zig 0.15 और macOS SDK प्रदान करता है)
nix develop

# सब कुछ बिल्ड करें (स्टैटिक लाइब्रेरी + शेयर्ड लाइब्रेरी + CLI)
zig build

# CLI चलाएँ
zig build run -- somefile.txt
echo "some text" | ./zig-out/bin/uchardet

# टेस्ट सूट चलाएँ
zig build test
```

### सीधे Nix से बिल्ड करना

```bash
nix build
# आउटपुट: result/bin/uchardet, result/lib/libuchardet.{a,dylib}, result/include/uchardet/uchardet.h
```

### Zig डिपेंडेंसी के रूप में उपयोग

अपनी `build.zig.zon` फ़ाइल में जोड़ें:

```zig
.dependencies = .{
    .uchardetz = .{
        .url = "https://github.com/pmarreck/uchardetz/archive/<commit>.tar.gz",
        .hash = "...",
    },
},
```

फिर अपनी `build.zig` में:

```zig
const uchardetz_dep = b.dependency("uchardetz", .{
    .target = target,
    .optimize = optimize,
});
your_module.linkLibrary(uchardetz_dep.artifact("uchardet"));
```

## आउटपुट

- **`libuchardet.a`** — स्टैटिक लाइब्रेरी
- **`libuchardet.dylib`** (या `.so`) — शेयर्ड लाइब्रेरी
- **`uchardet`** — CLI टूल
- **`include/uchardet/uchardet.h`** — सार्वजनिक C हेडर

## मूल प्रोजेक्ट की तुलना में सुधार

मूल प्रोजेक्ट ने 5 टेस्ट को "ज्ञात दोष" के रूप में छोड़ दिया था। यह फ़ोर्क उन सभी को ठीक करता है (60/60 पास, 0 रिग्रेशन)। कोई भी डिटेक्शन एल्गोरिदम की मूलभूत सीमा नहीं थी — हर एक का एक लक्षित समाधान था।

| टेस्ट | पहले का परिणाम | मूल कारण | सुधार |
|-------|---------------|----------|-------|
| `da/iso-8859-1` | `ISO-8859-15` | समान कॉन्फ़िडेंस स्कोर; ISO-8859-15 prober पहले चलता था और बराबरी में जीतता था | डेनिश probers का क्रम बदला गया ताकि ISO-8859-1 (पुराना, अधिक प्रचलित) बराबरी में जीते |
| `da/iso-8859-15` | *(छोड़ा गया)* | बाइट `0xA4` (8859-15 में यूरो चिह्न, 8859-1 में मुद्रा चिह्न) दोनों मॉडलों में `SYM` के रूप में मैप था, इसलिए समान स्कोर आते थे | ISO-8859-1 मैप में `0xA4` को `ILL` के रूप में चिह्नित किया गया — मुद्रा चिह्न (¤) वास्तविक टेक्स्ट में व्यावहारिक रूप से कभी उपयोग नहीं होता। जब `0xA4` दिखाई देता है, तो 8859-1 prober अब हार मान लेता है, जिससे 8859-15 जीतता है |
| `es/iso-8859-15` | `ISO-8859-1` | डेनिश जैसी ही `0xA4` अस्पष्टता | वही `ILL` सुधार स्पेनिश ISO-8859-1 मैप पर लागू किया गया (फ़्रेंच और जर्मन मैप पर भी सक्रिय रूप से लागू किया गया) |
| `he/iso-8859-8` | `WINDOWS-1255` | **टेस्ट डेटा स्वयं गलत था।** इसमें तार्किक क्रम वाला हिब्रू था (शब्दों के अंत में अंतिम-रूप अक्षर), जिसे prober सही ढंग से Windows-1255 के रूप में पहचानता है। ISO-8859-8 एक *विज़ुअल* एन्कोडिंग है | टेस्ट फ़ाइल को विज़ुअल क्रम वाले हिब्रू से बदला गया (पंक्तियों के भीतर बाइट-क्रम उलटा), जिससे अंतिम-अक्षर विश्लेषक को स्पष्ट विज़ुअल-क्रम संकेत मिलता है |
| `ja/utf-16le` | `unknown` | BOM मौजूद नहीं था। डिटेक्टर केवल BOM (`FE FF` / `FF FE`) के माध्यम से UTF-16 पहचानता था। इसके बिना, डेटा charset probers तक पहुँचता था जो सभी थ्रेशोल्ड से नीचे स्कोर करते थे | एक ह्यूरिस्टिक नल-बाइट पैटर्न डिटेक्टर जोड़ा गया: पहले 256 बाइट्स में बारी-बारी से आने वाले नल्स की जाँच करता है। सम स्थानों पर >20% नल्स और विषम पर <5% = UTF-16BE; उलटा पैटर्न = UTF-16LE |
| `ja/utf-16be` | `unknown` | ऊपर जैसा ही | वही ह्यूरिस्टिक, विपरीत नल-बाइट पैटर्न |

## मूल प्रोजेक्ट

यह [BYVoid/uchardet](https://github.com/BYVoid/uchardet) का एक फ़ोर्क है। मूल प्रामाणिक प्रोजेक्ट [freedesktop.org](https://www.freedesktop.org/wiki/Software/uchardet/) पर स्थानांतरित हो गया है।

---
