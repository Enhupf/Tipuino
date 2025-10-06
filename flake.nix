{
  description = "A very basic flake";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-25.05";
    flake-parts.url = "github:hercules-ci/flake-parts";
  };

  outputs =
    inputs@{ self, flake-parts, ... }: flake-parts.lib.mkFlake { inherit inputs; } (top:
    {
      systems = [ "x86_64-linux" ];
      perSystem = { pkgs, ... }:
        let
          scope = pkgs.newScope { };
        in
        {
          devShells = {
            default = scope ./shell.nix {};
          };
        }
      ;
    }
  );
}
