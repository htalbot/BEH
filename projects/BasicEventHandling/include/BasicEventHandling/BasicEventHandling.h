#ifndef __BASICEVENTHANDLING_H__
#define __BASICEVENTHANDLING_H__

#include "BasicEventHandling/BASICEVENTHANDLING_Export.h"

#include "ACEWin64Warnings/BeginSuppressWarnings.h"
#include "ace/Log_Msg.h"
#include "ace/Singleton.h"
#include "ace/Synch_Traits.h"
#include "ACEWin64Warnings/EndSuppressWarnings.h"

#include "BasicEventHandling/PrintfBase.h"

#if defined(ACE_WIN32)
#   pragma warning( disable : 4251 ) // wittingly: DPO deals with this issue.
#endif 

#include <string>
#include <map>


//*********************************************************************
namespace BasicEventHandling_ns {


//*********************************************************************
class BASICEVENTHANDLING_Export BasicEvent: protected PrintfBase
{
public:

    BasicEvent(const std::string & strNamespace,
                long lID,
                const std::string & strFormat);

    static bool ValidFormat(bool bCtr, const std::string & strFormat, std::map<long, std::string> & mapSpec);
    void sprintf(std::string & text, ...);
    void vsprintf(std::string & text, va_list args);
    const char * namespace_();
    long id();
    const char * format();
    const std::map<long, std::string> & mapspec();
    static std::string replace_special_char(const std::string & strValue);

protected:

    std::string m_strNamespace;
    long m_lID;
    std::string m_strFormat;
    std::map<long, std::string> m_mapSpec;
};


//*********************************************************************
// Pure base class for every event handlers
class BASICEVENTHANDLING_Export BasicEventHandler
{
public:

    virtual ~BasicEventHandler();
    virtual bool handle_event(const char * szSource,
                                BasicEvent & ev,
                                ACE_Log_Priority nPriority,
                                ...);
    virtual bool vhandle_event(const char * szSource,
                                BasicEvent & ev,
                                ACE_Log_Priority nPriority,
                                va_list args) = 0;

protected:
    ACE_Recursive_Thread_Mutex handle_event_mutex_;
    ACE_Recursive_Thread_Mutex vhandle_event_mutex_;
};


//**********************************************************************************************
// Default event handler (system log)
class BASICEVENTHANDLING_Export EventHandler_SystemLog: public BasicEventHandling_ns::BasicEventHandler
{
public:

    EventHandler_SystemLog();
    void set_flags(u_long ulPriority);
    virtual bool vhandle_event(const char * szSource,
                        BasicEventHandling_ns::BasicEvent & ev,
                        ACE_Log_Priority nPriority,
                        va_list args);
    virtual bool handle_event(const char * szSource,
                        BasicEventHandling_ns::BasicEvent & ev,
                        ACE_Log_Priority nPriority,
                        ...);

protected:
    ACE_Recursive_Thread_Mutex handle_event_mutex_;
    ACE_Recursive_Thread_Mutex vhandle_event_mutex_;
};


//**********************************************************************************************
// An EventHandler to stderr only
class BASICEVENTHANDLING_Export EventHandler_stderr: public BasicEventHandling_ns::BasicEventHandler
{
public:

    void open(const std::string & strProgramName);
    virtual bool vhandle_event(const char * szSource,
                                BasicEventHandling_ns::BasicEvent & ev,
                                ACE_Log_Priority nPriority,
                                va_list args);
    virtual bool handle_event(const char * szSource,
                        BasicEventHandling_ns::BasicEvent & ev,
                        ACE_Log_Priority nPriority,
                        ...);

protected:
    ACE_Recursive_Thread_Mutex handle_event_mutex_;
    ACE_Recursive_Thread_Mutex vhandle_event_mutex_;
};

//*********************************************************************
//***** Macros to make easy the process of defining Events
//*****    It is equivalent to the simple declaration:
//*****   BasicEventHandling_ns::BasicEvent ev(1, "format");
//*****
//***** Declaration:
#define BEGIN_EVENT_DECL_NAMESPACE(id)\
    namespace id {\
    const char decl_namespace[] = #id;

#define STATIC_EVENT_DECL(ev)\
    extern BasicEventHandling_ns::BasicEvent ev;

//***** Definition:
#define BEGIN_EVENT_DEF_NAMESPACE(id)\
    namespace id {\
    const char def_namespace[] = #id;

#define STATIC_EVENT_DEF(ev, id, format)\
    BasicEventHandling_ns::BasicEvent ev(def_namespace, id, format);

//***** Declaration/Definition:
#define END_EVENT_NAMESPACE()\
    }

//*********************************************************************
#define    ClassID(x)    #x        // not used, just to remember #...



//*********************************************************************
#define ALL_PRIORITIES (ACE_Log_Priority)1023


//**********************************************************************************************
class BASICEVENTHANDLING_Export BasicEventRaiser
{
public:

    BasicEventRaiser();
    virtual ~BasicEventRaiser();
    virtual void set_event_handler(BasicEventHandling_ns::BasicEventHandler * p, bool bHook = false);
    BasicEventHandling_ns::BasicEventHandler * get_event_handler();
    void set_filter(ACE_Log_Priority nPriority = ALL_PRIORITIES);    // want a subset of events
    virtual bool raise_event(const char * szSource,
                                BasicEventHandling_ns::BasicEvent & ev,
                                ACE_Log_Priority nPriority,
                                ...);
    virtual bool vraise_event(const char * szSource,
                                BasicEventHandling_ns::BasicEvent & ev,
                                ACE_Log_Priority nPriority,
                                va_list args);

protected:

    BasicEventHandling_ns::BasicEventHandler * pEventHandler_;
    bool bHook_;
    ACE_Log_Priority filter_;

    ACE_Recursive_Thread_Mutex raise_event_mutex_;
    ACE_Recursive_Thread_Mutex vraise_event_mutex_;
};



// To simplify 'raise' (or vraise) call, it would be interresting
// to automate 'szSource' parameter passing.
// For example, 'raise(ev_error1, LM_ERROR, 10, 20);'
// instead of 'raise("classX::method_y", ev_error1, LM_ERROR, 10, 20);'.
// In ISO C99, a macro called __VA_ARGS__ exists and allows to define another
// macro like:
// #define raise_(s, ...) report(__FUNCTION__, s, ##__VA_ARGS__);
// However, vc71 does not provides the macro __VA_ARGS__ (vc8 yes).
// To minimize the work of writing the class name and its
// method as the szSource argument, we define the following macro:

#if defined(ACE_WIN32)
#    define _SRC __FUNCTION__
#else
#    define _SRC __PRETTY_FUNCTION__
#endif

// In this way, we can call 'raise' method like this:
//        raise(_SRC, ev_error1, LM_ERROR, 10, 20);'.

}; // BasicEventHandling_ns

BASICEVENTHANDLING_SINGLETON_DECLARE(ACE_Singleton,
                                        BasicEventHandling_ns::EventHandler_SystemLog,
                                        ACE_Recursive_Thread_Mutex)

#define EVENT_HANDLER_SYSTEMLOG ACE_Singleton<BasicEventHandling_ns::EventHandler_SystemLog,\
                                                ACE_Recursive_Thread_Mutex>::instance()

#endif // __BASICEVENTHANDLING_H__
