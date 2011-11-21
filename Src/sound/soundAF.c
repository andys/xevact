                                     /*
                            XEvAct, Version 1.0a
                       Associates actions with Events
                            Jon A. Maxwell (JAM)
                                     */

/* Notes:
	I think the sound timing is off.  Sometimes when many sounds are
	playing at once some sounds will be cut short or not play.
*/


#include <X11/Xlib.h>
#include <X11/X.h>

#include <AF/AFlib.h>
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


/* non-player specific variables */
List		InMemList;
#define SND_NAMELEN	128


/* sound player variables for AF */
AC		ac;
AFAudioConn	*aud;
ATime		atime;
int		SPEAKER=0;
int		PLAY_DELAY=100;


/* this structure may need more fields for nonAU sound */
struct Sound {
	char name[SND_NAMELEN];
	int size;
	char *data;
	};



/* To change for nonAF-sound, change SetupSound() and the last part
   of this function.  Change LoadFile only if you need more data stored
   when loading. (maybe sample rate, stereo on/off, etc)
                                     */
void PlaySoundFile(char *filename, int forked) {
	static int sound_is_setup=FALSE;
	struct Sound *sound;
	
	DEBUG(D_CALLS) printf("PlaySoundFile: %s\n",filename);
	if (!sound_is_setup || forked) {
		DEBUG(D_CHECKP) printf("PSF: Connecting to AF server\n");
		Zero(&InMemList, sizeof(InMemList));
		if (!SetupSound()) {
			printf("Could not connect to sound server.\n");
			return;
			}
		sound_is_setup=TRUE;
		}
	sound=(struct Sound *) LoadFile(filename);
	DEBUG(D_CHECKP) printf("PSF: Loaded %s into %d\n",filename, sound);
	if (sound==NULL) return;

	/* this part is sound-device dependant */
	DEBUG(D_CHECKP) printf("PSF: Actually playing now!\n");
	atime=AFGetTime(ac);
	AFPlaySamples(ac, atime+PLAY_DELAY, sound->size, (unsigned char *)sound->data);
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



/* this will open a connection to the AF sound server.  If you are porting
   to a different sound format, this should do all the setup, so when we
   go to play the sample we can just do something like AFPlaySamples() or
   write(dev_audio_file_descriptor, data);
                                     */
Bool SetupSound() {
	AFSetACAttributes attributes;
	int device;
	char *server;

	device = SPEAKER;
	attributes.preempt = Mix;
	attributes.start_timeout = 0;
	attributes.end_silence = 0;
	attributes.play_gain = 0;
	attributes.rec_gain =  0;
	server=(char *)getenv("AUDIOFILE");
	if (server==NULL) server=":0";
	aud  =AFOpenAudioConn(server);
	if (aud==NULL) return FALSE;
	ac   =AFCreateAC(aud, device, ACPlayGain, &attributes);
	atime=AFGetTime(ac);
	return TRUE;
	}


