#include <X11/X.h>
#include <X11/XF86keysym.h> // /usr/include/X11/keysymdef.h
#include <X11/Xresource.h> // /usr/include/X11/Xresource.h

/* Constants */
// grep $USER /etc/passwd /** Is current user has default shell */
// chsh -s $(which zsh) /** Change default shell if not */
// grep zsh /etc/shells /** Is valid login shell *?
// TERMINAL_PATH will not be in effect once user has pre-defined shell.
#define TERMINAL "st"
#define TERMINAL_PATH "/bin/zsh"
#define EDITOR "vim"

// Brightness controll using (xdotool also useful)
static const char *brightup[]       = { "/home/vallabh/.vim/bin/brightness", "up", "10000", NULL };
static const char *brightdown[]     = { "/home/vallabh/.vim/bin/brightness", "down", "10000", NULL };

// Volume specific settings (xdotool also useful)
static const char *upvol[]   = { "/usr/bin/pactl", "set-sink-volume", "@DEFAULT_SINK@", "+1%",     NULL };
static const char *downvol[] = { "/usr/bin/pactl", "set-sink-volume", "@DEFAULT_SINK@", "-1%",     NULL };
static const char *mutevol[] = { "/usr/bin/pactl", "set-sink-mute",   "@DEFAULT_SINK@", "toggle",  NULL };


/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=9" };
static const char dmenufont[]       = "monospace:size=9";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
    /* xprop(1):
     *  WM_CLASS(STRING) = instance, class
     *  WM_NAME(STRING) = title
     *  xprop | awk ' /^WM_CLASS/{sub(/.* =/, "instance:"); sub(/,/, "\nclass:"); print} /^WM_NAME/{sub(/.* =/, "title:"); print}'
     *  xprop -root -notype -f WM_NAME "8u"  |  sed -n -r 's/WM_NAME = \"(.*)\"/\1/p'
     */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },

    // Tag - 8 ( Heavy on memory )
    {   "jetbrains-phpstorm",       "jetbrains-phpstorm",    NULL,       1<<7,       0,      1},
    {   "Postman",                  "postman",                NULL,       1<<7,       0,     1},
    {   "code",                     "code",                   NULL,       1 <<7,       0,    1},

    // Tag - 9 ( Things on WWW )
	{ "firefox",        "Navigator",       NULL,       1 << 8,       0,           0},
	{ "Google-chrome",  "google-chrome",   NULL,       1 << 8,       0,           0},

};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* Mask        | Value | Key */
/* ------------+-------+------------ */
/* ShiftMask   |     1 | Shift */
/* ControlMask |     4 | Ctrl */
/* Mod4Mask    |    64 | Windows */
/* Mod1Mask    |     8 | Alt */
/* LockMask    |     2 | Caps Lock */
/* Mod2Mask    |    16 | Num Lock */
/* Mod3Mask    |    32 | Scroll Lock */
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
#define SHCMD(cmd) { .v = (const char*[]){ TERMINAL_PATH, "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { TERMINAL, NULL };

static const Key keys[] = {
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
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
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

    /* Vallabh @START */

    // Patche(s) custom key(s)
    { MODKEY,                       XK_f,      togglefullscr,  {0} }, // Fullscreen window
    { MODKEY,                       XK_s,      togglesticky,   {0} }, // Stiky window

    { MODKEY|AltMask,              XK_s,       spawn,          SHCMD("screenkey &") },
    { MODKEY|AltMask|ShiftMask,    XK_s,       spawn,          SHCMD("pkill -9 screenkey") },
    { MODKEY,                       XK_x,       spawn,          SHCMD("xkill") },

    { 0,        XF86XK_MonBrightnessUp,         spawn,          {.v = brightup } },
    { 0,        XF86XK_MonBrightnessDown,       spawn,          {.v = brightdown } },
    { 0,        XF86XK_AudioLowerVolume,        spawn,          {.v = downvol } },
    { 0,        XF86XK_AudioMute,               spawn,          {.v = mutevol } },
    { 0,        XF86XK_AudioRaiseVolume,        spawn,          {.v = upvol   } },
    { 0,        XF86XK_Calculator,              spawn,          SHCMD(TERMINAL " -c calculator -n calculator -e bc -l") },

    /* start editor*/
    { MODKEY|ControlMask,           XK_Return, spawn,          SHCMD(TERMINAL " -c vrkansagara-ide -n vrkansagara-ide -e vim $HOME") },
    { MODKEY,                       XK_r,      spawn,          SHCMD(TERMINAL " -c htop -n htop -e htop -u $USER -d 60") },
    { MODKEY,                       XK_e,      spawn,          SHCMD(TERMINAL " -c ranger -n ranger -e ranger") },
    { MODKEY|ShiftMask,             XK_w,       spawn,         SHCMD(TERMINAL " -c nmtui -n nmtui -e sudo nmtui") },

	/* To quit dwm cleanly (It will hot reload all dwm config, see xinitrc for this) */
    /* close all session of current $USER , use startx */
    { MODKEY|ShiftMask|ControlMask, XK_q,      spawn,          SHCMD(TERMINAL " pkill -u $USER -9")},
    { MODKEY|ShiftMask,             XK_q,      quit,           {0} },


    /* Vallabh @END */
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },

{ ClkStatusText,        MODKEY,         Button2,        spawn,          {.v = termcmd } },
{ ClkStatusText,        0,              Button1,        sigstatusbar,   {.i = 1} },
{ ClkStatusText,        0,              Button2,        sigstatusbar,   {.i = 2} },
{ ClkStatusText,        0,              Button3,        sigstatusbar,   {.i = 3} },

	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

