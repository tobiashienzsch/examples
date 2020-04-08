.PHONY: all

SOURCE_FILES=main.c
BINARY_NAME=example-wayland

all:
	$(CC) -o $(BINARY_NAME) $(SOURCE_FILES) -lwayland-client -lGLESv2 -lEGL -lwayland-egl

clean:
	rm $(BINARY_NAME)
