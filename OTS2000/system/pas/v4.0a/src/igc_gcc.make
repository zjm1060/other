## -*- Makefile -*- 
## 
## User: ems 
## Time: 2006-10-10 18:19:03 
## Makefile created by Sun Studio. 
## 
## This file is generated automatically. 
##   
#### Compiler and tool definitions shared by all build targets ##### 
CC = gcc
BASICOPTS = -g 
CFLAGS = $(BASICOPTS)
 
COMPILE.c = $(CC)
CFLAGS_igc.dxe = $(CFLAGS) -c
CPPFLAGS_igc.dxe = $(CPPFLAGS)
 
LINK.c = $(COMPILE.c) $(LDFLAGS) $(TARGET ARCH)


# Define the target directories. 
TARGETDIR_igc.dxe=.

all: $(TARGETDIR_igc.dxe)/igc.dxe

## Target: igc.dxe 
OBJS_igc.dxe =  \
 	$(TARGETDIR_igc.dxe)/igc.o
SYSLIBS_igc.dxe = -lm -lnsl -lpthread #-lsocket 
USERLIBS_igc.dxe = ../../../pas/v4.0a/lib/igclib.lib ../../../dps/v4.0a/lib/dpslib.lib ../../../lan/v4.0a/lib/lan.lib ../../../dps/v4.0a/lib/ipclib.lib ../../../dms/v4.0a/lib/dblib.lib $(SYSLIBS_igc.dxe)
DEPLIBS_igc.dxe =
LDLIBS_igc.dxe = $(USERLIBS_igc.dxe)

# Link or archive 
$(TARGETDIR_igc.dxe)/igc.dxe: $(TARGETDIR_igc.dxe) $(OBJS_igc.dxe) $(DEPLIBS_igc.dxe)
		$(LINK.c) -o $@ $(OBJS_igc.dxe) $(LDLIBS_igc.dxe)

# Compile source files into .o files 
$(TARGETDIR_igc.dxe)/igc.o: $(TARGETDIR_igc.dxe) igc.c
		$(COMPILE.c) $(CFLAGS_igc.dxe) $(CPPFLAGS_igc.dxe) -o $@ igc.c

#### Clean target deletes all generated files #### 
clean:
	rm -f \
 		$(TARGETDIR_igc.dxe)/igc.dxe \
		$(TARGETDIR_igc.dxe)/igc.o 

# Enable dependency checking
#.KEEP_STATE: 
#.KEEP_STATE_FILE:.make.state.Sun-sparc-Solaris  