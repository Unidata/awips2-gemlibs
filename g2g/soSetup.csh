#!/bin/csh
#setenv OS_LIB "/export-1/cdbsrv/nawdev/nawips/os/linux2.6.18el32/lib"

setenv myLinkflags "-L/usr/lib/gcc/i386-redhat-linux/3.4.6 -shared -Wl,-soname,libg2g.so -o libg2g.so"

setenv myFflags "-I/export-1/cdbsrv/nawdev/nawips/gempak/include -fPIC -g -c -Wall -fno-second-underscore"

setenv myCflags "-I/export-1/cdbsrv/nawdev/nawips/gempak/include -I/export-1/cdbsrv/nawdev/nawips/os/linux2.6.18el32/include -DLinux -DUNDERSCORE -I/usr/X11R6/include -g -Wall -W -Wno-unused-parameter -Wfloat-equal -Wundef -Wpointer-arith -Wbad-function-cast -Wcast-align -Wsign-compare -Waggregate-return -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wmissing-noreturn -Wpacked -Wredundant-decls -Wnested-externs -Wunreachable-code -Winline -Wlong-long -g -Wall -W -Wno-unused-parameter -Wfloat-equal -Wundef -Wpointer-arith -Wbad-function-cast -Wcast-align -Wsign-compare -Waggregate-return -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wmissing-noreturn -Wpacked -Wredundant-decls -Wnested-externs -Wunreachable-code -Winline -Wlong-long -I/export-1/cdbsrv/nawdev/nawips/os/linux2.6.18el32/include/libxml2 -DDEBUG -c"

echo $myCflags

setenv myLibs  "/export-1/cdbsrv/nawdev/nawips/os/linux2.6.18el32/lib/libgemlib.a /export-1/cdbsrv/nawdev/nawips/os/linux2.6.18el32/lib/libtextlib.a /export-1/cdbsrv/nawdev/nawips/os/linux2.6.18el32/lib/libxslt.a /export-1/cdbsrv/nawdev/nawips/os/linux2.6.18el32/lib/libxml2.a /export-1/cdbsrv/nawdev/nawips/os/linux2.6.18el32/lib/libiconv.a /export-1/cdbsrv/nawdev/nawips/os/linux2.6.18el32/lib/libz.a"
#setenv myLibs "$OS_LIB/libgemlib.a $OS_LIB/libtextlib.a $OS_LIB/libxslt.a $OS_LIB/libxml2.a $OS_LIB/libiconv.a $OS_LIB/libz.a"

echo $myLibs

setenv myLinktail  "-lg2c -lc"

echo "my..flags have been set"
echo "to compile C program:       gcc "$myCflags" *.c"
echo "to compile Fortran program: g77 "$myFflags" *.f"
#echo "to create a so object: gcc "\$myLinkflags" *.o lib/cgemlib/*.o "\$myLibs" "\$myLinktail
echo "to create a so object: gcc "$myLinkflags" *.o "$myLibs" "$myLinktail
echo "now to deploy the so: cp libdmtest.so "\$GEMPAK_SO"\lib\."
echo "do not forget to add "\$GEMPAK_SO"\lib to your "\$LD_PRELOAD_PATH
