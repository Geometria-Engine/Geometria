#include "X11Interface.h"

#ifdef __linux__
#include <X11/Xlib.h>
#include <X11/extensions/Xrandr.h>
#endif

float X11Interface::GetLinuxRefreshRate()
{
	#ifdef __linux__
		int num_sizes;

		Display* dpy = XOpenDisplay(NULL);
		Window root = RootWindow(dpy, 0);
		XRRScreenSize* xrrs = XRRSizes(dpy, 0, &num_sizes);

		XRRScreenConfiguration* conf = XRRGetScreenInfo(dpy, root);
		return XRRConfigCurrentRate(conf);
	#else
		return 60;
	#endif
}