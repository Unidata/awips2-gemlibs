#include "geminc.h"
#include "gemprm.h"
#include "dbcmn.h"
#include <sys/timeb.h>

void db_rdtrgrid ( char *dimx, char *dimy, char *dataUri,
                   float *rdata, int *nword, int *iret )
/************************************************************************
 *									*
 * db_dataquery               						*
 *									*
 * m.gamazaychikov/SAIC	11/08	Created                                 *
 ************************************************************************/
{
    int      jj, ier, ier1;
    char     message[1024];
    struct   timeb t_callback, t_current;
/*---------------------------------------------------------------------*/
    message[0] = '\0';
  /*
   * If the pointer to the callback function is not set, return
   */
    if ( dataClbkPtr == NULL ) {
       ier = -17;
       er_wmsg ( "DB", &ier, "db_rdtrgrid", &ier1, 2, strlen("db_rdtrgrid") );
       *iret = -1;
       return;
    }

  /*
   * If the data uri is not set, return
   */
    if ( strlen(dataUri) == 0 ) {
       ier = -19;
       er_wmsg ( "DB", &ier, "db_rdtrgrid", &ier1, 2, strlen("db_rdtrgrid") );
       *iret = -1;
       return;
    }

    sprintf(message,"%s", dataUri);
    strcat (message, ";");
    strcat (message, dimx);
    strcat (message, ";");
    strcat (message, dimy);

    //ftime(&t_callback);

    dataClbkPtr(message);

    //ftime(&t_current);
    if ( rdataBackSize > 0 && rdataBack != NULL ) {
       for( jj=0; jj < rdataBackSize; jj++ ) {
          rdata[jj] = rdataBack[jj];
       }
       *iret = 0;
       *nword = rdataBackSize;
       G_FREE ( rdataBack, float );
       rdataBackSize = 0;
    }
    else {
       *iret = -1;
       *nword = 0;
    }
    return;
}
