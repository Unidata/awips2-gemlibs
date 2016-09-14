#!/bin/bash
#
# Dec 15, 2015 mjames@ucar	Created
# Sep 13, 2016 mjames@ucar	Updated for 16.2.2 using awips2-ncep/viz/gov.noaa.nws.ncep.ui.nsharp
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

myLibs="$OS_LIB/ginitp_alt.o $OS_LIB/gendp_alt.o"

myCflags="$CFLAGS -I. -I./Sndglib -I$NSHARP  -I$GEMPAK/include  -I$OS_INC -I$NWX \
-I/opt/X11/include/X11 -I/usr/include/Xm -I/opt/local/include -I/usr/include/malloc -Wcomment -Wno-return-type -Wincompatible-pointer-types -DUNDERSCORE -fPIC -DDEBUG -c"

myFflags="-I. -I$OS_INC -I$GEMPAK/include -I$NSHARP -fPIC -g -c -fno-second-underscore -fmax-errors=200 -std=f95"

# for shared object
myLinkflags="-L/usr/local/Cellar/gcc/4.9.2_1/lib/gcc/4.9/ -L/opt/local/lib -L$OS_LIB -L. -L./Sndglib -L/usr/X11R6/lib \
-shared -Wl -Wcomment -Wincompatible-pointer-types -Wimplicit-function-declaration -Wno-return-type,-install_name,libbignsharp.dylib -o libbignsharp.dylib"

# INCLUDE
myLibsInc="$OS_LIB/ginitp_alt.o $OS_LIB/gendp_alt.o $OS_LIB/libnxmlib.a $OS_LIB/libsnlist.a \
 $OS_LIB/libsflist.a $OS_LIB/libgemlib.a $OS_LIB/libcgemlib.a $OS_LIB/libgplt.a $OS_LIB/libdevice.a \
 $OS_LIB/libxwp.a $OS_LIB/libxw.a $OS_LIB/libps.a  $OS_LIB/libgn.a $OS_LIB/libcgemlib.a $OS_LIB/libgemlib.a \
 $OS_LIB/libnetcdf.a $OS_LIB/libtextlib.a $OS_LIB/libxml2.a $OS_LIB/libxslt.a \
 $OS_LIB/libgemlib.a $OS_LIB/libcgemlib.a $OS_LIB/librsl.a $OS_LIB/libbz2.a"

# TAIL
myLinktail="-I$OS_INC \
  -I$GEMPAK/include -I$NWX -I$NSHARP -I. -I./Sndglib  -I/opt/X11/include/X11 -I/usr/include -I/usr/include/Xm -I/opt/local/include/ -I/opt/local/include -lhdf5 -lgfortran -ljasper -lpng -liconv -lc -lXt -lX11 -lz -lm -lXm"
## RUN
$CC $myCflags *.c Sndglib/*.c
$FC $myFflags *.f
$CC $myLinkflags *.o $myLibsInc $myLinktail


echo ""
echo "$CC $myCflags *.c Sndglib/*.c"
echo ""
echo "$FC $myFflags *.f"
echo ""
echo "$CC $myLinkflags *.o $myLibsInc $myLinktail"
pwd
cp libbignsharp.dylib ~/awips2-ncep/viz/gov.noaa.nws.ncep.ui.nsharp.macosx/

exit
