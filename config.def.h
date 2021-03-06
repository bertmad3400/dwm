/* See LICENSE file for copyright and license details. */

/* Used terminal */
#define TERMINAL "st"


/* appearance */
static const unsigned int borderpx	= 3;		/* border pixel of windows */
static const unsigned int snap		= 32;		/* snap pixel */
static const unsigned int gappih	= 10;		/* horiz inner gap between windows */
static const unsigned int gappiv	= 10;		/* vert inner gap between windows */
static const unsigned int gappoh	= 10;		/* horiz outer gap between windows and screen edge */
static const unsigned int gappov	= 15;		/* vert outer gap between windows and screen edge */

static       int smartgaps			= 0;		/* 1 means no outer gap when there is only one window */

static const int showbar			= 1;		/* 0 means no bar */
static const int topbar				= 1;		/* 0 means bottom bar */

static const int vertpad			= 10;		/* vertical padding of bar */
static const int sidepad			= 10;		/* horizontal padding of bar */

static const int swallowfloating	= 0;		/* 1 means swallow floating windows by default */

static const int splitstatus		= 1;		/* 1 for split status items */
static const char *splitdelim		= ";";		/* Character used for separating status */

static const char *fonts[]			= { "monospace:size=10" };
static const char dmenufont[]		= "monospace:size=10";
static const char bg_col[]			= "#282828";
static const char fg_col[]			= "#ebdbb2";
static const char green_col[]		= "#98971a";
static const char red_col[]			= "#fb4934";
static const char *colors[][3]		= {
	/*				 fg		bg		border	*/
	[SchemeNorm] = { fg_col,	bg_col,	bg_col},
	[SchemeSel]  = { bg_col,	fg_col,		red_col},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	*	WM_CLASS(STRING) = instance, class
	*	WM_NAME(STRING) = title
	*/
	/* class		instance	title				tags mask	isfloating	isterminal	noswallow	monitor */
	{ "Gimp",		NULL,		NULL,				0,			1,			0,			0,			-1 },
	{ "Firefox",	NULL,		NULL,				1 << 8,		0,			0,			-1,			-1 },
	{ TERMINAL,		NULL,		NULL,				0,			0,			1,			0,			-1 },
	{ NULL,			NULL,		"Event Tester",		0,			0,			0,			1,			-1 }, /* xev */
};

/* layout(s) */
static const float mfact		= 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster		= 1;    /* number of clients in master area */
static const int resizehints	= 0;    /* 1 means respect size hints in tiled resizals */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
#include <X11/XF86keysym.h>

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",		tile },    /* first entry is default */
	{ "><>",		NULL },    /* no layout function means floating behavior */
	{ "[M]",		monocle },

	{ "H[]",		deck },
	{ "|M|",		centeredmaster },
	{ ">M>",		centeredfloatingmaster },
	{ "###",		gaplessgrid },
	{ "TTT",		bstack},

	/*
	{ "[@]",		spiral },
	{ "[\\]",		dwindle },
	{ "DD",			doubledeck },
	{ "===",		bstackhoriz },
	{ "HHH",		grid },
	{ "###",		nrowgrid },
	{ "---",		horizgrid },
	*/

	{ NULL,			NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,						KEY,		view,			{.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,			KEY,		toggleview,		{.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,				KEY,		tag,			{.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask,	KEY,		toggletag,		{.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, NULL };
static const char *termcmd[]  = { TERMINAL, NULL };
static const char scratchpadname[] = "scratchpad";
static const char *scratchpadcmd[] = { TERMINAL, "-t", scratchpadname, "-g", "120x34", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,						XK_d,		spawn,			{.v = dmenucmd } },
	{ MODKEY,						XK_Return,	spawn,			{.v = termcmd } },
	{ MODKEY|ShiftMask,				XK_Return,	togglescratch,	{.v = scratchpadcmd } },

	{ MODKEY,						XK_b,		togglebar,		{0} },

	{ MODKEY,						XK_i,		focusstack,		{.i = +1 } },
	{ MODKEY,						XK_o,		focusstack,		{.i = -1 } },

	/* Not used as i dislike having multiple master windows. If used the d key will conflict with calling dmenu
	{ MODKEY,						XK_i,		incnmaster,		{.i = +1 } },
	{ MODKEY,						XK_d,		incnmaster,		{.i = -1 } },
	*/

	{ MODKEY|ShiftMask,				XK_i,		setmfact,		{.f = -0.05} },
	{ MODKEY|ShiftMask,				XK_o,		setmfact,		{.f = +0.05} },

	/*
	{ MODKEY|Mod4Mask,				XK_i,		incrigaps,		{.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,	XK_i,		incrigaps,		{.i = -1 } },
	{ MODKEY|Mod4Mask,				XK_o,		incrogaps,		{.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,	XK_o,		incrogaps,		{.i = -1 } },
	{ MODKEY|Mod4Mask,				XK_6,		incrihgaps,		{.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,	XK_6,		incrihgaps,		{.i = -1 } },
	{ MODKEY|Mod4Mask,				XK_7,		incrivgaps,		{.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,	XK_7,		incrivgaps,		{.i = -1 } },
	{ MODKEY|Mod4Mask,				XK_8,		incrohgaps,		{.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,	XK_8,		incrohgaps,		{.i = -1 } },
	{ MODKEY|Mod4Mask,				XK_9,		incrovgaps,		{.i = +1 } },
	{ MODKEY|Mod4Mask|ShiftMask,	XK_9,		incrovgaps,		{.i = -1 } },
	*/

	/* Custom keybinds for vanitygaps */
	{ MODKEY,						XK_a,		togglegaps,		{0} },
	{ MODKEY|ShiftMask,				XK_a,		defaultgaps,	{0} },
	{ MODKEY,						XK_z,		incrgaps,		{.i = +3 } },
	{ MODKEY,						XK_x,		incrgaps,		{.i = -3 } },

	{ MODKEY,						XK_space,	zoom,			{0} },
	{ MODKEY,						XK_Tab,		view,			{0} },

	{ MODKEY,						XK_q,		killclient,		{0} },

	/* Custom keybinds for different layouts */
	{ MODKEY,						XK_t,		setlayout,		{.v = &layouts[0]} },
	{ MODKEY,						XK_f,		setlayout,		{.v = &layouts[1]} },
	{ MODKEY,						XK_m,		setlayout,		{.v = &layouts[2]} },
	{ MODKEY,						XK_r,		setlayout,		{.v = &layouts[3]} },
	{ MODKEY,						XK_c,		setlayout,		{.v = &layouts[4]} },
	{ MODKEY|ShiftMask,				XK_f,		setlayout,		{.v = &layouts[5]} },
	{ MODKEY,						XK_g,		setlayout,		{.v = &layouts[6]} },
	{ MODKEY|ShiftMask,				XK_t,		setlayout,		{.v = &layouts[7]} },
 /* { MODKEY,						XK_space,	setlayout,		{0} }, */


	{ MODKEY|ShiftMask,				XK_space,	togglefloating,	{0} },

	{ MODKEY,						XK_h,		focusmon,		{.i = -1 } },
	{ MODKEY,						XK_l,		focusmon,		{.i = +1 } },
	{ MODKEY|ShiftMask,				XK_h,		tagmon,			{.i = -1 } },
	{ MODKEY|ShiftMask,				XK_l,		tagmon,			{.i = +1 } },

	{ MODKEY,						XK_j,		cycle,			{ .i = -1 } },
	{ MODKEY|ShiftMask,				XK_j,		tagcycle,		{ .i = -1 } },
	{ MODKEY,						XK_k,		cycle,			{ .i = +1 } },
	{ MODKEY|ShiftMask,				XK_k,		tagcycle,		{ .i = +1 } },

	{ MODKEY,						XK_Down,	cycle,		{ .i = -1 } },
	{ MODKEY|ShiftMask,				XK_Down,	tagcycle,	{ .i = -1 } },
	{ MODKEY,						XK_Up,		cycle,		{ .i = +1 } },
	{ MODKEY|ShiftMask,				XK_Up,		tagcycle,	{ .i = +1 } },


	{ MODKEY,						XK_0,		view,		{.ui = ~0 } },
	{ MODKEY|ShiftMask,				XK_0,		tag,		{.ui = ~0 } },

	/* Program specific keybinds */
	{ MODKEY|ControlMask,			XK_k,		spawn,		SHCMD("keepassxc")},
	{ MODKEY|ControlMask,			XK_w,		spawn,		SHCMD("$BROWSER")},
	{ MODKEY|ControlMask,			XK_d,		spawn,		SHCMD("discord")},
	{ MODKEY|ControlMask,			XK_e,		spawn,		SHCMD("teams")},
	{ MODKEY|ControlMask,			XK_m,		spawn,		SHCMD("minecraft-launcher")},
	{ MODKEY|ControlMask,			XK_g,		spawn,		SHCMD("geogebra")},
	{ MODKEY|ControlMask,			XK_v,		spawn,		SHCMD("virtualbox")},

	/* Shortcuts for screenshots */
	/* Take screenshot of currently focused window */
	{ MODKEY,						XK_s,		spawn,		SHCMD("scrot -e 'xclip -selection clipboard -t image/png -i $f' -u $HOME/Screenshots/Focused/%Y-%m-%d_%H-%M-%S.png")},
	/* Take screenshot of the whole screen */
	{ MODKEY|ControlMask,			XK_s,		spawn,		SHCMD("scrot -e 'xclip -selection clipboard -t image/png -i $f' $HOME/Screenshots/Full/%Y-%m-%d_%H-%M-%S.png")},
	/* Freeze screen and allow user to specify selection */
	{ MODKEY|ShiftMask,				XK_s,		spawn,		SHCMD("scrot -e 'xclip -selection clipboard -t image/png -i $f' -s -f $HOME/Screenshots/Selection/%Y-%m-%d_%H-%M-%S.png")},

	/* Shortcuts for turning volume up or down */
	{ MODKEY,						XK_minus,	spawn,		SHCMD("pulsemixer --change-volume -2")},
	{ MODKEY,						XK_plus,	spawn,		SHCMD("pulsemixer --change-volume +2")},
	{ MODKEY|ShiftMask,				XK_m,		spawn,		SHCMD("pulsemixer --toggle-mute")},

	/* Keybinds related to the icons on F keys */
	{ 0,			XF86XK_AudioLowerVolume,		spawn,		SHCMD("pulsemixer --change-volume -2") },
	{ 0,			XF86XK_AudioRaiseVolume,		spawn,		SHCMD("pulsemixer --change-volume +2") },
	{ 0,			XF86XK_AudioMute,				spawn,		SHCMD("pulsemixer --toggle-mute") },
	{ 0,			XF86XK_MonBrightnessUp,			spawn,		SHCMD("backlight_control +2") },
	{ 0, 			XF86XK_MonBrightnessDown,		spawn,		SHCMD("backlight_control -2") },

	/* Keybinds specifically for launching specific urls using firefox */
	{ MODKEY|ControlMask,			XK_KP_Home,		spawn,		SHCMD("firefox music.youtube.com")},
	{ MODKEY|ControlMask,			XK_KP_Up,		spawn,		SHCMD("firefox finance.yahoo.com/quote/GME/")},

	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,				XK_q,			spawn,		SHCMD("sysact") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,			0,				Button1,		setlayout,		{0} },
	{ ClkLtSymbol,			0,				Button3,		setlayout,		{.v = &layouts[2]} },
	{ ClkWinTitle,			0,				Button2,		zoom,			{0} },
	{ ClkStatusText,		0,				Button2,		spawn,			{.v = termcmd } },
	{ ClkClientWin,			MODKEY,			Button1,		movemouse,		{0} },
	{ ClkClientWin,			MODKEY,			Button2,		togglefloating,	{0} },
	{ ClkClientWin,			MODKEY,			Button3,		resizemouse,	{0} },
	{ ClkTagBar,			0,				Button1,		view,			{0} },
	{ ClkTagBar,			0,				Button3,		toggleview,		{0} },
	{ ClkTagBar,			MODKEY,			Button1,		tag,			{0} },
	{ ClkTagBar,			MODKEY,			Button3,		toggletag,		{0} },
};
