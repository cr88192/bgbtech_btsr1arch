# bgbtech_btsr1arch
BtSR1 ISA / CPU Architecture

BtSR1, or BSR1, is a CPU architecture primarily intended for soft-core microcontroller applications.
The initial use case is intended to be loosely similar to that of a 32-bit analog of an MSP430, but with the option to
have higher performance and implement some peripherals directly in Verilog.

Also intended to be a free/open arch which (if anyone actually cares) may be modified/extended as appropriate for various
use cases. It is not intended to be a general purpose processor arch or for the distribution of application code, so it is
more acceptable for there to be variations or breaks in binary compatibility between implementations (granted, it is
preferable if breaks in compatibility can be avoided if possible).

My personal use-case is currently mostly in the context of using it for real-time motor control.

As of 2018-05-20, This is still at a fairly early stage of development and is not yet ready for general use, however some
initial results look promising, and I am making better progress than I was with BJX1 (if anything, because BSR1 is a bit simpler
from an implementation perspective, and also represents a somewhat lower target).


General design summary:
* Instruction set with a fixed 16-bit instruction format.
* Little endian, supports misaligned access to data.
* Intended for a small address space with fixed regions for ROM and RAM.
* Does not implement division in hardware.
* Currently lacks a general barrel shifter (to save cost, may add later).
* Does currently plan to support multiply in hardware.


bgbcc22: C compiler, partly reused from my BJX1 project, but modified to add support for BSR1.
* TODO: Make it more obvious how to use said compiler.

vmbase: Holds an emulator for the processor.

vlcore: Verilog implementation of the processor.
