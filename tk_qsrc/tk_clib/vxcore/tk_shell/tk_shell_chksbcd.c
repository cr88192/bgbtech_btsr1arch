int tk_shell_chksane_bitint()
{
	_UBitInt(7) b7a0, b7a1, b7a2;
	_UBitInt(24) b24a0;
	_UBitInt(10) b10a0;
	_UBitInt(15) b15rgb;
	
	b7a0=91;
	b7a1=73;
	b7a2=b7a0+b7a1;
	if(b7a2!=36)
		__debugbreak();
		
	b7a0=0xFFi7;
	b7a1=0xFZi7;
	
	b24a0 = (_UBitInt(24)) { 0x7ui3, b7a2, b7a1, b7a0 };
	
	(_UBitInt(24)) { 0x7ui3, b7a2, b7a1, b7a0 } = b24a0;
	
	__switchz(b24a0)
	{
		case 0x00ZZZZu24:	b7a2=b24a0[ 6: 0]; break;
		case 0x1ZZZZZu24:	b7a2=b24a0[13: 7]; break;
		case 0x2ZZZZZu24:	b7a2=b24a0[20:14]; break;
	}

	b10a0=(_UBitInt(10)) { b15rgb[14:12], b15rgb[ 9: 6], b15rgb[ 4: 2] };
}

int tk_shell_chksane_bcd()
{
	__bcd64 vi, vj, vk;
	u64		li, lj, lk;
	
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

	lj=0;
	li=0x12345678ULL;

	lj[ 7: 0]=li[31:24];
	lj[15: 8]=li[23:16];
	lj[23:16]=li[15: 8];
	lj[31:24]=li[ 7: 0];
	
	printf("BitSlice0: %llX %llX\n", li, lj);
	
	tk_shell_chksane_bitint();
}
