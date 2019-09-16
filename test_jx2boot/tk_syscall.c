/*
System Call interface for ROM.

This isn't a true interrupt, and as such may stomp the scratch registers.

sObj will be the "self object" as relevant for the call.
  The meaning of this object will depend on the message.

uMsg: Will identify the request.
  (15:12): Subsystem ID
  (11: 0): Request ID (Magic Number)

vParm1/vParm2: Will be message-specific parameters (such as user-provided structures).

*/

__declspec(syscall)
int __isr_syscall(void *sObj, int uMsg, void *vParm1, void *vParm2)
{
	int ret;

//	u64 ttb, tea;
//	u16 exsr;
	
//	ttb=__arch_ttb;
//	tea=__arch_tea;
//	exsr=(u16)(__arch_exsr);
//	ptetlb_rethow_exc=0;

	ret=TK_URES_ERR_BADMSG;
	
	switch((uMsg>>12)&15)
	{
	case 1:
		switch((uMsg>>8)&15)
		{
		case 0:
			switch(uMsg&255)
			{
			case 0x01:
				*((void **)vParm1)=tk_con_getctx();
				break;
			}
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
	
	return(ret);
}
