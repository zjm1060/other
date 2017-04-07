#ifndef	_DPS_MSAUD_INTERFACE_DEF
#define	_DPS_MSAUD_INTERFACE_DEF

#ifdef __cplusplus
extern "C"{
#endif 

/*nPitch:-10~10, nVolume:0~100, nSpeed:-10~10*/
/*if outputFile==NULL, play to device*/
/*English: 409; Simplified Chinese: 804*/
int	InitAudioLibraryMS(int lang_code, int nPitch, int nVolume, int nSpeed, char *outputFile, int ms_voice_item);
int ReleaseAudioLibraryMS(void);
int PlayStrAudioMS(char *str);
int CreateStrAudFileMS(char *str, char *outAudFile);

#ifdef __cplusplus
}
#endif

#endif  /*_DPS_MSAUD_INTERFACE_DEF*/

