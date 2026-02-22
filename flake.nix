{
  description = "Cross-compiled development environment for an i686 target";

  inputs = {
    nixpkgs.url = "nixpkgs/nixos-25.11";
  };

  outputs = { nixpkgs, ... }: {

    devShells.x86_64-linux.default = let
      pkgs = import nixpkgs { system = "x86_64-linux"; };
      cross = pkgs.pkgsCross.gnu32;

      oldNcurses = cross.ncurses.overrideAttrs (old: {
        dontStrip = true;
        postInstall = ''
          cp lib/libncurses*.{so,a} $out/lib/
        '';
        configureFlags = (old.configureFlags or []) ++ [
          "--host=i686-unknown-linux-musl"
        ];
        NIX_CFLAGS_COMPILE = "-mtune=pentium3 -mno-sse";
      });

      oldOpenssl = cross.openssl.overrideAttrs (old: {
        doCheck = false;
        dontCheck = true;
        configureFlags = (old.configureFlags or []) ++ [ "no-shared" "no-module" "no-tests" ];
        NIX_CFLAGS_COMPILE = "-mtune=pentium3 -mno-sse";
      });

      oldCurl = cross.curl.overrideAttrs (old: {
          configureFlags = (old.configureFlags or []) ++ [
          "--disable-ntlm-wb"
          "--with-openssl=${oldOpenssl.dev}"
        ];
        NIX_CFLAGS_COMPILE = "-mtune=pentium3 -mno-sse";
        doCheck = false;
      });

      oldJsonCpp = cross.jsoncpp.overrideAttrs (old: {
        NIX_CFLAGS_COMPILE = "-mtune=pentium3 -mno-sse";
      });
    in pkgs.mkShell {
      keep-outputs = true;

      nativeBuildInputs = [
        pkgs.pkgsCross.gnu32.gcc
        pkgs.pkgsCross.gnu32.binutils
        pkgs.pkgsCross.gnu32.pkg-config
      ];

      buildInputs = [
        oldNcurses
        oldCurl
        oldOpenssl
        oldJsonCpp
      ];

      shellHook = ''
        export CC=x86_64-unknown-linux-musl-gcc
        export CXX=x86_64-unknown-linux-musl-g++
        export PKG_CONFIG_ALLOW_CROSS=1
      '';
    };
  };
}

