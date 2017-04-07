/* IGC_GEN.c program
   cc -o IGC_GEN.dxe IGC_GEN.c /DBLIB/dblib.dlb
*/
#include "../../../pas/v4.0a/inc/IGC.h"

 
char		igc_def_file[IGC_LINE_BUF_CHAR];
char		igc_csoursefile[IGC_LINE_BUF_CHAR];

char		igc_debugpath[IGC_LINE_BUF_CHAR];
 
char		IGC_GEN_Err_file[IGC_LINE_BUF_CHAR];
char		IGC_filename[IGC_LINE_BUF_CHAR];
char		*IGC_GEN_Err_filename="IGC_GEN_Err.rec";

char		IGC_GEN_Global_variables_use_file[IGC_LINE_BUF_CHAR];
char		*IGC_GEN_Global_variables_use_filename="IGC_GEN_Global_variables_use.rec";
char		filebuf[IGC_LINE_BUF_CHAR];
char		copyoffilebuf[IGC_LINE_BUF_CHAR];
char		*filehead=	"BEGIN:";
char		*fileflag =	"INCLUDE";
char		*filetail=	"END";

char		*igc_filehead=	"intelligence-compute-begin:";
char		*igc_sec_start_type= "sec_start_type:";
char		*igc_filetail=	"intelligence-compute-end";



//20110908 add
int        use_r_times_I_floats[Ifloat+2];
int        use_r_times_I_doubles[Idouble+2];
int        use_r_times_I_ints[Iint+2];
int        use_r_times_I_uints[Iuint+2];
int        use_r_times_I_longs[Ilong+2];
int        use_r_times_I_ulongs[Iulong+2];
int        use_r_times_I_bools[Ibool+2];
int        use_r_times_I_shorts[Ishort+2];
int        use_r_times_I_ushorts[Iushort+2];
int        use_r_times_I_chars[Ichar+2];
int        use_r_times_I_uchars[Iuchar+2];
int        use_r_times_I_datetimes[Idatetime+2];
int        use_r_times_I_H9000_IGC_datas[IH9000_IGC_data+2];

int        use_r_times_I_H9000_IGC_IND_datas[IH9000_IGC_IND_data+2];
int        use_r_times_I_H9000_IGC_ANA_datas[IH9000_IGC_ANA_data+2];

int        use_r_times_I_H9000_timers[Itimer+2];
int        use_r_times_I_H9000_alarms[Ialarms+2];

int        use_w_times_I_floats[Ifloat+2];
int        use_w_times_I_doubles[Idouble+2];
int        use_w_times_I_ints[Iint+2];
int        use_w_times_I_uints[Iuint+2];
int        use_w_times_I_longs[Ilong+2];
int        use_w_times_I_ulongs[Iulong+2];
int        use_w_times_I_bools[Ibool+2];
int        use_w_times_I_shorts[Ishort+2];
int        use_w_times_I_ushorts[Iushort+2];
int        use_w_times_I_chars[Ichar+2];
int        use_w_times_I_uchars[Iuchar+2];
int        use_w_times_I_datetimes[Idatetime+2];
int        use_w_times_I_H9000_IGC_datas[IH9000_IGC_data+2];

int        use_w_times_I_H9000_IGC_IND_datas[IH9000_IGC_IND_data+2];
int        use_w_times_I_H9000_IGC_ANA_datas[IH9000_IGC_ANA_data+2];

int        use_w_times_I_H9000_timers[Itimer+2];
int        use_w_times_I_H9000_alarms[Ialarms+2];

#define     max_w_str_len  200
char        use_w_str_I_floats[Ifloat*2+1][max_w_str_len];
char        use_w_str_I_doubles[Idouble*2+1][max_w_str_len];
char        use_w_str_I_ints[Iint*2+1][max_w_str_len];
char        use_w_str_I_uints[Iuint*2+1][max_w_str_len];
char        use_w_str_I_longs[Ilong*2+1][max_w_str_len];
char        use_w_str_I_ulongs[Iulong*2+1][max_w_str_len];
char        use_w_str_I_bools[Ibool*2+1][max_w_str_len];
char        use_w_str_I_shorts[Ishort*2+1][max_w_str_len];
char        use_w_str_I_ushorts[Iushort*2+1][max_w_str_len];
char        use_w_str_I_chars[Ichar*2+1][max_w_str_len];
char        use_w_str_I_uchars[Iuchar*2+1][max_w_str_len];
char        use_w_str_I_datetimes[Idatetime*2+1][max_w_str_len];
char        use_w_str_I_H9000_IGC_datas[IH9000_IGC_data*2+1][max_w_str_len];

char        use_w_str_I_H9000_IGC_IND_datas[IH9000_IGC_IND_data*2+1][max_w_str_len];
char        use_w_str_I_H9000_IGC_ANA_datas[IH9000_IGC_ANA_data*2+1][max_w_str_len];

char        use_w_str_I_H9000_timers[Itimer*2+1][max_w_str_len];
char        use_w_str_I_H9000_alarms[Ialarms*2+1][max_w_str_len];

int searchgive_falg_0(char * str,int * R,char * strin0,
					   char * strin1,char * strin2,char * strin3,char * strin4,char * strin5,
					   char * strin6,char * strin7,char * strin8,char * strin9,char * strin10,
					   char * strin11,char * strin12,char * strin13,char * strin14,char * strin15,
					   char * strin16,char * strin17,char * strin18,char * strin19,char * strin20,
					   char * strin21,char * strin22,char * strin23,char * strin24,char * strin25,
					   char * strin26,char * strin27,char * strin28,char * strin29,char * strin30,char * strin31)
{
	
	int i=-1;
	int len=0;
	int F=0,G=0,K=0;

	if(str[0]==' ')
		str=str+1;
	if(i<0)
	{
		len=strlen(strin0);
		for(i=0;i<len;i++)
		{
			if(str[i]!=strin0[i])
			{
				i=-1;
				break;
			}	
		}
	}
	if(i<0)
	{
		len=strlen(strin1);
		for(i=0;i<len;i++)
		{
			if(str[i]!=strin1[i])
			{
				i=-1;
				break;
			}	
		}
	}
	if(i<0)
	{
		len=strlen(strin2);
		for(i=0;i<len;i++)
		{
			if(str[i]!=strin2[i])
			{
				i=-1;
				break;
			}	
		}
	}

	if(i<0)
	{
		len=strlen(strin3);
		for(i=0;i<len;i++)
		{
			if(str[i]!=strin3[i])
			{
				i=-1;
				break;
			}	
		}
	}

	if(i<0)
	{
		len=strlen(strin4);
		for(i=0;i<len;i++)
		{
			if(str[i]!=strin4[i])
			{
				i=-1;
				break;
			}	
		}
	}

	
	if(i<0)
	{
		len=strlen(strin5);
		for(i=0;i<len;i++)
		{
			if(str[i]!=strin5[i])
			{
				i=-1;
				break;
			}	
		}
	}

	if(i<0)
	{
		len=strlen(strin6);
		for(i=0;i<len;i++)
		{
			if(str[i]!=strin6[i])
			{
				i=-1;
				break;
			}	
		}
	}
	if(i<0)
	{
		len=strlen(strin7);
		for(i=0;i<len;i++)
		{
			if(str[i]!=strin7[i])
			{
				i=-1;
				break;
			}	
		}
	}

	if(i<0)
	{
		len=strlen(strin8);
		for(i=0;i<len;i++)
		{
			if(str[i]!=strin8[i])
			{
				i=-1;
				break;
			}	
		}
	}
	
	if(i<0)
	{
		len=strlen(strin9);
		for(i=0;i<len;i++)
		{
			if(str[i]!=strin9[i])
			{
				i=-1;
				break;
			}	
		}
	}
	if(i<0)
	{
		len=strlen(strin10);
		for(i=0;i<len;i++)
		{
			if(str[i]!=strin10[i])
			{
				i=-1;
				break;
			}	
		}
	}
	if(i<0)
	{
		len=strlen(strin11);
		for(i=0;i<len;i++)
		{
			if(str[i]!=strin11[i])
			{
				i=-1;
				break;
			}	
		}
	}
	if(i<0)
	{
		len=strlen(strin12);
		for(i=0;i<len;i++)
		{
			if(str[i]!=strin12[i])
			{
				i=-1;
				break;
			}	
		}
	}

	if(i<0)
	{
		len=strlen(strin13);
		for(i=0;i<len;i++)
		{
			if(str[i]!=strin13[i])
			{
				i=-1;
				break;
			}	
		}
	}

	if(i<0)
	{
		len=strlen(strin14);
		for(i=0;i<len;i++)
		{
			if(str[i]!=strin14[i])
			{
				i=-1;
				break;
			}	
		}
	}

	
	if(i<0)
	{
		len=strlen(strin15);
		for(i=0;i<len;i++)
		{
			if(str[i]!=strin15[i])
			{
				i=-1;
				break;
			}	
		}
	}

	if(i<0)
	{
		len=strlen(strin16);
		for(i=0;i<len;i++)
		{
			if(str[i]!=strin16[i])
			{
				i=-1;
				break;
			}	
		}
	}
	if(i<0)
	{
		len=strlen(strin17);
		for(i=0;i<len;i++)
		{
			if(str[i]!=strin17[i])
			{
				i=-1;
				break;
			}	
		}
	}

	if(i<0)
	{
		len=strlen(strin18);
		for(i=0;i<len;i++)
		{
			if(str[i]!=strin18[i])
			{
				i=-1;
				break;
			}	
		}
	}
	
	if(i<0)
	{
		len=strlen(strin19);
		for(i=0;i<len;i++)
		{
			if(str[i]!=strin19[i])
			{
				i=-1;
				break;
			}	
		}
	}
	
	if(i<0)
	{
		len=strlen(strin20);
		for(i=0;i<len;i++)
		{
			if(str[i]!=strin20[i])
			{
				i=-1;
				break;
			}	
		}
	}

	if(i<0)
	{
		len=strlen(strin21);
		for(i=0;i<len;i++)
		{
			if(str[i]!=strin21[i])
			{
				i=-1;
				break;
			}	
		}
	}
	if(i<0)
	{
		len=strlen(strin22);
		for(i=0;i<len;i++)
		{
			if(str[i]!=strin22[i])
			{
				i=-1;
				break;
			}	
		}
	}

	if(i<0)
	{
		len=strlen(strin23);
		for(i=0;i<len;i++)
		{
			if(str[i]!=strin23[i])
			{
				i=-1;
				break;
			}	
		}
	}

	if(i<0)
	{
		len=strlen(strin24);
		for(i=0;i<len;i++)
		{
			if(str[i]!=strin24[i])
			{
				i=-1;
				break;
			}	
		}
	}

	
	if(i<0)
	{
		len=strlen(strin25);
		for(i=0;i<len;i++)
		{
			if(str[i]!=strin25[i])
			{
				i=-1;
				break;
			}	
		}
	}

	if(i<0)
	{
		len=strlen(strin26);
		for(i=0;i<len;i++)
		{
			if(str[i]!=strin26[i])
			{
				i=-1;
				break;
			}	
		}
	}
	if(i<0)
	{
		len=strlen(strin27);
		for(i=0;i<len;i++)
		{
			if(str[i]!=strin27[i])
			{
				i=-1;
				break;
			}	
		}
	}

	if(i<0)
	{
		len=strlen(strin28);
		for(i=0;i<len;i++)
		{
			if(str[i]!=strin28[i])
			{
				i=-1;
				break;
			}	
		}
	}
	
	if(i<0)
	{
		len=strlen(strin29);
		for(i=0;i<len;i++)
		{
			if(str[i]!=strin29[i])
			{
				i=-1;
				break;
			}	
		}
	}
	if(i<0)
	{
		len=strlen(strin30);
		for(i=0;i<len;i++)
		{
			if(str[i]!=strin30[i])
			{
				i=-1;
				break;
			}	
		}
	}

	if(i<0)
	{
		len=strlen(strin31);
		for(i=0;i<len;i++)
		{
			if(str[i]!=strin31[i])
			{
				i=-1;
				break;
			}	
		}
	}

	if(i<0)
	{
		* R=0;
		return 0;
	}
	else
	{
		* R=len;
		for(i=0;i<len;i++)
		{
			str[i]=' ';
		}
        
		//
		F=0;
		G=0;
		K=0;
		i--;
		while(1)
		{
			i++;
			if(i>=IGC_LINE_BUF_CHAR-5)
				break;
			if(str[i]==0)
				break;

			if(G==1)
				break;
			if(str[i]=='[')
			{
				K=i;
				F=1;;
			}

			if(F==0)
			{
				if(str[i]!=' ')
					break;
			}
			else
			{
				if(str[i]!=']')
				{
					continue;
				}
				else
				{
					F=i;
					for(i=K;i<=F;i++)
					{
						str[i]=' ';
					}
					
					G=1;
					break;
				}
			}
		}


		return 1;
	}
}
void searchgivefalg(char * str,int index,int maxlen,int * flag,int Findex)
{
	int i=0,j=0;
	*flag=0;
 
	for(i=index;i<maxlen-1-index;i++)
	{
		if(str[i]=='=' && str[i+1]!='=')
		{
			*flag=1;
			break;
		}
		if(str[i]=='+' && str[i+1]=='=')
		{
			*flag=1;
			break;
		}
		if(str[i]=='-' && str[i+1]=='=')
		{
			*flag=1;
			break;
		}
		if(str[i]=='*' && str[i+1]=='=')
		{
			*flag=1;
			break;
		}
		if(str[i]=='-' && str[i+1]=='=')
		{
			*flag=1;
			break;
		}
		if(str[i]=='-' && str[i+1]=='-')
		{
			*flag=1;
			break;
		}
		if(str[i]=='+' && str[i+1]=='+')
		{
			*flag=1;
			break;
		}
		if(str[i]==0)
			break;
		if(str[i]>32)
			break;
	}
	
	if(*flag==0)
	{
		if(Findex<0)
			Findex=0-Findex;
	
		Findex--;
		for(i=Findex;i>0;i--)
		{
			if(str[i]=='+' && str[i-1]=='+')
			{
				*flag=1;
				break;
			}	
			if(str[i]=='-' && str[i-1]=='-')
			{
				*flag=1;
				break;
			}	
			if(str[i]==0)
				break;
			if(str[i]>32)
			break;
		}
		
	}
}

int main(int argc, char *argv[])
{  	
	FILE *IGC_fpdef,*IGC_fsc,*IGC_ferr,*IGC_fp,*IGC_globalvariables_fp;
	int  IGC_fpdef_ok=1,IGC_fsc_ok=1,IGC_ferr_ok=1,IGC_globalvariables_ok=1;
	int  IGC_file_def_linenum=0;
	int  Ffautl=0;
	int  n=0;
	int  i=0;
	int  Tj=0;
	int  Tn=0;
 
	char read_buf[IGC_LINE_BUF_CHAR];
	char globalnamecheck_buf[IGC_LINE_BUF_CHAR];
	char * globalnamecheck_buf_0;
	char globalnamecheck_1_buf[IGC_LINE_BUF_CHAR/4];
	int globalnamecheck_i,globalnamecheck_j,globalnamecheck_k,globalnamecheck_s,globalnamecheck_F,globalnamecheck_N,globalnamecheck_R;



	int  csoursefile_inited=0;
	int  null_line_num=0;
//	char san_char[1];
	int  initeathincfile=0;
	char incfuc_buf[2002][100];
	char igc_sec_start_type_buf[2002][100];
	char sec_start_types[2002];
	int  sec_start_timenum[2002];
	char sec_start_timetype[2002];
	char sec_start_types_reged[2002];
	char sec_start_types_month[2002];
	char sec_start_types_day[2002];
	char sec_start_types_hour[2002];
	char sec_start_types_minute[2002];
	char sec_start_types_second[2002];

	int  incfilenum=0;


	for(i=0;i<2002;i++)
	{
		memset(&incfuc_buf[i],0,100);
		memset(&igc_sec_start_type_buf[i],0,100);
		sec_start_types[i]=0;
		sec_start_timenum[i]=5;
		sec_start_timetype[i]=unit_second;
		sec_start_types_reged[i]=0;
		sec_start_types_month[i]=0;
		sec_start_types_day[i]=0;
		sec_start_types_hour[i]=0;
		sec_start_types_minute[i]=0;
		sec_start_types_second[i]=0;
		h9000_igc_cmd_num_sysuse[i]=-1;
		h9000_igc_char_sysuse[0]=0;
	}

	for(i=0;i<Ifloat*2+1;i++)
	{
		if(i<Ifloat+2)
		{
			use_r_times_I_floats[i]=0;
			use_w_times_I_floats[i]=0;
		}
		memset(use_w_str_I_floats[i],0,max_w_str_len);
	}
	for(i=0;i<Idouble*2+1;i++)
	{
		if(i<Idouble+2)
		{
			use_r_times_I_doubles[i]=0;
			use_w_times_I_doubles[i]=0;
		}
		memset(use_w_str_I_doubles[i],0,max_w_str_len);
	}
	for(i=0;i<Iint*2+1;i++)
	{
		if(i<Iint+2)
		{
			use_r_times_I_ints[i]=0;
			use_w_times_I_ints[i]=0;
		}
		memset(use_w_str_I_ints[i],0,max_w_str_len);
	}
	for(i=0;i<Iuint*2+1;i++)
	{
		if(i<Iuint+2)
		{
			use_r_times_I_uints[i]=0;
			use_w_times_I_uints[i]=0;
		}
		memset(use_w_str_I_uints[i],0,max_w_str_len);
	}
	
	for(i=0;i<Ilong*2+1;i++)
	{
		if(i<Ilong+2)
		{
			use_r_times_I_longs[i]=0;
			use_w_times_I_longs[i]=0;
		}
		memset(use_w_str_I_longs[i],0,max_w_str_len);
	}
	
	for(i=0;i<Iulong*2+1;i++)
	{
		if(i<Iulong+2)
		{
			use_r_times_I_ulongs[i]=0;
			use_w_times_I_ulongs[i]=0;
		}
		memset(use_w_str_I_ulongs[i],0,max_w_str_len);
	}
	
	for(i=0;i<Ibool*2+1;i++)
	{
		if(i<Ibool+2)
		{
			use_r_times_I_bools[i]=0;
			use_w_times_I_bools[i]=0;
		}
		memset(use_w_str_I_bools[i],0,max_w_str_len);
	}
	
	for(i=0;i<Ishort*2+1;i++)
	{
		if(i<Ishort+2)
		{
			use_r_times_I_shorts[i]=0;
			use_w_times_I_shorts[i]=0;
		}
		memset(use_w_str_I_shorts[i],0,max_w_str_len);
	}
	for(i=0;i<Iushort*2+1;i++)
	{
		if(i<Iushort+2)
		{
			use_r_times_I_ushorts[i]=0;
			use_w_times_I_ushorts[i]=0;
		}
		memset(use_w_str_I_ushorts[i],0,max_w_str_len);
	}
	
	for(i=0;i<Ichar*2+1;i++)
	{
		if(i<Ichar+2)
		{
			use_r_times_I_chars[i]=0;
			use_w_times_I_chars[i]=0;
		}
		memset(use_w_str_I_chars[i],0,max_w_str_len);
	}
	
	for(i=0;i<Iuchar*2+1;i++)
	{
		if(i<Iuchar+2)
		{
			use_r_times_I_uchars[i]=0;
			use_w_times_I_uchars[i]=0;
		}
		memset(use_w_str_I_uchars[i],0,max_w_str_len);
	}
	
	for(i=0;i<Idatetime*2+1;i++)
	{
		if(i<Idatetime+2)
		{
			use_r_times_I_datetimes[i]=0;
			use_w_times_I_datetimes[i]=0;
		}
		memset(use_w_str_I_datetimes[i],0,max_w_str_len);
	}
	
	for(i=0;i<IH9000_IGC_data*2+1;i++)
	{
		if(i<IH9000_IGC_data+2)
		{
			use_r_times_I_H9000_IGC_datas[i]=0;
			use_w_times_I_H9000_IGC_datas[i]=0;
		}
		memset(use_w_str_I_H9000_IGC_datas[i],0,max_w_str_len);
	}
	
	for(i=0;i<IH9000_IGC_IND_data*2+1;i++)
	{
		if(i<IH9000_IGC_IND_data+2)
		{
			use_r_times_I_H9000_IGC_IND_datas[i]=0;
			use_w_times_I_H9000_IGC_IND_datas[i]=0;
			memset(use_w_str_I_H9000_IGC_IND_datas[i],0,max_w_str_len);
		}
	}	
	for(i=0;i<IH9000_IGC_ANA_data*2+1;i++)
	{
		if(i<IH9000_IGC_ANA_data+2)
		{
			use_r_times_I_H9000_IGC_ANA_datas[i]=0;
			use_w_times_I_H9000_IGC_ANA_datas[i]=0;
		}
		memset(use_w_str_I_H9000_IGC_ANA_datas[i],0,max_w_str_len);
	}
	
	
	for(i=0;i<Itimer*2+1;i++)
	{
		if(i<Itimer+2)
		{
			use_r_times_I_H9000_timers[i]=0;
			use_w_times_I_H9000_timers[i]=0;
		}
		memset(use_w_str_I_H9000_timers[i],0,max_w_str_len);
	}
	for(i=0;i<Ialarms*2+1;i++)
	{
		if(i<Ialarms+2)
		{
			use_r_times_I_H9000_alarms[i]=0;
			use_w_times_I_H9000_alarms[i]=0;
		}
		memset(use_w_str_I_H9000_alarms[i],0,max_w_str_len);
	}
	

  /*	if(-1==igc_MountDB())
	{
		//Err_log("Fail to Mount RDB");
		exit(-1);
	}*/
	sprintf(igc_def_file, "%s%s", SYS_DEFINE_PATH, IGC_DEF_FILE);
	sprintf(igc_csoursefile, "%s%s", IGC_CSOURTHPATH, IGC_CSOURTHFINENAME);
	sprintf(IGC_GEN_Err_file, "%s%s", SYS_DEFINE_PATH, IGC_GEN_Err_filename);
	sprintf(IGC_GEN_Global_variables_use_file, "%s%s", SYS_DEFINE_PATH, IGC_GEN_Global_variables_use_filename);
 
	
	if((IGC_fpdef=fopen(igc_def_file,"r"))==NULL)
	{
		printf("igc.c:can't open(w) the igc_def_file  file %s!\n",igc_def_file);
		fclose(IGC_fpdef);
		IGC_fpdef_ok=0;
	}

	if(IGC_fpdef_ok==1)
	{
		
		if((IGC_fsc=fopen(igc_csoursefile,"w"))==NULL)
		{
			printf("igc.c:can't open(w) the igc_csoursefile file %s!\n",igc_csoursefile);
			fclose(IGC_fsc);
			IGC_fsc_ok=0;
		}
	}
	
	if(IGC_fpdef_ok==1 && IGC_fsc_ok==1)
	{
		if((IGC_ferr=fopen(IGC_GEN_Err_file,"w"))==NULL)
		{
			printf("igc.c:can't open(w) the error record file %s!\n",IGC_GEN_Err_file);
			fclose(IGC_ferr);
			IGC_ferr_ok=0;
		}
	}

	if(IGC_fpdef_ok==1 && IGC_fsc_ok==1 && IGC_ferr_ok==1)
	{
		if((IGC_globalvariables_fp=fopen(IGC_GEN_Global_variables_use_file,"w"))==NULL)
		{
			printf("igc.c:can't open(w) the Global variables use file record file %s!\n",IGC_GEN_Err_file);
			fclose(IGC_globalvariables_fp);
			IGC_globalvariables_ok=0;
		}
	}
 
	if(IGC_fpdef_ok==1 && IGC_fsc_ok==1 && IGC_ferr_ok==1 && IGC_globalvariables_ok==1)
	{
		fprintf (IGC_fpdef,"  ");
		fprintf (IGC_fsc,"  ");
		fprintf (IGC_ferr,"  ");
		
		
        IGC_file_def_linenum=0;
		Ffautl=0;
		
		
		
		
		if(csoursefile_inited==0)
		{
			fprintf (IGC_fsc,"/* igc.c program\n");
			fprintf (IGC_fsc,"    cc -o IGC.dxe igc.c /DBLIB/dblib.dlb auto gen\n");
			fprintf (IGC_fsc,"    20110718 chenxs */\n\n");
			
			fprintf (IGC_fsc,"#include \"../../../pas/v4.0a/inc/IGC.h\" \n");
			
			fprintf (IGC_fsc,"\n");	
			fprintf (IGC_fsc,"int init()\n");
			fprintf (IGC_fsc,"{\n");
			fprintf (IGC_fsc,"	int i=0;\n");
			fprintf (IGC_fsc,"	lacal_datetime_now=get_lacal_datetime_now();\n");
			
			fprintf (IGC_fsc,"	run_count=0;\n");
			fprintf (IGC_fsc,"	run_count_for_avg=0;\n");
			fprintf (IGC_fsc,"	all_run_millisecond=0;\n");
			fprintf (IGC_fsc,"	last_run_millisecond=0;\n");
			fprintf (IGC_fsc,"	avg_run_millisecond=0;\n");
			fprintf (IGC_fsc,"	max_run_millisecond=0;\n");
			fprintf (IGC_fsc,"	min_run_millisecond=0xFFFFFFFF;\n");
			fprintf (IGC_fsc,"	igc_CLK_TCK=get_CLK_TCK();\n");
			
			
			fprintf (IGC_fsc,"	for(i=0;i<Ifloat;i++)\n");
			fprintf (IGC_fsc,"		I_floats[i]=0;\n");
			fprintf (IGC_fsc,"	for(i=0;i<Idouble;i++)\n");
			fprintf (IGC_fsc,"		I_doubles[i]=0;\n");
			fprintf (IGC_fsc,"	for(i=0;i<Iuint;i++)\n");
			fprintf (IGC_fsc,"		I_uints[i]=0;\n");
			fprintf (IGC_fsc,"	for(i=0;i<Ilong;i++)\n");
			fprintf (IGC_fsc,"		I_longs[i]=0;\n");
			fprintf (IGC_fsc,"	for(i=0;i<Iulong;i++)\n");
			fprintf (IGC_fsc,"		I_ulongs[i]=0;\n");
			fprintf (IGC_fsc,"	for(i=0;i<Ibool;i++)\n");
			fprintf (IGC_fsc,"		I_bools[i]=0;\n");
			fprintf (IGC_fsc,"	for(i=0;i<Ishort;i++)\n");
			fprintf (IGC_fsc,"		I_shorts[i]=0;\n");
			fprintf (IGC_fsc,"	for(i=0;i<Iushort;i++)\n");
			fprintf (IGC_fsc,"		I_ushorts[i]=0;\n");
			fprintf (IGC_fsc,"	for(i=0;i<Ichar;i++)\n");
			fprintf (IGC_fsc,"		I_chars[i]=0;\n");
			
			fprintf (IGC_fsc,"	for(i=0;i<Iuchar;i++)\n");
			fprintf (IGC_fsc,"		I_uchars[i]=0;\n");
			fprintf (IGC_fsc,"	for(i=0;i<Idatetime;i++)\n");
			fprintf (IGC_fsc,"		I_datetimes[i]=get_lacal_datetime_now();\n");
			fprintf (IGC_fsc,"	for(i=0;i<IH9000_IGC_data;i++)\n");
			fprintf (IGC_fsc,"	{\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_datas[i].value=0;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_datas[i].ok_flag=0;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_datas[i].chan_fail=1;\n");
			
			fprintf (IGC_fsc,"		I_H9000_IGC_datas[i].change_time.day=1;\n");
			
			fprintf (IGC_fsc,"		I_H9000_IGC_datas[i].change_time.hour=1;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_datas[i].change_time.minute=1;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_datas[i].change_time.month=1;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_datas[i].change_time.second=1;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_datas[i].change_time.year=1;\n");
			
			fprintf (IGC_fsc,"		I_H9000_IGC_datas[i].update_time.day=1;\n");
			
			fprintf (IGC_fsc,"		I_H9000_IGC_datas[i].update_time.hour=1;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_datas[i].update_time.minute=1;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_datas[i].update_time.month=1;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_datas[i].update_time.second=1;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_datas[i].update_time.year=1;\n");
			
			fprintf (IGC_fsc,"	}\n");



			fprintf (IGC_fsc,"	for(i=0;i<IH9000_IGC_IND_data;i++)\n");
			fprintf (IGC_fsc,"	{\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_IND_datas[i].value=0;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_IND_datas[i].ok_flag=0;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_IND_datas[i].chan_fail=1;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_IND_datas[i].change_time.day=1;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_IND_datas[i].change_time.hour=1;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_IND_datas[i].change_time.minute=1;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_IND_datas[i].change_time.month=1;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_IND_datas[i].change_time.second=1;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_IND_datas[i].change_time.year=1;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_IND_datas[i].update_time.day=1;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_IND_datas[i].update_time.hour=1;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_IND_datas[i].update_time.minute=1;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_IND_datas[i].update_time.month=1;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_IND_datas[i].update_time.second=1;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_IND_datas[i].update_time.year=1;\n");
			fprintf (IGC_fsc,"	}\n");
			
			fprintf (IGC_fsc,"	for(i=0;i<IH9000_IGC_ANA_data;i++)\n");
			fprintf (IGC_fsc,"	{\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_ANA_datas[i].value=0;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_ANA_datas[i].ok_flag=0;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_ANA_datas[i].chan_fail=1;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_ANA_datas[i].change_time.day=1;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_ANA_datas[i].change_time.hour=1;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_ANA_datas[i].change_time.minute=1;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_ANA_datas[i].change_time.month=1;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_ANA_datas[i].change_time.second=1;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_ANA_datas[i].change_time.year=1;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_ANA_datas[i].update_time.day=1;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_ANA_datas[i].update_time.hour=1;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_ANA_datas[i].update_time.minute=1;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_ANA_datas[i].update_time.month=1;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_ANA_datas[i].update_time.second=1;\n");
			fprintf (IGC_fsc,"		I_H9000_IGC_ANA_datas[i].update_time.year=1;\n");
				
				
			fprintf (IGC_fsc,"		I_H9000_IGC_ANA_datas[i].D_BAND=1;\n"); 
			fprintf (IGC_fsc,"		I_H9000_IGC_ANA_datas[i].HH_LMT=0;\n");   
			fprintf (IGC_fsc,"		I_H9000_IGC_ANA_datas[i].H_LMT=0;\n");    
			fprintf (IGC_fsc,"		I_H9000_IGC_ANA_datas[i].L_LMT=0;\n");  
			fprintf (IGC_fsc,"		I_H9000_IGC_ANA_datas[i].LL_LMT=0;\n");  
			fprintf (IGC_fsc,"		I_H9000_IGC_ANA_datas[i].HR_LMT=0;\n");   
			fprintf (IGC_fsc,"		I_H9000_IGC_ANA_datas[i].LR_LMT=0;\n");  
			fprintf (IGC_fsc,"		I_H9000_IGC_ANA_datas[i].ENG_CVT=0;\n");	 
			fprintf (IGC_fsc,"		I_H9000_IGC_ANA_datas[i].HI_RANGE=0;\n");  
			fprintf (IGC_fsc,"		I_H9000_IGC_ANA_datas[i].LO_RANGE=0;\n");	 
			fprintf (IGC_fsc,"		I_H9000_IGC_ANA_datas[i].ZERO=0;\n");     
			fprintf (IGC_fsc,"		I_H9000_IGC_ANA_datas[i].COMPENSATE=0;\n"); 
			fprintf (IGC_fsc,"		I_H9000_IGC_ANA_datas[i].TR_LIM=0;\n"); 
			fprintf (IGC_fsc,"		I_H9000_IGC_ANA_datas[i].RT_LIM=0;\n");   
			fprintf (IGC_fsc,"	}\n");

			
			
			fprintf (IGC_fsc,"	for(i=0;i<Itimer;i++)\n");
			fprintf (IGC_fsc,"	{\n");
			fprintf (IGC_fsc,"		I_H9000_timers[i].autorestart=no;\n");
			fprintf (IGC_fsc,"		I_H9000_timers[i].full_count=600;\n");
			fprintf (IGC_fsc,"		I_H9000_timers[i].real_count=0;\n");
			fprintf (IGC_fsc,"	    I_H9000_timers[i].unit=unit_second;\n");
			fprintf (IGC_fsc,"		I_H9000_timers[i].enable=no;\n");
			fprintf (IGC_fsc,"		I_H9000_timers[i].reached_times=0;\n");
			fprintf (IGC_fsc,"		I_H9000_timers[i].inited=0;\n");
			
			
			fprintf (IGC_fsc,"		I_H9000_timers[i].lastdotime.day=lacal_datetime_now.day;\n");
			fprintf (IGC_fsc,"		I_H9000_timers[i].lastdotime.hour=lacal_datetime_now.hour;\n");
			fprintf (IGC_fsc,"		I_H9000_timers[i].lastdotime.minute=lacal_datetime_now.minute;\n");
			fprintf (IGC_fsc,"		I_H9000_timers[i].lastdotime.month=lacal_datetime_now.month;\n");
			fprintf (IGC_fsc,"		I_H9000_timers[i].lastdotime.second=lacal_datetime_now.second;\n");
			fprintf (IGC_fsc,"		I_H9000_timers[i].lastdotime.year=lacal_datetime_now.year;\n");
			
			fprintf (IGC_fsc,"	}\n");


			fprintf (IGC_fsc,"	for(i=0;i<Itimer_sysuse;i++)\n");
			fprintf (IGC_fsc,"	{\n");
			fprintf (IGC_fsc,"		h9000_igc_timeer_sysuse[i].autorestart=no;\n");
			fprintf (IGC_fsc,"		h9000_igc_timeer_sysuse[i].full_count=600;\n");
			fprintf (IGC_fsc,"		h9000_igc_timeer_sysuse[i].real_count=0;\n");
			fprintf (IGC_fsc,"	    h9000_igc_timeer_sysuse[i].unit=unit_second;\n");
			fprintf (IGC_fsc,"		h9000_igc_timeer_sysuse[i].enable=no;\n");
			fprintf (IGC_fsc,"		h9000_igc_timeer_sysuse[i].reached_times=0;\n");
			fprintf (IGC_fsc,"		h9000_igc_timeer_sysuse[i].inited=0;\n");
			
			
			fprintf (IGC_fsc,"		h9000_igc_timeer_sysuse[i].lastdotime.day=lacal_datetime_now.day;\n");
			fprintf (IGC_fsc,"		h9000_igc_timeer_sysuse[i].lastdotime.hour=lacal_datetime_now.hour;\n");
			fprintf (IGC_fsc,"		h9000_igc_timeer_sysuse[i].lastdotime.minute=lacal_datetime_now.minute;\n");
			fprintf (IGC_fsc,"		h9000_igc_timeer_sysuse[i].lastdotime.month=lacal_datetime_now.month;\n");
			fprintf (IGC_fsc,"		h9000_igc_timeer_sysuse[i].lastdotime.second=lacal_datetime_now.second;\n");
			fprintf (IGC_fsc,"		h9000_igc_timeer_sysuse[i].lastdotime.year=lacal_datetime_now.year;\n");
			
			fprintf (IGC_fsc,"		h9000_igc_char_sysuse[i]=0;\n");//20110905
			fprintf (IGC_fsc,"		h9000_igc_cmd_num_sysuse[i]=-1;\n");//20110905
			fprintf (IGC_fsc,"	}\n");

			
			
			fprintf (IGC_fsc,"	for(i=0;i<Ialarms;i++)\n");
			fprintf (IGC_fsc,"	{\n");
			
			fprintf (IGC_fsc,"	I_H9000_alarms[i].ok_flag=0;\n");
			fprintf (IGC_fsc,"	I_H9000_alarms[i].hostname[0]=0;\n");
			fprintf (IGC_fsc,"	I_H9000_alarms[i].logicname[0]=0;\n");
			fprintf (IGC_fsc,"	I_H9000_alarms[i].longname[0]=0;\n");
			fprintf (IGC_fsc,"	I_H9000_alarms[i].message[0]=0;\n");
			fprintf (IGC_fsc,"	I_H9000_alarms[i].repeatAlm[0]=0;\n");
			fprintf (IGC_fsc,"	I_H9000_alarms[i].sn_str[0]=0;\n");
			fprintf (IGC_fsc,"	I_H9000_alarms[i].state_str[0]=0;\n");
			fprintf (IGC_fsc,"	I_H9000_alarms[i].time_str[0]=0;\n");
			fprintf (IGC_fsc,"	I_H9000_alarms[i].usrname[0]=0;\n");
			
			
			fprintf (IGC_fsc,"	}\n");
			
			fprintf (IGC_fsc,"	return 1;\n");
			fprintf (IGC_fsc,"}\n");
			fprintf (IGC_fsc,"\n");		
			
			}
		while(1)
		{
			IGC_file_def_linenum++;
			if (fgets(filebuf,IGC_LINE_BUF_CHAR,IGC_fpdef)==NULL)
			{
				if (feof (IGC_fpdef))
				{
					Ffautl++;
					printf ("file %s exit without END\n",igc_def_file);
					fprintf (IGC_ferr,"file %s exit without END\n",igc_def_file);
				    break;
				}
				else
				{
					null_line_num++;
					if(null_line_num>100)
						break;
					else
					//Ffautl++;
					//fprintf (IGC_ferr,"Error! %3d line: is a null line.\n",IGC_file_def_linenum);
					continue;
				 
				}
			}

			n=0;
			while(filebuf[n]==' '||filebuf[n]==TAB)
			{
				n++;
			}

			if (filebuf[0]=='#')
				continue;

			if (filebuf[0]=='!')
				continue;

			if (filebuf[n]==LF || filebuf[n]==CR || filebuf[n]==0)
				continue;


			if (strncmp(filetail,&filebuf[n],strlen(filetail))==0)
			{
				printf ("\nFILE DEF END reacked");
				break;
			}
			if (strncmp(fileflag,&filebuf[n],strlen(fileflag)))
			{
				if (strncmp(filehead,&filebuf[n],strlen(filehead)))
				fprintf (IGC_ferr,"\n\tThe IGC def file %s line %d miss INCLUDE, %s Aborted!\n",igc_def_file,IGC_file_def_linenum,filebuf);
				continue;
			}
			n = n + strlen(fileflag);
			while(filebuf[n]==' '||filebuf[n]==TAB)
			{
				n++;
			}

			i=0;
			while(filebuf[n]!=LF && filebuf[n]!=CR && filebuf[n]!=0 
				&& filebuf[n]!=TAB&&filebuf[n]!=' '&& i<IGC_LINE_BUF_CHAR-1 && n<IGC_LINE_BUF_CHAR-1)
			{
				read_buf[i]=filebuf[n];
				i++;n++;
			}
			read_buf[i]=0;

			if (i==0)
			{
				fprintf (IGC_ferr,"\n\tThe IGC def file %s line %d No filename defined, %s Aborted!\n",igc_def_file,IGC_file_def_linenum,filebuf);
				continue;
			}
			sprintf(IGC_filename, "%s%s", SYS_DEFINE_PATH, read_buf);
			printf ("\n  :: Now Loading %s ...\n",IGC_filename);
			fprintf (IGC_ferr,"\n  :: Now Loading %s ...\n",IGC_filename);

			if((IGC_fp=fopen(IGC_filename,"r"))==NULL)
			{
				printf("IGC_GEN.c:can't open the file %s\n",IGC_filename);
				return -1;
			}
			
		 
			initeathincfile=0;
			while (1)
			{
				if(fgets(filebuf,IGC_LINE_BUF_CHAR,IGC_fp)==NULL)
				{
					if(feof(IGC_fp))
					{
						Ffautl++;
						printf("file %s exit without END\n",IGC_filename);
						break;
					}
					else
					{
						//Ffautl++;
						//fprintf(IGC_ferr,"Error!%3d line: is a null line.\n",r);
						continue;
					}
				}
			
				n = 0;
				while(filebuf[n]==' '||filebuf[n]==TAB)
				{
					n++;
				}
				
				if (strncmp(igc_filehead,&filebuf[n],strlen(igc_filehead))==0)
				{
					continue;
				}
				if (strncmp(igc_filetail,&filebuf[n],strlen(igc_filetail))==0)
				{
					printf ("\nSEC END reacked");
					break;
				}

				if (strstr(&filebuf[n],igc_sec_start_type))
				{
					 if(sec_start_types_reged[incfilenum]==1)//20110905 防止多次定义
						 continue;
					if(igc_sec_start_type_buf[incfilenum][0]==0)
					{
						sprintf(igc_sec_start_type_buf[incfilenum], "%s", &filebuf[n]);
						filebuf[n]=0;
						igc_sec_start_type_buf[incfilenum][99]=0;
						//continue;
					}
				}


				
				if (strstr(&filebuf[n],"h9000_igc_timeer_sysuse"))
				{
					printf ("\nIGC_GEN ERR,find h9000_igc_timeer_sysuse in %s",IGC_filename);
					fprintf (IGC_ferr,"IGC_GEN ERR,find h9000_igc_timeer_sysuse in %s\n",IGC_filename);
					break;
				}

				if (strstr(&filebuf[n],"Itimer_sysuse"))
				{
					printf ("\nIGC_GEN ERR,find Itimer_sysuse in %s",IGC_filename);
					fprintf (IGC_ferr,"IGC_GEN ERR,find Itimer_sysuse in %s\n",IGC_filename);
					break;
				}
 
				
				if (strstr(&filebuf[n],"h9000_igc_char_sysuse"))
				{
					printf ("\nIGC_GEN ERR,find h9000_igc_char_sysuse in %s",IGC_filename);
					fprintf (IGC_ferr,"IGC_GEN ERR,find h9000_igc_char_sysuse in %s\n",IGC_filename);
					break;
				}
				
				if (strstr(&filebuf[n],"Itimer_sysuse"))
				{
					printf ("\nIGC_GEN ERR,find Itimer_sysuse in %s",IGC_filename);
					fprintf (IGC_ferr,"IGC_GEN ERR,find Itimer_sysuse in %s\n",IGC_filename);
					break;
				}

				
				if (strstr(&filebuf[n],"h9000_igc_cmd_num_sysuse"))
				{
					printf ("\nIGC_GEN ERR,find h9000_igc_cmd_num_sysuse in %s",IGC_filename);
					fprintf (IGC_ferr,"IGC_GEN ERR,find h9000_igc_cmd_num_sysuse in %s\n",IGC_filename);
					break;
				}
				
				if (strstr(&filebuf[n],"Itimer_sysuse"))
				{
					printf ("\nIGC_GEN ERR,find Itimer_sysuse in %s",IGC_filename);
					fprintf (IGC_ferr,"IGC_GEN ERR,find Itimer_sysuse in %s\n",IGC_filename);
					break;
				}

				
				if (strstr(&filebuf[n],"sleep("))
				{
					printf ("\nIGC_GEN ERR,find Sleep( in %s",IGC_filename);
					fprintf (IGC_ferr,"IGC_GEN ERR,find sleep in %s\n",IGC_filename);
					break;
				}
				
				if (strstr(&filebuf[n],"Sleep("))
				{
					printf ("\nIGC_GEN ERR,find Sleep( in %s",IGC_filename);
					fprintf (IGC_ferr,"IGC_GEN ERR,find sleep in %s\n",IGC_filename);
					break;
				}
 
 
				if (strstr(&filebuf[n],"Sleep "))
				{
					printf ("\nIGC_GEN ERR,find Sleep( in %s",IGC_filename);
					fprintf (IGC_ferr,"IGC_GEN ERR,find sleep in %s\n",IGC_filename);
					break;
				}
				
				if (strstr(&filebuf[n],"sleep "))
				{
					printf ("\nIGC_GEN ERR,find Sleep( in %s",IGC_filename);
					fprintf (IGC_ferr,"IGC_GEN ERR,find sleep in %s\n",IGC_filename);
					break;
				}

				
				if (strstr(&filebuf[n],"memcpy"))
				{
					printf ("\nIGC_GEN ERR,find memcpy in %s",IGC_filename);
					fprintf (IGC_ferr,"IGC_GEN ERR,find memcpy in %s\n",IGC_filename);
					break;
				}
				
				if (strstr(&filebuf[n],"memset"))
				{
					printf ("\nIGC_GEN ERR,find memset( in %s",IGC_filename);
					fprintf (IGC_ferr,"IGC_GEN ERR,find memset in %s\n",IGC_filename);
					break;
				}

				if (strstr(&filebuf[n],"Malloc"))
				{
					printf ("\nIGC_GEN ERR,find Malloc( in %s",IGC_filename);
					fprintf (IGC_ferr,"IGC_GEN ERR,find memset in %s\n",IGC_filename);
					break;
				}

				
				if (strstr(&filebuf[n],"calloc"))
				{
					printf ("\nIGC_GEN ERR,find calloc( in %s",IGC_filename);
					fprintf (IGC_ferr,"IGC_GEN ERR,find memset in %s\n",IGC_filename);
					break;
				}

				
				if (strstr(&filebuf[n],"free"))
				{
					printf ("\nIGC_GEN ERR,find free( in %s",IGC_filename);
					fprintf (IGC_ferr,"IGC_GEN ERR,find memset in %s\n",IGC_filename);
					break;
				}
				
				if (strstr(&filebuf[n],"memcpy"))
				{
					printf ("\nIGC_GEN ERR,find memcpy( in %s",IGC_filename);
					fprintf (IGC_ferr,"IGC_GEN ERR,find memset in %s\n",IGC_filename);
					break;
				}
				
				if (strstr(&filebuf[n],"memmove"))
				{
					printf ("\nIGC_GEN ERR,find free( in %s",IGC_filename);
					fprintf (IGC_ferr,"IGC_GEN ERR,find memmove in %s\n",IGC_filename);
					break;
				}
				
				if (strstr(&filebuf[n],"memcmp"))
				{
					printf ("\nIGC_GEN ERR,find free( in %s",IGC_filename);
					fprintf (IGC_ferr,"IGC_GEN ERR,find memcmp in %s\n",IGC_filename);
					break;
				}
				
				
				if (strstr(&filebuf[n],"strstr"))
				{
					printf ("\nIGC_GEN ERR,find strstr( in %s",IGC_filename);
					fprintf (IGC_ferr,"IGC_GEN ERR,find strstr in %s\n",IGC_filename);
					break;
				}
				
				if (strstr(&filebuf[n],"strcpy"))
				{
					printf ("\nIGC_GEN ERR,find strcpy( in %s",IGC_filename);
					fprintf (IGC_ferr,"IGC_GEN ERR,find strstr in %s\n",IGC_filename);
					break;
				}
				
				if (strstr(&filebuf[n],"strncpy"))
				{
					printf ("\nIGC_GEN ERR,find strncpy( in %s",IGC_filename);
					fprintf (IGC_ferr,"IGC_GEN ERR,find strstr in %s\n",IGC_filename);
					break;
				}
				if (strstr(&filebuf[n],"strcat"))
				{
					printf ("\nIGC_GEN ERR,find strcat( in %s",IGC_filename);
					fprintf (IGC_ferr,"IGC_GEN ERR,find strstr in %s\n",IGC_filename);
					break;
				}	

				
				if (strstr(&filebuf[n],"strncat"))
				{
					printf ("\nIGC_GEN ERR,find strncat( in %s",IGC_filename);
					fprintf (IGC_ferr,"IGC_GEN ERR,find strstr in %s\n",IGC_filename);
					break;
				}
				
				
				if (strstr(&filebuf[n],"strcmp"))
				{
					printf ("\nIGC_GEN ERR,find strcmp( in %s",IGC_filename);
					fprintf (IGC_ferr,"IGC_GEN ERR,find strstr in %s\n",IGC_filename);
					break;
				}

				
				if (strstr(&filebuf[n],"strlen"))
				{
					printf ("\nIGC_GEN ERR,find strlen( in %s",IGC_filename);
					fprintf (IGC_ferr,"IGC_GEN ERR,find strstr in %s\n",IGC_filename);
					break;
				}
				//20110908 add
				memset(copyoffilebuf,0,IGC_LINE_BUF_CHAR);
				strcpy(copyoffilebuf,filebuf);
				//20110908 add
				memset(globalnamecheck_buf,0,IGC_LINE_BUF_CHAR);
				memset(globalnamecheck_1_buf,0,IGC_LINE_BUF_CHAR/4);
				sprintf(globalnamecheck_buf, "I_floats[");
				globalnamecheck_buf_0=strstr(&copyoffilebuf[n],globalnamecheck_buf);
				if (globalnamecheck_buf_0)
				{
					globalnamecheck_i=0;
					globalnamecheck_j=0;
					globalnamecheck_k=0;
					globalnamecheck_s=0;
					for(globalnamecheck_i=n;globalnamecheck_i<IGC_LINE_BUF_CHAR-n;globalnamecheck_i++)
					{
						if(globalnamecheck_j==0 && globalnamecheck_k==0)
						{ 
							if(copyoffilebuf[globalnamecheck_i]=='[')
							{
								globalnamecheck_j=1;
								copyoffilebuf[globalnamecheck_i]=' '; 
								globalnamecheck_s=globalnamecheck_i;  
								globalnamecheck_k=globalnamecheck_i;  
							}
						}
						else
						{
							if(copyoffilebuf[globalnamecheck_i]==']')
							{
								globalnamecheck_j=2;
								copyoffilebuf[globalnamecheck_i]=' ';

								if(globalnamecheck_1_buf[0]!=0)
								{
									globalnamecheck_1_buf[globalnamecheck_k]=0;

									for(globalnamecheck_j=globalnamecheck_s;globalnamecheck_j<globalnamecheck_k-globalnamecheck_s;globalnamecheck_j++)
									{
										if(globalnamecheck_1_buf[globalnamecheck_j]!=' ' 
											&& (globalnamecheck_1_buf[globalnamecheck_j] <48 || globalnamecheck_1_buf[globalnamecheck_j]>57))
										{
											globalnamecheck_j=-1;//not num
											break;
										}
									}

									if(globalnamecheck_j>=0)
									{
										if(sscanf(globalnamecheck_1_buf,"%d",&globalnamecheck_N))
										{
										   searchgivefalg(copyoffilebuf,globalnamecheck_i,IGC_LINE_BUF_CHAR,&globalnamecheck_F,copyoffilebuf-globalnamecheck_buf_0);
										}
										else
											globalnamecheck_j=-2;
									}

									if(globalnamecheck_j<0)
									{
										 searchgivefalg(copyoffilebuf,globalnamecheck_i,IGC_LINE_BUF_CHAR,&globalnamecheck_F,copyoffilebuf-globalnamecheck_buf_0);
									}

									if(globalnamecheck_F==1)//写标志
									{
										if(globalnamecheck_j<0)//非数字标志
										{
											if(use_w_str_I_floats[use_w_times_I_floats[Ifloat]][max_w_str_len-strlen(use_w_str_I_floats[Ifloat])-7]==0
												&& use_w_str_I_floats[use_w_times_I_floats[Ifloat]][max_w_str_len-strlen(use_w_str_I_floats[Ifloat])-8]==0)
											{
												strcat(use_w_str_I_floats[Ifloat],IGC_filename);
												strcat(use_w_str_I_floats[Ifloat],"\n    ");
											}
											use_w_times_I_floats[Ifloat]++;
										}
										else
										{
											if(use_w_str_I_floats[use_w_times_I_floats[globalnamecheck_N]][max_w_str_len-strlen(use_w_str_I_floats[globalnamecheck_N])-7]==0
												&& use_w_str_I_floats[use_w_times_I_floats[globalnamecheck_N]][max_w_str_len-strlen(use_w_str_I_floats[globalnamecheck_N])-8]==0)
											{
												strcat(use_w_str_I_floats[globalnamecheck_N],IGC_filename);
												strcat(use_w_str_I_floats[globalnamecheck_N],"\n    ");
											}
											use_w_times_I_floats[globalnamecheck_N]++;
										}
									}
									else
									{
										if(globalnamecheck_j<0)//非数字标志
										{
											use_r_times_I_floats[Ifloat]++;
										}
										else
										{
											use_r_times_I_floats[globalnamecheck_N]++;
										}
									}
								}
								globalnamecheck_j=0;
								globalnamecheck_k=0;
								globalnamecheck_s=0;
							}
							else
							{
								globalnamecheck_k++;
								if(globalnamecheck_k>globalnamecheck_s)
									globalnamecheck_1_buf[globalnamecheck_k-globalnamecheck_s-1]=copyoffilebuf[globalnamecheck_i];
								
							}
						}

					}
				}


				//20110908 add
				//20110908 add
				memset(globalnamecheck_buf,0,IGC_LINE_BUF_CHAR);
				memset(globalnamecheck_1_buf,0,IGC_LINE_BUF_CHAR/4);
				sprintf(globalnamecheck_buf, "I_doubles[");
				globalnamecheck_buf_0=strstr(&copyoffilebuf[n],globalnamecheck_buf);
				if (globalnamecheck_buf_0)
				{
					globalnamecheck_i=0;
					globalnamecheck_j=0;
					globalnamecheck_k=0;
					globalnamecheck_s=0;
					for(globalnamecheck_i=n;globalnamecheck_i<IGC_LINE_BUF_CHAR-n;globalnamecheck_i++)
					{
						if(globalnamecheck_j==0 && globalnamecheck_k==0)
						{ 
							if(copyoffilebuf[globalnamecheck_i]=='[')
							{
								globalnamecheck_j=1;
								copyoffilebuf[globalnamecheck_i]=' '; 
								globalnamecheck_s=globalnamecheck_i;  
								globalnamecheck_k=globalnamecheck_i;  
							}
						}
						else
						{
							if(copyoffilebuf[globalnamecheck_i]==']')
							{
								globalnamecheck_j=2;
								copyoffilebuf[globalnamecheck_i]=' ';

								if(globalnamecheck_1_buf[0]!=0)
								{
									globalnamecheck_1_buf[globalnamecheck_k]=0;

									for(globalnamecheck_j=globalnamecheck_s;globalnamecheck_j<globalnamecheck_k-globalnamecheck_s;globalnamecheck_j++)
									{
										if(globalnamecheck_1_buf[globalnamecheck_j]!=' ' 
											&& (globalnamecheck_1_buf[globalnamecheck_j] <48 || globalnamecheck_1_buf[globalnamecheck_j]>57))
										{
											globalnamecheck_j=-1;//not num
											break;
										}
									}

									if(globalnamecheck_j>=0)
									{
										if(sscanf(globalnamecheck_1_buf,"%d",&globalnamecheck_N))
										{
										   searchgivefalg(copyoffilebuf,globalnamecheck_i,IGC_LINE_BUF_CHAR,&globalnamecheck_F,copyoffilebuf-globalnamecheck_buf_0);
										}
										else
											globalnamecheck_j=-2;
									}

									if(globalnamecheck_j<0)
									{
										 searchgivefalg(copyoffilebuf,globalnamecheck_i,IGC_LINE_BUF_CHAR,&globalnamecheck_F,copyoffilebuf-globalnamecheck_buf_0);
									}

									if(globalnamecheck_F==1)//写标志
									{
										if(globalnamecheck_j<0)//非数字标志
										{
											if(use_w_str_I_doubles[use_w_times_I_doubles[Idouble]][max_w_str_len-strlen(use_w_str_I_doubles[Idouble])-7]==0
												&& use_w_str_I_doubles[use_w_times_I_doubles[Idouble]][max_w_str_len-strlen(use_w_str_I_doubles[Idouble])-8]==0)
											{
												strcat(use_w_str_I_doubles[Idouble],IGC_filename);
												strcat(use_w_str_I_doubles[Idouble],"\n    ");
											}
											use_w_times_I_doubles[Idouble]++;
										}
										else
										{
											if(use_w_str_I_doubles[use_w_times_I_doubles[globalnamecheck_N]][max_w_str_len-strlen(use_w_str_I_doubles[globalnamecheck_N])-7]==0
												&& use_w_str_I_doubles[use_w_times_I_doubles[globalnamecheck_N]][max_w_str_len-strlen(use_w_str_I_doubles[globalnamecheck_N])-8]==0)
											{
												strcat(use_w_str_I_doubles[globalnamecheck_N],IGC_filename);
												strcat(use_w_str_I_doubles[globalnamecheck_N],"\n    ");
											}
											use_w_times_I_doubles[globalnamecheck_N]++;
										}
									}
									else
									{
										if(globalnamecheck_j<0)//非数字标志
										{
											use_r_times_I_doubles[Idouble]++;
										}
										else
										{
											use_r_times_I_doubles[globalnamecheck_N]++;
										}
									}
								}
								globalnamecheck_j=0;
								globalnamecheck_k=0;
								globalnamecheck_s=0;
							}
							else
							{
								globalnamecheck_k++;
								if(globalnamecheck_k>globalnamecheck_s)
									globalnamecheck_1_buf[globalnamecheck_k-globalnamecheck_s-1]=copyoffilebuf[globalnamecheck_i];
								
							}
						}

					}
				}


				//20110908 add
								//20110908 add
				memset(globalnamecheck_buf,0,IGC_LINE_BUF_CHAR);
				memset(globalnamecheck_1_buf,0,IGC_LINE_BUF_CHAR/4);
				sprintf(globalnamecheck_buf, "I_ints[");
				globalnamecheck_buf_0=strstr(&copyoffilebuf[n],globalnamecheck_buf);
				if (globalnamecheck_buf_0)
				{
					globalnamecheck_i=0;
					globalnamecheck_j=0;
					globalnamecheck_k=0;
					globalnamecheck_s=0;
					for(globalnamecheck_i=n;globalnamecheck_i<IGC_LINE_BUF_CHAR-n;globalnamecheck_i++)
					{
						if(globalnamecheck_j==0 && globalnamecheck_k==0)
						{ 
							if(copyoffilebuf[globalnamecheck_i]=='[')
							{
								globalnamecheck_j=1;
								copyoffilebuf[globalnamecheck_i]=' '; 
								globalnamecheck_s=globalnamecheck_i;  
								globalnamecheck_k=globalnamecheck_i;  
							}
						}
						else
						{
							if(copyoffilebuf[globalnamecheck_i]==']')
							{
								globalnamecheck_j=2;
								copyoffilebuf[globalnamecheck_i]=' ';

								if(globalnamecheck_1_buf[0]!=0)
								{
									globalnamecheck_1_buf[globalnamecheck_k]=0;

									for(globalnamecheck_j=globalnamecheck_s;globalnamecheck_j<globalnamecheck_k-globalnamecheck_s;globalnamecheck_j++)
									{
										if(globalnamecheck_1_buf[globalnamecheck_j]!=' ' 
											&& (globalnamecheck_1_buf[globalnamecheck_j] <48 || globalnamecheck_1_buf[globalnamecheck_j]>57))
										{
											globalnamecheck_j=-1;//not num
											break;
										}
									}

									if(globalnamecheck_j>=0)
									{
										if(sscanf(globalnamecheck_1_buf,"%d",&globalnamecheck_N))
										{
										   searchgivefalg(copyoffilebuf,globalnamecheck_i,IGC_LINE_BUF_CHAR,&globalnamecheck_F,copyoffilebuf-globalnamecheck_buf_0);
										}
										else
											globalnamecheck_j=-2;
									}

									if(globalnamecheck_j<0)
									{
										 searchgivefalg(copyoffilebuf,globalnamecheck_i,IGC_LINE_BUF_CHAR,&globalnamecheck_F,copyoffilebuf-globalnamecheck_buf_0);
									}

									if(globalnamecheck_F==1)//写标志
									{
										if(globalnamecheck_j<0)//非数字标志
										{
											if(use_w_str_I_ints[use_w_times_I_ints[Iint]][max_w_str_len-strlen(use_w_str_I_ints[Iint])-7]==0
												&& use_w_str_I_ints[use_w_times_I_ints[Iint]][max_w_str_len-strlen(use_w_str_I_ints[Iint])-8]==0)
											{
												strcat(use_w_str_I_ints[Iint],IGC_filename);
												strcat(use_w_str_I_ints[Iint],"\n    ");
											}
											use_w_times_I_ints[Iint]++;
										}
										else
										{
											if(use_w_str_I_ints[use_w_times_I_ints[globalnamecheck_N]][max_w_str_len-strlen(use_w_str_I_ints[globalnamecheck_N])-7]==0
												&& use_w_str_I_ints[use_w_times_I_ints[globalnamecheck_N]][max_w_str_len-strlen(use_w_str_I_ints[globalnamecheck_N])-8]==0)
											{
												strcat(use_w_str_I_ints[globalnamecheck_N],IGC_filename);
												strcat(use_w_str_I_ints[globalnamecheck_N],"\n    ");
											}
											use_w_times_I_ints[globalnamecheck_N]++;
										}
									}
									else
									{
										if(globalnamecheck_j<0)//非数字标志
										{
											use_r_times_I_ints[Iint]++;
										}
										else
										{
											use_r_times_I_ints[globalnamecheck_N]++;
										}
									}
								}
								globalnamecheck_j=0;
								globalnamecheck_k=0;
								globalnamecheck_s=0;
							}
							else
							{
								globalnamecheck_k++;
								if(globalnamecheck_k>globalnamecheck_s)
									globalnamecheck_1_buf[globalnamecheck_k-globalnamecheck_s-1]=copyoffilebuf[globalnamecheck_i];
								
							}
						}

					}
				}


				//20110908 add
				//20110908 add
				memset(globalnamecheck_buf,0,IGC_LINE_BUF_CHAR);
				memset(globalnamecheck_1_buf,0,IGC_LINE_BUF_CHAR/4);
				sprintf(globalnamecheck_buf, "I_uints[");
				globalnamecheck_buf_0=strstr(&copyoffilebuf[n],globalnamecheck_buf);
				if (globalnamecheck_buf_0)
				{
					globalnamecheck_i=0;
					globalnamecheck_j=0;
					globalnamecheck_k=0;
					globalnamecheck_s=0;
					for(globalnamecheck_i=n;globalnamecheck_i<IGC_LINE_BUF_CHAR-n;globalnamecheck_i++)
					{
						if(globalnamecheck_j==0 && globalnamecheck_k==0)
						{ 
							if(copyoffilebuf[globalnamecheck_i]=='[')
							{
								globalnamecheck_j=1;
								copyoffilebuf[globalnamecheck_i]=' '; 
								globalnamecheck_s=globalnamecheck_i;  
								globalnamecheck_k=globalnamecheck_i;  
							}
						}
						else
						{
							if(copyoffilebuf[globalnamecheck_i]==']')
							{
								globalnamecheck_j=2;
								copyoffilebuf[globalnamecheck_i]=' ';

								if(globalnamecheck_1_buf[0]!=0)
								{
									globalnamecheck_1_buf[globalnamecheck_k]=0;

									for(globalnamecheck_j=globalnamecheck_s;globalnamecheck_j<globalnamecheck_k-globalnamecheck_s;globalnamecheck_j++)
									{
										if(globalnamecheck_1_buf[globalnamecheck_j]!=' ' 
											&& (globalnamecheck_1_buf[globalnamecheck_j] <48 || globalnamecheck_1_buf[globalnamecheck_j]>57))
										{
											globalnamecheck_j=-1;//not num
											break;
										}
									}

									if(globalnamecheck_j>=0)
									{
										if(sscanf(globalnamecheck_1_buf,"%d",&globalnamecheck_N))
										{
										   searchgivefalg(copyoffilebuf,globalnamecheck_i,IGC_LINE_BUF_CHAR,&globalnamecheck_F,copyoffilebuf-globalnamecheck_buf_0);
										}
										else
											globalnamecheck_j=-2;
									}

									if(globalnamecheck_j<0)
									{
										 searchgivefalg(copyoffilebuf,globalnamecheck_i,IGC_LINE_BUF_CHAR,&globalnamecheck_F,copyoffilebuf-globalnamecheck_buf_0);
									}

									if(globalnamecheck_F==1)//写标志
									{
										if(globalnamecheck_j<0)//非数字标志
										{
											if(use_w_str_I_uints[use_w_times_I_uints[Iuint]][max_w_str_len-strlen(use_w_str_I_uints[Iuint])-7]==0
												&& use_w_str_I_uints[use_w_times_I_uints[Iuint]][max_w_str_len-strlen(use_w_str_I_uints[Iuint])-8]==0)
											{
												strcat(use_w_str_I_uints[Iuint],IGC_filename);
												strcat(use_w_str_I_uints[Iuint],"\n    ");
											}
											use_w_times_I_uints[Iuint]++;
										}
										else
										{
											if(use_w_str_I_uints[use_w_times_I_uints[globalnamecheck_N]][max_w_str_len-strlen(use_w_str_I_uints[globalnamecheck_N])-7]==0
												&& use_w_str_I_uints[use_w_times_I_uints[globalnamecheck_N]][max_w_str_len-strlen(use_w_str_I_uints[globalnamecheck_N])-8]==0)
											{
												strcat(use_w_str_I_uints[globalnamecheck_N],IGC_filename);
												strcat(use_w_str_I_uints[globalnamecheck_N],"\n    ");
											}
											use_w_times_I_uints[globalnamecheck_N]++;
										}
									}
									else
									{
										if(globalnamecheck_j<0)//非数字标志
										{
											use_r_times_I_uints[Iuint]++;
										}
										else
										{
											use_r_times_I_uints[globalnamecheck_N]++;
										}
									}
								}
								globalnamecheck_j=0;
								globalnamecheck_k=0;
								globalnamecheck_s=0;
							}
							else
							{
								globalnamecheck_k++;
								if(globalnamecheck_k>globalnamecheck_s)
									globalnamecheck_1_buf[globalnamecheck_k-globalnamecheck_s-1]=copyoffilebuf[globalnamecheck_i];
								
							}
						}

					}
				}


				//20110908 add
				//20110908 add
				memset(globalnamecheck_buf,0,IGC_LINE_BUF_CHAR);
				memset(globalnamecheck_1_buf,0,IGC_LINE_BUF_CHAR/4);
				sprintf(globalnamecheck_buf, "I_longs[");
				globalnamecheck_buf_0=strstr(&copyoffilebuf[n],globalnamecheck_buf);
				if (globalnamecheck_buf_0)
				{
					globalnamecheck_i=0;
					globalnamecheck_j=0;
					globalnamecheck_k=0;
					globalnamecheck_s=0;
					for(globalnamecheck_i=n;globalnamecheck_i<IGC_LINE_BUF_CHAR-n;globalnamecheck_i++)
					{
						if(globalnamecheck_j==0 && globalnamecheck_k==0)
						{ 
							if(copyoffilebuf[globalnamecheck_i]=='[')
							{
								globalnamecheck_j=1;
								copyoffilebuf[globalnamecheck_i]=' '; 
								globalnamecheck_s=globalnamecheck_i;  
								globalnamecheck_k=globalnamecheck_i;  
							}
						}
						else
						{
							if(copyoffilebuf[globalnamecheck_i]==']')
							{
								globalnamecheck_j=2;
								copyoffilebuf[globalnamecheck_i]=' ';

								if(globalnamecheck_1_buf[0]!=0)
								{
									globalnamecheck_1_buf[globalnamecheck_k]=0;

									for(globalnamecheck_j=globalnamecheck_s;globalnamecheck_j<globalnamecheck_k-globalnamecheck_s;globalnamecheck_j++)
									{
										if(globalnamecheck_1_buf[globalnamecheck_j]!=' ' 
											&& (globalnamecheck_1_buf[globalnamecheck_j] <48 || globalnamecheck_1_buf[globalnamecheck_j]>57))
										{
											globalnamecheck_j=-1;//not num
											break;
										}
									}

									if(globalnamecheck_j>=0)
									{
										if(sscanf(globalnamecheck_1_buf,"%d",&globalnamecheck_N))
										{
										   searchgivefalg(copyoffilebuf,globalnamecheck_i,IGC_LINE_BUF_CHAR,&globalnamecheck_F,copyoffilebuf-globalnamecheck_buf_0);
										}
										else
											globalnamecheck_j=-2;
									}

									if(globalnamecheck_j<0)
									{
										 searchgivefalg(copyoffilebuf,globalnamecheck_i,IGC_LINE_BUF_CHAR,&globalnamecheck_F,copyoffilebuf-globalnamecheck_buf_0);
									}

									if(globalnamecheck_F==1)//写标志
									{
										if(globalnamecheck_j<0)//非数字标志
										{
											if(use_w_str_I_longs[use_w_times_I_longs[Ilong]][max_w_str_len-strlen(use_w_str_I_longs[Ilong])-7]==0
												&& use_w_str_I_longs[use_w_times_I_longs[Ilong]][max_w_str_len-strlen(use_w_str_I_longs[Ilong])-8]==0)
											{
												strcat(use_w_str_I_longs[Ilong],IGC_filename);
												strcat(use_w_str_I_longs[Ilong],"\n    ");
											}
											use_w_times_I_longs[Ilong]++;
										}
										else
										{
											if(use_w_str_I_longs[use_w_times_I_longs[globalnamecheck_N]][max_w_str_len-strlen(use_w_str_I_longs[globalnamecheck_N])-7]==0
												&& use_w_str_I_longs[use_w_times_I_longs[globalnamecheck_N]][max_w_str_len-strlen(use_w_str_I_longs[globalnamecheck_N])-8]==0)
											{
												strcat(use_w_str_I_longs[globalnamecheck_N],IGC_filename);
												strcat(use_w_str_I_longs[globalnamecheck_N],"\n    ");
											}
											use_w_times_I_longs[globalnamecheck_N]++;
										}
									}
									else
									{
										if(globalnamecheck_j<0)//非数字标志
										{
											use_r_times_I_longs[Ilong]++;
										}
										else
										{
											use_r_times_I_longs[globalnamecheck_N]++;
										}
									}
								}
								globalnamecheck_j=0;
								globalnamecheck_k=0;
								globalnamecheck_s=0;
							}
							else
							{
								globalnamecheck_k++;
								if(globalnamecheck_k>globalnamecheck_s)
									globalnamecheck_1_buf[globalnamecheck_k-globalnamecheck_s-1]=copyoffilebuf[globalnamecheck_i];
								
							}
						}

					}
				}


				//20110908 add
				//20110908 add
				memset(globalnamecheck_buf,0,IGC_LINE_BUF_CHAR);
				memset(globalnamecheck_1_buf,0,IGC_LINE_BUF_CHAR/4);
				sprintf(globalnamecheck_buf, "I_ulongs[");
				globalnamecheck_buf_0=strstr(&copyoffilebuf[n],globalnamecheck_buf);
				if (globalnamecheck_buf_0)
				{
					globalnamecheck_i=0;
					globalnamecheck_j=0;
					globalnamecheck_k=0;
					globalnamecheck_s=0;
					for(globalnamecheck_i=n;globalnamecheck_i<IGC_LINE_BUF_CHAR-n;globalnamecheck_i++)
					{
						if(globalnamecheck_j==0 && globalnamecheck_k==0)
						{ 
							if(copyoffilebuf[globalnamecheck_i]=='[')
							{
								globalnamecheck_j=1;
								copyoffilebuf[globalnamecheck_i]=' '; 
								globalnamecheck_s=globalnamecheck_i;  
								globalnamecheck_k=globalnamecheck_i;  
							}
						}
						else
						{
							if(copyoffilebuf[globalnamecheck_i]==']')
							{
								globalnamecheck_j=2;
								copyoffilebuf[globalnamecheck_i]=' ';

								if(globalnamecheck_1_buf[0]!=0)
								{
									globalnamecheck_1_buf[globalnamecheck_k]=0;

									for(globalnamecheck_j=globalnamecheck_s;globalnamecheck_j<globalnamecheck_k-globalnamecheck_s;globalnamecheck_j++)
									{
										if(globalnamecheck_1_buf[globalnamecheck_j]!=' ' 
											&& (globalnamecheck_1_buf[globalnamecheck_j] <48 || globalnamecheck_1_buf[globalnamecheck_j]>57))
										{
											globalnamecheck_j=-1;//not num
											break;
										}
									}

									if(globalnamecheck_j>=0)
									{
										if(sscanf(globalnamecheck_1_buf,"%d",&globalnamecheck_N))
										{
										   searchgivefalg(copyoffilebuf,globalnamecheck_i,IGC_LINE_BUF_CHAR,&globalnamecheck_F,copyoffilebuf-globalnamecheck_buf_0);
										}
										else
											globalnamecheck_j=-2;
									}

									if(globalnamecheck_j<0)
									{
										 searchgivefalg(copyoffilebuf,globalnamecheck_i,IGC_LINE_BUF_CHAR,&globalnamecheck_F,copyoffilebuf-globalnamecheck_buf_0);
									}

									if(globalnamecheck_F==1)//写标志
									{
										if(globalnamecheck_j<0)//非数字标志
										{
											if(use_w_str_I_ulongs[use_w_times_I_ulongs[Iulong]][max_w_str_len-strlen(use_w_str_I_ulongs[Iulong])-7]==0
												&& use_w_str_I_ulongs[use_w_times_I_ulongs[Iulong]][max_w_str_len-strlen(use_w_str_I_ulongs[Iulong])-8]==0)
											{
												strcat(use_w_str_I_ulongs[Iulong],IGC_filename);
												strcat(use_w_str_I_ulongs[Iulong],"\n    ");
											}
											use_w_times_I_ulongs[Iulong]++;
										}
										else
										{
											if(use_w_str_I_ulongs[use_w_times_I_ulongs[globalnamecheck_N]][max_w_str_len-strlen(use_w_str_I_ulongs[globalnamecheck_N])-7]==0
												&& use_w_str_I_ulongs[use_w_times_I_ulongs[globalnamecheck_N]][max_w_str_len-strlen(use_w_str_I_ulongs[globalnamecheck_N])-8]==0)
											{
												strcat(use_w_str_I_ulongs[globalnamecheck_N],IGC_filename);
												strcat(use_w_str_I_ulongs[globalnamecheck_N],"\n    ");
											}
											use_w_times_I_ulongs[globalnamecheck_N]++;
										}
									}
									else
									{
										if(globalnamecheck_j<0)//非数字标志
										{
											use_r_times_I_ulongs[Iulong]++;
										}
										else
										{
											use_r_times_I_ulongs[globalnamecheck_N]++;
										}
									}
								}
								globalnamecheck_j=0;
								globalnamecheck_k=0;
								globalnamecheck_s=0;
							}
							else
							{
								globalnamecheck_k++;
								if(globalnamecheck_k>globalnamecheck_s)
									globalnamecheck_1_buf[globalnamecheck_k-globalnamecheck_s-1]=copyoffilebuf[globalnamecheck_i];
								
							}
						}

					}
				}


				//20110908 add
				//20110908 add
				memset(globalnamecheck_buf,0,IGC_LINE_BUF_CHAR);
				memset(globalnamecheck_1_buf,0,IGC_LINE_BUF_CHAR/4);
				sprintf(globalnamecheck_buf, "I_bools[");
				globalnamecheck_buf_0=strstr(&copyoffilebuf[n],globalnamecheck_buf);
				if (globalnamecheck_buf_0)
				{
					globalnamecheck_i=0;
					globalnamecheck_j=0;
					globalnamecheck_k=0;
					globalnamecheck_s=0;
					for(globalnamecheck_i=n;globalnamecheck_i<IGC_LINE_BUF_CHAR-n;globalnamecheck_i++)
					{
						if(globalnamecheck_j==0 && globalnamecheck_k==0)
						{ 
							if(copyoffilebuf[globalnamecheck_i]=='[')
							{
								globalnamecheck_j=1;
								copyoffilebuf[globalnamecheck_i]=' '; 
								globalnamecheck_s=globalnamecheck_i;  
								globalnamecheck_k=globalnamecheck_i;  
							}
						}
						else
						{
							if(copyoffilebuf[globalnamecheck_i]==']')
							{
								globalnamecheck_j=2;
								copyoffilebuf[globalnamecheck_i]=' ';

								if(globalnamecheck_1_buf[0]!=0)
								{
									globalnamecheck_1_buf[globalnamecheck_k]=0;

									for(globalnamecheck_j=globalnamecheck_s;globalnamecheck_j<globalnamecheck_k-globalnamecheck_s;globalnamecheck_j++)
									{
										if(globalnamecheck_1_buf[globalnamecheck_j]!=' ' 
											&& (globalnamecheck_1_buf[globalnamecheck_j] <48 || globalnamecheck_1_buf[globalnamecheck_j]>57))
										{
											globalnamecheck_j=-1;//not num
											break;
										}
									}

									if(globalnamecheck_j>=0)
									{
										if(sscanf(globalnamecheck_1_buf,"%d",&globalnamecheck_N))
										{
										   searchgivefalg(copyoffilebuf,globalnamecheck_i,IGC_LINE_BUF_CHAR,&globalnamecheck_F,copyoffilebuf-globalnamecheck_buf_0);
										}
										else
											globalnamecheck_j=-2;
									}

									if(globalnamecheck_j<0)
									{
										 searchgivefalg(copyoffilebuf,globalnamecheck_i,IGC_LINE_BUF_CHAR,&globalnamecheck_F,copyoffilebuf-globalnamecheck_buf_0);
									}

									if(globalnamecheck_F==1)//写标志
									{
										if(globalnamecheck_j<0)//非数字标志
										{
											if(use_w_str_I_bools[use_w_times_I_bools[Ibool]][max_w_str_len-strlen(use_w_str_I_bools[Ibool])-7]==0
												&& use_w_str_I_bools[use_w_times_I_bools[Ibool]][max_w_str_len-strlen(use_w_str_I_bools[Ibool])-8]==0)
											{
												strcat(use_w_str_I_bools[Ibool],IGC_filename);
												strcat(use_w_str_I_bools[Ibool],"\n    ");
											}
											use_w_times_I_bools[Ibool]++;
										}
										else
										{
											if(use_w_str_I_bools[use_w_times_I_bools[globalnamecheck_N]][max_w_str_len-strlen(use_w_str_I_bools[globalnamecheck_N])-7]==0
												&& use_w_str_I_bools[use_w_times_I_bools[globalnamecheck_N]][max_w_str_len-strlen(use_w_str_I_bools[globalnamecheck_N])-8]==0)
											{
												strcat(use_w_str_I_bools[globalnamecheck_N],IGC_filename);
												strcat(use_w_str_I_bools[globalnamecheck_N],"\n    ");
											}
											use_w_times_I_bools[globalnamecheck_N]++;
										}
									}
									else
									{
										if(globalnamecheck_j<0)//非数字标志
										{
											use_r_times_I_bools[Ibool]++;
										}
										else
										{
											use_r_times_I_bools[globalnamecheck_N]++;
										}
									}
								}
								globalnamecheck_j=0;
								globalnamecheck_k=0;
								globalnamecheck_s=0;
							}
							else
							{
								globalnamecheck_k++;
								if(globalnamecheck_k>globalnamecheck_s)
									globalnamecheck_1_buf[globalnamecheck_k-globalnamecheck_s-1]=copyoffilebuf[globalnamecheck_i];
								
							}
						}

					}
				}


				//20110908 add

								//20110908 add
				memset(globalnamecheck_buf,0,IGC_LINE_BUF_CHAR);
				memset(globalnamecheck_1_buf,0,IGC_LINE_BUF_CHAR/4);
				sprintf(globalnamecheck_buf, "I_shorts[");
				globalnamecheck_buf_0=strstr(&copyoffilebuf[n],globalnamecheck_buf);
				if (globalnamecheck_buf_0)
				{
					globalnamecheck_i=0;
					globalnamecheck_j=0;
					globalnamecheck_k=0;
					globalnamecheck_s=0;
					for(globalnamecheck_i=n;globalnamecheck_i<IGC_LINE_BUF_CHAR-n;globalnamecheck_i++)
					{
						if(globalnamecheck_j==0 && globalnamecheck_k==0)
						{ 
							if(copyoffilebuf[globalnamecheck_i]=='[')
							{
								globalnamecheck_j=1;
								copyoffilebuf[globalnamecheck_i]=' '; 
								globalnamecheck_s=globalnamecheck_i;  
								globalnamecheck_k=globalnamecheck_i;  
							}
						}
						else
						{
							if(copyoffilebuf[globalnamecheck_i]==']')
							{
								globalnamecheck_j=2;
								copyoffilebuf[globalnamecheck_i]=' ';

								if(globalnamecheck_1_buf[0]!=0)
								{
									globalnamecheck_1_buf[globalnamecheck_k]=0;

									for(globalnamecheck_j=globalnamecheck_s;globalnamecheck_j<globalnamecheck_k-globalnamecheck_s;globalnamecheck_j++)
									{
										if(globalnamecheck_1_buf[globalnamecheck_j]!=' ' 
											&& (globalnamecheck_1_buf[globalnamecheck_j] <48 || globalnamecheck_1_buf[globalnamecheck_j]>57))
										{
											globalnamecheck_j=-1;//not num
											break;
										}
									}

									if(globalnamecheck_j>=0)
									{
										if(sscanf(globalnamecheck_1_buf,"%d",&globalnamecheck_N))
										{
										   searchgivefalg(copyoffilebuf,globalnamecheck_i,IGC_LINE_BUF_CHAR,&globalnamecheck_F,copyoffilebuf-globalnamecheck_buf_0);
										}
										else
											globalnamecheck_j=-2;
									}

									if(globalnamecheck_j<0)
									{
										 searchgivefalg(copyoffilebuf,globalnamecheck_i,IGC_LINE_BUF_CHAR,&globalnamecheck_F,copyoffilebuf-globalnamecheck_buf_0);
									}

									if(globalnamecheck_F==1)//写标志
									{
										if(globalnamecheck_j<0)//非数字标志
										{
											if(use_w_str_I_shorts[use_w_times_I_shorts[Ishort]][max_w_str_len-strlen(use_w_str_I_shorts[Ishort])-7]==0
												&& use_w_str_I_shorts[use_w_times_I_shorts[Ishort]][max_w_str_len-strlen(use_w_str_I_shorts[Ishort])-8]==0)
											{
												strcat(use_w_str_I_shorts[Ishort],IGC_filename);
												strcat(use_w_str_I_shorts[Ishort],"\n    ");
											}
											use_w_times_I_shorts[Ishort]++;
										}
										else
										{
											if(use_w_str_I_shorts[use_w_times_I_shorts[globalnamecheck_N]][max_w_str_len-strlen(use_w_str_I_shorts[globalnamecheck_N])-7]==0
												&& use_w_str_I_shorts[use_w_times_I_shorts[globalnamecheck_N]][max_w_str_len-strlen(use_w_str_I_shorts[globalnamecheck_N])-8]==0)
											{
												strcat(use_w_str_I_shorts[globalnamecheck_N],IGC_filename);
												strcat(use_w_str_I_shorts[globalnamecheck_N],"\n    ");
											}
											use_w_times_I_shorts[globalnamecheck_N]++;
										}
									}
									else
									{
										if(globalnamecheck_j<0)//非数字标志
										{
											use_r_times_I_shorts[Ishort]++;
										}
										else
										{
											use_r_times_I_shorts[globalnamecheck_N]++;
										}
									}
								}
								globalnamecheck_j=0;
								globalnamecheck_k=0;
								globalnamecheck_s=0;
							}
							else
							{
								globalnamecheck_k++;
								if(globalnamecheck_k>globalnamecheck_s)
									globalnamecheck_1_buf[globalnamecheck_k-globalnamecheck_s-1]=copyoffilebuf[globalnamecheck_i];
								
							}
						}

					}
				}


				//20110908 add

				//20110908 add
				memset(globalnamecheck_buf,0,IGC_LINE_BUF_CHAR);
				memset(globalnamecheck_1_buf,0,IGC_LINE_BUF_CHAR/4);
				sprintf(globalnamecheck_buf, "I_ushorts[");
				globalnamecheck_buf_0=strstr(&copyoffilebuf[n],globalnamecheck_buf);
				if (globalnamecheck_buf_0)
				{
					globalnamecheck_i=0;
					globalnamecheck_j=0;
					globalnamecheck_k=0;
					globalnamecheck_s=0;
					for(globalnamecheck_i=n;globalnamecheck_i<IGC_LINE_BUF_CHAR-n;globalnamecheck_i++)
					{
						if(globalnamecheck_j==0 && globalnamecheck_k==0)
						{ 
							if(copyoffilebuf[globalnamecheck_i]=='[')
							{
								globalnamecheck_j=1;
								copyoffilebuf[globalnamecheck_i]=' '; 
								globalnamecheck_s=globalnamecheck_i;  
								globalnamecheck_k=globalnamecheck_i;  
							}
						}
						else
						{
							if(copyoffilebuf[globalnamecheck_i]==']')
							{
								globalnamecheck_j=2;
								copyoffilebuf[globalnamecheck_i]=' ';

								if(globalnamecheck_1_buf[0]!=0)
								{
									globalnamecheck_1_buf[globalnamecheck_k]=0;

									for(globalnamecheck_j=globalnamecheck_s;globalnamecheck_j<globalnamecheck_k-globalnamecheck_s;globalnamecheck_j++)
									{
										if(globalnamecheck_1_buf[globalnamecheck_j]!=' ' 
											&& (globalnamecheck_1_buf[globalnamecheck_j] <48 || globalnamecheck_1_buf[globalnamecheck_j]>57))
										{
											globalnamecheck_j=-1;//not num
											break;
										}
									}

									if(globalnamecheck_j>=0)
									{
										if(sscanf(globalnamecheck_1_buf,"%d",&globalnamecheck_N))
										{
										   searchgivefalg(copyoffilebuf,globalnamecheck_i,IGC_LINE_BUF_CHAR,&globalnamecheck_F,copyoffilebuf-globalnamecheck_buf_0);
										}
										else
											globalnamecheck_j=-2;
									}

									if(globalnamecheck_j<0)
									{
										 searchgivefalg(copyoffilebuf,globalnamecheck_i,IGC_LINE_BUF_CHAR,&globalnamecheck_F,copyoffilebuf-globalnamecheck_buf_0);
									}

									if(globalnamecheck_F==1)//写标志
									{
										if(globalnamecheck_j<0)//非数字标志
										{
											if(use_w_str_I_ushorts[use_w_times_I_ushorts[Iushort]][max_w_str_len-strlen(use_w_str_I_ushorts[Iushort])-7]==0
												&& use_w_str_I_ushorts[use_w_times_I_ushorts[Iushort]][max_w_str_len-strlen(use_w_str_I_ushorts[Iushort])-8]==0)
											{
												strcat(use_w_str_I_ushorts[Iushort],IGC_filename);
												strcat(use_w_str_I_ushorts[Iushort],"\n    ");
											}
											use_w_times_I_ushorts[Iushort]++;
										}
										else
										{
											if(use_w_str_I_ushorts[use_w_times_I_ushorts[globalnamecheck_N]][max_w_str_len-strlen(use_w_str_I_ushorts[globalnamecheck_N])-7]==0
												&& use_w_str_I_ushorts[use_w_times_I_ushorts[globalnamecheck_N]][max_w_str_len-strlen(use_w_str_I_ushorts[globalnamecheck_N])-8]==0)
											{
												strcat(use_w_str_I_ushorts[globalnamecheck_N],IGC_filename);
												strcat(use_w_str_I_ushorts[globalnamecheck_N],"\n    ");
											}
											use_w_times_I_ushorts[globalnamecheck_N]++;
										}
									}
									else
									{
										if(globalnamecheck_j<0)//非数字标志
										{
											use_r_times_I_ushorts[Iushort]++;
										}
										else
										{
											use_r_times_I_ushorts[globalnamecheck_N]++;
										}
									}
								}
								globalnamecheck_j=0;
								globalnamecheck_k=0;
								globalnamecheck_s=0;
							}
							else
							{
								globalnamecheck_k++;
								if(globalnamecheck_k>globalnamecheck_s)
									globalnamecheck_1_buf[globalnamecheck_k-globalnamecheck_s-1]=copyoffilebuf[globalnamecheck_i];
								
							}
						}

					}
				}


				//20110908 add
				//20110908 add
				memset(globalnamecheck_buf,0,IGC_LINE_BUF_CHAR);
				memset(globalnamecheck_1_buf,0,IGC_LINE_BUF_CHAR/4);
				sprintf(globalnamecheck_buf, "I_chars[");
				globalnamecheck_buf_0=strstr(&copyoffilebuf[n],globalnamecheck_buf);
				if (globalnamecheck_buf_0)
				{
					globalnamecheck_i=0;
					globalnamecheck_j=0;
					globalnamecheck_k=0;
					globalnamecheck_s=0;
					for(globalnamecheck_i=n;globalnamecheck_i<IGC_LINE_BUF_CHAR-n;globalnamecheck_i++)
					{
						if(globalnamecheck_j==0 && globalnamecheck_k==0)
						{ 
							if(copyoffilebuf[globalnamecheck_i]=='[')
							{
								globalnamecheck_j=1;
								copyoffilebuf[globalnamecheck_i]=' '; 
								globalnamecheck_s=globalnamecheck_i;  
								globalnamecheck_k=globalnamecheck_i;  
							}
						}
						else
						{
							if(copyoffilebuf[globalnamecheck_i]==']')
							{
								globalnamecheck_j=2;
								copyoffilebuf[globalnamecheck_i]=' ';

								if(globalnamecheck_1_buf[0]!=0)
								{
									globalnamecheck_1_buf[globalnamecheck_k]=0;

									for(globalnamecheck_j=globalnamecheck_s;globalnamecheck_j<globalnamecheck_k-globalnamecheck_s;globalnamecheck_j++)
									{
										if(globalnamecheck_1_buf[globalnamecheck_j]!=' ' 
											&& (globalnamecheck_1_buf[globalnamecheck_j] <48 || globalnamecheck_1_buf[globalnamecheck_j]>57))
										{
											globalnamecheck_j=-1;//not num
											break;
										}
									}

									if(globalnamecheck_j>=0)
									{
										if(sscanf(globalnamecheck_1_buf,"%d",&globalnamecheck_N))
										{
										   searchgivefalg(copyoffilebuf,globalnamecheck_i,IGC_LINE_BUF_CHAR,&globalnamecheck_F,copyoffilebuf-globalnamecheck_buf_0);
										}
										else
											globalnamecheck_j=-2;
									}

									if(globalnamecheck_j<0)
									{
										 searchgivefalg(copyoffilebuf,globalnamecheck_i,IGC_LINE_BUF_CHAR,&globalnamecheck_F,copyoffilebuf-globalnamecheck_buf_0);
									}

									if(globalnamecheck_F==1)//写标志
									{
										if(globalnamecheck_j<0)//非数字标志
										{
											if(use_w_str_I_chars[use_w_times_I_chars[Ichar]][max_w_str_len-strlen(use_w_str_I_chars[Ichar])-7]==0
												&& use_w_str_I_chars[use_w_times_I_chars[Ichar]][max_w_str_len-strlen(use_w_str_I_chars[Ichar])-8]==0)
											{
												strcat(use_w_str_I_chars[Ichar],IGC_filename);
												strcat(use_w_str_I_chars[Ichar],"\n    ");
											}
											use_w_times_I_chars[Ichar]++;
										}
										else
										{
											if(use_w_str_I_chars[use_w_times_I_chars[globalnamecheck_N]][max_w_str_len-strlen(use_w_str_I_chars[globalnamecheck_N])-7]==0
												&& use_w_str_I_chars[use_w_times_I_chars[globalnamecheck_N]][max_w_str_len-strlen(use_w_str_I_chars[globalnamecheck_N])-8]==0)
											{
												strcat(use_w_str_I_chars[globalnamecheck_N],IGC_filename);
												strcat(use_w_str_I_chars[globalnamecheck_N],"\n    ");
											}
											use_w_times_I_chars[globalnamecheck_N]++;
										}
									}
									else
									{
										if(globalnamecheck_j<0)//非数字标志
										{
											use_r_times_I_chars[Ichar]++;
										}
										else
										{
											use_r_times_I_chars[globalnamecheck_N]++;
										}
									}
								}
								globalnamecheck_j=0;
								globalnamecheck_k=0;
								globalnamecheck_s=0;
							}
							else
							{
								globalnamecheck_k++;
								if(globalnamecheck_k>globalnamecheck_s)
									globalnamecheck_1_buf[globalnamecheck_k-globalnamecheck_s-1]=copyoffilebuf[globalnamecheck_i];
								
							}
						}

					}
				}


				//20110908 add
				//20110908 add
				memset(globalnamecheck_buf,0,IGC_LINE_BUF_CHAR);
				memset(globalnamecheck_1_buf,0,IGC_LINE_BUF_CHAR/4);
				sprintf(globalnamecheck_buf, "I_uchars[");
				globalnamecheck_buf_0=strstr(&copyoffilebuf[n],globalnamecheck_buf);
				if (globalnamecheck_buf_0)
				{
					globalnamecheck_i=0;
					globalnamecheck_j=0;
					globalnamecheck_k=0;
					globalnamecheck_s=0;
					for(globalnamecheck_i=n;globalnamecheck_i<IGC_LINE_BUF_CHAR-n;globalnamecheck_i++)
					{
						if(globalnamecheck_j==0 && globalnamecheck_k==0)
						{ 
							if(copyoffilebuf[globalnamecheck_i]=='[')
							{
								globalnamecheck_j=1;
								copyoffilebuf[globalnamecheck_i]=' '; 
								globalnamecheck_s=globalnamecheck_i;  
								globalnamecheck_k=globalnamecheck_i;  
							}
						}
						else
						{
							if(copyoffilebuf[globalnamecheck_i]==']')
							{
								globalnamecheck_j=2;
								copyoffilebuf[globalnamecheck_i]=' ';

								if(globalnamecheck_1_buf[0]!=0)
								{
									globalnamecheck_1_buf[globalnamecheck_k]=0;

									for(globalnamecheck_j=globalnamecheck_s;globalnamecheck_j<globalnamecheck_k-globalnamecheck_s;globalnamecheck_j++)
									{
										if(globalnamecheck_1_buf[globalnamecheck_j]!=' ' 
											&& (globalnamecheck_1_buf[globalnamecheck_j] <48 || globalnamecheck_1_buf[globalnamecheck_j]>57))
										{
											globalnamecheck_j=-1;//not num
											break;
										}
									}

									if(globalnamecheck_j>=0)
									{
										if(sscanf(globalnamecheck_1_buf,"%d",&globalnamecheck_N))
										{
										   searchgivefalg(copyoffilebuf,globalnamecheck_i,IGC_LINE_BUF_CHAR,&globalnamecheck_F,copyoffilebuf-globalnamecheck_buf_0);
										}
										else
											globalnamecheck_j=-2;
									}

									if(globalnamecheck_j<0)
									{
										 searchgivefalg(copyoffilebuf,globalnamecheck_i,IGC_LINE_BUF_CHAR,&globalnamecheck_F,copyoffilebuf-globalnamecheck_buf_0);
									}

									if(globalnamecheck_F==1)//写标志
									{
										if(globalnamecheck_j<0)//非数字标志
										{
											if(use_w_str_I_uchars[use_w_times_I_uchars[Iuchar]][max_w_str_len-strlen(use_w_str_I_uchars[Iuchar])-7]==0
												&& use_w_str_I_uchars[use_w_times_I_uchars[Iuchar]][max_w_str_len-strlen(use_w_str_I_uchars[Iuchar])-8]==0)
											{
												strcat(use_w_str_I_uchars[Iuchar],IGC_filename);
												strcat(use_w_str_I_uchars[Iuchar],"\n    ");
											}
											use_w_times_I_uchars[Iuchar]++;
										}
										else
										{
											if(use_w_str_I_uchars[use_w_times_I_uchars[globalnamecheck_N]][max_w_str_len-strlen(use_w_str_I_uchars[globalnamecheck_N])-7]==0
												&& use_w_str_I_uchars[use_w_times_I_uchars[globalnamecheck_N]][max_w_str_len-strlen(use_w_str_I_uchars[globalnamecheck_N])-8]==0)
											{
												strcat(use_w_str_I_uchars[globalnamecheck_N],IGC_filename);
												strcat(use_w_str_I_uchars[globalnamecheck_N],"\n    ");
											}
											use_w_times_I_uchars[globalnamecheck_N]++;
										}
									}
									else
									{
										if(globalnamecheck_j<0)//非数字标志
										{
											use_r_times_I_uchars[Iuchar]++;
										}
										else
										{
											use_r_times_I_uchars[globalnamecheck_N]++;
										}
									}
								}
								globalnamecheck_j=0;
								globalnamecheck_k=0;
								globalnamecheck_s=0;
							}
							else
							{
								globalnamecheck_k++;
								if(globalnamecheck_k>globalnamecheck_s)
									globalnamecheck_1_buf[globalnamecheck_k-globalnamecheck_s-1]=copyoffilebuf[globalnamecheck_i];
								
							}
						}

					}
				}


				//20110908 add
							//20110908 add
				memset(globalnamecheck_buf,0,IGC_LINE_BUF_CHAR);
				memset(globalnamecheck_1_buf,0,IGC_LINE_BUF_CHAR/4);
				sprintf(globalnamecheck_buf, "I_datetimes[");
				globalnamecheck_buf_0=strstr(&copyoffilebuf[n],globalnamecheck_buf);
				if (globalnamecheck_buf_0)
				{
					globalnamecheck_i=0;
					globalnamecheck_j=0;
					globalnamecheck_k=0;
					globalnamecheck_s=0;
					for(globalnamecheck_i=n;globalnamecheck_i<IGC_LINE_BUF_CHAR-n;globalnamecheck_i++)
					{
						if(globalnamecheck_j==0 && globalnamecheck_k==0)
						{ 
							if(copyoffilebuf[globalnamecheck_i]=='[')
							{
								globalnamecheck_j=1;
								copyoffilebuf[globalnamecheck_i]=' '; 
								globalnamecheck_s=globalnamecheck_i;  
								globalnamecheck_k=globalnamecheck_i;  
							}
						}
						else
						{
							if(copyoffilebuf[globalnamecheck_i]==']')
							{
								globalnamecheck_j=2;
								copyoffilebuf[globalnamecheck_i]=' ';

								if(globalnamecheck_1_buf[0]!=0)
								{
									globalnamecheck_1_buf[globalnamecheck_k]=0;

									for(globalnamecheck_j=globalnamecheck_s;globalnamecheck_j<globalnamecheck_k-globalnamecheck_s;globalnamecheck_j++)
									{
										if(globalnamecheck_1_buf[globalnamecheck_j]!=' ' 
											&& (globalnamecheck_1_buf[globalnamecheck_j] <48 || globalnamecheck_1_buf[globalnamecheck_j]>57))
										{
											globalnamecheck_j=-1;//not num
											break;
										}
									}
									if(!searchgive_falg_0(&copyoffilebuf[globalnamecheck_i],&globalnamecheck_R,
										".year",".month",".day",".hour",".minute",".second",
										"","","","","",
										"","","","","",
										"","","","","",
										"","","","","",
										"","","","","",""))
									{
										
									}
									if(globalnamecheck_j>=0)
									{
										if(sscanf(globalnamecheck_1_buf,"%d",&globalnamecheck_N))
										{
										   searchgivefalg(copyoffilebuf,globalnamecheck_i,IGC_LINE_BUF_CHAR,&globalnamecheck_F,copyoffilebuf-globalnamecheck_buf_0);
										}
										else
											globalnamecheck_j=-2;
									}

									if(globalnamecheck_j<0)
									{
										 searchgivefalg(copyoffilebuf,globalnamecheck_i,IGC_LINE_BUF_CHAR,&globalnamecheck_F,copyoffilebuf-globalnamecheck_buf_0);
									}

									if(globalnamecheck_F==1)//写标志
									{
										if(globalnamecheck_j<0)//非数字标志
										{
											if(use_w_str_I_datetimes[use_w_times_I_datetimes[Idatetime]][max_w_str_len-strlen(use_w_str_I_datetimes[Idatetime])-7]==0
												&& use_w_str_I_datetimes[use_w_times_I_datetimes[Idatetime]][max_w_str_len-strlen(use_w_str_I_datetimes[Idatetime])-8]==0)
											{
												strcat(use_w_str_I_datetimes[Idatetime],IGC_filename);
												strcat(use_w_str_I_datetimes[Idatetime],"\n    ");
											}
											use_w_times_I_datetimes[Idatetime]++;
										}
										else
										{
											if(use_w_str_I_datetimes[use_w_times_I_datetimes[globalnamecheck_N]][max_w_str_len-strlen(use_w_str_I_datetimes[globalnamecheck_N])-7]==0
												&& use_w_str_I_datetimes[use_w_times_I_datetimes[globalnamecheck_N]][max_w_str_len-strlen(use_w_str_I_datetimes[globalnamecheck_N])-8]==0)
											{
												strcat(use_w_str_I_datetimes[globalnamecheck_N],IGC_filename);
												strcat(use_w_str_I_datetimes[globalnamecheck_N],"\n    ");
											}
											use_w_times_I_datetimes[globalnamecheck_N]++;
										}
									}
									else
									{
										if(globalnamecheck_j<0)//非数字标志
										{
											use_r_times_I_datetimes[Idatetime]++;
										}
										else
										{
											use_r_times_I_datetimes[globalnamecheck_N]++;
										}
									}
								}
								globalnamecheck_j=0;
								globalnamecheck_k=0;
								globalnamecheck_s=0;
							}
							else
							{
								globalnamecheck_k++;
								if(globalnamecheck_k>globalnamecheck_s)
									globalnamecheck_1_buf[globalnamecheck_k-globalnamecheck_s-1]=copyoffilebuf[globalnamecheck_i];
								
							}
						}

					}
				}


				//20110908 add
				//20110908 add
				memset(globalnamecheck_buf,0,IGC_LINE_BUF_CHAR);
				memset(globalnamecheck_1_buf,0,IGC_LINE_BUF_CHAR/4);
				sprintf(globalnamecheck_buf, "I_H9000_IGC_datas[");
				globalnamecheck_buf_0=strstr(&copyoffilebuf[n],globalnamecheck_buf);
				if (globalnamecheck_buf_0)
				{
					globalnamecheck_i=0;
					globalnamecheck_j=0;
					globalnamecheck_k=0;
					globalnamecheck_s=0;

					globalnamecheck_R=0;//

					for(globalnamecheck_i=n;globalnamecheck_i<IGC_LINE_BUF_CHAR-n;globalnamecheck_i++)
					{
						if(globalnamecheck_j==0 && globalnamecheck_k==0)
						{ 
							if(copyoffilebuf[globalnamecheck_i]=='[')
							{
								globalnamecheck_j=1;
								copyoffilebuf[globalnamecheck_i]=' '; 
								globalnamecheck_s=globalnamecheck_i;  
								globalnamecheck_k=globalnamecheck_i;  
							}
						}
						else
						{
							if(copyoffilebuf[globalnamecheck_i]==']')//
							{
								globalnamecheck_j=2;
								copyoffilebuf[globalnamecheck_i]=' ';
								
								if(globalnamecheck_1_buf[0]!=0)
								{
									globalnamecheck_1_buf[globalnamecheck_k]=0;

									for(globalnamecheck_j=globalnamecheck_s;globalnamecheck_j<globalnamecheck_k-globalnamecheck_s;globalnamecheck_j++)
									{
										if(globalnamecheck_1_buf[globalnamecheck_j]!=' ' 
											&& (globalnamecheck_1_buf[globalnamecheck_j] <48 || globalnamecheck_1_buf[globalnamecheck_j]>57))
										{
											globalnamecheck_j=-1;//not num
											break;
										}
									}


									if(!searchgive_falg_0(&copyoffilebuf[globalnamecheck_i],&globalnamecheck_R,
										".value",".int_value",".chan_fail",".ok_flag",
										".update_time.year",".update_time.month",".update_time.day",".update_time.hour",".update_time.minute",".update_time.second",
										".change_time.year",".change_time.month",".change_time.day",".change_time.hour",".change_time.minute",".change_time.second",
										".update_time",".change_time",
										"","","","","","","","","","","","","",""))
									{
								 
									}
									if(globalnamecheck_j>=0)
									{
										if(sscanf(globalnamecheck_1_buf,"%d",&globalnamecheck_N))
										{
										   searchgivefalg(copyoffilebuf,globalnamecheck_i,IGC_LINE_BUF_CHAR,&globalnamecheck_F,copyoffilebuf-globalnamecheck_buf_0);
										}
										else
											globalnamecheck_j=-2;
									}

									if(globalnamecheck_j<0)
									{
										 searchgivefalg(copyoffilebuf,globalnamecheck_i,IGC_LINE_BUF_CHAR,&globalnamecheck_F,copyoffilebuf-globalnamecheck_buf_0);
									}

									if(globalnamecheck_F==1)//写标志
									{
										if(globalnamecheck_j<0)//非数字标志
										{
											if(use_w_str_I_H9000_IGC_datas[use_w_times_I_H9000_IGC_datas[IH9000_IGC_data]][max_w_str_len-strlen(use_w_str_I_H9000_IGC_datas[IH9000_IGC_data])-7]==0
												&& use_w_str_I_H9000_IGC_datas[use_w_times_I_H9000_IGC_datas[IH9000_IGC_data]][max_w_str_len-strlen(use_w_str_I_H9000_IGC_datas[IH9000_IGC_data])-8]==0)
											{
												strcat(use_w_str_I_H9000_IGC_datas[IH9000_IGC_data],IGC_filename);
												strcat(use_w_str_I_H9000_IGC_datas[IH9000_IGC_data],"\n    ");
											}
											use_w_times_I_H9000_IGC_datas[IH9000_IGC_data]++;
										}
										else
										{
											if(use_w_str_I_H9000_IGC_datas[use_w_times_I_H9000_IGC_datas[globalnamecheck_N]][max_w_str_len-strlen(use_w_str_I_H9000_IGC_datas[globalnamecheck_N])-7]==0
												&& use_w_str_I_H9000_IGC_datas[use_w_times_I_H9000_IGC_datas[globalnamecheck_N]][max_w_str_len-strlen(use_w_str_I_H9000_IGC_datas[globalnamecheck_N])-8]==0)
											{
												strcat(use_w_str_I_H9000_IGC_datas[globalnamecheck_N],IGC_filename);
												strcat(use_w_str_I_H9000_IGC_datas[globalnamecheck_N],"\n    ");
											}
											use_w_times_I_H9000_IGC_datas[globalnamecheck_N]++;
										}
									}
									else
									{
										if(globalnamecheck_j<0)//非数字标志
										{
											use_r_times_I_H9000_IGC_datas[IH9000_IGC_data]++;
										}
										else
										{
											use_r_times_I_H9000_IGC_datas[globalnamecheck_N]++;
										}
									}
								}
								globalnamecheck_j=0;
								globalnamecheck_k=0;
								globalnamecheck_s=0;
							}
							else
							{
								globalnamecheck_k++;
								if(globalnamecheck_k>globalnamecheck_s)
									globalnamecheck_1_buf[globalnamecheck_k-globalnamecheck_s-1]=copyoffilebuf[globalnamecheck_i];
								
							}
						}

					}
				}


				//20110908 add
				//20110908 add
				memset(globalnamecheck_buf,0,IGC_LINE_BUF_CHAR);
				memset(globalnamecheck_1_buf,0,IGC_LINE_BUF_CHAR/4);
				sprintf(globalnamecheck_buf, "I_H9000_IGC_IND_datas[");
				globalnamecheck_buf_0=strstr(&copyoffilebuf[n],globalnamecheck_buf);
				if (globalnamecheck_buf_0)
				{
					globalnamecheck_i=0;
					globalnamecheck_j=0;
					globalnamecheck_k=0;
					globalnamecheck_s=0;
					for(globalnamecheck_i=n;globalnamecheck_i<IGC_LINE_BUF_CHAR-n;globalnamecheck_i++)
					{
						if(globalnamecheck_j==0 && globalnamecheck_k==0)
						{ 
							if(copyoffilebuf[globalnamecheck_i]=='[')
							{
								globalnamecheck_j=1;
								copyoffilebuf[globalnamecheck_i]=' '; 
								globalnamecheck_s=globalnamecheck_i;  
								globalnamecheck_k=globalnamecheck_i;  
							}
						}
						else
						{
							if(copyoffilebuf[globalnamecheck_i]==']')
							{
								globalnamecheck_j=2;
								copyoffilebuf[globalnamecheck_i]=' ';

								if(globalnamecheck_1_buf[0]!=0)
								{
									globalnamecheck_1_buf[globalnamecheck_k]=0;

									for(globalnamecheck_j=globalnamecheck_s;globalnamecheck_j<globalnamecheck_k-globalnamecheck_s;globalnamecheck_j++)
									{
										if(globalnamecheck_1_buf[globalnamecheck_j]!=' ' 
											&& (globalnamecheck_1_buf[globalnamecheck_j] <48 || globalnamecheck_1_buf[globalnamecheck_j]>57))
										{
											globalnamecheck_j=-1;//not num
											break;
										}
									}

									if(!searchgive_falg_0(&copyoffilebuf[globalnamecheck_i],&globalnamecheck_R,
										".value",".chan_fail",".ok_flag",
										".update_time.year",".update_time.month",".update_time.day",".update_time.hour",".update_time.minute",".update_time.second",
										".change_time.year",".change_time.month",".change_time.day",".change_time.hour",".change_time.minute",".change_time.second",
										".update_time",".change_time",
										"","","","","","","","","","","","","","",""))
									{
										
									}

									if(globalnamecheck_j>=0)
									{
										if(sscanf(globalnamecheck_1_buf,"%d",&globalnamecheck_N))
										{
										   searchgivefalg(copyoffilebuf,globalnamecheck_i,IGC_LINE_BUF_CHAR,&globalnamecheck_F,copyoffilebuf-globalnamecheck_buf_0);
										}
										else
											globalnamecheck_j=-2;
									}

									if(globalnamecheck_j<0)
									{
										 searchgivefalg(copyoffilebuf,globalnamecheck_i,IGC_LINE_BUF_CHAR,&globalnamecheck_F,copyoffilebuf-globalnamecheck_buf_0);
									}

									if(globalnamecheck_F==1)//写标志
									{
										if(globalnamecheck_j<0)//非数字标志
										{
											if(use_w_str_I_H9000_IGC_IND_datas[use_w_times_I_H9000_IGC_IND_datas[IH9000_IGC_IND_data]][max_w_str_len-strlen(use_w_str_I_H9000_IGC_IND_datas[IH9000_IGC_IND_data])-7]==0
												&& use_w_str_I_H9000_IGC_IND_datas[use_w_times_I_H9000_IGC_IND_datas[IH9000_IGC_IND_data]][max_w_str_len-strlen(use_w_str_I_H9000_IGC_IND_datas[IH9000_IGC_IND_data])-8]==0)
											{
												strcat(use_w_str_I_H9000_IGC_IND_datas[IH9000_IGC_IND_data],IGC_filename);
												strcat(use_w_str_I_H9000_IGC_IND_datas[IH9000_IGC_IND_data],"\n    ");
											}
											use_w_times_I_H9000_IGC_IND_datas[IH9000_IGC_IND_data]++;
										}
										else
										{
											if(use_w_str_I_H9000_IGC_IND_datas[use_w_times_I_H9000_IGC_IND_datas[globalnamecheck_N]][max_w_str_len-strlen(use_w_str_I_H9000_IGC_IND_datas[globalnamecheck_N])-7]==0
												&& use_w_str_I_H9000_IGC_IND_datas[use_w_times_I_H9000_IGC_IND_datas[globalnamecheck_N]][max_w_str_len-strlen(use_w_str_I_H9000_IGC_IND_datas[globalnamecheck_N])-8]==0)
											{
												strcat(use_w_str_I_H9000_IGC_IND_datas[globalnamecheck_N],IGC_filename);
												strcat(use_w_str_I_H9000_IGC_IND_datas[globalnamecheck_N],"\n    ");
											}
											use_w_times_I_H9000_IGC_IND_datas[globalnamecheck_N]++;
										}
									}
									else
									{
										if(globalnamecheck_j<0)//非数字标志
										{
											use_r_times_I_H9000_IGC_IND_datas[IH9000_IGC_IND_data]++;
										}
										else
										{
											use_r_times_I_H9000_IGC_IND_datas[globalnamecheck_N]++;
										}
									}
								}
								globalnamecheck_j=0;
								globalnamecheck_k=0;
								globalnamecheck_s=0;
							}
							else
							{
								globalnamecheck_k++;
								if(globalnamecheck_k>globalnamecheck_s)
									globalnamecheck_1_buf[globalnamecheck_k-globalnamecheck_s-1]=copyoffilebuf[globalnamecheck_i];
								
							}
						}

					}
				}


				//20110908 add

				//20110908 add
				memset(globalnamecheck_buf,0,IGC_LINE_BUF_CHAR);
				memset(globalnamecheck_1_buf,0,IGC_LINE_BUF_CHAR/4);
				sprintf(globalnamecheck_buf, "I_H9000_IGC_ANA_datas[");
				globalnamecheck_buf_0=strstr(&copyoffilebuf[n],globalnamecheck_buf);
				if (globalnamecheck_buf_0)
				{
					globalnamecheck_i=0;
					globalnamecheck_j=0;
					globalnamecheck_k=0;
					globalnamecheck_s=0;
					for(globalnamecheck_i=n;globalnamecheck_i<IGC_LINE_BUF_CHAR-n;globalnamecheck_i++)
					{
						if(globalnamecheck_j==0 && globalnamecheck_k==0)
						{ 
							if(copyoffilebuf[globalnamecheck_i]=='[')
							{
								globalnamecheck_j=1;
								copyoffilebuf[globalnamecheck_i]=' '; 
								globalnamecheck_s=globalnamecheck_i;  
								globalnamecheck_k=globalnamecheck_i;  
							}
						}
						else
						{
							if(copyoffilebuf[globalnamecheck_i]==']')
							{
								globalnamecheck_j=2;
								copyoffilebuf[globalnamecheck_i]=' ';

								if(globalnamecheck_1_buf[0]!=0)
								{
									globalnamecheck_1_buf[globalnamecheck_k]=0;

									for(globalnamecheck_j=globalnamecheck_s;globalnamecheck_j<globalnamecheck_k-globalnamecheck_s;globalnamecheck_j++)
									{
										if(globalnamecheck_1_buf[globalnamecheck_j]!=' ' 
											&& (globalnamecheck_1_buf[globalnamecheck_j] <48 || globalnamecheck_1_buf[globalnamecheck_j]>57))
										{
											globalnamecheck_j=-1;//not num
											break;
										}
									}

									
									if(!searchgive_falg_0(&copyoffilebuf[globalnamecheck_i],&globalnamecheck_R,
										".value",".int_value",".chan_fail",".ok_flag",
										".update_time.year",".update_time.month",".update_time.day",".update_time.hour",".update_time.minute",".update_time.second",
										".change_time.year",".change_time.month",".change_time.day",".change_time.hour",".change_time.minute",".change_time.second",
										".update_time",".change_time",
										".D_BAND",".HH_LMT",".H_LMT",".L_LMT",".LL_LMT",".HR_LMT",".LR_LMT",".ENG_CVT",".HI_RANGE",".LO_RANGE",".ZERO",".COMPENSATE",".TR_LIM",".RT_LIM"))
									{
									
									}
									if(globalnamecheck_j>=0)
									{
										if(sscanf(globalnamecheck_1_buf,"%d",&globalnamecheck_N))
										{
										   searchgivefalg(copyoffilebuf,globalnamecheck_i,IGC_LINE_BUF_CHAR,&globalnamecheck_F,copyoffilebuf-globalnamecheck_buf_0);
										}
										else
											globalnamecheck_j=-2;
									}

									if(globalnamecheck_j<0)
									{
										 searchgivefalg(copyoffilebuf,globalnamecheck_i,IGC_LINE_BUF_CHAR,&globalnamecheck_F,copyoffilebuf-globalnamecheck_buf_0);
									}

									if(globalnamecheck_F==1)//写标志
									{
										if(globalnamecheck_j<0)//非数字标志
										{
											if(use_w_str_I_H9000_IGC_ANA_datas[use_w_times_I_H9000_IGC_ANA_datas[IH9000_IGC_ANA_data]][max_w_str_len-strlen(use_w_str_I_H9000_IGC_ANA_datas[IH9000_IGC_ANA_data])-7]==0
												&& use_w_str_I_H9000_IGC_ANA_datas[use_w_times_I_H9000_IGC_ANA_datas[IH9000_IGC_ANA_data]][max_w_str_len-strlen(use_w_str_I_H9000_IGC_ANA_datas[IH9000_IGC_ANA_data])-8]==0)
											{
												strcat(use_w_str_I_H9000_IGC_ANA_datas[IH9000_IGC_ANA_data],IGC_filename);
												strcat(use_w_str_I_H9000_IGC_ANA_datas[IH9000_IGC_ANA_data],"\n    ");
											}
											use_w_times_I_H9000_IGC_ANA_datas[IH9000_IGC_ANA_data]++;
										}
										else
										{
											if(use_w_str_I_H9000_IGC_ANA_datas[use_w_times_I_H9000_IGC_ANA_datas[globalnamecheck_N]][max_w_str_len-strlen(use_w_str_I_H9000_IGC_ANA_datas[globalnamecheck_N])-7]==0
												&& use_w_str_I_H9000_IGC_ANA_datas[use_w_times_I_H9000_IGC_ANA_datas[globalnamecheck_N]][max_w_str_len-strlen(use_w_str_I_H9000_IGC_ANA_datas[globalnamecheck_N])-8]==0)
											{
												strcat(use_w_str_I_H9000_IGC_ANA_datas[globalnamecheck_N],IGC_filename);
												strcat(use_w_str_I_H9000_IGC_ANA_datas[globalnamecheck_N],"\n    ");
											}
											use_w_times_I_H9000_IGC_ANA_datas[globalnamecheck_N]++;
										}
									}
									else
									{
										if(globalnamecheck_j<0)//非数字标志
										{
											use_r_times_I_H9000_IGC_ANA_datas[IH9000_IGC_ANA_data]++;
										}
										else
										{
											use_r_times_I_H9000_IGC_ANA_datas[globalnamecheck_N]++;
										}
									}
								}
								globalnamecheck_j=0;
								globalnamecheck_k=0;
								globalnamecheck_s=0;
							}
							else
							{
								globalnamecheck_k++;
								if(globalnamecheck_k>globalnamecheck_s)
									globalnamecheck_1_buf[globalnamecheck_k-globalnamecheck_s-1]=copyoffilebuf[globalnamecheck_i];
								
							}
						}

					}
				}


				//20110908 add
				//20110908 add
				memset(globalnamecheck_buf,0,IGC_LINE_BUF_CHAR);
				memset(globalnamecheck_1_buf,0,IGC_LINE_BUF_CHAR/4);
				sprintf(globalnamecheck_buf, "I_H9000_timers[");
				globalnamecheck_buf_0=strstr(&copyoffilebuf[n],globalnamecheck_buf);
				if (globalnamecheck_buf_0)
				{
					globalnamecheck_i=0;
					globalnamecheck_j=0;
					globalnamecheck_k=0;
					globalnamecheck_s=0;
					for(globalnamecheck_i=n;globalnamecheck_i<IGC_LINE_BUF_CHAR-n;globalnamecheck_i++)
					{
						if(globalnamecheck_j==0 && globalnamecheck_k==0)
						{ 
							if(copyoffilebuf[globalnamecheck_i]=='[')
							{
								globalnamecheck_j=1;
								copyoffilebuf[globalnamecheck_i]=' '; 
								globalnamecheck_s=globalnamecheck_i;  
								globalnamecheck_k=globalnamecheck_i;  
							}
						}
						else
						{
							if(copyoffilebuf[globalnamecheck_i]==']')
							{
								globalnamecheck_j=2;
								copyoffilebuf[globalnamecheck_i]=' ';

								if(globalnamecheck_1_buf[0]!=0)
								{
									globalnamecheck_1_buf[globalnamecheck_k]=0;

									for(globalnamecheck_j=globalnamecheck_s;globalnamecheck_j<globalnamecheck_k-globalnamecheck_s;globalnamecheck_j++)
									{
										if(globalnamecheck_1_buf[globalnamecheck_j]!=' ' 
											&& (globalnamecheck_1_buf[globalnamecheck_j] <48 || globalnamecheck_1_buf[globalnamecheck_j]>57))
										{
											globalnamecheck_j=-1;//not num
											break;
										}
									}
									if(!searchgive_falg_0(&copyoffilebuf[globalnamecheck_i],&globalnamecheck_R,
										".enable",".autorestart",".unit",".time_unit",
										".lastdotime.year",".lastdotime.month",".lastdotime.day",".lastdotime.hour",".lastdotime.minute",".lastdotime.second",
										".full_count",".real_count",".reached_times",".lastdotime",".inited","",
										"","","","","","","","","","","","","","","",""))
									{
									 
									}
									if(globalnamecheck_j>=0)
									{
										if(sscanf(globalnamecheck_1_buf,"%d",&globalnamecheck_N))
										{
										   searchgivefalg(copyoffilebuf,globalnamecheck_i,IGC_LINE_BUF_CHAR,&globalnamecheck_F,copyoffilebuf-globalnamecheck_buf_0);
										}
										else
											globalnamecheck_j=-2;
									}

									if(globalnamecheck_j<0)
									{
										 searchgivefalg(copyoffilebuf,globalnamecheck_i,IGC_LINE_BUF_CHAR,&globalnamecheck_F,copyoffilebuf-globalnamecheck_buf_0);
									}

									if(globalnamecheck_F==1)//写标志
									{
										if(globalnamecheck_j<0)//非数字标志
										{
											if(use_w_str_I_H9000_timers[use_w_times_I_H9000_timers[Itimer]][max_w_str_len-strlen(use_w_str_I_H9000_timers[Itimer])-7]==0
												&& use_w_str_I_H9000_timers[use_w_times_I_H9000_timers[Itimer]][max_w_str_len-strlen(use_w_str_I_H9000_timers[Itimer])-8]==0)
											{
												strcat(use_w_str_I_H9000_timers[Itimer],IGC_filename);
												strcat(use_w_str_I_H9000_timers[Itimer],"\n    ");
											}
											use_w_times_I_H9000_timers[Itimer]++;
										}
										else
										{
											if(use_w_str_I_H9000_timers[use_w_times_I_H9000_timers[globalnamecheck_N]][max_w_str_len-strlen(use_w_str_I_H9000_timers[globalnamecheck_N])-7]==0
												&& use_w_str_I_H9000_timers[use_w_times_I_H9000_timers[globalnamecheck_N]][max_w_str_len-strlen(use_w_str_I_H9000_timers[globalnamecheck_N])-8]==0)
											{
												strcat(use_w_str_I_H9000_timers[globalnamecheck_N],IGC_filename);
												strcat(use_w_str_I_H9000_timers[globalnamecheck_N],"\n    ");
											}
											use_w_times_I_H9000_timers[globalnamecheck_N]++;
										}
									}
									else
									{
										if(globalnamecheck_j<0)//非数字标志
										{
											use_r_times_I_H9000_timers[Itimer]++;
										}
										else
										{
											use_r_times_I_H9000_timers[globalnamecheck_N]++;
										}
									}
								}
								globalnamecheck_j=0;
								globalnamecheck_k=0;
								globalnamecheck_s=0;
							}
							else
							{
								globalnamecheck_k++;
								if(globalnamecheck_k>globalnamecheck_s)
									globalnamecheck_1_buf[globalnamecheck_k-globalnamecheck_s-1]=copyoffilebuf[globalnamecheck_i];
								
							}
						}

					}
				}


				//20110908 add
				//20110908 add
				memset(globalnamecheck_buf,0,IGC_LINE_BUF_CHAR);
				memset(globalnamecheck_1_buf,0,IGC_LINE_BUF_CHAR/4);
				sprintf(globalnamecheck_buf, "I_H9000_alarms[");
				globalnamecheck_buf_0=strstr(&copyoffilebuf[n],globalnamecheck_buf);
				if (globalnamecheck_buf_0)
				{
					globalnamecheck_i=0;
					globalnamecheck_j=0;
					globalnamecheck_k=0;
					globalnamecheck_s=0;
					for(globalnamecheck_i=n;globalnamecheck_i<IGC_LINE_BUF_CHAR-n;globalnamecheck_i++)
					{
						if(globalnamecheck_j==0 && globalnamecheck_k==0)
						{ 
							if(copyoffilebuf[globalnamecheck_i]=='[')
							{
								globalnamecheck_j=1;
								copyoffilebuf[globalnamecheck_i]=' '; 
								globalnamecheck_s=globalnamecheck_i;  
								globalnamecheck_k=globalnamecheck_i;  
							}
						}
						else
						{
							if(copyoffilebuf[globalnamecheck_i]==']')
							{
								globalnamecheck_j=2;
								copyoffilebuf[globalnamecheck_i]=' ';

								if(globalnamecheck_1_buf[0]!=0)
								{
									globalnamecheck_1_buf[globalnamecheck_k]=0;

									for(globalnamecheck_j=globalnamecheck_s;globalnamecheck_j<globalnamecheck_k-globalnamecheck_s;globalnamecheck_j++)
									{
										if(globalnamecheck_1_buf[globalnamecheck_j]!=' ' 
											&& (globalnamecheck_1_buf[globalnamecheck_j] <48 || globalnamecheck_1_buf[globalnamecheck_j]>57))
										{
											globalnamecheck_j=-1;//not num
											break;
										}
									}

									if(!searchgive_falg_0(&copyoffilebuf[globalnamecheck_i],&globalnamecheck_R,
										".ok_flag",".sn_str",".time_str",".repeatAlm",
										".igc_datetime.year",".igc_datetime.month",".igc_datetime.day",".igc_datetime.hour",".igc_datetime.minute",".igc_datetime.second",
										".logicname",".longname",".state_str",".hostname",".usrname",".message",
										"","","","","","","","","","","","","","","",""))
									{
								 
									 
									}

									if(globalnamecheck_j>=0)
									{
										if(sscanf(globalnamecheck_1_buf,"%d",&globalnamecheck_N))
										{
										   searchgivefalg(copyoffilebuf,globalnamecheck_i,IGC_LINE_BUF_CHAR,&globalnamecheck_F,copyoffilebuf-globalnamecheck_buf_0);
										}
										else
											globalnamecheck_j=-2;
									}

									if(globalnamecheck_j<0)
									{
										 searchgivefalg(copyoffilebuf,globalnamecheck_i,IGC_LINE_BUF_CHAR,&globalnamecheck_F,copyoffilebuf-globalnamecheck_buf_0);
									}

									if(globalnamecheck_F==1)//写标志
									{
										if(globalnamecheck_j<0)//非数字标志
										{
											if(use_w_str_I_H9000_alarms[use_w_times_I_H9000_alarms[Ialarms]][max_w_str_len-strlen(use_w_str_I_H9000_alarms[Ialarms])-7]==0
												&& use_w_str_I_H9000_alarms[use_w_times_I_H9000_alarms[Ialarms]][max_w_str_len-strlen(use_w_str_I_H9000_alarms[Ialarms])-8]==0)
											{
												strcat(use_w_str_I_H9000_alarms[Ialarms],IGC_filename);
												strcat(use_w_str_I_H9000_alarms[Ialarms],"\n    ");
											}
											use_w_times_I_H9000_alarms[Ialarms]++;
										}
										else
										{
											if(use_w_str_I_H9000_alarms[use_w_times_I_H9000_alarms[globalnamecheck_N]][max_w_str_len-strlen(use_w_str_I_H9000_alarms[globalnamecheck_N])-7]==0
												&& use_w_str_I_H9000_alarms[use_w_times_I_H9000_alarms[globalnamecheck_N]][max_w_str_len-strlen(use_w_str_I_H9000_alarms[globalnamecheck_N])-8]==0)
											{
												strcat(use_w_str_I_H9000_alarms[globalnamecheck_N],IGC_filename);
												strcat(use_w_str_I_H9000_alarms[globalnamecheck_N],"\n    ");
											}
											use_w_times_I_H9000_alarms[globalnamecheck_N]++;
										}
									}
									else
									{
										if(globalnamecheck_j<0)//非数字标志
										{
											use_r_times_I_H9000_alarms[Ialarms]++;
										}
										else
										{
											use_r_times_I_H9000_alarms[globalnamecheck_N]++;
										}
									}
								}
								globalnamecheck_j=0;
								globalnamecheck_k=0;
								globalnamecheck_s=0;
							}
							else
							{
								globalnamecheck_k++;
								if(globalnamecheck_k>globalnamecheck_s)
									globalnamecheck_1_buf[globalnamecheck_k-globalnamecheck_s-1]=copyoffilebuf[globalnamecheck_i];
								
							}
						}

					}
				}


				//20110908 add





			 
				if(igc_sec_start_type_buf[incfilenum][0]!=0 && sec_start_types[incfilenum]==0)
				{
					for(Tj=0;Tj<100-10;Tj++)
					{
						if(igc_sec_start_type_buf[incfilenum][Tj]==' ' || igc_sec_start_type_buf[incfilenum][Tj]==TAB)
							continue;
						
						if(sec_start_types[incfilenum]==0)
						{
							
							if(igc_sec_start_type_buf[incfilenum][Tj+1]!=':')
								continue;
						}

						if(sec_start_types[incfilenum]>0 && sec_start_types[incfilenum]<4)
						{
							if(sec_start_types[incfilenum]==1)
							{
								if(igc_sec_start_type_buf[incfilenum][Tj]>47 && igc_sec_start_type_buf[incfilenum][Tj]<58)
								{
									Tn=0;
								 
									while(1)
									{
										if(igc_sec_start_type_buf[incfilenum][Tj+Tn]>47 && igc_sec_start_type_buf[incfilenum][Tj+Tn]<58)
										{
										
											if(Tn==0)
												sec_start_timenum[incfilenum]=igc_sec_start_type_buf[incfilenum][Tj+Tn]-48;
											else
												sec_start_timenum[incfilenum]=sec_start_timenum[incfilenum]*10+igc_sec_start_type_buf[incfilenum][Tj+Tn]-48;
											Tn++;
										}
										else
											break;
									}
									Tj+=Tn;
								}
								
								if(igc_sec_start_type_buf[incfilenum][Tj] =='S' || igc_sec_start_type_buf[incfilenum][Tj] =='s')
								{
									sec_start_timetype[incfilenum]=unit_second;
									sec_start_types_reged[incfilenum]=1;
								}
								else if(igc_sec_start_type_buf[incfilenum][Tj] =='M' || igc_sec_start_type_buf[incfilenum][Tj] =='m')
								{
									sec_start_timetype[incfilenum]=unit_minute;
									sec_start_types_reged[incfilenum]=1;
								}
								else if(igc_sec_start_type_buf[incfilenum][Tj] =='H' || igc_sec_start_type_buf[incfilenum][Tj] =='h')
								{
									sec_start_timetype[incfilenum]=unit_hour;
									sec_start_types_reged[incfilenum]=1;
								}
								else if(igc_sec_start_type_buf[incfilenum][Tj] =='D' || igc_sec_start_type_buf[incfilenum][Tj] =='d')
								{
									sec_start_timetype[incfilenum]=unit_day;
									sec_start_types_reged[incfilenum]=1;
								}
								if(igc_sec_start_type_buf[incfilenum][Tj]==0)
									break;
								
							}
							else if(sec_start_types[incfilenum]==2)
							{
								for(Tn=0; Tn<(int)strlen(igc_sec_start_type_buf[incfilenum])-2-Tj; Tn++)
								{
									if(igc_sec_start_type_buf[incfilenum][Tn+Tj]<'0' || igc_sec_start_type_buf[incfilenum][Tn+Tj]>'9')
										igc_sec_start_type_buf[incfilenum][Tn+Tj]= ' ';
								}
								
								if(sscanf(&igc_sec_start_type_buf[incfilenum][Tj], "%d %d %d %d %d", &sec_start_types_month[incfilenum],  &sec_start_types_day[incfilenum], 
								 &sec_start_types_hour[incfilenum],  &sec_start_types_minute[incfilenum],  &sec_start_types_second[incfilenum])!=5)
								{
									printf("\n%s IGCGEN define format error\n",incfuc_buf[incfilenum]);
									fprintf (IGC_ferr,"\n%s IGCGEN define format error\n",incfuc_buf[incfilenum]);
									break;	     
								}
								else
								{
									sec_start_types_reged[incfilenum]=1;
										break;	     
								}
							
							}
							else if(sec_start_types[incfilenum]==3)
							{
								
							}
						}
						else
						{
							if(igc_sec_start_type_buf[incfilenum][Tj]=='P' || igc_sec_start_type_buf[incfilenum][Tj]=='p')
							{	
								sec_start_types[incfilenum]=1;
								Tj++;
							}
							else if(igc_sec_start_type_buf[incfilenum][Tj]=='T' || igc_sec_start_type_buf[incfilenum][Tj]=='t')
							{
								sec_start_types[incfilenum]=2;
								Tj++;
							}
							else if(igc_sec_start_type_buf[incfilenum][Tj]=='C' || igc_sec_start_type_buf[incfilenum][Tj]=='c')
							{	
								sec_start_types[incfilenum]=3;
								Tj++;
							}
						}
						//h9000_igc_timeer_sysuse
					}
					
					if(sec_start_types[incfilenum]==1)
					{
						if(sec_start_timetype[incfilenum]>=0 && sec_start_timetype[incfilenum]<4)
						{
							if(sec_start_timenum[incfilenum]>0 && sec_start_types[incfilenum]>0)
							{
								fprintf (IGC_fsc,"if(h9000_igc_timeer_sysuse[%d].enable==no)\n",incfilenum);
								fprintf (IGC_fsc,"{\n");
								fprintf (IGC_fsc,"h9000_igc_timeer_sysuse[%d].enable=yes;\n",incfilenum);
								fprintf (IGC_fsc,"h9000_igc_timeer_sysuse[%d].autorestart=no;\n",incfilenum);
								fprintf (IGC_fsc,"h9000_igc_timeer_sysuse[%d].unit=%d;\n",incfilenum,sec_start_timetype[incfilenum]);
								fprintf (IGC_fsc,"h9000_igc_timeer_sysuse[%d].full_count=%d;\n",incfilenum,sec_start_timenum[incfilenum]);
								fprintf (IGC_fsc,"}\n");
								fprintf (IGC_fsc,"if(h9000_igc_timeer_sysuse[%d].real_count>=h9000_igc_timeer_sysuse[%d].full_count)\n",incfilenum,incfilenum);
								fprintf (IGC_fsc,"{\n");
							}
						}
					}
					else if(sec_start_types[incfilenum]==2)
					{
						fprintf (IGC_fsc,"if((lacal_datetime_now.month==%d || %d==0) && (lacal_datetime_now.day==%d  || %d==0) && lacal_datetime_now.hour==%d && lacal_datetime_now.minute==%d)\n",
							sec_start_types_month[incfilenum],sec_start_types_month[incfilenum],
							sec_start_types_day[incfilenum],sec_start_types_day[incfilenum],
							sec_start_types_hour[incfilenum],
							sec_start_types_minute[incfilenum]);
					 	fprintf (IGC_fsc,"{\n");
						fprintf (IGC_fsc,"if(h9000_igc_char_sysuse[%d]==0)\n",incfilenum);
						fprintf (IGC_fsc,"    {\n");
						//fprintf (IGC_fsc,"    h9000_igc_char_sysuse[%d]=1;\n",incfilenum);
					}
					else if(sec_start_types[incfilenum]==3)
					{
						
					}
				}

				if(initeathincfile==0)
				{
					if(h9000_igc_cmd_num_sysuse[incfilenum]<=0)
					{
						
						sprintf(incfuc_buf[incfilenum], "  userfuc%d%s ()", incfilenum, read_buf);
						
						for(Tj=0;Tj<100;Tj++)
						{
							if(incfuc_buf[incfilenum][Tj]=='.')
								incfuc_buf[incfilenum][Tj]='_';
						}
						
						fputs("\n  void ",IGC_fsc);
						fputs(incfuc_buf[incfilenum],IGC_fsc);
						fputs("\n  {",IGC_fsc);
						
						incfilenum++;
						initeathincfile=1;
						
						if(incfilenum>1998)
							break;
					}
				}
				
				fputs("  ",IGC_fsc);
				fputs(filebuf,IGC_fsc);
		}
			
			if(initeathincfile==1)
			{
				fputs("  }",IGC_fsc);
				
				if(sec_start_types[incfilenum]==1)
				{
					if(sec_start_timetype[incfilenum]>=0 && sec_start_timetype[incfilenum]<4)
					{
						if(sec_start_timenum[incfilenum]>0)
						{
							if(igc_sec_start_type_buf[incfilenum][0]!=0)
							{
								if(sec_start_timetype[incfilenum]>=0 && sec_start_timetype[incfilenum]<4)
								{
									if(sec_start_timenum[incfilenum]>0 && sec_start_types[incfilenum]>0)
									{
										fputs("  }",IGC_fsc);
									}
								}
							}
						}
					}
				}
				else if(sec_start_types[incfilenum]==2)
				{
					if(sec_start_types_reged[incfilenum]==1)
					{ 
						fprintf (IGC_fsc,"\n    h9000_igc_char_sysuse[%d]=1;\n",incfilenum);
						fprintf (IGC_fsc,"}\n");
						
						fprintf (IGC_fsc,"else \n{\n");
						fprintf (IGC_fsc,"if(h9000_igc_char_sysuse[%d]!=0)\n",incfilenum);
						fprintf (IGC_fsc,"h9000_igc_char_sysuse[%d]=0;\n",incfilenum);
						fprintf (IGC_fsc,"}\n\n");
						fprintf (IGC_fsc,"}\n\n");
					}
				}
				else if(sec_start_types[incfilenum]==3)
				{
					
				}
			}
			fclose(IGC_fp);
			
			}
		if(IGC_file_def_linenum<1)
			printf ("\nthe def_file is empty!\n");
		if(Ffautl>0)
			printf ("\n%d err record,please see err file\n",IGC_file_def_linenum);
		

		//
		if(csoursefile_inited==0)
		{		
			fprintf (IGC_fsc,"\nint main(int argc, char *argv[])\n");
			fprintf (IGC_fsc,"{\n");
			
			
			fprintf (IGC_fsc,"  int igc_i=0;\n");
			fprintf (IGC_fsc,"  clock_t start_ct,end_ct;\n");
			
			fprintf (IGC_fsc,"  long igc_time_seclen;\n");
			fprintf (IGC_fsc,"  long temp_ts=0;\n");
			
			//fprintf (IGC_fsc,"  IGC_datetime lacal_datetime_now;\n");
			fprintf (IGC_fsc,"  int ws_ctrl_prvg,ws_ctrl_prvg_loop;\n");
			
			
			fprintf (IGC_fsc,"  #ifdef	WINDOWS_OS\n");
			fprintf (IGC_fsc,"      WSADATA		WSAData;\n");
			fprintf (IGC_fsc,"  #endif\n");
			//fprintf (IGC_fsc,"  igc_SetModuleName(\"IGC\");\n");
			
			//fprintf (IGC_fsc,"  igc_SetErrLogMode(argc, argv);\n");//20111001 cancle 
			
			
			//fprintf (IGC_fsc,"  igc_CreateLockFile(\".igc.lock\");\n");
			fprintf (IGC_fsc,"  #ifdef	WINDOWS_OS\n");
			fprintf (IGC_fsc,"  if(igc_WSAStartup()==-1)\n");
			fprintf (IGC_fsc,"  {\n");
			fprintf (IGC_fsc,"    igc_Err_log(\"Fail to initialize net!\\n\");\n");
			fprintf (IGC_fsc,"    exit(-1);\n");
			fprintf (IGC_fsc,"  }\n");
			fprintf (IGC_fsc,"  #endif\n");
			fprintf (IGC_fsc,"  if(-1==igc_MountDB(NULL))\n");
			
			fprintf (IGC_fsc,"  {\n");
			fprintf (IGC_fsc,"    igc_Err_log(\"Fail to Mount RDB\");\n");
			fprintf (IGC_fsc,"    exit(-1);\n");
			fprintf (IGC_fsc,"  }\n");
			fprintf (IGC_fsc,"  if(igc_lan_out_init()==-1)\n");
			fprintf (IGC_fsc,"  {\n");		
			fprintf (IGC_fsc,"    igc_Err_log(\"Fail to init lan_out\");\n");					
			fprintf (IGC_fsc,"    exit(-1);\n");
			fprintf (IGC_fsc,"  }\n");
			
			
			fprintf (IGC_fsc,"  if(igc_InterLockFileLoad()==-1)\n");
			fprintf (IGC_fsc,"  {\n");		
			fprintf (IGC_fsc,"    igc_Err_log(\"Fail to igc_InterLockFileLoad\");\n");					
			fprintf (IGC_fsc,"    exit(-1);\n");
			fprintf (IGC_fsc,"  }\n");
			
			fprintf (IGC_fsc,"  if(IGC_Init_RECORD_IPC()==-1)\n");
			fprintf (IGC_fsc,"  {\n");
			fprintf (IGC_fsc,"    igc_Err_log(\"Fail to IGC_Init_RECORD_IPC\");\n");
			fprintf (IGC_fsc,"    exit(-1);\n");
			fprintf (IGC_fsc,"  }\n");
			
			
			
			fprintf (IGC_fsc,"  if(-1==igc_InitStrTextDefine())\n");
			fprintf (IGC_fsc,"  {\n");
			fprintf (IGC_fsc,"  printf(\"Fail to igc_InitStrTextDefine\");\n");	
			fprintf (IGC_fsc,"  exit(-1);\n");
			fprintf (IGC_fsc,"  }\n");
			
			fprintf (IGC_fsc,"  init();\n");
			fprintf (IGC_fsc,"   ws_ctrl_prvg=-1;\n");	
			fprintf (IGC_fsc,"   ws_ctrl_prvg_loop=200;\n");
			

		//	fprintf (IGC_fsc,"   if(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)h9000_igc_cmd_op_fuc,NULL,0,&h9000_igc_starttype_cmd_fuc_pam)==NULL) \n");						
		//	fprintf (IGC_fsc,"   {\n");	
		//	fprintf (IGC_fsc,"   	printf(\"Create h9000_igc_cmd_op_fuc Thread() error!\\n\");\n");	
		//	fprintf (IGC_fsc,"   	exit(-1);\n");	   
		//	fprintf (IGC_fsc,"   }\n");	



			fprintf (IGC_fsc,"  for(;;)\n");
			
			fprintf (IGC_fsc,"  {\n");
			
			fprintf (IGC_fsc,"  if(ws_ctrl_prvg_loop==200)\n");	
			fprintf (IGC_fsc,"  	ws_ctrl_prvg=IGC_GetCurentHostCtrlPrvg(1);\n");	
			fprintf (IGC_fsc,"  else\n");	
			fprintf (IGC_fsc,"  	ws_ctrl_prvg=IGC_GetCurentHostCtrlPrvg(0);\n");	
			fprintf (IGC_fsc,"  ws_ctrl_prvg_loop++;\n");	
			fprintf (IGC_fsc,"  if(ws_ctrl_prvg_loop>10)\n");	
			fprintf (IGC_fsc,"  {\n");	
			fprintf (IGC_fsc,"  	ws_ctrl_prvg_loop=0;\n");	
			//fprintf (IGC_fsc,"  	if(ws_ctrl_prvg!=1) printf(\"\\nthis is not the main host,igc is standby\\n\");\n");	
			fprintf (IGC_fsc,"  }\n");	
			fprintf (IGC_fsc,"  if(ws_ctrl_prvg!=1)\n");	
			fprintf (IGC_fsc,"  {\n");	
			fprintf (IGC_fsc,"  	if(ws_ctrl_prvg!=1) printf(\"\\nthis is not the main host,igc is standby\\n\");\n");	
			fprintf (IGC_fsc,"  	sleep_second_igclib(10);\n");	
			fprintf (IGC_fsc,"  	continue;\n");	
			fprintf (IGC_fsc,"  }\n");	
			
			
			
			fprintf (IGC_fsc,"	   start_ct = clock();\n");
			
			csoursefile_inited=1;
				}
				
		if(csoursefile_inited==1)
		{
			fprintf (IGC_fsc,"	  lacal_datetime_now=get_lacal_datetime_now();\n");
		
			fprintf (IGC_fsc,"	  for(igc_i=0;igc_i<Itimer;igc_i++)\n");
			fprintf (IGC_fsc,"	  {\n");
			fprintf (IGC_fsc,"		  if(I_H9000_timers[igc_i].enable==yes)\n");
			fprintf (IGC_fsc,"		  {\n");
			fprintf (IGC_fsc,"			  if(I_H9000_timers[igc_i].inited==0)\n");
			fprintf (IGC_fsc,"			  {\n");
			fprintf (IGC_fsc,"				  I_H9000_timers[igc_i].inited=1;\n");
			fprintf (IGC_fsc,"				  I_H9000_timers[igc_i].lastdotime.day=lacal_datetime_now.day;\n");
			fprintf (IGC_fsc,"				  I_H9000_timers[igc_i].lastdotime.hour=lacal_datetime_now.hour;\n");
			fprintf (IGC_fsc,"				  I_H9000_timers[igc_i].lastdotime.minute=lacal_datetime_now.minute;\n");
			fprintf (IGC_fsc,"				  I_H9000_timers[igc_i].lastdotime.month=lacal_datetime_now.month;\n");
			fprintf (IGC_fsc,"				  I_H9000_timers[igc_i].lastdotime.second=lacal_datetime_now.second;\n");
			fprintf (IGC_fsc,"				  I_H9000_timers[igc_i].lastdotime.year=lacal_datetime_now.year;\n");
			fprintf (IGC_fsc,"				  I_H9000_timers[igc_i].real_count=0;\n");
			fprintf (IGC_fsc,"				  I_H9000_timers[igc_i].reached_times=0;\n");
			fprintf (IGC_fsc,"			  }\n");
			fprintf (IGC_fsc,"			  else\n");
			fprintf (IGC_fsc,"			  {\n");
			fprintf (IGC_fsc,"				  temp_ts=total_time_seconds(I_H9000_timers[igc_i].lastdotime,lacal_datetime_now);\n");
			fprintf (IGC_fsc,"				  switch(I_H9000_timers[igc_i].unit)\n");
			fprintf (IGC_fsc,"				  {\n");
			fprintf (IGC_fsc,"				  case unit_second:\n");
			fprintf (IGC_fsc,"					  break;\n");
			fprintf (IGC_fsc,"				  case unit_minute:\n");
			fprintf (IGC_fsc,"					  temp_ts=temp_ts/60;\n");
			fprintf (IGC_fsc,"					  break;\n");
			fprintf (IGC_fsc,"				  case unit_hour:\n");
			fprintf (IGC_fsc,"					  temp_ts=temp_ts/3600;\n");
			fprintf (IGC_fsc,"					  break;\n");
			fprintf (IGC_fsc,"				  case unit_day:\n");
			fprintf (IGC_fsc,"					  temp_ts=temp_ts/86400;\n");
			fprintf (IGC_fsc,"					  break;\n");
			fprintf (IGC_fsc,"				  default:\n");
			fprintf (IGC_fsc,"					  break;\n");
			fprintf (IGC_fsc,"				  }\n");
			fprintf (IGC_fsc,"				  if(temp_ts<0)\n");
			fprintf (IGC_fsc,"					  temp_ts=0-temp_ts;\n");
			fprintf (IGC_fsc,"				  I_H9000_timers[igc_i].real_count=(unsigned int)temp_ts;\n");
			fprintf (IGC_fsc,"				  if(I_H9000_timers[igc_i].real_count>=I_H9000_timers[igc_i].full_count)\n");
			fprintf (IGC_fsc,"				  {\n");
			fprintf (IGC_fsc,"					  if(I_H9000_timers[igc_i].autorestart==yes)\n");
			fprintf (IGC_fsc,"					  {	\n");
			fprintf (IGC_fsc,"						  I_H9000_timers[igc_i].real_count=0;\n");
			fprintf (IGC_fsc,"						  I_H9000_timers[igc_i].reached_times++;\n");
			fprintf (IGC_fsc,"					  }\n");
			fprintf (IGC_fsc,"					  else\n");
			fprintf (IGC_fsc,"					  {\n");
			fprintf (IGC_fsc,"						  I_H9000_timers[igc_i].enable=no;\n");
			fprintf (IGC_fsc,"					  }\n");
			fprintf (IGC_fsc,"					  I_H9000_timers[igc_i].inited=0;\n");
			fprintf (IGC_fsc,"				  }\n");
			fprintf (IGC_fsc,"			  }\n");
			fprintf (IGC_fsc,"		  }\n");
			fprintf (IGC_fsc,"	  }\n");


			fprintf (IGC_fsc,"	  for(igc_i=0;igc_i<Itimer_sysuse;igc_i++)\n");
			fprintf (IGC_fsc,"	  {\n");
			fprintf (IGC_fsc,"		  if(h9000_igc_timeer_sysuse[igc_i].enable==yes)\n");
			fprintf (IGC_fsc,"		  {\n");
			fprintf (IGC_fsc,"			  if(h9000_igc_timeer_sysuse[igc_i].inited==0)\n");
			fprintf (IGC_fsc,"			  {\n");
			fprintf (IGC_fsc,"				  h9000_igc_timeer_sysuse[igc_i].inited=1;\n");
			fprintf (IGC_fsc,"				  h9000_igc_timeer_sysuse[igc_i].lastdotime.day=lacal_datetime_now.day;\n");
			fprintf (IGC_fsc,"				  h9000_igc_timeer_sysuse[igc_i].lastdotime.hour=lacal_datetime_now.hour;\n");
			fprintf (IGC_fsc,"				  h9000_igc_timeer_sysuse[igc_i].lastdotime.minute=lacal_datetime_now.minute;\n");
			fprintf (IGC_fsc,"				  h9000_igc_timeer_sysuse[igc_i].lastdotime.month=lacal_datetime_now.month;\n");
			fprintf (IGC_fsc,"				  h9000_igc_timeer_sysuse[igc_i].lastdotime.second=lacal_datetime_now.second;\n");
			fprintf (IGC_fsc,"				  h9000_igc_timeer_sysuse[igc_i].lastdotime.year=lacal_datetime_now.year;\n");
			fprintf (IGC_fsc,"				  h9000_igc_timeer_sysuse[igc_i].real_count=0;\n");
			fprintf (IGC_fsc,"				  h9000_igc_timeer_sysuse[igc_i].reached_times=0;\n");
			fprintf (IGC_fsc,"			  }\n");
			fprintf (IGC_fsc,"			  else\n");
			fprintf (IGC_fsc,"			  {\n");
			fprintf (IGC_fsc,"				  temp_ts=total_time_seconds(h9000_igc_timeer_sysuse[igc_i].lastdotime,lacal_datetime_now);\n");
			fprintf (IGC_fsc,"				  switch(h9000_igc_timeer_sysuse[igc_i].unit)\n");
			fprintf (IGC_fsc,"				  {\n");
			fprintf (IGC_fsc,"				  case unit_second:\n");
			fprintf (IGC_fsc,"					  break;\n");
			fprintf (IGC_fsc,"				  case unit_minute:\n");
			fprintf (IGC_fsc,"					  temp_ts=temp_ts/60;\n");
			fprintf (IGC_fsc,"					  break;\n");
			fprintf (IGC_fsc,"				  case unit_hour:\n");
			fprintf (IGC_fsc,"					  temp_ts=temp_ts/3600;\n");
			fprintf (IGC_fsc,"					  break;\n");
			fprintf (IGC_fsc,"				  case unit_day:\n");
			fprintf (IGC_fsc,"					  temp_ts=temp_ts/86400;\n");
			fprintf (IGC_fsc,"					  break;\n");
			fprintf (IGC_fsc,"				  default:\n");
			fprintf (IGC_fsc,"					  break;\n");
			fprintf (IGC_fsc,"				  }\n");
			fprintf (IGC_fsc,"				  if(temp_ts<0)\n");
			fprintf (IGC_fsc,"					  temp_ts=0-temp_ts;\n");
			fprintf (IGC_fsc,"				  h9000_igc_timeer_sysuse[igc_i].real_count=(unsigned int)temp_ts;\n");
			fprintf (IGC_fsc,"				  if(h9000_igc_timeer_sysuse[igc_i].real_count>=h9000_igc_timeer_sysuse[igc_i].full_count)\n");
			fprintf (IGC_fsc,"				  {\n");
			fprintf (IGC_fsc,"					  if(h9000_igc_timeer_sysuse[igc_i].autorestart==yes)\n");
			fprintf (IGC_fsc,"					  {	\n");
			fprintf (IGC_fsc,"						  h9000_igc_timeer_sysuse[igc_i].real_count=0;\n");
			fprintf (IGC_fsc,"						  h9000_igc_timeer_sysuse[igc_i].reached_times++;\n");
			fprintf (IGC_fsc,"					  }\n");
			fprintf (IGC_fsc,"					  else\n");
			fprintf (IGC_fsc,"					  {\n");
			fprintf (IGC_fsc,"						  h9000_igc_timeer_sysuse[igc_i].enable=no;\n");
			fprintf (IGC_fsc,"					  }\n");
			fprintf (IGC_fsc,"					  h9000_igc_timeer_sysuse[igc_i].inited=0;\n");
			fprintf (IGC_fsc,"				  }\n");
			fprintf (IGC_fsc,"			  }\n");
			fprintf (IGC_fsc,"		  }\n");
			fprintf (IGC_fsc,"	  }\n");


			for(Tj=0;Tj<incfilenum;Tj++)
			{ 
				fprintf (IGC_fsc,"\n	  ");
				fprintf (IGC_fsc,incfuc_buf[Tj]);
				fprintf (IGC_fsc,";\n");
				
			}

			fprintf (IGC_fsc,"	  sleep_millisecond_igclib(loop_sleep_minisec);\n");
			fprintf (IGC_fsc,"	  end_ct = clock();\n");
			fprintf (IGC_fsc,"	  run_count_for_avg++;\n");
			
			fprintf (IGC_fsc,"	  if(run_count<99999999)\n");
			fprintf (IGC_fsc,"	      run_count++;\n");
			fprintf (IGC_fsc,"	  last_run_millisecond =(unsigned int)((end_ct-start_ct)*1000/CLOCKS_PER_SEC);\n");
			//fprintf (IGC_fsc,"	  last_run_millisecond=end_ct-start_ct;\n");
			//fprintf (IGC_fsc,"	  last_run_millisecond=last_run_millisecond*1000/igc_CLK_TCK;\n");
			fprintf (IGC_fsc,"	  if(min_run_millisecond>last_run_millisecond)\n");
			fprintf (IGC_fsc,"		  min_run_millisecond=last_run_millisecond;\n");
			fprintf (IGC_fsc,"	  if(max_run_millisecond<last_run_millisecond)\n");
			fprintf (IGC_fsc,"		  max_run_millisecond=last_run_millisecond;\n");
			fprintf (IGC_fsc,"	  all_run_millisecond+=last_run_millisecond;\n");
			fprintf (IGC_fsc,"	  if(run_count_for_avg>0)\n");
			fprintf (IGC_fsc,"		  avg_run_millisecond=all_run_millisecond/run_count_for_avg;\n");
			fprintf (IGC_fsc,"	  if(all_run_millisecond>99999999)\n");
			fprintf (IGC_fsc,"	  {\n");
			fprintf (IGC_fsc,"		  run_count_for_avg=0;\n");
			fprintf (IGC_fsc,"		  last_run_millisecond=0;\n");
			fprintf (IGC_fsc,"		  min_run_millisecond=0;\n");
			fprintf (IGC_fsc,"		  max_run_millisecond=0;\n");
			fprintf (IGC_fsc,"		  all_run_millisecond=0;\n");
			fprintf (IGC_fsc,"		  avg_run_millisecond=0;\n");
			fprintf (IGC_fsc,"	  }\n");
		 
			fprintf (IGC_fsc,"  }\n");
			fprintf (IGC_fsc,"  igc_UnMountDB();\n");
			fprintf (IGC_fsc,"  igc_InterLockFree();\n");
			
			fprintf (IGC_fsc,"}\n");



			//20110905 add
			
			fprintf (IGC_fsc,"  void h9000_igc_cmd_opfuc_do(int cmdnum)\n");
			fprintf (IGC_fsc,"  {\n");
			fprintf (IGC_fsc,"    int i=0;\n");
			fprintf (IGC_fsc,"  	switch(cmdnum)\n");
			fprintf (IGC_fsc,"  	{\n");
			
			for(i=0;i<Itimer_sysuse;i++)
			{
				if(h9000_igc_cmd_num_sysuse[i]>0)
				{
					fprintf (IGC_fsc,"  {\n");
					fprintf (IGC_fsc,"  case %d:\n",i);
					fprintf (IGC_fsc,"  		%s ",incfuc_buf[i]);
					fprintf (IGC_fsc,"  	break;\n");
					
					fprintf (IGC_fsc,"  	}\n");
				}
			}
			
			fprintf (IGC_fsc,"  	default:\n");
			fprintf (IGC_fsc,"  		printf(\"h9000_igc_cmd_opfuc_do Err,no this  cmdNum=%d\",cmdnum);\n");
			fprintf (IGC_fsc," 		}\n");
			fprintf (IGC_fsc,"   }\n");
			
			
			fprintf (IGC_fsc,"  void h9000_igc_cmd_op_fuc()\n");
			fprintf (IGC_fsc,"  {\n");	
			fprintf (IGC_fsc,"      int i=0;\n");
			fprintf (IGC_fsc,"  	int get_igc_cmd_num_sysuse=-1;\n");
			fprintf (IGC_fsc,"  	for(;;)\n");
			fprintf (IGC_fsc,"  	{\n");
			fprintf (IGC_fsc,"  		sleep_second_igclib(1000);\n");
			
			fprintf (IGC_fsc,"  		//get_igc_cmd_num_sysuse=......\n");
			
			fprintf (IGC_fsc,"  	     if(get_igc_cmd_num_sysuse>0)\n");
			fprintf (IGC_fsc,"  		{\n");
			fprintf (IGC_fsc,"  			for(i=0;i<Itimer_sysuse;i++)\n");
			fprintf (IGC_fsc,"              {\n");
			fprintf (IGC_fsc,"  	            if(h9000_igc_cmd_num_sysuse[i]==get_igc_cmd_num_sysuse)\n");
			fprintf (IGC_fsc,"                  {\n");
			fprintf (IGC_fsc,"                     h9000_igc_cmd_opfuc_do(get_igc_cmd_num_sysuse);\n");
			fprintf (IGC_fsc,"                      break;\n");
			fprintf (IGC_fsc,"                  }\n");
			fprintf (IGC_fsc,"              }\n");
			fprintf (IGC_fsc,"          }\n");
			fprintf (IGC_fsc,"      }\n");
			fprintf (IGC_fsc,"  }\n");
			
		}
		else
		{
			fprintf (IGC_ferr,"\ngenerete csourcefile err,file is null");
		}
		


		//printf("\n按任意键继续......\n");
		//scanf("%s", &san_char);
	    //ShellExecute("this","open","D:\\home\\ems\\system\\123copy_all_bin.bat",NULL,NULL,SW_SHOW);	
		//printf("NMAKE /f \"D:\\home\\ems\\system\\pas\\v4.0a\\src\\IGC.mak\" CFG=\"IGC - Win32 DEBUG\"");
 
#ifdef	    WINDOWS_OS

		sprintf(igc_debugpath, "%s%s", IGC_DEBUGPATH,"igc.exe");
        if(remove(igc_debugpath)==-1)
		{ 
			if(igc_access(igc_debugpath,0)<0) 
			{
				printf("\nIGC_GEN warning :can not remove %s\n",igc_debugpath);
				fprintf (IGC_ferr,"\nIGC_GEN:can not remove %s\n",igc_debugpath);
			}
			 
		}
	 
		sprintf(igc_debugpath, "%s%s", IGC_DEBUGPATH,"igc.ilk");
        if(remove(igc_debugpath)==-1)
		{
			if(igc_access(igc_debugpath,0)<0)  
			{
				printf("\nIGC_GEN warning :can not remove %s\n",igc_debugpath);
				fprintf (IGC_ferr,"\nIGC_GEN:can not remove %s\n",igc_debugpath);
			}
		}
		sprintf(igc_debugpath, "%s%s", IGC_DEBUGPATH,"igc.obj");
        if(remove(igc_debugpath)==-1)
		{
			if(igc_access(igc_debugpath,0)<0)  
			{
				printf("\nIGC_GEN warning :can not remove %s\n",igc_debugpath);
				fprintf (IGC_ferr,"\nIGC_GEN:can not remove %s\n",igc_debugpath);
			}
		}
#else
		sprintf(igc_debugpath, "%s%s", IGC_DEBUGPATH,"igc.dxe");
        if(remove(igc_debugpath)==-1)
		{ 
			if(igc_access(igc_debugpath,0)<0) 
			{
			//	printf("\nIGC_GEN:can not remove %s\n",igc_debugpath);
				fprintf (IGC_ferr,"\nIGC_GEN warning :can not remove %s\n",igc_debugpath);
			}
			
		}
#endif

		fclose(IGC_fpdef);
		fclose(IGC_fsc);
		fclose(IGC_ferr);

		fprintf(IGC_globalvariables_fp,"\n");
		for(Tn=0;Tn<Ifloat;Tn++)
		{
			if(use_w_times_I_floats[Tn]>1)
			{
				fprintf(IGC_globalvariables_fp,"\n★★!warning Write I_floats[%d] appear times=%d ",Tn,use_w_times_I_floats[Tn]);
				if(use_w_str_I_floats[Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_floats[Tn]);
			}
		}
		
		if(use_w_times_I_floats[Ifloat]>1)
		{	
			fprintf(IGC_globalvariables_fp,"\n★★!warning Write I_floats[?X] appear times=%d \n",use_w_times_I_floats[Ifloat]);
			for(Tn=0;Tn<use_w_times_I_floats[Ifloat];Tn++)
			{
				if(use_w_str_I_floats[Ifloat+Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_floats[Ifloat+Tn]);
			}
		}
		
		
		for(Tn=0;Tn<Idouble;Tn++)
		{
			if(use_w_times_I_doubles[Tn]>1)
			{
				fprintf(IGC_globalvariables_fp,"\n★★!warning Write I_doubles[%d] appear times=%d ",Tn,use_w_times_I_doubles[Tn]);
				if(use_w_str_I_doubles[Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_doubles[Tn]);
			}
		}
		
		if(use_w_times_I_doubles[Idouble]>1)
		{	
			fprintf(IGC_globalvariables_fp,"\n★★!warning Write I_doubles[?X] appear times=%d \n",use_w_times_I_doubles[Idouble]);
			for(Tn=0;Tn<use_w_times_I_doubles[Idouble];Tn++)
			{
				if(use_w_str_I_doubles[Idouble+Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_doubles[Idouble+Tn]);
			}
		}

		
		for(Tn=0;Tn<Iint;Tn++)
		{
			if(use_w_times_I_ints[Tn]>1)
			{
				fprintf(IGC_globalvariables_fp,"\n★★!warning Write I_ints[%d] appear times=%d ",Tn,use_w_times_I_ints[Tn]);
				if(use_w_str_I_ints[Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_ints[Tn]);
			}
		}
		
		if(use_w_times_I_ints[Iint]>1)
		{	
			fprintf(IGC_globalvariables_fp,"\n★★!warning Write I_ints[?X] appear times=%d \n",use_w_times_I_ints[Iint]);
			for(Tn=0;Tn<use_w_times_I_ints[Iint];Tn++)
			{
				if(use_w_str_I_ints[Iint+Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_ints[Iint+Tn]);
			}
		}

		
		for(Tn=0;Tn<Iuint;Tn++)
		{
			if(use_w_times_I_uints[Tn]>1)
			{
				fprintf(IGC_globalvariables_fp,"\n★★!warning Write I_uints[%d] appear times=%d ",Tn,use_w_times_I_uints[Tn]);
				if(use_w_str_I_uints[Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_uints[Tn]);
			}
		}
		
		if(use_w_times_I_uints[Iuint]>1)
		{	
			fprintf(IGC_globalvariables_fp,"\n★★!warning Write I_uints[?X] appear times=%d \n",use_w_times_I_uints[Iuint]);
			for(Tn=0;Tn<use_w_times_I_uints[Iuint];Tn++)
			{
				if(use_w_str_I_uints[Iuint+Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_uints[Iuint+Tn]);
			}
		}

		
		for(Tn=0;Tn<Ilong;Tn++)
		{
			if(use_w_times_I_longs[Tn]>1)
			{
				fprintf(IGC_globalvariables_fp,"\n★★!warning Write I_longs[%d] appear times=%d ",Tn,use_w_times_I_longs[Tn]);
				if(use_w_str_I_longs[Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_longs[Tn]);
			}
		}
		
		if(use_w_times_I_longs[Ilong]>1)
		{	
			fprintf(IGC_globalvariables_fp,"\n★★!warning Write I_longs[?X] appear times=%d \n",use_w_times_I_longs[Ilong]);
			for(Tn=0;Tn<use_w_times_I_longs[Ilong];Tn++)
			{
				if(use_w_str_I_longs[Ilong+Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_longs[Ilong+Tn]);
			}
		}
		
		for(Tn=0;Tn<Iulong;Tn++)
		{
			if(use_w_times_I_ulongs[Tn]>1)
			{
				fprintf(IGC_globalvariables_fp,"\n★★!warning Write I_ulongs[%d] appear times=%d ",Tn,use_w_times_I_ulongs[Tn]);
				if(use_w_str_I_ulongs[Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_ulongs[Tn]);
			}
		}
		
		if(use_w_times_I_ulongs[Iulong]>1)
		{	
			fprintf(IGC_globalvariables_fp,"\n★★!warning Write I_ulongs[?X] appear times=%d \n",use_w_times_I_ulongs[Iulong]);
			for(Tn=0;Tn<use_w_times_I_ulongs[Iulong];Tn++)
			{
				if(use_w_str_I_ulongs[Iulong+Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_ulongs[Iulong+Tn]);
			}
		}
		
		for(Tn=0;Tn<Ibool;Tn++)
		{
			if(use_w_times_I_bools[Tn]>1)
			{
				fprintf(IGC_globalvariables_fp,"\n★★!warning Write I_bools[%d] appear times=%d ",Tn,use_w_times_I_bools[Tn]);
				if(use_w_str_I_bools[Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_bools[Tn]);
			}
		}
		
		if(use_w_times_I_bools[Ibool]>1)
		{	
			fprintf(IGC_globalvariables_fp,"\n★★!warning Write I_bools[?X] appear times=%d \n",use_w_times_I_bools[Ibool]);
			for(Tn=0;Tn<use_w_times_I_bools[Ibool];Tn++)
			{
				if(use_w_str_I_bools[Ibool+Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_bools[Ibool+Tn]);
			}
		}
		
		for(Tn=0;Tn<Ishort;Tn++)
		{
			if(use_w_times_I_shorts[Tn]>1)
			{
				fprintf(IGC_globalvariables_fp,"\n★★!warning Write I_shorts[%d] appear times=%d ",Tn,use_w_times_I_shorts[Tn]);
				if(use_w_str_I_shorts[Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_shorts[Tn]);
			}
		}
		
		if(use_w_times_I_shorts[Ishort]>1)
		{	
			fprintf(IGC_globalvariables_fp,"\n★★!warning Write I_shorts[?X] appear times=%d \n",use_w_times_I_shorts[Ishort]);
			for(Tn=0;Tn<use_w_times_I_shorts[Ishort];Tn++)
			{
				if(use_w_str_I_shorts[Ishort+Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_shorts[Ishort+Tn]);
			}
		}

		
		for(Tn=0;Tn<Iushort;Tn++)
		{
			if(use_w_times_I_ushorts[Tn]>1)
			{
				fprintf(IGC_globalvariables_fp,"\n★★!warning Write I_ushorts[%d] appear times=%d ",Tn,use_w_times_I_ushorts[Tn]);
				if(use_w_str_I_ushorts[Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_ushorts[Tn]);
			}
		}
		
		if(use_w_times_I_ushorts[Iushort]>1)
		{	
			fprintf(IGC_globalvariables_fp,"\n★★!warning Write I_ushorts[?X] appear times=%d \n",use_w_times_I_ushorts[Iushort]);
			for(Tn=0;Tn<use_w_times_I_ushorts[Iushort];Tn++)
			{
				if(use_w_str_I_ushorts[Iushort+Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_ushorts[Iushort+Tn]);
			}
		}
		
		for(Tn=0;Tn<Ichar;Tn++)
		{
			if(use_w_times_I_chars[Tn]>1)
			{
				fprintf(IGC_globalvariables_fp,"\n★★!warning Write I_chars[%d] appear times=%d ",Tn,use_w_times_I_chars[Tn]);
				if(use_w_str_I_chars[Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_chars[Tn]);
			}
		}
		
		if(use_w_times_I_chars[Ichar]>1)
		{	
			fprintf(IGC_globalvariables_fp,"\n★★!warning Write I_chars[?X] appear times=%d \n",use_w_times_I_chars[Ichar]);
			for(Tn=0;Tn<use_w_times_I_chars[Ichar];Tn++)
			{
				if(use_w_str_I_chars[Ichar+Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_chars[Ichar+Tn]);
			}
		}
		
		for(Tn=0;Tn<Iuchar;Tn++)
		{
			if(use_w_times_I_uchars[Tn]>1)
			{
				fprintf(IGC_globalvariables_fp,"\n★★!warning Write I_uchars[%d] appear times=%d ",Tn,use_w_times_I_uchars[Tn]);
				if(use_w_str_I_uchars[Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_uchars[Tn]);
			}
		}
		
		if(use_w_times_I_uchars[Iuchar]>1)
		{	
			fprintf(IGC_globalvariables_fp,"\n★★!warning Write I_uchars[?X] appear times=%d \n",use_w_times_I_uchars[Iuchar]);
			for(Tn=0;Tn<use_w_times_I_uchars[Iuchar];Tn++)
			{
				if(use_w_str_I_uchars[Iuchar+Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_uchars[Iuchar+Tn]);
			}
		}
		
		for(Tn=0;Tn<Idatetime;Tn++)
		{
			if(use_w_times_I_datetimes[Tn]>1)
			{
				fprintf(IGC_globalvariables_fp,"\n★★!warning Write I_datetimes[%d] appear times=%d ",Tn,use_w_times_I_datetimes[Tn]);
				if(use_w_str_I_datetimes[Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_datetimes[Tn]);
			}
		}
		
		if(use_w_times_I_datetimes[Idatetime]>1)
		{	
			fprintf(IGC_globalvariables_fp,"\n★★!warning Write I_datetimes[?X] appear times=%d \n",use_w_times_I_datetimes[Idatetime]);
			for(Tn=0;Tn<use_w_times_I_datetimes[Idatetime];Tn++)
			{
				if(use_w_str_I_datetimes[Idatetime+Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_datetimes[Idatetime+Tn]);
			}
		}

		
		for(Tn=0;Tn<IH9000_IGC_data;Tn++)
		{
			if(use_w_times_I_H9000_IGC_datas[Tn]>1)
			{
				fprintf(IGC_globalvariables_fp,"\n★★!warning Write I_H9000_IGC_datas[%d] appear times=%d ",Tn,use_w_times_I_H9000_IGC_datas[Tn]);
				if(use_w_str_I_H9000_IGC_datas[Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_H9000_IGC_datas[Tn]);
			}
		}
		
		if(use_w_times_I_H9000_IGC_datas[IH9000_IGC_data]>1)
		{	
			fprintf(IGC_globalvariables_fp,"\n★★!warning Write I_H9000_IGC_datas[?X] appear times=%d \n",use_w_times_I_H9000_IGC_datas[IH9000_IGC_data]);
			for(Tn=0;Tn<use_w_times_I_H9000_IGC_datas[IH9000_IGC_data];Tn++)
			{
				if(use_w_str_I_H9000_IGC_datas[IH9000_IGC_data+Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_H9000_IGC_datas[IH9000_IGC_data+Tn]);
			}
		}

		
		for(Tn=0;Tn<IH9000_IGC_IND_data;Tn++)
		{
			if(use_w_times_I_H9000_IGC_IND_datas[Tn]>1)
			{
				fprintf(IGC_globalvariables_fp,"\n★★!warning Write I_H9000_IGC_IND_datas[%d] appear times=%d ",Tn,use_w_times_I_H9000_IGC_IND_datas[Tn]);
				if(use_w_str_I_H9000_IGC_IND_datas[Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_H9000_IGC_IND_datas[Tn]);
			}
		}
		
		if(use_w_times_I_H9000_IGC_IND_datas[IH9000_IGC_IND_data]>1)
		{	
			fprintf(IGC_globalvariables_fp,"\n★★!warning Write I_H9000_IGC_IND_datas[?X] appear times=%d \n",use_w_times_I_H9000_IGC_IND_datas[IH9000_IGC_IND_data]);
			for(Tn=0;Tn<use_w_times_I_H9000_IGC_IND_datas[IH9000_IGC_IND_data];Tn++)
			{
				if(use_w_str_I_H9000_IGC_IND_datas[IH9000_IGC_IND_data+Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_H9000_IGC_IND_datas[IH9000_IGC_IND_data+Tn]);
			}
		}
		
		for(Tn=0;Tn<IH9000_IGC_ANA_data;Tn++)
		{
			if(use_w_times_I_H9000_IGC_ANA_datas[Tn]>1)
			{
				fprintf(IGC_globalvariables_fp,"\n★★!warning Write I_H9000_IGC_ANA_datas[%d] appear times=%d ",Tn,use_w_times_I_H9000_IGC_ANA_datas[Tn]);
				if(use_w_str_I_H9000_IGC_ANA_datas[Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_H9000_IGC_ANA_datas[Tn]);
			}
		}
		
		if(use_w_times_I_H9000_IGC_ANA_datas[IH9000_IGC_ANA_data]>1)
		{	
			fprintf(IGC_globalvariables_fp,"\n★★!warning Write I_H9000_IGC_ANA_datas[?X] appear times=%d \n",use_w_times_I_H9000_IGC_ANA_datas[IH9000_IGC_ANA_data]);
			for(Tn=0;Tn<use_w_times_I_H9000_IGC_ANA_datas[IH9000_IGC_ANA_data];Tn++)
			{
				if(use_w_str_I_H9000_IGC_ANA_datas[IH9000_IGC_ANA_data+Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_H9000_IGC_ANA_datas[IH9000_IGC_ANA_data+Tn]);
			}
		}
		
		for(Tn=0;Tn<Itimer;Tn++)
		{
			if(use_w_times_I_H9000_timers[Tn]>1)
			{
				fprintf(IGC_globalvariables_fp,"\n★★!warning Write I_H9000_timers[%d] appear times=%d ",Tn,use_w_times_I_H9000_timers[Tn]);
				if(use_w_str_I_H9000_timers[Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_H9000_timers[Tn]);
			}
		}
		
		if(use_w_times_I_H9000_timers[Itimer]>1)
		{	
			fprintf(IGC_globalvariables_fp,"\n★★!warning Write I_H9000_timers[?X] appear times=%d \n",use_w_times_I_H9000_timers[Itimer]);
			for(Tn=0;Tn<use_w_times_I_H9000_timers[Itimer];Tn++)
			{
				if(use_w_str_I_H9000_timers[Itimer+Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_H9000_timers[Itimer+Tn]);
			}
		}
		
		for(Tn=0;Tn<Ialarms;Tn++)
		{
			if(use_w_times_I_H9000_alarms[Tn]>1)
			{
				fprintf(IGC_globalvariables_fp,"\n★★!warning Write I_H9000_alarms[%d] appear times=%d ",Tn,use_w_times_I_H9000_alarms[Tn]);
				if(use_w_str_I_H9000_alarms[Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_H9000_alarms[Tn]);
			}
		}
		
		if(use_w_times_I_H9000_alarms[Ialarms]>1)
		{	
			fprintf(IGC_globalvariables_fp,"\n★★!warning Write I_H9000_alarms[?X] appear times=%d \n",use_w_times_I_H9000_alarms[Ialarms]);
			for(Tn=0;Tn<use_w_times_I_H9000_alarms[Ialarms];Tn++)
			{
				if(use_w_str_I_H9000_alarms[Ialarms+Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_H9000_alarms[Ialarms+Tn]);
			}
		}
		
////
		fprintf(IGC_globalvariables_fp,"\n");
		for(Tn=0;Tn<Ifloat;Tn++)
		{
			if(use_w_times_I_floats[Tn]==1)
			{
				fprintf(IGC_globalvariables_fp,"\n★Write I_floats[%d] appear times=%d ",Tn,use_w_times_I_floats[Tn]);
				if(use_w_str_I_floats[Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_floats[Tn]);
			}
		}
		
		if(use_w_times_I_floats[Ifloat]==1)
		{	
			fprintf(IGC_globalvariables_fp,"\n★Write I_floats[?X] appear times=%d \n",use_w_times_I_floats[Ifloat]);
			for(Tn=0;Tn<use_w_times_I_floats[Ifloat];Tn++)
			{
				if(use_w_str_I_floats[Ifloat+Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_floats[Ifloat+Tn]);
			}
		}

	
		for(Tn=0;Tn<Idouble;Tn++)
		{
			if(use_w_times_I_doubles[Tn]==1)
			{
				fprintf(IGC_globalvariables_fp,"\n★Write I_doubles[%d] appear times=%d ",Tn,use_w_times_I_doubles[Tn]);
				if(use_w_str_I_doubles[Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_doubles[Tn]);
			}
		}
	
		if(use_w_times_I_doubles[Idouble]==1)
		{	
			fprintf(IGC_globalvariables_fp,"\n★Write I_doubles[?X] appear times=%d \n",use_w_times_I_doubles[Idouble]);
			for(Tn=0;Tn<use_w_times_I_doubles[Idouble];Tn++)
			{
				if(use_w_str_I_doubles[Idouble+Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_doubles[Idouble+Tn]);
			}
		}

		
		
		for(Tn=0;Tn<Iint;Tn++)
		{
			if(use_w_times_I_ints[Tn]==1)
			{
				fprintf(IGC_globalvariables_fp,"\n★Write I_ints[%d] appear times=%d ",Tn,use_w_times_I_ints[Tn]);
				if(use_w_str_I_ints[Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_ints[Tn]);
			}
		}
		
		if(use_w_times_I_ints[Iint]==1)
		{	
			fprintf(IGC_globalvariables_fp,"\n★Write I_ints[?X] appear times=%d \n",use_w_times_I_ints[Iint]);
			for(Tn=0;Tn<use_w_times_I_ints[Iint];Tn++)
			{
				if(use_w_str_I_ints[Iint+Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_ints[Iint+Tn]);
			}
		}
		
	
		for(Tn=0;Tn<Iuint;Tn++)
		{
			if(use_w_times_I_uints[Tn]==1)
			{
				fprintf(IGC_globalvariables_fp,"\n★Write I_uints[%d] appear times=%d ",Tn,use_w_times_I_uints[Tn]);
				if(use_w_str_I_uints[Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_uints[Tn]);
			}
		}
	
		if(use_w_times_I_uints[Iuint]==1)
		{	
			fprintf(IGC_globalvariables_fp,"\n★Write I_uints[?X] appear times=%d \n",use_w_times_I_uints[Iuint]);
			for(Tn=0;Tn<use_w_times_I_uints[Iuint];Tn++)
			{
				if(use_w_str_I_uints[Iuint+Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_uints[Iuint+Tn]);
			}
		}
		
	
		for(Tn=0;Tn<Ilong;Tn++)
		{
			if(use_w_times_I_longs[Tn]==1)
			{
				fprintf(IGC_globalvariables_fp,"\n★Write I_longs[%d] appear times=%d ",Tn,use_w_times_I_longs[Tn]);
				if(use_w_str_I_longs[Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_longs[Tn]);
			}
		}
		
		if(use_w_times_I_longs[Ilong]==1)
		{	
			fprintf(IGC_globalvariables_fp,"\n★Write I_longs[?X] appear times=%d \n",use_w_times_I_longs[Ilong]);
			for(Tn=0;Tn<use_w_times_I_longs[Ilong];Tn++)
			{
				if(use_w_str_I_longs[Ilong+Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_longs[Ilong+Tn]);
			}
		}
	
		for(Tn=0;Tn<Iulong;Tn++)
		{
			if(use_w_times_I_ulongs[Tn]==1)
			{
				fprintf(IGC_globalvariables_fp,"\n★Write I_ulongs[%d] appear times=%d ",Tn,use_w_times_I_ulongs[Tn]);
				if(use_w_str_I_ulongs[Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_ulongs[Tn]);
			}
		}
	
		if(use_w_times_I_ulongs[Iulong]==1)
		{	
			fprintf(IGC_globalvariables_fp,"\n★Write I_ulongs[?X] appear times=%d \n",use_w_times_I_ulongs[Iulong]);
			for(Tn=0;Tn<use_w_times_I_ulongs[Iulong];Tn++)
			{
				if(use_w_str_I_ulongs[Iulong+Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_ulongs[Iulong+Tn]);
			}
		}
	
		for(Tn=0;Tn<Ibool;Tn++)
		{
			if(use_w_times_I_bools[Tn]==1)
			{
				fprintf(IGC_globalvariables_fp,"\n★Write I_bools[%d] appear times=%d ",Tn,use_w_times_I_bools[Tn]);
				if(use_w_str_I_bools[Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_bools[Tn]);
			}
		}
		
		if(use_w_times_I_bools[Ibool]==1)
		{	
			fprintf(IGC_globalvariables_fp,"\n★Write I_bools[?X] appear times=%d \n",use_w_times_I_bools[Ibool]);
			for(Tn=0;Tn<use_w_times_I_bools[Ibool];Tn++)
			{
				if(use_w_str_I_bools[Ibool+Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_bools[Ibool+Tn]);
			}
		}
		
		
		for(Tn=0;Tn<Ishort;Tn++)
		{
			if(use_w_times_I_shorts[Tn]==1)
			{
				fprintf(IGC_globalvariables_fp,"\n★Write I_shorts[%d] appear times=%d ",Tn,use_w_times_I_shorts[Tn]);
				if(use_w_str_I_shorts[Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_shorts[Tn]);
			}
		}
	
		if(use_w_times_I_shorts[Ishort]==1)
		{	
			fprintf(IGC_globalvariables_fp,"\n★Write I_shorts[?X] appear times=%d \n",use_w_times_I_shorts[Ishort]);
			for(Tn=0;Tn<use_w_times_I_shorts[Ishort];Tn++)
			{
				if(use_w_str_I_shorts[Ishort+Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_shorts[Ishort+Tn]);
			}
		}

	
		for(Tn=0;Tn<Iushort;Tn++)
		{
			if(use_w_times_I_ushorts[Tn]==1)
			{
				fprintf(IGC_globalvariables_fp,"\n★Write I_ushorts[%d] appear times=%d ",Tn,use_w_times_I_ushorts[Tn]);
				if(use_w_str_I_ushorts[Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_ushorts[Tn]);
			}
		}
	
		if(use_w_times_I_ushorts[Iushort]==1)
		{	
			fprintf(IGC_globalvariables_fp,"\n★Write I_ushorts[?X] appear times=%d \n",use_w_times_I_ushorts[Iushort]);
			for(Tn=0;Tn<use_w_times_I_ushorts[Iushort];Tn++)
			{
				if(use_w_str_I_ushorts[Iushort+Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_ushorts[Iushort+Tn]);
			}
		}
	
		for(Tn=0;Tn<Ichar;Tn++)
		{
			if(use_w_times_I_chars[Tn]==1)
			{
				fprintf(IGC_globalvariables_fp,"\n★Write I_chars[%d] appear times=%d ",Tn,use_w_times_I_chars[Tn]);
				if(use_w_str_I_chars[Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_chars[Tn]);
			}
		}
	
		if(use_w_times_I_chars[Ichar]==1)
		{	
			fprintf(IGC_globalvariables_fp,"\n★Write I_chars[?X] appear times=%d \n",use_w_times_I_chars[Ichar]);
			for(Tn=0;Tn<use_w_times_I_chars[Ichar];Tn++)
			{
				if(use_w_str_I_chars[Ichar+Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_chars[Ichar+Tn]);
			}
		}
	
		for(Tn=0;Tn<Iuchar;Tn++)
		{
			if(use_w_times_I_uchars[Tn]==1)
			{
				fprintf(IGC_globalvariables_fp,"\n★Write I_uchars[%d] appear times=%d ",Tn,use_w_times_I_uchars[Tn]);
				if(use_w_str_I_uchars[Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_uchars[Tn]);
			}
		}
	
		if(use_w_times_I_uchars[Iuchar]==1)
		{	
			fprintf(IGC_globalvariables_fp,"\n★Write I_uchars[?X] appear times=%d \n",use_w_times_I_uchars[Iuchar]);
			for(Tn=0;Tn<use_w_times_I_uchars[Iuchar];Tn++)
			{
				if(use_w_str_I_uchars[Iuchar+Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_uchars[Iuchar+Tn]);
			}
		}
	
		for(Tn=0;Tn<Idatetime;Tn++)
		{
			if(use_w_times_I_datetimes[Tn]==1)
			{
				fprintf(IGC_globalvariables_fp,"\n★Write I_datetimes[%d] appear times=%d ",Tn,use_w_times_I_datetimes[Tn]);
				if(use_w_str_I_datetimes[Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_datetimes[Tn]);
			}
		}
	
		if(use_w_times_I_datetimes[Idatetime]==1)
		{	
			fprintf(IGC_globalvariables_fp,"\n★Write I_datetimes[?X] appear times=%d \n",use_w_times_I_datetimes[Idatetime]);
			for(Tn=0;Tn<use_w_times_I_datetimes[Idatetime];Tn++)
			{
				if(use_w_str_I_datetimes[Idatetime+Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_datetimes[Idatetime+Tn]);
			}
		}

	
		for(Tn=0;Tn<IH9000_IGC_data;Tn++)
		{
			if(use_w_times_I_H9000_IGC_datas[Tn]==1)
			{
				fprintf(IGC_globalvariables_fp,"\n★Write I_H9000_IGC_datas[%d] appear times=%d ",Tn,use_w_times_I_H9000_IGC_datas[Tn]);
				if(use_w_str_I_H9000_IGC_datas[Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_H9000_IGC_datas[Tn]);
			}
		}
	
		if(use_w_times_I_H9000_IGC_datas[IH9000_IGC_data]==1)
		{	
			fprintf(IGC_globalvariables_fp,"\n★Write I_H9000_IGC_datas[?X] appear times=%d \n",use_w_times_I_H9000_IGC_datas[IH9000_IGC_data]);
			for(Tn=0;Tn<use_w_times_I_H9000_IGC_datas[IH9000_IGC_data];Tn++)
			{
				if(use_w_str_I_H9000_IGC_datas[IH9000_IGC_data+Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_H9000_IGC_datas[IH9000_IGC_data+Tn]);
			}
		}

	
		for(Tn=0;Tn<IH9000_IGC_IND_data;Tn++)
		{
			if(use_w_times_I_H9000_IGC_IND_datas[Tn]==1)
			{
				fprintf(IGC_globalvariables_fp,"\n★Write I_H9000_IGC_IND_datas[%d] appear times=%d ",Tn,use_w_times_I_H9000_IGC_IND_datas[Tn]);
				if(use_w_str_I_H9000_IGC_IND_datas[Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_H9000_IGC_IND_datas[Tn]);
			}
		}
	
		if(use_w_times_I_H9000_IGC_IND_datas[IH9000_IGC_IND_data]==1)
		{	
			fprintf(IGC_globalvariables_fp,"\n★Write I_H9000_IGC_IND_datas[?X] appear times=%d \n",use_w_times_I_H9000_IGC_IND_datas[IH9000_IGC_IND_data]);
			for(Tn=0;Tn<use_w_times_I_H9000_IGC_IND_datas[IH9000_IGC_IND_data];Tn++)
			{
				if(use_w_str_I_H9000_IGC_IND_datas[IH9000_IGC_IND_data+Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_H9000_IGC_IND_datas[IH9000_IGC_IND_data+Tn]);
			}
		}
	
		for(Tn=0;Tn<IH9000_IGC_ANA_data;Tn++)
		{
			if(use_w_times_I_H9000_IGC_ANA_datas[Tn]==1)
			{
				fprintf(IGC_globalvariables_fp,"\n★Write I_H9000_IGC_ANA_datas[%d] appear times=%d ",Tn,use_w_times_I_H9000_IGC_ANA_datas[Tn]);
				if(use_w_str_I_H9000_IGC_ANA_datas[Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_H9000_IGC_ANA_datas[Tn]);
			}
		}
	
		if(use_w_times_I_H9000_IGC_ANA_datas[IH9000_IGC_ANA_data]==1)
		{	
			fprintf(IGC_globalvariables_fp,"\n★Write I_H9000_IGC_ANA_datas[?X] appear times=%d \n",use_w_times_I_H9000_IGC_ANA_datas[IH9000_IGC_ANA_data]);
			for(Tn=0;Tn<use_w_times_I_H9000_IGC_ANA_datas[IH9000_IGC_ANA_data];Tn++)
			{
				if(use_w_str_I_H9000_IGC_ANA_datas[IH9000_IGC_ANA_data+Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_H9000_IGC_ANA_datas[IH9000_IGC_ANA_data+Tn]);
			}
		}
	
		for(Tn=0;Tn<Itimer;Tn++)
		{
			if(use_w_times_I_H9000_timers[Tn]==1)
			{
				fprintf(IGC_globalvariables_fp,"\n★Write I_H9000_timers[%d] appear times=%d ",Tn,use_w_times_I_H9000_timers[Tn]);
				if(use_w_str_I_H9000_timers[Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_H9000_timers[Tn]);
			}
		}
	
		if(use_w_times_I_H9000_timers[Itimer]==1)
		{	
			fprintf(IGC_globalvariables_fp,"\n★Write I_H9000_timers[?X] appear times=%d \n",use_w_times_I_H9000_timers[Itimer]);
			for(Tn=0;Tn<use_w_times_I_H9000_timers[Itimer];Tn++)
			{
				if(use_w_str_I_H9000_timers[Itimer+Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_H9000_timers[Itimer+Tn]);
			}
		}
		
		
		for(Tn=0;Tn<Ialarms;Tn++)
		{
			if(use_w_times_I_H9000_alarms[Tn]==1)
			{
				fprintf(IGC_globalvariables_fp,"\n★Write I_H9000_alarms[%d] appear times=%d ",Tn,use_w_times_I_H9000_alarms[Tn]);
				if(use_w_str_I_H9000_alarms[Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_H9000_alarms[Tn]);
			}
		}
	
		if(use_w_times_I_H9000_alarms[Ialarms]==1)
		{	
			fprintf(IGC_globalvariables_fp,"\n★Write I_H9000_alarms[?X] appear times=%d \n",use_w_times_I_H9000_alarms[Ialarms]);
			for(Tn=0;Tn<use_w_times_I_H9000_alarms[Ialarms];Tn++)
			{
				if(use_w_str_I_H9000_alarms[Ialarms+Tn][0]!=0)
					fprintf(IGC_globalvariables_fp,"\n    %s",use_w_str_I_H9000_alarms[Ialarms+Tn]);
			}
		}
//////
		
		for(Tn=0;Tn<Ifloat;Tn++)
		{
			if(use_r_times_I_floats[Tn]>0)
				fprintf(IGC_globalvariables_fp,"\nRead I_floats[%d] appear times=%d \n",Tn,use_r_times_I_floats[Tn]);
		}
		if(use_r_times_I_floats[Ifloat]>0)
		{
			fprintf(IGC_globalvariables_fp,"\nRead I_floats[?X] appear times=%d \n",use_r_times_I_floats[Ifloat]);
		}

		
		for(Tn=0;Tn<Idouble;Tn++)
		{
			if(use_r_times_I_doubles[Tn]>0)
				fprintf(IGC_globalvariables_fp,"\nRead I_doubles[%d] appear times=%d \n",Tn,use_r_times_I_doubles[Tn]);
		}
		if(use_r_times_I_doubles[Idouble]>0)
		{
			fprintf(IGC_globalvariables_fp,"\nRead I_doubles[?X] appear times=%d \n",use_r_times_I_doubles[Idouble]);
		}
		
		for(Tn=0;Tn<Iint;Tn++)
		{
			if(use_r_times_I_ints[Tn]>0)
				fprintf(IGC_globalvariables_fp,"\nRead I_ints[%d] appear times=%d \n",Tn,use_r_times_I_ints[Tn]);
		}
		if(use_r_times_I_ints[Iint]>0)
		{
			fprintf(IGC_globalvariables_fp,"\nRead I_ints[?X] appear times=%d \n",use_r_times_I_ints[Iint]);
		}
		
		for(Tn=0;Tn<Iuint;Tn++)
		{
			if(use_r_times_I_uints[Tn]>0)
				fprintf(IGC_globalvariables_fp,"\nRead I_uints[%d] appear times=%d \n",Tn,use_r_times_I_uints[Tn]);
		}
		if(use_r_times_I_uints[Iuint]>0)
		{
			fprintf(IGC_globalvariables_fp,"\nRead I_uints[?X] appear times=%d \n",use_r_times_I_uints[Iuint]);
		}

		
		for(Tn=0;Tn<Ilong;Tn++)
		{
			if(use_r_times_I_longs[Tn]>0)
				fprintf(IGC_globalvariables_fp,"\nRead I_longs[%d] appear times=%d \n",Tn,use_r_times_I_longs[Tn]);
		}
		if(use_r_times_I_longs[Ilong]>0)
		{
			fprintf(IGC_globalvariables_fp,"\nRead I_longs[?X] appear times=%d \n",use_r_times_I_longs[Ilong]);
		}


		
		for(Tn=0;Tn<Iulong;Tn++)
		{
			if(use_r_times_I_ulongs[Tn]>0)
				fprintf(IGC_globalvariables_fp,"\nRead I_ulongs[%d] appear times=%d \n",Tn,use_r_times_I_ulongs[Tn]);
		}
		if(use_r_times_I_ulongs[Iulong]>0)
		{
			fprintf(IGC_globalvariables_fp,"\nRead I_ulongs[?X] appear times=%d \n",use_r_times_I_ulongs[Iulong]);
		}

		
		for(Tn=0;Tn<Ibool;Tn++)
		{
			if(use_r_times_I_bools[Tn]>0)
				fprintf(IGC_globalvariables_fp,"\nRead I_bools[%d] appear times=%d \n",Tn,use_r_times_I_bools[Tn]);
		}
		if(use_r_times_I_bools[Ibool]>0)
		{
			fprintf(IGC_globalvariables_fp,"\nRead I_bools[?X] appear times=%d \n",use_r_times_I_bools[Ibool]);
		}
		
		for(Tn=0;Tn<Ishort;Tn++)
		{
			if(use_r_times_I_shorts[Tn]>0)
				fprintf(IGC_globalvariables_fp,"\nRead I_shorts[%d] appear times=%d \n",Tn,use_r_times_I_shorts[Tn]);
		}
		if(use_r_times_I_shorts[Ishort]>0)
		{
			fprintf(IGC_globalvariables_fp,"\nRead I_shorts[?X] appear times=%d \n",use_r_times_I_shorts[Ishort]);
		}
		
		for(Tn=0;Tn<Iushort;Tn++)
		{
			if(use_r_times_I_ushorts[Tn]>0)
				fprintf(IGC_globalvariables_fp,"\nRead I_ushorts[%d] appear times=%d \n",Tn,use_r_times_I_ushorts[Tn]);
		}
		if(use_r_times_I_ushorts[Iushort]>0)
		{
			fprintf(IGC_globalvariables_fp,"\nRead I_ushorts[?X] appear times=%d \n",use_r_times_I_ushorts[Iushort]);
		}

		
		
		for(Tn=0;Tn<Ichar;Tn++)
		{
			if(use_r_times_I_chars[Tn]>0)
				fprintf(IGC_globalvariables_fp,"\nRead I_chars[%d] appear times=%d \n",Tn,use_r_times_I_chars[Tn]);
		}
		if(use_r_times_I_chars[Ichar]>0)
		{
			fprintf(IGC_globalvariables_fp,"\nRead I_chars[?X] appear times=%d \n",use_r_times_I_chars[Ichar]);
		}
		
		for(Tn=0;Tn<Iuchar;Tn++)
		{
			if(use_r_times_I_uchars[Tn]>0)
				fprintf(IGC_globalvariables_fp,"\nRead I_uchars[%d] appear times=%d \n",Tn,use_r_times_I_uchars[Tn]);
		}
		if(use_r_times_I_uchars[Iuchar]>0)
		{
			fprintf(IGC_globalvariables_fp,"\nRead I_uchars[?X] appear times=%d \n",use_r_times_I_uchars[Iuchar]);
		}
		
		for(Tn=0;Tn<Idatetime;Tn++)
		{
			if(use_r_times_I_datetimes[Tn]>0)
				fprintf(IGC_globalvariables_fp,"\nRead I_datetimes[%d] appear times=%d \n",Tn,use_r_times_I_datetimes[Tn]);
		}
		if(use_r_times_I_datetimes[Idatetime]>0)
		{
			fprintf(IGC_globalvariables_fp,"\nRead I_datetimes[?X] appear times=%d \n",use_r_times_I_datetimes[Idatetime]);
		}
		
		
		for(Tn=0;Tn<IH9000_IGC_data;Tn++)
		{
			if(use_r_times_I_H9000_IGC_datas[Tn]>0)
				fprintf(IGC_globalvariables_fp,"\nRead I_H9000_IGC_datas[%d] appear times=%d \n",Tn,use_r_times_I_H9000_IGC_datas[Tn]);
		}
		if(use_r_times_I_H9000_IGC_datas[IH9000_IGC_data]>0)
		{
			fprintf(IGC_globalvariables_fp,"\nRead I_H9000_IGC_datas[?X] appear times=%d \n",use_r_times_I_H9000_IGC_datas[IH9000_IGC_data]);
		}
		
		for(Tn=0;Tn<IH9000_IGC_IND_data;Tn++)
		{
			if(use_r_times_I_H9000_IGC_IND_datas[Tn]>0)
				fprintf(IGC_globalvariables_fp,"\nRead I_H9000_IGC_IND_datas[%d] appear times=%d \n",Tn,use_r_times_I_H9000_IGC_IND_datas[Tn]);
		}
		if(use_r_times_I_H9000_IGC_IND_datas[IH9000_IGC_IND_data]>0)
		{
			fprintf(IGC_globalvariables_fp,"\nRead I_H9000_IGC_IND_datas[?X] appear times=%d \n",use_r_times_I_H9000_IGC_IND_datas[IH9000_IGC_IND_data]);
		}
		
		for(Tn=0;Tn<IH9000_IGC_ANA_data;Tn++)
		{
			if(use_r_times_I_H9000_IGC_ANA_datas[Tn]>0)
				fprintf(IGC_globalvariables_fp,"\nRead I_H9000_IGC_ANA_datas[%d] appear times=%d \n",Tn,use_r_times_I_H9000_IGC_ANA_datas[Tn]);
		}
		if(use_r_times_I_H9000_IGC_ANA_datas[IH9000_IGC_ANA_data]>0)
		{
			fprintf(IGC_globalvariables_fp,"\nRead I_H9000_IGC_ANA_datas[?X] appear times=%d \n",use_r_times_I_H9000_IGC_ANA_datas[IH9000_IGC_ANA_data]);
		}

		
		for(Tn=0;Tn<Itimer;Tn++)
		{
			if(use_r_times_I_H9000_timers[Tn]>0)
				fprintf(IGC_globalvariables_fp,"\nRead I_H9000_timers[%d] appear times=%d \n",Tn,use_r_times_I_H9000_timers[Tn]);
		}
		if(use_r_times_I_H9000_timers[Itimer]>0)
		{
			fprintf(IGC_globalvariables_fp,"\nRead I_H9000_timers[?X] appear times=%d \n",use_r_times_I_H9000_timers[Itimer]);
		}
		
		for(Tn=0;Tn<Ialarms;Tn++)
		{
			if(use_r_times_I_H9000_alarms[Tn]>0)
				fprintf(IGC_globalvariables_fp,"\nRead I_H9000_alarms[%d] appear times=%d \n",Tn,use_r_times_I_H9000_alarms[Tn]);
		}
		if(use_r_times_I_H9000_alarms[Ialarms]>0)
		{
			fprintf(IGC_globalvariables_fp,"\nRead I_H9000_alarms[?X] appear times=%d \n",use_r_times_I_H9000_alarms[Ialarms]);
		}





	

		fclose(IGC_globalvariables_fp);
	 
	}
	else
	{
	
	}

	return 1;
}
