#include "geminc.h"
#include "gemprm.h"
#include "cnf.h"


void cnf_vals ( int *nclvl, float *clvl, int *icolrs, int *lintyp, 
		int *linwid, int *linlbl, int *nflvl, float *flvl, 
		int *ifcolr, int *iftype, int *iret )

/************************************************************************
 * cnf_vals								*
 *                                                                      *
 * This function sets up all the contour interval and attribute info.	*
 *                                                                      *
 * void cnf_vals ( nclvl, clvl, icolrs, lintyp, linwid, linlbl,		*
 * 		   nflvl, flvl, ifcolr, iret )				*
 *                                                                      *
 * Input parameters:                                                    *
 *	int	*nclvl		Number of contour line levels		*
 *	float	*clvl		Array of contour line levels		*
 *	int	*icolrs		Array of contour line colors		*
 *	int	*lintyp		Array of contour line types		*
 *	int	*linwid		Array of contour line widths		*
 *	int	*linlbl		Array of contour line labels		*
 *	int	*nflvl		Number of contour fill levels		*
 *	float	*flvl		Array of contour fill levels		*
 *	int	*ifcolr		Array of contour fill colors		*
 *	int	*iftype		Array of contour fill types 		*
 *                                                                      *
 * Output parameters:                                             	*
 *  *iret	int	Return code					*
 *			= 0 - normal					*
 *                                                                      *
 **                                                                     *
 * Log:                                                                 *
 * D.W.Plummer/NCEP	 4/00						*
 ***********************************************************************/

{
int	ii, nc, ncf, ncfx, found, it;
float	t;
/*---------------------------------------------------------------------*/

    *iret = 0;

    G_FREE ( _levval, float );
    G_FREE ( _cftype,   int );
    G_FREE (  _lcolr,   int );
    G_FREE (  _ltype,   int );
    G_FREE (  _lwidt,   int );
    G_FREE (  _llabl,   int );
    G_FREE (  _fcolr,   int );
    G_FREE (  _ftype,   int );
    _ncflvl = 0;

    _cflvl_alloc = *nclvl + *nflvl + 1;
    G_MALLOC ( _levval, float, _cflvl_alloc, "_levval" );
    G_MALLOC ( _cftype, int, _cflvl_alloc, "_cftype" );
    G_MALLOC (  _lcolr, int, _cflvl_alloc, " _lcolr" );
    G_MALLOC (  _ltype, int, _cflvl_alloc, " _ltype" );
    G_MALLOC (  _lwidt, int, _cflvl_alloc, " _lwidt" );
    G_MALLOC (  _llabl, int, _cflvl_alloc, " _llabl" );
    G_MALLOC (  _fcolr, int, _cflvl_alloc, " _fcolr" );
    G_MALLOC (  _ftype, int, _cflvl_alloc, " _ftype" );

    for ( ii = 0; ii < *nclvl; ii++ )  {
	_levval[_ncflvl] = clvl[ii];
	_lcolr[_ncflvl]  = icolrs[ii];
	_ltype[_ncflvl]  = lintyp[ii];
	_lwidt[_ncflvl]  = linwid[ii];
	_llabl[_ncflvl]  = linlbl[ii];
	_fcolr[_ncflvl]  = IMISSD;
	_ftype[_ncflvl]  = IMISSD;
	_cftype[_ncflvl] = CF_CNTR;
	_ncflvl++;
    }

    for ( ii = 0; ii < *nflvl; ii++ )  {

	found = G_FALSE;
	nc = 0;
	while ( !found  &&  nc < *nclvl )  {

	    if ( flvl[ii] == _levval[nc] )
		found = G_TRUE;
	    else
		nc++;

	}

	if ( ! found  &&  (_ncflvl+1) <= _cflvl_alloc )  {

	    _levval[_ncflvl] = flvl[ii];
            _fcolr[_ncflvl]  = ifcolr[ii];
            _ftype[_ncflvl]  = iftype[ii];
            _lcolr[_ncflvl]  = IMISSD;
            _ltype[_ncflvl]  = IMISSD;
            _lwidt[_ncflvl]  = IMISSD;
            _llabl[_ncflvl]  = IMISSD;
            _cftype[_ncflvl] = CF_FILL;
	    _ncflvl++;

	}
	else  {

	    _fcolr[nc]  = ifcolr[ii];
	    _ftype[nc]  = iftype[ii];
	    _cftype[nc] = CF_BOTH;
	}

    }

    if ( (_ncflvl+1) <= _cflvl_alloc )  {

        _levval[_ncflvl] = 999999.0;
        _fcolr[_ncflvl] = ifcolr[*nflvl];
        _ftype[_ncflvl] = iftype[*nflvl];
        _lcolr[_ncflvl] = IMISSD;
        _ltype[_ncflvl] = IMISSD;
        _lwidt[_ncflvl] = IMISSD;
        _llabl[_ncflvl] = IMISSD;
        _cftype[_ncflvl] = CF_FILL;
        _ncflvl++;

    }

    /*
     *  Sort arrays by increasing level value
     */

    for ( ncf = 0; ncf < _ncflvl-1; ncf++ )  {
 
        for ( ncfx = 0; ncfx < _ncflvl-ncf-1; ncfx++ )  {
 
            if ( _levval[ncfx] > _levval[ncfx+1] )  {
 
                    t = _levval[ncfx+1];
                    _levval[ncfx+1] = _levval[ncfx];
                    _levval[ncfx] = t;
 
                    it = _lcolr[ncfx+1];
                    _lcolr[ncfx+1] = _lcolr[ncfx];
                    _lcolr[ncfx] = it;
 
                    it = _ltype[ncfx+1];
                    _ltype[ncfx+1] = _ltype[ncfx];
                    _ltype[ncfx] = it;
 
                    it = _lwidt[ncfx+1];
                    _lwidt[ncfx+1] = _lwidt[ncfx];
                    _lwidt[ncfx] = it;
 
                    it = _llabl[ncfx+1];
                    _llabl[ncfx+1] = _llabl[ncfx];
                    _llabl[ncfx] = it;
 
                    it = _fcolr[ncfx+1];
                    _fcolr[ncfx+1] = _fcolr[ncfx];
                    _fcolr[ncfx] = it;
 
                    it = _ftype[ncfx+1];
                    _ftype[ncfx+1] = _ftype[ncfx];
                    _ftype[ncfx] = it;
 
                    it = _cftype[ncfx+1];
                    _cftype[ncfx+1] = _cftype[ncfx];
                    _cftype[ncfx] = it;
 
            }
 
        }
 
    }

}

/*=====================================================================*/

void cnf_gval ( int *index, float *value, int *iret )

/************************************************************************
 * cnf_gval								*
 *                                                                      *
 * This function returns the contour level value given an index.	*
 *                                                                      *
 * void cnf_gval ( index, value, iret )					*
 *                                                                      *
 * Input parameters:                                                    *
 *  index	int	Contour fill level index			*
 *                                                                      *
 * Output parameters:                                             	*
 *  *value	float	Value						*
 *  *iret	int	Return code					*
 *			= 0 - normal					*
 **                                                                     *
 * Log:                                                                 *
 * D.W.Plummer/NCEP	 4/04						*
 ***********************************************************************/

{
int	nvals, ier;
/*---------------------------------------------------------------------*/

    *iret = G_NORMAL;

    cnf_nval ( &nvals, &ier );

    if ( *index >=0 && *index < nvals )  {
	*value = _levval[*index];
    }
    else  {
	*value = RMISSD;
	*iret = -1;
    }

    return;

}

/*=====================================================================*/

void cnf_ginx ( float *value, int *index, int *iret )

/************************************************************************
 * cnf_ginx								*
 *                                                                      *
 * This function returns the index given the contour level value.	*
 *                                                                      *
 * void cnf_gval ( value, index, iret )					*
 *                                                                      *
 * Input parameters:                                                    *
 *  value	float	Value						*
 *                                                                      *
 * Output parameters:                                             	*
 *  *index	int	Contour fill level index			*
 *  *iret	int	Return code					*
 *			= 0 - normal					*
 **                                                                     *
 * Log:                                                                 *
 * D.W.Plummer/NCEP	 4/04						*
 ***********************************************************************/

{
int	ii, nvals, ier;
/*---------------------------------------------------------------------*/

    *iret = G_NORMAL;

    cnf_nval ( &nvals, &ier );

    for ( ii = 0; ii < nvals; ii++ )  {
	if ( _levval[ii] == *value )  {
	    *index = ii;
	    return;
	}
    }

    *iret = -1;
    *index = IMISSD;
    return;

}

/*=====================================================================*/

void cnf_nval ( int *nvals, int *iret )

/************************************************************************
 * cnf_nval								*
 *                                                                      *
 * This function returns the number of contour levels.			*
 *                                                                      *
 * void cnf_nval ( nvals, iret )					*
 *                                                                      *
 * Input parameters:                                                    *
 *  none								*
 *                                                                      *
 * Output parameters:                                             	*
 *  *nvals	int		Number of contour fill levels		*
 *  *iret	int		Return code				*
 *				= 0 - normal				*
 *                                                                      *
 **                                                                     *
 * Log:                                                                 *
 * D.W.Plummer/NCEP	 4/04						*
 ***********************************************************************/

{
/*---------------------------------------------------------------------*/

    *iret = G_NORMAL;

    *nvals = _ncflvl;

}

/*=====================================================================*/
void cnf_gcol ( float v, int which, int *color, int *type, int *iret )
/************************************************************************
 * cnf_gcol                                                             *
 *                                                                      *
 * This function gets the fill color and type given a numerical value.  *
 *                                                                      *
 * void cnf_gcol ( v, which, color, type, iret )                        *
 *                                                                      *
 * Input parameters:                                                    *
 *  v		float	Contour fill level value threshold		*
 *  which	int	1 (above) or -1 (below) v			*
 *                                                                      *
 * Output parameters:                                                   *
 *  *color	int	Color						*
 *  *type 	int	Type 						*
 *  *iret       int     Return code                                     *
 *                      = 0 - normal                                    *
 *                                                                      *
 **                                                                     *
 * Log:                                                                 *
 * D.W.Plummer/NCEP      4/04                                           *
 ***********************************************************************/

{
int     ip;
/*---------------------------------------------------------------------*/

    *iret = G_NORMAL;

    if ( which == -1 )  {

        /*
         * Get the color below threshold 'v'
         */
        ip = 0;
        while ( v != _levval[ip]  &&  ip < _ncflvl )  ip++;

    }
    else if ( which == 1 )  {

        /*
         * Get the color above threshold 'v'
         */
	ip = 0;
	while ( v != _levval[ip]  &&  ip < _ncflvl )  ip++;
	ip++;
	while ( _cftype[ip] > 0  &&  ip < _ncflvl )  ip++;

    }

    *color = _fcolr[ip];
    *type  = _ftype[ip];

}

/*=====================================================================*/
