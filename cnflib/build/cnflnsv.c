#include "geminc.h"
#include "gemprm.h"
#include "cnf.h"

/* -------------------------------------------------------------------- */

void cnf_lnsv ( int *type, int *npts, float *x, float *y, 
		float *value, int *iret )
/************************************************************************
 * cnf_lnsv								*
 *                                                                      *
 * This function saves a line (either CNTR or EDGE) to the appropriate	*
 * line structures.							*
 * 									*
 * Note that the ordering of the points is critical to the success of	*
 * the contour fill algorithm:						*
 * CONTOUR LINES:  higher grid values are to the right of the line,	*
 *                  lower grid values are to the  left of the line.	*
 * EDGE    LINES:   valid grid values are to the right of the line,	*
 *                missing grid values are to the  left of the line.	*
 *                                                                      *
 * void cnf_lnsv ( type, npts, x, y, value, iret )			*
 *                                                                      *
 * Input parameters:                                                    *
 *  *type	int	Type of line (CNTR or EDGE)			*
 *                                                                      *
 * Output parameters:                                             	*
 *  *iret	int	Return code					*
 *			= G_NORMAL - normal				*
 *			= -1 = insufficient number of points		*
 *			= -2 = CNF_INIT has not been called		*
 *                                                                      *
 **                                                                     *
 * Log:                                                                 *
 * D.W.Plummer/NCEP	02/04						*
 ***********************************************************************/

{
int		ii, numpts;
/* float 		dens; */
/* int 		maxpts=LLMXPT; */
int 		one=1;
/* int 		noutp, noutr, ier, nbytes; */
CNF_EDGEPT	*ept, *ept_tmp, *ept_start;
/*---------------------------------------------------------------------*/

    *iret = G_NORMAL;

    if ( *npts <= 1 )  {
        *iret = -1;
        return;
    }

    /*
     * If _cntrs.cntr or _edges.edge have not been allocated, return error.
     */
    if ( _cntrs.cntr == NULL  ||  _edges.edge == NULL )  {
	*iret = -2;
	return;
    }

    /*
    FILE *fptr;
    char buffer[80];
    fptr = cfl_aopn ( "lnsv_dump", &ier );
    sprintf ( buffer, "%d %d %f\n", *type, *npts, *value );
    cfl_writ ( fptr, strlen(buffer), buffer, &ier );
    for ( ii = 0; ii < *npts; ii++ )  {
	sprintf ( buffer, "%f %f\n", x[ii], y[ii] );
        cfl_writ ( fptr, strlen(buffer), buffer, &ier );
    }
    cfl_clos ( fptr, &ier );
    */

    switch ( *type )  {

	case	CNF_CNTR_TYPE:		/* Save CNTR (contour) line	*/

	    if ( _cntrs.ncntrs == _cntrs_alloc )  {
		/*
		 * Re-allocate contour line memory with more.
		 */
		_cntrs_alloc += CNF_CA_INCR;
	        G_REALLOC ( _cntrs.cntr, CNF_CNTR*, _cntrs_alloc, "CNF_CNTR*" );
printf("G_REALLOC cntr to %d\n", _cntrs_alloc );
	    }

	    /* Allocate a new contour */
	    G_MALLOC ( _cntrs.cntr[_cntrs.ncntrs], CNF_CNTR, one, "CNF_CNTR" );

	    /* Closed flag */
	    _cntrs.cntr[_cntrs.ncntrs]->status = CNF_OPEN;
	    if ( x[0]==x[*npts-1] && y[0]==y[*npts-1] )  {
		_cntrs.cntr[_cntrs.ncntrs]->status = CNF_CLOSED;
		if ( *npts == 2 )  {
		    _cntrs.cntr[_cntrs.ncntrs]->status = CNF_OPEN;
/* printf("Setting line %d to CNF_OPEN (%f,%f)\n", _cntrs.ncntrs, x[0], y[0] ); */
		}
	    }
/* if(_cntrs.ncntrs==137)printf("Setting line %d to %s (%f,%f)\n", _cntrs.ncntrs, _cntrs.cntr[_cntrs.ncntrs]->status==CNF_OPEN?"OPEN":"CLOSED", x[0], y[0] ); */

	 /*   if ( _smooth == 0.0F && _filtln == 0.0F )  {   */

	      /* Number of points in contour line */
	      _cntrs.cntr[_cntrs.ncntrs]->npts = *npts;
	      /* Pointer to coordinate information */
	      G_MALLOC ( _cntrs.cntr[_cntrs.ncntrs]->x, float, *npts, "CNTR x" );
	      G_MALLOC ( _cntrs.cntr[_cntrs.ncntrs]->y, float, *npts, "CNTR y" );
	      memcpy(_cntrs.cntr[_cntrs.ncntrs]->x,x,(*npts)*sizeof(float));
	      memcpy(_cntrs.cntr[_cntrs.ncntrs]->y,y,(*npts)*sizeof(float));

	      numpts = *npts;
            /*
	    }
	    else  {

	      dens = _smooth;
	      if ( _smooth == 2 )  dens = 5;

	      if ( _smooth == 0.0F )  {
		nbytes = *npts * sizeof(float);
		memcpy ( _xcvp, x, nbytes );
		memcpy ( _ycvp, y, nbytes );
		noutp = *npts;
	      }
	      else  {
	        cv_prmt ( npts, x, y, &dens, &maxpts, &_dvcvsc, &one, npts, 
			&noutp, _xcvp, _ycvp, &ier );
	      }

	      if ( _filtln == 0.0F )  {
		nbytes = noutp * sizeof(float);
		memcpy ( _xcvr, _xcvp, nbytes );
		memcpy ( _ycvr, _ycvp, nbytes );
		noutr = noutp;
	      }
	      else  {
	        cv_rduc ( &noutp, _xcvp, _ycvp, &_filtln, 
		        &noutr, _xcvr, _ycvr, &ier );
	      }

	      _xcvr[0] = x[0];
	      _ycvr[0] = y[0];
	      _xcvr[noutr-1] = x[*npts-1];
	      _ycvr[noutr-1] = y[*npts-1];

	      * Number of points in adjusted contour line *
	      _cntrs.cntr[_cntrs.ncntrs]->npts = noutr;
	      * Pointer to coordinate information *
	      G_MALLOC ( _cntrs.cntr[_cntrs.ncntrs]->x, float, noutr, "CNTR x" );
	      G_MALLOC ( _cntrs.cntr[_cntrs.ncntrs]->y, float, noutr, "CNTR y" );
	      memcpy(_cntrs.cntr[_cntrs.ncntrs]->x,_xcvr,noutr*sizeof(float));
	      memcpy(_cntrs.cntr[_cntrs.ncntrs]->y,_ycvr,noutr*sizeof(float));
	      numpts = noutr;

	    }
            */

	    /* Edge pointers */
	    _cntrs.cntr[_cntrs.ncntrs]->first = NULL_CNF_EDGEPT;
	    _cntrs.cntr[_cntrs.ncntrs]->last  = NULL_CNF_EDGEPT;
	    /* Usage flags */
	    _cntrs.cntr[_cntrs.ncntrs]->used[0] = G_FALSE;
	    _cntrs.cntr[_cntrs.ncntrs]->used[1] = G_FALSE;
	    /* Depth index */
	    _cntrs.cntr[_cntrs.ncntrs]->depth= 0;
	    /* Inside count, if CNF_CLOSED or CNF_OPEN */
	    _cntrs.cntr[_cntrs.ncntrs]->ninside= 0;
	    /* Inside  list, if CNF_CLOSED or CNF_OPEN */
	    _cntrs.cntr[_cntrs.ncntrs]->inside= (short*)NULL;
	    /* Contour value */
	    _cntrs.cntr[_cntrs.ncntrs]->value = *value;
	    /* Contour level */
	    _cntrs.cntr[_cntrs.ncntrs]->level = IMISSD;

	    /* Compute range record */
	    _cntrs.cntr[_cntrs.ncntrs]->range.minx =  FLT_MAX;
	    _cntrs.cntr[_cntrs.ncntrs]->range.miny =  FLT_MAX;
	    _cntrs.cntr[_cntrs.ncntrs]->range.maxx = -FLT_MAX;
	    _cntrs.cntr[_cntrs.ncntrs]->range.maxy = -FLT_MAX;
	    for ( ii = 0; ii < numpts; ii++ )  {
/* if(_cntrs.ncntrs==137)printf("%3d - (%f,%f)\n",ii, _cntrs.cntr[_cntrs.ncntrs]->x[ii], _cntrs.cntr[_cntrs.ncntrs]->y[ii]); */
		_cntrs.cntr[_cntrs.ncntrs]->range.minx = 
		    G_MIN(_cntrs.cntr[_cntrs.ncntrs]->range.minx,
			    _cntrs.cntr[_cntrs.ncntrs]->x[ii]);
		_cntrs.cntr[_cntrs.ncntrs]->range.miny =
		    G_MIN(_cntrs.cntr[_cntrs.ncntrs]->range.miny,
			    _cntrs.cntr[_cntrs.ncntrs]->y[ii]);
		_cntrs.cntr[_cntrs.ncntrs]->range.maxx =
		    G_MAX(_cntrs.cntr[_cntrs.ncntrs]->range.maxx,
			    _cntrs.cntr[_cntrs.ncntrs]->x[ii]);
		_cntrs.cntr[_cntrs.ncntrs]->range.maxy =
		    G_MAX(_cntrs.cntr[_cntrs.ncntrs]->range.maxy,
			    _cntrs.cntr[_cntrs.ncntrs]->y[ii]);
	    }
/* if(_cntrs.ncntrs==137)printf("%d - %s\n", _cntrs.ncntrs, _cntrs.cntr[_cntrs.ncntrs]->status==CNF_OPEN?"OPEN":"CLOSED"); */

	    _cntrs.max_npts = G_MAX ( _cntrs.max_npts, _cntrs.cntr[_cntrs.ncntrs]->npts );
	    _cntrs.ncntrs += 1;

	    break;

	case	CNF_EDGE_TYPE:		        /* Save EDGE (edge) line */

	    if ( _edges.nedges == _edges_alloc )  {
		/*
		 * Re-allocate edges memory with more.
		 */
		_edges_alloc += CNF_EA_INCR;
	        G_REALLOC ( _edges.edge, CNF_EDGE*, _edges_alloc, "CNF_EDGE*" );
printf("G_REALLOC edge to %d\n", _edges_alloc );
	    }

	    /* Allocate a new edge */
	    G_MALLOC ( _edges.edge[_edges.nedges], CNF_EDGE, one, "CNF_EDGE" );

	    /* Initialize range record */
	    _edges.edge[_edges.nedges]->range.minx =  FLT_MAX;
	    _edges.edge[_edges.nedges]->range.miny =  FLT_MAX;
	    _edges.edge[_edges.nedges]->range.maxx = -FLT_MAX;
	    _edges.edge[_edges.nedges]->range.maxy = -FLT_MAX;

	    ii = 0;
	    while ( ii < *npts )  {
		if ( ii == 0 )  {
		    ept = cnf_new_edge_pt ( x[ii], y[ii] );
		    ept_start = ept;
		    ept->prev = NULL_CNF_EDGEPT;
		}
		else  {
		    ept_tmp = ept;
		    ept = cnf_new_edge_pt ( x[ii], y[ii] );
		    ept_tmp->next = ept;
		    ept->next = NULL_CNF_EDGEPT;
		    ept->prev = ept_tmp;
		}
/* printf("Add point %d (%3.0f,%3.0f) to edge %d\n", ii, x[ii], y[ii], _edges.nedges ); */
		ept->cntr = NULL_CNF_CNTR;
		ept->drct = IMISSD;
		/* Compute range record */
	        _edges.edge[_edges.nedges]->range.minx =
		    G_MIN(_edges.edge[_edges.nedges]->range.minx,ept->pt->x);
	        _edges.edge[_edges.nedges]->range.miny =
		    G_MIN(_edges.edge[_edges.nedges]->range.miny,ept->pt->y);
	        _edges.edge[_edges.nedges]->range.maxx =
		    G_MAX(_edges.edge[_edges.nedges]->range.maxx,ept->pt->x);
	        _edges.edge[_edges.nedges]->range.maxy =
		    G_MAX(_edges.edge[_edges.nedges]->range.maxy,ept->pt->y);

		ii += 1;
		/* 
		 * Eliminate duplicate points
		while ( ii < *npts && 
			( x[ii] == x[ii-1] && y[ii] == y[ii-1] ) )  {
		    ii += 1;
		}
		 */
	    }
	    _edges.edge[_edges.nedges]->first = ept_start;

	    _edges.edge[_edges.nedges]->closed = CNF_OPEN;
	    if ( cnf_compare_pt ( ept->pt,
			_edges.edge[_edges.nedges]->first->pt ) == G_TRUE )  {
		/* Remove duplicate start and end point */
		ept->prev->next = ept_start;
	        ept_start->prev = ept->prev;
		G_FREE ( ept->pt, CNF_POINT );
		G_FREE ( ept, CNF_EDGEPT );
		_edges.edge[_edges.nedges]->closed = CNF_CLOSED;
	    }

	    _edges.nedges += 1;

	    break;

    }

}

/*=====================================================================*/
