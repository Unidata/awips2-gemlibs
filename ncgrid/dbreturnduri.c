#include "geminc.h"
#include "gemprm.h"
#include "dbcmn.h"

void db_returnduri(char *result)
{
/*----------------------------------------------------------------------*/
   
    if ( result != NULL ) {
       if ( strlen(result) > 0 ) {
          G_MALLOC ( duriStrBack, char, strlen(result)+1, "Error allocating duriStrBack" );
          sprintf (duriStrBack, "%s", result);
          duriStrLength = strlen(result);
       }
       else {
          duriStrBack = NULL;
          duriStrLength = 0;
       }
    }
    else {
       duriStrBack = NULL;
       duriStrLength = 0;
    }
    /*
    */
    return;

}
