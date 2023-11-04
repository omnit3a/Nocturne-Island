CC=gcc
CFLAGS=-Wall -I$(HDRDIR) -lm -lSDL2 -lSDL2_ttf

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
	cp *.txt $(PACKAGEDIR)
	zip -r $(PACKAGENAME) $(PACKAGEDIR)
