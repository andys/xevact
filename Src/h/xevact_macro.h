                                     /*
                                   xevact
                       Associates actions with Events
                            Jon A. Maxwell (JAM)
                                     */


/* general macros for xEventD */

#define DEBUG(x)	if (debug&(x)) 
#define CharToFlag(x)		(1<<(x-'A'))
#define Zero(ptr, size)	bzero(ptr, size)

