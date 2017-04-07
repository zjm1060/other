##
#		@Author	zlb
#		@Time		2011-3-19
#		@Desc 	manager管理服务器
##

include( ./configure.pri )
include( ../config.pri )
include( ../extconfig.pri )

TEMPLATE = app

QT += network

CONFIG += console thread

TARGET = mgrsvc

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

INCLUDEPATH +=	\
								./include	\
								../libglobal/include/		\
								../liblogplus/include/	\
								../libnetwork/include/	\
								../libutil/include/			\
								../libddb/include/

contains(DEFINES, TOPOLOGY_ENABLE ) {
    	INCLUDEPATH +=	../libtopology/include/
}
								
HEADERS +=	\
						include/qmgrsrc.h	\
						include/qmibsrc.h	\
						include/qpluginmanager.h
						

SOURCES +=	\
						src/main.cpp		\
						src/qmgrsrc.cpp	\
						src/qmibsrc.cpp	\
						src/qpluginmanager.cpp	
						

CONFIG(debug, debug|release) { 
    win32:LIBS += ../../../h9000/bin/libglobald1.lib		\
    							../../../h9000/bin/liblogplusd1.lib		\
    							../../../h9000/bin/libnetworkd1.lib		\
    							../../../h9000/bin/libutild1.lib			\
    							../../../h9000/bin/libddbd1.lib
    							
    unix:LIBS += -L../../../h9000/lib -lglobald -llogplusd -lnetworkd -lutild -lddbd
    
    contains(DEFINES, TOPOLOGY_ENABLE ) {
    	win32:LIBS +=	../../../h9000/bin/libtopologyd1.lib
    	
    	unix:LIBS +=	-ltopologyd
		}
}

CONFIG(release, debug|release) { 
    win32:LIBS += ../../../h9000/bin/libglobal1.lib		\
    							../../../h9000/bin/liblogplus1.lib	\
    							../../../h9000/bin/libnetwork1.lib	\
    							../../../h9000/bin/libutil1.lib			\
    							../../../h9000/bin/libddb1.lib
    							
    unix:LIBS += -L../../../h9000/lib -lglobal -llogplus -lnetwork -lutil -lddb
    
    contains(DEFINES, TOPOLOGY_ENABLE ) {
    	win32:LIBS +=	../../../h9000/bin/libtopology1.lib
    	
    	unix:LIBS +=	-ltopology
		}
}

