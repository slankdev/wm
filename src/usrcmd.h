
#pragma once
#include "types.h"

namespace usrcmd {

void quit(const Arg* arg _unused);
void spawn(const Arg* arg);
void kill(const Arg* arg _unused);
void focusnext(const Arg* arg);

} /* usrcmd */
