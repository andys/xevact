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


/* this should be a macro, but I was having trouble debugging :) */
Bool IsSet(int type) {return (set[type].is_set);}


/* figure out mask for XSelectInput to get the events we have actions for */
Mask ActionMask_Tot() {
	int	i;
	Mask	mask=0L;
	for (i=0; i<MAX_EVENT_TYPE; i++) {
		if (IsSet(i)) mask|=set[i].mask;
		}
	return mask;
	}


/* handle time, delays, stuff */
Bool TimeOk(Action *action, struct timeval *cur) {
	if (CheckTime(action, cur)) {
		SetTime(&action->prev_time, cur);
		return TRUE;
		}
	if (action->flags & CharToFlag('D')) SetTime(&action->prev_time, cur);
	return FALSE;
	}


/* call the action, based on its flags and stuff */
int CallIt(Action *action, XEvent *xev) {
	if (NULL!=action->call) {
		if (action->flags & CharToFlag('F')) {
			register int rc;
			rc=fork();
			if (rc==0) { /* child */
				action->call(action, xev);
				exit();
				}
			}
		else action->call(action, xev);
		}
	}


/* take an event and call the actions for it */
Action *ActionCall(XEvent *xev, int mode) {
	struct timeval	cur_time;
	struct timezone	timezone;
	ListElem elem;
	Action	 *def_action,*action;

	gettimeofday(&cur_time, &timezone);
	def_action=&set[xev->type].default_action;
	elem=ListFirstElem(&set[xev->type].tlist);
	while (NULL!=elem) {
		int	i;
		action=ListElemData(elem);
		for (i=0; i<action->tests; i++) {
			DEBUG(D_CHECKP) printf("test %d\n",i);
			/* not[i] defaults to FALSE --it is TRUE if we read in the
			   test-function as "!test-function" */
			if (action->match[i]!=NULL && action->not[i]==action->match[i](action, xev, i)) break;
			}
		if (i==action->tests) { /* all tests were true */
			if (TimeOk(action, &cur_time)) {
				DEBUG(D_CHECKP|D_EVENT) printf("DEBUG: Action is called\n");
				CallIt(action,xev);
				action->ref_count++;
				def_action=NULL;
				if (action->flags & CharToFlag('S')) break;
				}
			if (action->flags & CharToFlag('T')) break;
			}
		elem=ListNextElem(elem);
		}
	if (NULL!=def_action) {
		DEBUG(D_EVENT) printf("DEBUG: Default Action is called\n");
		if (TimeOk(def_action, &cur_time)) CallIt(def_action, xev);
		def_action->ref_count++;
		}
	return def_action;
	}

                                     /*
                                 Action File
                                     IO
                                     */

/* LoadActions is has a wimpy format;  it should read in tests in tree form like: 
   PlaySound "pipe.au" if Test_KeyCode['|'] && (Test_State["shift"] || Test_State["caps"])
   In addition, delay and even type should even be tests in the big 'if' --that would allow
   more complex testing, like if Type[KeyPress] || (Type[ButtonRelease] && Test_State["button1"])
                                     */

/* StrTok understands quotes (found in support.c) */
#define NextToken	token=StrTok(NULL, WHITE_SPACE); \
			if (NULL==token) continue;
Bool LoadActions(char *file) {
	FILE	*fp;
	int	type, i;
	char	line[MAX_LINE_LEN],*token;
	Action	action,*new_action=NULL;
	
	DEBUG(D_LOAD) printf("LoadActions: %s\n",file);
	fp=fopen(file, "r");
	if (NULL==fp) {
		printf("Cannot open file: %s.\n",file);
		return FALSE;
		}
	while (fgets(line,MAX_LINE_LEN,fp)) {
		if (line[0]=='#') continue;
		if (strlen(line)<=1) continue;

		DEBUG(D_LOAD) printf("DEBUG: Read line: %s\n",line);
		{register int slen;
			slen=strlen(line);
			line[slen-1]=0; /* take out \n from fgets */
			line[slen]=0;   /* add extra \0 for last param check */
			}

		Zero(&action, sizeof(action));

		/* Event Type */
		token=StrTok(line, WHITE_SPACE);
		if (NULL==token) continue;
		type=StrToType(token);
		
		/* Flags */
		NextToken;
		action.flags=StrToFlags(token);

		/* delay */
		NextToken;
		action.delay.tv_sec=atoi(token);
		NextToken;
		action.delay.tv_usec=atoi(token);
		while (action.delay.tv_usec>1000000) {
			action.delay.tv_usec-=1000000;
			action.delay.tv_sec++;
			}
		DEBUG(D_LOAD) printf("DEBUG: delay is sec %d usec %d\n",action.delay.tv_sec,action.delay.tv_usec);
		
		/* Condition & test functions */
		NextToken;
		action.tests=atoi(token);
		new_action=NULL;
		for (i=0; i<action.tests; i++) {
			NextToken;
			if (!strcmp("DEFAULT",token)) {
				new_action=&set[type].default_action;
				}
			else {
				if (token[0]=='-' && token[1]==0) action.match[i]=NULL;
				else {
					if (token[0]=='!') {
						action.not[i]=TRUE;
						token++;
						}
					else action.not[i]=FALSE;
					action.match[i]=StrToCall(token,CALL_TEST);
					}	
				}
			
			NextToken;
			action.cond[i]=StrToCondition(token);
			}
		DEBUG(D_LOAD) printf("Read %d tests\n",i);
		
		/* Call */
		NextToken;
		action.call=StrToCall(token,CALL_ACTION);

		/* call's parameters (in string action.string) */
		token=&token[strlen(token)+1];
		if (strlen(token)>0) {
			action.string=(char *)malloc(strlen(token)+1);
			strcpy(action.string, token);
			DEBUG(D_LOAD) printf("Call Function Parameter: %s\n", token);
			}
		else action.string=NULL;
		
		
		/* add to list */
		set[type].is_set=TRUE;
		if (NULL==new_action) {
			new_action=(Action *)malloc(sizeof(Action));
			bcopy(&action, new_action, sizeof(Action));
			ListAddAfter(&set[type].tlist, ListLastElem(&set[type].tlist), new_action);
			}
		else	bcopy(&action, new_action, sizeof(Action));
		}
	return TRUE;
	}


Condition StrToCondition(char *str) {
	KeySym	keysym;
	KeyCode	keycode;
	Condition condition;

	if (str[0]=='-' && str[1]==0) return 0;
	if (str[0]>='0' && str[0]<='9') {	/* keycode number is specified */
		return atoi(str);
		}
	else if (str[0]=='\'' || str[0]=='"') {	/* asking to convert a keysym text to keycode */
		str[strlen(str)-1]=0;
		str=&str[1];
		keysym=XStringToKeysym(str);
		if (keysym==0) {
			printf("StrToCondition: '%s' does not map to a keysym!\n",str);
			}
		keycode=XKeysymToKeycode(disp, keysym);
		DEBUG(D_CALLS) printf("StrToCondition: '%s' -> %d -> %d\n",str,keysym,keycode);
		return (keycode);
		}
	else {	/* otherwise look up the code in a special table */
		condition=StrToCondition_FromTable(str);
		if (condition==0) {printf("StrToCondition: '%s' not found in table.\n",str);}
		DEBUG(D_CALLS) printf("StrToCondition: '%s' -> %d\n",str,condition);
		return StrToCondition_FromTable(str);
		}
	}


Flag StrToFlags(char *str) {
	Flag flags=0L;
	
	for (;*str; str++) {
		if (isupper(*str)) flags|=CharToFlag(*str);
		}
	return flags;
	}

