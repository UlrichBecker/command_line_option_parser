###############################################################################
##                                                                           ##
##  Makefile for example e1 of the command-line option parser for classical  ##
##  order.                                                                   ##
##                                                                           ##
##---------------------------------------------------------------------------##
## File:   ~/example/e1/makefile                                             ##
## Author: Ulrich Becker                                                     ##
## Date:   10.11.2016                                                        ##
###############################################################################
BASEDIR = ../../src
SOURCES = cmd_opt_ex1.c $(BASEDIR)/parse_opts.c
EXE_NAME = e1

VPATH= $(BASEDIR)
INCDIR = $(BASEDIR)
CFLAGS = -g -O0

CC=gcc
CFLAGS +=-I$(INCDIR)

OBJDIR=.obj


OBJ = $(addprefix $(OBJDIR)/,$(addsuffix .o,$(notdir $(basename $(SOURCES)))))

.PHONY: all 
all: $(EXE_NAME)

$(OBJDIR):
	mkdir $(OBJDIR)

$(OBJDIR)/%.o: %.c $(SOURCES) $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXE_NAME): $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o $(EXE_NAME) core
	rmdir $(OBJDIR)
	
#=================================== EOF ======================================
