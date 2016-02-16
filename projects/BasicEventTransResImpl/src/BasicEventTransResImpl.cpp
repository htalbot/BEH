
#include "BasicEventHandling/BasicEventHandling.h"
#include "BasicEventTransResImpl/BasicEventTransResImpl.h"

#include "ACEWin64Warnings/BeginSuppressWarnings.h"
#include "ace/Configuration.h"
#include "ace/Configuration_Import_Export.h"
#include "ACEWin64Warnings/EndSuppressWarnings.h"

#include <sstream>

BEGIN_EVENT_DEF_NAMESPACE(BasicEventTransResEvents)
    STATIC_EVENT_DEF(ev_adding_id_format_failure,                1, "Namespace '%1(%s)', Event '%2(%d)' already exists.")
    STATIC_EVENT_DEF(ev_adding_language_format_failure,         2, "Language '%1(%s)' already exists.")
    STATIC_EVENT_DEF(ev_opening_file_failure,                    3, "Open file '%1(%s)' failed.")
    STATIC_EVENT_DEF(ev_begin_event_def_namespace_not_found,    4, "BEGIN _ EVENT _ DEF _ NAMESPACE macro not found (bad syntax in file '%1(%s)').")
    STATIC_EVENT_DEF(ev_end_event_namespace_not_found,            5, "END _ EVENT _ NAMESPACE macro not found (bad syntax in file '%1(%s)').")
    STATIC_EVENT_DEF(ev_event_def_bad_syntax,                    6, "STATIC_EVENT_DEF: bad syntax in file '%1(%s)'.")
    STATIC_EVENT_DEF(ev_no_comma,                                7, "'%1(%s)' - No comma in parameters.")
    STATIC_EVENT_DEF(ev_no_second_comma,                        8, "'%1(%s)' - Comma is missing in parameters.")
    STATIC_EVENT_DEF(ev_parsing_failure_for_id,                    9, "'%1(%s)' - Parsing parameters failed for id.")
    STATIC_EVENT_DEF(ev_ace_cfg_heap,                            10, "Can not open the file with ACE_Configuration_Heap (file = %1(%s)).")
    STATIC_EVENT_DEF(ev_ace_cfg_import,                            11, "Can not import config from file '%1(%s)'.")
    STATIC_EVENT_DEF(ev_ace_open_section,                        12, "Can not open section '%1(%s)' in file '%2(%s)'.")
    STATIC_EVENT_DEF(ev_ace_get_string_value,                    13, "Can not get string value for section '%1(%s)' in file '%2(%s)'.")
    STATIC_EVENT_DEF(ev_no_language_specified,                    14, "No language specified.")
    STATIC_EVENT_DEF(ev_event_not_found_in_translation_file,    15, "Namespace (%1(%s)) - ID (%2(%d)): not found in translation file. Format is set to the original one: '%3(%s)'.")
    STATIC_EVENT_DEF(ev_invalid_format,                            16, "Invalid format '%1(%s)' in file '%2(%s)'.")
    STATIC_EVENT_DEF(ev_namespace_begin_end_mismatch,            17, "BEGIN _ EVENT _ DEF _ NAMESPACE and END _ EVENT _ NAMESPACE mismatch in file '%1(%s)'.")
    STATIC_EVENT_DEF(ev_add_event_error,                        18, "Error while adding event from file '%1(%s)'.")
    STATIC_EVENT_DEF(ev_parsing_error,                            19, "Parsing error in file '%1(%s)'.")
END_EVENT_NAMESPACE()

using namespace BasicEventTransResEvents;

const std::string strMacroBeginEventDefNamespace("BEGIN_EVENT_DEF_NAMESPACE");
const std::string strEndMacroEventNamespace("END_EVENT_NAMESPACE");

//***********************************************************************************
Namespace::~Namespace()
{
    TMap_ID_EventObject::const_iterator it;
    for (it = map_ID_EventObject_.begin(); it != map_ID_EventObject_.end(); it++)
    {
        delete it->second;
    }
}

bool Namespace::Add(EventObject * pEventObject)
{
    TMap_ID_EventObject::iterator it = map_ID_EventObject_.find(pEventObject->get_ID());
    if (it != map_ID_EventObject_.end())
    {
        raise_event(_SRC, ev_adding_id_format_failure, LM_ERROR,
                    pEventObject->get_namespace().c_str(),
                    pEventObject->get_ID());
        return false;
    }

    map_ID_EventObject_[pEventObject->get_ID()] = pEventObject;

    return true;
}


bool Namespace::Found(long lID, EventObject ** ppEventObject) const
{
    TMap_ID_EventObject::const_iterator it = map_ID_EventObject_.find(lID);
    bool bFound = it != map_ID_EventObject_.end();

    if (bFound)
    {
        *ppEventObject = it->second;
    }

    return bFound;
}


const TMap_ID_EventObject & Namespace::get_id_eventobject_map() const
{
    return map_ID_EventObject_;
}


//***********************************************************************************
bool LangFormat::Add(const std::string & strLang, const std::string & strFormat)
{
    TMap_Lang_Format::iterator it = map_Lang_Format_.find(strLang);
    if (it != map_Lang_Format_.end())
    {
        raise_event(_SRC, ev_adding_language_format_failure, LM_ERROR,
                    strLang.c_str());
        return false;
    }

    map_Lang_Format_[strLang] = std::string("\"") + strFormat + "\"";

    return true;
}


bool LangFormat::Found(const std::string & strLang, std::string & strFormat) const
{
    TMap_Lang_Format::const_iterator it = map_Lang_Format_.find(strLang);
    bool bFound = it != map_Lang_Format_.end();

    if (!bFound)
    {
        return false;
    }

    strFormat = it->second;

    return true;
}


//***********************************************************************************
bool Namespace4Translation::Add(long lID, 
                                    const std::string & strLang, 
                                    const std::string & strFormat)
{
    TMap_ID_LangFormat::iterator it = map_ID_LangFormat_.find(lID);

    if (it != map_ID_LangFormat_.end())
    {
        LangFormat * pLangFormat = it->second;
        return pLangFormat->Add(strLang, strFormat);
    }
    else
    {
        LangFormat * pLangFormat = new LangFormat;
        if (!pLangFormat->Add(strLang, strFormat))
        {
            return false;
        }
        map_ID_LangFormat_[lID] = pLangFormat;
    }

    return true;
}


bool Namespace4Translation::Found(long lID, const std::string & strLang, std::string & strFormat) const
{
    TMap_ID_LangFormat::const_iterator it = map_ID_LangFormat_.find(lID);
    bool bFound = it != map_ID_LangFormat_.end();
    if (bFound)
    {
        LangFormat * pLangFormat = it->second;
        if (!pLangFormat->Found(strLang, strFormat))
        {
            bFound = false;
        }
    }

    return bFound;
}


const TMap_ID_LangFormat & Namespace4Translation::get_id_langformat_map() const
{
    return map_ID_LangFormat_;
}


//***********************************************************************************
TMap_NSID_NS Events::m_map_NSID_NS;
TMap_NSID_NS4Translation Events::m_map_NSID_NS4Translation;

//***********************************************************************************
Events::~Events()
{
    DeleteEvents();
}


void Events::DeleteEvents()
{
    TMap_NSID_NS::iterator it_NSID_NS;
    for (it_NSID_NS = m_map_NSID_NS.begin(); it_NSID_NS != m_map_NSID_NS.end(); it_NSID_NS++)
    {
        delete it_NSID_NS->second;
    }
    m_map_NSID_NS.clear();

    TMap_NSID_NS4Translation::iterator it_NSID_NS4Translation;
    for (it_NSID_NS4Translation = m_map_NSID_NS4Translation.begin(); 
            it_NSID_NS4Translation != m_map_NSID_NS4Translation.end(); 
            it_NSID_NS4Translation++)
    {
        Namespace4Translation * pNSLang = it_NSID_NS4Translation->second;
        TMap_ID_LangFormat map = pNSLang->get_id_langformat_map();
        TMap_ID_LangFormat::iterator itLangFormat;
        for (itLangFormat = map.begin(); itLangFormat != map.end(); itLangFormat++)
        {
            delete itLangFormat->second;
        }
        map.clear();
    }
    m_map_NSID_NS4Translation.clear();
}


bool Events::AddEventStatic(const std::string & strNamespace, 
                            long lID, 
                            const std::string & strFormat)
{
    TMap_NSID_NS::iterator itNamespace = m_map_NSID_NS.find(strNamespace);
    if (itNamespace != m_map_NSID_NS.end())
    {
        Namespace * ns = itNamespace->second;
        return ns->Add(new EventObject(strNamespace, "", lID, strFormat));
    }
    else
    {
        Namespace * ns = new Namespace;
        if (!ns->Add(new EventObject(strNamespace, "", lID, strFormat)))
        {
            return false;
        }
        m_map_NSID_NS[strNamespace] = ns;
    }

    return true;
}


bool Events::Found(const std::string & strNamespace, long lID, EventObject ** ppEventObject)
{
    TMap_NSID_NS::iterator it = m_map_NSID_NS.find(strNamespace);
    if (it == m_map_NSID_NS.end())
    {
        return false;
    }

    Namespace * ns = it->second;
    if (!ns->Found(lID, ppEventObject))
    {
        return false;
    }

    return true;
}


bool Events::AddEvent(const std::string & strNamespace, 
                        const std::string & strName, 
                        long lID, 
                        const std::string & strFormat)
{
    TMap_NSID_NS::iterator itNamespace = m_map_NSID_NS.find(strNamespace);
    if (itNamespace != m_map_NSID_NS.end())
    {
        Namespace * ns = itNamespace->second;
        ns->set_event_handler(this->get_event_handler());
        return ns->Add(new EventObject(strNamespace, strName, lID, strFormat));
    }
    else
    {
        Namespace * ns = new Namespace;
        ns->set_event_handler(this->get_event_handler());
        if (!ns->Add(new EventObject(strNamespace, strName, lID, strFormat)))
        {
            return false;
        }
        m_map_NSID_NS[strNamespace] = ns;
    }

    return true;
}


bool Events::CreateEventsDLLTable(const std::string & strFilename, 
                                    const std::string & strLanguage,
                                    bool & bAllTranslationFound)
{
    bAllTranslationFound = true;    // suppose all translations will be found

    if (strLanguage.empty())
    {
        raise_event(_SRC, ev_no_language_specified, LM_ERROR);
        return false;
    }

    std::ofstream fs;

    fs.open(strFilename.c_str());

    if (fs.fail())
    {
        raise_event(_SRC, ev_opening_file_failure, LM_ERROR,
                    strFilename.c_str());
        return false;
    }

    std::string strInclude("#include \"BasicEventHandling/BasicEventTransResImpl.h\"\n");
    fs.write(strInclude.c_str(), strInclude.length());

    std::string strLF("\n");
    fs.write(strLF.c_str(), strLF.length());

    std::string strBegin("BEGIN_EVENT_ENTRY()\n");
    fs.write(strBegin.c_str(), strBegin.length());

    TMap_NSID_NS::iterator itNs;
    for (itNs = m_map_NSID_NS.begin(); itNs != m_map_NSID_NS.end(); itNs++)
    {
        std::string strNamespace = itNs->first;
        Namespace * pNamespace = itNs->second;

        TMap_ID_EventObject mapIdFormat = pNamespace->get_id_eventobject_map();
        TMap_ID_EventObject::iterator itIdEventObject;
        for (itIdEventObject = mapIdFormat.begin(); itIdEventObject != mapIdFormat.end(); itIdEventObject++)
        {
            std::string strFormat = itIdEventObject->second->get_format(); // default format if not found in translation table

            TMap_NSID_NS4Translation::iterator it = m_map_NSID_NS4Translation.find(strNamespace);
            if (it != m_map_NSID_NS4Translation.end())
            {
                Namespace4Translation * pNS4Translation = it->second;
                std::string strFormatLang;
                if (pNS4Translation->Found(itIdEventObject->first, strLanguage, strFormatLang))
                {
                    strFormat = strFormatLang;
                }
                else
                {
                    std::string strFormatPercentDoubled = replace_percent_sign(strFormat.c_str());
                    raise_event(_SRC, ev_event_not_found_in_translation_file, LM_WARNING, 
                                strNamespace.c_str(),
                                itIdEventObject->first, 
                                strFormatPercentDoubled.c_str());
                    bAllTranslationFound = false;
                }
            }
            else
            {
                std::string strFormatPercentDoubled = replace_percent_sign(strFormat.c_str());
                raise_event(_SRC, ev_event_not_found_in_translation_file, LM_WARNING, 
                            strNamespace.c_str(),
                            itIdEventObject->first,
                            strFormatPercentDoubled.c_str());
                bAllTranslationFound = false;
            }

            std::stringstream ss;
            ss << "\tEVENT_ENTRY(" 
                << strNamespace
                << ", " 
                << itIdEventObject->first
                << ", " 
                << strFormat.c_str() 
                << ")\n";

            fs.write(ss.str().c_str(), ss.str().length());
        }
    }

    std::string strEnd("END_EVENT_ENTRY()\n");
    fs.write(strEnd.c_str(), strEnd.length());

    return true;
}


bool Events::ReadEventsSourceFile(const std::string & strFilename)
{
    if (strFilename.empty())
    {
        return true;
    }

    std::ifstream fs;
    fs.open(strFilename.c_str());
    if (fs.fail())
    {
        raise_event(_SRC, ev_opening_file_failure, LM_ERROR,
                    strFilename.c_str());
        return false;
    }

    MapNamespacePositions mapNSPos;
    if (!FindNamespaces(fs, mapNSPos))
    {
        raise_event(_SRC, ev_namespace_begin_end_mismatch, LM_ERROR,
                    strFilename.c_str());
        return false;
    }

    std::map<std::streamoff, NamespacePositions *> map_i = mapNSPos.GetMap();
    std::map<std::streamoff, NamespacePositions *>::iterator it;
    for (it = map_i.begin(); it != map_i.end(); it++)
    {
        NamespacePositions * p = it->second;
        std::string strNamespace = p->strNamespace_;

        fs.clear();
        fs.seekg(p->lBegin_);

        bool bEndReached(false);
        while (!bEndReached)
        {
            std::string strEventDef;
            if (!GetEventDefParams(fs, 
                                    p->lEnd_, 
                                    strEventDef,
                                    bEndReached))
            {
                raise_event(_SRC, ev_event_def_bad_syntax, LM_ERROR,
                            strFilename.c_str());
                return false;
            }
            else
            {
                if (!strEventDef.empty())
                {
                    std::string strName;
                    long lID;
                    std::string strFormat;
                    if (!ParseEventDef(strEventDef, strName, lID, strFormat))
                    {
                        raise_event(_SRC, ev_parsing_error, LM_ERROR,
                                    strFilename.c_str());
                        return false;
                    }

                    std::map<long, std::string> mapSpec;
                    if (!BasicEventHandling_ns::BasicEvent::ValidFormat(false, strFormat, mapSpec))
                    {
                        std::string strFormatPercentDoubled = replace_percent_sign(strFormat.c_str());
                        raise_event(_SRC, ev_invalid_format, LM_ERROR,
                                    strFormatPercentDoubled.c_str(),
                                    strFilename.c_str());
                        return false;
                    }

                    if (!AddEvent(strNamespace, strName, lID, strFormat))
                    {
                        raise_event(_SRC, ev_add_event_error, LM_ERROR,
                                    strFilename.c_str());
                        return false;
                    }
                }
            }
        }
    }

    return true;
}


bool Events::LoadTranslationFile(const std::string & strFilename)
{
    // Open config
    ACE_Configuration_Heap config;
    if (config.open() == -1)
    {
        raise_event(_SRC, ev_ace_cfg_heap, LM_ERROR,
                    strFilename.c_str());
        return false;
    }

    // Import config from strConfigFile
    ACE_Ini_ImpExp config_importer(config);
    if (config_importer.import_config(strFilename.c_str()) == -1)
    {
        raise_event(_SRC, ev_ace_cfg_import, LM_ERROR,
                    strFilename.c_str());
        return false;
    }

    int i(0);
    ACE_TString strNamespaceSection;
    while (config.enumerate_sections(config.root_section(), i, strNamespaceSection) == 0)
    {
        ACE_Configuration_Section_Key namespace_section;
        int rc = config.open_section(config.root_section(), strNamespaceSection.c_str(), 0, namespace_section);
        if (rc != 0)
        {
            raise_event(_SRC, ev_ace_open_section, LM_ERROR,
                        strNamespaceSection.c_str(),
                        strFilename.c_str());
            return false;
        }

        int j(0);
        ACE_TString strEventID;
        while (config.enumerate_sections(namespace_section, j, strEventID) == 0)
        {
            ACE_Configuration_Section_Key eventid_section;
            int rc = config.open_section(namespace_section, strEventID.c_str(), 0, eventid_section);
            if (rc != 0)
            {
                raise_event(_SRC, ev_ace_open_section, LM_ERROR,
                            strEventID.c_str(),
                            strFilename.c_str());
                return false;
            }

            ACE_TString strLanguage;
            enum ACE_Configuration::VALUETYPE type_string(ACE_Configuration::STRING);
            int k(0);
            while (config.enumerate_values(eventid_section, k, strLanguage, type_string) == 0)
            {
                ACE_TString strFormat;
                if (config.get_string_value(eventid_section, strLanguage.c_str(), strFormat) == -1)
                {
                    raise_event(_SRC, ev_ace_get_string_value, LM_ERROR,
                                strEventID.c_str(),
                                strLanguage.c_str());
                    return false;
                }

                std::map<long, std::string> mapSpec;
                if (!BasicEventHandling_ns::BasicEvent::ValidFormat(false, strFormat.c_str(), mapSpec))
                {
                    std::string strFormatPercentDoubled = replace_percent_sign(strFormat.c_str());
                    raise_event(_SRC, ev_invalid_format, LM_ERROR,
                                strFormatPercentDoubled.c_str(),
                                strFilename.c_str());
                    return false;
                }

                AddEventLang(strNamespaceSection.c_str(), 
                                atol(strEventID.c_str()), 
                                strLanguage.c_str(), 
                                strFormat.c_str());

                ++k;
            }

            ++j;
        }

        ++i;
    }

    return true;
}


bool Events::FindNamespaces(std::ifstream & fs, 
                                MapNamespacePositions & mapNSPos)
{
    assert(mapNSPos.GetMap().size() == 0);

    // look for strMacroBeginEventDefNamespace(s) and keep their positions
    fs.seekg(0);
    long lBeginNo(0);
    while (!fs.eof())
    {
        std::string strNamespace;
        long lPositionBegin(0);
        bool bMacroFound(false);
        if (GetNamespace(fs, strNamespace, bMacroFound, lPositionBegin))
        {
            if (bMacroFound)
            {
                NamespacePositions * p = mapNSPos.GetNamespacePositions(lBeginNo);
                if (p == NULL)
                {
                    p = new NamespacePositions;
                    mapNSPos.Add(lBeginNo, p);
                }
                p->strNamespace_ = strNamespace;
                p->lBegin_ = lPositionBegin;
                ++lBeginNo;
            }
        }
    }

    // look for strEndMacroEventNamespace(s) and keep their positions
    fs.clear();
    fs.seekg(0);
    long lEndNo(0);
    while (!fs.eof())
    {
        std::streamoff lPositionEnd(0);
        bool bMacroFound(false);
        if (GetEndEventNamespace_EndingPosition(fs, lPositionEnd, bMacroFound))
        {
            if (bMacroFound)
            {
                NamespacePositions * p = mapNSPos.GetNamespacePositions(lEndNo);
                if (p == NULL)
                {
                    p = new NamespacePositions;
                    mapNSPos.Add(lEndNo, p);
                }
                p->lEnd_ = lPositionEnd;
                ++lEndNo;
            }
        }
    }

    std::map<std::streamoff, NamespacePositions *> map_i = mapNSPos.GetMap();
    std::map<std::streamoff, NamespacePositions *>::iterator it;
    for (it = map_i.begin(); it != map_i.end(); it++)
    {
        NamespacePositions * p = it->second;
        if (p->lBegin_ == -1 || p->lEnd_ == -1)
        {
            mapNSPos.Clear();
            return false;
        }
    }

    for (it = map_i.begin(); it != map_i.end(); it++)
    {
        std::streamoff lBeginNo = it->first;
        std::streamoff lEndPos = it->second->lEnd_;
        if ((size_t)lBeginNo < map_i.size() - 1)
        {
            std::streamoff lNextPosBegin = map_i[lBeginNo + 1]->lBegin_;
            if (lNextPosBegin < lEndPos)
            {
                return false;
            }
        }
    }

    return true;
}


bool Events::GetNamespace(std::ifstream & fs, 
                            std::string & strNamespace, 
                            bool & bMacroFound,
                            long & lPositionBegin)
{
    const size_t lSize = strMacroBeginEventDefNamespace.length();

    char * szBuffer = new char[lSize + 1];
    std::auto_ptr<char> spBuffer(szBuffer);
    memset(szBuffer, 0, (lSize + 1) * sizeof(char));
    
    bMacroFound = false;
    bool bParamFound(true);
    std::streamoff lPos = fs.tellg();
    while (fs.read(szBuffer, lSize)) 
    {
        std::string strText(szBuffer);

        if (strText.find(strMacroBeginEventDefNamespace) != strText.npos)
        {
            lPositionBegin = (long)fs.tellg() - (long)strMacroBeginEventDefNamespace.length();
            bMacroFound = true;
        }

        if (bMacroFound)
        {
            if (!get_param_string(fs, strNamespace))
            {
                bParamFound = false;
            }
            break;
        }
        else
        {
            fs.seekg(lPos = lPos + 1);
        }

        memset(szBuffer, 0, (lSize + 1) * sizeof(char));
    }

    if (!bParamFound)
    {
        return false;
    }

    return true;
}


bool Events::GetEndEventNamespace_EndingPosition(std::ifstream & fs, std::streamoff & lEndingPosition, bool & bMacroFound)
{
    const size_t lSize = strEndMacroEventNamespace.length();

    char * szBuffer = new char[lSize + 1];
    std::auto_ptr<char> spBuffer(szBuffer);
    memset(szBuffer, 0, (lSize + 1) * sizeof(char));
    
    bMacroFound = false;
    bool bParamsFound(true);
    std::streamoff lPos = fs.tellg();
    while (fs.read(szBuffer, lSize)) 
    {
        std::string strText(szBuffer);

        if (strText.find(strEndMacroEventNamespace) != strText.npos)
        {
            bMacroFound = true;
        }

        if (bMacroFound)
        {
            break;
        }
        else
        {
            fs.seekg(lPos = lPos + 1);
        }

        memset(szBuffer, 0, (lSize + 1) * sizeof(char));
    }

    if (!bParamsFound)
    {
        return false;
    }

    lEndingPosition = fs.tellg();

    return true;
}


bool Events::GetEventDefParams(std::ifstream & fs,
                                std::streamoff lPosEnd,
                                std::string & strParams,
                                bool & bEndReached)
{
    const std::string strMacro("STATIC_EVENT_DEF");
    const size_t lSize = strMacro.length();

    char * szBuffer = new char[lSize + 1];
    std::auto_ptr<char> spBuffer(szBuffer);
    memset(szBuffer, 0, (lSize + 1) * sizeof(char));
    
    bool bMacroFound(false);
    bool bParamsFound(true);
    std::streamoff lPos = fs.tellg();
    while (fs.read(szBuffer, lSize) && lPos < lPosEnd) 
    {
        std::string strText(szBuffer);

        if (strText.find(strMacro) != strText.npos)
        {
            bMacroFound = true;
        }

        if (bMacroFound)
        {
            if (!get_param_string(fs, strParams))
            {
                bParamsFound = false;
            }
            break;
        }
        else
        {
            fs.seekg(lPos = lPos + 1);
        }

        memset(szBuffer, 0, (lSize + 1) * sizeof(char));
    }

    bEndReached = (lPos >= lPosEnd) || fs.eof();

    if (!bParamsFound)
    {
        return false;
    }

    return true;
}


bool Events::get_param_string(std::ifstream & fs, std::string & strParamsString)
{
    char c;
    bool bBegan(false);
    bool bEnd(false);
    long lOpenClose(0);
    while (fs.get(c))
    {
        if (c == '(')
        {
            ++lOpenClose;
            if (!bBegan)
            {
                bBegan = true;
                continue;
            }
        }

        if (c == ')')
        {
            if (--lOpenClose == 0)
            {
                bEnd = true;
                break;
            }
        }

        if (bBegan)
        {
            strParamsString += c;
        }
    }

    if (!bBegan || !bEnd)
    {
        return false;
    }
    
    return true;
}


std::string Events::replace_percent_sign(const std::string & strValue)
{
    std::string str;

    const char * psz = strValue.c_str();
    while(*psz) 
    {
        if(*psz == '%') 
        {
            str += "%%";
        }
        else
        {
            str += *psz;
        }

        psz++;
    }

    return str;
}


bool Events::AddEventLang(const std::string & strNamespace, long lID, 
                                const std::string & strLang, const std::string & strFormat)
{
    TMap_NSID_NS4Translation::iterator itNamespace = m_map_NSID_NS4Translation.find(strNamespace);
    if (itNamespace != m_map_NSID_NS4Translation.end())
    {
        Namespace4Translation * ns = itNamespace->second;
        return ns->Add(lID, strLang, strFormat);
    }
    else
    {
        Namespace4Translation * ns = new Namespace4Translation;
        if (!ns->Add(lID, strLang, strFormat))
        {
            return false;
        }
        m_map_NSID_NS4Translation[strNamespace] = ns;
    }

    return true;
}


bool Events::ParseEventDef(const std::string & strEventDef, 
                            std::string & strName, 
                            long & lID, 
                            std::string & strFormat)
{
    // first comma
    size_t nPosBegin(0);
    size_t nPos1 = strEventDef.find(',', nPosBegin);
    if (nPos1 == strEventDef.npos)
    {
        raise_event(_SRC, ev_no_comma, LM_ERROR,
                    strEventDef.c_str());
        return false;
    }

    // second comma
    size_t nPos2 = strEventDef.find(',', nPos1 + 1);
    if (nPos2 == strEventDef.npos)
    {
        raise_event(_SRC, ev_no_second_comma, LM_ERROR,
                    strEventDef.c_str());
        return false;
    }

    // Name
    strName = strEventDef.substr(0, nPos1);

    // Trim strName
    size_t nPosTrim1 = strName.find_first_not_of(' ');
    size_t nPosTrim2 = strName.find_last_not_of(' ');
    strName = strName.substr(nPosTrim1, nPosTrim2 - nPosTrim1 + 1);

    // ID
    std::string strID = strEventDef.substr(nPos1 + 1, nPos2 - nPos1 - 1);
    if (strID.empty())
    {
        raise_event(_SRC, ev_parsing_failure_for_id, LM_ERROR,
                    strEventDef.c_str());
        return false;
    }
    lID = atol(strID.c_str());

    // Format
    strFormat = strEventDef.substr(nPos2 + 1);

    // Trim Format
    nPosTrim1 = strFormat.find_first_of('"');
    nPosTrim2 = strFormat.find_last_of('"');
    strFormat = strFormat.substr(nPosTrim1, nPosTrim2 - nPosTrim1 + 1);

    return true;
}

