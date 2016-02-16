#ifndef __BASICEVENTTRANSRES_H__
#define __BASICEVENTTRANSRES_H__

#include "BasicEventTransRes/BASICEVENTTRANSRES_Export.h"

extern "C" BASICEVENTTRANSRES_Export const char * get_msg(const char * szNamespace, long lID, ...);
extern "C" BASICEVENTTRANSRES_Export const char * vget_msg(const char * szNamespace, long lID, va_list);
extern "C" BASICEVENTTRANSRES_Export const char * get_msg_format(const char * szNamespace, long lID);

#endif // __BASICEVENTTRANSRES_H__
