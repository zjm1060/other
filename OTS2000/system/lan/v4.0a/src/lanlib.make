## -*- Makefile -*-
##
## Project: lanlib.prd
## User: ems
## Time: 06/04/20 14:10:21
## Makefile created by Sun WorkShop.
##
## This file is generated automatically -- DO NOT EDIT.
##


#### Compiler and tool definitions shared by all build targets #####
include ../../../global.make

CFLAGS = $(BASICOPTS) -c


# Define the target directories.
TARGETDIR=/home/ems/system/lan/v4.0a/lib

CPPFLAGS += \
	-I/home/openwin/include \
	-I/home/dt/include 
	
OBJS = \
	$(TARGETDIR)/commonlib.o \
	$(TARGETDIR)/lanlib.o \
	$(TARGETDIR)/netudp.o \
	$(TARGETDIR)/udp.o 


# Link or archive
$(TARGETDIR)/lan.lib: $(TARGETDIR) $(OBJS)
	$(ARCHIVE) $@ $(OBJS)

# Compile source files into .o's
$(TARGETDIR)/commonlib.o: commonlib.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ commonlib.c
$(TARGETDIR)/lanlib.o: lanlib.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ lanlib.c
$(TARGETDIR)/netudp.o: netudp.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ netudp.c
$(TARGETDIR)/udp.o: udp.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ udp.c


###### clean target: deletes generated files ######
clean:
	$(RM) \
	$(TARGETDIR)/lan.lib \
	$(OBJS)

# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE: .make.state
