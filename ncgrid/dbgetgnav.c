#include "geminc.h"
#include "gemprm.h"
#include "dbcmn.h"
#include <sys/timeb.h>

void db_getgnav (char *model, char *eventName,char *navTime, char *gridNav, int *lGridNav, int *iret )  
/************************************************************************
 *									*
 * db_getgnav               						*
 *									*
 * m.gamazaychikov/SAIC	11/08	Created                                 *
 ************************************************************************/
{
    int      ier, ier1;
    char     queryText[320];
/*----------------------------------------------------------------------*/
  /*
   * If the pointer to the callback function is not set, return
   */
    if ( navClbkPtr == NULL ) {
       ier = -17;
       er_wmsg ( "DB", &ier, "db_getgnav", &ier1, 2, strlen("db_getgnav") );
       *iret = -1;
       return;
    }

  /*
   * Initialization
   */
    *iret = 0;
    *lGridNav = 0;
    gridNav[0] = '\0';
    sprintf (queryText, "%s|%s|%s",model,eventName,navTime);
   /*
    * Execute the callback to get the navigation
    */
    navClbkPtr(queryText);
    if ( navStrLength > 0 && navStrBack != NULL ) {
       sprintf (gridNav, "%s", navStrBack);
       *lGridNav = strlen(gridNav);
       G_FREE( navStrBack, char );
    }
    else {
       ier = -18;
       er_wmsg ( "DB", &ier, "Navigation", &ier1, 2, strlen("Navigation") );
       *iret = -1;
       return;
    }
       
    return;

}
