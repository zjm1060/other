## -*- Makefile -*-
##
## Project: igclib.prd
## User: ems
## Time: 06/04/20 14:10:21
## Makefile created by Sun WorkShop.
##
## This file is generated automatically -- DO NOT EDIT.
##



project: /home/ems/system/pas/v4.0a/lib/igclib.lib 

##### Compilers and tools definitions shared by all build objects #####
CC=cc
CFLAGS=-g
ARFLAGS=


###### Target: /home/ems/system/pas/v4.0a/lib/igclib.lib ######
TARGETDIR_igclib.LIB=/home/ems/system/pas/v4.0a/src
CPPFLAGS_igclib.LIB += \
	-I/home/openwin/include \
	-I/home/dt/include 
OBJS_igclib.LIB = \
	$(TARGETDIR_igclib.LIB)/igclib.o 


# Link or archive
/home/ems/system/pas/v4.0a/lib/igclib.lib: $(OBJS_igclib.LIB) 
	$(AR) $(ARFLAGS) -r /home/ems/system/pas/v4.0a/lib/igclib.lib $(OBJS_igclib.LIB) 

# Compile source files into .o's
$(TARGETDIR_igclib.LIB)/igclib.o: /home/ems/system/pas/v4.0a/src/igclib.c
	$(COMPILE.c) $(CFLAGS_igclib.LIB) $(CPPFLAGS_igclib.LIB) -o $(TARGETDIR_igclib.LIB)/igclib.o /home/ems/system/pas/v4.0a/src/igclib.c

###### clean target: deletes generated files ######
clean:
	$(RM) \
	/home/ems/system/pas/v4.0a/lib/igclib.lib \
	$(TARGETDIR_igclib.LIB)/igclib.o 
	$(RM) -r SunWS_cache/sb_* 

# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE: /home/ems/system/pas/v4.0a/src/.make.state.Makefile.ems.igclib
