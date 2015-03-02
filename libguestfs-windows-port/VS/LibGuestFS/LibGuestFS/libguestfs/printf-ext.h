#ifdef _WIN32

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

extern int asprintf(char** __restrict ret, const char * __restrict format, ...);
extern int vasprintf(char** __restrict ret, const char * __restrict format, va_list ap);

#endif /* _WIN32 */
