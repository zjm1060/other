##
#	@Author zlb
#	@Time	2011-7-28
# @Desc	ÍøÂçÍØÆË·¢ÏÖ
##

include( ./configure.pri )
include( ../config.pri )

TEMPLATE = lib

QT += network

win32:TARGET = libtopology
unix:TARGET = topology

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

INCLUDEPATH	+=	\
								./include/	\
								../libglobal/include/	\
								../liblogplus/include/	\
								../libutil/include/
								
win32:INCLUDEPATH	+=	\
								"$$(NET_SNMP_DIR)/include/"
								

HEADERS +=	\
				include/topocommon.h	\
				include/qicmp.h		\
				include/qrouteprobe.h	\
				include/qlogicaltopo.h	\
				include/qtraceroute.h	\
				include/qsnmp.h	\
				include/qtopologygraph.h	\
				include/qtopologyprobe.h
				
SOURCES +=	\
				src/topocommon.cpp	\
				src/qicmp.cpp				\
				src/qrouteprobe.cpp	\
				src/qlogicaltopo.cpp	\
				src/qtraceroute.cpp	\
				src/qsnmp.cpp	\
				src/qtopologygraph.cpp	\
				src/qtopologyprobe.cpp

CONFIG(debug, debug|release) { 
    win32:LIBS += \
    							../../../h9000/bin/libglobald1.lib		\
    							../../../h9000/bin/liblogplusd1.lib		\
    							"$$(NET_SNMP_DIR)/lib/netsnmp.lib"
    							
    unix:LIBS += -lnetsnmp -L../../../h9000/lib -lglobald -llogplusd
}
CONFIG(release, debug|release) { 
    win32:LIBS += \
    							../../../h9000/bin/libglobal1.lib		\
    							../../../h9000/bin/liblogplus1.lib	\
    							"$$(NET_SNMP_DIR)/lib/netsnmp.lib"
    							
    unix:LIBS += -lnetsnmp -L../../../h9000/lib -lglobal -llogplus
}
