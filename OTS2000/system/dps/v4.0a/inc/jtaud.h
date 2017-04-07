#ifndef	_DPS_JTAUD_INTERFACE_DEF
#define	_DPS_JTAUD_INTERFACE_DEF


#ifdef __cplusplus
extern "C"{
#endif 

/*nPitch:0~9, nVolume:0~9, nSpeed:0~9*/
/*if outputFile==NULL, play to device*/
int	InitAudioLibraryJT(int nPitch, int nVolume, int nSpeed, char *outputFile);
int ReleaseAudioLibraryJT(void);
int PlayStrAudioJT(char *str);
int CreateStrAudFileJT(char *str, char *outAudFile);


#ifdef __cplusplus
}
#endif


#endif  /*_DPS_JTAUD_INTERFACE_DEF*/




