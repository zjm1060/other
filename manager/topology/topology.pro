##
#	@Author zlb
#	@Time	2011-7-28
# @Desc	ÍøÂçÍØÆË·¢ÏÖ
##

include( ./configure.pri )
include( ../config.pri )

TEMPLATE = lib

QT += network

TARGET = topology

build_pass:CONFIG(debug, debug|release){
     TARGET = $$join(TARGET,,,d)
}

OUTPUT_DIR			=

CONFIG(debug, debug|release){
	OUTPUT_DIR		=	debug
}

CONFIG(release, debug|release){
	OUTPUT_DIR		=	release
}

MOC_DIR       	= $$OUTPUT_DIR/moc
OBJECTS_DIR  		= $$OUTPUT_DIR/obj

win32:DESTDIR		= ../../../h9000/bin
unix:DESTDIR		= ../../../h9000/lib

contains(CONFIG, TopologyDll ) {
    CONFIG += dll
} 
else {
    CONFIG += staticlib
}

win32:TopologyDll {
    DEFINES    += QT_DLL TOPOLOGY_DLL TOPOLOGY_MAKEDLL
}

symbian:TopologyDll {
    DEFINES    += QT_DLL TOPOLOGY_DLL TOPOLOGY_MAKEDLL
}

CONFIG           += thread

INCLUDEPATH	+=	../global/include/	\
								../logplus/include/	\
								./include/
								
								

HEADERS +=	\
				include/topocommon.h	\
				include/qicmp.h		\
				include/qrouteprobe.h	\
				include/qlogicaltopo.h	\
				include/qtraceroute.h
				
SOURCES +=	\
				src/topocommon.cpp	\
				src/qicmp.cpp				\
				src/qrouteprobe.cpp	\
				src/qlogicaltopo.cpp	\
				src/qtraceroute.cpp	

CONFIG(debug, debug|release) { 
    win32:LIBS += \
    							-L../../../h9000/bin globald1.lib logplusd1.lib
    							
    unix:LIBS += -L../../../h9000/lib -lglobald -llogplusd
}
CONFIG(release, debug|release) { 
    win32:LIBS += \
    							-L../../../h9000/bin global1.lib logplus1.lib
    							
    unix:LIBS += -L../../../h9000/lib -lglobal -llogplus
}
