/*#include "geminc.h"*/
/*#include "gemprm.h"*/
#include "cnf.h"

/* -------------------------------------------------------------------- */

void cnf_getnumcntrs ( float cval, int *numcntrs )
/************************************************************************
 * cnf_getnumcntrs							*
 *                                                                      *
 * This function returns the number contour lines for the given 	*
 * data value								*
 *                                                                      *
 * void cnf_getnumcntrs ( cval, numcntrs )				*
 *                                                                      *
 * Input parameters:                                                    *
 *  cval	int	Contour value					*
 *                                                                      *
 * Output parameters:                                             	*
 *  *numcntrs	int	Number of contour lines				*
 *                                                                      *
 **                                                                     *
 * Log:                                                                 *
 * S.Gilbert/NCEP	06/10						*
 ***********************************************************************/
{
    int ii, count;

    count = 0;

    for ( ii = 0; ii < _cntrs.ncntrs; ii++ )  {

         if ( _cntrs.cntr[ii]->value == cval ) {
            count++;
            
         }
     }

     *numcntrs = count;
}

void cnf_getcntrinfo ( float cval, int *numpolys, int *numpoints )
/************************************************************************
 * cnf_getcntrinfo							*
 *                                                                      *
 * This function returns the number contour lines and the number of 	*
 * points in each contour line for the given data value			*
 *                                                                      *
 * void cnf_getcntrinfo ( cval, numcntrs, numpoints )			*
 *                                                                      *
 * Input parameters:                                                    *
 *  cval	int	Contour value					*
 *                                                                      *
 * Output parameters:                                             	*
 *  *numcntrs	int	Number of contour lines				*
 *  *numpoints	int	Number of points in each contour line		*
 *                                                                      *
 **                                                                     *
 * Log:                                                                 *
 * S.Gilbert/NCEP	06/10						*
 ***********************************************************************/
{
    int ii, count;

    count = 0;

    for ( ii = 0; ii < _cntrs.ncntrs; ii++ )  {

         if ( _cntrs.cntr[ii]->value == cval ) {
            numpoints[ count ] = _cntrs.cntr[ii]->npts;
            count++;
            
         }
     }

     *numpolys = count;
}



void cnf_getcontour ( float cval, float *xcoord, float *ycoord,
                      int *numpolys, int *numpoints )
/************************************************************************
 * cnf_getcontour							*
 *                                                                      *
 * This function returns the number contour lines and the number of 	*
 * points in each contour line for the given data value.  It also 	*
 * returns the X and Y coordinates of all the points in the contour 	*
 * lines appended together in the same order as the numpoints array.	*
 *                                                                      *
 * void cnf_getcontour ( cval, xcoord, ycoord, numpolys, numpoints )	*
 *                                                                      *
 * Input parameters:                                                    *
 *  cval	int	Contour value					*
 *                                                                      *
 * Output parameters:                                             	*
 *  *xcoord     float   Array of X coordinates of all the points in     *
 *                      contour lines					*
 *  *ycoord     float   Array of Y coordinates of all the points in     *
 *                      contour lines					*
 *  *numpolys	int	Number of contour lines				*
 *  *numpoints	int	Number of points in each contour line		*
 *                                                                      *
 **                                                                     *
 * Log:                                                                 *
 * S.Gilbert/NCEP	06/10						*
 ***********************************************************************/
{
    int ii, nn, num, count;

    num = 0;
    count = 0;
    for ( ii = 0; ii < _cntrs.ncntrs; ii++ )  {

          if ( _cntrs.cntr[ii]->value != cval ) continue;

            numpoints[ count ] = _cntrs.cntr[ii]->npts;
            count++;
            /*
            if ( _cntrs.cntr[ii]->status == CNF_CLOSED ) {
               printf("CLOSED %f %f %f %f\n", _cntrs.cntr[ii]->x[0],
                                               _cntrs.cntr[ii]->y[0],
                                 _cntrs.cntr[ii]->x[_cntrs.cntr[ii]->npts-1],
                                 _cntrs.cntr[ii]->y[_cntrs.cntr[ii]->npts-1] );
            }
            else {
               printf("OPEN\n");
            }
            */ 
	    for ( nn = 0; nn < _cntrs.cntr[ii]->npts; nn++ )  {
	        xcoord[num] = _cntrs.cntr[ii]->x[nn];
		ycoord[num] = _cntrs.cntr[ii]->y[nn];
                num++;
	    }

    }
     *numpolys = count;

}

