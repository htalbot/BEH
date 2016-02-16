
#include "BasicEventTransResImpl/BasicEventTransResImpl.h"

#include "BasicEventSyntaxCheckerAppLayer.h"
#include "BasicEventSyntaxChecker/version.h"
#include <stdio.h>

#include "ACEWin64Warnings/BeginSuppressWarnings.h"
#include "ace/Get_Opt.h"
#include "ace/ACE.h"
#include "ACEWin64Warnings/EndSuppressWarnings.h"

#include <string>
#include <iostream>
#include <sstream>

BEGIN_EVENT_DEF_NAMESPACE(BasicEventSyntaxCheckerEvents)
    STATIC_EVENT_DEF(ev_can_not_create_output_file, 1, "Can not create output file: '%1(%s)'.")
END_EVENT_NAMESPACE()

using namespace BasicEventSyntaxCheckerEvents;

class BasicEventSyntaxChecker: public BasicEventHandling_ns::BasicEventRaiser
{
public:
    void print_syntax(const std::string & strReason,
                        const std::string & strProgName)
    {
        const char * fname = ACE::basename(strProgName.c_str());

        std::cout << fname << " failed.\n\n";
        std::cout << "Reason: " << strReason.c_str() << "\n\n";
        std::cout << "syntax: '" << fname << "<-f EventDefSourceFile>\n\n";
        std::cout << "EventDefSourceFile:\tfile containing events.\n";
    }

    bool get_params(int argc,
                    char * argv[],
                    std::string & strEventDefSourceFile)
    {
        const ACE_TCHAR options[] = ACE_TEXT("f:");
        ACE_Get_Opt cmd_opts(argc, argv, options);

        int option;
        if ((option = cmd_opts()) == EOF)
        {
            print_syntax("No argument.", argv[0]);
            return false;
        }

        std::stringstream ss;
        bool bError(false);
        while (option != EOF)
        {
            switch(option)
            {
            case 'f':
                strEventDefSourceFile = cmd_opts.opt_arg();
                break;
            case '?':
                bError = true;
                ss << "Mandatory argument is missing for option: '" << (char)cmd_opts.opt_opt() << "'.";
                break;
            default:
                bError = true;
                ss << "Bad parameters.";
                break;
            }

            if (bError)
            {
                print_syntax(ss.str(), argv[0]);
                break;
            }

            option = cmd_opts();
        }

        if (bError)
        {
            return false;
        }

        bool bMissingParam = strEventDefSourceFile.empty();
        if (bMissingParam)
        {
            print_syntax("Missing parameter.", argv[0]);
            return false;
        }

        return true;
    }

    bool CreateEventsCPP(const std::string & strInputFile)
    {
        std::string strName = ACE::basename(strInputFile.c_str());
        strName.find('.');
        std::string::size_type loc = strName.find('.');
        if (loc != std::string::npos)
        {
            strName = strName.substr(0, loc);
        }

        std::string strOutputFile = strName + ".events.cpp";
        std::ofstream ofs;
        ofs.open(strOutputFile.c_str());
        if (ofs.fail())
        {
            raise_event(_SRC, ev_can_not_create_output_file, LM_ERROR,
                        strOutputFile.c_str());
            return false;
        }

        std::string strIncludeFile = strName + ".evc";
        ofs << "#include \"" << strIncludeFile.c_str() << "\"" << std::endl;

        return true;
    }
};

BasicEventSyntaxCheckerAppLayer::BasicEventSyntaxCheckerAppLayer()
{
}


int BasicEventSyntaxCheckerAppLayer::run(int argc, char * argv[])
{
    //printf("BasicEventSyntaxCheckerAppLayer - version: %d.%d.%d (%s)\n",
    //        BASICEVENTSYNTAXCHECKER_MAJOR,
    //        BASICEVENTSYNTAXCHECKER_MINOR,
    //        BASICEVENTSYNTAXCHECKER_PATCH,
    //        BASICEVENTSYNTAXCHECKER_DebugFlag);
    //printf("\n");

    BasicEventHandling_ns::EventHandler_stderr handler_stderr;
    handler_stderr.open(argv[0]);

    std::string strEventDefSourceFile;

    BasicEventSyntaxChecker checker;
    checker.set_event_handler(&handler_stderr);
    if (!checker.get_params(argc,
                            argv,
                            strEventDefSourceFile))
    {
        return 1;
    }

    Events events;
    events.set_event_handler(&handler_stderr);

    if (!events.ReadEventsSourceFile(strEventDefSourceFile))
    {
        return 2;
    }

    if (!checker.CreateEventsCPP(strEventDefSourceFile))
    {
        return 3;
    }

    return 0;
}

