#include "geminc.h"
#include "gemprm.h"
#include "dbcmn.h"
#include <sys/timeb.h>

void db_scandb ( const char *modelName, const char *dbTag, const char *ensTag, 
           const char *timeTmplt, char *fileNames, int *iret)
/************************************************************************
 *                                        *
 * db_scandb                              *
 *                                        *
 * m.gamazaychikov/SAIC 09/11   Created                     *
 ************************************************************************/
{
    int      ier, ier1;
    char     queryType[25];
    char     queryText[320];

    struct   timeb t_callback, t_current;
/*---------------------------------------------------------------------*/
    *iret = 0;
    if ( flnmClbkPtr == NULL ) {
       ier = -17;
       er_wmsg ( "DB", &ier, "db_scandb", &ier1, 2, strlen("db_scandb") );
       *iret = ier;
       return;
    }

   /*
    * Populate the query strings 
    */
    fileNames[0] = '\0';
    queryText[0] = '\0';
    gEventName[0] = '\0';
    sprintf ( queryType, "%s", "scanDb" );
    sprintf ( ePlugIn,   "%s", "ncgrib" );
    sprintf ( eSrc, "%s", "GRID" );
    sprintf ( eParameters, "%s|%s|%s|%s", modelName,dbTag,ensTag,timeTmplt );
    sprintf ( eLibClass, "%s", "GempakScanDbRequest" );
   /*
    * Get the query text
    */
/*    db_getQueryText ( queryType, queryText, &ier );
     if ( ier !=0 ) {
       ier = -3;
       er_wmsg ( "DB", &ier, NULL, &ier1, 2, 0 );
       *iret = ier;
       return;
    }
*/
   /*
    * Execute the callback to get the filenames
    */
    ftime(&t_callback);
//    flnmClbkPtr(queryText);
    flnmClbkPtr (eParameters);
    ftime(&t_current);
    if ( flnmStrLength > 0 && flnmStrBack != NULL ) {
       strcpy (fileNames, flnmStrBack);
       G_FREE( flnmStrBack, char );
    }
    else {
       ier = -18;
       er_wmsg ( "DB", &ier, "fileNames", &ier1, 2, strlen("fileNames") );
       *iret = ier;
       return;
    }

    return;
}
