{
  description = "uchardetz - uchardet with Zig build system support";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        zig = pkgs.zig_0_15;

        # Build for a specific Zig target triple.
        # When target is null, build natively (no -Dtarget flag).
        mkUchardet = { target ? null, suffix ? "" }: pkgs.stdenvNoCC.mkDerivation {
          pname = "uchardetz${suffix}";
          version = "0.0.6";
          src = ./.;

          nativeBuildInputs = [ zig ];

          dontConfigure = true;
          dontFixup = true;

          buildPhase = ''
            export XDG_CACHE_HOME=$(mktemp -d)
            zig build --release=fast --prefix $out \
              ${if target != null then "-Dtarget=${target}" else ""}
          '';

          installPhase = "true";
        };

        # 5 cross-compilation targets:
        # macOS aarch64, Linux aarch64/x86_64, Windows aarch64/x86_64
        crossTargets = {
          aarch64-macos   = { target = "aarch64-macos";        suffix = "-aarch64-macos"; };
          aarch64-linux   = { target = "aarch64-linux-musl";   suffix = "-aarch64-linux"; };
          x86_64-linux    = { target = "x86_64-linux-musl";    suffix = "-x86_64-linux"; };
          aarch64-windows = { target = "aarch64-windows-gnu";  suffix = "-aarch64-windows"; };
          x86_64-windows  = { target = "x86_64-windows-gnu";  suffix = "-x86_64-windows"; };
        };
      in
      {
        # Native build (default)
        packages.default = mkUchardet {};

        # Cross-compiled builds for all 5 targets
        packages.aarch64-macos   = mkUchardet crossTargets.aarch64-macos;
        packages.aarch64-linux   = mkUchardet crossTargets.aarch64-linux;
        packages.x86_64-linux    = mkUchardet crossTargets.x86_64-linux;
        packages.aarch64-windows = mkUchardet crossTargets.aarch64-windows;
        packages.x86_64-windows  = mkUchardet crossTargets.x86_64-windows;

        devShells.default = pkgs.mkShell {
          nativeBuildInputs = [ zig ];
        };
      }
    );
}
