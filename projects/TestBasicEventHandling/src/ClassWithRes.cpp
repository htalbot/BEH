
#include "ClassWithRes.h"
#include "ClassWithRes.evc.h"

using namespace ClassWithResEvents;

void ClassWithRes::run(long lEventID, ...)
{
	va_list args;
	va_start(args, lEventID);

	switch(lEventID)
	{
		case 1:
			vraise_event(_SRC, ev1, LM_DEBUG, args);
			break;
		case 2:
			vraise_event(_SRC, ev2, LM_DEBUG, args);
			break;
		case 3:
			vraise_event(_SRC, ev3, LM_DEBUG, args);
			break;
	}

	va_end(args);
}

