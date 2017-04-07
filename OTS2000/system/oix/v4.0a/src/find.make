## -*- Makefile -*-
##
## User: ems
## Time: 2007-8-25 16:32:03
## Makefile created by the Native Languages Module.
##
## This file is generated automatically -- Changes will be lost if regenerated
##


#### Compiler and tool definitions shared by all build targets #####
CC = gcc
BASICOPTS   =-g -w -Wall `pkg-config gtk+-2.0  --cflags`
LINKOPTS =   `pkg-config gtk+-2.0  --libs`
CFLAGS = $(BASICOPTS)


# Define the target directories.
TARGETDIR_findstr.dxe= .


all: $(TARGETDIR_findstr.dxe)/findstr.dxe

## Target: findstr.dxe
OBJS_findstr.dxe =  \
	$(TARGETDIR_findstr.dxe)/main.o \
	$(TARGETDIR_findstr.dxe)/interface.o


# Link or archive
$(TARGETDIR_findstr.dxe)/findstr.dxe: $(TARGETDIR_findstr.dxe) $(OBJS_findstr.dxe)
	$(LINK.c) $(CFLAGS_findstr.dxe) $(LINKOPTS) $(CPPFLAGS_findstr.dxe) -o $@ $(OBJS_findstr.dxe) $(LDLIBS_findstr.dxe)


# Compile source files into .o files
$(TARGETDIR_findstr.dxe)/main.o: $(TARGETDIR_findstr.dxe) main.c
	$(COMPILE.c) $(CFLAGS_findstr.dxe) $(CPPFLAGS_findstr.dxe) -o $@ main.c

$(TARGETDIR_findstr.dxe)/interface.o: $(TARGETDIR_findstr.dxe) interface.c
	$(COMPILE.c) $(CFLAGS_findstr.dxe) $(CPPFLAGS_findstr.dxe) -o $@ interface.c



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR_findstr.dxe)/findstr.dxe \
		$(TARGETDIR_findstr.dxe)/main.o \
		$(TARGETDIR_findstr.dxe)/interface.o
	rm -f -r $(TARGETDIR_findstr.dxe)


# Create the target directory (if needed)
$(TARGETDIR_findstr.dxe):
	mkdir -p $(TARGETDIR_findstr.dxe)


# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.GNU-sparc-Solaris

