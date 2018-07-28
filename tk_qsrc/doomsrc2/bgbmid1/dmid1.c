#include "bgbmid.h"

#include "bgbmid.c"

int main(int argc, char *argv[])
{
	byte tb[256];
	FILE *fd;
	BGBMID_Context *ctx;
	short *obuf;
	byte *buf, *cs, *ce;
	char *ifn, *ofn;
	float ratesc;
	int lv, dt, tg, d0, d1, op, ch, sz;
	int i, j, k, noteticks, ntrk;

//	BGBMID_DumpPatches();

	ratesc=1;
	ifn=NULL; ofn=NULL;

	for(i=1; i<argc; i++)
	{
		if(argv[i][0]=='-')
		{
			if(!strcmp(argv[i], "-rt"))
			{
				ratesc=atof(argv[i+1]);
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

	BGBMID_ReadMidiMerge(ifn, &buf, &sz, &noteticks);

	ctx=BGBMID_NewContext();
	BGBMID_SetNoteTicks(ctx, noteticks);
	BGBMID_SetRateScale(ctx, ratesc);
	BGBMID_SetNoteLen(ctx, 1.0);

	BGBMID_SetOutputBuffer(ctx, NULL, 1<<24);

	i=BGBMID_SynthStream(ctx, buf, sz);
	printf("Stream End %d\n", i);

//	BGBMID_DumpMidi0(ofn, buf, sz, noteticks);

	obuf=BGBMID_GetOutputBuffer(ctx);
	i=BGBMID_GetOutputBufferPos(ctx);
	j=BGBMID_GetOutputSampleRate(ctx);
	BGBMID_StoreWAV(ofn, (byte *)obuf, 2, j, 16, i);

	return(0);
}
