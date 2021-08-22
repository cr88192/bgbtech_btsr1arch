#include <stdint.h>
#include <fenv.h>

uint32_t htonl(uint32_t v);
uint16_t htons(uint16_t v);
uint32_t ntohl(uint32_t v);
uint16_t ntohs(uint16_t v);

uint16_t htobe16(uint16_t host_16bits);
uint16_t htole16(uint16_t host_16bits);
uint16_t be16toh(uint16_t big_endian_16bits);
uint16_t le16toh(uint16_t little_endian_16bits);

uint32_t htobe32(uint32_t host_32bits);
uint32_t htole32(uint32_t host_32bits);
uint32_t be32toh(uint32_t big_endian_32bits);
uint32_t le32toh(uint32_t little_endian_32bits);

uint64_t htobe64(uint64_t host_64bits);
uint64_t htole64(uint64_t host_64bits);
uint64_t be64toh(uint64_t big_endian_64bits);
uint64_t le64toh(uint64_t little_endian_64bits);

__asm {
htobe64:
be64toh:
	PSHUF.B		R4, 0x1B, R4
	MOVLHD		R4, R4, R2
	PSHUF.B		R2, 0x1B, R2
	RTS

htole64:
le64toh:
	MOV		R4, R2
	RTS

htobe32:
be32toh:
htonl:
ntohl:
	PSHUF.B		R4, 0x1B, R2
	EXTU.L		R2, R2
	RTS

htole32:
le32toh:
	EXTU.L		R4, R2
	RTS

htobe16:
be16toh:
htons:
ntohs:
	PSHUF.B		R4, 0x11, R2
	EXTU.W		R2, R2
	RTS

htole16:
le16toh:
	EXTU.W	R4, R2
	RTS

};

#if 1

int  feclearexcept(int v);
int  fegetexceptflag(fexcept_t *ex, int v);
int  feraiseexcept(int v);
int  fesetexceptflag(const fexcept_t *ex, int v);
int  fetestexcept(int v);
int  fegetround(void);
int  fesetround(int v);
int  fegetenv(fenv_t *env);
int  feholdexcept(fenv_t *env);
int  fesetenv(const fenv_t *env);
int  feupdateenv(const fenv_t *env);

__asm {
feclearexcept:
	MOV		GBR, R6
	MOV		0x00FFFFFFFFFFFFFF, R1
	AND		R1, R6
	MOV		R6, GBR
	MOV		0, R2
	RTS

fegetenv:
fegetexceptflag:
	MOV		GBR, R6
	SHLD.Q	R6, -48, R7
	MOV.L	R7, (R4)
	MOV		0, R2
	RTS

fesetenv:
feupdateenv:
	MOV		GBR, R6
	MOV		0x0000FFFFFFFFFFFF, R1
	AND		R1, R6
	MOV.L	(R4), R7
	SHLD.Q	R7, 48, R7
	OR		R7, R6
	MOV		R6, GBR
	MOV		0, R2
	RTS

fesetexceptflag:
	MOV		GBR, R6
	MOV		0x00FFFFFFFFFFFFFF, R1
	AND		R1, R6
	MOV.L	(R4), R7
	AND		0xFF00, R7
	SHLD.Q	R7, 48, R7
	OR		R7, R6
	MOV		R6, GBR
	MOV		0, R2
	RTS

fegetround:
	MOV		GBR, R6
	SHLD.Q	R6, -48, R7
	AND		R7, 15, R2
	RTS

fesetround:
	MOV		GBR, R6
	MOV		0xFFF0FFFFFFFFFFFF, R1
	AND		R1, R6
	AND		15, R4
	SHLD.Q	R4, 48, R4
	OR		R4, R6
	MOV		R6, GBR
	MOV		0, R2
	RTS

feholdexcept:
	MOV		GBR, R6
	SHLD.Q	R6, -48, R7
	MOV.L	R7, (R4)

	MOV		0x00FFFFFFFFFFFFFF, R1
	AND		R1, R6
	MOV		R6, GBR

	MOV		0, R2
	RTS

feraiseexcept:
	MOV		-1, R2
	RTS

fetestexcept:
	MOV		GBR, R6
	SHLD.Q	R6, -48, R7
	AND		R7, R4, R2
	RTS
};

#endif
