{
  description = "spancopy - a CLI tool to span (copy) files with size threshold";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs =
    {
      self,
      nixpkgs,
      flake-utils,
      ...
    }@inputs:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = import nixpkgs { inherit system; };
      in
      {
        packages.default = pkgs.stdenv.mkDerivation {
          pname = "spancopy";
          version = "latest";
          src = ./.;
          nativeBuildInputs = with pkgs; [
            cmake
          ];
          cmakeFlags = [
            "-DCMAKE_BUILD_TYPE=Release"
          ];
        };
      }
    );
}
