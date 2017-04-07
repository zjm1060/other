include( ./config.pri )

TEMPLATE = subdirs
CONFIG   += ordered

SUBDIRS = libglobal liblogplus libutil 

contains(DEFINES, H9000_ENABLE ) {
    	SUBDIRS +=	libcscs
}

contains(DEFINES, TOPOLOGY_ENABLE ) {
    	SUBDIRS +=	libtopology
}

SUBDIRS+= libalarm libdea libfc libmib libnetwork libddb libmsg mgrsvc libctl mgr hdsyn

contains(DEFINES, HSHOW_ENABLE ) {
    	SUBDIRS +=	hshow
}
					