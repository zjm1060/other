#include	<stdio.h>
#include	<locale.h>
#include	<stdlib.h>
#include	<sapi.h>
#include	<sphelper.h>
#include	"../../../dms/v4.0a/inc/dms_com.h"
#include	"../inc/msaud.h"
#include	<atlconv.h>

#pragma comment(lib,"ole32.lib") //CoInitialize CoCreateInstance需要调用ole32.dll
#pragma comment(lib,"sapi.lib") //sapi.lib在SDK的lib目录,必须正确配置





static	ISpVoice	*pVoice = NULL;
static	int			pitch;
static	int			language_code;
static	WCHAR		w_outputFile[256];
static	BOOL		bOutputFile= FALSE;
static	BOOL		exLangState;


enum TTS_TYPE {TTS_SENTENCE, TTS_NUMBER, TTS_NUMBER_PERCENT, TTS_TELEGRAM};



/*English: 409; Simplified Chinese: 804*/
int	InitAudioLibraryMS(int lang_code, int nPitch, int nVolume, int nSpeed, char *outputFile, int voice_item)
{
	char								err_str[128];
	HRESULT                             hr;
	CComPtr<ISpObjectToken>             cpVoiceToken;
	CComPtr<IEnumSpObjectTokens>        cpEnum;
	ULONG                               ulCount = 0;
	WCHAR								Req_Attrs[128];
		
	USES_CONVERSION;
	
	if(GetExLangState())
		lang_code= 409;
	else
		lang_code= 804;

	if(lang_code!=409 && lang_code!=804)
	{
		Err_log("Only support english 409 or simplified chinese 804");
		return -1;
	}

	language_code= lang_code;

	if(nPitch<-10 || nPitch>10)
	{
		sprintf(err_str, "pitch value error(-10~10): %d", nPitch);
		Err_log(err_str);
		return -1;
	}
	else
		pitch= nPitch;

	if(nVolume<0 || nVolume>100)
	{
		sprintf(err_str, "volume value error(0~100): %d", nVolume);
		Err_log(err_str);
		return -1;
	}

	if(nSpeed<-10 || nSpeed>10)
	{
		sprintf(err_str, "speed value error(-10~10): %d", nSpeed);
		Err_log(err_str);
		return -1;
	}

	//COM初始化
	if (FAILED(CoInitialize(NULL)))
	{
		sprintf(err_str, "Fail to init COM");
		Err_log(err_str);
		return -1;
	}

	//获取ISpVoice接口
	if( FAILED( CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&pVoice) ) )
	{
		sprintf(err_str, "Fail to create ISpVoice instance");
		Err_log(err_str);
		CoUninitialize(); 
		return -1;
	}

	//Enumerate the available voices 
	swprintf(Req_Attrs, L"LANGUAGE=%d", lang_code);
	hr = SpEnumTokens(SPCAT_VOICES, Req_Attrs, NULL, &cpEnum);

	//Get the number of voices
	if(SUCCEEDED(hr))
		hr = cpEnum->GetCount(&ulCount);

	if(FAILED(hr))
	{
		sprintf(err_str, "Fail to get voices num");
		Err_log(err_str);
		cpEnum.Release();
		pVoice->Release();
		pVoice = NULL;
		CoUninitialize(); 
		return -1;
	}

	if(ulCount==0)
	{
		sprintf(err_str, "MS Voice don't support language %d", lang_code);
		Err_log(err_str);
		cpEnum.Release();
		pVoice->Release();
		pVoice = NULL;
		CoUninitialize(); 
		return -1;
	}
	else
	{
		sprintf(err_str, "%d ms voices supporting language %d", ulCount, lang_code);
		Err_log(err_str);
	}

	hr= -1;
	if(voice_item>=0 && voice_item<(signed)ulCount)
	{
		CSpDynamicString dstrDesc;

		cpVoiceToken.Release();
		hr = cpEnum->Item(voice_item, &cpVoiceToken);
			
		if(SUCCEEDED(hr))
		{
			hr = pVoice->SetVoice(cpVoiceToken);
			SpGetDescription(cpVoiceToken, &dstrDesc);
		}

		if(SUCCEEDED(hr))
		{
			SendTraceMsg(0, 0, "Succeed to set appointed %s voice", W2A(dstrDesc));
		}
		if(FAILED(hr))
		{
			SendTraceMsg(0, 0, "Fail to set appointed %s voice", W2A(dstrDesc));
		}
	}
	else
	{
		sprintf(err_str, "Appointed ms voice item %d isn't in range %d~%d", voice_item, 0, ulCount-1);
		Err_log(err_str);
		sprintf(err_str, "Use first find voice");
		Err_log(err_str);
	}

	if(FAILED(hr))
	{
		// Obtain a list of available voice tokens, set the voice to the token
		while(ulCount -- )
		{
			CSpDynamicString dstrDesc;

			cpVoiceToken.Release();
			hr = cpEnum->Next( 1, &cpVoiceToken, NULL );
			if(SUCCEEDED(hr))
			{
				hr = pVoice->SetVoice(cpVoiceToken);
				SpGetDescription(cpVoiceToken, &dstrDesc);
			}

			if(SUCCEEDED(hr))
			{
				SendTraceMsg(0, 0, "Succeed to set %s voice", W2A(dstrDesc));
//				break;
			}
			else
				SendTraceMsg(0, 0, "Fail to set %s voice", W2A(dstrDesc));
		}
	}
	cpVoiceToken.Release();
	cpEnum.Release();

	if(S_OK != pVoice->SetVolume(nVolume))
	{
		pVoice->Release();
		pVoice = NULL;
		CoUninitialize(); 
		return -1;
	}
	if(S_OK != pVoice->SetRate(nSpeed))
	{
		pVoice->Release();
		pVoice = NULL;
		CoUninitialize(); 
		return -1;
	}
	if(S_OK != pVoice->SetSyncSpeakTimeout(8*1000))
	{
		pVoice->Release();
		pVoice = NULL;
		CoUninitialize(); 
		return -1;
	}

	if(outputFile)
	{
		if(!MultiByteToWideChar(CP_ACP, 0, outputFile, -1, w_outputFile, sizeof(w_outputFile)))
		{
			pVoice->Release();
			pVoice = NULL;
			CoUninitialize(); 
			sprintf(err_str, "Invalid character in \"%s\"", outputFile);
			Err_log(err_str);
			return -1;
		}
		bOutputFile= TRUE;
	}
	Err_log("Success to init MS audio lib");
	return 0;
}



int ReleaseAudioLibraryMS(void)
{
	if(pVoice)
	{
		pVoice->Release();
		CoUninitialize(); 
	}
	pVoice= 0;
	return 0;
}


WCHAR*	GetNumericStrFromString(WCHAR *pSrcStr, WCHAR *pNumericStr, BOOL *bPercent)
{
	WCHAR	*pNumericStart;
	BOOL	bFindNumerics= FALSE;
	int		len= 0;

	*bPercent= FALSE;
	while(*pSrcStr)
	{
		if(!bFindNumerics)
		{
			if(iswdigit(*pSrcStr))
			{
				pNumericStart= pSrcStr;
				bFindNumerics= TRUE;
				len= 0;
			}
		}
		else
		{
			if(!iswdigit(*pSrcStr))
			{
				wcsncpy(pNumericStr, pNumericStart, len);
				pNumericStr[len]= 0;
				if(*pSrcStr==L'%')
				{
					*bPercent= TRUE;
					pSrcStr++;
				}
				return pSrcStr;
			}
		}
		pSrcStr++;
		len++;
	}
	if(bFindNumerics)
	{
		wcsncpy(pNumericStr, pNumericStart, len);
		pNumericStr[len]= 0;
		return pSrcStr;
	}
	return NULL;
} 



WCHAR*	GetSpeakStrFromString(WCHAR *pSrcStr, WCHAR *pSpeakStr, int *strType)
{
	WCHAR	*pStart;
	BOOL	bFindNumerics= FALSE;
	int		len= 0;

	pStart= pSrcStr;
	while(*pSrcStr)
	{
		if(len==0)
		{
			if(iswdigit(*pSrcStr))
				*strType= TTS_NUMBER;
			else
				*strType= TTS_SENTENCE;
		}
		else
		{
			if(*strType==TTS_SENTENCE && iswdigit(*pSrcStr))
				break;
			if(*strType==TTS_NUMBER && !iswdigit(*pSrcStr))
				break;
		}
		pSrcStr++;
		len++;
	}
	if(len>0)
	{
		if(*pSrcStr==L'%')
		{
			len++;
			pSrcStr++;
			*strType= TTS_NUMBER_PERCENT;
		}
		if(*strType==TTS_NUMBER && len>2)
			*strType= TTS_TELEGRAM;
		wcsncpy(pSpeakStr, pStart, len);
		pSpeakStr[len]= 0;
		return pSrcStr;
	}
	return NULL;
} 


int PlayStrAudioMS(char *str)
{
	char		err_str[128];
	WCHAR		wstr[256];
	WCHAR		xml_tag_str[1024];
	CComPtr<ISpStream> cpWavStream;
	WCHAR		*pNextStart, speakStr[256];
	WCHAR		*pXmlStr;
	int			strType;

	if(!pVoice)
	{  
		sprintf(err_str, "No init MS audio kernal\n"); 
		Err_log(err_str);
		return -1;
	}

	if(!MultiByteToWideChar(CP_ACP, 0, str, -1, wstr, sizeof(wstr)))
	{
		sprintf(err_str, "Invalid character in \"%s\"", str);
		Err_log(err_str);
		return -1;
	}

	pXmlStr= xml_tag_str;
	pXmlStr += swprintf(pXmlStr, L"<pitch absmiddle=\"%d\"><lang langid=\"%d\">", pitch, language_code);

	pNextStart= wstr;
	while(pNextStart=GetSpeakStrFromString(pNextStart, speakStr, &strType))
	{
		if(strType==TTS_SENTENCE || strType==TTS_NUMBER)
			pXmlStr += swprintf(pXmlStr, L"%s", speakStr);
		else if(strType==TTS_TELEGRAM)
			pXmlStr += swprintf(pXmlStr, L"<context ID=\"CHS_phone_postal\">%s</context>", speakStr);
		else if(strType==TTS_NUMBER_PERCENT)
			pXmlStr += swprintf(pXmlStr, L"<context ID=\"CHS_number_percentage\">%s</context>", speakStr);
	}
	pXmlStr += swprintf(pXmlStr, L"</lang></pitch>");
	
	if(bOutputFile)
	{
		HRESULT hr;
		CSpStreamFormat Fmt(SPSF_48kHz16BitStereo, &hr);
		if(S_OK != SPBindToFile(w_outputFile, SPFM_CREATE_ALWAYS,
			&cpWavStream, &Fmt.FormatId(), Fmt.WaveFormatExPtr() ) )
		{
			sprintf(err_str, "Failed to bind to output file");
			Err_log(err_str);
			return -1;
		}
		if(S_OK != pVoice->SetOutput(cpWavStream, TRUE))
		{
			cpWavStream.Release();
			sprintf(err_str, "Failed to set output stream");
			Err_log(err_str);
			return -1;
		}
	}

	if(S_OK!=pVoice->Speak(xml_tag_str, SPF_IS_XML, NULL))
	{
		sprintf(err_str, "Failed to play \"%s\"", str);
		Err_log(err_str);
		if(bOutputFile)
		{
			cpWavStream->Close();
			cpWavStream.Release();
		}
		return -1;
	}

	if(bOutputFile)
	{
		cpWavStream->Close();
		cpWavStream.Release();
	}
	return 0;
}


int CreateStrAudFileMS(char *str, char *outAudFile)
{
	char		err_str[128];
	WCHAR		wstr[256];
	WCHAR		xml_tag_str[1024];
	CComPtr<ISpStream> cpWavStream;
	WCHAR		*pNextStart, speakStr[256];
	WCHAR		*pXmlStr;
	WCHAR		w_outAudFile[256];
	int			strType;
	HRESULT		hr;

	if(!pVoice)
	{  
		sprintf(err_str, "No init MS audio kernal\n"); 
		Err_log(err_str);
		return -1;
	}

	if(!outAudFile)
	{  
		sprintf(err_str, "outAudFile is null\n"); 
		Err_log(err_str);
		return -1;
	}
	if(!MultiByteToWideChar(CP_ACP, 0, outAudFile, -1, w_outAudFile, sizeof(w_outAudFile)))
	{
		sprintf(err_str, "Invalid character in \"%s\"", outAudFile);
		Err_log(err_str);
		return -1;
	}

	if(!MultiByteToWideChar(CP_ACP, 0, str, -1, wstr, sizeof(wstr)))
	{
		sprintf(err_str, "Invalid character in \"%s\"", str);
		Err_log(err_str);
		return -1;
	}

	pXmlStr= xml_tag_str;
	pXmlStr += swprintf(pXmlStr, L"<pitch absmiddle=\"%d\"><lang langid=\"%d\">", pitch, language_code);

	pNextStart= wstr;
	while(pNextStart=GetSpeakStrFromString(pNextStart, speakStr, &strType))
	{
		if(strType==TTS_SENTENCE || strType==TTS_NUMBER)
			pXmlStr += swprintf(pXmlStr, L"%s", speakStr);
		else if(strType==TTS_TELEGRAM)
			pXmlStr += swprintf(pXmlStr, L"<context ID=\"CHS_phone_postal\">%s</context>", speakStr);
		else if(strType==TTS_NUMBER_PERCENT)
			pXmlStr += swprintf(pXmlStr, L"<context ID=\"CHS_number_percentage\">%s</context>", speakStr);
	}
	pXmlStr += swprintf(pXmlStr, L"</lang></pitch>");
	
	CSpStreamFormat Fmt(SPSF_48kHz16BitStereo, &hr);
	if(S_OK != SPBindToFile(w_outAudFile, SPFM_CREATE_ALWAYS,
		&cpWavStream, &Fmt.FormatId(), Fmt.WaveFormatExPtr() ) )
	{
		sprintf(err_str, "Failed to bind to %s", outAudFile);
		Err_log(err_str);
		return -1;
	}
	if(S_OK != pVoice->SetOutput(cpWavStream, TRUE))
	{
		cpWavStream.Release();
		sprintf(err_str, "Failed to set output stream");
		Err_log(err_str);
		return -1;
	}

	if(S_OK!=pVoice->Speak(xml_tag_str, SPF_IS_XML, NULL))
	{
		sprintf(err_str, "Failed to play \"%s\"", str);
		Err_log(err_str);
		cpWavStream->Close();
		cpWavStream.Release();
		return -1;
	}

	cpWavStream->Close();
	cpWavStream.Release();
	return 0;
}
