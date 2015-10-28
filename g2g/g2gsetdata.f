 	SUBROUTINE G2G_SETDATA( mlines, mlatlons, mlatPts, mlonPts,
     +                          msmthpts, msmthLat, msmthLon,
     +                          mextpts,  mextLat, mextLon,
     +		                mvaluein, msmth, mclosed, 
     +				mmnum, mmlat, mmlon,
     +             		mmfi, mmfj, mmvalue,
     +				iret )      
C************************************************************************
C* G2G_SETDATA                                                      	*
C*                                                                      *
C* This subroutine set line coordinates and values into the /GGLINE/	*
C* and /GGORIG/ common blocks.						* 
C*                             						*
C* Note: Lines in flat/flon is not used for further calculation.  Lines	*
C*       in origfi/origfj are only used to find the range of the	*
C*       original lines before extention. Lines in fi/fj are the REAL 	*
C*       lines used along with value, ismth, closed for actual G2G 	*
C*       calculation.							*
C*                             						*
C* G2G_SETDATA ( NPIN, FIIN, FJIN, VALUEIN, SMTH, CLSD, IRET )		*
C*                                                                      *
C* Input parameters:                                                    *
C*  MLINES	INTEGER	Number of contour lines				*
C*  MLATLONS(*) INTEGER	Number of points on each line			*
C*  MLATPTS(*)	REAL   	Latitudes of all points	(line-by-line)		*
C*  MLONPTS(*)	REAL   	Longitedes of all points (line-by-line)		*
C*  MSMTHPTS(*) INTEGER # of points on each line(smoothed in grid space)*
C*  MSMTHLAT(*)	REAL   	Y-Coordinates of points (smoothed in grid space)*
C*  MSMTHLON(*)	REAL   	X-Coordinates of points (smoothed in grid space)*
C*  MEXTPTS(*)  INTEGER # of points(smoothed/extended in grid space)	*
C*  MEXTLAT(*)	REAL   	Y-Coordinates of points (smoothed/extedned)	*
C*  MEXTLON(*)	REAL   	X-Coordinates of points (smoothed/extended)	*
C*  MVALUEIN(*) REAL	Value of the line				*
C*  MSMTH(*)	INTEGER Smooth factor of the line			*
C*  MCLOSED(*)	INTEGER Closeness of the line				*
C*                                                                      *
C* Output parameters:                                                   *
C*  IRET        INTEGER         Return code                     	*
C**                                                                     *
C* Log:                                                                 *
C* J. Wu/Chugach	03/10          Created                 		*
C************************************************************************
	INCLUDE		'GEMPRM.PRM'
	INCLUDE		'grphgd.cmn'
C
C------------------------------------------------------------------------
C
        REAL            mlatPts(*), mlonPts(*)
        REAL            msmthLat(*), msmthLon(*)
        REAL            mextLat(*), mextLon(*)
        REAL            mvaluein(*)
        INTEGER         mlines, mlatlons(*), msmthpts(*)
	INTEGER		mextpts(*), msmth(*)  
	INTEGER		mclosed(*), mpts
        INTEGER         mmnum
        REAL            mmlat(*), mmlon(*), mmvalue(*)
        REAL            mmfi(*), mmfj(*)
C------------------------------------------------------------------------
        iret = 0

C
C*      Set original Lat/lons to /GGLINE/flat, flon.
C
	nltlnlns = mlines;
C	
	mpts = 0
	DO  ii = 1, mlines

	    IF ( mlatlons( ii ) .ge. MAXPPL )  THEN
		nlatlons( ii ) = MAXPPL - 1
	    ELSE
		nlatlons( ii ) = mlatlons( ii )
	    END IF
	    	    
	    DO  jj = 1, nlatlons( ii )
                flat( jj, ii ) = mlatPts( jj + mpts ); 
                flon( jj, ii ) = mlonPts( jj + mpts ); 
	    END DO
            	    
	    mpts = mpts + mlatlons( ii )
           
	END DO
		
C
C*      Set smoothed lines in grid coords to 
C*         /GGORIG/origfi, origfj.
C
	noriglns = mlines;
C	
	mpts = 0
	DO  ii = 1, mlines

	    IF ( msmthpts( ii ) .ge. MAXPPL )  THEN
		orignpts( ii ) = MAXPPL - 1
	    ELSE
		orignpts( ii ) = msmthpts( ii )
	    END IF
	    	    
 	    DO  jj = 1, orignpts( ii )
                origfi( jj, ii ) = msmthLon( jj + mpts ); 
                origfj( jj, ii ) = msmthLat( jj + mpts ); 
	    END DO
           
	    mpts = mpts + msmthpts( ii )
           
	END DO
C
C*      Set smoothed/extended lines in grid coords to 
C*         /GGLINE/fi, fj.
C
	nlines = mlines;
C	
	mpts = 0
	DO  ii = 1, mlines
C
	    IF ( mextpts( ii ) .ge. MAXPPL )  THEN
		npts( ii ) = MAXPPL - 1
	    ELSE
		npts( ii ) = mextpts( ii )
	    END IF

	    DO  jj = 1, npts( ii )
                fi( jj, ii ) = mextLon( jj + mpts ); 
                fj( jj, ii ) = mextLat( jj + mpts ); 
	    END DO
C            	    
	    mpts = mpts + mextpts( ii )
           
	END DO
C
C*      Set line values to /GGLINE/value, ismth, closed
C
	DO  ii = 1, mlines
	   value( ii ) = mvaluein( ii )
           ismth( ii ) = msmth( ii )
           closed( ii ) = mclosed( ii )	  
	END DO        
C
C*      Set minimam/maximums to /GGMM/
C
C
	nmm = mmnum
	DO  ii = 1, nmm
	   flatmm( ii ) = mmlat( ii )
	   flonmm( ii ) = mmlon( ii )
	   fimm( ii ) = mmfi( ii )
	   fjmm( ii ) = mmfj( ii )
	   valuemm( ii ) = mmvalue( ii )
	END DO        
C	
	RETURN
	END
