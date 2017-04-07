#include "../inc/gtk_auto_check.h"
#ifdef SUN_OS
#include <pthread.h>
#endif
int ProgAutoCheck(GtkWidget*parent,int crt_n);
void PopupAutocheckFile(int crt_n,char* pathfilename);

void printfslist(char*str,GSList*list)
{
	POINTER* point;
	int i;
	printf("/////////////////////////\n");
	printf("!!!!start print %s  ,length=%d \n",str,g_slist_length(list));
	for (i=0;i<(int)g_slist_length(list);i++)
	{
		point=g_slist_nth(list, i)->data;
		printf("%d.%d.%d.%d\n",point->stn_id,point->dev_id,point->data_type,point->pt_id);
	}
	printf("!!!!end print %s  \n",str);
	printf("/////////////////////////\n\n");
	
}
void freeCheckresult()
{
	
	GSList*iterator = NULL;
	long length;
//	POINTER* ppoint; 
	GSList*item = NULL; 

	length=g_slist_length(newpt_result.Scan_state);
	//	if (g_slist_length(newpt_result.Scan_state)>0)
	{
		printfslist("Free newpt_result.Scan_state",newpt_result.Scan_state);
		for (iterator = newpt_result.Scan_state; iterator; iterator = iterator->next) 
			free(iterator->data);
		//for (i=0;i<g_slist_length(newpt_result.Scan_state);i++)
		//	{
		//		ppoint=g_slist_nth(newpt_result.Scan_state, i)->data;
		//	}
	}
	for (iterator = newpt_result.Alm_state; iterator; iterator = iterator->next) { 
		free(iterator->data);
    }
	for (iterator = newpt_result.Alm_ack; iterator; iterator = iterator->next) { 
		free(iterator->data);
    }
	
	for (iterator = newpt_result.Tag_state; iterator; iterator = iterator->next) { 
		free(iterator->data);
    } 
	//	printfslist("newpt_result.Man_set",newpt_result.Man_set);
	for (iterator = newpt_result.Man_set; iterator; iterator = iterator->next) { 
		free(iterator->data);
    }
	
	g_slist_free(newpt_result.Scan_state); 
	g_slist_free(newpt_result.Alm_state); 
	g_slist_free(newpt_result.Alm_ack); 
	
	g_slist_free(newpt_result.Tag_state);
	g_slist_free(newpt_result.Man_set); 
	
	//free后必须初始化，否则链表长度为1
	newpt_result.Scan_state=NULL; 
	newpt_result.Alm_state=NULL; 
	newpt_result.Alm_ack=NULL; 
	newpt_result.Tag_state=NULL;
	newpt_result.Man_set=NULL; 

	if (checkresult.Scan_state)
	{
		for (iterator = checkresult.Scan_state; iterator; iterator = iterator->next) { 
			free(iterator->data);
		}
	}
	for (iterator = checkresult.Alm_state; iterator; iterator = iterator->next) { 
		free(iterator->data);
    }
	for (iterator = checkresult.Alm_ack; iterator; iterator = iterator->next) { 
		free(iterator->data);
    }
	
	for (iterator = checkresult.Tag_state; iterator; iterator = iterator->next) { 
		free(iterator->data);
    } 
	for (iterator = checkresult.Man_set; iterator; iterator = iterator->next) { 
		free(iterator->data);
    }
	
	g_slist_free(checkresult.Scan_state); 
	g_slist_free(checkresult.Alm_state); 
	g_slist_free(checkresult.Alm_ack); 
	
	g_slist_free(checkresult.Tag_state);
	g_slist_free(checkresult.Man_set); 
	
	//free后必须初始化，否则链表长度为1
	checkresult.Scan_state=NULL; 
	checkresult.Alm_state=NULL; 
	checkresult.Alm_ack=NULL; 
	
	checkresult.Tag_state=NULL;
	checkresult.Man_set=NULL; 
	
	//printfslist("oldpt_result.Scan_state",oldpt_result.Scan_state);
	
	for (iterator = oldpt_result.Scan_state; iterator; iterator = iterator->next) { 
		free(iterator->data);
    } 
	printfslist("oldpt_result.Alm_state",oldpt_result.Alm_state);
	
	for (iterator = oldpt_result.Alm_state; iterator; iterator = iterator->next) { 
		free(iterator->data);
    }
	for (iterator = oldpt_result.Alm_ack; iterator; iterator = iterator->next) { 
		free(iterator->data);
    }
	
	for (iterator = oldpt_result.Tag_state; iterator; iterator = iterator->next) { 
		free(iterator->data);
    } 
	for (iterator = oldpt_result.Man_set; iterator; iterator = iterator->next) { 
		free(iterator->data);
    }
	
	g_slist_free(oldpt_result.Scan_state); 
	g_slist_free(oldpt_result.Alm_state); 
	g_slist_free(oldpt_result.Alm_ack); 
	
	g_slist_free(oldpt_result.Tag_state);
	g_slist_free(oldpt_result.Man_set); 
	
	//free后必须初始化，否则链表长度为1
	oldpt_result.Scan_state=NULL; 
	oldpt_result.Alm_state=NULL; 
	oldpt_result.Alm_ack=NULL; 
	
	oldpt_result.Tag_state=NULL;
	oldpt_result.Man_set=NULL; 
	
	
	
    for (iterator = delpt_result.Scan_state; iterator; iterator = iterator->next) { 
		free(iterator->data);
    } 
	for (iterator = delpt_result.Alm_state; iterator; iterator = iterator->next) { 
		free(iterator->data);
    }
	for (iterator = delpt_result.Alm_ack; iterator; iterator = iterator->next) { 
		free(iterator->data);
    }
	
	for (iterator = delpt_result.Tag_state; iterator; iterator = iterator->next) { 
		free(iterator->data);
    } 
	for (iterator = delpt_result.Man_set; iterator; iterator = iterator->next) { 
		free(iterator->data);
    }
	
	g_slist_free(delpt_result.Scan_state); 
	g_slist_free(delpt_result.Alm_state); 
	g_slist_free(delpt_result.Alm_ack); 
	
	g_slist_free(delpt_result.Tag_state);
	g_slist_free(delpt_result.Man_set); 
	
	//free后必须初始化，否则链表长度为1
	delpt_result.Scan_state=NULL; 
	delpt_result.Alm_state=NULL; 
	delpt_result.Alm_ack=NULL; 
	delpt_result.Tag_state=NULL;
	delpt_result.Man_set=NULL; 
	
}
gint pt_finder(gconstpointer item) {
	return g_ascii_strcasecmp(item, "second");
} 

void checkPropertyOption(DMS_COMMON db_cmm)
{
	GSList*item = NULL; 
	int i;
	int flag;
	int length;
	
	POINTER* oldppoint; 
	POINTER* tmppoint; 
	if (!db_cmm.fixed.iost.scanEnable)//扫查禁止
	{
		
		POINTER* ppoint = (POINTER*)malloc(sizeof(POINTER)); 
		*ppoint=db_cmm.point;
		ppoint->update_src=0;
		checkresult.Scan_state = g_slist_prepend  (checkresult.Scan_state, ppoint);
		flag=0;
		length=g_slist_length(delpt_result.Scan_state);
		//	for (i=0;i<g_slist_length(delpt_result.Scan_state);i++)
		for (i=g_slist_length(delpt_result.Scan_state)-1;i>=0;i--)
		{
			oldppoint=g_slist_nth(delpt_result.Scan_state, i)->data;
			if(memcmp((void*)ppoint , (void*)oldppoint,sizeof(POINTER)) ==0 )
			{
				
				item =  g_slist_nth(delpt_result.Scan_state,i);	
				delpt_result.Scan_state=g_slist_delete_link (delpt_result.Scan_state,item);
				oldpt_result.Scan_state=g_slist_prepend(oldpt_result.Scan_state,oldppoint);
				flag=1;
				break;
			}
		}
		if (!flag)
		{
			printfslist("before checkPropertyOption newpt_result.Scan_state",newpt_result.Scan_state);	
			tmppoint = (POINTER*)malloc(sizeof(POINTER));
			memcpy(tmppoint,ppoint,sizeof(POINTER));
			newpt_result.Scan_state=g_slist_prepend  (newpt_result.Scan_state, tmppoint);
			printfslist("end checkPropertyOption newpt_result.Scan_state",newpt_result.Scan_state);	
		}
		
	}
	
	
	if (!db_cmm.fixed.iost.almEnable)//报警禁止
	{
		
		POINTER* ppoint = (POINTER*)malloc(sizeof(POINTER)); 
		*ppoint=db_cmm.point;
		ppoint->update_src=0;
		checkresult.Alm_state = g_slist_prepend  (checkresult.Alm_state, ppoint);
		flag=0;
		
		//for (i=0;i<g_slist_length(delpt_result.Alm_state);i++)
		for (i=g_slist_length(delpt_result.Alm_state)-1;i>=0;i--)
		{
			oldppoint=g_slist_nth(delpt_result.Alm_state, i)->data;
			if(memcmp((void*)ppoint , (void*)oldppoint,sizeof(POINTER)) ==0 )
			{
				//	free(oldppoint);
				item =  g_slist_nth(delpt_result.Alm_state,i);
				delpt_result.Alm_state=g_slist_delete_link (delpt_result.Alm_state,item);
				oldpt_result.Alm_state=g_slist_prepend(oldpt_result.Alm_state,oldppoint);
				flag=1;
				break;
			}
		}
		if (!flag)
		{
			tmppoint = (POINTER*)malloc(sizeof(POINTER));
			memcpy(tmppoint,ppoint,sizeof(POINTER));
			newpt_result.Alm_state=g_slist_prepend  (newpt_result.Alm_state, tmppoint);
		}
		
		
		
	}
	
	if (db_cmm.fixed.iost.almNoAck==1)//报警未确认
	{
		POINTER* ppoint = (POINTER*)malloc(sizeof(POINTER)); 
		*ppoint=db_cmm.point;
		
		ppoint->update_src=0;
		checkresult.Alm_ack = g_slist_prepend  (checkresult.Alm_ack, ppoint);
		flag=0;
		
		//	for (i=0;i<g_slist_length(delpt_result.Alm_ack);i++)
		for (i=g_slist_length(delpt_result.Alm_ack)-1;i>=0;i--)
		{
			oldppoint=g_slist_nth(delpt_result.Alm_ack, i)->data;
			if(memcmp((void*)ppoint , (void*)oldppoint,sizeof(POINTER)) ==0 )
			{
				//		free(oldppoint);
				item =  g_slist_nth(delpt_result.Alm_ack,i);
				delpt_result.Alm_ack=g_slist_delete_link (delpt_result.Alm_ack,item);
				oldpt_result.Alm_ack=g_slist_prepend(oldpt_result.Alm_ack,oldppoint);
				flag=1;
				break;
			}
		}
		if (!flag)
		{
			tmppoint = (POINTER*)malloc(sizeof(POINTER));
			memcpy(tmppoint,ppoint,sizeof(POINTER));
			newpt_result.Alm_ack=g_slist_prepend  (newpt_result.Alm_ack, tmppoint);
		}
	}
	
	
	
	if ( db_cmm.fixed.iost.tagState>0)//挂牌
	{
		POINTER* ppoint = (POINTER*)malloc(sizeof(POINTER)); 
		*ppoint=db_cmm.point;
		
		//	checkresult.Tag_state=g_slist_prepend (checkresult.Tag_state, ppoint);
		ppoint->update_src=0;
		checkresult.Tag_state = g_slist_prepend  (checkresult.Tag_state, ppoint);
		flag=0;
		
		//	for (i=0;i<g_slist_length(delpt_result.Tag_state);i++)
		for (i=g_slist_length(delpt_result.Tag_state)-1;i>=0;i--)
		{
			oldppoint=g_slist_nth(delpt_result.Tag_state, i)->data;
			if(memcmp((void*)ppoint , (void*)oldppoint,sizeof(POINTER)) ==0 )
			{
				//		free(oldppoint);
				item =  g_slist_nth(delpt_result.Tag_state,i);
				delpt_result.Tag_state=g_slist_delete_link (delpt_result.Tag_state,item);
				oldpt_result.Tag_state=g_slist_prepend(oldpt_result.Tag_state,oldppoint);
				flag=1;
				break;
			}
		}
		if (!flag)
		{
			tmppoint = (POINTER*)malloc(sizeof(POINTER));
			memcpy(tmppoint,ppoint,sizeof(POINTER));
			newpt_result.Tag_state=g_slist_prepend  (newpt_result.Tag_state, tmppoint);
		}
	}
	

	//if (db_cmm.var.ind.status.man_set==1)//人工置数
	//扫查禁止&&报警禁止 表示人工修改实时库
	if((!db_cmm.fixed.iost.scanEnable)&&(!db_cmm.fixed.iost.almEnable))
	{
		POINTER* ppoint = (POINTER*)malloc(sizeof(POINTER)); 
		*ppoint=db_cmm.point;
		
		//checkresult.Man_set=g_slist_prepend (checkresult.Man_set, ppoint);
		ppoint->update_src=0;
		checkresult.Man_set = g_slist_prepend  (checkresult.Man_set, ppoint);
		flag=0;
		
		//	for (i=0;i<g_slist_length(delpt_result.Scan_state);i++)
		for (i=g_slist_length(delpt_result.Man_set)-1;i>=0;i--)
		{
			oldppoint=g_slist_nth(delpt_result.Man_set, i)->data;
			if(memcmp((void*)ppoint , (void*)oldppoint,sizeof(POINTER)) ==0 )
			{
				//		free(oldppoint);
				item =  g_slist_nth(delpt_result.Man_set,i);
				delpt_result.Man_set=g_slist_delete_link (delpt_result.Man_set,item);
				oldpt_result.Man_set=g_slist_prepend(oldpt_result.Man_set,oldppoint);
				flag=1;
				break;
			}
		}
		if (!flag)
		{
			tmppoint = (POINTER*)malloc(sizeof(POINTER));
			memcpy(tmppoint,ppoint,sizeof(POINTER));
			newpt_result.Man_set=g_slist_prepend  (newpt_result.Man_set, tmppoint);
		}
	}
	
}


int openChkRst2DAT()
{
	
	FILE	*fp;
	char filename[200];
	int num,num1;
	POINTER* ppoint;
	int i;
	sprintf(filename, "%s%s", AUTOCHECK_DIR,AUTOCHECK_DATA);
	fp= fopen(filename, "rb");
	
	if (!fp)
	{	
		printf("Open %s file error \n",filename);
		return 0;
	}
	
	//扫查禁止
	num1=fread(&num,sizeof(int),1,fp);
	for (i=0;i<num;i++)
	{
		
		ppoint = (POINTER*)malloc(sizeof(POINTER)); 
		num1=fread(ppoint,sizeof(POINTER),1,fp);
		delpt_result.Scan_state = g_slist_prepend  (delpt_result.Scan_state, ppoint);
		
		//	printf("openChkRst2DAT ppoint Scan_state stn=%d,dev=%d,type=%d,ptid=%d\n",ppoint->stn_id,ppoint->dev_id,ppoint->data_type,ppoint->pt_id);
		ppoint=NULL;
	}
	
	
	//报警禁止
	num1=fread(&num,sizeof(int),1,fp);
	for (i=0;i<num;i++)
	{
		
		ppoint = (POINTER*)malloc(sizeof(POINTER)); 
		num1=fread(ppoint,sizeof(POINTER),1,fp);
		delpt_result.Alm_state = g_slist_prepend  (delpt_result.Alm_state, ppoint);
		
		//		printf("openChkRst2DAT Alm_state stn=%d,dev=%d,type=%d,ptid=%d\n",ppoint->stn_id,ppoint->dev_id,ppoint->data_type,ppoint->pt_id);
		ppoint=NULL;
	}
	
	
	//报警未确认
	num1=fread(&num,sizeof(int),1,fp);
	for (i=0;i<num;i++)
	{
		
		ppoint = (POINTER*)malloc(sizeof(POINTER)); 
		num1=fread(ppoint,sizeof(POINTER),1,fp);
		delpt_result.Alm_ack = g_slist_prepend  (delpt_result.Alm_ack, ppoint);
		
		//		printf("Alm_ack stn=%d,dev=%d,type=%d,ptid=%d\n",ppoint->stn_id,ppoint->dev_id,ppoint->data_type,ppoint->pt_id);
		ppoint=NULL;
	}
	
	//挂牌
	num1=fread(&num,sizeof(int),1,fp);
	for (i=0;i<num;i++)
	{
		
		ppoint = (POINTER*)malloc(sizeof(POINTER)); 
		num1=fread(ppoint,sizeof(POINTER),1,fp);
		delpt_result.Tag_state = g_slist_prepend  (delpt_result.Tag_state, ppoint);
		
		//		printf("Tag_state stn=%d,dev=%d,type=%d,ptid=%d\n",ppoint->stn_id,ppoint->dev_id,ppoint->data_type,ppoint->pt_id);
		ppoint=NULL;
	}
	
	//人工置数
	num1=fread(&num,sizeof(int),1,fp);
	for (i=0;i<num;i++)
	{
		
		ppoint = (POINTER*)malloc(sizeof(POINTER)); 
		num1=fread(ppoint,sizeof(POINTER),1,fp);
		delpt_result.Man_set = g_slist_prepend  (delpt_result.Man_set, ppoint);
		
		//		printf("Man_set stn=%d,dev=%d,type=%d,ptid=%d\n",ppoint->stn_id,ppoint->dev_id,ppoint->data_type,ppoint->pt_id);
		ppoint=NULL;
	}
	
	
	
	fclose(fp);
	
	
	
	printf("g_slist_length(delpt_result.Scan_state=%d\n",g_slist_length(delpt_result.Scan_state));
	// 	printf("g_slist_length(delpt_result.Alm_state=%d\n",g_slist_length(delpt_result.Alm_state));
	// 	printf("g_slist_length(delpt_result.Alm_ack=%d\n",g_slist_length(delpt_result.Alm_ack));
	// 	printf("g_slist_length(delpt_result.Man_set=%d\n",g_slist_length(delpt_result.Man_set));
	
	return 1;
	
}
#ifdef _AIX
#include <iconv.h>
int code_convert(char *from_charset,char *to_charset,char *inbuf,size_t *inlen,char *outbuf,size_t *outlen)
{
	iconv_t cd;
	int rc;
	char **pin = &inbuf;
	char **pout = &outbuf;
	
	cd = iconv_open(to_charset,from_charset);
	if (cd==0) return -1;
	memset(outbuf,0,*outlen);
	if (iconv(cd,pin,inlen,pout,outlen)==-1)
	{
		return -1;
	}
	iconv_close(cd);
	return 0;
}

//UNICODE码转为GBK码
int u2g(char *inbuf,size_t *inlen,char *outbuf,size_t *outlen)
{
	return code_convert("UTF-8","GBK",inbuf,inlen,outbuf,outlen);
}
//GBK码转为UNICODE码
int g2u(char *inbuf,size_t *inlen,char *outbuf,size_t *outlen)
{
	code_convert("GBK","UTF-8",inbuf,inlen,outbuf,outlen);
	
}
#endif

#ifdef	WINDOWS_OS
wchar_t * ANSIToUnicode( const char* str )
{
	int textlen ;
	wchar_t * result;
	textlen = MultiByteToWideChar( CP_ACP, 0, str,-1, NULL,0 );
	result = (wchar_t *)malloc((textlen+1)*sizeof(wchar_t));
	memset(result,0,(textlen+1)*sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0,str,-1,(LPWSTR)result,textlen );
	return result;
}
char * UnicodeToUTF8( const wchar_t* str )
{
	char* result;
	int textlen;
	textlen = WideCharToMultiByte( CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL );
	result =(char *)malloc((textlen+1)*sizeof(char));
	memset(result, 0, sizeof(char) * ( textlen + 1 ) );
	WideCharToMultiByte( CP_UTF8, 0, str, -1, result, textlen, NULL, NULL );
	return result;
}
#endif
void write2utf8(FILE	*fp_file,char*xlsbuf)
{
#ifdef	WINDOWS_OS
	wchar_t *wdxlsbuf;	
#endif
	char* utf8buf;
	
	
#ifdef	WINDOWS_OS
	wdxlsbuf=ANSIToUnicode(xlsbuf);
	utf8buf=UnicodeToUTF8(wdxlsbuf);
	fprintf(fp_file,utf8buf);
	free(wdxlsbuf);
	free(utf8buf);
#else
	//g2u(xlsbuf,sizeof(xlsbuf),utf8buf,NULL);
	fprintf(fp_file,xlsbuf);
#endif
}

void writeOneXmlRow(FILE	*fp_file,int type,int index,char*pointid,char*entry_name,char*LongNameStr)
{
	char xlsbuf[1024];
	char sstype[5];
	
	switch(type )
    {
	case NewResult:
		sprintf(sstype,"s66");
		break;
	case DelResult:
		sprintf(sstype,"s81");
		break;
	case OldResult:
		sprintf(sstype,"s26");
		break;
	case AllResult:
		sprintf(sstype,"s26");
		break;
	default:
		sprintf(sstype,"s26");
	}
	
	
	fprintf(fp_file,"	<Row ss:AutoFitHeight='0' ss:StyleID='s26'>\n");
	
	fprintf(fp_file,"		<Cell ss:StyleID='%s'>\n",sstype);
	sprintf(xlsbuf,"		  <Data ss:Type='String'>%d</Data>\n",index);
	
	write2utf8(fp_file,xlsbuf);
	
	fprintf(fp_file,"		</Cell>\n");
	
	
	fprintf(fp_file,"		<Cell ss:StyleID='%s'>\n",sstype);
	sprintf(xlsbuf,"		  <Data ss:Type='String'>%s</Data>\n",pointid);
	
	write2utf8(fp_file,xlsbuf);
	fprintf(fp_file,"		</Cell>\n");
	
	
	fprintf(fp_file,"		<Cell ss:StyleID='%s'>\n",sstype);
	sprintf(xlsbuf,"		  <Data ss:Type='String'>%s</Data>\n",entry_name);
	
	write2utf8(fp_file,xlsbuf);
	fprintf(fp_file,"		</Cell>\n");
	
	
	fprintf(fp_file,"		<Cell ss:StyleID='%s'>\n",sstype);
	sprintf(xlsbuf,"		  <Data ss:Type='String'>%s</Data>\n",LongNameStr);
	
	write2utf8(fp_file,xlsbuf);
	fprintf(fp_file,"		</Cell>\n");
	
	fprintf(fp_file,"	</Row>\n");
}

//comp_mode=0 all result ,   resultfile:txtfile name,resultdata:datfile name, CHECK_RESULT*all_result all result ,del_ptresult=NULL 
//comp_mode=1 compare result,resultfile:compare result name resultdata:NULL , CHECK_RESULT*all_result new resultpt,
int saveChkRst2file(char* resultfile,char* resultdata,int comp_mode,CHECK_RESULT*all_result,CHECK_RESULT* del_ptresult)
{
	
	FILE	*fp_file,*fp_xls,*fp_dat;
	POINTER* point;
	char	buf[10240],xlsbuf[10240], *ptr= buf;
	// #ifdef	WINDOWS_OS
	// 	wchar_t *wdxlsbuf;
	// 	char* utf8buf;
	// #endif
	int num;
	int i,j;
	char filename[200],filexml[200];
	char pointid[20];
	char entry_name[ENTRY_NAME_SIZE];
	char LongNameStr[100];
	char LongNamerStr1[128];
	DMS_COMMON	db_cmm;
	
	
	
	sprintf(filename, "%s%s", AUTOCHECK_DIR,resultfile);
	fp_file= fopen(filename, "w");
	
	sprintf(filexml, "%s%s", AUTOCHECK_DIR,AUTOCHECK_XLS);
	fp_xls= fopen(filexml, "w");
	
	if (!fp_xls)
	{	
		printf("Open %s file error \n",filename);
	}
	//sprintf(xlsbuf,AUTOCHECKXMLHEADSTR);
	fprintf(fp_xls,AUTOCHECKXMLHEADSTR);
	
	if (!fp_file)
	{	
		printf("Open %s file error \n",filename);
	}
	if (!comp_mode)
	{
		sprintf(filename, "%s%s", AUTOCHECK_DIR,resultdata);
		fp_dat= fopen(filename, "wb");
		
		if (!fp_dat)
		{	
			printf("Open %s file error \n",filename);
		}
	}
	///////////////////////扫查禁止//////////////////////////////////////////////////
	if (!comp_mode)
	{
		sprintf(buf,"%s all result \n",_CS_(OIX_CSTR_ScanDisable));
		fprintf(fp_file,buf);
	}else
	{
		sprintf(buf,"%s new result \n",_CS_(OIX_CSTR_ScanDisable));
		fprintf(fp_file,buf);
	}
	sprintf(buf,"%s \t %s \t %s \t %s \t \n",_CS_(OIX_CSTR_SEQ_NO),_CS_(OIX_STR_IO_ADDRESS),_CS_(OIX_STR_REC_LOGNAME),_CS_(OIX_CSTR_LONGNAME));
	fprintf(fp_file,buf);
	
	//////////////////////////////////////////////////////////////////////////
	sprintf(xlsbuf,"<Worksheet ss:Name='扫查禁止'>\n");
	// #ifdef	WINDOWS_OS
	// 	wdxlsbuf=ANSIToUnicode(xlsbuf);
	// 	utf8buf=UnicodeToUTF8(wdxlsbuf);
	// 	fprintf(fp_file,utf8buf);
	// #endif
	write2utf8(fp_xls,xlsbuf);
	
	fprintf(fp_xls,AUTOCHECKXMLCOL);
	
	// #ifdef	WINDOWS_OS
	// 	wdxlsbuf=ANSIToUnicode(AUTOCHECKHEADXML);
	// 	utf8buf=UnicodeToUTF8(wdxlsbuf);
	// 	fprintf(fp_xls,utf8buf);
	// #endif
	write2utf8(fp_xls,AUTOCHECKHEADXML);
	
	//////////////////////////////////////////////////////////////////////////
	
	if (!comp_mode)
	{
		num=g_slist_length(all_result->Scan_state) ;
		fwrite(&num,sizeof(int),1,fp_dat);
	}
	for (i=g_slist_length(all_result->Scan_state)-1,j=0;i>=0;i--,j++)
	{
		point=g_slist_nth(all_result->Scan_state, i)->data;
		
		if ( ReadEntryById (point, &db_cmm ) == -1)
			continue;
		strcpy(entry_name,db_cmm.fixed.entryName);
		GetGroupLongnameById( db_cmm.point.stn_id, db_cmm.point.dev_id, LongNameStr );
		GetEntryLongnameById (&db_cmm.point,LongNamerStr1 );
		strcat( LongNameStr, LongNamerStr1 );
		
		sprintf(pointid,"%d.%d.%d.%d",point->stn_id,point->dev_id,point->data_type,point->pt_id);
		sprintf(buf,"%d\t%s\t%s\t%s \n",j,pointid,entry_name,LongNameStr);
		
		fprintf(fp_file, "%s", buf);
		
		writeOneXmlRow(fp_xls,AllResult,j,pointid,entry_name,LongNameStr);
		if (!comp_mode)
		{
			fwrite(point,sizeof(POINTER),1,fp_dat);
		}	//fflush(fp_file);
	}
	
	
	fprintf(fp_xls,"</Table>\n");
	fprintf(fp_xls,"</Worksheet>\n");	
	
	if (!comp_mode)
	{
		sprintf(buf,"%s all total\t%d \n\n",_CS_(OIX_CSTR_ScanDisable),j);
		fprintf(fp_file,buf);
	}else
	{
		sprintf(buf,"%s new total\t%d \n\n",_CS_(OIX_CSTR_ScanDisable),j);
		fprintf(fp_file,buf);
	}
	
	if (comp_mode==1)
	{
		sprintf(buf,"%s del result \n",_CS_(OIX_CSTR_ScanDisable));
		fprintf(fp_file,buf);
		
		for (i=g_slist_length(del_ptresult->Scan_state)-1,j=0;i>=0;i--,j++)
		{
			
			point=g_slist_nth(del_ptresult->Scan_state, i)->data;
			
			if ( ReadEntryById (point, &db_cmm ) == -1)
				continue;
			strcpy(entry_name,db_cmm.fixed.entryName);
			GetGroupLongnameById( db_cmm.point.stn_id, db_cmm.point.dev_id, LongNameStr );
			GetEntryLongnameById (&db_cmm.point,LongNamerStr1 );
			strcat( LongNameStr, LongNamerStr1 );
			
			sprintf(pointid,"%d.%d.%d.%d",point->stn_id,point->dev_id,point->data_type,point->pt_id);
			sprintf(buf,"%d\t%s\t%s\t%s \n",j,pointid,entry_name,LongNameStr);
			
			fprintf(fp_file, "%s", buf);
			
			
		}
		
		
		sprintf(buf,"%s del\t%d \n\n",_CS_(OIX_CSTR_ScanDisable),j);
		fprintf(fp_file,buf);
	}
	
	//////////////////////报警禁止///////////////////////////////////////////////////
	
	if (!comp_mode)
	{
		sprintf(buf,"%s all result \n",_CS_(OIX_CSTR_AlarmDisable));
		fprintf(fp_file,buf);
	}else
	{
		sprintf(buf,"%s new result \n",_CS_(OIX_CSTR_AlarmDisable));
		fprintf(fp_file,buf);
	}
	//////////////////////////////////////////////////////////////////////////
	sprintf(xlsbuf,"<Worksheet ss:Name='报警禁止'>\n");
	// #ifdef	WINDOWS_OS
	// 	wdxlsbuf=ANSIToUnicode(xlsbuf);
	// 	utf8buf=UnicodeToUTF8(wdxlsbuf);
	// 	fprintf(fp_xls,utf8buf);
	// #endif
	write2utf8(fp_xls,xlsbuf);
	
	fprintf(fp_xls,AUTOCHECKXMLCOL);
	
	// #ifdef	WINDOWS_OS
	// 	wdxlsbuf=ANSIToUnicode(AUTOCHECKHEADXML);
	// 	utf8buf=UnicodeToUTF8(wdxlsbuf);
	// 	fprintf(fp_xls,utf8buf);
	// #endif
	write2utf8(fp_xls,AUTOCHECKHEADXML);
	
	//////////////////////////////////////////////////////////////////////////
	sprintf(buf,"%s \t %s \t %s \t %s \t \n",_CS_(OIX_CSTR_SEQ_NO),_CS_(OIX_STR_IO_ADDRESS),_CS_(OIX_STR_REC_LOGNAME),_CS_(OIX_CSTR_LONGNAME),i);
	fprintf(fp_file,buf);
	if (!comp_mode)
	{
		num=g_slist_length(all_result->Alm_state) ;
		fwrite(&num,sizeof(int),1,fp_dat);
	}
	
	for (i=g_slist_length(all_result->Alm_state)-1,j=0;i>=0;i--,j++)
	{
		
		point=g_slist_nth(all_result->Alm_state, i)->data;
		
		if ( ReadEntryById (point, &db_cmm ) == -1)
			continue;
		strcpy(entry_name,db_cmm.fixed.entryName);
		GetGroupLongnameById( db_cmm.point.stn_id, db_cmm.point.dev_id, LongNameStr );
		GetEntryLongnameById (&db_cmm.point,LongNamerStr1 );
		strcat( LongNameStr, LongNamerStr1 );
		
		sprintf(pointid,"%d.%d.%d.%d",point->stn_id,point->dev_id,point->data_type,point->pt_id);
		sprintf(buf,"%d\t%s\t%s\t%s \n",j,pointid,entry_name,LongNameStr);
		
		fprintf(fp_file, "%s", buf);
		
		writeOneXmlRow(fp_xls,AllResult,j,pointid,entry_name,LongNameStr);
		if (!comp_mode)
		{	
			fwrite(point,sizeof(POINTER),1,fp_dat);
		}
	}
	
	if (!comp_mode)
	{
		sprintf(buf,"%s all total\t%d \n\n",_CS_(OIX_CSTR_AlarmDisable),j);
		fprintf(fp_file,buf);
	}else
	{
		sprintf(buf,"%s new total\t%d \n\n",_CS_(OIX_CSTR_AlarmDisable),j);
		fprintf(fp_file,buf);
	}
	
	fprintf(fp_xls,"</Table>\n");
	fprintf(fp_xls,"</Worksheet>\n");
	
	if (comp_mode==1)
	{
		sprintf(buf,"%s del result \n",_CS_(OIX_CSTR_AlarmDisable));
		fprintf(fp_file,buf);
		
		for (i=g_slist_length(del_ptresult->Alm_state)-1,j=0;i>=0;i--)
		{
			
			point=g_slist_nth(del_ptresult->Alm_state, i)->data;
			
			if ( ReadEntryById (point, &db_cmm ) == -1)
				continue;
			j++;
			strcpy(entry_name,db_cmm.fixed.entryName);
			GetGroupLongnameById( db_cmm.point.stn_id, db_cmm.point.dev_id, LongNameStr );
			GetEntryLongnameById (&db_cmm.point,LongNamerStr1 );
			strcat( LongNameStr, LongNamerStr1 );
			
			sprintf(pointid,"%d.%d.%d.%d",point->stn_id,point->dev_id,point->data_type,point->pt_id);
			sprintf(buf,"%d\t%s\t%s\t%s \n",j,pointid,entry_name,LongNameStr);
			
			fprintf(fp_file, "%s", buf);
			
			
		}
		
		
		sprintf(buf,"%s del\t%d \n\n",_CS_(OIX_CSTR_AlarmDisable),j);
		fprintf(fp_file,buf);
	}
	
	//////////////////////报警未确认///////////////////////////////////////////////////
	if (!comp_mode)
	{
		sprintf(buf,"%s all result \n",_CS_(OIX_CSTR_AlarmNoAck));
		fprintf(fp_file,buf);
	}else
	{
		sprintf(buf,"%s new result \n",_CS_(OIX_CSTR_AlarmNoAck));
		fprintf(fp_file,buf);
	}
	sprintf(buf,"%s \t %s \t %s \t %s \t \n",_CS_(OIX_CSTR_SEQ_NO),_CS_(OIX_STR_IO_ADDRESS),_CS_(OIX_STR_REC_LOGNAME),_CS_(OIX_CSTR_LONGNAME),i);
	fprintf(fp_file,buf);
	
	////////////////////////////////XML//////////////////////////////////////////
	sprintf(xlsbuf,"<Worksheet ss:Name='报警未确认'>\n");
	// #ifdef	WINDOWS_OS
	// 	wdxlsbuf=ANSIToUnicode(xlsbuf);
	// 	utf8buf=UnicodeToUTF8(wdxlsbuf);
	// 	fprintf(fp_xls,utf8buf);
	// #endif
	write2utf8(fp_xls,xlsbuf);
	
	fprintf(fp_xls,AUTOCHECKXMLCOL);
	
	// #ifdef	WINDOWS_OS
	// 	wdxlsbuf=ANSIToUnicode(AUTOCHECKHEADXML);
	// 	utf8buf=UnicodeToUTF8(wdxlsbuf);
	// 	fprintf(fp_xls,utf8buf);
	// #endif
	write2utf8(fp_xls,AUTOCHECKHEADXML);
	//////////////////////////////////////////////////////////////////////////
	
	if (!comp_mode)
	{
		num=g_slist_length(all_result->Alm_ack) ;
		fwrite(&num,sizeof(int),1,fp_dat);
	}
	for (i=g_slist_length(all_result->Alm_ack)-1,j=0;i>=0;i--,j++)
	{
		
		point=g_slist_nth(all_result->Alm_ack, i)->data;
		
		if ( ReadEntryById (point, &db_cmm ) == -1)
			continue;
		strcpy(entry_name,db_cmm.fixed.entryName);
		GetGroupLongnameById( db_cmm.point.stn_id, db_cmm.point.dev_id, LongNameStr );
		GetEntryLongnameById (&db_cmm.point,LongNamerStr1 );
		strcat( LongNameStr, LongNamerStr1 );
		
		sprintf(pointid,"%d.%d.%d.%d",point->stn_id,point->dev_id,point->data_type,point->pt_id);
		sprintf(buf,"%d\t%s\t%s\t%s \n",j,pointid,entry_name,LongNameStr);
		
		fprintf(fp_file, "%s", buf);
		
		writeOneXmlRow(fp_xls,AllResult,j,pointid,entry_name,LongNameStr);
		
		if (!comp_mode)
		{
			fwrite(point,sizeof(POINTER),1,fp_dat);
		}	//fflush(fp_file);
	}
	if (!comp_mode)
	{
		sprintf(buf,"%s all total\t%d \n\n",_CS_(OIX_CSTR_AlarmNoAck),j);
		fprintf(fp_file,buf);
	}else
	{
		sprintf(buf,"%s new total\t%d \n\n",_CS_(OIX_CSTR_AlarmNoAck),j);
		fprintf(fp_file,buf);
	}
	
	fprintf(fp_xls,"</Table>\n");
	fprintf(fp_xls,"</Worksheet>\n");
	
	if (comp_mode==1)
	{
		sprintf(buf,"%s del result \n",_CS_(OIX_CSTR_AlarmNoAck));
		fprintf(fp_file,buf);
		
		for (i=g_slist_length(del_ptresult->Alm_ack)-1,j=0;i>=0;i--)
		{
			
			point=g_slist_nth(del_ptresult->Alm_ack, i)->data;
			
			if ( ReadEntryById (point, &db_cmm ) == -1)
				continue;
			j++;
			strcpy(entry_name,db_cmm.fixed.entryName);
			GetGroupLongnameById( db_cmm.point.stn_id, db_cmm.point.dev_id, LongNameStr );
			GetEntryLongnameById (&db_cmm.point,LongNamerStr1 );
			strcat( LongNameStr, LongNamerStr1 );
			
			sprintf(pointid,"%d.%d.%d.%d",point->stn_id,point->dev_id,point->data_type,point->pt_id);
			sprintf(buf,"%d\t%s\t%s\t%s \n",j,pointid,entry_name,LongNameStr);
			
			fprintf(fp_file, "%s", buf);
			
			
		}
		
		
		sprintf(buf,"%s del\t%d \n\n",_CS_(OIX_CSTR_AlarmNoAck),j);
		fprintf(fp_file,buf);
	}
	
	
	//////////////////////挂牌///////////////////////////////////////////////////
	
	//
	if (!comp_mode)
	{
		sprintf(buf,"%s all result \n",_CS_(OIX_CSTR_TAG_OPR));
		fprintf(fp_file,buf);
	}else
	{
		sprintf(buf,"%s new result \n",_CS_(OIX_CSTR_TAG_OPR));
		fprintf(fp_file,buf);
	}
	sprintf(buf,"%s \t %s \t %s \t %s \t \n",_CS_(OIX_CSTR_SEQ_NO),_CS_(OIX_STR_IO_ADDRESS),_CS_(OIX_STR_REC_LOGNAME),_CS_(OIX_CSTR_LONGNAME),i);
	fprintf(fp_file,buf);
	
	////////////////////////////////XML//////////////////////////////////////////
	sprintf(xlsbuf,"<Worksheet ss:Name='挂牌'>\n");
	// #ifdef	WINDOWS_OS
	// 	wdxlsbuf=ANSIToUnicode(xlsbuf);
	// 	utf8buf=UnicodeToUTF8(wdxlsbuf);
	// 	fprintf(fp_xls,utf8buf);
	// #endif
	write2utf8(fp_xls,xlsbuf);
	
	fprintf(fp_xls,AUTOCHECKXMLCOL);
	
	// #ifdef	WINDOWS_OS
	// 	wdxlsbuf=ANSIToUnicode(AUTOCHECKHEADXML);
	// 	utf8buf=UnicodeToUTF8(wdxlsbuf);
	// 	fprintf(fp_xls,utf8buf);
	// #endif
	write2utf8(fp_xls,AUTOCHECKHEADXML);
	
	//////////////////////////////////////////////////////////////////////////
	
	
	if (!comp_mode)
	{
		num=g_slist_length(all_result->Tag_state) ;
		fwrite(&num,sizeof(int),1,fp_dat);
	}
	for (i=g_slist_length(all_result->Tag_state)-1,j=0;i>=0;i--,j++)
	{
		
		point=g_slist_nth(all_result->Tag_state, i)->data;
		
		if ( ReadEntryById (point, &db_cmm ) == -1)
			continue;
		strcpy(entry_name,db_cmm.fixed.entryName);
		GetGroupLongnameById( db_cmm.point.stn_id, db_cmm.point.dev_id, LongNameStr );
		GetEntryLongnameById (&db_cmm.point,LongNamerStr1 );
		strcat( LongNameStr, LongNamerStr1 );
		
		sprintf(pointid,"%d.%d.%d.%d",point->stn_id,point->dev_id,point->data_type,point->pt_id);
		sprintf(buf,"%d\t%s\t%s\t%s \n",j,pointid,entry_name,LongNameStr);
		
		fprintf(fp_file, "%s", buf);
		
		writeOneXmlRow(fp_xls,AllResult,j,pointid,entry_name,LongNameStr);
		
		if (!comp_mode)
		{
			fwrite(point,sizeof(POINTER),1,fp_dat);
		}	//fflush(fp_file);
	}
	if (!comp_mode)
	{
		sprintf(buf,"%s all total\t%d \n\n",_CS_(OIX_CSTR_TAG_OPR),j);
		fprintf(fp_file,buf);
	}else
	{
		sprintf(buf,"%s new total\t%d \n\n",_CS_(OIX_CSTR_TAG_OPR),j);
		fprintf(fp_file,buf);
	}
	
	fprintf(fp_xls,"</Table>\n");
	fprintf(fp_xls,"</Worksheet>\n");
	
	if (comp_mode==1)
	{
		sprintf(buf,"%s del result \n",_CS_(OIX_CSTR_TAG_OPR));
		fprintf(fp_file,buf);
		
		for (i=g_slist_length(del_ptresult->Tag_state)-1,j=0;i>=0;i--)
		{
			
			point=g_slist_nth(del_ptresult->Tag_state, i)->data;
			
			if ( ReadEntryById (point, &db_cmm ) == -1)
				continue;
			j++;
			strcpy(entry_name,db_cmm.fixed.entryName);
			GetGroupLongnameById( db_cmm.point.stn_id, db_cmm.point.dev_id, LongNameStr );
			GetEntryLongnameById (&db_cmm.point,LongNamerStr1 );
			strcat( LongNameStr, LongNamerStr1 );
			
			sprintf(pointid,"%d.%d.%d.%d",point->stn_id,point->dev_id,point->data_type,point->pt_id);
			sprintf(buf,"%d\t%s\t%s\t%s \n",j,pointid,entry_name,LongNameStr);
			
			fprintf(fp_file, "%s", buf);
			
			
		}
		
		
		sprintf(buf,"%s del\t%d \n\n",_CS_(OIX_CSTR_TAG_OPR),j);
		fprintf(fp_file,buf);
	}
	
	
	
	//////////////////////人工置数///////////////////////////////////////////////////
	//
	if (!comp_mode)
	{
		sprintf(buf,"%s all result \n",_CS_(OIX_CSTR_MAN_SET_OR_BLOCK));
		fprintf(fp_file,buf);
	}else
	{
		sprintf(buf,"%s new result \n",_CS_(OIX_CSTR_MAN_SET_OR_BLOCK));
		fprintf(fp_file,buf);
	}
	sprintf(buf,"%s \t %s \t %s \t %s \t \n",_CS_(OIX_CSTR_SEQ_NO),_CS_(OIX_STR_IO_ADDRESS),_CS_(OIX_STR_REC_LOGNAME),_CS_(OIX_CSTR_LONGNAME),i);
	fprintf(fp_file,buf);
	// 	////////////////////////////////XML//////////////////////////////////////////
	sprintf(xlsbuf,"<Worksheet ss:Name='人工置数'>\n");
	// #ifdef	WINDOWS_OS
	// 	wdxlsbuf=ANSIToUnicode(xlsbuf);
	// 	utf8buf=UnicodeToUTF8(wdxlsbuf);
	// 	fprintf(fp_xls,utf8buf);
	// #endif
	write2utf8(fp_xls,xlsbuf);
	
	fprintf(fp_xls,AUTOCHECKXMLCOL);
	
	// #ifdef	WINDOWS_OS
	// 	wdxlsbuf=ANSIToUnicode(AUTOCHECKHEADXML);
	// 	utf8buf=UnicodeToUTF8(wdxlsbuf);
	// 	fprintf(fp_xls,utf8buf);
	// #endif
	write2utf8(fp_xls,AUTOCHECKHEADXML);
	
	// 	//////////////////////////////////////////////////////////////////////////
	
	if (!comp_mode)
	{
		num=g_slist_length(all_result->Man_set) ;
		fwrite(&num,sizeof(int),1,fp_dat);
	}
	for (i=g_slist_length(all_result->Man_set)-1,j=0;i>=0;i--,j++)
	{
		
		point=g_slist_nth(all_result->Man_set, i)->data;
		
		if ( ReadEntryById (point, &db_cmm ) == -1)
			continue;
		strcpy(entry_name,db_cmm.fixed.entryName);
		GetGroupLongnameById( db_cmm.point.stn_id, db_cmm.point.dev_id, LongNameStr );
		GetEntryLongnameById (&db_cmm.point,LongNamerStr1 );
		strcat( LongNameStr, LongNamerStr1 );
		
		sprintf(pointid,"%d.%d.%d.%d",point->stn_id,point->dev_id,point->data_type,point->pt_id);
		sprintf(buf,"%d\t%s\t%s\t%s \n",j,pointid,entry_name,LongNameStr);
		
		fprintf(fp_file, "%s", buf);
		
		writeOneXmlRow(fp_xls,AllResult,j,pointid,entry_name,LongNameStr);
		if (!comp_mode)
		{
			fwrite(point,sizeof(POINTER),1,fp_dat);
		}	//fflush(fp_file);
	}
	if (!comp_mode)
	{
		sprintf(buf,"%s all total\t%d \n\n",_CS_(OIX_CSTR_MAN_SET_OR_BLOCK),j);
		fprintf(fp_file,buf);
	}else
	{
		sprintf(buf,"%s new total\t%d \n\n",_CS_(OIX_CSTR_MAN_SET_OR_BLOCK),j);
		fprintf(fp_file,buf);
	}
	fprintf(fp_xls,"</Table>\n");
	fprintf(fp_xls,"</Worksheet>\n");
	if (comp_mode==1)
	{
		sprintf(buf,"%s del result \n",_CS_(OIX_CSTR_MAN_SET_OR_BLOCK));
		fprintf(fp_file,buf);
		
		for (i=g_slist_length(del_ptresult->Man_set)-1,j=0;i>=0;i--)
		{
			
			point=g_slist_nth(del_ptresult->Man_set, i)->data;
			
			if ( ReadEntryById (point, &db_cmm ) == -1)
				continue;
			j++;
			strcpy(entry_name,db_cmm.fixed.entryName);
			GetGroupLongnameById( db_cmm.point.stn_id, db_cmm.point.dev_id, LongNameStr );
			GetEntryLongnameById (&db_cmm.point,LongNamerStr1 );
			strcat( LongNameStr, LongNamerStr1 );
			
			sprintf(pointid,"%d.%d.%d.%d",point->stn_id,point->dev_id,point->data_type,point->pt_id);
			sprintf(buf,"%d\t%s\t%s\t%s \n",j,pointid,entry_name,LongNameStr);
			
			fprintf(fp_file, "%s", buf);
			
			
		}
		
		
		sprintf(buf,"%s del\t%d \n\n",_CS_(OIX_CSTR_MAN_SET_OR_BLOCK),j);
		fprintf(fp_file,buf);
	}
	//
	
	fflush(fp_file);
	fclose(fp_file);
	if (!comp_mode)
	{	
		fflush(fp_dat);
		fclose(fp_dat);
	}
	
	fprintf(fp_xls, "</Workbook>");
	fflush(fp_xls);
	fclose(fp_xls);
	return 1;
}

//int saveOneResult(FILE	*fp_file,FILE	*fp_dat,FILE	*fp_xls,CHECK_RESULT*ptresult,int type)
int saveOneResult(FILE	*fp_file,FILE	*fp_dat,FILE	*fp_xls,GSList*pt_list,int type)
{
	int i,j;
	POINTER* point;
	char entry_name[64];
	char LongNameStr[100];
	char LongNamerStr1[128];
	DMS_COMMON	db_cmm;
	char pointid[20];
//	char	buf[10240];
	int num;
	char flag[5];
	char	PrintLocationRuler[160];
	int ruler=0;
	char record[10];
	
	switch (type)
	{
	case NewResult:
		sprintf(flag,"%s","NEW");
		break;
	case DelResult:
		sprintf(flag,"%s","DEL");
		break;
	case OldResult:
		sprintf(flag,"%s","OLD");
		break;
	}
	if (type==AllResult)
	{
		num=g_slist_length(pt_list) ;
		fwrite(&num,sizeof(int),1,fp_dat);
	}
	for (i=g_slist_length(pt_list)-1,j=0;i>=0;i--,j++)
	{
		ruler=0;
		point=g_slist_nth(pt_list, i)->data;
		
		if ( ReadEntryById (point, &db_cmm ) == -1)
			continue;
		
		if (type==AllResult)
		{
			fwrite(point,sizeof(POINTER),1,fp_dat);
		}else
		{
			
			//	strcpy(entry_name,db_cmm.fixed.entryName);
			GetPtNameById(point,entry_name,NULL);	
			GetStnLongnameById(db_cmm.point.stn_id, LongNameStr );
			GetGroupLongnameById( db_cmm.point.stn_id, db_cmm.point.dev_id, LongNamerStr1 );
			strcat( LongNameStr, LongNamerStr1 );
			GetEntryLongnameById (&db_cmm.point,LongNamerStr1 );
			strcat( LongNameStr, LongNamerStr1 );
			sprintf(record , "%d",  j);
			sprintf(pointid,"%d.%d.%d.%d",point->stn_id,point->dev_id,point->data_type,point->pt_id);
			//sprintf(buf,"%d\t\t%c\t\t%s\t\t%s\t\t%s \n",j,flag,pointid,entry_name,LongNameStr);		
			
			while(ruler<150)
			{
				PrintLocationRuler[ruler]=' ';
				ruler++;
			}
			PrintLocationRuler[ruler]='\0';	
			
			strncpy(PrintLocationRuler+0, record, strlen(record));			
			strncpy(PrintLocationRuler+10, flag,strlen(flag));				
			strncpy(PrintLocationRuler+18, pointid, strlen(pointid));
			strncpy(PrintLocationRuler+45, entry_name, strlen(entry_name));
			strncpy(PrintLocationRuler+90, LongNameStr, strlen(LongNameStr));
			
			
			fprintf(fp_file, "%s \n", PrintLocationRuler);
			writeOneXmlRow(fp_xls,type,j,pointid,entry_name,LongNameStr);
		}
		
	}
	
	return TRUE;
	
}
void showreslut(char* pathfilename)
{
	char cmdbuf[200];
	
#ifdef	WINDOWS_OS
	sprintf(cmdbuf,"notepad %s &",pathfilename);
#else
	sprintf(cmdbuf,"/usr/dt/bin/dtpad %s &",pathfilename);
#endif
	system(cmdbuf);


#ifdef SUN_OS
    pthread_exit(0);
#endif
#ifdef WINDOWS_OS
	ExitThread(0) ; 
	
#endif
}
void PopupAutocheckFile(int crt_n,char* pathfilename)
{
#ifdef  SUN_OS
	pthread_t tid1,tid2;
	int error;
#endif
#ifdef	WINDOWS_OS
	HANDLE  Hthread ; 
#endif 

#ifdef  SUN_OS
	error = pthread_create(&tid1, NULL, showreslut, (char*)(pathfilename ));
	if(error!=-1)
	{
		printf("Create showreslut Success!\n");
	
	}
	else
	{
		printf("Create showreslut failed!!! error=%d\n",error);
		
	}
#endif 
	
#ifdef  WINDOWS_OS
	Hthread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)showreslut, (char *)(pathfilename) , 0, NULL) ; 
	if(Hthread!=NULL)
	{
		printf("Create showreslut Success!\n");
	
	}
	else
	{
		printf("Create showreslut failed!!! error=%d\n",Hthread);
	}
#endif



//	PopAutochkDlg(main_w[crt_n],crt_n);
}
void getCheckName(int comp_mode,char* filename)
{
	struct tm *check_time;
	time_t t;
	long	tloc;
	char str_time[30];
	
	char hostname[20]; 
	
	gethostname(hostname,sizeof(hostname));
	t	 = time (&tloc);
	check_time = localtime (&t);
	
	//sprintf(str_time_eg, "%04d%02d%02d",check_time->tm_year + 1900,check_time->tm_mon+1,check_time->tm_mday);
	sprintf(str_time,"%02d%02d%02d_%02d%02d",check_time->tm_year + 1900,check_time->tm_mon+1,check_time->tm_mday,
		check_time->tm_hour,check_time->tm_min);
	if (comp_mode)
	{
		sprintf(filename,"%s_%s_chk",str_time,hostname);
	}else
		sprintf(filename,"CON_%s_%s_chk",str_time,hostname);
	
	
}
int saveChkRst2file1(int crt_n,char* resultfile,char* resultdata,int comp_mode,CHECK_RESULT*ptcheckresult,CHECK_RESULT*new_ptresult,CHECK_RESULT* del_ptresult,CHECK_RESULT* old_ptresult)
{
	
	FILE	*fp_file,*fp_xls,*fp_dat;
	char	buf[10240],xlsbuf[10240], *ptr= buf;
	
	char filename[200],filexml[200],datfilename[200];
	
	char tmpfilename[200];

#ifdef SUN_OS
	char cmdBuf[256];
#endif
	
	getCheckName(comp_mode,tmpfilename);
	
	sprintf(filename, "%s%s.txt", AUTOCHECK_DIR,tmpfilename);
	//sprintf(filename, "%s%s", AUTOCHECK_DIR,resultfile);
	
	fp_file= fopen(filename, "w");	
	if (!fp_file)
	{	
		printf("Open %s file error \n",filename);
		return -1;
	}
	
	sprintf(datfilename, "%s%s", AUTOCHECK_DIR,resultdata);
	if (comp_mode)
	{
		
		
		fp_dat= fopen(datfilename, "wb");
		if (!fp_dat)
		{	
			printf("Open %s file error \n",datfilename);
			return -1;
		}
	}
#ifdef	WINDOWS_OS
	sprintf(filexml, "%s%s.xls", AUTOCHECK_DIR,tmpfilename);
#else
	sprintf(filexml, "%s%s", AUTOCHECK_DIR,".tmpxml");
#endif
	
	fp_xls= fopen(filexml, "w");
	if (!fp_xls)
	{	
		printf("Open %s file error \n",filexml);
		return -1;
	}
	fprintf(fp_xls,AUTOCHECKXMLHEADSTR);
	///////////////////////扫查禁止//////////////////////////////////////////////////
	if((( 0x01 & (FilterCheck[crt_n].attrib_mask[0]))&&comp_mode==0)||comp_mode==1)		
	{
		
		
		sprintf(buf,"%s   %s%d  %s%d  %s%d  \n",_CS_(OIX_CSTR_ScanDisable),"NEW=",g_slist_length(new_ptresult->Scan_state),
			"DEL=",g_slist_length(del_ptresult->Scan_state),"OLD=",g_slist_length(old_ptresult->Scan_state));
		fprintf(fp_file,buf);
		sprintf(buf,"%s    %s      %s \t\t\t %s \t\t\t\t\t %s \t\t \n",_CS_(OIX_CSTR_SEQ_NO),_CS_(OIX_STR_TYPE),_CS_(OIX_STR_IO_ADDRESS),_CS_(OIX_STR_REC_LOGNAME),_CS_(OIX_CSTR_LONGNAME));
		
		fprintf(fp_file,buf);
		
		
		//////////////////////////////////////////////////////////////////////////
		sprintf(xlsbuf,"<Worksheet ss:Name='扫查禁止'>\n");
		write2utf8(fp_xls,xlsbuf);
		
		fprintf(fp_xls,AUTOCHECKXMLCOL);
		
		write2utf8(fp_xls,AUTOCHECKHEADXML);	
		
		printfslist("before saveOneResult newpt_result.Scan_state",newpt_result.Scan_state);	
		saveOneResult(fp_file,NULL,fp_xls,new_ptresult->Scan_state,NewResult);
		printfslist("end saveOneResult newpt_result.Scan_state",newpt_result.Scan_state);	
		
		saveOneResult(fp_file,NULL,fp_xls,del_ptresult->Scan_state,DelResult);
		saveOneResult(fp_file,NULL,fp_xls,old_ptresult->Scan_state,OldResult);
		if (comp_mode)
		{
			saveOneResult(fp_file,fp_dat,NULL,ptcheckresult->Scan_state,AllResult);
		}	
		
		
		fprintf(fp_xls,"</Table>\n");
		fprintf(fp_xls,"</Worksheet>\n");
	}
	///////////////////////报警禁止//////////////////////////////////////////////////
	if((( 0x01 & (FilterCheck[crt_n].attrib_mask[0]>>1))&&comp_mode==0)||comp_mode==1)		
	{
		
		sprintf(buf,"%s   %s%d  %s%d  %s%d  \n",_CS_(OIX_CSTR_AlarmDisable),"NEW=",g_slist_length(new_ptresult->Alm_state),
			"DEL=",g_slist_length(del_ptresult->Alm_state),"OLD=",g_slist_length(old_ptresult->Alm_state));
		fprintf(fp_file,buf);
		sprintf(buf,"%s    %s      %s \t\t\t %s \t\t\t\t\t %s \t\t \n",_CS_(OIX_CSTR_SEQ_NO),_CS_(OIX_STR_TYPE),_CS_(OIX_STR_IO_ADDRESS),_CS_(OIX_STR_REC_LOGNAME),_CS_(OIX_CSTR_LONGNAME));
		fprintf(fp_file,buf);
		//////////////////////////////////////////////////////////////////////////
		sprintf(xlsbuf,"<Worksheet ss:Name='报警禁止'>\n");
		write2utf8(fp_xls,xlsbuf);
		
		fprintf(fp_xls,AUTOCHECKXMLCOL);
		
		write2utf8(fp_xls,AUTOCHECKHEADXML);	
		
		saveOneResult(fp_file,NULL,fp_xls,new_ptresult->Alm_state,NewResult);
		saveOneResult(fp_file,NULL,fp_xls,del_ptresult->Alm_state,DelResult);
		saveOneResult(fp_file,NULL,fp_xls,old_ptresult->Alm_state,OldResult);
		if (comp_mode)
		{
			saveOneResult(fp_file,fp_dat,NULL,ptcheckresult->Alm_state,AllResult);
		}
		fprintf(fp_xls,"</Table>\n");
		fprintf(fp_xls,"</Worksheet>\n");
	}
	///////////////////////报警未确认//////////////////////////////////////////////////
	if((( 0x01 & (FilterCheck[crt_n].attrib_mask[0]>>2))&&comp_mode==0)||comp_mode==1)		
	{
		
		sprintf(buf,"%s   %s%d  %s%d  %s%d  \n",_CS_(OIX_CSTR_AlarmNoAck),"NEW=",g_slist_length(new_ptresult->Alm_ack),
			"DEL=",g_slist_length(del_ptresult->Alm_ack),"OLD=",g_slist_length(old_ptresult->Alm_ack));
		
		fprintf(fp_file,buf);
		sprintf(buf,"%s    %s      %s \t\t\t %s \t\t\t\t\t %s \t\t \n",_CS_(OIX_CSTR_SEQ_NO),_CS_(OIX_STR_TYPE),_CS_(OIX_STR_IO_ADDRESS),_CS_(OIX_STR_REC_LOGNAME),_CS_(OIX_CSTR_LONGNAME));	
		fprintf(fp_file,buf);
		//////////////////////////////////////////////////////////////////////////
		sprintf(xlsbuf,"<Worksheet ss:Name='报警未确认'>\n");
		write2utf8(fp_xls,xlsbuf);
		
		fprintf(fp_xls,AUTOCHECKXMLCOL);
		
		write2utf8(fp_xls,AUTOCHECKHEADXML);	
		
		saveOneResult(fp_file,NULL,fp_xls,new_ptresult->Alm_ack,NewResult);
		saveOneResult(fp_file,NULL,fp_xls,del_ptresult->Alm_ack,DelResult);
		saveOneResult(fp_file,NULL,fp_xls,old_ptresult->Alm_ack,OldResult);
		if (comp_mode)
		{
			saveOneResult(fp_file,fp_dat,NULL,ptcheckresult->Alm_ack,AllResult);
		}
		fprintf(fp_xls,"</Table>\n");
		fprintf(fp_xls,"</Worksheet>\n");
	}
	///////////////////////挂牌//////////////////////////////////////////////////
	if((( 0x01 & (FilterCheck[crt_n].attrib_mask[0]>>3))&&comp_mode==0)||comp_mode==1)		
	{
		
		sprintf(buf,"%s   %s%d  %s%d  %s%d  \n",_CS_(OIX_CSTR_TAG_OPR),"NEW=",g_slist_length(new_ptresult->Tag_state),
			"DEL=",g_slist_length(del_ptresult->Tag_state),"OLD=",g_slist_length(old_ptresult->Tag_state));
		
		fprintf(fp_file,buf);
		sprintf(buf,"%s    %s      %s \t\t\t %s \t\t\t\t\t %s \t\t \n",_CS_(OIX_CSTR_SEQ_NO),_CS_(OIX_STR_TYPE),_CS_(OIX_STR_IO_ADDRESS),_CS_(OIX_STR_REC_LOGNAME),_CS_(OIX_CSTR_LONGNAME));
		fprintf(fp_file,buf);
		//////////////////////////////////////////////////////////////////////////
		sprintf(xlsbuf,"<Worksheet ss:Name='挂牌'>\n");
		write2utf8(fp_xls,xlsbuf);
		
		fprintf(fp_xls,AUTOCHECKXMLCOL);
		
		write2utf8(fp_xls,AUTOCHECKHEADXML);	
		
		saveOneResult(fp_file,NULL,fp_xls,new_ptresult->Tag_state,NewResult);
		saveOneResult(fp_file,NULL,fp_xls,del_ptresult->Tag_state,DelResult);
		saveOneResult(fp_file,NULL,fp_xls,old_ptresult->Tag_state,OldResult);
		if (comp_mode)
		{
			saveOneResult(fp_file,fp_dat,NULL,ptcheckresult->Tag_state,AllResult);
		}
		fprintf(fp_xls,"</Table>\n");
		fprintf(fp_xls,"</Worksheet>\n");
	}
	///////////////////////人工置数//////////////////////////////////////////////////
	if((( 0x01 & (FilterCheck[crt_n].attrib_mask[0]>>5))&&comp_mode==0)||comp_mode==1)		
	{
		
		sprintf(buf,"%s   %s%d  %s%d  %s%d  \n",_CS_(OIX_CSTR_MAN_SET_OR_BLOCK),"NEW=",g_slist_length(new_ptresult->Man_set),
			"DEL=",g_slist_length(del_ptresult->Man_set),"OLD=",g_slist_length(old_ptresult->Man_set));
		
		fprintf(fp_file,buf);
		sprintf(buf,"%s    %s      %s \t\t\t %s \t\t\t\t\t %s \t\t \n",_CS_(OIX_CSTR_SEQ_NO),_CS_(OIX_STR_TYPE),_CS_(OIX_STR_IO_ADDRESS),_CS_(OIX_STR_REC_LOGNAME),_CS_(OIX_CSTR_LONGNAME));
		fprintf(fp_file,buf);
		//////////////////////////////////////////////////////////////////////////
		sprintf(xlsbuf,"<Worksheet ss:Name='人工置数'>\n");
		write2utf8(fp_xls,xlsbuf);
		
		fprintf(fp_xls,AUTOCHECKXMLCOL);
		
		write2utf8(fp_xls,AUTOCHECKHEADXML);	
		
		saveOneResult(fp_file,NULL,fp_xls,new_ptresult->Man_set,NewResult);
		saveOneResult(fp_file,NULL,fp_xls,del_ptresult->Man_set,DelResult);
		saveOneResult(fp_file,NULL,fp_xls,old_ptresult->Man_set,OldResult);
		if (comp_mode)
		{
			saveOneResult(fp_file,fp_dat,NULL,ptcheckresult->Man_set,AllResult);
		}
		///////////////////////重要事故///////////////////////////////////////////////////
		// 	sprintf(buf,"%s   \n",_CS_(OIX_CSTR_MAN_SET_OR_BLOCK));
		// 	fprintf(fp_file,buf);
		// 	sprintf(buf,"%s \t\t %s \t\t %s \t\t %s \t\t %s \t\t \n",_CS_(OIX_CSTR_SEQ_NO),_CS_(OIX_STR_TYPE),_CS_(OIX_STR_IO_ADDRESS),_CS_(OIX_STR_REC_LOGNAME),_CS_(OIX_CSTR_LONGNAME));
		// 	
		// 	fprintf(fp_file,buf);
		// 	//////////////////////////////////////////////////////////////////////////
		
		
		///////////////////////控制调节///////////////////////////////////////////////////
		// 	
		fprintf(fp_xls,"</Table>\n");
		fprintf(fp_xls,"</Worksheet>\n");
	}
	
	
	fflush(fp_file);
	fclose(fp_file);
	if (comp_mode)
	{	
		fflush(fp_dat);
		fclose(fp_dat);
	}
	
	fprintf(fp_xls, "</Workbook>");
	fflush(fp_xls);
	fclose(fp_xls);
#ifdef SUN_OS
	//convert to utf-8
	sprintf(cmdBuf,"iconv -f 'GBK' -t 'UTF-8' %s >%s%s.xls",filexml,AUTOCHECK_DIR,tmpfilename);
	printf("cmdBuf===%s\n",cmdBuf);
	system(cmdBuf);
#endif
	
	PopupAutocheckFile(crt_n,filename);
	return 1;
}

int checkEvent()
{
	int i,idx;
	POINTER pt;
	DMS_COMMON db_cmm;
	int Impt_index=0;
	int Ctrl_index=0;
	
	int start,count;
	count=shm_rec_addr->head.count;
	
	if(shm_rec_addr->head.header_ptr-count>0)
		start=shm_rec_addr->head.header_ptr-count;
	else
		start=shm_rec_addr->head.header_ptr-count+shm_rec_addr->head.buf_size;
	
	Impt_alm.num=0;
	Dev_ctrl.num=0;
	for (i=0,idx=0;i<count;i++)
	{
		idx=(start+i)%shm_rec_addr->head.buf_size;
		
		pt.stn_id=shm_rec_addr->rec[idx].ind_rec.stn_id;
		pt.dev_id=shm_rec_addr->rec[idx].ind_rec.dev_id;  
		pt.data_type=shm_rec_addr->rec[idx].ind_rec.data_type;
		pt.pt_id=shm_rec_addr->rec[idx].ind_rec.pt_id;
		if ( ReadEntryById (&pt, &db_cmm ) == -1)		
		{
			//start=(start+1)%shm_rec_addr->head.buf_size;
			continue;
		}//
		if (db_cmm.fixed.ioattr.keySignal)
		{
			Impt_alm.Evt_idx[Impt_index]=idx;
			Impt_index++;			
		}
		//IOST_SET
		//if( (shm_rec_addr->rec->cmd_rec.type_id>=C_LCU_ONOFF)&&(shm_rec_addr->rec->cmd_rec.type_id<=C_SYS_MODE_SET))
		if( (shm_rec_addr->rec->cmd_rec.type_id>=DPS_IOST_SET)&&(shm_rec_addr->rec->cmd_rec.type_id<=C_SYS_MODE_SET))
		{
			Dev_ctrl.Evt_idx[Ctrl_index]=idx;
			Ctrl_index++;
		}
		
		
		
	}
	Impt_alm.num=Impt_index;
	Dev_ctrl.num=Ctrl_index;
	
	
	return TRUE;
}
int ProgAutoCheck(GtkWidget*parent,int crt_n)
{
	
	int m,/*row,col,*/pt_num=0,grp_num,entry_num;
	char	stn_comment[STATION_COMMENT_SIZE];
	POINTER point;
	DMS_COMMON db_cmm;
	BOOL result=FALSE;
	int stn_num;
	int stn_idx,dev_idx,type_idx;
	UCHAR	stn_id;
	
	GetStationNum(&stn_num);
	stn_id= 0;
	
	freeCheckresult();
	
	openChkRst2DAT();
	
	for(stn_idx=1; stn_idx<=stn_num; stn_idx++) 
	{  		
		if(-1==GetStnIdByIndex(stn_idx, &stn_id))
			continue;
		
		if( GetStnCommentById((UCHAR)stn_id,(char*)( stn_comment))==-1 )
			continue;
		
		
		if(-1==GetGroupNumById((UCHAR)stn_id, &grp_num)) //所有lcu
			return -1;
		
		for(dev_idx=1;dev_idx<=grp_num;dev_idx++)
		{
			for(type_idx=1;type_idx<MAX_DATATYPE_NUM;type_idx++)
			{
				if(type_idx==2)
					continue;
				if(-1==GetEntryNumById((UCHAR)stn_id,(UCHAR)dev_idx, (UCHAR)type_idx, &entry_num))
					continue;
				//		printf("dev_id=%d,data_type=%d,entry_num=%d\n",dev_idx,type_idx,entry_num);
				point.stn_id = stn_id;
				point.dev_id = dev_idx;
				point.data_type=type_idx;
				for(m=0;m<entry_num;m++)
				{
					point.pt_id	= m;
					if ( ReadEntryById (&point, &db_cmm ) == -1)		
						continue;
					
					checkPropertyOption(db_cmm);
					//	GetPropertyOption(db_cmm,&result); 
					
					
				}
			}
			
		}
		
	}
	
	//checkEvent();
	
	printf("end checkPropertyOption g_slist_length(delpt_result.Scan_state=%d\n",g_slist_length(delpt_result.Scan_state));
	
	printf("g_slist_length(checkresult.Scan_state=%d\n",g_slist_length(checkresult.Scan_state));
	printf("g_slist_length(checkresult.Alm_state=%d\n",g_slist_length(checkresult.Alm_state));
	printf("g_slist_length(checkresult.Alm_ack=%d\n",g_slist_length(checkresult.Alm_ack));
	
	printf("g_slist_length(checkresult.Man_set=%d\n",g_slist_length(checkresult.Man_set));
	
	
	//	saveChkRst2file();
	
	if (COMPARE)
	{
		//	saveChkRst2file(COMPRESULT_FILE,AUTOCHECK_DATA,1,&newpt_result,&delpt_result);
		saveChkRst2file1(crt_n,COMPRESULT_FILE,AUTOCHECK_DATA,1,&checkresult,&newpt_result,&delpt_result,&oldpt_result);
	}
	//saveChkRst2file(AUTOCHECK_FILE,AUTOCHECK_DATA,0,&checkresult,NULL);
	
	return 1;
	
}

gint ProgConditionCheck(GtkWidget *widget , gpointer data)
{
	
	
	int m,/*row,col,*/pt_num=0,grp_num,entry_num;
	char	stn_comment[STATION_COMMENT_SIZE];
	POINTER point;
	DMS_COMMON db_cmm;
	BOOL result=FALSE;
	int stn_num;
	UCHAR	stn_id;
	int stn_idx,dev_idx,type_idx;
	gint crt_n = (gint) data ;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE;
	
	GetStationNum(&stn_num);
	stn_id= 0;
	
	freeCheckresult();
	
	for(stn_idx=1; stn_idx<=stn_num; stn_idx++) 
	{  		
		if(-1==GetStnIdByIndex(stn_idx, &stn_id))
			continue;
		
		if( GetStnCommentById((UCHAR)stn_id,(char*)( stn_comment))==-1 )
			continue;
		if(!( 0x01& (FilterCheck[crt_n].stn_mask>>(stn_idx-1))))
			continue;		
		
		if(-1==GetGroupNumById((UCHAR)stn_id, &grp_num)) //所有lcu
			return -1;
		
		for(dev_idx=1;dev_idx<=grp_num;dev_idx++)
		{
			if(!( 0x01 & (FilterCheck[crt_n].dev_mask[stn_idx-1]>>(dev_idx-1))))
				continue;		
			
			
			for(type_idx=1;type_idx<MAX_DATATYPE_NUM;type_idx++)
			{
				
				if(!(0x01 & (FilterCheck[crt_n].datatype_mask>>(type_idx-1))))
					continue;
				
				if(type_idx==2)
					continue;
				if(-1==GetEntryNumById((UCHAR)stn_id,(UCHAR)dev_idx, (UCHAR)type_idx, &entry_num))
					continue;
				//		printf("dev_id=%d,data_type=%d,entry_num=%d\n",dev_idx,type_idx,entry_num);
				point.stn_id = stn_id;
				point.dev_id = dev_idx;
				point.data_type=type_idx;
				for(m=0;m<entry_num;m++)
				{
					point.pt_id	= m;
					if ( ReadEntryById (&point, &db_cmm ) == -1)		
						continue;
					
					checkPropertyOption(db_cmm);
					//	GetPropertyOption(db_cmm,&result); 
					
					
				}
			}
			
		}
		
	}
	
	
	printf("end checkPropertyOption g_slist_length(delpt_result.Scan_state=%d\n",g_slist_length(delpt_result.Scan_state));
	
	printf("g_slist_length(checkresult.Scan_state=%d\n",g_slist_length(checkresult.Scan_state));
	printf("g_slist_length(checkresult.Alm_state=%d\n",g_slist_length(checkresult.Alm_state));
	printf("g_slist_length(checkresult.Alm_ack=%d\n",g_slist_length(checkresult.Alm_ack));
	
	printf("g_slist_length(checkresult.Man_set=%d\n",g_slist_length(checkresult.Man_set));
	
	
	printfslist("before save newpt_result.Scan_state",newpt_result.Scan_state);
	saveChkRst2file1(crt_n,COMPRESULT_FILE,AUTOCHECK_DATA,0,&checkresult,&newpt_result,&delpt_result,&oldpt_result);
		printfslist("end save newpt_result.Scan_state",newpt_result.Scan_state);
	
	return 1;
	
	
}

gint UpdataViewFromCheckFilter(FILTER_T *pFilter,int crt_n)
{
	GtkTreeIter iter;
	BOOL fixed;
	int i;
	char stnName[STATION_LONGNAME_SIZE];
	gchar *m_StnName;
	gchar *pConvert=0;

	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE;
	
	if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(storeStn[crt_n]), &iter))
	{	
		i=0;
		do 
		{
			if (pFilter->stn_id == 0)
			{
				if (pFilter->stn_mask & (0X1<<(i++)) ) 
					fixed = TRUE;  
				else
					fixed = FALSE;
			}
			else if (GetStnCommentById(pFilter->stn_id,stnName)!=-1)
			{
				gtk_tree_model_get(GTK_TREE_MODEL(storeStn[crt_n]),&iter,COLUMN_NAME,&m_StnName,-1);
				if (!strcmp(stnName,pConvert=_fromUtf8(m_StnName)))
				{
					pFilter->stn_mask = pFilter->stn_mask | (1<<i++);
					fixed = TRUE;  
				}
				else
				{
					pFilter->stn_mask = pFilter->stn_mask & (~(1<<i++));
					fixed = FALSE;
				}
				G_SAFE_FREE(pConvert);//dzy1017
			}
			gtk_list_store_set (GTK_LIST_STORE(storeStn[crt_n]), &iter, COLUMN_CHECKED, fixed, -1);
		} while (gtk_tree_model_iter_next(GTK_TREE_MODEL(storeStn[crt_n]), &iter));
	}
	if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(storeDev[crt_n]), &iter))
	{
		i=0;
		do 
		{
			if (pFilter->dev_mask[curStn] & (0X1<<(i++)) ) 
				fixed = TRUE;  
			else
				fixed = FALSE;
			gtk_list_store_set (GTK_LIST_STORE(storeDev[crt_n]), &iter, COLUMN_CHECKED, fixed, -1);
		} while (gtk_tree_model_iter_next(GTK_TREE_MODEL(storeDev[crt_n]),&iter));
	}
	if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(storeGroup[crt_n]), &iter))
	{
		i=0;
		do 
		{
			if (pFilter->group_mask[curDev] & (0X1<<(i++)))
				fixed = TRUE;
			else
				fixed = FALSE;
			gtk_list_store_set (GTK_LIST_STORE(storeGroup[crt_n]), &iter, COLUMN_CHECKED, fixed, -1);
		} while (gtk_tree_model_iter_next(GTK_TREE_MODEL(storeGroup[crt_n]),&iter));
	}
	if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(storeType[crt_n]), &iter))
	{
		i=0;
		do 
		{
			if (pFilter->datatype_mask & (0X1<<(i++)) ) 
				fixed = TRUE;  
			else
				fixed = FALSE;
			gtk_list_store_set (GTK_LIST_STORE(storeType[crt_n]), &iter, COLUMN_CHECKED, fixed, -1);
		} while (gtk_tree_model_iter_next(GTK_TREE_MODEL(storeType[crt_n]),&iter));
	}
	if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(storeAttrib[crt_n]), &iter))
	{
		i=0;
		do 
		{
			if (pFilter->attrib_mask[0] & (0X1<<(i++)) ) 
				fixed = TRUE;  
			else
				fixed = FALSE;
			gtk_list_store_set (GTK_LIST_STORE(storeAttrib[crt_n]), &iter, COLUMN_CHECKED, fixed, -1);
		} while (gtk_tree_model_iter_next(GTK_TREE_MODEL(storeAttrib[crt_n]),&iter));
	}
	return TRUE;
}

gint ChgTemplate(GtkComboBox* pComboBox, gpointer data)
{
	GtkTreeModel *pModel;
	GtkTreeIter iter;
	//	GtkWidget  *opmenu;
	int i;
//	UINT test;
	//	char *filterName;
	gchar *stn_id,*fault_mask,*type_id_mask,*dev_mask,*group_mask,*datatype_mask,*attrib_mask;
	UINT *stn_mask;
	int offset = 0;
	gint crt_n = (gint) data ;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE;
	
	pModel=gtk_combo_box_get_model(GTK_COMBO_BOX(pComboBox));
	if (!gtk_combo_box_get_active_iter(GTK_COMBO_BOX(pComboBox), &iter))
	{
		return FALSE;
	}
	gtk_tree_model_get(pModel, &iter, eStnId, &stn_id,
		eStnMask,&stn_mask,
		eFaultMask,&fault_mask, 
		eTypeIdMask,&type_id_mask,
		eDevMask,&dev_mask,
		eGroupMask,&group_mask,
		eDataTypeMask,&datatype_mask,
		eAttribMask,&attrib_mask,-1);
	
	FilterCheck[crt_n].stn_id = (UCHAR)atoi(stn_id);
//	sscanf(stn_mask,"%08X",&FilterCheck[crt_n].stn_mask);
	FilterCheck[crt_n].fault_mask = (UCHAR)atoi(fault_mask);
	FilterCheck[crt_n].type_id_mask = (UCHAR)atoi(type_id_mask);
	sscanf(dev_mask,"%08X",&FilterCheck[crt_n].dev_mask[FilterCheck[crt_n].stn_id-1]);
	for (i=0;i<MAX_GROUP_MASK_NUM;i++)
	{
		sscanf(group_mask+offset,"%08X",&FilterCheck[crt_n].group_mask[i]);
		offset += 8;
	}
	sscanf(datatype_mask,"%08X",&FilterCheck[crt_n].datatype_mask);
	sscanf(attrib_mask,"%08X",&FilterCheck[crt_n].attrib_mask[0]);
	UpdataViewFromCheckFilter(&FilterCheck[crt_n],crt_n);
	//	g_free(filterName);
	g_free(group_mask);
	return TRUE;
}

gboolean StnClicked(GtkTreeView *treeview,
					gpointer     data)
{
	GtkTreePath *path;
	GtkTreeModel *model;
	GtkTreeIter  iter;
	gchar *stnName;
	gchar *pConvert=0, *pConvert1=0;
	int i,j,station_num;
	BOOL fixed;
	gint *path_num;
	gint crt_n = (gint) data ;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE;
	gtk_tree_view_get_cursor(treeview,&path,NULL);
	path_num = gtk_tree_path_get_indices(path);
	curStn = *path_num;
	model = gtk_tree_view_get_model(treeview);
	gtk_tree_model_get_iter(model,&iter,path);
	gtk_tree_model_get(model,&iter,COLUMN_CHECKED,&fixed,-1);
	gtk_tree_model_get (model, &iter, COLUMN_NAME, &stnName, -1);
	gtk_list_store_clear(GTK_LIST_STORE(storeDev[crt_n]));
	if (fixed)
	{
		GetStationNum(&station_num);
		for (i=1;i<=station_num;i++)
		{
			if (!strcmp(StnDef[i].CName,pConvert1=_fromUtf8(stnName)))
			{
				for(j=1;j<=StnDef[i].dev_num;j++)
				{
					if(strcmp(StnDef[i].dev[j].CName,"")!=0)
					{ 
						if (FilterCheck[crt_n].dev_mask[curStn] & (0X1<<(j-1)) ) 
							fixed = TRUE;  
						else
							fixed = FALSE;
						gtk_list_store_append (GTK_LIST_STORE(storeDev[crt_n]), &iter);
						gtk_list_store_set (GTK_LIST_STORE(storeDev[crt_n]), &iter,COLUMN_CHECKED, fixed,COLUMN_NAME, pConvert=_toUtf8(StnDef[i].dev[j].CName),-1);
						G_SAFE_FREE(pConvert);//dzy1017
					}
				}
			}
			G_SAFE_FREE(pConvert1);//dzy1017
		}
	}
	g_free(stnName);
	return TRUE;
}

gboolean DevClicked(GtkTreeView *treeview,
					gpointer     data)
{
	GtkTreePath *path;
	GtkTreeModel *model;
	GtkTreeIter  iter;
	int i,j;
	gchar *devName;
	gint *path_num;
	BOOL fixed;
	gint crt_n = (gint) data ;
	gchar *pConvert=0, *pConvert1=0;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE;
	gtk_tree_view_get_cursor(treeview,&path,NULL);
	path_num = gtk_tree_path_get_indices(path);
	curDev = *path_num;
	
	model = gtk_tree_view_get_model(treeview);
	gtk_tree_model_get_iter(model,&iter,path);
	gtk_tree_model_get(model,&iter,COLUMN_CHECKED,&fixed,-1);
	gtk_tree_model_get (model, &iter, COLUMN_NAME, &devName, -1);
	gtk_list_store_clear(GTK_LIST_STORE(storeGroup[crt_n]));
	if (fixed)
	{
		for(i=1;i<=StnDef[curStn+1].dev_num;i++)
		{
			if (!strcmp(StnDef[curStn+1].dev[i].CName,pConvert1=_fromUtf8(devName)))
			{
				for (j=1;j<=(int)StnDef[curStn+1].dev[i].group_num;j++)
				{
					if(strcmp(StnDef[curStn+1].dev[i].group[j].CName,"")!=0)
					{ 
						if (FilterCheck[crt_n].group_mask[i-1] & (0X1<<(j-1)) ) 
							fixed = TRUE;  
						else
							fixed = FALSE;
						gtk_list_store_append (GTK_LIST_STORE(storeGroup[crt_n]), &iter);
						gtk_list_store_set (GTK_LIST_STORE(storeGroup[crt_n]), &iter,COLUMN_CHECKED, fixed,COLUMN_NAME, pConvert=_toUtf8(StnDef[curStn+1].dev[i].group[j].CName),-1);
						G_SAFE_FREE(pConvert);//dzy1017
					}
				}
				G_SAFE_FREE(pConvert1);//dzy1017
			}
		}
	}
	g_free(devName);
	return TRUE;
}

static void 
SetStnMask(GtkCellRendererToggle *cell,
		   gchar                 *path_str,
		   gpointer               data)
{
	GtkTreeIter  iter;
	GtkTreePath *path = gtk_tree_path_new_from_string (path_str);
	gboolean fixed;
	gint crt_n = (gint) data ;
	gint i = (gint)g_ascii_strtoll(path_str,NULL,0);
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return  ;
	gtk_tree_model_get_iter (GTK_TREE_MODEL(storeStn[crt_n]), &iter, path);
	gtk_tree_model_get (GTK_TREE_MODEL(storeStn[crt_n]), &iter, COLUMN_CHECKED, &fixed, -1);
	fixed ^= 1;
	gtk_list_store_set (storeStn[crt_n], &iter, COLUMN_CHECKED, fixed, -1);
	//	StnClicked(GTK_TREE_VIEW(gtk_tree_view_new_with_model(GTK_TREE_MODEL(storeStn[crt_n]))),data);
	if (fixed)
		FilterCheck[crt_n].stn_mask = FilterCheck[crt_n].stn_mask  | (1<<i);
	else
		FilterCheck[crt_n].stn_mask = FilterCheck[crt_n].stn_mask  & (~(1<<i)); /*  Clear bit N */
	gtk_tree_path_free (path);  
}
static void 
SetDevMask(GtkCellRendererToggle *cell,
		   gchar                 *path_str,
		   gpointer               data)
{
	GtkTreeIter  iter;
	GtkTreePath *path = gtk_tree_path_new_from_string (path_str);
	gboolean fixed;
	gint crt_n = (gint) data ;
	gint i = (gint)g_ascii_strtoll(path_str,NULL,0);
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return  ;
	gtk_tree_model_get_iter (GTK_TREE_MODEL(storeDev[crt_n]), &iter, path);
	gtk_tree_model_get (GTK_TREE_MODEL(storeDev[crt_n]), &iter, COLUMN_CHECKED, &fixed, -1);
	fixed ^= 1;
	gtk_list_store_set (storeDev[crt_n], &iter, COLUMN_CHECKED, fixed, -1);
	//DevClicked(GTK_TREE_VIEW(gtk_tree_view_new_with_model(GTK_TREE_MODEL(storeDev[crt_n]))),data);
	if (fixed)
		FilterCheck[crt_n].dev_mask[curStn] = FilterCheck[crt_n].dev_mask[curStn]  | (1<<i);
	else
		FilterCheck[crt_n].dev_mask[curStn] = FilterCheck[crt_n].dev_mask[curStn]  & (~(1<<i)); /*  Clear bit N */
	gtk_tree_path_free (path);  
}
static void 
SetGroupMask(GtkCellRendererToggle *cell,
			 gchar                 *path_str,
			 gpointer               data)
{
	GtkTreeIter  iter;
	GtkTreePath *path = gtk_tree_path_new_from_string (path_str);
	gboolean fixed;
	gint crt_n = (gint) data ;
	gint i = (gint)g_ascii_strtoll(path_str,NULL,0);
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return  ;
	gtk_tree_model_get_iter (GTK_TREE_MODEL(storeGroup[crt_n]), &iter, path);
	gtk_tree_model_get (GTK_TREE_MODEL(storeGroup[crt_n]), &iter, COLUMN_CHECKED, &fixed, -1);
	fixed ^= 1;
	gtk_list_store_set (storeGroup[crt_n], &iter, COLUMN_CHECKED, fixed, -1);
	if (fixed)
		FilterCheck[crt_n].group_mask[curDev] = FilterCheck[crt_n].group_mask[curDev]  | (1<<i);
	else
		FilterCheck[crt_n].group_mask[curDev] = FilterCheck[crt_n].group_mask[curDev]  & (~(1<<i)); /*  Clear bit N */
	gtk_tree_path_free (path);  
}
static void 
SetTypeMask(GtkCellRendererToggle *cell,
			gchar                 *path_str,
			gpointer               data)
{
	GtkTreeIter  iter;
	GtkTreePath *path = gtk_tree_path_new_from_string (path_str);
	gboolean fixed;
	gint crt_n = (gint) data ;
	gint i = (gint)g_ascii_strtoll(path_str,NULL,0);
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return  ;
	gtk_tree_model_get_iter (GTK_TREE_MODEL(storeType[crt_n]), &iter, path);
	gtk_tree_model_get (GTK_TREE_MODEL(storeType[crt_n]), &iter, COLUMN_CHECKED, &fixed, -1);
	fixed ^= 1;
	gtk_list_store_set (storeType[crt_n], &iter, COLUMN_CHECKED, fixed, -1);
	if (fixed)
		FilterCheck[crt_n].datatype_mask = FilterCheck[crt_n].datatype_mask  | (1<<i);
	else
		FilterCheck[crt_n].datatype_mask = FilterCheck[crt_n].datatype_mask  & (~(1<<i)); /*  Clear bit N */
	gtk_tree_path_free (path);  
}
static void 
SetAttribMask(GtkCellRendererToggle *cell,
			  gchar                 *path_str,
			  gpointer               data)
{
	GtkTreeIter  iter;
	GtkTreePath *path = gtk_tree_path_new_from_string (path_str);
	gboolean fixed;
	gint crt_n = (gint) data ;
	gint i = (gint)g_ascii_strtoll(path_str,NULL,0);
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return  ;
	gtk_tree_model_get_iter (GTK_TREE_MODEL(storeAttrib[crt_n]), &iter, path);
	gtk_tree_model_get (GTK_TREE_MODEL(storeAttrib[crt_n]), &iter, COLUMN_CHECKED, &fixed, -1);
	fixed ^= 1;
	gtk_list_store_set (storeAttrib[crt_n], &iter, COLUMN_CHECKED, fixed, -1);
	if (fixed)
		FilterCheck[crt_n].attrib_mask[0] = FilterCheck[crt_n].attrib_mask[0]  | (1<<i);
	else
		FilterCheck[crt_n].attrib_mask[0] = FilterCheck[crt_n].attrib_mask[0]  & (~(1<<i)); /*  Clear bit N */
	gtk_tree_path_free (path);  
}
void RadioToggled1(GtkWidget		*widget,
				   gpointer       data)
{
	gint crt_n = (gint) data ;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return ;
	if (gtk_toggle_button_get_active  (GTK_TOGGLE_BUTTON(widget)))
	{
		gtk_list_store_clear(GTK_LIST_STORE(storeFilter[crt_n]));
		ReadFilterXml(GTK_LIST_STORE(storeFilter[crt_n]),"Predefine");
		gtk_widget_set_sensitive(GTK_WIDGET(buttonAdd[crt_n]),FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(buttonDel[crt_n]),FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(buttonEdit[crt_n]),FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(comboBox[crt_n]),TRUE);
		gtk_combo_box_set_active(GTK_COMBO_BOX(comboBox[crt_n]),-1);
		template_flag = 0;//预定义模板
	}
}

void RadioToggled2(GtkWidget		*widget,
				   gpointer       data)
{
	gint crt_n = (gint) data ;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return ;
	if (gtk_toggle_button_get_active  (GTK_TOGGLE_BUTTON(widget)))
	{
		gtk_list_store_clear(GTK_LIST_STORE(storeFilter[crt_n]));
		ReadFilterXml(GTK_LIST_STORE(storeFilter[crt_n]),"Userdefine");
		gtk_widget_set_sensitive(GTK_WIDGET(buttonAdd[crt_n]),TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(buttonDel[crt_n]),TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(buttonEdit[crt_n]),TRUE);
		gtk_widget_set_sensitive(GTK_WIDGET(comboBox[crt_n]),TRUE);
		gtk_combo_box_set_active(GTK_COMBO_BOX(comboBox[crt_n]),-1);
		template_flag = 1;//自定义模板
	}
}
void RadioToggled3(GtkWidget		*widget,
				   gpointer       data)
{
	gint crt_n = (gint) data ;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return ;
	if (gtk_toggle_button_get_active  (GTK_TOGGLE_BUTTON(widget)))
	{
		gtk_widget_set_sensitive(GTK_WIDGET(buttonAdd[crt_n]),FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(buttonDel[crt_n]),FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(buttonEdit[crt_n]),FALSE);
		gtk_widget_set_sensitive(GTK_WIDGET(comboBox[crt_n]),FALSE);
		gtk_combo_box_set_active(GTK_COMBO_BOX(comboBox[crt_n]),-1);
		InitAlarmFilter(&FilterCheck[crt_n],crt_n);
		UpdataViewFromCheckFilter(&FilterCheck[crt_n],crt_n);
		template_flag = 2;//无模板
	}
}

gint on_DspSearch_btn_pressed(GtkWidget *widget,gpointer data)
{
	int i;
	int pt_num=0;
	GtkTreeIter iter;
	gchar *attribName;
	BOOL fixed;
    gint crt_n = (gint) data ;
	gchar *pConvert=0;
	com_property_col= sizeof(com_property)/sizeof(com_property[0]);
	Imp_property_col= sizeof(Imp_property)/sizeof(Imp_property[0]);
	Pol_SOE_property_col= sizeof(Pol_SOE_property)/sizeof(Pol_SOE_property[0]);
	opr_num = sizeof(opr_name)/sizeof(opr_name[0]);
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE;
	search_pt.stn_id = FilterCheck[crt_n].stn_id;
	search_pt.stn_mask = FilterCheck[crt_n].stn_mask;
	search_pt.dev_id = FilterCheck[crt_n].dev_id;
	search_pt.dev_mask = FilterCheck[crt_n].dev_mask[0];
	search_pt.data_type = FilterCheck[crt_n].datatype_mask;
	search_pt.stn_id = 1;
	search_pt.dev_id = 1;
	if (search_pt.stn_id==0)
	{
		PopupMessageBox(CondCheckDlg[crt_n], (MSG_MUST_SEL_STATION_FIRST));  //"MSG_MUST_SEL_STATION_FIRST" 必须先选择一个电站
        return FALSE;
	}
	
	if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(storeAttrib[crt_n]), &iter))
	{
		for (i=0;i<OPT_ITEM_NUM;i++)
		{
			gtk_tree_model_get(GTK_TREE_MODEL(storeAttrib[crt_n]),&iter,COLUMN_CHECKED,&fixed);
			gtk_tree_model_get(GTK_TREE_MODEL(storeAttrib[crt_n]),&iter,COLUMN_NAME,&attribName);
			strcpy(search_pt.property[i],pConvert=_fromUtf8(attribName));
			G_SAFE_FREE(pConvert);//dzy1017
			strcpy(search_pt.opr[i],"=");
			if (fixed)
				strcpy(search_pt.data.str[i],"1");
			else
				strcpy(search_pt.data.str[i],"empty");
			gtk_tree_model_iter_next(GTK_TREE_MODEL(storeAttrib[crt_n]),&iter);
		}
	}
	/*first delete old result win!!*/
	if (SearchResultWin[crt_n])
	{
		gtk_widget_destroy(SearchResultWin[crt_n]) ;
		SearchResultWin[crt_n]= (void *)NULL ;  
	}
	if  ((pt_num=SearchData()) <=0)
	{
		PopupMessageBox( CondCheckDlg[crt_n],(MSG_NUM_RESULT_IS_ZERO)) ; //"Number of results is zero!!!");
		return  FALSE; 
		
	}
	if (SearchResultWin[crt_n]=DspResult(crt_n,pt_num,0))
		DspSearchPage(crt_n,cur_search_page.end_id);
	g_free(attribName);
	return TRUE;
}

gint on_button_add_clicked(GtkWidget *widget,gpointer data)
{
	gchar *filterName,*m_filterName;
	GtkTreeIter iter;
	gint crt_n = (gint) data ;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE;
	
	filterName =(gchar*) gtk_entry_get_text(GTK_ENTRY(GTK_BIN (comboBox[crt_n])->child));
	if (strcmp(filterName,"")==0)
	{
		PopupMessageBox(CondCheckDlg[crt_n],MSG_INPUT_FILTER_NAME);
		return FALSE;
	}
	if (gtk_tree_model_get_iter_first(GTK_TREE_MODEL(storeFilter[crt_n]),&iter))
	{
		do 
		{
			gtk_tree_model_get(GTK_TREE_MODEL(storeFilter[crt_n]),&iter,eFilterName,&m_filterName,-1);
			if (strcmp(filterName,m_filterName)==0)
			{
				PopupMessageBox(CondCheckDlg[crt_n],MSG_FILTER_NAME_REPEAT);
				return FALSE;
			}
			
		} while (gtk_tree_model_iter_next(GTK_TREE_MODEL(storeFilter[crt_n]),&iter));
	}
	SaveFilterXml(filterName,&FilterCheck[crt_n],"Userdefine");
	gtk_list_store_clear(storeFilter[crt_n]);
	ReadFilterXml(GTK_LIST_STORE(storeFilter[crt_n]),"Userdefine");
	gtk_entry_set_text(GTK_ENTRY(GTK_BIN (comboBox[crt_n])->child),"");
	gtk_combo_box_popup(GTK_COMBO_BOX(comboBox[crt_n]));
	return TRUE;
}
gint on_button_del_clicked(GtkWidget *widget,gpointer data)
{
	gchar *filterName;
	gint crt_n = (gint) data ;
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE;
	
	filterName =(gchar*) gtk_entry_get_text(GTK_ENTRY(GTK_BIN (comboBox[crt_n])->child));
	if (filterName != NULL)
		DelFilterXml(filterName,"Userdefine");
	gtk_list_store_clear(GTK_LIST_STORE(storeFilter[crt_n]));
	ReadFilterXml(GTK_LIST_STORE(storeFilter[crt_n]),"Userdefine");
	gtk_entry_set_text(GTK_ENTRY(GTK_BIN (comboBox[crt_n])->child),"");
	gtk_combo_box_popup(GTK_COMBO_BOX(comboBox[crt_n]));
	return TRUE;
}
int CondCheckDelete(GtkWidget *w , gpointer data) ;
char *BuffAttrib[]  = {OIX_CSTR_ScanDisable,OIX_CSTR_AlarmDisable,OIX_CSTR_AlarmNoAck,OIX_STR_TAG_STATE,OIX_CSTR_SetNumber};
static GtkWidget*
CreatCondCheckDlg(GtkWidget *parent,int crt_n)
{
	GtkWidget *windows;
	GtkWidget *vbox,*vbox1,*vbox2,*hbox1,*hbox2,*hbox3,*hbox4;
	GtkWidget *scrolled_window1,*scrolled_window2,*scrolled_window3,*scrolled_window4,*scrolled_window5;
	GtkWidget *radio1,*radio2,*radio3;
	GtkWidget *button6;
	GtkWidget *listStn,*listDev,*listGroup,*listType,*listAttrib;
	GtkTreeIter iter;
	GtkCellRenderer *renderer;
	GtkTreeViewColumn *column;
	int station_num,i;
	char  recordTypeName[RECORD_LONGNAME_SIZE];
	gchar *pConvert=0;
	InitAlarmFilter(&FilterCheck[crt_n],crt_n);
	
	windows = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(windows),pConvert=_toUtf8EX(OIX_STR_COND_CHECK));
	G_SAFE_FREE(pConvert);//dzy1017
	gtk_window_set_transient_for(GTK_WINDOW(windows), GTK_WINDOW(parent)); 
	gtk_window_set_default_size(GTK_WINDOW(windows),800,600);
	gtk_signal_connect (GTK_OBJECT (windows), "delete_event", 
		GTK_SIGNAL_FUNC (CondCheckDelete),  GINT_TO_POINTER(crt_n)); 
	
	gtk_window_set_destroy_with_parent(GTK_WINDOW(windows),TRUE);
	gtk_window_set_position(GTK_WINDOW(windows),GTK_WIN_POS_CENTER);  
	gtk_widget_show (windows);
	
	vbox = gtk_vbox_new(FALSE,0);
	gtk_container_add (GTK_CONTAINER (windows), vbox);
	hbox1 = gtk_hbox_new(FALSE,0);
	gtk_box_pack_start (GTK_BOX (vbox), hbox1, FALSE, FALSE, 10);
	
	vbox1 = gtk_vbox_new(FALSE,0);
	radio1 = gtk_radio_button_new_with_label (NULL,pConvert=_toUtf8EX(OIX_STR_TEM_PREDEF));
	G_SAFE_FREE(pConvert);//dzy1017
	radio2 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON(radio1),pConvert=_toUtf8EX(OIX_STR_TEM_CUSTOM));
	G_SAFE_FREE(pConvert);//dzy1017
	radio3 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON(radio1),pConvert=_toUtf8EX(OIX_STR_TEM_NONE));
	G_SAFE_FREE(pConvert);//dzy1017
	g_signal_connect(GTK_OBJECT(radio1),"toggled",GTK_SIGNAL_FUNC(RadioToggled1),GINT_TO_POINTER(crt_n));
	g_signal_connect(GTK_OBJECT(radio2),"toggled",GTK_SIGNAL_FUNC(RadioToggled2),GINT_TO_POINTER(crt_n));
	g_signal_connect(GTK_OBJECT(radio3),"toggled",GTK_SIGNAL_FUNC(RadioToggled3),GINT_TO_POINTER(crt_n));
	gtk_box_pack_start (GTK_BOX (vbox1), radio1, FALSE, FALSE, 0);
	//gtk_box_pack_start (GTK_BOX (vbox1), radio2, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (vbox1), radio3, FALSE, FALSE, 0);
	gtk_box_pack_start (GTK_BOX (hbox1), vbox1, FALSE, FALSE, 50);
	
	vbox2 = gtk_vbox_new(FALSE,0);
	storeFilter[crt_n]=gtk_list_store_new(num_columns,G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING,G_TYPE_STRING, G_TYPE_STRING,G_TYPE_STRING);
	//	comboBox[crt_n] = gtk_combo_box_new_with_model(GTK_TREE_MODEL(storeFilter[crt_n]));
	comboBox[crt_n] = gtk_combo_box_entry_new_with_model(GTK_TREE_MODEL(storeFilter[crt_n]),0);
	g_signal_connect (G_OBJECT(comboBox[crt_n]), "changed", G_CALLBACK(ChgTemplate), GINT_TO_POINTER(crt_n));
	// 	renderer = gtk_cell_renderer_text_new();
	//  gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(comboBox[crt_n]), renderer, FALSE);
	//  gtk_cell_layout_set_attributes(GTK_CELL_LAYOUT(comboBox[crt_n]), renderer, "text", eFilterName, NULL);
	g_object_unref(storeFilter);
	gtk_box_pack_start (GTK_BOX (vbox2), comboBox[crt_n], FALSE, FALSE, 0);
	
	hbox2 = gtk_hbox_new(FALSE,0);
	buttonAdd[crt_n] = gtk_button_new_with_mnemonic(pConvert=_toUtf8EX(OIX_STR_TEM_ADD));
	G_SAFE_FREE(pConvert);//dzy1017
	buttonDel[crt_n] = gtk_button_new_with_mnemonic(pConvert=_toUtf8EX(OIX_STR_TEM_DEL));
	G_SAFE_FREE(pConvert);//dzy1017
	buttonEdit[crt_n] = gtk_button_new_with_mnemonic(pConvert=_toUtf8EX(OIX_STR_TEM_EDIT));
	G_SAFE_FREE(pConvert);//dzy1017
	gtk_signal_connect(GTK_OBJECT(buttonAdd[crt_n]), "clicked", GTK_SIGNAL_FUNC(on_button_add_clicked),   GINT_TO_POINTER(crt_n));
	gtk_signal_connect(GTK_OBJECT(buttonDel[crt_n]), "clicked", GTK_SIGNAL_FUNC(on_button_del_clicked),   GINT_TO_POINTER(crt_n));
	//	gtk_signal_connect(GTK_OBJECT(buttonEdit[crt_n]), "clicked", GTK_SIGNAL_FUNC(on_button_edit_clicked),   GINT_TO_POINTER(crt_n));
//	gtk_box_pack_start (GTK_BOX (hbox2), buttonAdd[crt_n], TRUE, TRUE, 10);
//	gtk_box_pack_start (GTK_BOX (hbox2), buttonDel[crt_n], TRUE, TRUE, 10);
//	gtk_box_pack_start (GTK_BOX (hbox2), buttonEdit[crt_n], TRUE, TRUE, 10);
	gtk_box_pack_start (GTK_BOX (vbox2), hbox2, FALSE, FALSE, 10);
	gtk_box_pack_start (GTK_BOX (hbox1), vbox2, TRUE, TRUE, 100);
	RadioToggled1(GTK_WIDGET(radio1),GINT_TO_POINTER(crt_n));
	
	storeStn[crt_n] = gtk_list_store_new (NUM_COLUMNS,G_TYPE_BOOLEAN,G_TYPE_STRING);
	storeDev[crt_n] = gtk_list_store_new (NUM_COLUMNS,G_TYPE_BOOLEAN,G_TYPE_STRING);
	storeGroup[crt_n] = gtk_list_store_new (NUM_COLUMNS,G_TYPE_BOOLEAN,G_TYPE_STRING);
	storeType[crt_n] = gtk_list_store_new (NUM_COLUMNS,G_TYPE_BOOLEAN,G_TYPE_STRING);
	storeAttrib[crt_n] = gtk_list_store_new (NUM_COLUMNS,G_TYPE_BOOLEAN,G_TYPE_STRING);
	hbox3 = gtk_hbox_new(FALSE,0);
	gtk_box_pack_start (GTK_BOX (vbox), hbox3, TRUE, TRUE, 0);
	listStn = gtk_tree_view_new_with_model(GTK_TREE_MODEL(storeStn[crt_n]));
	gtk_signal_connect(GTK_OBJECT(listStn),"cursor-changed",G_CALLBACK(StnClicked),GINT_TO_POINTER(crt_n));
	g_object_unref (storeStn[crt_n]);
	renderer = gtk_cell_renderer_toggle_new ();
	g_signal_connect (renderer, "toggled",G_CALLBACK (SetStnMask), GINT_TO_POINTER(crt_n));
	column = gtk_tree_view_column_new_with_attributes (pConvert=_toUtf8EX(OIX_STR_STATION),renderer,"active", COLUMN_CHECKED,NULL);
	G_SAFE_FREE(pConvert);//dzy1017
	gtk_tree_view_append_column (GTK_TREE_VIEW(listStn), column);
	renderer = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes (NULL,renderer,"text",COLUMN_NAME,NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW(listStn), column);
	//	gtk_tree_view_set_headers_visible(listStn,FALSE);
	scrolled_window1=gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_set_usize(scrolled_window1, 80, 400); 
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window1),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
	gtk_box_pack_start (GTK_BOX (hbox3), scrolled_window1, TRUE, TRUE, 5);
	gtk_container_add(GTK_CONTAINER(scrolled_window1), listStn);
	
	listDev = gtk_tree_view_new_with_model(GTK_TREE_MODEL(storeDev[crt_n]));
	gtk_signal_connect(GTK_OBJECT(listDev),"cursor-changed",G_CALLBACK(DevClicked),GINT_TO_POINTER(crt_n));
	g_object_unref (storeDev[crt_n]);
	renderer = gtk_cell_renderer_toggle_new ();
	g_signal_connect (renderer, "toggled",G_CALLBACK (SetDevMask), GINT_TO_POINTER(crt_n));
	column = gtk_tree_view_column_new_with_attributes (pConvert=_toUtf8EX(OIX_STR_DEV),renderer,"active", COLUMN_CHECKED,NULL);
	G_SAFE_FREE(pConvert);//dzy1017
	gtk_tree_view_append_column (GTK_TREE_VIEW(listDev), column);
	renderer = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes (NULL,renderer,"text",COLUMN_NAME,NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW(listDev), column);
	//	gtk_tree_view_set_headers_visible(listDev,FALSE);
	scrolled_window2=gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_set_usize(scrolled_window2, 80, 400); 
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window2),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
	gtk_box_pack_start (GTK_BOX (hbox3), scrolled_window2, TRUE, TRUE, 5);
	gtk_container_add(GTK_CONTAINER(scrolled_window2), listDev);
	
	listGroup = gtk_tree_view_new_with_model(GTK_TREE_MODEL(storeGroup[crt_n]));
	g_object_unref (storeGroup[crt_n]);
	renderer = gtk_cell_renderer_toggle_new ();
	g_signal_connect (renderer, "toggled",G_CALLBACK (SetGroupMask), GINT_TO_POINTER(crt_n));
	column = gtk_tree_view_column_new_with_attributes (pConvert=_toUtf8EX(OIX_STR_GROUP),renderer,"active", COLUMN_CHECKED,NULL);
	G_SAFE_FREE(pConvert);//dzy1017
	gtk_tree_view_append_column (GTK_TREE_VIEW(listGroup), column);
	renderer = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes (NULL,renderer,"text",COLUMN_NAME,NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW(listGroup), column);
	//	gtk_tree_view_set_headers_visible(listGroup,FALSE);
	scrolled_window3=gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_set_usize(scrolled_window3, 80, 400); 
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window3),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
	gtk_box_pack_start (GTK_BOX (hbox3), scrolled_window3, TRUE, TRUE, 5);
	gtk_container_add(GTK_CONTAINER(scrolled_window3), listGroup);
	
	listType = gtk_tree_view_new_with_model(GTK_TREE_MODEL(storeType[crt_n]));
	g_object_unref (storeType[crt_n]);
	renderer = gtk_cell_renderer_toggle_new ();
	g_signal_connect (renderer, "toggled",G_CALLBACK (SetTypeMask), GINT_TO_POINTER(crt_n));
	column = gtk_tree_view_column_new_with_attributes (pConvert=_toUtf8EX(OIX_STR_TYPE),renderer,"active", COLUMN_CHECKED,NULL);
	G_SAFE_FREE(pConvert);//dzy1017
	gtk_tree_view_append_column (GTK_TREE_VIEW(listType), column);
	renderer = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes (NULL,renderer,"text",COLUMN_NAME,NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW(listType), column);
	//	gtk_tree_view_set_headers_visible(listType,FALSE);
	scrolled_window4=gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_set_usize(scrolled_window4, 80, 400); 
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window4),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
	gtk_box_pack_start (GTK_BOX (hbox3), scrolled_window4, TRUE, TRUE, 5);
	gtk_container_add(GTK_CONTAINER(scrolled_window4), listType);
	
	listAttrib = gtk_tree_view_new_with_model(GTK_TREE_MODEL(storeAttrib[crt_n]));
	g_object_unref (storeAttrib[crt_n]);
	renderer = gtk_cell_renderer_toggle_new ();
	g_signal_connect (renderer, "toggled",G_CALLBACK (SetAttribMask), GINT_TO_POINTER(crt_n));
	column = gtk_tree_view_column_new_with_attributes (pConvert=_toUtf8EX(OIX_STR_ATTRIB),renderer,"active", COLUMN_CHECKED,NULL);
	G_SAFE_FREE(pConvert);//dzy1017
	gtk_tree_view_append_column (GTK_TREE_VIEW(listAttrib), column);
	renderer = gtk_cell_renderer_text_new ();
	column = gtk_tree_view_column_new_with_attributes (NULL,renderer,"text",COLUMN_NAME,NULL);
	gtk_tree_view_append_column (GTK_TREE_VIEW(listAttrib), column);
	//	gtk_tree_view_set_headers_visible(listAttrib,FALSE);
	scrolled_window5=gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_set_usize(scrolled_window5, 80, 400); 
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window5),GTK_POLICY_AUTOMATIC,GTK_POLICY_AUTOMATIC);
	gtk_box_pack_start (GTK_BOX (hbox3), scrolled_window5, TRUE, TRUE, 5);
	gtk_container_add(GTK_CONTAINER(scrolled_window5), listAttrib);
	
	
	hbox4 = gtk_hbox_new(FALSE,0);
	gtk_box_pack_start (GTK_BOX (vbox), hbox4, FALSE, FALSE, 10);
	/*
	lable = gtk_label_new(_toUtf8EX(OIX_STR_CHECK_FILE));
	gtk_box_pack_start(GTK_BOX(hbox4),lable,FALSE,FALSE,10);
	entry = gtk_entry_new();
	gtk_box_pack_start(GTK_BOX(hbox4),entry,FALSE,FALSE,5);
	button4 = gtk_button_new_with_mnemonic(_toUtf8EX(OIX_STR_SAVE_AS));
	gtk_box_pack_start(GTK_BOX(hbox4),button4,TRUE,TRUE,5);
	check = gtk_check_button_new_with_label(_toUtf8EX(OIX_STR_CHANGE_ROOM));
	gtk_box_pack_start(GTK_BOX(hbox4),check,TRUE,TRUE,80);
	button5 = gtk_button_new_with_mnemonic(_toUtf8EX(OIX_STR_SEARCH_DB));
	gtk_signal_connect(GTK_OBJECT(button5), "clicked", GTK_SIGNAL_FUNC(on_DspSearch_btn_pressed),   GINT_TO_POINTER(crt_n));
	gtk_box_pack_start(GTK_BOX(hbox4),button5,TRUE,FALSE,0);
	*/
	button6 = gtk_button_new_with_mnemonic(pConvert=_toUtf8EX(OIX_STR_EXE_CHECK));
	G_SAFE_FREE(pConvert);//dzy1017
	gtk_signal_connect(GTK_OBJECT(button6), "clicked", GTK_SIGNAL_FUNC(ProgConditionCheck),   GINT_TO_POINTER(crt_n));
	gtk_box_pack_start(GTK_BOX(hbox4),button6,FALSE,FALSE,100);
	
	GetStationNum(&station_num);
	for(i=1;i<=station_num;i++) 
	{
		if(strcmp(StnDef[i].CName,"")!=0)
		{					
			//if (isStnInMask((UCHAR)i)==0) continue ;	 
			gtk_list_store_append (storeStn[crt_n], &iter);
			gtk_list_store_set (storeStn[crt_n], &iter,COLUMN_CHECKED, TRUE,COLUMN_NAME, pConvert=_toUtf8(StnDef[i].CName),-1);	
			G_SAFE_FREE(pConvert);//dzy1017
		}
	}
	
	//StnClicked(listStn,gtk_tree_path_new_from_indices(0),NULL,GINT_TO_POINTER(crt_n));
	
	
	for(i=0;i<MAX_DATATYPE_NUM;i++)
	{
		if(GetRecLongnameByType((UCHAR)i, recordTypeName )!=-1)
		{	
			gtk_list_store_append (storeType[crt_n], &iter);
			gtk_list_store_set (storeType[crt_n], &iter,COLUMN_CHECKED, TRUE,COLUMN_NAME, pConvert=_toUtf8(recordTypeName),-1);
			G_SAFE_FREE(pConvert);//dzy1017
		}
	}
	for (i=0;i<5;i++)
	{
		gtk_list_store_append(storeAttrib[crt_n], &iter);
		gtk_list_store_set(storeAttrib[crt_n], &iter,COLUMN_CHECKED,TRUE,COLUMN_NAME,pConvert=_toUtf8(_CS_(BuffAttrib[i])),-1);
		G_SAFE_FREE(pConvert);//dzy1017
	}
	
	gtk_widget_show_all(windows);
	return windows;
}

int CondCheckDelete(GtkWidget *w , gpointer data)
{
	int  crt_n = (gint)data;
	GetCrtNumberOfWgt (w, &crt_n );
	if( crt_n < 0 || crt_n > MAX_CRT_NUMBER )
		return FALSE ;
	gtk_widget_destroy(CondCheckDlg[crt_n]);
	CondCheckDlg[crt_n] = (GtkWidget*)NULL ;
	return TRUE;
}
void ProgCondCheckTab (GtkWidget *parent, int crt_n )
{
	if (CondCheckDlg[crt_n]==(GtkWidget*)NULL)
	{
		CondCheckDlg[crt_n] = CreatCondCheckDlg(parent,crt_n);	
	}
	else
	{
		gtk_window_present(GTK_WINDOW(CondCheckDlg[crt_n]));
	}
}
