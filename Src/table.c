                                     /*
                                   xevact
                       Associates actions with Events
                            Jon A. Maxwell (JAM)
                                     */



#include <X11/Xlib.h>
#include <X11/X.h>

#include <sys/time.h>

#include <xevact_types.h>

#include <xevact_macro.h>
#include <xevact_const.h>
#include <xevact_list.h>
#include <xevact_struct.h>
#include <xevact_declare.h>


struct StrToCallTable_Elem StrToCallTable[]={
		{"None",	CALL_ANY,	NULL},
		{"NoAction",	CALL_ANY,	NULL},
		{"-",		CALL_ANY,	NULL},
#include <xevact_actions_table.h>
#include <xevact_tests_table.h>
		{NULL,		0,		NULL}	/* end of table */
		};

struct StrToConditionTable_Elem StrToConditionTable[] = {
	{"Shift",	ShiftMask},
	{"Caps",	LockMask},
	{"Control",	ControlMask},
	{"Compose",	Mod1Mask},
	{"Meta",	Mod1Mask},	/* Is meta the same thing as compose? */
	{"Alt",		Mod2Mask},
	{"Mod1",	Mod1Mask},
	{"Mod2",	Mod2Mask},
	{"Mod3",	Mod3Mask},
	{"Mod4",	Mod4Mask},
	{"Mod5",	Mod5Mask},
	{"Button1",	Button1Mask},
	{"Button2",	Button2Mask},
	{"Button3",	Button3Mask},
	{"Button4",	Button4Mask},
	{"Button5",	Button5Mask},
	/* any other keywords that StrToCondition should know about */
	{NULL,		NULL}	/* end */
	};


char *TypeToStrTable[MAX_EVENT_TYPE];

#define S(type, str) TypeToStrTable[type]=str;
NoRet InitTypeToStrTable() {
	int i;
	for (i=0; i<MAX_EVENT_TYPE; i++) {
		/* Fill in unknown as default string */
		S(i, "Unknown Type");
		}
	/* these can be taken out at random, in case you don't have
	   some of these symbols defined */
	S(KeyPress,		"KeyPress");
	S(KeyRelease,		"KeyRelease");
	S(ButtonPress,		"ButtonPress");
	S(ButtonRelease,	"ButtonRelease");
	S(MotionNotify,		"MotionNotify");
	S(EnterNotify,		"EnterNotify");
	S(LeaveNotify,		"LeaveNotify");
	S(FocusIn,		"FocusIn");
	S(FocusOut,		"FocusOut");
	S(KeymapNotify,		"KeymapNotify");
	S(Expose,		"Expose");
	S(GraphicsExpose,	"GraphicsExpose");
	S(NoExpose,		"NoExpose");
	S(VisibilityNotify,	"VisibilityNotify");
	S(CreateNotify,		"CreateNotify");
	S(DestroyNotify,	"DestroyNotify");
	S(UnmapNotify,		"UnmapNotify");
	S(MapNotify,		"MapNotify");
	S(MapRequest,		"MapRequest");
	S(ReparentNotify,	"ReparentNotify");
	S(ConfigureNotify,	"ConfigureNotify");
	S(ConfigureRequest,	"ConfigureRequest");
	S(GravityNotify,	"GravityNotify");
	S(ResizeRequest,	"ResizeRequest");
	S(CirculateNotify,	"CirculateNotify");
	S(CirculateRequest,	"CirculateRequest");
	S(PropertyNotify,	"PropertyNotify");
	S(SelectionClear,	"SelectionClear");
	S(SelectionRequest,	"SelectionRequest");
	S(SelectionNotify,	"SelectionNotify");
	S(ColormapNotify,	"ColormapNotify");
	S(ClientMessage,	"ClientMessage");
	S(MappingNotify,	"MappingNotify");
	}
#undef S

SetTable set[MAX_EVENT_TYPE];

/* We set these values run-time because otherwise if they change X
   any then we would have to re-make the array by hand (so we might
   not have one source run on X11R3, R4, R5, R6, etc)
                                     */
#define S SetAction
NoRet SetAction(int type, Mask mask, int is_set, Condition cond, int (*match)(), int (*call)(), char *string ) {
	set[type].is_set	=is_set;
	set[type].mask		=mask;
	set[type].default_action.cond[0]	=cond;
	set[type].default_action.string		=string;
	set[type].default_action.match[0]	=match;
	set[type].default_action.call		=call;
	set[type].default_action.ref_count	=0;
	}
NoRet InitActions() {
	Zero(set, sizeof(set));
	/* All working actions should also include the mask needed to
	   get the events arriving, so that the masks don't need to
	   be specified in the actions-file. (That's really why we
	   have this here at all, besides for the 1 or 2 default-on
	   ones)

	   Anything marked with NW is Not Working. 
                                     */
	/*type			acti-mask,	is on?	condition	search_funct	function 	func-args (string)*/
	S(KeyPress,		KeyPressMask,	1,	0,		Test_True,	PrintKey,	NULL);
	S(KeyRelease,		KeyReleaseMask,	0,	0,		Test_True,	PrintEvent,	NULL);
	S(ButtonPress,		ButtonPressMask,0,	0,		Test_True,	PrintEvent,	NULL);
	S(ButtonRelease,	ButtonReleaseMask,0,	0,		Test_True,	PrintEvent,	NULL);
	S(MotionNotify,		ButtonMotionMask|PointerMotionMask,
						0,	0,		Test_True,	PrintKey,	NULL);
	S(EnterNotify,		EnterWindowMask,0,	0,		Test_True,	PrintEvent,	NULL);
	S(LeaveNotify,		LeaveWindowMask,0,	0,		Test_True,	PrintEvent,	NULL);
	S(FocusIn,		FocusChangeMask,0,	0,		Test_True,	PrintEvent,	NULL);
	S(FocusOut,		FocusChangeMask,0,	0,		Test_True,	PrintEvent,	NULL);
	S(KeymapNotify,		KeymapStateMask,0,	0,		Test_True,	PrintEvent,	NULL);
	S(Expose,		ExposureMask,	0,	0,		Test_True,	PrintEvent,	NULL);
/*NW*/	S(GraphicsExpose,	0L,		0,	0,		Test_True,	PrintEvent,	NULL);
/*NW*/	S(NoExpose, 		0L,		0,	0,		Test_True,	PrintEvent,	NULL);
	S(VisibilityNotify,	VisibilityChangeMask,
						0,	0,		Test_True,	PrintEvent,	NULL);
	S(CreateNotify,		SubstructureNotifyMask,
						0,	0,		Test_True,	PrintEvent,	NULL);
	S(DestroyNotify,	SubstructureNotifyMask|StructureNotifyMask,
						0,	0,		Test_True,	PrintEvent,	NULL);
	S(UnmapNotify,		SubstructureNotifyMask|StructureNotifyMask,
						0,	0,		Test_True,	PrintEvent,	NULL);
	S(MapNotify,		SubstructureNotifyMask|StructureNotifyMask,
						0,	0,		Test_True,	PrintEvent,	NULL);
/*NW*/	S(MapRequest,		0L,		0,	0,		Test_True,	PrintEvent,	NULL);
	S(ReparentNotify,	SubstructureNotifyMask|StructureNotifyMask,
						0,	0,		Test_True,	PrintEvent,	NULL);
	S(ConfigureNotify,	SubstructureNotifyMask|StructureNotifyMask,
						0,	0,		Test_True,	PrintEvent,	NULL);
/*NW*/	S(ConfigureRequest,	0L,		0,	0,		Test_True,	PrintEvent,	NULL);
	S(GravityNotify,	SubstructureNotifyMask|StructureNotifyMask,
						0,	0,		Test_True,	PrintEvent,	NULL);
/*NW*/	S(ResizeRequest,	0L,		0,	0,		Test_True,	PrintEvent,	NULL);
	S(CirculateNotify,	SubstructureNotifyMask|StructureNotifyMask,
						0,	0,		Test_True,	PrintEvent,	NULL);
/*NW*/	S(CirculateRequest,	0L,		0,	0,		Test_True,	PrintEvent,	NULL);
	S(PropertyNotify,	PropertyChangeMask,0,	0,		Test_True,	PrintEvent,	NULL);
	S(SelectionClear,	0l,		0,	0,		Test_True,	PrintEvent,	NULL);
	S(SelectionRequest,	0l,		0,	0,		Test_True,	PrintEvent,	NULL);
	S(SelectionNotify,	0l,		0,	0,		Test_True,	PrintEvent,	NULL);
	S(ColormapNotify,	ColormapChangeMask,0,	0,		Test_True,	PrintEvent,	NULL);
	S(ClientMessage,	0l,		0,	0,		Test_True,	PrintEvent,	NULL);
	S(MappingNotify,	0l,		0,	0,		Test_True,	PrintEvent,	NULL);
	}
#undef S

/* -------------------- List Table Data -------------------- */
NoRet ListEvents() {
	int	i;
	
	printf("\nEvent Types:\n");
	for (i=0; i<MAX_EVENT_TYPE; i++) {
		printf("\t%s\n",TypeToStr(i));
	        }
	}

NoRet ListTests() {
	int	i;
	printf("\nTests:\n");
	for (i=0; StrToCallTable[i].name!=NULL; i++) {
		if (StrToCallTable[i].type==CALL_TEST) printf("\t%s\n",StrToCallTable[i].name);
		}
	}

NoRet ListActions() {
	int	i;
	printf("\nActions:\n");
	for (i=0; StrToCallTable[i].name!=NULL; i++) {
		if (StrToCallTable[i].type==CALL_ACTION) printf("\t%s\n",StrToCallTable[i].name);
		}
	}

NoRet ListConditions() {
	int	i;
	
	printf("\nConditions:\n");
	for (i=0; StrToConditionTable[i].name!=NULL; i++) {
		printf("\t%s\n",StrToConditionTable[i].name);
		}
	}
