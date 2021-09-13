##########################################################################
# Simple Makefile to compile Simple First Person Game,                   #
# project for the Computer Graphics course of teacher Antonino Casile.   #
# Students: Alberto Paparella and Martina Tenani.                        #
##########################################################################
# Launching the `make` command from the console into this directory      #
# (`SFPG/`) will create an executable in the same folder, named `sfpg`,  #
# which can be launched via console with `./sfpg`.                       #
##########################################################################
# Project structure:                                                     #
# - `include/` contains header files, with .h extension                  #
# - `obj/` will contain compiled code, with .o extension, not linked yet #
# - `src/` contains the actual source code, with .c extension            #
##########################################################################

# Application name
APP=sfpg
# Header files folder
IDIR=include
# Source code folder
SDIR=src
# Compiled files folder
ODIR=obj
# Compiler
CC=gcc
# Flags
CFLAGS=-I$(IDIR)

# Libraries used
LIBS=-lglut -lGLU -lGL -lGLEW -lm

# Header files names
_DEPS=readBMP.h shader.h shapes.h vertex.h light.h material.h collisionBox.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

# Source code files names
_OBJ=readBMP.o shader.o shapes.o main.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

# Here is where the magic happens
$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	@mkdir -p $(@D)
	$(CC) -c -o $@ $< $(CFLAGS)

$(APP): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

# Simple command to clean the directory, launched with `make clean`
clean:
	rm -f $(ODIR)/*.o $(APP)
	rm -r $(ODIR)