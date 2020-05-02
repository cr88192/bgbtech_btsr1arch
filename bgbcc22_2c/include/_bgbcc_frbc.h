#define FR2C_OP_BINOP_I			0x00
#define FR2C_OP_BINOP_L			0x01
#define FR2C_OP_BINOP_F			0x02
#define FR2C_OP_BINOP_D			0x03
#define FR2C_OP_BINOP_IC		0x04
#define FR2C_OP_BINOP_LC		0x05
#define FR2C_OP_BINOP_FC		0x06
#define FR2C_OP_BINOP_DC		0x07
#define FR2C_OP_UNOP_I			0x08
#define FR2C_OP_UNOP_L			0x09
#define FR2C_OP_UNOP_F			0x0A
#define FR2C_OP_UNOP_D			0x0B
#define FR2C_OP_UNOP_IC			0x0C
#define FR2C_OP_UNOP_LC			0x0D
#define FR2C_OP_UNOP_FC			0x0E
#define FR2C_OP_UNOP_DC			0x0F
#define FR2C_OP_CMP_I			0x10
#define FR2C_OP_CMP_L			0x11
#define FR2C_OP_CMP_F			0x12
#define FR2C_OP_CMP_D			0x13
#define FR2C_OP_CMP_IC			0x14
#define FR2C_OP_CMP_LC			0x15 	
#define FR2C_OP_CMP_FC			0x16
#define FR2C_OP_CMP_DC			0x17
#define FR2C_OP_JCMP_I			0x18
#define FR2C_OP_JCMP_L			0x19
#define FR2C_OP_JCMP_F			0x1A
#define FR2C_OP_JCMP_D			0x1B
#define FR2C_OP_JCMP_IC			0x1C
#define FR2C_OP_JCMP_LC			0x1D
#define FR2C_OP_JCMP_FC			0x1E
#define FR2C_OP_JCMP_DC			0x1F
#define FR2C_OP_JMP				0x20
#define FR2C_OP_JMPW			0x21
#define FR2C_OP_LDLVAR			0x22
#define FR2C_OP_STLVAR			0x23
#define FR2C_OP_LDGVAR			0x24
#define FR2C_OP_STGVAR			0x25
#define FR2C_OP_LDCONST			0x26
#define FR2C_OP_LDCONSTW		0x27
#define FR2C_OP_CONV			0x28
#define FR2C_OP_LDAGVAR			0x29
#define FR2C_OP_CONV_I			0x2A
#define FR2C_OP_CONV_L			0x2B
#define FR2C_OP_SHRMASK_I		0x2C
#define FR2C_OP_SHRMASK_L		0x2D
#define FR2C_OP_SHLMASK_I		0x2E
#define FR2C_OP_SHLMASK_L		0x2F
#define FR2C_OP_JCMP_RZI		0x30	//Jump-Compare Register Zero Int
#define FR2C_OP_JCMP_RZL		0x31	//Jump-Compare Register Zero Long
#define FR2C_OP_JCMP_RZF		0x32	//Jump-Compare Register Zero Float
#define FR2C_OP_JCMP_RZD		0x33	//Jump-Compare Register Zero Double
#define FR2C_OP_JCMP_AZI		0x34	//Jump-Compare Local Zero Int
#define FR2C_OP_JCMP_AZL		0x35	//Jump-Compare Local Zero Long
#define FR2C_OP_JCMP_AZF		0x36	//Jump-Compare Local Zero Float
#define FR2C_OP_JCMP_AZD		0x37	//Jump-Compare Local Zero Double
#define FR2C_OP_JCMP_LZI		0x38	//Jump-Compare Arg Zero Int
#define FR2C_OP_JCMP_LZL		0x39	//Jump-Compare Arg Zero Long
#define FR2C_OP_JCMP_LZF		0x3A	//Jump-Compare Arg Zero Float
#define FR2C_OP_JCMP_LZD		0x3B	//Jump-Compare Arg Zero Double
#define FR2C_OP_JCMP_RZP		0x3C	//Jump-Compare Register Zero Pointer
#define FR2C_OP_JCMP_AZP		0x3D	//Jump-Compare Local Zero Pointer
#define FR2C_OP_JCMP_LZP		0x3E	//Jump-Compare Arg Zero Pointer
#define FR2C_OP_JCMP_P			0x3F	//Jump-Compare - Pointer
#define FR2C_OP_LOADINDEX_I		0x40
#define FR2C_OP_LOADINDEX_L		0x41
#define FR2C_OP_LOADINDEX_F		0x42
#define FR2C_OP_LOADINDEX_D		0x43
#define FR2C_OP_LOADINDEX_SB	0x44
#define FR2C_OP_LOADINDEX_UB	0x45
#define FR2C_OP_LOADINDEX_SS	0x46
#define FR2C_OP_LOADINDEX_US	0x47
#define FR2C_OP_LOADINDEX_P		0x48
#define FR2C_OP_LOADINDEX_IC	0x49
#define FR2C_OP_LOADINDEX_LC	0x4A
#define FR2C_OP_LOADINDEX_FC	0x4B
#define FR2C_OP_LOADINDEX_DC	0x4C
#define FR2C_OP_LOADINDEX_SBC	0x4D
#define FR2C_OP_LOADINDEX_SSC	0x4E
#define FR2C_OP_LOADINDEX_PC	0x4F
#define FR2C_OP_STOREINDEX_I	0x50
#define FR2C_OP_STOREINDEX_L	0x51
#define FR2C_OP_STOREINDEX_F	0x52
#define FR2C_OP_STOREINDEX_D	0x53
#define FR2C_OP_STOREINDEX_B	0x54
#define FR2C_OP_STOREINDEX_S	0x55
#define FR2C_OP_STOREINDEX_P	0x56
#define FR2C_OP_LOADINDEX_UBC	0x57
#define FR2C_OP_LOADINDEX_USC	0x58
#define FR2C_OP_STOREINDEX_IC	0x59
#define FR2C_OP_STOREINDEX_LC	0x5A
#define FR2C_OP_STOREINDEX_FC	0x5B
#define FR2C_OP_STOREINDEX_DC	0x5C
#define FR2C_OP_STOREINDEX_BC	0x5D
#define FR2C_OP_STOREINDEX_SC	0x5E
#define FR2C_OP_STOREINDEX_PC	0x5F
#define FR2C_OP_LEA_I			0x60
#define FR2C_OP_LEA_L			0x61
#define FR2C_OP_LEA_F			0x62
#define FR2C_OP_LEA_D			0x63
#define FR2C_OP_LEA_B			0x64
#define FR2C_OP_LEA_S			0x65
#define FR2C_OP_LEA_P			0x66
#define FR2C_OP_LEALVAR_V		0x67
#define FR2C_OP_LEA_IC			0x68
#define FR2C_OP_LEA_LC			0x69
#define FR2C_OP_LEA_FC			0x6A
#define FR2C_OP_LEA_DC			0x6B
#define FR2C_OP_LEA_BC			0x6C
#define FR2C_OP_LEA_SC			0x6D
#define FR2C_OP_LEA_PC			0x6E
#define FR2C_OP_LEALVAR_VC		0x6F
#define FR2C_OP_CALL			0x70
#define FR2C_OP_CALLP			0x71
#define FR2C_OP_RET_V			0x72
#define FR2C_OP_RET				0x73
#define FR2C_OP_RETW			0x74
#define FR2C_OP_LOADSLOT		0x75
#define FR2C_OP_STORESLOT		0x76
#define FR2C_OP_INITOBJ			0x77
#define FR2C_OP_CSRV_V			0x78
#define FR2C_OP_CSRV			0x79
#define FR2C_OP_CSRVW			0x7A
#define FR2C_OP_INITARR			0x7B
#define FR2C_OP_LABEL			0x7C
#define FR2C_OP_LABEL2			0x7D
#define FR2C_OP_NOP2			0x7E
#define FR2C_OP_NOP				0x7F

#define FR2C_OP_BINOP2			0x80
#define FR2C_OP_BINOP2_C		0x81
#define FR2C_OP_UNOP2			0x82
#define FR2C_OP_UNOP2_C			0x83
#define FR2C_OP_CMP2			0x84
#define FR2C_OP_CMP2_C			0x85
#define FR2C_OP_LDAVAR			0x86
#define FR2C_OP_COPYOBJ			0x87
#define FR2C_OP_DROPOBJ			0x88
#define FR2C_OP_SIZEOF_I		0x89
#define FR2C_OP_OFFSETOF_I		0x8A
#define FR2C_OP_RET_C			0x8B
#define FR2C_OP_INITOBJARR		0x8C
#define FR2C_OP_CALL2			0x8D
#define FR2C_OP_MOV				0x8E
#define FR2C_OP_LEAJ			0x8F
#define FR2C_OP_ADDMUL			0x90
#define FR2C_OP_ADDMUL_IC		0x91
#define FR2C_OP_BINOP3_C		0x92
#define FR2C_OP_UNOP3_C			0x93
#define FR2C_OP_CALLIH			0x94
#define FR2C_OP_CALLIH2			0x95
#define FR2C_OP_CALLOBJ			0x96
#define FR2C_OP_CALLVIRT		0x97
#define FR2C_OP_CONV_V			0x98
#define FR2C_OP_CHKCONV_V		0x99
#define FR2C_OP_JCMP2			0x9A
#define FR2C_OP_JCMP2_C			0x9B

#define FR2C_OP_LDINITARR		0x9C
#define FR2C_OP_LDINITOBJ		0x9D
#define FR2C_OP_LDINITOBJARR	0x9E


#define FR2C_OP_BINOPW_I		0x0100
#define FR2C_OP_BINOPW_L		0x0101
#define FR2C_OP_BINOPW_F		0x0102
#define FR2C_OP_BINOPW_D		0x0103
#define FR2C_OP_BINOPW_IC		0x0104
#define FR2C_OP_BINOPW_LC		0x0105
#define FR2C_OP_BINOPW_FC		0x0106
#define FR2C_OP_BINOPW_DC		0x0107
#define FR2C_OP_UNOPW_I			0x0108
#define FR2C_OP_UNOPW_L			0x0109
#define FR2C_OP_UNOPW_F			0x010A
#define FR2C_OP_UNOPW_D			0x010B
#define FR2C_OP_UNOPW_IC		0x010C
#define FR2C_OP_UNOPW_LC		0x010D
#define FR2C_OP_UNOPW_FC		0x010E
#define FR2C_OP_UNOPW_DC		0x010F

#define FR2C_OP_CMPW_I			0x0110
#define FR2C_OP_CMPW_L			0x0111
#define FR2C_OP_CMPW_F			0x0112
#define FR2C_OP_CMPW_D			0x0113
#define FR2C_OP_CMPW_IC			0x0114
#define FR2C_OP_CMPW_LC			0x0115
#define FR2C_OP_CMPW_FC			0x0116
#define FR2C_OP_CMPW_DC			0x0117
#define FR2C_OP_JCMPW_I			0x0118
#define FR2C_OP_JCMPW_L			0x0119
#define FR2C_OP_JCMPW_F			0x011A
#define FR2C_OP_JCMPW_D			0x011B
#define FR2C_OP_JCMPW_IC		0x011C
#define FR2C_OP_JCMPW_LC		0x011D
#define FR2C_OP_JCMPW_FC		0x011E
#define FR2C_OP_JCMPW_DC		0x011F

#define FR2C_OP_SIZEOFW_I		0x0120
#define FR2C_OP_OFFSETOFW_I		0x0121
#define FR2C_OP_LOADSLOTW		0x0122
#define FR2C_OP_STORESLOTW		0x0123
#define FR2C_OP_LOADSLOTAW		0x0124
#define FR2C_OP_INITOBJW		0x0125
#define FR2C_OP_COPYOBJW		0x0126
#define FR2C_OP_DROPOBJW		0x0127

#define FR2C_OP_LOADINDEXW		0x0128
#define FR2C_OP_STOREINDEXW		0x0129
#define FR2C_OP_LOADINDEXAW		0x012A
#define FR2C_OP_LEAW			0x012B
#define FR2C_OP_LOADINDEXW_C	0x012C
#define FR2C_OP_STOREINDEXW_C	0x012D
#define FR2C_OP_LOADINDEXAW_C	0x012E
#define FR2C_OP_LEAW_C			0x012F

#define FR2C_OP_LOADSLOTOW		0x0130
#define FR2C_OP_STORESLOTOW		0x0131
#define FR2C_OP_LOADINDEXOW		0x0132
#define FR2C_OP_STOREINDEXOW	0x0133
#define FR2C_OP_LEAOW			0x0134
#define FR2C_OP_LOADINDEXOW_C	0x0135
#define FR2C_OP_STOREINDEXOW_C	0x0136
#define FR2C_OP_LEAOW_C			0x0137
#define FR2C_OP_RETOBJ			0x0138
#define FR2C_OP_CSRVOBJ			0x0139
#define FR2C_OP_RETOBJW			0x013A
#define FR2C_OP_CSRVOBJW		0x013B
#define FR2C_OP_RETW_C			0x013C
#define FR2C_OP_CONVW			0x013D
#define FR2C_OP_LOADVSF			0x013E
#define FR2C_OP_STOREVSF		0x013F

#define FR2C_OP_LOADXVAR		0x0140
#define FR2C_OP_STOREXVAR		0x0141
#define FR2C_OP_CLOSEFCN		0x0142
#define FR2C_OP_DELCXFCN		0x0143

#define FR2C_OP_LEAJW			0x0144
#define FR2C_OP_ADDMULW			0x0145
#define FR2C_OP_DIFFPTR			0x0146
#define FR2C_OP_DIFFPTRL		0x0147
#define FR2C_OP_INITARRW		0x0148
#define FR2C_OP_DIFFPTROBJ		0x0149

#define FR2C_OP_LDGVARW			0x014A
#define FR2C_OP_STGVARW			0x014B
#define FR2C_OP_LDAGVARW		0x014C
#define FR2C_OP_CALLPW			0x014D

#define FR2C_OP_LLN				0x014E
#define FR2C_OP_LFN				0x014F

#define FR2C_OP_CMPW_P			0x0150
#define FR2C_OP_CMPW_PC			0x0151
#define FR2C_OP_JCMPW_P			0x0152
#define FR2C_OP_JCMPW_PC		0x0153
#define FR2C_OP_JCMP2W			0x0154
#define FR2C_OP_JCMP2W_C		0x0155

#define FR2C_OP_BINOP2W			0x0158
#define FR2C_OP_BINOP2W_C		0x0159
#define FR2C_OP_UNOP2W			0x015A
#define FR2C_OP_UNOP2W_C		0x015B
#define FR2C_OP_CMP2W			0x015C
#define FR2C_OP_CMP2W_C			0x015D
#define FR2C_OP_BINOP3W_C		0x015E
#define FR2C_OP_UNOP3W_C		0x015F


#define FR2C_BINOP_ADD			0x00
#define FR2C_BINOP_SUB			0x01
#define FR2C_BINOP_MUL			0x02
#define FR2C_BINOP_DIV			0x03
#define FR2C_BINOP_MOD			0x04
#define FR2C_BINOP_AND			0x05
#define FR2C_BINOP_OR			0x06
#define FR2C_BINOP_XOR			0x07
#define FR2C_BINOP_SHL			0x08
#define FR2C_BINOP_SHR			0x09
#define FR2C_BINOP_SHRR			0x0A
#define FR2C_BINOP_MULH			0x0B
#define FR2C_BINOP_UMULH		0x0C
#define FR2C_BINOP_UDIV			0x0D

#define FR2C_UNOP_MOV			0x00
#define FR2C_UNOP_NEG			0x01
#define FR2C_UNOP_NOT			0x02
#define FR2C_UNOP_LNOT			0x03
#define FR2C_UNOP_INC			0x04
#define FR2C_UNOP_DEC			0x05

#define FR2C_CMP_EQ				0x00
#define FR2C_CMP_NE				0x01
#define FR2C_CMP_LT				0x02
#define FR2C_CMP_GT				0x03
#define FR2C_CMP_LE				0x04
#define FR2C_CMP_GE				0x05
#define FR2C_CMP_AL				0x06
#define FR2C_CMP_NV				0x07

#define FR2C_OPMODE_RR			0x00
#define FR2C_OPMODE_RA			0x01
#define FR2C_OPMODE_RL			0x02
#define FR2C_OPMODE_AR			0x03
#define FR2C_OPMODE_AA			0x04
#define FR2C_OPMODE_AL			0x05
#define FR2C_OPMODE_LR			0x06
#define FR2C_OPMODE_LA			0x07
#define FR2C_OPMODE_LL			0x08
#define FR2C_OPMODE_RRR			0x00
#define FR2C_OPMODE_RRA			0x01
#define FR2C_OPMODE_RRL			0x02
#define FR2C_OPMODE_RAR			0x03
#define FR2C_OPMODE_RAA			0x04
#define FR2C_OPMODE_RAL			0x05
#define FR2C_OPMODE_RLR			0x06
#define FR2C_OPMODE_RLA			0x07
#define FR2C_OPMODE_RLL			0x08
#define FR2C_OPMODE_ARR			0x09
#define FR2C_OPMODE_LRR			0x0A
#define FR2C_OPMODE_LRA			0x0B
#define FR2C_OPMODE_LAR			0x0C
#define FR2C_OPMODE_LRL			0x0D
#define FR2C_OPMODE_LLR			0x0E
#define FR2C_OPMODE_LLL			0x0F

#define FR2C_OPMODE2_IR			0x00
#define FR2C_OPMODE2_IA			0x01
#define FR2C_OPMODE2_IL			0x02
#define FR2C_OPMODE2_LR			0x03
#define FR2C_OPMODE2_LA			0x04
#define FR2C_OPMODE2_LL			0x05
#define FR2C_OPMODE2_FR			0x06
#define FR2C_OPMODE2_FA			0x07
#define FR2C_OPMODE2_FL			0x08
#define FR2C_OPMODE2_DR			0x09
#define FR2C_OPMODE2_DA			0x0A
#define FR2C_OPMODE2_DL			0x0B
#define FR2C_OPMODE2_PR			0x0C
#define FR2C_OPMODE2_PA			0x0D
#define FR2C_OPMODE2_PL			0x0E

#define FR2C_OPCONV_I2L			0x00	//Int->Long
#define FR2C_OPCONV_I2F			0x01	//Int->Float
#define FR2C_OPCONV_I2D			0x02	//Int->Double
#define FR2C_OPCONV_I2P			0x03	//Int->Pointer
#define FR2C_OPCONV_L2I			0x04	//Long->Int
#define FR2C_OPCONV_L2F			0x05	//Long->Float
#define FR2C_OPCONV_L2D			0x06	//Long->Double
#define FR2C_OPCONV_L2P			0x07	//Long->Pointer
#define FR2C_OPCONV_F2I			0x08	//Float->Int
#define FR2C_OPCONV_F2L			0x09	//Float->Long
#define FR2C_OPCONV_F2D			0x0A	//Float->Double
#define FR2C_OPCONV_D2I			0x0B	//Double->Int
#define FR2C_OPCONV_D2L			0x0C	//Double->Long
#define FR2C_OPCONV_D2F			0x0D	//Double->Float
#define FR2C_OPCONV_P2I			0x0E	//Pointer->Int
#define FR2C_OPCONV_P2L			0x0F	//Pointer->Long

#define FR2C_OPCONV2_SXB		0x00	//Sign Extend Byte
#define FR2C_OPCONV2_ZXB		0x01	//Zero Extend Byte
#define FR2C_OPCONV2_SXS		0x02	//Sign Extend Short
#define FR2C_OPCONV2_ZXS		0x03	//Zero Extend Short
#define FR2C_OPCONV2_SXI		0x04	//Sign Extend Int
#define FR2C_OPCONV2_ZXI		0x05	//Zero Extend Int
#define FR2C_OPCONV2_SXB2		0x06	//Sign Extend Byte 2
#define FR2C_OPCONV2_ZXB2		0x07	//Zero Extend Byte 2
#define FR2C_OPCONV2_SXS2		0x08	//Sign Extend Short 2
#define FR2C_OPCONV2_ZXS2		0x09	//Zero Extend Short 2
#define FR2C_OPCONV2_SXB3		0x0A	//Sign Extend Byte 3
#define FR2C_OPCONV2_ZXB3		0x0B	//Zero Extend Byte 3
#define FR2C_OPCONV2_SXB4		0x0C	//Sign Extend Byte 4
#define FR2C_OPCONV2_ZXB4		0x0D	//Zero Extend Byte 4
#define FR2C_OPCONV2_SXI2		0x0E	//Sign Extend Int 2
#define FR2C_OPCONV2_ZXI2		0x0F	//Zero Extend Int 2

#define FR2C_JMPMODE_A16		0x01
#define FR2C_JMPMODE_A32		0x02

#define FR2C_JMPMODE_LI12		0x08
#define FR2C_JMPMODE_LJ12		0x09
#define FR2C_JMPMODE_LF12		0x0A
#define FR2C_JMPMODE_LD12		0x0B
#define FR2C_JMPMODE_LL12		0x0C
#define FR2C_JMPMODE_LG12		0x0D

#define FR2C_JMPMODE_LI16		0x10
#define FR2C_JMPMODE_LJ16		0x11
#define FR2C_JMPMODE_LF16		0x12
#define FR2C_JMPMODE_LD16		0x13
#define FR2C_JMPMODE_LL16		0x14
#define FR2C_JMPMODE_LG16		0x15

#define FR2C_JMPMODE_LI24		0x18
#define FR2C_JMPMODE_LJ24		0x19
#define FR2C_JMPMODE_LF24		0x1A
#define FR2C_JMPMODE_LD24		0x1B
#define FR2C_JMPMODE_LL24		0x1C
#define FR2C_JMPMODE_LG24		0x1D

//0-15=Core Primitive Types
//Types 16-255=Extended Core Types
//Types 256-4095=User Types (structs/unions/...)
#define FR2C_TY_I				0x00		//Int
#define FR2C_TY_L				0x01		//Long (64-bit)
#define FR2C_TY_F				0x02		//Float
#define FR2C_TY_D				0x03		//Double
#define FR2C_TY_P				0x04		//Pointer (abstract)
#define FR2C_TY_V				0x05		//Void
#define FR2C_TY_S				0x06		//Struct / Union
#define FR2C_TY_NL				0x07		//Native Long (32/64 bit)
#define FR2C_TY_SB				0x08		//Signed Byte
#define FR2C_TY_UB				0x09		//Unsigned Byte
#define FR2C_TY_SS				0x0A		//Signed Short
#define FR2C_TY_US				0x0B		//Unsigned Short
#define FR2C_TY_UI				0x0C		//Unsigned Int
#define FR2C_TY_UL				0x0D		//Unsigned Long (64-bit)
#define FR2C_TY_UNL				0x0E		//Native Long (32/64 bit)
#define FR2C_TY_UNDEF_I			0x0F		//Undefined (Implicit Int)
//Types 16-255=Extended Core Types
#define FR2C_TY_I128			0x10		//Int128
#define FR2C_TY_UI128			0x11		//Unsigned Int128
#define FR2C_TY_F128			0x12		//Float128
#define FR2C_TY_F16				0x13		//Float16
#define FR2C_TY_VARIANT			0x14		//Variant
#define FR2C_TY_VARARGS			0x15		//VarArgs

//Basic Type
#define FR2C_TY_BASEMASK		0x00000FFF	//base type or struct
#define FR2C_TY_PTRMASK			0x0000F000	//pointer level
#define FR2C_TY_PTRIDX1			0x00001000	//pointer level
#define FR2C_TY_PTRSHL			12			//pointer level shl
#define FR2C_TY_ARRMASK			0x0FFF0000	//small array size
#define FR2C_TY_ARRSHL			16			//array size shl

#define FR2C_TY_BASETYMAX		4096		//base type limit
#define FR2C_TY_BASEARRMAX		4096		//base array limit

#define FR2C_TY_TYTY_MASK		0xF0000000	//type-type mask
#define FR2C_TY_TYTY_BASIC		0x00000000	//type-type basic
#define FR2C_TY_TYTY_BASIC2		0x10000000	//type-type basic2
#define FR2C_TY_TYTY_OVF1		0x20000000	//type-type overflow

//Basic2 Type
#define FR2C_TYB2_BASEMASK		0x0000003F	//base type or struct
#define FR2C_TYB2_PTRMASK		0x000000C0	//pointer level
#define FR2C_TYB2_PTRIDX1		0x00000040	//pointer level
#define FR2C_TYB2_PTRSHL		6			//pointer level shl
#define FR2C_TYB2_ARRMASK		0x0FFFFF00	//small array size
#define FR2C_TYB2_ARRSHL		8			//array size shl

#define FR2C_TYB2_BASETYMAX		64			//base type limit
#define FR2C_TYB2_BASEARRMAX	1048576		//base array limit

#define FR2C_TYOVF_IDXMASK		0x0000FFFF	//type overflow index



#define FR2C_REGTY_REGMASK		0x0F00000000000000ULL
#define FR2C_REGTY_REG			0x0000000000000000ULL
#define FR2C_REGTY_ARG			0x0100000000000000ULL
#define FR2C_REGTY_LOCAL		0x0200000000000000ULL
#define FR2C_REGTY_GLOBAL		0x0300000000000000ULL

#define FR2C_REGTY_IMM_INT		0x0400000000000000ULL
#define FR2C_REGTY_IMM_LONG		0x0500000000000000ULL
#define FR2C_REGTY_IMM_FLOAT	0x0600000000000000ULL
#define FR2C_REGTY_IMM_DOUBLE	0x0700000000000000ULL
#define FR2C_REGTY_IMM_STRING	0x0800000000000000ULL
#define FR2C_REGTY_IMM_WSTRING	0x0900000000000000ULL
#define FR2C_REGTY_IMM_LITERAL	0x0A00000000000000ULL
#define FR2C_REGTY_IMM_GBLADDR	0x0B00000000000000ULL

//#define FR2C_REGID_REGMASK		0x000000000000FFFFULL
//#define FR2C_REGID_TYPEMASK		0x0000FFFFFFFF0000ULL
//#define FR2C_REGID_TYPESHIFT	16

#define FR2C_REGID_REGMASK		0x0000000000FFFFFFULL
#define FR2C_REGID_TYPEMASK		0x00FFFFFFFF000000ULL
#define FR2C_REGID_TYPESHIFT	24

#define FR2C_REGINT_MASK		0x00000000FFFFFFFFULL
#define FR2C_REGLONG_MASK		0x00FFFFFFFFFFFFFFULL

#define FR2C_LITID_STRUCT		1
#define FR2C_LITID_UNION		2
#define FR2C_LITID_TYPEDEF		3
#define FR2C_LITID_FUNCTION		4
#define FR2C_LITID_GLOBALVAR	5
#define FR2C_LITID_ARGS			6
#define FR2C_LITID_LOCALS		7
#define FR2C_LITID_VAR			8
#define FR2C_LITID_LIST			9
#define FR2C_LITID_VALUE		10
#define FR2C_LITID_CLASS		11
#define FR2C_LITID_STATICVAR	12

#define FR2C_LITID_ARR1			16
#define FR2C_LITID_ARR2			17
#define FR2C_LITID_ARR4			18
#define FR2C_LITID_ARR8			19
#define FR2C_LITID_ARR16		20
#define FR2C_LITID_ASTD			21

#define FR2C_STATUS_YES					1
#define FR2C_STATUS_NO					0
#define FR2C_STATUS_ERR_GENERIC			-1
#define FR2C_STATUS_ERR_LOOKUPFAIL		-2
#define FR2C_STATUS_ERR_BADOPCODE		-3
#define FR2C_STATUS_ERR_BADVALUE		-4
#define FR2C_STATUS_ERR_UNHANDLEDTYPE	-5
#define FR2C_STATUS_ERR_BADOPARGS		-6
#define FR2C_STATUS_ERR_CANTRELEASE		-7
#define FR2C_STATUS_ERR_CANTACQUIRE		-8

#define FR2C_CMD_VARDECL				0x8000
#define FR2C_CMD_VARVALUE				0x8001
#define FR2C_CMD_FUNCTION				0x8002
#define FR2C_CMD_METHOD					0x8003
#define FR2C_CMD_BODY					0x8004
#define FR2C_CMD_ARGS					0x8005
#define FR2C_CMD_LOCALS					0x8006
#define FR2C_CMD_VARARGS				0x8007
#define FR2C_CMD_PROTOTYPE				0x8008
#define FR2C_CMD_STRUCT					0x8009
#define FR2C_CMD_UNION					0x800A
#define FR2C_CMD_CLASS					0x800B
#define FR2C_CMD_EXTENDS				0x800C
#define FR2C_CMD_IMPLEMENTS				0x800D
#define FR2C_CMD_S_PROTOTYPE			0x800E
#define FR2C_CMD_LIST					0x800F
#define FR2C_CMD_STATICVARDECL			0x8010

#define FR2C_ATTR_NAME					0x9000
#define FR2C_ATTR_SIG					0x9001
#define FR2C_ATTR_FLAGS					0x9002
#define FR2C_ATTR_VALUE					0x9003

#define FR2C_TERR_GENERIC				0xA000
#define FR2C_TERR_STACK_OVERFLOW		0xA001
#define FR2C_TERR_STACK_UNDERFLOW		0xA002
#define FR2C_TERR_FORMAT_SANITY			0xA003

#define FR2C_TERR_STATUS(st)			(0xA800+(st))


typedef struct { u64 val; } frbc_register;
typedef struct { u32 val; } frbc_type;

typedef int frbc_status;