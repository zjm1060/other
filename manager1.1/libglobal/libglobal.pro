##
#	@Author zlb
#	@Time	2011-8-30
# @Desc	通用功能
##

include( ./configure.pri )
include( ../config.pri )

TEMPLATE = lib

QT += network

win32:TARGET = libglobal
unix:TARGET = global

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

contains(CONFIG, GlobalDll ) {
    CONFIG += dll
} 
else {
    CONFIG += staticlib
}

win32:LogPlusDll {
    DEFINES    += QT_DLL GLOBAL_DLL GLOBAL_MAKEDLL
}

symbian:LogPlusDll {
    DEFINES    += QT_DLL GLOBAL_DLL GLOBAL_MAKEDLL
}						

INCLUDEPATH	+=	\
								include/

HEADERS +=	\
						include/global.h	\
						include/macro.h		\
						include/define.h	\
						include/common.h	\
						include/qdirex.h	\
						include/qfactory.h	\
						include/qfileex.h	\
						include/qsharedobject.h		\
						include/qproperties.h
				
SOURCES +=	\
						src/common.cpp		\
						src/qdirex.cpp		\
						src/qfactory.cpp	\
						src/qfileex.cpp		\
						src/qsharedobject.cpp		\
						src/qproperties.cpp

