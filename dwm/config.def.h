#include <X11/XF86keysym.h>
#include "fibonacci.c"

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int gappx     = 2;        /* gaps size between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "monospace:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005443";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class		    instance	title		tags mask	isfloating	monitor */

	/* { "Gimp",		NULL,		NULL,		0,		    1,		    -1 }, */
	{ "Firefox",		NULL,		NULL,		1 << 8,		0,		    -1 },
	{ "Telegram",		NULL,		NULL,		1 << 7,		0,		    -1 },
	{ "thunderbird",	NULL,		NULL,		1 << 8,		0,		    -1 },
	{ "discord",	    NULL,		NULL,		1 << 2,		0,		    -1 },

};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */


static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
 	{ "[@]",      spiral },
 	{ "[\\]",     dwindle },
};

/* Screenshot */
static const char *screenshot[] = {"scrot", "-Z", "0", "-q", "100", "/home/davign/Pictures/%Y-%m-%d_%T-screenshot.png"};
static const char *screenshotsel[] = {"scrot", "-s", "-Z", "0", "-q", "100", "/home/davign/Pictures/%Y-%m-%d_%T-screenshot.png"};

/* Volume Keys */
static const char *upvol[] ={ "volumectl", "up", "5", NULL };
static const char *downvol[] ={ "volumectl", "down", "5", NULL };
static const char *mutevol[] ={ "volumectl", "toggle", NULL };

static const char *downmic[] = { "pactl", "set-source-volume", "@DEFAULT_SOURCE@", "-5%", NULL};
static const char *upmic[] = { "pactl", "set-source-volume", "@DEFAULT_SOURCE@", "+5%", NULL};
static const char *mutemic[] = { "pactl", "set-source-mute", "@DEFAULT_SOURCE@", "toggle", NULL };

/* Backlight */
static const char *brightnessup[] = { "brightnessctl", "set", "+5%", NULL };
static const char *brightnessdown[] = { "brightnessctl", "set", "5%-", NULL };

/* Applications */
static const char *lock[] = { "xlock", "-mode", "worm", NULL};
static const char *calc[] = { "galculator", NULL};
static const char *okular[] = { "okular", NULL};
static const char *xcolor[] = { "xcolor", "-s", NULL};
static const char *mousepad[] = { "mousepad", NULL};
static const char *vscode[] = { "code", NULL};

/* Player Controls */
static const char *playerctlP[] = { "playerctl", "play-pause", NULL};
static const char *playerctls[] = { "playerctl", "stop", NULL};
static const char *playerctln[] = { "playerctl", "next", NULL};
static const char *playerctlp[] = { "playerctl", "previous", NULL};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },
#define CONTROL ControlMask
/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };
static const char *filebrws[] = { "pcmanfm", NULL };

static Key keys[] = {
	/* modifier                     key  			                function                argument */
	{ MODKEY,                       XK_space,  		                spawn,                  {.v = dmenucmd } },
	{ MODKEY,             		    XK_Return, 		                spawn,                  {.v = termcmd } },
	{ MODKEY,                       XK_b,     		                togglebar,              {0} },
	{ MODKEY,                       XK_j,      		                focusstack,             {.i = +1 } },
	{ MODKEY,                       XK_k,      		                focusstack,             {.i = -1 } },
	{ MODKEY,                       XK_i,      		                incnmaster,             {.i = +1 } },
	{ MODKEY,                       XK_d,      		                incnmaster,             {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_h,     		                setmfact,               {.f = -0.05} },
	{ MODKEY|ShiftMask,             XK_l,      		                setmfact,               {.f = +0.05} },
	{ MODKEY,                       XK_BackSpace, 	                zoom,		            {0} },
	{ MODKEY,                       XK_Tab,    		                view,                   {0} },
	{ MODKEY|ShiftMask,    	      	XK_c,			                killclient,             {0} },
	{ MODKEY,                       XK_t,      		                setlayout,              {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      		                setlayout,              {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      		                setlayout,              {.v = &layouts[2]} },
	{ MODKEY,                       XK_r,      		                setlayout,              {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,             XK_r,     		                setlayout,              {.v = &layouts[4]} },
	{ MODKEY,                       XK_a,      		                setlayout,              {0} },
	{ MODKEY|ShiftMask,             XK_space,  		                togglefloating,         {0} },
	{ MODKEY,                       XK_0,      		                view,                   {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      		                tag,                    {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  		                focusmon,               {.i = -1 } },
	{ MODKEY,                       XK_period, 		                focusmon,               {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  		                tagmon,                 {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, 		                tagmon,                 {.i = +1 } },
	{ MODKEY,                       XK_minus,  		                setgaps,                {.i = -1 } },
	{ ControlMask|Mod1Mask,         XK_Right,	                    viewnext,               {0} },
	{ ControlMask|Mod1Mask,         XK_Left,	                    viewprev,               {0} },
	{ ControlMask|Mod1Mask,         XK_End,		                    tagtonext,              {0} },
	{ ControlMask|Mod1Mask,         XK_Home,	                    tagtoprev,              {0} },
	{ MODKEY,                       XK_equal,  		                setgaps,                {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,  		                setgaps,                {.i = 0  } },
	TAGKEYS(                        XK_1,                                   		        0)
	TAGKEYS(                        XK_2,                                      		        1)
	TAGKEYS(                        XK_3,                                      		        2)
	TAGKEYS(                        XK_4,                                      		        3)
	TAGKEYS(                        XK_5,                                      		        4)
	TAGKEYS(                        XK_6,                                      		        5)
	TAGKEYS(                        XK_7,                                      		        6)
	TAGKEYS(                        XK_8,                                      		        7)
	TAGKEYS(                        XK_9,                                      		        8)
	{ MODKEY|ShiftMask,             XK_q,    		                quit,                   {0}},
	{ MODKEY,			            XK_e,			                spawn,		            {.v = filebrws}},
	{ 0, 				            XK_Print, 		                spawn,	   	            {.v = screenshot}},
	{ ShiftMask, 			        XK_Print, 		                spawn,	   	            {.v = screenshotsel}},
	{ 0,  			                XF86XK_Calculator,	            spawn,		            {.v = calc}},
    // Volume Keys
	{ 0,  		                    XF86XK_AudioLowerVolume,        spawn,		            {.v = downvol}},
	{ MODKEY,  		                XK_bracketright,                spawn,		            {.v = downvol}},
	{ 0,  		                    XF86XK_AudioMute, 	            spawn,		            {.v = mutevol}},
	{ MODKEY,  		                XK_slash,        	            spawn,		            {.v = mutevol}},
	{ 0, 				            XF86XK_AudioRaiseVolume,        spawn, 		            {.v = upvol}},
	{ MODKEY, 				        XK_bracketleft,                 spawn, 		            {.v = upvol}},
	{ CONTROL, 			            XF86XK_AudioRaiseVolume,        spawn, 		            {.v = upmic}},
	{ MODKEY|ShiftMask, 			XK_bracketleft,                 spawn, 		            {.v = upmic}},
	{ CONTROL,		                XF86XK_AudioLowerVolume,        spawn,		            {.v = downmic}},
	{ MODKEY|ShiftMask,		        XK_bracketright,                spawn,		            {.v = downmic}},
	{ CONTROL,	 		            XF86XK_AudioMute, 	            spawn,		            {.v = mutemic}},
	{ 0,   			                XF86XK_AudioMicMute, 	        spawn,		            {.v = mutemic}},
    { MODKEY|ShiftMask,             XK_slash,                       spawn,                  {.v = mutemic}},
    // PlayerCtl
	{ 0,  			                XF86XK_AudioPlay,	            spawn,		            {.v = playerctlP}},
	{ MODKEY,  			            XK_p,           	            spawn,		            {.v = playerctlP}},
	{ 0,  			                XF86XK_AudioStop,	            spawn,		            {.v = playerctls}},
	{ 0,  			                XF86XK_AudioPrev,	            spawn,		            {.v = playerctlp}},
	{ MODKEY|ShiftMask,  			XK_n,           	            spawn,		            {.v = playerctlp}},
	{ 0,			                XF86XK_AudioNext,	            spawn,		            {.v = playerctln}},
	{ MODKEY,	        		    XK_n,	                        spawn,		            {.v = playerctln}},

	{ MODKEY, 			            XK_w,			                spawn, 		            {.v = brightnessup}},
	{ 0, 				            XF86XK_MonBrightnessUp,         spawn, 		            {.v = brightnessup}},
	{ MODKEY, 			            XK_s,			                spawn,		            {.v = brightnessdown}},
	{ 0, 				            XF86XK_MonBrightnessDown,       spawn,		            {.v = brightnessdown}},
	{ MODKEY,			            XK_o,			                spawn,		            {.v = okular}},
	{ MODKEY,			            XK_l,			                spawn,		            {.v = lock}},
	{ MODKEY,			            XK_c,			                spawn,		            {.v = xcolor}},
	{ MODKEY|ShiftMask,	            XK_m,			                spawn,		            {.v = mousepad}},
	{ MODKEY,			            XK_v,			                spawn,		            {.v = vscode}},
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
