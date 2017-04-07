TEMPLATE = lib
CONFIG	+=  debug
DEFINES	+= CHECKUIAPP_DLL
QT  +=  xml
TMP_DIR           = Debug
UI_DIR            = $$TMP_DIR/ui
MOC_DIR           = $$TMP_DIR/moc
OBJECTS_DIR       = $$TMP_DIR

unix:UI_DIR = ./ui
win32:UI_DIR = ./ui 
 
 
win32{
H9000_HOME=e:/home/ems
}
unix{
H9000_HOME=/home/ems
}

SOURCES	+=src/checkui.cpp \ 
					src/checkWin.cpp \
					src/checkinterlock.cpp \
					src/checkcalcu.cpp \
					src/common.cpp \
					src/testui.cpp	\
					src/checkdbin.cpp	\
					src/operatexml.cpp	\
					src/checktemp.cpp \
					src/checkwater.cpp	\
					src/checkdcom.cpp
           
	
HEADERS	+=include/checkui.h  \ 
					include/checkWin.h \
					include/checkinterlock.h \
					include/checkcalcu.h \
					include/common.h \
					include/testui.h	\
					include/checkdbin.h	\
					include/operatexml.h	\
					include/checktemp.h \
					include/checkwater.h	\
					include/checkdcom.h	
					


FORMS   += ui/test.ui

INCLUDEPATH += $${H9000_HOME}/system/cfgmgr/include \
							$${H9000_HOME}/system/cfgmgr/appguiif/checkui/include \
							$${H9000_HOME}/system/cfgmgr/appguiif/appuiif/include \
							$${H9000_HOME}/system/cfgmgr/mainapp/include \
							$${H9000_HOME}/system/cfgmgr/appguiif/callH9000/include \
							 $${H9000_HOME}/system/dms/v4.0a/inc
DESTDIR = $${H9000_HOME}/h9000/lib
win32{
DLLDESTDIR =$${H9000_HOME}/h9000/dll
}


win32{
    DEF_FILE = checkui.def
} 

RESOURCES += checkui.qrc

TRANSLATIONS = checkui_zh.ts
win32:DEFINES	+= WIN32
LIBPATH += 	$${H9000_HOME}/h9000/lib \
						$${H9000_HOME}/system/dms/v4.0a/lib

win32:LIBS	+= -lappuiif -lcallh9000  
unix:LIBS	+= -lappuiif -lcallh9000 