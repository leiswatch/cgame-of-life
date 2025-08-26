CC=cc
CODEDIRS=. lib
INCDIRS=./raylib/include
LIBDIRS=./raylib/lib
LIBS=:libraylib.a m

OPTS=-O3
DEPFLAGS=-MP -MD
CFLAGS=-Wall -Wextra $(OPTS) $(foreach INCDIR,$(INCDIRS),-I$(INCDIR)) $(foreach LIBDIR,$(LIBDIRS),-L$(LIBDIR)) $(foreach LIB,$(LIBS),-l$(LIB))

CFILES=$(foreach DIR,$(CODEDIRS),$(wildcard $(DIR)/*.c))
OBJECTS=$(patsubst %.c,%.o,$(CFILES))
DEPFILES=$(patsubst %.c,%.d,$(CFILES))

BINARY=main

.PHONY: all clean test build

all: $(BINARY)

$(BINARY): $(OBJECTS)
	$(CC) $^ $(CFLAGS) -o $@

%.o: %.c
	$(CC) $< -c $(CFLAGS) $(DEPFLAGS) -o $@

test:
	@echo "test"

clean:
	@rm -rf $(BINARY) $(OBJECTS) $(DEPFILES)

-include $(DEPFILES)
