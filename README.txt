Parallel Overhead ReadMe
------------------------
Copyright © 2020 Linus Vanas <linus@vanas.fi>
SPDX-License-Identifier: MIT

WARNING: This game features rapidly changing colors.

Parallel Overhead is an endless runner game, where you take control of the
ships Truth and Beauty on a groundbreaking trip through hyperspace. A stable
hyperspace tunnel is finally being achieved with two ships locked to the
opposite walls of the tunnel. Well, almost stable...

On the web
----------
https://huitsi.net/games/parallel-overhead/
https://huitsi.net:61734/Huitsi/ParallelOverhead

Gameplay
--------
The executable binary for GNU/Linux systems is called parallel_overhead.
Parallel Overhead requires the SDL2-library to be installed in the system and
GLES 3.2 support.

The ships can be rotated clockwise with the left arrow key and anticlockwise
with the right arrow key. Keep the ships from falling through the holes!

Note that the window can be resized and the music and sound effect volumes can
be adjusted separately in your OS mixer.

Building
--------
Building Parallel Overhead is tested on Debian Buster. The required packages
are git-lfs (for fetching the audio assets), build-essential and libsdl2-dev.
After installing the dependencies and cloning the repository, Parallel Overhead
can be simply built with make.
