#include "geminc.h"
#include "gemprm.h"
#include "dbcmn.h"

int db_flnmCallback(void (*aFlnmCallback)(char*))
{

   flnmClbkPtr = aFlnmCallback;
   return(0);
}
