/*
 *  xv.h  -  header file for xv, but you probably guessed as much
 * 
 *  Author:    John Bradley, University of Pennsylvania
 *                (bradley@cis.upenn.edu)
 */

/*
 * Copyright 1989, 1990, 1991, 1992 by John Bradley and 
 *                       The University of Pennsylvania
 *
 * Permission to use, copy, and distribute for non-commercial purposes,
 * is hereby granted without fee, providing that the above copyright
 * notice appear in all copies and that both the copyright notice and this
 * permission notice appear in supporting documentation. 
 *
 * The software may be modified for your own purposes, but modified versions
 * may not be distributed.
 *
 * This software is provided "as is" without any expressed or implied warranty.
 *
 * The author may be contacted via:
 *    US Mail:   John Bradley
 *               GRASP Lab, Room 301C
 *               3401 Walnut St.  
 *               Philadelphia, PA  19104
 *
 *    Phone:     (215) 898-8813
 *    EMail:     bradley@cis.upenn.edu       
 */


#define REVDATE   "Version 2.21  Rev: 4/29/92"


/*************************************************/
/* START OF MACHINE-DEPENDENT CONFIGURATION INFO */
/*************************************************/

#ifdef __UMAXV__              /* for Encore Computers UMAXV */
#include <sys/fs/b4param.h>   /* Get bsd fast file system params*/
#endif

#ifndef VMS   /* VMS doesn't like multi-line '#if's */
  /* AIX and SysV r4 (but not sgi or UMAXV) use dirent */
# if   (defined(SVR4) && !defined(sgi) && !defined(__UMAXV__)) \
       || defined(_IBMR2) || defined(sco)
#  ifndef DIRENT
#   define DIRENT
#  endif
# endif
#endif /* !VMS */


/* include files */
#include <stdio.h>
#include <math.h>
#include <ctype.h>



#ifndef VMS
extern int   errno;             /* this SHOULD be in errno.h */
/*  COMMENTED OUT FOLLOWING FOR LINUX */
/* extern char *sys_errlist[]; */     /* this SHOULD be in errno.h */
#endif

#ifndef VMS     /* VMS hates multi-line '#if's */
  /* lots of machines don't have <memory.h> */
# if !defined(__convex__) && \
     !defined(__UMAXV__)  && \
     !defined(pyr)        && \
     !defined(sequent)
# include <memory.h>             /* for 'memset()' prototype */
# endif
#endif /* !VMS */

#ifdef VMS   /* VMS config, hacks & kludges */
#define DIRENT 1
#define MAXPATHLEN    512
#define popUp xv_popup
#define qsort xv_qsort
#define bzero(s,size) memset(s,0,size)
#define bcmp(s1,s2,size) xv_bcmp((char *)s1,(char *)s2,size)
#define random rand
#define cols xv_cols
typedef unsigned long u_long;
#include <errno.h>              /* in VMS they *are* in errno.h */
#include <perror.h>             /* and perror.h */
#endif


#ifndef VMS   /* VMS still hates multi-line '#if's */
/* lots of things don't have <malloc.h> */
/* A/UX systems include it from stdlib, from Xos.h */
# if !defined(ibm032)                    && \
     !defined(__convex__)                && \
     !(defined(vax) && !defined(ultrix)) && \
     !defined(mips)                      && \
     !defined(apollo)                    && \
     !defined(pyr)                       && \
     !defined(__UMAXV__)                 && \
     !defined(bsd43)                     && \
     !defined(macII)                     && \
     !defined(sequent)

#  if defined(hp300) || defined(hp800)
#   include <sys/malloc.h>                /* it's in 'sys' on HPs*/
#  else
#   include <malloc.h>
#  endif
# endif
#endif /* !VMS */

#if defined(NEED_MEMROUTINES)
#define memcpy(d,s,l) bcopy(s,d,l)
#endif


#include <X11/Xos.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>
#include <X11/keysym.h>
#include <X11/Intrinsic.h>
#include <X11/Xatom.h>
#include <X11/Xmd.h>



#if defined(NEEDSTIME) || defined(NEEDSDIR)
#include <sys/types.h>    /* only include <sys/types.h> once */
#endif

#ifdef NEEDSTIME
#if defined(SVR4) || defined(macII)
#include <sys/time.h>
#else                    /* not SVR4 */
#if !defined(sgi) && !defined(__UMAXV__)   /* sgi & UMAX don't have timeb.h */
#include <sys/timeb.h>
#endif                   /* not sgi */
#endif                   /* not SVR4 */

#undef SIGCHLD           /* defined in both the Xos.h and signal.h */
#include <signal.h>
#if defined(sco) && !defined(NOTIMER)
#include <sys/itimer.h>
#endif

#ifndef  sigmask
#define  sigmask(m)      (1 << ((m)-1))
#endif

#endif                   /* NEEDSTIME */


#ifdef NEEDSDIR
#  ifdef VMS
#    include <descrip.h>
#    include <stat.h>
#    include "dirent.h"
#  else
#    ifdef sco
#      include <sys/ndir.h>   /* sco has sys/ndir.h */
#    else
#      ifndef ATT
#        include <sys/dir.h>    /* everybody else (except ATT) has sys/dir.h */
#      endif  /* ATT */
#    endif  /* sco */

#    include <sys/param.h>
#    include <sys/stat.h>

#    ifdef DIRENT
#      include <dirent.h>
#    endif
#  endif /* !VMS */
#endif  /* NEEDSDIR */

#ifdef NEEDSARGS
#if defined(__STDC__) && !defined(NOSTDHDRS)
#include <stdarg.h>
#else
#include <varargs.h>
#endif
#endif

/* signal macros */
#if defined(SVR4) && !defined(HPUX7) && !(defined(_AUX_SOURCE))
#define HOLD_SIG         sighold(SIGALRM)  /* block ALRM sig from occurring */
#define RELEASE_SIG      sigrelse(SIGALRM)
#define PAUSE_SIG        sigpause(SIGALRM) /* sleep until ALRM signal */
#else
#define HOLD_SIG         sigblock(sigmask(SIGALRM))
#define RELEASE_SIG      sigblock(0)
#define PAUSE_SIG        sigpause(0)
#endif


#ifdef __UMAXV__
#undef  PAUSE_SIG
#define PAUSE_SIG sigpause(SIGALRM)
#endif


/* default for most folks */
#define UNCOMPRESS "/usr/ucb/uncompress"   /* for uncompressing .Z files */

#if defined(hpux) || defined (SVR4)
#undef  UNCOMPRESS
#define UNCOMPRESS "/usr/bin/uncompress"   /* for uncompressing .Z files */
#endif

#ifdef VMS
   /* you WILL have to personalize for your own DECUS VMS version of
      the Martin Minow LZDCMP for this to work properly... */
#  undef UNCOMPRESS
#  ifdef HAVE_LZW
#    define UNCOMPRESS "LZDCMP /Export = Unix /Mode = Binary"
#  else
#    define UNCOMPRESS "DECOMPRESS"
#  endif /* HAVE_LZW */
#endif /* VMS */


#if defined(i386) && !defined(sequent) && !defined(sun386) && !defined(_AIX)
#define MAXPATHLEN    500
#undef  UNCOMPRESS
#define UNCOMPRESS    "/usr/local/bin/uncompress"   /* uncompress program */
#undef  HOLD_SIG
#define HOLD_SIG      /* don't know how to handle signals  MWS 10/18/90 */
#undef  RELEASE_SIG
#define RELEASE_SIG   /* */
#undef  PAUSE_SIG
#define PAUSE_SIG     /* */
#endif


#ifndef MAXPATHLEN
#define MAXPATHLEN 1024
#endif

#ifdef NO_RANDOM
#define random rand
#endif

/*****************************/
/* END OF CONFIGURATION INFO */
/*****************************/




#define PROGNAME  "xv"             /* used in resource database */

#define DEFINFOGEOM "-10+10"       /* default position of info window */
#define DEFCTRLGEOM "+400+400"     /* default position of ctrl window */
#define DEFGAMGEOM  "+10-10"       /* default position of gamma window */

#define INFOWIDE 500               /* (fixed) size of info window */
#define INFOHIGH 250

#define CTRLWIDE 440               /* (fixed) size of control window */
#define CTRLHIGH 345
#define CTRL_LISTW   (330-70)

#define DIRWIDE  300               /* (fixed) size of directory window */
#define DIRHIGH  420

#define MAXNAMES 4096              /* max # of files in ctrlW list */

/* strings in the INFOBOX (used in SetISTR and GetISTR) */
#define NISTR         9    /* number of ISTRs */
#define ISTR_INFO     0
#define ISTR_WARNING  1
#define ISTR_FILENAME 2
#define ISTR_FORMAT   3
#define ISTR_RES      4
#define ISTR_CROP     5
#define ISTR_EXPAND   6
#define ISTR_COLOR    7
#define ISTR_COLOR2   8

/* potential values of 'infomode', used in info box drawing routines */
#define INF_NONE 0    /* empty box */
#define INF_STR  1    /* just ISTR_INFO */
#define INF_PART 2    /* filename, format, size and infostr */
#define INF_FULL 3    /* INF_PART + clipping, expansion, colorinfo */


/* buttons in the ctrl window */
#define NBUTTS   28
#define BNEXT    0
#define BPREV    1
#define BCROP    2
#define BUNCROP  3
#define BNORM    4
#define BMAX     5
#define BUP2     6
#define BDN2     7
#define BUP10    8
#define BDN10    9
#define BQUIT    10
#define B4BY3    11
#define BSAVE    12
#define BROTL    13
#define BINFO    14
#define BGAMMA   15
#define BASPECT  16
#define BROTR    17
#define BMAXPECT 18
#define BACROP   19
#define BSMOOTH  20
#define BDITH    21
#define BRAW     22
#define BLOAD    23
#define BDELETE  24
#define BFLIPH   25
#define BFLIPV   26
#define BGRAB    27


/* buttons in the 'save' window */
#define S_NBUTTS  3
#define S_BOK     0
#define S_BCANC   1
#define S_BRESCAN 2


/* buttons in the 'gamma' window */
#define G_NBUTTS   24
#define G_BAPPLY   0
#define G_BNOGAM   1
#define G_BRESET   2
#define G_BCLOSE   3
#define G_BUP_BR   4
#define G_BDN_BR   5
#define G_BUP_CN   6
#define G_BDN_CN   7
#define G_B1       8
#define G_B2       9
#define G_B3       10
#define G_B4       11
#define G_BSET     12
#define G_BUNDO    13
#define G_BREDO    14
#define G_BCOLREV  15
#define G_BRNDCOL  16
#define G_BHSVRGB  17
#define G_BCOLUNDO 18
#define G_BRV      19
#define G_BMONO    20
#define G_BMAXCONT 21
#define G_BGETRES  22
#define G_BHISTEQ  23


/* constants for setting radio buttons in dirW */
#define F_COLORS    0
#define F_FORMAT    1

#define F_FULLCOLOR 0
#define F_GREYSCALE 1
#define F_BWDITHER  2
#define F_REDUCED   3

#define F_GIF       0
#define F_PM        1
#define F_PBMRAW    2
#define F_PBMASCII  3
#define F_XBM       4
#define F_SUNRAS    5
#define F_PS        6

#ifdef HAVE_JPEG
#define F_JPEG      7
#endif

#ifdef HAVE_TIFF
#ifdef HAVE_JPEG
#define F_TIFF      8
#else
#define F_TIFF      7
#endif
#endif


/* definitions for page up/down, arrow up/down list control */
#define LS_PAGEUP   0
#define LS_PAGEDOWN 1
#define LS_LINEUP   2
#define LS_LINEDOWN 3
#define LS_HOME     4
#define LS_END      5

/* values 'epicmode' can take */
#define EM_RAW    0
#define EM_DITH   1
#define EM_SMOOTH 2

/* things EventLoop() can return (0 and above reserved for 'goto pic#') */
#define QUIT     -1    /* exit immediately  */
#define NEXTPIC  -2    /* goto next picture */
#define PREVPIC  -3    /* goto prev picture */
#define NEXTQUIT -4    /* goto next picture, quit if none (used by 'wait') */
#define LOADPIC  -5    /* load 'named' pic (from directory box) */
#define NEXTLOOP -6    /* load next pic, loop if we're at end */
#define DFLTPIC  -7    /* load the default image */
#define DELETE   -8    /* just deleted pic.  load 'right' thing */
#define GRABBED  -9    /* just grabbed a pic.  'load' it up */


/* possible values of 'rootMode' */
#define RM_NORMAL  0     /* default X tiling */
#define RM_TILE    1     /* integer tiling */
#define RM_MIRROR  2     /* mirror tiling */
#define RM_IMIRROR 3     /* integer mirror tiling */
#define RM_CENTER  4     /* modes >= RM_CENTER centered on some sort of bg */
#define RM_CENTILE 4     /* centered and tiled.  NOTE: equals RM_CENTER */
#define RM_CSOLID  5     /* centered on a solid bg */
#define RM_CWARP   6     /* centered on a 'warp-effect' bg */
#define RM_CBRICK  7     /* centered on a 'brick' bg */
#define RM_MAX     RM_CBRICK

/* types of 24-to-8 conversion */
#define CONV24_FAST 0
#define CONV24_SLOW 1
#define CONV24_BEST 2

/* definitions of first char of dirnames[i] (filetype) */
#define C_FIFO  'f'    /* FIFO special file */
#define C_CHR   'c'    /* character special file */
#define C_DIR   'd'    /* directory */
#define C_BLK   'b'    /* block special file */
#define C_LNK   'l'    /* symbolic link */
#define C_SOCK  's'    /* socket */
#define C_REG   ' '    /* regular file */


/* random string-placing definitions */
#define SPACING 3      /* vertical space between strings */
#define ASCENT   (mfinfo->ascent)
#define DESCENT  (mfinfo->descent)
#define CHIGH    (ASCENT + DESCENT)
#define LINEHIGH (CHIGH + SPACING)


#define STDINSTR "<stdin>"


#ifndef MAIN
#define WHERE extern
#else
#define WHERE
#endif

typedef unsigned char byte;

typedef struct { Window win;            /* window ID */
		 int len;               /* length of major axis */
		 int vert;              /* true if vertical, else horizontal */
		 int active;            /* true if scroll bar can do anything*/
		 int min,max;           /* min/max values 'pos' can take */
		 int val;               /* 'value' of scrollbar */
		 int page;              /* amt val change on pageup/pagedown */
		 int tpos;              /* thumb pos. (pixels from tmin) */
		 int tmin,tmax;         /* min/max thumb offsets (from 0,0) */
		 int tsize;             /* size of thumb (in pixels) */
		 unsigned long fg,bg;   /* colors */
		 void (*drawobj)();     /* redraws obj controlled by scrl*/
		 int uplit, dnlit;      /* true if up&down arrows are lit */
	       } SCRL;

typedef struct { Window win;            /* window ID */
		 int w,h;               /* size of window */
		 int active;            /* true if can do anything*/
		 int min,max;           /* min/max values 'pos' can take */
		 int val;               /* 'value' of dial */
		 int page;              /* amt val change on pageup/pagedown */
		 char *title;           /* title for this guage */
		 char *units;           /* string appended to value */
		 unsigned long fg,bg;   /* colors */
		 int rad, cx, cy;       /* internals */
		 int bx[4], by[4];      /* more internals */
		 void (*drawobj)();     /* redraws obj controlled by dial */
	       } DIAL;

typedef struct { Window win;            /* parent window */
		 int x,y,w,h;           /* size of button rectangle */
		 int lit;               /* if true, invert colors */
		 int active;            /* if false, stipple gray */
		 int toggle;            /* if true, clicking toggles state */
		 unsigned long fg,bg;   /* colors */
		 char *str;             /* string in button */
		 Pixmap pix;            /* use pixmap instead of string */
		 int pw,ph;             /* size of pixmap */
		 int style;             /* ... */
		 int fwidth;            /* width of frame */
	       } BUTT;


typedef struct rbutt { Window        win;      /* parent window */
		       int           x,y;      /* position in parent */
		       char         *str;      /* the message string */
		       int           selected; /* selected or not */
		       int           active;   /* selectable? */
		       struct rbutt *next;     /* pointer to next in group */
		       unsigned long fg,bg;    /* colors */
		     } RBUTT;


typedef struct cbutt { Window        win;      /* parent window */
		       int           x,y;      /* position in parent */
		       char         *str;      /* the message string */
		       int           val;      /* 1=selected, 0=not */
		       int           active;   /* selectable? */
		       unsigned long fg,bg;    /* colors */
		     } CBUTT;


typedef struct mbutt { Window        win;      /* parent window */
		       int           x,y,w,h;  /* position in parent */
		       char         *title;    /* title string in norm state */
		       int           selected; /* item# selected, or -1 */
		       int           active;   /* selectable? */
		       char        **list;     /* list of strings in menu */
		       int           nlist;    /* # of strings in menu */
		       Pixmap pix;             /* use pixmap instd of string */
		       int pw,ph;              /* size of pixmap */
		       unsigned long fg,bg;    /* colors */
		       Window        mwin;     /* popup menu window */
		     } MBUTT;


typedef struct { Window win;            /* window */
		 int x,y,w,h;           /* size of window */
		 unsigned long fg,bg;   /* colors */
		 char **str;            /* ptr to list of strings */
		 int   nstr;            /* number of strings */
		 int   selected;        /* number of 'selected' string */
		 int   nlines;          /* number of lines shown at once */
		 SCRL  scrl;            /* scrollbar that controls list */
		 int   filetypes;       /* true if filetype icons to be drawn*/
		 int   dirsonly;        /* if true, only dirs selectable */
	       } LIST;


#define MAX_GHANDS 16   /* maximum # of GRAF handles */

#define N_GFB 6
#define GFB_SPLINE 0
#define GFB_LINE   1
#define GFB_ADDH   2
#define GFB_DELH   3
#define GFB_RESET  4
#define GFB_GAMMA  5

#define GVMAX 8

typedef struct {  Window win;          /* window ID */
		  Window gwin;         /* graph subwindow */
		  int    spline;       /* spline curve or lines? */
		  int    entergamma;   /* currently entering gamma value */
		  int    gammamode;    /* currently using gamma function */
		  double gamma;        /* gamma value (if gammamode) */
		  int    nhands;       /* current # of handles */
		  XPoint hands[MAX_GHANDS];   /* positions of handles */
		  byte   func[256];    /* output function of GRAF */
		  BUTT   butts[N_GFB]; /* control buttons */
		  u_long fg,bg;        /* colors */
		  char   *str;         /* title string */
		  char   gvstr[GVMAX+1];    /* gamma value input string */
		} GRAF;

typedef struct {  int    spline;
		  int    entergamma;
		  int    gammamode;
		  double gamma;
		  int    nhands;
		  XPoint hands[MAX_GHANDS];
		  char   gvstr[GVMAX+1];
		} GRAF_STATE;


/* MACROS */
#define CENTERX(f,x,str) ((x)-XTextWidth(f,str,strlen(str))/2)
#define CENTERY(f,y) ((y)-((f->ascent+f->descent)/2)+f->ascent)

/* RANGE forces a to be in the range b..c (inclusive) */
#define RANGE(a,b,c) { if (a < b) a = b;  if (a > c) a = c; }

/* PTINRECT returns '1' if x,y is in rect (inclusive) */
#define PTINRECT(x,y,rx,ry,rw,rh) \
           ((x)>=(rx) && (y)>=(ry) && (x)<=(rx)+(rw) && (y)<=(ry)+(rh))

/* MONO returns total intensity of r,g,b components */
#define MONO(rd,gn,bl) (((rd)*11 + (gn)*16 + (bl)*5) >> 5)  /*.33R+ .5G+ .17B*/

/* ISPIPE returns true if the passed in character is considered the
   start of a 'load-from-pipe' or 'save-to-pipe' string */
#define ISPIPE(c) ((c)=='!' || (c)=='|')

/* X stuff */
WHERE Display       *theDisp;
WHERE int           theScreen;
WHERE unsigned int  ncells, dispWIDE, dispHIGH, dispDEEP;
WHERE unsigned int  vrWIDE, vrHIGH, maxWIDE, maxHIGH;
WHERE Colormap      theCmap, LocalCmap;
WHERE Window        rootW, mainW, vrootW;
WHERE GC            theGC;
WHERE unsigned long black, white, fg, bg, infofg, infobg;
WHERE unsigned long hicol, locol;
WHERE unsigned long blkRGB, whtRGB;
WHERE Font          mfont, monofont;
WHERE XFontStruct   *mfinfo, *monofinfo;
WHERE Visual        *theVisual;
WHERE Cursor        arrow, cross, tcross, zoom, inviso;
WHERE Pixmap        iconPix, iconmask;
WHERE int           cmapinstalled, showzoomcursor;

/* global vars used by LOAD routines */
WHERE byte          *pic;                   /* ptr to loaded picture */
WHERE int            pWIDE,pHIGH;           /* size of 'pic' */
WHERE byte           r[256],g[256],b[256];  /* colormap */
WHERE char          *cmd;                   /* program name for printf's */

//WHERE int            DEBUG;                 /* print debugging info */

WHERE int            mono;                  /* true if displaying grayscale */
WHERE char           formatStr[80];         /* short-form 'file format' */


/* more global variables, used by xv and xvmisc */
WHERE byte          *cpic;         /* cropped version of pic */
WHERE int           cWIDE, cHIGH,  /* size of cropped region */
                    cXOFF, cYOFF;  /* offset of region from 0,0 of pic */

WHERE byte          *epic;         /* expanded version of cpic */
                                   /* points to pic when at 1:1 expansion */
                                   /* this is converted to 'theImage' */
WHERE int           eWIDE, eHIGH;  /* size of epic */
WHERE unsigned int  normFact;      /* factor to shrink picture by for 'norm' */

WHERE int           p_offx, p_offy;  /* offset of reparented windows */
WHERE int           ch_offx,ch_offy; /* ChngAttr ofst for reparented windows */

WHERE byte           rorg[256], gorg[256], borg[256];  /* ORIGINAL colormap */
WHERE byte           rcmap[256], gcmap[256], bcmap[256]; /*post-cmap-editing*/
WHERE byte           rdisp[256],gdisp[256],bdisp[256];  /* DISPLAYED colors */
WHERE byte           colAllocOrder[256];   /* order to allocate cols */
WHERE unsigned long  freecols[256]; /* list of pixel values to free */
WHERE byte           rwpc2pc[256]; /* mapping of shared pixels in -rw mode */
WHERE int            nfcols;       /* number of colors to free */
WHERE unsigned long  cols[256];    /* maps pic pixel values to X pixel vals */
WHERE int            fc2pcol[256]; /* maps freecols into pic pixel values */
WHERE int            numcols;      /* # of desired colors in picture */

WHERE byte           fsgamcr[256]; /* gamma correction curve (for FS dither) */


WHERE XImage        *theImage;     /* X version of epic */


WHERE int           ncols;         /* max # of (different) colors to alloc */

WHERE char          str[128],      /* dummy string used for error messages */
                    basefname[128]; /* 'nice' part of current filename */

WHERE int           bwidth,        /* border width of created windows */
                    noglob,        /* force to only use colors it alloced */
                    perfect,       /* perfect color.  install own colormap */
                    fixedaspect,   /* fixed aspect ratio */
                    conv24,        /* 24to8 algorithm to use (CONV24_*) */
                    ninstall,      /* true if using icccm-complaint WM
				      (a WM that will does install CMaps */
                    useroot,       /* true if we should draw in rootW */
		    nolimits,	   /* No limits on picture size */
		    resetroot,     /* true if we should clear in window mode */
                    noqcheck,      /* true if we should NOT do QuickCheck */
                    rwcolor,       /* true if we should use R/W color cells */
                    rwthistime,    /* true if we DID use R/W color cells */
                    owncmap,       /* true if we should always create cmap */
                    epicmode,      /* either SMOOTH, DITH, or RAW */
                    autoclose,     /* if true, autoclose when iconifying */
                    xerrcode;      /* errorcode of last X error */

WHERE float         defaspect,     /* default aspect ratio to use */
                    normaspect;    /* normal aspect ratio of this picture */

WHERE int           crx1, cry1,    /* dimensions of cropping rectangle */
                    crx2, cry2;

WHERE unsigned long rootbg, rootfg;   /* fg/bg for root border */
WHERE int           waitsec;          /* secs btwn pics. -1=wait for event */
WHERE int           waitloop;         /* loop at end of slide show? */
WHERE int           automax;          /* maximize pic on open */
WHERE int           rootMode;         /* mode used for -root images */

WHERE int           nostat;           /* if true, don't stat() in LdCurDir */

WHERE int           ctrlColor;        /* whether or not to use colored butts */

WHERE char         *def_str;          /* used by rd_*() routines */
WHERE int           def_int;
WHERE char         *tmpdir;           /* equal to "/tmp" or $TMPDIR env var */


/* stuff used for 'info' box */
WHERE Window        infoW;
WHERE int           infoUp;       /* boolean:  whether infobox is visible */
WHERE int           infoMode;


/* stuff used for 'ctrl' box */
WHERE Window        ctrlW;
WHERE int           ctrlUp;       /* boolean:  whether ctrlbox is visible */
WHERE char         *namelist[MAXNAMES];  /* list of file names from argv */
WHERE char         *dispnames[MAXNAMES]; /* truncated names shown in listbox */
WHERE int           numnames, curname;
WHERE LIST          nList;
WHERE BUTT          but[NBUTTS];         /* command buttons in ctrl window */
WHERE Pixmap        grayTile, grayStip;  /* for drawing dim things */
WHERE MBUTT         dispMB;              /* display mode menu button */
WHERE MBUTT         conv24MB;            /* 24-to-8 conversion mode mbutt */


/* stuff used for 'directory' box */
WHERE Window        dirW, dnamW;
WHERE int           dirUp;       /* is dirW mapped or not */
WHERE LIST          dList;       /* list of filenames in current directory */
WHERE BUTT          dbut[S_NBUTTS];
WHERE CBUTT         browseCB;
WHERE CBUTT         savenormCB;


/* stuff used for 'gamma' box */
WHERE Window        gamW;
WHERE int           gamUp;       /* is gamW mapped or not */
WHERE BUTT          gbut[G_NBUTTS];
WHERE int           editColor;   /* currently selected color # */
WHERE int           hsvmode;     /* true if in HSVmode */
WHERE int cellgroup[256], curgroup, maxgroup;  /* grouped colorcell stuff */
WHERE int           cmapInGam;

/* stuff used for 'ps' box */
WHERE Window        psW;
WHERE int           psUp;       /* is psW mapped, or what? */
WHERE CBUTT         encapsCB, pscompCB;   


#ifdef HAVE_JPEG
/* stuff used for 'jpeg' box */
WHERE Window        jpegW;
WHERE int           jpegUp;       /* is jpegW mapped, or what? */
#endif


#ifdef HAVE_TIFF
/* stuff used for 'tiff' box */
WHERE Window        tiffW;
WHERE int           tiffUp;       /* is tiffW mapped, or what? */
#endif


#undef WHERE





/* function declarations for externally-callable functions */

#ifdef __STDC__ 
/****************************** XV.C ****************************/
void FixAspect(int, int *, int *);
int  DeleteCmd(void);
void HandleDispMode(void);
char *lower_str(char *);
int  rd_int(char *);
int  rd_str(char *);
int  rd_flag(char *);
int  rd_str_cl(char *, char *);

/****************************** XVEVENT.C ****************************/
int  EventLoop(void);
int  HandleEvent(XEvent *, int *);
void DrawWindow(int,int,int,int);
void WResize(int, int);
void WRotate(void);
void WCrop(int, int);
void WUnCrop(void);
void GetWindowPos(XWindowAttributes *);
void SetWindowPos(XWindowAttributes *);
void InvCropRect(void);
void SetEpicMode(void);
int  xvErrorHandler(Display *, XErrorEvent *);

/****************************** XVROOT.C ****************************/
void MakeRootPic(void);
void ClearRoot(void);
void SaverootInfo(void);
void KillOldRootInfo(void);

/*************************** XVMISC.C ***************************/
void StoreDeleteWindowProp(Window);
Window CreateWindow(char *, char *, unsigned int, unsigned int, 
		    unsigned long, unsigned long);
void CenterString(Window, char *, int, int);
void ULineString(Window, char *, int, int);
int  StringWidth(char *);
void FakeButtonPress(BUTT *);
void GenExpose(Window, int, int, int, int);
void DimRect(Window, int, int, int, int, u_long);
void xvDestroyImage(XImage *);
void SetCropString(int);
void Warning(void);
void FatalError(char *);
void Quit(int);
void LoadFishCursors(void);
void WaitCursor(void);
void SetCursors(int);
void Timer(int);

/*************************** XVCOLOR.C ***************************/
void   SortColormap(void);
void   AllocColors(void);
void   AllocRWColors(void);
Status xvAllocColor(Display *, Colormap, XColor *);
void   xvFreeColors(Display *, Colormap, unsigned long *, int, unsigned long);
void   FreeAllColors(void);
void   ApplyEditColor(int);

/*************************** XVIMAGE.C ***************************/
void Resize(int, int);
void DoZoom(int, int, int);
void DoCrop(void);
void UnCrop(void);
void AutoCrop(void);
void Rotate(int);
void Flip(int);
void FlipPic(byte *, int, int, int);
void FSDither(byte *, int, int, byte *);
void CreateXImage(void);

/*************************** XVSMOOTH.C ***************************/
void  Smooth(void);
byte *Smooth24(void);
void ColorDither(byte *, int, int);

/*************************** XV24TO8.C **************************/
void Init24to8(void);
int  Conv24to8(byte *, int, int, int);

/**************************** XVCTRL.C **************************/
void CreateCtrl(char *);
void CtrlBox(int);
void RedrawCtrl(int, int, int, int);
int  ClickCtrl(int, int);
void DrawCtrlStr(void);
void ScrollToCurrent(LIST *);

void LSCreate(LIST *, Window, int, int, int, int, int, char **, int, 
	      unsigned long, unsigned long, void (*)(void), int, int);
void LSRedraw(LIST *);
int  LSClick (LIST *, XButtonEvent *);
void LSNewData(LIST *, char **, int);
void LSKey(LIST *, int);


/*************************** XVINFO.C ***************************/
void  CreateInfo(char *);
void  InfoBox(int);
void  RedrawInfo(int, int, int, int);
void  SetInfoMode(int);
#if defined(__STDC__) && !defined(NOSTDHDRS)
void  SetISTR(int, ...);
#else
void  SetISTR();
#endif
char *GetISTR(int);

/**************************** XVDIR.C ***************************/
void CreateDirW(char *);
void DirBox(int);
void RedrawDirW(int,int,int,int);
int  ClickDirW(int, int);
void LoadCurrentDirectory(void);
void RedrawDDirW(void);
void RedrawDNamW(void);
void SelectDir(int);
void TrackDDirW(int,int);
int  DirKey(int);
int  DoSave(void);
void SetDirFName(char *);
char *GetDirFName(void);
void SetDirRButt(int, int);
int  Globify(char *);
FILE *OpenOutFile(char *);
int  CloseOutFile(FILE *, char *, int);
byte *HandleBWandReduced(int, int *, byte **, byte **, byte **);

/**************************** XVGAM.C **************************/
void CreateGam(char *);
int  GamCheckEvent(XEvent *);
void GamBox(int);
void NewCMap();
void RedrawCMap();
void ChangeEC(int);
void ApplyECctrls(void);
void GenerateFSGamma(void);
void GammifyColors(void);
void Gammify1(int);
void rgb2hsv(int, int, int, double *, double *, double *);
void hsv2rgb(double, double, double, int *, int *, int *);

/*************************** XVSCRL.C ***************************/
void SCCreate  (SCRL *, Window, int, int, int, int, int, int, int, int, 
                      unsigned long, unsigned long, void (*)(void));
void SCSetRange(SCRL *, int, int, int, int);
int  SCSetVal  (SCRL *, int);
void SCRedraw  (SCRL *);
void SCTrack   (SCRL *, int, int);


/*************************** XVDIAL.C ***************************/
void DCreate  (DIAL *, Window, int, int, int, int, int, int, int, int, 
                      unsigned long, unsigned long, char *, char *);
void DSetRange(DIAL *, int, int, int, int);
void DSetVal  (DIAL *, int);
void DSetActive(DIAL *, int);
void DRedraw  (DIAL *);
int  DTrack   (DIAL *, int, int);


/**************************** XVBUTT.C ***************************/

void BTCreate(BUTT *, Window, int, int, int, int, char *, 
	      unsigned long, unsigned long);
void BTSetActive(BUTT *, int);
void BTRedraw(BUTT *);
int  BTTrack (BUTT *);


RBUTT *RBCreate(RBUTT *, Window, int, int, char*, 
		unsigned long, unsigned long);
void   RBRedraw(RBUTT *, int);
void   RBSelect(RBUTT *, int);
int    RBWhich(RBUTT *);
int    RBCount(RBUTT *);
void   RBSetActive(RBUTT *, int, int);
int    RBClick(RBUTT *, int, int);
int    RBTrack(RBUTT *, int);

void   CBCreate(CBUTT *, Window, int, int, char *, u_long, u_long);
void   CBRedraw(CBUTT *);
void   CBSetActive(CBUTT *, int);
int    CBClick(CBUTT *,int,int);
int    CBTrack(CBUTT *);

void   MBCreate(MBUTT *, Window, int, int, int, int, char *, 
		char **, int, u_long, u_long);
void   MBRedraw(MBUTT *);
void   MBSetActive(MBUTT *, int);
int    MBClick(MBUTT *, int, int);
int    MBTrack(MBUTT *);


/**************************** XVGRAF.C ***************************/
void   CreateGraf(GRAF *, Window, int, int, u_long, u_long, char *);
void   InitGraf  (GRAF *);
void   RedrawGraf(GRAF *, int, int, int, int);
int    ClickGraf (GRAF *, Window, int, int);
int    GrafKey   (GRAF *, char *);
void   GenerateGrafFunc(GRAF *, int);
void   Graf2Str  (GRAF_STATE *, char *);
int    Str2Graf  (GRAF_STATE *, char *);
void   GetGrafState (GRAF *, GRAF_STATE *);
int    SetGrafState (GRAF *, GRAF_STATE *);
void   InitSpline(int *, int *, int, double *);
double EvalSpline(int *, int *, double *, int, double);


/**************************** XVGIF.C ***************************/
int LoadGIF(char *, int);

/*************************** XVGIFWR.C **************************/
int WriteGIF(FILE *, byte *, int, int, byte *, byte *, byte *, int, int);

/**************************** XVPM.C ****************************/
int LoadPM(char *, int);
int WritePM(FILE *, byte *, int, int, byte *, byte *, byte *, int, int);

/**************************** XVPBM.C ***************************/
int LoadPBM(char *, int);
int WritePBM(FILE *, byte *, int, int, byte *, byte *, byte *, int, int, int);

/**************************** XVXBM.C ***************************/
int LoadXBM(char *, int);
int WriteXBM(FILE *, byte *, int, int, char *);

/**************************** XVSUNRAS.C ***************************/
int LoadSunRas(char *, int);
int WriteSunRas(FILE *, byte *, int, int, byte *, byte *, byte*, 
		int, int, int);

/**************************** XVJPEG.C ***************************/
int  LoadJFIF(char *, int);
void CreateJPEGW(void);
void JPEGDialog(int);
int  JPEGCheckEvent(XEvent *);
void JPEGSaveParams(char *, int);

/**************************** XVTIFF.C ***************************/
int  LoadTIFF(char *, int);
void  CreateTIFFW();
void  TIFFDialog(int);
int   TIFFCheckEvent(XEvent *);
void  TIFFSaveParams(char *, int);

/*************************** XVPS.C ***************************/
void  CreatePSD(char *);
void  PSDialog(int);
int   PSCheckEvent(XEvent *);
void  PSSaveParams(char *, int);
void  PSResize(void);

/*************************** XVPOPUP.C ***************************/
void  CenterMapWindow(Window, int, int, int, int);
int   PopUp(char *, char **, int);
void  OpenAlert(char *);
void  CloseAlert();
int   PSCheckEvent(XEvent *);
void  TextRect(Window, char *, int, int, int, int, u_long);

/*************************** XVDFLT.C ***************************/
void LoadDfltPic();
void xbm2pic(char *, int, int, byte *, int, int, int, int, int);

/**************************** XVGRAB.C ***************************/
int Grab(void);
int LoadGrab(void);


/**************************** XVRLE.C ***************************/
int LoadRLE(char *, int);


#else     /* using non-ANSI cc.  Function defs, but no params */




/****************************** XV.C ****************************/
void FixAspect(), HandleDispMode();
int  DeleteCmd(), rd_int(), rd_str(), rd_flag(), rd_str_cl();
char *lower_str();

/****************************** XVEVENT.C ****************************/
int  EventLoop(), HandleEvent();
void DrawWindow(), WResize(), WRotate(), WCrop(), WUnCrop();
void GetWindowPos(), SetWindowPos(), InvCropRect(), SetEpicMode();
int  xvErrorHandler();

/****************************** XVROOT.C ****************************/
void MakeRootPic(), ClearRoot(), KillOldRootInfo(), SaveRootInfo();

/*************************** XVMISC.C ***************************/
void   StoreDeleteWindowProp();
Window CreateWindow();
int    StringWidth();
void   CenterString(), ULineString(), FakeButtonPress(), GenExpose();
void   DimRect(), SetCropString(), Warning(), FatalError(), Quit();
void   LoadFishCursors(), WaitCursor(), SetCursors(), Timer();
void   xvDestroyImage();

/*************************** XVCOLOR.C ***************************/
Status xvAllocColor();
void   SortColormap(), AllocColors(), AllocRWColors();
void   xvFreeColors(), FreeAllColors(), ApplyEditColor();

/*************************** XVIMAGE.C ***************************/
void Resize(), DoZoom(), DoCrop(), UnCrop(), AutoCrop(), Rotate(), Flip();
void FlipPic(), FSDither(), CreateXImage();

/*************************** XVSMOOTH.C ***************************/
void Smooth(), ColorDither();
byte *Smooth24();

/*************************** XV24TO8.C **************************/
void Init24to8();
int  Conv24to8();

/**************************** XVCTRL.C **************************/
void CreateCtrl(), CtrlBox(), RedrawCtrl(), DrawCtrlStr(), ScrollToCurrent();
int  ClickCtrl();

void LSCreate(), LSRedraw(), LSNewData(), LSKey();
int  LSClick();

/*************************** XVINFO.C ***************************/
void  CreateInfo(), InfoBox(), RedrawInfo(), SetInfoMode(), SetISTR();
char *GetISTR();

/**************************** XVDIR.C ***************************/
void CreateDirW(), DirBox(), RedrawDirW(), LoadCurrentDirectory();
int  ClickDirW(), DoSave(), DirKey();
void RedrawDDirW(), RedrawDNamW(), SelectDir(), TrackDDirW();
void SetDirFName(), SetDirRButt();
char *GetDirFName();
int  Globify(), CloseOutFile();
FILE *OpenOutFile();
byte *HandleBWandReduced();

/**************************** XVGAM.C **************************/
void CreateGam();
int  GamCheckEvent();
void GamBox(), NewCMap(), RedrawCMap();
void ChangeEC(), ApplyECctrls();
void GenerateFSGamma(), GammifyColors();
void Gammify1(), rgb2hsv(), hsv2rgb();

/*************************** XVSCRL.C ***************************/
void SCCreate(), SCSetRange(), SCRedraw(), SCTrack();
int SCSetVal();

/*************************** XVDIAL.C ***************************/
void DCreate(), DSetRange(), DSetVal(), DRedraw(), DSetActive();
int  DTrack();

/**************************** XVBUTT.C ***************************/
void BTCreate(), BTSetActive(), BTRedraw();
int  BTTrack();

RBUTT *RBCreate();
void   RBRedraw(), RBSelect(), RBSetActive();
int    RBWhich(), RBCount(), RBClick(), RBTrack();

void CBCreate(), CBRedraw(), CBSetActive();
int  CBClick(), CBTrack();

void MBCreate(), MBRedraw(), MBSetActive();
int  MBClick(), MBTrack();


/*************************** XVGRAF.C ***************************/
void   CreateGraf(), InitGraf(), RedrawGraf(), GenerateGrafFunc();
void   Graf2Str(), GetGrafState(), InitSpline();
int    ClickGraf(), GrafKey(), Str2Graf(), SetGrafState();
double EvalSpline();


/**************************** XVGIF.C ***************************/
int LoadGIF();

/*************************** XVGIFWR.C **************************/
int WriteGIF();

/**************************** XVPM.C ****************************/
int LoadPM(), WritePM();

/**************************** XVPBM.C ***************************/
int LoadPBM(), WritePBM();

/**************************** XVXBM.C ***************************/
int LoadXBM(), WriteXBM();

/**************************** XVSUNRAS.C ***************************/
int LoadSunRas(), WriteSunRas();

/**************************** XVJPEG.C ***************************/
int LoadJFIF(), JPEGCheckEvent();
void CreateJPEGW(), JPEGDialog(), JPEGSaveParams();

/**************************** XVTIFF.C ***************************/
int   LoadTIFF();
void  CreateTIFFW(), TIFFDialog(), TIFFSaveParams();
int   TIFFCheckEvent();

/*************************** XVPS.C ***************************/
void  CreatePSD(), PSDialog(), PSSaveParams(), PSResize();
int   PSCheckEvent();

/*************************** XVPOPUP.C ***************************/
void CenterMapWindow(), OpenAlert(), CloseAlert();
int  PopUp(), PUCheckEvent();
void TextRect();

/*************************** XVDFLT.C ***************************/
void LoadDfltPic(), xbm2pic();

/**************************** XVGRAB.C ***************************/
int Grab(), LoadGrab();

/**************************** XVRLE.C ***************************/
int LoadRLE();

#endif
