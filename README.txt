Parallel Overhead ReadMe
------------------------
Copyright © 2020 Linus Vanas <linus@vanas.fi>
SPDX-License-Identifier: MIT
https://huitsi.net/games/parallel-overhead/

WARNING: This game features rapidly changing colors.

Parallel Overhead is an endless runner game where you take control of the
ships Truth and Beauty on a groundbreaking trip through hyperspace. A stable
hyperspace tunnel is finally being achieved with two ships locked to the
opposite walls of the tunnel. Well, almost stable...

The game features music by oglsdl from
https://opengameart.org/content/fast-pulse

Playing the game
----------------
Parallel Overhead requires the SDL2-library to be installed in the system as
well as GLES 2.0 support. The parallel_overhead executable binary is located in
the 'games'-subdirectory of the installation directory. Parallel overhead has a
few command line options, run the game with the '--help'-option to find out
more about them.

Keep the ships from falling through the holes! The ships can be rotated
clockwise with the left arrow key and anticlockwise with the right arrow key.
The game can be paused/unpaused with space or enter, restarted with backspace
and exited with escape.

The game starts paused so that you can resize the window and adjusts the
(separate) music and sound effect volumes in your OS mixer.

Building from source
--------------------
The Parallel Overhead source code can be found at:
https://codeberg.org/Huitsi/ParallelOverhead
https://github.com/Huitsi/ParallelOverhead

When cloning with git, git-lfs is needed to get the audio assets. Make sure you
have initialized it with 'git lfs install' before cloning. Alternatively, the
sound effects can be re-exported from data-src with SFXR. The music can also be
downloaded from OpenGameArt.org but will need to be converted to .wav.
(SFXR: https://www.drpetter.se/project_sfxr.html)

Building Parallel Overhead is tested on Debian Buster. The required packages
are build-essential and libsdl2-dev. With those installed Parallel Overhead can
be simply built with 'make install'. The default installation location is just
a local 'local'-directory, so no root privileges are required. I you wish to
properly install Parallel overhead into your system, run (for example)
'make install prefix=/usr/local'.
