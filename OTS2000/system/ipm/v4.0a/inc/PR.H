


#define BUFSIZE 256
#define TOPRINTON 1
#define TOPRINTOFF 0

int 	toprint;	// flag 

HDC     prtdc;                          // device context to print on

char    gszDeviceName[BUFSIZE];        // current device name
char    gszPort      [BUFSIZE];        // current port
char    gszDriverName[BUFSIZE];        // current driver name

void Print();

