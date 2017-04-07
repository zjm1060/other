#ifndef     _DPPLIB_H
#define     _DPPLIB_H


#ifdef __cplusplus
extern "C"{
#endif 

unsigned short  read_bit(unsigned short ,unsigned char ,unsigned char );
unsigned short  write_bit (unsigned short ,unsigned short ,unsigned short );
unsigned short  hb_lb(unsigned short );
int Mod16(int );
int Mod8(int );

void ReadDb_ANA(LCUID,UCHAR,USHORT,USHORT, ANA_STRUCT*);
void ReadDb_IND(LCUID,UCHAR,USHORT,USHORT, IND_STRUCT*);
void ReadDb_IMP(LCUID,UCHAR,USHORT,USHORT, IMP_STRUCT*);
void ReadDb_OBJ(LCUID,UCHAR,USHORT,USHORT, DEVOBJ_STRUCT*);

void GetWordFromChar(USHORT ,USHORT ,UCHAR * ,USHORT *);
void ReadNewValue_ANA(LCUID ,UCHAR,USHORT ,USHORT ,UCHAR *,UCHAR *,UCHAR *,DPP_DATA *);
void ReadNewValue_IND(LCUID ,UCHAR,USHORT ,USHORT ,UCHAR *,UCHAR *,UCHAR *,DPP_DATA *);
void ReadNewValue_IMP(LCUID ,USHORT ,USHORT ,UCHAR *,UCHAR *,UCHAR *,DPP_DATA *);
void ReadNewValue_PARAM(LCUID,USHORT,USHORT ,UCHAR *,DPP_DATA *);
void ReadNewValue_OBJ(LCUID ,USHORT ,USHORT ,struct obj_data_seg *,DPP_DATA *);

void Process_IND_ALL(LCUID,UCHAR ,USHORT,USHORT ,DPP_DATA *);
void Process_IND(LCUID ,UCHAR ,USHORT ,USHORT , DPP_DATA *, DATE_TIME);

void Process_AI(LCUID ,USHORT ,USHORT , DPP_DATA *, DATE_TIME,UCHAR);
void Process_AO(LCUID ,USHORT ,USHORT , DPP_DATA *, DATE_TIME,UCHAR);
void Process_PI(LCUID ,USHORT ,USHORT , DPP_DATA *, DATE_TIME);
void Process_PARAM(LCUID ,USHORT ,USHORT , DPP_DATA *, DATE_TIME);
void Process_DII(LCUID ,USHORT ,USHORT , DPP_DATA *,DATE_TIME);
void Process_SOE(LCUID ,USHORT ,USHORT , UCHAR *,  DPP_DATA *);
void Process_POL(LCUID ,USHORT ,USHORT , DPP_DATA *, DATE_TIME);
void Process_DOUT(LCUID ,USHORT ,USHORT , DPP_DATA *, DATE_TIME);

void Process_OBJ(LCUID ,USHORT ,USHORT,DPP_DATA * );
void Process_OBJAlm(LCUID ,USHORT ,USHORT , UCHAR *,  DPP_DATA *,UCHAR,DATE_TIME);

void Process_LCU_MSG(LCUID ,USHORT ,USHORT , DPP_DATA *, DATE_TIME);
void Process_SYS_MSG(LCUID ,USHORT ,USHORT , DPP_DATA *, DATE_TIME);


//int ReadLcuLinkStat(LCUID,struct plc_data_seg *,DPP_DATA *);
int ReadLcuLinkStat(LCUID_INC_IDX,struct plc_data_seg *,DPP_DATA *);//cxs

void ChangeSendPro(LCUID,unsigned char *,unsigned short); 
USHORT ChangeSend_IND(LCUID,USHORT,USHORT,DIALMBUFSTRUCT *);
USHORT ChangeSend_ANA(LCUID,USHORT,USHORT,AIALMBUFSTRUCT *);
USHORT ChangeSend_IMP(LCUID,USHORT,USHORT,PIALMBUFSTRUCT *);
USHORT ChangeSend_SOE(LCUID,USHORT,USHORT,DIALMBUFSTRUCT *);
USHORT ChangeSend_OBJ(LCUID,USHORT,USHORT,OBJBUFSTRUCT *);

void AllSendPro(LCUID,unsigned char *,unsigned short);
void AllSend_ANA(LCUID, unsigned char,unsigned char,unsigned short,unsigned short,AIALLSTRUCT *,DATE_TIME); 
void AllSend_IMP(LCUID, unsigned char,unsigned char,unsigned short,unsigned short,PIALLSTRUCT *,DATE_TIME); 
void AllSend_IND(LCUID, unsigned char,unsigned short,unsigned short,IND_DATA *,DATE_TIME); 
void AllSend_SOE(LCUID,unsigned short,DI_STRUCT *,DATE_TIME);
void AllSend_OBJ(LCUID, unsigned char,unsigned char,unsigned short,unsigned short,OBJALLSTRUCT *,DATE_TIME); 

void ProcessPlcData(LCUID_INC_IDX, struct plc_data_seg *, DPP_DATA *, struct plc_send_seg *); 
void ProcessCommData(LCUID,struct plc_data_seg *, DPP_DATA *);
void ProcessData2(LCUID_INC_IDX , DPP_DATA *);

void Record_IND(unsigned short,DIALMBUFSTRUCT *);
void Record_ANA(unsigned short,AIALMBUFSTRUCT *);
void Record_IND_ALL(UCHAR,unsigned short,unsigned short, IND_DATA*);
void Record_ChangeBuf(LAN_HEAD, unsigned char *);
void Record_AllSendBuf(LAN_HEAD, unsigned char *);

unsigned short ReadDefineFile(LCUID, SENDATA *);
void Process_Sendata(LCUID,SENDATA *,struct plc_send_seg *);



/*int   Init_DppData();*/
void  init(DPP_DATA *);
void* scada(void*);


int  PollSysRtu();
int  CommAnaAlmPro(POINTER, float, unsigned short, DATE_TIME);


void CommWriteANA(LCUID,unsigned short,unsigned short,unsigned short,unsigned short,unsigned char *,unsigned char *);
void CommWriteIMP(LCUID,unsigned short,unsigned short,unsigned short,unsigned short,unsigned char *,unsigned char *);
void CommWritePOL(LCUID,unsigned short,unsigned short,unsigned short,unsigned short,unsigned char *,unsigned char *);


#ifdef __cplusplus
}
#endif



#endif   /*_DPPLIB_H*/

