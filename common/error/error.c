#include "error.h"

#include <stdarg.h>

#include "../print/print.h"

void eprintf(const char* format, ...) {
    va_list arguments;
    va_start(arguments, format);
    cfprintf(stderr, RED_BOLD, "[ ERROR ] ");
    cvfprintf(stderr, RED_BOLD, format, arguments);
}