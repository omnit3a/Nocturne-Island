CC=gcc
CFLAGS=-Wall -I$(HDRDIR) -lm -lpthread -lSDL2 -O2

HDRDIR=include/

SRCDIR=src
SRCFILE=*.c

OUTDIR=.
OUTFILE=nocturne-island

PACKAGEDIR=bin/
PACKAGENAME=nocturne.zip
ASSETDIR=assets/

all:
	clear
	$(CC) -o $(OUTDIR)/$(OUTFILE) $(SRCDIR)/$(SRCFILE) $(CFLAGS)

package: all
	cp $(OUTFILE) $(PACKAGEDIR)
	cp -r $(ASSETDIR) $(PACKAGEDIR)
	cp LICENSE $(PACKAGEDIR)
	zip -r $(PACKAGENAME) $(PACKAGEDIR)
