	SUBROUTINE MMAP_INIT ( iret )
C************************************************************************
C* MAP_INIT								*
C*									*
C* This routine initializes GEMPAK.					*
C*									*
C* MAP_INIT ( IRET )							*
C*									*
C* Output parameters:							*
C*	IRET		INTEGER		Return code			*
C**									*
C* Log:									*
C* S. Jacobs/EAI	 7/94						*
C* C. Lin/EAI	         4/96	GSDEV->GG_MOTF, take out GSFLNM		*
C************************************************************************
	CHARACTER	wname*72
C------------------------------------------------------------------------
C*	Initialize GEMPAK data blocks.
C
	CALL IN_BDTA  ( ier )

C------------------------------------------------------------------------
C*      Initialize GEMPAK common blocks
C
        CALL IP_INIT  ( respnd, iret )
C
C*      Initialize grid library common area grdcmn.cmn
C
        CALL GD_INIT  ( ier )
        CALL GG_INIT  ( 0, ier )
        iostat = 0
C------------------------------------------------------------------------

C
C*	Initialize graphics.
C
	CALL GG_INIT  ( 1, iret )
	IF  ( iret .eq. 0 )  THEN
C
C*	Set device in GEMPLT.
C*	If an error is encountered, write GEMPLT error message.
C
	wname = 'maptop'
	    CALL GG_MOTF ( wname, ier )
	    IF  ( ier .ne. 0 )  THEN
		CALL ER_WMSG  ( 'GEMPLT', ier, ' ', ier2 )
		iret = -6
	    END IF
C
	END IF
C*
	RETURN
	END
