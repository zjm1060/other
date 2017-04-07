##
#		@Author	zlb
#		@Time		2013-8-20
#		@Desc 	¿Ø¼þ
##

include( ./configure.pri )
include( ../config.pri )
include( ../extconfig.pri )

TEMPLATE = lib

QT += network xml

CONFIG += thread

win32:TARGET = libctl
unix:TARGET = ctl

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

UI_DIR          = $$OUTPUT_DIR/ui


contains(CONFIG, CtlDll ) {
    CONFIG += dll
    
    win32|symbian: DEFINES    += QT_DLL CTL_DLL CTL_MAKEDLL
} 
else {
    CONFIG += staticlib
}

INCLUDEPATH +=	\
								./include/	\
								../libglobal/include/		\
								../liblogplus/include/	\
								../libutil/include/			\
								../libnetwork/include/

HEADERS +=	\
						include/libctlglobal.h	\
						include/qbuttonlist.h	\
						include/qcomboboxfilesystem.h	\
						include/qdiskdelegate.h	\
						include/qdlgnewtask.h	\
						include/qfileicondelegate.h	\
						include/qframefilesystem.h	\
						include/qframesystem.h	\
						include/qlinkbutton.h	\
						include/qnumberdelegate.h	\
						include/qtreeviewfilesystem.h	

SOURCES +=	\
						src/qbuttonlist.cpp	\
						src/qcomboboxfilesystem.cpp	\
						src/qdiskdelegate.cpp	\
						src/qdlgnewtask.cpp	\
						src/qfileicondelegate.cpp	\
						src/qframefilesystem.cpp	\
						src/qframesystem.cpp	\
						src/qlinkbutton.cpp	\
						src/qnumberdelegate.cpp		\
						src/qtreeviewfilesystem.cpp	
 						
FORMS   +=	\
						ui/newtask.ui	\
						ui/system.ui
						

RESOURCES = libctl.qrc
TRANSLATIONS = libctl_en.ts libctl_zh.ts

CONFIG(debug, debug|release) { 
    win32:LIBS += ../../../h9000/bin/libglobald1.lib	\
    							../../../h9000/bin/liblogplusd1.lib	\
    							../../../h9000/bin/libnetworkd1.lib	\
    							../../../h9000/bin/libutild1.lib
    							
    unix:LIBS += -L../../../h9000/lib -lglobald -llogplusd -lnetworkd -lutild
}

CONFIG(release, debug|release) { 
    win32:LIBS += ../../../h9000/bin/libglobal1.lib	\
    							../../../h9000/bin/liblogplus1.lib	\
    							../../../h9000/bin/libnetwork1.lib	\
    							../../../h9000/bin/libutil1.lib
    							
    unix:LIBS += -L../../../h9000/lib -lglobal -llogplus -lnetwork -lutil
}

