## -*- Makefile -*-
##
## 用户： ems
## 时间： 2010-2-2 15:47:06
## 由 Sun Studio 创建的 Makefile。
##
## 此文件是自动生成的。
##


#### 由所有生成目标共享的编译器和工具定义 #####
CC = cc
BASICOPTS = -g
CFLAGS = $(BASICOPTS) -xCC


# 定义目标目录。
TARGETDIR_CreateIecDef.dxe=.


all: $(TARGETDIR_CreateIecDef.dxe)/CreateIecDef.dxe

## 目标： CreateIecDef.dxe
OBJS_CreateIecDef.dxe =  \
	$(TARGETDIR_CreateIecDef.dxe)/IecDef.o
SYSLIBS_CreateIecDef.dxe = -lm -lsocket -lnsl -lthread -lpthread 
USERLIBS_CreateIecDef.dxe = ../../../../lan/v4.0a/lib/lan.lib ../../../../dps/v4.0a/lib/ipclib.lib ../../../../dps/v4.0a/lib/dpslib.lib ../../../../dms/v4.0a/lib/dblib.lib $(SYSLIBS_CreateIecDef.dxe) 
DEPLIBS_CreateIecDef.dxe =      
LDLIBS_CreateIecDef.dxe = $(USERLIBS_CreateIecDef.dxe)


# 链接或归档
$(TARGETDIR_CreateIecDef.dxe)/CreateIecDef.dxe: $(TARGETDIR_CreateIecDef.dxe) $(OBJS_CreateIecDef.dxe) $(DEPLIBS_CreateIecDef.dxe)
	$(LINK.c) $(CFLAGS_CreateIecDef.dxe) $(CPPFLAGS_CreateIecDef.dxe) -o $@ $(OBJS_CreateIecDef.dxe) $(LDLIBS_CreateIecDef.dxe)


# 将源文件编译为 .o 文件
$(TARGETDIR_CreateIecDef.dxe)/IecDef.o: $(TARGETDIR_CreateIecDef.dxe) IecDef.c
	$(COMPILE.c) $(CFLAGS_CreateIecDef.dxe) $(CPPFLAGS_CreateIecDef.dxe) -o $@ IecDef.c



#### 清理目标将会删除所有生成的文件 ####
clean:
	rm -f \
		$(TARGETDIR_CreateIecDef.dxe)/CreateIecDef.dxe \
		$(TARGETDIR_CreateIecDef.dxe)/IecDef.o
	rm -f -r $(TARGETDIR_CreateIecDef.dxe)


# 创建目标目录（如果需要）
$(TARGETDIR_CreateIecDef.dxe):
	mkdir -p $(TARGETDIR_CreateIecDef.dxe)


# 启用依赖关系检查
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.Sun-x86-Solaris

