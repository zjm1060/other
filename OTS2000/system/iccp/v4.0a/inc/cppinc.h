
#ifndef CPPINC_H
#define CPPINC_H

#include "os.h"

#ifdef SUN_OS
#ifdef __cplusplus
extern "C" {
#endif
#include <errno.h>
#include <malloc.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/timeb.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#define sleep  usleep
#ifdef __cplusplus
}
#endif
#endif

#ifdef WINDOWS32
#include "stdafx.h"
#include <conio.h>
#include <errno.h>
#include <malloc.h>
#include <process.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>
#include <time.h>
#include <windows.h>
#include <winbase.h>
#endif

#include "semfct.h"
#include "shmfct.h"
#include "queuefct.h"

#endif
