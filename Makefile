# tool macros
CC = gcc
CLIBS = -lm -lSDL2 -lSDL2_ttf
CERROR = -Wall
CFLAGS = $(CERROR) $(CLIBS) $(addprefix -I, $(HDR_PATH))
LFLAGS = $(CFLAGS) -c

# path macros
BIN_PATH := bin
SRC_PATH := src
LIB_PATH := $(SRC_PATH)/libs
HDR_PATH := include $(LIB_PATH)/include
AST_PATH := assets
PKG_PATH := nocturne

# compile macros
GAME_NAME := nocturne-island
PKG_NAME := nocturne.zip
TARGET := $(BIN_PATH)/$(GAME_NAME)
PACKAGE := $(PKG_NAME)

PKG_FILES := $(AST_PATH) LICENSE CREDITS.txt CHANGELOG.txt

# src files & obj files
SRC := $(shell find $(SRC_PATH) -type f -name '*.c')
OBJ := $(patsubst $(SRC_PATH)/%.c, $(BIN_PATH)/%.o, $(SRC))

# clean files list

# default rule
all:
	@if [ ! -d $(BIN_PATH) ]; then mkdir $(BIN_PATH); fi;
	@$(MAKE) -C . $(TARGET)

# non-phony targets
$(TARGET): $(OBJ)
	@printf "%s:\n\t" $(notdir $(TARGET))
	@$(CC) $^ $(CFLAGS) -o $@
	@printf "Compiled Successfully!\n" 

$(BIN_PATH)/%.o: $(SRC_PATH)/%.c
	@if [ ! -d $(dir $@) ]; then mkdir -p $(dir $@); fi;
	@printf "\t%-15s %-30s " $(notdir $^) $@
	@echo $(CLIBS)
	@$(CC) $(LFLAGS) -o $@ $^

.PHONY: clean
clean:
	@rm -rf $(BIN_PATH) $(PKG_PATH) $(PACKAGE)

.PHONY: package
package: $(PKG_FILES) $(TARGET)
	@if [ ! -d $(PKG_PATH) ]; then mkdir -p $(PKG_PATH); fi;
	@printf "Copying files into package:\n"	
	@printf "\t%s\n" $^; cp $^ -r $(PKG_PATH)
	@printf "Creating package archive:\n"
	@printf "\t%s\n" $(PACKAGE)
	@zip -r $(PACKAGE) $(PKG_PATH) > /dev/null
