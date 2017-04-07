##
#	@Author zlb
#	@Time	2012-08-22
# @Desc	º”√‹À„∑®
##

include( ./configure.pri )
include( ../config.pri )

TEMPLATE = lib

win32:TARGET = libdea
unix:TARGET = dea

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

contains(CONFIG, DeaDll ) {
    CONFIG += dll
} 
else {
    CONFIG += staticlib
}

win32:DeaDll {
    DEFINES    += QT_DLL DEA_DLL DEA_MAKEDLL
}

symbian:DeaDll {
    DEFINES    += QT_DLL DEA_DLL DEA_MAKEDLL
}						

INCLUDEPATH	+=	\
								./include/	\
								../libglobal/include/		\
								../liblogplus/include/	\
								../libutil/include/
								

HEADERS +=	\
						include/extern.h	\
						include/global.h	\
						include/qabstractdes.h	\
						include/qdeaplaintext.h	\
						include/qdes.h		\
						include/qtripledes.h
				
SOURCES +=	\
						src/extern.cpp		\
						src/qabstractdes.cpp	\
						src/qdeaplaintext.cpp	\
						src/qdes.cpp		\
						src/qtripledes.cpp

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
