/****************************************
2005.11 updated by zyp for gtk
2006.3 for v4.0db
Windows Have  SYN to  UNIX  by chi 07.12.04 ;
********************************************/

#include <gtk/gtk.h>

#include	"../inc/gtk_ipm_k_def.h"



#include	"../inc/oix.h"
#include	"../inc/gtk_widgets.h"

/*char	demo_dsp_name_str[MAX_DEMO_DPS_NAME][MAX_FILE_NAME_LEN];*/
short	demo_dsp_flag=DEMO_DSP_OFF;/*[MAX_CRT_NUMBER]	= { DEMO_DSP_OFF, DEMO_DSP_OFF};*/
static int pause_demo=DEMO_DSP_OFF;/*[MAX_CRT_NUMBER] = { DEMO_DSP_OFF, DEMO_DSP_OFF};*/
int disable_demo_flag[MAX_CRT_NUMBER];

typedef struct
{
	char dspName[MAX_FILE_NAME_LEN];
	struct
	{
		char pointName[OLD_DBNAME_LEN-1];
		int type;
		union
		{
			USHORT	state;
			float	fValue;
		}realValue;
		BOOL fault;
		BOOL fail;
		BOOL almNoAck;
	}point[MAXOBJECTS];
	int  pointNum;
}DSP_CHECK_RESULT; 


DEMO_DSP_CLASS demo_dsp_class[MAX_DEMO_CLASS_NUM];


static	int	demo_timeout_id[MAX_CRT_NUMBER]  = { 0, 0};
static	short	dsp_cnt[MAX_CRT_NUMBER],dsp_cnt_before_pause[MAX_CRT_NUMBER];
extern int num_screen;
extern void	PopupMessageBox (GtkWidget *parent,char msg[] );
extern void GetCrtNumberOfWgt ( GtkWidget  *wgt, int     *crt_n );
extern int OpenDspFile (int crt_n, char	*filename);
extern char *_toUtf8EX(char *c);
void DemoDspProc (int crt_n/*GtkWidget* */ );
extern void PopupAutocheckFile(int crt_n,char* pathfilename);

void ChangeDspMode (int crt_n ,int mode )
{      
	if ( mode == DEMO_DSP_ON/*&&demo_dsp_flag[crt_n]== DEMO_DSP_OFF */)
	{
		demo_dsp_flag/*[crt_n]*/	= DEMO_DSP_ON;
		if(pause_demo/*[crt_n]*/ == DEMO_DSP_OFF && dsp_cnt_before_pause[crt_n]==0)
		dsp_cnt[crt_n]	= 0;
		/* printf("in ChangeDspMode::pause_demo=%d\n",pause_demo[crt_n]);*/
		
		if(pause_demo/*[crt_n]*/==DEMO_DSP_OFF)
		{
			printf("here is pause_demo is OFF,dsp_cnt=%d\n",dsp_cnt[crt_n]);
			if(demo_period<=0)
			{
				PopupMessageBox(main_w[crt_n],MSG_DDMO_PERIOD_INVALID);
				return;
			}
			else
			{
				DemoDspProc ( crt_n/*canvas[crt_n] */ );  
				
			}
		}
		else
		{
			printf("pause_demo \n");
			demo_dsp_flag/*[crt_n]*/	= DEMO_DSP_OFF;
			if( demo_timeout_id[crt_n] != 0 )
			{                        
				gtk_timeout_remove(demo_timeout_id[crt_n]);
				demo_timeout_id[crt_n]=0;
				printf("##in changeDspMode::remove demo_timeout_id\n");
			}
		}
	}
	else if (mode == DEMO_DSP_OFF/*&&demo_dsp_flag[crt_n]== DEMO_DSP_ON*/)
	{
		dsp_cnt[crt_n]	= 0;
		demo_dsp_flag/*[crt_n]*/	= DEMO_DSP_OFF;
		if( demo_timeout_id[crt_n] != 0 )
		{
			gtk_timeout_remove(demo_timeout_id[crt_n]);
			demo_timeout_id[crt_n]=0;
		}
		printf("DEMO_DSP_OFF:demo_timeout_id=%d\n",demo_timeout_id[crt_n]);
	}
}

void DemoDspProc (int crt_n/*GtkWidget* w*/ )
{
	gchar *pConvert=0;
	/*	int	crt_n;
printf("in DemoDspProc\n");
	GetCrtNumberOfWgt (w, &crt_n);
	if( crt_n < 0 )
		return; */
	if (demo_timeout_id[crt_n] !=0 )
	{
		gtk_timeout_remove(demo_timeout_id[crt_n]); 
		demo_timeout_id[crt_n]= 0;
	}
	
	demo_timeout_id[crt_n]	=gtk_timeout_add (demo_period*1000, (GtkFunction)DemoDspProc,GINT_TO_POINTER( crt_n)/*canvas[crt_n]*/);
	
	g_message(" demo_timer_id[%d] : %d    is added !!!" , crt_n ,demo_timeout_id[crt_n]);
	printf("in DemoDspProc:::dsp_cnt[%d]=%d\n",crt_n,dsp_cnt[crt_n]);
	if (demo_dsp_flag/*[crt_n]*/ == DEMO_DSP_OFF )
	return;
	if (disable_demo_flag[crt_n])
	return;
	/*       printf("in  DemoDspProc:::selected_class[crt_n]=%d\n",selected_class[crt_n]);
printf("###demo_dsp_name_str[%d]=%s\n",selected_class[crt_n],demo_dsp_class[selected_class[crt_n]].demo_dsp_name_str[dsp_cnt[crt_n]]);
*/	
	
	OpenDspFile ( crt_n, demo_dsp_class[selected_class[crt_n]].demo_dsp_name_str[dsp_cnt[crt_n]]);
	if (crt_n == 0)//只在主屏巡检
	{
		if (CheckAllPointOnDsp(crt_n,demo_dsp_class[selected_class[crt_n]].demo_dsp_name_str[dsp_cnt[crt_n]])!=0)
		{
			pause_demo = DEMO_DSP_OFF;
			demo_dsp_flag/*[crt_n]*/	= DEMO_DSP_OFF;
			PauseDemoDsp(NULL,crt_n);
			PopupMessageBox(main_w[crt_n],pConvert = _toUtf8(MSG_FAULT_POINT_ON_DSP));
			G_SAFE_FREE(pConvert);
		}
	}
	if ( dsp_cnt[crt_n] < demo_dsp_class[selected_class[crt_n]].demo_dsp_cnt-1 )
	dsp_cnt[crt_n]++;
	else
	dsp_cnt[crt_n]	= 0;
	dsp_cnt_before_pause[crt_n] = dsp_cnt[crt_n];

	
}

gint PauseDemoDsp(GtkWidget*w,int crt_n)
{

	int i;
	GtkWidget  *pause_demo_btn[MAX_CRT_NUMBER];
	gchar *pConvert=0;

	if(pause_demo==DEMO_DSP_OFF)
	{
		pause_demo = DEMO_DSP_ON;
		demo_dsp_flag	= DEMO_DSP_OFF;
	}
	else
	{
		pause_demo= DEMO_DSP_OFF;    
		demo_dsp_flag	= DEMO_DSP_OFF;
	}

	/*for(i=0;i<MAX_CRT_NUMBER;i++)*/
	for(i=0;i<num_screen;i++) /*hcl*/
	{
		pause_demo_btn[i] = gtk_object_get_data(GTK_OBJECT(main_w[i]),"pause_demo_btn");	
		
		if(pause_demo==DEMO_DSP_ON)
		{
			/*  pause_demo = DEMO_DSP_ON;
			demo_dsp_flag	= DEMO_DSP_OFF;*/
			gtk_object_set(GTK_OBJECT(pause_demo_btn[i]),"GtkButton::label",pConvert=_toUtf8EX(OIX_CONTINUE),NULL);       
		}
		else
		{
			/* pause_demo= DEMO_DSP_OFF;    
			demo_dsp_flag	= DEMO_DSP_OFF;*/
			gtk_object_set(GTK_OBJECT(pause_demo_btn[i]),"GtkButton::label",pConvert=_toUtf8EX(OIX_PAUSE),NULL);
			dsp_cnt[i]=dsp_cnt_before_pause[i];       
		}
		G_SAFE_FREE(pConvert);//dzy1017
		ChangeDspMode ( i ,DEMO_DSP_ON );
	}
	return TRUE;
}

void OpenNextDemoDsp(GtkWidget*w,int crt_n)
{
	int i;
	gchar *pConvert=0;
	/* for(i=0;i<MAX_CRT_NUMBER;i++)*/
	for(i=0;i<num_screen;i++) /*hcl*/
	{
		if (disable_demo_flag[i])
		continue;
		
		OpenDspFile ( i, demo_dsp_class[selected_class[i]].demo_dsp_name_str[dsp_cnt[i]]);
		
		if ( dsp_cnt[i] < demo_dsp_class[selected_class[i]].demo_dsp_cnt-1 )
		dsp_cnt[i]++;
		else
		dsp_cnt[i]	= 0;
	}
/*	if (CheckAllPointOnDsp(crt_n,demo_dsp_class[selected_class[crt_n]].demo_dsp_name_str[dsp_cnt[crt_n]])!=0)
	{
		PopupMessageBox(main_w[crt_n],pConvert = _toUtf8(MSG_FAULT_POINT_ON_DSP));
		G_SAFE_FREE(pConvert);
	}
*/
}

void ManDemodsp(int crt_n)
{

	GtkWidget *next_dsp_btn[MAX_CRT_NUMBER];
	printf("ManDemodsp::demo_timeout_id=%d\n",demo_timeout_id[crt_n]);
	
	if( demo_timeout_id[crt_n] != 0 )
	{                        
		gtk_timeout_remove(demo_timeout_id[crt_n]);
		demo_timeout_id[crt_n]=0;
		printf("##in ManDemodsp:: remove demo_timeout_id\n");
	}
	
	if (disable_demo_flag[crt_n])
	return;
	
	demo_dsp_flag/*[crt_n]*/	= DEMO_DSP_OFF;
	pause_demo/*[crt_n] */= DEMO_DSP_OFF;
	dsp_cnt_before_pause[crt_n]=0;
	dsp_cnt[crt_n]=0;    
	
	next_dsp_btn[crt_n] = gtk_object_get_data(GTK_OBJECT(main_w[crt_n]),"next_dsp_btn");
	gtk_widget_show (next_dsp_btn[crt_n]);   
	
	OpenDspFile ( crt_n, demo_dsp_class[selected_class[crt_n]].demo_dsp_name_str[dsp_cnt[crt_n]]);    
	dsp_cnt[crt_n]++;/* added on 2007.10.24*/
}

void ReadDemoDspName ( char	*full_name )
{
	FILE	*fp;
	int	i,j;
	char   tmp_str[256];
	
	for(i=0;i<num_screen;i++) /*hcl*/
		selected_class[i] =0;
	
	if( (fp = fopen( full_name, "r") ) == NULL)
	{
		printf ("OIX: in OpenFile: file not opened.%s\n", full_name);
		return;
	}

	i=-1;
	j=0;
	
	fscanf (fp,"%d",&demo_period);
	
	while(!feof(fp))
	{                
		fscanf (fp,"%s",tmp_str);
		if(strstr(tmp_str,".dbin")==0) //new class 
		{                          
			i++;

			if(i>=MAX_DEMO_CLASS_NUM)
			{
				printf("OIX: ReadDemoDspName error: too much demo_dsp class: max %d\n",
					MAX_DEMO_CLASS_NUM);
				break;
			}

			strcpy(demo_dsp_class[i].class_name,tmp_str); 
			demo_class_cnt = i+1;
			demo_dsp_class[i].demo_dsp_cnt =0;
			j=0;
		}
		else //demo dsp name
		{
			if(strcmp(tmp_str,"\n")!=0)
			{
				if(i<0)
				{
					printf ( "OIX: ReadDemoDspName error: must define class name first\n");
					continue;
				}
				if(j>=MAX_DEMO_DSP_NUM)
				{
					printf ( "OIX: ReadDemoDspName error: too much demo_dsp_name in class %s: max %d\n",
						demo_dsp_class[i].class_name, MAX_DEMO_DSP_NUM);
				}
				else
				{
					strcpy(demo_dsp_class[i].demo_dsp_name_str[j],tmp_str);
					j++;
					demo_dsp_class[i].demo_dsp_cnt =j;
				}
			}
		}  
	}
	fclose (fp);
	printf ( "OIX: ReadDemoDspName ok,demo_class_cnt=%d\n",demo_class_cnt);
}

void getDspCheckName(char *filename)
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

	sprintf(filename,"%s",str_time);

}

int CheckAllPointOnDsp(int crt_n,char *dspName)
{
	int	i;
	GDBUFF	*layer;
	GBRCD	*rcd;
	DMS_COMMON	dms_cmm;
	DSP_CHECK_RESULT dsp_check_result;
	int Ret = 0;
	dsp_check_result.pointNum = 0;
	strcpy(dsp_check_result.dspName,dspName);
	layer = &gdbase[crt_n]; 
	for ( i=0; i < layer->nextpos; i++ )
	{
		rcd	= &layer->buffer[i];
		if (strcmp(rcd->lnk.dbname,"")!=0 && ReadEntryByNameStr ( rcd->lnk.dbname,&dms_cmm ) != -1)
		{		
			if ((dms_cmm.fixed.ioattr.fault || dms_cmm.fixed.ioattr.fail) && dms_cmm.fixed.iost.almNoAck)
			{
				if (dms_cmm.point.data_type == ANA_TYPE)
				{
					if ((dms_cmm.var.anlg.fValue>dms_cmm.var.anlg.lmt_para.HI_WARN) ||
						(dms_cmm.var.anlg.fValue<dms_cmm.var.anlg.lmt_para.LO_WARN))
					{
					//	WriteDspCheckResult(dspName,rcd->lnk.dbname,&dms_cmm);
						strcpy(dsp_check_result.point[dsp_check_result.pointNum].pointName,rcd->lnk.dbname);
						dsp_check_result.point[dsp_check_result.pointNum].type = dms_cmm.point.data_type;
						dsp_check_result.point[dsp_check_result.pointNum].realValue.fValue = dms_cmm.var.anlg.fValue;
						dsp_check_result.point[dsp_check_result.pointNum].fail = dms_cmm.fixed.ioattr.fail;
						dsp_check_result.point[dsp_check_result.pointNum].fault = dms_cmm.fixed.ioattr.fault;
						dsp_check_result.point[dsp_check_result.pointNum].almNoAck = dms_cmm.fixed.iost.almNoAck;
						dsp_check_result.pointNum++;
						Ret = 1;	
					}
				}
				else
				{
					if ((dms_cmm.var.ind.status.state != dms_cmm.fixed.ioattr.normalState))
					{	
				//		WriteDspCheckResult(dspName,rcd->lnk.dbname,&dms_cmm);
						strcpy(dsp_check_result.point[dsp_check_result.pointNum].pointName,rcd->lnk.dbname);
						dsp_check_result.point[dsp_check_result.pointNum].type = dms_cmm.point.data_type;
						dsp_check_result.point[dsp_check_result.pointNum].realValue.state = dms_cmm.var.ind.status.state;
						dsp_check_result.point[dsp_check_result.pointNum].fail = dms_cmm.fixed.ioattr.fail;
						dsp_check_result.point[dsp_check_result.pointNum].fault = dms_cmm.fixed.ioattr.fault;
						dsp_check_result.point[dsp_check_result.pointNum].almNoAck = dms_cmm.fixed.iost.almNoAck;
						dsp_check_result.pointNum++;
						Ret = 1;
					}
				}
			}
		}
	}
	if (Ret)
		WriteDspCheckResult(&dsp_check_result,crt_n);
	return Ret;
}

int WriteDspCheckResult(DSP_CHECK_RESULT *dsp_check_result,int crt_n)
{
	
	FILE *fp;
	char err_str[256];
	char filename[200];
	char tmpfilename[200];
	int i,len;
	gchar *pConvert=0;
	gchar *strBool[2]={"否","是"};
	getDspCheckName(tmpfilename);
	sprintf(filename, "%s%s.log", AUTOCHECK_DIR,tmpfilename);
	if(-1==access(AUTOCHECK_DIR, 0))
	{
#ifdef	WINDOWS_OS
		if(-1==mkdir(AUTOCHECK_DIR))
#else
		if(-1==mkdir(AUTOCHECK_DIR, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH))
#endif
		{
			sprintf(err_str, "Fail to create path %s", AUTOCHECK_DIR);
			Err_log(err_str);
			return -1;
		}
	}
	if( (fp=fopen(filename, "a"))==NULL )
	{
		sprintf(err_str, "Fail to open %s", filename);
		Err_log(err_str);
		return -1;
	}	
//	fseek( fp, 0, SEEK_END ); 
//	len = ftell( fp );
	fprintf(fp,"\t%s\t\t\t\t%s\t\t\t%s\t\t%s\t\t%s\n",_CS_(OIX_STR_REC_LOGNAME),_CS_(OIX_CSTR_RT_VALUE),_CS_(OIX_STR_FAULT),
			_CS_(OIX_STR_FAIL),_CS_(OIX_CSTR_AlarmNoAck));
	for (i=0;i<dsp_check_result->pointNum;i++)
	{
		switch(dsp_check_result->point[i].type)
		{
			case ANA_TYPE:
				fprintf(fp,"%s\t\t%f\t\t%s\t\t%s\t\t%s\n",dsp_check_result->point[i].pointName,dsp_check_result->point[i].realValue.fValue,
					strBool[dsp_check_result->point[i].fault],strBool[dsp_check_result->point[i].fail],strBool[dsp_check_result->point[i].almNoAck]);
				break;
			default:
				fprintf(fp,"%s\t\t%d\t\t\t%s\t\t%s\t\t%s\n",dsp_check_result->point[i].pointName,dsp_check_result->point[i].realValue.state,
					strBool[dsp_check_result->point[i].fault],strBool[dsp_check_result->point[i].fail],strBool[dsp_check_result->point[i].almNoAck]);
		}
	}
	fclose(fp);
//	PopupAutocheckFile(filename);
	return 0;
}
