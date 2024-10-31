int tk_shell_chksane_bcd()
{
	__bcd64 vi, vj, vk;
	vi=12345678;
	vj=vi+vi;
	vk=24691356;

//	__debugbreak();

	printf("BCD 0: %08X %08X\n", vj, vk);

	if(vj!=vk)
		__debugbreak();

	vj=vi*vi;
	vk=152415765279684L;

	if(vj!=vk)
		__debugbreak();

//	__debugbreak();

	printf("BCD 1: %08X %08X\n", vj, vk);
	printf("BCD 1B: %016llX %016llX\n", vj, vk);

	if(vj!=vk)
		__debugbreak();

	printf("UTF1: %s %s %s\n",
		u8" G\u00F8\u00E0t\u00DF\u00EB",
		u8" \u0444\u0435\u0440\u0442\u044A",
		u8" \u0395\u03BB\u03BB\u03B7\u03BD\u03B9\u03BA\u03AC");

	printf("UTF1B: %s\n",
		u8" G\u00F8\u00E0t\u00DF\u00EB"
		u8" \u0444\u0435\u0440\u0442\u044A"
		u8" \u0395\u03BB\u03BB\u03B7\u03BD\u03B9\u03BA\u03AC");

//	while(1);
//	__debugbreak();
}
