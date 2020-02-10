int TKSH_InitCmds(void);

// char tb_cwd[256];

int tk_shell_chksane()
{
	char tb[64];

	int *pi, *pj;
	int i, j, k, l;
	
	pi=&i;
	pj=&j;
	
	*pi=123456;
	*pj=10;
	k=i/j;
	l=i%j;
	
	if(k!=12345)
		__debugbreak();
	if(l!=6)
		__debugbreak();
		
	strcpy(tb, "0123456789ABCDEF");
	*pi=1;
	memcpy(tb, "GHIJKLMN", *pi);
	if(memcmp(tb, "G1234567", 8))
		__debugbreak();
	*pi=3;
	memcpy(tb, "GHIJKLMN", *pi);
	if(memcmp(tb, "GHI34567", 8))
		__debugbreak();
	*pi=5;
	memcpy(tb, "GHIJKLMN", *pi);
	if(memcmp(tb, "GHIJK567", 8))
		__debugbreak();
	*pi=7;
	memcpy(tb, "GHIJKLMN", *pi);
	if(memcmp(tb, "GHIJKLM7", 8))
		__debugbreak();

	strcpy(tb, "0123456789ABCDEF");
	*pi=1;
	memset(tb, ' ', *pi);
	if(memcmp(tb, " 1234567", 8))
		__debugbreak();
	*pi=3;
	memset(tb, ' ', *pi);
	if(memcmp(tb, "   34567", 8))
		__debugbreak();
	*pi=5;
	memset(tb, ' ', *pi);
	if(memcmp(tb, "     567", 8))
		__debugbreak();
	*pi=7;
	memset(tb, ' ', *pi);
	if(memcmp(tb, "       7", 8))
		__debugbreak();

}

int main(int argc, char *argv[])
{
	TKPE_TaskInfo *task;
	char tb_cwd[256];
	char tbuf[256];

	tk_shell_chksane();

	tk_con_reset();

	task=TK_AllocNewTask();
	TK_SetCurrentTask(task);
	
	TKSH_InitCmds();

	strcpy(tb_cwd, "/");
	TK_Env_SetCwd(tb_cwd);

	TKSH_TryLoad("autoexec.pf", NULL);

	while(1)
	{
		TK_Env_GetCwd(tb_cwd, 256);
//		tk_con_chkreset();
		tk_puts(tb_cwd);
		tk_puts("$ ");
		tk_gets(tbuf);
		TKSH_ExecCmd(tbuf);
	}

	puts("Loop Terminate\n");
}
