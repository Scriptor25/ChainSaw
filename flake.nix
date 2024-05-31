{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs = {
    self,
    nixpkgs,
    flake-utils,
  }:
    flake-utils.lib.eachDefaultSystem
    (
      system: let
        pkgs = import nixpkgs {inherit system;};
        devPackages = with pkgs; [
          gcc
          gnumake
          cmake
          ninja
          llvm_17
          zlib
          libffi
          libedit
          libz
          ncurses
          libxml2
          libpfm
          curl
        ];
      in {
        devShell = pkgs.mkShell {
          packages = devPackages;
        };

        packages.default = pkgs.stdenv.mkDerivation {
          name = "chainsaw-lang";
          src = ./.;
          buildInputs = devPackages;
          preInstall = ''
            rm -rf build
            mkdir build
          '';
          installPhase = ''
            runHook preInstall
            cmake -S . -B build
            cmake --build build --target csaw --config Debug
            mkdir $out/bin
            cp build/ChainSawCLI $out/bin/
          '';
          meta = {
            description = "Chainsaw Programming Language";
          };
        };
      }
    );
}
