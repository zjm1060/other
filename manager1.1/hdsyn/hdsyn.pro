##
#		@Author	zlb
#		@Time		2012-6-20
#		@Desc 	H9000数据库文件同步管理工具
##

include( ./configure.pri )
include( ../config.pri )
include( ../extconfig.pri )

TEMPLATE = app

QT += network xml

CONFIG += thread

win32:DEFINES    += CTL_DLL

TARGET = hdsyn

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
								../mgr/include \
								../libglobal/include/		\
								../liblogplus/include/	\
								../libmib/include/			\
								../libutil/include/			\
								../libnetwork/include/	\
								../libfc/include/				\
								../libctl/include/

contains(DEFINES, TOPOLOGY_ENABLE ) {
    	INCLUDEPATH +=	../libtopology/include/
}
								
HEADER_FILES_DIR			=	../mgr/include
SRC_FILES_DIR					=	../mgr/src
UI_FILES_DIR					=	../mgr/ui

HEADERS +=	\
						include/qdlglogin.h	\
						include/qhdsyn.h	\
						$$HEADER_FILES_DIR/qabstractmonitor.h	\
						$$HEADER_FILES_DIR/qcomparefiledelegate.h	\
						$$HEADER_FILES_DIR/qconfiguregroupmanager.h	\
						$$HEADER_FILES_DIR/qdelegatesynccommit.h	\
						$$HEADER_FILES_DIR/qdlgbrowsefiles.h	\
						$$HEADER_FILES_DIR/qdlgbrowsefolders.h		\
						$$HEADER_FILES_DIR/qdlgcreategroup.h	\
						$$HEADER_FILES_DIR/qdlgcreateuser.h	\
						$$HEADER_FILES_DIR/qdlgeditfolder.h	\
						$$HEADER_FILES_DIR/qdlgedithost.h	\
						$$HEADER_FILES_DIR/qdlginstantupdate.h		\
						$$HEADER_FILES_DIR/qdlgmodifypassword.h	\
						$$HEADER_FILES_DIR/qfiletransfergroupmanager.h	\
						$$HEADER_FILES_DIR/qframeauthority.h	\	
						$$HEADER_FILES_DIR/qframebasicsettings.h	\					
						$$HEADER_FILES_DIR/qframefilereceiver.h	\
						$$HEADER_FILES_DIR/qframefiletransfergroup.h		\
						$$HEADER_FILES_DIR/qframeinspect.h	\
						$$HEADER_FILES_DIR/qframeinspectsetting.h	\
						$$HEADER_FILES_DIR/qframelocalfilesystem.h	\
						$$HEADER_FILES_DIR/qframeprivileges.h	\
						$$HEADER_FILES_DIR/qframeremotefilesystem.h	\
						$$HEADER_FILES_DIR/qframesynccommand.h	\
						$$HEADER_FILES_DIR/qframesyncfile.h	\
						$$HEADER_FILES_DIR/qframesyncfileshowlog.h	\
						$$HEADER_FILES_DIR/qframesyncgroupcommitfile.h	\
						$$HEADER_FILES_DIR/qframesyncgroupreleasefile.h	\
						$$HEADER_FILES_DIR/qframesyncgrouptransferfile.h	\
						$$HEADER_FILES_DIR/qframesyncgroupupdate.h	\
						$$HEADER_FILES_DIR/qframesyncgroupupdatefile.h	\
						$$HEADER_FILES_DIR/qgrouptransferqueue.h	\
						$$HEADER_FILES_DIR/qgrouptransferqueuedelegate.h	\
						$$HEADER_FILES_DIR/qinfomanager.h	\
						$$HEADER_FILES_DIR/qinspectdelegate.h	\
						$$HEADER_FILES_DIR/qinspectmanager.h	\
						$$HEADER_FILES_DIR/qinspectsettingdelegate.h		\
						$$HEADER_FILES_DIR/qlistviewcontrolledserver.h	\
						$$HEADER_FILES_DIR/qlistviewfilereceiver.h		\
						$$HEADER_FILES_DIR/qlistviewhorizontalicon.h	\
						$$HEADER_FILES_DIR/qlistviewprompt.h	\
						$$HEADER_FILES_DIR/qlistviewsyncfilerequestserver.h	\
						$$HEADER_FILES_DIR/qlistviewsyncfileserver.h	\
						$$HEADER_FILES_DIR/qmainwndprivilege.h	\
						$$HEADER_FILES_DIR/qmainwndsyncfile.h	\
						$$HEADER_FILES_DIR/qmessageboxex.h		\
						$$HEADER_FILES_DIR/qprocesssocket.h	\
						$$HEADER_FILES_DIR/qreadwritexmlfile.h	\
						$$HEADER_FILES_DIR/qreceiverinspect.h	\
						$$HEADER_FILES_DIR/qreceiverprivilege.h	\
						$$HEADER_FILES_DIR/qreceivertransferfile.h	\
						$$HEADER_FILES_DIR/qserverdelegate.h	\
						$$HEADER_FILES_DIR/qsyncfiledelegate.h	\
						$$HEADER_FILES_DIR/qsyncgroupcommitmanager.h	\
						$$HEADER_FILES_DIR/qsyncgroupreleasemanager.h	\
						$$HEADER_FILES_DIR/qsyncgroupupdatemanager.h	\
						$$HEADER_FILES_DIR/qsyncmanager.h	\
						$$HEADER_FILES_DIR/qtipsdelegate.h	\
						$$HEADER_FILES_DIR/qtransferinstructor.h	\
						$$HEADER_FILES_DIR/qtransferqueuedelegate.h	\
						$$HEADER_FILES_DIR/qtreeviewassociationfiles.h	\
						$$HEADER_FILES_DIR/qtreeviewcomparefile.h	\
						$$HEADER_FILES_DIR/qtreeviewgrouptransferdelegate.h	\
						$$HEADER_FILES_DIR/qtreeviewgrouptransferqueue.h		\
						$$HEADER_FILES_DIR/qtreeviewinspect.h	\
						$$HEADER_FILES_DIR/qtreeviewinspectsetting.h	\
						$$HEADER_FILES_DIR/qtreeviewsynccommit.h	\
						$$HEADER_FILES_DIR/qtreeviewsyncconffile.h	\
						$$HEADER_FILES_DIR/qtreeviewsyncdatfile.h	\
						$$HEADER_FILES_DIR/qtreeviewsyncdbfile.h	\
						$$HEADER_FILES_DIR/qtreeviewsyncdspfile.h	\
						$$HEADER_FILES_DIR/qtreeviewsyncfile.h	\
						$$HEADER_FILES_DIR/qtreeviewsynciecfile.h	\
						$$HEADER_FILES_DIR/qtreeviewsyncvoicefile.h	\
						$$HEADER_FILES_DIR/qtreeviewtransferqueue.h	\
						$$HEADER_FILES_DIR/qtreeviewupdateconffile.h	\
						$$HEADER_FILES_DIR/qtreeviewupdatedatfile.h	\
						$$HEADER_FILES_DIR/qtreeviewupdatedbfile.h		\
						$$HEADER_FILES_DIR/qtreeviewupdatedspfile.h	\
						$$HEADER_FILES_DIR/qtreeviewupdatefile.h		\
						$$HEADER_FILES_DIR/qtreeviewupdateiecfile.h	\
						$$HEADER_FILES_DIR/qtreeviewupdatertdbfile.h	\
						$$HEADER_FILES_DIR/qtreeviewupdatevoicefile.h	\
						$$HEADER_FILES_DIR/qwndgroupmanager.h

SOURCES +=	\
						src/main.cpp	\
						src/qdlglogin.cpp	\
						src/qhdsyn.cpp	\
						$$SRC_FILES_DIR/qabstractmonitor.cpp	\
						$$SRC_FILES_DIR/qcomparefiledelegate.cpp	\
						$$SRC_FILES_DIR/qconfiguregroupmanager.cpp	\
						$$SRC_FILES_DIR/qdelegatesynccommit.cpp	\
						$$SRC_FILES_DIR/qdlgbrowsefiles.cpp	\
						$$SRC_FILES_DIR/qdlgbrowsefolders.cpp		\
						$$SRC_FILES_DIR/qdlgcreategroup.cpp	\
						$$SRC_FILES_DIR/qdlgcreateuser.cpp	\
						$$SRC_FILES_DIR/qdlgeditfolder.cpp	\
						$$SRC_FILES_DIR/qdlgedithost.cpp	\						
						$$SRC_FILES_DIR/qdlginstantupdate.cpp		\
						$$SRC_FILES_DIR/qdlgmodifypassword.cpp	\
						$$SRC_FILES_DIR/qfiletransfergroupmanager.cpp	\
						$$SRC_FILES_DIR/qframeauthority.cpp	\		
						$$SRC_FILES_DIR/qframebasicsettings.cpp	\				
						$$SRC_FILES_DIR/qframefilereceiver.cpp	\
						$$SRC_FILES_DIR/qframefiletransfergroup.cpp		\
						$$SRC_FILES_DIR/qframeinspect.cpp	\
						$$SRC_FILES_DIR/qframeinspectsetting.cpp	\
						$$SRC_FILES_DIR/qframelocalfilesystem.cpp	\
						$$SRC_FILES_DIR/qframeprivileges.cpp	\
						$$SRC_FILES_DIR/qframeremotefilesystem.cpp	\
						$$SRC_FILES_DIR/qframesynccommand.cpp	\
						$$SRC_FILES_DIR/qframesyncfile.cpp	\
						$$SRC_FILES_DIR/qframesyncfileshowlog.cpp	\
						$$SRC_FILES_DIR/qframesyncgroupcommitfile.cpp	\
						$$SRC_FILES_DIR/qframesyncgroupreleasefile.cpp	\
						$$SRC_FILES_DIR/qframesyncgrouptransferfile.cpp	\
						$$SRC_FILES_DIR/qframesyncgroupupdate.cpp	\
						$$SRC_FILES_DIR/qframesyncgroupupdatefile.cpp	\
						$$SRC_FILES_DIR/qgrouptransferqueue.cpp	\
						$$SRC_FILES_DIR/qgrouptransferqueuedelegate.cpp	\
						$$SRC_FILES_DIR/qinfomanager.cpp	\
						$$SRC_FILES_DIR/qinspectdelegate.cpp	\
						$$SRC_FILES_DIR/qinspectmanager.cpp	\
						$$SRC_FILES_DIR/qinspectsettingdelegate.cpp		\
						$$SRC_FILES_DIR/qlistviewcontrolledserver.cpp	\
						$$SRC_FILES_DIR/qlistviewfilereceiver.cpp		\
						$$SRC_FILES_DIR/qlistviewhorizontalicon.cpp	\
						$$SRC_FILES_DIR/qlistviewprompt.cpp	\
						$$SRC_FILES_DIR/qlistviewsyncfilerequestserver.cpp	\
						$$SRC_FILES_DIR/qlistviewsyncfileserver.cpp	\
						$$SRC_FILES_DIR/qmainwndprivilege.cpp	\
						$$SRC_FILES_DIR/qmainwndsyncfile.cpp	\
						$$SRC_FILES_DIR/qmessageboxex.cpp		\
						$$SRC_FILES_DIR/qprocesssocket.cpp	\
						$$SRC_FILES_DIR/qreadwritexmlfile.cpp	\
						$$SRC_FILES_DIR/qreceiverinspect.cpp	\
						$$SRC_FILES_DIR/qreceiverprivilege.cpp	\
						$$SRC_FILES_DIR/qreceivertransferfile.cpp	\
						$$SRC_FILES_DIR/qserverdelegate.cpp	\
						$$SRC_FILES_DIR/qsyncfiledelegate.cpp	\
						$$SRC_FILES_DIR/qsyncgroupcommitmanager.cpp	\
						$$SRC_FILES_DIR/qsyncgroupreleasemanager.cpp	\
						$$SRC_FILES_DIR/qsyncgroupupdatemanager.cpp	\
						$$SRC_FILES_DIR/qsyncmanager.cpp	\
						$$SRC_FILES_DIR/qtipsdelegate.cpp	\
						$$SRC_FILES_DIR/qtransferinstructor.cpp		\
						$$SRC_FILES_DIR/qtransferqueuedelegate.cpp	\
						$$SRC_FILES_DIR/qtreeviewassociationfiles.cpp	\
						$$SRC_FILES_DIR/qtreeviewcomparefile.cpp	\
						$$SRC_FILES_DIR/qtreeviewgrouptransferdelegate.cpp	\
						$$SRC_FILES_DIR/qtreeviewgrouptransferqueue.cpp		\
						$$SRC_FILES_DIR/qtreeviewinspect.cpp	\
						$$SRC_FILES_DIR/qtreeviewinspectsetting.cpp	\
						$$SRC_FILES_DIR/qtreeviewsynccommit.cpp	\
						$$SRC_FILES_DIR/qtreeviewsyncconffile.cpp	\
						$$SRC_FILES_DIR/qtreeviewsyncdatfile.cpp	\
						$$SRC_FILES_DIR/qtreeviewsyncdbfile.cpp	\
						$$SRC_FILES_DIR/qtreeviewsyncdspfile.cpp	\
						$$SRC_FILES_DIR/qtreeviewsyncfile.cpp	\
						$$SRC_FILES_DIR/qtreeviewsynciecfile.cpp	\
						$$SRC_FILES_DIR/qtreeviewsyncvoicefile.cpp	\
						$$SRC_FILES_DIR/qtreeviewtransferqueue.cpp	\
						$$SRC_FILES_DIR/qtreeviewupdateconffile.cpp	\
						$$SRC_FILES_DIR/qtreeviewupdatedatfile.cpp	\
						$$SRC_FILES_DIR/qtreeviewupdatedbfile.cpp		\
						$$SRC_FILES_DIR/qtreeviewupdatedspfile.cpp	\
						$$SRC_FILES_DIR/qtreeviewupdatefile.cpp		\
						$$SRC_FILES_DIR/qtreeviewupdateiecfile.cpp	\
						$$SRC_FILES_DIR/qtreeviewupdatertdbfile.cpp	\
						$$SRC_FILES_DIR/qtreeviewupdatevoicefile.cpp	\
						$$SRC_FILES_DIR/qwndgroupmanager.cpp
 						
FORMS   +=	\
						$$UI_FILES_DIR/authorities.ui\
						$$UI_FILES_DIR/basicsettings.ui	\
						$$UI_FILES_DIR/browsefolders.ui	\
						$$UI_FILES_DIR/changepassword.ui	\
						$$UI_FILES_DIR/command.ui	\
						$$UI_FILES_DIR/creategroup.ui	\
						$$UI_FILES_DIR/createuser.ui	\
						$$UI_FILES_DIR/editfolder.ui	\
						$$UI_FILES_DIR/edithost.ui	\
						$$UI_FILES_DIR/inspect.ui	\
						$$UI_FILES_DIR/inspectsetting.ui	\
						$$UI_FILES_DIR/instantupdate.ui	\
						$$UI_FILES_DIR/loginex.ui		\
						$$UI_FILES_DIR/privileges.ui	\
						$$UI_FILES_DIR/syncfile.ui	\
						$$UI_FILES_DIR/syncfileshowlog.ui	\
						$$UI_FILES_DIR/syncgroupcommitfile.ui	\
						$$UI_FILES_DIR/syncgroupreleasefile.ui	\
						$$UI_FILES_DIR/syncgrouptransferfile.ui	\
						$$UI_FILES_DIR/syncgroupupdatefile.ui	\
						$$UI_FILES_DIR/transferfilegroup.ui
						

RESOURCES = ../mgr/mgr.qrc
TRANSLATIONS = hdsyn_en.ts hdsyn_zh.ts

CONFIG(debug, debug|release) { 
    win32:LIBS += ../../../h9000/bin/libglobald1.lib	\
    							../../../h9000/bin/liblogplusd1.lib	\
    							../../../h9000/bin/libnetworkd1.lib	\
    							../../../h9000/bin/libutild1.lib		\
    							../../../h9000/bin/libfcd1.lib			\
    							../../../h9000/bin/libctld1.lib	
    							
    unix:LIBS += -L../../../h9000/lib -lglobald -llogplusd -lnetworkd -lutild -lfcd -lctld
    
    contains(DEFINES, TOPOLOGY_ENABLE ) {
    	win32:LIBS +=	../../../h9000/bin/libtopologyd1.lib
    	
    	unix:LIBS +=	-ltopologyd
		}
}
CONFIG(release, debug|release) { 
    win32:LIBS += ../../../h9000/bin/libglobal1.lib	\
    							../../../h9000/bin/liblogplus1.lib	\
    							../../../h9000/bin/libnetwork1.lib	\
    							../../../h9000/bin/libutil1.lib		\
    							../../../h9000/bin/libfc1.lib			\
    							../../../h9000/bin/libctl1.lib	
    							
    unix:LIBS += -L../../../h9000/lib -lglobal -llogplus -lnetwork -lutil -lfc -lctl
    
    contains(DEFINES, TOPOLOGY_ENABLE ) {
    	win32:LIBS +=	../../../h9000/bin/libtopology1.lib
    	
    	unix:LIBS +=	-ltopology
		}
}

