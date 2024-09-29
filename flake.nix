{
  inputs = { nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable"; };

  outputs = { self, nixpkgs, ... }@inputs:
    let pkgs = nixpkgs.legacyPackages."x86_64-linux";
    in {
      devShells.x86_64-linux.default = pkgs.mkShell {
        packages = with pkgs; [
          cmake
          clang-tools
          sqlite
          fontconfig
          gtk3
          expat
          glew
          curl
          sqlite
          xorg.libXxf86vm
          just
        ];
      };

      packages.x86_64-linux = {
        doc = let
          docs = [
            "introduction"
            "branch"
            "enemy"
            "level-1"
            "level-2"
            "level-3"
            "logic"
            "tower"
          ];
          tex = pkgs.texlive.combine {
            inherit (pkgs.texlive) scheme-minimal latex-bin latexmk ctex;
          };
        in pkgs.stdenvNoCC.mkDerivation rec {
          name = "towerdefence-doc";
          src = ./docs;
          buildInputs = [ pkgs.coreutils tex ];
          phases = [ "unpackPhase" "buildPhase" "installPhase" ];
          buildPhase = ''
            export PATH="${pkgs.lib.makeBinPath buildInputs}";
            export XDG_CACHE_HOME="$(mktemp -d)";
            mkdir -p .cache/texmf-var
          '' + pkgs.lib.strings.concatMapStrings (d: ''
            env TEXMFHOME=.cache TEXMFVAR=.cache/texmf-var \
                latexmk -interaction=nonstopmode -pdfxe \
                ${d}.tex
          '') docs;
          installPhase = ''
            mkdir -p $out
          '' + pkgs.lib.concatMapStrings (d: ''
            cp ${d}.pdf $out/
          '') docs;
        };
      };
    };
}

