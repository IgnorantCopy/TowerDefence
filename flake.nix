{
  inputs = { nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable"; };

  outputs = { self, nixpkgs, ... }@inputs:
    let
      pkgs = nixpkgs.legacyPackages."x86_64-linux";
      build-deps = with pkgs; [ cmake pkg-config python3 ];
      deps = with pkgs; [
        sqlite
        fontconfig
        gtk3
        expat
        glew
        curl
        sqlite
        xorg.libXxf86vm
        pcre2
        mount
      ];
      dev-deps = with pkgs; [
        (llvmPackages_19.clang-tools.override {
          clang = clang_19.override {
            cc = gcc14.cc;
            useCcForLibs = true;
          };
          # enableLibcxx = true;
        })
        just
        gdb
      ];
    in {
      devShells.x86_64-linux.default =
        pkgs.mkShell.override { stdenv = pkgs.gcc14Stdenv; } {
          packages = build-deps ++ dev-deps ++ deps;
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

        tower-defence = pkgs.gcc14Stdenv.mkDerivation rec {
          name = "TowerDefence";
          version = "0.1.0";
          src = ./.;
          buildInputs = deps;
          nativeBuildInputs = build-deps ++ (with pkgs; [ patchelf ]);
          enableParallelBuilding = true;
          doCheck = false;
          installPhase = ''
            cp -r bin/TowerDefence/ $out/
            cp ../cocos2d/external/linux-specific/fmod/prebuilt/64-bit/libfmod.so $out/libfmod.so.6
            patchelf --add-rpath $out $out/TowerDefence
          '';
          cmakeFlags =
            [ "-DCMAKE_SKIP_BUILD_RPATH=ON" "-DCMAKE_SKIP_INSTALL_RPATH=OFF" ];
        };
      };

      apps.x86_64-linux.tower-defence = {
        type = "app";
        program = "${self.packages.x86_64-linux.tower-defence}/TowerDefence";
      };
    };
}

