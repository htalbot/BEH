#include "BasicEventHandling/BasicEventHandling.h"

#include "ace/ACE.h"

#include <assert.h>
#if defined(ACE_WIN32)
#    include <Psapi.h> // GetModuleBaseName()
#endif

#include <iostream>
#include <sstream>

//*********************************************************************
namespace BasicEventHandling_ns {


//*********************************************************************
BasicEvent::BasicEvent(const std::string & strNamespace,
                        long lID,
                        const std::string & strFormat)
:    m_strNamespace(strNamespace),
    m_lID(lID),
    m_strFormat(strFormat)
{
    ValidFormat(true, strFormat, m_mapSpec);  // if invalid ==> assert/throw
}

bool BasicEvent::ValidFormat(bool bCtr, const std::string & strFormat, std::map<long, std::string> & mapSpec)
{
    const char * pFormat = strFormat.c_str();
    const char * pLast = pFormat + strFormat.length() - 1;

    char * pPercentSign = const_cast<char *>(strchr(pFormat, '%'));
    while (pPercentSign != NULL)
    {
        if (*(pPercentSign + 1) == '%') // adjacent percent sign?
        {
            if (pPercentSign + 2 == pLast)
            {
                break;
            }

            pPercentSign = strchr(pPercentSign + 2, '%');
        }
        else
        {
            int nParamNumber(0);
            int nParameters = sscanf(pPercentSign + 1, "%d", &nParamNumber);
            if (nParameters == 0 || nParameters == EOF)
            {
                std::cout << "Percent sign is not followed by the number of the parameter.\n";
                if (!bCtr)
                {
                    return false;
                }
                else
                {
                    assert(nParameters != 0 && nParameters != EOF); // percent sign is not followed by the number of the parameter
                    throw 0;
                }
            }

            char * pStart = strchr(pPercentSign, '(');
            if (pStart == NULL)
            {
                std::cout << "Parameter is defined with no '('.\n";
                if (!bCtr)
                {
                    return false;
                }
                else
                {
                    assert(pStart != NULL); // the parameter is not defined with openning parenthese
                    throw 0;
                }
            }

            char * pEnd = strchr(pPercentSign + 1, ')');
            if (pEnd == NULL)
            {
                std::cout << "Parameter is defined with no ')'.\n";
                if (!bCtr)
                {
                    return false;
                }
                else
                {
                    assert(pEnd != NULL); // the parameter is not defined with closing parenthese
                    throw 0;
                }
            }

            std::string strSpec = std::string(pStart).substr(1, pEnd - pStart - 1);
            if (strSpec.empty())
            {
                std::cout << "Parameter specification is empty.\n";
                if (!bCtr)
                {
                    return false;
                }
                else
                {
                    assert(!strSpec.empty()); // the parameter specification is empty
                    throw 0;
                }
            }

            std::map<long, std::string>::iterator it = mapSpec.find(nParamNumber);
            if (it != mapSpec.end()) // already exist
            {
                bool bAlreadyExist = mapSpec[nParamNumber] != strSpec;
                if (bAlreadyExist)
                {
                    std::cout << "Parameter has already been assigned to a different spec.: " << strFormat.c_str() << "\n";
                    if (!bCtr)
                    {
                        return false;
                    }
                    else
                    {
                        assert(mapSpec[nParamNumber] == strSpec); // the parameter has already been assigned to a different spec
                        throw 0;
                    }
                }
            }
            mapSpec[nParamNumber] = strSpec;

            pPercentSign = strchr(pEnd, '%');
        }
    }

    int previous(0);
    std::map<long, std::string>::iterator it;
    for (it = mapSpec.begin(); it != mapSpec.end(); it++)
    {
        if (it->first != previous + 1)
        {
            std::cout << "Bad sequence of parameter numbers: " << strFormat.c_str() << "\n";
            // For example, %1(%d) and %3(%f): no %2 defined.
            throw 0;
        }

        previous = it->first;
    }

    return true;
}

void BasicEvent::sprintf(std::string & text, ...)
{
    va_list args;
    va_start(args, text);
    vsprintf(text, args);
    va_end(args);
}

void BasicEvent::vsprintf(std::string & text, va_list args)
{
    va_copy(va_args_, args);
    loop(text, this->format());
    va_end(va_args_);
}

//void BasicEvent::vsprintf(std::string & strText, va_list args) const
//{
//    strText.clear();
//    std::map<long, std::string> mapArgs;
//
//    try
//    {
//        long lIndex(1);
//        std::map<long, std::string>::const_iterator it;
//        for (it = m_mapSpec.begin(); it != m_mapSpec.end(); it++)
//        {
//            std::string strFormat = it->second;
//
//            std::stringstream ss;
//
//            if (strFormat.find("llu") != strFormat.npos)
//            {
//                long long n = va_arg(args, unsigned long long);
//                ss << n;
//            }
//            else if (strFormat.find_first_of("cCdioux") != strFormat.npos)
//            {
//                int n = va_arg(args, int);
//                ss << n;
//            }
//            else if (strFormat.find_first_of("eEfgG") != strFormat.npos)
//            {
//                double dValue = va_arg(args, double);
//                int nSize = ACE_OS::snprintf(NULL, 0, strFormat.c_str(), dValue) + 1;
//                char * pBuff = (char *)malloc(nSize);
//                ACE_OS::snprintf(pBuff, nSize, const_cast<char *>(strFormat.c_str()), dValue);
//                ss << pBuff;
//                free(pBuff);
//            }
//            else if (strFormat.find_first_of("np") != strFormat.npos)
//            {
//                assert(0);
//            }
//            else if (strFormat.find_first_of("sS") != strFormat.npos)
//            {
//                char * pszValue = va_arg(args, char *);
//                if (strlen(pszValue) > 0)
//                {
//                    int nSize = ACE_OS::snprintf(NULL, 0, strFormat.c_str(), pszValue) + 1;
//                    char * pBuff = (char *)malloc(nSize);
//                    ACE_OS::snprintf(pBuff, nSize, const_cast<char *>(strFormat.c_str()), pszValue);
//                    ss << pBuff;
//                    free(pBuff);
//                }
//            }
//            else
//            {
//                assert(0);
//            }
//            mapArgs[lIndex] = ss.str().c_str();
//
//            ++lIndex;
//        }
//
//        const char * pFormat = m_strFormat.c_str();
//        const char * pLast = pFormat + m_strFormat.length() - 1;
//
//        while (pFormat <= pLast)
//        {
//            if (*pFormat == '%')
//            {
//                if (pFormat != pLast)
//                {
//                    if (*(pFormat + 1) == '%')
//                    {
//                        strText += "%";
//                        pFormat += 2;
//                    }
//                    else
//                    {
//                        int nParamNumber(0);
//                        sscanf(pFormat + 1, "%d", &nParamNumber);
//                        strText += mapArgs[nParamNumber];
//                        pFormat = strchr(pFormat, ')');
//                        ++pFormat;
//                    }
//                }
//                else
//                {
//                    strText += *pFormat;
//                    ++pFormat;
//                }
//            }
//            else
//            {
//                strText += *pFormat;
//                ++pFormat;
//            }
//
//        }
//    }
//    catch(...)
//    {
//        strText = "Unable to parse args\n";
//    }
//}

const char * BasicEvent::namespace_()
{
    return m_strNamespace.c_str();
}

long BasicEvent::id()
{
    return m_lID;
}

const char * BasicEvent::format()
{
    return m_strFormat.c_str();
}

const std::map<long, std::string> & BasicEvent::mapspec()
{
    return m_mapSpec;
}

std::string BasicEvent::replace_special_char(const std::string & strValue)
{
    const char * psz = strValue.c_str();

    std::string str;

    str += *psz; // add the first quote

    ++psz; // points to the next character after the quote

    while(*psz)
    {
        if(*psz == '\n')
        {
            str += "\\n";
        }
        //else if (*psz == '\t')
        //{
        //    str += "\\t";
        //}
        else if (*psz == '\r')
        {
            str += "\\r";
        }
        else
        {
            str += *psz;
        }

        psz++;
    }

    return str;
}


//*********************************************************************
BasicEventHandler::~BasicEventHandler()
{
}

bool BasicEventHandler::handle_event(const char * szSource,
                                        BasicEvent & ev,
                                        ACE_Log_Priority nPriority,
                                        ...)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(handle_event_mutex_);

    va_list args;
    va_start(args, nPriority);
    bool rc = vhandle_event(szSource, ev, nPriority, args);
    va_end(args);

    return rc;
}


//**********************************************************************************************
EventHandler_SystemLog::EventHandler_SystemLog()
{
    u_long prev_flags = ACE_LOG_MSG->flags(); // get previous flags

    u_long bOpen = (ACE_LOG_MSG->flags() & ACE_Log_Msg::SYSLOG);
    if (!bOpen)
    {    // Open it

        static std::string strModuleName;

        // Get module name
#        if defined(ACE_WIN32)
            char szName[_MAX_PATH];
            GetModuleBaseName(GetCurrentProcess(), NULL, szName, _MAX_PATH);
            strModuleName = szName;
#        else
            std::stringstream ss;
            ss << "/proc/" << getpid() << "/cmdline";

            const int nSize(1024);
            char arg_list[nSize];
            memset(arg_list, 0, nSize * sizeof(char));
            int fd = open(ss.str().c_str(), O_RDONLY);
            size_t length = read(fd, arg_list, sizeof(arg_list));
            close(fd);

            if (length != 0)
            {
                strModuleName = arg_list;
            }
            else
            {
                throw;
            }
#        endif

        int status = ACE_LOG_MSG->open(ACE_TEXT(strModuleName.c_str()),
                            ACE_Log_Msg::SYSLOG, ACE::basename(strModuleName.c_str()));
        if (status != 0)
        {
            throw;
        }

        ACE_LOG_MSG->clr_flags(ACE_Log_Msg::SYSLOG); // remove SYSLOG flag
        ACE_LOG_MSG->set_flags(prev_flags); // reset flags to the previous value.
    }
}

void EventHandler_SystemLog::set_flags(u_long ulPriority)
{
    ACE_LOG_MSG->set_flags(ulPriority);
}

bool EventHandler_SystemLog::vhandle_event(const char * szSource,
                                            BasicEventHandling_ns::BasicEvent & ev,
                                            ACE_Log_Priority nPriority,
                                            va_list args)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(vhandle_event_mutex_);

    // Get formatted string
    std::string strText;
    ev.vsprintf(strText, args);

    // Prepend source and ID to the formatted string
    std::stringstream ss;
    ss << szSource << " - event id = " << ev.id() << ": " << strText.c_str();

    u_long prev_flags = ACE_LOG_MSG->flags(); // remember previous flags
    ACE_LOG_MSG->set_flags(ACE_Log_Msg::SYSLOG); // set flags as SYSLOG
    ACE_LOG_MSG->clr_flags((u_long)~ACE_Log_Msg::SYSLOG); // remove all flags that are not SYSLOG
    ACE_DEBUG((nPriority, ACE_TEXT("%s"), ss.str().c_str())); // log
    ACE_LOG_MSG->clr_flags(ACE_Log_Msg::SYSLOG); // remove SYSLOG flag
    ACE_LOG_MSG->set_flags(prev_flags); // reset the flags to the previuous value

    return true;
}

bool EventHandler_SystemLog::handle_event(const char * szSource,
                                            BasicEventHandling_ns::BasicEvent & ev,
                                            ACE_Log_Priority nPriority,
                                            ...)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(handle_event_mutex_);

    va_list args;
    va_start(args, nPriority);
    bool rc = vhandle_event(szSource, ev, nPriority, args);
    va_end(args);

    return rc;
}


//**********************************************************************************************
void EventHandler_stderr::open(const std::string & strProgramName)
{
    u_long ulFlags = ACE_LOG_MSG->flags();
    if (ulFlags & ACE_Log_Msg::SYSLOG)
    {
        ACE_LOG_MSG->set_flags(ACE_Log_Msg::STDERR);
    }
    else
    {
        ACE_LOG_MSG->open(ACE_TEXT(strProgramName.c_str()));
    }
}

bool EventHandler_stderr::vhandle_event(const char * szSource,
                                        BasicEventHandling_ns::BasicEvent & ev,
                                        ACE_Log_Priority nPriority,
                                        va_list args)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(vhandle_event_mutex_);

    //// Get formatted string
    std::string strText;
    ev.vsprintf(strText, args);

    // Prepend source and ID to the formatted string
    std::stringstream ss;
    ss << szSource << " - id = " << ev.id() << ": " << strText.c_str() << std::endl;

    // log
    u_long flags = ACE_LOG_MSG->flags();                // get preceeding flags
    ACE_LOG_MSG->clr_flags((u_long)~ACE_Log_Msg::STDERR);        // clear all flags that are not STDERR
    ACE_DEBUG((nPriority, ACE_TEXT("%s"), ss.str().c_str()));    // log
    ACE_LOG_MSG->set_flags(flags);                        // reset the flags to the preceeding values

    return true;
}

bool EventHandler_stderr::handle_event(const char * szSource,
                                            BasicEventHandling_ns::BasicEvent & ev,
                                            ACE_Log_Priority nPriority,
                                            ...)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(handle_event_mutex_);

    va_list args;
    va_start(args, nPriority);
    bool rc = vhandle_event(szSource, ev, nPriority, args);
    va_end(args);

    return rc;
}



//**********************************************************************************************
BasicEventRaiser::BasicEventRaiser()
:    pEventHandler_(NULL),
    bHook_(false),
    filter_(ALL_PRIORITIES) // all priorities defined in $ACE_ROOT/ace/Log_Priority.h
{
}

BasicEventRaiser::~BasicEventRaiser()
{
}

void BasicEventRaiser::set_event_handler(BasicEventHandling_ns::BasicEventHandler * p, bool bHook)
{
    bHook_ = bHook;
    pEventHandler_ = p;
    if (p == NULL)
    {
        ACE_LOG_MSG->clr_flags((u_long)~ACE_Log_Msg::SYSLOG);
    }
}

BasicEventHandler * BasicEventRaiser::get_event_handler()
{
    return pEventHandler_;
}

void BasicEventRaiser::set_filter(ACE_Log_Priority nPriority)    // want a subset of events
{
    filter_ = nPriority;
}

bool BasicEventRaiser::raise_event(const char * szSource,
                                    BasicEventHandling_ns::BasicEvent & ev,
                                    ACE_Log_Priority nPriority,
                                    ...)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(raise_event_mutex_);

    va_list args;
    va_start(args, nPriority);
    bool rc = vraise_event(szSource, ev, nPriority, args);
    va_end(args);
    return rc;
}

bool BasicEventRaiser::vraise_event(const char * szSource,
                                        BasicEventHandling_ns::BasicEvent & ev,
                                        ACE_Log_Priority nPriority,
                                        va_list args)
{
    ACE_Guard<ACE_Recursive_Thread_Mutex> guard(vraise_event_mutex_);

    bool bRc(true);

    if (nPriority & filter_)
    {
        if (pEventHandler_)
        {
            bRc = pEventHandler_->vhandle_event(szSource, ev, nPriority, args);
        }

        if (!pEventHandler_ || !bRc || bHook_)  // call default logging ?
        {
            // yes, call default logging...
            EVENT_HANDLER_SYSTEMLOG->vhandle_event(szSource, ev, nPriority, args);
        }
    }

    return bRc;
}

}; // BasicEventHandling
