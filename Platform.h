#ifndef _PLATFORM_H
#define _PLATFORM_H

#include <stdint.h>

#ifndef WIN32
#define _strnicmp strncasecmp
#define _stricmp strcasecmp
#define strncpy_s strncpy
#define _strdup strdup
#endif

typedef uint8_t uuid_t[16];

#endif
