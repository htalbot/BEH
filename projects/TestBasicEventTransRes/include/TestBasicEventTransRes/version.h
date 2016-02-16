
#ifndef __TESTBASICEVENTTRANSRES_VERSION_H__
#define __TESTBASICEVENTTRANSRES_VERSION_H__

#define TESTBASICEVENTTRANSRES_MAJOR 1
#define TESTBASICEVENTTRANSRES_MINOR 0
#define TESTBASICEVENTTRANSRES_PATCH 1


#if defined _MSC_VER
#   ifdef _DEBUG
        const char TESTBASICEVENTTRANSRES_DebugFlag[] = "DEBUG";
#   else
        const char TESTBASICEVENTTRANSRES_DebugFlag[] = "RELEASE";
#   endif
#else
#   ifdef DEBUG
        const char TESTBASICEVENTTRANSRES_DebugFlag[] = "DEBUG";
#   else
        const char TESTBASICEVENTTRANSRES_DebugFlag[] = "RELEASE";
#   endif
#endif


#endif // __TESTBASICEVENTTRANSRES_VERSION_H__
