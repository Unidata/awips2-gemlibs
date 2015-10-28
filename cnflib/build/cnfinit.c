#include "geminc.h"
#include "gemprm.h"
#include "cnf.h"

/* -------------------------------------------------------------------- */

CNF_EDGES       _edges;
int		_edges_alloc;

CNF_CNTRS       _cntrs;
int		_cntrs_alloc;

float	*_xcvp, *_ycvp, *_xcvr, *_ycvr;
float	_dvcvsc;

int     _ncflvl;
float   *_levval;  
int     *_cftype; 
int     *_lcolr; 
int     *_ltype;    
int     *_lwidt;   
int     *_llabl;  
int     *_fcolr;
int     *_ftype;
int     _cflvl_alloc;

/*
float	_smooth;
float	_filtln;
*/

/* -------------------------------------------------------------------- */

void cnf_init ( int *iret )

/************************************************************************
 * cnf_init								*
 *                                                                      *
 * This function initializes the CNF structures.			*
 *                                                                      *
 * void cnf_init ( iret )						*
 *                                                                      *
 * Input parameters:                                                    *
 *                                                                      *
 * Output parameters:                                             	*
 *  *iret		int	Return code				*
 *                                                                      *
 **                                                                     *
 * Log:                                                                 *
 * D.W.Plummer/NCEP	 4/00						*
 ***********************************************************************/
{
/* int	ier; */
    int	n = LLMXPT;
/*---------------------------------------------------------------------*/

    *iret = G_NORMAL;

    cnf_done ( iret );

    _edges.nedges = 0;
    _edges_alloc = CNF_EA_INCR;
    G_MALLOC ( _edges.edge, CNF_EDGE*, _edges_alloc, "Edges allocation" );

    _cntrs.ncntrs = 0;
    _cntrs.max_npts = 0;
    _cntrs_alloc = CNF_CA_INCR;
    G_MALLOC ( _cntrs.cntr, CNF_CNTR*, _cntrs_alloc, "Cntrs allocation" );

    _ncflvl = 0;

    _cflvl_alloc = 0;

    G_MALLOC ( _xcvp, float, n, "xcvp");
    G_MALLOC ( _ycvp, float, n, "ycvp");
    G_MALLOC ( _xcvr, float, n, "xcvr");
    G_MALLOC ( _ycvr, float, n, "ycvr");
 /*   gqcvsc ( &_dvcvsc, &ier );   */

}

/*=====================================================================*/
