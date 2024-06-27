/*
Copyright (C) 1996-1997 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/vt.h>
#include <stdarg.h>
#include <stdio.h>
#include <signal.h>

//#include <X11/cursorfont.h>

//#include "GL/gl.h"
//#include "GL/glx.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "quakedef.h"

#define WARP_WIDTH              320
#define WARP_HEIGHT             200

GLFWwindow* window;

unsigned short d_8to16table[256];
unsigned int d_8to24table[256];
unsigned char d_15to8table[65536];

static qboolean usedga = false;

#define stringify(m) { #m, m }

cvar_t vid_mode = {"vid_mode","0",false};
 
cvar_t  mouse_button_commands[3] =
{
    {"mouse1","+attack"},
    {"mouse2","+strafe"},
    {"mouse3","+forward"},
};

static int mouse_buttons=3;
static int mouse_buttonstate;
static int mouse_oldbuttonstate;
static float mouse_x, mouse_y;
static float p_mouse_x, p_mouse_y;
static float old_mouse_x, old_mouse_y;

cvar_t _windowed_mouse = {"_windowed_mouse", "1", true};
cvar_t m_filter = {"m_filter","0"};
static float old_windowed_mouse;

static int scr_width, scr_height;

#define KEY_MASK (KeyPressMask | KeyReleaseMask)
#define MOUSE_MASK (ButtonPressMask | ButtonReleaseMask | \
		    PointerMotionMask | ButtonMotionMask)

/*-----------------------------------------------------------------------*/

//int		texture_mode = GL_NEAREST;
//int		texture_mode = GL_NEAREST_MIPMAP_NEAREST;
//int		texture_mode = GL_NEAREST_MIPMAP_LINEAR;
int		texture_mode = GL_LINEAR;
//int		texture_mode = GL_LINEAR_MIPMAP_NEAREST;
//int		texture_mode = GL_LINEAR_MIPMAP_LINEAR;

int		texture_extension_number = 1;

float		gldepthmin, gldepthmax;

cvar_t	gl_ztrick = {"gl_ztrick","1"};

const char *gl_vendor;
const char *gl_renderer;
const char *gl_version;
const char *gl_extensions;

qboolean is8bit = false;
qboolean isPermedia = false;
qboolean gl_mtexable = false;

/*-----------------------------------------------------------------------*/
void D_BeginDirectRect (int x, int y, byte *pbitmap, int width, int height)
{
}

void D_EndDirectRect (int x, int y, int width, int height)
{
}

/*
=================
VID_Gamma_f

Keybinding command
=================
*/
void VID_Gamma_f (void)
{
	float	gamma, f, inf;
	unsigned char	palette[768];
	int		i;

	if (Cmd_Argc () == 2)
	{
		gamma = Q_atof (Cmd_Argv(1));

		for (i=0 ; i<768 ; i++)
		{
			f = pow ( (host_basepal[i]+1)/256.0 , gamma );
			inf = f*255 + 0.5;
			if (inf < 0)
				inf = 0;
			if (inf > 255)
				inf = 255;
			palette[i] = inf;
		}

		VID_SetPalette (palette);

		vid.recalc_refdef = 1;				// force a surface cache flush
	}
}

void VID_Shutdown(void)
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwTerminate();
}

int TranslateKey(int key) {
	switch(key) {
		case GLFW_KEY_PAGE_UP: return K_PGUP;
		case GLFW_KEY_PAGE_DOWN: return K_PGDN;
		case GLFW_KEY_HOME: return K_HOME;
		case GLFW_KEY_END: return K_END;
		case GLFW_KEY_LEFT: return K_LEFTARROW;
		case GLFW_KEY_RIGHT: return K_RIGHTARROW;
		case GLFW_KEY_DOWN: return K_DOWNARROW;
		case GLFW_KEY_UP: return K_UPARROW;
		case GLFW_KEY_ESCAPE: return K_ESCAPE;
		case GLFW_KEY_ENTER: return K_ENTER;
		case GLFW_KEY_TAB: return K_TAB;
		case GLFW_KEY_F1: return K_F1;
		case GLFW_KEY_F2: return K_F2;
		case GLFW_KEY_F3: return K_F3;
		case GLFW_KEY_F4: return K_F4;
		case GLFW_KEY_F5: return K_F5;
		case GLFW_KEY_F6: return K_F6;
		case GLFW_KEY_F7: return K_F7;
		case GLFW_KEY_F8: return K_F8;
		case GLFW_KEY_F9: return K_F9;
		case GLFW_KEY_F10: return K_F10;
		case GLFW_KEY_F11: return K_F11;
		case GLFW_KEY_F12: return K_F12;
		case GLFW_KEY_BACKSPACE: return K_BACKSPACE;
		case GLFW_KEY_DELETE: return K_DEL;
		case GLFW_KEY_PAUSE: return K_PAUSE;
		case GLFW_KEY_LEFT_SHIFT:
		case GLFW_KEY_RIGHT_SHIFT: return K_SHIFT;
		case GLFW_KEY_LEFT_CONTROL:
		case GLFW_KEY_RIGHT_CONTROL: return K_CTRL;
		case GLFW_KEY_LEFT_ALT:
		case GLFW_KEY_LEFT_SUPER:
		case GLFW_KEY_RIGHT_ALT:
		case GLFW_KEY_RIGHT_SUPER: return K_ALT;
		case GLFW_KEY_INSERT: return K_INS;
		case GLFW_KEY_KP_MULTIPLY: return '*';
		case GLFW_KEY_KP_ADD: return '+';
		case GLFW_KEY_MINUS:			/* FALLTHROUGH */
		case GLFW_KEY_KP_SUBTRACT: return '-';
		case GLFW_KEY_KP_DIVIDE: return '/';
		case GLFW_KEY_SPACE: return K_SPACE;
		case GLFW_KEY_EQUAL:			/* FALLTHROUGH */
		case GLFW_KEY_BACKSLASH:
		case GLFW_KEY_LEFT_BRACKET:
		case GLFW_KEY_RIGHT_BRACKET:
		case GLFW_KEY_APOSTROPHE:
		case GLFW_KEY_SEMICOLON:
		case GLFW_KEY_SLASH:
		case GLFW_KEY_PERIOD:
		case GLFW_KEY_COMMA:
			return key;

		default:
			if((key >= 65 && key <= 90) || (key >= 48 && key <= 57)) {
				return key;
			} else {
				return 0;
			}
	}
}


struct
{
	int key;
	int down;
} keyq[64];
int keyq_head=0;
int keyq_tail=0;

int config_notify=0;
int config_notify_width;
int config_notify_height;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	/*
	 * TODO - Handle Repeating Key presses!
	 */
	if(action == GLFW_PRESS) {
		keyq[keyq_head].key = TranslateKey(key);
		keyq[keyq_head].down = true;
		keyq_head = (keyq_head + 1) & 63;
		Key_Event(TranslateKey(key), true);
	} else if(action == GLFW_RELEASE) {
		keyq[keyq_head].key = TranslateKey(key);
		keyq[keyq_head].down = false;
		keyq_head = (keyq_head + 1) & 63;
		Key_Event(TranslateKey(key), false);
	}
}

static double old_xpos = 0;
static double old_ypos = 0;
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	//int xchange = floor(xpos) - old_xpos;
	//int ychange = floor(old_ypos) - ypos;
	//mouse_x = floor(xpos) - old_xpos;
	//mouse_y = floor(old_ypos) - ypos;

	//mouse_x = (floor(xpos) - old_xpos) * sensitivity.value;
	//mouse_y = (floor(old_ypos) - ypos) * sensitivity.value;
	mouse_x = (xpos - old_xpos);
	mouse_y = (old_ypos - ypos);

	//old_xpos = floor(xpos);
	//old_ypos = floor(ypos);
	old_xpos = xpos;
	old_ypos = ypos;
	//
	//int xchange = floor(old_xpos) - xpos;
	//int ychange = floor(old_ypos) - ypos;
	//int xchange = floor(xpos) - old_xpos;
	//int ychange = floor(ypos) - old_ypos;
	//int xchange = floor(xpos);
	//int ychange = floor(ypos);
	
	/*
	if(usedga) {
		mouse_x += xchange;
		mouse_y += ychange;
	} else if(_windowed_mouse.value) {
		printf("test\n");
		mouse_x += (float) ((int)xchange - (int)(scr_width / 2));
		mouse_y += (float) ((int)ychange - (int)(scr_height / 2));

		//Recapture mouse...
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	} else {
		mouse_x = (float) (xchange - p_mouse_x);
		mouse_y = (float) (ychange - p_mouse_y);
		p_mouse_x = xchange;
		p_mouse_y = ychange;
	}

	if(old_windowed_mouse != _windowed_mouse.value) {
		old_windowed_mouse = _windowed_mouse.value;
		if(!_windowed_mouse.value) {
			Con_Printf("Releasing Mouse...\n");
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		} else {
			Con_Printf("Capturing Mouse...\n");
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}
	old_xpos = floor(xpos);
	old_ypos = floor(ypos);
	*/
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	int b;
	if(action == GLFW_PRESS) {
		if(button == GLFW_MOUSE_BUTTON_LEFT) {
			b = 0;
		} else if(button == GLFW_MOUSE_BUTTON_MIDDLE) {
			b = 2;
		} else if(button == GLFW_MOUSE_BUTTON_RIGHT) {
			b = 1;
		}
		if(b >= 0) {
			//mouse_buttonstate |= 1 << b;
			Key_Event(K_MOUSE1 + b, false);
		}
	} else if(action == GLFW_RELEASE) {
		if(button == GLFW_MOUSE_BUTTON_LEFT) {
			b = 0;
		} else if(button == GLFW_MOUSE_BUTTON_MIDDLE) {
			b = 2;
		} else if(button == GLFW_MOUSE_BUTTON_RIGHT) {
			b = 1;
		}
		if(b >= 0) {
			//mouse_buttonstate &= ~(1 << b);
			Key_Event(K_MOUSE1 + b, false);
		}
	}
}

void window_size_callback(GLFWwindow* window, int width, int height) {
	scr_width = width;
	scr_height = height;
	glViewport(0, 0, width, height);
	Con_Printf("New Window Size is %dx%d\n",width, height);
}

void signal_handler(int sig)
{
	printf("Received signal %d, exiting...\n", sig);
	VID_Shutdown();
	exit(0);
}

void InitSig(void)
{
	signal(SIGHUP, signal_handler);
	signal(SIGQUIT, signal_handler);
	signal(SIGILL, signal_handler);
	signal(SIGTRAP, signal_handler);
	signal(SIGIOT, signal_handler);
	signal(SIGBUS, signal_handler);
	signal(SIGFPE, signal_handler);
	signal(SIGSEGV, signal_handler);
	signal(SIGTERM, signal_handler);
}

void VID_ShiftPalette(unsigned char *p)
{
	VID_SetPalette(p);
}

void	VID_SetPalette (unsigned char *palette)
{
	byte	*pal;
	unsigned short r,g,b;
	int     v;
	int     r1,g1,b1;
	int		k;
	unsigned short i;
	unsigned	*table;
	FILE *f;
	char s[255];
	float dist, bestdist;
	static qboolean palflag = false;

//
// 8 8 8 encoding
//
	pal = palette;
	table = d_8to24table;
	for (i=0 ; i<256 ; i++)
	{
		r = pal[0];
		g = pal[1];
		b = pal[2];
		pal += 3;
		
//		v = (255<<24) + (r<<16) + (g<<8) + (b<<0);
//		v = (255<<0) + (r<<8) + (g<<16) + (b<<24);
		v = (255<<24) + (r<<0) + (g<<8) + (b<<16);
		*table++ = v;
	}
	d_8to24table[255] &= 0xffffff;	// 255 is transparent

	// JACK: 3D distance calcs - k is last closest, l is the distance.
	// FIXME: Precalculate this and cache to disk.
	if (palflag)
		return;
	palflag = true;

	COM_FOpenFile("glquake/15to8.pal", &f);
	if (f) {
		fread(d_15to8table, 1<<15, 1, f);
		fclose(f);
	} else {
		for (i=0; i < (1<<15); i++) {
			/* Maps
 			000000000000000
 			000000000011111 = Red  = 0x1F
 			000001111100000 = Blue = 0x03E0
 			111110000000000 = Grn  = 0x7C00
 			*/
 			r = ((i & 0x1F) << 3)+4;
 			g = ((i & 0x03E0) >> 2)+4;
 			b = ((i & 0x7C00) >> 7)+4;
			pal = (unsigned char *)d_8to24table;
			for (v=0,k=0,bestdist=10000.0; v<256; v++,pal+=4) {
 				r1 = (int)r - (int)pal[0];
 				g1 = (int)g - (int)pal[1];
 				b1 = (int)b - (int)pal[2];
				dist = sqrt(((r1*r1)+(g1*g1)+(b1*b1)));
				if (dist < bestdist) {
					k=v;
					bestdist = dist;
				}
			}
			d_15to8table[i]=k;
		}
		sprintf(s, "%s/glquake", com_gamedir);
 		Sys_mkdir (s);
		sprintf(s, "%s/glquake/15to8.pal", com_gamedir);
		if ((f = fopen(s, "wb")) != NULL) {
			fwrite(d_15to8table, 1<<15, 1, f);
			fclose(f);
		}
	}
}

/*
===============
GL_Init
===============
*/
void GL_Init (void)
{
	gl_vendor = glGetString (GL_VENDOR);
	Con_Printf ("GL_VENDOR: %s\n", gl_vendor);
	gl_renderer = glGetString (GL_RENDERER);
	Con_Printf ("GL_RENDERER: %s\n", gl_renderer);

	gl_version = glGetString (GL_VERSION);
	Con_Printf ("GL_VERSION: %s\n", gl_version);
	gl_extensions = glGetString (GL_EXTENSIONS);
	//Con_Printf ("GL_EXTENSIONS: %s\n", gl_extensions);

	Con_Printf ("%s %s\n", gl_renderer, gl_version);

	glClearColor (1,0,0,0);
	glCullFace(GL_FRONT);
	glEnable(GL_TEXTURE_2D);

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.666);

	glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
	glShadeModel (GL_FLAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

/*
=================
GL_BeginRendering

=================
*/
void GL_BeginRendering (int *x, int *y, int *width, int *height)
{
	extern cvar_t gl_clear;

	*x = *y = 0;
	*width = scr_width;
	*height = scr_height;

//    if (!wglMakeCurrent( maindc, baseRC ))
//		Sys_Error ("wglMakeCurrent failed");

	glViewport (*x, *y, *width, *height);
}


void GL_EndRendering (void)
{
	glFlush();
	glfwSwapBuffers(window);
}

qboolean VID_Is8bit(void)
{
	return is8bit;
}

#ifdef GL_EXT_SHARED
void VID_Init8bitPalette() 
{
	// Check for 8bit Extensions and initialize them.
	int i;
	char thePalette[256*3];
	char *oldPalette, *newPalette;

	if (strstr(gl_extensions, "GL_EXT_shared_texture_palette") == NULL)
		return;

	Con_SafePrintf("8-bit GL extensions enabled.\n");
	glEnable( GL_SHARED_TEXTURE_PALETTE_EXT );
	oldPalette = (char *) d_8to24table; //d_8to24table3dfx;
	newPalette = thePalette;
	for (i=0;i<256;i++) {
		*newPalette++ = *oldPalette++;
		*newPalette++ = *oldPalette++;
		*newPalette++ = *oldPalette++;
		oldPalette++;
	}
	glColorTableEXT(GL_SHARED_TEXTURE_PALETTE_EXT, GL_RGB, 256, GL_RGB, GL_UNSIGNED_BYTE, (void *) thePalette);
	is8bit = true;
}

#else
extern void gl3DfxSetPaletteEXT(GLuint *pal);

void VID_Init8bitPalette(void) 
{
	// Check for 8bit Extensions and initialize them.
	int i;
	GLubyte table[256][4];
	char *oldpal;

	if (strstr(gl_extensions, "3DFX_set_global_palette") == NULL)
		return;

	Con_SafePrintf("8-bit GL extensions enabled.\n");
	glEnable( GL_SHARED_TEXTURE_PALETTE_EXT );
	oldpal = (char *) d_8to24table; //d_8to24table3dfx;
	for (i=0;i<256;i++) {
		table[i][2] = *oldpal++;
		table[i][1] = *oldpal++;
		table[i][0] = *oldpal++;
		table[i][3] = 255;
		oldpal++;
	}
//	gl3DfxSetPaletteEXT((GLuint *)table);
	is8bit = true;
}
#endif

void VID_Init(unsigned char *palette)
{
	int i;
	char	gldir[MAX_OSPATH];
	int width = 640, height = 480;
	int scrnum;
	unsigned long mask;

	S_Init();

	Cvar_RegisterVariable (&vid_mode);
	Cvar_RegisterVariable (&gl_ztrick);
	
	vid.maxwarpwidth = WARP_WIDTH;
	vid.maxwarpheight = WARP_HEIGHT;
	vid.colormap = host_colormap;
	vid.fullbright = 256 - LittleLong (*((int *)vid.colormap + 2048));

	if(!glfwInit()) {
		fprintf(stderr, "Failed to initialise GLFW!\n");
		exit(1);
	}

// interpret command-line params

// set vid parameters

	if ((i = COM_CheckParm("-width")) != 0)
		width = atoi(com_argv[i+1]);
	if ((i = COM_CheckParm("-height")) != 0)
		height = atoi(com_argv[i+1]);

	if ((i = COM_CheckParm("-conwidth")) != 0)
		vid.conwidth = Q_atoi(com_argv[i+1]);
	else
		vid.conwidth = 640;

	vid.conwidth &= 0xfff8; // make it a multiple of eight

	if (vid.conwidth < 320)
		vid.conwidth = 320;

	// pick a conheight that matches with correct aspect
	vid.conheight = vid.conwidth*3 / 4;

	if ((i = COM_CheckParm("-conheight")) != 0)
		vid.conheight = Q_atoi(com_argv[i+1]);
	if (vid.conheight < 200)
		vid.conheight = 200;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	if(!(window = glfwCreateWindow(width, height, "EisenSrc", NULL, NULL))) {
		fprintf(stderr, "Could not open a window!\n");
		glfwTerminate();
		exit(1);
	}

	if(glfwRawMouseMotionSupported()) {
		//glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	} else {
		fprintf(stderr, "Warning: Raw Mouse Motion is unsupported!\n");
	}

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	old_xpos = (scr_width / 2);
	old_ypos = (scr_width / 2);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);


	/* window attributes */
	glClear(GL_COLOR_BUFFER_BIT);

	if (COM_CheckParm("-window")) {
		putenv("MESA_GLX_FX=window");
	} else {
		putenv("MESA_GLX_FX=fullscreen");
	}

	glfwMakeContextCurrent(window);

	scr_width = width;
	scr_height = height;

	if (vid.conheight > height)
		vid.conheight = height;
	if (vid.conwidth > width)
		vid.conwidth = width;
	vid.width = vid.conwidth;
	vid.height = vid.conheight;

	vid.aspect = ((float)vid.height / (float)vid.width) *
				(320.0 / 240.0);
	vid.numpages = 2;

	InitSig(); // trap evil signals

	GL_Init();

	sprintf (gldir, "%s/glquake", com_gamedir);
	Sys_mkdir (gldir);

	VID_SetPalette(palette);

	// Check for 3DFX Extensions and initialize them.
	VID_Init8bitPalette();

	Con_SafePrintf ("Video mode %dx%d initialized.\n", width, height);

	vid.recalc_refdef = 1;				// force a surface cache flush
	
	// Capture Mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Sys_SendKeyEvents(void)
{
	if(window) {
		glfwPollEvents();
		while (keyq_head != keyq_tail)
		{
			Key_Event(keyq[keyq_tail].key, keyq[keyq_tail].down);
			keyq_tail = (keyq_tail + 1) & 63;
		}
	}
}

void Force_CenterView_f (void)
{
	cl.viewangles[PITCH] = 0;
}


void IN_Init(void)
{
	Cvar_RegisterVariable (&_windowed_mouse);
	Cvar_RegisterVariable (&m_filter);
	Cvar_RegisterVariable (&mouse_button_commands[0]);
	Cvar_RegisterVariable (&mouse_button_commands[1]);
	Cvar_RegisterVariable (&mouse_button_commands[2]);
	Cmd_AddCommand ("force_centerview", Force_CenterView_f);
}

void IN_Shutdown(void)
{
}

/*
===========
IN_Commands
===========
*/
void IN_Commands (void)
{
	int i;

	for (i=0 ; i<mouse_buttons ; i++) {
		if ( (mouse_buttonstate & (1<<i)) && !(mouse_oldbuttonstate & (1<<i)) )
			Key_Event (K_MOUSE1 + i, true);

		if ( !(mouse_buttonstate & (1<<i)) && (mouse_oldbuttonstate & (1<<i)) )
			Key_Event (K_MOUSE1 + i, false);
	}

	mouse_oldbuttonstate = mouse_buttonstate;
}

/*
===========
IN_Move
===========
*/
void IN_Move (usercmd_t *cmd)
{
	if (m_filter.value) {
		mouse_x = (mouse_x + old_mouse_x) * 0.5;
		mouse_y = (mouse_y + old_mouse_y) * 0.5;
	}

	old_mouse_x = mouse_x;
	old_mouse_y = mouse_y;
   
	mouse_x *= sensitivity.value;
	mouse_y *= sensitivity.value;
   
	if ( (in_strafe.state & 1) || (lookstrafe.value && (in_mlook.state & 1) )) {
		cmd->sidemove += m_side.value * mouse_x;
	} else {
		cl.viewangles[YAW] -= m_yaw.value * mouse_x;
	}
	if (in_mlook.state & 1)
		V_StopPitchDrift ();
   
	if ( (in_mlook.state & 1) && !(in_strafe.state & 1)) {
		cl.viewangles[PITCH] += m_pitch.value * mouse_y;
		if (cl.viewangles[PITCH] > 80)
			cl.viewangles[PITCH] = 80;
		if (cl.viewangles[PITCH] < -70)
			cl.viewangles[PITCH] = -70;
	} else {
		if ((in_strafe.state & 1) && noclip_anglehack)
			cmd->upmove -= m_forward.value * mouse_y;
		else
			cmd->forwardmove -= m_forward.value * mouse_y;
	}
	mouse_x = mouse_y = 0.0;
}


//void	VID_LockBuffer (void) {}
//void	VID_UnlockBuffer (void) {}

