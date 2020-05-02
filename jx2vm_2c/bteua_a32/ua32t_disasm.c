int UA32T_ThumbDisasm_Op1(u16 opw)
{
	char *opn;
	int op, ofs, rs, rt, rd;

	switch((opw>>12)&15)
	{
	case 0x0: case 0x1:
		op=(opw>>11)&3;
		ofs=(opw>>6)&31;
		rs=(opw>>3)&7;
		rd=opw&7;
		switch(op)
		{
			case 0: opn="LSL"; break;
			case 1: opn="LSR"; break;
			case 2: opn="ASR"; break;
			case 3:
				ofs=ofs&7;
				opn=(opw&512)?"SUB":"ADD";
				break;
		}
		if((opw&1024) || (op!=3))
			printf("%s R%d, R%d, #%d", opn, rd, rs, ofs);
		else
			printf("%s R%d, R%d, R%d", opn, rd, rs, ofs);
		break;
	case 0x2: case 0x3:
		ofs=opw&255;
		rd=(opw>>8)&7;
		op=(opw>>11)&3;
		switch(op)
		{
			case 0: opn="MOV"; break;
			case 1: opn="CMP"; break;
			case 2: opn="ADD"; break;
			case 3: opn="SUB"; break;
		}
		printf("%s R%d, #%d", opn, rd, ofs);
		break;
	case 0x4:
		switch((opw>>8)&15)
		{
		case 0x0:	case 0x1:
		case 0x2:	case 0x3:
			break;
		case 0x4:	case 0x5:
		case 0x6:	case 0x7:
			rs=(opw>>3)&15;
			rd=(opw&7)|((opw>>4)&8);
			switch((opw>>8)&3)
			{
				case 0: opn="ADD"; break;
				case 1: opn="CMP"; break;
				case 2: opn="MOV"; break;
				case 3: opn=(opw&128)?"BLX":"BX"; break;
			}
			printf("%s R%d, R%d", opn, rd, rs);
			break;
		}
		break;
	default:
		printf("?");
		break;
	}
}

int UA32T_ThumbDisasm_Op2(u16 opw1, u16 opw2)
{
	char *opn;
	int op, ofs, rs, rt, rd;

	switch((opw1>>12)&15)
	{
	case 0xE:
		switch((opw1>>8)&15)
		{
		default:
			printf("?");
			break;
		}
		break;

	case 0xF:
		switch((opw1>>8)&15)
		{
		case 0x2: case 0x6:
			switch((opw1>>4)&15)
			{
			case 0x0:
			case 0x1:
				rs=opw1&15;
				rd=(opw2>>8)&15;
				ofs=(opw2&255)|((opw2>>4)&0x700)|((opw1<<1)&0x800);
				opn="ADDW";
				printf("%s R%d, R%d, #0x%X", opn, rd, rs, ofs);
				break;
			case 0x4:
				rd=(opw2>>8)&15;
				ofs=(opw2&255)|((opw2>>4)&0x700)|((opw1<<1)&0x800)|
					((opw1&15)<<12);
				opn="MOVW";
				printf("%s R%d, R%d, #0x%X", opn, rd, rs, ofs);
				break;
			default:
				printf("?");
				break;
			}
			break;
		default:
			printf("?");
			break;
		}
		break;
	default:
		printf("?");
		break;
	}
}

int UA32T_ThumbDisasm(byte *spos, byte *epos)
{
	byte *cs;
	u16 opw1, opw2;
	int i, j, k, step;

	printf("%p..%p\n", spos, epos);

//	spos=(byte *)(((nlint)spos)&(~15));
//	epos=(byte *)((((nlint)epos)+15)&(~15));
	
	cs=spos;
	while(cs<epos)
	{
		printf("%08X  ", (int)((nlint)cs));
		
		opw1=((u16 *)cs)[0];
		opw2=((u16 *)cs)[1];
		step=2;
		if((opw1&0xE000)==0xE000)
			step=4;
		
		if(step==4)
		{
			printf("%04X-%04X ", opw1, opw2);
			UA32T_ThumbDisasm_Op2(opw1, opw2);
		}
		else
		{
			printf("%04X      ", opw1);
			UA32T_ThumbDisasm_Op1(opw1);
		}
		
		cs+=step;
		printf("\n");
	}
	return(0);
}
