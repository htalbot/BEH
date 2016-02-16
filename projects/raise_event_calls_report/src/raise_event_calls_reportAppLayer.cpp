
#include "raise_event_calls_reportAppLayer.h"
#include "raise_event_calls_report/version.h"
#include <stdio.h>

#include "BasicEventTransResImpl/BasicEventTransResImpl.h"

#include "ACEWin64Warnings/BeginSuppressWarnings.h"
#include "ace/ACE.h"
#include "ace/Get_Opt.h"
#include "ACEWin64Warnings/EndSuppressWarnings.h"

#include <string>
#include <sstream>
#include <list>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <memory>

BEGIN_EVENT_DEF_NAMESPACE(RaiseEventCallsReportEvents)
    STATIC_EVENT_DEF(ev_file_access, 1, "Can not open '%1(%s)'\n\n")
END_EVENT_NAMESPACE()

using namespace RaiseEventCallsReportEvents;

//****************************************************************************
class LineCoordinates
{
public:
    LineCoordinates(std::streamoff lBegin = 0, std::streamoff lEnd = 0)
        :    lBegin_(lBegin),
            lEnd_(lEnd)
    {
    }

    std::streamoff lBegin_;
    std::streamoff lEnd_;
};


//****************************************************************************
class EventCall
{
public:
    EventCall(std::streamoff lLineNumber,
                const std::string & strCall,
                const std::string & strEventName)
        :    lLineNumber_(lLineNumber),
            strCall_(strCall),
            strEventName_(strEventName)
    {
    }

    std::streamoff get_line_number() { return lLineNumber_; }
    std::string get_event_call() { return strCall_; }
    std::string get_event_name() { return strEventName_; }

private:

    std::streamoff lLineNumber_;
    std::string strCall_;
    std::string strEventName_;
};


//****************************************************************************
class Report: public BasicEventHandling_ns::BasicEventRaiser
{
public:
    void print_syntax(const std::string & strReason,
                        const std::string & strProgName)
    {
        const char * fname = ACE::basename(strProgName.c_str());

        std::cout << fname << " failed.\n\n";
        std::cout << "Reason: " << strReason << "\n\n";
        std::cout << "syntax: '" << fname << " <-s EventDefSourceFile>, <-r RaiseEventSourceFile>\n\n";
        std::cout << "EventDefSourceFile:\tfile containing the list of files defining EVENT_DEF.\n";
        std::cout << "RaiseEventSourceFile:\tfile containing the list of files using raise_event() method.\n";
    }


    bool get_params(int argc,
                    char * argv[],
                    std::string & strEventDefSourceFile,
                    std::string & strRaiseEventSourceFile)
    {
        const ACE_TCHAR options[] = ACE_TEXT("s:r:");
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
            case 'r':
                strRaiseEventSourceFile = cmd_opts.opt_arg();
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
                                strRaiseEventSourceFile.empty();
        if (bMissingParam)
        {
            print_syntax("Missing parameter.", argv[0]);
            return false;
        }

        return true;
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
                if (!line.empty())
                {
                    list.push_back(line);
                }
            }
            ifs.close();
        }

        return true;
    }

    bool parse_raiseevent_source_file(const std::string & strRaiseEventSourceFile,
                                        std::list<std::string> & list)
    {
        std::ifstream ifs;
        ifs.open(strRaiseEventSourceFile.c_str());
        if (ifs.fail())
        {
            raise_event(_SRC, ev_file_access, LM_ERROR,
                        strRaiseEventSourceFile.c_str());
            return false;
        }
        else
        {
            while (!ifs.eof())
            {
                std::string line;
                getline( ifs, line );
                if (!line.empty())
                {
                    list.push_back(line);
                }
            }
            ifs.close();
        }

        return true;
    }

    bool extract_raise_event_calls(const std::string & strFileName,
                                    std::vector<EventCall> & vectEventCall)
    {
        std::ifstream ifs;
        ifs.open(strFileName.c_str());
        if (ifs.fail())
        {
            raise_event(_SRC, ev_file_access, LM_ERROR,
                        strFileName.c_str());
            return false;
        }
        else
        {
            const std::string strRaiseEventCall("raise_event");
            const size_t lBufferSize = strRaiseEventCall.length();

            char * szBuffer = new char[lBufferSize + 1];
            std::auto_ptr<char> spBuffer(szBuffer);
            memset(szBuffer, 0, (lBufferSize  + 1) * sizeof(char));

            // Get the coordinates of begining/ending of lines
            std::map<std::streamoff, LineCoordinates> mapLine;
            std::streamoff lPosBegin(0);
            std::streamoff lPosEnd(0);
            std::streamoff lLineCount(1);
            char c;
            std::streamoff lOffset(0);
            std::string strLine;
            while (ifs.get(c))
            {
                if (c == '\n')
                {
                    lPosEnd = lPosBegin + (lOffset ? lOffset - 1 : lOffset);
                    mapLine[lLineCount] = LineCoordinates(lPosBegin, lPosEnd);
                    lPosBegin = ifs.tellg();
                    lOffset = 0;
                    ++lLineCount;
                    strLine.clear();
                }
                else
                {
                    strLine += c;
                    ++lOffset;
                }
            }

            // Restart from the begining of the file to find line numbers containing raise_event call.
            ifs.clear();
            ifs.seekg(0);

            std::list<std::streamoff> listLineNumber;

            std::streamoff lPos = ifs.tellg();
            while (ifs.read(szBuffer, lBufferSize ))
            {
                std::string strText(szBuffer);

                if (strText.find(strRaiseEventCall) != strText.npos)
                {
                    std::map<std::streamoff, LineCoordinates>::iterator it;
                    for (it = mapLine.begin(); it != mapLine.end(); it++)
                    {
                        std::streamoff lLineNumber = it->first;
                        LineCoordinates coord = it->second;
                        if (coord.lBegin_ <= lPos && lPos <= coord.lEnd_)
                        {
                            listLineNumber.push_back(lLineNumber);
                            break;
                        }
                    }
                }

                ++lPos;

                ifs.seekg(lPos);
            }

            // go back to each line and analyze raise_event call
            std::list<std::streamoff>::iterator it;
            for (it = listLineNumber.begin(); it != listLineNumber.end(); it++)
            {
                LineCoordinates coord = mapLine[*it];

                ifs.clear();
                ifs.seekg(coord.lBegin_);

                std::streamoff lPos = ifs.tellg();
                while (ifs.read(szBuffer, lBufferSize) && lPos < coord.lEnd_)
                {
                    std::string strText(szBuffer);

                    if (strText.find(strRaiseEventCall) != strText.npos)
                    {
                        std::string strParams;

                        long lCount(0);
                        bool bOpen(false);

                        char c;
                        while (ifs.get(c))
                        {
                            if (bOpen)
                            {
                                if (c == '(')
                                {
                                    ++lCount;
                                }

                                if (c == ')')
                                {
                                    --lCount;
                                }

                                strParams += c;
                            }
                            else
                            {
                                if (c == ' ' || c == '\n')
                                {
                                    strParams += c;
                                    continue;
                                }

                                if (c == '(')
                                {
                                    ++lCount;
                                    strParams += c;
                                    bOpen = true;
                                    continue;
                                }
                                else
                                {
                                    break; // not interresting raise_event call
                                }
                            }

                            if (lCount == 0)
                            {
                                break;
                            }
                        }
                        if (bOpen)
                        {
                            std::string strEventName;
                            if (get_event_name_from_params(strParams, strEventName))
                            {
                                std::string strEventCall("raise_event");
                                strEventCall += strParams;
                                EventCall event_call(*it, strEventCall, strEventName);
                                vectEventCall.push_back(event_call);
                            }
                        }
                    }

                    ++lPos;

                    ifs.seekg(lPos);
                }
            }
        }

        return true;
    }

    bool get_event_name_from_params(const std::string & strParams, std::string & strEventName)
    {
        // first comma
        size_t nPosBegin(0);
        size_t nPos1 = strParams.find(',', nPosBegin);
        if (nPos1 == strParams.npos)
        {
            return false;
        }

        // second comma
        size_t nPos2 = strParams.find(',', nPos1 + 1);
        if (nPos2 == strParams.npos)
        {
            return false;
        }

        strEventName = strParams.substr(nPos1 + 1, nPos2 - nPos1 - 1);

        // Trim strName
        size_t nPosTrim1 = strEventName.find_first_not_of(' ');
        size_t nPosTrim2 = strEventName.find_last_not_of(' ');
        strEventName = strEventName.substr(nPosTrim1, nPosTrim2 - nPosTrim1 + 1);

        size_t lPosNamespaceScope = strEventName.find("::");
        if (lPosNamespaceScope != strEventName.npos)
        {
            strEventName = strEventName.substr(lPosNamespaceScope + 2,
                                                strEventName.length() - lPosNamespaceScope - 2);
        }

        return true;
    }

#if defined (ACE_WIN32)
//    VC71 reports a strange warning: warning C4100: 'events' : unreferenced formal parameter
//    (even if it is used inside the function)
#    pragma warning( push )
#    pragma warning (disable : 4100)
#endif
    void get_possible_event_objects(const std::string & strEventName,
                                        const Events & events,
                                        std::vector<EventObject> & vectEventObject)
    {
        TMap_NSID_NS::iterator itNamespace;
        for (itNamespace = events.m_map_NSID_NS.begin(); itNamespace != events.m_map_NSID_NS.end(); itNamespace++)
        {
            Namespace * pNamespace = itNamespace->second;
            const TMap_ID_EventObject & m = pNamespace->get_id_eventobject_map();
            TMap_ID_EventObject::const_iterator itIDEventObject;
            for (itIDEventObject = m.begin(); itIDEventObject != m.end(); itIDEventObject++)
            {
                std::string strName = itIDEventObject->second->get_name();
                if (strName == strEventName)
                {
                    vectEventObject.push_back(*itIDEventObject->second);
                }
            }
        }
    }
#if defined (ACE_WIN32)
#    pragma warning( pop )
#endif

    void show_calls(const std::string & strFile,
                    const std::vector<EventCall> vectEventCall,
                    const Events & events)
    {
        long nBad(0);

        if (!vectEventCall.empty())
        {
            std::cout << "***** File: " << strFile << "*****" << std::endl;
            std::vector<EventCall>::const_iterator itEventCall;
            for (itEventCall = vectEventCall.begin(); itEventCall != vectEventCall.end(); itEventCall++)
            {
                EventCall event_call = *itEventCall;
                std::cout << "---> line " << event_call.get_line_number() << std::endl;
                std::cout << "\t" << event_call.get_event_call() << std::endl;
                std::vector<EventObject> vectEventObject;
                get_possible_event_objects(event_call.get_event_name(), events, vectEventObject);

                if (!vectEventObject.empty())
                {
                    std::vector<EventObject>::iterator itEventObject;
                    for (itEventObject = vectEventObject.begin(); itEventObject != vectEventObject.end(); itEventObject++)
                    {
                        std::cout << "\t!!!!! "
                                    << itEventObject->get_namespace()
                                    << " - "
                                    << itEventObject->get_format()
                                    << std::endl;
                    }
                }
                else
                {
                    ++nBad;
                    std::cout << "\tXXXXX no namespace found in event sources." << std::endl;
                }

                std::cout << std::endl;
            }
            std::cout << std::endl;
        }

        std::cout << "\n\n***** " << nBad << " bad call(s) found (look for XXXXX symbols)." << std::endl;
    }
};


Raise_event_calls_reportAppLayer::Raise_event_calls_reportAppLayer()
{
}


int Raise_event_calls_reportAppLayer::run(int argc, char * argv[])
{
    printf("Raise_event_calls_reportAppLayer - version: %d.%d.%d (%s)\n",
            RAISE_EVENT_CALLS_REPORT_MAJOR,
            RAISE_EVENT_CALLS_REPORT_MINOR,
            RAISE_EVENT_CALLS_REPORT_PATCH,
            RAISE_EVENT_CALLS_REPORT_DebugFlag);
    printf("\n");

    BasicEventHandling_ns::EventHandler_stderr handler_stderr;
    handler_stderr.open(argv[0]);

    std::string strEventDefSourceFile;
    std::string strRaiseEventSourceFile;

    Report report;
    report.set_event_handler(&handler_stderr);
    if (!report.get_params(argc,
                                argv,
                                strEventDefSourceFile,
                                strRaiseEventSourceFile))
    {
        return -1;
    }

    std::list<std::string> listEventDefSourceFile;
    if (!report.parse_eventdef_source_file(strEventDefSourceFile, listEventDefSourceFile))
    {
        return -1;
    }

    Events events;
    events.set_event_handler(&handler_stderr);

    std::list<std::string>::iterator it;
    for (it = listEventDefSourceFile.begin(); it != listEventDefSourceFile.end(); it++)
    {
        events.ReadEventsSourceFile(it->c_str());
    }

    std::list<std::string> listRaiseEventSourceFile;
    if (!report.parse_raiseevent_source_file(strRaiseEventSourceFile, listRaiseEventSourceFile))
    {
        return -1;
    }

    std::vector<EventCall> vectEventCall;
    for (it = listRaiseEventSourceFile.begin(); it != listRaiseEventSourceFile.end(); it++)
    {
        if (report.extract_raise_event_calls(*it, vectEventCall))
        {
            report.show_calls(*it, vectEventCall, events);
        }

           vectEventCall.clear();
    }

    std::cout << std::endl;

    return 0;
}

