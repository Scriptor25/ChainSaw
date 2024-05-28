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
        pkgs = nixpkgs.legacyPackages.${system};
      in {
        devShells.default = pkgs.mkShell {
          packages = with pkgs; [
            gcc
            gnumake
            cmake
            ninja
            llvmPackages_17.llvm
            llvmPackages_17.libllvm
            lldb_17
            zlib
            vector
          ];
        };
        preInstall = ''
          rm -rf build
        '';
        installPhase = ''
          runHook preInstall
          cmake -S . -B build
          cmake --build build --target csaw --config Debug
          runHook postInstall
        '';
        postInstall = ''
          cp build/csaw
        '';
      }
    );
}
