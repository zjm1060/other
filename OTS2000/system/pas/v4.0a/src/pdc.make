## -*- Makefile -*-
##
## User: ems
## Time: 2006-10-10 18:19:03
## Makefile created by Sun Studio.
##
## This file is generated automatically.
##

#### Compiler and tool definitions shared by all build targets #####
include ../../../global.make

CFLAGS = $(BASICOPTS) -c

# Define the target directories.
TARGETDIR=.
TARGET=pdc.$(SUFFIX)

all: $(TARGETDIR)/$(TARGET)

OBJS =  \
	$(TARGETDIR)/rpt_hour_gen.o \
	$(TARGETDIR)/rpt_month_gen.o \
	$(TARGETDIR)/safe_day.o \
	$(TARGETDIR)/wd_calc.o \
	$(TARGETDIR)/copy_rpt_data.o \
	$(TARGETDIR)/delhistory.o \
	$(TARGETDIR)/pdc.o \
	$(TARGETDIR)/rdbupdate.o \
	$(TARGETDIR)/sysallsend.o \
	$(TARGETDIR)/GetDiskSpace.o \
	$(TARGETDIR)/calcufn.o
	
USERLIBS = ../../../dpp/v4.0a/lib/dpplib.lib ../../../dps/v4.0a/lib/dpslib.lib ../../../lan/v4.0a/lib/lan.lib ../../../dps/v4.0a/lib/ipclib.lib ../../../dms/v4.0a/lib/dblib.lib $(SYSLIBS) 
DEPLIBS =      
LDLIBS = $(USERLIBS)

# Link or archive
$(TARGETDIR)/$(TARGET): $(TARGETDIR) $(OBJS) $(DEPLIBS)
	$(LINK) -o $@ $(OBJS) $(LDLIBS)


# Compile source files into .o files
$(TARGETDIR)/rpt_hour_gen.o: $(TARGETDIR) rpt_hour_gen.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ rpt_hour_gen.c

$(TARGETDIR)/rpt_month_gen.o: $(TARGETDIR) rpt_month_gen.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ rpt_month_gen.c

$(TARGETDIR)/safe_day.o: $(TARGETDIR) safe_day.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ safe_day.c

$(TARGETDIR)/wd_calc.o: $(TARGETDIR) wd_calc.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ wd_calc.c

$(TARGETDIR)/copy_rpt_data.o: $(TARGETDIR) copy_rpt_data.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ copy_rpt_data.c

$(TARGETDIR)/delhistory.o: $(TARGETDIR) delhistory.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ delhistory.c

$(TARGETDIR)/pdc.o: $(TARGETDIR) pdc.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ pdc.c

$(TARGETDIR)/rdbupdate.o: $(TARGETDIR) rdbupdate.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ rdbupdate.c

$(TARGETDIR)/sysallsend.o: $(TARGETDIR) sysallsend.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ sysallsend.c

$(TARGETDIR)/GetDiskSpace.o: $(TARGETDIR) GetDiskSpace.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ GetDiskSpace.c


$(TARGETDIR)/calcufn.o: $(TARGETDIR) ../../../pas/v4.0a/src/calcufn.c
	$(COMPILE) $(CFLAGS) $(CPPFLAGS) -o $@ ../../../pas/v4.0a/src/calcufn.c



#### Clean target deletes all generated files ####
clean:
	rm -f \
	$(TARGETDIR)/$(TARGET) \
	$(OBJS)
	
# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state

