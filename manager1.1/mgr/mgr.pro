##
#		@Author	zlb
#		@Time		2011-3-19
#		@Desc 	ΩÁ√Ê
##

include( ./configure.pri )
include( ../config.pri )
include( ../extconfig.pri )

TEMPLATE = app

QT += network xml

contains(QT_CONFIG, opengl):QT += opengl

win32:DEFINES    += CTL_DLL 

CONFIG += thread

TARGET = manager

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
								../libmib/include/			\
								../libutil/include/			\
								../libnetwork/include/	\
								../libfc/include/				\
								../libctl/include/
															
contains(DEFINES, TOPOLOGY_ENABLE ) {
    	INCLUDEPATH +=	../libtopology/include/
}

HEADERS +=	\
						include/qabstractfiletransfer.h	\
						include/qabstractgraphicsitem.h	\
						include/qabstractmonitor.h	\
						include/qcomparefiledelegate.h	\
						include/qconfiguregroupmanager.h	\
						include/qdelegatesynccommit.h	\
						include/qdlgbrowsefiles.h	\
						include/qdlgbrowsefolders.h		\
						include/qdlgcreategroup.h	\
						include/qdlgcreateuser.h	\
						include/qdlgeditfolder.h	\
						include/qdlgedithost.h	\
						include/qdlginstantupdate.h		\
						include/qdlgmodifypassword.h	\
						include/qfiletransfergroupmanager.h	\
						include/qfiletransfermanager.h	\
						include/qframeauthority.h	\	
						include/qframebasicsettings.h	\					
						include/qframecurve.h	\
						include/qframedisk.h	\
						include/qframefilereceiver.h	\
						include/qframefiletransfergroup.h		\
						include/qframeh9000process.h	\
						include/qframeinspect.h	\
						include/qframeinspectsetting.h	\
						include/qframelocalfilesystem.h	\
						include/qframememory.h	\
						include/qframenetwork.h		\
						include/qframeprivileges.h	\
						include/qframeprocessor.h	\
						include/qframeremotefilesystem.h	\
						include/qframesynccommand.h	\
						include/qframesyncfile.h	\
						include/qframesyncfileshowlog.h	\
						include/qframesyncgroupcommitfile.h	\
						include/qframesyncgroupreleasefile.h	\
						include/qframesyncgrouptransferfile.h	\
						include/qframesyncgroupupdate.h	\
						include/qframesyncgroupupdatefile.h	\
						include/qframesystemprocess.h	\
						include/qframetransferfile.h	\
						include/qgraphicsitemfirewall.h	\
						include/qgraphicsitemhost.h	\
						include/qgraphicsitemhub.h	\
						include/qgraphicsitemlink.h	\
						include/qgraphicsitemrouter.h	\
						include/qgraphicsitemswitch.h	\
						include/qgraphicspanel.h	\
						include/qgraphicspanelhawkeyes.h	\
						include/qgraphicspromptpanel.h	\
						include/qgrouptransferqueue.h	\
						include/qgrouptransferqueuedelegate.h	\
						include/qinfomanager.h	\
						include/qinspectdelegate.h	\
						include/qinspectmanager.h	\
						include/qinspectsettingdelegate.h		\
						include/qlistviewcontrolledserver.h	\
						include/qlistviewfilereceiver.h		\
						include/qlistviewhorizontalicon.h	\
						include/qlistviewprompt.h	\
						include/qlistviewsyncfilerequestserver.h	\
						include/qlistviewsyncfileserver.h	\
						include/qlogingraphicsscene.h	\
						include/qmainwnd.h	\
						include/qmainwndprivilege.h	\
						include/qmainwndsyncfile.h	\
						include/qmessageboxex.h		\
						include/qmessageboxlogout.h	\
						include/qprocessmanager.h	\
						include/qprocesssocket.h	\
						include/qreadwritexmlfile.h	\
						include/qreceiverinspect.h	\
						include/qreceiverprivilege.h	\
						include/qreceivertransferfile.h	\
						include/qresourcemanager.h	\
						include/qserverdelegate.h	\
						include/qsyncfiledelegate.h	\
						include/qsyncgroupcommitmanager.h	\
						include/qsyncgroupreleasemanager.h	\
						include/qsyncgroupupdatemanager.h	\
						include/qsyncmanager.h	\
						include/qsystemmanager.h	\
						include/qtipsdelegate.h	\
						include/qtransferinstructor.h	\
						include/qtransferqueuedelegate.h	\
						include/qtreeviewassociationfiles.h	\
						include/qtreeviewcomparefile.h	\
						include/qtreeviewgrouptransferdelegate.h	\
						include/qtreeviewgrouptransferqueue.h		\
						include/qtreeviewinspect.h	\
						include/qtreeviewinspectsetting.h	\
						include/qtreeviewsynccommit.h	\
						include/qtreeviewsyncconffile.h	\
						include/qtreeviewsyncdatfile.h	\
						include/qtreeviewsyncdbfile.h	\
						include/qtreeviewsyncdspfile.h	\
						include/qtreeviewsyncfile.h	\
						include/qtreeviewsynciecfile.h	\
						include/qtreeviewsyncvoicefile.h	\
						include/qtreeviewtransferqueue.h	\
						include/qtreeviewupdateconffile.h	\
						include/qtreeviewupdatedatfile.h	\
						include/qtreeviewupdatedbfile.h		\
						include/qtreeviewupdatedspfile.h	\
						include/qtreeviewupdatefile.h		\
						include/qtreeviewupdateiecfile.h	\
						include/qtreeviewupdatertdbfile.h	\
						include/qtreeviewupdatevoicefile.h	\
						include/qwndgraphics.h	\
						include/qwndgraphicsscene.h	\
						include/qwndgroupmanager.h	\
						include/qwndmanager.h

SOURCES +=	\
						src/main.cpp	\
						src/qabstractfiletransfer.cpp	\
						src/qabstractgraphicsitem.cpp	\
						src/qabstractmonitor.cpp	\
						src/qcomparefiledelegate.cpp	\
						src/qconfiguregroupmanager.cpp	\
						src/qdelegatesynccommit.cpp	\
						src/qdlgbrowsefiles.cpp	\
						src/qdlgbrowsefolders.cpp		\
						src/qdlgcreategroup.cpp	\
						src/qdlgcreateuser.cpp	\
						src/qdlgeditfolder.cpp	\
						src/qdlgedithost.cpp	\						
						src/qdlginstantupdate.cpp		\
						src/qdlgmodifypassword.cpp	\
						src/qfiletransfergroupmanager.cpp	\
						src/qfiletransfermanager.cpp	\
						src/qframeauthority.cpp	\		
						src/qframebasicsettings.cpp	\				
						src/qframecurve.cpp		\
						src/qframedisk.cpp	\
						src/qframefilereceiver.cpp	\
						src/qframefiletransfergroup.cpp		\
						src/qframeh9000process.cpp	\
						src/qframeinspect.cpp	\
						src/qframeinspectsetting.cpp	\
						src/qframelocalfilesystem.cpp	\
						src/qframememory.cpp	\
						src/qframenetwork.cpp		\
						src/qframeprivileges.cpp	\
						src/qframeprocessor.cpp	\
						src/qframeremotefilesystem.cpp	\
						src/qframesynccommand.cpp	\
						src/qframesyncfile.cpp	\
						src/qframesyncfileshowlog.cpp	\
						src/qframesyncgroupcommitfile.cpp	\
						src/qframesyncgroupreleasefile.cpp	\
						src/qframesyncgrouptransferfile.cpp	\
						src/qframesyncgroupupdate.cpp	\
						src/qframesyncgroupupdatefile.cpp	\
						src/qframesystemprocess.cpp	\
						src/qframetransferfile.cpp	\
						src/qgraphicsitemfirewall.cpp	\
						src/qgraphicsitemhost.cpp	\
						src/qgraphicsitemhub.cpp	\
						src/qgraphicsitemlink.cpp	\
						src/qgraphicsitemrouter.cpp	\
						src/qgraphicsitemswitch.cpp	\
						src/qgraphicspanel.cpp	\
						src/qgraphicspanelhawkeyes.cpp	\
						src/qgraphicspromptpanel.cpp	\
						src/qgrouptransferqueue.cpp	\
						src/qgrouptransferqueuedelegate.cpp	\
						src/qinfomanager.cpp	\
						src/qinspectdelegate.cpp	\
						src/qinspectmanager.cpp	\
						src/qinspectsettingdelegate.cpp		\
						src/qlistviewcontrolledserver.cpp	\
						src/qlistviewfilereceiver.cpp		\
						src/qlistviewhorizontalicon.cpp	\
						src/qlistviewprompt.cpp	\
						src/qlistviewsyncfilerequestserver.cpp	\
						src/qlistviewsyncfileserver.cpp	\
						src/qlogingraphicsscene.cpp	\
						src/qmainwnd.cpp	\
						src/qmainwndprivilege.cpp	\
						src/qmainwndsyncfile.cpp	\
						src/qmessageboxex.cpp		\
						src/qmessageboxlogout.cpp	\
						src/qprocessmanager.cpp	\
						src/qprocesssocket.cpp	\
						src/qreadwritexmlfile.cpp	\
						src/qreceiverinspect.cpp	\
						src/qreceiverprivilege.cpp	\
						src/qreceivertransferfile.cpp	\
						src/qresourcemanager.cpp	\
						src/qserverdelegate.cpp	\
						src/qsyncfiledelegate.cpp	\
						src/qsyncgroupcommitmanager.cpp	\
						src/qsyncgroupreleasemanager.cpp	\
						src/qsyncgroupupdatemanager.cpp	\
						src/qsyncmanager.cpp	\
						src/qsystemmanager.cpp	\
						src/qtipsdelegate.cpp	\
						src/qtransferinstructor.cpp		\
						src/qtransferqueuedelegate.cpp	\
						src/qtreeviewassociationfiles.cpp	\
						src/qtreeviewcomparefile.cpp	\
						src/qtreeviewgrouptransferdelegate.cpp	\
						src/qtreeviewgrouptransferqueue.cpp		\
						src/qtreeviewinspect.cpp	\
						src/qtreeviewinspectsetting.cpp	\
						src/qtreeviewsynccommit.cpp	\
						src/qtreeviewsyncconffile.cpp	\
						src/qtreeviewsyncdatfile.cpp	\
						src/qtreeviewsyncdbfile.cpp	\
						src/qtreeviewsyncdspfile.cpp	\
						src/qtreeviewsyncfile.cpp	\
						src/qtreeviewsynciecfile.cpp	\
						src/qtreeviewsyncvoicefile.cpp	\
						src/qtreeviewtransferqueue.cpp	\
						src/qtreeviewupdateconffile.cpp	\
						src/qtreeviewupdatedatfile.cpp	\
						src/qtreeviewupdatedbfile.cpp		\
						src/qtreeviewupdatedspfile.cpp	\
						src/qtreeviewupdatefile.cpp		\
						src/qtreeviewupdateiecfile.cpp	\
						src/qtreeviewupdatertdbfile.cpp	\
						src/qtreeviewupdatevoicefile.cpp	\
						src/qwndgraphics.cpp	\
						src/qwndgraphicsscene.cpp	\
						src/qwndgroupmanager.cpp	\
						src/qwndmanager.cpp
 						
FORMS   +=	\
						ui/authorities.ui\
						ui/basicsettings.ui	\
						ui/browsefolders.ui	\
						ui/changepassword.ui	\
						ui/command.ui	\
						ui/creategroup.ui	\
						ui/createuser.ui	\
						ui/disk.ui		\
						ui/editfolder.ui	\
						ui/edithost.ui	\
						ui/h9000process.ui	\
						ui/inspect.ui	\
						ui/inspectsetting.ui	\
						ui/instantupdate.ui	\
						ui/login.ui		\
						ui/memory.ui	\
						ui/network.ui	\
						ui/privileges.ui	\
						ui/processor.ui		\
						ui/syncfile.ui	\
						ui/syncfileshowlog.ui	\
						ui/syncgroupcommitfile.ui	\
						ui/syncgroupreleasefile.ui	\
						ui/syncgrouptransferfile.ui	\
						ui/syncgroupupdatefile.ui	\
						ui/systemprocess.ui	\
						ui/transferfile.ui	\
						ui/transferfilegroup.ui
						

RESOURCES = mgr.qrc
TRANSLATIONS = manager_en.ts manager_zh.ts

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

