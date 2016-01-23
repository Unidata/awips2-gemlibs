/************************************************************************
 * proto_cnf.h                                                          *
 *                                                                      *
 * These are the public and private function prototypes used by the CNF library.    *
 *                                                                      *
 **                                                                     *
 * D.W.Plummer/NCEP      2/04   Created                                 *
 ***********************************************************************/

#ifndef PROTO_CNF
#define PROTO_CNF

/*
 * CNF PUBLIC prototypes
 */

void	cnf_init	( int 		*iret );

void 	cnf_comp 	( int 		*imz_in, 
			  int 		*jmz_in, 
			  float 	*grid,
			  int 		*ioffx, 
			  int 		*ioffy, 
			  int 		*iskip, 
			  int 		*iret );

void	cnf_done	( int 		*iret );

void	cnf_dump	( int 		*dumptype,
       			  int		*iret );

void	cnf_fill	( int 		*iret );

void	cnf_gcol	( float 	threshold,
       			  int		which,
       			  int		*color,
       			  int		*type,
			  int		*iret );

void 	cnf_ginx	( float         *value,
			  int		*index,
       			  int		*iret );

void 	cnf_gnfo 	( int 		*ioffx, 
			  int 		*ioffy, 
			  int 		*skip, 
			  int 		*iret );

void 	cnf_gval	( int 		*index,
			  float		*value,
       			  int		*iret );

void	cnf_lnsv	( int 		*type,
       			  int		*npts,
			  float 	*x,
			  float 	*y,
			  float		*value,
			  int		*ier );

void	cnf_nval	( int 		*nvals,
       			  int		*iret );

void	cnf_prep	( int 		*iret );

void	cnf_ssmf	( float 	*smooth,
       			  float		*filtln	);


void 	cnf_vals 	( int 		*nclvl, 
			  float 	*clvl, 
			  int 		*icolrs, 
			  int 		*lintyp, 
			  int 		*linwid, 
			  int 		*linlbl, 
			  int 		*nflvl, 
			  float 	*flvl, 
			  int 		*ifcolr, 
			  int 		*iftype, 
			  int 		*iret );

void	cnf_pedg	( int		*iret );

void	cnf_plin	( int		*iret );

void cnf_getnumcntrs ( float cval, int *numcntrs );
void cnf_getcntrinfo ( float cval, int *numpolys, int *numpoints );

void cnf_getcontour ( float cval, float *xcoord, float *ycoord,
                      int *numpolys, int *numpoints );

void cnf_getnumedges ( int *numedges );


void cnf_getedgeinfo ( int *numedges, int *numpoints );

void cnf_getedges ( float *xcoord, float *ycoord,
                     int *numedges, int *numpoints );

/*
 * CNF PRIVATE prototypes
 */

CNF_EDGEPT* 	cnf_new_edge_pt 	( float 	x, 
					  float 	y );

int 		cnf_compare_pt 		( CNF_POINT 	*pt0, 
					  CNF_POINT 	*pt1 );

int 		cnf_range_int 		( CNF_RANGE 	rng0, 
					  CNF_RANGE 	rng1 );

void		cnf_free_edges		( void );

int     	prep_AdjEdges 		( int 		type );
int     	prep_LinkEdges 		( void );
int     	prep_ExamineEdgePts 	( void );
int     	prep_LinkCntrs 		( void );
int     	prep_ConnectCntrs 	( void );



#endif
