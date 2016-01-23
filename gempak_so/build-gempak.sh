#!/bin/bash
# 1. Create a Java project and source folder....
# 2. Configure build path (see M. Li's lecture note):
#	Right click the working project -> Build Path -> Configure build path
#	-> Libraries -> Add JARs -> com.sun.jna – jna.jar
# 3. cp /usr/lib/gcc/i386-redhat-linux/3.4.6/libg2c.so locally, e.g.,
#	$AWIPS2/lib/so
# 4. Use this script to create a shared library (.so)
# 5. Deploy the SL and add the path, e.g., AWIPS2/lib, to LD_LIBRARY_PATH
#    (in ~/.alias)

. ~/gempak/Gemenviron.profile
CC=gcc
FC=gfortran
echo "OS_INC = $OS_INC"
echo "OS_LIB = $OS_LIB"
echo "GEMPAK = $GEMPAK"
cd /Users/mj/awips2-gemlibs/gempak_so
echo "working in "`pwd`

myCflags="$CFLAGS -I. -I$GEMPAK/source/diaglib/dg -I$GEMPAK/source/gemlib/er \
 -I/opt/X11/include/X11 -I/usr/include/Xm -I/opt/local/include -I/usr/include/malloc -Wno-return-type -DUNDERSCORE  -fPIC -DDEBUG -c"

myFflags="-I. -I$OS_INC -I$GEMPAK/include -fPIC -g -c -Wtabs -fno-second-underscore"


myLinkflags="-L/usr/local/Cellar/gcc/4.9.2_1/lib/gcc/4.9/ -L/opt/local/lib -L/usr/X11R6/lib \
-shared -Wl -Wno-return-type,-install_name,libgempak.dylib -o libgdtest.dylib"

myLinktail="-lgfortran $OS_LIB/libjasper.a -lpng  -lc"

myLibs="-I$OS_INC -I$GEMPAK/include -I.  \
$OS_LIB/ginitp_alt.o \
$OS_LIB/gendp_alt.o \
$OS_LIB/libsflist.a \
$OS_LIB/gdlist.a \
$OS_LIB/libgemlib.a \
$OS_LIB/libcgemlib.a \
$OS_LIB/libgplt.a \
$OS_LIB/libdevice.a \
$OS_LIB/libgn.a \
$OS_LIB/libcgemlib.a \
$OS_LIB/libgemlib.a \
$OS_LIB/libnetcdf.a \
$OS_LIB/libtextlib.a \
$OS_LIB/libxslt.a \
$OS_LIB/libxml2.a \
-liconv \
$OS_LIB/libz.a \
$OS_LIB/librsl.a \
$OS_LIB/libgemlib.a \
-lbz2"

echo ""
echo "$CC $myCflags *.c"
echo ""
echo "$FC $myFflags *.f"
echo ""
echo "$CC $myLinkflags *.o $myLibs $myLinktail"

pwd
exit

$CC $myCflags *.c
$FC $myFflags *.f
$CC $myLinkflags *.o $myLibs $myLinktail

exit
