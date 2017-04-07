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
CC=cc
CPPFLAGS_HDB.LIB=-g -L-L$(ORACLE_HOME)/lib/ -L$(ORACLE_HOME)/rdbms/lib/ -mt -xCC 
ARFLAGS=


###### Target: /home/ems/system/hdb/v4.0a/lib/hdb.lib ######
TARGETDIR_HDB.LIB=/home/ems/system/hdb/v4.0a/src
CPPFLAGS_HDB.LIB += \
	-I$(ORACLE_HOME)/rdbms/demo \
	-I/home/openwin/include \
	-I/home/dt/include 
OBJS_HDB.LIB = \
	$(TARGETDIR_HDB.LIB)/comlib.o \
	$(TARGETDIR_HDB.LIB)/hdblib.o \
	/home/ems/system/dms/v4.0a/lib/dblib.lib \
	/home/ems/system/dps/v4.0a/lib/ipclib.lib \
	/home/ems/system/lan/v4.0a/lib/lan.lib \

# Link or archive
/home/ems/system/hdb/v4.0a/lib/hdb.lib: $(OBJS_HDB.LIB) 
	$(AR) $(ARFLAGS) -r /home/ems/system/hdb/v4.0a/lib/hdb.lib $(OBJS_HDB.LIB) 

# Compile source files into .o's
$(TARGETDIR_HDB.LIB)/comlib.o: comlib.c
	$(COMPILE.c) $(CFLAGS_HDB.LIB) $(CPPFLAGS_HDB.LIB) -o $(TARGETDIR_HDB.LIB)/comlib.o comlib.c
$(TARGETDIR_HDB.LIB)/hdblib.o: hdblib.c
	$(COMPILE.c) $(CFLAGS_HDB.LIB) $(CPPFLAGS_HDB.LIB) -o $(TARGETDIR_HDB.LIB)/hdblib.o hdblib.c


###### clean target: deletes generated files ######
clean:
	$(RM) \
	/home/ems/system/hdb/v4.0a/lib/hdb.lib \
	$(TARGETDIR_HDB.LIB)/comlib.o \
	$(TARGETDIR_HDB.LIB)/hdblib.o 

# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE: /home/ems/system/hdb/v4.0a/src/.make.state.Makefile.ems.hdblib
