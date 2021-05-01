/*

Ring Bus Interface

Data(127:0):
  Data passed with message.

Physical Addr (Low 4GB):
  00000000..00007FFF: Boot ROM (Base)
  00008000..0000BFFF: Boot ROM (Ext)
  0000C000..0000DFFF: Boot SRAM
  0000E000..0000FFFF: Reserved
  00010000..0001FFFF: Null Page
  00020000..00FFFFFF: Reserved
  01000000..3FFFFFFF: DRAM (Wrap based on RAM Size)
  40000000..7FFFFFFF: Reserved (More DRAM)
  80000000..EFFFFFFF: Reserved
  F0000000..FFFFFFFF: MMIO

Addr(47:32):
  0000..7FFF: VADDR (Userland)

  8000..BFFF: VADDR (System)
  C000..C7FF: Physical Address Range
  C800..CEFF: (Reserved)
  CF00..CFFF: MMIO Range
  D000..FFFF: VADDR (System)

Virtual Addresses will exist in a 48-bit space.
Some of this space will be used as a physical address range.


OPMOK (16-bit): Combined Request/Response Code
  (15:8): Access Status Bits
  ( 7:6): Operation Category
  ( 5:4): Operation Mode
  ( 3:0): Operation Element

Operation Category:
  00: ?
  01: IO Response
  10: IO Request
  11: Message/Interrupt

Access Status Bits(Request/Response):
  (3): No Cache
  (2): No Execute
  (1): No Write
  (0): No Read
    All 4 bits will be set on a TLB Miss.
    No Cache will indicate that the cache line should be evicted early.

Operation Mode (Request):
  00: IDLE or CCMD
  01: LOAD
  10: STORE
  11: SWAP (Load+Store)

Operation Mode (Response):
  00: NONE
  01: FAULT (Request Failed)
  10: OK (No Payload)
  11: OK (Has Payload)

Operation Element (Load/Store):
  0000: Signed Byte (N/A)
  0001: Signed Byte (N/A)
  0010: Signed DWord (MMIO)
  0011: QWord (MMIO)
  0100: Unsigned Byte (N/A)
  0101: Unsigned Byte (N/A)
  0110: Unsigned DWord (MMIO)
  0111: Tile128 (RAM)

Core Sequence (15:0)
  (15:10): Core ID
  ( 9: 8): Major Unit
  ( 7: 6): ~ Sub-Unit
  ( 5: 0): ~ Sequence

This is used to match up requests and responses, and to route responses back to their original requests.

IRQ:
  Data(15:12): Exception Class
  Data(11: 8): Core ID (1..14)
    0: Local to Core / First Valid Core.
    1..14: Core Number (Routed Interrupt)
    15: Broadcast Interrupt
  Data(7:0): Exception Number
  Seq(15:10): Core ID (First Seen)
    If a broadcast message gets back here, drop request.

*/

`ifndef HAS_RBIDEFS
`define HAS_RBIDEFS

// `include "../CoreDefs.v"
`include "CoreDefs.v"

parameter[7:0] JX2_RBI_OPM_IDLE		= 8'h00;	//

parameter[7:0] JX2_RBI_OPM_INVTLB	= 8'h82;	//
parameter[7:0] JX2_RBI_OPM_LDTLB	= 8'h83;	//
parameter[7:0] JX2_RBI_OPM_FLUSHIS	= 8'h84;	//
parameter[7:0] JX2_RBI_OPM_FLUSHDS	= 8'h85;	//
// parameter[7:0] JX2_RBI_OPM_IRQ		= 8'h86;	//Interrupt Request
parameter[7:0] JX2_RBI_OPM_IRQ		= 8'hC6;	//Interrupt Request

parameter[7:0] JX2_RBI_OPM_LDSB		= 8'h90;	//
parameter[7:0] JX2_RBI_OPM_LDSW		= 8'h91;	//
parameter[7:0] JX2_RBI_OPM_LDSL		= 8'h92;	//
parameter[7:0] JX2_RBI_OPM_LDSQ		= 8'h93;	//
parameter[7:0] JX2_RBI_OPM_LDUB		= 8'h94;	//
parameter[7:0] JX2_RBI_OPM_LDUW		= 8'h95;	//
parameter[7:0] JX2_RBI_OPM_LDUL		= 8'h96;	//
parameter[7:0] JX2_RBI_OPM_LDX		= 8'h97;	//

parameter[7:0] JX2_RBI_OPM_STSB		= 8'hA0;	//
parameter[7:0] JX2_RBI_OPM_STSW		= 8'hA1;	//
parameter[7:0] JX2_RBI_OPM_STSL		= 8'hA2;	//
parameter[7:0] JX2_RBI_OPM_STSQ		= 8'hA3;	//
parameter[7:0] JX2_RBI_OPM_STUB		= 8'hA4;	//
parameter[7:0] JX2_RBI_OPM_STUW		= 8'hA5;	//
parameter[7:0] JX2_RBI_OPM_STUL		= 8'hA6;	//
parameter[7:0] JX2_RBI_OPM_STX		= 8'hA7;	//

parameter[7:0] JX2_RBI_OPM_SWSB		= 8'hB0;	//
parameter[7:0] JX2_RBI_OPM_SWSW		= 8'hB1;	//
parameter[7:0] JX2_RBI_OPM_SWSL		= 8'hB2;	//
parameter[7:0] JX2_RBI_OPM_SWSQ		= 8'hB3;	//
parameter[7:0] JX2_RBI_OPM_SWUB		= 8'hB4;	//
parameter[7:0] JX2_RBI_OPM_SWUW		= 8'hB5;	//
parameter[7:0] JX2_RBI_OPM_SWUL		= 8'hB6;	//
parameter[7:0] JX2_RBI_OPM_SWX		= 8'hB7;	//


parameter[7:0] JX2_RBI_OPM_OKST		= 8'h60;	//
parameter[7:0] JX2_RBI_OPM_OKLD		= 8'h70;	//

parameter[7:0] JX2_RBI_OPM_FAIL_RT	= 8'h50;	//No Route

parameter[15:0] JX2_RBI_ADDRHI_PHYS	= 16'hC000;			//Physical Address Range


parameter[5:0] JX2_DCOPM_FLUSHIS	= 6'b000100;		//Flush I$ Request
parameter[5:0] JX2_DCOPM_FLUSHDS	= 6'b000101;		//Flush D$ Request

`endif
