## -*- Makefile -*-
##
## Project: cmd.prd
## User: ems
## Time: 06/04/04 09:39:04
## Makefile created by Sun WorkShop.
##
## This file is generated automatically -- DO NOT EDIT.
##



project: /usr/ems/system/cmd/v4.0a/lib/cmd.lib 

##### Compilers and tools definitions shared by all build objects #####
CC=/opt/SUNWspro/bin/../WS6U1/bin/cc
CFLAGS=-g -xCC 
ARFLAGS=


###### Target: cmd.lib ######
TARGETDIR_CMD.LIB=/usr/ems/system/cmd/v4.0a/lib
CPPFLAGS_CMD.LIB += \
	-I/usr/openwin/include \
	-I/usr/dt/include 
OBJS_CMD.LIB = \
	$(TARGETDIR_CMD.LIB)/CmdProcess.o 


# Link or archive
/usr/ems/system/cmd/v4.0a/lib/cmd.lib: $(OBJS_CMD.LIB) 
	$(AR) $(ARFLAGS) -r /usr/ems/system/cmd/v4.0a/lib/cmd.lib $(OBJS_CMD.LIB) 

# Compile source files into .o's
$(TARGETDIR_CMD.LIB)/CmdProcess.o: CmdProcess.c
	$(COMPILE.c) $(CFLAGS_CMD.LIB) $(CPPFLAGS_CMD.LIB) -o $(TARGETDIR_CMD.LIB)/CmdProcess.o CmdProcess.c


###### clean target: deletes generated files ######
clean:
	$(RM) \
	/usr/ems/system/cmd/v4.0a/lib/cmd.lib \
	$(TARGETDIR_CMD.LIB)/CmdProcess.o 

# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE: /usr/ems/system/cmd/v4.0a/src/.make.state.Makefile.ems.cmd
