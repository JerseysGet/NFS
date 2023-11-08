#ifndef __ERROR_H
#define __ERROR_H

#define FAILURE 1
#define SUCCESS 0

typedef int ErrorCode;

void eprintf(const char* format, ...);

#endif  