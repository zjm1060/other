TEMPLATE = lib
CONFIG	+=  debug
DEFINES	+= FUNCUIAPP_DLL
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

SOURCES	+=src/funcui.cpp \ 
					src/testui.cpp
           
	
HEADERS	+=include/funcui.h  \ 
					include/testui.h


FORMS   += ui/test.ui

INCLUDEPATH += $$(H9000_HOME)/system/cfgmgr/include \
							$$(H9000_HOME)/system/cfgmgr/appguiif/funcui/include \
							$$(H9000_HOME)/system/cfgmgr/appguiif/appuiif/include \
							$$(H9000_HOME)/system/cfgmgr/mainapp/include
               

DESTDIR = $$(H9000_HOME)/h9000/lib
win32{
DLLDESTDIR =$$(H9000_HOME)/h9000/dll
}


win32{
    DEF_FILE = funcui.def
} 

RESOURCES += funcui.qrc

#TRANSLATIONS = funcui_en.ts funcui_zh.ts
win32:DEFINES	+= WIN32
LIBPATH += 	$(H9000_HOME)/h9000/lib
win32:LIBS	+= -lappuiif   
unix:LIBS	+= -lappuiif 