#include "rt_def.h"

//#include "i_system.h"
//#include "i_sound.h"
//#include "i_video.h"
//#include "i_net.h"
//#include "v_video.h"
//#include "w_wad.h"

#include "z_zone.h"

#include <math.h>

// #include "bgbmid1/bgbmid.h"

#include <tkgdi/tkgdi.h>

#include "i_genmidi.c"

TKGHSND hSndDev;

byte *i_smus_css;		//start position (active song)
byte *i_smus_cse;		//end position (active song)
byte *i_smus_cs;		//current position (active song)
int i_smus_tt;			//remaining tics until next event


#if 0
volatile u32 *smus_regs;

int smus_notediv[128];
byte smus_noteatt[128];

byte smus_chanvol[16];		//volume
byte smus_chanpbl[16];		//pitch blend
byte smus_chanprg[16];		//program
byte smus_chanpan[16];		//pan
byte smus_chanmod[16];		//modulation wheel

byte smus_chanvn[16];
byte smus_channt[16];

//byte smus_vnflg[16];		//voice flag
byte smus_vnflg[32];		//voice flag

u32 smus_fmregdata[256*16];


int I_SMus_Tick();

int smus_irq_tt;

int smus_timer_irq()
{
	int i;

//	return(0);

	if(smus_irq_tt>0)
	{
		smus_irq_tt--;
		return(0);
	}
	
	i=I_SMus_Tick();
	if(i>0)
		smus_irq_tt=7;
//	smus_irq_tt=234;
//	smus_irq_tt=14;
//	smus_irq_tt=20;
//	smus_irq_tt=28;
	return(0);
}

static int smus_isinit=0;

double smus_ipow2(double y)
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

int SMus_Init()
{
//	float freq, ph;
	double freq, ph;
	double *rph;
	int i, j, k;
	
	if(smus_isinit)
		return(0);
	smus_isinit=1;

//	irq_addTimerIrq(smus_timer_irq);

#if 0
	ph = pow(3.14159, 6.69);
	if((ph<2116) || (ph>2120))
	{
		rph = &ph;
		*rph = 3.14159;
		ph = 1.0 / ph;

		printf("1.0 / 3.14159 = %f (expect ~ 0.318310)\n", ph);
		
//		printf("frexp(3.14159) -> (%f * 2^%d)\n", ph, k);

		ph = pow(3.14159, 6.69);

		printf("ph = %f (expect ~ 2118), "
			"log(3.14159)=%f (expect 1.144729), "
			"exp(7.65823729) = %f (expect ~ 2118)\n",
			ph, log(3.14159), exp(7.65823729));
			
		ph = frexp(3.14159, &k);
		printf("frexp(3.14159) -> (%f * 2^%d)\n", ph, k);
		
		__debugbreak();
	}
#endif

#ifdef __ADDR_X48__
	smus_regs=(u32 *)0xFFFFF008C000ULL;
#else
//	smus_regs=(u32 *)0xA0081800UL;
//	smus_regs=(u32 *)0xF0081800UL;
	smus_regs=(u32 *)0xF008C000UL;
#endif
	
	for(i=0; i<128; i++)
	{
		smus_noteatt[i]=63-(i>>1);
		
//		freq=pow(2, (i-69)/12.0)*440;
		freq=pow(2.0, (i-69)/12.0)*440.0;
//		freq=smus_ipow2((i-69)/12.0)*440.0;
		
//		ph=freq/62500.0;
		ph=freq/64000.0;
//		ph=freq/32000.0;
//		ph=freq/128000.0;
//		ph=freq/176000.0;
//		ph=freq/256000.0;
		j=(1<<20)*ph+0.5;
		if(j>=(1<<20))j=(1<<20)-1;
		if(j<1)j=1;
		smus_notediv[i]=j;
		
//		__debugbreak();
	}
	return(0);
}

int SMus_SetFmRegisterData(int prg, int idx, u32 val)
{
	smus_fmregdata[(prg*16)+idx]=val;
	return(0);
}

int SMus_SilenceAll()
{
	int i;

	if(!smus_regs)
		return(0);

	for(i=0; i<16; i++)
	{
//		smus_regs[i*4+0]=0;
//		smus_regs[i*4+1]=0;
//		smus_regs[i*4+2]=0;
//		smus_regs[i*4+3]=0;

		smus_regs[i*8+0]=0;
		smus_regs[i*8+1]=0;
		smus_regs[i*8+2]=0;
		smus_regs[i*8+3]=0;
		smus_regs[i*8+4]=0;
		smus_regs[i*8+5]=0;
		smus_regs[i*8+6]=0;
		smus_regs[i*8+7]=0;
	}

	for(i=0; i<16; i++)
	{
		smus_chanvol[i]=127;
		smus_chanpbl[i]=128;
		
//		smus_vnflg[i]=0;
		smus_chanvn[i]=0xFF;
	}

	for(i=0; i<32; i++)
	{
		smus_vnflg[i]=0;
	}

	return(0);
}

int SMus_NoteOff(int ch, int d0, int d1)
{
	int vn1, vn2, cvn;


	cvn=smus_chanvn[ch];
	if(cvn==0xFF)
		return(0);

	if(smus_channt[ch]!=d0)
		return(0);

	smus_chanvn[ch]=0xFF;

	vn1=(cvn&15)*2+0;
	vn2=(cvn&15)*2+1;
	
	if(cvn&16)
	{
//		vn1=(cvn&7)|0;
//		vn2=(cvn&7)|8;
	}else
	{
//		vn1=cvn&15;
//		vn2=vn1;
	}


//	if((vn1<8) && (smus_vnflg[vn1]&1))
	if(cvn&16)
	{
//		smus_regs[vn1]=0;
		smus_regs[vn1*4+0]=0;
//		smus_regs[vn2]=0;
		smus_regs[vn2*4+0]=0;
		smus_vnflg[vn1]=0;
		smus_vnflg[vn2]=0;
	}else
	{
//		smus_regs[vn1]=0;
		smus_regs[vn1*4+0]=0;
		smus_vnflg[vn1]=0;
	}
	return(0);
}

int SMus_FindFreeVoice(int fl)
{
	int i;

	if(fl&1)
	{
//		for(i=0; i<6; i++)
//		for(i=0; i<5; i++)
//		for(i=0; i<8; i++)
		for(i=0; i<16; i++)
		{
//			if(!smus_vnflg[i] && !smus_vnflg[8+i])
//			if(!smus_vnflg[i*2+0])
			if(!smus_vnflg[i*2+0] && !smus_vnflg[i*2+1])
			{
				return(16|i);
			}
		}

#if 0
		i=7;
		if(!smus_vnflg[i] && !smus_vnflg[8+i])
		{
			return(16|i);
		}
#endif
	}

//	for(i=0; i<6; i++)
//	for(i=0; i<5; i++)
//	for(i=0; i<8; i++)
	for(i=0; i<16; i++)
	{
		if(!smus_vnflg[i*2+0])
		{
			return(i);
		}
	}

#if 0
	i=7;
	if(!smus_vnflg[i])
	{
		return(i);
	}
#endif

#if 0
//	for(i=8; i<14; i++)
	for(i=8; i<16; i++)
//	for(i=8; i<13; i++)
	{
		if(!smus_vnflg[i])
		{
			return(i);
		}
	}
#endif

#if 0
	i=15;
	if(!smus_vnflg[i])
	{
		return(i);
	}
#endif
	
	return(-1);
}

int SMus_NoteOn(int ch, int d0, int d1)
{
	u32		*rec;
	byte	*brec;
	u32		rv1, rv2;
	int		fvn, vn1, vn2, prg, fn, note;
	int 	dv, att1, att2, vol, pbl;
	int 	basenote, fl, loghz;

	byte modfcn, carfcn, modksc;
	byte modlvl, carlvl, carksc;
	byte modvol, carvol;
	byte fbconn;

//	if(ch==8)
//		return(0);
		
//	if(smus_chanvn[ch]!=0xFF)
//		return(0);

	note=d0;


	prg=smus_chanprg[ch];
	if(ch==8)
		prg+=128;
//	rec=smus_fmregdata[prg];
	rec=smus_fmregdata+(prg*16);
	brec=(byte *)rec;

	basenote=(short)(rec[4]>>16);

	modfcn=(brec[4+ 3])&255;
	modksc=(brec[4+ 4])&255;
	modlvl=(brec[4+ 5])&255;
	fbconn=(brec[4+ 6])&255;
	carfcn=(brec[4+10])&255;
	carksc=(brec[4+11])&255;
	carlvl=(brec[4+12])&255;


	fvn=smus_chanvn[ch];
	if(fvn==0xFF)
		fvn=-1;

	if(fvn<0)
	{
		fl=0;
		if(!(fbconn&1) || (modfcn!=carfcn))
			fl|=1;
//		fn=SMus_FindFreeVoice(0);
		fvn=SMus_FindFreeVoice(fl);
	}
	if(fvn<0)
	{
		smus_chanvn[ch]=0xFF;
		return(0);
	}

	smus_channt[ch]=note;
	vn1=(fvn&15)*2+0;
	vn2=(fvn&15)*2+1;

	if(fvn&16)
	{
		smus_chanvn[ch]=fvn;
//		vn1=(fvn&7);
//		vn2=(fvn&7)|8;
	}else
	{
		smus_chanvn[ch]=fvn;
//		vn1=fvn&7;
//		vn2=vn1;
	}
	
//	if(smus_vnflg[vn1]&3)
//	{
//		return(0);
//	}

	note+=basenote;
	if(note<0)note=0;

	loghz=((note-96)<<8)/12;

	vol=(smus_chanvol[ch]*d1)>>7;
//	vol=(vol*snd_MusicVolume)>>4;
	vol=(vol*snd_MusicVolume)>>7;

	vol=__int_clamp(vol, 0, 127);

//	vol=__int_clamp(vol, 0, 63);
//	if(vol<0)	vol=0;
//	if(vol>63)	vol=63;

	modvol=63-(modlvl&63);
	carvol=63-(carlvl&63);
	
	pbl=smus_chanpbl[ch];
	if(pbl)
	{
		pbl=(pbl>>3)-16;
	}
	
	note=d0+pbl;
	note=__int_clamp(note, 0, 127);

	fn=5;
//	fn=0;
//	fn=modfcn;

	if(ch==8)
		fn=7;

	if(modfcn==carfcn)
		fn=modfcn;

//	if(!(fbconn&1) && (vn1<8))
	if(!(fbconn&1))
	{
//		modvol=modvol<<1;
		modvol=modvol>>1;
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
		modvol=__int_clamp(modvol, 0, 127);
		
	}else
	{
		modvol=(modvol*vol)>>6;
		carvol=(carvol*vol)>>6;
		fn=modfcn;
	}
	
	modvol=vol;
	carvol=vol;
	
	modvol=__int_clamp(modvol, 0, 127);
	carvol=__int_clamp(carvol, 0, 127);

//	if(vn2!=6)
	if(fvn&16)
	{
//		dv=smus_notediv[d0];
		dv=smus_notediv[note];
		att1=smus_noteatt[carvol];
		att2=smus_noteatt[modvol];
		
		rv1=dv|(att1<<22)|((carfcn&7)<<28);
		rv2=dv|(att2<<22)|((modfcn&7)<<28);

//		if(!(fbconn&1) && (vn1<4))
		if(!(fbconn&1))
//		if(1)
		{
			rv1|=0x00200000;
//			smus_vnflg[vn1]|=1;
//			smus_vnflg[vn2]|=2;
		}

		smus_vnflg[vn1]|=1;
		smus_vnflg[vn2]|=2;

//		smus_regs[vn1]=rv1;
//		smus_regs[vn2]=rv2;

		smus_regs[vn1*4+0]=rv1;
		smus_regs[vn2*4+0]=rv2;

//		smus_regs[vn2*4+0]=rv1;
//		smus_regs[vn1*4+0]=rv2;
	}else
	{
//		dv=smus_notediv[d0];
		dv=smus_notediv[note];
		att1=smus_noteatt[vol];
		
		rv1=dv|(att1<<22)|(fn<<28);

		smus_vnflg[vn1]|=2;
//		smus_regs[vn1]=rv1;
		smus_regs[vn1*4+0]=rv1;
	}
	return(0);
}

int SMus_PitchBlend(int ch, int d0)
{
	smus_chanpbl[ch]=d0>>6;
	return(0);
}

int SMus_Controller(int ch, int d0, int d1)
{

	if(d0==7)
	{
		smus_chanvol[ch]=d1;
		return(1);
	}

	if(d0==10)
	{
		smus_chanpan[ch]=d1;
		return(1);
	}

	if(d0==1)
	{
		smus_chanmod[ch]=d1;
		return(1);
	}

//	switch(d0)
//	{
//	case 7:
//		smus_chanvol[ch]=d1;
//		break;
//	}

	printf("SMus_Controller: ch=%d var=%d val=%d\n", ch, d0, d1);
	return(0);
}

int SMus_ProgramChange(int ch, int d0)
{
	printf("SMus_ProgramChange: %d %d\n", ch, d0);
	smus_chanprg[ch]=d0;
	return(0);
}

#endif

int SMus_Init()
{
	SMus_SilenceAll();
}

int SMus_SetFmRegisterData(int prg, int idx, u32 val)
{
	TKGDI_MIDI_COMMAND t_mcmd;
	TKGDI_MIDI_COMMAND *mcmd;

	mcmd=&t_mcmd;
	mcmd->op=16;
//	mcmd->ch=ch;
	mcmd->d0=prg;
	mcmd->d1=idx;
	mcmd->u0=val;
	
//	tkgModifyAudioDevice(hSndDev, TKGDI_FCC_mcmd, mcmd, NULL);
	tkgDeviceMidiCommand(hSndDev, mcmd);
	return(0);
}

int SMus_SilenceAll()
{
	SMus_SpecialParm(0, 0);
//	SMus_SpecialParm(1, snd_MusicVolume);
//	SMus_SpecialParm(1, snd_MusicVolume>>2);
	SMus_SpecialParm(1, snd_MusicVolume>>5);
}

int SMus_UpdateVolume()
{
//	SMus_SpecialParm(0, 0);
//	SMus_SpecialParm(1, snd_MusicVolume);
//	SMus_SpecialParm(1, snd_MusicVolume>>2);
	SMus_SpecialParm(1, snd_MusicVolume>>5);
}

int SMus_SpecialParm(int parm, int val)
{
	TKGDI_MIDI_COMMAND t_mcmd;
	TKGDI_MIDI_COMMAND *mcmd;

	mcmd=&t_mcmd;
	mcmd->op=17;
	mcmd->ch=0;
	mcmd->d0=parm;
	mcmd->d1=val;
	
//	tkgModifyAudioDevice(hSndDev, TKGDI_FCC_mcmd, mcmd, NULL);
	tkgDeviceMidiCommand(hSndDev, mcmd);
	return(0);
}

int SMus_NoteOff(int ch, int d0, int d1);

int SMus_NoteOn(int ch, int d0, int d1)
{
	TKGDI_MIDI_COMMAND t_mcmd;
	TKGDI_MIDI_COMMAND *mcmd;

	if(d1<0)
		d1=0;
	if(d1>127)
		d1=127;

	if(!d1)
	{
//		SMus_NoteOff(ch, d0, d1);
		SMus_NoteOff(ch, d0, 64);
		return(0);
	}
	
	mcmd=&t_mcmd;
	mcmd->op=1;
	mcmd->ch=ch;
	mcmd->d0=d0;
	mcmd->d1=d1;
	
//	tkgModifyAudioDevice(hSndDev, TKGDI_FCC_mcmd, mcmd, NULL);
	tkgDeviceMidiCommand(hSndDev, mcmd);
	return(0);
}

int SMus_NoteOff(int ch, int d0, int d1)
{
	TKGDI_MIDI_COMMAND t_mcmd;
	TKGDI_MIDI_COMMAND *mcmd;

	mcmd=&t_mcmd;
	mcmd->op=0;
	mcmd->ch=ch;
	mcmd->d0=d0;
	mcmd->d1=d1;
	
//	tkgModifyAudioDevice(hSndDev, TKGDI_FCC_mcmd, mcmd, NULL);
	tkgDeviceMidiCommand(hSndDev, mcmd);
	return(0);
}

int SMus_PitchBlend(int ch, int d0)
{
	TKGDI_MIDI_COMMAND t_mcmd;
	TKGDI_MIDI_COMMAND *mcmd;

	mcmd=&t_mcmd;
	mcmd->op=2;
	mcmd->ch=ch;
	mcmd->d0=d0;
//	mcmd->d1=d1;
	
//	tkgModifyAudioDevice(hSndDev, TKGDI_FCC_mcmd, mcmd, NULL);
	tkgDeviceMidiCommand(hSndDev, mcmd);
	return(0);
}

int SMus_Controller(int ch, int d0, int d1)
{
	TKGDI_MIDI_COMMAND t_mcmd;
	TKGDI_MIDI_COMMAND *mcmd;

	mcmd=&t_mcmd;
	mcmd->op=3;
	mcmd->ch=ch;
	mcmd->d0=d0;
	mcmd->d1=d1;
	
//	tkgModifyAudioDevice(hSndDev, TKGDI_FCC_mcmd, mcmd, NULL);
	tkgDeviceMidiCommand(hSndDev, mcmd);
	return(0);
}

int SMus_ProgramChange(int ch, int d0)
{
	TKGDI_MIDI_COMMAND t_mcmd;
	TKGDI_MIDI_COMMAND *mcmd;

	mcmd=&t_mcmd;
	mcmd->op=4;
	mcmd->ch=ch;
	mcmd->d0=d0;
//	mcmd->d1=d1;
	
//	tkgModifyAudioDevice(hSndDev, TKGDI_FCC_mcmd, mcmd, NULL);
	tkgDeviceMidiCommand(hSndDev, mcmd);
	return(0);
}


int I_SMus_ParseVLI(byte **rcs)
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

int I_SMus_ParseEvent()
{
	static int ld0, ld1;
	byte *cs;
	int tg, op, ch, d0, d1;
	int i, j, k;
	
	cs=i_smus_cs;
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
		k=I_SMus_ParseVLI(&cs);
		i_smus_tt+=k;
	}

	i_smus_cs=cs;

//	if((op==6) ||
//		(i_smus_cs>=i_smus_cse))		//End Of Song
	if(i_smus_cs>=i_smus_cse)		//End Of Song
	{
		SMus_SilenceAll();
		i_smus_cs=i_smus_css;		//Loop
		i_smus_tt=1;
		return(1);
	}

	switch(op)
	{
	case 0:
		SMus_NoteOff(ch, d0, d1);
		break;
	case 1:
		SMus_NoteOn(ch, d0, d1);
		break;
	case 2:
		SMus_PitchBlend(ch, d0<<6);
		break;

	case 3:
		switch(d0)
		{
		case 10: SMus_Controller(ch, 120, 1);		break;
		case 11: SMus_Controller(ch, 123, 1);		break;
		case 12: SMus_Controller(ch, 126, 1);		break;
		case 13: SMus_Controller(ch, 127, 1);		break;
		case 14: SMus_Controller(ch, 121, 1);		break;
		}
		break;

	case 4:
		switch(d0)
		{
		case 0:		SMus_ProgramChange(ch, d1); break;
		case 1:		SMus_Controller(ch,  0, d1); break;
		case 2:		SMus_Controller(ch,  1, d1); break;
		case 3:		SMus_Controller(ch,  7, d1); break;
		case 4:		SMus_Controller(ch, 10, d1); break;
		case 5:		SMus_Controller(ch, 11, d1); break;
		case 6:		SMus_Controller(ch, 91, d1); break;
		case 7:		SMus_Controller(ch, 93, d1); break;
		case 8:		SMus_Controller(ch, 64, d1); break;
		case 9:		SMus_Controller(ch, 67, d1); break;
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

int I_SMus_Tick()
{
	int i;
	if(i_smus_tt>0)
	{
		i_smus_tt--;
//		return(0);
		return(1);
	}
	
	i_smus_tt=0;
//	i=10;
	i=5;
//	while(!i_smus_tt)
	while(!i_smus_tt && ((i--)>0))
	{
		I_SMus_ParseEvent();
	}
	return(i>0);
}


#if 1
byte *i_mus_songs[32];
int i_mus_song_mask;

int i_mus_song_len[32];
int i_mus_song_ofs[32];

short *i_mus_pcm_buf[32];
int i_mus_pcm_len[32];

//BGBMID_Context *bmid_ctx;
byte *genmidi;

int i_mus_curhandle;
int i_mus_curpos;

int i_mus_pause;

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
		I_SMus_Tick();
		imus_accdt-=7;
	}
#endif
}

void I_MusicSubmit(void)	
{
	I_MusicFineTick();
}


//
// MUSIC API.
// Still no music done.
// Remains. Dummies.
//

void I_InitMusic(void)	
{
	byte *rec;
	int i, j, k;

	SMus_Init();

#if 0
//	genmidi = W_CacheLumpName("GENMIDI", PU_STATIC);
	genmidi = (byte *)i_genmidi0;

	if(genmidi)
	{
		for(i=0; i<175; i++)
		{
			rec=(u32 *)(genmidi+8+(i*36));
//			BGBMID_SetFmRegisterData(bmid_ctx, i, 0, ((u32 *)rec)[0]);
//			BGBMID_SetFmRegisterData(bmid_ctx, i, 1, ((u32 *)rec)[1]);
//			BGBMID_SetFmRegisterData(bmid_ctx, i, 2, ((u32 *)rec)[2]);
//			BGBMID_SetFmRegisterData(bmid_ctx, i, 3, ((u32 *)rec)[3]);
//			BGBMID_SetFmRegisterData(bmid_ctx, i, 4, ((u32 *)rec)[4]);
//			BGBMID_SetFmRegisterData(bmid_ctx, i, 5, ((u32 *)rec)[5]);
//			BGBMID_SetFmRegisterData(bmid_ctx, i, 6, ((u32 *)rec)[6]);
//			BGBMID_SetFmRegisterData(bmid_ctx, i, 7, ((u32 *)rec)[7]);
//			BGBMID_SetFmRegisterData(bmid_ctx, i, 8, ((u32 *)rec)[8]);

			SMus_SetFmRegisterData(i, 0, ((u32 *)rec)[0]);
			SMus_SetFmRegisterData(i, 1, ((u32 *)rec)[1]);
			SMus_SetFmRegisterData(i, 2, ((u32 *)rec)[2]);
			SMus_SetFmRegisterData(i, 3, ((u32 *)rec)[3]);
			SMus_SetFmRegisterData(i, 4, ((u32 *)rec)[4]);
			SMus_SetFmRegisterData(i, 5, ((u32 *)rec)[5]);
			SMus_SetFmRegisterData(i, 6, ((u32 *)rec)[6]);
			SMus_SetFmRegisterData(i, 7, ((u32 *)rec)[7]);
			SMus_SetFmRegisterData(i, 8, ((u32 *)rec)[8]);
		}
	}
#endif
}

void I_ShutdownMusic(void)
{
}

static int	looping=0;
static int	musicdies=-1;

void I_PlaySong(int handle, int looping)
{
	byte *buf;
	short *sbuf;
	int ofs, sz;
	int len;
	int i, j, k;
	
	I_InitMusic();
	SMus_UpdateVolume();
	
	ofs=i_mus_song_ofs[handle];
	sz=i_mus_song_len[handle];
	buf=i_mus_songs[handle]+ofs;
	
	printf("I_PlaySong: ofs=%d sz=%d\n", ofs, sz);
	
  // UNUSED.
//  handle = looping = 0;
//  musicdies = gametic + TICRATE*30;

//	if(!bmid_ctx)
//		return;

	i_smus_css=buf;
	i_smus_cse=buf+sz;
	i_smus_cs=buf;
	i_smus_tt=0;
	SMus_SilenceAll();

	i_mus_curhandle=handle;
	i_mus_curpos=0;
}

void I_PauseSong (int handle)
{
  // UNUSED.
//  handle = 0;

	i_mus_pause=1;
}

void I_ResumeSong (int handle)
{
  // UNUSED.
//  handle = 0;

	i_mus_pause=0;
}

void I_StopSong(int handle)
{
  // UNUSED.
//  handle = 0;
  
//  looping = 0;
//  musicdies = 0;

	i_mus_curhandle=0;
	i_mus_curpos=0;
	SMus_SilenceAll();
}

void I_UnRegisterSong(int handle)
{
	i_mus_song_mask&=~(1<<handle);
	
//	if(i_mus_pcm_buf[handle])
//	{
//		bgbmid_free(i_mus_pcm_buf[handle]);
//		i_mus_pcm_buf[handle]=NULL;
//	}
}

u16 i_getu16be(byte *ptr)
{
	return((ptr[0]<<8)|(ptr[1]<<0));
}

u32 i_getu32be(byte *ptr)
{
	return((ptr[0]<<24)|(ptr[1]<<16)|(ptr[2]<<8)|(ptr[3]<<0));
}

byte *I_SMus_EmitVLI(byte *ct, int v)
{
	if(v<128)
	{
		*ct++=(v&127);
	}else if(v<16384)
	{
		*ct++=0x80|((v>>7)&127);
		*ct++=(v&127);
	}else if(v<2097152)
	{
		*ct++=0x80|((v>>14)&127);
		*ct++=0x80|((v>>7)&127);
		*ct++=(v&127);
	}
	return(ct);
}

byte *I_SMus_EmitVLIB(byte *ct, int v)
{
	if(v<16384)
	{
		*ct++=0x80|((v>>7)&127);
		*ct++=(v&127);
	}else if(v<2097152)
	{
		*ct++=0x80|((v>>14)&127);
		*ct++=0x80|((v>>7)&127);
		*ct++=(v&127);
	}
	return(ct);
}

int I_SongMergeTracks(byte *obuf, byte **trkp, int *trksz, int ntrk, int ttsc)
{
	int trkdel[32];
	byte *trkcs[32];
	byte *trkcse[32];
	int trkltg[32];
	byte *ct, *cs0, *cs, *cse;
	int tg, op, ch, d0, d1, i0, i1, i2;
	int i, j, k, ts, ts1;
	
	for(i=0; i<ntrk; i++)
	{
		trkdel[i]=0;
		trkcs[i]=trkp[i];
		trkcse[i]=trkp[i]+trksz[i];
	}
	
	ct=obuf;

	for(i=0; i<ntrk; i++)
	{
		cs=trkcs[i];
		cse=trkcse[i];
		if(cs>=cse)
			continue;
		k=I_SMus_ParseVLI(&cs);
		trkdel[i]=k;
//		trkcs[i]=cs;
	}
	
	while(1)
	{
		ts=999999999;
		for(i=0; i<ntrk; i++)
		{
			cs=trkcs[i];
			cse=trkcse[i];
			if(cs>=cse)
				continue;

			if(trkdel[i]<=ts)
				ts=trkdel[i];
		}
		
		if(ts==999999999)
			break;
		
		ts1=(ts*ttsc)>>8;
		if(ts1>0)
		{
//			ct=I_SMus_EmitVLIB(ct, ts);

			*ct++=0x80;
			*ct++=0x00;
			ct=I_SMus_EmitVLI(ct, ts1);
		}

		i0=0;
//		if(ts>0)
//			i0=0x80;

		for(i=0; i<ntrk; i++)
		{
			cs=trkcs[i];
			cse=trkcse[i];
			if(cs>=cse)
				continue;

			trkdel[i]-=ts;
		
			if(trkdel[i]<=0)
			{
				cs0=cs;

				k=I_SMus_ParseVLI(&cs);
//				trkdel[i]=k;

//				printf("step trk=%d b=%02X step=%dB\n", i, *cs0, cs-cs0);

				cs0=cs;
//				tg=*cs++;

				j=*cs;
				if(j&128)
					{ tg=j; cs++; }
				else
					{ tg=trkltg[i]; }
				trkltg[i]=tg;

//				if(!(tg&128))
//				{
//					printf("I_SongMergeTracks: Un-Sync trk=%d v=%02X\n", i, tg);
//					cs=trkcse[i];
//					trkcs[i]=cs;
//					continue;
//				}
				
				if((tg&0xF0)==0xF0)
				{
//					printf("I_SongMergeTracks: SysEx trk=%d v=%02X\n", i, tg);

					if(tg==0xFF)
					{
						j=*cs++;
						if(j==0x2F)
						{
//							printf("I_SongMergeTracks: EOT\n");
							cs=trkcse[i];
							trkcs[i]=cs;
							continue;
						}
						
						k=I_SMus_ParseVLI(&cs);
						cs+=k;

//						k=I_SMus_ParseVLI(&cs);
//						trkdel[i]=k;
//						trkcs[i]=cs;
//						continue;
					}

					if(tg==0xF0)
					{
						j=*cs++;
						while(!(j&128))
							j=*cs++;
						if(j!=0xF7)
							cs--;
					}else
						if((tg==0xF1) || (tg==0xF2) || (tg==0xF3))
					{
						j=*cs++;
					}
					else
					{
					}
					
//					printf("I_SongMergeTracks: Non-EOX %d\n", j);
//					k=I_SMus_ParseVLI(&cs);
//					trkdel[i]=k;
					trkcs[i]=cs;
					continue;
				}

				op=(tg>>4)&7;
				ch=tg&15;
				ch=i;
				if((op!=0x4) && (op!=0x5))
				{
					d0=cs[0];
					d1=cs[1];
					cs+=2;
				}else
				{
					d0=cs[0];
					d1=0;
					cs++;
				}

//				printf("I_SongMergeTracks: trk=%d tg=%02X d0=%d d1=%d\n",
//					i, tg, d0, d1);
			
				switch(op)
				{
				case 0x0:	/* Note Off */
					*ct++=0x00|ch;
					*ct++=d0&127;
					break;
				case 0x1: /* Note On */
					if(d1==0)
					{
						*ct++=0x00|ch;
						*ct++=d0&127;
						break;
					}
				
					*ct++=0x10|ch;
//					*ct++=d0&127;
					*ct++=0x80|d0;
					*ct++=(d1&127);
					break;
				case 0x2: /* Note Aftertouch / Key Pressure */
					break;
				case 0x3:	/* Controller */
					k=0;
					switch(d0&127)
					{
						case 120: k=10; break;
						case 123: k=11; break;
						case 126: k=12; break;
						case 127: k=13; break;
						case 121: k=14; break;
					}
					if(!d1)k=0;
					if(k>0)
					{
						*ct++=0x30|ch;
						*ct++=k;
						break;
					}
					switch(d0&127)
					{
						case 0:  k=1; break;
						case 1:  k=2; break;
						case 7:  k=3; break;
						case 10: k=4; break;
						case 11: k=5; break;
						case 91: k=6; break;
						case 93: k=7; break;
						case 64: k=8; break;
						case 67: k=9; break;
					}
					if(k>0)
					{
						*ct++=0x40|ch;
						*ct++=k;
						*ct++=d1&127;
						break;
					}
					
					break;
				case 0x4:	/* Program Change */
					*ct++=0x40|ch;
					*ct++=0x00;
					*ct++=d0&127;
					break;
				case 0x5: /* Channel Aftertouch */
					break;
				case 0x6: /* Pitch Wheel */
					break;
				case 0x7:
					printf("Bad Tag 7\n");
//					cs+=d0-1;
//					while(!((*cs)&128))
//						cs++;
					break;
				}

//				k=I_SMus_ParseVLI(&cs);
//				trkdel[i]=k;
				trkcs[i]=cs;

				k=I_SMus_ParseVLI(&cs);
				trkdel[i]=k;
			}
		}
	}
	
	*ct++=0x60;
	
	return(ct-obuf);
}

int I_RegisterSong(void* data, int sz)
{
	byte *trkp[32];
	int trksz[32];
	char tb[5];
	byte *tbuf;
	byte *cs, *cse;
	int i, j, k, h, n, sz1, tsc;
	
	if(!data)
		return(0);
	
	for(i=1; i<32; i++)
		if(!(i_mus_song_mask&(1<<i)))
			break;
			
	h=i;
	i_mus_song_mask|=(1<<h);
	
//	i_mus_songs[h]=data;
//	i_mus_song_len[i]=((u16 *)data)[2];
//	i_mus_song_ofs[i]=((u16 *)data)[3];
	
	
//	((u32 *)tb)[0]=((u32 *)data)[0];
//	tb[4]=0;
	
	printf("%08X %08X %08X %08X  %s\n",
		((u32 *)data)[0], ((u32 *)data)[1],
		((u32 *)data)[2], ((u32 *)data)[3],
		tb);

	n=0;	tsc=256;
	cs=data;	cse=cs+sz;
	while(cs<cse)
	{
		i=((u32 *)cs)[0];
//		j=((u32 *)cs)[1];
		j=i_getu32be(cs+4);
		if(!i)
			break;
		if(j<0)
			break;

		((u32 *)tb)[0]=i;
		tb[4]=0;
		printf("%s %d\n", tb, j);

		if(i==0x6468544D)
		{
			k=(s16)(i_getu16be(cs+12));
			printf("tDiv=%d (%04X)\n", k, k);
//			if(k<0)

			tsc=(144*256)/k;

			cs+=8+j;
			continue;
		}

		if(i==0x6B72544D)
		{
			trkp[n]=cs+8;
			trksz[n]=j;
			n++;
			cs+=8+j;
			continue;
		}

		((u32 *)tb)[0]=i;
		tb[4]=0;

		printf("%s %d\n", tb, j);
			
		cs+=8+j;
	}

	printf("ntrack=%d\n", n);

	tbuf=Z_Malloc(sz*2, PU_STATIC, &(i_mus_songs[h]));
	sz1=I_SongMergeTracks(tbuf, trkp, trksz, n, tsc);

	i_mus_songs[h]=tbuf;
	i_mus_song_ofs[h]=0;
	i_mus_song_len[h]=sz1;
	
	return(h);

//  data = NULL;
//  return 1;
}

// Is the song playing?
int I_QrySongPlaying(int handle)
{
  // UNUSED.
  handle = 0;
//  return looping || musicdies > gametic;
}
#endif
