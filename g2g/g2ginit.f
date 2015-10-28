 	SUBROUTINE G2G_INIT ( iret )
C************************************************************************
C* G2G_INIT                                                             *
C*                                                                      *
C* This subroutine initialize the common block in grphgd.cmn.		*
C* This subroutine should be called beofre touching anything in 	*
C* those common blocks.							*
C*                                                                  	*
C* GGINIT ( IRET )            						*
C*                                                                      *
C* Output parameters:                                                   *
C*      IRET            INTEGER         Return code                     *
C**                                                                     *
C* Log:                                                                 *
C* J. Wu/Chugach	02/10          Created                 		*
C************************************************************************
	INCLUDE		'GEMPRM.PRM'
	INCLUDE		'grphgd.cmn'
C
C------------------------------------------------------------------------
        iret = 0
C
C*     Initialize the number of lines in /GGLINE/ and /GGORIG/.
C
        nlines = 0
        noriglns = 0
        nltlnlns = 0
C
C*     Initialize the number of maximum/minimums in /GGMM/.
C
        nmm    = 0
C
C*     Initialize the number of intersection points in /GGINTR/.
C
	ntotint = 0
C
	RETURN
	END
