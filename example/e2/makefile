###############################################################################
##                                                                           ##
## Makefile for example e2 of the command-line option parser for mixed order ##
##                                                                           ##
##---------------------------------------------------------------------------##
## File:   ~/example/e2/makefile                                             ##
## Author: Ulrich Becker                                                     ##
## Date:   12.11.2016                                                        ##
###############################################################################
BASEDIR = ../../src
SOURCES = cmd_opt_ex2.c $(BASEDIR)/parse_opts.c
EXE_NAME = e2

VPATH  = $(BASEDIR)
INCDIR = $(BASEDIR)
CFLAGS = -g -O0

CC     ?= gcc
CFLAGS += $(addprefix -I,$(INCDIR))

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
