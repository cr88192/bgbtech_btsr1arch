#include "rt_def.h"
#include "w_wad.h"
#include "z_zone.h"

#if 1
byte *i_mus_songs[32];
int i_mus_song_mask;

int i_mus_song_len[32];
int i_mus_song_ofs[32];

short *i_mus_pcm_buf[32];
int i_mus_pcm_len[32];

// BGBMID_Context *bmid_ctx;
byte *genmidi;

int i_mus_curhandle;
int i_mus_curpos;

int i_mus_pause;


void I_MusicGetAdvanceSamples(short *buf, int sn, int snmax)	
{
	short *sb;
	int bp, bl, sl;
	int i, j, k;

	sb=i_mus_pcm_buf[i_mus_curhandle];
	sl=i_mus_pcm_len[i_mus_curhandle];

	if(!sb || !sl || i_mus_pause)
	{
		for(i=0; i<snmax; i++)
		{
			buf[i*2+0]=0;
			buf[i*2+1]=0;
		}
	
		return;
	}
	
	bp=i_mus_curpos; bl=snmax; i=0;
	while(bp>=sl)bp-=sl;

	while(bl>0)
	{
		buf[i*2+0]=sb[bp*2+0];
		buf[i*2+1]=sb[bp*2+0];
		bp++; i++; bl--;
		if(bp>=sl)
			bp-=sl;
	}
	
	bp=i_mus_curpos+sn;
	while(bp>=sl)bp-=sl;
	i_mus_curpos=bp;
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
	
//	if(bmid_ctx)
//		return;

//	BGBMID_LoadPackGlobal("instruments.pak");

//	bmid_ctx=BGBMID_NewContext();

	genmidi = W_CacheLumpName("GENMIDI", PU_STATIC);

#if 0
	if(genmidi)
	{
		for(i=0; i<175; i++)
		{
			rec=genmidi+8+(i*36);
			BGBMID_SetFmRegisterData(bmid_ctx, i, 0, ((u32 *)rec)[0]);
			BGBMID_SetFmRegisterData(bmid_ctx, i, 1, ((u32 *)rec)[1]);
			BGBMID_SetFmRegisterData(bmid_ctx, i, 2, ((u32 *)rec)[2]);
			BGBMID_SetFmRegisterData(bmid_ctx, i, 3, ((u32 *)rec)[3]);
			BGBMID_SetFmRegisterData(bmid_ctx, i, 4, ((u32 *)rec)[4]);
			BGBMID_SetFmRegisterData(bmid_ctx, i, 5, ((u32 *)rec)[5]);
			BGBMID_SetFmRegisterData(bmid_ctx, i, 6, ((u32 *)rec)[6]);
			BGBMID_SetFmRegisterData(bmid_ctx, i, 7, ((u32 *)rec)[7]);
			BGBMID_SetFmRegisterData(bmid_ctx, i, 8, ((u32 *)rec)[8]);
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
	
	ofs=i_mus_song_ofs[handle];
	sz=i_mus_song_len[handle];
	buf=i_mus_songs[handle]+ofs;
	
	printf("I_PlaySong: ofs=%d sz=%d\n", ofs, sz);
	
  // UNUSED.
//  handle = looping = 0;
//  musicdies = gametic + TICRATE*30;

#if 0
	if(!bmid_ctx)
		return;

	if(!i_mus_pcm_buf[handle])
	{
		BGBMID_SetOutputSampleRate(bmid_ctx, 16000);
	
		BGBMID_SetNoteTicks(bmid_ctx, 140);
		BGBMID_SetRateScale(bmid_ctx, 1.0);
		BGBMID_SetNoteLen(bmid_ctx, 1.0);

		BGBMID_SetOutputBuffer(bmid_ctx, NULL, 1<<24);

		bmid_ctx->not_dt=16;

		i=BGBMID_SynthStream(bmid_ctx, buf, sz);
		printf("Stream End %d\n", i);
		
		sbuf=BGBMID_GetOutputBuffer(bmid_ctx);
		len=BGBMID_GetOutputBufferPos(bmid_ctx);

		i_mus_pcm_buf[handle]=sbuf;
		i_mus_pcm_len[handle]=len;
		printf("I_PlaySong: buf=%p len=%d\n", sbuf, len);
	}
#endif
	
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

int I_RegisterSong(void* data)
{
	int i;
	
	if(!data)
		return(0);
	
	for(i=1; i<32; i++)
		if(!(i_mus_song_mask&(1<<i)))
			break;
	
	i_mus_songs[i]=data;
	i_mus_song_mask|=(1<<i);
	
	i_mus_song_len[i]=((u16 *)data)[2];
	i_mus_song_ofs[i]=((u16 *)data)[3];
	
	return(i);

//  data = NULL;
//  return 1;
}

// Is the song playing?
int I_QrySongPlaying(int handle)
{
	// UNUSED.
	handle = 0;
	//  return looping || musicdies > gametic;
	return(0);
}
#endif
