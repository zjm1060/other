/************************************************************************/
/* SISCO SOFTWARE MODULE HEADER *****************************************/
/************************************************************************/
/*   (c) Copyright Systems Integration Specialists Company, Inc.,	*/
/*              1986-2001 All Rights Reserved             		*/
/*									*/
/* MODULE NAME : ukey.c 						*/
/* PRODUCT(S)  : MMSEASE						*/
/*									*/
/* MODULE DESCRIPTION : 						*/
/* This module contains function key handling functions.		*/
/* and other misc. It should be used when working in the		*/
/* UNIX environment in place of 'fkey.c' or 'nkey.c'.                   */
/*									*/
/* GLOBAL FUNCTIONS DEFINED IN THIS MODULE :				*/
/*									*/
/* MODIFICATION LOG :							*/
/*  Date     Who   Rev			Comments			*/
/* --------  ---  ------   -------------------------------------------	*/
/* 05/16/01  EJV     08    SYS_5: changed term_init, term_end to POSIX	*/
/*			   compatible (old didn't work on Tru64 csh).	*/
/* 08/28/98  EJV     07    Renamed functions echo to set_echo_on	*/
/*			                   noecho to set_echo_off.	*/
/* 05/22/98  EJV     06    Moved strnicmp,strcmpi,stricmp to cfg_util.c	*/
/* 02/27/98  EJV     05    Replaced faulty strcmpi func (strings were	*/
/*			   compared for len of smaller str only!).	*/
/* 01/26/98  EJV     04    Moved #... to beginning of line -Digital UNIX*/
/* 01/22/98  NAV     03    Added menu key functionality			*/
/* 05/27/97  DSF     02    No need to include suicacse.h		*/
/* 05/22/97  RKR     01    Changed the way select is called in kbhit	*/
/* 04/02/97  DTL   7.00    MMSEASE 7.0 release. See MODL70.DOC for	*/
/*			   history.					*/
/************************************************************************/

#include "glbtypes.h"

#ifndef PSOS_SYSTEM
#include <stdio.h>
#include <ctype.h>
#endif

#if defined(__alpha)
#include <sys/time.h>
#endif

#if (SYSTEM_SEL & (QNX_C86))
#include <dev.h>
#include <io.h>
#else
#if (SYSTEM_SEL & SYSVXWORKS)
#include <tyLib.h>
#include <ioLib.h>
#include <selectLib.h>
extern int vxfd;		/* See mmsapp.c	*/
/*
#else	/* !SYSVXWORKS	*/
#ifndef PSOS_SYSTEM
#include <termio.h>
#endif
*/
#endif	/* !SYSVXWORKS	*/
#endif

#if (SYSTEM_SEL & (SYS_QNX4 | NEW_SYSTEM | SYS_5))	/* POSIX compatible	*/
#if defined (__hpux)
#include <termios.h>
#include <termio.h>
/*
#include <sys/termios.h>
#include <termio.h>
*/
#else
#include <termios.h>
#include <sys/select.h>
#endif
#include <errno.h>
struct termios save_settings;
#endif	/* (SYS_QNX4 | NEW_SYSTEM | SYS_5)	*/

#include "sysincs.h"

#include "fkeydefs.h"
#include "scrndefs.h"

#ifdef PSOS_SYSTEM
extern char LastKeyHit;  /* This is the last key that cam into the system */
extern short AKeyWasHit;  /* This is the flag that say's that something happned */
extern short EchoFlag; /* To echo the data on the terminal */
extern unsigned long *TasksInSystemID;
extern short NetReadDataPresent; /* This is a flag that say's that there is net data */
#include <ctype.h>
#endif

static ST_INT nlfn (ST_VOID);
ST_VOID bad_key(ST_VOID);
ST_VOID fun_exe(ST_INT);
int     kbhit(ST_VOID);
ST_CHAR	getkch();

static	ST_CHAR console_char = 0x00;

ST_VOID (*funct_1) ();
ST_VOID (*funct_2) ();
ST_VOID (*funct_3) ();
ST_VOID (*funct_4) ();
ST_VOID (*funct_5) ();
ST_VOID (*funct_6) ();
ST_VOID (*funct_7) ();
ST_VOID (*funct_8) ();
ST_VOID (*funct_9) ();
ST_VOID (*funct_10) ();
ST_VOID (*funct_menu) ();

/*************************************************************************/

/* A table of function pointers is used to select the action
performed when a function key is pressed. The fun_exe () routine
selects the function based on the key code. The fun_null can be
used to set all fuunction key pointers to the "Illegal Key"
display function. */

#define F1  0x31	   /* 1 */
#define F2  0x32	   /* 2 */
#define F3  0x33	   /* 3 */
#define F4  0x34	   /* 4 */
#define F5  0x35	   /* 5 */
#define F6  0x36	   /* 6 */
#define F7  0x37	   /* 7 */
#define F8  0x38	   /* 8 */
#define F9  0x39	   /* 9 */
#define F10 0x30	   /* 0 */
#define MENU_KEY	0x1B	/* the escape key	*/

/************************************************************************/
/*			nlfn						*/
/* function to just return.  Used when need to set service routine to	*/
/* a dummy function							*/
/************************************************************************/

static ST_INT nlfn (ST_VOID)
{
  return(0);
}

ST_INT (*servefun)() = nlfn;

/************************************************************************/
/*			wait_debug_log					*/
/************************************************************************/

#ifdef UTIL_LIB
ST_VOID wait_debug_log (fptr,str)
FILE *fptr;
ST_CHAR *str;
  {
  fprintf (fptr,"\n %s",str);
  fflush(fptr);
  }

#else	/* !UTIL_LIB	*/

ST_VOID wait_debug_log (FILE *fptr, ST_CHAR *str)
  {
  flush_keys ();
  fprintf (fptr,"\n %s",str);
  fflush(fptr);
  if (fptr == stdout)
    {
    while (!kbhit())
      (*servefun) ();			/* execute background task	*/
    }
  flush_keys ();
  }
#endif	/* !UTIL_LIB	*/

/****************************************************************************/
/*                           log_hex_bytes				    */
/* This function provides a hex dump facility.				    */
/****************************************************************************/

#define cvt_nibble(a)		((a) > 9 ? (a) + 'A' - 10 : (a) + '0')
#define DEBUG_BUF_LEN 100

ST_VOID log_hex_bytes (FILE *dest, ST_UCHAR *ptr, ST_INT len)
                                /* pointer to hex data */
                                           /* length of data */
{                  
register ST_INT i;  
register ST_INT ascii_index;
register ST_INT hex_index;
ST_CHAR debug_buf[DEBUG_BUF_LEN+1]; /* output buffer			*/
ST_INT curlen;
ST_CHAR ch;
ST_UCHAR hex;
ST_INT addr = 0;

  /* format hex data							*/

  while (len)
    {
    curlen = min(len,16); 		/* # char's in current line	*/
    sprintf (debug_buf," %04X  ",addr);	/* data offset 			*/
    hex_index = 7;			/* where to put hex data 	*/
    ascii_index = 58;			/* where to put ascii conversion */
    debug_buf[ascii_index++] = '*';

    for (i = 0; i < DEBUG_BUF_LEN; ++i)
      debug_buf[i] = ' ';
    fprintf (dest,"%s","\n");

    for (i = 0; i < curlen; i++)	/* for each byte in this line	*/			    	
      {	  		    		
      ch = *ptr++;  			/* get next character		*/

      hex = (ch >> 4) & 0x0f;		/* write HEX chars for the byte	*/
      hex = cvt_nibble(hex);
      debug_buf[hex_index++] = hex;
      hex = ch & 0x0f;
      hex = cvt_nibble(hex);
      debug_buf[hex_index++] = hex;

      debug_buf[hex_index++] = ' ';

      if (isprint(ch))			/* print ASCII portion		*/
        debug_buf[ascii_index] = ch;
      else
        debug_buf[ascii_index] = '.';	/* just put a '.' there 	*/
      ascii_index++;

      if (i == 7)			/* half way through line	*/
        debug_buf[hex_index++] = ' ';   /* put an extra space here 	*/
      } 				/* end of line			*/

    debug_buf[ascii_index++] = '*';
    debug_buf[ascii_index++] = 0;
    for (i = hex_index; i < 58; i++)
      debug_buf[i] = ' ';  /* clear the buffer between hex and ascii data */

					/* write out the line		*/
    fprintf (dest,"%s",debug_buf);

    addr += 16;				/* prepare for next line	*/
    len -= curlen;
    }
  }

/************************************************************************/
/*				term_init				*/
/*  This function must be called before calling kbhit.	It initializes	*/
/*  the terminal to do raw types of input without having any of it	*/
/*  buffered.								*/
/************************************************************************/
#if (SYSTEM_SEL & (QNX_C86))
int	term_init ()
{
	int option_save;
	
	if ((option_save = get_option (stdin))  ==  -1)
		perror( "\nterm_init: Couldn't get the terminal characteristics" );
	else
		{
		if (set_option (stdin, option_save & (~(ECHO|EDIT)))  ==  -1)
			perror( "\nterm_init: Couldn't set the terminal for Raw input" );
		}
	return (option_save);
}
#endif	/* QNX_C86	*/

#if (SYSTEM_SEL & SYSVXWORKS)
ST_VOID	term_init ()
  {
  if (ioctl( 0, FIOSETOPTIONS, OPT_RAW | OPT_ABORT | OPT_CRMOD) )
    perror( "\nterm_init: Couldn't set the terminal characteristics" );
  }
#endif	/* SYSVXWORKS	*/

#if (SYSTEM_SEL & (SYS_QNX4 | SYS_5 | NEW_SYSTEM))	/* POSIX Compatible	*/
ST_VOID	term_init()
{
struct	termios	new_settings;

  if (tcgetattr (0, &save_settings) == -1)
    perror( "\nterm_init: Couldn't get the terminal characteristics" );
  else
    {
    memcpy (&new_settings, &save_settings, sizeof (struct termios));
    new_settings.c_cc[VMIN]=1;
    new_settings.c_cc[VTIME]=0;
    new_settings.c_lflag &= ~(ECHO|ICANON|ISIG|ECHOE|ECHOK);
    if (tcsetattr (0, TCSADRAIN, &new_settings) == -1)
      perror( "\nterm_init: Couldn't set the terminal for Raw input" );
    }
  return;
}
#endif	/* (SYS_QNX4 | SYS_5 | NEW_SYSTEM)	*/

#if 0 /* was (SYSTEM_SEL & (SYS_5) ) */
/* NOTE: This code was not working properly on Tru64 csh */
ST_VOID	term_init( save_ptr )
struct	termio	*save_ptr;
{
struct	termio	terminal_control_block;
struct	termio	*arg;

  arg = &terminal_control_block;
  if (ioctl( 0, TCGETA, save_ptr ) )
    perror( "\nterm_init: Couldn't get the terminal characteristics" );
  else
    {
    *arg = *save_ptr;
    arg->c_lflag &= ~ICANON;		/* get rid of buffering input	*/
    arg->c_cc[VTIME] = 0;		/* time out immediatedly	*/
    arg->c_cc[VMIN]  = 0;		/* set a 0 character minimum	*/
    if ( ioctl( 0, TCSETA, arg ) )
      perror( "\nterm_init: Couldn't set the terminal for Raw input" );
    }
}
#endif	/* SYS_5	*/

/************************************************************************/
/*				term_rest				*/
/*  This function must be called at the end of a program if term_init	*/
/*  has been called.  A rest of the terminal to it's state pointed to   */
/*  by save_ptr will be done.						*/
/************************************************************************/

#if (SYSTEM_SEL & (QNX_C86))
ST_VOID	term_rest (option_save)
int		option_save;
{
	if (set_option (stdin, option_save)  ==  -1)
		perror( "\nterm_rest: Couldn't reset the terminal attributes." );
}
#endif	/* QNX_C86	*/

#if (SYSTEM_SEL & SYSVXWORKS)
ST_VOID	term_rest ()
  {
  if (ioctl( 0, FIOSETOPTIONS, OPT_TERMINAL) )
    perror( "\nterm_rest: Couldn't reset the terminal characteristics" );
  }
#endif	/* SYSVXWORKS	*/

#if (SYSTEM_SEL & (SYS_5 | SYS_QNX4 | NEW_SYSTEM))	/* POSIX Compatible	*/
ST_VOID	term_rest ()
  {
  if (tcsetattr (0, TCSADRAIN, &save_settings) == -1)
    perror( "\nterm_rest: Couldn't reset the terminal characteristics" );
  }
#endif	/* (SYS_5 | SYS_QNX4 | NEW_SYSTEM)	*/

#if 0 /* was (SYSTEM_SEL & (SYS_5) ) */
/* NOTE: This code was not working properly on Tru64 csh */
ST_VOID	term_rest( save_ptr )
struct	termio	*save_ptr;
{
  if ( ioctl( 0, TCSETA, save_ptr ) )
    perror( "\nThis terminal is hosed." );
}
#endif	/* SYS_5	*/

#ifdef PSOS_SYSTEM
/************************************************************************/
/*				kbhit					*/
/************************************************************************/
int  	kbhit(ST_VOID)
  {
  int ReturnCode = 0;
  if (AKeyWasHit == 1)
    {
    console_char = LastKeyHit;
    ReturnCode = 1;
    }
  return (ReturnCode);  
  }
#else
/************************************************************************/
/*				kbhit					*/
/************************************************************************/

int  	kbhit(ST_VOID)
{
int	i;
ST_CHAR	ret_char;
#if (SYSTEM_SEL & (SYSVXWORKS | SYS_QNX4 | NEW_SYSTEM | SYS_5))
  fd_set readfds;
  fd_set writefds;
  fd_set exceptfds;
  int nfds;  
  struct timeval stTimeVal;
#endif

  fflush( stdout );
#if (SYSTEM_SEL & (QNX_C86))
  if (char_waiting (stdin))		/* if a key has been hit */
    i = read (0, &ret_char, 1);
  else
    i = 0;

  if ( i )
    {
    console_char = ret_char;
    return( SD_TRUE );
    }
  else
    {
    return( SD_FALSE );
    }
#else
#if (SYSTEM_SEL & (SYSVXWORKS | SYS_QNX4 | NEW_SYSTEM | SYS_5))
  /* Must do select before read or you'll get blocked.  */
  FD_ZERO(&readfds);
  FD_ZERO(&writefds);
  FD_ZERO(&exceptfds);
  FD_SET(0 , &readfds);		/* just do select on "stdin"	*/
  stTimeVal.tv_sec  = 0;    /*** wait for 0 second ***/
  stTimeVal.tv_usec = 0;
  errno=0;
/*  nfds=select(FD_SETSIZE,(int *)&readfds,(int *)&writefds,(int *)&exceptfds,&stTimeVal);
Modified 2006.12.22 zy*/
  nfds=select(FD_SETSIZE,&readfds,&writefds,&exceptfds,&stTimeVal);

  if (nfds <= 0)
    return (SD_FALSE); /* nothing to read.     */
  else
    return (SD_TRUE);  /* got something        */


#endif	/* SYSVXWORKS | SYS_QNX4 | NEW_SYSTEMi | SYS_5*/
#endif
}
#endif

/************************************************************************/
/* Functions unused by the MMS provider.                                */
/************************************************************************/

#ifndef UTIL_LIB

/**************************************************************************/
/* check_key - function to check for local command pending, execute if so.*/
/**************************************************************************/

/* Check for key pressed, if not just return.				  */
/* If so, check to see if function key, if not: illegal key.		  */
/* If function key, execute the function.					  */

ST_INT check_key ()
  {
ST_INT key;		     /* key gets character code 		  */

  if (kbhit())			/* Key Pressed ?			*/
    {
#ifdef PSOS_SYSTEM
    key = (ST_INT) get_a_char();
#else
    key = (ST_INT) getchar();
#endif
    if (key == MENU_KEY)
      {
      if (funct_menu)
        (*funct_menu) ();	/* may be used to refresh menu screen	*/
      return (0);
      }
    else if (key < 0x30 || key > 0x39)	/* check for 1-10		*/
      {
      return (0);			/* ret if not			*/
      }
    else
      {
      if (key == 0x30)
	return (1);
      else
	{
	fun_exe (key);
	return (0);
	}
      }
    }
  else
    {
    return (0);
    }
  }



/**************************************************************************/
/* Function to make all function keys illegal				  */
/**************************************************************************/
ST_VOID fun_null ()
{
  funct_1  = bad_key;
  funct_2  = bad_key;
  funct_3  = bad_key;
  funct_4  = bad_key;
  funct_5  = bad_key;
  funct_6  = bad_key;
  funct_7  = bad_key;
  funct_8  = bad_key;
  funct_9  = bad_key;
  funct_10 = bad_key;
  }


/**************************************************************************/
/* Illegal Key routine							  */
/* Need to Beep Here							  */
/**************************************************************************/

ST_VOID bad_key ()
  {
  flush_keys ();
  ; /* just ignore for now */
  }

/**************************************************************************/
/* Message display and wait function, used to display error messages	  */
/**************************************************************************/

#define DELAY_COUNT 32000

ST_VOID key_err (ST_CHAR msg[])

  {
ST_INT i;

  CLEARSCR;
  printf ("\n\n\n\n\n");
  printf ("%s",msg);
#ifdef PSOS_SYSTEM
    t_suspend(0);
#else
  for (i = 0; i < DELAY_COUNT; ++i)	/* just delay here		  */
    ;
  for (i = 0; i < DELAY_COUNT; ++i)	/* just delay here		  */
    ;
#endif  
  }

/************************************************************************/
/* Function to display a message and wait for a key hit 		*/

ST_VOID wait_msg (ST_CHAR *str)
  {
#ifndef PSOS_SYSTEM
  flush_keys ();
  printf ("\n %s",str);
  while (!kbhit())
    (*servefun) ();			/* execute background task	*/
  flush_keys ();
#else
  flush_keys ();
  printf ("\n %s",str);
  t_suspend(0);
#endif
  }

ST_VOID wait_key ()
  {
#ifdef PSOS_SYSTEM
  t_suspend(0);
#else
  while (!kbhit())
    (*servefun) ();			/* execute background task	*/
#endif
  }


/**************************************************************************/
/*	    Function Key Function Select Routine			  */
/* Select the key function by using the function pointer		  */
/**************************************************************************/


ST_VOID fun_exe (ST_INT key)   /* key code is passed to this routine        */
  {			  /* This must be the second ST_CHAR key sequence */
  switch (key)
     {
     case 0x31: 	  /* F1 */
       (*funct_1) ();
     break;

     case 0x32: 	  /* F2 */
       (*funct_2) ();
     break;

     case 0x33: 	  /* F3 */
       (*funct_3) ();
     break;

     case 0x34: 	  /* F4 */
       (*funct_4) ();
     break;

     case 0x35: 	  /* F5 */
       (*funct_5) ();
     break;

     case 0x36: 	  /* F6 */
       (*funct_6) ();
     break;

     case 0x37: 	  /* F7 */
       (*funct_7) ();
     break;

     case 0x38: 	  /* F8 */
       (*funct_8) ();
     break;

     case 0x39: 	  /* F9 */
	(*funct_9) ();
     break;

     case 0x30: 	 /* F10 */
       (*funct_10) ();
     break;

     default:		 /* Not a Function Key */
       bad_key ();
     break;
     }
  }


/************************************************************************/
/*				getkch					*/
/************************************************************************/

ST_CHAR	getkch(ST_VOID)
{
  return( console_char );
}

/************************************************************************/
/*				get_a_char				*/
/************************************************************************/

#ifdef PSOS_SYSTEM
ST_CHAR	get_a_char()
  {
  if (AKeyWasHit == 0)
    {
    /* Put Me to Sleep for some time */
    t_suspend(TasksInSystemID[0]);
    }

  if (AKeyWasHit == 1)
    {
    console_char = LastKeyHit;
    AKeyWasHit = 0;
    }
    
  return( console_char );
}

#else
ST_CHAR	get_a_char()
{
  while(!kbhit());
  console_char = (ST_CHAR) getchar();
  return( console_char );
}
#endif


/*************************************************************************/
/* Function to flush the keyboard character buffer			 */

ST_VOID flush_keys ()
  {
#ifdef PSOS_SYSTEM

  AKeyWasHit = 0;
  console_char = 0x00;
  LastKeyHit = 0x00;

#else
  while (kbhit()) /* this gets all pending characters & trashs them */
    get_a_char();

  return;
#endif
  }

/************************************************************************/
/*				set_echo_on				*/
/************************************************************************/
#if (SYSTEM_SEL & (QNX_C86))
ST_VOID	set_echo_on (ST_VOID)
{
	int option_save;
	
	if ((option_save = get_option (stdin))  ==  -1)
		perror( "\nset_echo_on: Couldn't get the terminal characteristics" );
	else
		{
		if (set_option (stdin, option_save | ECHO)  ==  -1)
			perror( "\nset_echo_on: Couldn't set the terminal for echo" );
		}
}
#else
ST_VOID	set_echo_on (ST_VOID)
{
#if !(SYSTEM_SEL & SYS_5)
#if !(SYSTEM_SEL & (SYSVXWORKS | SYS_QNX4 | NEW_SYSTEM))
#ifndef PSOS_SYSTEM
static struct	termio	terminal_control_block;
static struct	termio	*arg;

  arg = &terminal_control_block;
  if (ioctl( 0, TCGETA, arg ) )
    perror( "\nset_echo_on: Couldn't get the terminal characteristics" );
  else
    {
    arg->c_lflag |= ECHO;		/* enable echo			*/
    if ( ioctl( 0, TCSETA, arg ) )
      perror( "\nset_echo_on: Couldn't set the terminal for echo" );
    }
#endif   /* Not PSOS System */
#endif	/* !SYSVXWORKS	*/
#endif
}
#endif

/************************************************************************/
/*				set_echo_off				*/
/************************************************************************/
#if (SYSTEM_SEL & (QNX_C86))
ST_VOID	set_echo_off (ST_VOID)
{
	int option_save;
	
	if ((option_save = get_option (stdin))  ==  -1)
		perror( "\nset_echo_off: Couldn't get the terminal characteristics" );
	else
		{
		if (set_option (stdin, option_save & (~ECHO))  ==  -1)
			perror( "\nset_echo_off: Couldn't set the terminal for noecho" );
		}
}
#else
ST_VOID	set_echo_off (ST_VOID)
{
#if !(SYSTEM_SEL & SYS_5)
#if !(SYSTEM_SEL & (SYSVXWORKS | SYS_QNX4 | NEW_SYSTEM))
#ifndef PSOS_SYSTEM
static struct	termio	terminal_control_block;
static struct	termio	*arg;

  arg = &terminal_control_block;
  if (ioctl( 0, TCGETA, arg ) )
    perror( "\nset_echo_off: Couldn't get the terminal characteristics" );
  else
    {
    arg->c_lflag &= ~ECHO;		/* disable the echo bit 	*/
    if ( ioctl( 0, TCSETA, arg ) )
      perror( "\nset_echo_off: Couldn't set the terminal for noecho" );
    }
#endif  /* PSOS System */
#endif	/* !SYSVXWORKS	*/
#endif
}
#endif

ST_VOID list_bytes (ST_UCHAR *ptr, ST_INT len)
  {
  while (len)
    {
    printf (" %02X",*(ptr++) &0xff);
    len--;
    }
  }

/************************************************************************/
/*			list_ascii					*/
/************************************************************************/

ST_VOID list_ascii (ST_CHAR *ptr, ST_INT len)
  {
  while (len)
    {
    printf ("%c",*(ptr++) );
    len--;
    }
  }

/************************************************************************/
/*			ask						*/
/* Ask a yes/no question and return the answer. 			*/
/************************************************************************/
/*
ST_VOID  strget (char *);
*/

ST_BOOLEAN ask (ST_CHAR *question, ST_BOOLEAN default_ans)
  {
ST_CHAR  ans [100];
ST_INT	ret_ans;

  printf ("%s", question);
  flush_keys ();
  ans [0] = '?';
  gets (ans);
  if (ans [0] == 'n' || ans [0] == 'N')
    ret_ans = 0;
  else
    {
    if (ans [0] == 'y' || ans [0] == 'Y')
      ret_ans = 1;
    else
      ret_ans = default_ans;
    }

  return (ret_ans);
  }

/**************************************************************************/
/* Function to execute function key table until F10 is pressed. 	  */
/**************************************************************************/

ST_VOID do_fun (ST_VOID)
  {
ST_INT key;

if (SD_TRUE) return;

  for (;;)
    {
#ifdef PSOS_SYSTEM
    key = (ST_INT) get_a_char();
#else
    key = (ST_INT) getchar();
#endif
    if (key)				/* function keys start with 0	  */
      bad_key ();			/* Error if not function key	  */
    else
      {
#ifndef PSOS_SYSTEM
      key = (ST_INT) getchar();
#else
      key = (ST_INT) get_a_char();
#endif
      if (key != F10)
    	fun_exe (key);			/* Execute function key 	  */
      else
	return;
      }
    }
  }

/************************************************************************/

ST_VOID list_words (ST_UINT *ptr, ST_INT len)
  {
  while (len)
    {
    printf (" %04X",*(ptr++));
    len--;
    }
  }


#else	/* UTIL_LIB	*/

ST_VOID wait_msg (str)
ST_CHAR *str;
  {
  printf ("\n %s",str);
  }


#endif	/* UTIL_LIB	*/


