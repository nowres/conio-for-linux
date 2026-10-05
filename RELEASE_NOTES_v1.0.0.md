## conio-for-linux 1.0.0

First stable release. A Linux implementation of the DOS/Windows `conio.h` console API, built on ncurses.

### Highlights

- **Packages:** installable `.deb` and `.rpm` files for amd64, each as a runtime package and a development package.
- **Shared library:** `libconio.so.1` (soname `libconio.so.1`), alongside the existing static build.
- **Unit tests:** 15 tests covering cursor movement, colors, windows and keyboard input. They run on a pseudo-terminal, so they need no real terminal: `make -C tests test`.
- **Cleaner build:** no deprecation warnings on current ncurses.

### Packages

| Format | Runtime | Development |
|---|---|---|
| deb | `libconio1` | `libconio-dev` |
| rpm | `libconio` | `libconio-devel` |

Install both the runtime and development package to compile against the library, then link with `-lconio -lncurses`.

### Changes

- Replace deprecated `vwprintw` and `vwscanw` with `vw_printw` and `vw_scanw`.
- Remove duplicate `init_pair` and `attron` calls in `init_screen()`.
- Remove `fflush(stdin)`, which is undefined behaviour.
- `delline()` now initializes the screen, so it is safe as the first conio call.
- `cputs` now takes `const char*`.
- Remove unused `min` and `max` macros.
- Update the README (requirements, usage, full function list including `kbhit`, testing).
- Add a `.gitignore` for build output.
- Add a GitHub Actions workflow that runs the tests and builds the packages.

### Supported functions

`gotoxy`, `clrscr`, `clreol`, `delline`, `window`, `wherex`, `wherey`, `kbhit`, `getch`, `getche`, `cgets`, `cscanf`, `cputs`, `cprintf`, `textcolor`, `textbackground`

### Known limitations

- `cprintf` returns the length of the format string rather than the number of characters printed.
- `kbhit()` reads from the main screen, not the window set by `window()`.
- Packages are built for amd64 only.

**Full changelog:** https://github.com/nowres/conio-for-linux/compare/c2ee58e...v1.0.0
