        SUBROUTINE DB_ISDBFILE  ( afile, iret )
C************************************************************************
C* DB_ISDBFILE                                                          *
C*                                                                      *
C* This subroutine puts DB info into common.                            *
C*                                                                      *
C* DB_ISDBFILE  ( AFILE, IRET )                                 	*
C*                                                                      *
C* Input parameters:                                                    *
C*      AFILE           CHAR*           a file                          *
C*                                                                      *
C* Output parameters:                                                   *
C*      IRET            INTEGER         Return code                     *
C*                                        0 = A2DB file			*
C*                                       -1 = non-A2DB file		*
C**                                                                     *
C* Log:                                                                 *
C* m.gamazaychikov/CWS  05/11                                           *
C* m.gamazaychikov/CWS  09/11	Removed dbflag from CS, added -1 return	*
C************************************************************************
        INCLUDE         'GEMPRM.PRM'
        INCLUDE         'dbcmn.cmn'
C*
        CHARACTER*(*)   afile
C*
C------------------------------------------------------------------------
C*
        IF ( INDEX(afile,'A2DB' ) .gt. 0 ) THEN 
           dbread = .true.
           iret = 0
          ELSE 
           dbread = .false.
           iret = -1
        END IF
        RETURN
        END
