TEMPLATE = lib
TARGET   = callh9000
CONFIG   += debug
win32{
H9000_HOME=e:/home/ems
}
unix{
H9000_HOME=/home/ems
}
win32{
TMP_DIR           = $${H9000_HOME}/debug/callh9000
}
unix{
TMP_DIR          =debug
}
UI_DIR            = $$TMP_DIR/ui
MOC_DIR           = $$TMP_DIR/moc
OBJECTS_DIR       = $$TMP_DIR


DEFINES += _CALLH9000DLL 
DESTDIR = $${H9000_HOME}/h9000/lib
DLLDESTDIR += $${H9000_HOME}/h9000/dll


INCLUDEPATH += include  \
               $${H9000_HOME}/system/dms/v4.0a/inc \
               $${H9000_HOME}/system/ipm/v4.0a/inc 


HEADERS  += include/callh9000dllspec.h \
						include/calldblib.h \
						#include/callinterlockname.h \
						include/callipm.h
					

            
SOURCES  += src/calldblib.cpp \
						#src/callinterlockname.cpp \
						src/callipm.cpp

win32:DEFINES	+= WIN32
LIBPATH += 	$${H9000_HOME}/h9000/lib \
						$${H9000_HOME}/system/dms/v4.0a/lib

win32{            
LIBS +=-ldblib -lwsock32                     
}                     
unix{            
LIBS +=-ldblib                
}                     
                
           