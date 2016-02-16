
#ifndef __GENBASICEVENTTRANSRES_VERSION_H__
#define __GENBASICEVENTTRANSRES_VERSION_H__

#define GENBASICEVENTTRANSRES_MAJOR 1
#define GENBASICEVENTTRANSRES_MINOR 0
#define GENBASICEVENTTRANSRES_PATCH 1


#if defined _MSC_VER
#   ifdef _DEBUG
        const char GENBASICEVENTTRANSRES_DebugFlag[] = "DEBUG";
#   else
        const char GENBASICEVENTTRANSRES_DebugFlag[] = "RELEASE";
#   endif
#else
#   ifdef DEBUG
        const char GENBASICEVENTTRANSRES_DebugFlag[] = "DEBUG";
#   else
        const char GENBASICEVENTTRANSRES_DebugFlag[] = "RELEASE";
#   endif
#endif


#endif // __GENBASICEVENTTRANSRES_VERSION_H__
