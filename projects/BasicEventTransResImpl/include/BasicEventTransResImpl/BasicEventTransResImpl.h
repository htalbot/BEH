#ifndef __BASICEVENTTRANSRES_IMPL_H__
#define __BASICEVENTTRANSRES_IMPL_H__

#include "BasicEventTransResImpl/BASICEVENTTRANSRESIMPL_Export.h"
#include "BasicEventHandling/BasicEventHandling.h"

#include <map>
#include <string>
#include <fstream>
#include <assert.h>

//***********************************************************************************
void AddEvents(); // Defined with BEGIN_EVENT_ENTRY() in another module.

//***********************************************************************************
#define BEGIN_EVENT_ENTRY()\
    void AddEvents()\
    {

//***********************************************************************************
#define EVENT_ENTRY(ns, id, format)\
    Events::AddEventStatic(#ns, id, format);

//***********************************************************************************
#define END_EVENT_ENTRY()\
    }

//***********************************************************************************
class BASICEVENTTRANSRESIMPL_Export EventObject
{
public:
    EventObject(const std::string & strNamespace,
                const std::string & strName,
                long lID,
                const std::string strFormat)
    :    strNamespace_(strNamespace),
        strName_(strName),
        lID_(lID),
        strFormat_(strFormat)
    {
    }

    std::string get_namespace() { return strNamespace_; }
    std::string get_name() { return strName_; }
    long get_ID() { return lID_; }
    std::string get_format() { return strFormat_; }

protected:
    std::string strNamespace_;
    std::string strName_;
    long lID_;
    std::string strFormat_;
};

//***********************************************************************************
typedef std::map<long, EventObject *> TMap_ID_EventObject;

//***********************************************************************************
class BASICEVENTTRANSRESIMPL_Export Namespace: public BasicEventHandling_ns::BasicEventRaiser
{
public:
    ~Namespace();
    bool Add(EventObject *);
    bool Found(long lID, EventObject **) const;

    const TMap_ID_EventObject & get_id_eventobject_map() const;

protected:

    TMap_ID_EventObject map_ID_EventObject_;
};

//***********************************************************************************
typedef std::map<std::string, std::string> TMap_Lang_Format;

//***********************************************************************************
class BASICEVENTTRANSRESIMPL_Export LangFormat: public BasicEventHandling_ns::BasicEventRaiser
{
public:
    bool Add(const std::string & strLang, const std::string & strFormat);
    bool Found(const std::string & strLang, std::string & strFormat) const;

protected:

    TMap_Lang_Format map_Lang_Format_;
};

//***********************************************************************************
typedef std::map<long, LangFormat *> TMap_ID_LangFormat;

//***********************************************************************************
class BASICEVENTTRANSRESIMPL_Export Namespace4Translation
{
public:
    bool Add(long lID, const std::string & strLang, const std::string & strFormat);
    bool Found(long lID, const std::string & strLang, std::string & strFormat) const;

    const TMap_ID_LangFormat & get_id_langformat_map() const;

protected:

    TMap_ID_LangFormat map_ID_LangFormat_;
};

//***********************************************************************************
class BASICEVENTTRANSRESIMPL_Export NamespacePositions
{
public:
    NamespacePositions(): lBegin_(-1), lEnd_(-1) {}
    std::string strNamespace_;
    std::streamoff lBegin_;
    std::streamoff lEnd_;
};

//***********************************************************************************
class BASICEVENTTRANSRESIMPL_Export MapNamespacePositions
{
public:
    ~MapNamespacePositions()
    {
        Clear();
    }

    void Add(long lNamespaceNo, NamespacePositions * p)
    {
        #ifdef _DEBUG
        std::map<std::streamoff, NamespacePositions *>::iterator it = mapNamespacePositions_.find(lNamespaceNo);
        assert(it == mapNamespacePositions_.end());
        #endif
        mapNamespacePositions_[lNamespaceNo] = p;
    }

    NamespacePositions * GetNamespacePositions(long lNamespaceNo)
    {
        if (mapNamespacePositions_.find(lNamespaceNo) != mapNamespacePositions_.end())
        {
            return mapNamespacePositions_[lNamespaceNo];
        }
        else
        {
            return NULL;
        }
    }

    void Clear()
    {
        std::map<std::streamoff, NamespacePositions *>::iterator it;
        for (it = mapNamespacePositions_.begin(); it != mapNamespacePositions_.end(); it++)
        {
            delete it->second;
        }
        mapNamespacePositions_.clear();
    }

    const std::map<std::streamoff, NamespacePositions *> & GetMap() const
    {
        return mapNamespacePositions_;
    }

protected:
    std::map<std::streamoff, NamespacePositions *> mapNamespacePositions_;
};

//***********************************************************************************
typedef std::map<std::string, Namespace *> TMap_NSID_NS;

//***********************************************************************************
typedef std::map<std::string, Namespace4Translation *> TMap_NSID_NS4Translation;

//***********************************************************************************
class BASICEVENTTRANSRESIMPL_Export Events: public BasicEventHandling_ns::BasicEventRaiser
{
public:
    ~Events();

    // Static methods are used by DLL
    static void DeleteEvents();
    static bool AddEventStatic(const std::string & strNamespace,
                                long lID,
                                const std::string & strFormat);
    static bool Found(const std::string & strNamespace,
                        long lID,
                        EventObject ** ppEventObject);

    // Non-static methods are not used by DLL but by apps linking statically
    bool AddEvent(const std::string & strNamespace,
                    const std::string & strName,
                    long lID,
                    const std::string & strFormat);
    bool CreateEventsDLLTable(const std::string & strFilename,
                                const std::string & strLanguage,
                                bool & bAllTranslationFound);
    bool ReadEventsSourceFile(const std::string & strFilename);
    bool LoadTranslationFile(const std::string & strFilename);

public:

    static TMap_NSID_NS m_map_NSID_NS;
    static TMap_NSID_NS4Translation m_map_NSID_NS4Translation;

protected:
    static bool FindNamespaces(std::ifstream & fs,
                                MapNamespacePositions & mapNSPos);
    static bool GetNamespace(std::ifstream & fs,
                                std::string & strNamespace,
                                bool & bMacroFound,
                                long & lPositionBegin);
    static bool GetEndEventNamespace_EndingPosition(std::ifstream & fs,
                                                    std::streamoff & lPositionEnd,
                                                    bool & bMacroFound);
    static bool GetEventDefParams(std::ifstream & fs,
                                    std::streamoff lPosEnd,
                                    std::string & strParams,
                                    bool & bEndReached);
    static bool get_param_string(std::ifstream & fs, std::string & strParamsString);
    static std::string replace_percent_sign(const std::string & strValue);
    static bool AddEventLang(const std::string & strNamespace,
                                long lID,
                                const std::string & strLang,
                                const std::string & strFormat);

    bool ParseEventDef(const std::string & strEventDef,
                        std::string & strName,
                        long & lID,
                        std::string & strFormat);
};


#endif // __BASICEVENTTRANSRES_IMPL_H__
