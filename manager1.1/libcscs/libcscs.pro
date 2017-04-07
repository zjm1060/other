##
#	@Author zlb
#	@Time	2012-11-05
# @Desc	与H9000相关功能
##

include( ../config.pri )
include( ./configure.pri )

TEMPLATE = lib

win32:TARGET = libcscs
unix:TARGET = cscs

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

contains(CONFIG, CscsDll ) {
    CONFIG += dll
} 
else {
    CONFIG += staticlib
}

contains(DEFINES, H9000_ENABLE ) {
    unix:QMAKE_LFLAGS	+= -ldl
} 

win32:CscsDll {
    DEFINES    += QT_DLL CSCS_DLL CSCS_MAKEDLL
}

symbian:CscsDll {
    DEFINES    += QT_DLL CSCS_DLL CSCS_MAKEDLL
}

INCLUDEPATH	+=	\
								./include/	\
								../libglobal/include/		\
								../liblogplus/include/	\
								../libutil/include/			\
								../../dms/v4.0a/inc			\
								../../lan/v4.0a/inc			\
								../../dps/v4.0a/inc

HEADERS +=	\
						include/global.h	\
						include/qcscsdbscode.h	\
						include/qcscsinitialize.h
				
SOURCES +=	\
						src/qcscsdbscode.cpp	\
						src/qcscsinitialize.cpp
	
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

unix:LIBS += -lxml2

win32:LIBS += \
    							../../dps/v4.0a/lib/dpslib.lib	\
    							../../lan/v4.0a/lib/lan.lib	\
    							../../dms/v4.0a/lib/dblib.lib	\
    							../../dps/v4.0a/lib/ipclib.lib	\
    							../../syslog/v4.0a/lib/syslog.lib
    					
win32:LIBS += /NODEFAULTLIB:"LIBCMTD.lib"
    		
unix:LIBS += \
    							../../dps/v4.0a/lib/dpslib.lib	\
    							../../lan/v4.0a/lib/lan.lib	\
    							../../dms/v4.0a/lib/dblib.lib	\
    							../../dps/v4.0a/lib/ipclib.lib	\
    							../../syslog/v4.0a/lib/syslog.lib
    							