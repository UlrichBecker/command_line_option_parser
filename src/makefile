###############################################################################
##                                                                           ##
##      Optional makefile to crating a shared-library: libParseOptsC.so      ##
##                                                                           ##
##---------------------------------------------------------------------------##
## File:      command_line_option_parser/src/makefile                        ##
## Author:    Ulrich Becker                                                  ##
## Date:      13.06.2018                                                     ##
## Rervision:                                                                ##
###############################################################################
LIBNAME      ?= ParseOptsC
LIB_VERSION  ?= 1
LIB_REVISION ?= 0

LIB_DIR     ?= /usr/lib64/
HEADDER_DIR ?= /usr/include/

SOURCE_NAME := parse_opts

CC          ?= gcc
CURRENT_DIR = $(PWD)

LIB_FILENAME = lib$(LIBNAME).so

.PHONY: all
all: $(LIB_FILENAME).$(LIB_VERSION).$(LIB_REVISION)

$(SOURCE_NAME).o: $(SOURCE_NAME).c
	$(CC) -Wall -fPIC -c -I. $(CFLAGS) -o $@ $<

$(LIB_FILENAME).$(LIB_VERSION).$(LIB_REVISION): $(SOURCE_NAME).o
	$(CC) -shared -Wl,-soname,$(LIB_FILENAME).$(LIB_VERSION) -o $@  $<

.PHONY: install
install: all
	mkdir -p $(HEADDER_DIR)
	cp $(SOURCE_NAME).h $(HEADDER_DIR)
	mkdir -p $(LIB_DIR)
	cp $(LIB_FILENAME).$(LIB_VERSION).$(LIB_REVISION) $(LIB_DIR)
	cd $(LIB_DIR); \
	ln -sf $(LIB_FILENAME).$(LIB_VERSION).$(LIB_REVISION) $(LIB_FILENAME).$(LIB_VERSION); \
	ln -sf $(LIB_FILENAME).$(LIB_VERSION) $(LIB_FILENAME); \
	cd $(CURRENT_DIR)

.PHONY: uninstall
uninstall:
	rm $(LIB_DIR)$(LIB_FILENAME)*
	rm $(HEADDER_DIR)$(SOURCE_NAME).h

.PHONY: clean
clean:
	rm *.o
	rm $(LIB_FILENAME).$(LIB_VERSION).$(LIB_REVISION)

#=================================== EOF ======================================
