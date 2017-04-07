##
#	@Author zlb
#	@Time	2012-05-10
# @Desc	文件对比功能库
##

include( ./configure.pri )
include( ../config.pri )

TEMPLATE = lib

QT += xml

win32:TARGET = libfc
unix:TARGET = fc

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

contains(CONFIG, FcDll ) {
    CONFIG += dll
} 
else {
    CONFIG += staticlib
}

win32:MibDll {
    DEFINES    += QT_DLL FC_DLL FC_MAKEDLL
}

symbian:MibDll {
    DEFINES    += QT_DLL FC_DLL FC_MAKEDLL
}						

INCLUDEPATH	+=	\
								./include/	\
								../libglobal/include/		\
								../liblogplus/include/	\
								../libutil/include/

HEADERS +=	\
						include/qbinaryfilecompare.h		\
						include/qcmddeffilecompare.h		\
						include/qdbiodeffilecompare.h		\
						include/qfcextern.h	\
						include/qgenfilecompare.h		\
						include/qgroupdeffilecompare.h	\
						include/qmsgdeffilecompare.h		\
						include/qrecorddeffilecompare.h	\
						include/qstationdeffilecompare.h	\
						include/qtextfilecompare.h

SOURCES += 	\
						src/qbinaryfilecompare.cpp		\
						src/qcmddeffilecompare.cpp		\
						src/qdbiodeffilecompare.cpp		\
						src/qfcextern.cpp		\
						src/qgenfilecompare.cpp		\
						src/qgroupdeffilecompare.cpp	\
						src/qmsgdeffilecompare.cpp		\
						src/qrecorddeffilecompare.cpp	\
						src/qstationdeffilecompare.cpp	\
						src/qtextfilecompare.cpp

TRANSLATIONS = libfc_en.ts libfc_zh.ts

CONFIG(debug, debug|release) { 
    win32:LIBS += ../../../h9000/bin/libglobald1.lib		\
    							../../../h9000/bin/liblogplusd1.lib		\
    							../../../h9000/bin/libutild1.lib
    							
    unix:LIBS += -L../../../h9000/lib -lglobald -llogplusd -lutild
}

CONFIG(release, debug|release) { 
    win32:LIBS += ../../../h9000/bin/libglobal1.lib		\
    							../../../h9000/bin/liblogplus1.lib	\
    							../../../h9000/bin/libutil1.lib
    							
    unix:LIBS += -L../../../h9000/lib -lglobal -llogplus -lutil
}
