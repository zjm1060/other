##
#	@Author zlb
#	@Time	2011-8-25
# @Desc	日志系统
##

include( ./configure.pri )
include( ../config.pri )

TEMPLATE = lib

TARGET = logplus

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

contains(CONFIG, LogPlusDll ) {
    CONFIG += dll
} 
else {
    CONFIG += staticlib
}

win32:LogPlusDll {
    DEFINES    += QT_DLL LOGPLUS_DLL LOGPLUS_MAKEDLL
}

symbian:LogPlusDll {
    DEFINES    += QT_DLL LOGPLUS_DLL LOGPLUS_MAKEDLL
}

INCLUDEPATH	+=	../global/include	\
								include/
								

HEADERS +=		\
							include/qappender.h		\
							include/qfilter.h	\
							include/qloggingevent.h	\
							include/qloglevelmanager.h	\
							include/qloglayout.h	\
							include/qloglog.h	\
							include/qndc.h		\
							include/qlogextern.h	\
							include/qappenderattachable.h		\
							include/qconsoleappender.h	\
							include/qfileappender.h			\
							include/qhierarchy.h	\
							include/qlogger.h	\
							include/qloggerfactory.h		\
							include/qloggerimplement.h	\
							include/qpatternloglayout.h	\
							include/qstreams.h
				
SOURCES +=		\
							src/qappender.cpp		\
							src/qfilter.cpp	\
							src/qloggingevent.cpp	\
							src/qloglevelmanager.cpp	\
							src/qloglayout.cpp	\
							src/qloglog.cpp	\
							src/qndc.cpp		\
							src/qlogextern.cpp	\
							src/qappenderattachable.cpp	\
							src/qconsoleappender.cpp	\
							src/qfileappender.cpp			\
							src/qhierarchy.cpp	\
							src/qlogger.cpp	\
							src/qloggerfactory.cpp		\
							src/qloggerimplement.cpp	\
							src/qpatternloglayout.cpp	\
							src/qstreams.cpp
							

CONFIG(debug, debug|release) { 
    win32:LIBS += \
    							-L../../../h9000/bin globald1.lib
    							
    unix:LIBS += -L../../../h9000/lib -lglobald
}
CONFIG(release, debug|release) { 
    win32:LIBS += \
    							-L../../../h9000/bin global1.lib
    							
    unix:LIBS += -L../../../h9000/lib -lglobal
}
