#include "geminc.h"
#include "gemprm.h"
#include "cnf.h"

/* -------------------------------------------------------------------- */

void cnf_done ( int *iret )

/************************************************************************
 * cnf_done								*
 *                                                                      *
 * This function frees all CNF allocated structures.			*
 *                                                                      *
 * void cnf_done ( iret )						*
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
int	nc, tnc;
/*---------------------------------------------------------------------*/

    *iret = 0;

    cnf_free_edges ( );
    G_FREE ( _edges.edge, CNF_EDGE* );

    tnc = _cntrs.ncntrs;
    for ( nc = 0; nc < tnc; nc++ )  {
	if ( _cntrs.cntr[nc] != NULL_CNF_CNTR )  {
	    G_FREE ( _cntrs.cntr[nc]->x, float );
	    G_FREE ( _cntrs.cntr[nc]->y, float );
	    G_FREE ( _cntrs.cntr[nc]->inside, short );
	    G_FREE ( _cntrs.cntr[nc], CNF_CNTR );
	}
	_cntrs.ncntrs -= 1;
    }
/* printf("_cntrs.ncntrs = %d\n", _cntrs.ncntrs ); */
    G_FREE ( _cntrs.cntr, CNF_CNTR* );

    G_FREE ( _levval, float );
    G_FREE ( _cftype,   int );
    G_FREE (  _lcolr,   int );
    G_FREE (  _ltype,   int );
    G_FREE (  _lwidt,   int );
    G_FREE (  _llabl,   int );
    G_FREE (  _fcolr,   int );
    G_FREE (  _ftype,   int );

    G_FREE ( _xcvr, float );
    G_FREE ( _ycvr, float );
    G_FREE ( _xcvp, float );
    G_FREE ( _ycvp, float );

}

/*=====================================================================*/

void	cnf_free_edges ( )
{
int		ne, tne;
CNF_EDGEPT	*ept, *tept;
/*---------------------------------------------------------------------*/

    tne = _edges.nedges;
    for ( ne = 0; ne < tne; ne++ )  {
	if ( _edges.edge[ne] != NULL_CNF_EDGE )  {
            ept = _edges.edge[ne]->first;
	    if ( ept != NULL_CNF_EDGEPT )  {
                do {
	            tept = ept->next;
	            G_FREE ( ept->pt, CNF_POINT );
	            G_FREE ( ept, CNF_EDGEPT );
	            ept = tept;
	        } while ( ept != NULL_CNF_EDGEPT &&
		          ept != _edges.edge[ne]->first );
	    }
	}
        G_FREE ( _edges.edge[ne], CNF_EDGE );
	_edges.nedges -= 1;
    }
/* printf("_edges.nedges = %d\n", _edges.nedges ); */

}
