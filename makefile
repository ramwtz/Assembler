#paths to project files by extension
SOURCEDIR = ./src
ASSEMBLEDIR = ./src/assembler
BUILDDIR = ./build
BINDIR = ./bin

#variable path definitions
vpath %.c $(ASSEMBLEDIR) $(SOURCEDIR)
vpath %.o $(BUILDDIR)

#std flags for compiler
CC = gcc
CFLAGS = -Wall -pedantic -ansi -g -I'$(ASSEMBLEDIR)'

#project files
TARGET = $(BINDIR)/Assembler
SRCS = $(wildcard $(ASSEMBLEDIR)/*.c) $(SOURCEDIR)/main.c
FILENAMES = $(notdir $(SRCS))
OBJS = $(addprefix $(BUILDDIR)/,$(FILENAMES:.c=.o))

# OS specific part
ifeq ($(OS),Windows_NT)
	EXT = .exe
	SEP=\\
else
	EXT = $(empty)
	SEP=/
endif

#commands
all: dir $(TARGET)
	@echo $(TARGET) "Made"
	@echo;

dir:
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(BINDIR)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@$(EXT)

$(BUILDDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c  $< -o $@

#works using make in windows with minGW 
clean:
	@rm -rf $(subst /,$(SEP), $(OBJS))
	@echo Clean done