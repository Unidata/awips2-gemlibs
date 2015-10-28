	SUBROUTINE G2G_PARAM ( acatmap, adiscrete, adlines,
     +	 		       agglimt, aedgeopts, iret )
C************************************************************************
C* G2G_PARAM                                                          	*
C*                                                                      *
C* This subroutine takes all the original GRPHGD option parameters and	*
C* processes them for grapg-to-grid calculation.			*
C*                                                                      *
C* G2G_PARAM( ACATMAP, ADISCRETE, ADLINES, AGGLIMT, AEDGEOPTS, IRET )	*
C*                                                                      *
C* Input parameters:                                                    *
C*      ACATMAP(*)      CHARACTER       catmap input string		*
C*      ADISCRETE(*)    CHARACTER       discrete input string		*
C*      ADLINES(*)	CHARACTER       dlines input string		*
C*      AEDGEOPTS(*)    CHARACTER       edgeopts input string		*
C*      AGGLIMT(*)    	CHARACTER       gglimt input string		*
C*                                                                      *
C* Output parameters:                                                   *
C*      IRET            INTEGER         Return code                     *
C*					= >0  - misc warnings		*
C**                                                                     *
C* Log: 								*
C* J.Wu/Chugach		03/10		created				*
C* J.Wu/Chugach		06/10		moved bounds out of native code	*
C************************************************************************
	INCLUDE 	'GEMPRM.PRM'
	INCLUDE		'grphgd.cmn'

	INTEGER 	iret, ier
C
        CHARACTER       catmap*(LLMXLN), discrete*(LLMXLN), 
     +			dlines*(LLMXLN), 
     +			gglimt*(LLMXLN), edgeopts*(LLMXLN)

        CHARACTER       acatmap*(*), adiscrete*(*), adlines*(*),
     +                  agglimt*(*), aedgeopts*(*)
C
C*-----------------------------------------------------------------------
C
	iret = 0
C
C*	 Process CATMAP input
C
        CALL ST_NULL ( acatmap, catmap, lens, ier )
	CALL IN_CATMINP ( catmap, ier )
C
C*	 Process DISCRETE input.
C
	CALL ST_LDSP  ( adiscrete, discrete, lens, ier )
        CALL ST_NULL ( discrete, discrete, lens, ier )
	CALL IN_DISCRETE ( discrete, ier )
C
C*	 Process DLINES input.
C
	CALL ST_LDSP  ( adlines, dlines, lens, ier )
        CALL ST_NULL ( dlines, dlines, lens, ier )
	CALL IN_DLINES ( dlines, ier )
C
C*	 Process LIMITS input.
C
	CALL ST_LDSP  ( agglimt, gglimt, lens, ier )
        CALL ST_NULL ( gglimt, gglimt, lens, ier )
	CALL GLIMIT ( gglimt, ier )
C
C*	 Process EDGEOPTS input.
C
	CALL ST_LDSP  ( aedgeopts, edgeopts, lens, ier )
	CALL GGEDGE ( edgeopts, ier )
C
	RETURN
C*
	END
