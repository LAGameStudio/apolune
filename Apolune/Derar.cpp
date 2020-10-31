#include "Derar.h"

string RAR_File_Buffer;
string RAR_Password;

int CALLBACK RARCallbackProc(UINT msg,LPARAM UserData,LPARAM P1,LPARAM P2)
{
  switch(msg)
  {
    case UCM_CHANGEVOLUME:
    case UCM_CHANGEVOLUMEW: if ( P2 == RAR_VOL_NOTIFY ) OUTPUT("\n  ... volume %S\n",(wchar_t *)P1); return(0);
    case UCM_PROCESSDATA:
      {
       char *data=(char *)P1;
       char *temp=new char[P2+1];
       temp[P2]='\0';
       for ( int i=0; i<P2; i++ ) {
        temp[i]=data[i];
       }
       RAR_File_Buffer=string(temp);
      }
      return(0);
    case UCM_NEEDPASSWORD:
     OUTPUT("Derar::password: Archive required the password.\n");
     {
      const char *pwd=RAR_Password.c_str();
      char *writingto=(char *)P1;
      int i=0;
      while ( i<P2 && pwd[i] != '\0' ) {
       writingto[i]=pwd[i];
       i++;
      }
     }
     return(0);
    case UCM_NEEDPASSWORDW: OUTPUT("Derar::UCM_NEEDPASSWORDW callback to unsupported mode (Unicode).\n"); return (0);
  }
  return(0);
}
