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


Bool Test_State(Action *action, XEvent *xev, int tnum) {
	if (xev->xkey.state==action->cond[tnum]) {
		DEBUG(D_CALLS) printf("Test_State: TRUE\n");
		return TRUE;
		}
	else {
		DEBUG(D_CALLS) printf("Test_State: FALSE\n");
		return FALSE;
		}
	}


