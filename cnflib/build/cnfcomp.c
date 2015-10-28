#include "geminc.h"
#include "gemprm.h"
#include "cnf.h"

#define	Z(iii,jjj)  ( grid [ (long)(((jjj)*_kx) + (iii)) ] )

#define	LTYP_CNTR	1
#define	CNF_EDGE_TYPE	2

int	_kx, _ky;

/* -------------------------------------------------------------------- */

void	t2r_xy ( int nin, int njn, float *grid, int imz, int jmz, float zc, float *xpn, float *ypn );
void	cnfmiss ( float *grid, int imz, int jmz, float *xp, float *yp, int *nmax, int *iret );
void	cnfdrct ( float *grid, int imz, int jmz, float zc, int npts, float *xp, float *yp, int *iret );
void	cnfedge ( float *grid, int imz, int jmz, int ioffx, int ioffy, float skip1, float *xp, float *yp, int *iret );
int	iqrvrs ( float z, float zl, float zr );
int	lneqv ( float zcent, float zarr, float zc );

/* -------------------------------------------------------------------- */

void cnf_comp ( int *imz_in, int *jmz_in, float *grid, 
	        int *ioffx, int *ioffy, int *iskip, int *iret )

/************************************************************************
 * cnf_comp								*
 *                                                                      *
 * This function draws contours through a grid of data.			*
 *                                                                      *
 * void cnf_comp ( imz, jmz, grid, ioffx, ioffy, iskip, iret )		*
 *                                                                      *
 * Input parameters:                                                    *
 *  *imz		int	Grid x-dimension			*
 *  *jmz		int	Grid y-dimension			*
 *  *grid		float	Grid dimensioned (imz*jmz)		*
 *  *ioffx		int	Offset in x-direction			*
 *  *joffy		int	Offset in y-direction			*
 *  *iskip		int	Skip factor				*
 *                                                                      *
 * Output parameters:                                             	*
 *  *iret		int	Return code				*
 *                                                                      *
 **                                                                     *
 * Log:                                                                 *
 * D.W.Plummer/NCEP	 2/05						*
 ***********************************************************************/
{
int	imz, jmz;
int	nlines, imz2, ncflvl, ncend, nc, nc1, itype;
float	zmax1, zmin1, zc, zmin, zmax;
/*float	xp[LLMXPT], yp[LLMXPT];*/
float	*xp, *yp;
/*int	*ni=(int *)xp, *nj=(int *)yp;*/
int	*ni, *nj;
char	*l1, **l, **ms;
int	emiss, misval, rev;
int	ii, jj, iijj, i, j, ipos, ier, ixoff, iyoff, ix, i2;
float	skip1, fx, fy, d, zcent, xtmp, ytmp, dx, xx, dy, yy;
int	inc, is, ie, itemp, ip, jp, n, ipass, jpass, iz, jz, nmax, nmax2;
int	nexc, iexc, jexc;
int	MAXDIM, MAXL, MAXPT;
/* int	ltest1, ltest2, five=5; */
/*---------------------------------------------------------------------*/


    *iret = 0;

    imz = *imz_in;
    jmz = *jmz_in;
    _kx = imz;
    _ky = jmz;

    MAXDIM = G_MAX ( imz, jmz );
    MAXL   = MAXDIM * 2 + 1;

    MAXPT = LLMXPT;
    G_MALLOC ( xp, float, MAXPT, "Error allocating xp" );
    G_MALLOC ( yp, float, MAXPT, "Error allocating yp" );
    ni=(int *)xp;
    nj=(int *)yp;
    
    G_MALLOC ( l1, char, MAXDIM, "Error allocating l1" );
    for ( jj = 0; jj < MAXDIM; jj++ )  {
	l1[jj] = G_FALSE;
    }

    G_MALLOC ( l, char *, MAXL, "Error allocating l" );
    for ( ii = 0; ii < MAXL; ii++ )  {
	G_MALLOC ( l[ii], char, MAXL, "Error allocating l[ii]" );
        for ( jj = 0; jj < MAXL; jj++ )  {
	    l[ii][jj] = G_FALSE;
        }
    }
    G_MALLOC ( ms, char *, MAXL, "Error allocating ms" );
    for ( ii = 0; ii < MAXL; ii++ )  {
	G_MALLOC ( ms[ii], char, MAXL, "Error allocating ms[ii]" );
        for ( jj = 0; jj < MAXL; jj++ )  {
	    ms[ii][jj] = G_FALSE;
        }
    }

    /*
printf("imz,jmz= %d %d\n", imz, jmz );
for ( i = 1; i <= imz; i++ ) {
    printf("  %d", i );
for ( j = 1; j <= jmz; j++ ) {
    printf("  %.0f", Z(i-1,j-1) );
}
    printf("\n");
}
printf("%f\n", Z(2,2) );
*/

    skip1 = *iskip + 1.0F;

    ipos = 0;
    if ( imz <= 10 || jmz <= 10 )  {
	/*gscolr ( &five, &ier );*/
	iyoff = 0;
	ixoff = 0;
	ix = ixoff;
	for ( jj = 1; jj <= jmz; jj++ )  {
	    fy = (jj-1.0F) * skip1 + 1.0F + *ioffy;
	    for ( ii = 1; ii <= imz; ii++ )  {
	        fx = (ii-1.0F) * skip1 + 1.0F + *ioffx;
		d = Z(ii-1,jj-1);
		if ( ! ERMISS ( d ) )  {
		}
	    }
	}
    }

    nlines = 0;

/*     printf ( "BEGIN CONTOURING\n"); */

    zmax1 = -FLT_MAX;
    zmin1 =  FLT_MAX;
    misval = G_FALSE;

    imz2 = imz * 2 - 1;

/*     printf ( "BEGIN CONTOURING, CALL CNF_NVAL\n" ); */
    cnf_nval ( &ncflvl, &ier );
/*     printf ( "BEGIN CONTOURING, # of CONTOUR LEVELS = %d\n", ncflvl ); */

/*     printf ( "BEGIN CONTOURING, LOOP OVER %d %d\n", imz, jmz ); */

    for ( iijj = 1; iijj <= imz*jmz; iijj++ )  {

	if ( ! ERMISS ( grid[iijj-1] ) )  {
	    zmax1 = G_MAX ( zmax1, grid[iijj-1] );
	    zmin1 = G_MIN ( zmin1, grid[iijj-1] );
	}
	else  {
	    misval = G_TRUE;
	}
    }

    /*
     * GET NUMBER OF CONTOUR LEVELS
     */
    if ( misval == G_TRUE )  {
/* 	printf ( "THIS GRID CONTAINS MISSING DATA\n" ); */
        ncend = ncflvl;
    }
    else  {
	ncend = ncflvl - 1;
    }

    /*
     * CONTOURING
     */
/*     printf ( "ncflvl, ncend =  %d %d\n", ncflvl, ncend ); */
    for ( nc = 1; nc <= ncend; nc++ )  {

	nc1 = nc - 1;
	if ( nc == ncflvl )  {
	    zc = -9900;
	    zc = -999900;
	    zc = RMISSD + 99.0;
	    emiss = G_FALSE;
	    zmin = zc - 1;
	    zmax = zc + 1;
	    itype = CNF_EDGE_TYPE;
	    nc1 = -1;
	}
	else  {
	    cnf_gval ( &nc1, &zc, &ier );
	    emiss = misval;
	    zmin = zmin1;
	    zmax = zmax1;
	    itype = LTYP_CNTR;
	}
/* 	printf ( " Contouring ... nc =  %d, value= %f %.0f %.0f\n", nc, zc, zmin, zmax ); */

	if ( ( zc < zmax ) && ( zc > zmin ) )  {

	    for ( j = 1; j <= jmz; j++ )  {

		for ( i = 1; i <= imz; i++ )  {
		    l1[i-1] = ( Z(i-1,j-1) > zc ) ? G_TRUE : G_FALSE;
/* printf("  %.0f. %.0f.\n", Z(i-1,j-1), zc );		    	 */
		}

		for ( i = 1; i <= imz-1; i++ )  {

		    i2 = 2 * i;

		    l[i2-1][j-1]=( (l1[i-1] == G_TRUE  && l1[i] == G_TRUE ) || 
				   (l1[i-1] == G_FALSE && l1[i] == G_FALSE) ) 
			? G_FALSE : G_TRUE;
		    if ( emiss == G_TRUE && l[i2-1][j-1] == G_TRUE )  {
			if ( ERMISS(Z(i-1,j-1)) ||  ERMISS(Z(i,j-1)) )  {
			    l[i2-1][j-1] = G_FALSE;
			}
		    }

		    ms[i2-1][j-1] = G_FALSE;
		    if ( emiss == G_TRUE )  {

			if ( ERMISS(Z(i-1,j-1)) == G_TRUE )  {
			    ms[i2-1][j-1] = G_TRUE;
			}

			if ( i != imz )  {
			    if ( ERMISS(Z(i,j-1)) == G_TRUE )  {
				ms[i2-1][j-1] = G_TRUE;
			    }
			}
		    }
/* 		    printf(" i2, j, l(i2,j), l1(i), ms(i2,j) =  %d %d %s %s %s\n", i2, j,  */
/* 			    l[i2-1][j-1] == 1 ? "T" : "F",  */
/* 			    l1[i-1] == 1 ? "T" : "F",  */
/* 			    ms[i2-1][j-1] == 1 ? "T" : "F" ); */
		}
	    }

	    for ( i = 1; i <= imz; i++ )  {

		ii = 2 * i - 1;

		for ( j = 1; j <= jmz; j++ )  {
		    l1[j-1] = ( Z(i-1,j-1) > zc ) ? G_TRUE : G_FALSE;
		}

		for ( j = 1; j <= jmz-1; j++ )  {

		    l[ii-1][j-1]=( (l1[j-1] == G_TRUE  && l1[j] == G_TRUE ) || 
				   (l1[j-1] == G_FALSE && l1[j] == G_FALSE) ) 
			? G_FALSE : G_TRUE;
		    if ( emiss == G_TRUE && l[ii-1][j-1] == G_TRUE )  {
			if ( ERMISS(Z(i-1,j-1)) ||  ERMISS(Z(i-1,j)) )  {
			    l[ii-1][j-1] = G_FALSE;
			}
		    }

		    ms[ii-1][j-1] = G_FALSE;
		    if ( emiss == G_TRUE )  {

			if ( ERMISS(Z(i-1,j-1)) == G_TRUE )  {
			    ms[ii-1][j-1] = G_TRUE;
			}

			if ( j != jmz )  {
			    if ( ERMISS(Z(i-1,j)) == G_TRUE )  {
				ms[ii-1][j-1] = G_TRUE;
			    }
			}
		    }
/* 		    printf(" ii, j, l(ii,j), l1(j), ms(ii,j) =  %d %d %s %s %s\n", ii, j,  */
/* 			    l[ii-1][j-1] == 1 ? "T" : "F",  */
/* 			    l1[j-1] == 1 ? "T" : "F",  */
/* 			    ms[ii-1][j-1] == 1 ? "T" : "F" ); */
		}
	    }

	    for ( inc = 2; inc >= 1; inc-- )  {

		for ( j = 1; j <= jmz; j++ )  {

		    if ( j != jmz )  {
			is = 0;
			ie = imz2 + 1;
		    }
		    else  {
			is = 2;
			ie = imz2 - 1;
		    }

		    for ( itemp = is; itemp <= ie; itemp+=inc )  {

			i = itemp;
			if ( itemp == 0      )  i = 1;
			if ( itemp == imz2+1 )  i = imz2;
/* printf("itemp loop - itemp, inc, i, j %d %d %d %d\n", itemp ,inc, i, j ); */
/* printf("l[i-1][j-1]= %s\n", l[i-1][j-1] == 1 ? "T" : "F"); */
/* printf("ms[i-1][j-1]= %s\n", ms[i-1][j-1] == 1 ? "T" : "F"); */

			if ( l[i-1][j-1] == G_TRUE && 
			     ms[i-1][j-1] == G_FALSE )  {

			    /* 
			     * FOUND STARTING POINT OF CONTOUR (z = zc)
			     */

			    rev = G_FALSE;

			    n = 1;
			    ni[0] = i;
			    nj[0] = j;

			    ip = i;
			    jp = j;

			    if ( j == 1 || j == jmz  )  l[i-1][j-1] = G_FALSE;
			    if ( i == 1 || i == imz2 )  l[i-1][j-1] = G_FALSE;

			    ipass = 0;
			    jpass = 0;

	ten:		    ;

			    /* FINDING SUCCESSIVE POINT ON CONTOUR ( z = zc )  */

			    if ( ip%2 == 0 )  {

				/* ENTERING BOX from EITHER the TOP or the BOTTOM */
				
				iz = ip/2 + 1;
				jz = jp;
/* printf("iz, jz = %d %d\n", iz, jz ); */

				if ( jpass >= 0 )  {

				    /* ENTERING BOX from TOP */
/* printf("ENTERING BOX from TOP\n"); */

				    if ( jp >= 2 )  {

					if ( ms[ip][jp-2] == G_TRUE  ||
					     ms[ip-1][jp-2] == G_TRUE  ||
					     ms[ip-2][jp-2] == G_TRUE )  {
					    l[ip-1][jp-1] = G_FALSE;
/* printf("FAIL MISSING TEST\n"); */
					}
					else  {
/* printf("PASS MISSING TEST\n"); */
					    if ( l[ip][jp-2] == G_TRUE  &&
						 l[ip-1][jp-2] == G_TRUE  &&
						 l[ip-2][jp-2] == G_TRUE )  {
/* printf("SADDLE POINT PROCESSING\n"); */
						/* SADDLE POINT PROCESSING */
					        zcent = (Z(iz-2,jz-2) + Z(iz-1,jz-2) + Z(iz-2,jz-1) + Z(iz-1,jz-1)) * 0.25F;
						jj = jp - 1;
/* printf("zcent, jj = %f %d\n", zcent, jj ); */

						if ( lneqv(zcent,Z(iz-1,jz-1),zc) == G_TRUE )  {
/* 						if (((zcent>zc)&&(Z(iz-1,jz-1)>zc)) || ((zcent<=zc)&&(Z(iz-1,jz-1)<=zc)))  { */
						    ii = ip + 1;
						    goto twenty;
						}
						else  {
						    ii = ip - 1;
						    goto twenty;
						}
					    }
					    else  {
						/* DETERMINE WHICH SIDE TO EXIT, IF NONE ARE MISSING */
						ii = ip + 1;
						jj = jp - 1;
/* printf("1 - DETERMINE WHICH SIDE TO EXIT %d %d %s\n", ii, jj, l[ii-1][jj-1] == G_TRUE?"T":"F" ); */
						if ( l[ii-1][jj-1] == G_TRUE)  goto twenty;
						ii = ip;
/* printf("1 - DETERMINE WHICH SIDE TO EXIT %d %d %s\n", ii, jj, l[ii-1][jj-1] == G_TRUE?"T":"F" ); */
						if ( l[ii-1][jj-1] == G_TRUE)  goto twenty;
						ii = ip - 1;
/* printf("1 - DETERMINE WHICH SIDE TO EXIT %d %d %s\n", ii, jj, l[ii-1][jj-1] == G_TRUE?"T":"F" ); */
						if ( l[ii-1][jj-1] == G_TRUE)  goto twenty;
/* printf("1 - PASS THRU\n"); */

					    }
					}
				    }
				}

				if ( jpass <= 0 )  {

				    /* ENTERING BOX from BOTTOM */
/* printf("ENTERING BOX from BOTTOM\n"); */

				    if ( jp <= jmz-1 )  {

					if ( ms[ip-2][jp-1] == G_TRUE  ||
					     ms[ip-1][jp] == G_TRUE  ||
					     ms[ip][jp-1] == G_TRUE )  {
					    l[ip-1][jp-1] = G_FALSE;
					}
					else  {
					    if ( l[ip][jp-1] == G_TRUE  &&
						 l[ip-1][jp] == G_TRUE  &&
						 l[ip-2][jp-1] == G_TRUE )  {
/* printf("SADDLE POINT PROCESSING\n"); */

						/* SADDLE POINT PROCESSING */

						zcent = (Z(iz-2,jz-1) + Z(iz-1,jz-1) + Z(iz-2,jz) + Z(iz-1,jz)) * 0.25F;
						jj = jp;

						if ( lneqv(zcent,Z(iz-1,jz-1),zc) == G_TRUE )  {
/* 						if (((zcent>zc)&&(Z(iz-1,jz-1)>zc)) || ((zcent<=zc)&&(Z(iz-1,jz-1)<=zc)))  { */
						    ii = ip + 1;
						    goto twenty;
						}
						else  {
						    ii = ip - 1;
						    goto twenty;
						}
					    }
					    else  {

						/* DETERMINE WHICH SIDE TO EXIT, IF NONE ARE MISSING */
						ii = ip - 1;
						jj = jp;
/* printf("2 - DETERMINE WHICH SIDE TO EXIT %d %d %s\n", ii, jj, l[ii-1][jj-1] == G_TRUE?"T":"F" ); */
						if ( l[ii-1][jj-1] == G_TRUE)  goto twenty;
						ii = ip;
						jj = jp + 1;
/* printf("2 - DETERMINE WHICH SIDE TO EXIT %d %d %s\n", ii, jj, l[ii-1][jj-1] == G_TRUE?"T":"F" ); */
						if ( l[ii-1][jj-1] == G_TRUE)  goto twenty;
						ii = ip + 1;
						jj = jp;
/* printf("2 - DETERMINE WHICH SIDE TO EXIT %d %d %s\n", ii, jj, l[ii-1][jj-1] == G_TRUE?"T":"F" ); */
						if ( l[ii-1][jj-1] == G_TRUE)  goto twenty;
/* printf("2 - PASS THRU\n"); */
					    }
					}
				    }

				}

			    }

			    else  {

				/* ENTERING BOX from EITHER the LEFT or the RIGHT */
				iz = (ip+1) / 2;
				jz = jp;

				if ( ipass >= 0 )  {

				    /* ENTERING BOX from LEFT */
/* printf("ENTERING BOX from LEFT\n"); */

				    if ( ip < imz2 )  {

					if ( ms[ip][jp] == G_TRUE  ||
					     ms[ip+1][jp-1] == G_TRUE  ||
					     ms[ip][jp-1] == G_TRUE )  {
					    l[ip-1][jp-1] = G_FALSE;
					}
					else  {
					    if ( l[ip][jp-1] == G_TRUE  &&
						 l[ip+1][jp-1] == G_TRUE  &&
						 l[ip][jp] == G_TRUE )  {

/* printf("SADDLE POINT PROCESSING\n"); */
						/* SADDLE POINT PROCESSING */

					        zcent = (Z(iz-1,jz-1) + Z(iz,jz-1) + Z(iz-1,jz) + Z(iz,jz)) * 0.25F;
						ii = ip + 1;

						if ( lneqv(zcent,Z(iz-1,jz-1),zc) == G_TRUE )  {
/* 						if (((zcent>zc)&&(Z(iz-1,jz-1)>zc)) || ((zcent<=zc)&&(Z(iz-1,jz-1)<=zc)))  { */
						    jj = jp;
						    goto twenty;
						}
						else  {
						    jj = jp + 1;
						    goto twenty;
						}
					    }
					    else  {

						/* DETERMINE WHICH SIDE TO EXIT, IF NONE ARE MISSING */
						ii = ip + 1;
						jj = jp + 1;
/* printf("3 - DETERMINE WHICH SIDE TO EXIT %d %d %s\n", ii, jj, l[ii-1][jj-1] == G_TRUE?"T":"F" ); */
						if ( l[ii-1][jj-1] == G_TRUE)  goto twenty;
						ii = ip + 2;
						jj = jp;
/* printf("3 - DETERMINE WHICH SIDE TO EXIT %d %d %s\n", ii, jj, l[ii-1][jj-1] == G_TRUE?"T":"F" ); */
						if ( l[ii-1][jj-1] == G_TRUE)  goto twenty;
						ii = ip + 1;
/* printf("3 - DETERMINE WHICH SIDE TO EXIT %d %d %s\n", ii, jj, l[ii-1][jj-1] == G_TRUE?"T":"F" ); */
						if ( l[ii-1][jj-1] == G_TRUE)  goto twenty;
/* printf("3 - PASS THRU\n"); */

					    }
					}
				    }
				}

				if ( ipass <= 0 )  {

				    /* ENTERING BOX from RIGHT */
/* printf("ENTERING BOX from RIGHT\n"); */

				    if ( ip > 1 )  {

					if ( ms[ip-2][jp-1] == G_TRUE  ||
					     ms[ip-3][jp-1] == G_TRUE  ||
					     ms[ip-2][jp] == G_TRUE )  {
					    l[ip-1][jp-1] = G_FALSE;
					}
					else  {
					    if ( l[ip-2][jp-1] == G_TRUE  &&
						 l[ip-3][jp-1] == G_TRUE  &&
						 l[ip-2][jp] == G_TRUE )  {
/* printf("SADDLE POINT PROCESSING\n"); */

						/* SADDLE POINT PROCESSING */

						zcent = (Z(iz-2,jz-1) + Z(iz-1,jz-1) + Z(iz-2,jz) + Z(iz-1,jz)) * 0.25F;
						ii = ip - 1;

						if ( lneqv(zcent,Z(iz-1,jz-1),zc) == G_TRUE )  {
						    jj = jp;
/* printf("4a - GOTO 20\n"); */
						    goto twenty;
						}
						else  {
						    jj = jp + 1;
/* printf("4b - GOTO 20\n"); */
						    goto twenty;
						}
					    }
					    else  {

						/* DETERMINE WHICH SIDE TO EXIT, IF NONE ARE MISSING */
						ii = ip - 1;
						jj = jp;
/* printf("4 - DETERMINE WHICH SIDE TO EXIT %d %d %s\n", ii, jj, l[ii-1][jj-1] == G_TRUE?"T":"F" ); */
						if ( l[ii-1][jj-1] == G_TRUE)  goto twenty;
						ii = ip - 2;
/* printf("4 - DETERMINE WHICH SIDE TO EXIT %d %d %s\n", ii, jj, l[ii-1][jj-1] == G_TRUE?"T":"F" ); */
						if ( l[ii-1][jj-1] == G_TRUE)  goto twenty;
						ii = ip - 1;
						jj = jp + 1;
/* printf("4 - DETERMINE WHICH SIDE TO EXIT %d %d %s\n", ii, jj, l[ii-1][jj-1] == G_TRUE?"T":"F" ); */
						if ( l[ii-1][jj-1] == G_TRUE)  goto twenty;
/* printf("4 - PASS THRU\n"); */
					    }
					}
				    }
				}
			    }

			    nmax = n;

			    if ( ( l[i-1][j-1] == G_FALSE  && 
				  ms[i-1][j-1] == G_FALSE )  ||
				    ( ni[0] == ni[nmax-1] && 
				      nj[0] == nj[nmax-1] ) )  {

				/* SUCCESSIVE POINT CAN NOT BE FOUND */

				goto thirty;

			    }
			    else  {

				/* ANOTHER BRANCH FROM POINT (i,j) EXISTS */
				/* REVERSE THE ORDER OF THE POINTS        */

				ii = ni[0];
				ip = ni[1];
				jj = nj[0];
				jp = nj[1];
				nmax2 = nmax / 2;

				for ( n = 1; n <= nmax2; n++ )  {

				    nexc = (nmax+1) - n;

				    iexc = ni[n-1];
				    ni[n-1] = ni[nexc-1];
				    ni[nexc-1] = iexc;

				    jexc = nj[n-1];
				    nj[n-1] = nj[nexc-1];
				    nj[nexc-1] = jexc;

				}

				n = nmax - 1;

				rev = G_TRUE;

			    }

			    twenty: ;

			    /* (ii,jj) IS THE SUCCESSIVE POINT  */

			    n = n + 1;

			    /*if ( n > (int)(sizeof(xp)/sizeof(float)) )  {*/
			    if ( n > MAXPT )  {
				printf("*** INCREASE MAXPLT IN SUBROUTINE CNF_COMP ***\n");
				goto sortie;
			    }
			    else  {
			        ni[n-1] = ii;
			        nj[n-1] = jj;
			    }
			    l[ii-1][jj-1] = G_FALSE;
/* printf("n, ni(n), nj(n), l(ii,jj) = %d %d %d %s\n", n, ni[n-1], nj[n-1], l[ii-1][jj-1]==G_TRUE?"T":"F" ); */

			    if ( ii > ip )  {
				ipass =  1;
			    }
			    else  {
				ipass = -1;
			    }

			    if ( jj <= jp )  {
				jpass =  1;
			    }
			    else  {
				jpass = -1;
			    }

			    ip = ii;
			    jp = jj;
/* printf("ipass, jpass, ip, jp = %d %d %d %d\n", ipass, jpass, ip, jp ); */

			    goto ten;

			    thirty: ;

			    if ( nmax > 1 )  {

				/* CONVERSION FROM TOPOLOGICAL TO REAL CONTOUR */

				for ( n = 1; n <= nmax; n++ )  {

				    t2r_xy ( ni[n-1], nj[n-1], grid, imz, jmz, zc, &(xp[n-1]), &(yp[n-1]) ) ;
				}

				if ( rev == G_TRUE )  {

				    for ( n = 1; n <= nmax/2; n++ )  {

					ip = nmax - n + 1;
					xtmp = xp[n-1];
					xp[n-1] = xp[ip-1];
					xp[ip-1] = xtmp;
					ytmp = yp[n-1];
					yp[n-1] = yp[ip-1];
					yp[ip-1] = ytmp;

				    }

				}

				if ( itype == CNF_EDGE_TYPE )  {

				    /* 
				     * If drawing a "missing" edge line, adjust
				     * the locations to the opposite grid point.
				     */

				    cnfmiss ( grid, imz, jmz, xp, yp, &nmax, &ier );

				}
				else  {

				    cnfdrct ( grid, imz, jmz, zc, nmax, xp, yp, &ier );
				}

				/* SCALE THE X and Y COORDINATES */

				if ( itype == CNF_EDGE_TYPE )  {

				    if ( *ioffx != 0 || !G_DIFF(skip1,0.0F) )  {

					for ( n = 1; n <= nmax; n++ )  {

					    dx = xp[n-1] - (int)(xp[n-1]);
					    if ( G_DIFF(dx,0.0F) )  {
						xx = (xp[n-1]-1.0F) * skip1 + 1.0F + *ioffx;
					    }
					    else  {
						if ( dx < 0.5F )  {
						    xx = ((int)(xp[n-1]+1.0F)-1.0F) * skip1 + 1.0F + *ioffx;
						    /*xx = xx - 1.0F + dx;*/
						}
						else  {
						    xx = ((int)(xp[n-1])-1.0F) * skip1 + 1.0F + *ioffx;
						    /*xx = xx + dx;*/
						}
					    }
					    xp[n-1] = xx;
					}
				    }
				    
				    if ( *ioffy != 0 || !G_DIFF(skip1,0.0F) )  {

					for ( n = 1; n <= nmax; n++ )  {

					    dy = yp[n-1] - (int)(yp[n-1]);
					    if ( G_DIFF(dy,0.0F) )  {
						yy = (yp[n-1]-1.0F) * skip1 + 1.0F + *ioffy;
					    }
					    else  {
						if ( dy < 0.5F )  {
						    yy = ((int)(yp[n-1]+1.0F)-1.0F) * skip1 + 1.0F + *ioffy;
						    /*yy = yy - 1.0F + dy;*/
						}
						else  {
						    yy = ((int)(yp[n-1])-1.0F) * skip1 + 1.0F + *ioffy;
						    /*yy = yy + dy;*/
						}
					    }
					    yp[n-1] = yy;
					}
				    }
				}

				else {

				    if ( *ioffx != 0 || !G_DIFF(skip1,0.0F) )  {
					for ( n = 1; n <= nmax; n++ )  {
					    xp[n-1] = (xp[n-1]-1.0F) * skip1 + 1.0F + *ioffx;
					}
				    }

				    if ( *ioffy != 0 || !G_DIFF(skip1,0.0F) )  {
					for ( n = 1; n <= nmax; n++ )  {
					    yp[n-1] = (yp[n-1]-1.0F) * skip1 + 1.0F + *ioffy;
					}
				    }

				}

				/* SAVE CONTOUR */

/* printf(" Calling CNF_LNSV type,nmax,zc = %d %d  %d.\n", itype,nmax,(int)zc ); */
				cnf_lnsv ( &itype, &nmax, xp, yp, &zc, &ier );
/* printf(" Calling CNF_LNSV RETURNED\n"); */

			    }

			}

		    }

		}

	    }

	}

    }

    cnfedge ( grid, imz, jmz, *ioffx, *ioffy, skip1, xp, yp, &ier );

    sortie: ; /* [French, from feminine past participle of sortir, to go out, from Old French.] */

    for ( ii = 0; ii < MAXL; ii++ )  {
	G_FREE ( ms[ii], char );
    }
    G_FREE ( ms, char * );
    for ( ii = 0; ii < MAXL; ii++ )  {
	G_FREE ( l[ii], char );
    }
    G_FREE ( l, char * );
    G_FREE ( l1, char );
    G_FREE ( xp, float );
    G_FREE ( yp, float );

}

void	cnfedge ( float *grid, int imz, int jmz, int ioffx, int ioffy, float skip1, float *xp, float *yp, int *iret )
{
int	itype, ne, ix, iy, ier;
float	rmiss;
/* -------------------------------------------------------------------- */
    
    *iret = 0;

    itype = CNF_EDGE_TYPE;
    rmiss = RMISSD;
    ne = 0;

    ix = 1;
    iy = 1;

    /* Traverse up the left edge... */

    while ( iy < jmz )  {
	if ( Z(ix-1,iy-1) != RMISSD )  {
	    ne += 1;
	    xp[ne-1] = (ix-1.0) * skip1 + 1.0 + ioffx;
	    yp[ne-1] = (iy-1.0) * skip1 + 1.0 + ioffy;
	}
	else  {
	    if ( ne != 0 )  {
		cnf_lnsv ( &itype, &ne, xp, yp, &rmiss, &ier );
		ne = 0;
	    }
	}
	iy += 1;
    }

    /* Across the top... */

    while ( ix < imz )  {
	if ( Z(ix-1,iy-1) != RMISSD )  {
	    ne += 1;
	    xp[ne-1] = (ix-1.0F) * skip1 + 1.0F + ioffx;
	    yp[ne-1] = (iy-1.0F) * skip1 + 1.0F + ioffy;
	}
	else  {
	    if ( ne != 0 )  {
		cnf_lnsv ( &itype, &ne, xp, yp, &rmiss, &ier );
		ne = 0;
	    }
	}
	ix += 1;
    }

    /* Down the right edge... */

    while ( iy > 1 )  {
	if ( Z(ix-1,iy-1) != RMISSD )  {
	    ne += 1;
	    xp[ne-1] = (ix-1.0F) * skip1 + 1.0F + ioffx;
	    yp[ne-1] = (iy-1.0F) * skip1 + 1.0F + ioffy;
	}
	else  {
	    if ( ne != 0 )  {
		cnf_lnsv ( &itype, &ne, xp, yp, &rmiss, &ier );
		ne = 0;
	    }
	}
	iy -= 1;
    }

    /* Back across the bottom... */

    while ( ix >= 1 )  {
	if ( Z(ix-1,iy-1) != RMISSD )  {
	    ne += 1;
	    xp[ne-1] = (ix-1.0F) * skip1 + 1.0F + ioffx;
	    yp[ne-1] = (iy-1.0F) * skip1 + 1.0F + ioffy;
	}
	else  {
	    if ( ne != 0 )  {
		cnf_lnsv ( &itype, &ne, xp, yp, &rmiss, &ier );
		ne = 0;
	    }
	}
	ix -= 1;
    }

    if ( ne != 0 )  {
	cnf_lnsv ( &itype, &ne, xp, yp, &rmiss, &ier );
    }

}



void	cnfdrct ( float *grid, int imz, int jmz, float zc, int npts, float *xp, float *yp, int *iret )
{
int	found, rev, np, ixp, iyp, ichk, ier, ii;
float	dx, dy, zl, zr, xp1, yp1, xc, yc, area, tx, ty;
#define	M(X)	( X == RMISSD )
/* -------------------------------------------------------------------- */

    *iret = 0;
    rev = G_FALSE;

    /* Scan thru all points looking for a point that definitely *
     * indicates the direction...  */

    np = 1;
    found = G_FALSE;
    while ( np < npts && found == G_FALSE )  {

	ixp = (int)(xp[np-1]);
	iyp = (int)(yp[np-1]);
	dx = xp[np-1] - (int)(xp[np-1]);
	dy = yp[np-1] - (int)(yp[np-1]);

	zl = RMISSD;
	zr = RMISSD;
	if ( G_DIFF(dx,0.0F) && !G_DIFF(dy,0.0F) )  {

	    if ( xp[np] > xp[np-1] )  {
		zl = Z(ixp-1,(int)(yp[np-1]  ));
		zr = Z(ixp-1,(int)(yp[np-1]-1));
	    }
	    else if ( xp[np] < xp[np-1] )  {
		zr = Z(ixp-1,(int)(yp[np-1]  ));
		zl = Z(ixp-1,(int)(yp[np-1]-1));
	    }

	}
	else if ( !G_DIFF(dx,0.0F) && G_DIFF(dy,0.0F) )  {

	    if ( yp[np] > yp[np-1] )  {
		zl = Z((int)(xp[np-1]-1),iyp-1);
		zr = Z((int)(xp[np-1]  ),iyp-1);
	    }
	    else if ( yp[np] < yp[np-1] )  {
		zr = Z((int)(xp[np-1]-1),iyp-1);
		zl = Z((int)(xp[np-1]  ),iyp-1);
	    }
	}
	else if ( G_DIFF(dx,0.0F) && G_DIFF(dy,0.0F) )  {

	    xp1 = xp[np];
	    yp1 = yp[np];

	    if ( xp1 > ixp )  {
		if ( yp1 > iyp )  {
		    zl = Z(ixp-1,iyp  );
		    zr = Z(ixp  ,iyp-1);
		}
		else if ( yp1 < iyp )  {
		    zl = Z(ixp  ,iyp-1);
		    zr = Z(ixp-1,iyp-2);
		}
		else if ( G_DIFF(yp1,(float)iyp) )  {
		    zl = Z(ixp-1,iyp  );
		    zr = Z(ixp-1,iyp-2);
		}
	    }
	    else if ( xp1 < ixp )  {
		if ( yp1 > iyp )  {
		    zl = Z(ixp-2,iyp-1);
		    zr = Z(ixp-1,iyp  );
		}
		else if ( yp1 < iyp )  {
		    zl = Z(ixp-1,iyp-2);
		    zr = Z(ixp-2,iyp  );
		}
		else if ( G_DIFF(yp1,(float)iyp) )  {
		    zl = Z(ixp-1,iyp-2);
		    zr = Z(ixp-1,iyp  );
		}
	    }
	    else if ( G_DIFF(xp1,ixp) )  {
		if ( yp1 > iyp )  {
		    zl = Z(ixp-2,iyp-1);
		    zr = Z(ixp  ,iyp-1);
		}
		else if ( yp1 < iyp )  {
		    zl = Z(ixp  ,iyp-1);
		    zr = Z(ixp-2,iyp-1);
		}
		else if ( G_DIFF(yp1,(float)iyp) )  {
		    zl = RMISSD;
		    zr = RMISSD;
		}
	    }

	}

	ichk = iqrvrs ( zc, zl, zr );

	if ( ichk == 0 || ichk == 1 )  found = G_TRUE;
	if ( ichk == 1 )  rev = G_TRUE;

	np += 1;

    }

    if ( found == G_FALSE )  {

	if ( G_DIFF(xp[0],xp[npts-1]) && G_DIFF(yp[0],yp[npts-1]) )  {

	    /* If closed, check for zero area. */

	    cgr_centroid ( xp, yp, &npts, &xc, &yc, &area, &ier );
	    if ( !G_DIFF(area,0.0F) && area != RMISSD )  {
		printf ( "PROBLEM in CFDRCT - B1, area = %f\n", area );
	    }
	}

        *iret = -1;

        return;	

    }

    if ( rev == G_TRUE )  {
	for ( ii = 1; ii <= npts/2; ii++ )  {
	    tx = xp[ii-1];
	    ty = yp[ii-1];
	    xp[ii-1] = xp[npts-ii];
	    yp[ii-1] = yp[npts-ii];
	    xp[npts-ii] = tx;
	    yp[npts-ii] = ty;
	}
    }

}

int	iqrvrs ( float z, float zl, float zr )
{
int	iq;
/* -------------------------------------------------------------------- */
    if ( zl != RMISSD && zr != RMISSD )  {

	/* REVERSE... */

	iq = 1;
	if ( zl > z && z >= zr )  return ( iq );

	/* DON'T REVERSE... */

	iq = 0;
	if ( zl <= z && z < zr )  return ( iq );

	/* CAN'T TELL... */

	iq = 2;
	if ( zl >  z && zr >  z )  return ( iq );
	if ( zl <  z && zr <  z )  return ( iq );
	if ( zl == z && zr == z )  return ( iq );

    }
    else  {

	/* CAN'T TELL... */

	iq = 2;
	if ( zl == RMISSD && zr == RMISSD )  return ( iq );

	if ( zl == RMISSD )  {

	    /* REVERSE... */

	    iq = 1;
	    if ( z > zr )  return ( iq );

	    /* DON'T REVERSE... */

	    iq = 0;
	    if ( z < zr )  return ( iq );

	    /* CAN'T TELL... */

	    iq = 2;
	    return ( iq );

	}
	else  {
	     
	    /* REVERSE... */

	    iq = 1;
	    if ( z < zl )  return ( iq );

	    /* DON'T REVERSE... */

	    iq = 0;
	    if ( z > zl )  return ( iq );

	    /* CAN'T TELL... */

	    iq = 2;
	    return ( iq );

	}

    }

    return ( 2 );

}


void	cnfmiss ( float *grid, int imz, int jmz, float *xp, float *yp, int *nmax, int *iret )
{
int	insert, n, m;
float	dx, dy, deltax, deltay, xnew, ynew;
/* -------------------------------------------------------------------- */

    *iret = 0;

    for ( n = *nmax-1; n >= 1; n-- )  {

	dx = xp[n] - xp[n-1];
	dy = yp[n] - yp[n-1];
	deltax = xp[n-1] - (int)(xp[n-1]);
	deltay = yp[n-1] - (int)(yp[n-1]);
	xnew = xp[n];
	ynew = yp[n];
	insert = G_FALSE;
/* printf("%d %d %f %f %f %f %f %f %d\n", *nmax, n, dx, dy, deltax, deltay, xnew, ynew, insert ); */
	if ( G_ABS(dx) < 0.5F && G_ABS(dy) < 0.5F )  {
	    if ( dx > 0.0F && dy > 0.0F )  {
		if ( !G_DIFF(deltax,0.0F) &&
		    ! ERMISS(Z((int)(xp[n-1]-1),(int)(yp[n-1]))) )  {
		    insert = G_TRUE;
		    xnew = xp[n-1];
		}
		else if ( G_DIFF(deltax,0.0F) &&
		    ! ERMISS(Z((int)(xp[n-1]),(int)(yp[n-1]-1))) )  {
		    insert = G_TRUE;
		    ynew = yp[n-1];
		}
	    }
	    else if ( dx < 0.0F && dy < 0.0F )  {
		if ( !G_DIFF(deltax,0.0F) &&
		    ! ERMISS(Z((int)(xp[n-1]),(int)(yp[n-1]-2))) )  {
		    insert = G_TRUE;
		    xnew = xp[n-1];
		}
		else if ( G_DIFF(deltax,0.0F) &&
		    ! ERMISS(Z((int)(xp[n-1]-2),(int)(yp[n-1]))) )  {
		    insert = G_TRUE;
		    ynew = yp[n-1];
		}
	    }
	    else if ( dx > 0.0F && dy < 0.0F )  {
/* printf(": %f %f %f %d %d %f %s\n", dx, dy, deltax, (int)(xp[n-1]),(int)(yp[n-1]-1), Z((int)(xp[n-1]),(int)(yp[n-1]-1)), ERMISS(Z((int)(xp[n-1]),(int)(yp[n-1]-1))) ? "T" : "F" ); */
		if ( G_DIFF(deltax,0.0F) &&
		    ! ERMISS(Z((int)(xp[n-1]),(int)(yp[n-1]))) )  {
		    insert = G_TRUE;
		    ynew = yp[n-1];
		}
		else if ( !G_DIFF(deltax,0.0F) &&
		    ! ERMISS(Z((int)(xp[n-1]-1),(int)(yp[n-1]-2))) )  {
		    insert = G_TRUE;
		    xnew = xp[n-1];
		}
/* printf(": %f %f %f %f %s %d\n", dx, dy, deltax, Z((int)(xp[n-1]),(int)(yp[n-1]-1)), ERMISS(Z((int)(xp[n-1]),(int)(yp[n-1]-1))) ? "T" : "F", insert ); */
	    }
	    else if ( dx < 0.0F && dy > 0.0F )  {
		if ( !G_DIFF(deltax,0.0F) &&
		    ! ERMISS(Z((int)(xp[n-1]),(int)(yp[n-1]))) )  {
		    insert = G_TRUE;
		    xnew = xp[n-1];
		}
		else if ( G_DIFF(deltax,0.0F) &&
		    ! ERMISS(Z((int)(xp[n-1]-2),(int)(yp[n-1]-1))) )  {
		    insert = G_TRUE;
		    ynew = yp[n-1];
		}
	    }
	}

	if ( insert == G_TRUE )  {
/* printf("insert %f %f at n = %d, dx = %f, dy = %f, deltax = %f\n", xnew, ynew, n, dx, dy, deltax );*/

	    /* Prepare to add a point. */

	    *nmax = *nmax + 1;
	    for ( m = *nmax; m >= n+2; m-- )  {
		xp[m-1] = xp[m-2];
		yp[m-1] = yp[m-2];
	    }

	    xp[n] = xnew;
	    yp[n] = ynew;

	}

    }
}

void	t2r_xy ( int nin, int njn, float *grid, int imz, int jmz, float zc, float *xpn, float *ypn )
{
#define	EPS	1.E-15
int	iix, jjy;
float	x, y, xx, yy;
/* -------------------------------------------------------------------- */

    iix = ( nin + 1 ) / 2;
    jjy = njn;
    x = iix - 1;
    y = jmz - jjy;

    if ( nin%2 == 0 )  {

	xx = Z(iix,jjy-1) - Z(iix-1,jjy-1);

	if ( G_ABS(xx) > EPS )  {
	    x = x + (zc - Z(iix-1,jjy-1)) / xx;
	}
	else  {
	    x = x + 0.5;
	}

    }
    else  {

	yy = Z(iix-1,jjy) - Z(iix-1,jjy-1);

	if ( G_ABS(yy) > EPS )  {
	    y = y - (zc - Z(iix-1,jjy-1)) / yy;
	}
	else  {
	    y = y - 0.5;
	}
    }

    *xpn = x + 1.0F;
    *ypn = jmz - y;

}
int	lneqv ( float zcent, float zarr, float zc )
{
int	ltest1, ltest2;
/* -------------------------------------------------------------------- */

    ltest1 = ( zcent > zc ) ? G_TRUE : G_FALSE;
    ltest2 = ( zarr  > zc ) ? G_TRUE : G_FALSE;

    return ( ( ( ltest1==G_TRUE  && ltest2==G_FALSE ) || 
	       ( ltest1==G_FALSE && ltest2==G_TRUE  ) == G_TRUE ) 
	    ? G_TRUE 
	    : G_FALSE );
}
