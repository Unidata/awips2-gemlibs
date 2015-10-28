	SUBROUTINE G2G_FINDINT ( grid, hist, kx, ky, iret )
C************************************************************************
C* G2G_FINDINT                                                  	*
C*                                                                      *
C* This subroutine calculates the intersection points of the contour 	*
C* lines with the data grid in the graph-to-grid algorithm. 		*
C*                                                                      *
C* Note: 1. Smoothing/extending lines is done outside this subroutine	*
C*          since it involves coordinate transformation.  The results	*
C*          are set into /GGLINE/npts, fi, fj for use here.		*
C*          								*
C*       2. Wraping is NOT considered for now (see cgrrange.c).		*
C*                                                                	*
C* G2G_FIDINT ( GRID, HIST, KX, KY, IRET )				*
C*                                                                      *
C* Input parameters:                                                    *
C*      KX              INTEGER         Number of grid points in X      *
C*      KY              INTEGER         Number of grid points in Y      *
C*                                                                      *
C* Input and Output parameters:                                         *
C*      HIST (KX,KY)    REAL            Grid to keep history            *
C*                                                                      *
C* Output parameters:                                                   *
C*      GRID (KX,KY)    REAL            Grid to calculate point values  *
C*      IRET            INTEGER         Return code                     *
C**                                                                     *
C* Log:                                                                 *
C* J. Wu/Chugach     02/10	Extracted from gggint.f to avoid GTRANS *
C************************************************************************
        INCLUDE         'GEMPRM.PRM'
	INCLUDE		'grphgd.cmn'
C
	REAL		grid(kx,ky), hist(kx,ky)
C
	REAL            xin2(2), yin2(2)
C
	REAL		xints(MAXINT), yints(MAXINT)
C
	INTEGER		X_MN, X_MX, Y_MN, Y_MX
	LOGICAL		INGRID
C
	SCALE ( x ) = ( NINT( x * 1000.0 ) / 1000.0 )
C------------------------------------------------------------------------
	iret = NORMAL
C
C*	Initialize the intersection arrays
C
	ntotint = 0
C	
	DO  ii = 1, MAXINT
	    intsct(ii,INT_X) = RMISSD
	    intsct(ii,INT_Y) = RMISSD
	    intinfo(ii,INT_DRCT) = IMISSD
	    intinfo(ii,INT_INDX) = IMISSD
	    intinfo(ii,INT_LINE) = IMISSD
	END DO
C	
	DO  j = 1, 4
	    DO  i = 1, MAXDIM
		intptrs(i,j,NINTS) = 0
		intptrs(i,j,STPTR) = IMISSD
	    END DO
	END DO
C	
C
C*	SCALE minima and maxima
C
	DO  n = 1, nmm
	    fimm(n) = SCALE( fimm(n) )
	    fjmm(n) = SCALE( fjmm(n) )
	END DO
 	  
C	WRITE(6,*) 'Check input in CAVE Grid coordinate'
C	WRITE(6,*) 'Line(flonmm, flatmm) => (fimm, fjmm) - '
C	DO  ii = 1, nmm
C	   WRITE(6,*) '(', flonmm(ii), ',', flatmm(ii), ')=>(',
C     +                fimm(ii), ',', fjmm(ii), ') - ', 
C     +			valuemm( ii )    
C	END DO

C
C*	Find the range box for the original contour lines.
C	
	X_MN = 1
	X_MX = kx
	Y_MN = 1
	Y_MX = ky
C    
	IF ( visib ) THEN
	  DO  nn = 1, noriglns
	    DO  kk = 1, orignpts(nn)
	        X_MN = MIN ( X_MN, INT(origfi(kk, nn)) )
	        X_MX = MAX ( X_MX, INT(origfi(kk, nn)) )
	        Y_MN = MIN ( Y_MN, INT(origfj(kk, nn)) )
	        Y_MX = MAX ( Y_MX, INT(origfj(kk, nn)) )
	    END DO
	  END DO
C	  
	  X_MN = X_MN - 2
	  X_MX = X_MX + 2
	  Y_MN = Y_MN - 2
	  Y_MX = Y_MX + 2
       END IF
       
C
C*      Calculate intersection points for each line.
C
	DO  nl = 1, nlines
	 
C*	   Find the range for the smoothed/extended lines 
C*         in G (grid) coordinates.. 
C
	  rngi(nl,1) = fi(1,nl);
	  rngi(nl,2) = fi(1,nl);
	  rngj(nl,1) = fj(1,nl);
	  rngj(nl,2) = fj(1,nl);
	  DO  np = 2, npts(nl)
             IF ( rngi(nl, 1) .gt. fi(np,nl) )
     +           rngi(nl,1) = fi(np,nl)
             IF ( rngi(nl, 2) .lt. fi(np,nl) )
     +           rngi(nl,2) = fi(np,nl)
             IF ( rngj(nl, 1) .gt. fj(np,nl) )
     +           rngj(nl,1) = fj(np,nl)
             IF ( rngj(nl, 2) .lt. fj(np,nl) )
     +           rngj(nl,2) = fj(np,nl)           	     
	  END DO         	  
	  
C
C*	   Adjust line coodinates to 2 decimal places (this allows
C*	   LNSGIN to perform its calculations more definitively); 
C
	  DO  np = 1, npts(nl)
             fi( np, nl ) = SCALE( fi(np, nl) )
	     fj( np, nl ) = SCALE( fj(np, nl) )
	  END DO         	  
C	  
C	   Check input.............
C
C 	  WRITE(6,*) 'Check input in CAVE Grid coordinate'
C	  DO  ii = 1, nlines 
C	      WRITE(6,*) 'Line(flon, flat) => (fi, fj) - ', nl 
C	      
C	      IF ( nlines > noriglns ) THEN
C	        DO  jj = 1, npts(nl) 
C	          WRITE(6,*) fi(jj,nl), ',', fj(jj,nl)     
C	        END DO
C             ELSE 
C	        DO  jj = 1, npts(nl) 
C	            WRITE(6,*) flon(jj,nl), ',', flat(jj,nl), '=>',
C     +                         fi(jj,nl), ',', fj(jj,nl)     
C	        END DO
C	      
C	      END IF	       
C	  END DO

C	 
C*	   
C*         Calculate intersections now.......
C*	 
C*	   NOTE: We do not consider wrapping for now, so just 
C*         hard-coded "iwrap", "dx" to simplify the coding......
C
	  iwrap = 0
	  dx = 0
	  
	  DO  iw = 0, iwrap
C
C*	     First, intersect with columns
C            
	    intdim(1) = kx 
C
	    yin2(1) = 1
	    yin2(2) = ky
	    DO  ix = 1, kx 
C
	        xin2(1) = ix + iw*dx
		xin2(2) = ix + iw*dx
C
		IF ( rngj(nl,2) .lt. yin2(1) .or. 
     +		     rngj(nl,1) .gt. yin2(2) .or.
     +		     rngi(nl,1) .gt. xin2(1) .or. 
     +		     rngi(nl,2) .lt. xin2(1) )  THEN
 		ELSE
C
		    CALL LNSGIN ( xin2, yin2, fi(1,nl), 
     +				fj(1,nl), npts(nl), MAXINT, nmint, 
     +				xints, yints, iret )
C		      CALL ER_WMSG ( 'GRPHGD', iret, ' ', ier )
C
		    IF ( nmint .gt. 0 )  THEN
C
		       DO  nmi = 1, nmint
C
                          iy = yints(nmi)
			  IF ( ix .ge. X_MN .and. ix .le. X_MX .and.
     +			       iy .ge. Y_MN .and. iy .le. Y_MX )  THEN
C			      
			     IF ( ntotint+1 .le. MAXINT )  THEN
			        ntotint = ntotint + 1
			        intsct(ntotint,INT_X) = xints(nmi)-iw*dx
			        intsct(ntotint,INT_Y) = yints(nmi)
			        intinfo(ntotint,INT_DRCT) = 1
			        intinfo(ntotint,INT_INDX) = ix
			        intinfo(ntotint,INT_LINE) = nl
C
                                IF ( INGRID(ix,iy,kx,ky) .and.
     +				     iy .eq. yints(nmi) .and.
     +                              hist(ix,iy) .eq. INIT )  THEN
                                   hist(ix,iy) = EXACT
                                   grid(ix,iy) = value(nl)
                                END IF
                             ELSE
C			           CALL ER_WMSG ( 'GRPHGD', +3, 'Columns', ier )
                             END IF
                          END IF

		       END DO
C    
		    END IF
C
		END IF
C
	    END DO
C
C*	     Second, intersect with rows
C
	    intdim(2) = ky 
C
	    xin2(1) = 1 + iw*dx
	    xin2(2) = kx + iw*dx
	    DO  iy = 1, ky 
C
	        yin2(1) = iy
	        yin2(2) = iy
C
		IF ( rngj(nl,2) .lt. yin2(1) .or. 
     +		     rngj(nl,1) .gt. yin2(1) .or.
     +		     rngi(nl,1) .gt. xin2(2) .or. 
     +		     rngi(nl,2) .lt. xin2(1) )  THEN
		ELSE
C
		    CALL LNSGIN ( xin2, yin2, fi(1,nl), 
     +				  fj(1,nl), npts(nl), MAXINT, nmint, 
     +				  xints, yints, iret )
C
		    IF ( nmint .gt. 0 )  THEN
C
		       DO  nmi = 1, nmint
C
                          ix = xints(nmi)-iw*dx
			  IF ( ix .ge. X_MN .and. ix .le. X_MX .and.
     +			        iy .ge. Y_MN .and. iy .le. Y_MX )  THEN
C			     
			     IF ( ntotint+1 .le. MAXINT )  THEN
C			     
			        ntotint = ntotint + 1
			        intsct(ntotint,INT_X) = xints(nmi)-iw*dx
			        intsct(ntotint,INT_Y) = yints(nmi)
			        intinfo(ntotint,INT_DRCT) = 2
			        intinfo(ntotint,INT_INDX) = iy
			        intinfo(ntotint,INT_LINE) = nl
C
                                IF ( INGRID(ix,iy,kx,ky) .and. 
     +				     ix .eq. xints(nmi) .and.
     +                               hist(ix,iy) .eq. INIT )  THEN
                                    hist(ix,iy) = EXACT
                                    grid(ix,iy) = value(nl)
                                END IF
                             ELSE
C			           CALL ER_WMSG ( 'GRPHGD', +3, 'Rows', ier )
                             END IF
                          END IF
C
		       END DO
C    
		    END IF
C
		END IF
C
	    END DO
C
C*	     Third, intersect with positive diagonal
C
	    i3 = 0
C
	    xin2(1) = 0 + iw*dx
	    yin2(2) = ky + 1
C
	    DO  iy = ky, 1, -1
C
		i3 = i3 + 1
		yin2(1) = iy - 1
		xin2(2) = ky - iy + 2 + iw*dx
C
		IF ( rngj(nl,2) .lt. yin2(1) .or. 
     +		     rngj(nl,1) .gt. yin2(2) .or.
     +		     rngi(nl,1) .gt. xin2(2) .or. 
     +		     rngi(nl,2) .lt. xin2(1) )  THEN
		ELSE
C
		    CALL LNSGIN ( xin2, yin2, fi(1,nl), 
     +				  fj(1,nl), npts(nl), MAXINT, nmint, 
     +				  xints, yints, iret )
C
		    IF ( nmint .gt. 0 )  THEN
C
		       DO  nmi = 1, nmint
C
			  ixx = xints(nmi)-iw*dx
			  iyy = yints(nmi)
			  IF ( ixx .ge. 1 .and. ixx .le. kx .and.
     +				 iyy .ge. 1 .and. iyy .le. ky )  THEN
C     
			     IF ( ntotint+1 .le. MAXINT )  THEN
C			     
			        ntotint = ntotint + 1
			        intsct(ntotint,INT_X) = xints(nmi)-iw*dx
			        intsct(ntotint,INT_Y) = yints(nmi)
			        intinfo(ntotint,INT_DRCT) = 3
			        intinfo(ntotint,INT_INDX) = i3
			        intinfo(ntotint,INT_LINE) = nl
C
			        IF ( INGRID(ixx,iyy,kx,ky) .and. 
     +				     ixx .eq. xints(nmi) .and. 
     +				     iyy .eq. yints(nmi) .and.
     +				     hist(ixx,iyy) .eq. INIT )  THEN
				   hist(ixx,iyy) = EXACT
				   grid(ixx,iyy) = value(nl)
			        END IF
                             ELSE
C			           CALL ER_WMSG ( 'GRPHGD', +3, 'Pos Diags (1)', ier )
                             END IF
			  END IF
C
		      END DO
C    
		    END IF
C
	        END IF
C
	    END DO
C
	    yin2(1) = 0
	    xin2(2) = kx + 1 + iw*dx
C
	    DO  ix = 2, kx 
C
	        i3 = i3 + 1
	        xin2(1) = ix - 1 + iw*dx
	        yin2(2) = kx - ix + 2
C
		IF ( rngj(nl,2) .lt. yin2(1) .or. 
     +		     rngj(nl,1) .gt. yin2(2) .or.
     +		     rngi(nl,1) .gt. xin2(2) .or. 
     +		     rngi(nl,2) .lt. xin2(1) )  THEN
		ELSE
C
		    CALL LNSGIN ( xin2, yin2, fi(1,nl), 
     +				  fj(1,nl), npts(nl), MAXINT, nmint, 
     +				  xints, yints, iret )
C
		    IF ( nmint .gt. 0 )  THEN
C
		       DO  nmi = 1, nmint
C
			  ixx = xints(nmi)-iw*dx
			  iyy = yints(nmi)
			  IF ( ixx .ge. 1 .and. ixx .le. kx .and.
     +				iyy .ge. 1 .and. iyy .le. ky )  THEN
C     
			     IF ( ntotint+1 .le. MAXINT )  THEN
C			     
			        ntotint = ntotint + 1
			        intsct(ntotint,INT_X) = xints(nmi)-iw*dx
			        intsct(ntotint,INT_Y) = yints(nmi)
			        intinfo(ntotint,INT_DRCT) = 3
			        intinfo(ntotint,INT_INDX) = i3
			        intinfo(ntotint,INT_LINE) = nl
C
			        IF ( INGRID(ixx,iyy,kx,ky) .and. 
     +  			     ixx .eq. xints(nmi) .and. 
     +				     iyy .eq. yints(nmi) .and.
     +				     hist(ixx,iyy) .eq. INIT )  THEN
				    hist(ixx,iyy) = EXACT
				    grid(ixx,iyy) = value(nl)
			        END IF
                             ELSE
C			           CALL ER_WMSG ( 'GRPHGD', +3, 'Pos Diags (2)', ier )
                             END IF
			  END IF
C
		       END DO
C    
		    END IF
C
		END IF
C
	    END DO
C
	    intdim(3) = i3
C
C*	     Fourth, intersect with negative diagonal
C
	    i4 = 0
C
	    yin2(1) = 0
	    xin2(2) = 0 + iw*dx
C
	    DO  ix = 1, kx 
C
		i4 = i4 + 1
		xin2(1) = ix + 1 + iw*dx
		yin2(2) = ix + 1
C
		IF ( rngj(nl,2) .lt. yin2(1) .or. 
     +		     rngj(nl,1) .gt. yin2(2) .or.
     +		     rngi(nl,1) .gt. xin2(1) .or. 
     +		     rngi(nl,2) .lt. xin2(2) )  THEN
		ELSE
C
		    CALL LNSGIN ( xin2, yin2, fi(1,nl), 
     +				  fj(1,nl), npts(nl), MAXINT, nmint, 
     +				  xints, yints, iret )
C
		    IF ( nmint .gt. 0 )  THEN
C
		       DO  nmi = 1, nmint
C
			  ixx = xints(nmi)-iw*dx
			  iyy = yints(nmi)
			  IF ( ixx .ge. 1 .and. ixx .le. kx .and.
     +			        iyy .ge. 1 .and. iyy .le. ky )  THEN
C
			     IF ( ntotint+1 .le. MAXINT )  THEN
C			     
			        ntotint = ntotint + 1
			        intsct(ntotint,INT_X) = xints(nmi)-iw*dx
			        intsct(ntotint,INT_Y) = yints(nmi)
			        intinfo(ntotint,INT_DRCT) = 4
			        intinfo(ntotint,INT_INDX) = i4
			        intinfo(ntotint,INT_LINE) = nl
C
			        IF ( INGRID(ixx,iyy,kx,ky) .and. 
     + 				     ixx .eq. xints(nmi) .and. 
     +				     iyy .eq. yints(nmi) .and.
     +				     hist(ixx,iyy) .eq. INIT )  THEN
				    hist(ixx,iyy) = EXACT
				    grid(ixx,iyy) = value(nl)
			        END IF
                             ELSE
C			           CALL ER_WMSG ( 'GRPHGD', +3, 'Neg Diags (1)', ier )
                             END IF
			  END IF
C
		       END DO
C    
		    END IF
C
		END IF
C
	    END DO
C
	    xin2(1) = kx + 1 + iw*dx
	    yin2(2) = ky + 1
C
	    DO  iy = 2, ky 
C
		i4 = i4 + 1
		yin2(1) = iy - 1
		xin2(2) = kx - ky + iy - 1 + iw*dx
C
		IF ( rngj(nl,2) .lt. yin2(1) .or. 
     +		     rngj(nl,1) .gt. yin2(2) .or.
     +		     rngi(nl,1) .gt. xin2(1) .or. 
     +		     rngi(nl,2) .lt. xin2(2) )  THEN
		ELSE
C
		    CALL LNSGIN ( xin2, yin2, fi(1,nl), 
     +				  fj(1,nl), npts(nl), MAXINT, nmint, 
     +				  xints, yints, iret )
C
		    IF ( nmint .gt. 0 )  THEN
C
		       DO  nmi = 1, nmint
C
			  ixx = xints(nmi)-iw*dx
			  iyy = yints(nmi)
C			    
			  IF ( ixx .ge. 1 .and. ixx .le. kx .and.
     +				 iyy .ge. 1 .and. iyy .le. ky )  THEN
C     
			     IF ( ntotint+1 .le. MAXINT )  THEN
			        ntotint = ntotint + 1
			        intsct(ntotint,INT_X) = xints(nmi)-iw*dx
			        intsct(ntotint,INT_Y) = yints(nmi)
			        intinfo(ntotint,INT_DRCT) = 4
			        intinfo(ntotint,INT_INDX) = i4
			        intinfo(ntotint,INT_LINE) = nl
C
			        IF ( INGRID(ixx,iyy,kx,ky) .and.
     + 				     ixx .eq. xints(nmi) .and. 
     +				     iyy .eq. yints(nmi) .and.
     +				     hist(ixx,iyy) .eq. INIT )  THEN
C     
				    hist(ixx,iyy) = EXACT
				    grid(ixx,iyy) = value(nl)
			        END IF
                             ELSE
C			           CALL ER_WMSG ( 'GRPHGD', +3, 'Neg Diags (1)', ier )
                             END IF
			  END IF
C
		       END DO
C    
		    END IF
C
	        END IF
C
	    END DO
C
	    intdim(4) = i4
C
	  END DO
C
	END DO
C
C*	Sort the intptrs arrays.
C
	CALL GGSORT ( ntotint, intsct(1,INT_X), intsct(1,INT_Y),
     &                intinfo(1,1), intinfo(1,2), intinfo(1,3), ier )
        ndup = ier
C
C*	Fill in the intptrs array which contains the number of ints per
C*	row/col/diagonal as well as where they start in the intinfo array.
C
	nptr = 1
     	DO  ndir = 1, 4
	    DO  ndim = 1, intdim(ndir)
C
	        IF ( intinfo(nptr,INT_DRCT) .eq. ndir .and. 
     &	             intinfo(nptr,INT_INDX) .eq. ndim )  THEN
C	            
		    intptrs(ndim,ndir,STPTR) = nptr
C		    
	            DO WHILE ( intinfo(nptr,INT_DRCT) .eq. ndir .and.
     &                         intinfo(nptr,INT_INDX) .eq. ndim )
	                nptr = nptr + 1
	                intptrs(ndim,ndir,NINTS) = 
     &			        intptrs(ndim,ndir,NINTS) + 1
	            END DO
	        END IF
	    END DO
	END DO
C
C*	 Print out intersection information.
C
        WRITE (6,*)'INTERSECTIONS CALCULATION COMPLETE'
	numints = 0
	DO  jj = 1, 4
	    ntot = 0
            DO  ii = 1, intdim(jj)
		DO  nn = 1, intptrs(ii,jj,1)
		    ntot = ntot + 1
		    numints = numints + 1
		END DO
	    END DO
C		  
	    WRITE(6,*) 'Total number of ints in dir ',jj,
     &			' = ',ntot
	END DO
C
	WRITE(6,*)'TOTAL NUMBER of INTERSECTIONS = ', numints,
     &		'  (',nint((numints*100.0)/MAXINT),'% of MAX ALLOWED)'

	RETURN
	END
C
C*	Local function to check if a grid point is within the grid
C*      range.
C
	LOGICAL FUNCTION INGRID ( ix, iy, kx, ky )
	INGRID = .false.
	IF ( ix .ge. 1 .and. ix .le. kx .and.
     +	     iy .ge. 1 .and. iy .le. ky )  THEN
	     INGRID = .true.
	END IF
	RETURN
	END
