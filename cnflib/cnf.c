#include "geminc.h"
#include "gemprm.h"
#include "cnf.h"

/* -------------------------------------------------------------------- */

CNF_EDGEPT* cnf_new_edge_pt ( float x, float y )
/************************************************************************
 * cnf_new_edge_pt							*
 *                                                                      *
 * This function creates a new CNF edge point and returns its address.	*
 *                                                                      *
 * void cnf_new_edge_pt ( x, y )					*
 *                                                                      *
 * Input parameters:                                                    *
 *  x			float		X-coordinate			*
 *  y			float		Y-coordinate			*
 *                                                                      *
 * Output parameters:                                             	*
 *  none								*
 *                                                                      *
 * Returned parameters:                                             	*
 *  *cnf_new_edge_pt	CNF_EDGEPT	Pointer to new edge point	*
 *                                                                      *
 **                                                                     *
 * Log:                                                                 *
 * D.W.Plummer/NCEP	02/04						*
 ***********************************************************************/

{
CNF_POINT	*pt;
CNF_EDGEPT	*ept;
/*---------------------------------------------------------------------*/

    G_MALLOC ( ept, CNF_EDGEPT, 1, "CNF_EDGEPT" );

    G_MALLOC ( pt, CNF_POINT, 1, "CNF_POINT" );

    pt->x = x; 
    pt->y = y;

    ept->pt = pt;
    ept->cntr = NULL_CNF_CNTR;
    ept->drct = IMISSD;
    ept->next = NULL_CNF_EDGEPT;

    return ( ept );

}

/*=====================================================================*/

int cnf_compare_pt ( CNF_POINT* pt0, CNF_POINT* pt1 )
/************************************************************************
 * cnf_compare_pt							*
 *                                                                      *
 * This function compares two CNF points for equality.			*
 *                                                                      *
 * void cnf_compare_pt ( pt0, pt1 )					*
 *                                                                      *
 * Input parameters:                                                    *
 *  *pt0		CNF_POINT	Point #0			*
 *  *pt1		CNF_POINT	Point #1			*
 *                                                                      *
 * Output parameters:                                             	*
 *  none								*
 *                                                                      *
 * Returned parameters:                                             	*
 *  cnf_compare_pt	int		G_TRUE or G_FALSE		*
 *                                                                      *
 **                                                                     *
 * Log:                                                                 *
 * D.W.Plummer/NCEP	02/04						*
 ***********************************************************************/

{
/*---------------------------------------------------------------------*/

    if ( !pt0 )  return ( G_FALSE );
    if ( !pt1 )  return ( G_FALSE );

    if ( G_DIFF(pt0->x,pt1->x) && G_DIFF(pt0->y,pt1->y) )  return ( G_TRUE );

    return ( G_FALSE );

}

/*=====================================================================*/

/*
int cnf_range_int ( CNF_RANGE rng0, CNF_RANGE rng1 )
 ************************************************************************
 * cnf_range_int							*
 *                                                                      *
 * This function compares two CNF contour range records and returns	*
 * G_TRUE if the two ranges intersect.					*
 *                                                                      *
 * void cnf_range_int ( cntr0, cntr1 )					*
 *                                                                      *
 * Input parameters:                                                    *
 *  rng0		CNF_RANGE	Range #0			*
 *  rng1		CNF_RANGE	Range #1			*
 *                                                                      *
 * Output parameters:                                             	*
 *  none								*
 *                                                                      *
 * Returned parameters:                                             	*
 *  cnf_range_int	int		G_TRUE or G_FALSE		*
 *                                                                      *
 **                                                                     *
 * Log:                                                                 *
 * D.W.Plummer/NCEP	02/04						*
 ***********************************************************************
{
int	ier;
*---------------------------------------------------------------------*

    return ( cgr_ntrsct ( rng0.minx, rng0.miny, rng0.maxx, rng0.maxy, 
			  rng1.minx, rng1.miny, rng1.maxx, rng1.maxy,
	       		  &ier ) );

}
*/

/*=====================================================================*/
