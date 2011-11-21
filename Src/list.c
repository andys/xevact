                                     /*
                                   xevact
                       Associates actions with Events
                            Jon A. Maxwell (JAM)
                                     */


typedef void NoRet; 	/* God damn X stole my keyword */
typedef void* Data;
typedef int Bool;

#define TRUE 1
#define FALSE 0
#define NULL ((void *)0)

#include <xevact_list.h>

/* These list routines all are prefixed by "List" so that they don't
   interfer with names outside of the ADT (ie, ListPrev() instead of just
   Prev().) I thought about using #defines to make the names be easier to
   type, but found that the longer names were not a problem.  As far as I
   know, there is no way to get the list functions to core-dump if passed
   correct data or NULL pointers. (Will still crash if passed garbage
   addresses)  So the "if" can be dropped from the following example:
   if (NULL!=elem) elem=ListNextElem(elem);
                                     */
/* The nodes include /pointers/ to data instead of the data itself; this
   greatly increases the flexibility of these routines.
                                     */

/* P.S I'll probably use this self-same code in some of /my/ programs! --JAM */
/* And I did! --JAM */

/* stat collection --to make sure we free everything, etc */
int LS_Free=0, LS_Alloc=0, LS_Next=0, LS_Prev=0, LS_Error=0;
#define FreeS	LS_Free		/* C compiler was having problems with */
#define AllocS	LS_Alloc	/* stat collection varables in different */
#define NextS	LS_Next		/* modules having the same name, so I */
#define PrevS	LS_Prev		/* just define them what I want the hard way */
#define ErrorS	LS_Error	/* (this way is "better" anyhow, so thanks cc!)*/
NoRet ListStats() {
	printf("List Statistics:\n");
	printf("	  Frees: %d\n",FreeS);
	printf("	 Allocs: %d\n",AllocS);
	printf("	  Prevs: %d\n",PrevS);
	printf("	  Nexts: %d\n",NextS);
	printf("	 Errors: %d\n",ErrorS);
	}

/* init */
NoRet InitList(List *list) {
	if (NULL==list) {ErrorS++; return;}
	list->first=NULL;
	list->last=NULL;
	list->num_elements=0;
	}

/* memory (only used by list routines --PRIVATE) */
NoRet FreeElem(ListElem elem) {
	if (NULL==elem) {ErrorS++; return;}
	FreeS++;
	free(elem);
	}
ListElem AllocElem() {
	ListElem elem;
	AllocS++;
	elem=(ListElem)malloc(sizeof(struct ListElem));
	if (NULL==elem) {ErrorS++; return NULL;}
	(elem->next)=(elem->prev)=(elem->data)=NULL;
	return (elem);
	}

/* delete */
NoRet ListUnlinkElem(List *list, ListElem elem) {
	if (NULL==list || NULL==elem) {ErrorS++; return;}
	if (NULL!=elem->prev){elem->prev->next=elem->next;}
			else {list->first=elem->next;}
	if (NULL!=elem->next){elem->next->prev=elem->prev;}
			else {list->last =elem->prev;}
	elem->prev=NULL;
	elem->next=NULL;
	}
Data ListDeleteElem(List *list, ListElem elem) {
	Data data;
	if (NULL==elem || NULL==list) {ErrorS++; return NULL;}
	data=elem->data;
	ListUnlinkElem(list, elem);
	FreeElem(elem);
	list->num_elements--;
	return data;
	}

/* call(ptr) frees the appropriate type of data, so that resource tracking
/* can be done; call=NULL for no freeing of dataptr
/* (ex: ListClear(RunL, FreeElem); where FreeElem(ptr) frees ptr)
                                     */
NoRet ListClear(List *list, int (*call)()) {
	ListElem elem,ptr;
	if (NULL==list) {ErrorS++; return;}
	elem=list->first;
	while (NULL!=elem) {
		ptr=elem->next;
		if (NULL!=elem->data && NULL!=call) call(elem->data);
		FreeElem(elem);
		elem=ptr;
		}
	list->first=NULL;
	list->last =NULL;
	}

/* add new element containing data after a certain element in the list;
/* after=NULL means that the new element should be inserted at the
/* beginning.  A new ListElem is created for the data and returned to the
/* caller
                                     */
ListElem ListAddAfter(List *list, ListElem after, Data data) {
	ListElem elem;
	if (NULL==list) {ErrorS++; return NULL;}
	elem=AllocElem();
	elem->data=data;
	if (NULL==after) { /* add to beginning of list */
		elem->next=list->first;
		elem->prev=NULL;
		if (NULL!=list->first) list->first->prev=elem;
		if (NULL==list->last) {
			list->last=elem;
			elem->next=NULL;
			}
		list->first=elem;
		}
	else { /* add after existing node */
		elem->next=after->next;
		elem->prev=after;
		if (NULL!=after->next) after->next->prev=elem;
		after->next=elem;
		if (after==list->last) list->last=elem;
		}
	list->num_elements++;
	return elem;
	}

/* finds the element conataining data and returns it */
ListElem ListFindData(List *list, Data data) {
	ListElem elem;
	elem=list->first;
	while (NULL!=elem && elem->data!=data) {
		elem=elem->next;
		}
	return (elem);
	}

/* List information operations --All are immediately obvious */
ListElem ListNextElem(ListElem elem) {
	if (NULL==elem) {ErrorS++; return NULL;}
	NextS++; return (elem->next);
	}
ListElem ListPrevElem(ListElem elem) {
	if (NULL==elem) {ErrorS++; return NULL;}
	PrevS++; return (elem->prev);
	}
ListElem ListFirstElem(List *list)   {
	if (NULL==list) {ErrorS++; return NULL;}
	return (list->first);
	}
ListElem ListLastElem (List *list)   {
	if (NULL==list) {ErrorS++; return NULL;}
	return (list->last);
	}
int ListNumElem(List *list)	     {
	if (NULL==list)  {ErrorS++; return 0;}
	return (list->num_elements);
	}
Bool ListIsEmpty(List *list) {
	if (0==ListNumElem(list)) return TRUE; return FALSE;
	}
/* returns the data in a ListElem */
Data ListElemData(ListElem elem) {
	if (NULL==elem)  {ErrorS++; return NULL;}
	return (elem->data);
	}
