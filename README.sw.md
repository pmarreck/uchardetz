[English](README.md) | [العربية](README.ar.md) | [Azərbaycan](README.az.md) | [বাংলা](README.bn.md) | [Deutsch](README.de.md) | [Ελληνικά](README.el.md) | [Español](README.es.md) | [فارسی](README.fa.md) | [Français](README.fr.md) | [עברית](README.he.md) | [हिन्दी](README.hi.md) | [Magyar](README.hu.md) | [Italiano](README.it.md) | [日本語](README.ja.md) | [ភាសាខ្មែរ](README.km.md) | [한국어](README.ko.md) | [ਪੰਜਾਬੀ](README.pa.md) | [Polski](README.pl.md) | [پښتو](README.ps.md) | [Português (BR)](README.pt_br.md) | [Română](README.ro.md) | [Русский](README.ru.md) | [Kiswahili](README.sw.md) | [தமிழ்](README.ta.md) | [ไทย](README.th.md) | [Türkçe](README.tr.md) | [Українська](README.uk.md) | [اردو](README.ur.md) | [Tiếng Việt](README.vi.md) | [简体中文](README.zh_hans.md)

# uchardetz

Tawi (fork) ya [uchardet](https://github.com/BYVoid/uchardet) (maktaba ya kutambua usimbaji iliyohamishwa kutoka Mozilla) yenye msaada wa **mfumo wa kujenga wa Zig**.

## Kujenga kwa Zig

Tawi hili linaongeza `build.zig` na `build.zig.zon` ili maktaba, maktaba shirikishi, zana ya CLI, na majaribio yote yajengwe kwa mfumo wa kujenga wa Zig (0.15+). Hii inafanya iwe rahisi kutumia kama tegemezi katika miradi ya Zig au kukompali-msalaba kwa lengo lolote linalosaidiwa na Zig.

### Kujenga kwa Zig (kupitia Nix)

```bash
# Ingia kwenye dev shell (hutoa Zig 0.15 na macOS SDK)
nix develop

# Jenga kila kitu (maktaba tuli + maktaba shirikishi + CLI)
zig build

# Endesha CLI
zig build run -- somefile.txt
echo "some text" | ./zig-out/bin/uchardet

# Endesha majaribio
zig build test
```

### Kujenga moja kwa moja kwa Nix

```bash
nix build
# Matokeo: result/bin/uchardet, result/lib/libuchardet.{a,dylib}, result/include/uchardet/uchardet.h
```

### Kutumia kama tegemezi ya Zig

Ongeza kwenye `build.zig.zon` yako:

```zig
.dependencies = .{
    .uchardetz = .{
        .url = "https://github.com/pmarreck/uchardetz/archive/<commit>.tar.gz",
        .hash = "...",
    },
},
```

Kisha kwenye `build.zig` yako:

```zig
const uchardetz_dep = b.dependency("uchardetz", .{
    .target = target,
    .optimize = optimize,
});
your_module.linkLibrary(uchardetz_dep.artifact("uchardet"));
```

## Matokeo

- **`libuchardet.a`** — maktaba tuli
- **`libuchardet.dylib`** (au `.so`) — maktaba shirikishi
- **`uchardet`** — zana ya CLI
- **`include/uchardet/uchardet.h`** — kichwa cha C cha umma

## Marekebisho juu ya upstream

Upstream iliruka majaribio 5 kama "yanayojulikana kuwa yameharibika." Tawi hili linarekebisha yote (60/60 yanafaulu, regresheni 0). Hakuna iliyokuwa kikomo cha msingi cha algoriti ya kutambua — kila moja ilikuwa na suluhisho mahususi.

| Jaribio | Ilikuwa ikirudisha | Chanzo cha tatizo | Suluhisho |
|---------|-------------------|-------------------|-----------|
| `da/iso-8859-1` | `ISO-8859-15` | Alama za kujiamini zinafanana; prober ya ISO-8859-15 iliendesha kwanza, ikishinda sare | Mpangilio wa prober za Kideni ulibadilishwa ili ISO-8859-1 (ya zamani zaidi, inayotumika zaidi) ishinde sare |
| `da/iso-8859-15` | *(ilirukwa)* | Baiti `0xA4` (alama ya Euro katika 8859-15, alama ya sarafu katika 8859-1) imeandikwa kama `SYM` katika miundo zote mbili, kwa hivyo alama zilifanana | `0xA4` imewekwa alama `ILL` kwenye ramani ya ISO-8859-1 — alama ya sarafu (¤) haitumiki kamwe katika maandishi halisi. `0xA4` inapoonekana, prober ya 8859-1 sasa inajiondoa, na kuruhusu 8859-15 kushinda |
| `es/iso-8859-15` | `ISO-8859-1` | Utata uo huo wa `0xA4` kama wa Kideni | Suluhisho lile lile la `ILL` limetumika kwenye ramani ya Kihispania ya ISO-8859-1 (pia limetumika kwa kuzuia kwenye ramani za Kifaransa na Kijerumani) |
| `he/iso-8859-8` | `WINDOWS-1255` | **Data ya jaribio yenyewe ilikuwa mbaya.** Ilikuwa na Kiebrania kilichopangwa kimantiki (herufi za mwisho mwishoni mwa maneno), ambayo prober inaitambua kwa usahihi kama Windows-1255. ISO-8859-8 ni usimbaji wa *kuona* | Faili ya jaribio imebadilishwa na Kiebrania kilichopangwa kwa kuona (mifuatano iliyogeuzwa kwa baiti ndani ya mistari), ikimpa kichambuzi cha herufi za mwisho ishara wazi ya mpangilio wa kuona |
| `ja/utf-16le` | `unknown` | BOM haipo. Kitambulishi kilitambua UTF-16 kupitia BOM (`FE FF` / `FF FE`) tu. Bila hiyo, data ilipita kwenye charset prober zote ambazo alama zake zote zilikuwa chini ya kizingiti | Kiongezwa kitambulishi cha heuristic cha muundo wa baiti tupu: kinachunguza baiti 256 za kwanza kwa baiti tupu zinazobadilishana. >20% baiti tupu katika nafasi za jozi na <5% katika nafasi zisizo jozi = UTF-16BE; kinyume chake = UTF-16LE |
| `ja/utf-16be` | `unknown` | Sawa na hapo juu | Heuristic ile ile, muundo wa baiti tupu wa kinyume |

## Upstream

Hii ni tawi la [BYVoid/uchardet](https://github.com/BYVoid/uchardet). Mradi rasmi wa upstream umehamia [freedesktop.org](https://www.freedesktop.org/wiki/Software/uchardet/).

---
