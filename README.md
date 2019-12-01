# bgbtech_btsr1arch
BtSR1 and BJX2 ISA / CPU Architecture

BtSR1, or BSR1, is a CPU architecture primarily intended for soft-core microcontroller applications.
The initial use case is intended to be loosely similar to that of a 32-bit analog of an MSP430, but with the option to
have higher performance and implement some peripherals directly in Verilog.

BJX2 is a larger and more advanced ISA based on a similar design. It is a 64-bit ISA with 32 GPRs. It will be higher performance, but requires a larger FPGA.

For these, I am generally targeting FPGA's in the 25k to 100k LUT range (currently mostly Spartan-7 and Artix-7).


Side note, this is a personal / hobby project of Brendan G Bohannon ( cr88192@gmail.com ).


Also intended to be a free/open arch which (if anyone actually cares) may be modified/extended as appropriate for various
use cases. It is not intended to be a general purpose processor arch or for the distribution of application code, so it is
more acceptable for there to be variations or breaks in binary compatibility between implementations (granted, it is
preferable if breaks in compatibility can be avoided if possible).

My personal use-case is currently mostly in the context of using BSR1 for real-time motor control.

As of 2018-05-20, This is still at a fairly early stage of development and is not yet ready for general use, however some
initial results look promising, and I am making better progress than I was with BJX1 (if anything, because BSR1 is a bit simpler
from an implementation perspective, and also represents a somewhat lower target).

As of 2019-12-01, I have reached a stage where I can more-or-less run Doom and Quake on the BJX2 Verilog implementation on an FPGA. Work continues on this front.


General design summary (BtSR1):
* Instruction set with a fixed 16-bit instruction format.
* Little endian, supports misaligned access to data (optional).
* Intended for a small address space with fixed regions for ROM and RAM.
* Does not implement division in hardware.
* Omits the barrel shifter (to save cost, optional).
* Does multiply in hardware.

General design summary of BJX2:
* Instruction set with variable length 16/32/48 bit instructions.
* Little endian, supports misaligned load/store.
* Uses 32x 64-bit GPRs (27 usable as GPRs).
* Larger feature-set than BSR1, but is otherwise a similar design.
* Goal is to be easier to implement in hardware than my older (SH based) BJX1 designs.
* Supports predicated instructions.
* Supports explicitly parallel encodings (WEX2).

BJX2 currently has an FPU and MMU, with a 48 bit Virtual Address space and a 32-bit Physical Addresses space. The ISA and MMU design allow for a 48 bit Physical Address space as well. A mode exists where the Virtual Address space is confined to 32-bits, and a subset exists which uses 32-bit addresses.

Some aspects of BJX2 are still in flux and the design is not yet frozen.


bgbcc22: C compiler, partly reused from my BJX1 project, but modified to add support for BSR1 and BJX2.
* TODO: Make it more obvious how to use said compiler.

vmbase: Holds an emulator for the BSR1 ISA.

jx2vm: Emulator for the BJX2 ISA.

vlcore: Verilog implementation of the BSR1 processor.
* Fairly limited, BSR1 was meant as a small microcontroller ISA.

jx2vlcore: Verilog attempts at a BJX2 processor.
* jx2vlcore2: Partial rework of the former (old).
* jx2vlcore3: Another attempt (current).

tk_qsrc: Source for Quake, Doom, and C library.
* A lot of the code here was not written by me; check licensing more carefully in here.
* A lot of this is code for testing stuff, ...
