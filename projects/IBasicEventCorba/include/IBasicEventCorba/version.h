
#ifndef __IBASICEVENTCORBA_VERSION_H__
#define __IBASICEVENTCORBA_VERSION_H__

#define IBASICEVENTCORBA_MAJOR 1
#define IBASICEVENTCORBA_MINOR 0
#define IBASICEVENTCORBA_PATCH 1

#if defined _MSC_VER
#   ifdef _DEBUG
        const char IBASICEVENTCORBA_DebugFlag[] = "DEBUG";
#   else
        const char IBASICEVENTCORBA_DebugFlag[] = "RELEASE";
#   endif
#else
#   ifdef DEBUG
        const char IBASICEVENTCORBA_DebugFlag[] = "DEBUG";
#   else
        const char IBASICEVENTCORBA_DebugFlag[] = "RELEASE";
#   endif
#endif



#endif // __IBASICEVENTCORBA_VERSION_H__
