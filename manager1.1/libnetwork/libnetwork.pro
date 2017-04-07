##
#	@Author zlb
#	@Time	2011-3-1
# @Desc	µ×²ãÍ¨Ñ¶
##

include( ../config.pri )
include( ./configure.pri )

TEMPLATE = lib

QT += network

win32:TARGET = libnetwork
unix:TARGET = network

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

contains(CONFIG, NetworkDll ) {
    CONFIG += dll
} 
else {
    CONFIG += staticlib
}

win32:NetworkDll {
    DEFINES    += QT_DLL NETWORK_DLL NETWORK_MAKEDLL
}

symbian:NetworkDll {
    DEFINES    += QT_DLL NETWORK_DLL NETWORK_MAKEDLL
}

INCLUDEPATH	+=	\
								./include/	\
								../libglobal/include/		\
								../liblogplus/include/	\
								../libutil/include/
								

HEADERS +=	\
				include/qabstractmulticast.h	\
				include/qabstractsender.h	\
				include/qclient.h					\
				include/qclientmanager.h	\
				include/qconnection.h			\
				include/qcontrolledview.h	\
				include/qheartbeat.h			\
				include/qinterprocessclient.h	\
				include/qinterprocessserver.h	\
				include/qmemorysender.h		\
				include/qmulticastreceiver.h	\
				include/qmulticastsender.h	\
				include/qserver.h					\
				include/qservermanager.h	\
				include/qprocessorsender.h	\
				include/qprocesssender.h
				
SOURCES +=	\
				src/qabstractmulticast.cpp	\
				src/qabstractsender.cpp	\
				src/qclient.cpp					\
				src/qclientmanager.cpp	\
				src/qconnection.cpp			\
				src/qcontrolledview.cpp	\
				src/qheartbeat.cpp			\
				src/qinterprocessclient.cpp	\
				src/qinterprocessserver.cpp	\
				src/qmemorysender.cpp		\
				src/qmulticastreceiver.cpp	\
				src/qmulticastsender.cpp	\
				src/qserver.cpp					\
				src/qservermanager.cpp	\
				src/qprocessorsender.cpp	\
				src/qprocesssender.cpp
				
CONFIG(debug, debug|release) { 
    win32:LIBS += \
    							../../../h9000/bin/libglobald1.lib	\
    							../../../h9000/bin/liblogplusd1.lib	\
    							../../../h9000/bin/libutild1.lib
    							
    unix:LIBS += -L../../../h9000/lib -lglobald -llogplusd -lutild
}
CONFIG(release, debug|release) { 
    win32:LIBS += \
    							../../../h9000/bin/libglobal1.lib		\
    							../../../h9000/bin/liblogplus1.lib	\
    							../../../h9000/bin/libutil1.lib
    							
    unix:LIBS += -L../../../h9000/lib -lglobal -llogplus -lutil
}
