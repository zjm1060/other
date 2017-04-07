
#ifndef GWPRT_H
#define GWPRT_H




typedef struct GWProtocolControl {
  int   nPrc;    // process number 
  int   nPrt;    // protocol flag
                 //     0 = no messages
                 //     1 = error messages
                 //     2 = error, warning messages
                 //     3 = error, warning, information messages
  int   nLvl;    // protocol level (0 - 7)
} sGWProtocolCntrl;

typedef struct GWProtocolMessage {
  int       nTyp;
  int       nLvl;
  int       nPrc;
  int       nNum;
  long int  nTim;
  char      strMsg[256];
} sGWProtocolMsg;




#endif



