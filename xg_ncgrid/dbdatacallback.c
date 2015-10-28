#include "geminc.h"
#include "gemprm.h"
#include "dbcmn.h"

int db_dataCallback(void (*aDataCallback)(char*))
{

   dataClbkPtr = aDataCallback;
   return(0);
}
