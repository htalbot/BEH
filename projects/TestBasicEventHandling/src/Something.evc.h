
#ifndef __SOMETHING_EVC_H__
#define __SOMETHING_EVC_H__

#include "BasicEventHandling/BasicEventHandling.h"

BEGIN_EVENT_DECL_NAMESPACE(SomethingEvents)
	STATIC_EVENT_DECL(ev_doing_Something)
	STATIC_EVENT_DECL(ev_msg_with_double)
	STATIC_EVENT_DECL(ev_msg_with_sized_string)
END_EVENT_NAMESPACE()


#endif // __SOMETHING_EVC_H__
