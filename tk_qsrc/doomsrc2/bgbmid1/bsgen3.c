#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

#include <bgbmid.h>

FILE *bsg_ofd;

int bsg_snd_init()
{
	bsg_ofd=fopen("out.pcm", "wb");
}

int bsg_snd_deinit()
{
	fclose(bsg_ofd);
}

int bsg_out_samples(BSG_State *ctx, short *buf, int cnt)
{
	int i;
	if(bsg_ofd)fwrite(buf, 1, cnt*2, bsg_ofd);
	return(0);
}

int main(int argc, char *argv[])
{
	char tb[256];
	BSG_State *ctx;
	FILE *ofd, *ifd;
	char **a;
	int i, j, k;
	char *s;

	if(argc<2)
	{
		return(-1);
	}

	BSG_LoadVoice("common", "voice");
	BSG_LoadVoice("male0", "voice");
	BSG_LoadVoice("male1", "voice");
	BSG_LoadVoice("comp0", "voice");
	BSG_LoadVoice("female0", "voice");

	bsg_snd_init();

	ctx=BSG_NewContext("female0");
	ctx->out_cb=bsg_out_samples;

	for(i=1; i<argc; i++)
	{
		ifd=fopen(argv[i], "rt");
		if(ifd)
		{
			while(!feof(ifd))
			{
				memset(tb, 0, 256);
				fgets(tb, 255, ifd);

				BSG_SendText(ctx, tb);
			}
			fclose(ifd);
		}else
		{
			s=argv[i];
			BSG_PushWordEnd(ctx, s);
		}
	}

	BSG_PushWordEnd(ctx, "_");

	BSG_ProcessItems(ctx);

	printf("\n");

	bsg_snd_deinit();

	return(0);
}
