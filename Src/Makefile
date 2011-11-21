# define if you want the Audiofile support (8khz .au)
#SOUND=	soundAF
#SNDDEF= -DAUDIOFILE
#SNDLIB= -lAF
# define if you want no sound support
SOUND=  soundNONE
SNDDEF=
SNDLIB=

#XINCLUDE= -I/usr/local/X11R5/include
XLIB=-lX11

#--------------------------------------------------------------

OBJS=	action.o  declare.o list.o sound/$(SOUND).o  \
	support.o table.o xevact.o   xerror.o
OBJS2=	actions/*.o tests/*.o
INCLUDE= -I/usr/local/include -I$(PWD)/h $(XINCLUDE)
LIBS=	 $(SNDLIB) $(XLIB)
CFLAGS=  -g $(SNDDEF) $(INCLUDE)
TARG=	 xevact


all: make_tests make_actions mesg $(TARG)

make_tests:
	@cd tests; cat .message; make       \
	"CC=$(CC)"           \
	"CFLAGS=$(CFLAGS)" all
make_actions:
	@cd actions; cat .message;  make     \
	"CC=$(CC)"           \
	"CFLAGS=$(CFLAGS)" all

mesg:
	@cat .message

$(TARG): $(OBJS)
	-if [ -f $(SOUND).o ]; then mv -f $(SOUND).o sound/; fi
	$(CC) $(CFLAGS) -o $(TARG) $(OBJS) $(OBJS2) $(LIBS)
	strip $(TARG)

clean:
	/bin/rm -f $(OBJS) $(OBJS2) $(TARG)

