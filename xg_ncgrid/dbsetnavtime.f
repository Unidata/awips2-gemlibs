        SUBROUTINE DB_SETNAVTIME  ( atime, iret )
C************************************************************************
C* DB_SETNAVTIME                                                        *
C*                                                                      *
C* This subroutine puts DB info into common.                            *
C*                                                                      *
C* DB_SETNAVTIME  ( ATIME, IRET )                                       *
C*                                                                      *
C* Input parameters:                                                    *
C*      ATIME		CHAR*		a time                          *
C*                                                                      *
C* Output parameters:                                                   *
C*      IRET            INTEGER         Return code                     *
C*                                        0 = normal return             *
C**                                                                     *
C* Log:                                                                 *
C* m.gamazaychikov/CWS  10/10                                           *
C************************************************************************
        INCLUDE         'GEMPRM.PRM'
        INCLUDE         'dbcmn.cmn'
C*
        CHARACTER*(*)   atime
C*
C------------------------------------------------------------------------
        CALL ST_RNUL (atime, atime, lens, ier)
        navtime = atime(:lens)
        isnavtime = .true.
        iret = 0
        RETURN
        END
