#include "geminc.h"
#include "gemprm.h"
#include "dbcmn.h"

void db_returnfhrs(char *result)
/************************************************************************
 *                                                                      *
 * db_returnfhrs                                                         *
 *                                                                      *
 * m.gamazaychikov/CWS	06/11   Created                                 *
 ************************************************************************/
{
/*----------------------------------------------------------------------*/
   
    if ( result != NULL ) {
       if ( strlen(result) > 0 ) {
          G_MALLOC ( fhrsStrBack, char, strlen(result)+1, "Error allocating fhrsStrBack" );
          sprintf (fhrsStrBack, "%s", result);
          fhrsStrLength = strlen(result);
       }
       else {
          fhrsStrBack = NULL;
          fhrsStrLength = 0;
       }
    }
    else {
       fhrsStrBack = NULL;
       fhrsStrLength = 0;
    }
    return;
}
