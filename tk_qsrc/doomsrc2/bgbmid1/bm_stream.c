#include "bgbmid.h"

BMID_API int BGBMID_UpdateStreams(BGBMID_Context *ctx, int samp)
{
	BGBMID_Patch *cur;
	int i, j, k, l, l2, n;
	
	cur=ctx->streams;
	while(cur)
	{
//		l=samp;
		l=samp*(cur->samp->rate/((float)ctx->samplerate));

		l2=cur->samp->pcm_strpos-cur->samp->pcm;
		if(l2<l) { l=l2; }
		if(l2<0)
		{
			cur=cur->str_next;
			continue;
		}

		if(l2<(0.025*cur->samp->rate))
		{
			k=0.025*cur->samp->rate;
			for(i=l2; i>0; i--)
				{ cur->samp->pcm[k+i]=cur->samp->pcm[i]; }
			for(i=0; i<k; i++)
				{ cur->samp->pcm[i]=0; }

			cur->samp->pcm_strpos+=k;

			cur=cur->str_next;
			continue;
		}

		if(l2>=(0.25*cur->samp->rate))
		{
//			k=0.2*cur->samp->rate;
			k=l2-(0.05*cur->samp->rate);
			for(i=0; i<l2; i++)
				{ cur->samp->pcm[i]=cur->samp->pcm[k+i]; }
//			for(; i<l2; i++)
//				{ cur->samp->pcm[i]=0; }
			cur->samp->pcm_strpos=cur->samp->pcm+(l2-k);

			cur=cur->str_next;
			continue;
		}

		n=l2-l;
		for(i=0; i<l2; i++)
			{ cur->samp->pcm[i]=cur->samp->pcm[l+i]; }
//		for(; i<l2; i++)
//			{ cur->samp->pcm[i]=0; }
		cur->samp->pcm_strpos=cur->samp->pcm+n;
	
		cur=cur->str_next;
	}
	return(0);
}
