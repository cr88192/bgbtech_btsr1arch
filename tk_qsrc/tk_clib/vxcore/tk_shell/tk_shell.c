int TKSH_InitCmds(void);

#include "tk_shell_chksane.c"

int main(int argc, char *argv[])
{
	TKPE_TaskInfo *task;
	char *ars[3];
	char tb_cwd[256];
	char tbuf[256];

	tk_shell_chksane();

	tk_con_reset();
	
	tk_printf("TKSH 0\n");

	task=TK_AllocNewTask();
	TK_SetCurrentTask(task);

	tk_printf("TKSH 0, Init Syscall Task\n");
	TK_SpawnSyscallTask(task);
	
	TKSH_InitCmds();

	TKRA_RegisterHalGetContext();

	tk_printf("TKSH 1\n");

	strcpy(tb_cwd, "/");
	TK_Env_SetCwd(tb_cwd);

	tk_printf("TKSH 2\n");
//	TKSH_TryLoad("autoexec.pf", NULL);

//	tk_printf("TKSH 3\n");

	ars[0]="autoexec";
	ars[1]=NULL;
//	TKSH_TryLoad_ext("autoexec", NULL);
	TKSH_TryLoad_ext("boot/autoexec", NULL);
//	TKSH_TryLoad_ext("autoexec", ars);

//	tk_printf("TKSH 4\n");

	TK_SpawnShellTask(task, 0);

	while(1)
	{
		TK_YieldCurrentThread();
	}

#if 0
	while(1)
	{
		TK_Env_GetCwd(tb_cwd, 256);
//		tk_con_chkreset();
		tk_puts(tb_cwd);
		tk_puts("$ ");
//		tk_puts("$  \b");
		tk_gets(tbuf);
		TKSH_ExecCmd(tbuf);
		
		while(TKSH_IsCmdEntryMode())
		{
			tk_gets(tbuf);
			TKSH_ExecCmdEntry(tbuf);
		}
	}

	puts("Loop Terminate\n");
#endif
}
