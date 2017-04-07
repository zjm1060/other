TEMPLATE = lib

win32{
H9000_HOME=e:/home/ems
}
unix{
H9000_HOME=/home/ems
}

INCLUDEPATH += include \
						$${H9000_HOME}/system/cfgmgr/include     

TMP_DIR           = Debug
UI_DIR            = $$TMP_DIR/ui
MOC_DIR           = $$TMP_DIR/moc
OBJECTS_DIR       = $$TMP_DIR

HEADERS += include/uiappinterface.h
SOURCES += src/uiappinterface.cpp

DEFINES += UIAPP_DLL
win32:DEFINES += WIN32
DESTDIR = $${H9000_HOME}/h9000/lib
DLLDESTDIR += $${H9000_HOME}/h9000/dll

