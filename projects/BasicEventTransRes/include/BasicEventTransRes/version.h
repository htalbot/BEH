
#pragma once

#define BASICEVENTTRANSRES_MAJOR 1
#define BASICEVENTTRANSRES_MINOR 0
#define BASICEVENTTRANSRES_PATCH 1


#if defined _MSC_VER
#   ifdef _DEBUG
        const char BASICEVENTTRANSRES_DebugFlag[] = "DEBUG";
#   else
        const char BASICEVENTTRANSRES_DebugFlag[] = "RELEASE";
#   endif
#else
#   ifdef DEBUG
        const char BASICEVENTTRANSRES_DebugFlag[] = "DEBUG";
#   else
        const char BASICEVENTTRANSRES_DebugFlag[] = "RELEASE";
#   endif
#endif


