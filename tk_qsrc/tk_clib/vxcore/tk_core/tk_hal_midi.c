volatile u32 *tk_midi_regs;

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

u32 tk_midi_fmregdata[256*16];

byte tk_midi_musicvolume = 15;

int TK_Midi_Tick();

int tk_midi_irq_tt;

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

int TK_Midi_SetMasterParam(int var, int val)
{
	if(var==1)
	{
		tk_midi_musicvolume=val;
		return(0);
	}

	return(-1);
}

int TK_Midi_Init()
{
//	float freq, ph;
	double freq, ph;
	double *rph;
	int i, j, k;
	
	if(!tk_midi_regs)
		tk_midi_isinit=0;
	
	if(tk_midi_isinit)
		return(0);
	tk_midi_isinit=1;

	tk_midi_regs=(u32 *)0xFFFFF008C000ULL;
	
	for(i=0; i<128; i++)
	{
		tk_midi_noteatt[i]=63-(i>>1);
		
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
	return(0);
}

int TK_Midi_SetFmRegisterData(int prg, int idx, u32 val)
{
	tk_midi_fmregdata[(prg*16)+idx]=val;
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
	}

	return(0);
}

int TK_Midi_NoteOff(int ch, int d0, int d1)
{
	int vn1, vn2, cvn;

	if(!tk_midi_regs)
		return(0);

	cvn=tk_midi_chanvn[ch];
	if(cvn==0xFF)
		return(0);

	if(tk_midi_channt[ch]!=d0)
		return(0);

	tk_midi_chanvn[ch]=0xFF;

	vn1=(cvn&15)*2+0;
	vn2=(cvn&15)*2+1;

	if(cvn&16)
	{
		tk_midi_regs[vn1*4+0]=0;
		tk_midi_regs[vn2*4+0]=0;
		tk_midi_vnflg[vn1]=0;
		tk_midi_vnflg[vn2]=0;
	}else
	{
		tk_midi_regs[vn1*4+0]=0;
		tk_midi_vnflg[vn1]=0;
	}
	return(0);
}

int TK_Midi_FindFreeVoice(int fl)
{
	int i;

	if(fl&1)
	{
		for(i=0; i<16; i++)
		{
			if(!tk_midi_vnflg[i*2+0] && !tk_midi_vnflg[i*2+1])
			{
				return(16|i);
			}
		}
	}

	for(i=0; i<16; i++)
	{
		if(!tk_midi_vnflg[i*2+0])
		{
			return(i);
		}
	}
	
	return(-1);
}

int TK_Midi_NoteOn(int ch, int d0, int d1)
{
	u32		*rec;
	byte	*brec;
	u32		rv1, rv2;
	int		fvn, vn1, vn2, prg, fn, note, note1, note2;
	int 	dv, dv1, dv2, att1, att2, vol, pbl;
	int 	basenote, fl, loghz;

	byte modfcn, carfcn, modksc;
	byte modlvl, carlvl, carksc;
	byte modvol, carvol;
	byte fbconn, fineadj, fixnote, fixflag;

	note=d0;


	if(!tk_midi_regs)
		return(0);

	prg=tk_midi_chanprg[ch];
	if(ch==8)
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


	fvn=tk_midi_chanvn[ch];
	if(fvn==0xFF)
		fvn=-1;

	if(fvn<0)
	{
		fl=0;
		if(!(fbconn&1) || (modfcn!=carfcn) || (fixflag&4))
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
	note=__int_clamp(note, 0, 127);

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

	printf("TK_Midi_Controller: ch=%d var=%d val=%d\n", ch, d0, d1);
	return(0);
}

int TK_Midi_ProgramChange(int ch, int d0)
{
	printf("TK_Midi_ProgramChange: %d %d\n", ch, d0);
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
