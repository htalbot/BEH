
#include "GenBasicEventTransResAppLayer.h"
#include "GenBasicEventTransRes/version.h"
#include <stdio.h>

#include "ACEWin64Warnings/BeginSuppressWarnings.h"
#include "ace/Log_Msg.h"
#include "ace/Get_Opt.h"
#include "ace/ACE.h"

#include "BasicEventTransResImpl/BasicEventTransResImpl.h"

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <list>
#include "ACEWin64Warnings/EndSuppressWarnings.h"

BEGIN_EVENT_DEF_NAMESPACE(GenBasicEventTransResEvents)
    STATIC_EVENT_DEF(ev_program_failed, 1, "%1(%s) failed.\n")
    STATIC_EVENT_DEF(evReason, 2, "%1(%s)\n\n")
    STATIC_EVENT_DEF(ev_syntax_info, 3, "Syntax: '%1(%s)' <-s EventDefSourceFile>, <-t TranslationFile>, <-o EventsDLLResFile>\n\n")
    STATIC_EVENT_DEF(ev_syntax_info2, 4, "%1(%s)\n\n")
    STATIC_EVENT_DEF(ev_file_access, 5, "Can not open '%1(%s)'\n\n")
    STATIC_EVENT_DEF(ev_not_all_translations_found, 6, "Not all translations have been found\n\n")
END_EVENT_NAMESPACE()

using namespace GenBasicEventTransResEvents;

class Generator: public BasicEventHandling_ns::BasicEventRaiser
{
public:
    void print_syntax(const std::string & strReason,
                        const std::string & strProgName)
    {
        const char * fname = ACE::basename(strProgName.c_str());

        std::cout << fname << " failed.\n\n";
        std::cout << "Reason: " << strReason.c_str() << "\n\n";
        std::cout << "syntax: " << fname << " <-s EventDefSourceFile>, <-t TranslationFile>, <-o EventTransRes> <-l language>\n\n";
        std::cout << "EventDefSourceFile:\tfile containing the list of files defining EVENT_DEF.\n";
        std::cout << "TranslationFile:\tfile containing all previously defined translations.\n";
        std::cout << "EventTransRes:\toutput file that will be used to generate BasicEventTransRes.\n\n";
    }


    bool parse_eventdef_source_file(const std::string & strEventDefSourceFile,
                                    std::list<std::string> & list)
    {
        std::ifstream ifs;
        ifs.open(strEventDefSourceFile.c_str());
        if (ifs.fail())
        {
            raise_event(_SRC, ev_file_access, LM_ERROR,
                        strEventDefSourceFile.c_str());
            return false;
        }
        else
        {
            while (!ifs.eof())
            {
                std::string line;
                getline( ifs, line );
                list.push_back(line);
            }
            ifs.close();
        }

        return true;
    }

    bool get_params(int argc,
                    char * argv[],
                    std::string & strEventDefSourceFile,
                    std::string & strTranslationFile,
                    std::string & strEventsDLLResFile,
                    std::string & strLanguage)
    {
        const ACE_TCHAR options[] = ACE_TEXT("s:t:o:l:");
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
            case 's':
                strEventDefSourceFile = cmd_opts.opt_arg();
                break;
            case 't':
                strTranslationFile = cmd_opts.opt_arg();
                break;
            case 'o':
                strEventsDLLResFile = cmd_opts.opt_arg();
                break;
            case 'l':
                strLanguage = cmd_opts.opt_arg();
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

        bool bMissingParam = strEventDefSourceFile.empty() ||
                                strTranslationFile.empty() ||
                                strEventsDLLResFile.empty() ||
                                strLanguage.empty();
        if (bMissingParam)
        {
            print_syntax("Missing parameter.", argv[0]);
            return false;
        }

        return true;
    }

};

GenBasicEventTransResAppLayer::GenBasicEventTransResAppLayer()
{
}


int GenBasicEventTransResAppLayer::run(int argc, char * argv[])
{
    printf("GenBasicEventTransResAppLayer - version: %d.%d.%d (%s)\n",
            GENBASICEVENTTRANSRES_MAJOR,
            GENBASICEVENTTRANSRES_MINOR,
            GENBASICEVENTTRANSRES_PATCH,
            GENBASICEVENTTRANSRES_DebugFlag);
    printf("\n");

    BasicEventHandling_ns::EventHandler_stderr handler_stderr;
    handler_stderr.open(argv[0]);

    std::string strEventDefSourceFile;
    std::string strTranslationFile;
    std::string strEventsDLLResFile;
    std::string strLanguage;

    Generator generator;
    generator.set_event_handler(&handler_stderr);
    if (!generator.get_params(argc,
                                argv,
                                strEventDefSourceFile,
                                strTranslationFile,
                                strEventsDLLResFile,
                                strLanguage))
    {
        return -1;
    }

    std::list<std::string> listEventDefSourceFile;
    if (!generator.parse_eventdef_source_file(strEventDefSourceFile,
                                                /*==>*/ listEventDefSourceFile))
    {
        return -1;
    }

    Events events;
    events.set_event_handler(&handler_stderr);
    if (!events.LoadTranslationFile(strTranslationFile))
    {
        return -1;
    }

    std::list<std::string>::iterator it;
    for (it = listEventDefSourceFile.begin(); it != listEventDefSourceFile.end(); it++)
    {
        events.ReadEventsSourceFile(it->c_str());
    }

    bool bAllTranslationsFound(true);
    if (!events.CreateEventsDLLTable(strEventsDLLResFile, strLanguage, bAllTranslationsFound))
    {
        return -1;
    }

    if (!bAllTranslationsFound)
    {
        generator.raise_event(_SRC, ev_not_all_translations_found, LM_WARNING);
        return ev_not_all_translations_found.id();
    }

    return 0;
}

