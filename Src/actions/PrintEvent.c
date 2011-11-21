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


int PrintEvent(Action *action, XEvent *xev) {
	DEBUG(D_CALLS) printf("PrintEvent: %d %d\n",action, xev);
	
	/* AnyEvent */
	printf("Event Type: %s\t",TypeToStr(xev->type));
	printf("Serial: %u\t",	xev->xany.serial);
	printf("Send: %s\t",   (xev->xany.send_event)?"YES":"NO");
	printf("Window: %d\t",	xev->xany.window);
	
	if (xev->type&(KeyPress|KeyRelease)) {
		printf("KeyCode: %d\tState: %d\t",xev->xkey.keycode,xev->xkey.state);
		printf("Key: %s\t",TranslateKeyCode(xev));
		}
	
	printf("\n");
	return;
	}


