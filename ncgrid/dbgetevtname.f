        SUBROUTINE DB_GETEVTNAME  ( aname, iret )
C************************************************************************
C* DB_GETEVTNAME                                                        *
C*                                                                      *
C* This subroutine gets evtname from  common.                           *
C*                                                                      *
C* DB_GETEVTNAME  ( ANAME, IRET )                                       *
C*                                                                      *
C* Input parameters:                                                    *
C*      ANAME           CHAR*           Event name                      *
C*                                                                      *
C* Output parameters:                                                   *
C*      IRET            INTEGER         Return code                     *
C*                                        0 = normal return             *
C**                                                                     *
C* Log:                                                                 *
C* m.gamazaychikov/CWS	07/09                                           *
C************************************************************************
        INCLUDE         'GEMPRM.PRM'
        INCLUDE         'dbcmn.cmn'
C*
        CHARACTER*(*)   aname
C*
C------------------------------------------------------------------------
C*

        IF ( isevtname ) THEN 
           CALL ST_LSTR ( evtname, levtname, ier )
           aname = evtname ( :levtname)
           iret = 0   
c           isevtname = .false.
         ELSE
           iret = -1
           aname =  ' '
        END IF
        IF ( isensnames ) THEN
c           print *, "DB_GETEVTNAME setting ens member name"
c           print *, "DB_GETEVTNAME before setting iensmem=", iensmem
           CALL ST_LSTR ( ensnames, lensnames, ier )
           aname = ensnames(:lensnames)
c           print *, "DB_GETEVTNAME set ens member name to ", aname
c           print *, "DB_GETEVTNAME after setting iensmem=", iensmem
        END IF
        CALL ST_NULL (aname, aname, lstr, ier)
        RETURN
        END
