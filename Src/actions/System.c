                                     /*
                                   xevact
                       Associates actions with Events
                            Jon A. Maxwell (JAM)
                                     */



#include <X11/Xlib.h>
#include <X11/X.h>

#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include <xevact_types.h>

#include <xevact_macro.h>
#include <xevact_const.h>
#include <xevact_list.h>
#include <xevact_struct.h>
#include <xevact_declare.h>

int System(Action *action, XEvent *xev) {
	system(action->string);
	fflush(stdout);
	}

