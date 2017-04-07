unix {
	SUFFIX=
	
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
		
		contains( DEFINES, Q_OS_SOLARIS_X86):SUFFIX =xxe
		contains( DEFINES, Q_OS_SOLARIS_SPARC):SUFFIX =dxe
	}
	
	contains( UNAME, AIX):SUFFIX =axe
}
