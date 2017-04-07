##
#		@Author	zlb
#		@Time		2011-3-19
#		@Desc 	ΩÁ√Ê
##

include( ./configure.pri )
include( ../config.pri )

TEMPLATE = app

QT += network

CONFIG += thread

TARGET = manager

build_pass:CONFIG(debug, debug|release):TARGET = $$join(TARGET,,,_d)

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

INCLUDEPATH +=	../global/include/		\
								../logplus/include/		\
								../topology/include/	\
								../network/include/		
								
								

HEADERS +=	extern.h		\
						qmainwnd.h	\
						qnontopomainwnd.h		\
						qbuttonlist.h		\
						qlistgroupwidget.h	\
						qcomboboxfilesystem.h		\
						qabstractfiletransfer.h	\
						qfiletransferparser.h	\
						qwndmanager.h		\
						qsystemmanager.h	\
						qprocessmanager.h	\
						qresourcemanager.h	\
						qprocessmanager.h		\
						qsyncmanager.h			\
						qwndgroupmanager.h	\
						qsyncgroupmanager.h		\
						qframecurve.h		\
						qframesystem.h	\
						qframenetwork.h	\
						qframesystemprocess.h	\
						qdlgnewtask.h		\
						qdlgbrowsefolders.h		\
						qframeh9000process.h	\
						qframesystemprocess.h	\
						qframeprocessor.h			\
						qframedisk.h				\
						qframememory.h			\
						qframesyncfile.h		\
						qabstractsync.h			\
						qframefilesystem.h	\
						qframegroupsync.h		\
						qframeremotefilesystem.h	\
						qgrouptransferqueue.h		\
						qtransferinstructor.h		\
						qtreeitem.h					\
						qtreemodel.h				\
						qtreeviewfilesystem.h	\
						qtreeviewtransferqueue.h	\
						qtipsdelegate.h			\
						qserverdelegate.h		\
						qfilecontroller.h		\
						qviewcontroller.h		\
						qprocesssender.h		\
						qmessageprocessor.h		\
						qmessagefiletransfer.h	\
						qmessagecomputersystem.h	\
						qmessagesystem.h		\
						qmessagenetworkadapter.h	\
						qmessageprocess.h		\
						qmessagedisk.h	\
						qmessagememory.h		\
						qprocesssender.h		\
						qmessageperformer.h	\
						qprocessorsender.h	\
						qmemorysender.h			\
						qtempsavepath.h			\
						qabstractgraphicsitem.h	\
						qgraphicsitemrouter.h	\
						qgraphicsitemswitch.h	\
						qgraphicsitemhost.h		\
						qgraphicsitemlink.h		\
						qwndgraphics.h
						

SOURCES +=	main.cpp		\
						extern.cpp		\
						qmainwnd.cpp	\
						qnontopomainwnd.cpp		\
						qbuttonlist.cpp		\
						qlistgroupwidget.cpp	\
						qcomboboxfilesystem.cpp		\
						qabstractfiletransfer.cpp	\
						qfiletransferparser.cpp	\
						qwndmanager.cpp		\
						qsystemmanager.cpp	\
						qprocessmanager.cpp	\
						qresourcemanager.cpp	\
						qprocessmanager.cpp		\
						qsyncmanager.cpp	\
						qwndgroupmanager.cpp	\
						qsyncgroupmanager.cpp	\
						qframecurve.cpp		\
						qframesystem.cpp	\
						qframenetwork.cpp	\
						qframesystemprocess.cpp	\
						qdlgnewtask.cpp			\
						qdlgbrowsefolders.cpp		\
						qframeh9000process.cpp	\
						qframesystemprocess.cpp	\
						qframeprocessor.cpp			\
						qframedisk.cpp				\
						qframememory.cpp			\
						qframesyncfile.cpp		\
						qabstractsync.cpp			\
						qframefilesystem.cpp	\
						qframegroupsync.cpp		\
						qframeremotefilesystem.cpp	\
						qgrouptransferqueue.cpp			\
						qtransferinstructor.cpp			\
						qtreeitem.cpp					\
						qtreemodel.cpp				\
						qtreeviewfilesystem.cpp	\
						qtreeviewtransferqueue.cpp	\
						qtipsdelegate.cpp			\
						qserverdelegate.cpp		\
						qfilecontroller.cpp		\
						qviewcontroller.cpp		\
						qprocesssender.cpp		\
						qmessageprocessor.cpp	\
						qmessagecomputersystem.cpp	\
						qmessagesystem.cpp	\
						qmessagefiletransfer.cpp	\
						qmessagenetworkadapter.cpp	\
						qmessageprocess.cpp	\
						qmessagedisk.cpp	\
						qmessagememory.cpp	\
						qmessageperformer.cpp	\
						qprocesssender.cpp	\
						qprocessorsender.cpp	\
						qmemorysender.cpp			\
						qtempsavepath.cpp			\
						qabstractgraphicsitem.cpp	\
						qgraphicsitemrouter.cpp	\
						qgraphicsitemswitch.cpp	\
						qgraphicsitemhost.cpp		\
						qgraphicsitemlink.cpp		\
						qwndgraphics.cpp
						
						

win32 {
		HEADERS +=	qabstractwmi.h	\
						qwmifactory.h		\
						qwmiprocessor.h	\
						qwmiperfprocessor.h		\
						qwmicomputersystem.h	\
						qwmisystem.h		\
						qwminetworkadapter.h	\
						qwmiprocess.h		\
						qwmidiskdrive.h	\
						qwmicdromdrive.h	\
						qwmilogicaldisk.h	\
						qwmiphysicalmemory.h	\
						qwmisystemmemory.h
						
		SOURCES += qabstractwmi.cpp	\
						qwmifactory.cpp		\
						qwmiprocessor.cpp		\
						qwmiperfprocessor.cpp		\
						qwmicomputersystem.cpp	\
						qwmisystem.cpp		\
						qwminetworkadapter.cpp	\
						qwmiprocess.cpp		\
						qwmidiskdrive.cpp	\
						qwmicdromdrive.cpp	\
						qwmilogicaldisk.cpp	\
						qwmiphysicalmemory.cpp	\
						qwmisystemmemory.cpp
 }
 else	{
 		HEADERS +=	qstatfactory.h	\
 						qabstractstat.h			\
 						qstatsystem.h				\
 						qstatcomputersystem.h	\
 						qstatnetworkadapter.h	\
 						qstatprocess.h			\
 						qstatprocessor.h		\
 						qstatperfprocessor.h	\
 						qstatphysicalmemory.h	\
 						qstatsystemmemory.h		\
 						qstatlogicaldisk.h		\
 						qstatdiskdrive.h		\
 						qstatcdromdrive.h
 						
 		SOURCES += qstatfactory.cpp	\
 						qabstractstat.cpp		\
 						qstatsystem.cpp			\
 						qstatcomputersystem.cpp	\
 						qstatnetworkadapter.cpp	\
 						qstatprocess.cpp		\
 						qstatprocessor.cpp	\
 						qstatperfprocessor.cpp	\
 						qstatphysicalmemory.cpp	\
 						qstatsystemmemory.cpp		\
 						qstatlogicaldisk.cpp		\
 						qstatdiskdrive.cpp			\
 						qstatcdromdrive.cpp
 }
 						
FORMS   +=	ui/processor.ui		\
						ui/system.ui	\
						ui/network.ui	\
						ui/systemprocess.ui	\
						ui/h9000process.ui	\
						ui/newtask.ui	\
						ui/browsefolders.ui	\
						ui/disk.ui		\
						ui/memory.ui	\
						ui/syncfile.ui	\
						ui/syncgroupfile.ui
						

RESOURCES = src.qrc
TRANSLATIONS = manager_en.ts manager_zh.ts

CONFIG(debug, debug|release) { 
    win32:LIBS += ../../../h9000/bin/globald1.lib		\
    							../../../h9000/bin/logplusd1.lib	\
    							../../../h9000/bin/networkd1.lib	\
    							../../../h9000/bin/topologyd1.lib
    							
    unix:LIBS += -L../../../h9000/lib -lglobald -llogplusd -lnetworkd	-ltopologyd
}
CONFIG(release, debug|release) { 
    win32:LIBS += ../../../h9000/bin/global1.lib	\
    							../../../h9000/bin/logplus1.lib	\
    							../../../h9000/bin/network1.lib	\
    							../../../h9000/bin/topology1.lib
    							
    unix:LIBS += -L../../../h9000/lib -lglobal -llogplus -lnetwork	-ltopology
}

unix {
	UNAME = $$system(uname -s)
	contains( UNAME, SunOS){
		SUB_UNAME = $$system(uname -p)
		!contains( SUB_UNAME, sparc):DEFINES+= Q_OS_SOLARIS_X86
		contains( SUB_UNAME, sparc){
			DEFINES+= Q_OS_SOLARIS_SPARC
			
			UNAME_MODEL = $$system(uname -m)
			contains( UNAME_MODEL, sun4u):DEFINES+= Q_OS_SOLARIS_SPARC_SUN4U
			contains( UNAME_MODEL, sun4v):DEFINES+= Q_OS_SOLARIS_SPARC_SUN4V
		}
		
		LIBS+=-lkstat -lpicl
		
		contains( DEFINES, Q_OS_SOLARIS_X86):LIBS +=-lsmbios
	}
	
	contains( UNAME, AIX):LIBS +=-lperfstat
}
