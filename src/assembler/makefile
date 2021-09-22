#paths to project files by extension

#SOURCEDIR = ./src
#INCLUDEDIR = ./include
BUILDDIR = ../../build
BINDIR = ../../bin

#vpath %.h $(INCLUDEDIR)
#vpath %.c $(SOURCEDIR)
vpath %.o $(BUILDDIR)

#std flags for compiler
CC = gcc
CFLAGS = -Wall -pedantic -ansi -g -I'$(INCLUDEDIR)'

#project files
SRCS = $(wildcard *.c)
OBJNAMES := $(SRCS:.c=.o) 
#OBJS = $(foreach obj, $(OBJNAMES), $(addprefix $(BUILDDIR)/, $(obj)))

# OS specific part
ifeq ($(OS),Windows_NT)
	EXT = .exe
	SEP=\\
else
	EXT = $(empty)
	SEP=/
endif

#TEMP := $(subst /,$(SEP), $(OBJS))

#commands
all: dir $(OBJNAMES) Assembler

dir:
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(BINDIR)

%.o: %.c
	$(CC) $(CFLAGS) -c  $< -o $(BUILDDIR)/$@

#Assembler: $(OBJS)
#	$(CC) $(CFLAGS) $^ -o $(BINDIR)/$@$(EXT)
#	@echo;
#	@echo $@ "Made"
#	@echo;

#works using make in windows with minGW 
clean:
	@rm -rf $(subst /,$(SEP), $(OBJS))
	@echo Clean done