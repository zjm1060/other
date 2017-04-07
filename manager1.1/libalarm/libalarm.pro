##
#	@Author zlb
#	@Time	2012-10-15
# @Desc	±¨¾¯ÏµÍ³
##

include( ../config.pri )
include( ./configure.pri )

TEMPLATE = lib

QT += xml

win32:TARGET = libalarm
unix:TARGET = alarm

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

contains(CONFIG, AlarmDll ) {
    CONFIG += dll
} 
else {
    CONFIG += staticlib
}

win32:AlarmDll {
    DEFINES    += QT_DLL ALARM_DLL ALARM_MAKEDLL
}

symbian:AlarmDll {
    DEFINES    += QT_DLL ALARM_DLL ALARM_MAKEDLL
}

INCLUDEPATH	+=	\
								./include/	\
								../libglobal/include/		\
								../liblogplus/include/	\
								../libutil/include/

contains(DEFINES, H9000_ENABLE ) {
    INCLUDEPATH	+=	\
								../libcscs/include/
} 

HEADERS +=	\
						include/extern.h	\
						include/global.h	\
						include/qreadwritealarmfromxmlfile.h
				
SOURCES +=	\
						src/extern.cpp	\
						src/qreadwritealarmfromxmlfile.cpp	
						
contains(DEFINES, H9000_ENABLE ) {
    HEADERS +=	\
						include/qalarmh9000event.h
				
		SOURCES +=	\
						src/qalarmh9000event.cpp
} 					
				
CONFIG(debug, debug|release) { 
    win32:LIBS += \
    							../../../h9000/bin/libglobald1.lib	\
    							../../../h9000/bin/liblogplusd1.lib	\
    							../../../h9000/bin/libutild1.lib
    				
    		
    unix:LIBS += -L../../../h9000/lib -lglobald -llogplusd -lutild
    
    contains(DEFINES, H9000_ENABLE ) {
    	win32:LIBS += \
    								../../../h9000/bin/libcscsd1.lib
		
			unix:LIBS += -lcscsd
		}
}

CONFIG(release, debug|release) { 
    win32:LIBS += \
    							../../../h9000/bin/libglobal1.lib		\
    							../../../h9000/bin/liblogplus1.lib	\
    							../../../h9000/bin/libutil1.lib
    							
    unix:LIBS += -L../../../h9000/lib -lglobal -llogplus -lutil
    
    contains(DEFINES, H9000_ENABLE ) {
    	win32:LIBS += \
    								../../../h9000/bin/libcscs1.lib
		
			unix:LIBS += -lcscs
		}
}
    				
