#ifndef _PLATFORM_H
#define _PLATFORM_H

#ifndef WIN32
#define _strnicmp strncasecmp
#define _stricmp strcasecmp
#define strncpy_s strncpy
#define _strdup strdup
#endif

#endif
