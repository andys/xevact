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


int PlaySound(Action *action, XEvent *xev) {
	DEBUG(D_CALLS) printf("PlaySound: %s\n",action->string);
#	ifdef AUDIOFILE
		/* PlaySoundFile(filename, have_we_forked?);*/
		PlaySoundFile(action->string, 0!=(action->flags & CharToFlag('F')) );
#	endif
	}

