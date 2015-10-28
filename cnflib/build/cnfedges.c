#include "geminc.h"
#include "gemprm.h"
#include "cnf.h"

/* -------------------------------------------------------------------- */

void cnf_getnumedges ( int *numedges )
/************************************************************************
 * cnf_getnumedges                                                      *
 *                                                                      *
 * This function returns the number edge areas associated with the data *
 *                                                                      *
 * void cnf_getnumedges ( numedges )	                                *
 *                                                                      *
 * Input parameters:                                                    *
 *                                                                      *
 * Output parameters:                                                   *
 *  *numedges   int     Number of edge polygons                         *
 *                                                                      *
 **                                                                     *
 * Log:                                                                 *
 * S.Gilbert/NCEP       06/10                                           *
 ***********************************************************************/
{
     *numedges = _edges.nedges;
}

void cnf_getedgeinfo ( int *numedges, int *numpoints )
/************************************************************************
 * cnf_getedgeinfo                                                      *
 *                                                                      *
 * This function returns the number edge areas associated with the data *
 * and the number of points in each polygon				*
 *                                                                      *
 * void cnf_getedgeinfo ( numedges, numpoints )                         *
 *                                                                      *
 * Input parameters:                                                    *
 *                                                                      *
 * Output parameters:                                                   *
 *  *numedges   int     Number of edge polygons                         *
 *  *numpoints  int     Number of points in each edge polygon           *
 *                                                                      *
 **                                                                     *
 * Log:                                                                 *
 * S.Gilbert/NCEP       06/10                                           *
 ***********************************************************************/
{
    int ii, count;
    CNF_EDGEPT      *ept;

    for ( ii = 0; ii < _edges.nedges; ii++ )  {
         if ( _edges.edge[ii] == NULL_CNF_EDGE ) continue;

         count = 0;
         ept = _edges.edge[ii]->first;

         do {
            count++;
            ept = ept->next;
            
         } while ( ept != NULL_CNF_EDGEPT &&
                   ept != _edges.edge[ii]->first );

         numpoints[ ii ] = count;
     }

     *numedges = _edges.nedges;
}



void cnf_getedges ( float *xcoord, float *ycoord,
                     int *numedges, int *numpoints )
/************************************************************************
 * cnf_getedges	                                                        *
 *                                                                      *
 * This function returns the number edge areas associated with the data *
 * and the number of points in each polygon. It also       		*
 * returns the X and Y coordinates of all the points in the edge	*
 * polygons appended together in the same order as the numpoints array. *
 *                                                                      *
 * void cnf_getedges ( xcoord, ycoord, numedges, numpoints )            *
 *                                                                      *
 * Input parameters:                                                    *
 *                                                                      *
 * Output parameters:                                                   *
 *  *xcoord     float   Array of X coordinates of all the points in     *
 *                      edge ploygons                                   *
 *  *ycoord     float   Array of Y coordinates of all the points in     *
 *                      edge ploygons                                   *
 *  *numedges   int     Number of edge polygons                         *
 *  *numpoints  int     Number of points in each edge polygon           *
 *                                                                      *
 **                                                                     *
 * Log:                                                                 *
 * S.Gilbert/NCEP       06/10                                           *
 ***********************************************************************/
{
    int ii, num, count;
    CNF_EDGEPT      *ept;

    num = 0;
    for ( ii = 0; ii < _edges.nedges; ii++ )  {
         if ( _edges.edge[ii] == NULL_CNF_EDGE ) continue;

         count = 0;
         ept = _edges.edge[ii]->first;

         do {
            count++;
            xcoord[num] = ept->pt->x;
            ycoord[num] = ept->pt->y;
            num++;
            ept = ept->next;
            
         } while ( ept != NULL_CNF_EDGEPT &&
                   ept != _edges.edge[ii]->first );

         numpoints[ ii ] = count;
     }

     *numedges = _edges.nedges;
}
