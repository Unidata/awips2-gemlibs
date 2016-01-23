	SUBROUTINE G2G_CALC ( grid, kx, ky, hist, hstgrd, iret )
C************************************************************************
C* G2G_CALC                                                             *
C*                                                                      *
C* This subroutine does all the original GRPHGD calculation in grid	*
C* space. The following parameters should be set before calling this	*
C* subroutine:								*
C*									*
C* 1. User input from the GUI:						*
C*     hstgrd*(LLMXLN) 	 -   Toggle writing history grid (y/n)		*
C*     discrete*(LLMXLN) -  set via IN_DISCRETE( discrete, ier) 	*
C*     dlines*(LLMXLN)   - set via  IN_DLINES ( dlines, ier )		*
C*     gglimt*(LLMXLN)   - set via  GLIMIT ( gglimt, ier )		*
C*     edgeopts*(LLMXLN) - set via  GGEDGE ( edgeopts, ier )		*
C*									*
C* 2. Inputs pre-set in the common block (grphgd.cmn)			*
C*    									*
C*    a. Inputs from the original contour lines and converted into	*
C*       grid space: fi, fj, fimm, fjmm. 				*
C*	COMMON  / GGLINE /  nlines, npts, value, closed,		*
C*     		            rngi, rngj, fi, fj				*
C*      COMMON  / GGMM /  nmm, valuemm, fimm, fjmm			*
C*                                                                      *
C*      COMMON  / GGORIG / nltlnlns, noriglns, nlatlons, orignpts,	*
C*                         origfi, origfjnmm, 				*
C*                                                                      *
C*    b. Intersection points from g2g_findint.f, rngi, rngj		*
C*       								*
C*      COMMON  / GGINTR /  intdim, intsct, intinfo, intptrs		*
C*                                                                      *
C*    c. Options for grid edge						*
C*	COMMON	/ GGEOPT / visib, bvalue, ddist, spec			*
C*                                                                      *
C*    d. Limits for grid values						*
C*	COMMON  / GGLIMT /  lochk, loval, hichk, hival, defalt		*
C*									*
C* G2G_CALC ( GRID, KX, KY, HIST, IRET )				*
C*                                                                      *
C* Input parameters:                                                    *
C*      KX              INTEGER         Data grid x-dimension		*
C*      KY              INTEGER         Data grid y-dimension		*
C*      HSTGRD    	INTEGER         Toggle writing history grid(1/0)*
C*                                                                      *
C* Output parameters:                                                   *
C*      GRID(*)         REAL            Data grid			*
C*      HIST(*)         REAL            History data grid		*
C*      IRET            INTEGER         Return code                     *
C*					= >0  - misc warnings		*
C*					= -1  - misc error 		*
C*                                                                     	*
C**                                                                     *
C* Log:                                                                 *
C* J. Wu/Chugach	02/10	Extracted from ggdriv.f			*
C************************************************************************
	INCLUDE 	'GEMPRM.PRM'
	INCLUDE		'grphgd.cmn'

	INTEGER 	iret, ier, kx, ky, hstgrd
C
	REAL 		grid(*),  hist(*)
C
        LOGICAL         proces, hstflg
C
	REAL		grdmin, grdmax
	INTEGER		nmiss, ngpts
C-----------------------------------------------------------------------
C
        iret = 0
	proces = .true.
C
C*       Check to see if grid is too large.
C
        IF ( ( kx * ky ) .gt. LLMXTG )  THEN
            iret = +1
            WRITE(6,*) 'Number of grid points (', kx*ky,
     &                  ') exceeds maximum allowed (', LLMXTG, ')'
            proces = .false.
        END IF
C
C*       Check to see if grid dimensions are too big for
C*	 intersection arrays.
C
        IF ( ( kx + ky ) .gt. MAXDIM )  THEN
	    iret = +2
            WRITE(6,*) 'Total of grid dimensions (', kx+ky,
     & 			') exceeds maximum allowed (', MAXDIM, ')'
	    proces = .false.
        END IF
C
	IF ( proces )  THEN

	   hstflg = .true.
	   IF ( hstgrd .ne. 1 ) THEN
	       hstflg = .false.
	   END IF

C
C*	     Initialize history and data arrays
C
            imn = 1
	    imx = kx
	    jmn = 1
	    jmx = ky

	    IF ( hstflg )
     &	         WRITE(6,*) 'Total number of lines = ', nlines,
     &		    ' (', nint((nlines*100.0)/MAXLIN),'% of MAX ALLOWED)'
C
	    IF ( nlines .eq. 0 )  THEN
C
C*	         Set data array to default value.
C
               	DO  ij = 1, kx*ky
		    grid(ij) = defalt
		    hist(ij) = INIT
		END DO
C
		proces = .false.
C
	    END IF
C
        END IF

C
C*	NOW start calculation - discrete or continuous
C
C	Note: Bounds search is done outside of native code.
C
       IF ( proces ) THEN
C
C*          GRAPH-TO-GRID calculation begins......
C*
C*	    Check whether it is discrete or continuous
C
	   CALL IN_DISCQ ( istate, ier )
C
C*	    The following is for discrete case
C
	   IF ( istate .eq. 1 ) THEN
C
C*	        Perform DLINE SEARCH
C
	       CALL GDLINE ( imn, imx, jmn, jmx, grid, hist, kx, ky, ier )
	       IF ( hstflg )
     +	            WRITE (6,*) 'DLINE SEARCH COMPLETE, ier =', ier
C
C*	        Perform DLINE WEIGHTED SEARCH
C
	       CALL GDLWEI ( imn, imx, jmn, jmx, grid, hist, kx, ky, ier )
	       IF ( hstflg )
     +	            WRITE (6,*) 'DLINE WEIGHTED SEARCH DONE, ier =', ier
C
	    ELSE
C
C*	         The following is for continuous case
C
C*	         Perform RADIAL SEARCH
C
	       CALL GRADLS (imn, imx, jmn, jmx, grid, hist, kx, ky, ier)
	       IF ( hstflg )
     +	            WRITE (6,*) 'RADIAL SEARCH COMPLETE, ier =', ier
C
C*	         Perform LIKE-VALUE SEARCH
C
	        CALL GLVALU ( imn, imx, jmn, jmx, grid, hist, kx,
     +							ky, ier )
	       IF ( hstflg )
     +	            WRITE (6,*) 'LIKE-VALUE SEARCH COMPLETE, ier =', ier
C
C*	        Check the state of DLINES. If yes, add calls to
C*              GDLINE and GWEIGS
C
	       CALL IN_DLINQ ( istate, istateL, epsi, ier )
C
	       IF ( istate .eq. 1 ) THEN
C
C*	            Perform DLINE SEARCH
C
	          CALL GDLINE ( imn, imx, jmn, jmx, grid, hist, kx,
     +							ky, ier )
	          IF ( hstflg )
     +	               WRITE (6,*) 'DLINE SEARCH COMPLETE, ier =', ier
C
	       END IF
C
C*	        Perform WEIGHTED SEARCH
C
 	       CALL GWEIGS (imn, imx, jmn, jmx, grid, hist, kx, ky, ier)
 	       IF ( hstflg )
     +	           WRITE (6,*) 'WEIGHTED SEARCH COMPLETE, ier =', ier
C
C*	        Perform SMOOTHING
C
 	       CALL GSMOOT (imn, imx, jmn, jmx, grid, hist, kx, ky, ier)
 	       IF ( hstflg )
     +	           WRITE (6,*) 'SMOOTHING COMPLETE, ier =', ier
C
C*	        Apply limits, if any.
C
 	        CALL GGLIMS (imn, imx, jmn, jmx, grid, hist, kx, ky, ier)
 	        IF ( hstflg )
     +	          WRITE (6,*) 'LIMITS COMPLETE, ier =', ier
C
C*	      This is the end of continous case
C
	    ENDIF
C
C*	End of claculation
C
       ENDIF
C
C
C*	Print a summary of the results
C
       IF  ( hstflg .and. proces )  THEN
C
	    nmiss = 0
	    grdmin =  1.0E10
	    grdmax = -1.0E10
	    ngpts = (imx-imn+1)*(jmx-jmn+1)
	    DO  ii = 1, ngpts
		IF ( grid(ii) .ne. RMISSD )  THEN
		    grdmin = min(grdmin,grid(ii))
		    grdmax = max(grdmax,grid(ii))
		ELSE
		    nmiss = nmiss + 1
		END IF
	    END DO
C
	    WRITE(6,*) 'Total number of grid points = ', ngpts
	    WRITE(6,*) 'Grid minimum and maximum = ', grdmin, grdmax
	    WRITE(6,*) 'Number of missing values = ', nmiss
C
	END IF
C
	RETURN
C*
	END
