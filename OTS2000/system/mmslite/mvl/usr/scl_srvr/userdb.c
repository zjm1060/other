#include "glbtypes.h"
#include "sysincs.h"
#include "scl.h"	/* for SCL_INFO, SCL_LD, etc.	*/
#include "usermap.h"	/* for DATA_MAP, etc.	*/
#include "sx_log.h"
#include "mvl_defs.h"
#include "mvl_uca.h"
#include "str_util.h"	/* for strn..._safe	protos	*/
#include "db_61850.h"	/* "deadband" structures & functions	*/
#include "../../../../dms/v4.0a/inc/dms_com.h"
#include "../../../../dps/v4.0a/inc/ipc_common.h"


#ifdef DEBUG_SISCO
SD_CONST static ST_CHAR *SD_CONST thisFileName = __FILE__;
#endif

#define LINE_BUF_LEN	1024	/* max len of line read from file.	*/


/************************************************************************/
/*			datamap_cfg_rd2					*/
/* Read each line in the "Mapping Cfg" file. Copy data from one line	*/
/* to one entry in the "map_arr" array in a DATA_MAP_LD structure.	*/
/* Find the correct DATA_MAP_LD structure by searching the 'map_ld_array'*/
/* for a matching "ldevice".						*/
/* When done reading, sort the "map_arr" array in each DATA_MAP_LD.	*/
/* NOTE: Ignore empty lines or lines beginning with '#'.		*/
/************************************************************************/
static ST_RET init_ld (ST_CHAR *datamap_cfg_filename,
	DATA_MAP_LD **map_ld_array,	/* array of ptrs to LD mappings	*/
	ST_UINT ld_count)		/* num array elements		*/
  {
FILE *in_fp;
ST_CHAR in_buf[LINE_BUF_LEN];	/* buffer for one line read from file	*/
ST_CHAR token_buf[LINE_BUF_LEN];	/* copy of "in_buf". Modified by get_next_string.*/
ST_CHAR *curptr;		/* ptr to current position in token_buf	*/
char seps[] = ",\t\n";
DATA_MAP *map_entry;
ST_INT line_num;		/* number of lines in file	*/
ST_UINT entry_num;		/* number of valid entries in file	*/
ST_RET retcode = SD_SUCCESS;
ST_CHAR *ldevice;		/* first token on line	*/
ST_CHAR *leaf;			/* 2nd token	*/
DATA_MAP_LD *map_ld;
ST_UINT ld_idx;			/* current LD array index	*/

  in_fp = fopen (datamap_cfg_filename, "r");
  if (in_fp == NULL)
    {
    SXLOG_ERR1 ("Error opening input file '%s'", datamap_cfg_filename);
    return (SD_FAILURE);
    }

  /* CRITICAL: reset "tmp_count" for each LD before 2nd pass.	*/
  for (ld_idx = 0; ld_idx < ld_count; ld_idx++)
    {
    map_ld_array[ld_idx]->tmp_count = 0;
    }

  /* Read one line at a time from the input file	*/
  /* NOTE: length of every line checked on first pass, so no need to recheck here.*/
  entry_num = 0;	
  line_num = 0;
  while (fgets (in_buf, sizeof(in_buf) - 1, in_fp) != NULL)
    {
    line_num++;

    /* Make a copy of input buffer to be used by "get_next_string", which will
     * modify the copied buffer (token_buf). Keep input buffer (in_buf)
     * intact.
     * NOTE: could use "strtok" here, but it doesn't nicely handle extra
     *       delimiters or quoted strings. 
     */
    strcpy (token_buf, in_buf);
    curptr = token_buf;	/* init "curptr"	*/
    ldevice = get_next_string(&curptr, seps);	/* "curptr" adjusted by this funct	*/

    /* If NULL, this is empty line. If first char is '#', this is comment line.*/
    if (ldevice == NULL || ldevice[0] == '#')
      continue;		/* Ignore empty lines & comment lines	*/
    if (ldevice [0] == '\0')
      {
      /* First token is empty. This is probably empty line.	*/
      /* Ignore this line, but log error if more tokens found. 	*/
      if ((leaf = get_next_string (&curptr, seps)) != NULL)
        SXLOG_ERR3 ("Input ignored because first token is empty at line %d in '%s'. Second token='%s'",
                    line_num, datamap_cfg_filename, leaf);
      continue;
      }

    /* Find map_ld with same "ldevice" as this leaf.		*/
    /* Set "map_entry" to next available ptr in the array.	*/
    map_entry = NULL;
    for (ld_idx = 0; ld_idx < ld_count; ld_idx++)
      {
      map_ld = map_ld_array[ld_idx];
      if (strcmp (map_ld->ldevice, ldevice) == 0)
        {
        /* Set map_entry to next available ptr in the array.	*/
        map_entry = map_ld->map_arr[map_ld->tmp_count];
        map_ld->tmp_count++;	/* increment counter	*/
        break;
        }
      }
    if (map_entry==NULL)
      {
      SXLOG_ERR2 ("LD='%s' found only on 2nd pass reading '%s'. File changed?",
                 ldevice, datamap_cfg_filename);
      retcode = SD_FAILURE;
      break;	/* break out of main loop	*/
      }

    /* Save data from this line in "map_entry".	*/
    retcode = datamap_save_line (
              datamap_cfg_filename,
              line_num,
              in_buf,
              map_entry,	/* save data in this struct	*/
              curptr,
              seps);

    /* If ANYTHING failed so far, stop looping.	*/
    if (retcode)
      break;	/* get out of loop	*/
    entry_num++;
    }	/* end main "while" loop	*/

  /* Now, for each LD, sort all entries alphabetically by leaf name.	*/
  if (retcode == SD_SUCCESS)
    {
    for (ld_idx = 0; ld_idx < ld_count; ld_idx++)
      {
      map_ld = map_ld_array[ld_idx];
      /* Both counts must match (i.e. we counted the same both times).	*/
      if (map_ld->tmp_count != map_ld->map_count)
        {	/* should only happen if file modified between passes*/
        SXLOG_ERR2 ("Entry count for LD='%s' changed on 2nd pass reading '%s'",
                  map_ld->ldevice, datamap_cfg_filename);
        retcode = SD_FAILURE;
        break;	/* stop on first error	*/
        }


	}
    }

  /* Log if successful. Otherwise, error already logged.	*/
  if (retcode == SD_SUCCESS)
    SXLOG_FLOW3 ("Done reading file '%s', %d lines, %d valid entries",
                 datamap_cfg_filename, line_num, entry_num);

  fclose (in_fp);
  return (retcode);
  }