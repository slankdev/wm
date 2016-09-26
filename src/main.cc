
#include "wm.h"
#include "exception.h"
#include "log.h"
#include "usrcmd.h"
#include "config.h"


static void init_logs()
{
    mainlog::instance().open_new("/home/slank/wm.main.log");
    keylog::instance().open_new("/home/slank/wm.key.log");
    statlog::instance().open_new("/home/slank/wm.stat.log");
    errlog::instance().open_new("/home/slank/wm.err.log");
}

int main()
{
    try {
        init_logs();
        wm wm;
        wm.setup();
        wm.main_loop();
    } catch (std::exception& e) {
        errlog::instance().write("EXCEPTION: %s", e.what());
    }
}
