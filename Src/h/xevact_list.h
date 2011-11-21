                                     /*
                                   xevact
                       Associates actions with Events
                            Jon A. Maxwell (JAM)
                                     */



typedef struct List List;
typedef struct ListElem* ListElem;

struct ListElem {
	ListElem prev,next;
	Data data;	/* generic pointer to... data */
	};
struct List {
	ListElem first,last;	/* shortcuts for fast processing */
	int num_elements;	/* for NumRunning=NumElem(RunL) speed */
	};

