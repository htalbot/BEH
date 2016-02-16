
#include "Class2WithRes.h"
#include "Class2WithRes.evc.h"

using namespace Class2WithResEvents;

void Class2WithRes::run(long lEventID, ...)
{
	va_list args;
	va_start(args, lEventID);

	switch(lEventID)
	{
		case 1:
//			vraise_event(_SRC, ClassWithResEvents::ev1, LM_DEBUG, args); // to show parent event usage
			vraise_event(_SRC, ev1, LM_DEBUG, args);
			break;
		case 2:
			vraise_event(_SRC, ev2, LM_DEBUG, args);
			break;
		case 3:
			vraise_event(_SRC, ev3, LM_DEBUG, args);
			break;
		case 4:
			vraise_event(_SRC, ev4, LM_DEBUG, args);
			break;
	}

	va_end(args);
}

