#!/bin/bash
CC=gcc
FC=gfortran
echo "OS_INC = $OS_INC"
echo "OS_LIB = $OS_LIB"
echo "GEMPAK = $GEMPAK"
cd /Users/mj/awips2-gemlibs/aodt
echo "working in "`pwd`


echo "Compiling program... "
gcc -fPIC -g -c -Wall *.c *.h

echo "Creating a Shared Library object... "
#gcc -shared -Wl,-soname,libaodtv64.so -o libaodtv64.so *.o -lc

#myLinkflags="-L/usr/local/Cellar/gcc/4.9.2_1/lib/gcc/4.9/ -L/opt/local/lib \
#-shared -Wl -Wno-return-type,-install_name,libcnflib.dylib -o libcnflib.dylib"

gcc -shared -Wl,-Wno-return-type,-install_name,libaodtv64.dylib -o libaodtv64.dylib *.o -lc

echo "library is created and cp to build.cave\n "
echo " "
#cp libaodtv64.so $DEV_BASE/workspace/build.cave/static/common/cave/caveEnvironment/lib
endif
