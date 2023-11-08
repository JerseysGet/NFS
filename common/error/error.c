#include "error.h"

#include <stdarg.h>

#include "../print/print.h"

void eprintf(const char* format, ...) {
    va_list arguments;
    va_start(arguments, format);
    cvfprintf(stderr, RED_BOLD, format, arguments);
}