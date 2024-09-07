#!/bin/sh

set -xe

CFLAGS="-Wall -Wextra -Werror -pedantic -std=c17"
CC=clang
FILES="src/main.c src/window.c src/log.c src/config.c src/message.c"

clang-format -i $FILES --style="{BasedOnStyle: llvm, IndentWidth: 4, ColumnLimit: 200}"
$CC $CFLAGS $(pkg-config --cflags gtk4-layer-shell-0) $(pkg-config --cflags gtk4) \
	-o bow $FILES $(pkg-config --libs gtk4-layer-shell-0) $(pkg-config --libs gtk4)

