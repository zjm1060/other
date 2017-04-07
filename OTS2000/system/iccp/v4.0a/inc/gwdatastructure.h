
#ifndef GWDATASTRUCTURE_H
#define GWDATASTRUCTURE_H



#define byte   unsigned char
#define FLAGS  unsigned long int



//---- configuration and program files ----
#ifdef WINDOWS32
#define CONF_IMSDATADEF_NAM        "E:\\Projects\\AL\\AL001\\Software\\gateway\\gwconf\\IMSDataDef.dat"
#define CONF_IMSDATASETNAME_NAM    "E:\\Projects\\AL\\AL001\\Software\\gateway\\gwconf\\IMSDataSetName.ini"
#define DAT_IMSDATASET_NAM         "T:\\Tmp\\IMSDataSet.dat"
#endif

#ifdef HPUX
#define CONF_IMSDATADEF_NAM        "/usr/gwaysw/gway/gwconf/IMSDataDef.dat"
#define CONF_IMSDATASETNAME_NAM    "/usr/gwaysw/gway/gwconf/IMSDataSetName.ini"
#define DAT_IMSDATASET_NAM         "/tmp/IMSDataSet.dat"
#define PRC_FIL_GWAPPWS            "/usr/gwaysw/gway/gwbin/gwapppws"
#define PRC_FIL_GWCNTRL            "/usr/gwaysw/gway/gwbin/gwcntrl"
#define PRC_FIL_GWDC               "/usr/gwaysw/gway/gwbin/gwdc"
#define PRC_FIL_GWIEC101           "/usr/gwaysw/gway/gwbin/gateway.dxe"
#define PRC_FIL_GWIMS              "/usr/gwaysw/gway/gwbin/gwims"
#define PRC_FIL_GWMAIN             "/usr/gwaysw/gway/gwbin/gwmain"
#define PRC_FIL_GWPRT              "/usr/gwaysw/gway/gwbin/gwprt"
#define MIT_XML_OSI_CFG_FILE	   "/usr/gwaysw/gway/gwconf/osicfg.xml"
#define MIT_XML_MIT_CFG_FILE	   "/usr/gwaysw/gway/gwconf/gwdccfg.xml"
#define MIT_XML_MIT_LOG_FILE	   "/usr/gwaysw/gway/gwconf/logcfg.xml"
#define MIT_XML_OSI_CFG_FILE_S	   "/usr/gwaysw/gway/gwconf/osicfg_s.xml"
#define MIT_XML_MIT_CFG_FILE_S	   "/usr/gwaysw/gway/gwconf/gwdccfg_s.xml"
#define MIT_XML_MIT_LOG_FILE_S	   "/usr/gwaysw/gway/gwconf/logcfg_s.xml"
#endif



//---- shared memory sections
#define SHM_DATA_KEY        10012
#define SHM_IFCNTRL_KEY     10021
#define SHM_PRCCNTRL_KEY    10002
#define SHM_PRTCNTRL_KEY    10001

#define SHM_DATA_NAM        "M_Data"
#define SHM_IFCNTRL_NAM     "M_IFCntrl"
#define SHM_PRCCNTRL_NAM    "M_PrcCntrl"
#define SHM_PRTCNTRL_NAM    "M_PrtCntrl"

//---- semaphores
#define SEM_DATA_KEY        10012

#define SEM_DATA_NAM        "S_Data"

//---- queues
#define Q_101I_KEY          3011
#define Q_101O_KEY          3001
#define Q_DC0O_KEY          2001
#define Q_DC1O_KEY          2011
#define Q_DC2O_KEY          2021
#define Q_DCI_KEY           2101
#define Q_IMS_KEY           1001
#define Q_PRT_KEY              1

#define Q_101I_NAM          "Q_101r"
#define Q_101O_NAM          "Q_101w"
#define Q_DC0O_NAM          "Q_DC0o"
#define Q_DC1O_NAM          "Q_DC1o"
#define Q_DC2O_NAM          "Q_DC2o"
#define Q_DCI_NAM           "Q_DCi"
#define Q_IMS_NAM           "Q_IMS"
#define Q_PRT_NAM           "Q_PRT"

#define Q_101I_SIZ          32768
#define Q_101O_SIZ          32768
#define Q_DC0O_SIZ          32768
#define Q_DC1O_SIZ          32768
#define Q_DC2O_SIZ          32768
#define Q_DCI_SIZ           32768
#define Q_IMS_SIZ           32768
#define Q_PRT_SIZ           32768




#define MAX_SIGNAL_NUM  32768

#define TYPE_MEASUREMENT    0x01
#define TYPE_INDICATION     0x02
#define TYPE_COMMAND        0x04
#define TYPE_SETPOINT       0x08

#define ID_OFFSET_MEASUREMENT
#define ID_OFFSET_INDICATION
#define ID_OFFSET_COMMAND
#define ID_OFFSET_SETPOINT

#define DATASRC_101_APPC   0x01      // Application Controller
#define DATASRC_101_DC0    0x11      // Dispatch Center 0 (Cascade DC) - not used
#define DATASRC_101_DC1    0x12      // Dispatch Center 1 (National DC)
#define DATASRC_101_DC2    0x13      // Dispatch Center 2 (Central China DC)
#define DATASRC_IMS        0x21      // IMS
#define DATASRC_APPWS      0x22      // Application Workstation
#define DATASRC_TASE2_DC0  0x31      // Dispatch Center
#define DATASRC_TASE2_DC1  0x32      // Dispatch Center
#define DATASRC_TASE2_DC2  0x33      // Dispatch Center

#define DATATYPE_M_DISCRETE     1    // Measurement
#define DATATYPE_M_REAL         2    // Measurement
#define DATATYPE_M_STATE        3    // Measurement
#define DATATYPE_I_SINGLE       4    // Indication - Single
#define DATATYPE_I_DOUBLE       5    // Indication - Double
#define DATATYPE_CMD                 // Command
#define DATATYPE_SP_DISCRETE         // Setpoint
#define DATATYPE_SP_REAL             // Setpoint
#define DATATYPE_INFO_MSG            // Information Message

#define MSG_TYP_ERR     0x01
#define MSG_TYP_WARN    0x02
#define MSG_TYP_INFO    0x03

#define PRC_NUM_GWMAIN      1
#define PRC_NUM_GWCNTRL     2
#define PRC_NUM_GWAPPWS    11
#define PRC_NUM_GWDC0      31
#define PRC_NUM_GWDC1      32
#define PRC_NUM_GWDC2      33
#define PRC_NUM_GWIMS      21
#define PRC_NUM_GWIEC101   41   //modified 31 to 41 2003.7.16 zy
#define PRC_NUM_GWPRT      63



//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
typedef struct GWTimeStamp {
  long int   nTim;
  short int  nMSec;
} sGWTimeStamp;



//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
typedef struct GWSignalValue {
  byte                  nTyp;       // Measurement / Indication / Command / Setpoint
  byte                  cVal[33];           // actual value to transmit
  bool                  bQFlg;
  byte                  cTim[6];    // Timestamp
  struct GWSignalValue *pNxt;
} sGWSignalValue;



//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
typedef struct GWTranslationTable {
  //-- general part
  int  nID;                         // ID number 
  struct GWTranslationTable *pNxt;  //
  byte nType;                       // Measurement / Indication / Command / Setpoint
  byte nDatSrc;                     // bits IMS/APPC/DC0/DC1/DC2
  byte nDatDst;                     // bits IMS/APPC/DC0/DC1/DC2

  //-- signal data part 
  sGWSignalValue *pSigValAct;        // 
  sGWSignalValue *pSigValNew;        // start of list of new signal value data to transmit
  sGWSignalValue *pSigValLst;        // pointer to last signal value data in list
  byte            nValNew;           // new signal value flag
  byte            nValProcess;       // if set: new signal value not yet processed
  long int        nVal;              // signal data (counter) 
  double          rVal;              // signal data (analog)
  FLAGS           bVal;              // signal data (bits)
  FLAGS           nValFlg;           // signal value flags
  sGWTimeStamp    sValTim;           // signal value time stamp

  int             nLCUNum;           //
  char            strUnit[17];       //
  double          rMinVal;           //
  double          rMaxVal;           //

  //-- special part: IMS
  bool      bIMS;                    //
  byte      nIMSState;               //
  char      strIMSNam[33];           //
  long int  nIMSChnNum;              //
  byte      nIMSDataType;            //

  //-- special part: ICCP/TASE.2
  //     DC0
  bool      bDC0;                    // flag "transmit to / receive from" DC0
  bool      bDC0Ready;               // flag "read to transmit"
  byte      nDC0State;               // state of ICCP/TASE.2 interface to DC0
  byte      nDC0State101;            // state of IEC 101 interface to DC0
  char      strDC0Nam[33];           // ICCP item name at DC0
  byte      nDC0DatTyp;              // data type at DC0
  bool      bDC0Cntrl;               // flag "control by DC0 allowed"
  bool      bDC0DblI;                // flag "double indication"
  long int  nDC0RelID;               // double indication: related signal id
  byte      nDC0Bit;                 // double indication: bit position 
  char      strDC0RelNam[33];        // double indication: related ICCP/TASE.2 name
  char      strDC0DescI[9];          // double indication: ON/OFF/OPEN/CLOSE
  bool      bDC0SoE;                 // flag "transmit as SoE message"
  //     DC1
  bool      bDC1;                    // flag "transmit to / receive from" DC1
  bool      bDC1Ready;               // flag "read to transmit"
  byte      nDC1State;               // state of ICCP/TASE.2 interface to DC1
  byte      nDC1State101;            // state of IEC 101 interface to DC1
  char      strDC1Nam[33];           // ICCP item name at DC1
  byte      nDC1DatTyp;              // data type at DC1
  bool      bDC1Cntrl;               // flag "control by DC1 allowed"
  bool      bDC1DblI;                // flag "double indication"
  long int  nDC1RelID;               // double indication: related signal id
  byte      nDC1Bit;                 // double indication: bit position 
  char      strDC1RelNam[33];        // double indication: related ICCP/TASE.2 name
  char      strDC1DescI[9];          // double indication: ON/OFF/OPEN/CLOSE
  bool      bDC1SoE;                 // flag "transmit as SoE message"
  //     DC2
  bool      bDC2;                    // flag "transmit to / receive fro m" DC2
  bool      bDC2Ready;               // flag "read to transmit"
  byte      nDC2State;               // state of ICCP/TASE.2 interface to DC2
  byte      nDC2State101;            // state of IEC 101 interface to DC2
  char      strDC2Nam[33];           // ICCP item name at DC2
  byte      nDC2DatTyp;              // data type at DC2
  bool      bDC2Cntrl;               // flag "control by DC2 allowed"
  bool      bDC2DblI;                // flag "double indication"
  long int  nDC2RelID;               // double indication: related signal id
  byte      nDC2Bit;                 // double indication: bit position 
  char      strDC2RelNam[33];        // double indication: related ICCP/TASE.2 name
  char      strDC2DescI[9];          // double indication: ON/OFF/OPEN/CLOSE
  bool      bDC2SoE;                 // flag "transmit as SoE message"

  //-- special part: IEC 60870-5-101
  //     DC0
  byte      n101Destination0;
  byte      n101DataType0;
  //     DC1
  byte      n101Destination1;
  byte      n101DataType1;
  //     DC2
  byte      n101Destination2;
  byte      n101DataType2;

  //-- special part: IEC 60870-5-101
  //     APPC
  bool      bAppC;                   //
  bool      bAppCReady;              //
  byte      nAppCState;              //
  long int  nAppCBlkNum;             //
  long int  nAppCBlkPos;             //
  long int  nAppCChnNum;             //

} sGWTransTbl;



//------------------------------------------------------------------------------
// internal communication buffer for interface ICCP/TASE.2 <-> IEC 101 
//------------------------------------------------------------------------------
typedef struct GWSignalTASE2 {
  int          nFct;
  byte         nDst;
  char         strDCNam[33];
  FLAGS        nType;
  byte         nValBits;
  long int     nVal;
  double       rVal;
  FLAGS        nValidity;
  FLAGS        nCurrentSource;
  FLAGS        nNormalValue;
  FLAGS        nTimeStampQuality;
  FLAGS        nOverflow;
  sGWTimeStamp sTimeStamp;
} sGWSigTASE2;



//------------------------------------------------------------------------------
// IIE 101 receive/send buffer structure 
//------------------------------------------------------------------------------
typedef struct GWSignalTypeX {
  int             nTyp;           // signal type
                                  //    1: Single point
                                  //    2: Signal point with time tag
                                  //    9: Measure value without time tag
                                  //   46: Double point control command (used for dispatch center)
                                  //   48: Set point command normalized value
                                  //   51: Bitstring of 32 bit control command (used for AC450)
                                  //   ..
  int             nSrc;           // origin of signal
                                  //     application controller
                                  //     National Dispatch Center
                                  //     ...
  unsigned short  nChnNum;        // iec101 channel number
  unsigned char	  nCauseOfTrans;  // CYCLE:2 SPONTANEOUS:3 INTERROGATION:20 (for date)
                                  // ACT:6 DEACTCON:8 (for command active and deactive)
                                  // ACTCON:7 DEACTCON:9 (for command acknowledgement)
  unsigned char	  nQuality;       // Quality for all, digital value, command confirm inside Quality
                                  //   IV NT SB BL 0 0 0 OV    (for Measure value)
                                  //   IV NT SB BL 0  0  0 SPI (for Single point: SPI 0--OFF; 1--ON)
                                  //   IV NT SB BL 0  0  DPI (for Double point: DPI bit0-1 0--Transit; 1--OFF; 2--ON; 3--Indeterminate)
                                  //   Command(bit 7: 0--EXEC; 1--SELECT)
                                  //   bit0-1 DCS 1--OFF; 2--ON
                                  //   bit0-1 RCS 1--LOWER; 2--HIGHER
  unsigned char	  nValueType;     // type of the value(0--Status; 1--Float; 2--Step; 3--Count; 4--Status Detect)
  union UVALUE {
    float           fValue;       // Mesured Value, Set point value, Parameter Set
    char            cVTI;         // Step VTI(-64 63)[1-7]
    unsigned short  nValue[2];    // Status Change Detected
    int	            iCountRead;   // Integrated Count(32bits)
  } uValue;
  unsigned int    nTimeTagType;   // time tag: short:1 (min,sec and ms), long:2, no:0
  unsigned short  nMs;            // 0-999 ms (milliseconds)
  unsigned short  nSeconds;       // 0-59 (seconds)
  unsigned short  nMinutes;       // 0-59 min
  unsigned short  nHours;         // 0-23h
  unsigned short  nDayOfWeek;     // 1-7 day of week
  unsigned short  nDay;           // 1-31 day
  unsigned short  nMonth;         // 1-12 month
  unsigned short  nYear;          // 0-99 years
} sGWSignalTypeX;

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
#define StartAssociations                      11
#define StopAssociations                       12
#define GetAssociationSummary                  13
  
#define ReadRemoteDataValues                   21
#define WriteRemoteDataValues                  22
#define GetRemoteDataValueNames                23
#define GetRemote DataValuesAttributes         24
#define ReadRemoteDataSet                      25
#define WriteRemoteDataSet                     26
#define CreateRemoteDataSet                    27
#define DeleteRemoteDataSet                    28
#define GetRemoteDataSetElementNames           29
  
#define DisplayLocalDataValue                  31
#define ModifyLocalDataValue                   32
#define ReconfigureDataValueChanedCondition    33
#define GetLocalDataValueNames                 34
#define GetLocalDataSetNames                   35
  
#define StartTransferSets                      41
#define StopTransferSets                       42
  
#define StartInformatonMessages                51
#define StopInformationMessages                52
#define SendInformationMessage                 53
  
#define SelectDevice                           61
#define OperateDevice                          62
#define SetDeviceTag                           63
#define GetDeviceTag                           64
#define GetLocalDevices                        65
  
#define GetConfigurationRemotes                71
#define GetRemoteDateValueNames                72
#define GetRemoteDataSetNames                  73
#define GetRemoteDataSetAttributes             74
#define GetRemoteTransferSetAttributes         75
#define GetRemoteInfoMessages                  76
#define GetRemoteDevices                       77
  
#define DynMemCheck                            81
#define GetStatistics                          82
#define SetDebugLevels                         83
#define ScreenLogOn                            84
#define ScreenLogOff                           85
#define GetSizes                               86
#define ReconfigureChangeMode                  87
#define ModifyAddressMatching                  88



typedef struct GWTASE2Function11 {    // start association(s)
  short int  nFctId;
} sGWTase2Fct11;

typedef struct GWTASE2Function12 {    // stop association(s)
  short int  nFctId;
}sGWTase2FCT12;

typedef struct GWTASE2Function13 {    // association summary
  short int  nFctId;
}sGWTase2FCT13;

typedef struct GWTASE2Function21 {    // remote variables - get data value(s)
  short int  nFctId;
}sGWTase2FCT21;

typedef struct GWTASE2Function22 {    // remote variables - set data value(s)
  short int  nFctId;
}sGWTase2FCTr22;

typedef struct GWTASE2Function23 {    // remote variables - get data value name
  short int  nFctId;
}sGWTase2FCT23;

typedef struct GWTASE2Function24 {    // remote variables - get data value type
  short int  nFctId;
}sGWTase2FCT24;

typedef struct GWTASE2Function25 {    // remote variables - get data set element values
  short int  nFctId;
}sGWTase2FCT25;

typedef struct GWTASE2Function26 {    // remote variables - set data set element values
  short int  nFctId;
}sGWTase2FCT26;

typedef struct GWTASE2Function27 {    // remote variables - create date set
  short int  nFctId;
}sGWTase2FCT27;

typedef struct GWTASE2Function28 {    // remote variables - delete data set
  short int  nFctId;
}sGWTase2FCT28;

typedef struct GWTASE2Function29 {    // remote variables - get data set element names
  short int  nFctId;
}sGWTase2FCT29;

typedef struct GWTASE2Function31 {    // local variables - display local variable value(s)
  short int  nFctId;
}sGWTase2FCT31;

typedef struct GWTASE2Function32 {    // local variables - modify local data values
  short int  nFctId;
}sGWTase2FCT32;

typedef struct GWTASE2Function33 {    // local variables -  modify local data value change condition
  short int  nFctId;
}sGWTase2FCT33;

typedef struct GWTASE2Function34 {    // local variables - get local data value names
  short int  nFctId;
}sGWTase2FCT34;

typedef struct GWTASE2Function35 {    // local variables - get local data set names
  short int  nFctId;
}sGWTase2FCT35;

typedef struct GWTASE2Function41 {    // start transfers
  short int  nFctId;
}sGWTase2FCT41;

typedef struct GWTASE2Function42 {    // stop transfers
  short int  nFctId;
}sGWTase2FCT42;

typedef struct GWTASE2Function51 {    // start information messages (client)
  short int  nFctId;
}sGWTase2FCT51;

typedef struct GWTASE2Function52 {    // stop information maessages (client)
  short int  nFctId;
}sGWTase2FCT52;

typedef struct GWTASE2Function53 {    // send message (server)
  short int  nFctId;
}sGWTase2FCT53;

typedef struct GWTASE2Function61 {    // device control - select operation
  short int  nFctId;
}sGWTase2FCT61;

typedef struct GWTASE2Function62 {    // device control - operate operation
  short int  nFctId;
}sGWTase2FCT62;

typedef struct GWTASE2Function63 {    // device control - set tag
  short int  nFctId;
}sGWTase2FCT63;

typedef struct GWTASE2Function64 {    // device control - get tag
  short int  nFctId;
}sGWTase2FCT64;

typedef struct GWTASE2Function65 {    // device control - get local devices
  short int  nFctId;
}sGWTase2FCT65;

typedef struct GWTASE2Function71 {    // configuration - get configures remotes
  short int  nFctId;
}sGWTase2FCT71;

typedef struct GWTASE2Function72 {    // configuration - get remote data value names
  short int  nFctId;
}sGWTase2FCT72;

typedef struct GWTASE2Function73 {    // configuration - get remote data set names
  short int  nFctId;
}sGWTase2FCT73;

typedef struct GWTASE2Function74 {    // configuration - get remote data set attributes
  short int  nFctId;
}sGWTase2FCT74;

typedef struct GWTASE2Function75 {    // configuration -  ger remote transfer set attributes
  short int  nFctId;
}sGWTase2FCT75;

typedef struct GWTASE2Function76 {    // configuration - get remote info messages
  short int  nFctId;
}sGWTase2FCT76;

typedef struct GWTASE2Function77 {    // configuration - get remote devices
  short int  nFctId;
}sGWTase2FCT77;

typedef struct GWTASE2Function81 {    // misc - check allocated memory pointers
  short int  nFctId;
}sGWTase2FCT81;

typedef struct GWTASE2Function82 {    // misc - get iccp statistics
  short int  nFctId;
}sGWTase2FCTR82;

typedef struct GWTASE2Function83 {    // misc - set iccp-lite debug levels
  short int  nFctId;
}sGWTase2FCT83;

typedef struct GWTASE2Function84 {    // misc - enable log messages to screen
  short int  nFctId;
}sGWTase2FCT84;

typedef struct GWTASE2Function85 {    // misc - disable log messages to screen
  short int  nFctId;
}sGWTase2FCT85;

typedef struct GWTASE2Function86 {    // misc -  get sizes
  short int  nFctId;
}sGWTase2FCT86;

typedef struct GWTASE2Function87 {    // misc - reconfigure data value changes
  short int  nFctId;
}sGWTase2FCT87;

typedef struct GWTASE2Function88 {    // misc -  modify address matching
  short int  nFctId;
}sGWTase2FCT88;



//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
typedef struct GWProcessControl {
  int   nPrc;           // process number 
  char  strNam[16];     // process name
  char  strPar0[64];    // parameter 0
  char  strPar1[64];    // parameter 1
  char  strPar2[64];    // parameter 2
  char  strPar3[64];    // parameter 3
  char  strPar4[64];    // parameter 4
  int   nPrcId;         // process id
  int   nSta;           // flag
  int   nWork;          // flag
} sGWProcessCntrl;



//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
typedef struct GWProtocolControl {
  int   nPrc;    // process number 
  int   nPrt;    // protocol flag
                 //     0 = no messages
                 //     1 = error messages
                 //     2 = error, warning messages
                 //     3 = error, warning, information messages
  int   nLvl;    // protocol level (0 - 7)
} sGWProtocolCntrl;

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
typedef struct GWProtocolMessage {
  int       nTyp;
  int       nLvl;
  int       nPrc;
  int       nNum;
  long int  nTim;
  char      strMsg[256];
} sGWProtocolMsg;



#endif  // GWDATSTRUCTURE_H
