#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tkgdi/tkgdi.h>

#include <stdarg.h>

typedef unsigned char byte;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

TKGHDC i_hDc;

byte *screen;
int i_screen_dirty;
int i_doexit;

double calc_val_x, calc_val_y, calc_val_z;
char calc_numbuf[32];
byte calc_numbuf_dirty;
byte calc_numbuf_opr;

char calc_resultbuf[64];


void I_Key_Event(int ch, int dn)
{
	int i, l;
	
	if(!dn)
		return;
	
	if(ch==TK_K_ESCAPE)
	{
		i_doexit=1;
		return;
	}
	
	if((ch=='q') || (ch=='Q'))
	{
		i_doexit=1;
		return;
	}
	
	if(ch==TK_K_BACKSPACE)
	{
		l=strlen(calc_numbuf);
		if(l>0)
		{
			calc_numbuf[l-1]=0;
		}
		calc_val_y=atof(calc_numbuf);
		calc_numbuf_dirty=1;
		return;
	}
	
	if(((ch>='0') && (ch<='9')) || (ch=='.'))
	{
		l=strlen(calc_numbuf);

		if(ch=='.')
		{
			for(i=0; i<l; i++)
				if(calc_numbuf[i]=='.')
					break;
			if(i<l)
				return;
		}
	
		if(l<16)
		{
			calc_numbuf[l]=ch;
			calc_numbuf[l+1]=0;
		}
		calc_val_y=atof(calc_numbuf);
		calc_numbuf_dirty=1;
		return;
	}
	
	if((ch=='+') || (ch=='-') || (ch=='*') || (ch=='/'))
	{
		if(calc_numbuf_opr)
		{
			calc_val_x=calc_val_z;
			calc_val_y=0;
			calc_numbuf_opr=ch;
			calc_numbuf[0]=0;
			calc_numbuf_dirty=1;
			return;
		}
	
		calc_val_x=calc_val_y;
		calc_val_y=0;
		calc_numbuf_opr=ch;
		calc_numbuf[0]=0;
		calc_numbuf_dirty=1;
		return;
	}
	
	if(ch=='=')
	{
		calc_val_x=0;
		calc_val_y=calc_val_z;
		calc_numbuf_opr=0;
		
		sprintf(calc_numbuf, "%f", calc_val_y);
		l=strlen(calc_numbuf);
		while(l>0)
		{
			if(calc_numbuf[l-1]=='0')
			{
				calc_numbuf[l-1]=0;
				l--;
				continue;
			}
			if(calc_numbuf[l-1]=='.')
			{
				calc_numbuf[l-1]=0;
				l--;
				break;
			}
			break;
		}
		calc_val_y=atof(calc_numbuf);
		calc_numbuf_dirty=1;
		return;
	}
}

void I_Input()
{
	TKGDI_EVENT t_imsg;
	TKGDI_EVENT *imsg;
	int i, j, c, n, dn;

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

				if(c>=256)
					continue;
				
				I_Key_Event (c, dn);
			}
		}
		
		return;
	}

	n=64;
//	while(*kb)
	while((tk_kbhit()>0) && ((n--)>0))
	{
//		printf("Debug B3-1 %s:%d\n", __FILE__, __LINE__);

		c=tk_getch();
		if(c<0)
			break;
	
//		printf("Debug B3-2 %s:%d\n", __FILE__, __LINE__);

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
			break;
		default:
			dn=(c&0x80)?0:1;
			c=c&0x7F;
			break;
		}

		I_Key_Event (c, dn);
	}
}


// TKGDI_BITMAPINFOHEADER i_t_dibinfo;
TKGDI_BITMAPINFOHEADER *i_dibinfo = NULL;
byte *i_dibinfo_pal;
// TKGHDC i_hDc;

void I_InitTkGdi()
{
	int clo, chi;
	int i, j, k;

	if(i_dibinfo)
		return;
		
//	i_dibinfo = &i_t_dibinfo;
	i_dibinfo = tkgGlobalAlloc(sizeof(TKGDI_BITMAPINFOHEADER)+1024);
	memset(i_dibinfo, 0, sizeof(TKGDI_BITMAPINFOHEADER)+1024);

#if 0
	i_dibinfo->biSize=sizeof(TKGDI_BITMAPINFOHEADER);
	i_dibinfo->biWidth=320;
	i_dibinfo->biHeight=200;

	i_dibinfo->biBitCount=8;
	i_dibinfo->biClrUsed=256;
	i_dibinfo->biClrImportant=16;

	i_dibinfo_pal=(byte *)(i_dibinfo+1);

	memset(i_dibinfo_pal, 0, 256*4);

	for(i=0; i<16; i++)
	{
		clo=0x00;	chi=0xAA;
		if(i&8)
			{ clo=0x55;	chi=0xFF; }
		i_dibinfo_pal[i*4+0]=(i&1)?chi:clo;
		i_dibinfo_pal[i*4+1]=(i&2)?chi:clo;
		i_dibinfo_pal[i*4+2]=(i&4)?chi:clo;
		i_dibinfo_pal[i*4+3]=255;
	}
#endif

	TKGDI_SetupDibHead256Default(i_dibinfo, 320, 200);

	i_dibinfo->biBitCount=16;

//	tk_printf("  1\n", hDc);

	i_hDc=tkgCreateDisplay(i_dibinfo);
	tkgSetWindowTitle(i_hDc, "Calc");

	i_dibinfo->biBitCount=8;
//	i_dibinfo->biHeight=-200;

	if(!screen)
	{
		screen=tkgGlobalAlloc((320+4)*(200+4));
	}
}

void I_FinishUpdate (void)
{
	int i, j, k;

	I_InitTkGdi();

	if(!screen)
		return;

	if(i_screen_dirty)
	{
		tkgBlitImage(i_hDc, 0, 0, i_dibinfo, screen);
		i_screen_dirty=0;
	}

	I_Input();
}

void D_DrawSymbolButton(int x, int y, int ch)
{
	TKGDI_ImgUtil_FillRect8(screen, 320, 200, x+0, y+0, x+16, y+16, 0x07);
	TKGDI_ImgUtil_DrawOutlineRect8(
		screen, 320, 200, x+0, y+16, x+16, y+ 0, 0x0F, 0x00);
	TKGDI_ImgUtil_DrawTextChar8(screen, 320, 200, x+4, y+4, ch, 0x00);
}

void D_DrawStringButton(int x, int y, char *str, int len)
{
	int l1;
	l1=len-1;
	if(l1<0)	l1=0;
	
	TKGDI_ImgUtil_FillRect8(screen, 320, 200, x+0, y+0, x+16+l1*8, y+16, 0x07);
	TKGDI_ImgUtil_DrawOutlineRect8(
		screen, 320, 200, x+0, y+16, x+16+l1*8, y+ 0, 0x0F, 0x00);
	TKGDI_ImgUtil_DrawTextString8(screen, 320, 200, x+4, y+4, str, 0x00);
}

void D_DrawStringButtonRJust(int x, int y, char *str, int len)
{
	int l1;
	l1=len-1;
	if(l1<0)	l1=0;
	
	TKGDI_ImgUtil_FillRect8(screen, 320, 200, x+0, y+0, x+16+l1*8, y+16, 0x07);
	TKGDI_ImgUtil_DrawOutlineRect8(
		screen, 320, 200, x+0, y+16, x+16+l1*8, y+ 0, 0x0F, 0x00);
	TKGDI_ImgUtil_DrawTextString8(screen, 320, 200,
		(x+4)+(len-strlen(str))*8, y+4, str, 0x00);
}

int main()
{
	I_InitTkGdi();

	if(!screen)
	{
		printf("Init Failed\n");
		return(0);
	}
	
	memset(screen, 0x08, 320*200);
	i_screen_dirty=1;

	D_DrawSymbolButton(24, 24, '~');
	D_DrawSymbolButton(48, 24, '0');
	D_DrawSymbolButton(72, 24, '.');
	D_DrawSymbolButton(96, 24, '=');

	D_DrawSymbolButton(24, 48, '1');
	D_DrawSymbolButton(48, 48, '2');
	D_DrawSymbolButton(72, 48, '3');
	D_DrawSymbolButton(96, 48, '+');

	D_DrawSymbolButton(24, 72, '4');
	D_DrawSymbolButton(48, 72, '5');
	D_DrawSymbolButton(72, 72, '6');
	D_DrawSymbolButton(96, 72, '-');

	D_DrawSymbolButton(24, 96, '7');
	D_DrawSymbolButton(48, 96, '8');
	D_DrawSymbolButton(72, 96, '9');
	D_DrawSymbolButton(96, 96, '*');

	D_DrawStringButtonRJust(24, 168, "0", 16);
	D_DrawStringButtonRJust(0, 144, "", 36);

	while(!i_doexit)
	{
		if(calc_numbuf_dirty)
		{
			if(strlen(calc_numbuf)>0)
			{
				D_DrawStringButtonRJust(24, 168, calc_numbuf, 16);
			}else
			{
				D_DrawStringButtonRJust(24, 168, "0", 16);
			}
			
			if(calc_numbuf_opr>0)
			{
				switch(calc_numbuf_opr)
				{
					case '+':	calc_val_z=calc_val_x+calc_val_y; break;
					case '-':	calc_val_z=calc_val_x-calc_val_y; break;
					case '*':	calc_val_z=calc_val_x*calc_val_y; break;
					case '/':	calc_val_z=calc_val_x/calc_val_y; break;
				}
			
				sprintf(calc_resultbuf, "%f %c %f = %f",
					calc_val_x,
					calc_numbuf_opr,
					calc_val_y,
					calc_val_z);
			}else
			{
				calc_resultbuf[0]=0;
			}

			D_DrawStringButtonRJust(0, 144, calc_resultbuf, 36);
			
			i_screen_dirty=1;
		}
	
		thrd_yield();

//		usleep(0);
		I_FinishUpdate();
	}

	return(0);
}
