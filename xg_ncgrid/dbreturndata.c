#include "geminc.h"
#include "gemprm.h"
#include "dbcmn.h"

void db_returndata(float *result, int *nrez)
{
    int jj;
/*----------------------------------------------------------------------*/
    if ( *nrez != 0 && result != NULL ) {
       if ( *nrez > 0 ) {
         rdataBackSize = *nrez;
         G_MALLOC ( rdataBack, float, rdataBackSize,
                 "Error allocating rdataBack grid" );
         for( jj=0; jj < rdataBackSize; jj++ ) {
            rdataBack[jj] = result[jj];
         }
       }
       else {
         rdataBack = NULL;
         rdataBackSize = 0;
       }
    }
    else {
       rdataBack = NULL;
       rdataBackSize = 0;
    }
    return;
}
