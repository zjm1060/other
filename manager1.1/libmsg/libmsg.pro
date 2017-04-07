##
#	@Author zlb
#	@Time	2011-12-06
# @Desc	通讯消息
##

include( ./configure.pri )
include( ../config.pri )

TEMPLATE = lib

QT += network xml

win32:TARGET = libmsg
unix:TARGET = msg

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

contains(CONFIG, MsgDll ) {
    CONFIG += dll
} 
else {
    CONFIG += staticlib
}

win32:MsgDll {
    DEFINES    += QT_DLL MSG_DLL MSG_MAKEDLL
}

symbian:MsgDll {
    DEFINES    += QT_DLL MSG_DLL MSG_MAKEDLL
}						

INCLUDEPATH	+=	\
								./include/	\
								../libglobal/include/		\
								../liblogplus/include/	\
								../libutil/include/
								
contains(DEFINES, H9000_ENABLE ) {
    	INCLUDEPATH	+= \
    									../libcscs/include	\
    									../../dms/v4.0a/inc			\
											../../lan/v4.0a/inc			\
											../../dps/v4.0a/inc
}

HEADERS +=	\
						include/qmessagecomputersystem.h	\
						include/qmessageconfigure.h	\
						include/qmessagecontrolled.h	\
						include/qmessagecscs.h	\
						include/qmessageddb.h		\
						include/qmessagedisk.h		\
						include/qmessageextern.h	\
						include/qmessagefiletransfer.h	\
						include/qmessagelocalserver.h		\
						include/qmessagememory.h	\
						include/qmessagenetworkadapter.h	\
						include/qmessageprocess.h	\
						include/qmessageprocessor.h	\
						include/qmessageremoteserver.h	\
						include/qmessagestatuschanged.h	\
						include/qmessagesystem.h		\
						include/qmessagetopology.h	\
						include/qmulticastmsgelect.h
				
SOURCES +=	\
						src/qmessagecomputersystem.cpp		\
						src/qmessageconfigure.cpp	\
						src/qmessagecontrolled.cpp	\
						src/qmessagecscs.cpp	\
						src/qmessageddb.cpp		\
						src/qmessagedisk.cpp	\
						src/qmessageextern.cpp		\
						src/qmessagefiletransfer.cpp		\
						src/qmessagelocalserver.cpp	\
						src/qmessagememory.cpp	\
						src/qmessagenetworkadapter.cpp		\					
						src/qmessageprocess.cpp		\
						src/qmessageprocessor.cpp	\
						src/qmessageremoteserver.cpp	\
						src/qmessagestatuschanged.cpp	\
						src/qmessagesystem.cpp		\
						src/qmessagetopology.cpp	\
						src/qmulticastmsgelect.cpp

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
