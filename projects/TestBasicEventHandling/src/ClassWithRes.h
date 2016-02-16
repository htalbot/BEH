#ifndef __CLASSWITHRES_H__
#define __CLASSWITHRES_H__

#include "BasicEventHandling/BasicEventHandling.h"

class ClassWithRes: public BasicEventHandling_ns::BasicEventRaiser
{
public:
	void run(long lEventID, ...);
};

#endif // __CLASSWITHRES_H__
