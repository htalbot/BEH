
#include "TestBasicEventHandlingAppLayer.h"
#include "TestBasicEventHandling/version.h"
#include "BasicEventCorba/BasicEventCorba.h"

#include <stdio.h>

#include "BasicEventHandling/BasicEventHandling.h"
#include "ace/Init_ACE.h"
#include "ace/streams.h"
#include "ace/Date_Time.h"

#include <assert.h>

#include <sstream>

#include "ClassWithRes.h"
#include "Class2WithRes.h"

#include "Something.evc.h"
#include "ClassWithRes.evc.h"
#include "Class2WithRes.evc.h"

using namespace SomethingEvents;


#include "ace/Task.h"
class MyTask: public ACE_Task_Base
{
protected:
    int svc()
    {
        BasicEventHandling_ns::BasicEvent ev("Test...", 1, "%1(%s) - ping failure on '%2(%s)'.");
        for (int i = 0; i != 100000000; i++)
        {
            ACE_DEBUG((LM_DEBUG, "%T - %d\n", i));
            EVENT_HANDLER_SYSTEMLOG->handle_event("AAA", ev,
                LM_DEBUG,
                "start",
                "192.168.2.102");
        }

        return 0;
    }
};
// ===> guard on vhandle_event


//*********************************************************************
class EventHandler_File: public BasicEventHandling_ns::BasicEventHandler
{
    // Attention, celle classe ne gere pas de taille limite pour le fichier...
    // En fait, il s'agit ici d'un exemple d'utilisation d'heritage
    // de l'interface EventReport

public:

    EventHandler_File(const std::string & strFile)
        :    m_pOutput(NULL),
            m_strFile(strFile)
    {
        m_pOutput = new std::ofstream(m_strFile.c_str(), ios::app);
        assert (m_pOutput != NULL);

        ACE_LOG_MSG->msg_ostream(m_pOutput);
        ACE_LOG_MSG->set_flags(ACE_Log_Msg::OSTREAM);
    }

    ~EventHandler_File()
    {
        delete m_pOutput;
    }

    virtual bool vhandle_event(const char * szSource,
                                BasicEventHandling_ns::BasicEvent & ev,
                                ACE_Log_Priority nPriority,
                                va_list args)
    {
        std::string strText;
        ev.vsprintf(strText, args);

        // Prepend Date, Source, Priority and ID to the formatted string (szBuff)
        ACE_Date_Time dt;
        std::stringstream ss;
        ss << dt.year()                            // date_time/year
            << "-" << setfill('0') << setw(2)
            << dt.month()                        // date_time/month
            << "-" << setfill('0') << setw(2)
            << dt.day()                            // date_time/day
            << " " << setfill('0') << setw(2)
            << dt.hour()                        // date_time/hour
            << ":" << setfill('0') << setw(2)
            << dt.minute()                        // date_time/minute
            << ":" << setfill('0') << setw(2)
            << dt.second()                        // date_time/second
            << "." << setfill('0') << setw(3)
            << dt.microsec()                    // date_time/microsec
            << "\t"
            << szSource                            // source
            << "\t"
            << nPriority                        // Priority
            << "\t"
            << ev.id()                            // ID
            << "\t"
            << strText.c_str();                            // text

        // log
        u_long flags = ACE_LOG_MSG->flags();                // get preceeding flags
        ACE_LOG_MSG->clr_flags((u_long)~ACE_Log_Msg::OSTREAM);        // clear all flags that are not OSTREAM
        ACE_DEBUG((nPriority, ACE_TEXT(ss.str().c_str())));    // log
        ACE_LOG_MSG->set_flags(flags);                        // reset the flags to the preceeding ones

        return true;
    }

private:

    ACE_OSTREAM_TYPE * m_pOutput;
    std::string m_strFile;
};

class Something: public BasicEventHandling_ns::BasicEventRaiser
{
public:
    void DoSomething(ACE_Log_Priority lPriorityToTestFilter, long lValue)
    {
        raise_event(_SRC, ev_doing_Something, lPriorityToTestFilter,
                    /*param1*/lPriorityToTestFilter,
                    /*param2*/lValue,
                    /*param1, again!*/lPriorityToTestFilter);
    }

    void show_double(double dValue)
    {
        raise_event(_SRC, ev_msg_with_double, LM_INFO,
                    dValue);
    }

    void show_sized_string(const std::string & strValue)
    {
        raise_event(_SRC, ev_msg_with_sized_string, LM_INFO,
                    strValue.c_str());
    }
};



//******************************************************************************
// This 'BEGIN_EVENT_DEF_NAMESPACE' is not set in a different file.
// That is to test the assert when there is a syntax error
// in BEGIN_EVENT_DEF_NAMESPACE...

//    Here, errors are seen only at runtime. To see errors at compile time, place
//    BEGIN_EVENT_DEF_NAMESPACE() ... END_EVENT_NAMESPACE() in a '.evc' file
//    and include it (.evc) in a .cpp of the project.
//    In the 'Custom build step' for xyz.evc file, place 'BasicEventSyntaxChecker -f  "xyz.evc"'
//    (with something in the 'Outputs' field).
//     To assert, place this line uncommented between 'BEGIN_EVENT_DEF_NAMESPACE' and 'END_EVENT_NAMESPACE'
//    STATIC_EVENT_DEF(ev_english, 1, "The %1(%d) years old %2(%s) is not %3%s).\n")  // error: %3%s). (missing '(') ==> assert.
BEGIN_EVENT_DEF_NAMESPACE(test_event_multilang_events)
    STATIC_EVENT_DEF(ev_english, 1, "The %1(%d) years old %2(%s) is not %3(%s).\n")
    STATIC_EVENT_DEF(ev_french, 2, "Le %2(%s) age de %1(%d) ans n'est pas %3(%s).\n")
END_EVENT_NAMESPACE()

void test_event_multilang()
{
    using namespace test_event_multilang_events;

    BasicEventHandling_ns::BasicEventRaiser raiser;
    BasicEventHandling_ns::EventHandler_stderr handler_stderr;
    handler_stderr.open("test_event_multilang");
    raiser.set_event_handler(&handler_stderr);

    // The 10 years old cat is not dangerous
    raiser.raise_event(_SRC, ev_english, LM_DEBUG,
                        10,
                        "dog",
                        "dangerous");

    // Le chat age de 10 ans n'est pas dangereux
    raiser.raise_event(_SRC, ev_french, LM_DEBUG,
                        10,
                        "chien",
                        "dangereux");
}



TestBasicEventHandlingAppLayer::TestBasicEventHandlingAppLayer()
{
}


int TestBasicEventHandlingAppLayer::run()
{
    //ACE::init();

    //    MyTask task;
    //    task.activate(0, 5);

    //    task.wait();

    //ACE::fini();

    //    return 0;

    printf("TestBasicEventHandlingAppLayer - version: %d.%d.%d (%s)\n",
            TESTBASICEVENTHANDLING_MAJOR,
            TESTBASICEVENTHANDLING_MINOR,
            TESTBASICEVENTHANDLING_PATCH,
            TESTBASICEVENTHANDLING_DebugFlag);
    printf("\n");

    // Test %I64u format
    BasicEventHandling_ns::BasicEvent evI64u("TestBasicEventHandling_I64u", 1, "Call %1(%llu), %2(%d).\n");
    long long llVal(1);
    llVal <<= 60;
    long lVal0(61);
    printf("llVal = %llu, lVal = %ld\n", llVal, lVal0);
    EVENT_HANDLER_SYSTEMLOG->handle_event(_SRC, evI64u, LM_DEBUG, llVal, lVal0);

    ////// Test %d format (pour voir si ça marche encore après l'intégration de %I64u)
    ////BasicEventHandling_ns::BasicEvent evD("TestBasicEventHandling_D", 1, "Call %1(%d).\n");
    ////long lVal(1);
    ////lVal <<= 30;
    ////EVENT_HANDLER_SYSTEMLOG->handle_event(_SRC, evD, LM_DEBUG, lVal);


    BasicEventHandling_ns::BasicEvent ev("TestBasicEventHandling_mainEvents", 1, "Call %1(%s).\n");

    // Test when the number of arguments is less than the number specified in specs
  //  try
  //  {
     //   BasicEventHandling_ns::BasicEvent ev_test_nb_args("TestBasicEventHandling_mainEvents", 1, "Parameter1: %1(%s), Parameter2: %2(%s).\n");
     //   EVENT_HANDLER_SYSTEMLOG->handle_event(_SRC, ev_test_nb_args, LM_DEBUG, "aa");
  //  }
  //  catch(...)
  //  {
        //ACE_DEBUG((LM_ERROR, ACE_TEXT("Bad number of arguments!!!")));    // log
  //  }

    // Something is a BasicEventHandling_ns::BasicEventRaiser child
    Something thing1;
    thing1.DoSomething(LM_DEBUG, 34); // reports to system log by default
    thing1.show_double(1.2345); // reports to system log by default
    thing1.show_sized_string("Hello"); // reports to system log by default

    Something thing2;
    {
        EventHandler_File handler_file("test_EventReport.txt");
        thing2.set_event_handler(&handler_file); // to report to file only
        thing2.DoSomething(LM_DEBUG, 79); // reports to file only
        thing2.set_event_handler(NULL); // do not report to file any longer
    }

    thing2.set_filter(LM_DEBUG); // want only LM_DEBUG
    thing2.DoSomething(LM_INFO, 81); // not reported
    thing2.DoSomething(LM_DEBUG, 87); // reported into system log
    thing2.set_filter(ACE_Log_Priority(LM_WARNING | LM_INFO)); // want only LM_WARNING and LM_INFO
    thing2.DoSomething(LM_INFO, 93); // reported into system log
    thing2.DoSomething(LM_DEBUG, 94); // not reported
    thing2.DoSomething(LM_WARNING, 95); // reported into system log
    thing2.set_filter(); // reset filter to all priorities
    thing2.DoSomething(LM_INFO, 82); // reported into system log
    thing2.DoSomething(LM_DEBUG, 89); // reported into system log

    // ClassWithRes is a BasicEventHandling_ns::BasicEventRaiser child
    ClassWithRes res1;
    res1.run(1, "Johnny");    // reported into system log
    res1.run(2);            // reported into system log
    res1.run(3, 32);        // reported into system log

    {
        Class2WithRes res2;
        EventHandler_File handler_file("test_EventReport.txt");
        res2.set_event_handler(&handler_file); // to report to file only
        res2.run(1, "Hubert");    // reported into file
        res2.run(2);            // reported into file
        res2.run(3, 25);        // reported into file
        res2.run(4);            // reported into file
        thing2.set_event_handler(NULL); // do not report to file any longer
    }

    thing2.raise_event(_SRC, ClassWithResEvents::ev1, LM_DEBUG,
                        "Georges");    // reported into system log
    thing2.raise_event(_SRC, Class2WithResEvents::ev1, LM_DEBUG,
                        "Paul");    // reported into system log

    // Do it last. It shows that the parameters calling order
    // is the same in french and in english even when there are
    // differences in format of the two languages.
//    test_event_multilang();

    BasicEventHandling_ns::BasicEvent ev4corba("Ev4CorbaEvents", 1, "Event id = %1(%d), text = %2(%s), float = %3(%lf), %4(%s).");

    {
        IBASICEVENTCORBA_module::BasicEventCorba event_corba;
        for (int i = 0; i != 1; i++)
        {
            BasicEventCorba_ns::BasicEventCorba::convert(
                ev4corba,
                event_corba,
                "localhost",
                "TestBasicEventHandling::main()",
                LM_DEBUG,
                10,
                "123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890",
                5.0f,
                "point final");

            printf("%d - Event id = %d, format = \"%s\"\n", i, event_corba.event_id_, event_corba.format_.operator const CORBA::Char *());

            ACE_Time_Value tv(0, 100000);
            ACE_OS::sleep(tv);
        }
    }


    //ACE_OS::sleep(5);

    return 0;
}

