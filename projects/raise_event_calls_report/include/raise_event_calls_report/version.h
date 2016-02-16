
#ifndef __RAISE_EVENT_CALLS_REPORT_VERSION_H__
#define __RAISE_EVENT_CALLS_REPORT_VERSION_H__

#define RAISE_EVENT_CALLS_REPORT_MAJOR 1
#define RAISE_EVENT_CALLS_REPORT_MINOR 0
#define RAISE_EVENT_CALLS_REPORT_PATCH 1


#if defined _MSC_VER
#   ifdef _DEBUG
        const char RAISE_EVENT_CALLS_REPORT_DebugFlag[] = "DEBUG";
#   else
        const char RAISE_EVENT_CALLS_REPORT_DebugFlag[] = "RELEASE";
#   endif
#else
#   ifdef DEBUG
        const char RAISE_EVENT_CALLS_REPORT_DebugFlag[] = "DEBUG";
#   else
        const char RAISE_EVENT_CALLS_REPORT_DebugFlag[] = "RELEASE";
#   endif
#endif


#endif // __RAISE_EVENT_CALLS_REPORT_VERSION_H__
