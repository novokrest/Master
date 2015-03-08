#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

int asprintf(char** __restrict ret, const char* __restrict format, ...)
{
    va_list ap;
    int len;

    va_start(ap, format);
    len = vasprintf(ret, format, ap);
    va_end(ap);

    return len;
}

int vasprintf(char** __restrict ret, const char* __restrict format, va_list ap)
{
    int len;

    len = _vscprintf(format, ap);
    if (len < 0) {
        return -1;
    }

    *ret = (char*)malloc(len + 1);
    if (!*ret) {
        return -1;
    }

    _vsnprintf_s(*ret, len + 1, len, format, ap);
    //(*ret)[len] = '\0';

    return len;
}
