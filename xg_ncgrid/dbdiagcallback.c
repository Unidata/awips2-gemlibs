#include "geminc.h"
#include "gemprm.h"
#include "dbcmn.h"

int db_diagCallback(void (*aDiagCallback)(char*))
{

   diagClbkPtr = aDiagCallback;
   return(0);
}
