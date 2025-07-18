with import <nixpkgs> {};
pkgs.mkShell {
  nativeBuildInputs = with pkgs; [ 
    cmake
    gcc
    gdb
    pkg-config

    alsa-lib
    libGL
    dbus
    SDL2
    xorg.libX11
    xorg.libXrandr
    xorg.libXinerama
    xorg.libXcursor
    xorg.libXi
    xorg.libXScrnSaver
    xssstate
    libxkbcommon
    curl
    libllvm
  ];

  NIX_LD_LIBRARY_PATH = lib.makeLibraryPath [
    stdenv.cc.cc
    alsa-lib
    xorg.libXrandr
    xorg.libXinerama
    xorg.libXcursor
    xorg.libXi
    libxkbcommon
    libGL
  ];

  NIX_LD = lib.fileContents "${stdenv.cc}/nix-support/dynamic-linker";
}