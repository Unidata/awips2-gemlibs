	SUBROUTINE G2G_EDGEOPT ( kx, ky, iret )
C************************************************************************
C* G2G_EGDEOPT                                                    	*
C*                                                                      *
C* This subroutine specifies edge options for contour lines, using the	*
C* smoothed lines (not extended yet) saved in /GGORIG/origfi, origfj	*
C* and the bounds saved in /GGEOPT/ visib, bvalue, ddist,spec.		*
C*                                                                      *
C* Note: One or two 5-point (closed rectangle) bounds may be added to   *
C*       the end of the /GGLINE/fi, fj only and nlines is incremented	*
C*                                                                      *
C* G2G_EGDEOPT( KX, KY, IRET )						*
C*                                                                      *
C* Input parameters:                                                    *
C*      KX              INTEGER         Data grid x-dimension           *
C*      KY              INTEGER         Data grid y-dimension           *
C*                                                                      *
C* Output parameters:                                                   *
C*      IRET            INTEGER         Return code                     *
C*                                                                     	*
C**                                                                     *
C* Log:                                					*
C* J. Wu/Chugach	03/10          extracted from ggbndconds.f	*
C************************************************************************
	INCLUDE 	'GEMPRM.PRM'
	INCLUDE		'grphgd.cmn'

	INTEGER 	iret, kx, ky
C
	INTEGER         X_MN, X_MX, Y_MN, Y_MX
	LOGICAL		valid_bnd
C-----------------------------------------------------------------------
C
	iret = 0
C
C* 	If user specified boudnary conds and an open line is encountered, 
C*	warn the user and return.
C
	IF ( ddist(1) .ne. IMISSD )  THEN
	  DO  nl = 1, nlines
	    if ( closed(nl) .ne. CLSD ) THEN
		iret = -15
		RETURN
	    END IF
	  END DO
        END IF
C
C*	Check for valid boundary conditions.
C
	valid_bnd = .false.
	if ( (ddist(1) .gt. 0 .and. ddist(2) .gt. ddist(1)) .or.
     +	     (ddist(1) .gt. 0 .and. ddist(2) .lt. 0) .or.
     +       (ddist(2) .gt. 0 .and. ddist(1) .lt. 0) )
     +	    valid_bnd = .true.     
C
C* 	If countours extend beyond the grid edge, reset visib to true
C
	X_MN = kx
        X_MX = 1
        Y_MN = ky
        Y_MX = 1
	
C	DO  ii = 1, noriglns
C            WRITE(6,*) 'line ',ii,' has ',orignpts( ii ), 'pts'
C	    	    
C	    DO  jj = 1, orignpts( ii )
C                WRITE(6,*) 'pt ', jj, ' is ', origfi( jj, ii ),
C     +                     ', ', origfj( jj, ii )		
C	    END DO
           
C	END DO

    	DO  ii = 1, noriglns
            DO  jj = 1, orignpts( ii )
                X_MN = MIN ( X_MN, INT( origfi(jj, ii) ) )
                X_MX = MAX ( X_MX, INT( origfi(jj, ii) ) + 1 )
                Y_MN = MIN ( Y_MN, INT( origfj(jj, ii) )  )
                Y_MX = MAX ( Y_MX, INT( origfj(jj, ii) ) + 1 )
            END DO
        END DO

C
	IF ( valid_bnd .and. .not. visib ) THEN
	  IF ( X_MN .lt. 1 .or. Y_MN .lt. 1 .or.
     +	       X_MX .gt. kx .or. Y_MX .gt. ky ) THEN
	    visib = .true.
	    iret = 5
	  END IF
	END IF 

	IF ( spec .eq. 'e' ) THEN
	    X_MN = MIN (  1, X_MN )
	    X_MX = MAX ( kx, X_MX )
	    Y_MN = MIN (  1, Y_MN )
	    Y_MX = MAX ( ky, Y_MX )
	END IF
C	
C*    	Add bounding rectangles.
C
	IF ( ddist(1) .gt. 0 .and. ddist(2) .gt. ddist(1) ) THEN

	    DO ii = 1, 2
	      nlines = nlines + 1
	      npts(nlines) = 5
	      value(nlines) = bvalue(ii)
	      
	      fi( 1, nlines ) = X_MN - ddist(ii)
	      fj( 1, nlines ) = Y_MN - ddist(ii)	      
	      
	      fi( 2, nlines ) = X_MX + ddist(ii)
              fj( 2, nlines ) = Y_MN - ddist(ii)
	      
	      fi( 3, nlines ) = X_MX + ddist(ii)
              fj( 3, nlines ) = Y_MX + ddist(ii)
	      
	      fi( 4, nlines ) = X_MN - ddist(ii)
              fj( 4, nlines ) = Y_MX + ddist(ii)
	      
	      fi( 5, nlines ) = X_MN - ddist(ii)
              fj( 5, nlines ) = Y_MN - ddist(ii)
	      
	    END DO 
C*
	ELSE IF ( ddist(1) .gt. 0 .and. ddist(2) .lt. 0 .or.
     +              ddist(2) .gt. 0 .and. ddist(1) .lt. 0 ) THEN

	    IF ( ddist(1) .gt. 0 .and. ddist(2) .lt. 0 ) THEN
	      ij = 1
	    ELSE
	      ij = 2
	    END IF
C
	    nlines = nlines + 1
            npts(nlines) = 5
            value(nlines) = bvalue(ij)
	    
	    fi( 1, nlines ) = X_MN - ddist(ij);
	    fj( 1, nlines ) = Y_MN - ddist(ij);
	    
	    fi( 2, nlines ) = X_MX + ddist(ij)
            fj( 2, nlines ) = Y_MN - ddist(ij)
	      
	    fi( 3, nlines ) = X_MX + ddist(ij)
            fj( 3, nlines ) = Y_MX + ddist(ij)
	      
	    fi( 4, nlines ) = X_MN - ddist(ij)
            fj( 4, nlines ) = Y_MX + ddist(ij)
	      
	    fi( 5, nlines ) = X_MN - ddist(ij)
            fj( 5, nlines ) = Y_MN - ddist(ij)
	
	END IF
C	
C	IF ( nlines .gt. noriglns ) THEN
C            WRITE(6,*) nlines - noriglns, '  lines added at edge'
C	    DO  ii = noriglns+1, nlines
C                WRITE(6,*) 'line ', ii, ' has ', npts( ii ), 'pts'
C	    	    
C	        DO  jj = 1, npts( ii )
C                    WRITE(6,*) 'pt ', jj, ' is ', fi( jj, ii ),
C     +                     ', ', fj( jj, ii )		
C	        END DO
C            END DO
C	END IF
C        WRITE(6,*) 'EXIT G2G_EDGEOPT'
C*
	RETURN
	END
