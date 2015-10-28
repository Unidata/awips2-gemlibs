#include "geminc.h"
#include "gemprm.h"
#include "dbcmn.h"

void db_returnnav(char *result)
/************************************************************************
 *                                                                      *
 * db_returnnav                                                         *
 *                                                                      *
 * m.gamazaychikov/CWS	06/11   Created                                 *
 ************************************************************************/
{
/*----------------------------------------------------------------------*/
   
    if ( result != NULL ) {
       if ( strlen(result) > 0 ) {
          G_MALLOC ( navStrBack, char, strlen(result)+1, "Error allocating navStrBack" );
          sprintf (navStrBack, "%s", result);
          navStrLength = strlen(result);
       }
       else {
          navStrBack = NULL;
          navStrLength = 0;
       }
    }
    else {
       navStrBack = NULL;
       navStrLength = 0;
    }
    return;
}
