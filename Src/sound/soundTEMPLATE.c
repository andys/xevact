                                     /*
                            XEvAct, Version 1.0a
                       Associates actions with Events
                            Jon A. Maxwell (JAM)
                                     */



#include <X11/Xlib.h>
#include <X11/X.h>

/* audio includes */

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <limits.h>

#include <string.h>

#include <xevact_types.h>

#include <xevact_macro.h>
#include <xevact_const.h>
#include <xevact_list.h>
#include <xevact_struct.h>
#include <xevact_declare.h>

struct Sound *LoadFile(char *filename);


/* sound player variables for device */
/* static ! */


List		InMemList;

#define SND_NAMELEN	128

/* this structure may need more fields for nonAU sound */
struct Sound {
	char name[SND_NAMELEN];
	int size;
	char *data;
	};



/* forked tell us if we have forked to play this sound, because some
   connection sound like AF doesn't like two processes writing to the same
   connection (maybe just the same ac, but who cares?). */
void PlaySoundFile(char *filename, int forked) {
	static int sound_is_setup=FALSE;
	struct Sound *sound;
	
	DEBUG(D_CALLS) printf("PlaySoundFile: %s\n",filename);
	if (!sound_is_setup || forked) {
		DEBUG(D_CHECKP) printf("PSF: Connecting to AF server\n");
		Zero(&InMemList, sizeof(InMemList));
		if (!SetupSound()) return;
		sound_is_setup=TRUE;
		}
	sound=(struct Sound *) LoadFile(filename);
	DEBUG(D_CHECKP) printf("PSF: Loaded %s into %d\n",filename, sound);
	if (sound==NULL) return;

	/* this part is sound-device dependant */
	DEBUG(D_CHECKP) printf("PSF: Actually playing now!\n");
	/* now play the sound */
	}




/* This finds out if the sound is already in memeory and loads it if not.
   Everything is stored in a linked list of sounds, in memory, so that
   the next time we go to play the sound, it takes no time to load
                                     */
struct Sound *LoadFile(char *filename) {
	ListElem elem;
	struct	Sound *sound;
	struct	stat statbuf;
	int	fd;

	elem=ListFirstElem(&InMemList);
	while (NULL!=elem) {
		sound=(struct Sound *)ListElemData(elem);
		if (!strcmp(sound->name,filename)) {
			return sound;
			}
		elem=ListNextElem(elem);
		}
	sound=(struct Sound *)malloc(sizeof(struct Sound));
	Zero(sound, sizeof(struct Sound));
	if (!stat(filename, &statbuf)) 
	strncpy(sound->name, filename, SND_NAMELEN);
	sound->size=statbuf.st_size;
	sound->data=(char *)malloc(sound->size);
	fd=open(filename, O_RDONLY, 0);
	if (fd<0) return FALSE;
	read(fd, sound->data, sound->size);
	close(fd);
	ListAddAfter(&InMemList, ListLastElem(&InMemList), sound);
	return sound;
	}



Bool SetupSound() {
	/* open the sound device/connection/whatever */
	return TRUE;
	}


