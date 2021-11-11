/*
Copyright (C) 1994-1995 Apogee Software, Ltd.

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
#include <stdio.h>
#include <string.h>

#include "rt_main.h"
#include "rt_def.h"
#include "rt_in.h"
// #include "_rt_in.h"
#include "isr.h"
#include "keyb.h"
#include "rt_util.h"
#include "rt_vh_a.h"
#include "rt_cfg.h"
#include "rt_msg.h"
#include "rt_playr.h"
#include "rt_net.h"
#include "rt_com.h"
#include "rt_cfg.h"

#include "gfxdrv_keys.h"


#define MAXMESSAGELENGTH		(COM_MAXTEXTSTRINGLENGTH-1)

//****************************************************************************
//
// GLOBALS
//
//****************************************************************************]

//
// Used by menu routines that need to wait for a button release.
// Sometimes the mouse driver misses an interrupt, so you can't wait for
// a button to be released.  Instead, you must ignore any buttons that
// are pressed.
//
int IgnoreMouse = 0;

// configuration variables
//
// boolean  SpaceBallPresent;
// boolean  CybermanPresent;
// boolean  AssassinPresent;
boolean  MousePresent;
boolean  JoysPresent[MaxJoys];
boolean  JoyPadPresent	= 0;

volatile boolean PausePressed = 0;
volatile boolean PanicPressed = 0;

//	Global variables
//
boolean  Paused;
char LastASCII;
volatile int LastScan;

static volatile boolean ExtendedKeyFlag;
static volatile boolean KeyUpFlag;
static volatile int pausecount=0;
static int LEDs;
static volatile int KBFlags;

byte Joy_xb,
	Joy_yb,
	Joy_xs,
	Joy_ys;
word Joy_x,
	Joy_y;


int LastLetter = 0;
char LetterQueue[MAXLETTERS];
ModemMessage MSG;

//	'q','w','e','r','t','y','u','i','o','p','[',']','\\', 0 ,'a','s',

char ScanChars[128] =	// Scan code names with single chars
{
	0 , 0 ,'1','2','3','4','5','6','7','8','9','0','-','=', 0 , 0 ,
	'q','w','e','r','t','y','u','i','o','p','[',']', 0 , 0 ,'a','s',
	'd','f','g','h','j','k','l',';','\'','`', 0 ,'\\','z','x','c','v',
	'b','n','m',',','.','/', 0 , 0 , 0 ,' ', 0 , 0 , 0 , 0 , 0 , 0 ,
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,'-', 0 ,'5', 0 ,'+', 0 ,
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0
};

char ShiftedScanChars[128] =	// Shifted Scan code names with single chars
{
	0 , 0 ,'!','@','#','$','%','^','&','*','(',')','_','+', 0 , 0 ,
	'Q','W','E','R','T','Y','U','I','O','P','{','}', 0 , 0 ,'A','S',
	'D','F','G','H','J','K','L',':','"','~', 0 ,'|','Z','X','C','V',
	'B','N','M','<','>','?', 0 , 0 , 0 ,' ', 0 , 0 , 0 , 0 , 0 , 0 ,
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,'-', 0 ,'5', 0 ,'+', 0 ,
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 ,
	0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0
};


volatile int Keyboard[MAXKEYBOARDSCAN];
volatile int KeyboardQueue[KEYQMAX];
volatile int Keystate[MAXKEYBOARDSCAN];
volatile int Keyhead;
volatile int Keytail;

int KeyboardStarted=false;


void I_StartupKeyboard (void)
{
	if (KeyboardStarted==true)
		return;
	KeyboardStarted=true;

	Keyhead = Keytail = 0;
	memset((void *)Keystate,0,sizeof(Keystate));
	if (!quiet)
		printf("I_StartupKeyboard: Keyboard Started\n");
}

#if 1
void I_AddScanByteKeyboard (int scan)
{
	int k;
	int temp;
	int keyon;
	int strippedkey;

	k = scan;

#if 0
	if ( KBFlags & kb_pr_led )
	{
		if ( k == kb_resend )
		{
			// Handle resend
			KBFlags |= kb_fe;
		}
		else if (k == kb_ack)
		{
			// Handle ack
			KBFlags |= kb_fa;
		}
	}
	else
#endif
	if ( pausecount )
	{
		pausecount--;
	}
	else if ( k == 0xe1 )			// Handle Pause key
	{
		PausePressed = true;
		pausecount = 5;
	}
	else if ( k == 0x46 )			// Handle Panic key (Scroll Lock)
	{
		PanicPressed = true;
	}
	else
	{
		if ( k == 0xF0 )
		{
			KeyUpFlag = true;
		}
		else
			if ( k == 0xE0 )
		{
			ExtendedKeyFlag = true;
		}
		else
		{
			keyon = k & 0x80;
			strippedkey = k & 0x7f;

			if ( ExtendedKeyFlag )
			{
				if ( ( strippedkey == sc_LShift ) ||
					( strippedkey == sc_RShift ) )
				{
					k = sc_None;
				}
/*
				else
				{
					if ( strippedkey == sc_Alt )
					{
						k = sc_RightAlt | keyon;
					}
					if ( strippedkey == sc_Control )
					{
						k = sc_RightCtrl | keyon;
					}
				}
*/
			}

			if ( k != sc_None )
			{
				if ( strippedkey == sc_LShift )
				{
					k = sc_RShift | keyon;
				}

				if ( !keyon )
//				if (!KeyUpFlag)		// Up event
				{
					LastScan = k;
				}

				if (k & 0x80)		// Up event
//				if (KeyUpFlag)		// Up event
				{
					Keystate[k&0x7f]=0;
				}
				else					// Down event
				{
					Keystate[k]=1;
				}

//				if(KeyUpFlag)
//					k|=0x80;

				KeyboardQueue[ Keytail ] = k;
				Keytail = ( Keytail + 1 )&( KEYQMAX - 1 );
			}

			ExtendedKeyFlag = false;
			KeyUpFlag = false;
		}
	}

}
#endif

#if 0
void I_AddScanByteKeyboard (int scan)
{
	KeyboardQueue[ Keytail ] = scan;
	Keytail = ( Keytail + 1 )&( KEYQMAX - 1 );
}
#endif

#if 0
static const short ps2_key2scan[256]={
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,		//00..07
	0x066, 0x00D, 0x000, 0x000, 0x000, 0x05A, 0x000, 0x000,		//08..0F
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,		//10..17
	0x000, 0x000, 0x000, 0x076, 0x000, 0x000, 0x000, 0x000,		//18..1F
	0x029, 0x016, 0x052, 0x026, 0x025, 0x02E, 0x03D, 0x052,		//20..27
	0x046, 0x045, 0x03E, 0x055, 0x041, 0x04E, 0x049, 0x04A,		//28..2F
	0x045, 0x016, 0x01E, 0x026, 0x025, 0x02E, 0x036, 0x03D,		//30..37
	0x03E, 0x046, 0x04C, 0x04C, 0x041, 0x055, 0x049, 0x04A,		//38..3F
	0x01E, 0x01C, 0x032, 0x021, 0x023, 0x024, 0x02B, 0x034,		//40..47
	0x033, 0x043, 0x03B, 0x042, 0x04B, 0x03A, 0x031, 0x044,		//48..4F
	0x04D, 0x015, 0x02D, 0x01B, 0x02C, 0x03C, 0x02A, 0x01D,		//50..57
	0x022, 0x035, 0x01A, 0x054, 0x05D, 0x05B, 0x036, 0x04E,		//58..5F
	0x00E, 0x01C, 0x032, 0x021, 0x023, 0x024, 0x02B, 0x034,		//60..67
	0x033, 0x043, 0x03B, 0x042, 0x04B, 0x03A, 0x031, 0x044,		//68..6F
	0x04D, 0x015, 0x02D, 0x01B, 0x02C, 0x03C, 0x02A, 0x01D,		//70..77
	0x022, 0x035, 0x01A, 0x054, 0x05D, 0x05B, 0x00E, 0x171,		//78..7F
	0x175, 0x172, 0x16B, 0x174, 0x011, 0x014, 0x012, 0x005,		//80..87
	0x006, 0x004, 0x00C, 0x003, 0x00B, 0x083, 0x00A, 0x001,		//88..8F
	0x009, 0x078, 0x007, 0x170, 0x000, 0x17A, 0x17D, 0x16C,		//90..97
	0x169, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,		//98..9F
	0x058, 0x07E, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,		//A0..A7
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,		//A8..AF
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,		//B0..B7
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,		//B8..BF
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,		//C0..C7
	0x000, 0x000, 0x000, 0x000, 0x079, 0x07B, 0x07C, 0x14A,		//C8..CF
	0x070, 0x069, 0x072, 0x07A, 0x06B, 0x073, 0x074, 0x06C,		//D0..D7
	0x075, 0x07D, 0x15A, 0x071, 0x000, 0x000, 0x000, 0x000,		//D8..DF
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,		//E0..E7
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,		//E8..EF
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,		//F0..F7
	0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,		//F8..FF
};
#endif

static short ps2_key2scan[256];
boolean ps2_key2scan_init = 0;

int Key_Event(int key, int down)
{
	int sc;
	int i, j, k;
	
	if(!ps2_key2scan_init)
	{
		for(i=0; i<128; i++)
		{
			j=ASCIINames[i];
			if(j)	ps2_key2scan[j]=i;
			j=ShiftNames[i];
			if(j)	ps2_key2scan[j]=i;
		}
	
		ps2_key2scan['0']=sc_0;
		ps2_key2scan['1']=sc_1;
		ps2_key2scan['2']=sc_2;
		ps2_key2scan['3']=sc_3;
		ps2_key2scan['4']=sc_4;
		ps2_key2scan['5']=sc_5;
		ps2_key2scan['6']=sc_6;
		ps2_key2scan['7']=sc_7;
		ps2_key2scan['8']=sc_8;
		ps2_key2scan['9']=sc_9;
		ps2_key2scan['-']=sc_Minus;
		ps2_key2scan['_']=sc_Minus;
		ps2_key2scan['=']=sc_Equals;
		ps2_key2scan['+']=sc_Plus;

		ps2_key2scan['A']=sc_A;		ps2_key2scan['B']=sc_B;
		ps2_key2scan['C']=sc_C;		ps2_key2scan['D']=sc_D;
		ps2_key2scan['E']=sc_E;		ps2_key2scan['F']=sc_F;
		ps2_key2scan['G']=sc_G;		ps2_key2scan['H']=sc_H;
		ps2_key2scan['I']=sc_I;		ps2_key2scan['J']=sc_J;
		ps2_key2scan['K']=sc_K;		ps2_key2scan['L']=sc_L;
		ps2_key2scan['M']=sc_M;		ps2_key2scan['N']=sc_N;
		ps2_key2scan['O']=sc_O;		ps2_key2scan['P']=sc_P;
		ps2_key2scan['Q']=sc_Q;		ps2_key2scan['R']=sc_R;
		ps2_key2scan['S']=sc_S;		ps2_key2scan['T']=sc_T;
		ps2_key2scan['U']=sc_U;		ps2_key2scan['V']=sc_V;
		ps2_key2scan['W']=sc_W;		ps2_key2scan['X']=sc_X;
		ps2_key2scan['Y']=sc_Y;		ps2_key2scan['Z']=sc_Z;

		ps2_key2scan['a']=sc_A;		ps2_key2scan['b']=sc_B;
		ps2_key2scan['c']=sc_C;		ps2_key2scan['d']=sc_D;
		ps2_key2scan['e']=sc_E;		ps2_key2scan['f']=sc_F;
		ps2_key2scan['g']=sc_G;		ps2_key2scan['h']=sc_H;
		ps2_key2scan['i']=sc_I;		ps2_key2scan['j']=sc_J;
		ps2_key2scan['k']=sc_K;		ps2_key2scan['l']=sc_L;
		ps2_key2scan['m']=sc_M;		ps2_key2scan['n']=sc_N;
		ps2_key2scan['o']=sc_O;		ps2_key2scan['p']=sc_P;
		ps2_key2scan['q']=sc_Q;		ps2_key2scan['r']=sc_R;
		ps2_key2scan['s']=sc_S;		ps2_key2scan['t']=sc_T;
		ps2_key2scan['u']=sc_U;		ps2_key2scan['v']=sc_V;
		ps2_key2scan['w']=sc_W;		ps2_key2scan['x']=sc_X;
		ps2_key2scan['y']=sc_Y;		ps2_key2scan['z']=sc_Z;

		ps2_key2scan[K_DEL			]=0x153;

		ps2_key2scan[K_UPARROW		]=0x148;
		ps2_key2scan[K_DOWNARROW	]=0x150;
		ps2_key2scan[K_LEFTARROW	]=0x14B;
		ps2_key2scan[K_RIGHTARROW	]=0x14D;
		ps2_key2scan[K_CTRL			]=0x01D;
		ps2_key2scan[K_ALT			]=0x038;
		ps2_key2scan[K_SHIFT		]=0x02A;
		ps2_key2scan[K_INS			]=0x152;
		ps2_key2scan[K_PGDN			]=0x151;
		ps2_key2scan[K_PGUP			]=0x149;
		ps2_key2scan[K_HOME			]=0x147;
		ps2_key2scan[K_END			]=0x14F;
//		ps2_key2scan[K_PAUSE		]=0x14F;

		ps2_key2scan[K_F1			]=0x03B;
		ps2_key2scan[K_F2			]=0x03C;
		ps2_key2scan[K_F3			]=0x03D;
		ps2_key2scan[K_F4			]=0x03E;
		ps2_key2scan[K_F5			]=0x03F;
		ps2_key2scan[K_F6			]=0x040;
		ps2_key2scan[K_F7			]=0x041;
		ps2_key2scan[K_F8			]=0x042;
		ps2_key2scan[K_F9			]=0x043;
		ps2_key2scan[K_F10			]=0x044;
		ps2_key2scan[K_F11			]=0x057;
		ps2_key2scan[K_F12			]=0x058;
	
		ps2_key2scan_init=1;
	}
	
	sc=ps2_key2scan[key&0xFF];
	if(!sc)
	{
		printf("Key_Event: No Scancode for %X\n", key);
		return(0);
	}else
	{
//		printf("Key_Event: Scancode %X -> %X\n", key, sc);
	}

//	if(key&0x8000)
	if(!down)
	{
		if(sc&0x100)
			I_AddScanByteKeyboard(0xE0);
		I_AddScanByteKeyboard(0xF0);
//		I_AddScanByteKeyboard(sc&0xFF);
		I_AddScanByteKeyboard((sc&0xFF)|0x80);
		return(0);
	}

	if(sc&0x100)
		I_AddScanByteKeyboard(0xE0);
	I_AddScanByteKeyboard(sc&0xFF);
	return(0);
}


//****************************************************************************
//
// LOCALS
//
//****************************************************************************]

static KeyboardDef KbdDefs = {0x1d,0x38,0x47,0x48,0x49,0x4b,0x4d,0x4f,0x50,0x51};
static JoystickDef JoyDefs[MaxJoys];
static ControlType Controls[MAXPLAYERS];


static boolean  IN_Started;

static	Direction	DirTable[] =		// Quick lookup for total direction
{
	dir_NorthWest, dir_North,  dir_NorthEast,
	dir_West,		dir_None,	dir_East,
	dir_SouthWest, dir_South,  dir_SouthEast
};

int (*function_ptr)();

static char *ParmStrings[] = {"nojoys","nomouse","spaceball","cyberman","assassin",NULL};


//******************************************************************************
//
// INL_GetMouseDelta () - Gets the amount that the mouse has moved from the
//								mouse driver
//
//******************************************************************************

void INL_GetMouseDelta(int *x, int *y)
{
	*x=0;
	*y=0;
}



//******************************************************************************
//
// IN_GetMouseButtons () - Gets the status of the mouse buttons from the
//								mouse driver
//
//******************************************************************************

word IN_GetMouseButtons( void )

{
	return(0);
}


//******************************************************************************
//
// IN_IgnoreMouseButtons () -
//	Tells the mouse to ignore the currently pressed buttons.
//
//******************************************************************************

void IN_IgnoreMouseButtons ( void )
{
	word  buttons;

	IgnoreMouse |= IN_GetMouseButtons();
}

//******************************************************************************
//
// IN_GetJoyAbs () - Reads the absolute position of the specified joystick
//
//******************************************************************************

void IN_GetJoyAbs (word joy, word *xp, word *yp)
{
#if 0
	Joy_x  = Joy_y = 0;
	Joy_xs = joy? 2 : 0;		// Find shift value for x axis
	Joy_xb = 1 << Joy_xs;		// Use shift value to get x bit mask
	Joy_ys = joy? 3 : 1;		// Do the same for y axis
	Joy_yb = 1 << Joy_ys;

	JoyStick_Vals ();

	*xp = Joy_x;
	*yp = Joy_y;
#endif

	*xp=0;
	*yp=0;
}



//******************************************************************************
//
// INL_GetJoyDelta () - Returns the relative movement of the specified
//							joystick (from +/-127)
//
//******************************************************************************

#if 0
void INL_GetJoyDelta (word joy, int *dx, int *dy)
{
	word		x, y;
	JoystickDef *def;
	static longword lasttime;

	IN_GetJoyAbs (joy, &x, &y);
	def = JoyDefs + joy;

	if (x < def->threshMinX)
	{
		if (x < def->joyMinX)
			x = def->joyMinX;

		x = -(x - def->threshMinX);
		x *= def->joyMultXL;
		x >>= JoyScaleShift;
		*dx = (x > 127)? -127 : -x;
	}
	else if (x > def->threshMaxX)
	{
		if (x > def->joyMaxX)
			x = def->joyMaxX;

		x = x - def->threshMaxX;
		x *= def->joyMultXH;
		x >>= JoyScaleShift;
		*dx = (x > 127)? 127 : x;
	}
	else
		*dx = 0;

	if (y < def->threshMinY)
	{
		if (y < def->joyMinY)
			y = def->joyMinY;

		y = -(y - def->threshMinY);
		y *= def->joyMultYL;
		y >>= JoyScaleShift;
		*dy = (y > 127)? -127 : -y;
	}
	else if (y > def->threshMaxY)
	{
		if (y > def->joyMaxY)
			y = def->joyMaxY;

		y = y - def->threshMaxY;
		y *= def->joyMultYH;
		y >>= JoyScaleShift;
		*dy = (y > 127)? 127 : y;
	}
	else
		*dy = 0;

	lasttime = ticcount;
}
#endif

void INL_GetJoyDelta (word joy, int *dx, int *dy)
{
	static word		lx, ly;
	word				x, y;

	IN_GetJoyAbs (joy, &x, &y);
	*dx=x-lx;
	*dy=y-ly;
	lx=x;
	ly=y;
}


//******************************************************************************
//
// INL_GetJoyButtons () - Returns the button status of the specified
//								joystick
//
//******************************************************************************

word INL_GetJoyButtons (word joy)
{
#if 0
	word  result;

//	result = inp (0x201);	// Get all the joystick buttons
	result >>= joy? 6 : 4;  // Shift into bits 0-1
	result &= 3;				// Mask off the useless bits
	result ^= 3;
	return (result);
#endif

	return(0);
}

#if 0
//******************************************************************************
//
// IN_GetJoyButtonsDB () - Returns the de-bounced button status of the
//								specified joystick
//
//******************************************************************************

word IN_GetJoyButtonsDB (word joy)
{
	longword lasttime;
	word result1,result2;

	do
	{
		result1 = INL_GetJoyButtons (joy);
		lasttime = ticcount;
		while (ticcount == lasttime)
			;
		result2 = INL_GetJoyButtons (joy);
	} while (result1 != result2);

	return(result1);
}
#endif

//******************************************************************************
//
// INL_StartMouse () - Detects and sets up the mouse
//
//******************************************************************************

boolean INL_StartMouse (void)
{
	return(true);
}



//******************************************************************************
//
// INL_SetJoyScale () - Sets up scaling values for the specified joystick
//
//******************************************************************************

void INL_SetJoyScale (word joy)
{
#if 0
	JoystickDef *def;

	def = &JoyDefs[joy];
	def->joyMultXL = JoyScaleMax / (def->threshMinX - def->joyMinX);
	def->joyMultXH = JoyScaleMax / (def->joyMaxX - def->threshMaxX);
	def->joyMultYL = JoyScaleMax / (def->threshMinY - def->joyMinY);
	def->joyMultYH = JoyScaleMax / (def->joyMaxY - def->threshMaxY);
#endif
}



//******************************************************************************
//
// IN_SetupJoy () - Sets up thresholding values and calls INL_SetJoyScale()
//						to set up scaling values
//
//******************************************************************************

void IN_SetupJoy (word joy, word minx, word maxx, word miny, word maxy)
{
	word	d,r;
	JoystickDef *def;

	def = &JoyDefs[joy];

	def->joyMinX = minx;
	def->joyMaxX = maxx;
	r = maxx - minx;
	d = r / 3;
	def->threshMinX = ((r / 2) - d) + minx;
	def->threshMaxX = ((r / 2) + d) + minx;

	def->joyMinY = miny;
	def->joyMaxY = maxy;
	r = maxy - miny;
	d = r / 3;
	def->threshMinY = ((r / 2) - d) + miny;
	def->threshMaxY = ((r / 2) + d) + miny;

	INL_SetJoyScale (joy);
}


//******************************************************************************
//
// INL_StartJoy () - Detects & auto-configures the specified joystick
//						The auto-config assumes the joystick is centered
//
//******************************************************************************

boolean INL_StartJoy (word joy)
{
#if 0
	word x,y;

	IN_GetJoyAbs (joy, &x, &y);

	if
	(
		((x == 0) || (x > MaxJoyValue - 10))
	|| ((y == 0) || (y > MaxJoyValue - 10))
	)
		return(false);
	else
	{
		IN_SetupJoy (joy, 0, x * 2, 0, y * 2);
		return (true);
	}
#endif

	return(false);
}



//******************************************************************************
//
// INL_ShutJoy() - Cleans up the joystick stuff
//
//******************************************************************************

void INL_ShutJoy (word joy)
{
	JoysPresent[joy] = false;
}



//******************************************************************************
//
// IN_Startup() - Starts up the Input Mgr
//
//******************************************************************************


void IN_Startup (void)
{
	boolean checkjoys,
			checkmouse,
			checkcyberman,
			checkspaceball,
			swiftstatus,
			checkassassin;

	word	i;

	if (IN_Started==true)
		return;

	checkjoys		= true;
	checkmouse		= true;
//	checkcyberman	= false;
//	checkassassin	= false;
//	checkspaceball	= false;
//	SpaceBallPresent = false;
//	CybermanPresent  = false;
//	AssassinPresent  = false;

	for (i = 1; i < _argc; i++)
	{
		switch (US_CheckParm (_argv[i], ParmStrings))
		{
		case 0:
			checkjoys = false;
		break;

		case 1:
			checkmouse = false;
		break;

		case 2:
 //		checkspaceball = true;
		break;

		case 3:
//			checkcyberman = true;
//			checkmouse = false;
		break;

		case 4:
//			checkassassin = true;
//			checkmouse = false;
		break;
		}
	}

	MousePresent = checkmouse ? INL_StartMouse() : false;

	if (!MousePresent)
		mouseenabled = false;
	else
		{
		if (!quiet)
			printf("IN_Startup: Mouse Present\n");
		}

	for (i = 0;i < MaxJoys;i++)
		{
		JoysPresent[i] = checkjoys ? INL_StartJoy(i) : false;
		if (INL_StartJoy(i))
			{
			if (!quiet)
				printf("IN_Startup: Joystick Present\n");
			}
		}

//	if (checkspaceball)
//		{
//		OpenSpaceBall ();
//		spaceballenabled=true;
//		}

#if 0
	if ((checkcyberman || checkassassin) && (swiftstatus = SWIFT_Initialize ()))
	{
		int dynamic;

		if (checkcyberman)
			{
//			CybermanPresent = swiftstatus;
//			cybermanenabled = true;
			}
		else if (checkassassin)
			{
			AssassinPresent = checkassassin & swiftstatus;
			assassinenabled = true;
			}

		dynamic = SWIFT_GetDynamicDeviceData ();

		SWIFT_TactileFeedback (40, 20, 20);

		if (SWIFT_GetDynamicDeviceData () == 2)
			Error ("SWIFT ERROR : External Power too high!\n");

		SWIFT_TactileFeedback (100, 10, 10);
		if (!quiet)
			printf("IN_Startup: Swift Device Present\n");
	}
#endif

	IN_Started = true;
}


#if 0
//******************************************************************************
//
// IN_Default() - Sets up default conditions for the Input Mgr
//
//******************************************************************************

void IN_Default (boolean gotit, ControlType in)
{
	if
	(
		(!gotit)
	||	((in == ctrl_Joystick1) && !JoysPresent[0])
	||	((in == ctrl_Joystick2) && !JoysPresent[1])
	||	((in == ctrl_Mouse) && !MousePresent)
	)
		in = ctrl_Keyboard1;
	IN_SetControlType (0, in);
}
#endif

//******************************************************************************
//
// IN_Shutdown() - Shuts down the Input Mgr
//
//******************************************************************************

void IN_Shutdown (void)
{
	word  i;

	if (IN_Started==false)
		return;

//	INL_ShutMouse();

//	for (i = 0;i < MaxJoys;i++)
//		INL_ShutJoy(i);

//	if (CybermanPresent || AssassinPresent)
//		SWIFT_Terminate ();

//	CloseSpaceBall ();

	IN_Started = false;
}


//******************************************************************************
//
// IN_ClearKeysDown() - Clears the keyboard array
//
//******************************************************************************

void IN_ClearKeysDown (void)
{
	LastScan = sc_None;
	memset ((void *)Keyboard, 0, sizeof (Keyboard));
}


//******************************************************************************
//
// IN_ReadControl() - Reads the device associated with the specified
//	player and fills in the control info struct
//
//******************************************************************************

void IN_ReadControl (int player, ControlInfo *info)
{
	boolean	realdelta;
	word		buttons;
	int			dx,dy;
	Motion		mx,my;
	ControlType type;

	KeyboardDef *def;

	dx = dy = 0;
	mx = my = motion_None;
	buttons = 0;


	switch (type = Controls[player])
	{
		case ctrl_Keyboard:
			def = &KbdDefs;

#if 0
			if (Keyboard[def->upleft])
				mx = motion_Left,my = motion_Up;
			else if (Keyboard[def->upright])
				mx = motion_Right,my = motion_Up;
			else if (Keyboard[def->downleft])
				mx = motion_Left,my = motion_Down;
			else if (Keyboard[def->downright])
				mx = motion_Right,my = motion_Down;
#endif
			if (Keyboard[sc_UpArrow])
				my = motion_Up;
			else if (Keyboard[sc_DownArrow])
				my = motion_Down;

			if (Keyboard[sc_LeftArrow])
				mx = motion_Left;
			else if (Keyboard[sc_RightArrow])
				mx = motion_Right;

			if (Keyboard[def->button0])
				buttons += 1 << 0;
			if (Keyboard[def->button1])
				buttons += 1 << 1;
			realdelta = false;
		break;

#if 0
		case ctrl_Joystick1:
		case ctrl_Joystick2:
			INL_GetJoyDelta (type - ctrl_Joystick, &dx, &dy);
			buttons = INL_GetJoyButtons (type - ctrl_Joystick);
			realdelta = true;
		break;

		case ctrl_Mouse:
			INL_GetMouseDelta (&dx,&dy);
			buttons = IN_GetMouseButtons ();
			realdelta = true;
		break;
#endif
	}

	if (realdelta)
	{
		mx = (dx < 0)? motion_Left : ((dx > 0)? motion_Right : motion_None);
		my = (dy < 0)? motion_Up : ((dy > 0)? motion_Down : motion_None);
	}
	else
	{
		dx = mx * 127;
		dy = my * 127;
	}

	info->x = dx;
	info->xaxis = mx;
	info->y = dy;
	info->yaxis = my;
	info->button0 = buttons & (1 << 0);
	info->button1 = buttons & (1 << 1);
	info->button2 = buttons & (1 << 2);
	info->button3 = buttons & (1 << 3);
	info->dir = DirTable[((my + 1) * 3) + (mx + 1)];
}


//******************************************************************************
//
// IN_WaitForKey() - Waits for a scan code, then clears LastScan and
//	returns the scan code
//
//******************************************************************************

ScanCode IN_WaitForKey (void)
{
	ScanCode result;

	while (!(result = LastScan))
		;
	LastScan = 0;
	return (result);
}


//******************************************************************************
//
// IN_Ack() - waits for a button or key press.  If a button is down, upon
// calling, it must be released for it to be recognized
//
//******************************************************************************

boolean  btnstate[8];

void IN_StartAck (void)
{
	unsigned i,
				buttons = 0;

//
// get initial state of everything
//
	LastScan = 0;

	IN_ClearKeysDown ();
	memset (btnstate, 0, sizeof(btnstate));

	buttons = 0;
//	buttons = IN_JoyButtons () << 4;

	buttons |= IN_GetMouseButtons();

//	if (SpaceBallPresent && spaceballenabled)
//		{
//		buttons |= GetSpaceBallButtons ();
//		}

	for (i=0;i<8;i++,buttons>>=1)
		if (buttons&1)
			btnstate[i] = true;
}



//******************************************************************************
//
// IN_CheckAck ()
//
//******************************************************************************

boolean IN_CheckAck (void)
{
	unsigned i,
				buttons = 0;

//
// see if something has been pressed
//
	if (LastScan)
		return true;

	buttons = IN_JoyButtons () << 4;

	buttons |= IN_GetMouseButtons();

	for (i=0;i<8;i++,buttons>>=1)
		if ( buttons&1 )
		{
			if (!btnstate[i])
				return true;
		}
		else
			btnstate[i]=false;

	return false;
}



//******************************************************************************
//
// IN_Ack ()
//
//******************************************************************************

void IN_Ack (void)
{
	IN_StartAck ();

	while (!IN_CheckAck ())
	;
}



//******************************************************************************
//
// IN_UserInput() - Waits for the specified delay time (in ticks) or the
//	user pressing a key or a mouse button. If the clear flag is set, it
//	then either clears the key or waits for the user to let the mouse
//	button up.
//
//******************************************************************************

boolean IN_UserInput (long delay)
{
	long lasttime;

	lasttime = ticcount;

	IN_StartAck ();
	do
	{
		if (IN_CheckAck())
			return true;
	} while ((ticcount - lasttime) < delay);

	return (false);
}

//===========================================================================


/*
===================
=
= IN_JoyButtons
=
===================
*/

byte IN_JoyButtons (void)
{
	unsigned joybits;

//	joybits = inp (0x201);  // Get all the joystick buttons
	joybits = 0;
//	joybits >>= 4;			// only the high bits are useful
//	joybits ^= 15;			// return with 1=pressed

	return joybits;
}


//******************************************************************************
//
// IN_UpdateKeyboard ()
//
//******************************************************************************

void IN_UpdateKeyboard (void)
{
	int tail;
	int key;

	I_HandleInput ();

// #ifndef __BJX2__
	QueueLetterInput ();
// #endif

	if (Keytail != Keyhead)
	{
		tail = Keytail;

		while (Keyhead != tail)
		{
			if (KeyboardQueue[Keyhead] & 0x80)		// Up event
			{
				key = KeyboardQueue[Keyhead] & 0x7F;	// AND off high bit

//				if (keysdown[key])
//				{
//					KeyboardQueue[Keytail] = KeyboardQueue[Keyhead];
//					Keytail = (Keytail+1)&(KEYQMAX-1);
//				}
//				else
					Keyboard[key] = 0;
			}
			else												// Down event
			{
				Keyboard[KeyboardQueue[Keyhead]] = 1;
//				keysdown[KeyboardQueue[Keyhead]] = 1;
			}

			Keyhead = (Keyhead+1)&(KEYQMAX-1);

		}		// while
	}			// if

	// Carry over movement keys from the last refresh
//	keysdown[sc_RightArrow] = Keyboard[sc_RightArrow];
//	keysdown[sc_LeftArrow]  = Keyboard[sc_LeftArrow];
//	keysdown[sc_UpArrow]	= Keyboard[sc_UpArrow];
//	keysdown[sc_DownArrow]  = Keyboard[sc_DownArrow];
	}



//******************************************************************************
//
// IN_InputUpdateKeyboard ()
//
//******************************************************************************

#if 1
int IN_InputUpdateKeyboard (void)
{
	int key;
	int returnval = 0;
	boolean done = false;

//	_disable ();

	I_HandleInput ();

// #ifndef __BJX2__
	QueueLetterInput ();
// #endif

	if (Keytail != Keyhead)
	{
		int tail = Keytail;

		while (!done && (Keyhead != tail))
		{
			if (KeyboardQueue[Keyhead] & 0x80)		// Up event
			{
				key = KeyboardQueue[Keyhead] & 0x7F;	// AND off high bit

				Keyboard[key] = 0;
			}
			else												// Down event
			{
				Keyboard[KeyboardQueue[Keyhead]] = 1;
				returnval = KeyboardQueue[Keyhead];
				done = true;
			}

			Keyhead = (Keyhead+1)&(KEYQMAX-1);
		}
	}			// if

//	_enable ();

	return (returnval);
}
#endif

//******************************************************************************
//
// IN_ClearKeyboardQueue ()
//
//******************************************************************************

void IN_ClearKeyboardQueue (void)
{
	return;

//	IN_ClearKeysDown ();

//	Keytail = Keyhead = 0;
//	memset (KeyboardQueue, 0, sizeof (KeyboardQueue));
//	I_SendKeyboardData(0xf6);
//	I_SendKeyboardData(0xf4);
}


#if 0
//******************************************************************************
//
// IN_DumpKeyboardQueue ()
//
//******************************************************************************

void IN_DumpKeyboardQueue (void)
{
	int head = Keyhead;
	int tail = Keytail;
	int key;

	if (tail != head)
	{
		SoftError( "START DUMP\n");

		while (head != tail)
		{
			if (KeyboardQueue[head] & 0x80)		// Up event
			{
				key = KeyboardQueue[head] & 0x7F;	// AND off high bit

//				if (keysdown[key])
//				{
//					SoftError( "%s - was put in next refresh\n",
//											IN_GetScanName (key));
//				}
//				else
//				{
					if (Keyboard[key] == 0)
						SoftError( "%s %ld - was lost\n", IN_GetScanName (key), key);
					else
						SoftError( "%s %ld - up\n", IN_GetScanName (key), key);
//				}
			}
			else												// Down event
				SoftError( "%s %ld - down\n", IN_GetScanName (KeyboardQueue[head]), KeyboardQueue[head]);

			head = (head+1)&(KEYQMAX-1);
		}		// while

		SoftError( "END DUMP\n");

	}			// if
}
#endif


//******************************************************************************
//
// QueueLetterInput ()
//
//******************************************************************************

void QueueLetterInput (void)
{
	static int lasthead;
	int head = Keyhead;
	int tail = Keytail;
	char c;
	int scancode;
	boolean send = false;

#if 1
	if(tail == lasthead)
		return;
	lasthead = tail;
#endif

//	printf("QueueLetterInput %d %d\n", head, tail);

	while (head != tail)
	{
		if (!(KeyboardQueue[head] & 0x80))		// Down event
		{
			scancode = KeyboardQueue[head];

			if (Keyboard[sc_RShift] || Keyboard[sc_LShift])
			{
				c = ShiftedScanChars[scancode];
			}
			else
			{
				c = ScanChars[scancode];
			}

//			printf("Sc %X\n", scancode);

			// If "is printable char", queue the character
			if (c)
			{
//				printf("Pc %X\n", c);
			
				LetterQueue[LastLetter] = c;
				LastLetter = (LastLetter+1)&(MAXLETTERS-1);

				// If typing a message, update the text with 'c'

				if ( MSG.messageon )
				{
					Keystate[scancode]=0;
					KeyboardQueue[head] = 0;
					if ( MSG.inmenu )
					{
						if ( ( c == 'A' ) || ( c == 'a' ) )
						{
							MSG.towho = MSG_DIRECTED_TO_ALL;
							send		= true;
						}

						if ( ( gamestate.teamplay ) &&
							( ( c == 'T' ) || ( c == 't' ) ) )
						{
							MSG.towho = MSG_DIRECTED_TO_TEAM;
							send		= true;
						}

						if ( ( c >= '0' ) && ( c <= '9' ) )
						{
							int who;

							if ( c == '0' )
							{
								who = 10;
							}
							else
							{
								who = c - '1';
							}

							// Skip over local player
							if ( who >= consoleplayer )
							{
								who++;
							}

							if ( who < numplayers )
							{
								MSG.towho = who;
								send		= true;
							}
						}

						if ( send )
						{
							MSG.messageon = false;
							KeyboardQueue[ head ] = 0;
							Keyboard[ scancode ]  = 0;
							LastScan				= 0;
							FinishModemMessage( MSG.textnum, true );
						}
					}
					else if ( ( scancode >= sc_1 ) && ( scancode <= sc_0 ) &&
						( Keyboard[ sc_Alt ] ) )
					{
						int msg;

						msg = scancode - sc_1;

						if ( CommbatMacros[ msg ].avail )
						{
							MSG.length = strlen( CommbatMacros[ msg ].macro ) + 1;
							strcpy( Messages[ MSG.textnum ].text,
								CommbatMacros[ msg ].macro );

							MSG.messageon = false;
							FinishModemMessage( MSG.textnum, true );
							KeyboardQueue[ head ] = 0;
							Keyboard[ sc_Enter ]  = 0;
							Keyboard[ sc_Escape ] = 0;
							LastScan				= 0;
						}
						else
						{
							MSG.messageon = false;
							MSG.directed  = false;

							FinishModemMessage( MSG.textnum, false );
							AddMessage( "No macro.", MSG_MACRO );
							KeyboardQueue[ head ] = 0;
							Keyboard[ sc_Enter ]  = 0;
							Keyboard[ sc_Escape ] = 0;
							LastScan				= 0;
						}
					}
					else if ( MSG.length < MAXMESSAGELENGTH )
					{
						UpdateModemMessage (MSG.textnum, c);
					}
				}
			}
			else
			{
				// If typing a message, check for special characters

				if ( MSG.messageon && MSG.inmenu )
				{
					if ( scancode == sc_Escape )
					{
						MSG.messageon = false;
						MSG.directed  = false;
						FinishModemMessage( MSG.textnum, false );
						KeyboardQueue[head] = 0;
						Keyboard[sc_Enter]  = 0;
						Keyboard[sc_Escape] = 0;
						LastScan				= 0;
					}
				}
				else if ( MSG.messageon && !MSG.inmenu )
				{
					if ( ( scancode >= sc_F1 ) &&
						( scancode <= sc_F10 ) )
					{
						MSG.remoteridicule = scancode - sc_F1;
						MSG.messageon = false;
						FinishModemMessage(MSG.textnum, true);
						KeyboardQueue[head] = 0;
						Keyboard[sc_Enter]  = 0;
						Keyboard[sc_Escape] = 0;
						LastScan				= 0;
					}

					switch (scancode)
					{
						case sc_BackSpace:
							KeyboardQueue[head] = 0;
							if (MSG.length > 1)
							{
								ModemMessageDeleteChar (MSG.textnum);
							}
							Keystate[scancode]=0;
							break;

						case sc_Enter:
							MSG.messageon = false;
							FinishModemMessage(MSG.textnum, true);
							KeyboardQueue[head] = 0;
							Keyboard[sc_Enter]  = 0;
							Keyboard[sc_Escape] = 0;
							LastScan				= 0;
							Keystate[scancode]=0;
							break;

						case sc_Escape:
							MSG.messageon = false;
							MSG.directed  = false;
							FinishModemMessage(MSG.textnum, false);
							KeyboardQueue[head] = 0;
							Keyboard[sc_Enter]  = 0;
							Keyboard[sc_Escape] = 0;
							LastScan				= 0;
							break;
					}
				}
			}
		}

		head = (head+1)&(KEYQMAX-1);
	}		// while
}
