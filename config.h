/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx  = 3;        /* border pixel of windows */
static const int startwithgaps[]    = { 0 };	/* 1 means gaps are used by default, this can be customized for each tag */
static const unsigned int gappx[]   = { 10 };   /* default gap between windows in pixels, this can be customized for each tag */
static unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static int showbar            = 1;        /* 0 means no bar */
static int topbar             = 1;        /* 0 means bottom bar */
static char *fonts[]          = { "monospace:size=10", "JoyPixels:pixelsize=10:antialias=true:autohint=true", "FontAwesome:pixelsize=14"  };
static const char dmenufont[]       = "monospace:size=10";
static char normbgcolor[]            = "#222222";
static char normbordercolor[]      = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor,  normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating		isterminal		noswallow 	 monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,          	0,   			0,			 -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,          	0, 				-1,			 -1 },
    { "St",       NULL,       NULL,       0, 	        0,   	        1,    	  	    0,  		 -1 },
	{ NULL,       NULL,   "Event Tester", 0,       	    0, 	 	        0,    	        1,           -1 }, /* xev */
    { "Zathura",    NULL,       NULL,       0,            0,              0,              1,           -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[T]",      tile },    /* first entry is default */
	{ "[F]",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

#define MODKEY Mod4Mask

#define TAGKEYS(CHAIN,KEY,TAG) \
     { MODKEY,                       CHAIN,    KEY,      view,           {.ui = 1 << TAG} }, \
     { MODKEY|ControlMask,           CHAIN,    KEY,      toggleview,     {.ui = 1 << TAG} }, \
     { MODKEY|ShiftMask,             CHAIN,    KEY,      tag,            {.ui = 1 << TAG} }, \
     { MODKEY|ControlMask|ShiftMask, CHAIN,    KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run_history" };
static const char *termcmd[]  = { "st", NULL };

static const char *mutevol[] = { "/usr/bin/amixer", "set",   "Master", "toggle",  NULL };
static const char *playpause[] = { "/usr/bin/playerctl", "play-pause", NULL };
static const char *next[] = { "/usr/bin/playerctl", "next", NULL };
static const char *prev[] = { "/usr/bin/playerctl", "previous", NULL };

/*
 * Xresources preferences to load at startup
 */
ResourcePref resources[] = {
		{ "color0",		STRING,	&normbordercolor },
		{ "color8",		STRING,	&selbordercolor },
		{ "color0",		STRING,	&normbgcolor },
		{ "foreground",		STRING,	&normfgcolor },
		{ "color15",		STRING,	&selfgcolor },
		{ "color4",		STRING,	&selbgcolor },
		{ "borderpx",		INTEGER, &borderpx },
		{ "snap",		INTEGER, &snap },
		{ "showbar",		INTEGER, &showbar },
		{ "topbar",		INTEGER, &topbar },
		{ "nmaster",		INTEGER, &nmaster },
		{ "resizehints",	INTEGER, &resizehints },
		{ "mfact",		FLOAT,	&mfact },
		{ "swallowfloating",	INTEGER, &swallowfloating },
};


static Key keys[] = {
	/* modifier         	keychain        key        function        argument */
	{ MODKEY,      	  		   XK_d, 	    XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,				   XK_d,		XK_p,	   spawn,		   SHCMD("passmenu2 -i")},
	{ MODKEY,                  XK_d,        XK_c,      spawn,          SHCMD("= --dmenu=dmenu")},
    { MODKEY,      			   XK_d,        XK_k,      spawn,          SHCMD("dmenu_kb")},
	{ MODKEY,                  XK_d,        XK_e,      spawn,          SHCMD("dmenu_emoji")},
	{ 0,					   -1,			XK_Print,  spawn,	   	   SHCMD("dmenu_scrot") },
	{ ShiftMask,               -1,          XK_Print,  spawn,          SHCMD("flameshot gui") },
	{ MODKEY,		           -1,		    XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                  -1, 		    XK_b,      togglebar,      {0} },
	{ MODKEY|ShiftMask,        -1,          XK_b,      spawn,          SHCMD("kill -44 $(pidof dwmblocks)")},
	{ MODKEY,                  -1,		    XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY|ShiftMask,        -1,          XK_s,      focusstack,     {.i = +1 } },
	{ MODKEY,                  -1,		    XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,        -1,          XK_w,      focusstack,     {.i = -1 } },
	{ MODKEY,                  -1,		    XK_x,      incnmaster,     {.i = +1 } },
	{ MODKEY,                  -1,     	    XK_z,      incnmaster,     {.i = -1 } },
	{ MODKEY,                  -1,     		XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY|ShiftMask,        -1,          XK_a,      setmfact,       {.f = -0.05} },
	{ MODKEY,                  -1,		  	XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,        -1,          XK_d,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,        -1,     		XK_Return, zoom,           {0} },
	{ MODKEY,                  -1,     		XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,        -1,     		XK_q,      killclient,     {0} },
	{ MODKEY,                  -1,     		XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                  -1,     		XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                  -1,     		XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                  -1,     		XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,        -1,     		XK_space,  togglefloating, {0} },
	{ MODKEY,                  -1,     		XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,        -1,     		XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                  -1,     		XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                  -1,     		XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,        -1,     		XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,        -1,     		XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                  -1, 		    XK_F5,     xrdb,           {.v = NULL } },
	{ MODKEY,                  -1,     		XK_minus,  setgaps,        {.i = -5 } },
	{ MODKEY,                  -1,     		XK_equal,  setgaps,        {.i = +5 } },
	{ MODKEY|ShiftMask,        -1,     		XK_minus,  setgaps,        {.i = GAP_RESET } },
	{ MODKEY|ShiftMask,        -1,     		XK_equal,  setgaps,        {.i = GAP_TOGGLE} },
	TAGKEYS(                   -1,     		XK_1,                      0)
	TAGKEYS(                   -1,     		XK_2,                      1)
	TAGKEYS(                   -1,     		XK_3,                      2)
	TAGKEYS(                   -1,     		XK_4,                      3)
	TAGKEYS(                   -1,     		XK_5,                      4)
	TAGKEYS(                   -1,     		XK_6,                      5)
	TAGKEYS(                   -1,     		XK_7,                      6)
	TAGKEYS(                   -1,     		XK_8,                      7)
	TAGKEYS(                   -1,     		XK_9,                      8)
	{ 0,                       -1,	  		XF86XK_AudioLowerVolume, spawn, SHCMD("amixer set Master 5%-; kill -44 $(pidof dwmblocks)") },
	{ 0,                       -1,			XF86XK_AudioMute, spawn,   SHCMD("/usr/bin/amixer set Master toggle; kill -44 $(pidof dwmblocks)") },
	{ 0,                       -1,			XF86XK_AudioRaiseVolume, spawn, SHCMD("amixer set Master 5%+; kill -44 $(pidof dwmblocks)") },
    { 0,                       -1,			XF86XK_AudioNext, spawn, {.v = next   } },
    { 0,                       -1,			XF86XK_AudioPrev, spawn, {.v = prev   } },
    { 0,                       -1,			XF86XK_AudioPlay, spawn, {.v = playpause   } },
	{ MODKEY|ShiftMask,        -1,     		XK_r,      quit,           {0} },
	{ MODKEY|ShiftMask,        -1,          XK_e,      spawn,          SHCMD("pkill Xorg")},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
