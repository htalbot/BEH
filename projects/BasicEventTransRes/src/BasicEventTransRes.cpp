// EventsDLL.cpp : Defines the entry point for the DLL application.
//

#include "BasicEventHandling/BasicEventHandling.h"
#include "BasicEventTransRes/BasicEventTransRes.h"
#include "BasicEventTransResImpl/BasicEventTransResImpl.h"

#include <string>

//***********************************************************************************
std::string g_strBuffer;
std::string g_strvbuffer;
std::string g_strformat;

void my_init()
{
    AddEvents();
}

void my_fini()
{
    Events::DeleteEvents();
}


#if defined(ACE_WIN32)
    extern "C" BOOL WINAPI DllMain(HINSTANCE /*instance*/, DWORD reason, LPVOID)
    {
        if (reason == DLL_PROCESS_ATTACH)
        {
            my_init();
        }
        else if (reason == DLL_PROCESS_DETACH)
        {
            my_fini();
        }
        return TRUE;
    }
#else
    __attribute__ ((constructor)) void init()
    {
        /* code here is executed after dlopen() has loaded the module */
        my_init();
    }
    __attribute__ ((destructor)) void fini()
    {
        my_fini();
        /* code here is executed just before dlclose() unloads the module */
    }
# endif /* ACE_WIN32 */


//***********************************************************************************
const char * get_msg(const char * szNamespace, long lID, ...)
{
    EventObject * pEventObject;
    if (!Events::Found(szNamespace, lID, &pEventObject))
    {
        return NULL;
    }

    va_list args;
    va_start(args, lID);
    g_strvbuffer = vget_msg(szNamespace, lID, args);
    va_end(args);

    return g_strvbuffer.c_str();
}

//***********************************************************************************
const char * vget_msg(const char * szNamespace, long lID, va_list args)
{
    EventObject * pEventObject = NULL;
    if (!Events::Found(szNamespace, lID, &pEventObject))
    {
        return NULL;
    }

    BasicEventHandling_ns::BasicEvent ev(szNamespace, lID, pEventObject->get_format());
    ev.vsprintf(g_strBuffer, args);

    return g_strBuffer.c_str();
}

//***********************************************************************************
const char * get_msg_format(const char * szNamespace, long lID)
{
    EventObject * pEventObject = NULL;
    if (!Events::Found(szNamespace, lID, &pEventObject))
    {
        return NULL;
    }

    g_strformat = pEventObject->get_format();

    return g_strformat.c_str();
}


