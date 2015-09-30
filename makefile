
# Directories.
BINDIR = bin
SRCDIR = src

# Toolchain.
CC = gcc

# Toolchain configuration.
CFLAGS = -fopenmp -std=c99 -O3
LIBS =
EXEC = rainbow-magic

SRC = $(wildcard $(SRCDIR)/*.c)

all: release debug profiling

release:
	mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -DNDEBUG $(SRC) -o $(BINDIR)/$(EXEC) $(LIBS)

debug:
	mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $(SRC) -o $(BINDIR)/$(EXEC).debug $(LIBS)

profiling:
	mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) -pg -DNDEBUG $(SRC) -o $(BINDIR)/$(EXEC).prof $(LIBS)

clean:
	rm -f $(BINDIR)/$(EXEC)
	rm -f $(BINDIR)/$(EXEC).debug
