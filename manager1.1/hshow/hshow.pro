##
#		@Author	zlb
#		@Time		2013-6-4
#		@Desc 	×ÀÃæ¹¤¾ß
##

include( ./configure.pri )
include( ../config.pri )
include( ../extconfig.pri )

TEMPLATE = app

QT += network xml

CONFIG += thread

win32:DEFINES    += CTL_DLL

TARGET = hshow

build_pass:CONFIG(debug, debug|release):TARGET = $$join(TARGET,,,_d)
build_pass:unix:TARGET = $$join(TARGET,,,.$$SUFFIX)

OUTPUT_DIR			=

CONFIG(debug, debug|release){
	OUTPUT_DIR		=	debug
}

CONFIG(release, debug|release){
	OUTPUT_DIR		=	release
}

MOC_DIR       	= $$OUTPUT_DIR/moc
OBJECTS_DIR  		= $$OUTPUT_DIR/obj
DESTDIR					= ../../../h9000/bin
UI_DIR          = $$OUTPUT_DIR/ui

INCLUDEPATH +=	\
								./include/	\
								../libglobal/include/		\
								../liblogplus/include/	\
								../libutil/include/			\
								../libnetwork/include/	\
								../libctl/include/	

HEADERS +=	\
						include/qabstractpanel.h		\
						include/qfloatingdockwidget.h		\
						include/qframefilesystempanel.h	\
						include/qframeprocesspanel.h	\
						include/qhshow.h	\
						include/qipcsocket.h		\
						include/qpanelcomputer.h		\
						include/qpaneldatetime.h		\
						include/qpanelfactory.h			\
						include/qpanelfilesystem.h	\
						include/qpanelprocess.h			\
						include/qpanelmachine.h			\
						include/qpanelmenu.h				\
						include/qpaneltools.h				\
						include/qpaneltrash.h				\
						include/qreadwritexmlfile.h

SOURCES +=	\
						src/main.cpp	\
						src/qabstractpanel.cpp		\
						src/qfloatingdockwidget.cpp		\
						src/qframefilesystempanel.cpp	\
						src/qframeprocesspanel.cpp	\
						src/qhshow.cpp	\
						src/qipcsocket.cpp	\
						src/qpanelcomputer.cpp		\
						src/qpaneldatetime.cpp		\
						src/qpanelfactory.cpp			\
						src/qpanelfilesystem.cpp	\
						src/qpanelprocess.cpp			\
						src/qpanelmachine.cpp			\
						src/qpanelmenu.cpp				\
						src/qpaneltools.cpp				\
						src/qpaneltrash.cpp				\
						src/qreadwritexmlfile.cpp	
 						
FORMS   +=	\
						ui/filesystem.ui	\
						ui/processes.ui
						

RESOURCES = ./hshow.qrc

TRANSLATIONS = hshow_en.ts hshow_zh.ts

CONFIG(debug, debug|release) { 
    win32:LIBS += ../../../h9000/bin/libglobald1.lib	\
    							../../../h9000/bin/liblogplusd1.lib	\
    							../../../h9000/bin/libnetworkd1.lib	\
    							../../../h9000/bin/libutild1.lib		\
    							../../../h9000/bin/libctld1.lib	
    							
    unix:LIBS += -L../../../h9000/lib -lglobald -llogplusd -lnetworkd -lutild -lctld
}
CONFIG(release, debug|release) { 
    win32:LIBS += ../../../h9000/bin/libglobal1.lib	\
    							../../../h9000/bin/liblogplus1.lib	\
    							../../../h9000/bin/libnetwork1.lib	\
    							../../../h9000/bin/libutil1.lib			\
    							../../../h9000/bin/libctl1.lib	
    							
    unix:LIBS += -L../../../h9000/lib -lglobal -llogplus -lnetwork -lutil -lctl
}

