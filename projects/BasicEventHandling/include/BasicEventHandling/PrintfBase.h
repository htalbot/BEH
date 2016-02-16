#ifndef __SPRINTFBASE_H__
#define __SPRINTFBASE_H__

#ifdef ACE_WIN32
#pragma warning(disable : 4251) // wittingly: DPO deals with this issue.
#endif

#include <string>
#include <map>
#include <sstream>

namespace BasicEventHandling_ns {

union DATA
{
    long lValue;
    unsigned long ulValue;
    double dValue;
    long double ldValue;
    unsigned long long llValue;
    unsigned long long ullValue;
    char * strValue;
};

struct PrintfArg
{
    PrintfArg()
    {
        memset(&data, 0, sizeof(unsigned long long));
        is_text_ = false;
    }

    ~PrintfArg()
    {
        if (is_text_)
        {
            delete [] data.strValue;
        }
    }

    PrintfArg(const PrintfArg& rhs)
    {
        cpy(rhs);
    }

    const PrintfArg& operator=(const PrintfArg& rhs)
    {
        cpy(rhs);
        return *this;
    }

    void cpy(const PrintfArg& rhs)
    {
        this->spec_ = rhs.spec_;
        this->text_ = rhs.text_;
        this->is_text_ = rhs.is_text_;

        if (rhs.is_text_)
        {
            this->data.strValue = new char[strlen(rhs.data.strValue) + 1];
            strcpy(this->data.strValue, rhs.data.strValue);
        }
        else
        {
            this->data = rhs.data;
        }
    }

    std::string spec_;
    std::string text_;
    bool is_text_;
    DATA data;
};

class BASICEVENTHANDLING_Export PrintfBase
{
public:
    enum Action
    {
        PREPARE,
        SET,
        RESOLVE
    };

protected:
    virtual long get_long_value(int /*param_no*/) // %d, %i, %o, %x, %X, %ld, %li, %lo, %lx, %lX
    {
        long n = va_arg(va_args_, long);
        return n;
    }

    virtual unsigned long get_unsigned_long_value(int /*param_no*/) // %lu
    {
        unsigned long n = va_arg(va_args_, unsigned long);
        return n;
    }

    virtual double get_double_value(int /*param_no*/) // %lf, %lF, %le, %lE, %lg, %lG
    {
        double n = va_arg(va_args_, double);
        return n;
    }

    virtual long long get_long_long_value(int /*param_no*/) // lld
    {
        long long n = va_arg(va_args_, long long);
        return n;
    }

    virtual unsigned long long get_unsigned_long_long_value(int /*param_no*/) // llu
    {
        unsigned long long n = va_arg(va_args_, unsigned long long);
        return n;
    }

    virtual std::string get_string_value(int /*param_no*/) // %s
    {
        std::string s;

        char * pszValue = va_arg(va_args_, char *);
        if (strlen(pszValue) > 0)
        {
            s = pszValue;
        }

        return s;
    }


    virtual void loop(std::string & text, const std::string & format)
    {
        printf_args_.clear();

        for (int action = PREPARE; action <= RESOLVE; action++)
        {
            const char * pFormat = format.c_str();
            const char * pLast = pFormat + format.length() - 1;

            if (action == SET)
            {
                set();
                continue;
            }

            if (action == RESOLVE)
            {
                text.clear();
            }

            while (pFormat <= pLast)
            {
                if (*pFormat == '%')
                {
                    if (pFormat != pLast)
                    {
                        if (*(pFormat + 1) == '%')
                        {
                            if (action == RESOLVE)
                            {
                                text += "%";
                            }
                            pFormat += 2;
                        }
                        else
                        {
                            int param_no(0);
                            int nParameters = sscanf(pFormat + 1, "%d", &param_no);
                            if (nParameters == 0 || nParameters == EOF)
                            {
                                u_long prev_flags = ACE_LOG_MSG->flags(); // remember previous flags
                                ACE_LOG_MSG->set_flags(ACE_Log_Msg::SYSLOG); // set flags as SYSLOG
                                ACE_LOG_MSG->clr_flags((u_long)~ACE_Log_Msg::SYSLOG); // remove all flags that are not SYSLOG
                                ACE_DEBUG((LM_ERROR, ACE_TEXT("Event with bad format"))); // log
                                ACE_LOG_MSG->clr_flags(ACE_Log_Msg::SYSLOG); // remove SYSLOG flag
                                ACE_LOG_MSG->set_flags(prev_flags); // reset the flags to the previuous value

                                return;
                            }

                            while (*(++pFormat) != '%')
                            {
                            }

                            std::string spec("%");

                            while (*(++pFormat) != ')')
                            {
                                spec += pFormat[0];
                            }

                            switch(action)
                            {
                                case PREPARE:
                                    prepare(spec,  param_no);
                                    break;

                                case RESOLVE:
                                    std::string value_string;
                                    resolve(param_no, value_string);
                                    text += value_string;
                                    break;
                            };

                            pFormat = strchr(pFormat, ')');
                            ++pFormat;
                        }
                    }
                    else
                    {
                        if (action == RESOLVE)
                        {
                            text += *pFormat;
                        }
                        ++pFormat;
                    }
                }
                else
                {
                    if (action == RESOLVE)
                    {
                        text += *pFormat;
                    }
                    ++pFormat;
                }
            }
        }
    }

public:
    void prepare(const std::string & spec, int param_no)
    {
        PrintfArg arg;
        arg.spec_ = spec;
        printf_args_[param_no] = arg;
    }

    void set()
    {
        std::map<long, PrintfArg>::iterator it;
        for (it = printf_args_.begin(); it != printf_args_.end(); it++)
        {
            std::stringstream ss;

            if (it->second.spec_ == "%d"
                || it->second.spec_ == "%i"
                || it->second.spec_ == "%o"
                || it->second.spec_ == "%x"
                || it->second.spec_ == "%X"
                || it->second.spec_ == "%ld"
                || it->second.spec_ == "%li"
                || it->second.spec_ == "%lo"
                || it->second.spec_ == "%lx"
                || it->second.spec_ == "%lX")
            {
                long n = get_long_value(it->first);
                it->second.data.lValue = n;
                ss << n;
            }
            else if (it->second.spec_ == "%u")
            {
                unsigned long n = get_unsigned_long_value(it->first);
                it->second.data.ulValue = n;
                ss << n;
            }
            else if (it->second.spec_ == "%f"
                        || it->second.spec_ == "%F"
                        || it->second.spec_ == "%e"
                        || it->second.spec_ == "%E"
                        || it->second.spec_ == "%g"
                        || it->second.spec_ == "%G"
                        || it->second.spec_ == "%lf"
                        || it->second.spec_ == "%lF"
                        || it->second.spec_ == "%le"
                        || it->second.spec_ == "%lE"
                        || it->second.spec_ == "%lg"
                        || it->second.spec_ == "%lG")
            {
                double n = get_double_value(it->first);
                it->second.data.dValue = n;
                ss << n;
            }
            else if (it->second.spec_ == "%ll")
            {
                long long n = get_long_long_value(it->first);
                it->second.data.llValue = n;
                ss << n;
            }
            else if (it->second.spec_ == "%llu")
            {
                unsigned long long n = get_unsigned_long_long_value(it->first);
                it->second.data.ullValue = n;
                ss << n;
            }
            else if (it->second.spec_ == "%s")
            {
                ss << get_string_value(it->first);
                it->second.data.strValue = new char[ss.str().length() + 1];
                memset(it->second.data.strValue, 0, ss.str().length());
                strcpy(it->second.data.strValue, const_cast<char *>(ss.str().c_str()));
                it->second.is_text_ = true;
            }

            it->second.text_ = ss.str();
        }
    }

    void set(va_list & args)
    {
        va_list previous;
        va_copy(previous, args);
        va_copy(va_args_, args);
        set();
        va_end(va_args_);
        va_end(previous);
    }

    void resolve(int param_no, std::string & value_string)
    {
        value_string = printf_args_[param_no].text_;
    }

    std::map<long, PrintfArg> & get_args()
    {
        return printf_args_;
    }

protected:
    std::map<long, PrintfArg> printf_args_;
    va_list va_args_;
};

}; // BasicEventHandling_ns


#endif // __SPRINTFBASE_H__
