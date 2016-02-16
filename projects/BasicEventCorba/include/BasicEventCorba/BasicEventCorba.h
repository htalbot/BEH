
#ifndef __BASICEVENTCORBA_H__
#define __BASICEVENTCORBA_H__

#include "BasicEventCorba/BASICEVENTCORBA_Export.h"
#include "BasicEventCorba/version.h"
#include "BasicEventHandling/BasicEventHandling.h"
#include "IBasicEventCorba/IBasicEventCorbaC.h"

namespace BasicEventCorba_ns
{
    extern "C" BASICEVENTCORBA_Export void BasicEventCorba_get_version(int & major, int & minor, int & patch);

    class BASICEVENTCORBA_Export BasicEventCorba
    {
    public:
        static bool convert(
            BasicEventHandling_ns::BasicEvent &,
            IBASICEVENTCORBA_module::BasicEventCorba &,
            const std::string & node_id,
            const std::string & source,
            ACE_Log_Priority priority,
            ...);

        static bool vconvert(
            BasicEventHandling_ns::BasicEvent &,
            IBASICEVENTCORBA_module::BasicEventCorba &,
            const std::string & node_id,
            const std::string & source,
            ACE_Log_Priority priority,
            va_list & args);
    };

} // BasicEventCorba_ns namespace


#endif // __BASICEVENTCORBA_H__
