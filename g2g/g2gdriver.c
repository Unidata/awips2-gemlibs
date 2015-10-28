#include "geminc.h"
#include "gemprm.h"

int g2g_driver( float *grid, float *hist, int kx, int ky, int nlines, 
                int *nlatlons, float *latPts, float *lonPts,
                int *nsmthpts, float *smthLat, float *smthLon,
                int *nextpts,  float *extLat, float *extLon,
		float *values, int *ksmth,  int *iclosed,		
                int mmnum, float *mmlat, float *mmlon,
                float *mmfi, float *mmfj, float *mmvalue,
		char *catmap, char *hstgrd,  
		char *discrete, char *dlines, 
    		char *gglimt, char *edgeopts )
/************************************************************************
 * g2g_driver								*
 *									*
 * This program executes the graph-to-grid algorithm.			*
 *                                                                      *
 * Input parameters:                                                    *
 *  GRID(*)	float	Data grid of kx * ky				*
 *  HIST(*)	float	History grid of kx * ky				*
 *  KX		int	Number of grid points in x			*
 *  KY		int	Number of grid points in y			*
 *  NLINES	int	Number of contour lines				*
 *  NLATLONS(*) int   	Number of points on each line			*
 *  LATPTS(*)	float   Latitudes of all points	(line-by-line)		*
 *  LONPTS(*)	float   Longitedes of all points (line-by-line)		*
 *  NSMTHPTS(*) int   	# of points on each line(smoothed in grid space)*
 *  SMTHLAT(*)	float   Y-Coordinates of points (smoothed in grid space)*
 *  SMTHLON(*)	float   X-Coordinates of points (smoothed in grid space)*
 *  NEXTPTS(*) int   	# of points(smoothed/extended in grid space)	*
 *  EXTLAT(*)	float   Y-Coordinates of points (smoothed/extedned)	*
 *  EXTLON(*)	float   X-Coordinates of points (smoothed/extended)	*
 *  CATMAP	char*   s1=x;s2=y;...sN=z				*
 *  HISTGRD   	char*	Toggle writing history grid (y/n)		*
 *  DISCRETE	char*   a-b=x;c-d=y;e-f=z				*
 *  DLINES	char*   [yes/no][;yes/no] | epsilon			*
 *  GGLIMS    	char*   Grid value limitations and control		*
 *  EDGEOPTS	char*  	Option to specify boundary conditions.		*
 *                                                            		*
 * Output parameters:                                          		*
 *									*
 **									*
 * Log:									*
 * J. Wu/Chugach      03/10   Created    				*
 ***********************************************************************/
{
    int		ier;
/*---------------------------------------------------------------------*/

    /*
     *  Initialize graph-to-grid common area "grphgd.cmn"
     */
    printf( "Initialize g2g lib kx = %d and ky = %d\n", kx, ky);
    g2g_init( &ier );
   
        
    /*
     * Set line coordinates into common block.
     */
    printf( "Set lines and min/max info into common block\n" ); 
    g2g_setdata( &nlines, nlatlons, latPts, lonPts,
                 nsmthpts, smthLat, smthLon,
                 nextpts,  extLat, extLon,
     		 values, ksmth, iclosed, 
		 &mmnum, mmlat, mmlon, mmfi, mmfj, mmvalue,
		 &ier );     

     /*
     *  Set the user options
     */    
    printf( "Processing options\n");    
    g2g_param( catmap, discrete, dlines, gglimt, edgeopts,
               &ier,strlen(catmap), strlen(discrete), strlen(dlines),
	       strlen(gglimt), strlen(edgeopts) );
    
    /*
     * Check bounds with edge options.
     */
    printf( "Add rectangle bounds at the edge\n");        
    g2g_edgeopt( &kx, &ky, &ier );         
    
    /*
     *  Calculate intersection points between the contour lines and grids.
     */
    printf( "Calculate intersections\n"); 
          
    g2g_findint ( grid, hist, &kx, &ky, &ier );
       
    /*
     * Do graph-to-grid calcualtion now.
     */
    printf( "Run Graph-To-Grid calculations\n");    
    int hstflag = 0;
    if ( hstgrd != NULL && strlen(hstgrd) > 0 && 
         ( hstgrd[0] == 'Y' || hstgrd[0] == 'y' ) ) {
        hstflag = 1;
    }
    
    g2g_calc ( grid, &kx, &ky, hist, &hstflag, &ier );
            
    printf( "G2G Calculations Completed\n");
       
    return 0;

}
