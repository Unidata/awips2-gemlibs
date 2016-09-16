#!/bin/bash
#
# Dec 15, 2015 mjames@ucar	Created
# Sep 13, 2016 mjames@ucar	Updated for 16.2.2
#
# awips2-ncep/viz/gov.noaa.nws.ncep.ui.nsharp
# awips2-ncep/viz/gov.noaa.nws.ncep.ui.nsharp.macosx
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

# Set C flags.  Include necessary *.h files from application and library functions, e.g., diaglib/dg/*.h
#	gdlist/gdlist.h for gdldsp.c.  Note that diaglib/dg/*.h has layers of *.h, de.h, dl.h, df.h, dv.h.

myLibs="$OS_LIB/ginitp_alt.o $OS_LIB/gendp_alt.o"

myCflags="$CFLAGS -I. -I./Sndglib -I$NSHARP  -I$GEMPAK/include  -I$OS_INC -I$NWX \
-I/opt/X11/include/X11 -I/usr/include/Xm -I/opt/local/include -I/usr/include/malloc -Wcomment -Wno-return-type -Wincompatible-pointer-types -DUNDERSCORE -fPIC -DDEBUG -c"

myFflags="-I. -I$OS_INC -I$GEMPAK/include -I$NSHARP -fPIC -g -c -fno-second-underscore -fmax-errors=200 -std=f95"

# for shared object
myLinkflags="-L/usr/local/Cellar/gcc/4.9.2_1/lib/gcc/4.9/ -L/opt/local/lib -L$OS_LIB -L. -L./Sndglib -L/usr/X11R6/lib \
-shared -Wl -Wcomment -Wincompatible-pointer-types -Wimplicit-function-declaration -Wno-return-type,-install_name,libbignsharp.dylib -o libbignsharp.dylib"

myLibsInc="$OS_LIB/ginitp_alt.o $OS_LIB/gendp_alt.o $OS_LIB/libnxmlib.a $OS_LIB/libsnlist.a \
 $OS_LIB/libsflist.a $OS_LIB/libgemlib.a $OS_LIB/libcgemlib.a $OS_LIB/libgplt.a $OS_LIB/libdevice.a \
 $OS_LIB/libxwp.a $OS_LIB/libxw.a $OS_LIB/libps.a  $OS_LIB/libgn.a $OS_LIB/libcgemlib.a $OS_LIB/libgemlib.a \
 $OS_LIB/libnetcdf.a $OS_LIB/libtextlib.a $OS_LIB/libxml2.a $OS_LIB/libxslt.a \
 $OS_LIB/libgemlib.a $OS_LIB/libcgemlib.a $OS_LIB/librsl.a $OS_LIB/libbz2.a"

myLinktail="-I$OS_INC \
  -I$GEMPAK/include -I$NWX -I$NSHARP -I. -I./Sndglib  -I/opt/X11/include/X11 -I/usr/include -I/usr/include/Xm -I/opt/local/include/ -I/opt/local/include -lhdf5 -lgfortran -ljasper -lpng -liconv -lc -lXt -lX11 -lz -lm -lXm"

$CC $myCflags *.c Sndglib/*.c
$FC $myFflags *.f
$CC $myLinkflags *.o $myLibsInc $myLinktail

echo ""
echo "$CC $myCflags *.c Sndglib/*.c"
echo ""
echo "$FC $myFflags *.f"
echo ""
echo "$CC $myLinkflags *.o $myLibsInc $myLinktail"

#cp libbignsharp.dylib ~/awips2-ncep/viz/gov.noaa.nws.ncep.ui.nsharp.macosx/

exit
