# Variables
CFLAGS       := -g -Wall -Wextra -Werror -std=c17
CC           := clang
FILES        := src/main.c src/window.c src/log.c src/config.c src/message.c
FORMAT_STYLE := "{BasedOnStyle: llvm, IndentWidth: 4, ColumnLimit: 200}"
OUTPUT       := bow
EXTERNAL_DIR := external

# External dependency versions
GTK4_VERSION := 4.10.4
LAYER_SHELL_VERSION := 1.0.4

# Static build flags
STATIC_CFLAGS := -I$(EXTERNAL_DIR)/include
STATIC_LIBS   := $(EXTERNAL_DIR)/lib/libgtk-4.a \
                 $(EXTERNAL_DIR)/lib/libgtk4-layer-shell.a \
                 $(EXTERNAL_DIR)/lib/libglib-2.0.a \
                 $(EXTERNAL_DIR)/lib/libgobject-2.0.a \
                 $(EXTERNAL_DIR)/lib/libpango-1.0.a \
                 $(EXTERNAL_DIR)/lib/libcairo.a

# Targets
.PHONY: all format clean deps

all: deps format $(OUTPUT)

deps: | $(EXTERNAL_DIR)
	@echo "Downloading and building dependencies..."
	mkdir -p $(EXTERNAL_DIR)/src

	# Download GTK4
	cd $(EXTERNAL_DIR)/src && \
	wget https://download.gnome.org/sources/gtk/4.10/gtk-$(GTK4_VERSION).tar.xz && \
	tar xf gtk-$(GTK4_VERSION).tar.xz && \
	cd gtk-$(GTK4_VERSION) && \
	meson setup --prefix=$(CURDIR)/$(EXTERNAL_DIR) --default-library=static build/ && \
	ninja -C build/ install

	# Download gtk4-layer-shell
	cd $(EXTERNAL_DIR)/src && \
	wget https://github.com/wmww/gtk4-layer-shell/archive/refs/tags/v$(LAYER_SHELL_VERSION).tar.gz && \
	tar xf v$(LAYER_SHELL_VERSION).tar.gz && \
	cd gtk4-layer-shell-$(LAYER_SHELL_VERSION) && \
	meson setup --prefix=$(CURDIR)/$(EXTERNAL_DIR) --default-library=static build/ && \
	ninja -C build/ install

$(EXTERNAL_DIR):
	mkdir -p $(EXTERNAL_DIR)

format:
	clang-format -i $(FILES) --style=$(FORMAT_STYLE)

$(OUTPUT): $(FILES)
	$(CC) $(CFLAGS) $(STATIC_CFLAGS) -o $(OUTPUT) $(FILES) $(STATIC_LIBS) -lX11 -lwayland-client -lxkbcommon

clean:
	rm -f $(OUTPUT)
	rm -rf $(EXTERNAL_DIR)
