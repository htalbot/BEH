
#pragma once

#define BASICEVENTHANDLING_MAJOR 1
#define BASICEVENTHANDLING_MINOR 0
#define BASICEVENTHANDLING_PATCH 1


#if defined _MSC_VER
#   ifdef _DEBUG
        const char BASICEVENTHANDLING_DebugFlag[] = "DEBUG";
#   else
        const char BASICEVENTHANDLING_DebugFlag[] = "RELEASE";
#   endif
#else
#   ifdef DEBUG
        const char BASICEVENTHANDLING_DebugFlag[] = "DEBUG";
#   else
        const char BASICEVENTHANDLING_DebugFlag[] = "RELEASE";
#   endif
#endif


