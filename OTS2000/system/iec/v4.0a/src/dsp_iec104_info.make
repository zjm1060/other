## -*- Makefile -*-
##
## 用户： ems
## 时间： 2009-12-25 15:53:01
## 由 Sun Studio 创建的 Makefile。
##
## 此文件是自动生成的。
##


#### 由所有生成目标共享的编译器和工具定义 #####
CC = cc
BASICOPTS = -g
CFLAGS = $(BASICOPTS) -xCC


# 定义目标目录。
TARGETDIR_iec104info.dxe=.


all: $(TARGETDIR_iec104info.dxe)/iec104info.dxe

## 目标： iec104info.dxe
OBJS_iec104info.dxe =  \
	$(TARGETDIR_iec104info.dxe)/dsp_iec104_info.o
SYSLIBS_iec104info.dxe = -lm -lsocket -lnsl 
USERLIBS_iec104info.dxe = ../../../../dps/v4.0a/lib/ipclib.lib ../../../../dps/v4.0a/lib/dpslib.lib ../../../../dms/v4.0a/lib/dblib.lib $(SYSLIBS_iec104info.dxe) 
DEPLIBS_iec104info.dxe =     
LDLIBS_iec104info.dxe = $(USERLIBS_iec104info.dxe)


# 链接或归档
$(TARGETDIR_iec104info.dxe)/iec104info.dxe: $(TARGETDIR_iec104info.dxe) $(OBJS_iec104info.dxe) $(DEPLIBS_iec104info.dxe)
	$(LINK.c) $(CFLAGS_iec104info.dxe) $(CPPFLAGS_iec104info.dxe) -o $@ $(OBJS_iec104info.dxe) $(LDLIBS_iec104info.dxe)


# 将源文件编译为 .o 文件
$(TARGETDIR_iec104info.dxe)/dsp_iec104_info.o: $(TARGETDIR_iec104info.dxe) dsp_iec104_info.c
	$(COMPILE.c) $(CFLAGS_iec104info.dxe) $(CPPFLAGS_iec104info.dxe) -o $@ dsp_iec104_info.c



#### 清理目标将会删除所有生成的文件 ####
clean:
	rm -f \
		$(TARGETDIR_iec104info.dxe)/iec104info.dxe \
		$(TARGETDIR_iec104info.dxe)/dsp_iec104_info.o
# 启用依赖关系检查
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.Sun-x86-Solaris

