

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

} /* usrcmd */
