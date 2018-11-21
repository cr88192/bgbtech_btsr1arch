#ifdef _M_X64
#define UAX_WINX64
#endif

#ifdef __linux
#define UAX_SYSVAMD64
#define UAX_LINUX
#endif

#ifdef UAX_WINX64
#ifndef UAX_WINDOWS
#define UAX_WINDOWS
#endif
#endif


#define UAX_REG_EAX		0x00
#define UAX_REG_ECX		0x01
#define UAX_REG_EDX		0x02
#define UAX_REG_EBX		0x03
#define UAX_REG_ESP		0x04
#define UAX_REG_EBP		0x05
#define UAX_REG_ESI		0x06
#define UAX_REG_EDI		0x07

#define UAX_REG_RAX		0x10
#define UAX_REG_RCX		0x11
#define UAX_REG_RDX		0x12
#define UAX_REG_RBX		0x13
#define UAX_REG_RSP		0x14
#define UAX_REG_RBP		0x15
#define UAX_REG_RSI		0x16
#define UAX_REG_RDI		0x17

#define UAX_REG_AX		0x20
#define UAX_REG_CX		0x21
#define UAX_REG_DX		0x22
#define UAX_REG_BX		0x23
#define UAX_REG_SP		0x24
#define UAX_REG_BP		0x25
#define UAX_REG_SI		0x26
#define UAX_REG_DI		0x27

#define UAX_REG_AL		0x30
#define UAX_REG_CL		0x31
#define UAX_REG_DL		0x32
#define UAX_REG_BL		0x33
#define UAX_REG_SPL		0x34
#define UAX_REG_BPL		0x35
#define UAX_REG_SIL		0x36
#define UAX_REG_DIL		0x37

#define UAX_REG_R0D		0x00
#define UAX_REG_R1D		0x01
#define UAX_REG_R2D		0x02
#define UAX_REG_R3D		0x03
#define UAX_REG_R4D		0x04
#define UAX_REG_R5D		0x05
#define UAX_REG_R6D		0x06
#define UAX_REG_R7D		0x07
#define UAX_REG_R8D		0x08
#define UAX_REG_R9D		0x09
#define UAX_REG_R10D	0x0A
#define UAX_REG_R11D	0x0B
#define UAX_REG_R12D	0x0C
#define UAX_REG_R13D	0x0D
#define UAX_REG_R14D	0x0E
#define UAX_REG_R15D	0x0F

#define UAX_REG_R0Q		0x10
#define UAX_REG_R1Q		0x11
#define UAX_REG_R2Q		0x12
#define UAX_REG_R3Q		0x13
#define UAX_REG_R4Q		0x14
#define UAX_REG_R5Q		0x15
#define UAX_REG_R6Q		0x16
#define UAX_REG_R7Q		0x17
#define UAX_REG_R8Q		0x18
#define UAX_REG_R9Q		0x19
#define UAX_REG_R10Q	0x1A
#define UAX_REG_R11Q	0x1B
#define UAX_REG_R12Q	0x1C
#define UAX_REG_R13Q	0x1D
#define UAX_REG_R14Q	0x1E
#define UAX_REG_R15Q	0x1F

#define UAX_REG_R0W		0x20
#define UAX_REG_R1W		0x21
#define UAX_REG_R2W		0x22
#define UAX_REG_R3W		0x23
#define UAX_REG_R4W		0x24
#define UAX_REG_R5W		0x25
#define UAX_REG_R6W		0x26
#define UAX_REG_R7W		0x27
#define UAX_REG_R8W		0x28
#define UAX_REG_R9W		0x29
#define UAX_REG_R10W	0x2A
#define UAX_REG_R11W	0x2B
#define UAX_REG_R12W	0x2C
#define UAX_REG_R13W	0x2D
#define UAX_REG_R14W	0x2E
#define UAX_REG_R15W	0x2F

#define UAX_REG_R0B		0x30
#define UAX_REG_R1B		0x31
#define UAX_REG_R2B		0x32
#define UAX_REG_R3B		0x33
#define UAX_REG_R4B		0x34
#define UAX_REG_R5B		0x35
#define UAX_REG_R6B		0x36
#define UAX_REG_R7B		0x37
#define UAX_REG_R8B		0x38
#define UAX_REG_R9B		0x39
#define UAX_REG_R10B	0x3A
#define UAX_REG_R11B	0x3B
#define UAX_REG_R12B	0x3C
#define UAX_REG_R13B	0x3D
#define UAX_REG_R14B	0x3E
#define UAX_REG_R15B	0x3F

#define UAX_REG_XMM0	0x40
#define UAX_REG_XMM1	0x41
#define UAX_REG_XMM2	0x42
#define UAX_REG_XMM3	0x43
#define UAX_REG_XMM4	0x44
#define UAX_REG_XMM5	0x45
#define UAX_REG_XMM6	0x46
#define UAX_REG_XMM7	0x47
#define UAX_REG_XMM8	0x48
#define UAX_REG_XMM9	0x49
#define UAX_REG_XMM10	0x4A
#define UAX_REG_XMM11	0x4B
#define UAX_REG_XMM12	0x4C
#define UAX_REG_XMM13	0x4D
#define UAX_REG_XMM14	0x4E
#define UAX_REG_XMM15	0x4F

#define UAX_REG_AH		0xF4		//
#define UAX_REG_CH		0xF5		//
#define UAX_REG_DH		0xF6		//
#define UAX_REG_BH		0xF7		//

#define UAX_REG_RIP		0xF8		//no register
#define UAX_REG_Z		0xFF		//no register

#define UAX_RLC_REL8	0x01		//
#define UAX_RLC_REL16	0x02		//
#define UAX_RLC_REL32	0x03		//
#define UAX_RLC_ABS8	0x04		//
#define UAX_RLC_ABS16	0x05		//
#define UAX_RLC_ABS32	0x06		//
#define UAX_RLC_ABS64	0x07		//

#define UAX_CSEG_TEXT	0x00		//.text section
#define UAX_CSEG_DATA	0x01		//.data section
#define UAX_CSEG_BSS	0x02		//.bss section

#define UAX_CSEG_DYN	0x04		//dynamic sections

#define UAX_OP_ADC					4
#define UAX_OP_ADD					5
#define UAX_OP_AND					6
#define UAX_OP_BSF					7
#define UAX_OP_BSR					8
#define UAX_OP_BSWAP				9
#define UAX_OP_BT					10
#define UAX_OP_BTC					11
#define UAX_OP_BTR					12
#define UAX_OP_BTS					13
#define UAX_OP_CALL					14
#define UAX_OP_CALL_W				15
#define UAX_OP_CBW					16
#define UAX_OP_CWDE					17
#define UAX_OP_CDQE					18
#define UAX_OP_CLC					19
#define UAX_OP_CLD					20
#define UAX_OP_CLI					21
#define UAX_OP_CLTS					22
#define UAX_OP_CMC					23
#define UAX_OP_CMOVO				24
#define UAX_OP_CMOVNO				25
#define UAX_OP_CMOVNAE				26
#define UAX_OP_CMOVB				27
#define UAX_OP_CMOVNB				28
#define UAX_OP_CMOVNC				29
#define UAX_OP_CMOVAE				30
#define UAX_OP_CMOVZ				31
#define UAX_OP_CMOVE				32
#define UAX_OP_CMOVNZ				33
#define UAX_OP_CMOVNE				34
#define UAX_OP_CMOVNA				35
#define UAX_OP_CMOVBE				36
#define UAX_OP_CMOVNBE				37
#define UAX_OP_CMOVA				38
#define UAX_OP_CMOVS				39
#define UAX_OP_CMOVNS				40
#define UAX_OP_CMOVPE				41
#define UAX_OP_CMOVP				42
#define UAX_OP_CMOVNP				43
#define UAX_OP_CMOVPO				44
#define UAX_OP_CMOVNGE				45
#define UAX_OP_CMOVL				46
#define UAX_OP_CMOVNL				47
#define UAX_OP_CMOVGE				48
#define UAX_OP_CMOVNG				49
#define UAX_OP_CMOVLE				50
#define UAX_OP_CMOVNLE				51
#define UAX_OP_CMOVG				52
#define UAX_OP_CMP					53
#define UAX_OP_CMPSB				54
#define UAX_OP_CMPSW				55
#define UAX_OP_CMPSD				56
#define UAX_OP_CMPXCHG				57
#define UAX_OP_CPUID				58
#define UAX_OP_CWD					59
#define UAX_OP_CDQ					60
#define UAX_OP_CQO					61
#define UAX_OP_DAA					62
#define UAX_OP_DAS					63
#define UAX_OP_DEC					64
#define UAX_OP_DIV					65
#define UAX_OP_EMMS					66
#define UAX_OP_ENTER				67
#define UAX_OP_HLT					68
#define UAX_OP_IDIV					69
#define UAX_OP_IMUL					70
#define UAX_OP_IN					71
#define UAX_OP_INC					72
#define UAX_OP_INSB					73
#define UAX_OP_INSW					74
#define UAX_OP_INSD					75
#define UAX_OP_INSQ					76
#define UAX_OP_INT					77
#define UAX_OP_INTO					78
#define UAX_OP_INTXT				79
#define UAX_OP_INVLD				80
#define UAX_OP_INVPLG				81
#define UAX_OP_IRET					82
#define UAX_OP_IRETD				83
#define UAX_OP_JA					84
#define UAX_OP_JAE					85
#define UAX_OP_JB					86
#define UAX_OP_JBE					87
#define UAX_OP_JC					88
#define UAX_OP_JE					89
#define UAX_OP_JG					90
#define UAX_OP_JGE					91
#define UAX_OP_JL					92
#define UAX_OP_JLE					93
#define UAX_OP_JNA					94
#define UAX_OP_JNAE					95
#define UAX_OP_JNB					96
#define UAX_OP_JNBE					97
#define UAX_OP_JNC					98
#define UAX_OP_JNE					99
#define UAX_OP_JNG					100
#define UAX_OP_JNGE					101
#define UAX_OP_JNL					102
#define UAX_OP_JNLE					103
#define UAX_OP_JNO					104
#define UAX_OP_JNP					105
#define UAX_OP_JNS					106
#define UAX_OP_JNZ					107
#define UAX_OP_JO					108
#define UAX_OP_JP					109
#define UAX_OP_JPE					110
#define UAX_OP_JPO					111
#define UAX_OP_JS					112
#define UAX_OP_JZ					113
#define UAX_OP_JMP					114
#define UAX_OP_LAHF					115
#define UAX_OP_LAR					116
#define UAX_OP_LEA					117
#define UAX_OP_LEAVE				118
#define UAX_OP_LGDT					119
#define UAX_OP_LIDT					120
#define UAX_OP_LLDT					121
#define UAX_OP_LMSW					122
#define UAX_OP_LMTR					123
#define UAX_OP_LODSB				124
#define UAX_OP_LODSW				125
#define UAX_OP_LODSD				126
#define UAX_OP_LODSQ				127
#define UAX_OP_LOOP					128
#define UAX_OP_LOOPE				129
#define UAX_OP_LOOPZ				130
#define UAX_OP_LOOPNE				131
#define UAX_OP_LOOPNZ				132
#define UAX_OP_LSL					133
#define UAX_OP_MOV					134
#define UAX_OP_MOVSB				135
#define UAX_OP_MOVSW				136
#define UAX_OP_MOVSD				137
#define UAX_OP_MOVSQ				138
#define UAX_OP_MOVSX				139
#define UAX_OP_MOVSXB				140
#define UAX_OP_MOVSXW				141
#define UAX_OP_MOVSXD				142
#define UAX_OP_MOVZX				143
#define UAX_OP_MOVZXB				144
#define UAX_OP_MOVZXW				145
#define UAX_OP_MOVZXD				146
#define UAX_OP_MUL					147
#define UAX_OP_NEG					148
#define UAX_OP_NOP					149
#define UAX_OP_NOP2					150
#define UAX_OP_NOT					151
#define UAX_OP_OR					152
#define UAX_OP_OUT					153
#define UAX_OP_POP					154
#define UAX_OP_POPA					155
#define UAX_OP_POPAD				156
#define UAX_OP_POPF					157
#define UAX_OP_POPFD				158
#define UAX_OP_POPFQ				159
#define UAX_OP_PUSH					160
#define UAX_OP_PUSH_CS				161
#define UAX_OP_PUSH_SS				162
#define UAX_OP_PUSH_DS				163
#define UAX_OP_PUSH_ES				164
#define UAX_OP_PUSH_FS				165
#define UAX_OP_PUSH_GS				166
#define UAX_OP_PUSHA				167
#define UAX_OP_PUSHAW				168
#define UAX_OP_PUSHAD				169
#define UAX_OP_PUSHF				170
#define UAX_OP_PUSHFW				171
#define UAX_OP_PUSHFD				172
#define UAX_OP_PUSHFQ				173
#define UAX_OP_RCL					174
#define UAX_OP_RCR					175
#define UAX_OP_ROL					176
#define UAX_OP_ROR					177
#define UAX_OP_RDTSC				178
#define UAX_OP_RET					179
#define UAX_OP_RETF					180
#define UAX_OP_SAHF					181
#define UAX_OP_SAL					182
#define UAX_OP_SAR					183
#define UAX_OP_SBB					184
#define UAX_OP_SCAS					185
#define UAX_OP_SCASB				186
#define UAX_OP_SCASW				187
#define UAX_OP_SCASD				188
#define UAX_OP_SCASQ				189
#define UAX_OP_SETA					190
#define UAX_OP_SETAE				191
#define UAX_OP_SETB					192
#define UAX_OP_SETBE				193
#define UAX_OP_SETC					194
#define UAX_OP_SETE					195
#define UAX_OP_SETG					196
#define UAX_OP_SETGE				197
#define UAX_OP_SETL					198
#define UAX_OP_SETLE				199
#define UAX_OP_SETNA				200
#define UAX_OP_SETNAE				201
#define UAX_OP_SETNB				202
#define UAX_OP_SETNBE				203
#define UAX_OP_SETNC				204
#define UAX_OP_SETNE				205
#define UAX_OP_SETNG				206
#define UAX_OP_SETNGE				207
#define UAX_OP_SETNL				208
#define UAX_OP_SETNLE				209
#define UAX_OP_SETNO				210
#define UAX_OP_SETNP				211
#define UAX_OP_SETNS				212
#define UAX_OP_SETNZ				213
#define UAX_OP_SETO					214
#define UAX_OP_SETP					215
#define UAX_OP_SETPE				216
#define UAX_OP_SETPO				217
#define UAX_OP_SETS					218
#define UAX_OP_SETZ					219
#define UAX_OP_SFENCE				220
#define UAX_OP_SGDT					221
#define UAX_OP_SHL					222
#define UAX_OP_SHLD					223
#define UAX_OP_SHR					224
#define UAX_OP_SHRD					225
#define UAX_OP_SIDT					226
#define UAX_OP_SLDT					227
#define UAX_OP_SMSW					228
#define UAX_OP_STC					229
#define UAX_OP_STD					230
#define UAX_OP_STI					231
#define UAX_OP_STOSB				232
#define UAX_OP_STOSW				233
#define UAX_OP_STOSD				234
#define UAX_OP_STR					235
#define UAX_OP_SUB					236
#define UAX_OP_TEST					237
#define UAX_OP_UD2					238
#define UAX_OP_WAIT					239
#define UAX_OP_FWAIT				240
#define UAX_OP_WBINVD				241
#define UAX_OP_WRMSR				242
#define UAX_OP_XADD					243
#define UAX_OP_XCHG					244
#define UAX_OP_XOR					245

#define UAX_OP_ADDPD				392
#define UAX_OP_ADDPS				393
#define UAX_OP_ADDSD				394
#define UAX_OP_ADDSS				395
#define UAX_OP_ADDSUBPD				396
#define UAX_OP_ADDSUBPS				397
#define UAX_OP_ANDPD				398
#define UAX_OP_ANDPS				399
#define UAX_OP_ANDNPD				400
#define UAX_OP_ANDNPS				401
#define UAX_OP_CMPPD				402
#define UAX_OP_CMPPS				403
#define UAX_OP_CMPSS				404
#define UAX_OP_CMPEQPD				405
#define UAX_OP_CMPLTPD				406
#define UAX_OP_CMPLEPD				407
#define UAX_OP_CMPUNORDPD			408
#define UAX_OP_CMPNEQPD				409
#define UAX_OP_CMPNLTPD				410
#define UAX_OP_CMPNLEPD				411
#define UAX_OP_CMPORDPD				412
#define UAX_OP_CMPEQPS				413
#define UAX_OP_CMPLTPS				414
#define UAX_OP_CMPLEPS				415
#define UAX_OP_CMPUNORDPS			416
#define UAX_OP_CMPNEQPS				417
#define UAX_OP_CMPNLTPS				418
#define UAX_OP_CMPNLEPS				419
#define UAX_OP_CMPORDPS				420
#define UAX_OP_CMPEQSD				421
#define UAX_OP_CMPLTSD				422
#define UAX_OP_CMPLESD				423
#define UAX_OP_CMPUNORDSD			424
#define UAX_OP_CMPNEQSD				425
#define UAX_OP_CMPNLTSD				426
#define UAX_OP_CMPNLESD				427
#define UAX_OP_CMPORDSD				428
#define UAX_OP_CMPEQSS				429
#define UAX_OP_CMPLTSS				430
#define UAX_OP_CMPLESS				431
#define UAX_OP_CMPUNORDSS			432
#define UAX_OP_CMPNEQSS				433
#define UAX_OP_CMPNLTSS				434
#define UAX_OP_CMPNLESS				435
#define UAX_OP_CMPORDSS				436
#define UAX_OP_CMPGEPD				437
#define UAX_OP_CMPGTPD				438
#define UAX_OP_CMPGEPS				439
#define UAX_OP_CMPGTPS				440
#define UAX_OP_CMPGESD				441
#define UAX_OP_CMPGTSD				442
#define UAX_OP_CMPGESS				443
#define UAX_OP_CMPGTSS				444
#define UAX_OP_COMISD				445
#define UAX_OP_COMISS				446
#define UAX_OP_CVTDQ2PD				447
#define UAX_OP_CVTDQ2PS				448
#define UAX_OP_CVTPD2DQ				449
#define UAX_OP_CVTPD2PI				450
#define UAX_OP_CVTPD2PS				451
#define UAX_OP_CVTPI2PD				452
#define UAX_OP_CVTPI2PS				453
#define UAX_OP_CVTPS2DQ				454
#define UAX_OP_CVTPS2PD				455
#define UAX_OP_CVTPS2PI				456
#define UAX_OP_CVTSD2SI				457
#define UAX_OP_CVTSD2SS				458
#define UAX_OP_CVTSI2SD				459
#define UAX_OP_CVTSI2SS				460
#define UAX_OP_CVTSS2SD				461
#define UAX_OP_CVTSS2SI				462
#define UAX_OP_CVTTPD2PI			463
#define UAX_OP_CVTTPD2DQ			464
#define UAX_OP_CVTTPS2DQ			465
#define UAX_OP_CVTTPS2PI			466
#define UAX_OP_CVTTSD2SI			467
#define UAX_OP_CVTTSS2SI			468
#define UAX_OP_DIVPD				469
#define UAX_OP_DIVPS				470
#define UAX_OP_DIVSD				471
#define UAX_OP_DIVSS				472
#define UAX_OP_DPPD					473
#define UAX_OP_DPPS					474
#define UAX_OP_EXTRACTPS			475
#define UAX_OP_HADDPD				476
#define UAX_OP_HADDPS				477
#define UAX_OP_HSUBPD				478
#define UAX_OP_HSUBPS				479
#define UAX_OP_INSERTPS				480
#define UAX_OP_LDDQU				481
#define UAX_OP_MASKMOVDQU			482
#define UAX_OP_MASKMOVQ				483
#define UAX_OP_MAXPD				484
#define UAX_OP_MAXPS				485
#define UAX_OP_MAXSD				486
#define UAX_OP_MAXSS				487
#define UAX_OP_MINPD				488
#define UAX_OP_MINPS				489
#define UAX_OP_MINSD				490
#define UAX_OP_MINSS				491
#define UAX_OP_MOVAPD				492
#define UAX_OP_MOVAPS				493
#define UAX_OP_MOVD					494
#define UAX_OP_MOVQ					495
#define UAX_OP_MOVDDUP				496
#define UAX_OP_MOVDQA				497
#define UAX_OP_MOVDQU				498
#define UAX_OP_MOVDQ2Q				499
#define UAX_OP_MOVHLPS				500
#define UAX_OP_MOVHPD				501
#define UAX_OP_MOVHPS				502
#define UAX_OP_MOVLHPS				503
#define UAX_OP_MOVLPD				504
#define UAX_OP_MOVLPS				505
#define UAX_OP_MOVSKPD				506
#define UAX_OP_MOVSKPS				507
#define UAX_OP_MOVNTDQ				508
#define UAX_OP_MOVNTI				509
#define UAX_OP_MOVNTPD				510
#define UAX_OP_MOVNTPS				511
#define UAX_OP_MOVNTQ				512
#define UAX_OP_MOVQ2DQ				513
#define UAX_OP_MOVSHDUP				514
#define UAX_OP_MOVSLDUP				515
#define UAX_OP_MOVSS				516
#define UAX_OP_MOVUPD				517
#define UAX_OP_MOVUPS				518
#define UAX_OP_MPSADBW				519
#define UAX_OP_MULPD				520
#define UAX_OP_MULPS				521
#define UAX_OP_MULSD				522
#define UAX_OP_MULSS				523
#define UAX_OP_ORPD					524
#define UAX_OP_ORPS					525
#define UAX_OP_PABSB				526
#define UAX_OP_PABSW				527
#define UAX_OP_PABSD				528
#define UAX_OP_PACKSSWB				529
#define UAX_OP_PACKSSWW				530
#define UAX_OP_PACKUSWB				531
#define UAX_OP_PADDB				532
#define UAX_OP_PADDW				533
#define UAX_OP_PADDD				534
#define UAX_OP_PADDQ				535
#define UAX_OP_PADDSB				536
#define UAX_OP_PADDSW				537
#define UAX_OP_PADDUSB				538
#define UAX_OP_PADDUSW				539
#define UAX_OP_PALIGNR				540
#define UAX_OP_PAND					541
#define UAX_OP_PANDN				542
#define UAX_OP_PAVGB				543
#define UAX_OP_PAVGW				544
#define UAX_OP_PCMPEQB				545
#define UAX_OP_PCMPEQW				546
#define UAX_OP_PCMPEQD				547
#define UAX_OP_PCMPGTB				548
#define UAX_OP_PCMPGTW				549
#define UAX_OP_PCMPGTD				550
#define UAX_OP_PEXTRW				551
#define UAX_OP_PHADDW				552
#define UAX_OP_PHADDD				553
#define UAX_OP_PHADDSW				554
#define UAX_OP_PHSUBW				555
#define UAX_OP_PHSUBD				556
#define UAX_OP_PHSUBSW				557
#define UAX_OP_PINSRW				558
#define UAX_OP_PMADDUBSW			559
#define UAX_OP_PMADDWD				560
#define UAX_OP_PMAXSW				561
#define UAX_OP_PMAXUB				562
#define UAX_OP_PMINSW				563
#define UAX_OP_PMINUB				564
#define UAX_OP_PMOVMSKB				565
#define UAX_OP_PMULHRSW				566
#define UAX_OP_PMULHUW				567
#define UAX_OP_PMULHW				568
#define UAX_OP_PMULW				569
#define UAX_OP_PMULUDQ				570
#define UAX_OP_POR					571
#define UAX_OP_PSADBW				572
#define UAX_OP_PSHUFB				573
#define UAX_OP_PSHUFD				574
#define UAX_OP_PSHUFHW				575
#define UAX_OP_PSHUFLW				576
#define UAX_OP_PSHUFW				577
#define UAX_OP_PSIGNB				578
#define UAX_OP_PSIGNW				579
#define UAX_OP_PSIGND				580
#define UAX_OP_PSLLDQ				581
#define UAX_OP_PSLLW				582
#define UAX_OP_PSLLD				583
#define UAX_OP_PSLLQ				584
#define UAX_OP_PSRAW				585
#define UAX_OP_PSRAD				586
#define UAX_OP_PSRLDQ				587
#define UAX_OP_PSUBB				588
#define UAX_OP_PSUBW				589
#define UAX_OP_PSUBD				590
#define UAX_OP_PSUBQ				591
#define UAX_OP_PSUBSB				592
#define UAX_OP_PSUBSW				593
#define UAX_OP_PSUBUSB				594
#define UAX_OP_PSUBUSW				595
#define UAX_OP_PUNPCKHBW			596
#define UAX_OP_PUNPCKHWD			597
#define UAX_OP_PUNPCKHDQ			598
#define UAX_OP_PUNPCKHQDQ			599
#define UAX_OP_PUNPCKLBW			600
#define UAX_OP_PUNPCKLWD			601
#define UAX_OP_PUNPCKLDQ			602
#define UAX_OP_PUNPCKLQDQ			603
#define UAX_OP_PXOR					604
#define UAX_OP_RCPPS				605
#define UAX_OP_RCPSS				606
#define UAX_OP_RSQRTPS				607
#define UAX_OP_RSQRTSS				608
#define UAX_OP_SHUFPD				609
#define UAX_OP_SHUFPS				610
#define UAX_OP_SQRTPS				611
#define UAX_OP_SQRTSD				612
#define UAX_OP_SQRTSS				613
#define UAX_OP_SUBPD				614
#define UAX_OP_SUBPS				615
#define UAX_OP_SUBSD				616
#define UAX_OP_SUBSS				617
#define UAX_OP_UCOMISD				618
#define UAX_OP_UCOMISS				619
#define UAX_OP_UNPCKHPD				620
#define UAX_OP_UNPCKHPS				621
#define UAX_OP_UNPCKLPD				622
#define UAX_OP_UNPCKLPS				623
#define UAX_OP_XORPD				624
#define UAX_OP_XORPS				625

#define UAX_LBL_LOCALSTART	0xC000
#define UAX_LBL_NEAR		0x10000		//within +/- 128 bytes

#define UAX_MEMSC_TMASK		0xF0		//type mask
#define UAX_MEMSC_BYTE		0x10		//BYTE
#define UAX_MEMSC_WORD		0x20		//BYTE
#define UAX_MEMSC_DWORD		0x30		//BYTE
#define UAX_MEMSC_QWORD		0x40		//BYTE
#define UAX_MEMSC_XMM		0x50		//BYTE

typedef struct UAX_Context_s UAX_Context;
typedef struct UAX_OpcodeArg_s UAX_OpcodeArg;

typedef struct UAX_OpcodeDesc_s UAX_OpcodeDesc;

#ifdef UAX_WINX64
#define UAX_MAX_CACHEVAR	6
#endif

#ifdef UAX_SYSVAMD64
#define UAX_MAX_CACHEVAR	4
#endif

#ifdef UAX_WINX64
#define UAX_REG_RCCTX			UAX_REG_RDI
#define BTEJ_X64_FRIDX_RCCTX	BTEJ_X64_FRIDX_RDI
#endif

#ifdef UAX_SYSVAMD64
#define UAX_REG_RCCTX			UAX_REG_RBX
#define BTEJ_X64_FRIDX_RCCTX	BTEJ_X64_FRIDX_RBX
#endif

struct UAX_Context_s {
byte *sec_buf[8];
byte *sec_end[8];
byte *sec_pos[8];
byte sec;
byte nsec;

int lbl_ofs[256];	//label offsets
int rlc_ofs[256];	//reloc offsets
u16 lbl_id[256];	//label IDs
u16 rlc_id[256];	//reloc label IDs
byte lbl_sec[256];	//label section
byte rlc_sec[256];	//reloc section
byte rlc_ty[256];	//reloc type
int nlbl;
int nrlc;
u16 lblrov;			//labels (local/temp)

byte reg_idx[UAX_MAX_CACHEVAR];
byte reg_reg[UAX_MAX_CACHEVAR];
int reg_live;
int reg_resv;
int reg_save;

int jitfl;
byte isjq;

UAX_Context *next;
};

/*
sc==0: register, immediate, or label
	breg!=Z: register
	breg==Z: immediate (disp)
sc!=0: memory reference
 */
struct UAX_OpcodeArg_s {
byte sc;		//scale for Reg/RM forms (1,2,3,4 for mem-ref)
byte ireg;		//index for Reg/RM forms
byte breg;		//base for Reg/RM forms, or register
s64 disp;		//displacement for Reg/RM forms, or immed
int lbl;		//label
};

struct UAX_OpcodeDesc_s {
int id;
char *opstr;
char *args;
int next;
};