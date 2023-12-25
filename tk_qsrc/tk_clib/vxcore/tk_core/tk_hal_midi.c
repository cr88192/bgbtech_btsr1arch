volatile u32 *tk_midi_regs;

volatile u32 *tk_midi_patchmem;
u32 *tk_midi_patchbm;

u16 tk_midi_patch_base[256];
u16 tk_midi_patch_size[256];
u16 tk_midi_patch_lbeg[256];
u16 tk_midi_patch_lend[256];
u16 tk_midi_patch_rate[256];

byte *tk_midi_css;		//start position (active song)
byte *tk_midi_cse;		//end position (active song)
byte *tk_midi_cs;		//current position (active song)
int tk_midi_tt;			//remaining tics until next event

int tk_midi_notediv[128];
byte tk_midi_noteatt[128];

byte tk_midi_chanvol[16];		//volume
byte tk_midi_chanpbl[16];		//pitch blend
byte tk_midi_chanprg[16];		//program
byte tk_midi_chanpan[16];		//pan
byte tk_midi_chanmod[16];		//modulation wheel

byte tk_midi_chanvn[16];
byte tk_midi_channt[16];

//byte tk_midi_vnflg[16];		//voice flag
byte tk_midi_vnflg[32];		//voice flag
byte tk_midi_vnchn[32];		//voice channel

u32 tk_midi_fmregdata[256*16];

byte tk_midi_musicvolume = 15;

int TK_Midi_Tick();

int tk_midi_irq_tt;

byte *tk_midi_patchwad;
byte *tk_midi_patchwad_index;
int tk_midi_sz_patchwad;
short tk_midi_patchwad_indexsz;
byte tk_midi_patchwad_live;

int tk_midi_patchwad_sndofs[512];
int tk_midi_patchwad_sndlen[512];
int tk_midi_patchwad_sndrate[512];

int tk_midi_timer_irq()
{
	int i;

//	return(0);

	if(tk_midi_irq_tt>0)
	{
		tk_midi_irq_tt--;
		return(0);
	}
	
	i=TK_Midi_Tick();
	if(i>0)
		tk_midi_irq_tt=7;
//	tk_midi_irq_tt=234;
//	tk_midi_irq_tt=14;
//	tk_midi_irq_tt=20;
//	tk_midi_irq_tt=28;
	return(0);
}

static int tk_midi_isinit = 0;

double tk_midi_ipow2(double y)
{
	double x, z, z1;
	
	if(y>0)
	{
		x=1; z=y;
		while(z>1)
			{ x=x*2; z=z-1; }
//		z1=(z*z+z)*0.5;
//		z1=(z1+z)*0.5;
		z1=z;
//		x=x*(z+1);
		x=x*(1+z1);
		return(x);
	}else
	{
		x=1; z=-y;
		while(z>1)
			{ x=x*0.5; z=z-1; }
//		z1=(z*z+z)*0.5;
//		z1=(z1+z)*0.5;
		z1=z;
//		x=x*(1-(z*0.5));
		x=x*(1-(z1*0.5));
		return(x);
	}
}

int TK_Midi_PatchMemFindFree(int cnt)
{
	u32 v0;
	int l1;
	int i, j, k;
	
	i=0;
	while(i<65536)
	{
		v0=tk_midi_patchbm[i>>5];
		if(v0==0xFFFFFFFFU)
		{
			i=(i+32)&(~31);
			continue;
		}
		if(((v0>>(i&31))&0xFF)==0xFF)
		{
			i+=8;
			continue;
		}

		if((v0>>(i&31))&1)
		{
			i++;
			continue;
		}
		
		l1=i+cnt;
		if(l1>65536)
			break;

		if((l1>>5)==(i>>5))
		{
			for(j=i; j<l1; j++)
				if((v0>>(j&31))&1)
					break;
		}else
		{
			for(j=i; j<l1; j++)
				if((tk_midi_patchbm[j>>5]>>(j&31))&1)
					break;
		}
		
		if(j>=l1)
		{
			return(i);
		}
	}
	
	return(-1);
}

int TK_Midi_PatchMemMarkUsed(int base, int cnt)
{
	int i, l;
	
	l=base+cnt;
	for(i=base; i<l; i++)
		tk_midi_patchbm[i>>5]|=1<<(i&31);
}

int TK_Midi_PatchMemMarkFree(int base, int cnt)
{
	int i, l;
	
	l=base+cnt;
	for(i=base; i<l; i++)
		tk_midi_patchbm[i>>5]&=~(1<<(i&31));
}

int TK_Midi_PatchMemAllocSamples(int size)
{
	int base, cnt;
	
	cnt=(size+15)>>4;
	base=TK_Midi_PatchMemFindFree(cnt);
	if(base<0)
		return(base);
	TK_Midi_PatchMemMarkUsed(base, cnt);
	return(base);
}


int TK_Midi_SetMasterParam(int var, int val)
{
	if(var==1)
	{
		if(val<0)	val=0;
		if(val>15)	val=15;
		tk_midi_musicvolume=val;
		return(0);
	}

	return(-1);
}

int TK_Midi_WadLookupLump(byte *wad, u64 lname, int *roffs, int *rsz)
{
	byte *debase, *dep;
	int dofs, dsz;
	int i, j, k;

	if(!wad)
		return(-1);

	if((*(u32 *)wad)!=TKGDI_FCC_IWAD)
	{
		tk_printf("TK_Midi_WadLookupLump: Not IWAD\n");
		return(-1);
	}
	dsz=((u32 *)wad)[1];
	dofs=((u32 *)wad)[2];
	
	debase=wad+dofs;
	for(i=0; i<dsz; i++)
	{
		dep=debase+(i<<4);
		if(((u64 *)dep)[1]==lname)
		{
			*roffs=((u32 *)dep)[0];
			*rsz=((u32 *)dep)[1];
			return(i);
		}
	}

	tk_printf("TK_Midi_WadLookupLump: Not Found %08s\n", &lname);
	return(-1);
}

static const short tk_midi_notehz[128] = {
    8,    8,    9,   10,   10,   11,   12,   12,		//0..7
   13,   14,   15,   15,   16,   17,   18,   19,		//8..15
   21,   22,   23,   25,   26,   28,   29,   31,		//16..23
   33,   35,   37,   39,   41,   44,   46,   49,		//24..31
   52,   55,   58,   62,   65,   69,   73,   78,		//32..39
   82,   87,   93,   98,  104,  110,  117,  123,		//40..47
  131,  139,  147,  156,  165,  175,  185,  196,		//48..55
  207,  220,  233,  247,  262,  277,  294,  311,		//56..63
  330,  349,  370,  392,  415,  440,  466,  494,		//64..71
  523,  554,  587,  622,  659,  698,  740,  784,		//72..79
  831,  880,  923,  988, 1047, 1109, 1175, 1245,		//80..87
 1319, 1397, 1480, 1568, 1661, 1760, 1864, 1976,
 2093, 2217, 2349, 2489, 2637, 2793, 2960, 3136,
 2211, 3520, 3729, 3951, 4186, 4434, 4699, 4978,
 5274, 5587, 5920, 6272, 6645, 7040, 7459, 7902,
 8372, 8870, 9397, 9956, 9999, 9999, 9999, 9999
};

static const char *tk_midi_hexdig="0123456789ABCDEF";

static byte tk_midi_msgmaskpgm[256];

int TK_Midi_LookupGetPatchAuMem(
	int pgm, int note,
	int *rbase, int *rlen,
	int *rlbeg, int *rlend,
	int *rsstep, int *rsflg)
{
	byte *indx, *rec;
	u64 lnam;
	double f, g;
	int indxsz, hz, mhz, nhz, bhz, sidx;
	int sbase, slen, srate, sstep;
	int lbeg, lend, wlofs, wlsz;
	int bidx, bbhz, lflg;
	int i, j, k;

	if(!tk_midi_patchwad_live)
		return(-1);

	indx=tk_midi_patchwad_index;
	indxsz=tk_midi_patchwad_indexsz;
	
	hz=tk_midi_notehz[note];
	bidx=-1; bbhz=0;
	
	for(i=0; i<indxsz; i++)
	{
		rec=indx+(i<<4);
		if(rec[0]!=pgm)
			continue;
			
		mhz=*(u16 *)(rec+ 8);
		nhz=*(u16 *)(rec+10);
		bhz=*(u16 *)(rec+12);

#if 1
		if(hz<mhz)
			continue;
		if(hz>nhz)
			continue;

		bidx=i;
		break;
#endif

#if 0
		if((hz<mhz) || (hz>nhz))
		{
			if(bidx<0)
			{
				bidx=i;
				bbhz=bhz;
			}else
			{
				j=hz-bhz;
				k=hz-bbhz;
				j=j^(j>>31);
				k=k^(k>>31);
				if(j<k)
				{
					bidx=i;
					bbhz=bhz;
				}
			}
		}else
		{
			bidx=i;
			break;
		}
#endif
	}

//	if(i>=indxsz)
	if(bidx<0)
	{
		if(tk_midi_msgmaskpgm[pgm])
			return(-1);
	
		tk_midi_msgmaskpgm[pgm]=1;
		tk_printf("TK_Midi_LookupGetPatchAuMem: Fail Lookup pgm=%d note=%d\n", 
			pgm, note);
		return(-1);
	}

	rec=indx+(bidx<<4);
	mhz=*(u16 *)(rec+ 8);
	nhz=*(u16 *)(rec+10);
	bhz=*(u16 *)(rec+12);
	lflg=*(u16 *)(rec+14);

	sidx=*(u16 *)(rec+2);
	lbeg=*(u16 *)(rec+4);
	lend=*(u16 *)(rec+6);

	if((sidx<1) || (sidx>511))
	{
		tk_printf("TK_Midi_LookupGetPatchAuMem: "
			"Bad Sound Index %04X\n", sidx);
		return(-1);
	}

	sbase=tk_midi_patchwad_sndofs[sidx];
	slen=tk_midi_patchwad_sndlen[sidx];
	srate=tk_midi_patchwad_sndrate[sidx];
	
	if(!sbase)
	{
		lnam=TKGDI_FCC_SND_;
		lnam|=((u64)tk_midi_hexdig[(sidx>>12)&15])<<32;
		lnam|=((u64)tk_midi_hexdig[(sidx>> 8)&15])<<40;
		lnam|=((u64)tk_midi_hexdig[(sidx>> 4)&15])<<48;
		lnam|=((u64)tk_midi_hexdig[(sidx>> 0)&15])<<56;
		
		wlofs=0;	wlsz=0;

		TK_Midi_WadLookupLump(tk_midi_patchwad, lnam, &wlofs, &wlsz);
		
		if(!wlofs)
		{
			tk_printf("TK_Midi_LookupGetPatchAuMem: "
				"Fail Lookup Sound %04X\n", sidx);
			return(-1);
		}
		
		sbase=(wlofs+16)>>4;
//		slen=(wlsz-16)&(~15);
		slen=(wlsz-32)&(~15);
		
		j=tk_midi_patchwad[wlofs+1]-'A';
		switch(j)
		{
			case  0: srate= 8000; break;
			case  1: srate=11025; break;
			case  2: srate=16000; break;
			case  3: srate=22050; break;
			case  4: srate=32000; break;
			case  5: srate=44100; break;
			case  6: srate=64000; break;
			case  7: srate=88200; break;
			case  8: srate= 4000; break;
			case  9: srate=48000; break;
			case 10: srate= 6000; break;
			case 11: srate=96000; break;
			default:
				tk_printf("TK_Midi_LookupGetPatchAuMem: "
					"Bad Sound Sample-Rate %04X %d\n", sidx, j);

				srate=16000;
				break;
		}

		tk_dbg_printf("TK_Midi_LookupGetPatchAuMem: "
			"Loaded Patch %04X B=%04X L=%d Rt=%d\n", sidx, sbase, slen, srate);

		tk_midi_patchwad_sndofs[sidx]=sbase;
		tk_midi_patchwad_sndlen[sidx]=slen;
		tk_midi_patchwad_sndrate[sidx]=srate;
	}

	if(lend>=slen)
		lend=slen-1;

//	f=hz*((srate*1.0)/bhz);
	f=srate*((hz*1.0)/bhz);

#if 0
	if(f>4000)
		tk_dbg_printf("TK_Midi_LookupGetPatchAuMem: Debug 0, f=%f, "
			"srate=%d hz=%d bhz=%d\n", f,
			srate, hz, bhz);
#endif
	
	sstep=(1<<20)*(f/65200.0);
//	sstep=(1<<20)*(f/64000.0);
//	sstep=(1<<20)*(f/1043200.0);

	*rbase=sbase;
	*rlen=slen;
	*rlbeg=lbeg;
	*rlend=lend;
	*rsstep=sstep;
	*rsflg=lflg;

	return(0);
}

int tk_midi_vol2att(int vol)
{
	int v, e, v1;
	
	if(!vol)
		return(63);
	
	v=vol; e=0;
	while(v<=64)
		{ v=v<<1; e++; }
	
	if(e>7)
		return(63);
	
	if(v==128)
		return(e<<3);
	
	v1=(e<<3)+(8-((v>>4)&7));
	return(v1);
}

int TK_Midi_Init()
{
	TK_FILE *fd;
//	float freq, ph;
	double freq, ph;
	double *rph;
	int fsz, lofs, lsz;
	int i, j, k, l;
	
	if(!tk_midi_regs)
		tk_midi_isinit=0;
	
	if(tk_midi_isinit)
		return(0);
	tk_midi_isinit=1;

	tk_midi_regs=(u32 *)0xFFFFF008C000ULL;
	tk_midi_patchmem=(u32 *)0xC00020800000ULL;

	tk_midi_patchbm=tk_malloc(8192);
	memset(tk_midi_patchbm, 0, 8192);
	
	for(i=0; i<128; i++)
	{
//		tk_midi_noteatt[i]=63-(i>>1);
		tk_midi_noteatt[i]=tk_midi_vol2att(i);
		
//		freq=pow(2, (i-69)/12.0)*440;
		freq=pow(2.0, (i-69)/12.0)*440.0;
//		freq=tk_midi_ipow2((i-69)/12.0)*440.0;
		
//		ph=freq/62500.0;
		ph=freq/64000.0;
//		ph=freq/32000.0;
//		ph=freq/128000.0;
//		ph=freq/176000.0;
//		ph=freq/256000.0;
		j=(1<<20)*ph+0.5;
		if(j>=(1<<20))j=(1<<20)-1;
		if(j<1)j=1;
		tk_midi_notediv[i]=j;
		
//		__debugbreak();
	}
	
	for(i=0; i<256; i++)
	{
		tk_midi_patch_base[i]=0;
		tk_midi_patch_size[i]=0;
		tk_midi_patch_lbeg[i]=0;
		tk_midi_patch_lend[i]=0;
		tk_midi_patch_rate[i]=0;
	}
	
	tk_midi_patchwad=NULL;
	tk_midi_sz_patchwad=0;
	tk_midi_patchwad_live=0;
	tk_midi_patchwad_index=NULL;

	fd=NULL;
	fd=tk_fopen("/boot/tksys/instrum.wad", "rb");
	if(fd)
	{
		tk_printf("TK_Midi_Init: Load Patch WAD\n");
	
		tk_fseek(fd, 0, 2);
		fsz=tk_ftell(fd);
		tk_midi_patchwad=tk_malloc(fsz);
		tk_fseek(fd, 0, 0);
		tk_fread(tk_midi_patchwad, 1, fsz, fd);
		tk_fclose(fd);
		tk_midi_sz_patchwad=fsz;
	}
	
	if(tk_midi_patchwad && (tk_midi_sz_patchwad<(1<<20)))
	{
//		memcpy(
//			tk_midi_patchmem,
//			tk_midi_patchwad,
//			tk_midi_sz_patchwad);

		l=tk_midi_sz_patchwad>>2;
		for(i=0; i<l; i++)
		{
			tk_midi_patchmem[i]=
				((u32 *)tk_midi_patchwad)[i] ^ 0x55555555;
		}

		tk_midi_patchwad_live=1;

		tk_dbg_printf("TK_Midi_Init: Patch WAD Live\n");
	}

	if(tk_midi_patchwad)
	{
		lofs=0;		lsz=0;
		i=TK_Midi_WadLookupLump(tk_midi_patchwad,
			TKGDI_ECC_PATCHIDX, &lofs, &lsz);
		if(i>=0)
		{
			tk_midi_patchwad_index=tk_midi_patchwad+lofs;
			tk_midi_patchwad_indexsz=lsz>>4;

			tk_dbg_printf("TK_Midi_Init: Patch WAD Index ofs=%d sz=%d\n",
				lofs, lsz);
		}
	}

	return(0);
}

int TK_Midi_SetFmRegisterData(int prg, int idx, u32 val)
{
	tk_midi_fmregdata[(prg*16)+idx]=val;
	return(0);
}

int TK_Midi_UploadPatch(TKGDI_MIDI_PATCHINFO *mpat, void *data)
{
	int idx, len, base;

	tk_midi_patchwad_live=0;
	
	idx=mpat->ptIndex;
	len=mpat->ptLength;

	base=TK_Midi_PatchMemAllocSamples(len);

	tk_midi_patch_base[idx]=base;
	tk_midi_patch_size[idx]=len;
	tk_midi_patch_lbeg[idx]=mpat->ptLoopBeg;
	tk_midi_patch_lend[idx]=mpat->ptLoopEnd;
	tk_midi_patch_rate[idx]=mpat->nSamplesPerSec;
	
	memcpy(tk_midi_patchmem+base*4, data, len);
	return(0);
}

int TK_Midi_SilenceAll()
{
	int i;

	if(!tk_midi_regs)
		return(0);

	for(i=0; i<16; i++)
	{
		tk_midi_regs[i*8+0]=0;
		tk_midi_regs[i*8+1]=0;
		tk_midi_regs[i*8+2]=0;
		tk_midi_regs[i*8+3]=0;
		tk_midi_regs[i*8+4]=0;
		tk_midi_regs[i*8+5]=0;
		tk_midi_regs[i*8+6]=0;
		tk_midi_regs[i*8+7]=0;
	}

	for(i=0; i<16; i++)
	{
		tk_midi_chanvol[i]=127;
		tk_midi_chanpbl[i]=128;
		tk_midi_chanvn[i]=0xFF;
	}

	for(i=0; i<32; i++)
	{
		tk_midi_vnflg[i]=0;
		tk_midi_vnchn[i]=0xFF;
	}

	memset(tk_midi_msgmaskpgm, 0, 256);

	return(0);
}

int TK_Midi_NoteOff(int ch, int d0, int d1)
{
	u32 v1, v2;
	int vn1, vn2, cvn;

	if(!tk_midi_regs)
		return(0);

	cvn=tk_midi_chanvn[ch];
	if(cvn==0xFF)
		return(0);

	if(tk_midi_channt[ch]!=d0)
		return(0);

//	tk_midi_chanvn[ch]=0xFF;

	vn1=(cvn&15)*2+0;
	vn2=(cvn&15)*2+1;

	if(cvn&16)
	{
		if(tk_midi_vnflg[vn1]&8)
		{
			if(tk_midi_vnflg[vn1]&4)
			{
				/* Stop the loop. */
				v1=tk_midi_regs[vn1*4+0];
				v1&=~0x00200000;
				tk_midi_regs[vn1*4+0]=v1;

				tk_midi_vnflg[vn1]&=~4;

				/* Jump to end of loop. */
				v2=tk_midi_regs[vn2*4+2];
				tk_midi_regs[vn2*4+3]=v2>>16;
			}else
			{
				v1=tk_midi_regs[vn1*4+0];

//				if(ch==8)

//				if(!(v1&0x00200000))
//					tk_midi_chanvn[ch]=0xFF;
			}

			v1=tk_midi_regs[vn2*4+3];
			v2=tk_midi_regs[vn1*4+2];
			if((v1&0xFFFF)>=((v2>>16)&0xFFFF))
			{
				/* Done playing, Stop Now*/
				tk_midi_regs[vn1*4+0]=0;
				tk_midi_regs[vn2*4+0]=0;
				tk_midi_vnflg[vn1]=0;
				tk_midi_vnflg[vn2]=0;

				tk_midi_regs[vn1*4+3]=0;
				tk_midi_regs[vn2*4+3]=0;

				tk_midi_chanvn[ch]=0xFF;
			}
			
		}else
		{
			tk_midi_regs[vn1*4+0]=0;
			tk_midi_regs[vn2*4+0]=0;
			tk_midi_vnflg[vn1]=0;
			tk_midi_vnflg[vn2]=0;

//			tk_midi_regs[vn1*4+3]=0;
//			tk_midi_regs[vn2*4+3]=0;

			tk_midi_chanvn[ch]=0xFF;
		}
	}else
	{
		tk_midi_regs[vn1*4+0]=0;
		tk_midi_vnflg[vn1]=0;
		tk_midi_chanvn[ch]=0xFF;
	}
	return(0);
}

int TK_Midi_FindFreeVoice(int fl)
{
	u32 v1, v2;
	int vn1, vn2, vnfl;
	int i, j;

	if(fl&1)
	{
		for(i=0; i<16; i++)
		{
			vnfl=tk_midi_vnflg[i*2+0];

			vn1=i*2+0;
			vn2=i*2+1;
			j=tk_midi_vnchn[vn1];
			if(vnfl && ((tk_midi_chanvn[j]&15)!=i))
			{
				v1=tk_midi_regs[vn1*4+0];

				if(v1&0x00200000)
				{
					tk_printf("TK_Midi_FindFreeVoice: "
						"Drop orphaned voice A.\n");

					tk_midi_regs[vn1*4+0]=0;
					tk_midi_regs[vn2*4+0]=0;
					tk_midi_vnflg[vn1]=0;
					tk_midi_vnflg[vn2]=0;
				}else
				{
					v1=tk_midi_regs[vn2*4+3];
					v2=tk_midi_regs[vn1*4+2];
					if((v1&0xFFFF)>=((v2>>16)&0xFFFF))
					{
						tk_midi_regs[vn1*4+0]=0;
						tk_midi_regs[vn2*4+0]=0;
						tk_midi_vnflg[vn1]=0;
						tk_midi_vnflg[vn2]=0;
					}
				}
			}
		}

		for(i=0; i<16; i++)
		{
			vnfl=tk_midi_vnflg[i*2+0];
			vn1=i*2+0;
			vn2=i*2+1;
					
			if(!tk_midi_vnflg[i*2+0] && !tk_midi_vnflg[i*2+1])
			{
//				tk_midi_regs[vn1*4+3]=0;
				tk_midi_regs[vn2*4+3]=0;

				return(16|i);
			}
		}

		for(i=0; i<16; i++)
		{
			vnfl=tk_midi_vnflg[i*2+0];
			vn1=i*2+0;
			vn2=i*2+1;

			if((vnfl&0xC)==0x08)
			{
				v1=tk_midi_regs[vn2*4+3];
				v2=tk_midi_regs[vn1*4+2];
				if((v1&0xFFFF)>=((v2>>16)&0xFFFF))
				{
					/* Done playing, Stop Now*/
					tk_midi_regs[vn1*4+0]=0;
					tk_midi_regs[vn2*4+0]=0;
					tk_midi_vnflg[vn1]=0;
					tk_midi_vnflg[vn2]=0;

					tk_midi_regs[vn1*4+3]=0;
					tk_midi_regs[vn2*4+3]=0;
					
					for(j=0; j<16; j++)
					{
						if((tk_midi_chanvn[j]&15)==i)
							{ tk_midi_chanvn[j]=0xFF; }
					}

					return(16|i);
				}
			}
		}

		for(i=0; i<16; i++)
		{
			vn1=i*2+0;
			vn2=i*2+1;
			j=tk_midi_vnchn[vn1];
			if((tk_midi_chanvn[j]&15)!=i)
			{
				tk_dbg_printf("TK_Midi_FindFreeVoice: "
					"Alloc orphaned voice A.\n");
				return(16|i);
			}
		}
	}else
	{
		for(i=0; i<16; i++)
		{
			if(!tk_midi_vnflg[i*2+0])
			{
				return(i);
			}
		}

		for(i=0; i<16; i++)
		{
			vn1=i*2+0;
			vn2=i*2+1;
			j=tk_midi_vnchn[vn1];
			if((tk_midi_chanvn[j]&15)!=i)
			{
				tk_dbg_printf("TK_Midi_FindFreeVoice: "
					"Alloc orphaned voice B.\n");
				return(i);
			}
		}
	}
	
	tk_printf("TK_Midi_FindFreeVoice: No Free voices.\n");
	return(-1);
}

int TK_Midi_NoteOn(int ch, int d0, int d1)
{
	u32		*rec;
	byte	*brec;
	u32		rv1, rv2;
	int		fvn, vn1, vn2, prg, fn, note, note1, note2;
	int 	dv, dv1, dv2, att1, att2, vol, pbl;
	int 	basenote, fl, loghz, note0b;

	byte modfcn, carfcn, modksc;
	byte modlvl, carlvl, carksc;
	byte modvol, carvol;
	byte fbconn, fineadj, fixnote, fixflag;
	int lbase, llen, lbeg, lend, lstep, lflg;

	note=d0;
	note0b=d0;


	if(!tk_midi_regs)
		return(0);

	prg=tk_midi_chanprg[ch];
//	if(ch==8)
	if(ch==9)
		prg+=128;
//	rec=tk_midi_fmregdata[prg];
	rec=tk_midi_fmregdata+(prg*16);
	brec=(byte *)rec;

	fixflag=brec[0];
	fineadj=brec[2];
	fixnote=brec[3];

	basenote=(short)(rec[4]>>16);

	modfcn=(brec[4+ 3])&255;
	modksc=(brec[4+ 4])&255;
	modlvl=(brec[4+ 5])&255;
	fbconn=(brec[4+ 6])&255;
	carfcn=(brec[4+10])&255;
	carksc=(brec[4+11])&255;
	carlvl=(brec[4+12])&255;

	lbase=0;	llen=0;
	lbeg=0;		lend=0;
	lstep=0;	lflg=0;

	fvn=tk_midi_chanvn[ch];
	if(fvn==0xFF)
		fvn=-1;

	if(fvn<0)
	{
		fl=0;
//		if(!(fbconn&1) || (modfcn!=carfcn) || (fixflag&4))
		if(!(fbconn&1) || (modfcn!=carfcn) || (fixflag&4) ||
			tk_midi_sz_patchwad)
			fl|=1;
//		fl=1;
		fvn=TK_Midi_FindFreeVoice(fl);
	}
	if(fvn<0)
	{
		tk_midi_chanvn[ch]=0xFF;
		return(0);
	}

	tk_midi_channt[ch]=note;

	vn1=(fvn&15)*2+0;
	vn2=(fvn&15)*2+1;

	tk_midi_vnchn[vn1]=ch;
	tk_midi_vnchn[vn2]=ch;

	if(fvn&16)
	{
		tk_midi_chanvn[ch]=fvn;
	}else
	{
		tk_midi_chanvn[ch]=fvn;
	}

	note+=basenote;
	if(note<0)note=0;

	if(fixflag&1)
	{
		note=fixnote;
	}

	loghz=((note-96)<<8)/12;

	note1=note;
	note2=note+(fineadj-128);

//	note1=__int_clamp(note1, 0, 127);
//	note2=__int_clamp(note2, 0, 127);

	vol=(tk_midi_chanvol[ch]*d1)>>7;
	vol=(vol*tk_midi_musicvolume)>>4;

	vol=__int_clamp(vol, 0, 127);

	modvol=63-(modlvl&63);
	carvol=63-(carlvl&63);
	
	pbl=tk_midi_chanpbl[ch];
	if(pbl)
	{
		pbl=(pbl>>3)-16;
	}
	
	note=d0+pbl;
//	note=d0;
	note=__int_clamp(note, 0, 127);

	TK_Midi_LookupGetPatchAuMem(prg, note,
		&lbase, &llen, &lbeg, &lend, &lstep, &lflg);

	note1=note1+pbl;
	note2=note2+pbl;
	note1=__int_clamp(note1, 0, 127);
	note2=__int_clamp(note2, 0, 127);

	fn=5;

	if(ch==8)
		fn=7;

	if(modfcn==carfcn)
		fn=modfcn;

	if(!(fbconn&1))
	{
//		modvol=modvol>>1;
		carvol=(carvol*vol)>>6;
		
		switch((fbconn>>1)&7)
		{
			case 0: modvol=0; break;
			case 1: modvol=modvol>>4; break;
			case 2: modvol=modvol>>3; break;
			case 3: modvol=modvol>>2; break;
			case 4: modvol=modvol>>1; break;
			case 5: modvol=modvol   ; break;
			case 6: modvol=modvol<<1; break;
			case 7: modvol=modvol<<2; break;
		}
		
		modvol=modvol<<1;
//		modvol=modvol<<2;
//		modvol=modvol<<3;
		
		modvol=__int_clamp(modvol, 0, 127);
		
	}else
	{
		modvol=(modvol*vol)>>6;
		carvol=(carvol*vol)>>6;
		fn=modfcn;
	}
	
//	modvol=vol;
//	carvol=vol;
	
	modvol=__int_clamp(modvol, 0, 127);
	carvol=__int_clamp(carvol, 0, 127);

	if(lbase)
	{
		dv1=lstep;
		dv2=0;
		att1=tk_midi_noteatt[vol];
		att2=tk_midi_noteatt[vol];
		
		modfcn=0;
		carfcn=0;

		rv1=dv1|(att1<<22)|((carfcn&7)<<28);
		rv2=dv2|(att2<<22)|((modfcn&7)<<28);

		rv1|=0x00100000;
//		if(prg<0x80)
		if(lflg&4)
			rv1|=0x00200000;
		if(lflg&8)
			rv2|=0x00200000;

		tk_midi_vnflg[vn1]|=1;
		tk_midi_vnflg[vn2]|=2;

		tk_midi_vnflg[vn1]|=8;
		tk_midi_vnflg[vn2]|=8;
		
//		if(!(tk_midi_regs[vn1*4+0]&0x00200000))
		if(tk_midi_regs[vn1*4+0]&0x00200000)
			tk_midi_vnflg[vn1]|=4;

#if 0
		if(	((tk_midi_regs[vn1*4+2]&0xFFFF)==lbase) &&
			(tk_midi_regs[vn1*4+0]&0x00200000))
		{
			rv2=(rv2&(~0xFFFF))|
				(tk_midi_regs[vn2*4+3]&0xFFFF);
		}
#endif

		tk_midi_regs[vn1*4+0]=rv1;
		tk_midi_regs[vn2*4+0]=rv2;

		tk_midi_regs[vn1*4+2]=lbase	| (llen<<16);
		tk_midi_regs[vn2*4+2]=lbeg	| (lend<<16);

//		if(	((tk_midi_regs[vn1*4+2]&0xFFFF)!=lbase) ||
//			!(tk_midi_regs[vn1*4+0]&0x00200000))
//		{
//			tk_midi_regs[vn2*4+3]=0;
//		}
	}
	else

//	if(vn2!=6)
	if(fvn&16)
	{
//		dv=tk_midi_notediv[d0];
		dv1=tk_midi_notediv[note1];
		dv2=tk_midi_notediv[note2];
		att1=tk_midi_noteatt[carvol];
		att2=tk_midi_noteatt[modvol];
		
		rv1=dv1|(att1<<22)|((carfcn&7)<<28);
		rv2=dv2|(att2<<22)|((modfcn&7)<<28);

//		if(!(fbconn&1) && (vn1<4))
		if(!(fbconn&1))
//		if(1)
		{
			rv1|=0x00200000;
		}

		tk_midi_vnflg[vn1]|=1;
		tk_midi_vnflg[vn2]|=2;

		tk_midi_regs[vn1*4+0]=rv1;
		tk_midi_regs[vn2*4+0]=rv2;
	}else
	{
//		dv=tk_midi_notediv[d0];
		dv=tk_midi_notediv[note];
		att1=tk_midi_noteatt[vol];
		
		rv1=dv|(att1<<22)|(fn<<28);

		tk_midi_vnflg[vn1]|=2;
//		tk_midi_regs[vn1]=rv1;
		tk_midi_regs[vn1*4+0]=rv1;
	}
	return(0);
}

int TK_Midi_PitchBlend(int ch, int d0)
{
	tk_midi_chanpbl[ch]=d0>>6;
	return(0);
}

int TK_Midi_Controller(int ch, int d0, int d1)
{

	if(d0==7)
	{
		tk_midi_chanvol[ch]=d1;
		return(1);
	}

	if(d0==10)
	{
		tk_midi_chanpan[ch]=d1;
		return(1);
	}

	if(d0==1)
	{
		tk_midi_chanmod[ch]=d1;
		return(1);
	}

	tk_dbg_printf("TK_Midi_Controller: ch=%d var=%d val=%d\n", ch, d0, d1);
	return(0);
}

int TK_Midi_ProgramChange(int ch, int d0)
{
	tk_dbg_printf("TK_Midi_ProgramChange: %d %d\n", ch, d0);
	tk_midi_chanprg[ch]=d0;
	return(0);
}


int TK_Midi_ParseVLI(byte **rcs)
{
	byte *cs;
	int i, j;

	cs=*rcs;
	
	i=*cs++; j=0;
	while(i&0x80)
	{
		j=(j<<7)|(i&127);
		i=*cs++;
	}
	j=(j<<7)|(i&127);
	
	*rcs=cs;
	return(j);
}

#if 0
int TK_Midi_ParseEvent()
{
	static int ld0, ld1;
	byte *cs;
	int tg, op, ch, d0, d1;
	int i, j, k;
	
	cs=tk_midi_cs;
	i=*cs++;

	tg=i;
	op=(tg>>4)&7;
	ch=tg&15;
	d0=*cs++;
	if((op!=0) && (op!=2) && (op!=3) && (op!=7))
	{
		if(op==1)
		{
			if(d0&128)
			{
				d0&=127;
				d1=*cs++;
			}else
			{
				d1=ld1;
			}

			ld0=d0;
			ld1=d1;
		}else
		{
			d1=*cs++;
		}
	}else
	{
		d1=0;
	}

	if(i&128)
	{
		k=TK_Midi_ParseVLI(&cs);
		tk_midi_tt+=k;
	}

	tk_midi_cs=cs;

//	if((op==6) ||
//		(tk_midi_cs>=tk_midi_cse))		//End Of Song
	if(tk_midi_cs>=tk_midi_cse)		//End Of Song
	{
		TK_Midi_SilenceAll();
		tk_midi_cs=tk_midi_css;		//Loop
		tk_midi_tt=1;
		return(1);
	}

	switch(op)
	{
	case 0:
		TK_Midi_NoteOff(ch, d0, d1);
		break;
	case 1:
		TK_Midi_NoteOn(ch, d0, d1);
		break;
	case 2:
		TK_Midi_PitchBlend(ch, d0<<6);
		break;

	case 3:
		switch(d0)
		{
		case 10: TK_Midi_Controller(ch, 120, 1);		break;
		case 11: TK_Midi_Controller(ch, 123, 1);		break;
		case 12: TK_Midi_Controller(ch, 126, 1);		break;
		case 13: TK_Midi_Controller(ch, 127, 1);		break;
		case 14: TK_Midi_Controller(ch, 121, 1);		break;
		}
		break;

	case 4:
		switch(d0)
		{
		case 0:		TK_Midi_ProgramChange(ch, d1); break;
		case 1:		TK_Midi_Controller(ch,  0, d1); break;
		case 2:		TK_Midi_Controller(ch,  1, d1); break;
		case 3:		TK_Midi_Controller(ch,  7, d1); break;
		case 4:		TK_Midi_Controller(ch, 10, d1); break;
		case 5:		TK_Midi_Controller(ch, 11, d1); break;
		case 6:		TK_Midi_Controller(ch, 91, d1); break;
		case 7:		TK_Midi_Controller(ch, 93, d1); break;
		case 8:		TK_Midi_Controller(ch, 64, d1); break;
		case 9:		TK_Midi_Controller(ch, 67, d1); break;
		}
		break;

	case 6:
		break;
	case 7:
		break;

	default:
		printf("SMus: Unknown Op %d\n", op);

	}
	
	return(0);
}
#endif

#if 0
int TK_Midi_Tick()
{
	int i;
	if(tk_midi_tt>0)
	{
		tk_midi_tt--;
//		return(0);
		return(1);
	}
	
	tk_midi_tt=0;
//	i=10;
	i=5;
//	while(!tk_midi_tt)
	while(!tk_midi_tt && ((i--)>0))
	{
		TK_Midi_ParseEvent();
	}
	return(i>0);
}


int imus_curms;
int imus_lastms;
int imus_accdt;

void I_MusicFineTick(void)	
{
#if 1
	int dt;

	if(i_mus_curhandle<=0)
		return;

	imus_curms=TK_GetTimeMs();
	dt=imus_curms-imus_lastms;
	imus_lastms=imus_curms;
	
	if(dt<0)
		dt=0;
	if(dt>250)
		dt=0;
	
	imus_accdt+=dt;
	while(imus_accdt>0)
	{
		TK_Midi_Tick();
		imus_accdt-=7;
	}
#endif
}

void I_MusicSubmit(void)	
{
	I_MusicFineTick();
}
#endif
