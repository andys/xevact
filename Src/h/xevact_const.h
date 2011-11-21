                                     /*
                                   xevact
                       Associates actions with Events
                            Jon A. Maxwell (JAM)
                                     */


#define VERSION	"1.0d"

#define DEFAULT_ACTIONS_FILE	"/usr/local/lib/xevact.actions"
#define MAX_EVENT_TYPE		(LASTEvent)
#define MAX_LINE_LEN	256	/* this could be too small (for LoadActions) */
#define MAX_TESTS	10	/* takes up maybe 8 bytes*max_test for each action */
#define KEY_BUFF_SIZE	256	/* 256 is overkill */

#define WHITE_SPACE " ,\t\n\r"	/* comma in whitespace makes the delay feild look good */

#define SET_REF	1	/* TRUE: action->reference_count++; in ActionCall() */

/* debug modes */
#define D_CALLS	(1<<0)	/* each call writes it's name and some info immediately */
#define D_MISC	(1<<1)	/* ? */
#define D_LIST	(1<<2)	/* will print out the action-list for each type at startup */
#define D_EVENT	(1<<3)	/* event related (?) info */
#define D_WASTE	(1<<4)	/* print out wasted events, their type, etc */
#define D_CHECKP (1<<5)	/* checkpoint kinda things.  values of temporary stuff, etc */
#define D_LOAD	(1<<6)	/* LoadActions debugging stuff */

/* call types */
#define CALL_ANY	(-1)
#define CALL_ACTION	(1<<0)
#define CALL_TEST	(1<<1)

/* junk */
#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif
