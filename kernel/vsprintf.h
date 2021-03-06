#ifndef __VSPRINTF_H__
#define __VSPRINTF_H__

#include <stdarg.h>
#include "string.h"

int _vsprintf(char *buf, const char *fmt, va_list args);
int _sprintf( char *buf, const char *fmt, ... );
//int dbgprintf( const char *fmt, ...);
void CheckOSReport(void);
void closeLog(void);
#endif