CC=gcc
LD=gcc
GDWARF=-gdwarf-2
RM=rm
MKDIR=mkdir

BIN_PATH=./bin/
BIN=server client
BINS=$(addprefix $(BIN_PATH)/, $(BIN))
OBJECTS=$(foreach b, $(BIN), $(SRC_PATH)/$(b)/main.o)

UTIL_CPP ?= $(wildcard $(SRC_PATH)/util/*.c)
UTIL=$(patsubst %.c, %.o, $(UTIL_CPP))

SRC_PATH=./src/

CFLAGS=-Wall -Wextra -g
LDFLAGS=

.PHONY: all

.PRECIOUS: %.o

all: $(BIN_PATH) $(BINS)

$(BIN_PATH):
	$(MKDIR) $(BIN_PATH)

$(BIN_PATH)/%: $(UTIL) $(OBJECTS)
	$(CC) $(SRC_PATH)/$(@F)/main.o $(UTIL) $(CFLAGS) -o $@ $(LDFLAGS)

clean:
	- $(RM) $(SRC_PATH)/*/*.o
	- $(RM) -r $(BIN_PATH)

%.o: %.c
	$(CC) -c $(CFLAGS) -o "$@" "$<"
