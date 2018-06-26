void __start()
{
	TK_FILE *fd;
	int (*bootptr)();

	puts("Boot 0\n");
	printf("Print 0 %0X %0X %0X %0X\n",
		0x12345678, 0x9ABCDEF0, 0x1234CDEF, 0x9ABC5678);
	tk_vfile_init();
	
	fd=tk_fopen("bootload.sys", "rb");
	if(!fd)
	{
		puts("Fail Open BOOTLOAD.SYS\n");
		return;
	}
	
	TKPE_LoadStaticPE(fd, &bootptr);
	
	printf("Boot Pointer %p\n", bootptr);
	
	if(bootptr)
		bootptr();
}
