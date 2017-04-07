/****************************************************************************
   MODULE: recio.h
  PURPOSE: record input/output header file included in application
COPYRIGHT: (C) 1994-1995, William Pierpoint
  VERSION: 2.13
  RELEASE: September 4, 1995
*****************************************************************************/

#ifndef RECIO_H
#define RECIO_H

/* include FILE and size_t types */
#ifndef EOF
#include <stdio.h>
#endif

/* include struct tm and time_t types */
#ifndef CLOCKS_PER_SEC
#include <time.h>
#endif

/* define SIZE_T_MAX */
#ifndef SIZE_T_MAX
#define SIZE_T_MAX (~(size_t)0)
#endif

/* structure of REC abstract data type */
typedef struct {
        int      r_fd;       /* descriptor */
        unsigned r_flags;    /* status flags */
        FILE    *r_fp;       /* file pointer */
  const char    *r_fname;    /* pointer to name of file */
        long     r_recno;    /* record number */
        unsigned r_fldno;    /* field number */
        size_t   r_colno;    /* column number */
        size_t   r_recsiz;   /* size of record buffer */
        size_t   r_reclen;   /* length of record buffer */
        char    *r_recp;     /* pointer to record buffer */
        size_t   r_fldsiz;   /* size of field buffer */
        char    *r_fldp;     /* pointer to field buffer */
        int      r_fldch;    /* field separator character */
        int      r_txtch;    /* text delimiter character */
  const char    *r_tmfmt;    /* pointer to time format string */
        int      r_cxtno;    /* context number */
} REC;

/* structure of rpos_t abstract data type */
typedef struct {
        int      r_fd;       /* descriptor */
        unsigned r_fldno;    /* field number  */
        size_t   r_colno;    /* column number */
        long     r_recno;    /* record number */
} rpos_t;

/* constant expression macros */
#define RECBUFSIZ   80       /* minimum size of record buffer */
#define FLDBUFSIZ   20       /* minimum size of field buffer */
#define RECFLDCH   ' '       /* record field separator character */
#define RECTXTCH   ' '       /* text delimiter character */
#define RECBEGYR  1951       /* year time format %y begins */

#define ROPEN_MAX   12       /* max no of rec streams open at same time */
#define R_READ       0       /* read mode */
#define R_WRITE      1       /* write/append mode */

/* compiler dependencies */
#if defined (__BORLANDC__)
#define NSBUFSIZ    34       /* string buffer sized for ltoa, dtoa, etc */

#ifndef TIME_T_MIN
#define TIME_T_MIN  ((time_t)0)
#endif

#ifndef TIME_T_MAX
#ifndef LONG_MAX
#include <limits.h>
#endif
#define TIME_T_MAX  ((time_t)LONG_MAX)
#endif

#else
/*
#error For this compiler, you need to set:
#error (1) NSBUFSIZ.  See DESIGN.TXT section 3.6.
#error (2) TIME_T_MIN & TIME_T_MAX.  See DESIGN.TXT section 3.5.
*/
#endif
#define NSBUFSIZ 34
extern char _r_nsbuf[NSBUFSIZ]; /* numeric to string conversion buffer */

/* enumerate errors */
enum {
    R_EDOM=1,                /* argument outside domain */
    R_EINVAL,                /* invalid argument value */
    R_EINVDAT,               /* invalid data */
    R_EINVMOD,               /* invalid read or write mode */
    R_EMISDAT,               /* missing data */
    R_ENOMEM,                /* could not allocate memory */
    R_ENOPUT,                /* failed put to stream */
    R_ERANGE,                /* data out of range */
    R_EFAULT                 /* unknown error */
};

/* enumerate warnings */
enum {
    R_WEMPSTR=1,             /* data string empty */
    R_WNOREG,                /* unable to register function (atexit full) */
    R_WWIDTH,                /* column width too small for data */
    R_WTMFMT,                /* date/time data incomplete */
    R_WFAULT                 /* unknown warning */
};

/* define static record streams */
extern REC *recin;           /* recin  record pointer */
extern REC *recout;          /* recout record pointer */
extern REC *recerr;          /* recerr record pointer */
extern REC *recprn;          /* recprn record pointer */

#define RECIN       -1       /* recin  context number */
#define RECOUT      -2       /* recout context number */
#define RECERR      -3       /* recerr context number */
#define RECPRN      -4       /* recprn context number */

/* record status flags definitions */
#define _R_COL       1       /* beginning column flag (bit 0) */
#define _R_WRT       2       /* read/write stream flag (bit 1) */
#define _R_TXT       4       /* current field text char indicator (bit 2) */
#define _R_EOF     128       /* end-of-file indicator (bit 7) */
#define _R_ERR     256       /* error indicator (bits 8-11) */
#define _R_WRN    4096       /* warning indicator (bits 12-15) */

#define _R_ERRMAX   15       /* maximum error number */
#define _R_WRNMAX   15       /* maximum warning number */

/* functions implemented as macros */
#define rskipfld(rp)     (rskipnfld((rp), 1))
#define rresetrec(rp)    (rsetrecstr((rp), rrecs(rp)))
#define rnames(rp)       ((rp)->r_fname)
#define rrecs(rp)        ((rp)->r_recp)
#define rflds(rp)        ((rp)->r_fldp)
#define rrecno(rp)       ((rp)->r_recno)
#define rfldno(rp)       ((rp)->r_fldno)
#define rcolno(rp)       ((rp)->r_colno + rbegcolno(rp))
#define rcxtno(rp)       ((rp)->r_cxtno)
#define rbegcolno(rp)    ((rp)->r_flags & _R_COL)
#define ristxtfld(rp)    ((rp)->r_flags & _R_TXT)
#define reof(rp)         ((rp)->r_flags & _R_EOF)

#define rgetfldpos(rp, pos)  (_rgetfldpos((rp), &(pos)))
#define rsetfldpos(rp, pos)  (_rsetfldpos((rp), &(pos)))

#define rgeti(rp)        (rbgeti( (rp), 10))
#define rgetl(rp)        (rbgetl( (rp), 10))
#define rgetui(rp)       (rbgetui((rp), 10))
#define rgetul(rp)       (rbgetul((rp), 10))

#define rcgeti(rp, begcol, endcol)  (rcbgeti( (rp), (begcol), (endcol), 10))
#define rcgetl(rp, begcol, endcol)  (rcbgetl( (rp), (begcol), (endcol), 10))
#define rcgetui(rp, begcol, endcol) (rcbgetui((rp), (begcol), (endcol), 10))
#define rcgetul(rp, begcol, endcol) (rcbgetul((rp), (begcol), (endcol), 10))

#define rputi(rp, num)   (rbputi( (rp), 10, (num)))
#define rputl(rp, num)   (rbputl( (rp), 10, (num)))
#define rputui(rp, num)  (rbputui((rp), 10, (num)))
#define rputul(rp, num)  (rbputul((rp), 10, (num)))

#define rcputi(rp, begcol, endcol, num)  (rcbputi( (rp), (begcol), (endcol), 10, (num)))
#define rcputl(rp, begcol, endcol, num)  (rcbputl( (rp), (begcol), (endcol), 10, (num)))
#define rcputui(rp, begcol, endcol, num) (rcbputui((rp), (begcol), (endcol), 10, (num)))
#define rcputul(rp, begcol, endcol, num) (rcbputul((rp), (begcol), (endcol), 10, (num)))

#define strimbegs(str)           (scntrimbegs((str), ' ', SIZE_T_MAX))
#define strimends(str)           (scntrimends((str), ' ', SIZE_T_MAX))
#define strims(str)              (scntrims((str), ' ', SIZE_T_MAX))
#define sctrimbegs(str, ch)      (scntrimbegs((str), (ch), SIZE_T_MAX))
#define sctrimends(str, ch)      (scntrimends((str), (ch), SIZE_T_MAX))
#define sctrims(str, ch)         (scntrims((str), (ch), SIZE_T_MAX))
#define scpys(dst, src)          (_scpys(&(dst), (src)))
#define scats(dst, src)          (_scats(&(dst), (src)))

#ifdef __cplusplus
extern "C" {
#endif

/* function prototypes */
/* source: recio.c */
extern void  rclose(REC *rp);
extern int   rcloseall(void);
extern int   risvalid(REC *rp);
extern REC  *ropen(const char *filename, const char *mode);
extern void  rsetbegcolno(REC *rp, int colno);
extern void  rsetcxtno(REC *rp, int cxtno);
extern void  rsetfldch(REC *rp, int ch);
extern void  rsettmfmt(REC *rp, const char *fmt);
extern void  rsettxtch(REC *rp, int ch);

/* source: rerr.c */
extern void  rclearerr(REC *rp);
extern int   rerror(REC *rp);
extern char *rerrstr(REC *rp);
extern int   rseterr(REC *rp, int errnum);
extern void  rseterrfn(void(*rerrfn)(REC *rp));
extern char *rstrerror(int errnum);
extern void  rinit(void(*rerrfn)(REC *rp), void(*rwarnfn)(REC *rp));

/* source: remsg.c */
extern void rerrmsg(REC *rp);

/* source: rwarn.c */
extern void  rclearwarn(REC *rp);
extern int   rwarning(REC *rp);
extern char *rwarnstr(REC *rp);
extern int   rsetwarn(REC *rp, int warnum);
extern void  rsetwarnfn(void(*rwarnfn)(REC *rp));
extern char *rstrwarning(int warnum);

/* source: rwmsg.c */
extern void rwarnmsg(REC *rp);

/* source: rget.c */
extern char    *rgetrec(REC *rp);
extern void     _rgetfldpos(REC *rp, rpos_t *pos);
extern void     _rsetfldpos(REC *rp, const rpos_t *pos);
extern void     rsetfldsiz(REC *rp, size_t fldsize);
extern void     rsetfldstr(REC *rp, const char *s);
extern void     rsetrecsiz(REC *rp, size_t recsize);
extern void     rsetrecstr(REC *rp, const char *s);
extern unsigned rskipnfld(REC *rp, unsigned num);
extern unsigned rnumfld(REC *rp);

/* source: rgetf.c */
extern float  rgetf(REC *rp);
extern double rgetd(REC *rp);

/* source: rgets.c */
extern int    rgetc(REC *rp);
extern char  *rgets(REC *rp);

/* source: rgett.c */
extern time_t    rgett(REC *rp);
extern struct tm rgettm(REC *rp);
extern time_t    sftotime(const char *s, const char *fmt);
extern struct tm sftotm(const char *s, const char *fmt);
extern time_t    tmtotime(struct tm);
extern struct tm timetotm(time_t);
extern void      rsetbegyr(int year);

/* source: rbget.c */
extern int           rbgeti( REC *rp, int base);
extern unsigned int  rbgetui(REC *rp, int base);
extern long          rbgetl( REC *rp, int base);
extern unsigned long rbgetul(REC *rp, int base);

/* source: rcgetf.c */
extern float  rcgetf(REC *rp, size_t begcol, size_t endcol);
extern double rcgetd(REC *rp, size_t begcol, size_t endcol);

/* source: rcgets.c */
extern int    rcgetc(REC *rp, size_t col);
extern char  *rcgets(REC *rp, size_t begcol, size_t endcol);

/* source: rcgett.c */
extern time_t    rcgett(REC *rp, size_t begcol, size_t endcol);
extern struct tm rcgettm(REC *rp, size_t begcol, size_t endcol);

/* source: rcbget.c */
extern int           rcbgeti( REC *rp, size_t begcol, size_t endcol, int base);
extern unsigned int  rcbgetui(REC *rp, size_t begcol, size_t endcol, int base);
extern long          rcbgetl( REC *rp, size_t begcol, size_t endcol, int base);
extern unsigned long rcbgetul(REC *rp, size_t begcol, size_t endcol, int base);

/* source: rput.c */
extern void rputrec(REC *rp);

/* source: rputf.c */
extern void rputf(REC *rp, float f);
extern void rputd(REC *rp, double d);

/* source: rputs.c */
extern void rputc(REC *rp, int ch);
extern void rputs(REC *rp, const char *str);

/* source: rputt.c */
extern void rputt(REC *rp, time_t time);
extern void rputtm(REC *rp, struct tm t);

/* source: rbput.c */
extern void rbputi(REC *rp, int base, int i);
extern void rbputui(REC *rp, int base, unsigned int ui);
extern void rbputl(REC *rp, int base, long l);
extern void rbputul(REC *rp, int base, unsigned long ul);

/* source: rcputs.c */
extern void rcputc(REC *rp, size_t col, int ch);
extern void rcputs(REC *rp, size_t begcol, size_t endcol, const char *str);

/* source: rcputf.c */
extern void rcputf(REC *rp, size_t begcol, size_t endcol, float f);
extern void rcputd(REC *rp, size_t begcol, size_t endcol, double d);

/* source: rcputt.c */
extern void rcputt(REC *rp, size_t begcol, size_t endcol, time_t time);
extern void rcputtm(REC *rp, size_t begcol, size_t endcol, struct tm t);

/* source: rcbput.c */
extern void rcbputi(REC *rp, size_t begcol, size_t endcol, int base, int i);
extern void rcbputui(REC *rp, size_t begcol, size_t endcol, int base, unsigned int ui);
extern void rcbputl(REC *rp, size_t begcol, size_t endcol, int base, long l);
extern void rcbputul(REC *rp, size_t begcol, size_t endcol, int base, unsigned long ul);

/* source: rfix.c */
extern void rbfixi( REC *rp, int base);
extern void rbfixui(REC *rp, int base);
extern void rbfixl( REC *rp, int base);
extern void rbfixul(REC *rp, int base);
extern void rfixc(REC *rp);
extern void rfixf(REC *rp);
extern void rfixd(REC *rp);
extern void rfixi(REC *rp);
extern void rfixl(REC *rp);
extern void rfixt(REC *rp);
extern void rfixui(REC *rp);
extern void rfixul(REC *rp);

/* source: rstr.c */
extern char *scntrimbegs(char *str, int ch, size_t num);
extern char *scntrimends(char *str, int ch, size_t num);
extern char *scntrims(char *str, int ch, size_t num);
extern char *_scpys(char **dst, const char *src);
extern char *_scats(char **dst, const char *src);

#ifdef  __cplusplus
}
#endif

#endif
