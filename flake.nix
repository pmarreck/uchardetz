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
      in
      {
        packages.default = pkgs.stdenvNoCC.mkDerivation {
          pname = "uchardetz";
          version = "0.0.6";
          src = ./.;

          nativeBuildInputs = [ zig ];

          dontConfigure = true;
          dontFixup = true;

          buildPhase = ''
            export XDG_CACHE_HOME=$(mktemp -d)
            zig build --release=fast --prefix $out
          '';

          installPhase = "true"; # zig build --prefix handles this
        };

        devShells.default = pkgs.mkShell {
          nativeBuildInputs = [ zig ];
        };
      }
    );
}
