#include <bgbmid.h>

#ifdef _WIN32
__declspec(dllexport) void dummy()
{
}
#endif

int main(int argc, char *argv[])
{
	byte tb[256];
	FILE *fd;
	BGBMID_Context *ctx;
	short *obuf;
	byte *buf, *cs, *ce;
	char *ifn, *ofn, *s, *s0, *s1;
	char *playfn;
	float ratesc, freqsc;
	double adt, adt2;
	int lv, dt, tg, d0, d1, op, ch, sz;
	int t0, t1, t2, t3;
	int i, j, k, noteticks, ntrk;

//	BGBMID_DumpPatches();
//	dummy();

	ratesc=1; freqsc=1;
	ifn=NULL; ofn=NULL; playfn=NULL;

	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			//set rate scale
			if(!strcmp(argv[i], "-rt"))
			{
				ratesc=atof(argv[i+1]);
				i++;
				continue;
			}

			//set frequency scale
			if(!strcmp(argv[i], "-fr"))
			{
				freqsc=atof(argv[i+1]);
				i++;
				continue;
			}

			if(!strncmp(argv[i], "-patch", 6))
			{
				s=argv[i]+6;
//				j=atoi(s);
				s0=s;
				while(*s0 && (*s0!='='))s0++;
				if(*s0=='=')s0++;
				
				s1=s;
				while(*s1 && (*s1!='='))
				{
					j=atoi(s1);
					printf("Set Patch %d=%s\n", j, s0);
					BGBMID_SetPatchName(j, s0);

					while(*s1 && (*s1!='=') && (*s1!=','))s1++;
					if(*s1==',')s1++;
				}
			}

			if(!strcmp(argv[i], "-play"))
			{
				playfn=argv[i+1];
				i++;
				continue;
			}

			continue;
		}

		if(!ifn) { ifn=argv[i]; continue; }
		if(!ofn) { ofn=argv[i]; continue; }
	}

	if(!ifn)return(-1);
	if(!ofn)ofn="out.wav";

	if(!strcmp(ifn, "NONE"))
	{
		buf=NULL;
		sz=0;
		noteticks=172;
	}else
	{
		BGBMID_ReadMidiMerge(ifn, &buf, &sz, &noteticks);
	}

	BGBMID_LoadPackGlobal("instruments.pak");

	ctx=BGBMID_NewContext();
	BGBMID_SetNoteTicks(ctx, noteticks);
	BGBMID_SetRateScale(ctx, ratesc);
	BGBMID_SetFreqScale(ctx, freqsc);
	BGBMID_SetNoteLen(ctx, 1.0);

//	BGBMID_SetReverbBaseEffect(ctx, 1.0, 1.0);
//	BGBMID_SetReverbBaseEffect(ctx, 0.5, 2.0);

//	BGBMID_SetReverbBaseEffect(ctx, 1.0, 2.0);

//	BGBMID_SetReverbBaseEffect(ctx, 0.1, 0.5);

//	BGBMID_SetupReverbSinc(ctx, 4.0);
	BGBMID_SetupReverbSinc(ctx, 2.0);
//	BGBMID_SetupReverbSinc(ctx, 1.0);
//	BGBMID_SetupReverbCosc(ctx, 1.0);
//	BGBMID_SetupReverbCosc(ctx, 0.1);
//	BGBMID_SetupReverbCos(ctx, 0.1);

//	BGBMID_SetFilterEffect(ctx, 0.0, 1.0);
//	BGBMID_SetupFilterCosc(ctx, 8.0);
	BGBMID_SetupFilterCosc(ctx, 1.0);
//	BGBMID_SetupFilterFalloff(ctx, 0.1);

	BGBMID_SetOutputBuffer(ctx, NULL, 1<<24);

	if(!strcmp(ofn, "DSP"))
	{
		BGBMID_InitChannels(ctx);
		BGBMID_SetInputBuffer(ctx, buf, sz);

#if 0
//		BGBMID_PlaySampleName(ctx, 
//			"WoodenToaster_AvastFluttershysQuarterSlot");
//		BGBMID_PlaySampleName(ctx, 
//			"WoodenToaster_RainbowFactory");
//		BGBMID_PlaySampleName(ctx, 
//			"WoodenToaster_BeyondHerGarden");

//		BGBMID_PlaySampleName(ctx, 
//			"WoodenToaster_AvastFluttershysQuarterSlot_dump");
//		BGBMID_PlaySampleName(ctx, 
//			"WoodenToaster_BeyondHerGarden_dump");
//		BGBMID_PlaySampleName(ctx, 
//			"WoodenToaster_RainbowFactory_dump");
#endif
		if(playfn)
			{ BGBMID_PlaySampleName(ctx, playfn); }

		BGBMID_SoundDev_Init();

//		BGBMID_PlayMixTime(ctx, 50/1000.0);

		adt=0; adt2=0;

		t0=BGBMID_TimeMS();
		while(1)
		{
			t1=BGBMID_TimeMS();
			if((t1-t0)<15)
			{
				thSleep(5);
				continue;
			}

			k=32+rand()&31;
			thSleep(k);
			

//			printf("T=%d, Dt=%d ", t1, t1-t0);
			printf("T=%3d:%02d.%03d, Dt=%d ",
				t1/60000, (t1/1000)%60, t1%1000, t1-t0);

//			BGBMID_VoxEnv_UpdateModelListener(ctx, 0);
			BGBMID_PlayMixTime(ctx, (t1-t0)/1000.0);

			t2=BGBMID_TimeMS();
			adt+=t1-t0;
			adt2+=t2-t1;
			printf("\tDt2=%2d Adt2=%.2f", t2-t1, adt2/adt);

			printf("        \r");
			
			t0=t1;
		}
		printf("\n");
		BGBMID_SoundDev_DeInit();
	}else
	{
//		i=BGBMID_SynthStream(ctx, buf, sz);

		BGBMID_InitChannels(ctx);
		BGBMID_SetInputBuffer(ctx, buf, sz);

//		BGBMID_PlaySampleName(ctx, 
//			"WoodenToaster_RainbowFactory");
//		BGBMID_PlaySampleName(ctx, 
//			"WoodenToaster_BeyondHerGarden");

		if(playfn)
			{ BGBMID_PlaySampleName(ctx, playfn); }

		BGBMID_VoxEnv_UpdateModelListener(ctx, 0);
		
		if(!buf)
		{
			ctx->dt+=5*60*172;
			ctx->not_dt=1;
		}
		
		i=BGBMID_DoSynth(ctx);

		printf("Stream End %d\n", i);

//		BGBMID_DumpMidi0(ofn, buf, sz, noteticks);

		obuf=BGBMID_GetOutputBuffer(ctx);
		i=BGBMID_GetOutputBufferPos(ctx);
		j=BGBMID_GetOutputSampleRate(ctx);
		BGBMID_StoreWAV(ofn, (byte *)obuf, 2, j, 16, i);
	}

	return(0);
}
