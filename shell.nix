{ pkgs ? import <nixpkgs> { } }:

let
    buildLibs = with pkgs; (with xorg; [
    ]);
in with pkgs; mkShell {
    buildInputs = [
        gcc
        gnumake
        ccls
    ] ++ buildLibs;
    shellHook = ''
        export LD_LIBRARY_PATH="$LD_LIBRARY_PATH:${lib.makeLibraryPath buildLibs}"
    '';
}

