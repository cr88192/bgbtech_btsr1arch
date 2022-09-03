#include <stdint.h>

typedef uint8_t	byte;
typedef int8_t		sbyte;
typedef uint16_t	u16;
typedef int16_t	s16;
typedef uint32_t	u32;
typedef int32_t	s32;
typedef uint64_t	u64;
typedef int64_t	s64;

#define VMX86_MAXOPS	16

#define VMX86_REGS_RAX		0x00
#define VMX86_REGS_RCX		0x01
#define VMX86_REGS_RDX		0x02
#define VMX86_REGS_RBX		0x03
#define VMX86_REGS_RSP		0x04
#define VMX86_REGS_RBP		0x05
#define VMX86_REGS_RSI		0x06
#define VMX86_REGS_RDI		0x07
#define VMX86_REGS_R8		0x08
#define VMX86_REGS_R9		0x09
#define VMX86_REGS_R10		0x0A
#define VMX86_REGS_R11		0x0B
#define VMX86_REGS_R12		0x0C
#define VMX86_REGS_R13		0x0D
#define VMX86_REGS_R14		0x0E
#define VMX86_REGS_R15		0x0F

#define VMX86_REGS_MM0		0x10
#define VMX86_REGS_MM1		0x11
#define VMX86_REGS_MM2		0x12
#define VMX86_REGS_MM3		0x13
#define VMX86_REGS_MM4		0x14
#define VMX86_REGS_MM5		0x15
#define VMX86_REGS_MM6		0x16
#define VMX86_REGS_MM7		0x17

#define VMX86_REGS_FLAGS	0x1C
#define VMX86_REGS_RIP		0x1E
#define VMX86_REGS_ZZR		0x1F

#define VMX86_REGS_XMM0		0x20
#define VMX86_REGS_XMM1		0x22
#define VMX86_REGS_XMM2		0x24
#define VMX86_REGS_XMM3		0x26
#define VMX86_REGS_XMM4		0x28
#define VMX86_REGS_XMM5		0x2A
#define VMX86_REGS_XMM6		0x2C
#define VMX86_REGS_XMM7		0x2E
#define VMX86_REGS_XMM8		0x30
#define VMX86_REGS_XMM9		0x32
#define VMX86_REGS_XMM10	0x34
#define VMX86_REGS_XMM11	0x36
#define VMX86_REGS_XMM12	0x38
#define VMX86_REGS_XMM13	0x3A
#define VMX86_REGS_XMM14	0x3C
#define VMX86_REGS_XMM15	0x3E

#define VMX86_REGS_MAX		0x40

#define VMX86_FLAGS_C		0x0001
#define VMX86_FLAGS_P		0x0004
#define VMX86_FLAGS_A		0x0010
#define VMX86_FLAGS_Z		0x0040
#define VMX86_FLAGS_S		0x0080
#define VMX86_FLAGS_T		0x0100
#define VMX86_FLAGS_I		0x0200
#define VMX86_FLAGS_D		0x0400
#define VMX86_FLAGS_O		0x0800


#define VMX86_OPFX_DATASZ	0x0001	//66
#define VMX86_OPFX_ADDRSZ	0x0002	//67
#define VMX86_OPFX_REPNE	0x0004	//F2
#define VMX86_OPFX_REPE		0x0008	//F3
#define VMX86_OPFX_LOCK		0x0010	//F0
#define VMX86_OPFX_OP2		0x0020	//0F
#define VMX86_OPFX_FS		0x0040	//64
#define VMX86_OPFX_GS		0x0080	//65
#define VMX86_OPFX_CS		0x0100	//2E
#define VMX86_OPFX_SS		0x0200	//36
#define VMX86_OPFX_DS		0x0400	//3E
#define VMX86_OPFX_ES		0x0800	//26

#define VMX86_FMID_OPREG_B		0x00	//reg (opcode)
#define VMX86_FMID_OPREG_W		0x01	//reg (opcode)
#define VMX86_FMID_OPREG_L		0x02	//reg (opcode)
#define VMX86_FMID_OPREG_Q		0x03	//reg (opcode)
#define VMX86_FMID_ALI_B		0x04	//AL, Byte
#define VMX86_FMID_ALI_W		0x05	//AX, Word
#define VMX86_FMID_ALI_L		0x06	//EAX, DWord
#define VMX86_FMID_ALI_Q		0x07	//RAX, QWord
#define VMX86_FMID_REGRM_B		0x08	//reg, r/m
#define VMX86_FMID_REGRM_W		0x09	//reg, r/m
#define VMX86_FMID_REGRM_L		0x0A	//reg, r/m
#define VMX86_FMID_REGRM_Q		0x0B	//reg, r/m
#define VMX86_FMID_RMREG_B		0x0C	//r/m, reg
#define VMX86_FMID_RMREG_W		0x0D	//r/m, reg
#define VMX86_FMID_RMREG_L		0x0E	//r/m, reg
#define VMX86_FMID_RMREG_Q		0x0F	//r/m, reg

#define VMX86_FMID_OPREGIMM_B	0x10	//reg (opcode), Imm8
#define VMX86_FMID_OPREGIMM_W	0x11	//reg (opcode), Imm16
#define VMX86_FMID_OPREGIMM_L	0x12	//reg (opcode), Imm32
#define VMX86_FMID_OPREGIMM_Q	0x13	//reg (opcode), Imm64

#define VMX86_FMID_RMU_B		0x18	//r/m
#define VMX86_FMID_RMU_W		0x19	//r/m
#define VMX86_FMID_RMU_L		0x1A	//r/m
#define VMX86_FMID_RMU_Q		0x1B	//r/m
#define VMX86_FMID_RMIMM_B		0x1C	//r/m, imm
#define VMX86_FMID_RMIMM_W		0x1D	//r/m, imm
#define VMX86_FMID_RMIMM_L		0x1E	//r/m, imm
#define VMX86_FMID_RMIMM_Q		0x1F	//r/m, imm


#define VMX86_NMID_ADD			0x0000
#define VMX86_NMID_OR			0x0001
#define VMX86_NMID_ADC			0x0002
#define VMX86_NMID_SBB			0x0003
#define VMX86_NMID_AND			0x0004
#define VMX86_NMID_SUB			0x0005
#define VMX86_NMID_XOR			0x0006
#define VMX86_NMID_CMP			0x0007
#define VMX86_NMID_MOV			0x0008
#define VMX86_NMID_LEA			0x000B
#define VMX86_NMID_TEST			0x000C
#define VMX86_NMID_XCHG			0x000D
#define VMX86_NMID_INC			0x000C
#define VMX86_NMID_DEC			0x000D
#define VMX86_NMID_PUSH			0x000E
#define VMX86_NMID_POP			0x000F

struct VMX86_Opcode_s {
void (*Run)(VMX86_Context *ctx, VMX86_Opcode *op);
u64 (*GetMem)(VMX86_Context *ctx, VMX86_Opcode *op);
void (*SetMem)(VMX86_Context *ctx, VMX86_Opcode *op, u64 val);
s64 rip;	//Instruction Address
s64 imm;	//Immediate
s32 disp;	//Displacement (Mem)
u16 opfx;	//Opcode Prefix
u16 nmid;	//Name
byte fmid;	//Form
byte sz;	//Size (Bytes)
byte rex;	//Rex Prefix
byte rn;	//Reg
byte rb;	//Base
byte ri;	//Index
byte sc;	//Scale
};

struct VMX86_Trace_s {
VMX86_Trace *(*Run)(VMX86_Context *ctx, VMX86_Trace *tr);
s64 rip;
byte n_ops;
VMX86_Trace *tr_hnext;	//Next (hash chain)
VMX86_Trace *tr_next;	//Next (by RIP)
VMX86_Trace *tr_jump;	//Jump Target
VMX86_Opcode *ops[VMX86_MAXOPS];
};

struct VMX86_Context_s {
u64 regs[VMX86_REGS_MAX];
VMX86_Trace *tr_next;

byte	is_x64;			//Operating in Long Mode.
s64		cached_agu;		//cached AGU address

byte *nat_ram;			//native virtual-address mapping.
int nat_ramsz;			//native RAM size (log2)
};
