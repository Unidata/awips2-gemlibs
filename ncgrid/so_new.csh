#!/bin/csh
#
# 1. Create a Java project and source folder....
#
# 2. Configure build path (see M. Li's lecture note): 
#	Right click the working project -> Build Path -> Configure build path 
#	-> Libraries -> Add JARs -> com.sun.jna – jna.jar
#
# 3. cp /usr/lib/gcc/i386-redhat-linux/3.4.6/libg2c.so locally, e.g.,
#	$AWIPS2/lib/so 
#
# 4. Use this script to create a shared library (.so)
#
# 5. Deploy the SL and add the path, e.g., AWIPS2/lib, to LD_LIBRARY_PATH 
#    (in ~/.alias)
#

$RM *.o *.so

if ( $NA_OS == "linux2.6.18_i686" ) then
  set build_base = "/export-1/cdbsrv/nawopr/nawips"
else
  set build_base = "/export/nco-lw-sjacobs/scottj/gempak/workspace/trunk/nawips"
endif

echo $build_base

set build_lib = "$build_base/os/$NA_OS/lib"
set build_gem = "$build_base/gempak"

set machtype = `uname -i`

echo $build_base
echo $build_lib
echo $build_gem
echo $machtype

if ( $NA_OS == "linux2.6.18_i686" ) then
  set myBZlib = "$build_lib/libbz2.a"
else
  set myBZlib = "-lbz2"
endif

#
# Set Link flags
#
set myLinkflags = "-L/usr/lib/gcc/${machtype}-redhat-linux/3.4.6/ -shared -Wl,-soname,libgempak.so -o libgdtest.so"
set myLinktail = "-lg2c -lc"

#
# Set C flags.
# Include necessary *.h files from application and library functions,
#     e.g., diaglib/dg/*.h 
#     gdlist/gdlist.h for gdldsp.c.
#     Note that diaglib/dg/*.h has layers of *.h, de.h, dl.h, df.h, dv.h.
#
#set myCflags = "-fPIC $CFLAGS -I$build_gem/source/diaglib/dg -I$build_gem/source/gemlib/er -DDEBUG -c"
set myCflags = "-fPIC -I$build_gem/include -I$build_base/os/$NA_OS/include -DLinux -DUNDERSCORE -I/usr/X11R6/include -DG_64BIT -I$build_gem/source/diaglib/dg -I$build_gem/source/gemlib/er -DDEBUG -c"

#
# Set F flags.
#
set myFflags = "-fPIC -g -c -Wall -fno-second-underscore"

echo
echo "Cflags and Fflags have been set"
echo " "

#
# Add libraries for gdlist (following gdlist.mk)
#


set myLibs = "${build_lib}/ginitp_alt.o ${build_lib}/gendp_alt.o ${build_lib}/libsflist.a ${build_lib}/libgdlist.a ${build_lib}/libdiaglib.a ${build_lib}/libgridlib.a ${build_lib}/libprmcnvlib.a ${build_lib}/libgemlib.a ${build_lib}/libsflib.a ${build_lib}/libgplt.a ${build_lib}/libdevice.a ${build_lib}/libgn.a ${build_lib}/libcgemlib.a ${build_lib}/libgemlib.a ${build_lib}/libnetcdf.a ${build_lib}/libtextlib.a ${build_lib}/libxslt.a ${build_lib}/libxml2.a ${build_lib}/libiconv.a ${build_lib}/libz.a ${myBZlib}"

#
# Compile all C programs
#
echo "Compiling C programs... "
echo " "
$CC $myCflags *.c

#
# Compile all Fortran  programs
#
echo "Compiling Fortran programs... "
echo " "
$FC $myFflags *.f

#
# Create a shared library
#
echo "Creating a Shared Library object... "
#echo "$CC $myLinkflags *.o $myLibs $myLinktail"
$CC $myLinkflags *.o $myLibs $myLinktail

set newlib = `ll -altr *.so | awk '{print $9}'`

echo " "
if ( $newlib == "libgdtest.so" ) then
	echo " "
	echo "****** SUCCESS creating shared library ******"
	echo " "
	mkdir $machtype
	cp libgdtest.so $machtype/libgempak_${machtype}.so
	mv libgdtest.so $machtype/libgdtest_${machtype}.so
        /bin/rm *.o
else
        echo " "
	echo "****** FAILED to create shared library ******"
	echo " "
endif

exit
