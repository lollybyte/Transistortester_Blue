{ pkgs ? import <nixpkgs> {} }:
  pkgs.mkShell {
    # nativeBuildInputs is usually what you want -- tools you need to run
    nativeBuildInputs = with pkgs.buildPackages; [
      arduino-ide
      fritzing
      kicad
      gh
    ];
  shellHook = ''
     echo Transistortester_Blue Enviornment
     export PATH="$(pwd)/bin:$PATH"
     if [ ! -f "bin/arduino-lint" ]; then
       echo "Download arduino-lint to $(pwd)/bin"
       mkdir $(pwd)/bin
       curl -fsSL https://raw.githubusercontent.com/arduino/arduino-lint/main/etc/install.sh | BINDIR=bin sh 
     fi
  '';
}
