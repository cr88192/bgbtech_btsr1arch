/*
Sanity check for RISC-V submode.

Note that the BGBCC RISC-V submode sticks with BJX2 ASM syntax.
 */

.riscv

_chksane_riscv:
	NOP
	NOP

	RTS		// JALR X0, X1, 0
	
	NOP
	NOP
	NOP
	
//	BRA		_chksane_riscv

.endriscv

.global X(tk_checksane_rvi_asm)
X(tk_checksane_rvi_asm):
	CPUID	1
	SHLD.Q	R0, -24, R4
	TEST	1, R4
	BF		.L00
	RTS
	.L00:

	ADD		-16, SP
	MOV		LR, R1
	MOV.Q	R1, (SP, 0)

	

#if 1
	MOV		_chksane_riscv, R3
	ADD		1, R3
	JSR		R3
#endif

//	BREAK
	
	MOV.Q	(SP, 0), R1

//	BREAK

	ADD		16, SP

	JMP		R1
