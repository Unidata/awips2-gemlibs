#include "geminc.h"
#include "gemprm.h"
#include "dbcmn.h"

int db_navCallback(void (*aNavCallback)(char *))
{

   navClbkPtr = aNavCallback;
   return(0);
}
