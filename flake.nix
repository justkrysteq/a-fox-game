{
	description = "A Fox Game";

	inputs = {
		nixpkgs.url = "github:nixos/nixpkgs?ref=nixos-unstable";
	};

	outputs = { self, nixpkgs }:
	let
		pkgs = nixpkgs.legacyPackages.x86_64-linux;

		# Aliases
		m = pkgs.writeShellScriptBin "m" ''
			#!${pkgs.bash}/bin/bash
			make
		'';

		c = pkgs.writeShellScriptBin "c" ''
			#!${pkgs.bash}/bin/bash
			make clean
		'';

		run = pkgs.writeShellScriptBin "run" ''
			#!${pkgs.bash}/bin/bash
			./a-fox-game
		'';
		
		a = pkgs.writeShellScriptBin "a" ''
			#!${pkgs.bash}/bin/bash
			if [ -f ./a-fox-game ]; then
				make clean;
				make;
			else
				make;
			fi

			./a-fox-game
		'';
	in
	{
		devShells.x86_64-linux.default = pkgs.mkShell {
			buildInputs = with pkgs; [
				gcc gnumake SDL2 SDL2_ttf valgrind

				# Aliases
				m c run a
			];
		};
	};
}
