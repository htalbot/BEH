
#ifndef __TESTBASICEVENTHANDLING_VERSION_H__
#define __TESTBASICEVENTHANDLING_VERSION_H__

#define TESTBASICEVENTHANDLING_MAJOR 1
#define TESTBASICEVENTHANDLING_MINOR 0
#define TESTBASICEVENTHANDLING_PATCH 1


#if defined _MSC_VER
#   ifdef _DEBUG
        const char TESTBASICEVENTHANDLING_DebugFlag[] = "DEBUG";
#   else
        const char TESTBASICEVENTHANDLING_DebugFlag[] = "RELEASE";
#   endif
#else
#   ifdef DEBUG
        const char TESTBASICEVENTHANDLING_DebugFlag[] = "DEBUG";
#   else
        const char TESTBASICEVENTHANDLING_DebugFlag[] = "RELEASE";
#   endif
#endif


#endif // __TESTBASICEVENTHANDLING_VERSION_H__
