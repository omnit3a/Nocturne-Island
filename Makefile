CC=gcc
CFLAGS=-Wall -I$(INCLUDE) -lm -lSDL2 -lSDL2_ttf

INCLUDE=include/

SRC := $(shell find . -name *.c)

OUT=nocturne-island

PACKAGEDIR=bin/
PACKAGENAME=nocturne.zip
ASSETDIR=assets/

all:
	clear
	$(CC) -o $(OUT) $(SRC) $(CFLAGS)

package: all
	cp $(OUTFILE) $(PACKAGEDIR)
	cp -r $(ASSETDIR) $(PACKAGEDIR)
	cp LICENSE $(PACKAGEDIR)
	cp *.txt $(PACKAGEDIR)
	zip -r $(PACKAGENAME) $(PACKAGEDIR)
