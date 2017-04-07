## -*- Makefile -*-
##
## User: ems
## Time: 2006-7-5 15:07:30
## Makefile created by Sun Studio.
##
## This file is generated automatically.
##


#### Compiler and tool definitions shared by all build targets #####
include ../../../global.make

CFLAGS = $(BASICOPTS) -c

# Define the target directories.
TARGETDIR=.
TARGET=periodwrite_hour.$(SUFFIX)

all: $(TARGETDIR)/$(TARGET)

CPPFLAGS = \
	-I$(ORACLE_HOME)/precomp/public \
	-I$(ORACLE_HOME)/rdbms/public \
	-I$(ORACLE_HOME)/rdbms/demo \
	-I../inc
	
OBJS =  \
	$(TARGETDIR)/periodwrite_hour.o

USERLIBS = ../../../lan/v4.0a/lib/lan.lib ../../../dps/v4.0a/lib/ipclib.lib ../../../dms/v4.0a/lib/dblib.lib  ../lib/hdb.lib $(ORACLELIBS)
DEPLIBS =
LDLIBS = $(USERLIBS)


# Link or archive
$(TARGETDIR)/$(TARGET): $(TARGETDIR) $(OBJS) $(DEPLIBS)
	$(LINK) -o $@ $(OBJS) $(LDLIBS)


# Compile source files into .o files
$(TARGETDIR)/periodwrite_hour.o: $(TARGETDIR) periodwrite_hour.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ periodwrite_hour.c



#### Clean target deletes all generated files ####
clean:
	rm -f \
		$(TARGETDIR)/$(TARGET) \
		$(OBJS)
		
# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state

