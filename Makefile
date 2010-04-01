# dlt ( dynamic link tracer )

VERSION = 0.0.1

# includes and libs
LIBS = -ldl

# flags
CFLAGS = -Wall -O3 ${LIBS} -DVERSION=\"${VERSION}\" -I./include

SRC = src/main.c src/module.c src/trace.c
BIN = dlt

all:
	gcc -o $(BIN) $(SRC) $(CFLAGS)
	bin/modcomp example/asm.c
	bin/modcomp example/test.c

clean:
	rm -f $(BIN)
	rm -f example/*.so

install:
	cp -f $(BIN) /usr/local/bin/
	cp -f example/hook.h /usr/local/include
