
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <string>
#include <vector>
#include <exception>
#include <sstream>
#include <ostream>
#include <string>

#include <X11/Xlib.h>
#include <X11/Xlibint.h>
#include <X11/keysym.h>
#include <X11/Xproto.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/cursorfont.h>
#include <X11/extensions/Xrandr.h>
#include <X11/extensions/Xrender.h>

#include "types.h"
#include "log.h"
#include "exception.h"
#include "usrcmd.h"


using mainlog = log<class main_log>;
using keylog  = log<class keypress_log>;
using statlog = log<class status_log>;
using errlog  = log<class error_log>;


enum { CurNormal, CurMove, CurResize, CurLast };

class wm {
public:
    static Display* dpy;
    static int screen;
    static bool running;
    static Window root;
    static uint32_t numlockmask;
    static std::vector<Monitor> mon;
    void (*handler[LASTEvent])(XEvent *);
    Atom wm_protocols;
    Atom wm_delete_window;
    Atom wm_state;
    Atom wm_take_focus;
    Atom net_supported;
    Atom net_wm_state;
    Atom net_wm_state_fullscreen;
    Atom net_wm_state_above;
    GC gc;
    XFontStruct* fn;
    Cursor cursors[CurLast];

    wm();
    ~wm();
    void out_status();
    void init_handler();
    void initatoms();
    void setup(void);
    void grab_keys();
    void main_loop();

    static int xerrorstart(Display* dpy0 _unused, XErrorEvent* ee _unused);
    static int xerror_handler(Display *dpy, XErrorEvent *e);
    static void quit();
    static void keypress(XEvent *e);
    static void maprequest(XEvent *e);
    static void focus(Client *c);
    static Client* find_client(Window w);
    static Client* manage_client(Window w, XWindowAttributes *wa);
    static unsigned long alloc_color(const char *name);
};
