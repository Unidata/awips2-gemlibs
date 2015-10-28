#include "geminc.h"
#include "gemprm.h"
#include "dbcmn.h"

int db_duriCallback(void (*aDuriCallback)(char*))
{

   duriClbkPtr = aDuriCallback;
   return(0);
}
