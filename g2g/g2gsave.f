	SUBROUTINE G2G_SAVE ( grid, hist, hstgrd, agdfile, 
     +                      aproj, acpyfil, agdarea, aanlyss, 
     +                      akxky,amaxgrd, agparm, agdatim, 
     +                      agvcord, aglevel, iret )
C************************************************************************
C* G2G_SAVE                                                             *
C*                                                                      *
C* This subroutine writes a grid and into a GEMPAK grid file.		*
C*                                                                      *
C* G2G_SAVE ( GRID, HIST, HSTGRD, AGDFILE, APROJ, ACPYFIL, AGDAREA,	*
C*	      AANLYSS, AKXKY, AMAXGRD, AGPARM, AGDATTIM, AGVCORD, 	*
C*	      AGLEVEL, IRET )						*
C*                                                                      *
C* Input parameters:                                                    *
C*      GRID(*)         REAL            Data grid			*
C*      HIST(*)         REAL            History data grid		*
C*      AGDFILE(*)      CHARACTER       Output grid file name		*
C*      APROJ(*)        CHARACTER       Projection of the grid		*
C*      ACPYFIL(*)      CHARACTER       CPYFIL of the grid - not used	*
C*      AGDAREA(*)      CHARACTER       Graphic area of the grid	*
C*      AANLYSS*)       CHARACTER       Analysis bolck - not used	*
C*      AKXKY(*)      	CHARACTER       Dimension of the grid		*
C*      HSTGLD(*)       INTEGER       	Flag to write out hist data	*
C*      AMAXGRD(*)      CHARACTER       Number of mximum grids		*
C*      AGPARM(*)       CHARACTER       Parameter name of the grid	*
C*      AGDATIM(*)      CHARACTER       Gempak date and time		*
C*      AGVCORD(*)      CHARACTER       Vertical coodinate 		*
C*      AGLEVEL(*)      CHARACTER       Level of the grid		*
C*                                                                      *
C* Output parameters:                                                   *
C*      IRET            INTEGER         Return code                     *
C*					= >0  - misc warnings		*
C*					= -1  - misc error 		*
C*					= -8  - unable to create gd file*
C*					= -13 - grid write problem	*
C*                                                                     	*
C**                                                                     *
C* Log:                                                                 *
C* J. Wu/CWS		07/10   Extracted from ggdriv.f			*
C************************************************************************
	INCLUDE 	'GEMPRM.PRM'
	INCLUDE		'grphgd.cmn'

	INTEGER 	iret, ier, hstgrd
C
	REAL 		grid(*), hist(*)
C*
        CHARACTER       gdfile*(LLMXLN), proj*(LLMXLN), cpyfil*(LLMXLN),
     +                  gdarea*(LLMXLN), anlyss*(LLMXLN), kxky*(LLMXLN),
     +                  maxgrd*(LLMXLN), gparm*(LLMXLN),
     +                  gdatim*(LLMXLN), 
     +                  gvcord*(LLMXLN), glevel*(LLMXLN)
C
        CHARACTER       agdfile*(*), aproj*(*), acpyfil*(*),
     +                  agdarea*(*), aanlyss*(*), akxky*(*),
     +                  amaxgrd*(*), agparm*(*),
     +                  agdatim*(*), 
     +                  agvcord*(*), aglevel*(*)
C*
        CHARACTER       newfil*256
        LOGICAL         proces, exist, gottm
C
	INTEGER		ighdr(LLGDHD), level(2), ivcord
	CHARACTER	parm*12, gdattm(2)*20, vparm*(LLMXLN)
	LOGICAL		rewrit/.true./, hstflg
C
	CHARACTER	gpack*12/' '/
C*
C-----------------------------------------------------------------------
C
	iret = 0
	proces = .true.
C
C*	Process input parameters
C	
        CALL ST_NULL ( agdfile, gdfile, lens, ier )
        CALL ST_NULL ( aproj, proj, lens, ier )
        CALL ST_NULL ( acpyfil, cpyfil, lens, ier )
        CALL ST_NULL ( agdarea, gdarea, lens, ier )
        CALL ST_NULL ( aanlyss, anlyss, lens, ier )
        CALL ST_NULL ( akxky, kxky, lens, ier )
        CALL ST_NULL ( amaxgrd, maxgrd, lens, ier )
        CALL ST_NULL ( agparm, gparm, lens, ier )
        CALL ST_NULL ( agdatim, gdatim, lens, ier )
        CALL ST_NULL ( agvcord, gvcord, lens, ier )
        CALL ST_NULL ( aglevel, glevel, lens, ier )
	
	cpyfil = ' '
	anlyss = ' '

C	
C*	Initialize IN and GD library
C
	CALL IN_BDTA ( ier )
	CALL GD_INIT( ier )

C*	Initialize DG library
C*
C* 	DG_INTL - initializes the grid diagnostics package common
C* 	blocks.  This is a one-time initialization.  DG_INTL must be	
C* 	called at the beginning of any program that uses grid diagnostic
C* 	functions.		

	CALL DG_INTL ( ier )
C
C*      Create the grid file
C* 
C*	FL_INQR - determines whether a file exists.
C*
C*	GDGCFL - creates a GEMPAK grid file if the user input 
C*	for 'gdoutf' does not exist.
C
	WRITE (6,*) 'Before RETURN from FL_INDR =', ier
	CALL FL_INQR ( gdfile, exist, newfil, ier )
	WRITE (6,*) 'RETURN from FL_INDR =', ier

	IF ( .not. exist ) THEN
	    WRITE (6,*) 'FILE not exist'
	    CALL GDGCFL ( gdfile, proj, gdarea, kxky, maxgrd, cpyfil,
     +                    anlyss, ier )
	    WRITE (6,*) 'RETURN from GDGCFL =', ier
	    IF ( ier .ne. 0 ) THEN
		CALL ER_WMSG  ( 'GDCFIL', ier, ' ', ier1 )
	        iret = -8
	        CALL ER_WMSG  ( 'GRPHGD', iret, gdfile, ier )
	        RETURN
	    END IF
	END IF
C
C*      Process the GDFILE input
C*
C*	DG_NFIL - This subroutine opens grid files and initializes the grid
C*	diagnostics package.	
C
	IF ( proces ) THEN
            CALL DG_NFIL ( gdfile, gdfile, ier )
            IF ( ier .ne. 0 ) THEN
                CALL ER_WMSG ( 'DG', ier, ' ', irr )
                proces = .false.
            END IF
	END IF
C
C*      Process the GDATTIM input
C*
C*	DG_NDTM - scans the user input for GDATTIM and creates an 
C* 	internal list of times to process. DG_NFIL must be called first to
C* 	set either a template for the first GDFILE entry or to open the file
C* 	associated with it. DG_NFIL also determines first and last times
C* 	associated with the first GDFILE entry. The information from DG_NFIL
C* 	is in DGCMN.CMN							
C*									
C* 	All indeterminant time substitutions are based on the times		
C* 	associated with the first GDFILE entry.				
C
	IF ( proces ) THEN
            CALL DG_NDTM ( gdatim, ier )
            IF ( ier .ne. 0 ) THEN
                CALL ER_WMSG ( 'DG', ier, gdatim, irr )
                proces = .false.
            END IF
	END IF
C
C*      Get the next time to process.
C
	IF ( proces ) THEN
            CALL DG_NTIM ( .true., .false., gdattm, gottm, ier )
            proces = ( ier .eq. 0 .and. gottm )
	END IF
C
	DO ii = 1, LLGDHD
	    ighdr ( ii ) = 0
	END DO
C
	IF ( proces )  THEN
C	   
	   hstflg = .true.           
	   IF ( hstgrd .ne. 1 ) THEN
	       hstflg = .false.
	   END IF
C
C*	    Get level value, vertical coord
C
	    CALL ST_ILST ( glevel, ':', -1, 2, level, num, ier )
	    CALL ST_LCUC ( gvcord, gvcord, ier )
	    CALL LV_CORD ( gvcord, vparm, ivcord, ier )
C
C*	     Write computed grid and history grid to grid file.
C
	    parm = gparm(:12)
            CALL DG_NWDT ( grid, gdattm, level, ivcord, parm,
     +                     ighdr, gpack, rewrit, ier )
	    IF ( hstflg )  WRITE (6,*) 'RETURN from DG_NWDT =', ier
	    IF  ( ier .ne. 0 )  THEN
	    	CALL ER_WMSG ( 'DG', ier, ' ', ier )
		iret = -13
	    	RETURN
	    END IF
	    proces = ier .eq. 0
C
C*	     Figure whether history grid needs to be written.
C
            IF  ( hstflg .and. proces )  THEN
C
	        parm = 'hist' // gparm(:8)
                CALL DG_NWDT ( hist, gdattm, level, ivcord, parm,
     +                         ighdr, gpack, rewrit, ier )
	        IF ( hstflg )  WRITE(6,*) 'RETURN from DG_NWDT =', ier
		IF  ( ier .ne. 0 )  THEN
	    	    CALL ER_WMSG ( 'DG', ier, ' ', ier )
		    iret = -13
	    	    RETURN
		END IF
C
	    END IF
C
	END IF
C
	CALL DG_NEND ( ier )
C
	RETURN
C*
	END
