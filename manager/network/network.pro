##
#	@Author zlb
#	@Time	2011-3-1
# @Desc	µ×²ãÍ¨Ñ¶
##

include( ../config.pri )
include( ./configure.pri )

TEMPLATE = lib

QT += network

TARGET = network

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
								../global/include/	\
								../logplus/include/	\
								./include/
								
								
								

HEADERS +=	\
				include/qheartbeat.h			\
				include/qserver.h					\
				include/qconnection.h			\
				include/qclient.h					\
				include/qclientmanager.h	\
				include/qservermanager.h	\
				include/qimessage.h				\
				include/qmessageparser.h
				
SOURCES +=	\
				src/qheartbeat.cpp			\
				src/qserver.cpp					\
				src/qconnection.cpp			\
				src/qclient.cpp					\
				src/qclientmanager.cpp	\
				src/qservermanager.cpp	\
				src/qmessageparser.cpp
				
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
