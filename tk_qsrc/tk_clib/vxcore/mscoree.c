#include <vxcore.h>
#include <stdio.h>

VXCORE_API int _CorExeMain()
{
	vx_printstr("_CorExeMain\n");
	vx_exit();
}

VXCORE_API int _CorDllMain()
{
	vx_printstr("_CorDllMain\n");
	vx_exit();
}
