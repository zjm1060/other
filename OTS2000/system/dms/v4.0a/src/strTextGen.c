#include	"../../../dms/v4.0a/inc/dms_com.h"
#include	"../../../dms/v4.0a/inc/stringTextDef.h"


#pragma	comment(lib, "wsock32.lib")
#pragma	comment(lib, "../../../dms/v4.0a/lib/dblib.lib")
#pragma	comment(lib, "../../../dps/v4.0a/lib/ipclib.lib")


int err_num;

int MakeEntryHashTable(STR_TEXT_DEF *text_list)
{
	GULONG	id;
	USHORT	hash_val, idle_pos;
	GULONG	key;
	int		repeat_times[12];
	USHORT hash_pn;

	hash_pn= FindLastPrimeNumber(MAX_STR_TEXT_UNIT_NUM);
	text_list->hash_pn= hash_pn;

	memset(repeat_times, 0 ,sizeof(repeat_times));

	for(id=0; id<MAX_STR_TEXT_UNIT_NUM; id++)
	{
		text_list->str_text_unit[id].hash_index= 0xFFFF;
		text_list->str_text_unit[id].hash_next= 0xFFFF;
	}

	for(id=0; id<text_list->def_num; id++)
	{
		GetNameStrHash(text_list->str_text_unit[id].name, &text_list->str_text_unit[id].hash_key);
		key= text_list->str_text_unit[id].hash_key;
		hash_val= key%hash_pn;
		if(text_list->str_text_unit[hash_val].hash_index!=0xFFFF)
			continue;
		text_list->str_text_unit[hash_val].hash_index= id;
	}

	idle_pos=0;
	for(id=0; id<text_list->def_num; id++)
	{
		int		times= 0;

		key= text_list->str_text_unit[id].hash_key;
		hash_val= key%hash_pn;
		if(text_list->str_text_unit[hash_val].hash_index==id)
		{
			repeat_times[times]++;		
			continue;
		}
		for(; idle_pos<MAX_STR_TEXT_UNIT_NUM; idle_pos++)
		{
			if(text_list->str_text_unit[idle_pos].hash_index==0xFFFF)
				break;
		}
		if(idle_pos>=MAX_STR_TEXT_UNIT_NUM)
		{
			printf("\nMake str text hash table error");
			getchar();
			exit(0);
		}
		text_list->str_text_unit[idle_pos].hash_index= id;
		times++;
		while(text_list->str_text_unit[hash_val].hash_next!=0xFFFF)
		{
			if(!strcmp(text_list->str_text_unit[id].name, text_list->str_text_unit[text_list->str_text_unit[hash_val].hash_index].name))
			{
				printf("\nRepeat define in str text define file: %s", text_list->str_text_unit[id].name); 
				err_num++;
			}

			times++;
			hash_val= text_list->str_text_unit[hash_val].hash_next;
		}
		if(!strcmp(text_list->str_text_unit[id].name, text_list->str_text_unit[text_list->str_text_unit[hash_val].hash_index].name))
		{
			printf("\nRepeat define in str text define file: %s", text_list->str_text_unit[id].name); 
			err_num++;
		}

		text_list->str_text_unit[hash_val].hash_next= idle_pos;
		repeat_times[times]++;		
	}

#define	STATISTIC
#ifdef	STATISTIC
	{
		FILE	*fp;
		if(fp=fopen("/home/ems/h9000/tmp/statistic.txt", "a"))
		{
			fprintf(fp, "\n\n%d: ", text_list->def_num);
			for(id=0; id<12; id++)
				fprintf(fp, "%d ", repeat_times[id]);
			fclose(fp);
		}
	}
#endif

	return 0;		
}


int MakeStrTextList(STR_TEXT_DEF *text_list)
{
	FILE	*fp;
	char	filename[256];
    char	buffer[256]; 
	char    str_num; 
	char    strbuf[3][MAX_CHAR_NUM_IN_LINE];

	memset(text_list, 0, sizeof(STR_TEXT_DEF));

	sprintf(filename, "%s%s", SYS_DEFINE_PATH, STR_TEXT_DEFINE_FILE);
	if((fp=fopen(filename, "r"))==NULL)
	{ 
		printf("\nFail to open %s", filename); 
		err_num++;
		return -1;
	}
		
	text_list->def_num= 0; 
	while( 1 )
	{ 
		if(fgets(buffer, sizeof(buffer), fp) == NULL)
			break;

		str_num= GetStringsFromStr(buffer, sizeof(strbuf)/sizeof(strbuf[0]), strbuf);
		if(str_num!=2 && str_num!=3)
			continue;

		strncpy(text_list->str_text_unit[text_list->def_num].name, strbuf[0], sizeof(text_list->str_text_unit[0].name));
		text_list->str_text_unit[text_list->def_num].name[sizeof(text_list->str_text_unit[0].name)-1]= 0;
		strncpy(text_list->str_text_unit[text_list->def_num].value[0], strbuf[1], sizeof(text_list->str_text_unit[0].value[0]));
		text_list->str_text_unit[text_list->def_num].value[0][sizeof(text_list->str_text_unit[0].value[0])-1]= 0;
		if(str_num==3)
		{
			strncpy(text_list->str_text_unit[text_list->def_num].value[1], strbuf[2], sizeof(text_list->str_text_unit[0].value[1]));
			text_list->str_text_unit[text_list->def_num].value[1][sizeof(text_list->str_text_unit[0].value[1])-1]= 0;
		}
		text_list->def_num++;
		if(text_list->def_num>=MAX_STR_TEXT_UNIT_NUM)
		{
			printf("\nToo much str text define: max %d", MAX_STR_TEXT_UNIT_NUM);
			err_num++;
			break;
		}
	}
	fclose(fp);

	MakeEntryHashTable(text_list);

	return 0;   
}
	
void SwapStrTextList(STR_TEXT_DEF *text_list)
{
	int		i;

	Swap2Byte(&text_list->hash_pn);
	Swap4Byte(&text_list->def_num);
	for(i=0; i<MAX_STR_TEXT_UNIT_NUM; i++)
	{
		Swap2Byte(&text_list->str_text_unit[i].hash_index);
		Swap2Byte(&text_list->str_text_unit[i].hash_next);
		Swap4Byte(&text_list->str_text_unit[i].hash_key);
	}
}


int	main(int argc, char *argv[])
{
	STR_TEXT_DEF text_list;
	FILE	*fp;
	char	filename[256];
	STR_TEXT_DEF*	ptr;

	printf("\nNow start to build str text list\n");

	if(-1==MakeStrTextList(&text_list))
	{
		printf("\nFail to built str text list\n");
		getchar();
		exit(0);
	}

	sprintf(filename, "%s%s", SYS_DATA_PATH, STR_TEXT_DATA_FILE);

	if((STR_TEXT_DEF*)-1==(ptr=(STR_TEXT_DEF*)MountFile(filename, sizeof(STR_TEXT_DEF))))
	{
		fp= fopen(filename, "wb");
		if(!fp)
		{
			printf("\nFail to create str text list file %s\n", filename);
			getchar();
			exit(0);
		}

		SwapStrTextList(&text_list);
		if(fwrite(&text_list, 1, sizeof(STR_TEXT_DEF), fp) != sizeof(STR_TEXT_DEF) )
		{
			printf("\nFail to write %s\n", filename);
			err_num++;
		}
		else
			printf("\nSuccess to write %s\n", filename);
		fclose(fp);
	}
	else
	{
		text_list.ex_lang_state= ptr->ex_lang_state;
		SwapStrTextList(&text_list);
		*ptr= text_list;
		UnMountFile((void*)ptr, sizeof(STR_TEXT_DEF));
		utime(filename, NULL);
		printf("\nSuccess to update %s\n", filename);
	}

	if(err_num)
		getchar();
}



			

