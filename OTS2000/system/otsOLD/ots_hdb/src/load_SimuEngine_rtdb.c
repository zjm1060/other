

/*	load_simulog_rtdb.c

	This program is originated from calcu_fn.c in PAS subsystem by zy and
	re-apdated to be used for real time syetem simulation.

	Created by wdk 1997.11.10.
*/

#define	SIMULOG_MAIN

#include "/home/ems/system/ots/ots_hdb/inc/ots.h"
FILE	*fp_gen, *fp_list;
static	double	blocked_time;

#ifdef	UNIX_VERSION
char	*genfile	="/OTSDATA/simulog.dat";
char	*fp_list	="/OTSDATA/simulogEcho.txt";
char	*seqid_file ="/OTSDATA/sequence_id.txt"; 
char	*seqdbin_file	="/OTSDATA/sequence_dbin.txt";
char	*seqauto_file	="/OTSDATA/sequence_auto.txt";
#else
char	*genfile	="/home/ems/h9000/ots/simulog.dat";
char	*listfile	="/home/ems/h9000/his/trap/simulogList.txt";
char	*seqid_file	="/home/ems/h9000/ots/sequence_id.txt";
char	*seqdbin_file	="/home/ems/h9000/ots/sequence_dbin.txt";
char	*seqauto_file	="/home/ems/h9000/ots/sequence_auto.txt";
#endif

DMS_COMMON	dms_rec;

int FillMdlWithParam();
int FillStepShowID();
int ReadSequenceDBIN();

/*----------------------------------------------------------------------------------------------*/

int	SimulogDataLoad ()
{
	int	n_nlf_f_end, n_x_end, n_y_end;
	if ( ( fp_list = fopen ( listfile, "w" ) ) == NULL )
	{
		printf ( "SimulogDataLoad:can't open(w) the file %s!\n", listfile );
		return -1;
	}

	if ( ( fp_gen = fopen ( genfile, "rb" ) ) == NULL )
	{
		printf ( "SimulogDataLoad:can't open(wb) the file %s!\n", genfile );
		return -1;
	}

	if ( fread ( &n_sequence, sizeof (int), 1, fp_gen ) != 1 )
	{
		printf ( "SimulogDataLoad: file %s read n_sequence error\n", genfile );
		return -1;
	}
	printf ( "seq number = %d\n", n_sequence );
	if ( fread ( &n_scene, sizeof (int), 1, fp_gen ) != 1 )
	{
		printf ( "SimulogDataLoad: file %s read n_scene error\n", genfile );
		return -1;
	}
	printf ( "scene number = %d\n", n_scene );
	if ( fread ( &n_step, sizeof (int), 1, fp_gen ) != 1 )
	{
		printf ( "SimulogDataLoadc: file %s read n_step error\n", genfile );
		return -1;
	}
	printf ( "n_step number = %d\n", n_step );
	if ( fread ( &n_show, sizeof (int), 1, fp_gen ) != 1 )
	{
		printf ( "SimulogDataLoadc: file %s read n_show error\n", genfile );
		return -1;
	}

	if ( fread ( &n_group, sizeof (int), 1, fp_gen ) != 1 )
	{
		printf ( "SimulogDataLoad: file %s read n_group error\n", genfile );
		return -1;
	}
	printf ( "n_group number = %d\n", n_group );
	if ( fread ( &n_entry, sizeof (int), 1, fp_gen ) != 1 )
	{
		printf ( "SimulogDataLoad: file %s read n_entry error\n", genfile );
		return -1;
	}
	printf ( "n_entry number = %d\n", n_entry );
	if ( fread ( &n_init, sizeof (int), 1, fp_gen ) != 1 )
	{
		printf ( "SimulogDataLoad: file %s read n_init error\n", genfile );
		return -1;
	}

	if ( fread ( &n_sceneseg, sizeof (int), 1, fp_gen ) != 1 )
	{
		printf ( "SimulogDataLoad: file %s read n_sceneseg error\n", genfile );
		return -1;
	}

	if ( fread ( &n_model, sizeof (int), 1, fp_gen ) != 1 )
	{
		printf ( "SimulogDataLoad: file %s read n_model error\n", genfile );
		return -1;
	}
	printf ( "model number = %d\n", n_model );

	if ( fread ( &n_tfb, sizeof (int), 1, fp_gen ) != 1 )
	{
		printf ( "SimulogDataLoad: file %s read n_tfb error\n", genfile );
		return -1;
	}
	printf ( "tfb number = %d\n", n_tfb );

	if ( fread ( &n_param, sizeof (int), 1, fp_gen ) != 1 )
	{
		printf ( "SimulogDataLoad: file %s read n_param error\n", genfile );
		return -1;
	}
	printf ( "param number = %d\n", n_param );

	if ( fread ( &n_param_seq, sizeof (int), 1, fp_gen ) != 1 )
	{
		printf ( "SimulogDataLoad: file %s read n_param_seq error\n", genfile );
		return -1;
	}
	printf ( "param seq number = %d\n", n_param_seq );

	if ( fread ( &n_spare, sizeof (int), 1, fp_gen ) != 1 )
	{
		printf ( "SimulogDataLoad: file %s read n_spare error\n", genfile );
		return -1;
	}
	printf ( "spare number = %d\n", n_spare );

	if ( fread ( &n_nlf, sizeof (int), 1, fp_gen ) != 1 )
	{
		printf ( "SimulogDataLoad: file %s read n_nlf error\n", genfile );
		return -1;
	}
	printf ( "NLF number = %d\n", n_nlf );

	if ( fread ( &n_nlf_crv, sizeof (int), 1, fp_gen ) != 1 )
	{
		printf ( "SimulogDataLoad: file %s read n_nlf_crv error\n", genfile );
		return -1;
	}
	printf ( "NLF curve number = %d\n", n_nlf_crv );

	if ( fread ( &n_unit, sizeof (int), 1, fp_gen ) != 1 )
	{
		printf ( "SimulogDataLoad: file %s read n_unit error\n", genfile );
		return -1;
	}
	printf ( "GenUnit number = %d\n", n_unit );

	if ( fread ( &n_timer, sizeof (int), 1, fp_gen ) != 1 )
	{
		printf ( "SimulogDataLoad: file %s read n_timer error\n", genfile );
		return -1;
	}
	printf ( "Timed PRC number = %d\n", n_timer );

	//yyp 2008.3.27	

	if ( (signed) fread (sequencedb, sizeof (SEQUENCE_DEF), n_sequence, fp_gen ) != n_sequence )
	{
		printf ( "SimulogDataLoad: %s read sequencedb error\n", genfile );
		return -1;
	}

	if ( (signed) fread (scenedb, sizeof (SCENE_DEF), n_scene, fp_gen ) != n_scene )
	{
		printf ( "SimulogDataLoad: %s read scenedb error\n", genfile );
		return -1;
	}


	if ( (signed) fread ( stepdb, sizeof ( STEP_DEF), n_step, fp_gen ) != n_step )
	{
		printf ( "SimulogDataLoad: file %s stepdb read error\n", genfile );
		return -1;
	}

	//yyp add 2009.7.8

	if ( (signed) fread ( showdb, sizeof ( SHOW_DEF), n_show, fp_gen ) != n_show )
	{
		printf ( "SimulogDataLoad: file %s showdb read error\n", genfile );
		return -1;
	}

	if ( (signed) fread( groupdb, sizeof ( GROUP_DEF), n_group, fp_gen ) != n_group )
	{
		printf ( "SimulogDataLoad: file %s groupdb read error\n", genfile );
		return -1;
	}

	if ( (signed) fread ( entrydb, sizeof ( ENTRY_DEF), n_entry, fp_gen ) != n_entry )
	{
		printf ( "SimulogDataLoad: file %s entrydb read error\n", genfile );
		return -1;
	}

	printf ( "SimulogDataLoad: Read entrydb ok.\n" );
	
	if ( (signed) fread ( initdb, sizeof (INIT_DEF), n_init, fp_gen ) != n_init )
	{
		printf ( "SimulogDataLoad: file %s initdb read error\n", genfile );
		return -1;
	}

	if ( (signed) fread ( scenesegdb, sizeof (SCENESEG_DEF), n_sceneseg, fp_gen ) != n_sceneseg )
	{
		printf ( "SimulogDataLoad: file %s scenesegdb read error\n", genfile );
		return -1;
	}

	if ( (signed) fread ( mdb, sizeof (MODEL_DEF), n_model, fp_gen ) != n_model )
	{
		printf ( "SimulogDataLoad: file %s modeldb read error\n", genfile );
		return -1;
	}
	printf ( "SimulogDataLoad: Read mdb ok.\n" );

	if ( (signed) fread ( tfbdb, sizeof (TFB_DEF), n_tfb, fp_gen ) != n_tfb )
	{
		printf ( "SimulogDataLoad: file %s tfbdb read error\n", genfile );
		return -1;
	}

	printf ( "SimulogDataLoad: Read tfbdb ok.\n" );

	if ( (signed) fread ( paramdb, sizeof (PARAM_DEF), n_param, fp_gen ) != n_param )
	{
		printf ( "SimulogDataLoad: file %s paramdb read error\n", genfile );
		return -1;
	}
	printf ( "SimulogDataLoad: Read paramdb ok.\n" );

	if ( (signed) fread ( paramseqdb, sizeof (PARAM_DEF), n_param_seq, fp_gen ) != n_param_seq )
	{
		printf ( "SimulogDataLoad: file %s paramseqdb read error\n", genfile );
		return -1;
	}
	printf ( "SimulogDataLoad: Read paramseqdb ok.\n" );

	if ( (signed) fread ( sparedb, sizeof (SPARE_DEF), n_spare, fp_gen ) != n_spare )
	{
		printf ( "SimulogDataLoad: file %s sparedb read error\n", genfile );
		return -1;
	}
	printf ( "SimulogDataLoad: Read sparedb ok.\n" );

	if ( (signed) fread ( nlfdb, sizeof (NLF_DEF), n_nlf, fp_gen ) != n_nlf )
	{
		printf ( "SimulogDataLoad: file %s nlfdb read error\n", genfile );
		return -1;
	}
	printf ( "SimulogDataLoad: Read nlfdb ok.\n" );

	if ( (signed) fread ( nlf_ptr, sizeof (HE_PTR_DEF), n_nlf_crv, fp_gen ) != n_nlf_crv )
	{
		printf ( "SimulogDataLoad: file %s nlf_ptr read error\n", genfile );
		return -1;
	}
	printf ( "SimulogDataLoad: Read nlf_ptr ok.\n" );

	n_x_end	= nlfdb[n_nlf-1].x.end;
	n_y_end	= nlfdb[n_nlf-1].y.end;
	n_nlf_f_end	= nlf_ptr[nlfdb[n_nlf-1].f.end-1].end;
	printf ( "n_nlf= %d\tx_end= %d\ty_end= %d\tf_end= %d\n", 
		n_nlf, n_x_end, n_y_end, n_nlf_f_end );
	//yyp debug? 
	//nlfdata	= *(NLF_DATA_DEF *) calloc ( 1, sizeof (NLF_DATA_DEF) );
	
	//if ( nlfdata == NULL )
	//{
	//	printf ( "SimulogDataLoad: nlfdb *calloc error\n" );
	//	return -1;
	//}
	
	if ( (signed) fread ( (&nlfdata.x[0]), sizeof (double), n_x_end, fp_gen ) != n_x_end )
	{
		printf ( "SimulogDataLoad: file %s nlfdata X read error\n", genfile );
		return -1;
	}
	printf ( "SimulogDataLoad: Read nlf X ok.\n" );
	if ( (signed) fread ( (&nlfdata.y[0]), sizeof (double), n_y_end, fp_gen ) != n_y_end )
	{
		printf ( "SimulogDataLoad: file %s nlfdata Y read error\n", genfile );
		return -1;
	}
	printf ( "SimulogDataLoad: Read nlf Y ok.\n" );
	//yyp debug
	n_y_end = (unsigned int) fread ( (&nlfdata.f[0]), sizeof (double), n_nlf_f_end, fp_gen );
	if(n_y_end !=n_nlf_f_end)
	{
		printf ( "SimulogDataLoad: file %s nlfdata F read error\n", genfile );
		return -1;
	}
	printf ( "SimulogDataLoad: Read nlf F ok.\n" );
	

	if ( (signed) fread ( unitdb, sizeof (UNIT_DATA_DEF), n_unit, fp_gen ) != n_unit )
	{
		printf ( "SimulogDataLoad: file %s unitdb read error\n", genfile );
		return -1;
	}

	if ( (signed) fread ( timerdb, sizeof (TIMER_DEF), n_timer, fp_gen ) != n_timer )
	{
		printf ( "SimulogDataLoad: file %s timerdb read error\n", genfile );
		return -1;
	}

	printf ( "in : n_sequence: %d, n_step: %d, n_show: %d, n_group: %d, \n n_entry: %d, n_init: %d\tn_model:%d\tn_nlf: %d\n", 
		n_sequence, n_step, n_show, n_group, n_entry, n_init, n_model, n_nlf );

	fclose ( fp_gen );

	printf ("SimulogDataLoad OK!\n\n" );
	FillMdlWithParam();

	//yyp debug
	//	FillStepShowID();
	FillDisturbScene();

	//added by yyp 2008.11.12
	if ( ReadSequenceID() == -1 )
	{
		printf ( "sequence_id.txt error!\n" );
		return -1;
	}

	//added by yyp 2009.8.19
	if ( ReadSequenceDBIN() == -1 )
	{
		printf ( "sequence_dbin.txt error!\n" );
		return -1;
	}

	//added by yyp 2009.8.12
	if ( ReadSequenceAuto() == -1 )
	{
		printf ( "sequence_auto.txt error!\n" );
		return -1;
	}
	
	return TRUE;
}

int FillDisturbScene()
{
	int i,j;
	for(i=0;i<n_scene;i++)
	{
		if(scenedb[i].type == TYPE_DISTURB)
		{
			for(j=scenedb[i].head;j<scenedb[i].head+scenedb[i].num;j++)
			{
				GetAnaValueFromName(scenesegdb[j].name,&(scenesegdb[j].data.value));
			}
		}
	}
	return TRUE;
}

//added by yyp 2009.7.8
int FillStepShowID()
{
	int i,j,k;

	char name[SHOWNAME_SIZE];
	for(i=0;i<n_sequence;i++)
	{
		for(j=sequencedb[i].head; j< sequencedb[i].head +sequencedb[i].num ;j++)
		{	
			stepdb[j].showid = -1;
			sprintf(name,"%s__%s",sequencedb[i].seq_name,stepdb[j].o_name);
			for(k=0;k<n_show;k++)
			{
				if( strcmp(name,showdb[k].name) == 0 )
				{
					stepdb[j].showid = k;
					break;
				}
			}
		}
	}
	return 0;
}

//added by yyp 2009.4.14
int FillMdlWithParam()
{
	int i,j,k;
	int tmphead;
	MODEL_DEF mdl_tmp;
	
	for(i=0;i<n_model;i++)
	{
		if ( !strcmp ( mdb[i].type, TMPL_FROMTEMPLATE ) )
		{
			for(j=0;j<n_model;j++)
			{
				if ( !strcmp ( mdb[i].template_name, mdb[j].name ) )
				{
					break;
				}
			}
			if(j==n_model)
			{
				printf ( "SimulogDataLoad: can't find template %s in modeldb error\n",   mdb[i].template_name);
				return -1;
			}
			else
			{
				mdl_tmp=mdb[i];
				memcpy(&mdb[i],&mdb[j],sizeof(MODEL_DEF));
				memcpy(mdb[i].type,mdl_tmp.type,DBNAME_SIZE);
				mdb[i].t_step =  mdl_tmp.t_step ;
				mdb[i].iterations = mdl_tmp.iterations;
				mdb[i].param = mdl_tmp.param; 
				memcpy(mdb[i].name,mdl_tmp.name,DBNAME_SIZE);
				memcpy(mdb[i].template_name,mdl_tmp.template_name,DBNAME_SIZE);
				tmphead = mdb[i].tfb.head;
				mdb[i].tfb.head = n_tfb; 
				mdb[i].spare.head = n_spare;

				for(k=mdb[j].tfb.head;k<mdb[j].tfb.end;k++)
				{
					memcpy(&tfbdb[n_tfb],&tfbdb[k],sizeof(TFB_DEF));
					n_tfb = n_tfb +1;
				}
				for(k=mdb[j].spare.head;k<mdb[j].spare.end;k++)
				{
					memcpy(&sparedb[n_spare],&sparedb[k],sizeof(SPARE_DEF));
					n_spare = n_spare +1;
				}

				mdb[i].tfb.end = n_tfb; 
				mdb[i].spare.end = n_spare;
				for(k=mdb[i].spare.head;k<mdb[i].spare.end;k++)
				{
					sparedb[k].i = sparedb[k].i + mdb[i].tfb.head - tmphead;
					sparedb[k].j = sparedb[k].j + mdb[i].tfb.head - tmphead;
				}
			}
			for(j=mdb[i].tfb.head;j<mdb[i].tfb.end;j++)
			{
				if(tfbdb[j].vin.flag == ON)
				{
					if( strstr(tfbdb[j].vin.dbname, ".") )
					{
						continue;
					}

					for(k=mdb[i].param.head;k<mdb[i].param.end;k++)
					{
						if(!strcmp(tfbdb[j].vin.dbname,paramdb[k].param_name))
						{
							memcpy(tfbdb[j].vin.dbname,paramdb[k].pt_name,DBNAME_SIZE);
							break;
						}
					}
				}
				if(tfbdb[j].output.flag == ON)
				{
					if( strstr(tfbdb[j].output.dbname, ".") )
					{
						continue;
					}
					for(k=mdb[i].param.head;k<mdb[i].param.end;k++)
					{
						if(!strcmp(tfbdb[j].output.dbname,paramdb[k].param_name))
						{
							memcpy(tfbdb[j].output.dbname,paramdb[k].pt_name,DBNAME_SIZE);
							break;
						}
					}
				}
				if(tfbdb[j].a.nlf_tag == ON)
				{
					if( strstr(tfbdb[j].a.nlf_name, ".") )
					{
						continue;
					}
					for(k=mdb[i].param.head;k<mdb[i].param.end;k++)
					{
						if(!strcmp(tfbdb[j].a.nlf_name,paramdb[k].param_name))
						{
							memcpy(tfbdb[j].a.nlf_name,paramdb[k].pt_name,DBNAME_SIZE);
							break;
						}
					}
				}
				if(tfbdb[j].b.nlf_tag == ON)
				{
					if( strstr(tfbdb[j].b.nlf_name, ".") )
					{
						continue;
					}
					for(k=mdb[i].param.head;k<mdb[i].param.end;k++)
					{
						if(!strcmp(tfbdb[j].b.nlf_name,paramdb[k].param_name))
						{
							memcpy(tfbdb[j].b.nlf_name,paramdb[k].pt_name,DBNAME_SIZE);
							break;
						}
					}
				}
				if(tfbdb[j].c.nlf_tag == ON)
				{
					if( strstr(tfbdb[j].c.nlf_name, ".") )
					{
						continue;
					}
					for(k=mdb[i].param.head;k<mdb[i].param.end;k++)
					{
						if(!strcmp(tfbdb[j].c.nlf_name,paramdb[k].param_name))
						{
							memcpy(tfbdb[j].c.nlf_name,paramdb[k].pt_name,DBNAME_SIZE);
							break;
						}
					}
					//added by yyp 2009.10.9
					if( CheckStrIsNumeric ( tfbdb[j].c.nlf_name ) == TRUE)
					{
						tfbdb[j].c.nlf_tag = IS_CONSTANT;
						tfbdb[j].c.data = atof(tfbdb[j].c.nlf_name);
						tfbdb[j].c.dtmp = tfbdb[j].c.data;
					}
				}
				if(tfbdb[j].d.nlf_tag == ON)
				{
					if( strstr(tfbdb[j].d.nlf_name, ".") )
					{
						continue;
					}
					for(k=mdb[i].param.head;k<mdb[i].param.end;k++)
					{
						if(!strcmp(tfbdb[j].d.nlf_name,paramdb[k].param_name))
						{
							memcpy(tfbdb[j].d.nlf_name,paramdb[k].pt_name,DBNAME_SIZE);
							break;
						}
						
					}
				}
			}
		}
	}	
	return TRUE;
}


int ReadSequenceID()
{
	char linebuf[200];
	FILE *fp;
	int n=0;
	seqid.num = n;
	if (( fp = fopen ( seqid_file,"r"))==NULL)
	{
		printf ("==== can't open (w) the file %s! \n",seqid_file );
		return -1;
	}
	else
	{
		/* printf ( "\n==== open (w) %s  success! \n",FileName  ); */
	}
	for(;;)
	{
		if ( fgets ( linebuf, LINE_BUF_LENTH, fp ) == NULL )
		{
			if ( feof ( fp ) )
			{
				break;
			}
			else
			{
				printf ( "This is a Null line!" );
				continue;
			}
		}
		else
		{
			if(!strncmp ( "!", linebuf, 1 ))
			{
				continue;
			}
			if(strlen(linebuf) <=1) continue;
			sscanf(linebuf,"%s	%s",seqid.seg[n].seqid, seqid.seg[n].seqname);
			n++;
			if( n>= MAX_SEQUENCE)
			{
				break;
			}
		}
	}
	seqid.num = n;
	fclose(fp);	
	return TRUE;
}

int ReadSequenceDBIN()
{
	int i;
	char linebuf[200];
	char seqname[100],dbin[100],dbin2[100];
	FILE *fp;
	int id=-1;
	
	for(i=0;i<n_sequence;i++)
	{
		strcpy(sequencedb[i].dbin[0],"NULL");
		strcpy(sequencedb[i].dbin[1],"NULL");
	}
	if (( fp = fopen ( seqdbin_file,"r"))==NULL)
	{
		printf ("==== can't open (w) the file %s! \n",seqid_file );
		return -1;
	}
	else
	{
		/* printf ( "\n==== open (w) %s  success! \n",FileName  ); */
	}
	for(;;)
	{
		if ( fgets ( linebuf, LINE_BUF_LENTH, fp ) == NULL )
		{
			if ( feof ( fp ) )
			{
				break;
			}
			else
			{
				printf ( "This is a Null line!" );
				continue;
			}
		}
		else
		{
			if(!strncmp ( "!", linebuf, 1 ))
			{
				continue;
			}
			if(strlen(linebuf) <=1) continue;
			sscanf(linebuf,"%s	%s	%s",seqname, dbin, dbin2);
			id = GetSequenceIdByName ( seqname );
			if(id>=0 && id<n_sequence)
			{
				strcpy(sequencedb[id].dbin[0],dbin);
				strcpy(sequencedb[id].dbin[1],dbin2);
			}
		}
	}
	fclose(fp);	
	return TRUE;
}

int ReadSequenceAuto()
{
	char linebuf[200];
	FILE *fp;
	int n=0;
	seqid_auto.num = 0;
	if (( fp = fopen ( seqauto_file,"r"))==NULL)
	{
		printf ("==== can't open (w) the file %s! \n",seqauto_file );
		return -1;
	}
	else
	{
		/* printf ( "\n==== open (w) %s  success! \n",FileName  ); */
	}
	for(;;)
	{
		if ( fgets ( linebuf, LINE_BUF_LENTH, fp ) == NULL )
		{
			if ( feof ( fp ) )
			{
				break;
			}
			else
			{
				printf ( "This is a Null line!" );
				continue;
			}
		}
		else
		{
			if(!strncmp ( "!", linebuf, 1 ))
			{
				continue;
			}
			if(strlen(linebuf) <=1) continue;
			sscanf(linebuf,"%s",seqid_auto.seg[n].seqname);
			n++;
			if( n>= MAX_SEQUENCE)
			{
				break;
			}
		}
	}
	seqid_auto.num = n;
	fclose(fp);	
	return TRUE;
}

