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


int PrintKey(Action *action, XEvent *xev) {
	char	*string;
	DEBUG(D_CALLS) printf("PrintKey: %d %d\n",action, xev);

	string=(char *)TranslateKeyCode(xev);
	if (string == NULL) return;
	if (*string == '\r') printf("\n");
	else if (strlen(string) == 1) printf("%s", string);
	else {
		/* PrintKey should not print things like <<Shift>>
		     --let PrintEvent do that, since PrintKey is for
		     printing out char-by-char type of output */
		/* printf("<<%s>>", string); */
		DEBUG(D_MISC) printf("String keycode.\n");
		}
	fflush(stdout);
	}


