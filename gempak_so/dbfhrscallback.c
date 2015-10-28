#include "geminc.h"
#include "gemprm.h"
#include "dbcmn.h"

int db_fhrsCallback(void (*aFhrsCallback)(void))
{

   fhrsClbkPtr = aFhrsCallback;
   return(0);
}
