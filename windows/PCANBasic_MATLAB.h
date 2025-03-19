#ifndef PCANBASIC_FORMATLAB_H
#define PCANBASIC_FORMATLAB_H

/* 
 * If you are using MinGW, we can define the Windows types ourselves.
 * If you have a MSVC environment that has <windows.h>, you can 
 * include it instead:
 */
/* #include <windows.h>   // If you want to rely on real Windows definitions */

#ifndef _WINDEF_
  /* Or define them manually for MATLAB’s loadlibrary: */
  typedef unsigned long         DWORD;  /* 32-bit */
  typedef unsigned short        WORD;   /* 16-bit */
  typedef unsigned char         BYTE;   /* 8-bit  */
  typedef char*                 LPSTR;  /* pointer to char */
  typedef unsigned long long    UINT64; /* 64-bit */

#endif

#include "PCANBasic.h"

#endif
