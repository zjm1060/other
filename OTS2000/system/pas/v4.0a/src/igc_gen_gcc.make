## -*- Makefile -*- ## 
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
CFLAGS_igc_gen.dxe = $(CFLAGS) -c
CPPFLAGS_igc_gen.dxe = $(CPPFLAGS)

LINK.c = $(COMPILE.c) $(LDFLAGS) $(TARGET ARCH)

# Define the target directories. 
TARGETDIR_igc_gen.dxe=.

all: $(TARGETDIR_igc_gen.dxe)/igc_gen.dxe
  
     
## Target: igc_gen.dxe 
OBJS_igc_gen.dxe =  \
 	$(TARGETDIR_igc_gen.dxe)/igc_gen.o
SYSLIBS_igc_gen.dxe = -lm -lnsl -lpthread #-lsocket
USERLIBS_igc_gen.dxe = ../../../pas/v4.0a/lib/igclib.lib ../../../dps/v4.0a/lib/dpslib.lib ../../../lan/v4.0a/lib/lan.lib ../../../dps/v4.0a/lib/ipclib.lib ../../../dms/v4.0a/lib/dblib.lib $(SYSLIBS_igc_gen.dxe)
DEPLIBS_igc_gen.dxe =
LDLIBS_igc_gen.dxe = $(USERLIBS_igc_gen.dxe)
  
# Link or archive
$(TARGETDIR_igc_gen.dxe)/igc_gen.dxe: $(OBJS_igc_gen.dxe) $(DEPLIBS_igc_gen.dxe)
	$(LINK.c) -o $@ $(OBJS_igc_gen.dxe) $(LDLIBS_igc_gen.dxe)


# Compile source files into .o files 
$(TARGETDIR_igc_gen.dxe)/igc_gen.o: $(TARGETDIR_igc_gen.dxe) igc_gen.c
	$(COMPILE.c) $(CFLAGS_igc_gen.dxe) $(CPPFLAGS_igc_gen.dxe) -o $@ igc_gen.c

#### Clean target deletes all generated files #### 
clean:
	rm -f \
		$(TARGETDIR_igc_gen.dxe)/igc_gen.dxe \
		$(TARGETDIR_igc_gen.dxe)/igc_gen.o

# Enable dependency checking
#.KEEP_STATE:
#.KEEP_STATE_FILE:.make.state.Sun-sparc-Solaris