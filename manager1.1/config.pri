CONFIG	+=  debug_and_release build_all

CONFIG           += GlobalDll LogPlusDll UtilDll NetworkDll AlarmDll MibDll  MsgDll DeaDll FcDll DdbDll CtlDll

DEFINES	+= H9000_ENABLE
#DEFINES += TOPOLOGY_ENABLE #提供拓扑发现功能
DEFINES += HSHOW_ENABLE			#编译桌面工具

contains(DEFINES, H9000_ENABLE ) {
    	CONFIG           +=	CscsDll
}

contains(DEFINES, TOPOLOGY_ENABLE ) {
    	CONFIG           +=	TopologyDll
}

