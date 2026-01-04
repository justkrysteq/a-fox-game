COMPILER = gcc
# CFLAGS = -c -Wall -Wextra -Wpedantic -Werror -Wstrict-overflow=5 -Wimplicit-fallthrough=3 -Warray-bounds=2 -Wstringop-overflow=4 -Wformat=2 -Wformat-security -D_FORTIFY_SOURCE=3 -fstack-protector-strong -fstack-clash-protection -fPIE -pie -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack -fno-strict-overflow -fno-strict-aliasing -fsanitize=address -fsanitize=undefined -fsanitize=leak -fno-omit-frame-pointer -Wstrict-prototypes -Wold-style-definition
# LDFLAGS = -lasan -lubsan -lSDL2
#
# g++ -O2 -o main main.cpp -lm -lSDL2 -lpthread -ldl -lrt
CFLAGS = -c `sdl2-config --cflags --libs` -Wall -Wextra -Wpedantic -Werror
LDFLAGS = -lSDL2 -lSDL2_ttf

SRCS = main.c $(wildcard modules/*.c)
OBJS = $(SRCS:.c=.o)

all: beat-em-up

beat-em-up: ${OBJS}
	${COMPILER} -o $@ $^ ${LDFLAGS}
	rm -f ${OBJS}

main.o: main.c
	${COMPILER} ${CFLAGS} main.c

clean:
	rm -f $(OBJS) beat-em-up
