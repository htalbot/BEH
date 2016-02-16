
#include "TestBasicEventTransResAppLayer.h"
#include "TestBasicEventTransRes/version.h"
#include <stdio.h>

#include "BasicEventHandling/BasicEventHandling.h"
#include "BasicEventTransRes/BasicEventTransRes.h"

#ifdef ACE_WIN32
#pragma warning(push)
#pragma warning(disable: 4702)
#endif

#include <string>
#include <iostream>
#include <list>

BEGIN_EVENT_DEF_NAMESPACE(TestEventsDLLEvents)
    STATIC_EVENT_DEF(ev_program_failed, 1, "%1(%s) failed.\n")
    STATIC_EVENT_DEF(evReason, 2, "%1(%s)\n\n")
    STATIC_EVENT_DEF(ev_syntax_info, 3, "Syntax: '%1(%s)' <-s EventDefSourceFile>\n\n")
    STATIC_EVENT_DEF(ev_syntax_info2, 4, "%1(%s)\n\n")
    STATIC_EVENT_DEF(ev_file_access, 5, "Can not open '%1(%s)'\n\n")
END_EVENT_NAMESPACE()

using namespace TestEventsDLLEvents;

class CTestBasicEventTransRes: public BasicEventHandling_ns::BasicEventRaiser
{
public:

    void prompt(const std::list<std::string> & list)
    {
        std::list<std::string>::const_iterator it;
        for (it = list.begin(); it != list.end(); it++)
        {
            std::cout << it->c_str() << std::endl;
        }
        std::cout << "9- Quit.\n";

        std::cout << std::endl;
        std::cout << "Choice: ";
    }

    void advise()
    {
        std::cout << std::endl << "Bad option!!!\n\n";
    }

    bool ValideNamespace(const char c)
    {
        const std::string strChoices("123456789");
        if (strChoices.find(c) == strChoices.npos)
        {
            return false;
        }

        return true;
    }

    const char * GetMsg(const char * szNamespace, long lID, ...)
    {
        va_list args;
        va_start(args, lID);
        const char * szMsg = vget_msg(szNamespace, lID, args);
        va_end(args);

        return szMsg;
    }
};



TestBasicEventTransResAppLayer::TestBasicEventTransResAppLayer()
{
}


int TestBasicEventTransResAppLayer::run(int /*argc*/, char * argv[])
{
    printf("TestBasicEventTransResAppLayer - version: %d.%d.%d (%s)\n",
            TESTBASICEVENTTRANSRES_MAJOR,
            TESTBASICEVENTTRANSRES_MINOR,
            TESTBASICEVENTTRANSRES_PATCH,
            TESTBASICEVENTTRANSRES_DebugFlag);
    printf("\n");

    BasicEventHandling_ns::EventHandler_stderr handler_stderr;
    handler_stderr.open(argv[0]);

    CTestBasicEventTransRes app;
    app.set_event_handler(&handler_stderr);

    std::string strNamespace("ClassWithResEvents");
    std::string strNamespace2("SomethingEvents");

    std::list<std::string> listEvent;
    listEvent.push_back("1- Hello, %s!");
    listEvent.push_back("2- How old are You?");
    listEvent.push_back("3- I am %d years old.");
    listEvent.push_back("4- doing something interresting...");
    listEvent.push_back("8- get_msg_format()");

    char c(0);
    app.prompt(listEvent);
    std::cin >> c;

    const char * szMsg(NULL);

    while (c != '9')
    {
        while (!app.ValideNamespace(c))
        {
            app.advise();
            app.prompt(listEvent);
            std::cin >> c;
        }

        try
        {
            switch(c)
            {
            case '1':
                szMsg = get_msg(strNamespace.c_str(), 1, "Albert");
                std::cout << std::endl;
                std::cout << "=========> " << szMsg << "\n\n";
                break;
            case '2':
                szMsg = get_msg(strNamespace.c_str(), 2);
                std::cout << std::endl;
                std::cout << "=========> " << szMsg << "\n\n";
                break;
            case '3':
                szMsg = app.GetMsg(strNamespace.c_str(), 3, 109);
                std::cout << std::endl;
                std::cout << "=========> " << szMsg << "\n\n";
                break;
            case '4':
                szMsg = app.GetMsg(strNamespace2.c_str(), 1, 9, 15, 9);
                std::cout << std::endl;
                std::cout << "=========> " << szMsg << "\n\n";
                break;
            case '8':
                {
                    char szNamespace[64];
                    std::cout << "Namespace: ";
                    std::cin >> szNamespace;

                    long lID;
                    std::cout << "ID: ";
                    std::cin >> lID;

                    szMsg = get_msg_format(szNamespace, lID);
                    std::cout << std::endl;
                    std::cout << "=========> " << szMsg << "\n\n";
                }
                break;
            case '9':
                break;
            default:
                std::cout << std::endl;
                std::cout << "Message not found!\n\n";
                break;
            }
        }
        catch(...)
        {
            std::cout << std::endl;
            std::cout << "Message not found!\n\n";
        }

        app.prompt(listEvent);
        std::cin >> c;
    }

    return 0;
}

