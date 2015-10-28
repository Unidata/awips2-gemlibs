#include "geminc.h"
#include "gemprm.h"
#include "cnf.h"

/* -------------------------------------------------------------------- */

void cnf_dump ( int *dumptype, int *iret )
/************************************************************************
 * cnf_dump								*
 *                                                                      *
 * This function dumps the contents of the global structures.		*
 *                                                                      *
 * void cnf_dump ( dumptype, iret )					*
 *                                                                      *
 * Input parameters:                                                    *
 *  *dumptype	int	Type of dump					*
 *   			= 0 - edges (short)				*
 *   			= 1 - cntrs (short)				*
 *   			= 2 - edges (long)				*
 *   			= 3 - cntrs (long)				*
 *   			= 4 - Contour interval information		*
 *                                                                      *
 * Output parameters:                                             	*
 *  *iret	int	Return code					*
 *			= G_NORMAL - normal				*
 *                                                                      *
 **                                                                     *
 * Log:                                                                 *
 * D.W.Plummer/NCEP	02/04						*
 ***********************************************************************/

{
int		ii, jj, nn;
CNF_EDGEPT	*ept, *tept;
char		buffer[128], prev[32], curr[32], next[32];
/*---------------------------------------------------------------------*/

    *iret = G_NORMAL;

    switch ( *dumptype )  {

	case	0:			              /* EDGES (short) */

	    printf("SHORT DUMP\nNumber of EDGES = %d\n", _edges.nedges );

	    for ( ii = 0; ii < _edges.nedges; ii++ )  {
		if ( _edges.edge[ii] != NULL_CNF_EDGE )  {
		    printf("EDGE #%d ... %s\n", ii, 
			    _edges.edge[ii]->closed==CNF_CLOSED?"(closed)":"" );
		    nn = 0;
		    ept = _edges.edge[ii]->first;
		    if ( ept != NULL_CNF_EDGEPT )  {
		        do {
			    if ( nn == 0 || nn == 1 ||
				 ept->next == NULL_CNF_EDGEPT ||
				 ept->next == _edges.edge[ii]->first ||
				 ept->next->next == NULL_CNF_EDGEPT ||
				 ept->next->next == _edges.edge[ii]->first )
		                printf("\tPoint #%3d - (%0.2f,%0.2f)\n", 
			            nn, ept->pt->x, ept->pt->y );
			    nn += 1; ept = ept->next;
		        } while ( ept != NULL_CNF_EDGEPT &&
			          ept != _edges.edge[ii]->first );
		    }
		    else  {
			printf(" --- NULL EDGE ---\n");
		    }
		}
	    }

	    break;

	case	2:			/* EDGES (long)			*/

	    printf("LONG DUMP\nNumber of EDGES = %d\n", _edges.nedges );

	    for ( ii = 0; ii < _edges.nedges; ii++ )  {
		if ( _edges.edge[ii] != NULL_CNF_EDGE )  {
		    printf("EDGE #%d ... %s\n", ii, 
			    _edges.edge[ii]->closed==CNF_CLOSED?"(closed)":"" );
		    nn = 0;
		    ept = _edges.edge[ii]->first;
		    if ( ept != NULL_CNF_EDGEPT )  {
		        do {
			    tept = ept->next;
			    if ( ept->prev != NULL_CNF_EDGEPT )
			        sprintf(prev,"(%f,%f)",
				    ept->prev->pt->x, ept->prev->pt->y );
			    else
			        sprintf(prev,"(   NULL,   NULL)");
			    if ( ept != NULL_CNF_EDGEPT )
			        sprintf(curr,"(%f,%f)",
				    ept->pt->x, ept->pt->y );
			    else
			        sprintf(curr,"(   NULL,   NULL)");
			    if ( ept->next != NULL_CNF_EDGEPT )
			        sprintf(next,"(%f,%f)",
				    ept->next->pt->x, ept->next->pt->y );
			    else
			        sprintf(next,"(   NULL,   NULL)");
		            sprintf(buffer,"   Point #%3d - %s-%s-%s %s %s",
			        nn, prev, curr, next,
				( ept->cntr != NULL_CNF_CNTR )?"- *":"NULL",
			( tept == NULL_CNF_EDGEPT )?"NULL_CNF_EDGEPT":"" );
		            printf("%s\n", buffer );
		            ept = tept;
			    nn += 1;
		        } while ( ept != NULL_CNF_EDGEPT &&
			          ept != _edges.edge[ii]->first );
		    }
		    else  {
			printf(" --- NULL EDGE ---\n");
		    }
		}
	    }

	    break;

	case	1:			/* LINES (short)		*/

	    printf("Number of CONTOUR LINES = %d\n", _cntrs.ncntrs );

	    for ( ii = 0; ii < _cntrs.ncntrs; ii++ )  {
		printf("LINE #%d (npts=%d%s,used=%1d;%1d,level=%d,depth=%d,nin=%d) RANGE (%0.1f,%0.1f)-(%0.1f,%0.1f), VALUE=%0.1E\n", ii, 
			    _cntrs.cntr[ii]->npts,
			    _cntrs.cntr[ii]->status==CNF_CLOSED?",closed":
			    _cntrs.cntr[ii]->status==CNF_HOLE?",hole":"",
			    _cntrs.cntr[ii]->used[0],
			    _cntrs.cntr[ii]->used[1],
			    _cntrs.cntr[ii]->level,
			    _cntrs.cntr[ii]->depth,
			    _cntrs.cntr[ii]->ninside,
			    _cntrs.cntr[ii]->range.minx,
			    _cntrs.cntr[ii]->range.miny,
			    _cntrs.cntr[ii]->range.maxx,
			    _cntrs.cntr[ii]->range.maxy,
		     	    _cntrs.cntr[ii]->value );
		nn = 0;
		printf("\tPoint #%3d - (%0.1f,%0.1f)\n", nn,
			_cntrs.cntr[ii]->x[nn],
			_cntrs.cntr[ii]->y[nn] );
		nn = _cntrs.cntr[ii]->npts-1;
		printf("\tPoint #%3d - (%0.1f,%0.1f)\n", nn,
			_cntrs.cntr[ii]->x[nn],
			_cntrs.cntr[ii]->y[nn] );
	    }


	    break;

	case	3:			/* LINES (long)			*/

	    printf("Number of CONTOUR LINES = %d\n", _cntrs.ncntrs );

	    for ( ii = 0; ii < _cntrs.ncntrs; ii++ )  {
		printf("LINE #%d (npts=%d%s,used=%1d;%1d,level=%d,depth=%d,nin=%d) RANGE (%0.1f,%0.1f)-(%0.1f,%0.1f), VALUE=%0.1E\n", ii, 
			    _cntrs.cntr[ii]->npts,
			    _cntrs.cntr[ii]->status==CNF_CLOSED?",closed":
			    _cntrs.cntr[ii]->status==CNF_HOLE?",hole":"",
			    _cntrs.cntr[ii]->used[0],
			    _cntrs.cntr[ii]->used[1],
			    _cntrs.cntr[ii]->level,
			    _cntrs.cntr[ii]->depth,
			    _cntrs.cntr[ii]->ninside,
			    _cntrs.cntr[ii]->range.minx,
			    _cntrs.cntr[ii]->range.miny,
			    _cntrs.cntr[ii]->range.maxx,
			    _cntrs.cntr[ii]->range.maxy,
		     	    _cntrs.cntr[ii]->value );
		for ( nn = 0; nn < _cntrs.cntr[ii]->npts; nn++ )  {
		    printf("\tPoint #%3d - (%0.1f,%0.1f)\n", nn,
			_cntrs.cntr[ii]->x[nn],
			_cntrs.cntr[ii]->y[nn] );
		}
	    }


	    break;

	case	4:		/* Contour and fill interval info	*/

	    printf("COMBINED CONTOUR LEVEL INFORMATION :\n");
	    printf("Number of contour fill levels (combined) = %d\n", _ncflvl );
	    printf(" lvl       value    typ    col    typ    wid    lbl   fcol\n");
		for ( ii = 0; ii < _ncflvl; ii++ )  {
		    printf("%5d%12.3f %5d%7d%7d%7d%7d%7d%7d\n", ii,
		    _levval[ii],_cftype[ii],_lcolr[ii],_ltype[ii],
		    _lwidt[ii],_llabl[ii],_fcolr[ii],_ftype[ii] );
		}


	    break;

	case	51:		/* DUMP ALL */

	    printf("%d\n", _cntrs.ncntrs );
	    for ( ii = 0; ii < _cntrs.ncntrs; ii++ )  {
		printf("1 %d %f\n", 
			_cntrs.cntr[ii]->npts, _cntrs.cntr[ii]->value );
		for ( jj = 0; jj < _cntrs.cntr[ii]->npts; jj++ )
		    printf("%f %f\n", 
			    _cntrs.cntr[ii]->x[jj], _cntrs.cntr[ii]->y[jj] );
	    }

	    printf("%d\n", _edges.nedges );
	    for ( ii = 0; ii < _edges.nedges; ii++ )  {
		nn = 0;
		ept = _edges.edge[ii]->first;
		if ( ept != NULL_CNF_EDGEPT )  {
		    do {
			nn += 1;
			ept = ept->next;
		    } while ( ept != _edges.edge[ii]->first );
		}
		printf("2 %d -9999.0\n", nn );
		ept = _edges.edge[ii]->first;
		if ( ept != NULL_CNF_EDGEPT )  {
		    do {
			printf("%f %f\n", ept->pt->x, ept->pt->y );
			ept = ept->next;
		    } while ( ept != _edges.edge[ii]->first );
		}

	    }

	    break;

    }
    
    printf("CNF_DUMP - complete.\n");

}

/*=====================================================================*/
