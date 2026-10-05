conio-for-linux
===============

Conio.h for linux 1.0

A Linux implementation of the DOS/Windows `conio.h` console I/O API, built on ncurses.

Requirements
============

* GCC and GNU Make
* ncurses development files (e.g. `libncurses-dev` on Debian/Ubuntu, `ncurses-devel` on Fedora)

Installation
============

* run `make` to build the library (output: `dist/Debug/GNU-Linux-x86/libconio.a`).

Usage
=====

* include `conio.h` in your program.
* link against `libconio.a` and ncurses, e.g. `gcc prog.c libconio.a -lncurses`.

Currently supported functions
=============================

Cursor and screen:

* gotoxy
* clrscr
* clreol
* delline
* window
* wherex
* wherey

Input:

* kbhit
* getch
* getche
* cgets
* cscanf

Output:

* cputs
* cprintf

Text attributes:

* textcolor
* textbackground

Testing
=======

* run `make -C tests test` to run the unit tests.
* each test runs in a forked child on a pseudo-terminal, so no real terminal is needed.

License
=======

This library is licensed under GNU GPLv3 License http://www.gnu.org/copyleft/gpl.html
