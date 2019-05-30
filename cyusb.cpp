
#define CYDLL_EXPORT 1
#include "cyusb.h"

#include "cyusb_imp.h"

// Export both decorated and undecorated names.
//   GetCyUSB     - Undecorated name, which can be easily used with GetProcAddress
//                Win32 API function.
//   _GetCyUSB@0  - Common name decoration for __stdcall functions in C language.
#if CYUSBAPI_OPTION==1
#pragma comment(linker, "/export:GetCyUSB=_GetCyUSB@0")
#endif

CyUSB* CYUSBAPI GetCyUSB(void){
  return dynamic_cast<CyUSB*>(new CyUSB_impl());
}
