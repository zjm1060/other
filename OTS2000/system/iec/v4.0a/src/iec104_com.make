## -*- Makefile -*-
##
## 用户： ems
## 时间： 2009-12-24 17:24:02
## 由 Sun Studio 创建的 Makefile。
##
## 此文件是自动生成的。
##


#### 由所有生成目标共享的编译器和工具定义 #####
CC = cc
BASICOPTS = -g
CFLAGS = $(BASICOPTS) -xCC


# 定义目标目录。
TARGETDIR_iec104_com.dxe=.


all: $(TARGETDIR_iec104_com.dxe)/iec104_com.dxe

## 目标： iec104_com.dxe
OBJS_iec104_com.dxe =  \
	$(TARGETDIR_iec104_com.dxe)/iec104_app_fun.o \
	$(TARGETDIR_iec104_com.dxe)/iec104_com.o \
	$(TARGETDIR_iec104_com.dxe)/iec104_fun.o
SYSLIBS_iec104_com.dxe = -lm -lsocket -lnsl -lthread 
USERLIBS_iec104_com.dxe =../../../../dps/v4.0a/lib/dpslib.lib  ../../../../lan/v4.0a/lib/lan.lib ../../../../dms/v4.0a/lib/dblib.lib ../../../../dps/v4.0a/lib/ipclib.lib $(SYSLIBS_iec104_com.dxe) 
DEPLIBS_iec104_com.dxe =      
LDLIBS_iec104_com.dxe = $(USERLIBS_iec104_com.dxe)


# 链接或归档
$(TARGETDIR_iec104_com.dxe)/iec104_com.dxe: $(TARGETDIR_iec104_com.dxe) $(OBJS_iec104_com.dxe) $(DEPLIBS_iec104_com.dxe)
	$(LINK.c) $(CFLAGS_iec104_com.dxe) $(CPPFLAGS_iec104_com.dxe) -o $@ $(OBJS_iec104_com.dxe) $(LDLIBS_iec104_com.dxe)


# 将源文件编译为 .o 文件
$(TARGETDIR_iec104_com.dxe)/iec104_app_fun.o: $(TARGETDIR_iec104_com.dxe) iec104_app_fun.c
	$(COMPILE.c) $(CFLAGS_iec104_com.dxe) $(CPPFLAGS_iec104_com.dxe) -o $@ iec104_app_fun.c

$(TARGETDIR_iec104_com.dxe)/iec104_com.o: $(TARGETDIR_iec104_com.dxe) iec104_com.c
	$(COMPILE.c) $(CFLAGS_iec104_com.dxe) $(CPPFLAGS_iec104_com.dxe) -o $@ iec104_com.c

$(TARGETDIR_iec104_com.dxe)/iec104_fun.o: $(TARGETDIR_iec104_com.dxe) iec104_fun.c
	$(COMPILE.c) $(CFLAGS_iec104_com.dxe) $(CPPFLAGS_iec104_com.dxe) -o $@ iec104_fun.c



#### 清理目标将会删除所有生成的文件 ####
clean:
	rm -f \
		$(TARGETDIR_iec104_com.dxe)/iec104_com.dxe \
		$(TARGETDIR_iec104_com.dxe)/iec104_app_fun.o \
		$(TARGETDIR_iec104_com.dxe)/iec104_com.o \
		$(TARGETDIR_iec104_com.dxe)/iec104_fun.o
# 启用依赖关系检查
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.Sun-x86-Solaris

