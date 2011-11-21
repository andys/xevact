                                     /*
                                   xevact
                       Associates actions with Events
                            Jon A. Maxwell (JAM)
                                     */


#include <xevact_actions_declare.h>
#include <xevact_tests_declare.h>

extern int debug;
extern Display *disp;
extern SetTable set[];
extern char *TypeToStrTable[MAX_EVENT_TYPE];
extern struct StrToCallTable_Elem StrToCallTable[];
extern struct StrToConditionTable_Elem StrToConditionTable[];

/* forward declarations */
/*c/action.c:23:	*/	Mask	ActionMask_Tot() ;
/*c/action.c:34:	*/	Bool	TimeOk(Action *action, struct timeval *cur) ;
/*c/action.c:45:	*/	int	CallIt(Action *action, XEvent *xev) ;
/*c/action.c:61:	*/	Action	*ActionCall(XEvent *xev, int mode) ;
/*c/action.c:113:	*/	Bool	LoadActions(char *file) ;
/*c/action.c:207:	*/	Condition	StrToCondition(char *str) ;
/*c/action.c:236:	*/	Flag	StrToFlags(char *str) ;
/*c/list.c:34:	*/	NoRet	ListStats() ;
/*c/list.c:44:	*/	NoRet	InitList(List *list) ;
/*c/list.c:52:	*/	NoRet	FreeElem(ListElem elem) ;
/*c/list.c:57:	*/	ListElem	AllocElem() ;
/*c/list.c:67:	*/	NoRet	ListUnlinkElem(List *list, ListElem elem) ;
/*c/list.c:76:	*/	Data	ListDeleteElem(List *list, ListElem elem) ;
/*c/list.c:90:	*/	NoRet	ListClear(List *list, int (*call)()) ;
/*c/list.c:109:	*/	ListElem	ListAddAfter(List *list, ListElem after, Data data) ;
/*c/list.c:136:	*/	ListElem	ListFindData(List *list, Data data) ;
/*c/list.c:146:	*/	ListElem	ListNextElem(ListElem elem) ;
/*c/list.c:150:	*/	ListElem	ListPrevElem(ListElem elem) ;
/*c/list.c:154:	*/	ListElem	ListFirstElem(List *list)   ;
/*c/list.c:158:	*/	ListElem	ListLastElem (List *list)   ;
/*c/list.c:162:	*/	int	ListNumElem(List *list)	     ;
/*c/list.c:166:	*/	Bool	ListIsEmpty(List *list) ;
/*c/list.c:170:	*/	Data	ListElemData(ListElem elem) ;
/*c/soundAF.c:47:	*/	void	PlaySoundFile(char *filename, int forked) ;
/*c/soundAF.c:75:	*/	struct	Sound *LoadFile(char *filename) ;
/*c/soundAF.c:110:	*/	Bool	SetupSound() ;
/*c/support.c:17:	*/	char	*TypeToStr(int type) ;
/*c/support.c:25:	*/	int	StrToType(char *str) ;
/*c/support.c:35:	*/	int	(*StrToCall(char *str, int type)) ;
/*c/support.c:46:	*/	Condition	StrToCondition_FromTable(char *str) ;
/*c/support.c:59:	*/	char	*TranslateKeyCode(XEvent *ev) ;
/*c/support.c:78:	*/	Bool	CheckTime(Action *action, struct timeval *cur) ;
/*c/support.c:86:	*/	NoRet	SetTime(struct timeval *dest, struct timeval *src) ;
/*c/support.c:93:	*/	char	*StrTok(char *str, char *white) ;
/*c/table.c:57:	*/	NoRet	InitTypeToStrTable() ;
/*c/table.c:107:	*/	NoRet	SetAction(int type, Mask mask, int is_set, Condition cond, int (*match)(), int (*call)(), char *string ) ;
/*c/table.c:116:	*/	NoRet	InitActions() ;
/*c/xevact.c:28:	*/	void	select_all_windows(Display *disp, Window root, unsigned long type) ;
/*c/xevact.c:52:	*/	int	main(int argc, char *argv[]) ;
