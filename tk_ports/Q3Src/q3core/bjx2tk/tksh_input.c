/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

#include <errno.h>
#include <stdio.h>

#include <dirent.h>
#include <unistd.h>
#include <sys/mman.h>
// #include <sys/time.h>

#include <tkgdi/tkgdi.h>

#include "../client/client.h"

void Sys_QueEvent(
	int time, sysEventType_t type, int value, int value2,
	int ptrLength, void *ptr );

void IN_Init( void ) {
}

void IN_Frame (void) {
}

void IN_Shutdown( void ) {
}

int I_TkgRemapKey(int keysym, int *key)
{
	switch (keysym)
	{
		case TK_K_NUMPAD9:		*key = K_KP_PGUP; break;
		case TK_K_NUMPAD3:		*key = K_KP_PGDN; break;
		case TK_K_NUMPAD7:		*key = K_KP_HOME; break;
		case TK_K_NUMPAD1:		*key = K_KP_END; break;
		case TK_K_NUMPAD4:		*key = K_KP_LEFTARROW; break;
		case TK_K_NUMPAD6:		*key = K_KP_RIGHTARROW; break;
		case TK_K_NUMPAD2:		*key = K_KP_DOWNARROW; break;
		case TK_K_NUMPAD8:		*key = K_KP_UPARROW; break;
		case TK_K_NUMPAD5:		*key = K_KP_5;  break;
		case TK_K_NUMPAD0:		*key = K_KP_INS; break;

		case TK_K_PGUP:			*key = K_PGUP; break;
		case TK_K_PGDN:			*key = K_PGDN; break;
		case TK_K_HOME:			*key = K_HOME; break;
		case TK_K_END:			*key = K_END; break;
		case TK_K_LEFTARROW:	*key = K_LEFTARROW; break;
		case TK_K_RIGHTARROW:	*key = K_RIGHTARROW;    break;
		case TK_K_DOWNARROW:	*key = K_DOWNARROW; break;
		case TK_K_UPARROW:		*key = K_UPARROW;   break;

		case TK_K_ESCAPE:		*key = K_ESCAPE;    break;

		case TK_K_NUMPAD_ENTR:	*key = K_KP_ENTER;  break;
		case TK_K_ENTER:		*key = K_ENTER;    break;

		case TK_K_TAB:			*key = K_TAB;      break;

		case TK_K_F1:			*key = K_F1;       break;
		case TK_K_F2:			*key = K_F2;       break;
		case TK_K_F3:			*key = K_F3;       break;
		case TK_K_F4:			*key = K_F4;       break;
		case TK_K_F5:			*key = K_F5;       break;
		case TK_K_F6:			*key = K_F6;       break;
		case TK_K_F7:			*key = K_F7;       break;
		case TK_K_F8:			*key = K_F8;       break;
		case TK_K_F9:			*key = K_F9;       break;
		case TK_K_F10:			*key = K_F10;      break;
		case TK_K_F11:			*key = K_F11;      break;
		case TK_K_F12:			*key = K_F12;      break;

		case TK_K_BACKSPACE:	*key = K_BACKSPACE; break;

		case TK_K_NUMPAD_DEL:	*key = K_KP_DEL; break;
		case TK_K_DEL:			*key = K_DEL; break;
		case TK_K_INS:			*key = K_INS; break;

		case TK_K_PAUSE:		*key = K_PAUSE;    break;

		case TK_K_SHIFT:		*key = K_SHIFT;   break;
		case TK_K_CTRL:			*key = K_CTRL;  break;
		case TK_K_ALT:			*key = K_ALT;     break;

		case TK_K_NUMPAD_MUL:	*key = '*'; break;
		case TK_K_NUMPAD_ADD:	*key = K_KP_PLUS; break;
		case TK_K_NUMPAD_SUB:	*key = K_KP_MINUS; break;
		case TK_K_NUMPAD_DIV:	*key = K_KP_SLASH; break;

		case '!':				*key = '1'; break;
		case '@':				*key = '2'; break;
		case '#':				*key = '3'; break;
		case '$':				*key = '4'; break;
		case '%':				*key = '5'; break;
		case '^':				*key = '6'; break;
		case '&':				*key = '7'; break;
		case '*':				*key = '8'; break;
		case '(':				*key = '9'; break;
		case ')':				*key = '0'; break;
		case TK_K_SPACE:		*key = K_SPACE; break;
		
		default:
			*key = keysym;
			if((keysym>='A') && (keysym<='Z'))
				*key = (keysym - 'A') + 'a';
			break;
	}
	return(*key);
}

void I_KeyEvent(int keysym, int dn)
{
	int key;
	int t = 0; // default to 0 in case we don't set

	I_TkgRemapKey(keysym, &key);

	if(dn)
	{
		Sys_QueEvent( t, SE_KEY, key, qtrue, 0, NULL );
		if((key>0) && (key<='~'))
			Sys_QueEvent( t, SE_CHAR, key, 0, 0, NULL );
	}else
	{
		Sys_QueEvent( t, SE_KEY, key, qfalse, 0, NULL );
	}
}

void Sys_SendKeyEvents (void)
{
	TKGDI_EVENT t_imsg;
	TKGDI_EVENT *imsg;
	int i, j, k, c, dn;

//	memcpy(i_lkeymask, i_keymask, 8*4);

	if(i_hDc>1)
	{
		thrd_yield();
		imsg=&t_imsg;

		while(1)
		{
			j=tkgPollEvent(i_hDc, imsg);
			if(j<1)
				break;
			if(imsg->fccMsg==0)
				break;
			if(imsg->fccMsg==TKGDI_FCC_keyb)
			{
				c=imsg->wParm1;
				dn=!(c&0x8000);
				c=c&0x7FFF;
			
				switch(c)
				{
				case   8: c=K_BACKSPACE; break;
				case 153: c=K_PAUSE; break;
				case 154: c=K_MWHEELUP; break;
				case 155: c=K_MWHEELDOWN; break;
				case 157: c=K_MOUSE1; break;
				case 158: c=K_MOUSE2; break;
				case 159: c=K_MOUSE3; break;
				default: break;
				}
				
				if(c>=256)
					continue;
				
				I_KeyEvent (c, dn);
			}
		}
		
		return;
	}

	while(tk_kbhit())
	{
		c=tk_getch();

		switch(c)
		{
		case 0x7F:
			c=tk_getch(); dn=1;
			break;
		case 0xFF:
			c=tk_getch(); dn=0;
			break;
		case 0x80:
			c=tk_getch();
			c=(c<<8)|tk_getch();
			dn=(c&0x8000)?0:1;
			c&=0x7FFF;
			break;
		default:
			dn=(c&0x80)?0:1;
			c=c&0x7F;
			break;
		}
		
		switch(c)
		{
		case   8: c=K_BACKSPACE; break;
		case 153: c=K_PAUSE; break;
		case 154: c=K_MWHEELUP; break;
		case 155: c=K_MWHEELDOWN; break;
		case 157: c=K_MOUSE1; break;
		case 158: c=K_MOUSE2; break;
		case 159: c=K_MOUSE3; break;
		default: break;
		}
		
		if(c>=256)
			continue;
		
		I_KeyEvent (c, dn);
	}
}

