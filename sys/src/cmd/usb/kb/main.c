#include <u.h>
#include <libc.h>
#include <thread.h>
#include "usb.h"
#include "hid.h"

typedef struct Parg Parg;

enum
{
	Ndevs = 10,
	Arglen = 80,
	Nargs = 10,
};

static void
usage(void)
{
	fprint(2, "usage: %s [-dkm] [-a n] [dev...]\n", argv0);
	threadexitsall("usage");
}

void
threadmain(int argc, char **argv)
{
	char args[Arglen];
	char *as;
	char *ae;
	int accel;
	int pena;
	int csps[] = { KbdCSP, PtrCSP, 0 };

	quotefmtinstall();
	pena = 1;
	ae = args+sizeof(args);
	as = seprint(args, ae, "kb");
	ARGBEGIN{
	case 'a':
		accel = strtol(EARGF(usage()), nil, 0);
		as = seprint(as, ae, " -a %d", accel);
		break;
	case 'd':
		usbdebug++;
		/* fall */
	case 'k':
		as = seprint(as, ae, " -k");
		pena = 0;
		break;
	case 'm':
		as = seprint(as, ae, " -m");
		pena = 1;
		break;
	default:
		usage();
	}ARGEND;

	rfork(RFNOTEG);
	fmtinstall('U', Ufmt);
	threadsetgrp(threadid());
	if(pena == 0)
		csps[1] = 0;
	startdevs(args, argv, argc, matchdevcsp, csps, kbmain);
	threadexits(nil);
}
