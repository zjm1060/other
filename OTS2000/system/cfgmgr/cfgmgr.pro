TEMPLAT= app
CONFIG  += console qt debug
TARGET  =	cfgmgr
QT  +=  network

TMP_DIR           = Debug
UI_DIR            = $${TMP_DIR}/ui
MOC_DIR           = $${TMP_DIR}/moc
OBJECTS_DIR       = $${TMP_DIR}


win32{
H9000_HOME=e:/home/ems
}
unix{
H9000_HOME=/home/ems
}

INCLUDEPATH      += $${H9000_HOME}/system/cfgmgr/include \
										$${H9000_HOME}/system/cfgmgr/mainapp/include \
										$${H9000_HOME}/system/cfgmgr/appguiif/appuiif/include 
										

HEADERS          += mainapp/include/gedocument.h \
                    mainapp/include/gemainwindow.h \
                    mainapp/include/appwindow.h \
                    include/mdiMainWin.h


SOURCES          += mainapp/src/main.cpp  \
                    mainapp/src/gedocument.cpp  \
                    mainapp/src/gemainwindow.cpp  \
                    mainapp/src/appwindow.cpp  \
                    mainapp/src/mdiMainWin.cpp



#RESOURCES=cfgmgr.qrc
RC_FILE = cfgmgr.rc     
TRANSLATIONS = cfgmgr_zh.ts

DESTDIR = $${H9000_HOME}/h9000/cfgmgr
LIBPATH += $${H9000_HOME}/h9000/lib 
					 

#win32:LIBS +=
#unix:LIBS  +=  