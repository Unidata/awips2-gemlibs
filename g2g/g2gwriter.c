#include "geminc.h"
#include "gemprm.h"

int g2g_writer( float *grid, float *hist, char *hstgrd,  
		char *gdfile, char *proj, char *cpyfil, 
		char *gdarea, char *anlyss, char *kxky,
                char *maxgrd, char *gparm, char *gdatim, 
                char *gvcord, char *glevel )
/************************************************************************
 * g2g_writer								*
 *									*
 * This program writes the graph-to-grid results to a GEMPAK grid file.	*
 *                                                                      *
 * Input parameters:                                                    *
 *  GRID(*)	float	Data grid of kx * ky				*
 *  HIST(*)	float	History grid of kx * ky				*
 *  HISTGRD   	char*	Toggle writing history grid (y/n)		*
 *  GDFILE(*)   char*   Output grid file name				*
 *  PROJ(*)     char*   Projection of the grid				*
 *  CPYFIL(*)   char*   CPYFIL of the grid - not used			*
 *  GDAREA(*)   char*   Graphic area of the grid			*
 *  ANLYSS*)    char*   Analysis bolck - not used			*
 *  KXKY(*)     char* 	Dimension of the grid				*
 *  MAXGRD(*)   char*   Number of mximum grids				*
 *  GPARM(*)    char*   Parameter name of the grid			*
 *  GDATIM(*)   char*   Gempak date and time				*
 *  GVCORD(*)   char*   Vertical coodinate 				*
 *  GLEVEL(*)   char*   Level of the grid				*
 *                                                            		*
 * Output parameters:                                          		*
 *									*
 **									*
 * Log:									*
 * J. Wu/CWS      07/10   Created    					*
 ***********************************************************************/
{
    int		ier;
/*---------------------------------------------------------------------*/
       
    /*
     * Call Fortran subroutine to do the job.
     */
    printf( "Write out grid to %s\n", gdfile);    
    int hstflag = 0;
    if ( hstgrd != NULL && strlen(hstgrd) > 0 && 
         ( hstgrd[0] == 'Y' || hstgrd[0] == 'y' ) ) {
        hstflag = 1;
    }

    g2g_save( grid, hist, &hstflag, 
	      gdfile, proj, cpyfil, gdarea,
              anlyss, kxky, maxgrd, gparm,
              gdatim, gvcord, glevel, &ier,
     	      strlen(gdfile), strlen(proj), strlen(cpyfil), strlen(gdarea),
	      strlen(anlyss), strlen(kxky), strlen(maxgrd), strlen(gparm),
	      strlen(gdatim), strlen(gvcord), strlen(glevel) );
        
    printf( "G2G Writing Completed\n");
       
    return ier;

}
