## -*- Makefile -*-
##
## Project: hdblib.prd
## User: ems
## Time: 06/05/25 15:19:28
## Makefile created by Sun WorkShop.
##
## This file is generated automatically -- DO NOT EDIT.
##



project: /home/ems/system/hdb/v4.0a/lib/hdb.lib 

##### Compilers and tools definitions shared by all build objects #####
include ../../../global.make

CFLAGS = $(BASICOPTS) -c -mt


###### Target: /home/ems/system/hdb/v4.0a/lib/hdb.lib ######
TARGETDIR=/home/ems/system/hdb/v4.0a/lib

CPPFLAGS += \
	-I$(ORACLE_HOME)/precomp/public \
	-I$(ORACLE_HOME)/rdbms/public \
	-I$(ORACLE_HOME)/rdbms/demo \
	-I/home/openwin/include \
	-I/home/dt/include 
	
OBJS = \
	$(TARGETDIR)/comlib.o \
	$(TARGETDIR)/hdblib.o \

# Link or archive
$(TARGETDIR)/hdb.lib: $(TARGETDIR) $(OBJS) 
	$(ARCHIVE) $@ $(OBJS)

# Compile source files into .o's
$(TARGETDIR)/comlib.o: comlib.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $(TARGETDIR)/comlib.o comlib.c
$(TARGETDIR)/hdblib.o: hdblib.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $(TARGETDIR)/hdblib.o hdblib.c


###### clean target: deletes generated files ######
clean:
	$(RM) \
	$(TARGETDIR)/hdb.lib \
	$(OBJS)

# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE: .make.state
