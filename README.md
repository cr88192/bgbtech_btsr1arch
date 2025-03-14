# bgbtech_btsr1arch
BtSR1 and BJX2 ISA / CPU Architectures

There are two ISAs in this project:
* BtSR1 is an older and simpler 32-bit ISA
* BJX2 is a fancier 64-bit ISA

Considering renaming BJX2 to BJX or BJXG (2025-03).
* A long standing issue, is that the name is unfortunate.
* I didn't realize this at first, but it is an issue.


BtSR1, or BSR1, is a CPU architecture primarily intended for soft-core microcontroller applications.
The initial use case is intended to be loosely similar to that of a 32-bit analog of an MSP430, but with the option to
have higher performance and implement some peripherals directly in Verilog.

BJX2 is a larger and more advanced ISA starting from a similar design. It is a 64-bit ISA with 32 or 64 GPRs (baseline is 32, but an optional extension allows for 64 GPRs). It will be higher performance, but requires a larger FPGA. It supports VLIW bundles and SIMD.

For these, I am generally targeting FPGA's in the 25k to 100k LUT range (currently mostly Spartan-7 and Artix-7).

(2025-03) Though, BJX is mostly impractical much under 50K LUT.
* It works best thus far on XC7A100T or XC7A200T.
* It is possible to shoe-horn it onto an XC7S50, with mixed results.


Side note, this is a personal / hobby project of Brendan G Bohannon ( cr88192@gmail.com ).


Also intended to be a free/open arch which (if anyone actually cares) may be modified/extended as appropriate for various
use cases. It is not intended to be a general purpose processor arch or for the distribution of application code, so it is
more acceptable for there to be variations or breaks in binary compatibility between implementations (granted, it is
preferable if breaks in compatibility can be avoided if possible).

My personal use-case is currently mostly in the context of using BSR1 for real-time motor control (Update, 2021-07: BJX2 has mostly taken over this role, as single-wide cores can be fit onto an XC7S25). ( Update, 2024-09: I have mostly dropped these use-cases in favor of using RISC-V instead. I am currently focusing on BJX2 for more computationally oriented tasks, rather than microcontroller tasks. )

Update, 2025-03: Should I have my own dedicated RV32 core?...
* Removed some older comments.


General design summary of BtSR1:
* Instruction set with a fixed 16-bit instruction format.
* Little endian, supports misaligned access to data (optional).
* Intended for a small address space with fixed regions for ROM and RAM.
* Does not implement division in hardware.
* Omits the barrel shifter (to save cost, optional).
* Does multiply in hardware.
* (Add: Not in active development or maintenance).

General design summary of BJX2:
* Instruction set with variable length 16/32/64/96 bit instructions.
* Little endian, supports misaligned load/store.
* Uses 32x (or 64x) 64-bit GPRs (27 or 59 usable as GPRs).
* Larger feature-set than BSR1, but is otherwise a similar design.
* Goal is to be easier to implement in hardware than my older (SH based) BJX1 designs.
* Supports predicated instructions.
* Supports explicitly parallel encodings (WEX2).
* Has split into 3 major variants (XG1, XG2, and XG3)

BJX2 currently has an FPU and MMU, with a 48 bit Virtual Address space and a 32-bit Physical Addresses space. The ISA and MMU design allow for a 48 bit Physical Address space as well. A mode exists where the Virtual Address space is confined to 32-bits, and a subset exists which uses 32-bit addresses.

Some aspects of BJX2 are still in flux and the design is not yet frozen.
Update 2021-07: The core ISA of BJX2 has mostly stabilized, and significant design changes have become less frequent. Things may still break without warning, though I am making an effort to try to avoid this.

Update, 2025-03:
* XG1: Original form of the ISA, 16/32/64/96 bit encodings.
* XG2: 32/64/96 bit encodings (drops 16-bit for more encoding bits).
* XG3: Repacked to be encoding-space compatible with RV64G.

XG1: Natively 32 GPRs for 32-bit encodings, but supports 64 GPRs for a subset.

XG2: Drops 16 bit ops, can encode 64 GPRs for the entire ISA, also expands immediate fields in various cases.

XG3: Natively 64 GPRs, can exist in the same encoding space as RV64G (replacing the 16 bit RV-C encodings). It is possible to freely mix XG3 and RV64G instructions in this mode (but not RV-C, where RV64GC is a different CPU mode). XG3 uses the same register numbering as RISC-V, just merges the X and F registers into a single register space; and uses a variant of the RV64 LP64 ABI.


bgbcc22 (BGBCC): C compiler, partly reused from my BJX1 project, but modified to add support for BSR1 and BJX2.
* TODO: Make it more obvious how to use said compiler.

I will add an optional exception to the MIT license for BGBCC:
* One may, at their discretion, choose to disregard the middle clause of the MIT license, allowing derived copies to be understood as-if they were CC0 or Public Domain.

Change, 2025-03, Note that BGBCC is now MIT-0 / MIT-ZERO.

Note for target names:
* "BJX2": Original XG1 ISA.
* "XG2A": XG2
* "XRVA": RISC-V (assumes PE/COFF output)
* "XG3RV" / "X3RV": XG3



vmbase: Holds an emulator for the BSR1 ISA.

jx2vm: Emulator for the BJX2 ISA.

vlcore: Verilog implementation of the BtSR1 processor.
* Fairly limited, BSR1 was meant as a small microcontroller ISA.

jx2vlcore: Verilog attempts at a BJX2 processor.
* jx2vlcore3: Another attempt (current).

(2025-03): jx2vlcore3 does XG1/XG2/XG3 and RV64G.
* But, can't boot a standard RV OS kernel.


tk_qsrc: Source for Quake, Doom, and C library.
* A lot of the code here was not written by me; check licensing more carefully in here.
* A lot of this is code for testing stuff, ...
* tk_qsrc/doomsrc2: Doom goes here.
* tk_qsrc/tk_clib: C library and OS stuff.

tk_ports: Various software I have ported to BJX2.
* License depends on the program.
* I am trying to avoid distributing any files which I don't have the rights to, but it is possible some might leak through.
* Generally, people will be expected to provide their own game WADs or similar, ...

