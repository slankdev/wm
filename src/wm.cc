
#include "wm.h"
#include "config.h" 

#define CLEANMASK(mask)   (mask & ~(wm::numlockmask|LockMask) \
        & (ShiftMask|ControlMask|Mod1Mask|Mod2Mask|Mod3Mask|Mod4Mask|Mod5Mask))


uint32_t wm::numlockmask = 0;
std::vector<Monitor> wm::mon;
Display* wm::dpy = nullptr;
int wm::screen   = 0;
bool wm::running = false;
Window wm::root  = 0;

void wm::out_status()
{
    statlog& log = statlog::instance();
    log.clean();

    log.write("Display: %p", dpy);
    log.write("screen : %d", screen);
    log.write("root win : %d", root);

    for (size_t i=0; i<mon.size(); i++) {
        log.write("Monitor:%zd", i);
        log.write(" + x,y,width,height: %zd,%zd,%zd,%zd", 
            mon[i].x, mon[i].y, mon[i].width, mon[i].height);
    }
}
void wm::quit()
{
    running = false;
}
wm::wm()
{
    mainlog::instance().write("call %s()", __func__);

    /* open display */
    dpy = XOpenDisplay(nullptr);
    if (!dpy) {
        throw wmexception("can't open display");
    }
    screen = DefaultScreen(dpy);
    root   = RootWindow(dpy, screen);

    int num_scrn;
    XRRMonitorInfo* m = XRRGetMonitors(dpy, root, true, &num_scrn);
    if (!m) {
        throw wmexception("XRRGetMonitors");
    }
    for (int i=0; i<num_scrn; i++) {
        mainlog::instance().write("  Monitor%d", i);
        mainlog::instance().write("   + x        : %d", m[i].x        );
        mainlog::instance().write("   + y        : %d", m[i].y        );
        mainlog::instance().write("   + width    : %d", m[i].width    );
        mainlog::instance().write("   + height   : %d", m[i].height   );
        Monitor m0(m[i].x, m[i].y, m[i].width, m[i].height);
        mon.push_back(m0);
    }

    init_handler();
}
wm::~wm()
{
    mainlog::instance().write("call %s()", __func__);
    XCloseDisplay(dpy);
}
Client* wm::find_client(Window w)
{
    for (size_t i=0; i<mon.size(); i++) {
        for (size_t j=0; j<mon[i].clients.size(); j++) {
            if (mon[i].clients[j].w == w)
                return &mon[i].clients[j];
        }
    }
    return NULL;
}
Client* wm::manage_client(Window w, XWindowAttributes *wa)
{
    XGrabServer(dpy);
    Client c;
    c.w = w;
    c.x = wa->x;
    c.y = wa->y;
    c.width = wa->width;
    c.height = wa->height;

    c.mon = &mon[0];
    mon[0].clients.push_back(c);

    c.x      = mon[0].x;
    c.y      = mon[0].y;
    c.width  = mon[0].width;
    c.height = mon[0].height;

    XUngrabServer(dpy);
    XSelectInput(dpy, c.w, PropertyChangeMask | EnterWindowMask);
    XMoveResizeWindow(dpy, c.w, c.x, c.y, c.width, c.height);
    XRaiseWindow(dpy, c.w);
    XMapWindow(dpy, c.w);

    mainlog::instance().write("new client %x on monitor %p: %dx%d+%d+%d",
        c.w, c.mon, c.width, c.height, c.x, c.y);

    return &mon[0].clients[mon[0].clients.size()-1];
}
void wm::focus(Client *c)
{
    mainlog::instance().write("call %s", __func__);

    if (mon[0].cur == c) return;
    XSetInputFocus(dpy, c->w, RevertToParent, CurrentTime);
    mon[0].cur = c;
}
void wm::init_handler()
{
    mainlog::instance().write("call %s()", __func__);

    memset(&handler, 0, sizeof(handler));
    handler[ButtonPress]      = nullptr;
    handler[ClientMessage]    = nullptr;
    handler[ConfigureRequest] = nullptr;
    handler[ConfigureNotify]  = nullptr;
    handler[DestroyNotify]    = nullptr;
    handler[EnterNotify]      = nullptr;
    handler[Expose]           = nullptr;
    handler[FocusIn]          = nullptr;
    handler[KeyPress]         = keypress;
    handler[MappingNotify]    = nullptr;
    handler[MapRequest]       = maprequest;
    handler[MotionNotify]     = nullptr;
    handler[PropertyNotify]   = nullptr;
    handler[UnmapNotify]      = nullptr;
}
void wm::keypress(XEvent *e)
{
    mainlog::instance().write("call %s", __func__);

    XKeyEvent *ev = &e->xkey;
    KeySym keysym = XKeycodeToKeysym(dpy, (KeyCode)ev->keycode, 0);
    for (uint32_t i = 0; i < sizeof(keys)/sizeof(keys[0]); i++) {
        if (keysym == keys[i].keysym
                && CLEANMASK(keys[i].mod) == CLEANMASK(ev->state)
                && keys[i].func)
            keys[i].func(&(keys[i].arg));
    }
}
void wm::maprequest(XEvent *e)
{
    mainlog::instance().write("call %s", __func__);

    XMapRequestEvent *ev = &e->xmaprequest;
    XWindowAttributes wa;
    mainlog::instance().write("MapRequest on %x", ev->window);
    if (!XGetWindowAttributes(dpy, ev->window, &wa) || wa.override_redirect) {
        mainlog::instance().write(
             "cannot get attibute of %x, or it has override_redirect flag. ignoring",
             ev->window);
        return;
    }
    if (!find_client(ev->window)) {
        Client *c = manage_client(ev->window, &wa);
        focus(c);
    }
}
void wm::initatoms()
{
    wm_protocols            = XInternAtom(dpy, "WM_PROTOCOLS", False);
    wm_delete_window        = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
    wm_state                = XInternAtom(dpy, "WM_STATE", False);
    wm_take_focus           = XInternAtom(dpy, "WM_TAKE_FOCUS", False);
    net_supported           = XInternAtom(dpy, "_NET_SUPPORTED", False);
    net_wm_state            = XInternAtom(dpy, "_NET_WM_STATE", False);
    net_wm_state_fullscreen = XInternAtom(dpy, "_NET_WM_STATE_FULLSCREEN", False);
    net_wm_state_above      = XInternAtom(dpy, "_NET_WM_STATE_ABOVE", False);
}
void wm::setup(void)
{
    mainlog::instance().write("call %s()", __func__);

    XSetErrorHandler(xerrorstart);

    XSelectInput(dpy, root, SubstructureRedirectMask);
    XSync(dpy, false);
    XSetErrorHandler(xerror_handler);

    initatoms();

    cursors[CurNormal] = XCreateFontCursor(dpy, XC_left_ptr);
    cursors[CurMove] = XCreateFontCursor(dpy, XC_fleur);
    cursors[CurResize] = XCreateFontCursor(dpy, XC_sizing);

    XSetWindowAttributes wa;
    wa.cursor = cursors[CurNormal];
    wa.event_mask = SubstructureRedirectMask | SubstructureNotifyMask |
        StructureNotifyMask | ButtonPressMask | EnterWindowMask;
    XChangeWindowAttributes(dpy, root, CWEventMask | CWCursor, &wa);

    if (!(gc = XCreateGC(dpy, root, 0, NULL))) {
        throw wmexception("failed to create gc");
    }
    XSetForeground(dpy, gc, BlackPixel(dpy, screen));
    grab_keys();
    XSelectInput(dpy, root, wa.event_mask);
    running = true;
}
void wm::grab_keys()
{
    XUngrabKey(dpy, AnyKey, AnyModifier, root);
    for (size_t i = 0; i < sizeof(keys)/sizeof(keys[0]); ++i) {
        XGrabKey(dpy, XKeysymToKeycode(dpy, keys[i].keysym),
            keys[i].mod, root, True, GrabModeAsync, GrabModeAsync);
    }
}
void wm::main_loop()
{
    mainlog::instance().write("call %s", __func__);
    while (running) {
        out_status();

        XEvent e;
        XNextEvent(dpy, &e);

        if (handler[e.type]) {
            mainlog::instance().write("  supported event: %d", e.type);
            handler[e.type](&e);
        } else {
            mainlog::instance().write("  unsupported event: %d", e.type);
        }

    }
}
int wm::xerrorstart(Display* dpy0 _unused, XErrorEvent* ee _unused)
{
    std::string errstr = "wm: another window manager is already running";
    throw wmexception(errstr.c_str());
}
int wm::xerror_handler(Display *dpy, XErrorEvent *e)
{
    static char buf[64];
    XGetErrorText(dpy, e->error_code, buf, sizeof(buf));
    fprintf(stderr, "X error received: request=%d, code=%d %s\n",
            e->request_code, e->error_code, buf);
    return 0;
}

