        SUBROUTINE DB_SETEVTNAME  ( aname, iret )
C************************************************************************
C* DB_SETEVTNAME                                                        *
C*                                                                      *
C* This subroutine puts DB debug into common.                            *
C*                                                                      *
C* DB_SETEVTNAME  ( ANAME, IRET )                                       *
C*                                                                      *
C* Input parameters:                                                    *
C*      ANAME		CHAR*		a name                          *
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
        CHARACTER*(*)   aname
C*
C------------------------------------------------------------------------
C*

        CALL ST_RNUL (aname, aname, lens, ier)
        evtname = aname(:lens)
        isevtname = .true.
        iret = 0
        RETURN
        END
