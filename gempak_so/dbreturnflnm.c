#include "geminc.h"
#include "gemprm.h"
#include "dbcmn.h"

void db_returnflnm(char *result)
{
/*----------------------------------------------------------------------*/
   
    if ( flnmStrBack != NULL ) {
       G_FREE( flnmStrBack, char );
    }
    if ( result != NULL ) {
       if ( strlen(result) > 0 ) {
          G_MALLOC ( flnmStrBack, char, strlen(result)+1, "Error allocating flnmStrBack" );
          sprintf (flnmStrBack, "%s", result);
          flnmStrLength = strlen(result);
       }
       else {
          flnmStrBack = NULL;
          flnmStrLength = 0;
       }
    }
    else {
       flnmStrBack = NULL;
       flnmStrLength = 0;
    }
    /*
    */
    return;

}
