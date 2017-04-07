#include	"../inc/dms_com.h"
#include	"../inc/stringTextDef.h"


STR_TEXT_DEF *text_list=(STR_TEXT_DEF*)-1;


BOOL	GetExLangState(void)
{
#ifdef	EXTEND_LANGUAGE_SYSTEM
#ifdef	ONLY_USE_EXTEND_LANGUAGE
	return 1;
#endif
	if(text_list!=(STR_TEXT_DEF*)-1 && text_list->ex_lang_state)
		return 1;
#endif
	return 0;
}


int	SetExLangState(BOOL	ex_lang_state)
{
#ifdef	EXTEND_LANGUAGE_SYSTEM
#ifdef	ONLY_USE_EXTEND_LANGUAGE
	return 1;
#endif
	if(text_list!=(STR_TEXT_DEF*)-1)
		text_list->ex_lang_state= ex_lang_state;
#endif
	return 0;
}


char* _CS_( char* name )
{    
	GULONG		key;
	USHORT		index, hash_pn, hash_val;

	hash_pn= text_list->hash_pn;
	Swap2Byte(&hash_pn);

	GetNameStrHash(name, &key);
	hash_val= key%hash_pn;

	index= text_list->str_text_unit[hash_val].hash_index;
	Swap2Byte(&index);
	if(index>=MAX_STR_TEXT_UNIT_NUM)
		return name;

	Swap4Byte(&key);
	while(key!=text_list->str_text_unit[index].hash_key 
		|| strcmp(name, text_list->str_text_unit[index].name))
	{
		if(text_list->str_text_unit[hash_val].hash_next==0xFFFF)
			return name;
		hash_val= text_list->str_text_unit[hash_val].hash_next;
		Swap2Byte(&hash_val);
		index= text_list->str_text_unit[hash_val].hash_index;
		Swap2Byte(&index);
		if(index>=MAX_STR_TEXT_UNIT_NUM)
			return name;
	}
	if(index>=MAX_STR_TEXT_UNIT_NUM)
		return name;
	return text_list->str_text_unit[index].value[GetExLangState()];
}


int	InitStrTextDefine(void)
{
	char	filename[256];

	sprintf(filename, "%s%s", SYS_DATA_PATH, STR_TEXT_DATA_FILE);
	text_list= MountFile(filename, sizeof(STR_TEXT_DEF));
	if(text_list==(STR_TEXT_DEF*)-1)
		return -1;
	
#ifndef	WINDOWS_OS
	if(GetExLangState())
	{
		if(strcmp("EX_LANGUAGE", _CS_("EX_LANGUAGE")))
			setenv("LANG", _CS_("EX_LANGUAGE"), 1);
	}
	else
	{
		if(strcmp("LOCAL_LANGUAGE", _CS_("LOCAL_LANGUAGE")))
			setenv("LANG", _CS_("LOCAL_LANGUAGE"), 1);
	}
#endif
	return 0;
}
			

