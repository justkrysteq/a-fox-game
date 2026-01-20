COMPILER = gcc

# CFLAGS = -c `sdl2-config --cflags --libs` -Wall -Wextra -Wpedantic -Wstrict-overflow=5 -Wimplicit-fallthrough=3 -Warray-bounds=2 -Wstringop-overflow=4 -Wformat=2 -fPIE -pie -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack -fno-strict-overflow -fno-strict-aliasing -fno-omit-frame-pointer -Wstrict-prototypes -Wold-style-definition -g -O0
# CFLAGS = -c `sdl2-config --cflags --libs` -O2
CFLAGS = -c `sdl2-config --cflags --libs` -Wall -Wextra -Wpedantic -Werror -Wstrict-overflow=5 -Wimplicit-fallthrough=3 -Warray-bounds=2 -Wstringop-overflow=4 -Wformat=2 -Wformat-security -D_FORTIFY_SOURCE=3 -fstack-protector-strong -fstack-clash-protection -fPIE -pie -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack -fno-strict-overflow -fno-strict-aliasing -fno-omit-frame-pointer -Wstrict-prototypes -Wold-style-definition
LDFLAGS = -lSDL2 -lSDL2_ttf

SRCS = main.c $(wildcard modules/*.c)
OBJS = $(SRCS:.c=.o)

all: a-fox-game

a-fox-game: ${OBJS}
	${COMPILER} -o $@ $^ ${LDFLAGS}
	rm -f ${OBJS}

main.o: main.c
	${COMPILER} ${CFLAGS} main.c

clean:
	rm -f $(OBJS) a-fox-game
