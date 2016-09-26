

#include "usrcmd.h"
#include "wm.h"

namespace usrcmd {

void quit(const Arg* arg _unused)
{
    wm::quit();
}
void spawn(const Arg* arg)
{
    mainlog::instance().write("call %s(%s)", __func__, (const char*)arg->v);

    if (fork() == 0) {
        close(ConnectionNumber(wm::dpy));
        setsid();
        execlp((const char*)arg->v, (const char*)arg->v, NULL);
        throw wmexception("excec failed");
    }
}
void kill(const Arg* arg _unused)
{
    Window w;
    int revert;
    XGetInputFocus(wm::dpy, &w, &revert);
    mainlog::instance().write("call %s() want to kill 0x%x", __func__, w);

    for (Monitor& m : wm::mon) {
        for (size_t i=0; i<m.clients.size(); i++) {
            if (w == m.clients[i].w) {
                XKillClient(wm::dpy, w);
                m.clients.erase(m.clients.begin() + i);
                return ;
            }
        }
    }
    throw wmexception("client to delete was not found. it is bug");
}
void focusnext(const Arg* arg)
{
    mainlog::instance().write("call %s(%d)", __func__, arg->i);
}


} /* usrcmd */
