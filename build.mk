# Standalone shared-library build (used by packaging/CI): make -f build.mk
CC ?= gcc
CFLAGS ?= -O2 -Wall
VERSION = 1.0.0
SOMAJOR = 1

all: dist/libconio.so.$(VERSION)

dist/libconio.so.$(VERSION): conio.c conio.h
	mkdir -p dist
	$(CC) $(CFLAGS) -fPIC -shared -Wl,-soname,libconio.so.$(SOMAJOR) conio.c -o $@ -lncurses
	ln -sf libconio.so.$(VERSION) dist/libconio.so.$(SOMAJOR)
	ln -sf libconio.so.$(SOMAJOR) dist/libconio.so

clean:
	rm -rf dist

.PHONY: all clean
