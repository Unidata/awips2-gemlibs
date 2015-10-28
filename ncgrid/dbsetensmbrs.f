        SUBROUTINE DB_SETENSMBRS  ( aname, iret )
C************************************************************************
C* DB_SETENSMBRS                                                        *
C*                                                                      *
C* This subroutine puts DB info into common.                            *
C*                                                                      *
C* DB_SETENSMBRS  ( ANAME, IRET )                                       *
C*                                                                      *
C* Input parameters:                                                    *
C*      ANAME           CHAR*           a name                          *
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
        ensnames = aname(:lens)
        isensnames = .true.
        iensmem = 1
        iret = 0
        RETURN
        END
