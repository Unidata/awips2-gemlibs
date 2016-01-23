/************************************************************************
 * cnfcmn.h                                                             *
 *                                                                      *
 * This file contains definitions used by the CNF functions.         	*
 *                                                                      *
 **                                                                     *
 * D.W.Plummer/NCEP      2/04   Created                                 *
 ***********************************************************************/

#ifndef _cnfcmn_include
#define _cnfcmn_include


/* -------------------- COMBINED CONTOUR and FILL ATTRIBUTE INFORMATION */

#define    CF_CNTR        1
#define    CF_FILL       -1
#define    CF_BOTH        0

#define    CNF_OPEN       0
#define    CNF_CLOSED     1
#define    CNF_HOLE       2

extern     int    _ncflvl;              /* Number of combined levels    */
extern     float  *_levval;             /* Level values                 */
extern     int    *_cftype;             /* Level flag for type          */
extern     int    *_lcolr;              /* Level line colors            */
extern     int    *_ltype;              /* Level line types             */
extern     int    *_lwidt;              /* Level line widths            */
extern     int    *_llabl;              /* Level line labels            */
extern     int    *_fcolr;              /* Level fill colors            */
extern     int    *_ftype;              /* Level fill types             */
extern     int    _cflvl_alloc;         /* Number of allocated levels   */


/* ----------------------------------- CONTOUR LINE and EDGE STRUCTURES */

#define    CNF_CNTR_TYPE    1
#define    CNF_EDGE_TYPE    2

#define    CNF_CNTR_START   0
#define    CNF_CNTR_END     1

struct point {
    double    x;                    /* point x coord                    */
    double    y;                    /* point y coord                    */
};
#define CNF_POINT        struct point
#define NULL_CNF_POINT   (CNF_POINT *)NULL

struct    cnf_range
{
    float    minx;                        /* Minimum x value            */
    float    miny;                        /* Minimum y value            */
    float    maxx;                        /* Maximum x value            */
    float    maxy;                        /* Maximum y value            */
};
#define CNF_RANGE        struct cnf_range

/* ------------------------------------------------ CONTOUR INFORMATION */

struct    cnf_cntr
{
    int            npts;        /* Number of points                     */
    float          *x;          /* X-coordinates                        */
    float          *y;          /* Y-coordinates                        */
    float          value;       /* Contour line value                   */
    int            level;       /* Contour line level                   */
    CNF_RANGE      range;       /* Range record                         */
    int            status;      /* Status flag (OPEN, CLOSED or HOLE)	*/
    struct edgept  *first;      /* First pt ptr to edge pt              */
    struct edgept  *last;       /* Last  pt ptr to edge pt              */
    int            used[2];     /* Flags whether cntr line used yet     */
    int            depth;       /* Depth index, 0 if status not CLOSED or
                                   HOLE; otherwise equal to the number of
                                   other CLOSED/HOLE lines that it is 
				   inside 				*/
    int            ninside;     /* Number of CLOSED/HOLE contours inside,
                                   = 0 if not CLOSED/HOLE               */
    short          *inside;     /* List of contour lines inside, NULL
                                   if not CLOSED/HOLE; otherwise list
                                   consists of line numbers             */
};
#define CNF_CNTR             struct cnf_cntr
#define NULL_CNF_CNTR        (CNF_CNTR *)NULL

struct    cnf_cntrs
{
    int        ncntrs;          /* Current number of contour lines      */
    int        max_npts;        /* Current maximum number of points	*/
    CNF_CNTR    **cntr;         /* Contour lines                        */
};
#define CNF_CNTRS            struct cnf_cntrs

/* ------------------------------------- EDGE (or CLIPPING) INFORMATION */

struct    edgept
{
    CNF_POINT      *pt;         /* Pointer to (x,y) value               */
    CNF_CNTR       *cntr;       /* Pointer to contour line              */
    int            drct;        /* Contour line direction               */
    struct edgept  *next;       /* Pointer to next edge point           */
    struct edgept  *prev;       /* Pointer to previous edge point       */
};
#define CNF_EDGEPT              struct edgept
#define NULL_CNF_EDGEPT         (CNF_EDGEPT *)NULL

struct    cnf_edge
{
    CNF_RANGE      range;       /* Range record                         */
    CNF_EDGEPT    *first;       /* Pointer to first point of edge       */
    int           closed;       /* Flag if edge is closed               */
};
#define CNF_EDGE            struct cnf_edge
#define NULL_CNF_EDGE       (CNF_EDGE *)NULL

struct    cnf_edges
{
    int         nedges;         /* Number of edge lines                 */
    CNF_EDGE    **edge;         /* Edges                                */
};
#define CNF_EDGES       struct cnf_edges



/* ------------------------ GLOBAL DECLARATIONS ----------------------- */

#define	G_NORMAL		( 0 )

/* -------------------------------------------------------------- EDGES */
extern    CNF_EDGES      _edges;
extern    int            _edges_alloc;
#define   CNF_EA_INCR    256

/* ----------------------------------------------------------- CONTOURS */
extern    CNF_CNTRS      _cntrs;
extern    int            _cntrs_alloc;
#define   CNF_CA_INCR    4096


/*
extern	float	_smooth;
extern	float	_filtln;
*/

extern  float	*_xcvp, *_ycvp, *_xcvr, *_ycvr;
extern	float	_dvcvsc;


/* ------------------------------------------------------------- MACROS */


/*=====================================================================*/

#include "proto_cnf.h"
#endif
