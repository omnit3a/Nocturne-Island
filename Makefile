CC=gcc
CFLAGS=-Wall -I$(HDRDIR) -lm -lpthread -lSDL2

HDRDIR=include/

SRCDIR=src
SRCFILE=*.c

OUTDIR=bin
OUTFILE=nocturne-island

all:
	clear
	$(CC) -o $(OUTDIR)/$(OUTFILE) $(SRCDIR)/$(SRCFILE) $(CFLAGS)
