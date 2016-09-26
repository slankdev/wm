

#pragma once


#include <vector>
#include <stdint.h>
#include <stddef.h>
#include <X11/keysym.h>
#include <X11/Xlib.h>

#ifndef _unused
# if defined(__GNUC__)
#  define _unused  __attribute__((__unused__))
# else
#  define _unused
# endif
#endif


struct Arg {
    union {
        int i;
        uint32_t ui;
        float f;
        const void *v;
    };
    static Arg set_i(int ii)
    {
        Arg a;
        a.i = ii;
        return a;
    }
    static Arg set_ui(uint32_t iui)
    {
        Arg a;
        a.ui = iui;
        return a;
    }
    static Arg set_f(float iff)
    {
        Arg a;
        a.f = iff;
        return a;
    }
    static Arg set_v(const void* iv)
    {
        Arg a;
        a.v = iv;
        return a;
    }
};


struct Key {
	uint32_t mod;
	KeySym keysym;
	void (*func)(const Arg *);
	const Arg arg;
};

struct Monitor;
struct Client {
    Window w;
    size_t x, y, width, height;
    Monitor *mon;
};

struct Monitor {
    size_t x;
    size_t y;
    size_t width;
    size_t height;
    Client *cur;
    std::vector<Client> clients;
    Monitor(size_t ax, size_t ay, size_t aw, size_t ah) : 
        x(ax), y(ay), width(aw), height(ah) {}
};
