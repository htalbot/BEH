
#include "BasicEventCorba/BasicEventCorba.h"
#include "BasicEventCorba/version.h"
#include "BasicEventHandling/PrintfBase.h"

namespace BasicEventCorba_ns
{
    void BasicEventCorba_get_version(int & major, int & minor, int & patch)
    {
        major = BASICEVENTCORBA_MAJOR;
        minor = BASICEVENTCORBA_MINOR;
        patch = BASICEVENTCORBA_PATCH;
    }

    bool BasicEventCorba::convert(
        BasicEventHandling_ns::BasicEvent & basic_event,
        IBASICEVENTCORBA_module::BasicEventCorba & corba_event,
        const std::string & node_id,
        const std::string & source,
        ACE_Log_Priority priority,
        ...)
    {
        va_list args;
        va_start(args, priority);

        bool rc = vconvert(
            basic_event, 
            corba_event,
            node_id,
            source,
            priority,
            args);

        va_end(args);

        return rc;
    }

    bool BasicEventCorba::vconvert(
        BasicEventHandling_ns::BasicEvent & basic_event,
        IBASICEVENTCORBA_module::BasicEventCorba & corba_event,
        const std::string & node_id,
        const std::string & source,
        ACE_Log_Priority priority,
        va_list & va_args)
    {
        BasicEventHandling_ns::PrintfBase p;

        corba_event.event_id_ = basic_event.id();
        corba_event.format_ = basic_event.format();
        corba_event.namespace_ = basic_event.namespace_();

        corba_event.map_spec_.length(static_cast<CORBA::ULong>(basic_event.mapspec().size()));
        corba_event.args_.length(static_cast<CORBA::ULong>(basic_event.mapspec().size()));

        ::CORBA::ULong i(0);
        std::map<long, std::string>::const_iterator it;
        for (it = basic_event.mapspec().begin(); it != basic_event.mapspec().end(); it++)
        {
            IBASICEVENTCORBA_module::EventSpec spec;
            spec.param_number = it->first;
            spec.spec_string = it->second.c_str();
            corba_event.map_spec_[i] = spec;

            p.prepare(it->second, i);

            ++i;
        }

        p.set(va_args);

        i = 0;
        std::map<long, BasicEventHandling_ns::PrintfArg> args = p.get_args();
        std::map<long, BasicEventHandling_ns::PrintfArg>::iterator it_arg;
        for (it_arg = args.begin(); it_arg != args.end(); it_arg++)
        {
            IBASICEVENTCORBA_module::EventArg event_arg;

            if (it_arg->second.spec_ == "%d"
                || it_arg->second.spec_ == "%i"
                || it_arg->second.spec_ == "%o"
                || it_arg->second.spec_ == "%x"
                || it_arg->second.spec_ == "%X"
                || it_arg->second.spec_ == "%ld"
                || it_arg->second.spec_ == "%li"
                || it_arg->second.spec_ == "%lo"
                || it_arg->second.spec_ == "%lx"
                || it_arg->second.spec_ == "%lX")
            {
                event_arg.arg_.lValue(it_arg->second.data.lValue);
            }
            else if (it_arg->second.spec_ == "%u")
            {
                event_arg.arg_.ulValue(it_arg->second.data.ulValue);
            }
            else if (it_arg->second.spec_ == "%f"
                || it_arg->second.spec_ == "%F"
                || it_arg->second.spec_ == "%e"
                || it_arg->second.spec_ == "%E"
                || it_arg->second.spec_ == "%g"
                || it_arg->second.spec_ == "%G"
                || it_arg->second.spec_ == "%lf"
                || it_arg->second.spec_ == "%lF"
                || it_arg->second.spec_ == "%le"
                || it_arg->second.spec_ == "%lE"
                || it_arg->second.spec_ == "%lg"
                || it_arg->second.spec_ == "%lG")
            {
                event_arg.arg_.dValue(it_arg->second.data.dValue);
            }
            else if (it_arg->second.spec_ == "%ll")
            {
                event_arg.arg_.llValue(it_arg->second.data.llValue);
            }
            else if (it_arg->second.spec_ == "%llu")
            {
                event_arg.arg_.ullValue(it_arg->second.data.ullValue);
            }
            else if (it_arg->second.spec_ == "%s")
            {
                event_arg.arg_.strValue(CORBA::string_dup(it_arg->second.data.strValue));
            }

            corba_event.args_[i] = event_arg;

            ++i;
        }

        corba_event.node_id_ = node_id.c_str();
        corba_event.source_id_ = source.c_str();
        corba_event.severity_ = priority;

        return true;
    }
} // BasicEventCorba_ns namespace
