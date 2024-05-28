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
          llvmPackages_17.llvm
          llvmPackages_17.libllvm
          lldb_17
          zlib
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
          '';
          installPhase = ''
            runHook preInstall
            cmake -S . -B build
            cmake --build build --target csaw --config Debug
            mkdir $out/bin
            cp build/csaw $out/bin/
          '';
          meta = {
            description = "Chainsaw Programming Language";
          };
        };
      }
    );
}
