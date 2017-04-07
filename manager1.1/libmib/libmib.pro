##
#	@Author zlb
#	@Time	2011-12-06
# @Desc	管理信息库
##

include( ./configure.pri )
include( ../config.pri )
include( ../extconfig.pri)

TEMPLATE = lib

win32:TARGET = libmib
unix:TARGET = mib

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

contains(CONFIG, MibDll ) {
    CONFIG += dll
} 
else {
    CONFIG += staticlib
}

win32:MibDll {
    DEFINES    += QT_DLL MIB_DLL MIB_MAKEDLL
}

symbian:MibDll {
    DEFINES    += QT_DLL MIB_DLL MIB_MAKEDLL
}						

INCLUDEPATH	+=	\
								./include/	\
								../libglobal/include/		\
								../liblogplus/include/	\
								../libutil/include/			\
								../libtopology/include/

contains(DEFINES, H9000_ENABLE ) {
    	INCLUDEPATH	+= \
    									../libcscs/include
}
								

HEADERS +=	\
						include/qmibcdromdrive.h	\
						include/qmibcomputersystem.h	\
						include/qmibdiskdrive.h	\
						include/qmibextern.h	\
						include/qmiblogicaldisk.h	\
						include/qmibnetworkadapter.h	\
						include/qmibpefprocessor.h		\
						include/qmibphysicalmemory.h	\
						include/qmibprocess.h	\
						include/qmibprocessor.h	\
						include/qmibremoteserver.h	\
						include/qmibserver.h	\
						include/qmibsystem.h	\
						include/qmibsystemmemory.h	\
						include/qmibtopology.h

SOURCES += \
						src/qmibcdromdrive.cpp	\
						src/qmibcomputersystem.cpp	\
						src/qmibdiskdrive.cpp	\
						src/qmibextern.cpp	\
						src/qmiblogicaldisk.cpp	\
						src/qmibnetworkadapter.cpp	\
						src/qmibpefprocessor.cpp		\
						src/qmibphysicalmemory.cpp	\
						src/qmibprocess.cpp	\
						src/qmibprocessor.cpp	\
						src/qmibremoteserver.cpp	\
						src/qmibserver.cpp	\
						src/qmibsystem.cpp	\
						src/qmibsystemmemory.cpp	\
						src/qmibtopology.cpp

win32 {
		HEADERS +=	\
						include/qwmicdromdrive.h			\
						include/qwmicomputersystem.h	\
						include/qwmidiskdrive.h		\
						include/qwmilogicaldisk.h	\
						include/qwminetworkadapter.h	\
						include/qwmiperfprocessor.h		\
						include/qwmiphysicalmemory.h	\
						include/qwmiprocess.h		\
						include/qwmiprocessor.h	\
						include/qwmisystem.h		\
						include/qwmisystemmemory.h
						
						
		SOURCES += \
						src/qwmicdromdrive.cpp			\
						src/qwmicomputersystem.cpp	\
						src/qwmidiskdrive.cpp		\
						src/qwmilogicaldisk.cpp	\
						src/qwminetworkadapter.cpp	\
						src/qwmiperfprocessor.cpp		\
						src/qwmiphysicalmemory.cpp	\
						src/qwmiprocess.cpp		\
						src/qwmiprocessor.cpp	\
						src/qwmisystem.cpp		\
						src/qwmisystemmemory.cpp
 }
 else	{
 		HEADERS +=	\
 						include/qstatcdromdrive.h		\
 						include/qstatcomputersystem.h	\
 						include/qstatdiskdrive.h		\
 						include/qstatlogicaldisk.h		\
 						include/qstatnetworkadapter.h	\ 						
 						include/qstatperfprocessor.h	\
 						include/qstatphysicalmemory.h	\
 						include/qstatprocess.h			\
 						include/qstatprocessor.h		\
 						include/qstatsystem.h				\
 						include/qstatsystemmemory.h

 		SOURCES += \
 						src/qstatcdromdrive.cpp	\
 						src/qstatcomputersystem.cpp	\
 						src/qstatdiskdrive.cpp			\
 						src/qstatlogicaldisk.cpp		\
 						src/qstatnetworkadapter.cpp	\
 						src/qstatperfprocessor.cpp	\
 						src/qstatphysicalmemory.cpp	\
 						src/qstatprocess.cpp		\
 						src/qstatprocessor.cpp	\			
 						src/qstatsystem.cpp			\
 						src/qstatsystemmemory.cpp
 }

CONFIG(debug, debug|release) { 
    win32:LIBS += ../../../h9000/bin/libglobald1.lib		\
    							../../../h9000/bin/liblogplusd1.lib		\
    							../../../h9000/bin/libutild1.lib			\
    							../../../h9000/bin/libcscsd1.lib
    							
    unix:LIBS += -L../../../h9000/lib -lglobald -llogplusd -lutild -lcscsd
    
    contains(DEFINES, H9000_ENABLE ) {
    	win32:LIBS += \
    								../../../h9000/bin/libcscsd1.lib
		
			unix:LIBS += -lcscsd
		}
}

CONFIG(release, debug|release) { 
    win32:LIBS += ../../../h9000/bin/libglobal1.lib		\
    							../../../h9000/bin/liblogplus1.lib	\
    							../../../h9000/bin/libutil1.lib
    							
    unix:LIBS += -L../../../h9000/lib -lglobal -llogplus -lutil
    
    contains(DEFINES, H9000_ENABLE ) {
    	win32:LIBS += \
    								../../../h9000/bin/libcscs1.lib
		
			unix:LIBS += -lcscs
		}
}

contains( DEFINES, Q_OS_SOLARIS_X86):SUFFIX =xxe
contains( DEFINES, Q_OS_SOLARIS_X86):SUFFIX =xxe

unix {
	contains( UNAME, SunOS){
			LIBS+=-lkstat -lpicl
			
			contains( DEFINES, Q_OS_SOLARIS_X86):LIBS +=-lsmbios
	}
	
	contains( UNAME, AIX){
			HEADERS +=include/qperfstat.h
			SOURCES +=src/qperfstat.cpp
		
			LIBS +=-lperfstat
	}
}