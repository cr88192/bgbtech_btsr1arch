int tk_ps2kbhit(void)
	{ return(P_PS2KB_STAT&1); }

int tk_ps2scanch(void)
{
	while(!(P_PS2KB_STAT&1))
		sleep_0();
	return(P_PS2KB_RX);
}

int tk_ps2getch(void)
{
	static byte st_e0;
	static byte st_f0;
	int c;
	
	c=tk_ps2scanch();
	if(c==0xE0)
	{
		st_e0=1;
		return(tk_ps2getch());
	}

	if(c==0xF0)
	{
		st_f0=1;
		return(tk_ps2getch());
	}
}
