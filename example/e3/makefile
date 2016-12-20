###############################################################################
##                                                                           ##
## Makefile for example e3 of the command-line option parser for mixed order ##
##                                                                           ##
##---------------------------------------------------------------------------##
## File:   ~/example/e3/makefile                                             ##
## Author: Ulrich Becker                                                     ##
## Date:   12.11.2016                                                        ##
###############################################################################
BASEDIR = ../../src
SOURCES = cmd_opt_ex3.c $(BASEDIR)/parse_opts.c
EXE_NAME = e3

VPATH  = $(BASEDIR)
INCDIR = $(BASEDIR)
CFLAGS = -g -O0

CC     ?= gcc
CFLAGS += $(addprefix -I,$(INCDIR))

OBJDIR = .obj

OBJ = $(addprefix $(OBJDIR)/,$(addsuffix .o,$(notdir $(basename $(SOURCES)))))

.PHONY: all 
all: $(EXE_NAME)

$(OBJDIR):
	mkdir $(OBJDIR)

$(OBJDIR)/%.o: %.c $(SOURCES) $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXE_NAME): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean
clean:
	rm -f $(OBJDIR)/*.o $(EXE_NAME) core
	rmdir $(OBJDIR)
	
#=================================== EOF ======================================
