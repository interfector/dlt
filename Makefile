# dlt ( dynamic link tracer )

VERSION = 0.0.1

# includes and libs
LIBS = -ldl -lpthread

# flags
CFLAGS = -Wall -O3 ${LIBS} -DVERSION=\"${VERSION}\" -I./include

SRC = src/main.c src/module.c src/elf.c src/trace.c
BIN = dlt

all:
	gcc -o $(BIN) $(SRC) $(CFLAGS)

example:
	bin/modcomp example/asm.c
	bin/modcomp example/test.c

clean:
	rm -f $(BIN)

install: all
	cp -f $(BIN) /usr/local/bin/
	cp -f include/dlt.h /usr/local/include
