[English](README.md) | [العربية](README.ar.md) | [Azərbaycan](README.az.md) | [বাংলা](README.bn.md) | [Deutsch](README.de.md) | [Ελληνικά](README.el.md) | [Español](README.es.md) | [فارسی](README.fa.md) | [Français](README.fr.md) | [עברית](README.he.md) | [हिन्दी](README.hi.md) | [Magyar](README.hu.md) | [Italiano](README.it.md) | [日本語](README.ja.md) | [ភាសាខ្មែរ](README.km.md) | [한국어](README.ko.md) | [ਪੰਜਾਬੀ](README.pa.md) | [Polski](README.pl.md) | [پښتو](README.ps.md) | [Português (BR)](README.pt_br.md) | [Română](README.ro.md) | [Русский](README.ru.md) | [Kiswahili](README.sw.md) | [தமிழ்](README.ta.md) | [ไทย](README.th.md) | [Türkçe](README.tr.md) | [Українська](README.uk.md) | [اردو](README.ur.md) | [Tiếng Việt](README.vi.md) | [简体中文](README.zh_hans.md)

# uchardetz

Un fork de [uchardet](https://github.com/BYVoid/uchardet) (une bibliothèque de détection d'encodage portée depuis Mozilla) avec prise en charge du **système de build Zig**.

## Build Zig

Ce fork ajoute `build.zig` et `build.zig.zon` afin que la bibliothèque statique, la bibliothèque partagée, l'outil en ligne de commande et les tests puissent tous être compilés avec le système de build Zig (0.15+), facilitant ainsi son utilisation comme dépendance dans des projets Zig ou la compilation croisée vers n'importe quelle cible supportée par Zig.

### Compiler avec Zig (via Nix)

```bash
# Entrer dans le shell de développement (fournit Zig 0.15 et le SDK macOS)
nix develop

# Tout compiler (bibliothèque statique + bibliothèque partagée + CLI)
zig build

# Exécuter la CLI
zig build run -- somefile.txt
echo "some text" | ./zig-out/bin/uchardet

# Lancer la suite de tests
zig build test
```

### Compiler directement avec Nix

```bash
nix build
# Produit : result/bin/uchardet, result/lib/libuchardet.{a,dylib}, result/include/uchardet/uchardet.h
```

### Utilisation comme dépendance Zig

Ajoutez à votre `build.zig.zon` :

```zig
.dependencies = .{
    .uchardetz = .{
        .url = "https://github.com/pmarreck/uchardetz/archive/<commit>.tar.gz",
        .hash = "...",
    },
},
```

Puis dans votre `build.zig` :

```zig
const uchardetz_dep = b.dependency("uchardetz", .{
    .target = target,
    .optimize = optimize,
});
your_module.linkLibrary(uchardetz_dep.artifact("uchardet"));
```

## Artefacts produits

- **`libuchardet.a`** — bibliothèque statique
- **`libuchardet.dylib`** (ou `.so`) — bibliothèque partagée
- **`uchardet`** — outil en ligne de commande
- **`include/uchardet/uchardet.h`** — en-tête C publique

## Corrections par rapport au projet amont

Le projet amont ignorait 5 tests en les marquant comme « défaillances connues ». Ce fork les corrige tous (60/60 réussis, 0 régressions). Aucun ne relevait d'une limitation fondamentale de l'algorithme de détection — chacun avait une correction ciblée.

| Test | Résultat obtenu | Cause racine | Correction |
|------|----------------|--------------|------------|
| `da/iso-8859-1` | `ISO-8859-15` | Scores de confiance identiques ; le prober ISO-8859-15 s'exécutait en premier et gagnait en cas d'égalité | Inversion de l'ordre des probers danois pour que ISO-8859-1 (plus ancien, plus courant) l'emporte en cas d'égalité |
| `da/iso-8859-15` | *(ignoré)* | L'octet `0xA4` (signe euro en 8859-15, signe monétaire en 8859-1) était mappé à `SYM` dans les deux modèles, produisant des scores identiques | `0xA4` marqué comme `ILL` dans la table ISO-8859-1 — le signe monétaire (¤) n'est pratiquement jamais utilisé en texte réel. Lorsque `0xA4` apparaît, le prober 8859-1 abandonne désormais, laissant 8859-15 gagner |
| `es/iso-8859-15` | `ISO-8859-1` | Même ambiguïté `0xA4` que pour le danois | Même correction `ILL` appliquée à la table espagnole ISO-8859-1 (également appliquée de manière préventive aux tables française et allemande) |
| `he/iso-8859-8` | `WINDOWS-1255` | **Les données de test elles-mêmes étaient erronées.** Elles contenaient de l'hébreu en ordre logique (lettres finales en fin de mot), que le prober identifie correctement comme Windows-1255. ISO-8859-8 est un encodage *visuel* | Remplacement du fichier de test par de l'hébreu en ordre visuel (séquences d'octets inversées à l'intérieur des lignes), fournissant à l'analyseur de lettres finales un signal clair d'ordre visuel |
| `ja/utf-16le` | `unknown` | Pas de BOM présent. Le détecteur ne reconnaissait UTF-16 que via le BOM (`FE FF` / `FF FE`). Sans celui-ci, les données passaient aux probers de jeux de caractères qui scoraient tous sous le seuil | Ajout d'un détecteur heuristique de motifs d'octets nuls : analyse les 256 premiers octets à la recherche de nuls alternés. >20 % de nuls aux positions paires avec <5 % aux impaires = UTF-16BE ; le schéma inverse = UTF-16LE |
| `ja/utf-16be` | `unknown` | Identique au précédent | Même heuristique, motif d'octets nuls inversé |

## Projet amont

Ceci est un fork de [BYVoid/uchardet](https://github.com/BYVoid/uchardet). Le projet amont canonique a migré vers [freedesktop.org](https://www.freedesktop.org/wiki/Software/uchardet/).

---
