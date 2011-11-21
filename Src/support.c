                                     /*
                                   xevact
                       Associates actions with Events
                            Jon A. Maxwell (JAM)
                                     */



#include <X11/Xlib.h>
#include <X11/X.h>

#include <string.h>
#include <sys/time.h>

#include <xevact_types.h>

#include <xevact_macro.h>
#include <xevact_const.h>
#include <xevact_list.h>
#include <xevact_struct.h>
#include <xevact_declare.h>


char *TypeToStr(int type) {
	static int	table_init=TRUE;
	if (table_init) {
		InitTypeToStrTable();
		table_init=FALSE;
		}
	return (TypeToStrTable[type]);
	}
int StrToType(char *str) {
	int	i;
	for (i=0; i<MAX_EVENT_TYPE; i++) {
		if (0==strcasecmp(str,TypeToStr(i)) ) {return i;}
		}
	return 0;
	}	



int (*StrToCall(char *str, int type)) {
	int	i;
	for (i=0; StrToCallTable[i].name!=NULL; i++) {
		if (!strcasecmp(str, StrToCallTable[i].name)) {
			if (!(StrToCallTable[i].type & type)) {
				printf("StrToCall: %s found in table but is used incorrectly.\n",str);
				return NULL;
				}
			return StrToCallTable[i].call;
			}
		}
	printf("StrToCall: %s not found in table.\n",str);
	return NULL;
	}

Condition StrToCondition_FromTable(char *str) {
	int	i;
	for (i=0; StrToConditionTable[i].name!=NULL; i++) {
		if (!strcasecmp(str, StrToConditionTable[i].name)) {
			return StrToConditionTable[i].condition;
			}
		}
	return (Condition)0;
	}


static char key_buff[KEY_BUFF_SIZE];

char *TranslateKeyCode(XEvent *ev) {
	int	count;
	char	*tmp;
	KeySym	ks;

	if (ev) {
		count=XLookupString((XKeyEvent *)ev, key_buff, KEY_BUFF_SIZE, &ks,NULL);
		key_buff[count] = 0;
		if (count==0) {
			tmp=XKeysymToString(ks);
			if (tmp) strcpy(key_buff, tmp);
			else strcpy(key_buff, "");
			}
		return key_buff;
		}
	else return NULL;
	}


Bool CheckTime(Action *action, struct timeval *cur) {
	DEBUG(D_CALLS) printf("Checktime \n");
	if (cur->tv_sec - (action->prev_time.tv_sec)  > (action->delay.tv_sec)) return TRUE;
	if (cur->tv_sec - (action->prev_time.tv_sec)  < (action->delay.tv_sec)) return FALSE;
	if (cur->tv_usec- (action->prev_time.tv_usec) > (action->delay.tv_usec)) return TRUE;
	return FALSE;
	}

NoRet SetTime(struct timeval *dest, struct timeval *src) {
	dest->tv_sec=src->tv_sec;
	dest->tv_usec=src->tv_usec;
	}

/* we need a strtok that understands quotes */
/* white CANNOT have ' or " in it!!! */
char *StrTok(char *str, char *white) {
	static char *string=NULL;
	char *pos,*start;
	int mode;
	
	if (str!=NULL) string=str;
	if (string==NULL) return NULL;
	while (*string!=0 && strchr(white, *string)) {string++;}
	pos=string; mode=0;
	while (1) {
		while (*pos!=0 &&  !strchr(white, *pos)) {
			if (*pos=='"') {
				if (mode==0) mode=1;
				else if (mode==1) mode=0;
				}
			else if (*pos=='\'') {
				if (mode==0) mode=2;
				else if (mode==2) mode=0;
				}
			pos++;
			}
		if (mode==0) break; else pos++;
		}
	if (pos==string) return NULL;

	start=string;
	if (*pos) string=pos+1;
	else string=pos;
	*pos=0;
	return start;
	}
	