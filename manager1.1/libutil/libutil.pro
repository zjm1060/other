##
#	@Author zlb
#	@Time	2011-3-1
# @Desc	实用工具类库
##

include( ../config.pri )
include( ./configure.pri )

TEMPLATE = lib

QT += network xml

win32:TARGET = libutil
unix:TARGET = util

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

contains(CONFIG, UtilDll ) {
    CONFIG += dll
} 
else {
    CONFIG += staticlib
}

win32:UtilDll {
    DEFINES    += QT_DLL UTIL_DLL UTIL_MAKEDLL
}

symbian:UtilDll {
    DEFINES    += QT_DLL UTIL_DLL UTIL_MAKEDLL
}

INCLUDEPATH	+=	\
								./include/	\
								../libglobal/include/		\
								../liblogplus/include/
								

HEADERS +=	\
					include/qabstractalarm.h	\
					include/qabstractcontroller.h		\
					include/qabstractfilecompare.h	\
					include/qabstractmib.h	\
					include/qabstractreadwritexmlfile.h	\
					include/qabstractreceiver.h	\
					include/qalarmfactory.h	\
					include/qconnectionmanager.h	\
					include/qddbwarehouse.h	\
					include/qdeafactory.h		\
					include/qfilecomparefactory.h	\
					include/qfilecontroller.h	\
					include/qglobalinfo.h	\
					include/qidea.h	\
					include/qimessage.h				\
					include/qimulticastmsg.h	\
					include/qmessageparser.h	\
					include/qmibfactory.h	\
					include/qmibmanager.h	\
					include/qmulticastmsgparser.h	\
					include/qprivilegemanager.h	\
					include/qreceiverparser.h	\
					include/qtranslateinstaller.h	\
					include/qtreeitem.h	\
					include/qtreemodel.h	\
					include/qutildef.h	\
					include/qutilreadwritexmlfile.h	\
					include/qviewcontroller.h
				
SOURCES +=	\
					src/qabstractalarm.cpp	\
					src/qabstractcontroller.cpp		\
					src/qabstractfilecompare.cpp	\
					src/qabstractmib.cpp	\
					src/qabstractreadwritexmlfile.cpp	\
					src/qabstractreceiver.cpp	\
					src/qalarmfactory.cpp	\
					src/qconnectionmanager.cpp	\
					src/qddbwarehouse.cpp	\
					src/qdeafactory.cpp		\
					src/qfilecomparefactory.cpp	\
					src/qfilecontroller.cpp	\
					src/qglobalinfo.cpp	\
					src/qmessageparser.cpp	\
					src/qmibfactory.cpp	\
					src/qmibmanager.cpp	\
					src/qmulticastmsgparser.cpp	\
					src/qprivilegemanager.cpp	\
					src/qreceiverparser.cpp	\
					src/qtranslateinstaller.cpp	\
					src/qtreeitem.cpp	\
					src/qtreemodel.cpp	\
					src/qutildef.cpp	\
					src/qutilreadwritexmlfile.cpp	\
					src/qviewcontroller.cpp
					

win32 {
		HEADERS +=	\
						include/qabstractwmi.h	\
						include/qwmifactory.h
						
		SOURCES += \
						src/qabstractwmi.cpp	\
						src/qwmifactory.cpp
 }
 else	{
 		HEADERS +=	\
 						include/qabstractstat.h	\
 						include/qstatfactory.h

 		SOURCES += \
 						src/qabstractstat.cpp	\
 						src/qstatfactory.cpp
 }

TRANSLATIONS = libutil_en.ts libutil_zh.ts
				
CONFIG(debug, debug|release) { 
    win32:LIBS += \
    							../../../h9000/bin/libglobald1.lib	\
    							../../../h9000/bin/liblogplusd1.lib
    							
    unix:LIBS += -L../../../h9000/lib -lglobald -llogplusd
}
CONFIG(release, debug|release) { 
    win32:LIBS += \
    							../../../h9000/bin/libglobal1.lib	\
    							../../../h9000/bin/liblogplus1.lib
    							
    unix:LIBS += -L../../../h9000/lib -lglobal -llogplus
}
