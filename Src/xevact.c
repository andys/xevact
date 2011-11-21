                                     /*
                                   xevact
                       Associates actions with Events
                            Jon A. Maxwell (JAM)
                                     */



#include <X11/Xlib.h>
#include <X11/X.h>

#include <string.h>
#include <sys/time.h>
#include <stdio.h>

#include <xevact_types.h>
#include <xevact_macro.h>
#include <xevact_const.h>
#include <xevact_list.h>
#include <xevact_struct.h>
#include <xevact_declare.h>

Display *disp;
char *PROGNAME;


/* This recursive function originally written by Dominic Giampaolo
   (nick@cs.maxine.wpi.edu), and modified by JAM (jmaxwell@acm.vt.edu).
   
   (Before it would call XSelectInput twice on some windows)
                                     */

void select_all_windows(Display *disp, Window root, unsigned long type) {
	Window	parent, *children;
	int	stat, i, j, k;
	unsigned int	nchildren;

	XSelectInput(disp, root, type);

	stat=XQueryTree(disp, root, &root, &parent, &children, &nchildren);
	if (stat == FALSE) {
		fprintf(stderr, "Can't query window tree.\n");
		return;
		}
	if (nchildren == 0) return;

	/* select for all children windows too */
	for(i=0; i < nchildren; i++) {
		select_all_windows(disp, children[i], type);
		}     

	XFree((char *)children);
	}

int usage() {
	printf("%s%s%s\n%s\n%s\n%s\n%s\n%s\n%s%s%s\n",
		"USAGE: ",PROGNAME," [options...]",
		"	-actions <file>         Use <file> for defining actions.",
		"	-display <disp>		Use X display <disp> (include the :0 )",
		"	-debug <level>		Set debug level;  Each level is a power of 2",
		"	-nograb			No using XGrabServer(), but /could/ miss windows",
		"	-list			Print out known tests, actions, and event types",
		"	Version ",VERSION, ", by JAM (jmaxwell@acm.vt.edu)");
	exit(0);
	}


int main(int argc, char *argv[]) {
	char	*hostname,*actionsfile;
	char	*string;
	XEvent	xev;
	unsigned long	type;
	int	count=0, grab=TRUE, wasted_events=0;
	int	i;
	
	PROGNAME=argv[0];
	hostname = ":0";
	actionsfile = DEFAULT_ACTIONS_FILE;

	/* args should be in seperate call, but oh well */
	for (i=1; i<argc; i++) {
		     if (!strcmp(argv[i],"-actions")) {
			actionsfile=argv[++i];
			}
		else if (!strcmp(argv[i],"-display")) {
			hostname=argv[++i];
			}
		else if (!strcmp(argv[i],"-debug")) {
			debug=atoi(argv[++i]);
			}
		else if (!strcmp(argv[i],"-nograb")) {
			grab=FALSE;
			}
		else if (!strcmp(argv[i],"-list")) {
			ListEvents();
			ListTests();
			ListActions();
			ListConditions();
			exit(0);
			}
		else {usage();}
		}

	/* setup X connection */  /* must setup X before loading actions
	                             since it uses Xlib calls */
	disp=XOpenDisplay(hostname);
	if (disp==NULL) {
		fprintf(stderr, "Cannot open X display: %s\n", hostname);
		exit(1);
		}
	NewXErrors();
 
	/* setup action tables */
	InitActions();
	if (!LoadActions(actionsfile)) usage();
	type=ActionMask_Tot()|SubstructureNotifyMask;

	DEBUG(D_LIST) {		/* print action list for events */
		int	i;  ListElem elem;
		for (i=0; i<MAX_EVENT_TYPE; i++) {
			if (!set[i].is_set) {
				printf("Event type %s is not set.\n",TypeToStr(i));
				}
			else {
				printf("Event type %s:\n",TypeToStr(i));
				printf("	");
				for (elem=ListFirstElem(&set[i].tlist); elem!=NULL; elem=ListNextElem(elem)) {
					printf("%x(%d) -> ",ListElemData(elem),((Action *)ListElemData(elem))->cond[0]);
					}
				printf("NULL\n");
				}
			}
		}

	/* grabbing server makes it so we know we got /all/ windows!
	   but it stops everything while we mark all the windows.    */
	if (grab) XGrabServer(disp);
	select_all_windows(disp, DefaultRootWindow(disp), type);
	if (grab) XUngrabServer(disp);

	/* main app loop */
	while (1) {
		XNextEvent(disp, &xev);
		DEBUG(D_EVENT) printf("DEBUG: Received Event Type: %s\n",TypeToStr(xev.type));
		if (IsSet(xev.type)) {
			ActionCall(&xev, SET_REF);
			}
		if (xev.type==CreateNotify) {
			XCreateWindowEvent *xcwe;
			xcwe=(XCreateWindowEvent *)&xev;
			select_all_windows(disp, xcwe->window, type);
			}
		else if (!IsSet(xev.type)) {
			wasted_events++;
			DEBUG(D_WASTE) printf("Wasted Event of Type: %s (%d)\n",TypeToStr(xev.type),xev.type);
			DEBUG(D_WASTE) printf("Wasted Events: %d\n",wasted_events);
			}
		}
	}


