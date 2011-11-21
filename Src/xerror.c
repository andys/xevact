                                     /*
                            XError, Version 1.0a
                  X Error Handler for less verbose output.
                            Jon A. Maxwell (JAM)
                                     */



#include <X11/Xlib.h>
#include <X11/X.h>

#include <string.h>
#include <sys/time.h>
#include <stdio.h>

#define TRUE	1
#define FALSE	0

/* header stuff */
int NewXErrorHandler(Display *disp, XErrorEvent *xeev);
int NewXIOErrorHandler(Display *disp);

/* register new error handler functions */
int NewXErrors() {
	XSetErrorHandler  (NewXErrorHandler);
	XSetIOErrorHandler(NewXIOErrorHandler);
	}


                             /* Normal Errors */
int NewXErrorHandler(Display *disp, XErrorEvent *xeev) {
	NewPrintXError(disp, xeev);
	fflush(stdout);
	}
                             /* Fatal Errors */
int NewXIOErrorHandler(Display *disp) {
	fprintf(stderr,"Xerror: Fatal error on display %s.\n",DisplayString(disp));
	fflush(stdout);
	}


                          /* Print error message */
#define ERRTEXT_LENGTH	64
#define ONLY_FIRST_WORD	TRUE

int NewPrintXError(Display *disp, XErrorEvent *xeev) {
	char buf[ERRTEXT_LENGTH+1];
	
	XGetErrorText(disp, xeev->error_code, buf, ERRTEXT_LENGTH);
	if (ONLY_FIRST_WORD) {
		register char *cptr;
		cptr=strchr(buf, ' ');
		if (NULL!=cptr) *cptr=0;
		}
	fprintf(stderr,
		"Xerror: %s(%d) Serial: %3d OpCode: %d.%d Display: \"%s\" RID: %d\n",
			buf,xeev->error_code,xeev->serial,xeev->request_code,
			xeev->minor_code,DisplayString(disp),xeev->resourceid);
	}
