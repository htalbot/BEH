
#pragma once

#define BASICEVENTCORBA_MAJOR 1
#define BASICEVENTCORBA_MINOR 0
#define BASICEVENTCORBA_PATCH 1


#if defined _MSC_VER
#   ifdef _DEBUG
        const char BASICEVENTCORBA_DebugFlag[] = "DEBUG";
#   else
        const char BASICEVENTCORBA_DebugFlag[] = "RELEASE";
#   endif
#else
#   ifdef DEBUG
        const char BASICEVENTCORBA_DebugFlag[] = "DEBUG";
#   else
        const char BASICEVENTCORBA_DebugFlag[] = "RELEASE";
#   endif
#endif


