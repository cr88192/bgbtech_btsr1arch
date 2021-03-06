
//**************************************************************************
//**
//** v_video.c : Heretic 2 : Raven Software, Corp.
//**
//** $RCSfile: v_video.c,v $
//** $Revision: 1.2 $
//** $Date: 95/09/11 14:58:16 $
//** $Author: cjr $
//**
//**************************************************************************

#include "h2def.h"

#define SC_INDEX 0x3c4

dt_scrpix *screen;
int dirtybox[4];
int usegamma;

byte gammatable[5][256] =
{
{1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,164,165,166,167,168,169,170,171,172,173,174,175,176,177,178,179,180,181,182,183,184,185,186,187,188,189,190,191,192,193,194,195,196,197,198,199,200,201,202,203,204,205,206,207,208,209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,243,244,245,246,247,248,249,250,251,252,253,254,255},

{2,4,5,7,8,10,11,12,14,15,16,18,19,20,21,23,24,25,26,27,29,30,31,32,33,34,36,37,38,39,40,41,42,44,45,46,47,48,49,50,51,52,54,55,56,57,58,59,60,61,62,63,64,65,66,67,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,144,145,146,147,148,148,149,150,151,152,153,154,155,156,157,158,159,160,161,162,163,163,164,165,166,167,168,169,170,171,172,173,174,175,175,176,177,178,179,180,181,182,183,184,185,186,186,187,188,189,190,191,192,193,194,195,196,196,197,198,199,200,201,202,203,204,205,205,206,207,208,209,210,211,212,213,214,214,215,216,217,218,219,220,221,222,222,223,224,225,226,227,228,229,230,230,231,232,233,234,235,236,237,237,238,239,240,241,242,243,244,245,245,246,247,248,249,250,251,252,252,253,254,255},

{4,7,9,11,13,15,17,19,21,22,24,26,27,29,30,32,33,35,36,38,39,40,42,43,45,46,47,48,50,51,52,54,55,56,57,59,60,61,62,63,65,66,67,68,69,70,72,73,74,75,76,77,78,79,80,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,133,134,135,136,137,138,139,140,141,142,143,144,144,145,146,147,148,149,150,151,152,153,153,154,155,156,157,158,159,160,160,161,162,163,164,165,166,166,167,168,169,170,171,172,172,173,174,175,176,177,178,178,179,180,181,182,183,183,184,185,186,187,188,188,189,190,191,192,193,193,194,195,196,197,197,198,199,200,201,201,202,203,204,205,206,206,207,208,209,210,210,211,212,213,213,214,215,216,217,217,218,219,220,221,221,222,223,224,224,225,226,227,228,228,229,230,231,231,232,233,234,235,235,236,237,238,238,239,240,241,241,242,243,244,244,245,246,247,247,248,249,250,251,251,252,253,254,254,255},

{8,12,16,19,22,24,27,29,31,34,36,38,40,41,43,45,47,49,50,52,53,55,57,58,60,61,63,64,65,67,68,70,71,72,74,75,76,77,79,80,81,82,84,85,86,87,88,90,91,92,93,94,95,96,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127,128,129,130,131,132,133,134,135,135,136,137,138,139,140,141,142,143,143,144,145,146,147,148,149,150,150,151,152,153,154,155,155,156,157,158,159,160,160,161,162,163,164,165,165,166,167,168,169,169,170,171,172,173,173,174,175,176,176,177,178,179,180,180,181,182,183,183,184,185,186,186,187,188,189,189,190,191,192,192,193,194,195,195,196,197,197,198,199,200,200,201,202,202,203,204,205,205,206,207,207,208,209,210,210,211,212,212,213,214,214,215,216,216,217,218,219,219,220,221,221,222,223,223,224,225,225,226,227,227,228,229,229,230,231,231,232,233,233,234,235,235,236,237,237,238,238,239,240,240,241,242,242,243,244,244,245,246,246,247,247,248,249,249,250,251,251,252,253,253,254,254,255},

{16,23,28,32,36,39,42,45,48,50,53,55,57,60,62,64,66,68,69,71,73,75,76,78,80,81,83,84,86,87,89,90,92,93,94,96,97,98,100,101,102,103,105,106,107,108,109,110,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,128,128,129,130,131,132,133,134,135,136,137,138,139,140,141,142,143,143,144,145,146,147,148,149,150,150,151,152,153,154,155,155,156,157,158,159,159,160,161,162,163,163,164,165,166,166,167,168,169,169,170,171,172,172,173,174,175,175,176,177,177,178,179,180,180,181,182,182,183,184,184,185,186,187,187,188,189,189,190,191,191,192,193,193,194,195,195,196,196,197,198,198,199,200,200,201,202,202,203,203,204,205,205,206,207,207,208,208,209,210,210,211,211,212,213,213,214,214,215,216,216,217,217,218,219,219,220,220,221,221,222,223,223,224,224,225,225,226,227,227,228,228,229,229,230,230,231,232,232,233,233,234,234,235,235,236,236,237,237,238,239,239,240,240,241,241,242,242,243,243,244,244,245,245,246,246,247,247,248,248,249,249,250,250,251,251,252,252,253,254,254,255,255}
};

extern byte *tinttable;

int R_PixelBlend(int pixa, int pixb);

#if 1
int R_PixelBlend(int pixa, int pixb)
{
	return(((pixa&0x7BDE)+(pixb&0x7BDE))>>1);
}
#endif

int V_BlendEven(int pixa, int pixb)
{
	int pix;
	if(sizeof(dt_scrpix)==1)
		pix = tinttable[pixa+(pixb<<8)];
	else
//		pix = VID_BlendEven16(pixa, pixb);
		pix = R_PixelBlend(pixa, pixb);
	return(pix);
}

//---------------------------------------------------------------------------
//
// PROC V_DrawPatch
//
// Draws a column based masked pic to the screen.
//
//---------------------------------------------------------------------------

void V_DrawPatch(int x, int y, patch_t *patch)
{
	int count;
	int col;
	column_t *column;
	dt_scrpix *desttop;
	dt_scrpix *dest;
	byte *source;
	int w;

	y -= SHORT(patch->topoffset);
	x -= SHORT(patch->leftoffset);
	if(x < 0 || x+SHORT(patch->width) > SCREENWIDTH || y < 0
		|| y+SHORT(patch->height) > SCREENHEIGHT)
	{
//		I_Error("Bad V_DrawPatch");
		return;
	}
	col = 0;
	desttop = screen+y*SCREENWIDTH+x;
	w = SHORT(patch->width);
	for(; col < w; x++, col++, desttop++)
	{
		column = (column_t *)((byte *)patch+LONG(patch->columnofs[col]));
		// Step through the posts in a column
		while(column->topdelta != 0xff)
		{
			source = (byte *)column+3;
			dest = desttop+column->topdelta*SCREENWIDTH;
			count = column->length;
			while(count--)
			{
//				*dest = *source++;
				*dest = colormaps[*source++];
				dest += SCREENWIDTH;
			}
			column = (column_t *)((byte *)column+column->length+4);
		}
	}
}

/*
==================
=
= V_DrawFuzzPatch
=
= Masks a column based translucent masked pic to the screen.
=
==================
*/
void V_DrawFuzzPatch (int x, int y, patch_t *patch)
{
	int			count,col;
	column_t	*column;
	dt_scrpix	*desttop, *dest;
	byte		*source;
	int			w;
	
	y -= SHORT(patch->topoffset);
	x -= SHORT(patch->leftoffset);

	if (x<0||x+SHORT(patch->width) >SCREENWIDTH || y<0 || y+SHORT(patch->height)>SCREENHEIGHT
)
		I_Error ("Bad V_DrawPatch");

	col = 0;
	desttop = screen+y*SCREENWIDTH+x;
	
	w = SHORT(patch->width);
	for ( ; col<w ; x++, col++, desttop++)
	{
		column = (column_t *)((byte *)patch + LONG(patch->columnofs[col]));

// step through the posts in a column
	
		while (column->topdelta != 0xff )
		{
			source = (byte *)column + 3;
			dest = desttop + column->topdelta*SCREENWIDTH;
			count = column->length;
			
			while (count--)
			{
				if(sizeof(dt_scrpix)==1)
					*dest = tinttable[*dest + ((*source++)<<8)];
				else
//					*dest = VID_BlendEven16(*dest, colormaps[(*source++)]);
					*dest = R_PixelBlend(*dest, colormaps[(*source++)]);
				dest += SCREENWIDTH;
			}
			column = (column_t *)((byte *)column+column->length+4);
		}
	}			
}

/*
==================
=
= V_DrawAltFuzzPatch
=
= Masks a column based translucent masked pic to the screen.
=
==================
*/
extern byte *tinttable;

void V_DrawAltFuzzPatch (int x, int y, patch_t *patch)
{
	int			count,col;
	column_t	*column;
	dt_scrpix	*desttop, *dest;
	byte		*source;
	int			w;
	
	y -= SHORT(patch->topoffset);
	x -= SHORT(patch->leftoffset);

	if (x<0||x+SHORT(patch->width) >SCREENWIDTH || y<0 
		|| y+SHORT(patch->height)>SCREENHEIGHT
)
	{	
		I_Error ("Bad V_DrawPatch");
	}

	col = 0;
	desttop = screen+y*SCREENWIDTH+x;
	
	w = SHORT(patch->width);
	for ( ; col<w ; x++, col++, desttop++)
	{
		column = (column_t *)((byte *)patch + LONG(patch->columnofs[col]));

// step through the posts in a column
	
		while (column->topdelta != 0xff )
		{
			source = (byte *)column + 3;
			dest = desttop + column->topdelta*SCREENWIDTH;
			count = column->length;
			
			while (count--)
			{
				if(sizeof(dt_scrpix)==1)
					*dest = tinttable[((*dest)<<8) + *source++];
				else
//					*dest = VID_BlendEven16(*dest, colormaps[*source++]);
					*dest = R_PixelBlend(*dest, colormaps[*source++]);
				dest += SCREENWIDTH;
			}
			column = (column_t *)((byte *)column+column->length+4);
		}
	}			
}

/*
==================
=
= V_DrawShadowedPatch
=
= Masks a column based masked pic to the screen.
=
==================
*/

void V_DrawShadowedPatch(int x, int y, patch_t *patch)
{
	int			count,col;
	column_t	*column;
	dt_scrpix	*desttop, *dest;
	byte		*source;
	dt_scrpix	*desttop2, *dest2;
	int			w;
	
	y -= SHORT(patch->topoffset);
	x -= SHORT(patch->leftoffset);

	if (x<0||x+SHORT(patch->width) >SCREENWIDTH || y<0 || y+SHORT(patch->height)>SCREENHEIGHT
)
		I_Error ("Bad V_DrawPatch");

	col = 0;
	desttop = screen+y*SCREENWIDTH+x;
	desttop2 = screen+(y+2)*SCREENWIDTH+x+2;
	
	w = SHORT(patch->width);
	for ( ; col<w ; x++, col++, desttop++, desttop2++)
	{
		column = (column_t *)((byte *)patch + LONG(patch->columnofs[col]));

// step through the posts in a column
	
		while (column->topdelta != 0xff )
		{
			source = (byte *)column + 3;
			dest = desttop + column->topdelta*SCREENWIDTH;
			dest2 = desttop2 + column->topdelta*SCREENWIDTH;
			count = column->length;
			
			while (count--)
			{
				if(sizeof(dt_scrpix)==1)
					*dest2 = tinttable[((*dest2)<<8)];
				else
//					*dest2 = VID_BlendEven16(*dest2, colormaps[0]);
					*dest2 = R_PixelBlend(*dest2, colormaps[0]);
				dest2 += SCREENWIDTH;
//				*dest = *source++;
				*dest = colormaps[*source++];
				dest += SCREENWIDTH;

			}
			column = (column_t *)(  (byte *)column + column->length
+ 4 );
		}
	}			
}

void V_MemCpy_ScrPix(dt_scrpix	*dst, byte *src, int cnt)
{
	byte		*cs, *cse, *cse1;
	dt_scrpix	*ct, *cmap;
	int p0, p1, p2, p3;
	
	cmap=colormaps;
	ct=dst;
	cs=src;
	cse=src+cnt;

	cse1=src+(cnt&(~3));
	while(cs<cse1)
	{
		p0=cs[0];		p1=cs[1];
		p2=cs[2];		p3=cs[3];
		p0=cmap[p0];	p1=cmap[p1];
		p2=cmap[p2];	p3=cmap[p3];
		ct[0]=p0;		ct[1]=p1;
		ct[2]=p2;		ct[3]=p3;

//		ct[0]=cmap[cs[0]];
//		ct[1]=cmap[cs[1]];
//		ct[2]=cmap[cs[2]];
//		ct[3]=cmap[cs[3]];

		ct+=4; cs+=4;
	}

	while(cs<cse)
	{
//		*ct++=colormaps[*cs++];
		*ct++=cmap[*cs++];
	}
}

//---------------------------------------------------------------------------
//
// PROC V_DrawRawScreen
//
//---------------------------------------------------------------------------

void V_DrawRawScreen(byte *raw)
{

	V_MemCpy_ScrPix(screen, raw, SCREENWIDTH*SCREENHEIGHT);

#if 0
	byte		*cs;
	dt_scrpix	*ct;
	int i, n;

	if(sizeof(dt_scrpix)==1)
	{
//		memcpy(screen, raw, SCREENWIDTH*SCREENHEIGHT);
	}else
	{
		n=SCREENWIDTH*SCREENHEIGHT;
//		for(i=0; i<n; i++)
//			{ screen[i]=colormaps[raw[i]]; }

		cs=raw;
		ct=screen;
		i=n;
		while(i>8)
		{
			ct[0]=colormaps[cs[0]];		ct[1]=colormaps[cs[1]];
			ct[2]=colormaps[cs[2]];		ct[3]=colormaps[cs[3]];
			ct[4]=colormaps[cs[4]];		ct[5]=colormaps[cs[5]];
			ct[6]=colormaps[cs[6]];		ct[7]=colormaps[cs[7]];
			cs+=8;		ct+=8;		i-=8;
		}
		while((i--)>0)
			{ *ct++=colormaps[*cs++]; }
	}
#endif
}

//---------------------------------------------------------------------------
//
// PROC V_Init
//
//---------------------------------------------------------------------------

void V_Init(void)
{
	// I_AllocLow will put screen in low dos memory on PCs.
	screen = (dt_scrpix *)I_AllocLow(
		SCREENWIDTH*SCREENHEIGHT*sizeof(dt_scrpix));
}
