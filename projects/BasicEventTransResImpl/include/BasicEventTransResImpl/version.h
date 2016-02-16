
#pragma once

#define BASICEVENTTRANSRESIMPL_MAJOR 1
#define BASICEVENTTRANSRESIMPL_MINOR 0
#define BASICEVENTTRANSRESIMPL_PATCH 1


#if defined _MSC_VER
#   ifdef _DEBUG
        const char BASICEVENTTRANSRESIMPL_DebugFlag[] = "DEBUG";
#   else
        const char BASICEVENTTRANSRESIMPL_DebugFlag[] = "RELEASE";
#   endif
#else
#   ifdef DEBUG
        const char BASICEVENTTRANSRESIMPL_DebugFlag[] = "DEBUG";
#   else
        const char BASICEVENTTRANSRESIMPL_DebugFlag[] = "RELEASE";
#   endif
#endif


