with import <nixpkgs> {};
pkgs.mkShell {
  nativeBuildInputs = with pkgs; [ 
    cmake
    gcc
    gdb
    pkg-config

    alsa-lib
    libGL
    libllvm
  ];

  NIX_LD_LIBRARY_PATH = lib.makeLibraryPath [
    stdenv.cc.cc
    alsa-lib
    libGL
  ];

  NIX_LD = lib.fileContents "${stdenv.cc}/nix-support/dynamic-linker";
}