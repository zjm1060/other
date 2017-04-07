UNAME=$(shell uname -s)

ifeq ($(UNAME), SunOS)
SUB_UNAME=$(shell uname -p)
ifeq ($(SUB_UNAME), sparc)
OS=OS_SOLARIS_SPARC
SUFFIX=dxe
else
OS=OS_SOLARIS_X86
SUFFIX=xxe
endif

else
ifeq ($(UNAME), AIX)
OS=OS_AIX
SUFFIX=axe
endif
endif

ifndef OS
	$(error makefile has not supported the current os.)
endif

ifeq ($(OS), OS_AIX)
CC=gcc
BASICOPTS =-g -Wall -D_LINUX_SOURCE_COMPAT
else
CC=cc
BASICOPTS =-g
endif

COMPILE = $(CC)
LINK = $(COMPILE) $(LDFLAGS) $(TARGET ARCH)
ARCHIVE= $(AR) $(ARFLAGS)

BASICLIBS = -lm -lnsl -lpthread

ifeq ($(OS), OS_AIX)
SPECIALLIBS =
else
SPECIALLIBS = -xlic_lib=sunperf -lsocket
endif

SYSLIBS = $(SPECIALLIBS) $(BASICLIBS) -lXm -lcurses

ORACLELIBS= -L$(ORACLE_HOME)/lib/ -L$(ORACLE_HOME)/rdbms/lib/ -lclntsh -lthread -lintl $(SYSLIBS)
