#include "geminc.h"
#include "gemprm.h"
#include "dbcmn.h"

#define A2DB_TAG	"A2DB_"

extern  A2Data_t        A2DtTable;

void db_a2dtget ( char *alias, char *path, char *template, int *iret )  
/************************************************************************
 *									*
 * db_a2dtget               						*
 *									*
 * m.gamazaychikov/SAIC	09/11	Created                                 *
 ************************************************************************/
{
    int      ier, ii, ic, ipos, ic2, ipos2;
    char     name[49], tmpl[49];

/*----------------------------------------------------------------------*/

   /*
    * Initialization
    */
    *iret = 0;
    path[0] = '\0';
    template[0] = '\0';
    

    db_isdbfile ( alias, &ier,  strlen (alias) ); 

   /*
    * For the A2DB data create path and template
    */
    if ( ier == 0 ) {

      /*
       * Remove the name of the storm/volcano from the alias.
       */
      /*
       * Find a match for the alias.
       */
       for ( ii = 0; ii < A2DtTable.numtmpl; ii++ )  {
          strcpy( path, A2DtTable.info[ii].path );
          cst_nocc ( A2DtTable.info[ii].template, '*', 1, 0, &ipos2, &ic2 );
          if ( ic2 == 0 ) {
            /*
             * Remove the name of the storm/volcano from the alias.
             */
             cst_nocc ( alias, ':', 1, 0, &ipos, &ic );
             if  ( ic == 0 )  {
                strcpy ( name, &alias[ipos+1] );
                alias[ipos] = CHNULL;
             }
             if ( strcmp( A2DtTable.info[ii].alias, alias ) == 0 )  {
                if  ( ic == 0 )  {
                   strcpy( tmpl, A2DtTable.info[ii].template );
                   cst_rpst ( tmpl, "*", name, template, &ier );
                }
                else {
                   strcpy( template, A2DtTable.info[ii].template );
                }
             }
          }
          else {
             if ( strcmp( A2DtTable.info[ii].alias, alias ) == 0 )  {
                strcpy( template, A2DtTable.info[ii].template );
             }
          }
       }
   /*
    * For the non-A2DB data return
    */
    }
    else {
       *iret = -1;
    }
  
       
    return;

}
