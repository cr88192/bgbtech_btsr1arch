static byte frgl_keymap[32];
static byte frgl_lkeymap[32];
static unsigned short frgl_keybuf2[64];
static int frgl_keybuf2_pos;
byte gfxdrv_lastkeys[2048];

static int keyb_num_shift[10]={
')', '!', '@', '#', '$', '%', '^', '&', '*', '('
};

u16 *FRGL_GetKeybuf()
{
	static u16 frgl_keybuf3[64];
	u16 *buf;

	buf=frgl_keybuf3;
	memcpy(buf, frgl_keybuf2, 128);

	frgl_keybuf2_pos=0;
	frgl_keybuf2[0]=0;

//	printf("buf %p\n", buf);

	return(buf);
}

void FRGL_EndInputFrame()
{
	memcpy(frgl_lkeymap, frgl_keymap, 32);
}

int FRGL_KeyDown(int key)
{
	if(frgl_keymap[key>>3]&(1<<(key&7)))return(1);
	return(0);
}

int GfxDev_Key_Event(int key, int down)
{
	static int skm=0, skmd=0;
	int akey, akey2, nskm;
//	keyhandler *cur;

	akey=key;
	if(FRGL_KeyDown(K_SHIFT))
	{
		if(key>='a' && key<='z')
			akey=key-'a'+'A';
		if(key>='0' && key<='9')
			akey=keyb_num_shift[key-'0'];

		if(key=='\'')akey='"';
		if(key=='[')akey='{';
		if(key==']')akey='}';
		if(key=='-')akey='_';
		if(key=='=')akey='+';
		if(key==';')akey=':';
		if(key==',')akey='<';
		if(key=='.')akey='>';
		if(key=='/')akey='?';
		if(key=='`')akey='~';
		if(key=='\\')akey='|';
	}

	nskm=0;
	if(FRGL_KeyDown(K_CTRL))
	{
		switch(akey)
		{
		case K_F1: nskm=K_SKM_F1; break;
		case K_F2: nskm=K_SKM_F2; break;
		case K_F3: nskm=K_SKM_F3; break;
		case K_F4: nskm=K_SKM_F4; break;
		case K_F5: nskm=K_SKM_F5; break;
		case K_F6: nskm=K_SKM_F6; break;
		case K_F7: nskm=K_SKM_F7; break;
		case K_F8: nskm=K_SKM_F8; break;
		case K_F9: nskm=K_SKM_F9; break;
		case K_F10: nskm=K_SKM_F10; break;
		case K_F11: nskm=K_SKM_F11; break;
		case K_F12: nskm=K_SKM_F12; break;
		default: break;
		}
	}else if(!skm)
	{
		if(!FRGL_KeyDown(K_CTRL) && !FRGL_KeyDown(K_ALT) &&
			!FRGL_KeyDown(K_SHIFT))
		{
			if(akey==K_ESC)
				nskm=K_SKM_ESC;
		}
	}

	if(down)
	{
		akey2=akey;
		if(skm && (akey>=' ') && (akey<='~'))
		{
			akey2=akey|skm;
			skmd=skm;
//			skm=0;
		}
	
		frgl_keymap[akey>>3]|=1<<(akey&7);
		if(frgl_keybuf2_pos<63)
		{
			frgl_keybuf2[frgl_keybuf2_pos++]=akey2;
			frgl_keybuf2[frgl_keybuf2_pos]=0;
		}

//		cur=frgl_keyhandlers;
//		while(cur)
//		{
//			cur->func(akey2, 1);
//			cur=cur->next;
//		}
	}else
	{
		akey2=akey;
		if(skm && skmd && (akey>=' ') && (akey<='~'))
		{
			akey2=akey|skm;
			skm=0; skmd=0;
		}
	
		frgl_keymap[akey>>3]&=~(1<<(akey&7));
		if(frgl_keybuf2_pos<63)
		{
			frgl_keybuf2[frgl_keybuf2_pos++]=akey2|0x8000;
			frgl_keybuf2[frgl_keybuf2_pos]=0;
		}

//		cur=frgl_keyhandlers;
//		while(cur)
//		{
//			cur->func(akey2, 0);
//			cur=cur->next;
//		}
	}
	
	if(nskm)
		{ skm=nskm; }
	return(0);
}


int FRGL_TimeMS()
{
#ifdef _WIN32
	static unsigned int init;
	unsigned int t;

	t=timeGetTime();
	if(!init)init=t;

	return((unsigned int)(t-init));
#else

#ifdef __EMSCRIPTEN__
	struct timeval	tp;
	static int      secbase; 

	gettimeofday(&tp, NULL);  
	if(!secbase)secbase=tp.tv_sec;
	return(((tp.tv_sec-secbase)*1000)+tp.tv_usec/1000);
#endif

#ifndef linux
	static int init;
	int t;

	t=clock();
	t*=CLOCKS_PER_SEC/1000.0;
//	t=FRGL_TimeMS();

	if(!init)init=t;

	return((unsigned int)(t-init));
#endif
#ifdef linux
	struct timeval	tp;
	static int      secbase; 

	gettimeofday(&tp, NULL);  
	if(!secbase)secbase=tp.tv_sec;
	return(((tp.tv_sec-secbase)*1000)+tp.tv_usec/1000);
#endif
#endif
}
