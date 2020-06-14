/*
WALLS:
  WALLSTRT
  WALL%d
  WALLSTOP
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef unsigned char byte;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef unsigned short word;

#include "bt1h_targa.c"

#include "sprites.h"
#include "t_wadlist.c"

#define W3D_MAPWAD
#include "w3d_map2rtl.c"

struct {
char *name;
int id;
}soundlist[]=
{
// {"P_APISTL", 12},
{"P_APISTL",	49},
{"P_AMP40",		11},
{"D_OPEN",		10},
{"D_CLOSE",		7},
{"E_ELEVEN",	0},
{"G_WEAPON",	30},
{"E_PWALL",		34},

{"BL_1SEE",		15},
{"BL_2SEE",		15},
{"BL_OUCH",		27},
{"BL_DIE",		46},

{"LG_1SEE",		1},
{"LG_1SEE3",	1},
{"LG_OUCH",		27},
{"LG_1DIE",		26},

{"HG_1SEE",		15},
{"HG_2SEE",		15},
{"HG_OUCH",		27},
{"HG_DIE",		46},

{"FM_OUCH",		0},
{"FM_DIE",		35},
{"FM_SEE",		76},

{"G_LIFE2",		36|4096},
{"G_1UP",		44|4096},
{"G_KEY",		45|4096},

{"MO_OUCH",		35},
{"MO_DIE",		35},
{"MO_SEE",		76},
{"MO_DRAIN",	3|4096},
{NULL, 0}
};

//guard sight: 001
//dog sight: 002 (woof)
//door close: 007
//door open: 010
//machine gun bullet: 011
//player pistol: 012
//chaingun: 013
//ss sight: 015
//chaingun guy sight: 017
//chaingun guy die: 019
//chaingun guy fire: 022
//machine gun fire: 024
//guard die 2: 025 "Aagh"
//guard die 3: 026 "Ow"
//guard hurt: 027 "Ugh"
//pushwall: 034
//dog die: 035
//painful die: 037 "Aaaargh"
//40: dialog
//44: dialog
//guard die 4: 046 "My Namen!"
//enemy pistol: 049
//bong rip: 052
//54: dialog
//61: dialog / die
//65: laugh 1
//69: laugh 2
//71: ? sight
//74: ? dies
//76: dog sight (woof woof)
// 78: ?
// 80: crash/break
// 82: "Yeah!"
// 84: "Schiest!"
// 86: scream / die
// 88: lame die
// 91: dialog / die
// 96: dialog
// 102: dialog
// 103: fart
// 105: cough
// 107: pain
// 109: die
// 112: sight / dialog
// 115: death / dialog
// 119: dialog

struct {
int dspr;
int wspr;
}spritelist[4096]=
{

{LIFEITEMA01, 31},	{LIFEITEMA02, 31},
{LIFEITEMA03, 31},	{LIFEITEMA04, 31},
{LIFEITEMA05, 31},	{LIFEITEMA06, 31},
{LIFEITEMA07, 31},	{LIFEITEMA08, 31},

{LIFEITEMB01, 32},	{LIFEITEMB02, 32},
{LIFEITEMB03, 32},	{LIFEITEMB04, 32},
{LIFEITEMB05, 32},	{LIFEITEMB06, 32},
{LIFEITEMB07, 32},	{LIFEITEMB08, 32},

{LIFEITEMD01, 33},	{LIFEITEMD02, 33},
{LIFEITEMD03, 33},	{LIFEITEMD04, 33},
{LIFEITEMD05, 33},	{LIFEITEMD06, 33},
{LIFEITEMD07, 33},	{LIFEITEMD08, 33},

{LIFEITEMC01, 34},	{LIFEITEMC02, 34},
{LIFEITEMC03, 34},	{LIFEITEMC04, 34},
{LIFEITEMC05, 34},	{LIFEITEMC06, 34},
{LIFEITEMC07, 34},	{LIFEITEMC08, 34},
{LIFEITEMC09, 34},	{LIFEITEMC10, 34},
{LIFEITEMC11, 34},	{LIFEITEMC12, 34},
{LIFEITEMC13, 34},	{LIFEITEMC14, 34},
{LIFEITEMC15, 34},
// {LIFEITEMC16, 34},

{FBASIN1, 27},	{FBASIN2, 27},	{FBASIN3, 27},

{SPR55_IBARREL, 3},
{SPR33_CBARREL, 37},
{SPR9_MONKMEAL, 8},
{PORRIDGE1, 26},
{PORRIDGE2, 26},
{SPR_MP40, 29},

{ONEUP01, 35},
{ONEUP02, 35},
{ONEUP03, 35},
{ONEUP04, 35},
{ONEUP05, 35},
{ONEUP06, 35},
{ONEUP07, 35},
{ONEUP08, 35},

{SPR0_YLIGHT, 16},
{SPR1_RLIGHT, 16},
{SPR2_GLIGHT, 16},
{SPR3_BLIGHT, 16},

{SPR4_CHAND, 6},
{SPR5_LAMPOFF, 5},
{SPR50_TREE, 10},
{SPR51_PLANT, 13},

// {SPR73_GKEY1, 22},
// {SPR73_GKEY2, 22},

{SPR_MONK_DRAIN1, 139},
{SPR_MONK_DRAIN2, 140},
{SPR_MONK_DRAIN3, 141},
{SPR_MONK_DRAIN4, 140},
{SPR_MONK_DRAIN5, 141},
{SPR_MONK_DRAIN6, 139},

{SPR_MONK_S1, 103},		{SPR_MONK_S2, 104},
{SPR_MONK_S3, 105},		{SPR_MONK_S4, 106},
{SPR_MONK_S5, 107},		{SPR_MONK_S6, 108},
{SPR_MONK_S7, 109},		{SPR_MONK_S8, 110},

{SPR_MONK_W11, 103},	{SPR_MONK_W12, 104},
{SPR_MONK_W13, 105},	{SPR_MONK_W14, 106},
{SPR_MONK_W15, 107},	{SPR_MONK_W16, 108},
{SPR_MONK_W17, 109},	{SPR_MONK_W18, 110},

{SPR_MONK_W21, 111},	{SPR_MONK_W22, 112},
{SPR_MONK_W23, 113},	{SPR_MONK_W24, 114},
{SPR_MONK_W25, 115},	{SPR_MONK_W26, 116},
{SPR_MONK_W27, 117},	{SPR_MONK_W28, 118},

{SPR_MONK_W31, 119},	{SPR_MONK_W32, 120},
{SPR_MONK_W33, 121},	{SPR_MONK_W34, 122},
{SPR_MONK_W35, 123},	{SPR_MONK_W36, 124},
{SPR_MONK_W37, 125},	{SPR_MONK_W38, 126},

{SPR_MONK_W41, 127},	{SPR_MONK_W42, 128},
{SPR_MONK_W43, 129},	{SPR_MONK_W44, 130},
{SPR_MONK_W45, 131},	{SPR_MONK_W46, 132},
{SPR_MONK_W47, 133},	{SPR_MONK_W48, 134},

{SPR_MONK_PAIN1, 129},	{SPR_MONK_PAIN2, 135},

{SPR_MONK_DIE1, 135},	{SPR_MONK_DIE2, 135},
{SPR_MONK_DIE3, 136},	{SPR_MONK_DIE4, 137},
{SPR_MONK_DEAD, 138},

{SPR_LOWGRD_SHOOT1, 100},	{SPR_LOWGRD_SHOOT2, 101},
{SPR_LOWGRD_SHOOT3, 101},	{SPR_LOWGRD_SHOOT4, 102},

{SPR_LOWGRD_S1, 54},		{SPR_LOWGRD_S2, 55},
{SPR_LOWGRD_S3, 56},		{SPR_LOWGRD_S4, 57},
{SPR_LOWGRD_S5, 58},		{SPR_LOWGRD_S6, 59},
{SPR_LOWGRD_S7, 60},		{SPR_LOWGRD_S8, 61},

{SPR_LOWGRD_W11, 62},		{SPR_LOWGRD_W12, 63},
{SPR_LOWGRD_W13, 64},		{SPR_LOWGRD_W14, 65},
{SPR_LOWGRD_W15, 66},		{SPR_LOWGRD_W16, 67},
{SPR_LOWGRD_W17, 68},		{SPR_LOWGRD_W18, 69},

{SPR_LOWGRD_W21, 70},		{SPR_LOWGRD_W22, 71},
{SPR_LOWGRD_W23, 72},		{SPR_LOWGRD_W24, 73},
{SPR_LOWGRD_W25, 74},		{SPR_LOWGRD_W26, 75},
{SPR_LOWGRD_W27, 76},		{SPR_LOWGRD_W28, 77},

{SPR_LOWGRD_W31, 78},		{SPR_LOWGRD_W32, 79},
{SPR_LOWGRD_W33, 80},		{SPR_LOWGRD_W34, 81},
{SPR_LOWGRD_W35, 82},		{SPR_LOWGRD_W36, 83},
{SPR_LOWGRD_W37, 84},		{SPR_LOWGRD_W38, 85},

{SPR_LOWGRD_W41, 86},		{SPR_LOWGRD_W42, 87},
{SPR_LOWGRD_W43, 88},		{SPR_LOWGRD_W44, 89},
{SPR_LOWGRD_W45, 90},		{SPR_LOWGRD_W46, 91},
{SPR_LOWGRD_W47, 92},		{SPR_LOWGRD_W48, 93},

{SPR_LOWGRD_PAIN1, 98},		{SPR_LOWGRD_PAIN2, 98},
{SPR_LOWGRD_DIE1, 94},		{SPR_LOWGRD_DIE2, 95},
{SPR_LOWGRD_DIE3, 96},		{SPR_LOWGRD_DIE4, 97},
{SPR_LOWGRD_DEAD, 99},

{SPR_LOWGRD_WPAIN1, 95},	{SPR_LOWGRD_WPAIN2, 95},
{SPR_LOWGRD_WDIE1, 94},		{SPR_LOWGRD_WDIE2, 95},
{SPR_LOWGRD_WDIE3, 96},		{SPR_LOWGRD_WDIE4, 97},
{SPR_LOWGRD_WDEAD, 99},

{SPR_SNEAKY_DEAD, 99},
{SPR_RISE1, 97},			{SPR_RISE2, 96},
{SPR_RISE3, 95},			{SPR_RISE4, 94},

#if 0
{SPR_BLITZ_SHOOT1, 189},	{SPR_BLITZ_SHOOT2, 189},
{SPR_BLITZ_SHOOT3, 189},	{SPR_BLITZ_SHOOT4, 189},

{SPR_BLITZ_S1, 143},		{SPR_BLITZ_S2, 143},
{SPR_BLITZ_S3, 145},		{SPR_BLITZ_S4, 145},
{SPR_BLITZ_S5, 147},		{SPR_BLITZ_S6, 147},
{SPR_BLITZ_S7, 149},		{SPR_BLITZ_S8, 149},

{SPR_BLITZ_W11, 151},		{SPR_BLITZ_W12, 151},
{SPR_BLITZ_W13, 153},		{SPR_BLITZ_W14, 153},
{SPR_BLITZ_W15, 155},		{SPR_BLITZ_W16, 155},
{SPR_BLITZ_W17, 157},		{SPR_BLITZ_W18, 157},

{SPR_BLITZ_W21, 159},		{SPR_BLITZ_W22, 159},
{SPR_BLITZ_W23, 161},		{SPR_BLITZ_W24, 161},
{SPR_BLITZ_W25, 163},		{SPR_BLITZ_W26, 163},
{SPR_BLITZ_W27, 165},		{SPR_BLITZ_W28, 165},

{SPR_BLITZ_W31, 167},		{SPR_BLITZ_W32, 167},
{SPR_BLITZ_W33, 169},		{SPR_BLITZ_W34, 169},
{SPR_BLITZ_W35, 171},		{SPR_BLITZ_W36, 171},
{SPR_BLITZ_W37, 173},		{SPR_BLITZ_W38, 173},

{SPR_BLITZ_W41, 175},		{SPR_BLITZ_W42, 175},
{SPR_BLITZ_W43, 177},		{SPR_BLITZ_W44, 177},
{SPR_BLITZ_W45, 179},		{SPR_BLITZ_W46, 179},
{SPR_BLITZ_W47, 181},		{SPR_BLITZ_W48, 181},

{SPR_BLITZ_PAIN1, 183},		{SPR_BLITZ_PAIN2, 183},
{SPR_BLITZ_DIE1, 183},		{SPR_BLITZ_DIE2, 183},
{SPR_BLITZ_DIE3, 185},		{SPR_BLITZ_DIE4, 185},
{SPR_BLITZ_DEAD1, 187},		{SPR_BLITZ_DEAD2, 187},

{SPR_BLITZ_WPAIN1, 183},	{SPR_BLITZ_WPAIN2, 183},
{SPR_BLITZ_WDIE1, 183},		{SPR_BLITZ_WDIE2, 183},
{SPR_BLITZ_WDIE3, 185},		{SPR_BLITZ_WDIE4, 185},
{SPR_BLITZ_WDEAD1, 187},	{SPR_BLITZ_WDEAD2, 187},
#endif

#if 1
{SPR_HIGHGRD_SHOOT1, 188},	{SPR_HIGHGRD_SHOOT2, 189},
{SPR_HIGHGRD_SHOOT3, 189},	{SPR_HIGHGRD_SHOOT4, 190},

{SPR_HIGHGRD_S1, 142},		{SPR_HIGHGRD_S2, 143},
{SPR_HIGHGRD_S3, 144},		{SPR_HIGHGRD_S4, 145},
{SPR_HIGHGRD_S5, 146},		{SPR_HIGHGRD_S6, 147},
{SPR_HIGHGRD_S7, 148},		{SPR_HIGHGRD_S8, 149},

{SPR_HIGHGRD_W11, 150},		{SPR_HIGHGRD_W12, 151},
{SPR_HIGHGRD_W13, 152},		{SPR_HIGHGRD_W14, 153},
{SPR_HIGHGRD_W15, 154},		{SPR_HIGHGRD_W16, 155},
{SPR_HIGHGRD_W17, 156},		{SPR_HIGHGRD_W18, 157},

{SPR_HIGHGRD_W21, 158},		{SPR_HIGHGRD_W22, 159},
{SPR_HIGHGRD_W23, 160},		{SPR_HIGHGRD_W24, 161},
{SPR_HIGHGRD_W25, 162},		{SPR_HIGHGRD_W26, 163},
{SPR_HIGHGRD_W27, 164},		{SPR_HIGHGRD_W28, 165},

{SPR_HIGHGRD_W31, 166},		{SPR_HIGHGRD_W32, 167},
{SPR_HIGHGRD_W33, 168},		{SPR_HIGHGRD_W34, 169},
{SPR_HIGHGRD_W35, 170},		{SPR_HIGHGRD_W36, 171},
{SPR_HIGHGRD_W37, 172},		{SPR_HIGHGRD_W38, 173},

{SPR_HIGHGRD_W41, 174},		{SPR_HIGHGRD_W42, 175},
{SPR_HIGHGRD_W43, 176},		{SPR_HIGHGRD_W44, 177},
{SPR_HIGHGRD_W45, 178},		{SPR_HIGHGRD_W46, 179},
{SPR_HIGHGRD_W47, 180},		{SPR_HIGHGRD_W48, 181},

{SPR_HIGHGRD_PAIN1, 186},		{SPR_HIGHGRD_PAIN2, 186},
{SPR_HIGHGRD_DIE1, 182},		{SPR_HIGHGRD_DIE2, 183},
{SPR_HIGHGRD_DIE3, 184},		{SPR_HIGHGRD_DIE4, 185},
{SPR_HIGHGRD_DEAD, 187},

{SPR_HIGHGRD_WPAIN1, 186},		{SPR_HIGHGRD_WPAIN2, 186},
{SPR_HIGHGRD_WDIE1, 182},		{SPR_HIGHGRD_WDIE2, 183},
{SPR_HIGHGRD_WDIE3, 184},		{SPR_HIGHGRD_WDIE4, 185},
{SPR_HIGHGRD_WDEAD, 187},
#endif

#if 1
{SPR_STRIKE_SHOOT1, 289},	{SPR_STRIKE_SHOOT2, 290},
{SPR_STRIKE_SHOOT3, 291},	{SPR_STRIKE_SHOOT4, 290},

{SPR_STRIKE_S1, 242},		{SPR_STRIKE_S2, 243},
{SPR_STRIKE_S3, 244},		{SPR_STRIKE_S4, 245},
{SPR_STRIKE_S5, 246},		{SPR_STRIKE_S6, 247},
{SPR_STRIKE_S7, 248},		{SPR_STRIKE_S8, 249},

{SPR_STRIKE_W11, 250},		{SPR_STRIKE_W12, 251},
{SPR_STRIKE_W13, 252},		{SPR_STRIKE_W14, 253},
{SPR_STRIKE_W15, 254},		{SPR_STRIKE_W16, 255},
{SPR_STRIKE_W17, 256},		{SPR_STRIKE_W18, 257},

{SPR_STRIKE_W21, 258},		{SPR_STRIKE_W22, 259},
{SPR_STRIKE_W23, 260},		{SPR_STRIKE_W24, 261},
{SPR_STRIKE_W25, 262},		{SPR_STRIKE_W26, 263},
{SPR_STRIKE_W27, 264},		{SPR_STRIKE_W28, 265},

{SPR_STRIKE_W31, 266},		{SPR_STRIKE_W32, 267},
{SPR_STRIKE_W33, 268},		{SPR_STRIKE_W34, 269},
{SPR_STRIKE_W35, 270},		{SPR_STRIKE_W36, 271},
{SPR_STRIKE_W37, 272},		{SPR_STRIKE_W38, 273},

{SPR_STRIKE_W41, 274},		{SPR_STRIKE_W42, 275},
{SPR_STRIKE_W43, 276},		{SPR_STRIKE_W44, 277},
{SPR_STRIKE_W45, 278},		{SPR_STRIKE_W46, 279},
{SPR_STRIKE_W47, 280},		{SPR_STRIKE_W48, 281},

{SPR_STRIKE_PAIN1, 274},	{SPR_STRIKE_PAIN2, 282},
{SPR_STRIKE_DIE1,  283},	{SPR_STRIKE_DIE2,  284},
{SPR_STRIKE_DIE3,  285},	{SPR_STRIKE_DIE4,  287},
{SPR_STRIKE_DEAD1, 288},	{SPR_STRIKE_DEAD2, 288},

{SPR_STRIKE_WPAIN1, 274},	{SPR_STRIKE_WPAIN2, 282},
{SPR_STRIKE_WDIE1,  283},	{SPR_STRIKE_WDIE2,  284},
{SPR_STRIKE_WDIE3,  285},	{SPR_STRIKE_WDIE4,  287},
{SPR_STRIKE_WDEAD1, 288},	{SPR_STRIKE_WDEAD2, 288},
#endif


#if 0
{SPR_WILLIAM_SHOOT1, 189},	{SPR_WILLIAM_SHOOT2, 189},
{SPR_WILLIAM_SHOOT3, 189},	{SPR_WILLIAM_SHOOT4, 189},

{SPR_WILLIAM_S1, 143},		{SPR_WILLIAM_S2, 143},
{SPR_WILLIAM_S3, 145},		{SPR_WILLIAM_S4, 145},
{SPR_WILLIAM_S5, 147},		{SPR_WILLIAM_S6, 147},
{SPR_WILLIAM_S7, 149},		{SPR_WILLIAM_S8, 149},

{SPR_WILLIAM_W11, 151},		{SPR_WILLIAM_W12, 151},
{SPR_WILLIAM_W13, 153},		{SPR_WILLIAM_W14, 153},
{SPR_WILLIAM_W15, 155},		{SPR_WILLIAM_W16, 155},
{SPR_WILLIAM_W17, 157},		{SPR_WILLIAM_W18, 157},

{SPR_WILLIAM_W21, 159},		{SPR_WILLIAM_W22, 159},
{SPR_WILLIAM_W23, 161},		{SPR_WILLIAM_W24, 161},
{SPR_WILLIAM_W25, 163},		{SPR_WILLIAM_W26, 163},
{SPR_WILLIAM_W27, 165},		{SPR_WILLIAM_W28, 165},

{SPR_WILLIAM_W31, 167},		{SPR_WILLIAM_W32, 167},
{SPR_WILLIAM_W33, 169},		{SPR_WILLIAM_W34, 169},
{SPR_WILLIAM_W35, 171},		{SPR_WILLIAM_W36, 171},
{SPR_WILLIAM_W37, 173},		{SPR_WILLIAM_W38, 173},

{SPR_WILLIAM_W41, 175},		{SPR_WILLIAM_W42, 175},
{SPR_WILLIAM_W43, 177},		{SPR_WILLIAM_W44, 177},
{SPR_WILLIAM_W45, 179},		{SPR_WILLIAM_W46, 179},
{SPR_WILLIAM_W47, 181},		{SPR_WILLIAM_W48, 181},

{SPR_WILLIAM_PAIN1, 183},		{SPR_WILLIAM_PAIN2, 183},
{SPR_WILLIAM_DIE1, 183},		{SPR_WILLIAM_DIE2, 183},
{SPR_WILLIAM_DIE3, 185},		{SPR_WILLIAM_DIE4, 185},
{SPR_WILLIAM_DEAD1, 187},		{SPR_WILLIAM_DEAD2, 187},

{SPR_WILLIAM_WPAIN1, 183},	{SPR_WILLIAM_WPAIN2, 183},
{SPR_WILLIAM_WDIE1, 183},		{SPR_WILLIAM_WDIE2, 183},
{SPR_WILLIAM_WDIE3, 185},		{SPR_WILLIAM_WDIE4, 185},
{SPR_WILLIAM_WDEAD1, 187},	{SPR_WILLIAM_WDEAD2, 187},
#endif

{0, 0}
};

int LumpIWadIndexForName(char *str)
{
	int i, j, k;
	
	for(i=0; wadlist[i]; i++)
	{
		if(!strcmp(wadlist[i], str))
			return(i);
	}
	return(-1);
}

char *LumpNameForSprite(int spr)
{
	static int shapestart=-1;

	if(shapestart<0)
	{
		shapestart=LumpIWadIndexForName("SHAPSTRT");
	}
	
	return(wadlist[shapestart+spr]);
//	return(NULL);
}

typedef struct
{
        char            identification[4];              // should be IWAD
        int                     numlumps;
        int                     infotableofs;
} wadinfo_t;


typedef struct
{
        int                     filepos;
        int                     size;
        char            name[8];
} filelump_t;


byte		*pal;
u16			pal_15to8[65536];
u16			pal_15to8_ls[65536];

FILE		*wad_fd;
filelump_t  wad_dir[2048];
wadinfo_t	wad_head;
int 		wad_n_lumps;

byte		*wad_data;
int			wad_rover;

u16 *wall_rgb555;
u16 *wall_rgb555hi;
byte *wall_rgb8;

u16 *wall_mort128;

byte *wall_patch8;
int wall_patch8_sz;


u16 *spr_rgb555;
u16 *spr_rgb555hi;
byte *spr_rgb8;
int spr_xs;
int spr_ys;

byte *spr_patch8;
int spr_patch8_sz;

void w_strupr_n (char *t, char *s, int n)
{
	int i;
	for(i=0; *s && (i<n); i++)
		{ *t++ = toupper(*s++); }
	for(; i<n; i++)
		*t++=0;
}

int AddWadLump(char *name, void *buf, int sz)
{
	char tn[9];
	int n;
	
	w_strupr_n(tn, name, 8);
	
	n=wad_n_lumps++;
	wad_dir[n].filepos=wad_rover;
	wad_dir[n].size=sz;
	memcpy(wad_dir[n].name, tn, 8);
	
	memcpy(wad_data+wad_rover, buf, sz);
	wad_rover+=sz;
	wad_rover=(wad_rover+15)&(~15);
	
	return(n);
}

void *LoadFile(char *name, int *rsz)
{
	FILE *fd;
	void *buf;
	int sz;
	
	fd=fopen(name, "rb");
	if(!fd)return(NULL);
	fseek(fd, 0, 2);
	sz=ftell(fd);
	fseek(fd, 0, 0);
	buf=malloc(sz+16);
	fread(buf, 1, sz, fd);
	fclose(fd);
	*rsz=sz;
	return(buf);
}

void StoreFile(char *name, void *buf, int sz)
{
	FILE *fd;
	
	fd=fopen(name, "wb");
	if(!fd)return;
	fwrite(buf, 1, sz, fd);
	fclose(fd);
}

u32 DecodeBlockPvr_Unpack555to32(u16 px)
{
	int cr, cg, cb;
	u32 pxc;
	cr=(px>>10)&31;	cr=(cr<<3)|(cr>>2);
	cg=(px>> 5)&31;	cg=(cg<<3)|(cg>>2);
	cb=(px>> 0)&31;	cb=(cb<<3)|(cb>>2);
	pxc=(cr<<16)|(cg<<8)|cb;
	return(pxc);
}

u16 DecodeBlockPvr_Pack555from32(u32 px)
{
	int cr, cg, cb;
	int pxc;
	cr=(px>>16)&255;
	cg=(px>> 8)&255;
	cb=(px>> 0)&255;
	
	pxc=((cr<<7)&0x7C00)|((cg<<2)&0x03E0)|(cb>>3);
	return(pxc);
}

void DecodeBlockPvr_InterpTab4(u16 pxa, u16 pxb, u16 *pxt)
{
	u32 upxa, upxb, upxc, upxd;
	int cra, cga, cba;
	int crb, cgb, cbb;
	int cr, cg, cb;

	pxt[0]=pxa;
	pxt[3]=pxb;
	
	cra=(pxa>>10)&31;	cra=(cra<<3)|(cra>>2);
	cga=(pxa>> 5)&31;	cga=(cga<<3)|(cga>>2);
	cba=(pxa>> 0)&31;	cba=(cba<<3)|(cba>>2);
	crb=(pxb>>10)&31;	crb=(crb<<3)|(crb>>2);
	cgb=(pxb>> 5)&31;	cgb=(cgb<<3)|(cgb>>2);
	cbb=(pxb>> 0)&31;	cbb=(cbb<<3)|(cbb>>2);

	cr=((5*cra)+(3*crb))/8;
	cg=((5*cga)+(3*cgb))/8;
	cb=((5*cba)+(3*cbb))/8;
	pxt[1]=((cr<<7)&0x7C00)|((cg<<2)&0x03E0)|((cb>>3)&0x001F);

	cr=((3*cra)+(5*crb))/8;
	cg=((3*cga)+(5*cgb))/8;
	cb=((3*cba)+(5*cbb))/8;
	pxt[2]=((cr<<7)&0x7C00)|((cg<<2)&0x03E0)|((cb>>3)&0x001F);
}

int DecodeBlockPvr(u64 bx0, u64 bx1, u64 bx2, u64 bx3, u16 *blk)
{
	u16 clr01a[4], clr01b[4];
	u16 clr23a[4], clr23b[4];
	u16 clr02a[4], clr02b[4];
	u16 clr13a[4], clr13b[4];
	u16 clrat[16], clrbt[16], clrt[4];
	u16 clr0a, clr0b, clr1a, clr1b, clr2a, clr2b, clr3a, clr3b;
	u32 pix0, pix1, pix2, pix3;
	int i, j, k;
	
	clr0a=(bx0>>48)&65535;	clr0b=(bx0>>32)&65535;	pix0=bx0&0xFFFFFFFF;
	clr1a=(bx1>>48)&65535;	clr1b=(bx1>>32)&65535;	pix1=bx1&0xFFFFFFFF;
	clr2a=(bx2>>48)&65535;	clr2b=(bx2>>32)&65535;	pix2=bx2&0xFFFFFFFF;
	clr3a=(bx3>>48)&65535;	clr3b=(bx3>>32)&65535;	pix3=bx3&0xFFFFFFFF;
	
	DecodeBlockPvr_InterpTab4(clr0a, clr1a, clr01a);
	DecodeBlockPvr_InterpTab4(clr0b, clr1b, clr01b);
	DecodeBlockPvr_InterpTab4(clr2a, clr3a, clr23a);
	DecodeBlockPvr_InterpTab4(clr2b, clr3b, clr23b);

	DecodeBlockPvr_InterpTab4(clr0a, clr2a, clr02a);
	DecodeBlockPvr_InterpTab4(clr0b, clr2b, clr02b);
	DecodeBlockPvr_InterpTab4(clr1a, clr3a, clr13a);
	DecodeBlockPvr_InterpTab4(clr1b, clr3b, clr13b);

//	DecodeBlockPvr_InterpTab4(clr01a[1], clr23a[1], clrt02a);
//	DecodeBlockPvr_InterpTab4(clr01b[1], clr23b[1], clrt02b);
//	DecodeBlockPvr_InterpTab4(clr01a[2], clr23a[2], clrt13a);
//	DecodeBlockPvr_InterpTab4(clr01b[2], clr23b[2], clrt13b);

	for(i=0; i<4; i++)
	{
//		DecodeBlockPvr_InterpTab4(clr01a[i], clr23a[i], clrat+(i*4));
//		DecodeBlockPvr_InterpTab4(clr01b[i], clr23b[i], clrbt+(i*4));

		DecodeBlockPvr_InterpTab4(clr02a[i], clr13a[i], clrat+(i*4));
		DecodeBlockPvr_InterpTab4(clr02b[i], clr13b[i], clrbt+(i*4));
	}

	for(i=0; i<4; i++)
		for(j=0; j<4; j++)
	{
//		DecodeBlockPvr_InterpTab4(clrat[i*4+j], clrbt[i*4+j], clrt);
		DecodeBlockPvr_InterpTab4(
			clrat[(i/2+1)*4+(j/2+1)],
			clrbt[(i/2+1)*4+(j/2+1)], clrt);

//		clrt[0]=0x0000;	//0000-0000-0000-0000
//		clrt[1]=0x294A;	//0010-1001-0100-1010
//		clrt[2]=0x56B5;	//0101-0110-1011-0101
//		clrt[3]=0x7FFF; //0111-1111-1111-1111
		
//		k=(pix0>>(30-((i*4+j)*2)))&3;
		k=(pix0>>((i*4+j)*2))&3;
//		blk[i*4+j]=clrt[k];
		blk[i*4+j]=clrt[3-k];
//		blk[i*4+j]=clrt[1];
	}
}

int DecodeBlockPvr_BlendEven555(int clra, int clrb)
{
	int clrc;
	clrc=((clra&0x7BDE)+(clrb&0x7BDE))>>1;
	return(clrc);
}

int DecodeBlockPvr_Avg2(u64 bx0, u64 bx1, u64 bx2, u64 bx3, u16 *blk)
{
	u16 pxt[16];
	DecodeBlockPvr(bx0, bx1, bx2, bx3, pxt);
	
	blk[0]=DecodeBlockPvr_BlendEven555(
		DecodeBlockPvr_BlendEven555(pxt[ 0], pxt[ 1]),
		DecodeBlockPvr_BlendEven555(pxt[ 4], pxt[ 5]));
	blk[1]=DecodeBlockPvr_BlendEven555(
		DecodeBlockPvr_BlendEven555(pxt[ 2], pxt[ 3]),
		DecodeBlockPvr_BlendEven555(pxt[ 6], pxt[ 7]));
	blk[2]=DecodeBlockPvr_BlendEven555(
		DecodeBlockPvr_BlendEven555(pxt[ 8], pxt[ 9]),
		DecodeBlockPvr_BlendEven555(pxt[12], pxt[13]));
	blk[3]=DecodeBlockPvr_BlendEven555(
		DecodeBlockPvr_BlendEven555(pxt[10], pxt[11]),
		DecodeBlockPvr_BlendEven555(pxt[14], pxt[15]));
}

// static int wall_mort16[16]=
// {
// };

static int wall_mortpadx[64]={
0x000, 0x001, 0x004, 0x005, 0x010, 0x011, 0x014, 0x015,
0x040, 0x041, 0x044, 0x045, 0x050, 0x051, 0x054, 0x055,
0x100, 0x101, 0x104, 0x105, 0x110, 0x111, 0x114, 0x115,
0x140, 0x141, 0x144, 0x145, 0x150, 0x151, 0x154, 0x155,
0x400, 0x401, 0x404, 0x405, 0x410, 0x411, 0x414, 0x415,
0x440, 0x441, 0x444, 0x445, 0x450, 0x451, 0x454, 0x455,
0x500, 0x501, 0x504, 0x505, 0x510, 0x511, 0x514, 0x515,
0x540, 0x541, 0x544, 0x545, 0x550, 0x551, 0x554, 0x555
};

int ReadWall(int num)
{
	byte tbh[64];
	char tb[256];
	FILE *fd;
	int is_pvr;
	u64 *bxp;
	int ix0, ix1, ix2, ix3;
	u64 bx0, bx1, bx2, bx3;
	u32 pix;
	u16 pxt[16];
	byte clrt[4];
	u16 *hitga;
	
	int clra, clrb, i0, i1, usels;
	int i, j, k, l;

	if(!wall_rgb555)
	{
		wall_rgb555=malloc(64*64*2);
		wall_rgb8=malloc(64*64);
		wall_rgb555hi=malloc(128*128*2);
		wall_patch8=malloc(128*128*2);
		
		wall_mort128=malloc(32*32*2);
		
		for(i=0; i<32; i++)
			for(j=0; j<32; j++)
		{
			k=(wall_mortpadx[j]<<1)|wall_mortpadx[i];
			wall_mort128[i*32+j]=k;
		}
	}


	if(!(num&1))
		return(-1);
	
	hitga=NULL;
	
#if 0
	sprintf(tb, "f3d_base/walls/%03d.tga", (num/2)+1);
	hitga=BTIC1H_Img_LoadHiTGA(tb, &j, &k);
#endif

	if(hitga)
	{
		usels=0;
		is_pvr=0;

		for(i=0; i<64; i++)
			for(j=0; j<64; j++)
		{
			wall_rgb555[i*64+j]=(hitga[(63-i)*64+j]<<1)+1;
		}

		free(hitga);

	}else
	{
		sprintf(tb, "w3d_base/walls/%03d.5551", num);
		fd=fopen(tb, "rb");
		if(!fd)
		{
			printf("Fail Open %s\n", tb);
			return(-1);
		}
	
		is_pvr=0;
		fread(tbh, 1, 64, fd);
	//	if((tb[0]!=64) || (tb[1]!=64))
	//	if(0)
		if((tbh[0]==3) && (tbh[4]==128) && (tbh[8]==128))
		{
			printf("PVRTC\n");
			is_pvr=1;
		}else
			if(((tbh[4]!=64) || (tbh[8]!=64)) && (num!=31) && (num!=41))
		{
			fclose(fd);
			return(-1);
		}
		
		printf("%d/%d: %d %d\n", num, num/2, tbh[4], tbh[8]);
		
	//	fseek(fd, 2, 0);
		fseek(fd, 0x38, 0);
		fread(wall_rgb555, 2, 4096, fd);
		fclose(fd);

		usels=0;
		j=(num/2+1);
		if(j==14)	usels=1;
		if(j==15)	usels=1;
		if(j==64)	usels=1;
		if(j==65)	usels=1;

	//	usels=1;

	}


	if(is_pvr)
	{
		bxp=(u64 *)wall_rgb555;

#if 1
		for(i=0; i<32; i++)
			for(j=0; j<32; j++)
		{
			ix0=(((i+0)&31)*32)+((j+0)&31);
			ix1=(((i+0)&31)*32)+((j+1)&31);
			ix2=(((i+1)&31)*32)+((j+0)&31);
			ix3=(((i+1)&31)*32)+((j+1)&31);

			ix0=wall_mort128[ix0];
			ix1=wall_mort128[ix1];
			ix2=wall_mort128[ix2];
			ix3=wall_mort128[ix3];
		
			bx0=bxp[ix0];
			bx1=bxp[ix1];
			bx2=bxp[ix2];
			bx3=bxp[ix3];
//			DecodeBlockPvr_Avg2(bx0, bx1, bx2, bx3, pxt);
			DecodeBlockPvr(bx0, bx1, bx2, bx3, pxt);

			for(k=0; k<16; k++)
			{
				i0=(k>>2)&3;	i1=(k   )&3;
				wall_rgb555hi[(i*4+i0)*128+(j*4+i1)]=(pxt[i0*4+i1]<<1)+1;
			}

			DecodeBlockPvr_Avg2(bx0, bx1, bx2, bx3, pxt);

			wall_rgb8[(j*2+0)*64+(i*2+0)]=pal_15to8[((pxt[0]<<1)+1)&65535];
			wall_rgb8[(j*2+1)*64+(i*2+0)]=pal_15to8[((pxt[1]<<1)+1)&65535];
			wall_rgb8[(j*2+0)*64+(i*2+1)]=pal_15to8[((pxt[2]<<1)+1)&65535];
			wall_rgb8[(j*2+1)*64+(i*2+1)]=pal_15to8[((pxt[3]<<1)+1)&65535];

#if 0
			k=(i*32+j)*4;
//			clra=wall_rgb555[k+0];
//			clrb=wall_rgb555[k+1];
//			pix=wall_rgb555[k+2]|(wall_rgb555[k+3]<<8);

			clra=wall_rgb555[k+2];
			clrb=wall_rgb555[k+3];
			pix=wall_rgb555[k+0]|(wall_rgb555[k+1]<<16);

			clrt[0]=pal_15to8[(clra<<1)&65535];
			clrt[3]=pal_15to8[(clrb<<1)&65535];
			k=((clra&0x7BDE)+(clrb&0x7BDE))>>1;
			l=((clra&0x7BDE)+(k&0x7BDE))>>1;
			clrt[1]=pal_15to8[(l<<1)&65535];
			l=((clrb&0x7BDE)+(k&0x7BDE))>>1;
			clrt[2]=pal_15to8[(l<<1)&65535];
			
			wall_rgb8[(j*2+0)*64+(i*2+0)]=clrt[(pix>>30)&3];
			wall_rgb8[(j*2+0)*64+(i*2+1)]=clrt[(pix>>26)&3];
			wall_rgb8[(j*2+1)*64+(i*2+0)]=clrt[(pix>>14)&3];
			wall_rgb8[(j*2+1)*64+(i*2+1)]=clrt[(pix>>10)&3];
#endif
		}
#endif

		return(1);
	}
	
	for(i=0; i<64; i++)
		for(j=0; j<64; j++)
	{
		k=wall_rgb555[j*64+i];
		wall_rgb555hi[(j*2+0)*128+(i*2+0)]=k/2;
		wall_rgb555hi[(j*2+0)*128+(i*2+1)]=k/2;
		wall_rgb555hi[(j*2+1)*128+(i*2+0)]=k/2;
		wall_rgb555hi[(j*2+1)*128+(i*2+1)]=k/2;

		if(usels)
			k=pal_15to8_ls[k];
		else
			k=pal_15to8[k];
		k=((i^j)&1)?(k>>8):k;
		wall_rgb8[i*64+j]=k;
//		wall_rgb8[i*64+j]=pal_15to8[wall_rgb555[j*64+((i+32)&63)]];
	}
	
	memset(wall_patch8, 255, 128*128*2);
//	memcpy(wall_patch8+650, wall_rgb8, 4096);
//	memset(wall_patch8+650, 255, 4096);

//	wall_patch8_sz=4096+768;
	wall_patch8_sz=672+65*68;

	for(i=0; i<64; i++)
	{
		memcpy(wall_patch8+672+i*68+2, wall_rgb8+i*64, 64);
//		memcpy(wall_patch8+672+i*68, wall_rgb8+i*64, 16);
		wall_patch8[672+i*68+0]=0;
		wall_patch8[672+i*68+1]=64;
	}
	
	((u16 *)wall_patch8)[0]=64;
	((u16 *)wall_patch8)[1]=64;
	((u16 *)wall_patch8)[2]=64;
	((u16 *)wall_patch8)[3]=0;
//	((u16 *)wall_patch8)[4]=64;
	((u16 *)wall_patch8)[4]=0;
//	for(i=0; i<64; i++)
	for(i=0; i<320; i++)
	{
//		((u16 *)wall_patch8)[5+i]=650+(i*64);
//		((u16 *)wall_patch8)[5+i]=650+((i&63)*64);
		((u16 *)wall_patch8)[5+i]=672+((i&63)*68);
	}
//	((u16 *)wall_patch8)[5+i]=650;

//	wall_rgb8[0]=0x40;
//	wall_rgb8[1]=0x40;
	
	return(1);
}


int ReadSprite(int num)
{
	byte tbh[64];
	char tb[256];
	FILE *fd;
	int is_pvr;
	u64 *bxp;
	int ix0, ix1, ix2, ix3;
	u64 bx0, bx1, bx2, bx3;
	u32 pix;
	u16 pxt[16];
	byte clrt[4];
	byte *ct, *cte, *cs, *cse, *cs0;
	int clra, clrb, i0, i1, usels;
	int i, j, k, l;
	
//	if(!(num&1))
//		return(-1);
	
	sprintf(tb, "w3d_base/sprites/%03d.5551", num);
	fd=fopen(tb, "rb");
	if(!fd)
	{
		printf("Fail Open %s\n", tb);
		return(-1);
	}

	if(!spr_rgb555)
	{
		spr_rgb555=malloc(128*256*2);
		spr_rgb8=malloc(128*256);
//		wall_rgb555hi=malloc(128*128*2);
		spr_patch8=malloc(256*512*2);
	}

	usels=0;

//	j=(num/2+1);
//	if(j==14)	usels=1;
//	if(j==15)	usels=1;
//	if(j==64)	usels=1;
//	if(j==65)	usels=1;

//	usels=1;

	
	is_pvr=0;
	fread(tbh, 1, 64, fd);
//	if((tb[0]!=64) || (tb[1]!=64))
//	if(0)
//	if((tbh[0]==3) && (tbh[4]==128) && (tbh[8]==128))
	if(tbh[0]==3)
	{
		printf("PVRTC\n");
		is_pvr=1;
	}
//	else
//		if(((tbh[4]!=64) || (tbh[8]!=64)) && (num!=31) && (num!=41))
//	{
//		fclose(fd);
//		return(-1);
//	}

	spr_xs=*(int *)(tbh+4);
	spr_ys=*(int *)(tbh+8);
	
	printf("Spr %d/%d: %dx%d\n", num, num/2, spr_xs, spr_ys);
	
//	fseek(fd, 2, 0);
	fseek(fd, 0x38, 0);
	fread(spr_rgb555, 2, spr_xs*spr_ys*2, fd);
	fclose(fd);
	
	if(is_pvr)
	{
		bxp=(u64 *)wall_rgb555;

#if 0
		for(i=0; i<32; i++)
			for(j=0; j<32; j++)
		{
			ix0=(((i+0)&31)*32)+((j+0)&31);
			ix1=(((i+0)&31)*32)+((j+1)&31);
			ix2=(((i+1)&31)*32)+((j+0)&31);
			ix3=(((i+1)&31)*32)+((j+1)&31);

			ix0=wall_mort128[ix0];
			ix1=wall_mort128[ix1];
			ix2=wall_mort128[ix2];
			ix3=wall_mort128[ix3];
		
			bx0=bxp[ix0];
			bx1=bxp[ix1];
			bx2=bxp[ix2];
			bx3=bxp[ix3];
//			DecodeBlockPvr_Avg2(bx0, bx1, bx2, bx3, pxt);
			DecodeBlockPvr(bx0, bx1, bx2, bx3, pxt);

			for(k=0; k<16; k++)
			{
				i0=(k>>2)&3;	i1=(k   )&3;
				wall_rgb555hi[(i*4+i0)*128+(j*4+i1)]=pxt[i0*4+i1];
			}

			DecodeBlockPvr_Avg2(bx0, bx1, bx2, bx3, pxt);

			wall_rgb8[(j*2+0)*64+(i*2+0)]=pal_15to8[(pxt[0]<<1)&65535];
			wall_rgb8[(j*2+1)*64+(i*2+0)]=pal_15to8[(pxt[1]<<1)&65535];
			wall_rgb8[(j*2+0)*64+(i*2+1)]=pal_15to8[(pxt[2]<<1)&65535];
			wall_rgb8[(j*2+1)*64+(i*2+1)]=pal_15to8[(pxt[3]<<1)&65535];

#if 0
			k=(i*32+j)*4;
//			clra=wall_rgb555[k+0];
//			clrb=wall_rgb555[k+1];
//			pix=wall_rgb555[k+2]|(wall_rgb555[k+3]<<8);

			clra=wall_rgb555[k+2];
			clrb=wall_rgb555[k+3];
			pix=wall_rgb555[k+0]|(wall_rgb555[k+1]<<16);

			clrt[0]=pal_15to8[(clra<<1)&65535];
			clrt[3]=pal_15to8[(clrb<<1)&65535];
			k=((clra&0x7BDE)+(clrb&0x7BDE))>>1;
			l=((clra&0x7BDE)+(k&0x7BDE))>>1;
			clrt[1]=pal_15to8[(l<<1)&65535];
			l=((clrb&0x7BDE)+(k&0x7BDE))>>1;
			clrt[2]=pal_15to8[(l<<1)&65535];
			
			wall_rgb8[(j*2+0)*64+(i*2+0)]=clrt[(pix>>30)&3];
			wall_rgb8[(j*2+0)*64+(i*2+1)]=clrt[(pix>>26)&3];
			wall_rgb8[(j*2+1)*64+(i*2+0)]=clrt[(pix>>14)&3];
			wall_rgb8[(j*2+1)*64+(i*2+1)]=clrt[(pix>>10)&3];
#endif
		}
#endif

		return(1);
	}
	
	for(i=0; i<spr_ys; i++)
		for(j=0; j<spr_xs; j++)
	{
		k=spr_rgb555[i*spr_xs+j];

		if(usels)
			k=pal_15to8_ls[k];
		else
			k=pal_15to8[k];
		k=((i^j)&1)?(k>>8):k;
		spr_rgb8[j*spr_ys+i]=k;
	}

//	memset(spr_patch8, 255, 128*128*2);

	((u16 *)spr_patch8)[0]=spr_ys;
	((u16 *)spr_patch8)[1]=spr_xs;
	((u16 *)spr_patch8)[2]=spr_ys;
	((u16 *)spr_patch8)[3]=0;
//	((u16 *)spr_patch8)[4]=64;
	((u16 *)spr_patch8)[4]=0;

	ct=spr_patch8+672;
	cte=ct+256*512*2;
	for(i=0; i<spr_xs; i++)
	{
		((u16 *)spr_patch8)[5+i]=ct-spr_patch8;
		cs=spr_rgb8+i*spr_ys;
		cs0=cs;
		cse=cs+spr_ys;
		while(cs<cse)
		{
			for(j=0; ((cs+j)<cse) && (cs[j]==255); j++);
			for(k=j; ((cs+k)<cse) && (cs[k]!=255); k++);
			if(!(k-j))
				break;
//			*ct++=j;
			*ct++=(cs-cs0)+j;
			*ct++=k-j;
			memcpy(ct, cs+j, k-j);
			ct+=k-j;
			cs+=k;
		}
		*ct++=0xFF;
		*ct++=0xFF;
	}
	for(; i<320; i++)
	{
		((u16 *)spr_patch8)[5+i]=((u16 *)spr_patch8)[5+i-spr_xs];
	}
	
	if(ct>cte)
		__debugbreak();
	
	spr_patch8_sz=ct-spr_patch8;
	
	printf("ReadSprite: %d Patch Size=%d\n", num, spr_patch8_sz);
	
	return(1);
}


int PatchShiftLeft(int amt)
{
	int i, j, k;

	for(i=0; i<320; i++)
	{
		j=(i&63)+amt;
		if(j>=64)j=64;
//		((u16 *)wall_patch8)[5+i]=650+(j*64);
		((u16 *)wall_patch8)[5+i]=672+(j*68);
	}
}

int PatchShiftRight(int amt)
{
	int i, j, k;

	for(i=0; i<320; i++)
	{
		j=(i&63)-amt;
		if(j<0)j=64;
//		((u16 *)wall_patch8)[5+i]=650+(j*64);
		((u16 *)wall_patch8)[5+i]=672+(j*68);
	}
}

int SetupPalette()
{
	int cr, cg, cb, cy, d, bi, bd, bi2, bd2;
	int dr, dg, db;
	int i, j, k, n;
	
	pal=LoadFile("palette.lmp", &k);

#if 0
	for(i=0; i<256; i++)
	{
		cr=pal[i*3+0];
		cg=pal[i*3+1];
		cb=pal[i*3+2];

		cr<<=2;
		cg<<=2;
		cb<<=2;

		pal[i*3+0]=cr;
		pal[i*3+1]=cg;
		pal[i*3+2]=cb;
	}
#endif

	for(i=0; i<32768; i++)
	{
		cr=(i>>10)&31;	cr=(cr<<3)|(cr>>2);
		cg=(i>> 5)&31;	cg=(cg<<3)|(cg>>2);
		cb=(i>> 0)&31;	cb=(cb<<3)|(cb>>2);
		
		bi=0; bd=1<<24;
//		for(j=0; j<256; j++)
//		for(j=0; j<224; j++)
		for(j=0; j<252; j++)
		{
			dr=cr-pal[j*3+0];
			dg=cg-pal[j*3+1];
			db=cb-pal[j*3+2];
			d=(dr*dr)+2*(dg*dg)+(db*db);
			
			if(d<bd)
				{ bi2=bi; bd2=bd; bi=j; bd=d; }
			else if(d<bd2)
				{ bi2=j; bd2=d; }
		}
		
//		pal_15to8[i]=bi;
//		pal_15to8[i+32768]=bi;

		if((bd*1.25)>bd2)
		{
			bi=bi|(bi2<<8);
		}else
		{
			bi=bi|(bi<<8);
		}

//		pal_15to8[i*2+0]=bi;
		pal_15to8[i*2+0]=0xFFFF;
		pal_15to8[i*2+1]=bi;
//		pal_15to8[i*2+1]=0xFFFF;
	}


	for(i=0; i<32768; i++)
	{
		cr=(i>>10)&31;	cr=(cr<<3)|(cr>>2);
		cg=(i>> 5)&31;	cg=(cg<<3)|(cg>>2);
		cb=(i>> 0)&31;	cb=(cb<<3)|(cb>>2);
		
		cy=(2*cg+cr+cb)/2;

//		cr=(5*cy+3*cr)/8;
//		cg=(5*cy+3*cg)/8;
//		cb=(5*cy+3*cb)/8;

		cr=(3*cy+1*cr)/4;
		cg=(3*cy+1*cg)/4;
		cb=(3*cy+1*cb)/4;

//		cr=(3*cy+5*cr)/8;
//		cg=(3*cy+5*cg)/8;
//		cb=(3*cy+5*cb)/8;
//		cr=(cr+cy)/2;
//		cg=(cg+cy)/2;
//		cb=(cb+cy)/2;

//		cr=cy;
//		cg=cy;
//		cb=cy;
		
		bi=0; bd=1<<24;
//		for(j=0; j<256; j++)
//		for(j=0; j<224; j++)
		for(j=0; j<252; j++)
		{
			dr=cr-pal[j*3+0];
			dg=cg-pal[j*3+1];
			db=cb-pal[j*3+2];
			d=(dr*dr)+2*(dg*dg)+(db*db);
			
			if(d<bd)
				{ bi2=bi; bd2=bd; bi=j; bd=d; }
			else if(d<bd2)
				{ bi2=j; bd2=d; }
		}
		
//		pal_15to8[i]=bi;
//		pal_15to8[i+32768]=bi;

		if((bd*1.25)>bd2)
		{
			bi=bi|(bi2<<8);
		}else
		{
			bi=bi|(bi<<8);
		}

		pal_15to8_ls[i*2+0]=bi;
		pal_15to8_ls[i*2+1]=bi;
	}
}

int main()
{
	char tb[256];
	FILE *fd;
	void *buf;
	char *s0, *s1;
	int i, j, k, n;
	
//	pal=LoadFile("palette.lmp", &k);
	SetupPalette();
	
	wad_data=malloc(1<<24);
	wad_rover=16;
	wad_n_lumps=0;

	AddWadLump("WOLFSTRT", "", 0);
	
	n=1;
	for(i=0; i<144; i++)
	{
		j=ReadWall(i);
//		if((j<0) && (i!=31))
		if(j<0)
			continue;

#if 0
		if(i==33)
		{
//			sprintf(tb, "WALL%d", n++);
			sprintf(tb, "WALL%d", 16);
			AddWadLump(tb, wall_rgb8, 4096);
		}
#endif
		
		n=(i/2)+1;
//		sprintf(tb, "WALL%d", n++);
		sprintf(tb, "WALL%d", n);
		AddWadLump(tb, wall_rgb8, 4096);
		
		printf("%s\n", tb);
		
		sprintf(tb, "tmp_walls/wall%d.tga", n);
		BTIC1H_Img_SavePal8TGA(tb, wall_rgb8, 64, 64, pal);
//		BTIC1H_Img_SaveHiTGA(tb, wall_rgb555hi, 128, 128, pal);
	}

#if 1
	for(i=126; i<144; i++)
	{
		j=ReadWall(i);
		if(j<0)
			continue;
		n=((i-126)/2)+1;

		sprintf(tb, "WLFDOOR%d", n);
		AddWadLump(tb, wall_rgb8, 4096);
		printf("%s\n", tb);

		sprintf(tb, "tmp_walls/door%d.tga", n);
		BTIC1H_Img_SavePal8TGA(tb, wall_rgb8, 64, 64, pal);

		for(j=0; j<8; j++)
		{
			sprintf(tb, "WLFDR%dA%d", n, j);
//			PatchShiftLeft((j+1)*8);
			PatchShiftRight((j+1)*8);
			AddWadLump(tb, wall_patch8, wall_patch8_sz);
		}
	}
#endif

	AddWadLump("PAL", pal, 768);

	for(i=0; soundlist[i].name; i++)
	{
		n=soundlist[i].id;
		if(n<=0)
			continue;
		buf=NULL;
		if(!(n&4096))
		{
			sprintf(tb, "w3d_base/sfx/%03d.wav", n&1023);
			buf=LoadFile(tb, &k);
		}
		if(!buf)
		{
			sprintf(tb, "w3d_base/lsfx/%03d.wav", n&1023);
			buf=LoadFile(tb, &k);
		}
		if(!buf)
			continue;
		AddWadLump(soundlist[i].name, buf, k);
		printf("%s %s\n", soundlist[i].name, tb);
	}
	
	for(i=0; i<640; i++)
	{
		j=ReadSprite(i);
		if(j<0)
			continue;
//		n=((i-126)/2)+1;
		n=i;

		sprintf(tb, "tmp_sprite/spr%d.tga", n);
		BTIC1H_Img_SavePal8TGA(tb, spr_rgb8, spr_ys, spr_xs, pal);
	}

	for(i=0; (spritelist[i].dspr>0); i++)
	{
		k=spritelist[i].wspr;
		j=ReadSprite(k);
		if(j<0)
			continue;

		k=spritelist[i].dspr;
		s0=LumpNameForSprite(k);
		if(!s0)continue;

		AddWadLump(s0, spr_patch8, spr_patch8_sz);
		printf("Sprite %s(%d) sz=%d\n", s0, k, spr_patch8_sz);
	}

	printf("Blitz1 %d\n", SPR_BLITZ_SHOOT1);

//	ReadSprite(i)
	
#if 1
	for(i=0; i<60; i++)
	{
		sprintf(tb, "w3d_base/maps/w%02d.map", i);
		fd=fopen(tb, "rb");
		if(!fd)
		{
			printf("Fail %s\n", tb);
			continue;
		}
		
		ReadMap(fd);
		fclose(fd);

		ConvMap(i);
//		SaveMapRtl(i);
		SaveMapBuffer(i, spr_patch8, &j);

		sprintf(tb, "MAP%02d", i+1);
		AddWadLump(tb, spr_patch8, j);
	}
#endif

	memcpy(wad_head.identification, "PWAD", 4);
	wad_head.numlumps=wad_n_lumps;
	wad_head.infotableofs=wad_rover;
	
	memcpy(wad_data+wad_rover, wad_dir, wad_n_lumps*16);
	wad_rover+=wad_n_lumps*16;
	
	memcpy(wad_data, &wad_head, sizeof(wad_head));
	
	StoreFile("w3dmap.wad", wad_data, wad_rover);
}
