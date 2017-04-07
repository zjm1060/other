#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>			
#include <memory.h>
#include <time.h>

#include "../../../dms/v4.0a/inc/dms_com.h"
#include "../../../dms/v4.0a/inc/dms_functions.h"
#include "../../../dms/v4.0a/inc/dms_util_def.h"
#include "../../../dms/v4.0a/inc/dms_structure_def.h"
#include "../../../dms/v4.0a/inc/dms_text_def.h"
#include "../../../lan/v4.0a/inc/lan.h"
#include "../../../dms/v4.0a/inc/os_type_def.h"


#define	GET_PT_ID		0
#define	GET_LOGICALNAME	1
#define	GET_LONGNAME	2
#define	GET_FVALUE		3
#define	GET_IND_STATE	4
#define	GET_OBJ_STATE	5

