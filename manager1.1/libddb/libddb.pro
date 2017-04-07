##
#	@Author zlb
#	@Time	2012-8-14
# @Desc	分布式数据库
##

include( ../config.pri )
include( ./configure.pri )

TEMPLATE = lib

QT += network

win32:TARGET = libddb
unix:TARGET = ddb

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

contains(CONFIG, DdbDll ) {
    CONFIG += dll
} 
else {
    CONFIG += staticlib
}

win32:DdbDll {
    DEFINES    += QT_DLL DDB_DLL DDB_MAKEDLL
}

symbian:DdbDll {
    DEFINES    += QT_DLL DDB_DLL DDB_MAKEDLL
}


INCLUDEPATH	+=	\
								./include/	\
								../libglobal/include/		\
								../liblogplus/include/	\
								../libnetwork/include/	\
								../libutil/include/
								

HEADERS +=	\
						include/global.h	\
						include/qddbcontroller.h	\
						include/qddbmanager.h	\
						include/qleaderelection.h	\
						include/qmulticastview.h
				
SOURCES +=	\
						src/qddbcontroller.cpp	\
						src/qddbmanager.cpp	\
						src/qleaderelection.cpp	\
						src/qmulticastview.cpp
						
					
				
CONFIG(debug, debug|release) { 
    win32:LIBS += \
    							../../../h9000/bin/libglobald1.lib	\
    							../../../h9000/bin/liblogplusd1.lib	\
    							../../../h9000/bin/libnetworkd1.lib	\
    							../../../h9000/bin/libutild1.lib
    							
    unix:LIBS += -L../../../h9000/lib -lglobald -llogplusd -lnetworkd -lutild
}

CONFIG(release, debug|release) { 
    win32:LIBS += \
    							../../../h9000/bin/libglobal1.lib		\
    							../../../h9000/bin/liblogplus1.lib	\
    							../../../h9000/bin/libnetwork1.lib	\
    							../../../h9000/bin/libutil1.lib
    							
    unix:LIBS += -L../../../h9000/lib -lglobal -llogplus -lnetwork -lutil
}
