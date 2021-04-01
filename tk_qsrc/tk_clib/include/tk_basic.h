/*
TestKern Basic Interpreter
* Will also support G-Code.

Will focus more on limiting memory use than on performance.

Will store programs in a tokenized form, and implement a dialect similar to
older 8-bit era BASIC.

Tokens will be 16-bit, with the high 4 bits encoding a tag:
  0xxx: Command / Keyword
  1xxx: Symbol Reference
  2xxx: -
  3xxx: -
  4xxx: Small Positive Int (    0 .. 4095)
  5xxx: Small Negative Int (-4096 ..   -1)
  6xxx: Fractional Value (+/- 0.000 .. 2.000)
  7xxx: Fractional Value (+/- 00.00 .. 20.00)
  8xxx: -
  9xxx: -

Commands:
  0000 .. 01FF: Core / Basic
  0200 .. 03FF: Function Calls
  0400 .. 05FF: -
  0600 .. 07FF: -
  0800 .. 0BFF: G00.0 .. G99.9 (CNC)
  0C00 .. 0FFF: M00.0 .. M99.9 (CNC)

 */

#define 

struct TKBAS_Context_s {
u16 **line_buf;
int n_line;
int m_line;
};
