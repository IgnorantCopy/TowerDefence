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
        ];
      };
    };
}

