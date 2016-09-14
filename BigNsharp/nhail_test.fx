      program test
      integer idate
      double precision To, Tdo
      real hvars(20)
      
      read *, idate
      read *, To
      read *, Tdo

      call HAILCAST1(To, Tdo, hvars)
      stop
      end


      subroutine HAILCAST1( To, Tdo, hvars)
c  rej  old version, patched
     
      implicit none

      double precision To, Tdo, T, Td, dT, Toff
      double precision D, Dav, Dmax, Dmin, Dcont
      double precision DavC,davcm,sig,svr,davcb
      double precision UPMAXV, UPMAXT, 
     * ZBAS, TBAS, TOPT,TOPZ, CAPE, SHEAR,ESI
      character*8 cdate
      integer idate, IC, report, i
      parameter(Toff = 1.0, dT = 0.5)
c      character*60 report
      real hvars(20)

      idate = 02000000
      hvars(1) = To
      hvars(2) = Tdo
      
C     CONVERT INTEGER FILE NAME TO CHARACTER EQUIVALENT
      write(cdate,'(i8.8)') idate

      D = 0.0
      Dav = 0.0
      DavC = 0.0
      Dmax = 0.0
      Dmin = 100000.0
      Dcont = 0.0
      IC = 0
      sig = 0
      svr = 0
    
      print *, " -- Running HailCast Ensemble Model --" 
      do T = To - Toff, To + Toff, dT
         do Td = Tdo - Toff, Tdo + Toff, dT
            call hailcloud4 (T, Td, cdate)
            call hailstone2 (UPMAXV, UPMAXT, D, ZBAS, TBAS, TOPT,
     *      TOPZ, CAPE, SHEAR, cdate)
c          convert cm to inches      Dav = 0.0

            D=D/2.54
            print *, "IC= ", IC, "  UPMAXV=", UPMAXV, "  D=", D
            if(d.ge.1.95) sig=sig+1
            if(d.ge.0.75) svr=svr+1
            ESI=(CAPE*SHEAR)/1000
	    if(UPMAXV .gt. 7.0) then
		DavC = DavC + D
		IC= IC + 1
		if(D. lt. Dmin) Dmin = D
	    endif
	    
            if(D. gt. Dmax) Dmax = D
             enddo
           enddo
      
      hvars (3) = ic
      
      if (ic.le.0) then
       davc=0 
       else
       DavC = DavC / IC
      endif

      hvars (4) = davc
      hvars (5) = dmax
      hvars (6) = dmin
 
      hvars (7) = sig
      hvars (8) = svr
      
c  rej  best fit for davc based on reported hail size

      if(davc.eq.0) then
      davcm=0
      report=1
c      "  No hail produced"
      elseif((davc.lt..9).and.(davc.gt.0)) then 
      davcm=davc+.2 
      report=2
c       "  Dime to Quarter most likely, isolated Golfball"
      elseif((davc.ge..9).and.(davc.le.1.4)) then 
      davcm=davc+.1 
      report=3
c       "  A few golfballs possible"
      elseif((davc.gt.1.4).and.(davc.le.2.2)) then 
      davcm=davc+.4 
      report=4
c      "  Tennis / Baseballs possible"
      elseif(davc.gt.2.2) then 
      davcm=davc+.7
      report=5
c      "  Baseballs or Larger"
      endif   
      
      hvars (9) = davcm
      hvars (10) = report
      
      davcb=davc+.6
      
      hvars (11) = davcb
           
c      write(*,202) ic,dmax, davc, dmin
c      write(*,203) report,davcm
c202   format(" ",i2," Convecting Members",/," Max: ",f4.1,

c     *"  Avg: ",f4.1,"  Min: ",f4.1)
c203   format(" Report Fcst:",i2,/,
c     *" (Corrected Output: ",f4.1,")")
c      print*, sig, svr

       do 9345 i=1, 11
       	   print *, i, hvars(i)
9345   continue

       return 
       end



      subroutine hailcloud4(Tinput, Tdinput, cdate)

****************************************************************
***            SKYWATCH EENDIMENSIONELE WOLKMODEL
*** DIE WOLKMODEL WERK VOLGENS DIE PAKKIE METODE, EN ENTRAINMENT
*** WORD IN AG GENEEM DMV SP-KOORDINATE.  DIE MODEL KLASSIFISEER
*** EERS DIE TFI DEUR NA DIE CAPE EN WINDSKUIWING TE KYK EN DAN
*** TE BESLUIT WATTER TIPE STORM (ENKELSEL,MULTISEL OF SUPERSEL)
*** IS MOONTLIK.
***
***********Jan '97: Mmabatho se tephi ipv Windhoek se tephi word
*** gelees en ook is die gemiddelde stasiedruk en stasiehoogtes
*** aangepas by die nuutste inligting
****************************************************************
        COMMON /DATA/ TFI(100,7),SOUNDNG(100,7),WOLKDTA(100,10),
     *              TCA(100),R(100)
      CHARACTER*72 filename, filename1
      character*8 cdate	 
      double precision Tinput, Tdinput
      integer unitnumber, IDATUM, ISTASIE
   

      TMAX = Tinput + 273.16
      TDOU = Tdinput + 273.16
      
      write(filename,4) cdate
      unitnumber=1
      open(unit=1, file=filename)
4     format("/tmp/",a8,".dat")


      write(filename1,5) cdate          
      unitnumber=2
      open(unit=2, file=filename1)
5     format("/tmp/c",a8,".dat")
    
    
*** LEES DIE INVOER VANAF LEER TEPHI 


	

        DO 10 ITEL=1,100
    

   
        READ(1,*, END=15) SOUNDNG(ITEL,2), SOUNDNG(ITEL,1),
     *  soundng(itel,4), soundng(itel, 5),
     *  soundng(itel,6), soundng(itel, 7) 
     
             
c rej
           SOUNDNG(ITEL,3)=0.0
	   
	   if (soundng(itel,5).eq.-9999) then
	    soundng(itel,5)=-90
	    endif
	   if (soundng(itel,4).eq.-9999) then
	     soundng(itel,4)=-85
	    endif

           SOUNDNG(ITEL,4)=SOUNDNG(ITEL,4) + 273.16
           SOUNDNG(ITEL,5)=SOUNDNG(ITEL,5) + 273.16

c                SOUNDNG(ITEL,7)= SOUNDNG(ITEL,7)/2.0

         
c         PRINT *,SOUNDNG(ITEL,2), SOUNDNG(ITEL,4), SOUNDNG(ITEL,5),
c     *  SOUNDNG(ITEL,6), SOUNDNG(ITEL,7)

10    CONTINUE

15    ITEL=ITEL-1


*** INTERPOLEER 'N FYNER SOUNDING
      CALL INTSOUN(TFI,SOUNDNG,ITEL,JTEL,ISTASIE,OPPDRUK)
c     PRINT *,'============== WOLKBASIS DATA ====================='
c     PRINT 109,OPPDRUK/100,TMAX-273.16,TDOU-273.16
c109   FORMAT(' OPP DRUK=',F6.1,'  MAKS OPP TEMP=',F6.1,
c     *        '  OPP DOUPUNT=',F6.1)
*** BEREKEN DIE WOLKBASIS PARAMETERS
      CALL WOLKBAS(TFI,JTEL,WBASP,WBASRS,WBASTMP,WOLKDTA,TMAX,
     *             TDOU,OPPDRUK)
c     PRINT 110,WBASP/100.,WBASTMP-273.16,WBASRS*1000.0
c110   FORMAT(' WOLKBASIS DRUK=',F6.1,'  TEMP=',F6.1,
c     *        '  MENGVERH=',F8.1)
*** BEREKEN DIE TIPE WOLK MOONTLIK DMV CAPE EN WINDSKUIWING 
c     PRINT *,'============== WOLK KENMERKE ======================'
      CALL WOLKSRT(SOUNDNG,TFI,WBASP,WBASTMP,ITIPWLK,JTEL,CAPE
     *             ,SHEAR1,RICH,PSEUDO,ITEL)
c      PRINT *,'================ OPSTROMINGS ======================'
*** BEREKEN DIE WOLKPAKKIE SE TEMPPROFIEL - MAW VOLG NATADIABAAT
*** MET ENTRAINMENT, TOT BY WOLKTOP.
      CALL NATAD(WBASP,WBASTMP,WBASRS,TFI,JTEL,NTRAIN,WOLKDTA,
     *           ITIPWLK,WMAX, BETA)



c111   FORMAT(' MAXIMUM OPSTROMING=',F6.1,' STORMSTERKTE S=',F4.2)
*** SKRYF DIE DATA UIT
      WRITE(2,'(''*************   SP-CLOUD MODEL   **************'')')
      IF(NTRAIN.EQ.1)WRITE(2,'(i10,2X,I5,''  CLOUDTOP ENTRAINMENT'')') 
     *                           IDATUM,ISTASIE
      IF(NTRAIN.EQ.2)WRITE(2,'(i10,2X,I5,''  LATERAL ENTRAINMENT'')')
     *                           IDATUM,ISTASIE
      WRITE(2,'(''MAX TEMP    DEW-POINT'')')
      WRITE(2,'(3X,F4.1,9X,F4.1)')TMAX-273.16,TDOU-273.16
      WRITE(2,
     *'('' CAPE  SHEAR  RICHSON  TYPECLD  PSEUDO'')')
      WRITE(2,'(F7.1,3X,F6.3,5X,F6.0,4X,9X,I2,6X,F6.1)')
     *               CAPE,SHEAR1*1000.0,RICH,ITIPWLK,PSEUDO
      WRITE(2,
     *'(''  P       CP    TA    TD    TC    RS    VU    HGHT'')')


*** SKRYF DIE WOLK SE WAARDES UIT
      DO 200 I=1,JTEL+1
      WRITE(2,
     * '(F5.0,F8.1,4F6.1,F7.1,F8.1, F6.1)')(WOLKDTA(I,KK),KK=1,8)
         if(WOLKDTA(I,7) .gt. wmaxim) then
            wmaxim = WOLKDTA(I,7)
         endif
 
         if(WOLKDTA(I,7).lt.0.0) GOTO 666
200   CONTINUE

666   continue


      close(unit=1)
      close(unit=2)
 
      return
      end
















 
      SUBROUTINE INTSOUN(TFI,SOUNDNG,ITEL,JTEL,ISTASIE,OPPDRUK)
****************************************************************
*** INTERPOLEER 'N FYNER SOUNDING
****************************************************************
      DIMENSION ISTNAAM(10),ISTHGTE(10),STDRUK(10)
      DIMENSION TFI(100,7),SOUNDNG(100,7)
      DATA ISTNAAM/71119,71877,71878,719999,68538,68461,68174,
     *             68588,68816,68842/
      DATA ISTHGTE/900,1084,905,988,1686,1600,1000,
     *            1000,1000, 1000/
      DATA STDRUK/910.0,864.0,900.0,880.0,880.0,840.0,840.0,
     *            900.0,900.0,900.0/

      

      I=1 
      JTEL=1
      IHGTJA=0
	ISTASIE=71119
*** SOEK DIE REGTE HOOGTE EN GEM OPP DRUK VIR DIE STASIE
           tfi(1,5)=soundng(1,1)
           OPPDRUK=SOUNDNG(1,2)*100
          IHGTJA=1
100   CONTINUE
      IPRES=10*(SOUNDNG(1,2)/10)
      PVLAK=IPRES
*** SOEK DIE TWEE VLAKKE WEERSKANTE VAN P
12    IF(I.LT.ITEL.AND.JTEL.LT.90)THEN
***********
10      IF(PVLAK.LE.SOUNDNG(I,2).AND.PVLAK.GT.SOUNDNG(I+1,2).
     *                        AND.SOUNDNG(I+1,2).NE.0.0)THEN
          PDIFF=SOUNDNG(I,2)-SOUNDNG(I+1,2)
          VDIFF=SOUNDNG(I,2)-PVLAK
          VERH=VDIFF/PDIFF
          TDIFF=SOUNDNG(I+1,4)-SOUNDNG(I,4)
          TDDIFF=SOUNDNG(I+1,5)-SOUNDNG(I,5)
          TFI(JTEL,1)=PVLAK
          TFI(JTEL,3)=SOUNDNG(I,4)+(TDIFF*VERH)
          IF(SOUNDNG(I+1,4).GE.350.0)THEN
            TFI(JTEL,4)=SOUNDNG(I+1,5)
          ELSE
            TFI(JTEL,4)=SOUNDNG(I,5)+(TDDIFF*VERH)
          ENDIF
          IF(PVLAK.EQ.SOUNDNG(I,2))THEN 
            TFI(JTEL,2)=SOUNDNG(I,3)
            TFI(JTEL,4)=SOUNDNG(I,5)
          ELSE
            TFI(JTEL,2)=0.0
          ENDIF
          IF(IHGTJA.EQ.1)THEN 
            TFI(JTEL+1,5)=(287.04*(TFI(JTEL,3)+1.0)/(9.78956
     *               *TFI(JTEL,1)*100.0)) * 2500.0 + TFI(JTEL,5)
          ELSE
            TFI(JTEL+1,5)=0.0 
          ENDIF
          JTEL=JTEL+1
          PVLAK=PVLAK-25.0
        ELSE
          I=I+1
          GOTO 12
        ENDIF
***********
        GOTO 12
      ENDIF
      IF(SOUNDNG(ITEL,2).LE.10.0)THEN
        TFI(JTEL-1,4)=SOUNDNG(ITEL-1,4) 
        TFI(JTEL-1,5)=SOUNDNG(ITEL-1,5) 
      ENDIF
      JTEL=JTEL-1
      RETURN
      END 











      FUNCTION XINTERP(TFI,P,JVAL,JTEL) 
****************************************************************
*** INTERPOLASIE TUSSEN 2 VLAKKE
****************************************************************
      DIMENSION TFI(100,7)
*** SOEK DIE 2 VLAKKE WEERSKANTE VAN P
      DO 20 I=1,JTEL-1
        IF(P.LT.TFI(I,1).AND.P.GE.TFI(I+1,1))THEN 
          PDIFF=TFI(I,1)-TFI(I+1,1)
          VDIFF=TFI(I,1)-P
          VERH=VDIFF/PDIFF
          ADIFF=TFI(I+1,JVAL)-TFI(I,JVAL)
          IF(ADIFF.LT.0.0)ADIFF=-1.0*ADIFF
          IF(TFI(I+1,JVAL).LT.TFI(I,JVAL))THEN
            XINTERP=TFI(I,JVAL)-(ADIFF*VERH)
          ELSE
            XINTERP=TFI(I,JVAL)+(ADIFF*VERH)
          ENDIF
        ENDIF
20    CONTINUE
      RETURN
      END 
















      FUNCTION XNATV(TK,P,DP) 
*******************************************************************
*** BEREKEN DIE TEMP OP DIE VLG VLAK DP PASCAL HOER AS P DEUR XNATV
*******************************************************************
      CP=1004.64
      RD=287.04
      RV=461.48
      EPS=0.622
*** BEREKEN DIE LATENTE HITTE 
      IF(TK.LT.233.16)THEN
        AL=(-0.566*(TK-273.16)+1200.0)*4186.0
      ELSE
        AL=(-0.566*(TK-273.16)+597.3)*4186.0
      ENDIF
      TB=TK
*** BEREKEN DIE VERSKILLENDE DELE VD VERG
      XP=611.0*EXP((AL/RV)*(1.0/273.16-1.0/TB))
      A=1.0+(AL*EPS*XP/(RD*TB*P))
      B=1.0+(EPS*EPS*AL*AL*XP/(CP*RD*P*TB*TB))
*** T WERK DIE TEMP BY DIE VOLGENDE VLAK UIT VLG XNATV
      XNATV=TB-(A/B)*((RD*TB*DP)/(P*CP))
      RETURN
      END 






      FUNCTION DATV(T1,P,DP)
*******************************************************************
*** BEREKEN DIE TEMP OP DIE VLG VLAK DP PASCAL HOER AS P DEUR DATV
*******************************************************************
      CP=1004.64
      RD=287.04
      DATV=T1-RD*T1/(CP*P)*DP 
      RETURN
      END 





      SUBROUTINE SP(TE,TD,P,T1,P1)
*******************************************************************
*** BEREKEN DIE SATURATION POINT SE DRUK EN TEMP VIR DRUKVLAK
*** DR   - SP IS DAAR WAAR LUG OP DR NET VERSADIG WORD AS DIT
*** ADIABATIES STYG 
*******************************************************************
      CP=1004.64
      RD=287.04
      RV=461.48
      EPS=0.622
*** BEREKEN MENGVERH BY TE DMV CLAUSSIUS CLAPEYRON : R(BY TE)=RS(BY TD)
      DP=500.0
      AL=(-0.566*(TD-273.16)+597.3)*4186.0
      E=611.0*EXP((AL/RV)*(1.0/273.16-1.0/TD))
      R=EPS*E/P
      P1=P
      T1=TE
*** WERK NOU T LANGS DATV OP TOT RS=R DWS T=TD DWS VERSADIGINGSPUNT
      DO 10 I=1,100 
*** BEREKEN VERSADIGINGS MENGVERH BY TE 
        AL=(-0.566*(TE-273.16)+597.3)*4186.0
        ES=611.0*EXP((AL/RV)*(1.0/273.16-1.0/TE)) 
        RS=EPS*ES/P1
        IF(RS.LE.R)GOTO 15
*** BEREKEN DAN DIE VLG TEMP DEUR DATV
        T1=TE-RD*TE/(CP*P1)*DP
        TE=T1
        P1=P1-DP
10    CONTINUE
15    RETURN
      END 
















      SUBROUTINE UPDRAFT(WBASRS,DR,TK,RS,VU,TFI,JTEL,ITIPWLK,CLWATER,
     * LOADING, WBASP)
*******************************************************************
*** BEREKEN DIE OPSTROMING IN DIE WOLK DEUR BUOYANCY
*******************************************************************
      DIMENSION TFI(100,7)

      RD=287.04
      G=9.78956
      DP=50.0
      TC=TK
      P=DR/100.0
      TA=XINTERP(TFI,P,3,JTEL)

*** BEREKEN LUGDIGTHEID IN KD/M3
      DIGTA=(P*100.0/(RD*(1.0+0.609*RS/(1.0+RS))*(TA)))
     
*** BEREKEN DIE Z-INKRIMENT IN M MBV HIDROSTATIESE VGL
      DELZ=-100.0*(-DP)/(DIGTA*G)
*** BEREKEN DIE TOTALE WATERINHOUD VD WOLK OP VLAK P
       CLWATER=WBASRS-RS

c      WRITE(5,109) P, CLWATER*1000
c109   FORMAT(F4.0, 2X, F5.2)

*** BEREKEN DIE VIRTUELE OMGEWINGS TEMP 
      TAKELV=TA
      VIRT=(1.0+0.608*(RS/(1.0+RS)))
      TAVIR=VIRT*TAKELV

*** BEREKEN DIE VIRTUELE WOLKTEMP
      TCKELV=TC
      TCVIR=VIRT*TCKELV

*** BEREKEN DIE UPDRAFT
      A=VU*VU+2.0*G*DELZ*((TCVIR-TAVIR)/TAVIR)
      B=-2.0*G*CLWATER*DELZ	
      VOLGVU=SQRT(ABS(A+B))
      IF(A+B.LT.0.0)VOLGVU=-1.0*VOLGVU
      VU=VOLGVU
      RETURN
      END 









      FUNCTION XINTWLK(P1,P2,PTFI,WAARDE1,WAARDE2)
*******************************************************************
***INTERPOLASIE VAN WOLKDATA OP VLAK TUSSEN 2 NATAD VLAKKE
*******************************************************************
*** SOEK DIE 2 VLAKKE WEERSKANTE VAN P

      IF(PTFI.LT.P1.AND.PTFI.GE.P2)THEN 
        PDIFF=P1-P2 
        VDIFF=P1-PTFI
        VERH=VDIFF/PDIFF
        VERH=(EXP(VERH)-1.0)/1.71828
        ADIFF=WAARDE1-WAARDE2 
        XINTWLK=WAARDE1-(ADIFF*VERH)
      ENDIF
      RETURN
      END







 
      SUBROUTINE NATAD(WBASP,WBASTMP,WBASRS,TFI,JTEL,NTRAIN,WOLKDTA,
     *                 ITIPWLK,WMAX, BETA)
*******************************************************************
*** BEREKEN DIE WOLKPAKKIE SE TEMP EN MENGVERH, DAN WORD DIE UPDRAFT
*** VERKRY DEUR DIE PAKKIE SE TEMP TE VERGELYK MET DIE TEMPVD SOUNDING
*******************************************************************
      DIMENSION TFI(100,7),WOLKDTA(100,10)
      CP=1004.64
      RD=287.04
      RV=461.48
      EPS=0.622
      DP=5000.0
      G=9.78956
      BL=2464759.0
      WBASVU=4.0
      

*** STEL DIE ENTRAINMENT PARAMETER VOLGENS DIE TIPE WOLK
      IF(ITIPWLK.EQ.0)BETA=0.1
      IF(ITIPWLK.EQ.1)BETA=0.1
      IF(ITIPWLK.EQ.2)BETA=0.075
      IF(ITIPWLK.EQ.3)BETA=0.050
	

*** STEL DIE TIPE ENTRAINMENT: CLOUDTOP VIR CB (1-4) EN LATERAL
*** VIR CU (0)
      IF(ITIPWLK.GE.1)THEN
        NTRAIN=1
      ELSE
        NTRAIN=2
      ENDIF

*** KRY DIE BEGINWAARDES
      TK=WBASTMP
      VU=WBASVU
      RS=WBASRS
      CSPT=WBASTMP
      CSPP=WBASP
      VORIGP=WBASP
      VORIGTK=WBASTMP
      VORIGRS=WBASRS
      VORIGVU=WBASVU
      P=WBASP-DP
      T=WBASTMP
      WMAX=VU

*** KRY DIE PLEK IN DIE TEPHI WAAR DIE WOLKBASIS IS
      DO 100 J=1,100
        WOLKDTA(J,7)=WBASVU
        IF(WBASP.GT.TFI(J,1)*100.0)GOTO 20
100   CONTINUE

*** BEREKEN DIE WOLKTOP SE SP - 300MB HIER AANGENEEM :DVIR LATERAL
20    IF(NTRAIN.EQ.1)THEN
        PTE3=XINTERP(TFI,300.0,3,JTEL)
        PTD3=XINTERP(TFI,300.0,4,JTEL)
        PTE4=XINTERP(TFI,400.0,3,JTEL)
        PTD4=XINTERP(TFI,400.0,4,JTEL)
        PTE=PTE3
        PTD=PTE-((PTE3-PTD3)+(PTE4-PTD4))*0.5
        CALL SP(PTE,PTD,30000.0,ESPT,ESPP)
      ENDIF
      JJ=J
      DO 200 I=1,200
        IF(NTRAIN.EQ.2)THEN

*** LATERAL ENTRAINMENT - INTERPOLEER DIE OMG TEMP EN DOUP OP P VAN OMG IN K
          PTE=XINTERP(TFI,(P/100.0),3,JTEL)
          IF(P.GT.30000.0)THEN
            PTD=XINTERP(TFI,(P/100.0),4,JTEL)
          ELSE
            PTD=PTE-TDEPRES

*** HIER WORD NOU AANVAAR DAT HOER AS 300MB HET DIESELFDE VOG
          ENDIF
*** BEREKEN SP VAN VLAK P MB
          CALL SP(PTE,PTD,P,ESPT,ESPP)
        ENDIF

*** VIR CLOUDTOP:D GEEN ENTRAINMENT NA 300MB ANDERS CHAOS, DWS BETA=0 
        IF(NTRAIN.EQ.1.AND.(P/100.0).LT.300.0)BETA=0.0

*** BEREKEN EDELQW EN EDELQL OP ESPP - DIS TEMPVERSKILLE TUSSEN ESPT
*** EN XNATVOP ESPP (VANAF CSPP), EN DATV OP ESPP ONDERSKEIDELIK
        PRES=CSPP
        ESPTNAT=CSPT
        ESPTDRG=CSPT
110     IF(PRES.GT.(ESPP+300.0))THEN
          ESPTNAT=XNATV(ESPTNAT,PRES,500.0)
          ESPTDRG=DATV(ESPTDRG,PRES,500.0)
          PRES=PRES-500.0
          GOTO 110
        ENDIF
        EDELQW=ABS(ESPTNAT-ESPT)
        EDELQL=ABS(ESPT-ESPTDRG)

*** BEREKEN DIE MENGINGS SP SE VLAK
        AMSPP=CSPP-BETA*(CSPP-P)

*** BEREKEN DIE MENGINGS SP SE TEMPS
        PRES=CSPP
        AMTNAT=CSPT 
        AMTDRG=CSPT 
120     IF(PRES.GT.(AMSPP+50.0))THEN
          AMTNAT=XNATV(AMTNAT,PRES,100.0)
          AMTDRG=DATV(AMTDRG,PRES,100.0)
          PRES=PRES-100.0
          GOTO 120
        ENDIF
        AMND=ABS(AMTNAT-AMTDRG)

*** BEREKEN NOU AMDELQW EN DAN AMSPT - DIS MENGVLAK SE SP TEMP
        AMDELQW=AMND*(1.0-EDELQL/(EDELQL+EDELQW)) 
        AMSPT=AMTNAT-AMDELQW

*** BEREKEN NOU DIE PAKKIE SE TEMP OP P, DIS OOK TD OP P
        PRES=AMSPP
        T=AMSPT
130     IF(PRES.GT.(P+50.0))THEN
          T=XNATV(T,PRES,100.0)
          PRES=PRES-100.0
          GOTO 130
        ENDIF

*** STEL NUWE PAKKIE SE SP OP MENGINGS VLAK OP
        CSPP=AMSPP
        CSPT=AMSPT

*** KRY DOUPUNT DEPRESSIE - SAL LATER GEBRUIK WORD AS DR<300MB
        TDEPRES=PTE-PTD
        TK=T

*** BEREKEN DIE FINALE DAMPDRUK EN DA MENGVERH NA ENTRAIN OP TK GEWERK HET
        E=611.0*EXP((BL/RV)*(1.0/273.16-1.0/TK))
        RS=EPS*E/(P-E)

*** BEREKEN DIE UPDRAFT IN M/S
        CALL UPDRAFT(WBASRS,P,TK,RS,VU,TFI,JTEL,ITIPWLK,CLWATER,LOADING,
     * WBASP)

       

*** TOETS OF DR OP EEN VAN TFI SE DRUKVLAKKE LE
        DO 140 MM=1,JTEL
          IF(TFI(MM,1).GE.(P/100.0).AND.TFI(MM,1).LT.(VORIGP/100.0))THEN
            WOLKDTA(MM+1,1)=TFI(MM,1)
            WOLKDTA(MM+1,2)=TFI(MM,2)
            WOLKDTA(MM+1,3)=TFI(MM,3)-273.16
            WOLKDTA(MM+1,4)=TFI(MM,4)-273.16
            WOLKDTA(MM+1,5)=XINTWLK(VORIGP,P,TFI(MM,1)*100.0
     *                             ,(VORIGTK-273.16),(TK-273.16))
            WOLKDTA(MM+1,6)=XINTWLK(VORIGP,P,TFI(MM,1)*100.0
     *                             ,(VORIGRS*1000.0),(RS*1000.0))
            WOLKDTA(MM+1,7)=XINTWLK(VORIGP,P,TFI(MM,1)*100.0
     *                             ,VORIGVU,VU)
            WOLKDTA(MM+1,8)=TFI(MM,5)
            WOLKDTA(MM+1,9)=CLWATER*1000.0
          ENDIF
140     CONTINUE

*** DEKRIMENTEER P MET 25 MB
        VORIGP=P
        VORIGTK=TK
        VORIGRS=RS
        VORIGVU=VU
        P=P-DP

*** KRY DIE MAKSIMUM WIND SPOED
        IF(VU.GT.WMAX)THEN
          WMAX=VU
          WMAXDRK=P 
        ENDIF

	  
*** TOETS VIR DIE EINDE VAN DIE LOPIE - UPDRAFT NEG, WOLK GROEI NIE MEER
        IF(VU.LT.-20.0)GOTO 70
        IF(P.LE.TFI(JTEL,1)*100.0.OR.P.LE.10000.0)GOTO 70

200   CONTINUE
70    continue

      RETURN
      END 










      FUNCTION XINTBAS(TFI,PP,JVAL,JTEL)
*******************************************************************
*** INTERPOLASIE VAN DIE WOLKBASIS TUSSEN 2 VLAKKE
*******************************************************************
      DIMENSION TFI(100,6)

*** SOEK DIE 2 VLAKKE WEERSKANTE VAN P
      P=PP/100.0
      DO 20 I=1,JTEL-1
        IF(P.LT.TFI(I,1).AND.P.GE.TFI(I+1,1))THEN 
          PDIFF=TFI(I,1)-TFI(I+1,1)
          VDIFF=TFI(I,1)-P
          VERH=VDIFF/PDIFF
          ADIFF=TFI(I+1,JVAL)-TFI(I,JVAL)
          IF(ADIFF.LT.0.0)ADIFF=-1.0*ADIFF
          IF(TFI(I+1,JVAL).LT.TFI(I,JVAL))THEN
            XINTBAS=TFI(I,JVAL)-(ADIFF*VERH)
          ELSE
            XINTBAS=TFI(I,JVAL)+(ADIFF*VERH)
          ENDIF
        ENDIF
20    CONTINUE
      RETURN
      END 










      SUBROUTINE WOLKBAS(TFI,JTEL,WBASP,WBASRS,WBASTMP,WOLKDTA,
     *                   TMAX,TDOU,OPPDRUK)
*******************************************************************
*** BEREKEN DIE WOLKBASIS PARAMETERS VANAF DIE MAKS TEMP
*******************************************************************
      DIMENSION TFI(100,7),WOLKDTA(100,10)
      CP=1004.64
      RD=287.04
      RV=461.48
      EPS=0.622
      DP=100.0
      T1=TMAX
      TD1=TDOU
      P=OPPDRUK

*** BEREKEN MENGVERH BY TE DMV CLAUSSIUS CLAPEYRON :D R(BY T1)=RS(BY TD1)
      AL=(-0.566*(TD1-273.16)+597.3)*4186.0
      E=611.0*EXP((AL/RV)*(1.0/273.16-1.0/TD1))
      R=EPS*E/P
      JJ=1

*** WERK NOU T LANGS DATV OP TOT RS=R DWS T=TD DWS WOLKBASIS
      DO 20 I=1,500 

*** BEREKEN MENGVERH BY T1
        AL=(-0.566*(T1-273.16)+597.3)*4186.0
        ES=611.0*EXP((AL/RV)*(1.0/273.16-1.0/T1)) 
        RS=EPS*ES/P 

*** STEL DIE DATA IN WOLKDTA
        IF(TFI(JJ,1).GE.(P/100.0))THEN
          WOLKDTA(JJ,1)=TFI(JJ,1)
          WOLKDTA(JJ,2)=TFI(JJ,2)
          WOLKDTA(JJ,3)=TFI(JJ,3)-273.16
          WOLKDTA(JJ,4)=TFI(JJ,4)-273.16
          WOLKDTA(JJ,5)=T1-273.16
          WOLKDTA(JJ,6)=RS*1000
          WOLKDTA(JJ,8)=TFI(JJ,5)
          JJ=JJ+1
        ENDIF
        IF(RS.LE.R)GOTO 10
*** BEREKEN VOLGENDE VLAK SE TEMP VLG DATV
        T2=T1-RD*T1/(CP*P)*DP 
        T1=T2
        P=P-DP
20    CONTINUE

10    IF(RS.LE.R)THEN
        WBASRS=RS
        WBASP=P
        WBASTMP=T1
        WOLKDTA(JJ,1)=WBASP/100.0
        WOLKDTA(JJ,2)=XINTBAS(TFI,WBASP,5,JTEL)
        WOLKDTA(JJ,3)=XINTBAS(TFI,WBASP,3,JTEL)-273.16
        WOLKDTA(JJ,4)=XINTBAS(TFI,WBASP,4,JTEL)-273.16
        WOLKDTA(JJ,5)=T1-273.16
        WOLKDTA(JJ,6)=RS*1000 
        WOLKDTA(JJ,8)=XINTBAS(TFI,WBASP,5,JTEL)
      ELSE
*** WOLKBASIS NIE GEKRY - LUG TE DROOG OF SO-IETS 
        WBASRS=9999 
        WBASTMP=9999
        WBASP=9999
      ENDIF
      RETURN
      END 









      SUBROUTINE WOLKSRT(SOUNDNG,TFI,WBASP,WBAST,ITIPWLK,JTEL,
     *                   CAPE,SHEAR1,R,PSEUDO,ITEL)
*******************************************************************
*** DIE TFI WORD ONTEED OM TE BESLUIT WATTER TIPE WOLK IS MOONTLIK
*** DIE DAG AFHANGENDE VAN DIE WINDSKUIWING EN DIE CAPE.
*** DAAR WORD BESLUIT OF DIT ENKELSEL, MULTISEL OF SUPERSEL STORMS
*** GAAN WEES.
*******************************************************************
      DIMENSION TFI(100,7),SOUNDNG(100,7)
      RD=287.04
      RV=461.48
      EPS=0.622

*** VOLG DIE PAKKIE NATADIABATIES TOT WOLKTOP OM CAPE TE BEREKEN
*** CAPE IS DIE POSITIEWE AREA TUSSEN DIE PAKKIE EN OMGEWING OP TFI
      DP=500.0
      PRES=WBASP
      PSEUDO=WBAST
10    IF(PRES.LT.100500.0)THEN
        PSEUDO=XNATV(PSEUDO,PRES,-500.0)
        PRES=PRES+500.0
        GOTO 10
      ENDIF

c     PRINT 11,PSEUDO
c11    FORMAT(' WOLKBASIS NATBOL POTENSIELE TEMP=',F6.1)
      PRES=WBASP
      TNAT=WBAST
      CAPE=0.0
20    IF(PRES.GT.15000.0)THEN 
*** T1 IS PAKKIE TEMP, TO1 IS OMG TEMP
        T1=TNAT
        TO1=XINTERP(TFI,PRES/100.0,3,JTEL)
        DP1000=PRES-100000.0
        Q1=DATV(T1,PRES,DP1000)
        QO1=DATV(TO1,PRES,DP1000)
        T2=XNATV(T1,PRES,DP)
        PRES=PRES-DP
        TO2=XINTERP(TFI,PRES/100.0,3,JTEL)
        DP1000=PRES-100000.0
        Q2=DATV(T2,PRES,DP1000)
        QO2=DATV(TO2,PRES,DP1000)

*** BEREKEN NOU DIE CAPE AS PAKKIE SE TEMP WEL WARMER AS DIE OMGEWING IS
*** DIE CAPE WORD BEREKEN DEUR DIE OPP TUSSEN Q1,QO1,Q2,QO2 TE KRY
        TEST=(0.5*RD*(T1+T2)*0.5/(0.5*(PRES+PRES+DP))*
     *         ((Q2-QO2)/QO2 + (Q1-QO1)/QO1) *DP) 
        IF(Q2.GE.QO2.AND.TEST.GE.0.0)THEN
          CAPE=CAPE+(0.5*RD*(T1+T2)*0.5/(0.5*(PRES+PRES+DP))*
     *         ((Q2-QO2)/QO2 + (Q1-QO1)/QO1) *DP) 
          
        ENDIF

        TNAT=T2
        GOTO 20
      ENDIF
      
*****************BEREKEN NOU DIE WINDSKUIWING*****************
*** KRY EERS DIE 850, 500 EN 400 WINDE
     
       DO 100 I=1,ITEL-1
          RIGTSFC=SOUNDNG(1,6)
          SPOEDSF=SOUNDNG(1,7)
        IF(SOUNDNG(I,2).EQ.850.0)THEN
           RIGT850=SOUNDNG(I,6)
           SPOED85=SOUNDNG(I,7)
           H850=SOUNDNG(I,1)
        ENDIF
	if(soundng(1,2).lt.850) then
	  rigt850=soundng(1,6)
	  spoed85=soundng(1,7)
	 endif
          IF(SOUNDNG(I,2).EQ.700.0)THEN
             RIGT700=SOUNDNG(I,6)
             SPOED70=SOUNDNG(I,7)
             H700=SOUNDNG(I,1)
          ENDIF
            IF(SOUNDNG(I,2).EQ.500.0)THEN
                 RIGT500=SOUNDNG(I,6)
                 SPOED50=SOUNDNG(I,7)
                 H500=SOUNDNG(I,1)
            ENDIF 
           IF(SOUNDNG(I,2).EQ.400.0)THEN
              RIGT400=SOUNDNG(I,6)
              SPOED40=SOUNDNG(I,7)
              H400=SOUNDNG(I,1)
           ENDIF
100   CONTINUE


*** KRY DIE KOMPONENTE
      USFC=-1.0*SPOEDSF*SIN(RIGTSFC*0.0174532)
      VSFC=-1.0*SPOEDSF*COS(RIGTSFC*0.0174532)
      U850=-1.0*SPOED85*SIN(RIGT850*0.0174532)
      V850=-1.0*SPOED85*COS(RIGT850*0.0174532)
      U700=-1.0*SPOED70*SIN(RIGT700*0.0174532)
      V700=-1.0*SPOED70*COS(RIGT700*0.0174532)
      U500=-1.0*SPOED50*SIN(RIGT500*0.0174532)
      V500=-1.0*SPOED50*COS(RIGT500*0.0174532)
      U400=-1.0*SPOED40*SIN(RIGT400*0.0174532)
      V400=-1.0*SPOED40*COS(RIGT400*0.0174532)


       U600=0.5*(U500+U400)
       V600=0.5*(V500+V400)
c      PRINT *, "COMP",U850,V850, U700,V700, U500, V500,U400, V400

       A=(U500-(U500-U400)*0.25) + U500 + U600+ U700 + U850 + USFC
       B=(V500-(V500-V400)*0.25) + V500 + V600+ V700 + V850 + VSFC
       UBO=A/6.0
       VBO=B/6.0

       UBO1= (U500-(U500-U400)*0.25)
       VBO1= (V500-(V500-V400)*0.25)

*** ONDERSTE WIND
      UON=U850*0.935
      VON=V850*0.935
      UON1=U850
      VON1=V850

*** DIE DIKTE
        DZ= (H500-(H500-H400)*0.25)-(H850*0.935)
        DZ1= (H500-(H500-H400)*0.25)- H850
         

*** SKUIWING VIR CAPE FORMULE = DU*DU + DV*DV
      US=(UBO-UON)*(UBO-UON) + (VBO-VON)*(VBO-VON)

*** WINDSKUIWING  DV/DZ = DU/DZ I  +  DV/DZ J
*** GROOTTE VAN   DV/DZ = SQRT(DU/DZ * DU/DZ  +  DV/DZ * DV/DZ)
      SHEAR=SQRT(((UBO-UON)/DZ)*((UBO-UON)/DZ) +
     *           ((VBO-VON)/DZ)*((VBO-VON)/DZ))
      SHEAR1=SQRT(((UBO1-UON1)/DZ1)*((UBO1-UON1)/DZ1) +
     *           ((VBO1-VON1)/DZ1)*((VBO1-VON1)/DZ1))

*** DIE RICHARDSON GETAL
      R=CAPE/(0.5*US)

*** DEFINIEER DIE TIPE WOLK VOLGENS WINDSKUIWING EN CAPE
      TYPE=CAPE*SHEAR1
      IF(TYPE.LE.1.0)ITIPWLK=0
      IF(TYPE.LE.3.0.AND.TYPE.GT.1.0)ITIPWLK=1
      IF(TYPE.GT.3.0.AND.TYPE.LE.5.0) ITIPWLK=2
      IF(TYPE.GT.5.0) ITIPWLK=3

200   FORMAT(' CAPE=',F6.1,'  WINDSKUIWING=',F7.5,'  TIPE WOLK=',I1,
     *       '  RICHARDSON=',F7.1)
      RETURN
      END 
      subroutine hailstone2(UPMAXV, UPMAXT, D, ZBAS, TBAS, TOPT,
     * TOPZ, CAPE, SHEAR,cdate)
*****************************************************************
***        SKYWATCH EENDIMENSIONELE HAELMODEL
***  DIE PROGRAM VOORSPEL HAEL KORREL GROOTES VANAF N 
***  'N WOLK SOOS VOORSPEL DEUR SKYWATCH WOLKMODEL SWM
*****************************************************************
      implicit double precision(A-H), double precision(O-Z)
      DIMENSION TCA(100),R(100),VUU(100),DD(20),BEGTYD(20),TAA(100)
      DIMENSION JST(6),ISTN(6),IHT(6),IPRES(6),
     *          ZA(100)
      CHARACTER*12 DTIPE(7)
      COMMON /AAA/ PA(100), ITEL
*****************************************************************
***                      VERANDERLIKES
***  A        =VENTILASIE KOEFFISIENT
***  AK       =TERMIESE KONDUKTIWITEIT
***  ANU      =DINAMIESE VISKOSITEIT
***  ALF      =LATENTE HITTE VAN FUSIE
***  ALS      =LATENTE HITTE VAN SUBLIMASIE
***  ALV      =   "      "    " VERDAMPING
***  CD       =DRAG KOEFFISIENT
***  CI       =SPESIFIEKE HITTEKAPASITEIT VAN YS
***  CW       =    "             "        VAN WATER
***  CLADWAT  =ADIABATIESE WOLKWATER INHOUD
***  CLWATER  =TOTALE         "        "
***  D        =DEURSNIT VAN HAEL KORREL 
***  DELRW    =VERSKIL IN DAMPDIGTH. TUSSEN HAELOPP. EN WOLKLUG
***  DENSA    =DIGTHEID VAN DIE LUG (WOLKOMG)
***  DENSE    =    "    VAN DIE HAEL KORREL
***  DI       =DIFFUSIWITEIT
***  DGM       =TOTALE MASSATOENAME VAN KORREL
***  DGMI      =MASSATOENAME AGV YSDEELTJIES
***  DGMW      =     "       AGV WATERDRUPPELTJIES
***  EI       =COLLECTION EFFICIENCY FOR ICE
***  EW       =    "          "       "  WATER
***  FW       =FRAKSIE WATER IN DIE HAEL KORREL
***  G        =GRAVITASIE VERSNELLING
***  GM       =TOTALE MASSA VAN HAEL KORREL
***  GMW      =MASSA WATER   "   "     "
***  GMI      =MASSA YS      "   "     "
***  ISEK     =TYD TELLER (AANTAL SEKONDES)
***  ISEKDEL  =TYDSTAPPE  (IN SEKONDES) 
***  P        =DRUK 
***  PA       =OMG. DRUK VAN TFI
***  PC       =PERSENTASIE WOLKWATER TUSSEN -20 EN -40 GRADE C
***  R        =MENGVERH VAN LUG
***  RE       =REYNOLDS GETAL 
***  RS       =VERSADIGINGS MENGVERH VANLUG
***  REENWAT  =REENWATER INHOUD VAN WOLK
***  TAU      =WOLK SE LEEFTYD
***  T        =GEINTERPOLEERDE OMG TEMP 
***  TA       =OMG TEMP VAN TFI (MATRIKS)
***  TD       =OMG DOUPUNT VAN TFI
***  TC       =WOLKTEMP OP VLAK P
***  TCA      =WOLKTEMP MATRIKS
***  TS       =HAELKORREL SE OPP TEMP
***  V        =WERKLIKE SNELHEID VAN KORREL
***  VT       =TERMINAAL VALSNELHEID VAN KORREL
***  VUU      =UPDRAFT MATRIKS
***  VU       =UPDRAFT SNELHEID
***  WBAST    =WOLKBASIS TEMP.
***  WBASTD   =    "     DOUPUNT
***  WBASRS   =    "     VERSADIGINGS MENGVER.
***  WBASTW   =    "     NATBOL POTENS. TEMP.
***  WBASP    =    "     DRUK 
***  WTOPP    =WOLKTOP DRUK
***  WTOPT    =   "    TEMP
***  WWATER   =WOLKWATER INHOUD
***  WYS      =WOLKYS     "
***  XI       =KONSENTRASIE YS TEENGEKOM DEUR DRUPPEL
***  XW       =KONSENTRASIE WATER TEENGEKOM DEUR DRUPPEL
***  Z        =HOOGTE VAN KORREL BO OPP.
*****************************************************************
      CHARACTER*72 filename, filename1
      INTEGER unitnumber, unitnumber1
      character*8 cdate
      DATA RV/461.48/,RD/287.04/,G/9.78956/
      DATA PI/3.141592654/,ALF/79.7/,ALV/597.3/
      DATA ALS/677.0/,CI/0.5/,CW/1./
      DATA DTIPE/'  NONE','   SHOT','   PEA','  GRAPE','  WALNUT',
     *           '  GOLF',' >GOLF'/
      DATA JST/71119,68442,68424,68242,68538,68461/
      DATA ISTN/3HWSE,3HBFN,3HUPN,3HMMA,3HDEA,3HBET/
      DATA IHT/766,1350,845,1282,1287,1681/
      DATA IPRES/925,864,900,880,880,840/
      
      write(filename,4) cdate
      unitnumber=1  
      open(unit=unitnumber, file=filename)
4     format("/tmp/c",a8,".dat")

      write(filename1,5) cdate
	unitnumber1=4
      open(unit=unitnumber1, file=filename1)
5     format("/tmp/h",a8,".dat")

      SEKDEL=0.2
******************** 1. INVOER DATA *****************************
***            LEES DIE OMGEWINGS PARAMETERS IN
***  EERS DIE DATUM,MAXTEMP,STASIENR IN 
      CALL LEESDTA(PA,ZA,VUU,R,TAA,TCA,IDAT,WBASP,VBASIS,
     *           ISTNR,ITEL,TMAXT,ITIPWLK, CAPE, SHEAR,
     *           UPMAXV, UPMAXT,RICH,TDEW, ZBAS, TBAS, TOPT,
     *           TOPZ)

 
      BEGTYD(1)=60. 
      DD(1)=300.
      TAU=4200.
      STHGTE=ZA(1)
      TAA(1)=TMAX
      TCA(1)=TAA(1) 
      DO 3 I=1,ITEL 
        IF(PA(I).EQ.WBASP)THEN
          PBEGIN=PA(I)
          RSBEGIN=R(I)
          P0=PA(I)
          RS0=R(I)
          TABEGIN=TAA(I)
          TCBEGIN=TCA(I)
          ZBAS=ZA(I)
          V=VUU(I)
          
        ENDIF
3     CONTINUE

      ZBEGIN=ZBAS-STHGTE
      JTEL=0

***  STEL TOETS VIR EMBRIO: 0 NOG NIE 1E KEER GEVRIES, 1 MOENIE WEER VRIES
      NOFROZE=0
C      WRITE(4,133)IDAT,ISTNR
C133   FORMAT(BZ,I10,5X,I5)
C      WRITE(4,31)
C31    FORMAT(1X,'DRUK ',2X,'OMG-T',3X,'WOLK-T',3X,'MENG ',4X,'UPDR')
C      DO 32 J=1,ITEL
C        IF(R(J).GT.0.)WRITE(4,33)PA(J),TAA(J),TCA(J),R(J),VUU(J)
C33      FORMAT(1X,F5.1,2X,F6.2,2X,F6.2,2X,F6.3,2X,F7.2)
C32    CONTINUE
35    CONTINUE

      JTEL=JTEL+1
C      WRITE(4,36)JTEL,DD(JTEL),BEGTYD(JTEL)
C36    FORMAT('NUWE EMBRIO ',I1,' VAN=',F4.0,
C     *'MIKROMETER NA',I4,' SEK')

      SEK=BEGTYD(JTEL)
      VU=V
      Z=ZBEGIN
      TC=TCBEGIN
      TA=TABEGIN
      WBASP=P0
      P=PBEGIN
      WBASRS=RS0
      RS=RSBEGIN
      RSS=RS/1000.

***  STEL REENWATER INHOUD AANVANKLIK 0 
      REENWAT=0.

***  KRY DIGTH. VAN LUG (G/CM3)
      DENSA=(P*100./(RD*(1.+0.609*RSS/(1.+RSS))*(TC+273.16)))/1000.

***            KORREL PARAMETERS
***  OPP. TEMP, FRAKSIEWATER, DEURSNIT(M),FRAKSIE WATER, DIGTHEID
      TS=TC
      D=DD(JTEL)/10000.
      PC=0.
      FW=1.0
      DENSE=1.
c      write(4,'("  TS    TC     D     P    FW     Z      V",
c     *          "       VU      VT   SEK",
c     *       " TIPE GROEI")')

40    CONTINUE
      SEK=SEK+SEKDEL

  
**********************  2. VERWERK PARAMETERS  ********************** 
***                 BEREKEN OMGEWINGS PARAMETERS
***  KRY UPDRAFT
      CALL INTERP(VUU,VU,P,IFOUT)
***  AS TYD KLEINER AS 60 MIN - BEREKEN SIN VAN UPDRAFT SODAT UPDRAFT 
***  KONSTANT BLY TOT 40 MIN EN DAN WEER AFPLAT TOT 60 MIN
***  AS TYD GROTER AS 60 MIN - STOP UPDRAFT, WOLK VAL PLAT

        TIME=0.0
        upIndex=5
        TIME=CAPE*(SHEAR/1000.0)

        IF(TIME.LT.1.0)TIME=1.0        
        
          if(upIndex.eq.5.and.TIME.lt.5.0) then
         
           DUR = (-2.5 * TIME**2 + 25.0 * TIME - 2.5)*60.0
           else
            DUR=3600.0
        endif
      
      ITIME = int(DUR)
      IF(ITIME.LT.600) ITIME = 600 
     
      IF(SEK.GT.ITIME)VU=0.0

      IF(IFOUT.EQ.1)GOTO 100


***  BEREKEN TERMINAAL VALSNELHEID VAN DIE KORREL (GEBRUIK VORIGE WAARDES)
      CALL TERMINL(DENSA,DENSE,D,VT,TC) 

***  WERKLIKE SNELHEID VAN DIE KORREL (OPWAARTS +)
      V=VU-VT

***  BEREKEN HOOGTE VAN NUWE DRUKVLAK UIT HIDROST. VGL.
      P=(P*100.-(DENSA*1000.*G*V/100.)*SEKDEL)/100.
      Z=Z+(V/100.)*SEKDEL

***  BEREKEN NUWE WOLKTEMP OP NUWE P-VLAK
      CALL INTERP(TCA,TC,P,IFOUT)
      IF(IFOUT.EQ.1)GOTO 100

***  BEREKEN PERSENTASIE GEVRIESDE WATER/100
      PC=0.008*(1.274)**(-20.-TC)
      IF(TC.GT.-20.)PC=0.
      IF(PC.GT.1.)PC=1.
      IF(PC.LT.0.)PC=0.

***  BEREKEN MENGVERH. OP NUWE P-VLAK, DAN NUWE WOLKDIGTHEID
      CALL INTERP(R,RS,P,IFOUT)
      IF(IFOUT.EQ.1)GOTO 100
      RSS=RS/1000.
      DENSA=(P*100./(RD*(1.+0.609*RSS/(1.+RSS))*(TC+273.16)))/1000.

***  BEREKEN DIE TOTALE WATERINHOUD VD WOLK OP VLAK P, EN ADIAB. WAARDE
      CALL WOLKWAT(XW,XI,CLWATER,CLADWAT,REENWAT,WWATER,WYS,
     *           PC,WBASRS,RSS,TC,DENSA,ITIPWLK,SEK, WBASP, P)


**************  3. TOETS VIR TIPE GROEI - NAT OF DROOG  **************
***       NATGROEI - KORREL OPP TEMP.GE.0, DROOG - KORREL OPP TEMP.LT.0
      IF(TS.GE.-9..AND.TC.GE.-9..AND.NOFROZE.EQ.0)GOTO 42
      IF(TS.LT.0.)THEN
***  DROE GROEI
        FW=0.
        ITIPE=1
        ELSE
***  NAT GROEI
        TS=0.
        ITIPE=2
      ENDIF


***     VRIES DIE OORSPRONKLIKE EMBRIO BY OMG. TEMP -8 - DROE GROEI DAARNA
42    IF(TS.GE.-9..AND.TC.GE.-9..AND.NOFROZE.EQ.0)THEN
        IF(TC.LE.-8.)THEN
***  DROE GROEI
          FW=0.
          TS=TC
          ITIPE=1
          NOFROZE=1 
          ELSE
***  NATGROEI
          FW=1.
          TS=TC
          ITIPE=2
          NOFROZE=0 
        ENDIF
      ENDIF


***  DIGTHEID VAN KORREL - HANG AF VAN FW - SLEGS WATER=1 GM/L=1000KG/M3
***                                         SLEGS YS  =0.9 GM/L
      DENSE=(FW*(1.0 - 0.9)+0.9)


***  DAMPDIGTHEIDS VERSKIL TUSSEN KORREL EN OMGEWING
      CALL DAMPDIG(DELRW,PC,TS,TC)


*********************  4. KORREL MASSA GROEI  *******************
      CALL MASSAGR(GM,D,GM1,DGM,EW,EI,DGMW,DGMI,GMW,GMI,DI, 
     *           TC,TS,P,DENSE,FW,VT,XW,XI,SEKDEL)


***************  5. BEREKEN DIE HITTE BUDGET VAN KORREL  **************
      SEKK=SEK-BEGTYD(JTEL)
      CALL HEATBUD(TS,FW,TC,D,DENSA,GM1,DGM,VT,DELRW,DGMW,
     *           DGMI,GMW,GMI,DI,SEKDEL,ITIPE,P)
c      IF(MOD(DINT(SEK/SEKDEL),DINT(5.0/SEKDEL)).EQ.0)
c     *WRITE(4,71)TS,TC,D,P,FW,Z,V,VU,VT,SEK,ITIPE
c71    FORMAT(F5.1,' ',F5.1,' ',F8.5,' ',F5.0,' ',F4.2,' ',
c     *       F6.0,' ',F7.1,' ',F7.1,' ',F7.1,' ',F6.0,
c     *       ' ',I2)

c      IF(SEK/100*100.EQ.SEK)
c       IF(MOD(DINT(SEK/SEKDEL),DINT(300.0/SEKDEL)).EQ.0)
c     *PRINT 72,Ts,TC,D,P,Z,V,SEK/60.,ITIPE
c72    FORMAT('Ts=',F5.1,' Tc=',F5.1,' D=',F5.3,' P=',F4.0,
c     *' Z=',F6.0,' V=',F7.1,' Tyd=',f4.1,' Groei=',I2)
       
       if((sek.gt.500).and.(v.lt.50).and.(v.gt.-50).and.
     *(vu.le.400).and.(nofroze.eq.0)) then
        upmaxv=4
	upmaxt=tbas
	ic=ic-1
	d=0
       endif



*********  6. TOETS DEURSNIT VAN KORREL EN HOOGTE BO GROND  ************
***  TOETS OF DEURSNIT VAN KORREL GROTER IS AS LIMIET - BREEK KORREL OP
      CALL BREAKUP(DENSE,D,GM,FW)
***  TOETS OF KORREL ONDER WOLKBASIS IS - GEEN WOLKDRUPPELS OF YS MEER NIE
      IF(P.GE.WBASP)THEN
        XW=0.
        XI=0.
      ENDIF

***  TOETS OF KORREL AL BY GROND IS
      IF(Z.LE.0.) GOTO 100

***  TOETS OF WOLK LEEFTYD VERBY IS - KORREL VAL DAN NETSO UIT???
      IF(SEK.GE.TAU)GOTO 100 

***  GAAN TERUG VIR VOLGENDE TYDSTAP
      
	GOTO 40
***  SKRYF WAARDES UIT EN STOP LOPIE

100   CONTINUE
      TTYD=SEK/60. 
c      WRITE(4,110)D,TTYD,Z,TMAX
c110   FORMAT('Diameter=',F5.2,' cm   Time=',F5.1,
c     *' min    Height=',F6.0,'   Max temp=',F3.0)


***  SKRYF DIE DATA VIR DIE VOORSPELLERS UIT
      IF(DABS(FW - 1.0).LT.0.001) then
         NRD=1
         D=0.0
        else
         IF(D.LT.0.1)NRD=1
         IF(D.GE.0.1.AND.D.LT.0.4)NRD=2
         IF(D.GE.0.4.AND.D.LT.1.3)NRD=3
         IF(D.GE.1.3.AND.D.LT.2.1)NRD=4
         IF(D.GE.2.1.AND.D.LT.3.3)NRD=5
         IF(D.GE.3.3.AND.D.LT.5.3)NRD=6
         IF(D.GE.5.3)NRD=7
      endif     

   
      BINDEX=RSBEGIN*TCBEGIN
c      print *, IDATUM,TMAXT,TDEW,CAPE,SHEAR,RICH,UPMAXV,
c     * UPMAXT,TCBEGIN,BINDEX,TIME,ITIME,D,DTIPE(NRD),TTYD
c      write(2,132)ID,TMAXT,TDEW,CAPE,SHEAR,RICH,UPMAXV,
c     * UPMAXT,TCBEGIN,BINDEX,TIME,ITIME,D,DTIPE(NRD),TTYD
c      write(5,132)ID,TMAXT,TDEW,CAPE,SHEAR,RICH,UPMAXV,
c     * UPMAXT,TCBEGIN,BINDEX,TIME,ITIME,D,DTIPE(NRD),TTYD
c132   FORMAT(I6,10F7.1,I6,F6.1,A12,F4.1)
      
C      WRITE(2,140)D,NRD,DTIPE(NRD),ttyd
C140   FORMAT('Diameter=',F4.2,' cm       Category= ',I1,'   (',A12,')', 
C     *       '      Time=',F5.1,' min')

      close(unit = 1)
c      close(unit = 2)
c      close(unit = 4)
      close(unit = 5)
     
 20   continue

      return
      END 



      SUBROUTINE HEATBUD(TS,FW,TC,D,DENSA,GM1,DGM,VT,DELRW,DGMW,
     *           DGMI,GMW,GMI,DI,SEKDEL,ITIPE,P)
******************************************************************
*** BEREKEN DIE KORREL SE HEAT BUDGET
******************************************************************
      implicit double precision (A-H), double precision (O-Z)
      DATA RV/461.48/,RD/287.04/,G/9.78956/
      DATA PI/3.141592654/,ALF/79.7/,ALV/597.3/
      DATA ALS/677.0/,CI/0.5/,CW/1./
***  BEREKEN EERS DIE KONSTANTES
      AK=(5.8+0.0184*TC)*10.**(-5.)
      TK=TC+273.15
      ANU=1.717E-4*(393.0/(TK+120.0))*(TK/273.15)**1.5
      RE=D*VT*DENSA/ANU
      H=(0.71)**(1.0/3.0)*(RE**0.50)
      E=(0.60)**(1.0/3.0)*(RE**0.50)


      IF(RE.LT.6000.0)THEN
         AH=0.78+0.308*H
         AE=0.78+0.308*E
      ELSEIF(RE.GE.6000.0.AND.RE.LT.20000.0)THEN
         AH=0.76*H
         AE=0.76*E
      ELSEIF(RE.GE.20000.0) THEN
         AH=(0.57+9.0E-6*RE)*H
         AE=(0.57+9.0E-6*RE)*E
      ENDIF

***  VIR DROE GROEI IS FW=0, BEREKEN NUWE TS, ITIPE=1
***  VIR NATGROEI IS TS=0, BEREKEN NUWE FW, ITIPE=2
      IF(ITIPE.EQ.2)GOTO 60

***  DROE GROEI
50    CONTINUE

      TS=TS-TS*DGM/GM1+SEKDEL/(GM1*CI)*(2.*PI*D*(AH*AK*(TC-TS)
     *-AE*ALS*DI*DELRW)+ DGMW/SEKDEL*(ALF+CW*TC)+DGMI/SEKDEL*CI*TC)

51    FORMAT(I4,'TS=',F6.2,'DGM=',F14.13,'GM1=',F14.12,
     *'DGMW=',F14.13,'DGMI=',F14.13)
      GOTO 70
60    CONTINUE

***  NATGROEI
      FW=FW-FW*DGM/GM1+SEKDEL/(GM1*ALF)*(2.*PI*D*(AH*AK*TC
     *-AE*ALV*DI*DELRW)+DGMW/SEKDEL*(ALF+CW*TC)+DGMI/SEKDEL*CI*TC)

70    CONTINUE
      IF(FW.GT.1.)FW=1.
      IF(FW.LT.0.)FW=0.
      RETURN
      END 


******************************************************************
*** BEREKEN DIE KORREL SE MASSA GROEI
******************************************************************
      SUBROUTINE MASSAGR(GM,D,GM1,DGM,EW,EI,DGMW,DGMI,GMW,GMI,DI,
     *           TC,TS,P,DENSE,FW,VT,XW,XI,SEKDEL)
            
      implicit double precision(A-H), double precision (O-Z)
      PI=3.141592654

***  BEREKEN DIFFUSIWITEIT DI 
      D0=0.226
      DI=D0*((TC+273.16)/273.16)**1.81*(1000./P)
  
***  COLLECTION EFFICIENCY VIR WATER EN YS
      EW=1.0
      IF(TS.GE.-5.0)THEN
        EI=1.00
        ELSE
        EI=0.21
      ENDIF

***  MASSA, MASSA WATER, MASSA YS IN KORREL
      GM=PI/6.*(D**3.)*DENSE
      GMW=FW*GM
      GMI=GM-GMW

***  STOOR DIE MASSA
      GM1=GM



*********************  4. KORREL MASSA GROEI  *******************
***  BEREKEN NUWE DEURSNIT

      D=D+SEKDEL*0.5*VT/DENSE*(XW*EW+XI*EI)

***  BEREKEN MASSA TOENAME AGV WATERDRUPPELS
      GMW2=GMW+SEKDEL*(PI/4.*D**2.*VT*XW*EW)
      DGMW=GMW2-GMW 
      GMW=GMW2

***  EN VIR YS
      GMI2=GMI+SEKDEL*(PI/4.*D**2.*VT*XI*EI)
      DGMI=GMI2-GMI 
      GMI=GMI2

***  EN DAN TOTALE MASSA VERANDERING
      DGM=DGMW+DGMI 
      RETURN
      END 


******************************************************************
***  BEREKEN DIE TOTALE WATERINHOUD VD WOLK OP VLAK P, EN ADIAB. WAARDE
*****************************************************************
      SUBROUTINE WOLKWAT(XW,XI,CLWATER,CLADWAT,REENWAT,WWATER,WYS,
     *           PC,WBASRS,RSS,TC,DENSA,ITIPWLK,SEK, WBASP, P)

      implicit double precision(A-H), double precision(O-Z)

      CLWATER=WBASRS/1000.-RSS
      CLADWAT=DENSA*CLWATER

***  WOLKYS EN VLOEIBARE WATER
      WWATER=CLADWAT
      WYS=PC*CLADWAT
      XW=WWATER-WYS 
      XI=WYS
      RETURN
      END 


******************************************************************
***  INTERPRETEER WAARDES VAN RS OP VLAK P TUSSEN 2 VLAKKE VAN R
***  (OP DRUKVLAKKE PP)
******************************************************************
      SUBROUTINE INTERP(R,RS,P,IFOUT)

      implicit double precision(A-H), double precision(O-Z)
      DIMENSION R(100)
      COMMON /AAA/ PP(100), ITEL
      IFOUT=0

***  SOEK DIE 2 VLAKKE WEERSKANTE VAN P 
      DO 10 I=1,ITEL-1
      IF(P.LE.PP(I).AND.P.GT.PP(I+1))GOTO 20
10    CONTINUE
      IFOUT=1
      GOTO 30
20    CONTINUE

***  KRY VERHOUDING VAN WAAR P TUSSEN 2 VLAKKE LE 
      PDIFF=PP(I)-PP(I+1)
      VDIFF=PP(I)-P 
      VERH=VDIFF/PDIFF

***  KRY VERSKIL TUSSEN 2 R WAARDES
C     PRINT *,'P=',P,'R(I+1)=',R(I+1),'RI=',R(I),'PPI+1=',PP(I+1)
      RDIFF=ABS(R(I+1)-R(I))

***  BEREKEN NUWE WAARDE
      IF(R(I+1).LT.R(I))THEN
        RS=R(I)-(RDIFF*VERH)
        ELSE
        RS=R(I)+(RDIFF*VERH)
      ENDIF
30    RETURN
      END 


**************************************************************
***  BEREKEN DIE VERSADIGINGS DAMPDIGTHEIDSVERSKIL SI EENHEDE TUSSEN
***  HAELKORREL OPV. EN DIE WOLK OMGEWING, TOV VAN OF WATER OF
***  YS AFHANGENDE VAN DIE WATER/YS VERHOUDING IN OMG., EN
***  OF DIT NAT OF DROE GROEI VAN DIE KORREL IS
**************************************************************
      SUBROUTINE DAMPDIG(DELRW,PC,TS,TC)
      implicit double precision(A-H), double precision(O-Z)
      DATA RV/461.48/,ALV/2500000./,ALS/2836050./ 

***  EERS VIR KORREL: TOETS VIR NATGROEI OF DROE GROEI VAN KORREL
      TSK=TS+273.16 
      TCK=TC+273.16 
      IF(TSK.GE.(0.+273.16))THEN
***  NAT GROEI
        ESAT=611.*EXP(ALV/RV*(1./273.16-1./TSK))
        ELSE
***  DROE GROEI
        ESAT=611.*EXP(ALS/RV*(1./273.16-1./TSK))
      ENDIF

      RHOKOR=ESAT/(RV*TSK)
***  NOU VIR OMG.: IN VERH MET PC
      ESATW=611.*EXP(ALV/RV*(1./273.16-1./TCK))
      RHOOMGW=ESATW/(RV*TCK)
      ESATI=611.*EXP(ALS/RV*(1./273.16-1./TCK))
      RHOOMGI=ESATI/(RV*TCK)
      RHOOMG=PC*(RHOOMGI-RHOOMGW)+RHOOMGW

***  BEREKEN NOU DIE VERSKIL(G/CM3): <0 VIR KONDSASIE, >0 VIR VERDAMPING
      DELRW=(RHOKOR-RHOOMG) / 1000.
      RETURN
      END 


********************************************************************
***  BEREKEN TERMINAAL VALSNELHEID VAN HAELKORREL (SI-EENHEDE)
********************************************************************

      SUBROUTINE TERMINL(DENSA,DENSE,D,VT,TC)
      implicit double precision(A-H), double precision(O-Z)
      DATA B0/-3.18657/,B1/0.992696/,B2/-0.00153193/,
     *B3/-0.0009877059/,B4/-0.000578878/,B5/0.0000855176/,
     *B6/-0.00000327815/,G/9.78956/, PI/3.141592654/

      DENSASI=DENSA*1000.
      DENSESI=DENSE*1000.
      DD=D/100.

      GMASS=(DENSESI*PI*(DD**3.0))/6.0
      TCK=TC+273.16 

***  BEREKEN ANU
      ANU=(0.00001718)*(273.16+120.)/(TCK+120.)*(TCK/273.16)**(3./2.)


C     Calculate the Best Number, X
      GX=(8.0*GMASS*G*DENSASI)/(PI*(ANU)**2.0)
      RE=(GX/0.6)**0.5


c     Select appropriate equations for terminal velocity
      IF(GX.LT.550)GOTO 10
      IF(GX.GE.550.AND.GX.LT.1800)GOTO 20
      IF(GX.GE.1800.AND.GX.LT.3.45E08)GOTO 30
      IF(GX.GE.3.45E08)GOTO 40  

10    CONTINUE
      W=DLOG10(GX)
      Y= -1.7095 + 1.33438*W - 0.11591*(W**2.0)      
      RE=10**Y
      VT=ANU*RE/(DD*DENSASI)
      GOTO 70
20    CONTINUE

      W=DLOG10(GX)
      Y= -1.81391 + 1.34671*W - 0.12427*(W**2.0) + 0.0063*(W**3.0)
      RE=10**Y
      VT=ANU*RE/(DD*DENSASI)
      GOTO 70
30    CONTINUE

      RE=0.4487*(GX**0.5536)
      VT=ANU*RE/(DD*DENSASI)
      GOTO 70
40    CONTINUE

      RE=(GX/0.6)**0.5
      VT=ANU*RE/(DD*DENSASI)
      GOTO 70
70    CONTINUE


      VT=VT*100.
      RE1=RE

      RETURN
      END 


**************************************************************
***  TOETS OF DIE KORREL TE GROOT IS EN BREEK DIT DAN OP
***  DEUR HELFTE VAN DIE WATER AF TE GOOI
**************************************************************
      SUBROUTINE BREAKUP(DENSE,D,GM,FW) 
      implicit double precision(A-H), double precision(O-Z)
      DATA PI/3.141592654/
      
      IF(D.LE.0.9) GOTO 10

      WATER=FW*GM
      GMI=GM-WATER

      CRIT=0.268+0.1389*GMI 
       IF(WATER.GT.CRIT)THEN
           WAT=WATER-CRIT
           GM=GM-WAT
           FW=(CRIT)/GM
       ELSE
           GOTO 10
       ENDIF
       
      IF(FW.GT.1.0) FW=1.0
      IF(FW.LT.0.0) FW=0.0

      DENSE=(FW*(1.0 - 0.9)+0.9)

      D=(6.*GM/(PI*DENSE))**(1./3.)

10    CONTINUE
      RETURN
      END 


      SUBROUTINE LEESDTA(PA,ZA,VUU,R,TAA,TCA,IDAT,WBASP,VBASIS,
     *          ISTNR,ITEL,TMAXT,ITIPWLK, CAPE, SHEAR, UPMAXV, 
     *          UPMAXT, RICH, TDEW, ZBAS, TBAS, TOPT, TOPZ)


      implicit double precision(A-H), double precision(O-Z)
*******************************************************
*** LEES DIE DATA IN
*******************************************************
      DIMENSION TCA(100),R(100),VUU(100),TAA(100),
     *          PA(100),ZA(100)

      READ(1,*)
c      READ(1,'(I10,2X,I5)')IDAT,ISTNR


      READ(1,*)
      READ(1,*)
      READ(1,*)TMAXT,TDEW

C      WRITE(2,3)TMAXT,TIDATUM,TMAX,TDEW,CAPE,SHEAR,RICH,UPMAXV,
c     * UPMAXT,TCBEGIN,BINDEX,TIME,ITIME,D,DTIPE(NRD),TTYD
c3     FORMAT('Max T=',F5.1,' Dew-point=',F5.1)

      READ(1,*)
      READ(1,*)CAPE,SHEAR,RICH,ITIPWLK,PSEUDO
C      WRITE(2,4)CAPE,SHEAR,ITIPWLK,PSEUDO
C4     FORMAT('CAPE=',F6.1,' Shear=',F6.3,
C     *       ' Storm Type=',I2,' Wet adiabat=',F6.2)
      
 
      
      READ(1,*)
      DO 10 I=1,100 
        READ(1,*,END=20,ERR=20)PA(I),HGTE,TAA(I),DUM,TCA(I),
     *                  R(I),VUU(I),ZA(I)
        IF(HGTE.NE.0)WBASP=PA(I)
        IF(HGTE.NE.0)VBEGIN=VUU(I)
        IF(HGTE.NE.0)ZBAS=ZA(I)
        IF(HGTE.NE.0)TBAS=TCA(I)

*** VUU NA CM/S!!
        VUU(I)=VUU(I)*100.0

10    CONTINUE

20    ITEL=I

        UPMAXV=0.0
        
        DO 30 K=1,ITEL  

         IF(UPMAXV.LT.VUU(K))THEN
           UPMAXV=VUU(K)
           UPMAXT=TCA(K)
           UPMAXP=PA(K)

         ENDIF
           IF(TCA(K).LT.0.AND.TCA(K).LT.TAA(K).OR.
     *        VUU(K).LT.0)THEN
              TOPT=TCA(K)
              TOPZ=ZA(K)
              TOPP=PA(K)
              GOTO 35
           ENDIF
30      CONTINUE
35      UPMAXV=UPMAXV/100.0

c35    WRITE(2,36)UPMAXV/100.,UPMAXT,TOPT,TOPP
c36    FORMAT('Updraft Max V=',F6.1,' at ',F5.1,
c     *       ' C,    Cld Top T=',F5.1,' at ',F5.1,' hPa')

      RETURN
      END 
