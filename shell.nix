{
  pkgs
}:
let
  x = 42;
in
  pkgs.mkShell {
    name = "Tipuino Dev Shell";
    packages = with pkgs; [
      platformio
      platformio-core
    ];
    shellHook = ''
      echo "Welcome to the Tipuino Development Shell"
    '';
  }
