
#pragma once
#define MODKEY Mod1Mask


static Key keys[] = {
	/* modifier          key          function           argument */
	{ MODKEY,            XK_q,        usrcmd::quit,      {0}                     },
	{ MODKEY|ShiftMask,  XK_Return,   usrcmd::spawn,     Arg::set_v("urxvt")     },
	{ MODKEY,            XK_Return,   usrcmd::spawn,     Arg::set_v("xterm")     },
	{ MODKEY,            XK_f,        usrcmd::spawn,     Arg::set_v("chromium")  },
	{ MODKEY,            XK_p,        usrcmd::spawn,     Arg::set_v("dmenu_run") },
	{ MODKEY,            XK_c,        usrcmd::kill,      {0}                     },
	{ MODKEY,            XK_j,        usrcmd::focusnext, Arg::set_i(+1)          },
	{ MODKEY,            XK_k,        usrcmd::focusnext, Arg::set_i(-1)          },
	// { MODKEY,            XK_space,    setlayout,  {0}                     },
	// { MODKEY|ShiftMask,  XK_h,        setmfact,   Arg::set_f(-0.05)       },
	// { MODKEY|ShiftMask,  XK_l,        setmfact,   Arg::set_f(0.05)        },
};

