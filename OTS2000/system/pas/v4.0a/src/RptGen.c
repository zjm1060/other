#include	"../../../dms/v4.0a/inc/dms_com.h"
#include        "../../../dms/v4.0a/inc/os_type_def.h"
#include        "../../../pas/v4.0a/inc/rpt_def.h"

void GetStnGrpRange();
void GenHourRptFileName ();
void GenMonthRptFileName();


#pragma	comment(lib, "../../../dms/v4.0a/lib/dblib.lib")
#pragma	comment(lib, "../../../dps/v4.0a/lib/ipclib.lib")
#pragma	comment(lib, "../../../lan/v4.0a/lib/lan.lib")


typedef struct

{ 
    char    station_name[STATION_NAME_SIZE];

    UCHAR   grp_num;

    char    group_name[100][GROUP_NAME_SIZE];

} STN_GRP_DATA;



STN_GRP_DATA    stn_data[10];  

GULONG          stn_num;



static char      HD_time[120],HD_name[120],HD_full_name[158];

static char      HM_time[120],HM_name[120],HM_full_name[158];



struct tm 	timeptr;

time_t 		t;



short    	Min, Hour, Hour1, Day;

static char     hour_data[58],month_data[58];



RPT_DATA	buf1[CLE];

RPT_DATA 	tmp_data[H_MAX];

char            command1[150],command2[150];



void GetStnGrpRange()

{

     UCHAR  i,j;

     GULONG jj;
          
     GetStationNum( &stn_num );

     for ( i=1; i<=(UCHAR)stn_num; i++ )
	 {	     
		UCHAR stnId;
		GetStnIdByIndex(i, &stnId);
		GetStnNameById( stnId, stn_data[i].station_name );	   

		GetGroupNumById( stnId,&jj);

		stn_data[i].grp_num = (UCHAR)jj;

		for ( j=1; j<=stn_data[i].grp_num; j++)
		{
			GetGroupNameById( stnId,j,stn_data[i].group_name[j] );
		}
	}
}



void GenHourRptFileName ()

{

     timeptr=*localtime (&t);

     sprintf (HD_time,"%04d%02d%02d",timeptr.tm_year+1900,timeptr.tm_mon+1,timeptr.tm_mday);

     strcpy(HD_name,"HD");

     strcat ( HD_name,HD_time);

     strcat (HD_name,".DATA");

     strcpy(HD_full_name,RPTLOG_NAME_PATH);

     strcat( HD_full_name,HD_name);

     strcpy(hour_data, HD_full_name);

}



void GenMonthRptFileName()

{

     timeptr=*localtime(&t);

     sprintf(HM_time,"%04d%02d",timeptr.tm_year+1900,timeptr.tm_mon+1);

     strcpy(HM_name,"HM");

     strcat ( HM_name,HM_time);

     strcat (HM_name,".DATA");

     strcpy(HM_full_name,RPTLOG_NAME_PATH);

     strcat( HM_full_name,HM_name);

     strcpy(month_data, HM_full_name);

}



void main()

{	 

	 int	     k,pnt_number,pnt_number0,ie,jd;

	 char	     tmp[75];

	 short	     ii,sum,sumcalc, sumimp;



	 BOOL        statistic;

         UCHAR       i,j,type;

	 USHORT      id;

	 GULONG      datanum;

	 POINTER     ptr;

	 DMS_COMMON  dms_cmm;



	 char        full_name[150],name[150],rpt_def_name[150],err_str[128];	 

	 FILE        *fp_month_def,*fp_month_data,*fp_month_data_tmp;

	 FILE        *fp,*impfp,*fp_hour_def,*fp_hour_data,*fp_hour_data_tmp;

     
   
	 k=MountDB (NULL);	 
	 if(k== -1) { printf("Mount DATABASE Fail\n");  exit(1); }

	 GetStnGrpRange();

	 strcpy(rpt_def_name,RPTDEF_NAME_PATH);

	 strcat(rpt_def_name, "hourly_rpt_file.def");	 

	 if( (fp=fopen(rpt_def_name,"w"))==NULL)

	 {

		sprintf(err_str,"RptGen;EC:001;can't open  hourly_rpt_file.def  !!!");

		Err_log(err_str);

		exit(0);			

	 }



	 strcpy(rpt_def_name,RPTDEF_NAME_PATH);

	strcat(rpt_def_name, "monthly_rpt_file.def");     

	 if( (impfp=fopen(rpt_def_name, "w"))==NULL )

	 {

           sprintf(err_str,"RptGen;EC:002;can't open  monthly_rpt_file.def  !!!");

           Err_log(err_str);

	   exit(0);

         } 



	 rewind(fp);

	 sum=0;

	 sumcalc=0;

         fprintf(fp,"%9d\n",sum);

	 fprintf(fp,"%9d\n",sumcalc);



	 rewind(impfp);

	 sumimp=0;	

	 fprintf(impfp,"%9d\n",sumimp);

	 fprintf(impfp,"%9d\n",sumcalc);



	 for(type=1;type<10;type++)

	 {
	  if( (strcmp(recordname[type],"ANA")!=0)&&			

            (strcmp(recordname[type],"CAL")!=0)&&

            (strcmp(recordname[type],"IMP")!=0 )   ) continue;



            for(i=1; i<=(UCHAR)stn_num; i++)

	       {			

		    if( strcmp( stn_data[i].station_name,"") == 0 ) continue;

			
		    for(j=1;j<=stn_data[i].grp_num;j++)

		    {
				UCHAR stnId;
				GetStnIdByIndex(i, &stnId);

		      if(strcmp(stn_data[i].group_name[j],"")==0) continue;				 

                      if( -1 == GetEntryNumById(stnId,j,type,&datanum)) continue;

		      if(datanum<=0) continue;			 

			 			

 	     	      for(id=0; id<(USHORT)datanum; id++)

		      { 

		       ptr.stn_id      =  stnId;

        	       ptr.dev_id      =  j;

                       ptr.data_type   =  type;

                       ptr.pt_id       =  id;				

 	               if(-1 == ReadEntryById ( &ptr, &dms_cmm ) ) continue;
 	                                  

		       strcpy(full_name,stn_data[i].station_name);

                       strcat ( full_name,".");

		       strcat(full_name, stn_data[i].group_name[j]);

		       strcat ( full_name,".");

		       strcat ( full_name,recordname[type]);

		       strcat ( full_name,".");

                       strcat ( full_name, dms_cmm.fixed.entryName);			   

				

		       if( strstr(stn_data[i].group_name[j],"SYS")!=NULL )

			{	

			  if( (strcmp(dms_cmm.fixed.entryName,"QCZYG")!=0)&&

			      (strcmp(dms_cmm.fixed.entryName,"QCZWG")!=0)&&

			      (strcmp(recordname[type],"CAL")!=0) ) continue;			  					

			}

			   /*printf("\n**** type=%d,stn_id=%d,grp_id=%d,zs=%d,id=%d,name=%s\n",

				   type,i,j,datanum,id,full_name);*/

                           	

			   if(strcmp(recordname[type],"IMP")==0) 

			     {

			       strcpy(name,full_name);

			       strcat(name,"_H");

			       fprintf(impfp, "%s\n", name);

				

			       strcpy(name,full_name);

                               strcat(name,"_L");

                               fprintf(impfp, "%s\n", name);

	

			       strcpy(name,full_name);

                               strcat(name,"_S");

                               fprintf(impfp, "%s\n", name);



			       strcpy(name,full_name);

                               strcat(name,"_M");

                               fprintf(impfp, "%s\n", name);



                               strcpy(name,full_name);

                               strcat(name,"_T");

                               fprintf(impfp, "%s\n", name);



                               strcpy(name,full_name);

                               strcat(name,"_Y");

                               fprintf(impfp, "%s\n", name);

										

			       strcpy(name,full_name);

                               strcat(name,"_R");

                               fprintf(impfp, "%s\n", name);



			       strcpy(name,full_name);		

                               strcat(name,"_U");

                               fprintf(impfp, "%s\n", name);



                               strcpy(name,full_name);

                               strcat(name,"_N");

                               fprintf(impfp, "%s\n", name);										



			       strcpy(name,full_name);

                               strcat(name,"_J");

                               fprintf(impfp, "%s\n", name);



			       strcpy(name,full_name);		

                               strcat(name,"_K");

                               fprintf(impfp, "%s\n", name);



                               strcpy(name,full_name);

                               strcat(name,"_X");

                               fprintf(impfp, "%s\n", name);



                               /*** 当为3.0系统时，加下面语句 ***/

                               strcpy(name,full_name);

                               strcat(name,"_I");

                               fprintf(impfp, "%s\n", name);



			       strcpy(name,full_name);

                               strcat(name,"_V");

                               fprintf(impfp, "%s\n", name);

					 

			       strcpy(name,full_name);

                               strcat(name,"_P");

                               fprintf(impfp, "%s\n", name);

                               /***   add  end  ***/



				sumimp=sumimp+15;      /* 当为2.0系统时,sumimp+12。当为3.0系统时，sumimp+15 */

			      }

			    else 

				if (strcmp(recordname[type],"CAL")==0)

				{

				  sumcalc++;

				  fprintf(impfp, "%s\n", full_name);

				  fprintf(fp, "%s\n", full_name);

					

				  sum++;

				  sumimp++;

				}

				else

				{

				  fprintf(fp, "%s\n", full_name);

                                  sum++;

				}

			  }

                   }		

	     }

     }



    rewind(fp);

    fprintf(fp,"%9d\n", sum);

    fprintf(fp,"%9d\n", sumcalc);

    fclose(fp);



    rewind(impfp);

    fprintf(impfp,"%9d\n", sumimp);

    fprintf(impfp,"%9d\n", sumcalc);

    fclose(impfp);


   
    /*************** add  statistic(POL & INT) ****************/

#ifndef	 WINDOWS_OS
    system( "cp /home/ems/h9000/def/monthly_rpt_file.def /home/ems/h9000/def/monthly_rpt_file.sav");    
#else
	system( "copy \\home\\ems\\h9000\\def\\monthly_rpt_file.def \\home\\ems\\h9000\\def\\monthly_rpt_file.sav");

#endif


	strcpy(rpt_def_name,RPTDEF_NAME_PATH);

	strcat(rpt_def_name, "monthly_rpt_file.def");	 

	if( (impfp=fopen(rpt_def_name, "w"))==NULL)

	{

	  sprintf( err_str,"RptGen:EC:004;can't open  monthly_rpt_file.def  !!!");

	  Err_log(err_str);		

	  exit(0);			

	}

    

	strcpy(rpt_def_name,RPTDEF_NAME_PATH);

        strcat(rpt_def_name, "monthly_rpt_file.sav");

	if( (fp=fopen(rpt_def_name, "r"))==NULL )

	{

          sprintf(err_str,"RptGen:EC:005;can't open  monthly_rpt_file.sav  !!!"); 

	  Err_log(err_str);

	  exit(0);

        }


       fscanf(fp,"%d",&k); fprintf(impfp,"%9d\n",sumimp);	
	fscanf(fp,"%d",&k); fprintf(impfp,"%9d\n",sumcalc);		
	

	for(ii=0;ii<(sumimp-sumcalc);ii++)

	{	 

	  fscanf(fp,"%s",tmp); fprintf(impfp,"%s\n",tmp);	  
	}


       
	for(type=1;type<10;type++)

	{

	  if( (strcmp(recordname[type],"POL")!=0)&&(strcmp(recordname[type],"INT")!=0) ) continue;

		

	    for(i=1; i<=(UCHAR)stn_num; i++)

		{
			UCHAR stnId;
			GetStnIdByIndex(i, &stnId);

		  if( strcmp( stn_data[i].station_name,"") == 0 ) continue;
		  for(j=1;j<=stn_data[i].grp_num;j++)

		   {

		      if(strcmp(stn_data[i].group_name[j],"")==0 || 

			 strstr(stn_data[i].group_name[j],"SYS")!=NULL ) continue;

                      if( -1 == GetEntryNumById(stnId,j,type,&datanum)) continue;

		      if(datanum<=0) continue;

		      for(id=0; id<(USHORT)datanum; id++)

			 { 

			   ptr.stn_id      =  stnId;

        	           ptr.dev_id      =  j;

                           ptr.data_type   =  type;

                           ptr.pt_id       =  id;

			   IsPtStatisticById(&ptr,&statistic);			   
 	                   if(-1 == ReadEntryById ( &ptr, &dms_cmm ) ) continue;       	    

			   /*if ( dms_cmm.fixed.ioattr.doublePt == 1) id++;*/
			   if (statistic == 0) continue;			   

			   strcpy(full_name,stn_data[i].station_name);

                           strcat ( full_name,".");

			   strcat(full_name, stn_data[i].group_name[j]);

			   strcat ( full_name,".");

			   strcat ( full_name,recordname[type]);

			   strcat ( full_name,".");

                           strcat ( full_name, dms_cmm.fixed.entryName);				

				

			   strcpy(name,full_name);

			   strcat(name,"_Z");

			   fprintf(impfp, "%s\n", name);

				

			   strcpy(name,full_name);

                           strcat(name,"_A");

                           fprintf(impfp, "%s\n", name);



                           strcpy(name,full_name);

                           strcat(name,"_G");

                           fprintf(impfp, "%s\n", name);



			   strcpy(name,full_name);

                           strcat(name,"_B");

                           fprintf(impfp, "%s\n", name);



/*			   strcpy(name,full_name);

                           strcat(name,"_C");

                           fprintf(impfp, "%s\n", name);



			   strcpy(name,full_name);

                           strcat(name,"_E");

                           fprintf(impfp, "%s\n", name);

*/					

			   sumimp=sumimp+4;				

			  }

		     }		

	       }	

	}


       
	for(ii=0;ii<sumcalc;ii++)

	{

	  fscanf(fp,"%s",tmp); fprintf(impfp,"%s\n",tmp);

	}


       
	fclose(fp);

	rewind(impfp);

	fprintf(impfp,"%9d\n", sumimp);

	fprintf(impfp,"%9d\n", sumcalc);

	fclose(impfp);
	
	UnMountDB();



	printf("\n++++++++ Create OK ++++++++\n");





/***********added by hfz 2005.08.30*************/



	t = time(NULL);			            /*    get  second   from  1970  to  现在    */

	timeptr=*localtime (&t);		    /*    get  local time    */

	Hour=timeptr.tm_hour;		            /*    get  Hour    */



	if (timeptr.tm_hour >= RPT_START_TIME )     /*    RPT_START_TIME 在rpt_def.h头文件中定义   */   

	    {	

		GenHourRptFileName ();	            /*    创建日数据文件名   */

		GenMonthRptFileName();              /*    创建月数据文件名   */

	    }



	else {

		t = t - RPT_START_TIME*3600;

                GenHourRptFileName ();

                GenMonthRptFileName();

	     }





/***day rpt def 文件与day rpt data 文件比较***/



    if (access(hour_data,06)==-1)	/*    if  day data file not exist    */

	{ printf("RptGen.c: %s file not exist\n",hour_data); goto MONTH; }

    else

	{ 	

		  strcpy(rpt_def_name,RPTDEF_NAME_PATH);

	      strcat(rpt_def_name, "hourly_rpt_file.def");

          if( (fp_hour_def=fopen(rpt_def_name, "r") ) == NULL)

		  {

	       printf("can't open  hourly_rpt_file.def  !\n"); exit(0);			

		  }

          fscanf ( fp_hour_def,"%d",&hour_rpt_def_number);

          fscanf ( fp_hour_def,"%d",&hour_rpt_def_number0);
          pnt_number  = hour_rpt_def_number;
          pnt_number0 = hour_rpt_def_number0;
          Swap4Byte(&pnt_number);
          Swap4Byte(&pnt_number0);
          strcpy(rpt_def_name,RPTLOG_NAME_PATH);

	  strcat(rpt_def_name, "hour_data_tmp");

          if((fp_hour_data_tmp = fopen(rpt_def_name, "wb+")) == NULL)   /*   create data tmp file   */

	  {

	    printf("can't create  hour_data_tmp  !\n"); exit(1);			

          }				

	  fwrite(&pnt_number,sizeof(int),1,fp_hour_data_tmp);  /* write day data num1 */

          fwrite(&pnt_number0,sizeof(int),1,fp_hour_data_tmp); /* write day data num2 */          
		  

          for (k = 0; k<hour_rpt_def_number; k++)

	  {

            fscanf ( fp_hour_def,"%s",hourly_name_def[k]);

	    fwrite(&hourly_name_def[k],sizeof(char),75,fp_hour_data_tmp);  /* write string name */

          }

          fclose ( fp_hour_def );



	  buf1[0].value.float_data = 0;   /*   清零   */

          for (k = 0; k < hour_rpt_def_number * TIM_hour; k++)

          {

             if ( fwrite (&buf1[0],sizeof (RPT_DATA),1,fp_hour_data_tmp) != 1)   /* init write 0 */

	      {

               	  printf ("\nRptGen.c:: hour_data_tmp file fwrite error\n");

               	  fclose ( fp_hour_data_tmp);

               	  exit(1);

	      }

          }          



	  if( ( fp_hour_data = fopen(hour_data,"rb+") ) == NULL)

	  {

	     printf("can't open  %s !\n",hour_data); exit(0);			

	  }

          fread(&hour_rpt_data_number,sizeof(int),1,fp_hour_data);   /* read day data num1 */

          fread(&hour_rpt_data_number0,sizeof(int),1,fp_hour_data);  /* read day data num1 */

		  Swap4Byte(&hour_rpt_data_number);

          Swap4Byte(&hour_rpt_data_number0);

          for(k = 0; k < hour_rpt_data_number ; k++)

	  {      

            fread(&hourly_name_data[k],sizeof(char),75,fp_hour_data);  /* read  string name */

	  }	    



          for (ie = 0; ie<hour_rpt_def_number; ie++)

	  {

            for(jd = 0; jd < hour_rpt_data_number ; jd++)

	    {

              if(strcmp(hourly_name_def[ie],hourly_name_data[jd])!=0)  continue;

					     

              for(k = 0; k < TIM_hour ; k++)

	      {						   

		fseek(fp_hour_data,(long) hour_rpt_data_number * 75+8,0);      /*skip day data file num1&num2&string name */						   

		fseek(fp_hour_data,(long)( jd + k * hour_rpt_data_number) * sizeof(RPT_DATA),1);        

		fread(&tmp_data[0], sizeof(RPT_DATA), 1, fp_hour_data);



		fseek(fp_hour_data_tmp,(long) hour_rpt_def_number * 75+8,0);   

		fseek(fp_hour_data_tmp,(long)( ie + k * hour_rpt_def_number) * sizeof(RPT_DATA),1);

		fwrite(&tmp_data[0], sizeof(RPT_DATA), 1, fp_hour_data_tmp);

	      }

	      break;

	    }				  

	  }

          

	  fclose ( fp_hour_data);

	  fclose ( fp_hour_data_tmp);




#ifndef	 WINDOWS_OS
        strcpy ( command1,"rm /home/ems/h9000/his/rpt/");
#else
	strcpy ( command1,"del \\home\\ems\\h9000\\his\\rpt\\");

#endif
        strcat ( command1, HD_name );

	printf ( "%s\n",command1 );

        system (command1);



#ifndef	 WINDOWS_OS
		strcpy ( command2, "mv /home/ems/h9000/his/rpt/hour_data_tmp /home/ems/h9000/his/rpt/");

#else
		strcpy ( command2, "move \\home\\ems\\h9000\\his\\rpt\\hour_data_tmp \\home\\ems\\h9000\\his\\rpt\\");           

#endif
		strcat ( command2, HD_name );

		printf ( "%s\n",command2);

        system (command2);          

		  		 

	    }	





/***month rpt def 文件与month rpt data 文件比较***/

MONTH:

     if (access(month_data,06)==-1)	/*    if  month data file not exist    */

	    { printf("RptGen.c: %s file not exist\n",month_data); exit(0); }

     else

	    { 	  

	      strcpy(rpt_def_name,RPTDEF_NAME_PATH);

	      strcat(rpt_def_name, "monthly_rpt_file.def");

          if((fp_month_def=fopen(rpt_def_name, "r")) == NULL)

	      {

	         printf("can't open  monthly_rpt_file.def !\n"); exit(0);			

          }

          fscanf ( fp_month_def,"%d",&month_rpt_def_number);

          fscanf ( fp_month_def,"%d",&month_rpt_def_number0);
          pnt_number =month_rpt_def_number;
          pnt_number0 = month_rpt_def_number0;

		  Swap4Byte(&pnt_number);

          Swap4Byte(&pnt_number0);


		  strcpy(rpt_def_name,RPTLOG_NAME_PATH);

	      strcat(rpt_def_name, "month_data_tmp");

	      if((fp_month_data_tmp = fopen(rpt_def_name, "wb+"))==NULL)   /*   create data tmp file   */

	      {

		 printf("can't create  month_data_tmp !\n"); exit(0);			

	      }		

	      fwrite(&pnt_number,sizeof(int),1,fp_month_data_tmp);  /* write month data num1 */

              fwrite(&pnt_number0,sizeof(int),1,fp_month_data_tmp); /* write month data num2 */	      

              

	      for (k = 0; k<month_rpt_def_number; k++)			  

	      {

                fscanf ( fp_month_def,"%s",monthly_name_def[k]);

		fwrite(&monthly_name_def[k],sizeof(char),75,fp_month_data_tmp); /* write string name */

                /*printf("**********monthly_name_def[%d]=%s\n",k,monthly_name_def[k]);*/
	      }

              fclose ( fp_month_def );

              

	      buf1[0].value.float_data = 0;

              for(k = 0; k < month_rpt_def_number * TIM_month; k++)

	      {

                 if( fwrite(&buf1[0],sizeof(RPT_DATA),1,fp_month_data_tmp) != 1)

                  {

                    printf("\nRptGen.c:: month_data_tmp file fwrite error\n");

                    fclose(fp_month_data_tmp);

                    exit(1);

                  }

              }



              if( ( fp_month_data = fopen(month_data,"rb+") ) == NULL )

	      {

		 printf("can't open  %s !\n",month_data); exit(0);			

	      }

         fread(&month_rpt_data_number,sizeof(int),1,fp_month_data);  /* read month data num1 */

         fread(&month_rpt_data_number0,sizeof(int),1,fp_month_data); /* read month data num2 */

	     Swap4Byte(&month_rpt_data_number);

         Swap4Byte(&month_rpt_data_number0);

         for(k = 0; k < month_rpt_data_number ; k++)

	     {

               fread(&monthly_name_data[k],sizeof(char),75,fp_month_data);  /* read  string name */

	     } 



            for (ie = 0; ie<month_rpt_def_number; ie++)

	   {

              for(jd = 0; jd < month_rpt_data_number ; jd++)

	      {

		      if(strcmp(monthly_name_def[ie],monthly_name_data[jd])!=0)  continue;

		      for(k = 0; k < TIM_month ; k++)

		      {						   

			  fseek(fp_month_data,(long) month_rpt_data_number * 75+8,0);      /*skip month data file num1&num2&string name */							   

			  fseek(fp_month_data,(long)( jd + k * month_rpt_data_number) * sizeof(RPT_DATA),1);        

		          fread(&tmp_data[0], sizeof(RPT_DATA), 1, fp_month_data);           

                           

			  fseek(fp_month_data_tmp,(long) month_rpt_def_number * 75+8,0);   

			  fseek(fp_month_data_tmp,(long)( ie + k * month_rpt_def_number) * sizeof(RPT_DATA),1);

			  fwrite(&tmp_data[0], sizeof(RPT_DATA), 1, fp_month_data_tmp);

		       }

		      break;

		    }				  

	       }

         

	 fclose ( fp_month_data);

	 fclose ( fp_month_data_tmp);



#ifndef   WINDOWS_OS
	  strcpy ( command1,"rm /home/ems/h9000/his/rpt/");

#else
          strcpy ( command1,"del \\home\\ems\\h9000\\his\\rpt\\");

#endif
          strcat ( command1, HM_name );

	  printf ( "%s\n",command1 );

          system (command1);



#ifndef   WINDOWS_OS
          strcpy ( command2, "mv /home/ems/h9000/his/rpt/month_data_tmp /home/ems/h9000/his/rpt/");

#else
	  strcpy ( command2, "move \\home\\ems\\h9000\\his\\rpt\\month_data_tmp \\home\\ems\\h9000\\his\\rpt\\");          

#endif
	  strcat ( command2, HM_name );

	  printf ( "%s\n",command2);

          system (command2);

         

      }
}

