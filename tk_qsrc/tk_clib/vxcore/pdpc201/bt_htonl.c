#include <stdint.h>

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
