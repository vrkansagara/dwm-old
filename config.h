/* See LICENSE file for copyright and license details. */
#include <X11/X.h>
#include <X11/XF86keysym.h> // /usr/include/X11/keysymdef.h
#include <X11/Xresource.h> // /usr/include/X11/Xresource.h
#include "layouts.c"

/* Constants */
#define TERMINAL "st"
#define EDITOR "vim"

// grep $USER /etc/passwd /** Is current user has default shell */
// chsh -s $(which zsh) /** Change default shell if not */
// grep zsh /etc/shells /** Is valid login shell *?
#define TERMINAL_PATH "/bin/zsh"

/* appearance */
static unsigned int borderpx        = 3;        /* border pixel of windows */
static unsigned int snap            = 32;       /* snap pixel */
static unsigned int gappih          = 20;       /* horiz inner gap between windows */
static unsigned int gappiv          = 10;       /* vert inner gap between windows */
static unsigned int gappoh          = 10;       /* horiz outer gap between windows and screen edge */
static unsigned int gappov          = 30;       /* vert outer gap between windows and screen edge */
static int swallowfloating          = 0;        /* 1 means swallow floating windows by default */
static int smartgaps				= 0;        /* 1 means no outer gap when there is only one window */
static int showbar					= 1;        /* 0 means no bar */
static const char *fonts[]          = { "Fira Code Medium:size=12:antialias=true:autohint=true", };
static int topbar					= 1;        /* 0 means bottom bar */
static const char dmenufont[]		= { "Fira Code Medium:size=12:antialias=true:autohint=true", };
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#770000"; // #005577
static char selbgcolor[]            = "#005577";
static char *colors[][3]		= {
	/*               fg           bg           border   */
	[SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
	[SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};


/* initial layouts per tag ( Index of layouts[]  */
static const int initlayouts[] = { 0, 1, 3, 0, 5, 4, 2 ,2, 2 };

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",		tile },						/* 0. Default: Master on left, slaves on right */
	{ "[D]",		deck},						/* 1. Master on left, slaves in monocle-like mode on right */

	{ "[M]",		monocle },                  /* 2. All windows on top of eachother ( Full window ) */
	{ "|M|",		centeredmaster },			/* 3. Master in middle, slaves on sides */
	{ ">M>",		centeredfloatingmaster },	/* 4. Same but master floats */

	// { "HHH",		grid },						/* 5. Grid layout */
	// { "TTT",		bstack},					/* 6. Master on top, slaves on bottom */
	// { "===",		bstackhoriz},				/* 7. Bstack horiz layout */

	{ "><>",		NULL },						/* 8. no layout function means floating behavior */
	{ NULL,			NULL },						/* ~ Failback */
};


static const Rule rules[] = {
	/* poxrop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 *	xprop | awk ' /^WM_CLASS/{sub(/.* =/, "instance:"); sub(/,/, "\nclass:"); print} /^WM_NAME/{sub(/.* =/, "title:"); print}'
	 *	xprop -root -notype -f WM_NAME "8u"  |  sed -n -r 's/WM_NAME = \"(.*)\"/\1/p'
	 */

	/** class           instance,               title,              tags,               isfloating,             monitor */
	// Tag - 1 (General usse case)
	{   "Thunar",      "thunar",      NULL,       1<<0,       0,      0},

	// Tag - 2 (Terminal way on demand )
	{   "St",      "st",      NULL,       1<<1,       0,      0},


	// Tag - 3 (Socializing)
	{   "Skype",      "skype",      NULL,       1<<2,       0,      0},
	{   "Microsoft Teams - Preview","microsoft teams - preview",      NULL,       1<<2,       0,      0},

	// Tag - 4 (Office on the way)
	{   "ringcentral",      "ringcentral",      NULL,       1<<3,       0,      0},

	// Tag - 5 ( Remote things)
	{   "org.remmina.Remmina" ,"org.remmina.Remmina",       NULL,       1 << 4,       0,           0 },
	{   "Anydesk",      "anydesk",      NULL,       1<<4,       0,      0},

	// Tag - 6 (Utilities)
	{ "Hubstaff"    ,"Hubstaff",       NULL,       1 << 5,       0,           0 },
	{ "KeePassXC"    ,"keepassxc",       NULL,       1 << 5,       0,           0 },
	{ "obs"    ,"obs",       NULL,       1 << 5,       0,           0 },

	// Tag - 7 ( VIM way, Light weight on memory )
	{   "vrkansagara-ide",               "vrkansagara-ide",      NULL,       1<<6,       0,      -1},

	// Tag - 8 ( Heavy on memory )
	{   "jetbrains-phpstorm",       "jetbrains-phpstorm",    NULL,       1<<7,       0,      0},
	{   "Postman",                  "postman",                NULL,       1<<7,       0,      0},
	{   "code",                     "code",                   NULL,       1 << 7,       0,    0 },
	{ 	"Geany",					"geany",               NULL,       1 << 7,       0,           1 },

	// Tag - 9 ( Things on WWWW )
	{ "Firefox"                 ,"Navigator",               NULL,       1 << 8,       0,           1 },
	{ "Google-chrome"           ,"google-chrome",          NULL,       1 << 8,       0,      1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

/* Mask        | Value | Key */
/* ------------+-------+------------ */
/* ShiftMask   |     1 | Shift */
/* LockMask    |     2 | Caps Lock */
/* ControlMask |     4 | Ctrl */
/* Mod1Mask    |     8 | Alt */
/* Mod2Mask    |    16 | Num Lock */
/* Mod3Mask    |    32 | Scroll Lock */
/* Mod4Mask    |    64 | Windows */
/* Mod5Mask    |   128 | ??? */

/* key definitions */
// #define MODKEY Mod1Mask // Alt key for meta (Default as per DWM)
#define AltMask Mod1Mask  // Alt key for meta
#define MODKEY Mod4Mask // Window key for meta
#define TAGKEYS(KEY,TAG) \
{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
// static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { TERMINAL, NULL };


static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },


	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },		/* tile */
	{ MODKEY|ShiftMask,				XK_t,      setlayout,      {.v = &layouts[1]} },		/* deck */

	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },		/* monocle */
	{ MODKEY|ShiftMask,				XK_m,	   setlayout,	   {.v = &layouts[3]} },		/* centeredmaster */
	{ MODKEY|ControlMask|ShiftMask,	XK_m,	   setlayout,	   {.v = &layouts[4]} },		/* centeredfloatingmaster */

	{ MODKEY,						XK_g,	   setlayout,	   {.v = &layouts[5]} },		/* grid */
	{ MODKEY|ShiftMask,				XK_g,	   setlayout,	   {.v = &layouts[6]} },		/* bstack (TTT) */
	{ MODKEY|ShiftMask|ControlMask, XK_g,	   setlayout,	   {.v = &layouts[7]} },		/* bstackhoriz(===)*/

	{ MODKEY,						XK_f,	   setlayout,	   {.v = &layouts[8]} },		/* bstackhoriz */
	
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	
	
	// Patche(s) custom key(s)
	{ MODKEY,                       XK_s,      togglesticky,   {0} }, // Stiky window
	{ MODKEY,						XK_f,      togglefullscr,  {0} }, // Fullscreen window

};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

