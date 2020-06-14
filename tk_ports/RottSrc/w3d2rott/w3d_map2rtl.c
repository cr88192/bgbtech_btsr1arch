#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLEVELNAMELENGTH 23
#define ALLOCATEDLEVELNAMELENGTH 24
#define NUMPLANES		3
#define NUMHEADEROFFSETS 100

typedef unsigned char byte;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef unsigned short word;

struct W3D_MapHead_s {
u32 magic;
u16 rle;
u16 w;
u16 h;
u32 ceil;
u32 floor;
u16 length[3];
u32 offset[3];
u16 mapNameLen;
u16 musNameLen;
u32 fpartTime;
byte spartTime[5];

char *mapName;
char *musName;

u16 *plane[3];
};

typedef struct
{
	unsigned used;
	unsigned CRC;
	unsigned RLEWtag;
	unsigned MapSpecials;
	unsigned planestart[ NUMPLANES ];
	unsigned planelength[ NUMPLANES ];
	char	Name[ ALLOCATEDLEVELNAMELENGTH ];
} RTLMAP;

struct W3D_MapHead_s maphead;
RTLMAP rtlmap;

u16 *rtlplane_all;
u16 *rtlplane[3];
byte *rtlcdat[3];
int rtlcdsz[3];

int rtlrov;
FILE *rtl_fd;

static unsigned short int crc16tab[256] =
{
	0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
	0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
	0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
	0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
	0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
	0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
	0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
	0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
	0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
	0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
	0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
	0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
	0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
	0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
	0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
	0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
	0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
	0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
	0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
	0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
	0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
	0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
	0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
	0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
	0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
	0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
	0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
	0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
	0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
	0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
	0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
	0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
};

word CalculateCRC (byte *source, unsigned size)
{
   unsigned i;
   int checksum;
	int tmp;

   checksum=0;

   for (i = 0; i < size; i++)
      {
   	tmp=checksum^(*(source++));
	   checksum=(checksum>>8)^crc16tab[tmp & 0xff];
      }

   return ((word)checksum);

}


void Lvl_CarmackExpand( u16 *source, u16 *dest, u16 length )
{
	#define NEARTAG	0xA7
	#define FARTAG	0xA8


	u32		chhigh, offset;
	u16		*copyptr, *outptr;
	byte	*inptr;
	u16		ch, count;

	length /= 2;

	inptr = (byte *)source;
	outptr = dest;

	while( length )
	{
		ch = *(u16 *)inptr;
		inptr += 2;
		chhigh = ch >> 8;
		if( chhigh == NEARTAG )
		{
			count = ch & 0xff;
			if( ! count )
			{	// have to insert a word containing the tag byte
				ch |= *inptr++;
				*outptr++ = (u16)ch;
				length--;
			}
			else
			{
				offset = *inptr++;
				copyptr = outptr - offset;
				length -= count;
				while( count-- )
				{
					*outptr++ = *copyptr++;
				}
			}
		}
		else if( chhigh == FARTAG )
		{
			count = ch & 0xff;
			if( ! count )
			{				// have to insert a word containing the tag byte
				ch |= *inptr++;
				*outptr++ = ch;
				length--;
			}
			else
			{
				offset = *(u16 *)inptr;
				inptr += 2;
				copyptr = dest + offset;
				length -= count;
				while( count-- )
				{
					*outptr++ = *copyptr++;
				}
			}
		}
		else
		{
			*outptr++ = ch;
			length--;
		}
	}
}

void Lvl_RLEWexpand( u16 *source, u16 *dest, 
	   long length, unsigned rlewtag )
{
	unsigned value, count, i;
	u16 *end;

	end = dest + (length >> 1);

	do
	{
		value = *source++;
		if( value != rlewtag )
			{ *dest++ = value; }
		else
		{
			count = *source++;
			value = *source++;

			for( i = 1 ; i <= count ; ++i )
			{
				*dest++ = value;
			}
		}

	} while( dest < end );
}

int Lvl_RLEWencode( u16 *source, u16 *dest, 
	   int length, u16 rlewtag )
{
	u16 *cs, *cs1, *cse, *ct;
	int j;
	
	cs=source;	cse=source+(length>>1);
	ct=dest;
	while(cs<cse)
	{
		cs1=cs+1;
		while((cs1<cse) && (*cs1==*cs))cs1++;
		j=cs1-cs;
		
		if((j>3) || ((j>1) && (*cs==rlewtag)))
		{
			*ct++=rlewtag;
			*ct++=j;
			*ct++=*cs;
			cs+=j;
			continue;
		}
	
		if(*cs==rlewtag)
		{
			*ct++=rlewtag;
			*ct++=1;
			*ct++=*cs++;
			continue;
		}
		*ct++=*cs++;
		continue;
	}
	
	j=(ct-dest)<<1;
	return(j);
}

int Lvl_BTRLE1Wencode( u16 *source, byte *dest, int length)
{
	u16 *cs, *cs1, *cse;
	byte *ct;
	int value;
	int i, j, k;
	
	cs=source;	cse=source+(length>>1);
	ct=dest;	value=-999999;
	while(cs<cse)
	{
		cs1=cs;
		while((cs1<cse) && (*cs1==value))cs1++;
		j=cs1-cs;
		
		if(j>2)
		{
			if(j<32)
//			if(1)
			{
				if(j>=32)
					j=31;
				*ct++=0xC0+j;
				cs+=j;
				continue;
			}
			
			if(j>=4096)
				j=4095;
		
			*ct++=0xC0;
			*ct++=j&255;
			*ct++=(j>>8)&255;
			cs+=j;
			continue;
		}

		value=*cs++;
		if(value<0x80)
		{
			*ct++=value;
		}else if(value<0x4000)
		{
			*ct++=0x80+((value>>8)&0x3F);
			*ct++=value&255;
		}else
		{
			*ct++=0xC0;
			*ct++=value&255;
			*ct++=(value>>8)&255;
		}
		continue;
	}
	
	j=ct-dest;
	return(j);
}

int ReadMap(FILE *fd)
{
	byte *cdat;
	u16 *cbuf;
	int i, j, k, dsz;

	fread(&(maphead.magic), 1, 4, fd);
	fread(&(maphead.rle), 1, 2, fd);
	fread(&(maphead.w), 1, 2, fd);
	fread(&(maphead.h), 1, 2, fd);
	fread(&(maphead.ceil), 1, 4, fd);
	fread(&(maphead.floor), 1, 4, fd);
	fread(&(maphead.length), 2, 3, fd);
	fread(&(maphead.offset), 4, 3, fd);
	fread(&(maphead.mapNameLen), 1, 2, fd);
	fread(&(maphead.musNameLen), 1, 2, fd);
	fread(&(maphead.fpartTime), 4, 1, fd);
	fread(&(maphead.spartTime), 1, 5, fd);
	
	maphead.mapName=malloc(maphead.mapNameLen+1);
	maphead.musName=malloc(maphead.musNameLen+1);
	fread(maphead.mapName, 1, maphead.mapNameLen, fd);
	fread(maphead.musName, 1, maphead.musNameLen, fd);
	maphead.mapName[maphead.mapNameLen]=0;
	maphead.musName[maphead.musNameLen]=0;
	
	printf("%s\n", maphead.mapName);
	
	maphead.plane[0]=malloc(64*64*2);
	maphead.plane[1]=malloc(64*64*2);
	maphead.plane[2]=malloc(64*64*2);
	
	for(i=0; i<3; i++)
	{
		cdat=malloc(maphead.length[i]);
		fseek(fd, maphead.offset[i], 0);
		fread(cdat, 1, maphead.length[i], fd);
		dsz=*(u16 *)cdat;
		cbuf=malloc(dsz);

		Lvl_CarmackExpand( (u16 *)cdat+1, cbuf, dsz );
		Lvl_RLEWexpand( cbuf+1, maphead.plane[i], 64*64*2, maphead.rle );
		
		free(cdat);
		free(cbuf);
	}
	return(0);
}

int secret_nxtmap;

int ConvMap_TilePlainSolidP(int tile)
{
	if(tile<1)
		return(0);
	if(tile>0x30)
		return(0);
	return(1);
}

int ConvMap_TileNormalAreaP(int tile)
{
	if(tile<(0x6B+ 1))
		return(0);
	if(tile>(0x6B+47))
		return(0);
	return(1);
}

int ConvMap(int mapnum)
{
	int l1, l2, l3, l1fill;
	int lga1[3][9];
	int lga2[3][5*5];
	int x, y, x1, y1, x2, y2, crc;
	int xn1, xp1, yn1, yp1;
	int i, j, k;

	if(!rtlplane[0])
	{
		rtlplane_all=malloc(3*128*128*2);
		rtlplane[0]=rtlplane_all+(0*128*128);
		rtlplane[1]=rtlplane_all+(1*128*128);
		rtlplane[2]=rtlplane_all+(2*128*128);
//		rtlplane[0]=malloc(128*128*2);
//		rtlplane[1]=malloc(128*128*2);
//		rtlplane[2]=malloc(128*128*2);
		
		rtlcdat[0]=malloc(128*128*4);
		rtlcdat[1]=malloc(128*128*4);
		rtlcdat[2]=malloc(128*128*4);
	}
	
	memset(rtlplane[0], 0, 128*128*2);
	memset(rtlplane[1], 0, 128*128*2);
	memset(rtlplane[2], 0, 128*128*2);
	
//	l1fill=maphead.plane[0][63*64+63];
//	if((l1fill<=0) || (l1fill>0x60))
//		l1fill=-1;
	
	for(y=0; y<64; y++)
		for(x=0; x<64; x++)
	{
		l1=maphead.plane[0][y*64+x];
		l2=maphead.plane[1][y*64+x];
		l3=maphead.plane[2][y*64+x];

		xn1=x-1;	xp1=x+1;
		yn1=y-1;	yp1=y+1;
		if(xn1<0)xn1=0;
		if(yn1<0)yn1=0;
		if(xp1>63)xp1=63;
		if(yp1>63)yp1=63;

		l1fill=l1;
		if((l1fill<=0) || (l1fill>0x30))
			l1fill=-1;
		
		for(j=0; j<3; j++)
		{
#if 0
			for(y1=-2; y1<=2; y1++)
				for(x1=-2; x1<=2; x1++)
			{
				x2=x+x1;	y2=y+y1;
				if(x2<0)x2=0;
				if(y2<0)y2=0;
				if(x2>63)x2=63;
				if(y2>63)y2=63;
				lga2[j][(y1+2)*5+(x1+2)]=maphead.plane[j][(y2*64+x2)&4095];
			}
#endif

#if 1
			lga1[j][0]=maphead.plane[j][((yn1)*64+(xn1))&4095];
			lga1[j][1]=maphead.plane[j][((yn1)*64+(x  ))&4095];
			lga1[j][2]=maphead.plane[j][((yn1)*64+(xp1))&4095];
			lga1[j][3]=maphead.plane[j][((y  )*64+(xn1))&4095];
			lga1[j][4]=maphead.plane[j][((y  )*64+(x  ))&4095];
			lga1[j][5]=maphead.plane[j][((y  )*64+(xp1))&4095];
			lga1[j][6]=maphead.plane[j][((yp1)*64+(xn1))&4095];
			lga1[j][7]=maphead.plane[j][((yp1)*64+(x  ))&4095];
			lga1[j][8]=maphead.plane[j][((yp1)*64+(xp1))&4095];
#endif
		}
		
		if(	((lga1[0][1]==0x15) && (lga1[0][7]==0x15))	||
			((lga1[0][3]==0x15) && (lga1[0][5]==0x15))	)
		{
			l2=0x6B;
			l3=0xE200+(mapnum+1);

			if((mapnum%10)==8)
				l3=0xE200+(mapnum+1);
			
			if((mapnum%10)==9)
				l3=secret_nxtmap;

			if(l1==0x6B)
			{
				l2=0x6A;
				secret_nxtmap=l3;
				printf("Secret Exit %d,%d\n", x,y);
				l3=0xE400+(((mapnum/10)*10)+9);
			}else
			{
				printf("Exit %d,%d\n", x,y);
			}
		}
				
//		if(l1==0x6A)	/* Ambush Area */
//			l1=0x6B+1;
//		if(l1==0x6B)	/* ? */
//			l1=0x6B+1;

		if((l1==0x6A) || (l1==0x6B))
		{
			if(ConvMap_TileNormalAreaP(lga1[0][1]))
				{ l1=lga1[0][1]; }
			else if(ConvMap_TileNormalAreaP(lga1[0][3]))
				{ l1=lga1[0][3]; }
			else if(ConvMap_TileNormalAreaP(lga1[0][5]))
				{ l1=lga1[0][5]; }
			else if(ConvMap_TileNormalAreaP(lga1[0][7]))
				{ l1=lga1[0][7]; }
			else { l1=0x6B+1; }
		}

		
		/* Clear out filler */
//		if((l1==1) && (l2==0) && (l3==0))
//		if((l1==l1fill) && (l2==0) && (l3==0))
		if(ConvMap_TilePlainSolidP(l1) && (l2==0) && (l3==0))
//		if(0)
		{
//			l1=2;

//			l1=maphead.plane[0][y*64+x];

			for(j=0; j<9; j++)
//			for(j=0; j<25; j++)
			{
//				if(lga1[0][j]!=1)	break;
//				if(lga1[0][j]!=l1fill)	break;
				if(!ConvMap_TilePlainSolidP(lga1[0][j]))	break;
				if(lga1[1][j]!=0)	break;
				if(lga1[2][j]!=0)	break;
			}
			if(j>=9)
//			if(j>=25)
				l1=0;

		}
		
		if(l2==0x62)	/* Pushwall */
		{
			l2=80;
		}

		if(l2==0x63)	/* Victory Exit */
		{
			l2=0x6B;
			printf("Victory Exit %04X\n", l3);
//			l3=0xE200+(mapnum+1);
			l3=0xE200+(mapnum+2);
		}
		
//		rtlplane[0][y*128+x]=l1;
//		rtlplane[1][y*128+x]=l2;
//		rtlplane[2][y*128+x]=l3;

		rtlplane[0][y*128+(x+32)]=l1;
		rtlplane[1][y*128+(x+32)]=l2;
		rtlplane[2][y*128+(x+32)]=l3;
	}

	rtlplane[0][0*128+0]=179+2;		//FLOORNUM + n
	rtlplane[0][0*128+1]=197+7;		//CEILINGNUM + n / SKY
	rtlplane[0][0*128+2]=216+7;		//LIGHTLEVELBASE + 0..7
//	rtlplane[0][0*128+2]=216+1;		//LIGHTLEVELBASE + 0..7
	rtlplane[0][0*128+3]=252+15;	//LIGHTRATEBASE + (0..15)
	
	rtlplane[1][0*128+0]=89+1;		//LEVELHEIGHT 89+(1..7)
	rtlplane[1][0*128+1]=0;			//Horizon
	rtlplane[1][0*128+2]=104;		//FOG
//	rtlplane[1][0*128+3]=0;
	rtlplane[1][0*128+3]=99;
}


int SaveMapRtl(int mapnum)
{
	int crc;
	int i, j, k;

	crc=CalculateCRC((byte *)rtlplane_all, 3*128*128*2);

	rtlmap.used=1;
	rtlmap.CRC=crc;
	rtlmap.RLEWtag=maphead.rle;
	rtlmap.MapSpecials=0;
//	rtlmap.planestart[ NUMPLANES ];
//	unsigned planelength[ NUMPLANES ];
	strncpy(rtlmap.Name, maphead.mapName, 23);

	rtlmap.RLEWtag=0x4231;

	for(i=0; i<3; i++)
	{
		if(rtlmap.RLEWtag==0x4231)
		{
			j=Lvl_BTRLE1Wencode(
				rtlplane[i], rtlcdat[i],
				128*128*2);
		}else
		{
			j=Lvl_RLEWencode(
				rtlplane[i], (u16 *)(rtlcdat[i]),
				128*128*2, rtlmap.RLEWtag);
		}
		rtlcdsz[i]=j;
		
		rtlmap.planestart[i]=rtlrov;
		rtlmap.planelength[i]=j;
		rtlrov+=j;
		rtlrov=(rtlrov+3)&(~3);
	}

	i=8+mapnum*sizeof(RTLMAP);
	fseek(rtl_fd, i, 0);
	fwrite(&rtlmap, 1, sizeof(RTLMAP), rtl_fd);

	for(i=0; i<3; i++)
	{
		fseek(rtl_fd, rtlmap.planestart[i], 0);
		fwrite(rtlcdat[i], 1, rtlcdsz[i]+7, rtl_fd);
	}

	printf("total size: %d\n", rtlrov);

// byte *rtlcdat[3];
// int *rtlcdsz[3];

	return(0);
}

int SaveMapBuffer(int mapnum, byte *obuf, int *rsz)
{
	int crc, rov;
	int i, j, k;

	crc=CalculateCRC((byte *)rtlplane_all, 3*128*128*2);

	rtlmap.used=1;
	rtlmap.CRC=crc;
	rtlmap.RLEWtag=maphead.rle;
	rtlmap.MapSpecials=0;
//	rtlmap.planestart[ NUMPLANES ];
//	unsigned planelength[ NUMPLANES ];
	strncpy(rtlmap.Name, maphead.mapName, 23);

	rtlmap.RLEWtag=0x4231;

	rov=sizeof(RTLMAP);

	for(i=0; i<3; i++)
	{
		if(rtlmap.RLEWtag==0x4231)
		{
			j=Lvl_BTRLE1Wencode(
				rtlplane[i], rtlcdat[i],
				128*128*2);
		}else
		{
			j=Lvl_RLEWencode(
				rtlplane[i], (u16 *)(rtlcdat[i]),
				128*128*2, rtlmap.RLEWtag);
		}
		rtlcdsz[i]=j;
		
		rtlmap.planestart[i]=rov;
		rtlmap.planelength[i]=j;
		rov+=j;
		rov=(rov+3)&(~3);
	}

//	i=8+mapnum*sizeof(RTLMAP);
//	fseek(rtl_fd, i, 0);
//	fwrite(&rtlmap, 1, sizeof(RTLMAP), rtl_fd);

	memcpy(obuf, &rtlmap, sizeof(RTLMAP));

	for(i=0; i<3; i++)
	{
//		fseek(rtl_fd, rtlmap.planestart[i], 0);
//		fwrite(rtlcdat[i], 1, rtlcdsz[i]+7, rtl_fd);
		memcpy(obuf+rtlmap.planestart[i], rtlcdat[i], rtlcdsz[i]);
	}

	printf("map size: %d\n", rov);

// byte *rtlcdat[3];
// int *rtlcdsz[3];

	*rsz=rov;

	return(0);
}

#ifndef W3D_MAPWAD

int main()
{
	char tb1[256];
	FILE *fd;
	int i, j, k;
	
	rtlrov = 8 + 100*sizeof(RTLMAP);
	rtl_fd = fopen("w3dmap.rtl", "wb");
	
	fseek(rtl_fd, 0, 0);
	memset(tb1, 0, 256);
	k=rtlrov;
	while(k>0)
	{
		fwrite(tb1, 1, 256, rtl_fd);
		k-=256;
	}
	
	memcpy(tb1, "RTL", 4);
	tb1[4]=0x01;
	tb1[5]=0x01;

	fseek(rtl_fd, 0, 0);

	fwrite(tb1, 1, 8, rtl_fd);

	
	for(i=0; i<60; i++)
	{
		sprintf(tb1, "w3d_base/maps/w%02d.map", i);
		fd=fopen(tb1, "rb");
		if(!fd)
		{
			printf("Fail %s\n", tb1);
			continue;
		}
		
		ReadMap(fd);
		fclose(fd);

		ConvMap(i);
		SaveMapRtl(i);
	}
}

#endif