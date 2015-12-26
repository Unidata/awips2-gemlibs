#!/bin/bash
#
# Updated for Unidata AWIPS II OS X, December 2015
#
# 1. Create a Java project and source folder....
#
# 2. Configure build path (see M. Li's lecture note):
#	Right click the working project -> Build Path -> Configure build path
#	-> Libraries -> Add JARs -> com.sun.jna – jna.jar
#
# 3. cp /usr/lib/gcc/i386-redhat-linux/3.4.6/libg2c.so locally, e.g.,
#	$AWIPS2/tools/lib and make myLinkFlags to contain -L$AWIPS2/tools/lib
#
# 4. Use this script to create a shared library (.so)
#
# 5. Deploy the SL and add the path, e.g., AWIPS2/lib, to LD_LIBRARY_PATH
#    (in ~/.alias)
#
. ~/gempak/Gemenviron.profile
export NSHARP=$GEMPAK/source/programs/gui/nsharp
export NWX=$GEMPAK/source/programs/gui/nwx

CC=gcc
FC=gfortran

echo "OS_INC = $OS_INC"
echo "OS_LIB = $OS_LIB"
echo "GEMPAK = $GEMPAK"
echo "NWX    = $NWX"
cd /Users/mj/awips2-gemlibs/nsharp
echo "working in "`pwd`

#  check that libraries exist
libs=(snlist sflist nxmlib gemlib gplt cgemlib rsl device xwp xw ps gn nsharp netcdf textlib)
for file in ${libs[@]}
do
  if [ ! -f $OS_LIB/lib$file.a ]; then
    echo "$OS_LIB/lib$file.a does not exist"
    if [ -f $OS_LIB/$file.a ]; then
      cp $OS_LIB/$file.a $OS_LIB/lib$file.a
      echo "copied OS_LIB/$file.a to OS_LIB/lib$file.a for linking"
    fi
  fi
done

#rm -rf *.o glibnsharp.so Sndglib/*.o

# -L/usr/local/Cellar/gcc/4.9.2_1/lib/gcc/4.9/ is for -lgfortran

#myLinktail=" \
#$OS_LIB/ginitp_alt.o $OS_LIB/gendp_alt.o -lnxmlib  \
#-lsndg -lsnlist -lsflist -lgemlib -lgplt -lcgemlib -ldevice -lxwp -lxw -lps -lgn -lnsharp -lgemlib -lrsl -lnetcdf \
#-ltextlib -lxml2 -lxslt -liconv -lbz2 -lhdf5 -lgfortran -ljasper -lpng -liconv -lc -lXt -lX11 -lz -lm"

#
# Set C flags.  Include necessary *.h files from application and library functions, e.g., diaglib/dg/*.h
#	gdlist/gdlist.h for gdldsp.c.  Note that diaglib/dg/*.h has layers of *.h, de.h, dl.h, df.h, dv.h.
#
# We need to compile here, not link.
#CFLAGS="-DUNDERSCORE -DLinux -Wno-return-type -I. -I./Sndglib -I$OS_INC -I$GEMPAK/include \
#        -I$NWX -I$NSHARP -I/opt/X11/include -I/usr/include -I/opt/local/include"
#myCflags="$CFLAGS -DUNDERSCORE -fPIC -DDEBUG -c"


myLibs="$OS_LIB/ginitp_alt.o $OS_LIB/gendp_alt.o"
#$OS_LIB/libsnlist.a $OS_LIB/libsflist.a \
#$OS_LIB/libnxmlib.a $OS_LIB/libgemlib.a $OS_LIB/libgplt.a \
#$OS_LIB/libcgemlib.a $OS_LIB/libdevice.a $OS_LIB/libxwp.a \
#$OS_LIB/libxw.a $OS_LIB/libps.a $OS_LIB/libgn.a $OS_LIB/librsl.a $OS_LIB/libgemlib.a $OS_LIB/libgplt.a $OS_LIB/libnetcdf.a \
#$OS_LIB/libtextlib.a $OS_LIB/libxml2.a $OS_LIB/libxslt.a $OS_LIB/libbz2.a $OS_LIB/libnsharp.a"
#LIBS   = -L./Sndglib -lsndg \
#       $(OS_LIB)/ginitp_alt.o $(OS_LIB)/gendp_alt.o \
#       -L$(OS_LIB) -lsnlist -lsnlib -lsflist -lsflib -lnxmlib -ldiaglib \
#       -lgemlib -lprmcnvlib -lgridlib -lgplt -lgridlib -lcgemlib \
#       -ldevice -lxwp -lxw -lps -lgn -lgemlib -lnetcdf \
#       -ltextlib -lxml2 -lxslt -liconv -lbz2 \
#       $(XLIBS) -lz -lm

# clang -I/opt/local/include -I./Sndglib -I/Users/mj/gempak/gempak/include
# -I/Users/mj/gempak/gempak/source/programs/gui/nsharp -I/Users/mj/gempak/os/darwin/include
# -I/Users/mj/gempak/gempak/source/programs/gui/nwx -I/usr/include -I/opt/local/lib -DUNDERSCORE -fPIC -DDEBUG -c *.c Sndglib/*.c

##
## SET
##

myCflags="$CFLAGS -I. -I./Sndglib -I$NSHARP  -I$GEMPAK/include  -I$OS_INC -I$NWX \
-I/opt/X11/include/X11 -I/usr/include/Xm -I/opt/local/include -I/usr/include/malloc -Wno-return-type -DUNDERSCORE -fPIC -DDEBUG -c"

myFflags="-I. -I$OS_INC -I$GEMPAK/include -I$NSHARP -fPIC -g -c -Wtabs -fno-second-underscore"

# for shared object
# FLAGS
myLinkflags="
-L/usr/local/Cellar/gcc/4.9.2_1/lib/gcc/4.9/ -L/opt/local/lib -L$OS_LIB -L. -L./Sndglib -L/usr/X11R6/lib \
-shared -Wl -Wno-return-type,-install_name,libbignsharp.dylib -o libbignsharp.dylib"

# -L$OS_LIB -L./Sndglib -L/usr/X11R6/lib -I. -I./Sndglib -I$OS_INC \
 #-I$GEMPAK/include -I$NWX -I$NSHARP -I/opt/X11/include -I/usr/include -I/usr/include/Xm -I/opt/local/include/ #-I/opt/local/include


# INCLUDE
 myLibsInc="$OS_LIB/ginitp_alt.o $OS_LIB/gendp_alt.o $OS_LIB/libnxmlib.a $OS_LIB/libsnlist.a \
 $OS_LIB/libsflist.a $OS_LIB/libgemlib.a $OS_LIB/libcgemlib.a $OS_LIB/libgplt.a $OS_LIB/libdevice.a \
 $OS_LIB/libxwp.a $OS_LIB/libxw.a $OS_LIB/libps.a  $OS_LIB/libgn.a $OS_LIB/libcgemlib.a $OS_LIB/libgemlib.a \
 $OS_LIB/libnetcdf.a $OS_LIB/libtextlib.a $OS_LIB/nwx.a  $OS_LIB/nsharp.a  $OS_LIB/libxml2.a $OS_LIB/libxslt.a \
 $OS_LIB/libgemlib.a $OS_LIB/libcgemlib.a $OS_LIB/librsl.a $OS_LIB/libbz2.a"


# TAIL
 myLinktail="-I. -I./Sndglib -I$OS_INC \
  -I$GEMPAK/include -I$NWX -I$NSHARP -I/opt/X11/include/X11 -I/usr/include -I/usr/include/Xm -I/opt/local/include/ -I/opt/local/include -lhdf5 -lgfortran -ljasper -lpng -liconv -lc -lXt -lX11 -lz -lm -lXm"
##
## RUN
##

echo ""
echo "$CC $myCflags *.c Sndglib/*.c"
echo ""
echo "$FC $myFflags *.f"
echo ""
echo "$CC $myLinkflags *.o $myLibsInc $myLinktail"

$CC $myCflags *.c Sndglib/*.c
$FC $myFflags *.f
$CC $myLinkflags *.o $myLibsInc $myLinktail
#	gcc "$myLinkflags" *.o $OS_LIB/libgdlist.a $myLibs $myLinktail"

echo ""
echo "$CC $myCflags *.c Sndglib/*.c"
echo ""
echo "$FC $myFflags *.f"
echo ""
echo "$CC $myLinkflags *.o $myLibsInc $myLinktail"

exit

check=`ls -altr libbignsharp.*`
echo $check

exit

echo ""
if [[ $check == "libbignsharp.so" ]]; then
	echo "****** Shared library is created ****** "
	echo ""
#	cp libbignsharp.so $DEV_BASE/workspace/gov.noaa.nws.ncep.ui.nsharp.linux32
else
	echo "****** Houston, we got problems ****** "
  echo ""
fi

exit
