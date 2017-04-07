## -*- Makefile -*-
##
## 用户：ems
## 时间：2010-4-22 16:16:03
## 由 Sun Studio 创建的 Makefile。
##
## 此文件是自动生成的。
##


#### 由所有生成目标共享的编译器和工具定义 #####
CC = cc
BASICOPTS = -g
CFLAGS = $(BASICOPTS) -xCC


# 定义目标目录。
TARGETDIR_dl476_client_recv.dxe=.


all: $(TARGETDIR_dl476_client_recv.dxe)/dl476_client_recv.dxe

## 目标：dl476_client_recv.dxe
OBJS_dl476_client_recv.dxe =  \
	$(TARGETDIR_dl476_client_recv.dxe)/dl476_app_fun.o \
	$(TARGETDIR_dl476_client_recv.dxe)/dl476_client_recv.o \
	$(TARGETDIR_dl476_client_recv.dxe)/dl476_fun.o
SYSLIBS_dl476_client_recv.dxe = -lm -lsocket -lnsl -lthread 
USERLIBS_dl476_client_recv.dxe = ../../../dps/v4.0a/lib/ipclib.lib ../../../lan/v4.0a/lib/lan.lib ../../../dps/v4.0a/lib/dpslib.lib ../../../dms/v4.0a/lib/dblib.lib $(SYSLIBS_dl476_client_recv.dxe) 
DEPLIBS_dl476_client_recv.dxe =      
LDLIBS_dl476_client_recv.dxe = $(USERLIBS_dl476_client_recv.dxe)


# 链接或归档
$(TARGETDIR_dl476_client_recv.dxe)/dl476_client_recv.dxe: $(TARGETDIR_dl476_client_recv.dxe) $(OBJS_dl476_client_recv.dxe) $(DEPLIBS_dl476_client_recv.dxe)
	$(LINK.c) $(CFLAGS_dl476_client_recv.dxe) $(CPPFLAGS_dl476_client_recv.dxe) -o $@ $(OBJS_dl476_client_recv.dxe) $(LDLIBS_dl476_client_recv.dxe)


# 将源文件编译为 .o 文件
$(TARGETDIR_dl476_client_recv.dxe)/dl476_app_fun.o: $(TARGETDIR_dl476_client_recv.dxe) dl476_app_fun.c
	$(COMPILE.c) $(CFLAGS_dl476_client_recv.dxe) $(CPPFLAGS_dl476_client_recv.dxe) -o $@ dl476_app_fun.c

$(TARGETDIR_dl476_client_recv.dxe)/dl476_client_recv.o: $(TARGETDIR_dl476_client_recv.dxe) dl476_client_recv.c
	$(COMPILE.c) $(CFLAGS_dl476_client_recv.dxe) $(CPPFLAGS_dl476_client_recv.dxe) -o $@ dl476_client_recv.c

$(TARGETDIR_dl476_client_recv.dxe)/dl476_fun.o: $(TARGETDIR_dl476_client_recv.dxe) dl476_fun.c
	$(COMPILE.c) $(CFLAGS_dl476_client_recv.dxe) $(CPPFLAGS_dl476_client_recv.dxe) -o $@ dl476_fun.c



#### 清理目标将会删除所有生成的文件 ####
clean:
	rm -f \
		$(TARGETDIR_dl476_client_recv.dxe)/dl476_client_recv.dxe \
		$(TARGETDIR_dl476_client_recv.dxe)/dl476_app_fun.o \
		$(TARGETDIR_dl476_client_recv.dxe)/dl476_client_recv.o \
		$(TARGETDIR_dl476_client_recv.dxe)/dl476_fun.o
# 启用依赖关系检查
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.Sun-sparc-Solaris

