.section .text

__text_start:

__get_stdin:
  PUSH     R14
  ADD      #-152, R15
  MOV      __stdin, RQ14
  MOV      RQ14, RQ2

L00E003EC:
  ADD      #152, R15
  POP      RQ14
  RTS     

__get_stdout:
  PUSH     R14
  ADD      #-152, R15
  MOV      __stdout, RQ14
  MOV      RQ14, RQ2

L00E003ED:
  ADD      #152, R15
  POP      RQ14
  RTS     

__get_stderr:
  PUSH     R14
  ADD      #-152, R15
  MOV      __stderr, RQ14
  MOV      RQ14, RQ2

L00E003EE:
  ADD      #152, R15
  POP      RQ14
  RTS     

printf:
  PUSH     LR
  ADD      #-1528, R15
  MOV.Q    RQ4, @(R15, 232)
  MOV      #240, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 224)
  MOV      #1264, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 216)
  MOV.Q    RQ4, @RQ3
  MOV.Q    RQ5, @(RQ3, 8)
  MOV.Q    RQ6, @(RQ3, 16)
  MOV.Q    RQ7, @(RQ3, 24)
  MOV.Q    RQ20, @(RQ3, 32)
  MOV.Q    RQ21, @(RQ3, 40)
  MOV.Q    RQ22, @(RQ3, 48)
  MOV.Q    RQ23, @(RQ3, 56)
  FMOV.D   DR4, @(RQ3, 64)
  FMOV.D   DR5, @(RQ3, 72)
  FMOV.D   DR6, @(RQ3, 80)
  FMOV.D   DR7, @(RQ3, 88)
  MOV      #8, R0
  MOV.L    R0, @(R3, 96)
  MOV      #64, R0
  MOV.L    R0, @(R3, 104)
  MOV      #1536, R1
  ADD      R15, R1
  MOV.Q    R1, @(R3, 112)
  MOV      #240, RQ4
  ADD      R15, RQ4
  MOV.Q    @(R15, 232), RQ5
  MOV      #1264, RQ6
  ADD      R15, RQ6
  BSR      vsprintf
  MOV.L    RD2, @(R15, 212)
  MOV      #240, RQ4
  ADD      R15, RQ4
  BSR      tk_puts
  MOV.L    @(R15, 212), RD2

L00E003EF:
  ADD      #1528, R15
  RET     

L00C0000E:
  PUSH     R31
  PUSH     R30
  PUSH     R29
  PUSH     R28
  PUSH     R27
  PUSH     R26
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  PUSH     R9
  PUSH     R8
  RTS     

fopen:
  PUSH     LR
  BSR      L00C0000E
  ADD      #-80, R15
  MOV.Q    RQ4, @(R15, 40)
  MOV.Q    RQ5, @(R15, 32)
  MOV.Q    @(R15, 40), RQ13
  MOV      RQ13, RQ14
  MOV.Q    @(R15, 32), RQ11
  MOV      RQ11, RQ12
  MOV      #0, RD10
  MOV.Q    RQ14, TU3CD6E720/fnm
  MOV.Q    RQ12, TU3CD6E720/modus
  MOV.L    RD10, TU3CD6E720/err
  BSR      findSpareSpot
  MOV.L    TU3CD6E720/err, RD9
  CMP/EQ   RD9
  BT       L00800000
  MOV      #120, RQ8
  EXTS.L   RQ8, RQ8
  MOV.Q    RQ8, @(R15, 24)
  MOV      RQ8, RQ4
  BSR      malloc
  MOV      RQ2, RQ31
  MOV      #0, RQ30
  MOV      RQ31, RQ29
  MOV      RQ30, RQ28
  MOV.Q    RQ31, TU3CD6E720/myfile
  MOV.Q    RQ29, @(R15, 8)
  MOV.Q    RQ28, @R15
  CMPQ/EQ  RQ28, RQ29
  BT       L00800001
  MOV      L00C00015, RQ4
  BSR      tk_puts
  MOV      #1, RD27
  MOV.L    RD27, TU3CD6E720/err
  BRA      L00800002

L00800001:
  BSR      fopen2
  MOV.L    TU3CD6E720/err, RD26
  CMP/EQ   #0, RD26
  BT       L00800003
  MOV.Q    TU3CD6E720/myfile, RQ4
  BSR      free

L00800003:

L00800002:

L00800000:
  MOV.L    TU3CD6E720/err, RD7
  CMP/EQ   RD7
  BT       L00800004
  MOV      #0, RQ26
  MOV.Q    RQ26, TU3CD6E720/myfile

L00800004:
  MOV.Q    TU3CD6E720/myfile, RQ2

L00E003F0:
  ADD      #80, R15

L00C00018:
  POP      RQ8
  POP      RQ9
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  POP      RQ26
  POP      RQ27
  POP      RQ28
  POP      RQ29
  POP      RQ30
  POP      RQ31
  RET     

L00C0001A:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  RTS     

fclose:
  PUSH     LR
  BSR      L00C0001A
  ADD      #-128, R15
  MOV      RQ4, RQ14
  MOV      #0, RQ12
  CMPQ/EQ  RQ12, RQ14
  BT       L00800052
  MOV      #-1, RD2
  BRA      L00E003F1

L00800052:
  MOV.L    @(RQ14, 108), RD13
  CMP/EQ   RD13
  BT       L00800053
  MOV      #-1, RD2
  BRA      L00E003F1

L00800053:
  MOV      RQ14, RQ4
  BSR      fflush
  MOV.L    @RQ14, RD13
  MOV      RD13, RD4
  BSR      __close
  MOV      #0, RD2

L00E003F1:
  ADD      #128, R15

L00C0001D:
  POP      RQ12
  POP      RQ13
  POP      RQ14
  RET     

L00C0001F:
  PUSH     R31
  PUSH     R30
  PUSH     R29
  PUSH     R28
  PUSH     R27
  PUSH     R26
  PUSH     R25
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  PUSH     R9
  PUSH     R8
  RTS     

fread:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-152, R15
  MOV.Q    RQ4, @(R15, 120)
  MOV.Q    RQ5, @(R15, 112)
  MOV.Q    RQ6, @(R15, 104)
  MOV      RQ7, RQ12
  MOV      #0, RQ9
  CMPQ/EQ  RQ9, RQ12
  BT       L00800054
  MOV      #-1, RQ14
  EXTS.L   RQ14, RQ14
  MOV      RQ14, RQ2
  BRA      L00E003F2

L00800054:
  MOV.Q    @(R15, 104), RQ8
  CMPQ/EQ  #1, RQ8
  BT       L00800055
  MOV.Q    @(R15, 112), RQ13
  BRA      L00800056

L00800055:
  MOV.Q    @(R15, 112), RQ31
  CMPQ/EQ  #1, RQ31
  BT       L00800057
  MOV.Q    @(R15, 104), RQ13
  BRA      L00800058

L00800057:
  MOV.Q    @(R15, 112), RQ13
  MOV.Q    @(R15, 104), RQ5
  MOV      RQ13, RQ4
  MOV      __umulsq, R3
  JSR      R3
  MOV      RQ2, RQ13

L00800058:

L00800056:
  MOV.Q    @(RQ12, 40), RQ14
  MOV      RQ13, RQ30
  MOV      RQ14, RQ29
  MOV.Q    RQ30, @(R15, 48)
  MOV.Q    RQ29, @(R15, 40)
  CMPQ/GT  RQ30, RQ29
  BT       L00800059
  MOV      #0, RD28
  MOV.L    RD28, @(RQ12, 4)

L00800059:
  MOV      #1, RD27
  MOV.L    RD27, @(RQ12, 4)
  MOV.L    @(RQ12, 72), RD10
  CMP/EQ   #-1, RD10
  BT       L0080005A
  MOV.L    @(RQ12, 72), RD10
  EXTS.B   RD10, RD26
  MOV.Q    @(RQ12, 48), RQ11
  ADD      RQ11, #-1, RQ25
  MOV.Q    RQ25, @(RQ12, 48)
  MOV.B    RD26, @RQ25
  MOV      #-1, RD27
  MOV.L    RD27, @(RQ12, 72)
  MOV.L    RD26, @(R15, 32)
  MOV.Q    RQ25, @(R15, 16)

L0080005A:
  MOV.L    @(RQ12, 4), RD10
  CMP/EQ   RD10
  BT       L0080005B
  MOV.L    @(RQ12, 12), RD10
  CMP/EQ   RD10
  BT       L0080005C
  MOV      #80, RQ11
  ADD      R15, RQ11
  MOV.Q    @(R15, 120), RQ4
  MOV      RQ12, RQ5
  MOV      RQ13, RQ6
  MOV      RQ11, RQ7
  BSR      freadSlowT
  BRA      L0080005D

L0080005C:
  MOV.Q    @(RQ12, 56), RQ11
  MOV.Q    @(RQ12, 48), RQ7
  SUB      RQ11, RQ7, RD10
  MOV      RQ13, RQ14
  EXTS.L   RD10, RQ6
  MOV.Q    RQ7, @(R15, 16)
  MOV.Q    RQ6, @(R15, 48)
  CMPQ/HI  RQ6, RQ14
  BT       L0080005E
  MOV.Q    @(RQ12, 48), RQ11
  MOV.Q    @(R15, 120), RQ4
  MOV      RQ11, RQ5
  MOV      RQ13, RQ6
  BSR      memcpy
  MOV      RQ13, RQ5
  MOV.Q    RQ5, @(R15, 80)
  MOV.Q    @(RQ12, 48), RQ11
  LEA.B    @(RQ11, RQ13), RQ4
  MOV.Q    RQ4, @(RQ12, 48)
  MOV.Q    RQ4, @(R15, 16)
  BRA      L0080005F

L0080005E:
  MOV      #80, RQ11
  ADD      R15, RQ11
  MOV.Q    @(R15, 120), RQ4
  MOV      RQ12, RQ5
  MOV      RQ13, RQ6
  MOV      RQ11, RQ7
  BSR      freadSlowB

L0080005F:

L0080005D:
  MOV.Q    @(R15, 104), RQ3
  CMPQ/EQ  #1, RQ3
  BT       L00800060
  MOV.Q    @(R15, 80), RQ14
  MOV.Q    @(R15, 112), RQ2
  MOV.Q    RQ2, @(R15, 48)
  CMPQ/EQ  RQ2, RQ14
  BT       L00800061
  MOV      #1, RQ14
  EXTS.L   RQ14, RQ14
  MOV      RQ14, RQ23
  MOV.Q    RQ23, @(R15, 88)
  BRA      L00800062

L00800061:
  MOV      #0, RQ14
  EXTS.L   RQ14, RQ14
  MOV      RQ14, RQ22
  MOV.Q    RQ22, @(R15, 88)

L00800062:
  BRA      L00800063

L00800060:
  MOV.Q    @(R15, 112), RQ21
  CMPQ/EQ  #1, RQ21
  BT       L00800064
  MOV.Q    @(R15, 80), RQ20
  MOV.Q    RQ20, @(R15, 88)
  BRA      L00800065

L00800064:
  MOV.Q    @(R15, 112), RQ19
  MOV      #0, RQ18
  CMPQ/EQ  RQ18, RQ19
  BT       L00800066
  MOV      #0, RQ14
  EXTS.L   RQ14, RQ14
  MOV      RQ14, RQ17
  MOV.Q    RQ17, @(R15, 88)
  BRA      L00800067

L00800066:
  MOV.Q    @(R15, 80), RQ25
  MOV.Q    @(R15, 112), RQ5
  MOV      RQ25, RQ4
  BSR      __udivsq
  MOV      RQ2, RQ25
  MOV.Q    RQ25, @(R15, 88)

L00800067:

L00800065:

L00800063:
  MOV.Q    @(R15, 88), RQ2
  BRA      L00E003F2
  BRA      L00800068

L0080005B:
  MOV      #76, RQ11
  ADD      R15, RQ11
  MOV.L    @RQ12, RD10
  MOV      RD10, RD4
  MOV.Q    @(R15, 120), RQ5
  MOV      RQ13, RQ6
  MOV      RQ11, RQ7
  BSR      __read
  MOV      RD2, RD25
  EXTS.L   RD25, RQ26
  MOV.Q    RQ26, @(R15, 64)
  MOV.L    RD25, @(R15, 32)
  MOV.L    @(R15, 76), RD27
  MOV.L    @(R15, 76), RD28
  CMP/EQ   #0, RD28
  BT       L00800069
  BSR      __get_errno
  MOV      RQ2, RQ11
  MOV.L    @(R15, 64), RD10
  EXTS.L   RD10, RD10
  MOV.L    RD10, @RQ11
  MOV      #0, RQ14
  EXTS.L   RQ14, RQ14
  MOV      RQ14, RQ25
  MOV.Q    RQ25, @(R15, 80)
  MOV      #1, RD25
  MOV.L    RD25, @(RQ12, 64)
  BRA      L0080006A

L00800069:
  MOV.Q    @(R15, 64), RQ16
  MOV.Q    RQ16, @(R15, 80)

L0080006A:
  MOV.Q    @(R15, 104), RQ16
  CMPQ/EQ  #1, RQ16
  BT       L0080006B
  MOV.Q    @(R15, 80), RQ14
  MOV.Q    @(R15, 112), RQ16
  MOV.Q    RQ16, @(R15, 48)
  CMPQ/EQ  RQ16, RQ14
  BT       L0080006C
  MOV      #1, RQ14
  EXTS.L   RQ14, RQ14
  MOV      RQ14, RQ16
  MOV.Q    RQ16, @(R15, 88)
  BRA      L0080006D

L0080006C:
  MOV      #0, RQ14
  EXTS.L   RQ14, RQ14
  MOV      RQ14, RQ16
  MOV      #1, RD17
  MOV.L    RD17, @(RQ12, 68)
  MOV.Q    RQ16, @(R15, 88)

L0080006D:
  BRA      L0080006E

L0080006B:
  MOV.Q    @(R15, 112), RQ16
  CMPQ/EQ  #1, RQ16
  BT       L0080006F
  MOV.Q    @(R15, 80), RQ16
  MOV.Q    @(R15, 104), RQ14
  MOV.Q    @(R15, 80), RQ17
  MOV.Q    RQ17, @(R15, 48)
  MOV.Q    RQ16, @(R15, 88)
  CMPQ/EQ  RQ17, RQ14
  BT       L00800070
  MOV      #1, RD16
  MOV.L    RD16, @(RQ12, 68)

L00800070:
  BRA      L00800071

L0080006F:
  MOV.Q    @(R15, 112), RQ16
  MOV      #0, RQ17
  CMPQ/EQ  RQ17, RQ16
  BT       L00800072
  MOV      #0, RQ14
  EXTS.L   RQ14, RQ14
  MOV      RQ14, RQ16
  MOV.Q    RQ16, @(R15, 88)
  BRA      L00800073

L00800072:
  MOV.Q    @(R15, 80), RQ25
  MOV.Q    @(R15, 112), RQ5
  MOV      RQ25, RQ4
  BSR      __udivsq
  MOV      RQ2, RQ25
  MOV.Q    RQ25, @(R15, 88)

L00800073:
  MOV      RQ13, RQ14
  MOV.Q    @(R15, 80), RQ16
  MOV.Q    RQ16, @(R15, 48)
  CMPQ/EQ  RQ16, RQ14
  BT       L00800074
  MOV      #1, RD16
  MOV.L    RD16, @(RQ12, 68)

L00800074:

L00800071:

L0080006E:
  MOV.Q    @(RQ12, 24), RQ14
  MOV      RQ14, RQ16
  MOV.Q    @(R15, 80), RQ17
  ADD      RQ16, RQ17, RQ18
  MOV      RQ18, RQ14
  MOV.Q    RQ14, @(RQ12, 24)
  MOV.Q    RQ18, @(R15, 48)
  MOV.Q    RQ16, @(R15, 40)
  MOV.Q    @(R15, 88), RQ2
  BRA      L00E003F2

L00800068:

L00E003F2:
  ADD      #152, R15

L00C00027:
  POP      RQ8
  POP      RQ9
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  POP      RQ25
  POP      RQ26
  POP      RQ27
  POP      RQ28
  POP      RQ29
  POP      RQ30
  POP      RQ31
  RET     

fwrite:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-136, R15
  MOV.Q    RQ4, @(R15, 104)
  MOV.Q    RQ5, @(R15, 96)
  MOV.Q    RQ6, @(R15, 88)
  MOV      RQ7, RQ12
  MOV      #0, RQ9
  CMPQ/EQ  RQ9, RQ12
  BT       L0080008C
  MOV      #-1, RQ14
  EXTS.L   RQ14, RQ14
  MOV      RQ14, RQ2
  BRA      L00E003F3

L0080008C:
  MOV.Q    @(R15, 88), RQ8
  CMPQ/EQ  #1, RQ8
  BT       L0080008D
  MOV.Q    @(R15, 96), RQ13
  BRA      L0080008E

L0080008D:
  MOV.Q    @(R15, 96), RQ31
  CMPQ/EQ  #1, RQ31
  BT       L0080008F
  MOV.Q    @(R15, 88), RQ13
  BRA      L00800090

L0080008F:
  MOV.Q    @(R15, 96), RQ13
  MOV.Q    @(R15, 88), RQ5
  MOV      RQ13, RQ4
  BSR      __umulsq
  MOV      RQ2, RQ13

L00800090:

L0080008E:
  MOV.Q    @(RQ12, 40), RQ14
  MOV      RQ13, RQ30
  MOV      RQ14, RQ29
  MOV.Q    RQ30, @(R15, 40)
  MOV.Q    RQ29, @(R15, 32)
  CMPQ/GT  RQ30, RQ29
  BT       L00800091
  MOV      #0, RD28
  MOV.L    RD28, @(RQ12, 4)
  MOV.L    @(RQ12, 76), RD11
  CMP/EQ   #3, RD11
  BT       L00800092
  MOV.L    @(RQ12, 12), RD11
  CMP/EQ   RD11
  BT       L00800092
  BRA      L00800093

L00800093:
  MOV      #1, RD27
  MOV.L    RD27, @(RQ12, 4)

L00800092:

L00800091:
  MOV.L    @(RQ12, 4), RD11
  CMP/EQ   RD11
  BT       L00800094
  MOV      #72, RQ10
  ADD      R15, RQ10
  MOV.Q    @(R15, 104), RQ4
  MOV.Q    @(R15, 96), RQ5
  MOV.Q    @(R15, 88), RQ6
  MOV      RQ12, RQ7
  MOV      RQ13, RQ20
  MOV      RQ10, RQ21
  BSR      fwriteSlow
  MOV.Q    @(R15, 72), RQ2
  BRA      L00E003F3
  BRA      L00800095

L00800094:
  MOV      #60, RQ10
  ADD      R15, RQ10
  MOV.L    @RQ12, RD11
  MOV      RD11, RD4
  MOV.Q    @(R15, 104), RQ5
  MOV      RQ13, RQ6
  MOV      RQ10, RQ7
  BSR      __write
  MOV      RD2, RD26
  EXTS.L   RD26, RQ25
  MOV.L    RD26, @(R15, 12)
  MOV.Q    RQ25, @(R15, 64)
  MOV.L    @(R15, 60), RD27
  MOV.L    @(R15, 60), RD28
  CMP/EQ   #0, RD28
  BT       L00800096
  MOV      #1, RD25
  MOV.L    RD25, @(RQ12, 64)
  MOV      #0, RQ14
  EXTS.L   RQ14, RQ14
  MOV      RQ14, RQ26
  MOV.Q    RQ26, @(R15, 64)
  BSR      __get_errno
  MOV      RQ2, RQ10
  EXTS.L   RQ26, RD11
  MOV.L    RD11, @RQ10

L00800096:
  MOV.Q    @(R15, 88), RQ7
  CMPQ/EQ  #1, RQ7
  BT       L00800097
  MOV.Q    @(R15, 64), RQ14
  MOV.Q    @(R15, 96), RQ6
  MOV.Q    RQ6, @(R15, 40)
  CMPQ/EQ  RQ6, RQ14
  BT       L00800098
  MOV      #1, RQ14
  EXTS.L   RQ14, RQ14
  MOV      RQ14, RQ5
  MOV.Q    RQ5, @(R15, 72)
  BRA      L00800099

L00800098:
  MOV      #0, RQ14
  EXTS.L   RQ14, RQ14
  MOV      RQ14, RQ4
  MOV.Q    RQ4, @(R15, 72)

L00800099:
  BRA      L0080009A

L00800097:
  MOV.Q    @(R15, 96), RQ3
  CMPQ/EQ  #1, RQ3
  BT       L0080009B
  MOV.Q    @(R15, 64), RQ2
  MOV.Q    RQ2, @(R15, 72)
  BRA      L0080009C

L0080009B:
  MOV.Q    @(R15, 64), RQ25
  MOV.Q    RQ25, @(R15, 72)
  MOV.Q    @(R15, 96), RQ5
  MOV.Q    @(R15, 72), RQ4
  BSR      __udivsq
  MOV      RQ2, RQ25
  MOV.Q    RQ25, @(R15, 72)

L0080009C:

L0080009A:
  MOV.Q    @(RQ12, 24), RQ14
  MOV      RQ14, RQ23
  MOV.Q    @(R15, 64), RQ22
  ADD      RQ23, RQ22, RQ21
  MOV      RQ21, RQ14
  MOV.Q    RQ14, @(RQ12, 24)
  MOV.Q    RQ23, @(R15, 32)
  MOV.Q    RQ21, @(R15, 40)
  MOV.Q    @(R15, 72), RQ2
  BRA      L00E003F3

L00800095:

L00E003F3:
  ADD      #136, R15
  BRA      L00C00027

fputc:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-104, R15
  MOV.L    RD4, @(R15, 72)
  MOV      RQ5, RQ14
  MOV      #76, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 56)
  MOV      #0, RD9
  MOV.L    RD9, @(RQ14, 4)
  MOV.Q    @(RQ14, 48), RQ13
  MOV.Q    @(RQ14, 56), RQ10
  ADD      RQ10, #-2, RQ11
  CMPQ/GT  RQ13, RQ11
  BT       L00800161
  MOV.L    @(RQ14, 76), RD12
  CMP/EQ   #3, RD12
  BT       L00800161
  BRA      L00800162

L00800162:
  MOV.L    @(RQ14, 12), RD12
  CMP/EQ   RD12
  BT       L00800163
  MOV.L    @(R15, 72), RD8
  CMP/EQ   #10, RD8
  BT       L00800164
  MOV.L    @(RQ14, 76), RD12
  CMP/EQ   #1, RD12
  BT       L00800165
  MOV.Q    @(RQ14, 48), RQ13
  MOV      RQ13, RQ10
  ADD      RQ13, #1, RQ11
  MOV.Q    RQ11, @(RQ14, 48)
  MOV      #13, RD31
  MOV.B    RD31, @RQ10
  MOV.Q    @(RQ14, 48), RQ13
  MOV      RQ13, RQ10
  ADD      RQ13, #1, RQ11
  MOV.Q    RQ11, @(RQ14, 48)
  MOV      #10, RD30
  MOV.B    RD30, @RQ10
  BRA      L00800166

L00800165:
  MOV.L    @(R15, 72), RD12
  EXTS.B   RD12, RD12
  MOV      #76, RQ29
  ADD      R15, RQ29
  MOV.B    RD12, @RQ29
  MOV      #1, RQ28
  EXTS.L   RQ28, RQ28
  MOV      #1, RQ27
  EXTS.L   RQ27, RQ27
  MOV.Q    RQ28, @(R15, 8)
  MOV.Q    RQ27, @R15
  MOV      RQ29, RQ4
  MOV      RQ28, RQ5
  MOV      RQ27, RQ6
  MOV      RQ14, RQ7
  BSR      fwrite
  MOV      RQ2, RQ26
  MOV.Q    RQ26, @(R15, 16)
  CMPQ/EQ  #1, RQ26
  BT       L00800167
  MOV      #-1, RD2
  BRA      L00E003F4

L00800167:

L00800166:
  BRA      L00800168

L00800164:
  MOV.L    @(R15, 72), RD12
  EXTS.B   RD12, RD12
  MOV.Q    @(RQ14, 48), RQ13
  MOV      RQ13, RQ10
  ADD      RQ13, #1, RQ11
  MOV.Q    RQ11, @(RQ14, 48)
  MOV.B    RD12, @RQ10

L00800168:
  BRA      L00800169

L00800163:
  MOV.L    @(R15, 72), RD12
  EXTS.B   RD12, RD12
  MOV.Q    @(RQ14, 48), RQ13
  MOV      RQ13, RQ10
  ADD      RQ13, #1, RQ11
  MOV.Q    RQ11, @(RQ14, 48)
  MOV.B    RD12, @RQ10

L00800169:
  BRA      L0080016A

L00800161:
  MOV.L    @(R15, 72), RD12
  EXTS.B   RD12, RD12
  MOV      #76, RQ25
  ADD      R15, RQ25
  MOV.B    RD12, @RQ25
  MOV      #1, RQ26
  EXTS.L   RQ26, RQ26
  MOV      #1, RQ27
  EXTS.L   RQ27, RQ27
  MOV.Q    RQ27, @R15
  MOV.Q    RQ26, @(R15, 8)
  MOV      RQ25, RQ4
  MOV      RQ26, RQ5
  MOV      RQ27, RQ6
  MOV      RQ14, RQ7
  BSR      fwrite
  MOV      RQ2, RQ28
  MOV.Q    RQ28, @(R15, 16)
  CMPQ/EQ  #1, RQ28
  BT       L0080016B
  MOV      #-1, RD2
  BRA      L00E003F4

L0080016B:

L0080016A:
  MOV.L    @(R15, 72), RD2

L00E003F4:
  ADD      #104, R15
  BRA      L00C00027

fputs:
  PUSH     LR
  PUSH     R14
  PUSH     R13
  ADD      #-152, R15
  MOV.Q    RQ4, @(R15, 24)
  MOV.Q    RQ5, @(R15, 16)
  MOV.Q    @(R15, 24), RQ4
  BSR      strlen
  MOV      RQ2, RQ14
  MOV      #1, RD7
  EXTS.L   RD7, RD7
  MOV.L    RD7, @(R15, 8)
  MOV.Q    @(R15, 24), RQ4
  MOV      RQ14, RQ5
  MOV.Q    @(R15, 8), RQ6
  MOV.Q    @(R15, 16), RQ7
  BSR      fwrite
  MOV      RQ2, RQ13
  CMPQ/EQ  #1, RQ13
  BT       L0080016C
  MOV      #-1, RD2
  BRA      L00E003F5
  BRA      L0080016D

L0080016C:
  MOV      #0, RD2
  BRA      L00E003F5

L0080016D:

L00E003F5:
  ADD      #152, R15
  POP      RQ13
  POP      RQ14
  RET     

fprintf:
  PUSH     LR
  ADD      #-296, R15
  MOV.Q    RQ4, @(R15, 24)
  MOV.Q    RQ5, @(R15, 16)
  MOV      #32, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 8)
  MOV.Q    RQ4, @RQ3
  MOV.Q    RQ5, @(RQ3, 8)
  MOV.Q    RQ6, @(RQ3, 16)
  MOV.Q    RQ7, @(RQ3, 24)
  MOV.Q    RQ20, @(RQ3, 32)
  MOV.Q    RQ21, @(RQ3, 40)
  MOV.Q    RQ22, @(RQ3, 48)
  MOV.Q    RQ23, @(RQ3, 56)
  FMOV.D   DR4, @(RQ3, 64)
  FMOV.D   DR5, @(RQ3, 72)
  FMOV.D   DR6, @(RQ3, 80)
  FMOV.D   DR7, @(RQ3, 88)
  MOV      #16, R0
  MOV.L    R0, @(R3, 96)
  MOV      #64, R0
  MOV.L    R0, @(R3, 104)
  MOV      #304, R1
  ADD      R15, R1
  MOV.Q    R1, @(R3, 112)
  MOV.Q    @(R15, 24), RQ4
  MOV.Q    @(R15, 16), RQ5
  MOV      #32, RQ6
  ADD      R15, RQ6
  BSR      vfprintf
  MOV.L    RD2, @(R15, 4)
  MOV.L    @(R15, 4), RD2

L00E003F6:
  ADD      #296, R15
  RET     

vfprintf:
  PUSH     LR
  BSR      L00C0001A
  ADD      #-272, R15
  MOV.Q    RQ4, @(R15, 24)
  MOV.Q    RQ5, @(R15, 16)
  MOV.Q    RQ6, @(R15, 8)
  MOV      #0, RD13
  MOV.Q    @(R15, 24), RQ12
  MOV.L    RD13, @(RQ12, 8)
  MOV      #0, RQ13
  MOV.Q    RQ13, @R15
  MOV.Q    @(R15, 16), RQ4
  MOV.Q    @(R15, 8), RQ5
  MOV      RQ12, RQ6
  MOV      RQ13, RQ7
  BSR      vvprintf
  MOV      RD2, RD14
  MOV      RD14, RD2

L00E003F7:
  ADD      #272, R15
  BRA      L00C0001D

sprintf:
  PUSH     LR
  ADD      #-296, R15
  MOV.Q    RQ4, @(R15, 24)
  MOV.Q    RQ5, @(R15, 16)
  MOV      #32, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 8)
  MOV.Q    RQ4, @RQ3
  MOV.Q    RQ5, @(RQ3, 8)
  MOV.Q    RQ6, @(RQ3, 16)
  MOV.Q    RQ7, @(RQ3, 24)
  MOV.Q    RQ20, @(RQ3, 32)
  MOV.Q    RQ21, @(RQ3, 40)
  MOV.Q    RQ22, @(RQ3, 48)
  MOV.Q    RQ23, @(RQ3, 56)
  FMOV.D   DR4, @(RQ3, 64)
  FMOV.D   DR5, @(RQ3, 72)
  FMOV.D   DR6, @(RQ3, 80)
  FMOV.D   DR7, @(RQ3, 88)
  MOV      #16, R0
  MOV.L    R0, @(R3, 96)
  MOV      #64, R0
  MOV.L    R0, @(R3, 104)
  MOV      #304, R1
  ADD      R15, R1
  MOV.Q    R1, @(R3, 112)
  MOV.Q    @(R15, 24), RQ4
  MOV.Q    @(R15, 16), RQ5
  MOV      #32, RQ6
  ADD      R15, RQ6
  BSR      vsprintf
  MOV.L    RD2, @(R15, 4)
  MOV.L    @(R15, 4), RD2

L00E003F8:
  ADD      #296, R15
  RET     

L00C00032:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  RTS     

vsprintf:
  PUSH     LR
  BSR      L00C00032
  ADD      #-264, R15
  MOV.Q    RQ4, @(R15, 24)
  MOV.Q    RQ5, @(R15, 16)
  MOV.Q    RQ6, @(R15, 8)
  MOV      #0, RQ13
  MOV.Q    @(R15, 16), RQ4
  MOV.Q    @(R15, 8), RQ5
  MOV      RQ13, RQ6
  MOV.Q    @(R15, 24), RQ7
  BSR      vvprintf
  MOV      RD2, RD14
  CMP/PZ   RD14
  BT       L0080016E
  MOV.Q    @(R15, 24), RQ12
  EXTS.L   RD14
  LEA.B    @(RQ12, RD14), RQ13
  MOV      #0, RD11
  MOV.B    RD11, @RQ13

L0080016E:
  MOV      RD14, RD2

L00E003F9:
  ADD      #264, R15

L00C00033:
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  RET     

ungetc:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  ADD      #-144, R15
  MOV      RD4, RD12
  MOV      RQ5, RQ13
  MOV.L    @(RQ13, 72), RD14
  CMP/EQ   #-1, RD14
  BT       L008001B2
  CMP/EQ   #-1, RD12
  BT       L008001B2
  BRA      L008001B3

L008001B2:
  MOV      #-1, RD2
  BRA      L00E003FA

L008001B3:
  EXTU.B   RD12, RD14
  MOV.L    RD14, @(RQ13, 72)
  MOV      #0, RD11
  MOV.L    RD11, @(RQ13, 8)
  MOV.L    RD11, @(RQ13, 4)
  EXTU.B   RD12, RD14
  MOV      RD14, RD2

L00E003FA:
  ADD      #144, R15

L00C00035:
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  RTS     

L00C00037:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  PUSH     R9
  RTS     

fgetc:
  PUSH     LR
  BSR      L00C00037
  ADD      #-136, R15
  MOV.Q    RQ4, @(R15, 32)
  MOV      #44, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 24)
  MOV      #1, RQ13
  EXTS.L   RQ13, RQ13
  MOV      #1, RQ12
  EXTS.L   RQ12, RQ12
  MOV.Q    RQ13, @(R15, 16)
  MOV.Q    RQ12, @(R15, 8)
  MOV      #44, RQ4
  ADD      R15, RQ4
  MOV      RQ13, RQ5
  MOV      RQ12, RQ6
  MOV.Q    @(R15, 32), RQ7
  BSR      fread
  MOV      RQ2, RQ14
  MOV      #0, RQ11
  CMPQ/EQ  RQ11, RQ14
  BT       L008001B4
  MOV      #-1, RD2
  BRA      L00E003FB

L008001B4:
  MOV      #44, RQ10
  ADD      R15, RQ10
  MOVU.B   @RQ10, RD9
  MOV.L    RD9, @(R15, 4)
  MOV      RD9, RD2

L00E003FB:
  ADD      #136, R15

L00C00038:
  POP      RQ9
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  RET     

fseek:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-120, R15
  MOV      RQ4, RQ12
  MOV.Q    RQ5, @(R15, 80)
  MOV.L    RD6, @(R15, 76)
  MOV.Q    @(RQ12, 24), RQ14
  MOV.Q    @(RQ12, 48), RQ11
  MOV.Q    @(RQ12, 32), RQ9
  SUB      RQ11, RQ9, RD13
  MOV      RQ14, RQ8
  EXTS.L   RD13, RQ14
  ADD      RQ8, RQ14, RQ31
  MOV.L    @(RQ12, 92), RD13
  MOV.Q    RQ31, @(R15, 64)
  CMP/EQ   #1, RD13
  BT       L008001B5
  MOV      RQ12, RQ4
  BSR      fflush

L008001B5:
  MOV.L    @(R15, 76), RD30
  CMP/EQ   #0, RD30
  BT       L008001B6
  MOV.Q    @(R15, 80), RQ10
  BRA      L008001B7

L008001B6:
  MOV.L    @(R15, 76), RD29
  CMP/EQ   #1, RD29
  BT       L008001B8
  MOV.Q    @(R15, 64), RQ28
  MOV.Q    @(R15, 80), RQ27
  ADD      RQ28, RQ27, RQ10

L008001B8:

L008001B7:
  MOV.L    @(R15, 76), RD26
  CMP/EQ   #2, RD26
  BT       L008001B9
  MOV.L    @RQ12, RD13
  MOV      RD13, RD4
  MOV.Q    @(R15, 80), RQ5
  MOV      #2, RD6
  BSR      __seek
  MOV.L    @RQ12, RD13
  MOV      RD13, RD4
  BSR      __tell
  MOV      RQ2, RQ10
  MOV.Q    @(RQ12, 32), RQ11
  MOV.Q    @(RQ12, 40), RQ14
  LEA.B    @(RQ11, RQ14), RQ9
  MOV.Q    RQ9, @(RQ12, 56)
  MOV.Q    @(RQ12, 56), RQ11
  MOV.Q    RQ11, @(RQ12, 48)
  MOV.Q    @(RQ12, 40), RQ14
  MOV      RQ10, RQ25
  MOV      RQ14, RQ26
  SUB      RQ25, RQ26, RQ8
  MOV      RQ8, RQ14
  MOV.Q    RQ14, @(RQ12, 24)
  MOV.Q    RQ26, @(R15, 8)
  MOV.Q    RQ25, @(R15, 16)
  BRA      L008001BA

L008001B9:
  MOV.L    @RQ12, RD13
  MOV      RD13, RD4
  MOV      RQ10, RQ5
  MOV.L    @(R15, 76), RD6
  BSR      __seek
  MOV.Q    @(RQ12, 32), RQ11
  MOV.Q    @(RQ12, 40), RQ14
  LEA.B    @(RQ11, RQ14), RQ9
  MOV.Q    RQ9, @(RQ12, 56)
  MOV.Q    @(RQ12, 56), RQ11
  MOV.Q    RQ11, @(RQ12, 48)
  MOV.Q    @(RQ12, 40), RQ14
  MOV      RQ10, RQ7
  MOV      RQ14, RQ6
  SUB      RQ7, RQ6, RQ8
  MOV      RQ8, RQ14
  MOV.Q    RQ14, @(RQ12, 24)
  MOV.Q    RQ7, @(R15, 16)
  MOV.Q    RQ6, @(R15, 8)

L008001BA:
  MOV      #0, RD5
  MOV.L    RD5, @(RQ12, 4)
  MOV.L    RD5, @(RQ12, 8)
  MOV      #-1, RD4
  MOV.L    RD4, @(RQ12, 72)
  MOV      RD5, RD2

L00E003FC:
  ADD      #120, R15
  BRA      L00C00027

L00C0003D:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  PUSH     R9
  PUSH     R8
  RTS     

ftell:
  PUSH     LR
  BSR      L00C0003D
  ADD      #-144, R15
  MOV      RQ4, RQ13
  MOV.Q    @(RQ13, 24), RQ14
  MOV.Q    @(RQ13, 48), RQ12
  MOV.Q    @(RQ13, 32), RQ11
  SUB      RQ12, RQ11, RD10
  EXTS.L   RD10, RQ9
  ADD      RQ14, RQ9, RQ8
  MOV      RQ8, RQ14
  MOV      RQ14, RQ2

L00E003FD:
  ADD      #144, R15

L00C0003E:
  POP      RQ8
  POP      RQ9
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  RET     

setvbuf:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-104, R15
  MOV.Q    RQ4, @(R15, 72)
  MOV.Q    RQ5, @(R15, 64)
  MOV.L    RD6, @(R15, 60)
  MOV.Q    RQ7, @(R15, 48)
  MOV.L    @(R15, 60), RD10
  CMP/EQ   #3, RD10
  BT       L008001BE
  MOV.L    @(R15, 60), RD9
  MOV.Q    @(R15, 72), RQ8
  MOV.L    RD9, @(RQ8, 76)
  MOV      #0, RD2
  BRA      L00E003FE

L008001BE:
  MOV      #0, RQ14
  MOV.Q    @(R15, 64), RQ31
  MOV      RQ31, RQ13
  MOV      RQ14, RQ12
  CMPQ/EQ  RQ12, RQ13
  BT       L008001BF
  MOV.Q    @(R15, 48), RQ30
  MOV      #2, RD29
  CMPQ/HS  RD29, RQ30
  BT       L008001C0
  MOV      #-1, RD2
  BRA      L00E003FE

L008001C0:
  MOV.Q    @(R15, 48), RQ28
  ADD      RQ28, #8, RQ27
  MOV.Q    RQ27, @(R15, 16)
  MOV      RQ27, RQ4
  BSR      malloc
  MOV      RQ2, RQ11
  MOV      #0, RQ14
  MOV      RQ11, RQ13
  MOV      RQ14, RQ12
  CMPQ/EQ  RQ12, RQ13
  BT       L008001C1
  MOV      #-1, RD2
  BRA      L00E003FE

L008001C1:
  BRA      L008001C2

L008001BF:
  MOV.Q    @(R15, 48), RQ26
  MOV      #10, RD25
  CMPQ/HS  RD25, RQ26
  BT       L008001C3
  MOV      #-1, RD2
  BRA      L00E003FE

L008001C3:
  MOV.Q    @(R15, 64), RQ7
  MOV      RQ7, RQ11
  MOV      #1, RD6
  MOV.Q    @(R15, 72), RQ5
  MOV.L    RD6, @(RQ5, 100)
  MOV.Q    @(R15, 48), RQ4
  SUB      #8, RQ4
  MOV.Q    RQ4, @(R15, 48)

L008001C2:
  MOV.Q    @(R15, 72), RQ3
  MOV.L    @(RQ3, 104), RD2
  MOV.L    RD2, @(R15, 12)
  CMP/EQ   RD2
  BT       L008001C4
  MOV.Q    @(R15, 72), RQ25
  MOV.Q    @(RQ25, 80), RQ14
  MOV      RQ14, RQ4
  BSR      free

L008001C4:
  MOV.Q    @(R15, 72), RQ23
  MOV.Q    RQ11, @(RQ23, 80)
  MOV.Q    @(RQ23, 80), RQ14
  ADD      RQ14, #2, RQ13
  MOV.Q    RQ13, @(RQ23, 32)
  MOV.Q    @(RQ23, 32), RQ14
  MOV      RQ14, RQ13
  ADD      RQ14, #1, RQ12
  MOV.Q    RQ12, @(RQ23, 32)
  MOV      #0, RD22
  MOV.B    RD22, @RQ13
  MOV.Q    @(RQ23, 32), RQ14
  MOV      RQ14, RQ13
  ADD      RQ14, #1, RQ12
  MOV.Q    RQ12, @(RQ23, 32)
  MOV.B    RD22, @RQ13
  MOV.Q    @(R15, 48), RQ21
  MOV.Q    RQ21, @(RQ23, 40)
  MOV.Q    @(RQ23, 32), RQ14
  MOV.Q    @(RQ23, 40), RQ20
  LEA.B    @(RQ14, RQ20), RQ13
  MOV.Q    RQ13, @(RQ23, 56)
  MOV.Q    @(RQ23, 56), RQ14
  MOV      #10, RD19
  MOV.B    RD19, @RQ14
  MOV.L    @(RQ23, 92), RD18
  MOV.Q    RQ20, @(R15, 16)
  MOV.L    RD18, @(R15, 12)
  CMP/EQ   #1, RD18
  BT       L008001C5
  MOV.Q    @(R15, 72), RQ17
  MOV.Q    @(RQ17, 32), RQ14
  MOV.Q    RQ14, @(RQ17, 48)
  BRA      L008001C6

L008001C5:
  MOV.Q    @(R15, 72), RQ16
  MOV.Q    @(RQ16, 56), RQ14
  MOV.Q    RQ14, @(RQ16, 48)

L008001C6:
  MOV.L    @(R15, 60), RD16
  MOV.Q    @(R15, 72), RQ17
  MOV.L    RD16, @(RQ17, 76)
  MOV.L    @(RQ17, 12), RD18
  MOV.L    RD18, @(R15, 12)
  CMP/EQ   #0, RD18
  BT       L008001C7
  MOV.Q    @(R15, 72), RQ16
  MOV.L    @(RQ16, 76), RD17
  MOV.L    RD17, @(R15, 12)
  CMP/EQ   #2, RD17
  BT       L008001C7
  BRA      L008001C8

L008001C8:
  MOV      #0, RD16
  MOV.Q    @(R15, 72), RQ17
  MOV.L    RD16, @(RQ17, 4)

L008001C7:
  MOV      #0, RD2

L00E003FE:
  ADD      #104, R15
  BRA      L00C00027

setbuf:
  PUSH     LR
  BSR      L00C00037
  ADD      #-136, R15
  MOV.Q    RQ4, @(R15, 40)
  MOV.Q    RQ5, @(R15, 32)
  MOV      #0, RQ13
  MOV.Q    @(R15, 32), RQ9
  MOV      RQ9, RQ11
  MOV      RQ13, RQ10
  CMPQ/EQ  RQ10, RQ11
  BT       L008001C9
  MOV      #0, RQ13
  MOV      #0, RQ12
  EXTS.L   RQ12, RQ12
  MOV.Q    @(R15, 40), RQ4
  MOV      RQ13, RQ5
  MOV      #3, RD6
  MOV      RQ12, RQ7
  BSR      setvbuf
  MOV      RD2, RD14
  BRA      L008001CA

L008001C9:
  MOV      #6144, RQ12
  EXTS.L   RQ12, RQ12
  MOV.Q    @(R15, 40), RQ4
  MOV.Q    @(R15, 32), RQ5
  MOV      #1, RD6
  MOV      RQ12, RQ7
  BSR      setvbuf
  MOV      RD2, RD14

L008001CA:
  MOV      RD14, RD2

L00E003FF:
  ADD      #136, R15
  BRA      L00C00038

fflush:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-120, R15
  MOV      RQ4, RQ13
  MOV.Q    @(RQ13, 48), RQ14
  MOV.Q    @(RQ13, 32), RQ11
  CMPQ/EQ  RQ11, RQ14
  BT       L008001CE
  MOV.L    @(RQ13, 92), RD12
  CMP/EQ   #1, RD12
  BT       L008001CE
  BRA      L008001CF

L008001CF:
  MOV      #84, RQ14
  ADD      R15, RQ14
  MOV.Q    @(RQ13, 48), RQ11
  MOV.Q    @(RQ13, 32), RQ31
  SUB      RQ11, RQ31, RD12
  EXTS.L   RD12, RQ10
  MOV.Q    @(RQ13, 32), RQ11
  MOV.L    @RQ13, RD12
  MOV      RD12, RD4
  MOV      RQ11, RQ5
  MOV      RQ10, RQ6
  MOV      RQ14, RQ7
  BSR      __write
  MOV      RD2, RD9
  EXTS.L   RD9, RQ8
  MOV.L    @(R15, 84), RD30
  MOV.L    @(R15, 84), RD29
  CMP/EQ   #0, RD29
  BT       L008001D0
  MOV      #1, RD28
  MOV.L    RD28, @(RQ13, 64)
  BSR      __get_errno
  MOV      RQ2, RQ14
  EXTS.L   RQ8, RD12
  MOV.L    RD12, @RQ14
  MOV      #-1, RD2
  BRA      L00E00400

L008001D0:
  MOV.Q    @(RQ13, 24), RQ10
  MOV      RQ10, RQ27
  MOV      RQ8, RQ26
  ADD      RQ27, RQ26, RQ25
  MOV      RQ25, RQ10
  MOV.Q    RQ10, @(RQ13, 24)
  MOV.Q    @(RQ13, 32), RQ14
  MOV.Q    RQ14, @(RQ13, 48)

L008001CE:
  MOV      #0, RD2

L00E00400:
  ADD      #120, R15
  BRA      L00C00027

fscanf:
  PUSH     LR
  ADD      #-312, R15
  MOV.Q    RQ4, @(R15, 40)
  MOV.Q    RQ5, @(R15, 32)
  MOV      #48, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 24)
  MOV.Q    RQ4, @RQ3
  MOV.Q    RQ5, @(RQ3, 8)
  MOV.Q    RQ6, @(RQ3, 16)
  MOV.Q    RQ7, @(RQ3, 24)
  MOV.Q    RQ20, @(RQ3, 32)
  MOV.Q    RQ21, @(RQ3, 40)
  MOV.Q    RQ22, @(RQ3, 48)
  MOV.Q    RQ23, @(RQ3, 56)
  FMOV.D   DR4, @(RQ3, 64)
  FMOV.D   DR5, @(RQ3, 72)
  FMOV.D   DR6, @(RQ3, 80)
  FMOV.D   DR7, @(RQ3, 88)
  MOV      #16, R0
  MOV.L    R0, @(R3, 96)
  MOV      #64, R0
  MOV.L    R0, @(R3, 104)
  MOV      #320, R1
  ADD      R15, R1
  MOV.Q    R1, @(R3, 112)
  MOV      #0, RQ7
  MOV.Q    RQ7, @(R15, 8)
  MOV.Q    @(R15, 32), RQ4
  MOV      #48, RQ5
  ADD      R15, RQ5
  MOV.Q    @(R15, 40), RQ6
  MOV.Q    @(R15, 8), RQ7
  BSR      vvscanf
  MOV.L    RD2, @(R15, 20)
  MOV.L    @(R15, 20), RD2

L00E00401:
  ADD      #312, R15
  RET     

sscanf:
  PUSH     LR
  ADD      #-312, R15
  MOV.Q    RQ4, @(R15, 40)
  MOV.Q    RQ5, @(R15, 32)
  MOV      #48, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 24)
  MOV.Q    RQ4, @RQ3
  MOV.Q    RQ5, @(RQ3, 8)
  MOV.Q    RQ6, @(RQ3, 16)
  MOV.Q    RQ7, @(RQ3, 24)
  MOV.Q    RQ20, @(RQ3, 32)
  MOV.Q    RQ21, @(RQ3, 40)
  MOV.Q    RQ22, @(RQ3, 48)
  MOV.Q    RQ23, @(RQ3, 56)
  FMOV.D   DR4, @(RQ3, 64)
  FMOV.D   DR5, @(RQ3, 72)
  FMOV.D   DR6, @(RQ3, 80)
  FMOV.D   DR7, @(RQ3, 88)
  MOV      #16, R0
  MOV.L    R0, @(R3, 96)
  MOV      #64, R0
  MOV.L    R0, @(R3, 104)
  MOV      #320, R1
  ADD      R15, R1
  MOV.Q    R1, @(R3, 112)
  MOV      #0, RQ7
  MOV.Q    RQ7, @(R15, 8)
  MOV.Q    @(R15, 32), RQ4
  MOV      #48, RQ5
  ADD      R15, RQ5
  MOV.Q    @(R15, 8), RQ6
  MOV.Q    @(R15, 40), RQ7
  BSR      vvscanf
  MOV.L    RD2, @(R15, 20)
  MOV.L    @(R15, 20), RD2

L00E00402:
  ADD      #312, R15
  RET     

L00C00044:
  PUSH     R31
  PUSH     R30
  PUSH     R29
  PUSH     R28
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  PUSH     R9
  PUSH     R8
  RTS     

memcpy:
  PUSH     LR
  BSR      L00C00044
  ADD      #-96, R15
  MOV      RQ4, RQ8
  MOV.Q    RQ5, @(R15, 32)
  MOV.Q    RQ6, @(R15, 24)
  MOV      RQ8, RQ14
  MOV.Q    @(R15, 32), RQ31
  MOV      RQ31, RQ13
  MOV.Q    @(R15, 24), RQ30
  AND      RQ30, #-4, RQ9
  LEA.B    @(RQ14, RQ9), RQ11
  MOV      RQ11, RQ12

L00800387:
  CMPQ/EQ  RQ12, RQ14
  BT       L00800388
  MOV      RQ13, RQ11
  ADD      #4, RQ13
  MOVU.L   @RQ11, RD10
  MOV      RQ14, RQ11
  ADD      #4, RQ14
  MOVU.L   RD10, @RQ11
  BRA      L00800387

L00800388:
  MOV.Q    @(R15, 24), RQ29
  AND      RQ29, #3, RQ9
  MOV      #0, RQ28
  CMPQ/EQ  RQ28, RQ9
  BT       L00800389
  CMPQ/EQ  #1, RQ9
  BT       L0080038A
  CMPQ/EQ  #2, RQ9
  BT       L0080038B
  CMPQ/EQ  #3, RQ9
  BT       L0080038C
  BRA      L0080038D

L00800389:
  BRA      L0080038D

L0080038A:
  MOV.B    @RQ13, RD10
  MOV.B    RD10, @RQ14
  BRA      L0080038D

L0080038B:
  MOV.B    @RQ13, RD10
  MOV.B    RD10, @RQ14
  ADD      RQ14, #1, RQ11
  MOV      RQ11, RQ14
  ADD      RQ13, #1, RQ11
  MOV      RQ11, RQ13
  MOV.B    @RQ13, RD10
  MOV.B    RD10, @RQ14
  BRA      L0080038D

L0080038C:
  MOV.B    @RQ13, RD10
  MOV.B    RD10, @RQ14
  ADD      RQ14, #1, RQ11
  MOV      RQ11, RQ14
  ADD      RQ13, #1, RQ11
  MOV      RQ11, RQ13
  MOV.B    @RQ13, RD10
  MOV.B    RD10, @RQ14
  ADD      RQ14, #1, RQ11
  MOV      RQ11, RQ14
  ADD      RQ13, #1, RQ11
  MOV      RQ11, RQ13
  MOV.B    @RQ13, RD10
  MOV.B    RD10, @RQ14
  BRA      L0080038D

L0080038D:
  MOV      RQ8, RQ2

L00E00403:
  ADD      #96, R15

L00C00045:
  POP      RQ8
  POP      RQ9
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  POP      RQ28
  POP      RQ29
  POP      RQ30
  POP      RQ31
  RET     

strcpy:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  ADD      #-136, R15
  MOV      RQ4, RQ12
  MOV      RQ5, RQ11
  MOV      RQ12, RQ14

L00800324:
  MOV      RQ11, RQ13
  ADD      #1, RQ11
  MOV.B    @RQ13, RD10
  MOV      RQ14, RQ13
  ADD      #1, RQ14
  MOV.B    RD10, @RQ13
  CMP/EQ   RD10
  BT       L00800325
  BRA      L00800324

L00800325:
  MOV      RQ12, RQ2

L00E00404:
  ADD      #136, R15

L00C00047:
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  RTS     

L00C00049:
  PUSH     R31
  PUSH     R30
  PUSH     R29
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  PUSH     R9
  PUSH     R8
  RTS     

strncpy:
  PUSH     LR
  BSR      L00C00049
  ADD      #-120, R15
  MOV.Q    RQ4, @(R15, 56)
  MOV      RQ5, RQ10
  MOV.Q    RQ6, @(R15, 40)
  MOV.Q    @(R15, 56), RQ9
  MOV      RQ9, RQ14
  MOV      #0, RQ12
  EXTS.L   RQ12, RQ12
  MOV      RQ12, RQ13

L00800326:
  MOV      RQ13, RQ12
  MOV.Q    @(R15, 40), RQ8
  MOV.Q    RQ8, @(R15, 24)
  CMPQ/GT  RQ12, RQ8
  BT       L00800327
  MOV.B    @RQ10, RD11
  MOV.B    RD11, @RQ14
  MOV.B    @RQ10, RD11
  CMP/EQ   RD11
  BT       L00800328
  BRA      L00800327

L00800328:
  ADD      #1, RQ14
  ADD      #1, RQ10

L00800329:
  ADD      #1, RQ13
  BRA      L00800326

L00800327:

L0080032A:
  MOV      RQ13, RQ12
  MOV.Q    @(R15, 40), RQ31
  MOV.Q    RQ31, @(R15, 24)
  CMPQ/GT  RQ12, RQ31
  BT       L0080032B
  MOV      RQ14, RQ30
  ADD      #1, RQ14
  MOV      #0, RD29
  MOV.B    RD29, @RQ30

L0080032C:
  ADD      #1, RQ13
  BRA      L0080032A

L0080032B:
  MOV.Q    @(R15, 56), RQ2

L00E00405:
  ADD      #120, R15

L00C0004A:
  POP      RQ8
  POP      RQ9
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  POP      RQ29
  POP      RQ30
  POP      RQ31
  RET     

strcat:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  ADD      #-144, R15
  MOV      RQ4, RQ11
  MOV      RQ5, RQ12
  MOV      RQ11, RQ14

L0080032D:
  MOV.B    @RQ14, RD13
  CMP/EQ   RD13
  BT       L0080032E
  ADD      #1, RQ14
  BRA      L0080032D

L0080032E:

L0080032F:
  MOV.B    @RQ12, RD13
  MOV.B    RD13, @RQ14
  CMP/EQ   RD13
  BT       L00800330
  ADD      #1, RQ14
  ADD      #1, RQ12
  BRA      L0080032F

L00800330:
  MOV      RQ11, RQ2

L00E00406:
  ADD      #144, R15
  BRA      L00C00035

strcmp:
  PUSH     LR
  BSR      L00C00037
  ADD      #-120, R15
  MOV      RQ4, RQ10
  MOV      RQ5, RQ9
  MOV      RQ10, RQ13
  MOV      RQ9, RQ12

L0080033B:
  MOVU.B   @RQ13, RD14
  CMP/EQ   RD14
  BT       L0080033C
  MOVU.B   @RQ13, RD14
  MOVU.B   @RQ12, RD11
  CMP/GT   RD14, RD11
  BT       L0080033D
  MOV      #-1, RD2
  BRA      L00E00407
  BRA      L0080033E

L0080033D:
  MOVU.B   @RQ13, RD14
  MOVU.B   @RQ12, RD11
  CMP/GT   RD11, RD14
  BT       L0080033F
  MOV      #1, RD2
  BRA      L00E00407

L0080033F:

L0080033E:
  ADD      #1, RQ13
  ADD      #1, RQ12
  BRA      L0080033B

L0080033C:
  MOVU.B   @RQ12, RD14
  CMP/EQ   RD14
  BT       L00800340
  MOV      #0, RD2
  BRA      L00E00407
  BRA      L00800341

L00800340:
  MOV      #-1, RD2
  BRA      L00E00407

L00800341:

L00E00407:
  ADD      #120, R15
  BRA      L00C00038

L00C0004E:
  PUSH     R31
  PUSH     R30
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  PUSH     R9
  PUSH     R8
  RTS     

strncmp:
  PUSH     LR
  BSR      L00C0004E
  ADD      #-112, R15
  MOV.Q    RQ4, @(R15, 40)
  MOV.Q    RQ5, @(R15, 32)
  MOV.Q    RQ6, @(R15, 24)
  MOV      #0, RQ12
  EXTS.L   RQ12, RQ12
  MOV      RQ12, RQ14
  MOV.Q    @(R15, 40), RQ8
  MOV      RQ8, RQ10
  MOV.Q    @(R15, 32), RQ31
  MOV      RQ31, RQ9

L00800342:
  MOV      RQ14, RQ12
  MOV.Q    @(R15, 24), RQ30
  MOV.Q    RQ30, @(R15, 8)
  CMPQ/GT  RQ12, RQ30
  BT       L00800343
  MOVU.B   @(RQ10, RQ14), RD13
  MOVU.B   @(RQ9, RQ14), RD11
  CMP/GT   RD13, RD11
  BT       L00800344
  MOV      #-1, RD2
  BRA      L00E00408
  BRA      L00800345

L00800344:
  MOVU.B   @(RQ10, RQ14), RD13
  MOVU.B   @(RQ9, RQ14), RD11
  CMP/GT   RD11, RD13
  BT       L00800346
  MOV      #1, RD2
  BRA      L00E00408
  BRA      L00800347

L00800346:
  MOVU.B   @(RQ10, RQ14), RD13
  CMP/EQ   RD13
  BT       L00800348
  MOV      #0, RD2
  BRA      L00E00408

L00800348:

L00800347:

L00800345:
  ADD      #1, RQ14
  BRA      L00800342

L00800343:
  MOV      #0, RD2

L00E00408:
  ADD      #112, R15

L00C0004F:
  POP      RQ8
  POP      RQ9
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  POP      RQ30
  POP      RQ31
  RET     

memchr:
  PUSH     LR
  BSR      L00C0004E
  ADD      #-128, R15
  MOV.Q    RQ4, @(R15, 56)
  MOV.L    RD5, @(R15, 52)
  MOV.Q    RQ6, @(R15, 40)
  MOV      #0, RQ13
  EXTS.L   RQ13, RQ13
  MOV      RQ13, RQ12
  MOV.Q    @(R15, 56), RQ11
  MOV      RQ11, RQ14

L0080034A:
  MOV      RQ12, RQ13
  MOV.Q    @(R15, 40), RQ10
  MOV.Q    RQ10, @(R15, 24)
  CMPQ/GT  RQ13, RQ10
  BT       L0080034B
  MOVU.B   @RQ14, RD9
  MOV.L    @(R15, 52), RD8
  EXTU.B   RD8, RD8
  MOV      RD9, RD31
  EXTU.B   RD8, RD30
  MOV.L    RD31, @(R15, 12)
  MOV.L    RD30, @(R15, 8)
  CMP/EQ   RD30, RD31
  BT       L0080034C
  MOV      RQ14, RQ2
  BRA      L00E00409

L0080034C:
  ADD      #1, RQ14
  ADD      #1, RQ12
  BRA      L0080034A

L0080034B:
  MOV      #0, RQ30
  MOV.Q    RQ30, @R15
  MOV      RQ30, RQ2

L00E00409:
  ADD      #128, R15
  BRA      L00C0004F

strchr:
  PUSH     LR
  BSR      L00C0003D
  ADD      #-128, R15
  MOV      RQ4, RQ13
  MOV      RD5, RD9

L0080034D:
  MOV.B    @RQ13, RD14
  CMP/EQ   RD14
  BT       L0080034E
  MOV.B    @RQ13, RD14
  EXTS.B   RD9, RD12
  MOV      RD14, RD11
  EXTS.B   RD12, RD10
  CMP/EQ   RD10, RD11
  BT       L0080034F
  MOV      RQ13, RQ2
  BRA      L00E0040A

L0080034F:
  ADD      #1, RQ13
  BRA      L0080034D

L0080034E:
  CMP/EQ   RD9
  BT       L00800350
  MOV      RQ13, RQ2
  BRA      L00E0040A

L00800350:
  MOV      #0, RQ8
  MOV      RQ8, RQ2

L00E0040A:
  ADD      #128, R15
  BRA      L00C0003E

L00C00053:
  PUSH     R31
  PUSH     R30
  PUSH     R29
  PUSH     R28
  PUSH     R27
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  PUSH     R9
  PUSH     R8
  RTS     

memset:
  PUSH     LR
  BSR      L00C00053
  ADD      #-88, R15
  MOV      RQ4, RQ11
  MOV.L    RD5, @(R15, 36)
  MOV.Q    RQ6, @(R15, 24)
  MOV.L    @(R15, 36), RD31
  MOV      RD31, RD13
  SHAD     RD13, #8, RD12
  OR       RD12, RD13
  SHAD     RD13, #16, RD12
  OR       RD12, RD13
  MOV      RQ11, RQ14
  MOV.Q    @(R15, 24), RQ30
  LEA.B    @(RQ11, RQ30), RQ8
  AND      RQ30, #-16, RQ10
  LEA.B    @(RQ11, RQ10), RQ9

L0080036F:
  CMPQ/GT  RQ14, RQ9
  BT       L00800370
  MOV.L    RD13, @RQ14
  MOV.L    RD13, @(RQ14, 4)
  MOV.L    RD13, @(RQ14, 8)
  MOV.L    RD13, @(RQ14, 12)
  ADD      #16, RQ14
  BRA      L0080036F

L00800370:
  MOV.Q    @(R15, 24), RQ29
  AND      RQ29, #-4, RQ10
  LEA.B    @(RQ11, RQ10), RQ9

L00800371:
  CMPQ/GT  RQ14, RQ9
  BT       L00800372
  MOV.L    RD13, @RQ14
  ADD      #4, RQ14
  BRA      L00800371

L00800372:

L00800373:
  CMPQ/GT  RQ14, RQ8
  BT       L00800374
  MOV      RQ14, RQ28
  ADD      #1, RQ14
  MOV.L    @(R15, 36), RD27
  MOVU.B   RD27, @RQ28
  BRA      L00800373

L00800374:
  MOV      RQ11, RQ2

L00E0040B:
  ADD      #88, R15

L00C00054:
  POP      RQ8
  POP      RQ9
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  POP      RQ27
  POP      RQ28
  POP      RQ29
  POP      RQ30
  POP      RQ31
  RET     

strlen:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  ADD      #-136, R15
  MOV      RQ4, RQ10
  MOV      RQ10, RQ14
  MOV      #0, RD13

L00800377:
  MOV.B    @RQ14, RD12
  CMP/EQ   RD12
  BT       L00800378
  ADD      #1, RQ14
  ADD      #1, RD13
  BRA      L00800377

L00800378:
  EXTS.L   RD13, RQ11
  MOV      RQ11, RQ2

L00E0040C:
  ADD      #136, R15
  BRA      L00C00047

malloc:
  PUSH     LR
  PUSH     R14
  ADD      #-160, R15
  MOV.Q    RQ4, @(R15, 24)
  MOV      #16, RQ14
  ADD      R15, RQ14
  MOV.Q    @(R15, 24), RQ4
  MOV      RQ14, RQ5
  BSR      __allocmem
  MOV.Q    @(R15, 16), RQ2

L00E0040D:
  ADD      #160, R15
  POP      RQ14
  RET     

realloc:
  PUSH     LR
  BSR      L00C00044
  ADD      #-112, R15
  MOV.Q    RQ4, @(R15, 56)
  MOV.Q    RQ5, @(R15, 48)
  MOV.Q    @(R15, 48), RQ12
  MOV      #0, RQ11
  CMPQ/EQ  RQ11, RQ12
  BT       L00800393
  MOV.Q    @(R15, 56), RQ4
  BSR      free
  MOV      #0, RQ14
  MOV      RQ14, RQ2
  BRA      L00E0040E

L00800393:
  MOV.Q    @(R15, 48), RQ4
  BSR      malloc
  MOV      RQ2, RQ13
  MOV      #0, RQ14
  MOV      RQ13, RQ10
  MOV      RQ14, RQ9
  MOV.Q    RQ10, @(R15, 24)
  MOV.Q    RQ9, @(R15, 16)
  CMPQ/EQ  RQ9, RQ10
  BT       L00800394
  MOV      #0, RQ14
  MOV      RQ14, RQ2
  BRA      L00E0040E

L00800394:
  MOV      #0, RQ14
  MOV.Q    @(R15, 56), RQ8
  CMPQ/EQ  RQ14, RQ8
  BT       L00800395
  MOV.Q    @(R15, 56), RQ31
  ADD      RQ31, #-8, RQ14
  MOV.Q    @RQ14, RQ30
  MOV      RQ30, RQ29
  MOV      RQ29, RQ30
  MOV.Q    @(R15, 48), RQ28
  MOV.Q    RQ30, @(R15, 8)
  MOV.Q    RQ29, @(R15, 40)
  MOV.Q    RQ28, @R15
  CMPQ/GT  RQ30, RQ28
  BT       L00800396
  MOV.Q    @(R15, 40), RQ28
  MOV.Q    RQ28, @(R15, 48)

L00800396:
  MOV      RQ13, RQ4
  MOV.Q    @(R15, 56), RQ5
  MOV.Q    @(R15, 48), RQ6
  BSR      memcpy
  MOV.Q    @(R15, 56), RQ4
  BSR      free

L00800395:
  MOV      RQ13, RQ2

L00E0040E:
  ADD      #112, R15
  BRA      L00C00045

free:
  PUSH     LR
  PUSH     R14
  PUSH     R13
  ADD      #-136, R15
  MOV      RQ4, RQ13
  MOV      #0, RQ14
  CMPQ/EQ  RQ14, RQ13
  BT       L00800397
  MOV      RQ13, RQ4
  BSR      __freemem

L00800397:

L00E0040F:
  ADD      #136, R15
  POP      RQ13
  POP      RQ14
  RET     

abort:
  PUSH     LR
  ADD      #-136, R15
  MOV      #1, RD4
  BSR      raise
  MOV      #1, RD4
  BSR      exit

L00E00410:
  ADD      #136, R15
  RET     

exit:
  PUSH     LR
  ADD      #-152, R15
  MOV.L    RD4, @(R15, 12)
  MOV.L    @(R15, 12), RD4
  BSR      __exit

L00E00411:
  ADD      #152, R15
  RET     

rand:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  ADD      #-152, R15
  MOV.Q    TU3CD6E720/myseed, RQ13
  MOV      #0x0000FFF1, RQ7
  MUL.L    RQ13, RQ7, RQ14
  ADD      RQ14, #1, RQ13
  SHLD.Q   RQ13, #-16, RQ14
  AND      RQ14, #32767, RQ12
  EXTS.L   RQ12, RD7
  MOV.L    RD7, @(R15, 28)
  MOV.Q    RQ13, TU3CD6E720/myseed
  MOV.L    @(R15, 28), RD2

L00E00412:
  ADD      #152, R15

L00C0005F:
  POP      RQ12
  POP      RQ13
  POP      RQ14
  RTS     

atoi:
  PUSH     LR
  PUSH     R14
  PUSH     R13
  ADD      #-152, R15
  MOV.Q    RQ4, @(R15, 24)
  MOV      #0, RQ13
  MOV.Q    @(R15, 24), RQ4
  MOV      RQ13, RQ5
  MOV      #10, RD6
  BSR      strtol
  MOV      RQ2, RQ14
  EXTS.L   RQ14, RD7
  MOV.L    RD7, @(R15, 4)
  MOV.L    @(R15, 4), RD2

L00E00413:
  ADD      #152, R15
  POP      RQ13
  POP      RQ14
  RET     

strtol:
  PUSH     LR
  BSR      L00C0003D
  ADD      #-128, R15
  MOV      RQ4, RQ11
  MOV.Q    RQ5, @(R15, 32)
  MOV.L    RD6, @(R15, 28)
  MOV      #0, RD14
  MOV      RD14, RD13

L008003DB:
  MOV.B    @RQ11, RD14
  EXTU.B   RD14, RD12
  MOV      RD12, RD4
  BSR      isspace
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L008003DC
  ADD      #1, RQ11
  BRA      L008003DB

L008003DC:
  MOV.B    @RQ11, RD14
  CMP/EQ   #45, RD14
  BT       L008003DD
  MOV      #1, RD13
  ADD      #1, RQ11
  BRA      L008003DE

L008003DD:
  MOV.B    @RQ11, RD14
  CMP/EQ   #43, RD14
  BT       L008003DF
  ADD      #1, RQ11

L008003DF:

L008003DE:
  MOV      RQ11, RQ4
  MOV.Q    @(R15, 32), RQ5
  MOV.L    @(R15, 28), RD6
  BSR      strtoul
  MOV      RQ2, RQ10
  CMP/EQ   RD13
  BT       L008003E0
  NEG      RQ10, RQ8
  MOV      RQ8, RQ9
  BRA      L008003E1

L008003E0:
  MOV      RQ10, RQ9

L008003E1:
  MOV      RQ9, RQ2

L00E00414:
  ADD      #128, R15
  BRA      L00C0003E

strtoul:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-104, R15
  MOV      RQ4, RQ9
  MOV.Q    RQ5, @(R15, 64)
  MOV.L    RD6, @(R15, 60)
  MOV      #0, RQ12
  EXTS.L   RQ12, RQ12
  MOV      RQ12, RQ11
  MOV      #0, RD14
  MOV      RD14, RD10
  MOV.L    @(R15, 60), RD8
  CMP/EQ   RD8
  BT       L008003C4
  MOV      #1, RD10

L008003C4:

L008003C5:
  MOV.B    @RQ9, RD14
  EXTU.B   RD14, RD13
  MOV      RD13, RD4
  BSR      isspace
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L008003C6
  ADD      #1, RQ9
  BRA      L008003C5

L008003C6:

L008003C7:
  MOV.B    @RQ9, RD14
  EXTU.B   RD14, RD13
  MOV      RD13, RD4
  BSR      isdigit
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L008003C8
  MOV.L    @(R15, 60), RD31
  CMP/EQ   #0, RD31
  BT       L008003C9
  MOV.B    @RQ9, RD14
  CMP/EQ   #48, RD14
  BT       L008003CA
  MOV      #8, RD30
  MOV.L    RD30, @(R15, 60)
  BRA      L008003CB

L008003CA:
  MOV      #10, RD29
  MOV      #0, RD10
  MOV.L    RD29, @(R15, 60)

L008003CB:

L008003C9:
  MOV.L    @(R15, 60), RD28
  EXTS.L   RQ28, RQ28
  MOV      RQ11, RQ12
  MOV      RQ28, RQ5
  MOV      RQ12, RQ4
  BSR      __umulsq
  MOV      RQ2, RQ12
  MOV.B    @RQ9, RD14
  SUB      RD14, #48, RD13
  MOV      RQ12, RQ28
  EXTS.L   RD13, RQ12
  ADD      RQ28, RQ12, RQ11
  ADD      #1, RQ9
  MOV.Q    RQ28, @(R15, 32)
  BRA      L008003CC

L008003C8:
  MOV.B    @RQ9, RD14
  EXTU.B   RD14, RD13
  MOV      RD13, RD4
  BSR      isalpha
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L008003CD
  MOV.B    @RQ9, RD14
  CMP/EQ   #88, RD14
  BT       L008003CE
  MOV.B    @RQ9, RD14
  CMP/EQ   #120, RD14
  BT       L008003CE
  BRA      L008003CF

L008003CE:
  MOV.L    @(R15, 60), RD27
  CMP/EQ   #0, RD27
  BT       L008003D0
  MOV.L    @(R15, 60), RD26
  CMP/EQ   #8, RD26
  BT       L008003D1
  CMP/EQ   RD10
  BT       L008003D1
  BRA      L008003D0

L008003D0:
  MOV      #16, RD25
  MOV      #0, RD10
  ADD      #1, RQ9
  MOV.L    RD25, @(R15, 60)
  BRA      L008003D2

L008003D1:
  MOV.L    @(R15, 60), RD7
  CMP/EQ   #16, RD7
  BT       L008003D3
  ADD      #1, RQ9
  BRA      L008003D4

L008003D3:
  BRA      L008003D5

L008003D4:

L008003D2:
  BRA      L008003D6

L008003CF:
  MOV.L    @(R15, 60), RD6
  CMP/GT   #10, RD6
  BT       L008003D7
  BRA      L008003D5
  BRA      L008003D8

L008003D7:
  MOV.L    @(R15, 60), RD25
  EXTS.L   RQ25, RQ25
  MOV      RQ11, RQ12
  MOV      RQ25, RQ5
  MOV      RQ12, RQ4
  BSR      __umulsq
  MOV      RQ2, RQ12
  MOV.B    @RQ9, RD14
  EXTU.B   RD14, RD13
  MOV.Q    RQ25, @(R15, 32)
  MOV      RD13, RD4
  BSR      toupper
  MOV      RD2, RD14
  SUB      RD14, #65, RD13
  EXTS.L   RD13, RQ26
  ADD      RQ12, RQ26, RQ25
  ADD      RQ25, #10, RQ11
  ADD      #1, RQ9
  MOV.Q    RQ25, @(R15, 32)

L008003D8:

L008003D6:
  BRA      L008003D9

L008003CD:
  BRA      L008003D5

L008003D9:

L008003CC:
  BRA      L008003C7

L008003D5:
  MOV      #0, RQ5
  MOV.Q    @(R15, 64), RQ3
  MOV      RQ3, RQ4
  MOV      RQ5, RQ2
  MOV.Q    RQ4, @(R15, 8)
  MOV.Q    RQ2, @R15
  CMPQ/EQ  RQ2, RQ4
  BT       L008003DA
  MOV.Q    @(R15, 64), RQ23
  MOV.Q    RQ9, @RQ23

L008003DA:
  MOV      RQ11, RQ2

L00E00415:
  ADD      #104, R15
  BRA      L00C00027

abs:
  PUSH     R14
  ADD      #-152, R15
  MOV      RD4, RD14
  CMP/PZ   RD14
  BT       L008003ED
  NEG      RD14, RD14

L008003ED:
  MOV      RD14, RD2

L00E00416:
  ADD      #152, R15
  POP      RQ14
  RTS     

__get_errno:
  PUSH     R14
  ADD      #-152, R15
  MOV      __errno, RQ14
  MOV      RQ14, RQ2

L00E00417:
  ADD      #152, R15
  POP      RQ14
  RTS     

__open:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-56, R15
  MOV.Q    RQ4, @(R15, 24)
  MOV.L    RD5, @(R15, 20)
  MOV.L    RD6, @(R15, 16)
  MOV.Q    @(R15, 24), RQ10
  MOV      RQ10, RQ11
  MOV      RQ11, RQ4
  MOV      L00C0006A, RQ5
  BSR      tk_fopen
  MOV      RQ2, RQ12
  MOV      #0, RQ9
  CMPQ/EQ  RQ9, RQ12
  BT       L008007E5
  MOV.L    @(R15, 16), RD13
  EXTU.L   RQ13, RQ13
  MOV      #-1, RD8
  MOV.L    RD8, @RQ13
  MOV      RD8, RD2
  BRA      L00E00418

L008007E5:
  MOV.L    @(R15, 16), RD13
  EXTU.L   RQ13, RQ13
  MOV      #0, RD31
  MOV.L    RD31, @RQ13
  MOV      #3, RD14

L008007E6:
  MOV.L    btshx_tk_nhandles, RD30
  CMP/GT   RD14, RD30
  BT       L008007E7
  MOV      btshx_tk_handles, RQ29
  EXTS.L   RD14
  MOV.Q    @(RQ29, RD14), RQ13
  MOV      #0, RQ28
  CMPQ/EQ  RQ28, RQ13
  BT       L008007E8
  MOV      btshx_tk_handles, RQ27
  EXTS.L   RD14
  MOV.Q    RQ12, @(RQ27, RD14)
  MOV      RD14, RD2
  BRA      L00E00418

L008007E8:

L008007E9:
  ADD      #1, RD14
  BRA      L008007E6

L008007E7:
  MOV.L    btshx_tk_nhandles, RD26
  MOV      #256, RD25
  CMP/GE   RD25, RD26
  BT       L008007EA
  MOV.L    @(R15, 16), RD13
  EXTU.L   RQ13, RQ13
  MOV      #-1, RD7
  MOV.L    RD7, @RQ13
  MOV      RD7, RD2
  BRA      L00E00418

L008007EA:
  MOV.L    btshx_tk_nhandles, RD5
  MOV      RD5, RD6
  ADD      #1, RD5
  MOV      RD6, RD14
  MOV      btshx_tk_handles, RQ4
  MOV.Q    RQ12, @(RQ4, RD14)
  MOV.L    RD5, btshx_tk_nhandles
  MOV      RD14, RD2

L00E00418:
  ADD      #56, R15
  BRA      L00C00027

__read:
  PUSH     LR
  BSR      L00C0004E
  ADD      #-112, R15
  MOV.L    RD4, @(R15, 44)
  MOV.Q    RQ5, @(R15, 32)
  MOV.Q    RQ6, @(R15, 24)
  MOV      RQ7, RQ13
  MOV.L    @(R15, 44), RD12
  MOV      #3, RD11
  CMP/GE   RD11, RD12
  BT       L008007DF
  MOV      #0, RD2
  BRA      L00E00419

L008007DF:
  MOV      #0, RQ10
  CMPQ/EQ  RQ10, RQ13
  BT       L008007E0
  MOV      #0, RD9
  MOV.L    RD9, @RQ13

L008007E0:
  MOV      btshx_tk_handles, RQ8
  MOV.L    @(R15, 44), RD31
  EXTS.L   RD31
  MOV.Q    @(RQ8, RD31), RQ30
  MOV.L    @(R15, 24), RD9
  EXTS.L   RD9, RD9
  MOV.L    RD9, @(R15, 4)
  MOV.Q    RQ30, @(R15, 8)
  MOV.Q    @(R15, 32), RQ4
  MOV      #1, RD5
  MOV      RD9, RD6
  MOV      RQ30, RQ7
  BSR      tk_fread
  MOV      RD2, RD14
  CMP/PZ   RD14
  BT       L008007E1
  MOV      #1, RD7
  MOV.L    RD7, @RQ13

L008007E1:
  MOV      RD14, RD2

L00E00419:
  ADD      #112, R15
  BRA      L00C0004F

__write:
  PUSH     LR
  BSR      L00C00053
  ADD      #-88, R15
  MOV.L    RD4, @(R15, 44)
  MOV.Q    RQ5, @(R15, 32)
  MOV.Q    RQ6, @(R15, 24)
  MOV      RQ7, RQ11
  MOV.L    @(R15, 44), RD10
  MOV      #3, RD9
  CMP/GE   RD9, RD10
  BT       L008007E2
  MOV.Q    @(R15, 32), RQ8
  MOV      RQ8, RQ14
  MOV.L    @(R15, 24), RD13
  EXTS.L   RD13, RD13
  MOV      RQ14, RQ4
  MOV      RD13, RD5
  BSR      tk_puts_n
  MOV      #0, RD2
  BRA      L00E0041A

L008007E2:
  MOV      #0, RQ31
  CMPQ/EQ  RQ31, RQ11
  BT       L008007E3
  MOV      #0, RD30
  MOV.L    RD30, @RQ11

L008007E3:
  MOV.Q    @(R15, 32), RQ29
  MOV      RQ29, RQ14
  MOV      btshx_tk_handles, RQ28
  MOV.L    @(R15, 44), RD27
  EXTS.L   RD27
  MOV.Q    @(RQ28, RD27), RQ30
  MOV.L    @(R15, 24), RD13
  EXTS.L   RD13, RD13
  MOV.Q    RQ30, @R15
  MOV      RQ14, RQ4
  MOV      #1, RD5
  MOV      RD13, RD6
  MOV      RQ30, RQ7
  BSR      tk_fwrite
  MOV      RD2, RD12
  CMP/PZ   RD12
  BT       L008007E4
  MOV      #1, RD7
  MOV.L    RD7, @RQ11

L008007E4:
  MOV      RD12, RD2

L00E0041A:
  ADD      #88, R15
  BRA      L00C00054

__seek:
  PUSH     LR
  BSR      L00C00032
  ADD      #-152, R15
  MOV.L    RD4, @(R15, 44)
  MOV.Q    RQ5, @(R15, 32)
  MOV.L    RD6, @(R15, 28)
  MOV      btshx_tk_handles, RQ12
  MOV.L    @(R15, 44), RD11
  MOV.Q    @(RQ12, RD11), RQ14
  MOV.L    @(R15, 32), RD13
  EXTS.L   RD13, RD13
  MOV      RQ14, RQ4
  MOV      RD13, RD5
  MOV.L    @(R15, 28), RD6
  BSR      tk_fseek

L00E0041B:
  ADD      #152, R15
  BRA      L00C00033

L00C00072:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  RTS     

__close:
  PUSH     LR
  BSR      L00C00072
  ADD      #-112, R15
  MOV      RD4, RD14
  MOV      #3, RD12
  CMP/GE   RD12, RD14
  BT       L008007EB
  BRA      L00E0041C

L008007EB:
  MOV      btshx_tk_handles, RQ11
  EXTS.L   RD14
  MOV.Q    @(RQ11, RD14), RQ13
  MOV      #0, RQ10
  EXTS.L   RD14
  MOV.Q    RQ10, @(RQ11, RD14)
  MOV      RQ13, RQ4
  BSR      tk_fclose

L00E0041C:
  ADD      #112, R15

L00C00074:
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  RET     

__remove:
  ADD      #-160, R15
  MOV.Q    RQ4, @(R15, 8)

L00E0041D:
  ADD      #160, R15
  RTS     

__rename:
  ADD      #-160, R15
  MOV.Q    RQ4, @(R15, 8)
  MOV.Q    RQ5, @R15

L00E0041E:
  ADD      #160, R15
  RTS     

__tell:
  PUSH     LR
  BSR      L00C00032
  ADD      #-136, R15
  MOV.L    RD4, @(R15, 28)
  MOV      btshx_tk_handles, RQ12
  MOV.L    @(R15, 28), RD11
  MOV.Q    @(RQ12, RD11), RQ13
  MOV      RQ13, RQ4
  BSR      tk_ftell
  MOV      RD2, RD14
  EXTS.L   RD14, RQ11
  MOV.Q    RQ11, @R15
  MOV      RQ11, RQ2

L00E0041F:
  ADD      #136, R15
  BRA      L00C00033

L00C0007A:
  PUSH     R31
  PUSH     R30
  PUSH     R29
  PUSH     R28
  PUSH     R27
  PUSH     R26
  PUSH     R25
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  PUSH     R9
  PUSH     R8
  RTS     

tk_printf:
  PUSH     LR
  BSR      L00C0007A
  ADD      #-296, R15
  MOV.Q    RQ4, @(R15, 72)
  MOV      #80, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 64)
  MOV.Q    RQ4, @RQ3
  MOV.Q    RQ5, @(RQ3, 8)
  MOV.Q    RQ6, @(RQ3, 16)
  MOV.Q    RQ7, @(RQ3, 24)
  MOV.Q    RQ20, @(RQ3, 32)
  MOV.Q    RQ21, @(RQ3, 40)
  MOV.Q    RQ22, @(RQ3, 48)
  MOV.Q    RQ23, @(RQ3, 56)
  FMOV.D   DR4, @(RQ3, 64)
  FMOV.D   DR5, @(RQ3, 72)
  FMOV.D   DR6, @(RQ3, 80)
  FMOV.D   DR7, @(RQ3, 88)
  MOV      #8, R0
  MOV.L    R0, @(R3, 96)
  MOV      #64, R0
  MOV.L    R0, @(R3, 104)
  MOV      #416, R1
  ADD      R15, R1
  MOV.Q    R1, @(R3, 112)
  MOV.Q    @(R15, 72), RQ10
  MOV      RQ10, RQ13

L0080080B:
  MOV.B    @RQ13, RD14
  CMP/EQ   RD14
  BT       L0080080C
  MOV.B    @RQ13, RD14
  CMP/EQ   #37, RD14
  BT       L0080080D
  MOV      RQ13, RQ12
  ADD      #1, RQ13
  MOV.B    @RQ12, RD14
  MOV      RD14, RD4
  BSR      tk_putc
  BRA      L0080080B

L0080080D:
  MOV.B    @(RQ13, 1), RD14
  CMP/EQ   #37, RD14
  BT       L0080080E
  ADD      #2, RQ13
  MOV      #37, RD4
  BSR      tk_putc
  BRA      L0080080B

L0080080E:
  ADD      #1, RQ13
  MOV.B    @RQ13, RD14
  CMP/EQ   #48, RD14
  BT       L0080080F
  MOV      #48, RD14
  MOV      RD14, RD9
  ADD      #1, RQ13
  MOV.L    RD9, @(R15, 52)
  BRA      L00800810

L0080080F:
  MOV      #32, RD14
  MOV      RD14, RD8
  MOV.L    RD8, @(R15, 52)

L00800810:
  MOV      #0, RD11
  MOV.B    @RQ13, RD14
  CMP/GE   #48, RD14
  BT       L00800811
  MOV.B    @RQ13, RD14
  CMP/GT   #57, RD14
  BT       L00800811
  BRA      L00800812

L00800812:

L00800813:
  MOV.B    @RQ13, RD14
  CMP/GE   #48, RD14
  BT       L00800814
  MOV.B    @RQ13, RD14
  CMP/GT   #57, RD14
  BT       L00800814
  BRA      L00800815

L00800815:
  MOV      RD11, RD14
  MOV      #10, RD31
  MUL.L    RD31, RD14, RD14
  MOV      RQ13, RQ12
  ADD      #1, RQ13
  MOV.B    @RQ12, RD30
  SUB      RD30, #48, RD29
  ADD      RD14, RD29, RD11
  MOV.L    RD30, @(R15, 12)
  MOV.L    RD29, @(R15, 8)
  BRA      L00800813

L00800814:

L00800811:
  MOV      #0, RD28
  MOV.B    @RQ13, RD14
  MOV.L    RD28, @(R15, 32)
  CMP/EQ   #46, RD14
  BT       L00800816
  ADD      #1, RQ13
  MOV.B    @RQ13, RD14
  CMP/GE   #48, RD14
  BT       L00800817
  MOV.B    @RQ13, RD14
  CMP/GT   #57, RD14
  BT       L00800817
  BRA      L00800818

L00800818:

L00800819:
  MOV.B    @RQ13, RD14
  CMP/GE   #48, RD14
  BT       L0080081A
  MOV.B    @RQ13, RD14
  CMP/GT   #57, RD14
  BT       L0080081A
  BRA      L0080081B

L0080081B:
  MOV.L    @(R15, 32), RD27
  MOV      RD27, RD14
  MOV      #10, RD26
  MUL.L    RD26, RD14, RD14
  MOV      RQ13, RQ12
  ADD      #1, RQ13
  MOV.B    @RQ12, RD25
  SUB      RD25, #48, RD7
  ADD      RD14, RD7, RD27
  MOV.L    RD27, @(R15, 32)
  MOV.L    RD25, @(R15, 12)
  MOV.L    RD7, @(R15, 8)
  BRA      L00800819

L0080081A:

L00800817:

L00800816:
  MOV      RQ13, RQ12
  ADD      #1, RQ13
  MOV.B    @RQ12, RD14
  CMP/EQ   #99, RD14
  BT       L0080081C
  CMP/EQ   #100, RD14
  BT       L0080081D
  CMP/EQ   #88, RD14
  BT       L0080081E
  CMP/EQ   #115, RD14
  BT       L0080081F
  CMP/EQ   #112, RD14
  BT       L00800820
  CMP/EQ   #102, RD14
  BT       L00800821
  BRA      L00800822

L0080081C:
  MOV      #80, RQ4
  ADD      R15, RQ4
  MOV      __va64_arg_i, R3
  JSR      R3
  MOV      R2, RD14
  MOV      RD14, RD25
  MOV.L    RD25, @(R15, 36)
  MOV      RD25, RD4
  BSR      tk_putc
  BRA      L00800823

L0080081D:
  MOV      #80, RQ4
  ADD      R15, RQ4
  BSR      __va64_arg_i
  MOV      R2, RD14
  MOV      RD14, RD25
  MOV.L    RD25, @(R15, 36)
  CMP/EQ   RD11
  BT       L00800824
  MOV.L    @(R15, 36), RD4
  MOV      RD11, RD5
  BSR      tk_print_decimal_n
  BRA      L00800825

L00800824:
  MOV.L    @(R15, 36), RD4
  BSR      tk_print_decimal

L00800825:
  BRA      L00800823

L0080081E:
  MOV      #80, RQ4
  ADD      R15, RQ4
  BSR      __va64_arg_i
  MOV      R2, RD14
  MOV      RD14, RD25
  MOV.L    RD25, @(R15, 36)
  CMP/EQ   RD11
  BT       L00800826
  MOV.L    @(R15, 36), RD4
  BSR      tk_print_hex_genw
  MOV      RD2, RD11

L00800826:
  MOV.L    @(R15, 36), RD4
  MOV      RD11, RD5
  BSR      tk_print_hex_n
  BRA      L00800823

L0080081F:
  MOV      #80, RQ4
  ADD      R15, RQ4
  MOV      __va64_arg_l, R3
  JSR      R3
  MOV      R2, RQ12
  MOV      RQ12, RQ25
  MOV.Q    RQ25, @(R15, 40)
  MOV      RQ25, RQ4
  BSR      tk_puts
  BRA      L00800823

L00800820:
  MOV      #80, RQ4
  ADD      R15, RQ4
  BSR      __va64_arg_l
  MOV      R2, RQ12
  MOV      RQ12, RQ25
  MOV      RQ25, RQ7
  EXTU.L   RD7
  MOV      RD7, RD14
  MOV.Q    RQ25, @(R15, 40)
  MOV      RD14, RD4
  BSR      tk_print_hex
  BRA      L00800823

L00800821:
  MOV      #0x6809D498, DR7
  FMOV.D   DR7, @(R15, 56)
  MOV      #80, RQ4
  ADD      R15, RQ4
  MOV      __va64_arg_d, R3
  JSR      R3
  FMOV.D   DR2, @R15
  FMOV.D   @R15, DR7
  FMOV.D   DR7, @(R15, 56)
  FMOV.D   @(R15, 56), DR4
  BSR      tk_print_float
  BRA      L00800823

L00800822:
  BRA      L00800823

L00800823:
  BRA      L0080080B

L0080080C:

L00E00420:
  ADD      #296, R15

L00C00085:
  POP      RQ8
  POP      RQ9
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  POP      RQ25
  POP      RQ26
  POP      RQ27
  POP      RQ28
  POP      RQ29
  POP      RQ30
  POP      RQ31
  RET     

L00C0008B:
  PUSH     R31
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  PUSH     R9
  PUSH     R8
  RTS     

__init_stdin:
  PUSH     LR
  BSR      L00C0008B
  ADD      #-440, R15
  MOV      TU3CD6E720/permFiles, RQ14
  MOV      #0, RD13
  MOV      #120, RD7
  MUL.L    RD13, RD7
  STS      R0, RQ12
  ADD      RQ14, RQ12
  MOV      RQ12, RQ11
  MOV      #1, RD10
  MOV      #120, RD7
  MUL.L    RD10, RD7
  STS      R0, RQ12
  ADD      RQ14, RQ12
  MOV      RQ12, RQ9
  MOV      #2, RD8
  MOV      #120, RD7
  MUL.L    RD8, RD7
  STS      R0, RQ12
  ADD      RQ14, RQ12
  MOV      RQ12, RQ31
  MOV.Q    RQ11, __stdin
  MOV.Q    RQ9, __stdout
  MOV.Q    RQ31, __stderr

L00E00421:
  ADD      #440, R15

L00C0008C:
  POP      RQ8
  POP      RQ9
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  POP      RQ31
  RET     

L00C0008E:
  PUSH     R31
  PUSH     R30
  PUSH     R29
  PUSH     R28
  PUSH     R27
  PUSH     R26
  PUSH     R25
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  PUSH     R9
  PUSH     R8
  PUSH     FR15
  PUSH     FR14
  PUSH     FR13
  PUSH     FR12
  PUSH     FR11
  PUSH     FR10
  PUSH     FR9
  RTS     

dblcvt:
  PUSH     LR
  BSR      L00C0008E
  ADD      #-256, R15
  FMOV.D   DR4, @(R15, 168)
  MOV.L    RD4, @(R15, 164)
  MOV.Q    RQ5, @(R15, 152)
  MOV.L    RD6, @(R15, 148)
  MOV.Q    RQ7, @(R15, 136)
  MOV      #176, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 88)
  FMOV.D   @(R15, 168), DR15
  MOV      #0, DR14
  FCMP/GT  DR15, DR14
  BT       L008002CD
  FMOV.D   @(R15, 168), DR13
  FMOV     DR13, DR12
  FNEG     DR12
  MOV      #45, RD14
  MOV      RD14, RD12
  FMOV.D   DR12, @(R15, 128)
  MOV.L    RD12, @(R15, 100)
  BRA      L008002CE

L008002CD:
  FMOV.D   @(R15, 168), DR11
  MOV      #32, RD14
  MOV      RD14, RD11
  FMOV.D   DR11, @(R15, 128)
  MOV.L    RD11, @(R15, 100)

L008002CE:
  MOV      #0, RD13
  FMOV.D   @(R15, 128), DR10
  MOV      #0, DR9
  FCMP/GT  DR9, DR10
  BT       L008002CF

L008002D0:
  FMOV.D   @(R15, 128), DR9
  MOV      #0, DR10
  FCMP/GT  DR9, DR10
  BT       L008002D1
  CMP/GE   #35, RD13
  BT       L008002D1
  BRA      L008002D2

L008002D2:
  ADD      #1, RD13
  MOV      #0x99999998, DR9
  FMOV.D   @(R15, 128), DR10
  FMUL     DR9, DR10
  FMOV.D   DR10, @(R15, 128)
  BRA      L008002D0

L008002D1:
  BRA      L008002D3

L008002CF:
  FMOV.D   @(R15, 128), DR9
  MOV      #0, DR10
  FCMP/EQ  DR10, DR9
  BT       L008002D4
  MOV      #0, RD13
  BRA      L008002D5

L008002D4:
  FMOV.D   @(R15, 128), DR9
  MOV      #0, DR10
  FCMP/GT  DR9, DR10
  BT       L008002D6

L008002D7:
  FMOV.D   @(R15, 128), DR9
  MOV      #0, DR10
  FCMP/GT  DR9, DR10
  BT       L008002D8
  CMP/GT   #-35, RD13
  BT       L008002D8
  BRA      L008002D9

L008002D9:
  ADD      #-1, RD13
  MOV      #0, DR9
  FMOV.D   @(R15, 128), DR10
  FMUL     DR9, DR10
  FMOV.D   DR10, @(R15, 128)
  BRA      L008002D7

L008002D8:

L008002D6:

L008002D5:

L008002D3:
  CMP/GT   #-35, RD13
  BT       L008002DA
  CMP/GE   #35, RD13
  BT       L008002DA
  BRA      L008002DB

L008002DA:
  MOV      #0, RD13
  MOV      #0, DR9
  FMOV.D   DR9, @(R15, 128)

L008002DB:
  MOV.L    @(R15, 164), RD10
  CMP/EQ   #69, RD10
  BT       L008002DC
  MOV.L    @(R15, 164), RD9
  CMP/EQ   #101, RD9
  BT       L008002DD
  MOV.L    @(R15, 164), RD8
  CMP/EQ   #102, RD8
  BT       L008002DE
  MOV.L    @(R15, 164), RD31
  CMP/EQ   #70, RD31
  BT       L008002DF
  BRA      L008002E0

L008002DC:

L008002DD:
  MOV      #0, RD30
  MOV.L    RD30, @(R15, 104)
  BRA      L008002E1

L008002DE:

L008002DF:
  CMP/PZ   RD13
  BT       L008002E2
  MOV      #1, RD29
  MOV.L    RD29, @(R15, 104)
  BRA      L008002E3

L008002E2:
  MOV      #-1, RD28
  MOV.L    RD28, @(R15, 104)

L008002E3:
  BRA      L008002E1

L008002E0:
  CMP/PZ   RD13
  BT       L008002E4
  MOV.L    @(R15, 148), RD27
  CMP/GT   RD13, RD27
  BT       L008002E5
  MOV      #1, RD26
  MOV.L    RD26, @(R15, 104)
  BRA      L008002E6

L008002E5:
  MOV      #0, RD25
  MOV.L    RD25, @(R15, 104)

L008002E6:
  BRA      L008002E7

L008002E4:
  MOV      #-4, RD7
  CMP/GE   RD7, RD13
  BT       L008002E8
  MOV      #-1, RD25
  MOV.L    RD25, @(R15, 104)
  BRA      L008002E9

L008002E8:
  MOV      #0, RD25
  MOV.L    RD25, @(R15, 104)

L008002E9:

L008002E7:
  BRA      L008002E1

L008002E1:
  MOV.L    @(R15, 104), RD6
  CMP/EQ   RD6
  BT       L008002EA
  MOV.L    @(R15, 104), RD5
  CMP/EQ   #1, RD5
  BT       L008002EB
  MOV.L    @(R15, 104), RD4
  CMP/EQ   #-1, RD4
  BT       L008002EC
  BRA      L008002ED

L008002EA:
  MOV.L    @(R15, 148), RD3
  CMP/GE   #53, RD3
  BT       L008002EE
  MOV.L    @(R15, 148), RD23
  MOV      RD23, RD2
  MOV.L    RD2, @(R15, 112)
  BRA      L008002EF

L008002EE:
  MOV      #53, RD22
  MOV.L    RD22, @(R15, 112)

L008002EF:
  MOV      #0, DR9
  MOV      #0, RD21
  FMOV.D   DR9, @(R15, 120)
  MOV.L    RD21, @(R15, 116)

L008002F0:
  MOV.L    @(R15, 116), RD20
  ADD      #1, RD20
  MOV.L    RD20, @(R15, 116)
  MOV.L    @(R15, 112), RD19
  CMP/GT   RD19, RD20
  BT       L008002F1
  MOV      #0x99999998, DR9
  FMOV.D   @(R15, 120), DR10
  FMUL     DR9, DR10
  FMOV.D   DR10, @(R15, 120)
  BRA      L008002F0

L008002F1:
  FMOV.D   @(R15, 120), DR9
  FMOV.D   @(R15, 128), DR10
  FADD     DR9, DR10
  FMOV.D   DR10, @(R15, 128)
  MOV      #0, DR11
  FCMP/GT  DR10, DR11
  BT       L008002F2
  MOV      #0x99999998, DR9
  FMOV.D   @(R15, 128), DR10
  FMUL     DR9, DR10
  ADD      #1, RD13
  FMOV.D   DR10, @(R15, 128)

L008002F2:
  BRA      L008002ED

L008002EB:
  MOV.L    @(R15, 148), RD18
  ADD      RD13, RD18, RD14
  CMP/GE   #53, RD14
  BT       L008002F3
  MOV.L    @(R15, 148), RD16
  ADD      RD13, RD16, RD17
  MOV.L    RD17, @(R15, 112)
  BRA      L008002F4

L008002F3:
  MOV      #53, RD16
  MOV.L    RD16, @(R15, 112)

L008002F4:
  MOV      #0, DR9
  MOV      #0, RD16
  FMOV.D   DR9, @(R15, 120)
  MOV.L    RD16, @(R15, 116)

L008002F5:
  MOV.L    @(R15, 116), RD16
  MOV      RD16, RD14
  ADD      #1, RD16
  MOV.L    RD16, @(R15, 116)
  MOV.L    @(R15, 112), RD17
  CMP/GT   RD14, RD17
  BT       L008002F6
  MOV      #0x99999998, DR9
  FMOV.D   @(R15, 120), DR10
  FMUL     DR9, DR10
  FMOV.D   DR10, @(R15, 120)
  BRA      L008002F5

L008002F6:
  FMOV.D   @(R15, 120), DR9
  FMOV.D   @(R15, 128), DR10
  FADD     DR9, DR10
  FMOV.D   DR10, @(R15, 128)
  MOV      #0, DR11
  FCMP/GT  DR10, DR11
  BT       L008002F7
  MOV      #0x99999998, DR9
  FMOV.D   @(R15, 128), DR10
  FMUL     DR9, DR10
  ADD      #1, RD13
  FMOV.D   DR10, @(R15, 128)

L008002F7:
  BRA      L008002ED

L008002EC:
  MOV.L    @(R15, 148), RD16
  MOV      #53, RD17
  CMP/GE   RD17, RD16
  BT       L008002F8
  MOV.L    @(R15, 148), RD16
  ADD      RD16, RD13, RD14
  ADD      RD14, #1, RD17
  MOV.L    RD17, @(R15, 112)
  BRA      L008002F9

L008002F8:
  MOV      #53, RD16
  MOV.L    RD16, @(R15, 112)

L008002F9:
  MOV      #0, DR9
  MOV      #0, RD16
  FMOV.D   DR9, @(R15, 120)
  MOV.L    RD16, @(R15, 116)

L008002FA:
  MOV.L    @(R15, 116), RD16
  MOV      RD16, RD14
  ADD      #1, RD16
  MOV.L    RD16, @(R15, 116)
  MOV.L    @(R15, 112), RD17
  CMP/GT   RD14, RD17
  BT       L008002FB
  MOV      #0x99999998, DR9
  FMOV.D   @(R15, 120), DR10
  FMUL     DR9, DR10
  FMOV.D   DR10, @(R15, 120)
  BRA      L008002FA

L008002FB:
  MOV.L    @(R15, 112), RD16
  MOV      #0, RD17
  CMP/GE   RD17, RD16
  BT       L008002FC
  FMOV.D   @(R15, 120), DR9
  FMOV.D   @(R15, 128), DR10
  FADD     DR9, DR10
  FMOV.D   DR10, @(R15, 128)

L008002FC:
  FMOV.D   @(R15, 128), DR9
  MOV      #0, DR10
  FCMP/GT  DR9, DR10
  BT       L008002FD
  MOV      #0x99999998, DR9
  FMOV.D   @(R15, 128), DR10
  FMUL     DR9, DR10
  ADD      #1, RD13
  FMOV.D   DR10, @(R15, 128)

L008002FD:
  CMP/PZ   RD13
  BT       L008002FE
  MOV      #1, RD16
  MOV.L    RD16, @(R15, 104)

L008002FE:
  BRA      L008002ED

L008002ED:
  MOV.L    @(R15, 104), RD16
  CMP/EQ   #-1, RD16
  BT       L008002FF
  MOV.Q    @(R15, 136), RQ16
  MOV.L    @(R15, 100), RD17
  MOV.B    RD17, @RQ16
  MOV      #48, RD18
  MOV.B    RD18, @(RQ16, 1)
  MOV      #46, RD19
  MOV.B    RD19, @(RQ16, 2)
  MOV      #0, RD20
  MOV.B    RD20, @(RQ16, 3)

L00800300:
  ADD      #1, RD13
  CMP/EQ   RD13
  BT       L00800301
  MOV.L    @(R15, 148), RD25
  ADD      #-1, RD25
  MOV.L    RD25, @(R15, 148)
  MOV.Q    @(R15, 136), RQ4
  MOV      L00C0008F, RQ5
  BSR      strcat
  BRA      L00800300

L00800301:
  FMOV.D   @(R15, 128), DR9
  FSTCI    DR9
  MOV      R0, RD14
  MOV      RD14, RD25
  MOV.L    @(R15, 148), RD26
  ADD      #-1, RD26
  MOV      RD25, RD14
  MOV      #0x1999999A, RD7
  MOV      #6553, RD7
  DMULS    RD7, RD14
  STS      R1, RD6
  MOV      #10, RD7
  DMULU    RD7, RD6
  STS      R0, RD6
  SUB      RD6, RD14
  ADD      RD14, #48, RD27
  EXTS.B   RD27, RD14
  MOV      #176, RQ28
  ADD      R15, RQ28
  MOV.B    RD14, @RQ28
  MOV      #0, RD29
  MOV.B    RD29, @(RQ28, 1)
  MOV.L    RD27, @(R15, 72)
  MOV.L    RD26, @(R15, 148)
  MOV.L    RD25, @(R15, 116)
  MOV.Q    @(R15, 136), RQ4
  MOV      RQ28, RQ5
  BSR      strcat
  MOV      RD26, RD30
  MOV.L    RD30, @(R15, 108)

L00800302:
  MOV.L    @(R15, 108), RD16
  MOV      RD16, RD14
  ADD      #-1, RD16
  MOV.L    RD16, @(R15, 108)
  CMP/PL   RD14
  BT       L00800303
  FMOV.D   @(R15, 128), DR9
  MOV.L    @(R15, 116), RD25
  EXTS.L   RD25
  MOV      RD25, R0
  FLDCI    DR10
  FMOV     DR9, DR11
  FSUB     DR10, DR11
  MOV      #0, DR12
  FMUL     DR12, DR11
  FSTCI    DR11
  MOV      R0, RD14
  MOV      RD14, RD25
  MOV      RD25, RD14
  MOV      #0x1999999A, RD7
  MOV      #6553, RD7
  DMULS    RD7, RD14
  STS      R1, RD6
  MOV      #10, RD7
  DMULU    RD7, RD6
  STS      R0, RD6
  SUB      RD6, RD14
  ADD      RD14, #48, RD26
  EXTS.B   RD26, RD14
  MOV      #176, RQ27
  ADD      R15, RQ27
  MOV.B    RD14, @RQ27
  MOV      #0, RD28
  MOV.B    RD28, @(RQ27, 1)
  FMOV.D   DR11, @(R15, 128)
  FMOV.D   DR10, @(R15, 56)
  FMOV.D   DR9, @(R15, 64)
  MOV.L    RD26, @(R15, 72)
  MOV.L    RD25, @(R15, 116)
  MOV.Q    @(R15, 136), RQ4
  MOV      RQ27, RQ5
  BSR      strcat
  BRA      L00800302

L00800303:
  BRA      L00800304

L008002FF:
  MOV.L    @(R15, 104), RD16
  CMP/EQ   #1, RD16
  BT       L00800305
  FMOV.D   @(R15, 128), DR9
  FSTCI    DR9
  MOV      R0, RD14
  MOV      RD14, RD25
  MOV.Q    @(R15, 136), RQ26
  MOV.L    @(R15, 100), RD27
  MOV.B    RD27, @RQ26
  MOV      #0, RD28
  MOV.B    RD28, @(RQ26, 1)
  MOV      RD25, RD14
  MOV      #0x1999999A, RD7
  MOV      #6553, RD7
  DMULS    RD7, RD14
  STS      R1, RD6
  MOV      #10, RD7
  DMULU    RD7, RD6
  STS      R0, RD6
  SUB      RD6, RD14
  ADD      RD14, #48, RD29
  EXTS.B   RD29, RD14
  MOV      #176, RQ30
  ADD      R15, RQ30
  MOV.B    RD14, @RQ30
  MOV.B    RD28, @(RQ30, 1)
  MOV.L    RD29, @(R15, 72)
  MOV.L    RD25, @(R15, 116)
  MOV      RQ26, RQ4
  MOV      RQ30, RQ5
  BSR      strcat
  MOV.L    @(R15, 148), RD31
  ADD      RD13, RD31
  MOV      RD31, RD8
  MOV.L    RD8, @(R15, 108)
  MOV.L    RD31, @(R15, 148)

L00800306:
  MOV.L    @(R15, 108), RD16
  MOV      RD16, RD14
  ADD      #-1, RD16
  MOV.L    RD16, @(R15, 108)
  CMP/PL   RD14
  BT       L00800307
  MOV.L    @(R15, 148), RD16
  MOV.L    @(R15, 108), RD17
  SUB      RD16, RD17, RD14
  SUB      RD14, #1, RD18
  MOV.L    RD18, @(R15, 72)
  CMP/EQ   RD13, RD18
  BT       L00800308
  MOV.Q    @(R15, 136), RQ4
  MOV      L00C00090, RQ5
  BSR      strcat

L00800308:
  FMOV.D   @(R15, 128), DR9
  MOV.L    @(R15, 116), RD25
  EXTS.L   RD25
  MOV      RD25, R0
  FLDCI    DR10
  FMOV     DR9, DR11
  FSUB     DR10, DR11
  MOV      #0, DR12
  FMUL     DR12, DR11
  FSTCI    DR11
  MOV      R0, RD14
  MOV      RD14, RD25
  MOV      RD25, RD14
  MOV      #0x1999999A, RD7
  MOV      #6553, RD7
  DMULS    RD7, RD14
  STS      R1, RD6
  MOV      #10, RD7
  DMULU    RD7, RD6
  STS      R0, RD6
  SUB      RD6, RD14
  ADD      RD14, #48, RD26
  EXTS.B   RD26, RD14
  MOV      #176, RQ27
  ADD      R15, RQ27
  MOV.B    RD14, @RQ27
  MOV      #0, RD28
  MOV.B    RD28, @(RQ27, 1)
  FMOV.D   DR11, @(R15, 128)
  FMOV.D   DR10, @(R15, 56)
  FMOV.D   DR9, @(R15, 64)
  MOV.L    RD26, @(R15, 72)
  MOV.L    RD25, @(R15, 116)
  MOV.Q    @(R15, 136), RQ4
  MOV      RQ27, RQ5
  BSR      strcat
  BRA      L00800306

L00800307:
  BRA      L00800309

L00800305:
  FMOV.D   @(R15, 128), DR9
  FSTCI    DR9
  MOV      R0, RD14
  MOV      RD14, RD25
  MOV.Q    @(R15, 136), RQ26
  MOV.L    @(R15, 100), RD27
  MOV.B    RD27, @RQ26
  MOV      #0, RD28
  MOV.B    RD28, @(RQ26, 1)
  MOV      RD25, RD14
  MOV      #0x1999999A, RD7
  MOV      #6553, RD7
  DMULS    RD7, RD14
  STS      R1, RD6
  MOV      #10, RD7
  DMULU    RD7, RD6
  STS      R0, RD6
  SUB      RD6, RD14
  ADD      RD14, #48, RD29
  EXTS.B   RD29, RD14
  MOV      #176, RQ30
  ADD      R15, RQ30
  MOV.B    RD14, @RQ30
  MOV.B    RD28, @(RQ30, 1)
  MOV.L    RD29, @(R15, 72)
  MOV.L    RD25, @(R15, 116)
  MOV      RQ26, RQ4
  MOV      RQ30, RQ5
  BSR      strcat
  MOV      RQ26, RQ4
  MOV      L00C00090, RQ5
  BSR      strcat
  MOV.L    @(R15, 148), RD8
  MOV      RD8, RD31
  MOV.L    RD31, @(R15, 108)

L0080030A:
  MOV.L    @(R15, 108), RD16
  MOV      RD16, RD14
  ADD      #-1, RD16
  MOV.L    RD16, @(R15, 108)
  CMP/PL   RD14
  BT       L0080030B
  FMOV.D   @(R15, 128), DR9
  MOV.L    @(R15, 116), RD25
  EXTS.L   RD25
  MOV      RD25, R0
  FLDCI    DR10
  FMOV     DR9, DR11
  FSUB     DR10, DR11
  MOV      #0, DR12
  FMUL     DR12, DR11
  FSTCI    DR11
  MOV      R0, RD14
  MOV      RD14, RD25
  MOV      RD25, RD14
  MOV      #0x1999999A, RD7
  MOV      #6553, RD7
  DMULS    RD7, RD14
  STS      R1, RD6
  MOV      #10, RD7
  DMULU    RD7, RD6
  STS      R0, RD6
  SUB      RD6, RD14
  ADD      RD14, #48, RD26
  EXTS.B   RD26, RD14
  MOV      #176, RQ27
  ADD      R15, RQ27
  MOV.B    RD14, @RQ27
  MOV      #0, RD28
  MOV.B    RD28, @(RQ27, 1)
  FMOV.D   DR11, @(R15, 128)
  FMOV.D   DR10, @(R15, 56)
  FMOV.D   DR9, @(R15, 64)
  MOV.L    RD26, @(R15, 72)
  MOV.L    RD25, @(R15, 116)
  MOV.Q    @(R15, 136), RQ4
  MOV      RQ27, RQ5
  BSR      strcat
  BRA      L0080030A

L0080030B:

L00800309:

L00800304:
  MOV.L    @(R15, 104), RD16
  CMP/EQ   #0, RD16
  BT       L0080030C
  MOV      #176, RQ16
  ADD      R15, RQ16
  MOV      #69, RD17
  MOV.B    RD17, @RQ16
  CMP/PZ   RD13
  BT       L0080030D
  NEG      RD13, RD13
  MOV      #176, RQ16
  ADD      R15, RQ16
  MOV      #45, RD17
  MOV.B    RD17, @(RQ16, 1)
  BRA      L0080030E

L0080030D:
  MOV      #176, RQ16
  ADD      R15, RQ16
  MOV      #43, RD17
  MOV.B    RD17, @(RQ16, 1)

L0080030E:
  MOV      RD13, RD14
  MOV      #0x1999999A, RD7
  MOV      #6553, RD7
  DMULS    RD7, RD14
  STS      R1, RD14
  MOV      RD14, RD25
  MOV      #0x1999999A, RD7
  MOV      #6553, RD7
  DMULS    RD7, RD25
  STS      R1, RD6
  MOV      #10, RD7
  DMULU    RD7, RD6
  STS      R0, RD6
  SUB      RD6, RD25
  ADD      RD25, #48, RD14
  EXTS.B   RD14, RD25
  MOV      #176, RQ26
  ADD      R15, RQ26
  MOV.B    RD25, @(RQ26, 2)
  MOV      RD13, RD14
  MOV      #0x1999999A, RD7
  MOV      #6553, RD7
  DMULS    RD7, RD14
  STS      R1, RD6
  MOV      #10, RD7
  DMULU    RD7, RD6
  STS      R0, RD6
  SUB      RD6, RD14
  ADD      RD14, #48, RD25
  EXTS.B   RD25, RD14
  MOV.B    RD14, @(RQ26, 3)
  MOV      #0, RD27
  MOV.B    RD27, @(RQ26, 4)
  MOV.L    RD25, @(R15, 72)
  MOV.Q    @(R15, 136), RQ4
  MOV      RQ26, RQ5
  BSR      strcat
  BRA      L0080030F

L0080030C:
  MOV.L    @(R15, 164), RD16
  CMP/EQ   #71, RD16
  BT       L00800310
  MOV.L    @(R15, 164), RD16
  CMP/EQ   #103, RD16
  BT       L00800310
  BRA      L00800311

L00800310:
  MOV.Q    @(R15, 136), RQ4
  MOV      #46, RD5
  BSR      strchr
  MOV      RQ2, RQ25
  MOV      #0, RQ26
  MOV      RQ25, RQ27
  MOV      RQ26, RQ28
  MOV.Q    RQ28, @(R15, 32)
  MOV.Q    RQ27, @(R15, 40)
  MOV.Q    RQ26, @(R15, 48)
  MOV.Q    RQ25, @(R15, 80)
  CMPQ/EQ  RQ28, RQ27
  BT       L00800312
  MOV.Q    @(R15, 80), RQ25
  ADD      #1, RQ25
  MOV.Q    RQ25, @(R15, 80)
  MOV      RQ25, RQ4
  BSR      strlen
  MOV      RQ2, RQ26
  LEA.B    @(RQ25, RQ26), RQ27
  ADD      RQ27, #-1, RQ25
  MOV.Q    RQ27, @(R15, 48)
  MOV.Q    RQ26, @(R15, 24)
  MOV.Q    RQ25, @(R15, 80)

L00800313:
  MOV.Q    @(R15, 80), RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   #46, RD14
  BT       L00800314
  MOV.Q    @(R15, 80), RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   #48, RD14
  BT       L00800314
  BRA      L00800315

L00800315:
  MOV.Q    @(R15, 80), RQ16
  MOV      #0, RD17
  MOV.B    RD17, @RQ16
  ADD      #-1, RQ16
  MOV.Q    RQ16, @(R15, 80)
  BRA      L00800313

L00800314:
  MOV.Q    @(R15, 80), RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   #46, RD14
  BT       L00800316
  MOV.Q    @(R15, 80), RQ16
  MOV      #0, RD17
  MOV.B    RD17, @RQ16

L00800316:

L00800312:

L00800311:

L0080030F:
  MOV.Q    @(R15, 136), RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   #32, RD14
  BT       L00800317
  MOV.Q    @(R15, 136), RQ25
  ADD      RQ25, #1, RQ26
  MOV.Q    RQ26, @(R15, 48)
  MOV      #176, RQ4
  ADD      R15, RQ4
  MOV      RQ26, RQ5
  BSR      strcpy
  BRA      L00800318

L00800317:
  MOV      #176, RQ4
  ADD      R15, RQ4
  MOV.Q    @(R15, 136), RQ5
  BSR      strcpy

L00800318:
  MOV      #176, RQ4
  ADD      R15, RQ4
  BSR      strlen
  MOV      RQ2, RQ25
  MOV.Q    @(R15, 152), RQ26
  MOV      RQ25, RQ27
  SUB      RQ26, RQ27, RQ28
  EXTS.L   RQ28, RD29
  MOV.Q    @(R15, 136), RQ30
  MOV      #0, RD31
  MOV.B    RD31, @RQ30
  MOV.L    RD29, @(R15, 108)
  MOV.Q    RQ25, @(R15, 24)

L00800319:
  MOV.L    @(R15, 108), RD16
  CMP/GT   #0, RD16
  BT       L0080031A
  MOV.Q    @(R15, 136), RQ4
  MOV      L00C00091, RQ5
  BSR      strcat
  MOV.L    @(R15, 108), RD16
  ADD      #-1, RD16
  MOV.L    RD16, @(R15, 108)
  BRA      L00800319

L0080031A:
  MOV.Q    @(R15, 136), RQ4
  MOV      #176, RQ5
  ADD      R15, RQ5
  BSR      strcat

L00E00422:
  ADD      #256, R15

L00C00092:
  POP      FR9
  POP      FR10
  POP      FR11
  POP      FR12
  POP      FR13
  POP      FR14
  POP      FR15
  POP      RQ8
  POP      RQ9
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  POP      RQ25
  POP      RQ26
  POP      RQ27
  POP      RQ28
  POP      RQ29
  POP      RQ30
  POP      RQ31
  RET     

vvprintf:
  PUSH     LR
  BSR      L00C0007A
  ADD      #-456, R15
  MOV.Q    RQ4, @(R15, 176)
  MOV.Q    RQ5, @(R15, 168)
  MOV.Q    RQ6, @(R15, 160)
  MOV.Q    RQ7, @(R15, 152)
  MOV      #188, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 104)
  MOV      #0, RD9
  MOV      #0, RD12

L008000C3:
  CMP/EQ   RD9
  BT       L008000C4
  MOV.Q    @(R15, 176), RQ8
  MOV.B    @RQ8, RD14
  CMP/EQ   RD14
  BT       L008000C5
  MOV      #1, RD9
  BRA      L008000C6

L008000C5:
  MOV.Q    @(R15, 176), RQ31
  MOV.B    @RQ31, RD14
  CMP/EQ   #37, RD14
  BT       L008000C7
  MOV.Q    @(R15, 176), RQ30
  ADD      #1, RQ30
  MOV.Q    RQ30, @(R15, 176)
  MOV.Q    @(R15, 176), RQ29
  MOV.B    @RQ29, RD14
  CMP/EQ   #100, RD14
  BT       L008000C8
  MOV.Q    @(R15, 176), RQ28
  MOV.B    @RQ28, RD14
  CMP/EQ   #105, RD14
  BT       L008000C8
  BRA      L008000C9

L008000C8:
  MOV.Q    @(R15, 168), RQ4
  BSR      __va64_arg_i
  MOV      R2, RD14
  MOV      RD14, RD27
  MOV.L    RD27, @(R15, 148)
  MOV      #0, RD26
  CMP/GE   RD26, RD27
  BT       L008000CA
  MOV.L    @(R15, 148), RD25
  NEG      RD25, RD7
  MOV.L    RD7, @(R15, 132)
  BRA      L008000CB

L008000CA:
  MOV.L    @(R15, 148), RD5
  MOV      RD5, RD6
  MOV.L    RD6, @(R15, 132)

L008000CB:
  MOV      #188, RQ4
  ADD      R15, RQ4
  MOV      RQ4, RQ10

L008000CC:
  MOV.L    @(R15, 132), RD25
  MOV      RD25, RD14
  MOV      #0x1999999A, RD7
  MOV      #6553, RD7
  DMULU    RD7, RD14
  STS      R1, RD6
  MOV      #10, RD7
  DMULU    RD7, RD6
  STS      R0, RD6
  SUB      RD6, RD14
  ADD      RD14, #48, RD26
  EXTS.B   RD26, RD14
  MOV      RQ10, RQ13
  ADD      #1, RQ10
  MOV.B    RD14, @RQ13
  MOV      #0x1999999A, RD7
  MOV      #6553, RD7
  DMULU    RD7, RD25
  STS      R1, RD7
  MOV      RD7, RD25
  MOV.L    RD26, @(R15, 84)
  MOV.L    RD25, @(R15, 132)
  CMP/EQ   #0, RD25
  BT       L008000CC

L008000CD:
  MOV.L    @(R15, 148), RD3
  CMP/PZ   RD3
  BT       L008000CE
  MOV      RQ10, RQ13
  ADD      #1, RQ10
  MOV      #45, RD2
  MOV.B    RD2, @RQ13

L008000CE:

L008000CF:
  ADD      #-1, RQ10
  MOV      #0, RQ13
  MOV.Q    @(R15, 160), RQ23
  MOV      RQ23, RQ11
  MOV      RQ13, RQ22
  MOV.Q    RQ22, @(R15, 56)
  CMPQ/EQ  RQ22, RQ11
  BT       L008000D0
  MOV.B    @RQ10, RD14
  EXTS.B   RD14, RD21
  MOV.Q    @(R15, 152), RQ20
  MOV      RQ20, RQ13
  ADD      #1, RQ20
  MOV.B    RD21, @RQ13
  MOV.L    RD21, @(R15, 84)
  MOV.Q    RQ20, @(R15, 152)
  BRA      L008000D1

L008000D0:
  MOV.B    @RQ10, RD14
  MOV      RD14, RD4
  MOV.Q    @(R15, 160), RQ5
  BSR      fputc

L008000D1:
  ADD      #1, RD12
  MOV      RQ10, RQ13
  MOV      #188, RQ19
  ADD      R15, RQ19
  MOV      RQ19, RQ11
  CMPQ/EQ  RQ11, RQ13
  BT       L008000CF

L008000D2:
  BRA      L008000D3

L008000C9:
  MOV.Q    @(R15, 176), RQ18
  MOV.B    @RQ18, RD14
  CMP/EQ   #115, RD14
  BT       L008000D4
  MOV.Q    @(R15, 168), RQ4
  BSR      __va64_arg_l
  MOV      R2, RQ13
  MOV      RQ13, RQ25
  MOV.Q    RQ25, @(R15, 120)
  MOV      #0, RQ26
  CMPQ/EQ  RQ26, RQ25
  BT       L008000D5
  MOV      L00C00093, RQ16
  MOV      RQ16, RQ17
  MOV.Q    RQ17, @(R15, 120)

L008000D5:
  MOV.Q    @(R15, 160), RQ16
  MOV      #0, RQ17
  CMPQ/EQ  RQ17, RQ16
  BT       L008000D6
  MOV.Q    @(R15, 120), RQ4
  BSR      strlen
  MOV      RQ2, RQ25
  MOV.Q    RQ25, @(R15, 112)
  MOV.Q    @(R15, 152), RQ4
  MOV.Q    @(R15, 120), RQ5
  MOV      RQ25, RQ6
  BSR      memcpy
  MOV.Q    @(R15, 152), RQ16
  LEA.B    @(RQ16, RQ25), RQ16
  EXTS.L   RD12, RQ17
  MOV      RQ25, RQ18
  ADD      RQ17, RQ18, RQ19
  EXTS.L   RQ19, RD12
  MOV.Q    RQ19, @(R15, 48)
  MOV.Q    RQ18, @(R15, 32)
  MOV.Q    RQ17, @(R15, 40)
  MOV.Q    RQ16, @(R15, 152)
  BRA      L008000D7

L008000D6:
  MOV.Q    @(R15, 120), RQ4
  MOV.Q    @(R15, 160), RQ5
  BSR      fputs
  MOV.Q    @(R15, 120), RQ4
  BSR      strlen
  MOV      RQ2, RQ25
  EXTS.L   RD12, RQ26
  MOV      RQ25, RQ27
  ADD      RQ26, RQ27, RQ28
  EXTS.L   RQ28, RD12
  MOV.Q    RQ28, @(R15, 40)
  MOV.Q    RQ26, @(R15, 32)
  MOV.Q    RQ25, @(R15, 48)

L008000D7:
  BRA      L008000D8

L008000D4:
  MOV.Q    @(R15, 176), RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   #99, RD14
  BT       L008000D9
  MOV.Q    @(R15, 168), RQ4
  BSR      __va64_arg_i
  MOV      R2, RD14
  MOV      RD14, RD25
  MOV      #0, RQ13
  MOV.Q    @(R15, 160), RQ26
  MOV      RQ26, RQ11
  MOV      RQ13, RQ27
  MOV.Q    RQ27, @(R15, 56)
  MOV.L    RD25, @(R15, 148)
  CMPQ/EQ  RQ27, RQ11
  BT       L008000DA
  MOV.L    @(R15, 148), RD14
  EXTS.B   RD14, RD14
  MOV.Q    @(R15, 152), RQ16
  MOV      RQ16, RQ13
  ADD      #1, RQ16
  MOV.B    RD14, @RQ13
  MOV.Q    RQ16, @(R15, 152)
  BRA      L008000DB

L008000DA:
  MOV.L    @(R15, 148), RD4
  MOV.Q    @(R15, 160), RQ5
  BSR      fputc

L008000DB:
  ADD      #1, RD12
  BRA      L008000DC

L008000D9:
  MOV.Q    @(R15, 176), RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   #110, RD14
  BT       L008000DD
  MOV.Q    @(R15, 168), RQ4
  BSR      __va64_arg_l
  MOV      R2, RQ13
  MOV      RQ13, RQ25
  MOV.L    RD12, @RQ25
  MOV.Q    RQ25, @(R15, 96)
  BRA      L008000DE

L008000DD:
  MOV.Q    @(R15, 176), RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   #37, RD14
  BT       L008000DF
  MOV      #0, RQ13
  MOV.Q    @(R15, 160), RQ16
  MOV      RQ16, RQ11
  MOV      RQ13, RQ17
  MOV.Q    RQ17, @(R15, 56)
  CMPQ/EQ  RQ17, RQ11
  BT       L008000E0
  MOV      #37, RD14
  MOV.Q    @(R15, 152), RQ16
  MOV      RQ16, RQ13
  ADD      #1, RQ16
  MOV.B    RD14, @RQ13
  MOV.Q    RQ16, @(R15, 152)
  BRA      L008000E1

L008000E0:
  MOV      #37, RD4
  MOV.Q    @(R15, 160), RQ5
  BSR      fputc

L008000E1:
  ADD      #1, RD12
  BRA      L008000E2

L008000DF:
  MOV.Q    @(R15, 176), RQ25
  MOV.B    @RQ25, RD14
  MOV      L00C00094, RQ4
  MOV      RD14, RD5
  BSR      strchr
  MOV      RQ2, RQ13
  MOV      #0, RQ11
  MOV      RQ13, RQ26
  MOV      RQ11, RQ27
  MOV.Q    RQ27, @(R15, 16)
  MOV.Q    RQ26, @(R15, 56)
  CMPQ/EQ  RQ27, RQ26
  BT       L008000E3
  MOV.Q    @(R15, 176), RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   RD14
  BT       L008000E3
  BRA      L008000E4

L008000E4:
  MOV.Q    @(R15, 168), RQ4
  BSR      __va64_arg_d
  FMOV.D   DR2, @(R15, 8)
  FMOV.D   @(R15, 8), DR7
  FMOV.D   DR7, @(R15, 136)
  MOV.Q    @(R15, 176), RQ25
  MOV.B    @RQ25, RD14
  EXTS.B   RD14, RD26
  MOV      #0, RQ27
  EXTS.L   RQ27, RQ27
  MOV.Q    RQ27, @(R15, 48)
  MOV.L    RD26, @(R15, 84)
  FMOV.D   @(R15, 136), DR4
  MOV      RD26, RD4
  MOV      RQ27, RQ5
  MOV      #6, RD6
  MOV      #188, RQ7
  ADD      R15, RQ7
  BSR      dblcvt
  MOV      #188, RQ4
  ADD      R15, RQ4
  BSR      strlen
  MOV      RQ2, RQ28
  MOV      #0, RQ13
  MOV.Q    @(R15, 160), RQ29
  MOV      RQ29, RQ11
  MOV      RQ13, RQ30
  MOV.Q    RQ30, @(R15, 56)
  MOV.Q    RQ28, @(R15, 112)
  CMPQ/EQ  RQ30, RQ11
  BT       L008000E5
  MOV.Q    @(R15, 152), RQ4
  MOV      #188, RQ5
  ADD      R15, RQ5
  MOV.Q    @(R15, 112), RQ6
  BSR      memcpy
  MOV.Q    @(R15, 112), RQ16
  MOV.Q    @(R15, 152), RQ17
  LEA.B    @(RQ17, RQ16), RQ17
  MOV.Q    RQ17, @(R15, 152)
  BRA      L008000E6

L008000E5:
  MOV      #188, RQ4
  ADD      R15, RQ4
  MOV.Q    @(R15, 160), RQ5
  BSR      fputs

L008000E6:
  EXTS.L   RD12, RQ16
  MOV.Q    @(R15, 112), RQ17
  ADD      RQ16, RQ17, RQ18
  EXTS.L   RQ18, RD12
  MOV.Q    RQ18, @(R15, 48)
  MOV.Q    RQ17, @(R15, 32)
  MOV.Q    RQ16, @(R15, 40)
  BRA      L008000E7

L008000E3:
  MOV.Q    @(R15, 168), RQ25
  MOV      RQ25, RQ13
  MOV      #176, RQ11
  ADD      R15, RQ11
  MOV      RQ11, RQ4
  MOV.Q    @(R15, 160), RQ5
  MOV.Q    @(R15, 152), RQ6
  MOV      RQ13, RQ7
  MOV      RD12, RD20
  BSR      examine
  MOV      RD2, RD26
  ADD      RD26, RD12
  MOV      #0, RQ13
  MOV.Q    @(R15, 152), RQ27
  MOV      RQ27, RQ11
  MOV      RQ13, RQ28
  MOV.Q    RQ28, @(R15, 56)
  MOV.L    RD26, @(R15, 92)
  CMPQ/EQ  RQ28, RQ11
  BT       L008000E8
  MOV.L    @(R15, 92), RD16
  MOV.Q    @(R15, 152), RQ17
  EXTS.L   RD16
  LEA.B    @(RQ17, RD16), RQ17
  MOV.Q    RQ17, @(R15, 152)

L008000E8:

L008000E7:

L008000E2:

L008000DE:

L008000DC:

L008000D8:

L008000D3:
  BRA      L008000E9

L008000C7:
  MOV      #0, RQ13
  MOV.Q    @(R15, 160), RQ16
  MOV      RQ16, RQ11
  MOV      RQ13, RQ17
  MOV.Q    RQ17, @(R15, 56)
  CMPQ/EQ  RQ17, RQ11
  BT       L008000EA
  MOV.Q    @(R15, 176), RQ16
  MOV.B    @RQ16, RD17
  EXTS.B   RD17, RD18
  MOV.Q    @(R15, 152), RQ19
  MOV      RQ19, RQ13
  ADD      #1, RQ19
  MOV.B    RD18, @RQ13
  MOV.Q    RQ19, @(R15, 152)
  MOV.L    RD17, @(R15, 84)
  BRA      L008000EB

L008000EA:
  MOV.Q    @(R15, 176), RQ25
  MOV.B    @RQ25, RD26
  MOV.L    RD26, @(R15, 84)
  MOV      RD26, RD4
  MOV.Q    @(R15, 160), RQ5
  BSR      fputc

L008000EB:
  ADD      #1, RD12

L008000E9:

L008000C6:
  MOV.Q    @(R15, 176), RQ16
  ADD      #1, RQ16
  MOV.Q    RQ16, @(R15, 176)
  BRA      L008000C3

L008000C4:
  MOV      RD12, RD2

L00E00423:
  ADD      #456, R15
  BRA      L00C00085

vvscanf:
  PUSH     LR
  BSR      L00C0008E
  ADD      #-528, R15
  MOV.Q    RQ4, @(R15, 360)
  MOV.Q    RQ5, @(R15, 352)
  MOV.Q    RQ6, @(R15, 344)
  MOV.Q    RQ7, @(R15, 336)
  MOV      #0, RD14
  MOV      RD14, RD9
  MOV      #0, RD14
  MOV      RD14, RD8
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ31
  MOV      RQ31, RQ11
  MOV      RQ13, RQ10
  MOV.L    RD9, @(R15, 332)
  MOV.L    RD8, @(R15, 328)
  CMPQ/EQ  RQ10, RQ11
  BT       L008001D2
  MOV.Q    @(R15, 344), RQ4
  BSR      ftell
  MOV      RQ2, RQ30
  MOV.Q    RQ30, @(R15, 248)
  BRA      L008001D3

L008001D2:
  MOV.Q    @(R15, 336), RQ28
  MOV      RQ28, RQ29
  MOV.Q    RQ29, @(R15, 240)

L008001D3:
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ27
  MOV      RQ27, RQ11
  MOV      RQ13, RQ10
  CMPQ/EQ  RQ10, RQ11
  BT       L008001D4
  MOV.Q    @(R15, 336), RQ26
  MOV      RQ26, RQ13
  ADD      #1, RQ26
  MOV.B    @RQ13, RD14
  EXTU.B   RD14, RD12
  MOV.Q    RQ26, @(R15, 336)
  BRA      L008001D5

L008001D4:
  MOV.Q    @(R15, 344), RQ4
  BSR      fgetc
  MOV      RD2, RD12

L008001D5:
  MOV      #0, RD25
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ26
  MOV      RQ26, RQ11
  MOV      RQ13, RQ10
  MOV.L    RD25, @(R15, 224)
  CMPQ/EQ  RQ10, RQ11
  BT       L008001D6
  CMP/EQ   #-1, RD12
  BT       L008001D6
  BRA      L008001D7

L008001D6:
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ7
  MOV      RQ7, RQ11
  MOV      RQ13, RQ10
  CMPQ/EQ  RQ10, RQ11
  BT       L008001D8
  CMP/EQ   RD12
  BT       L008001D8
  BRA      L008001D7

L008001D7:
  MOV      #-1, RD2
  BRA      L00E00424

L008001D8:

L008001D9:
  MOV.L    @(R15, 332), RD6
  CMP/EQ   RD6
  BT       L008001DA
  MOV.Q    @(R15, 360), RQ5
  MOV.B    @RQ5, RD14
  CMP/EQ   RD14
  BT       L008001DB
  MOV      #1, RD4
  MOV.L    RD4, @(R15, 332)
  BRA      L008001DC

L008001DB:
  MOV.Q    @(R15, 360), RQ3
  MOV.B    @RQ3, RD14
  CMP/EQ   #37, RD14
  BT       L008001DD
  MOV.L    @(R15, 224), RD2
  CMP/EQ   RD2
  BT       L008001DD
  BRA      L008001DE

L008001DD:
  MOV.Q    @(R15, 360), RQ23
  MOV.B    @RQ23, RD14
  CMP/EQ   #37, RD14
  BT       L008001DF
  MOV.Q    @(R15, 360), RQ22
  ADD      #1, RQ22
  MOV      #0, RD21
  MOV      #0, RD20
  MOV      #0, RD19
  MOV.B    @RQ22, RD14
  MOV.Q    RQ22, @(R15, 360)
  MOV.L    RD21, @(R15, 232)
  MOV.L    RD20, @(R15, 228)
  MOV.L    RD19, @(R15, 236)
  CMP/EQ   #42, RD14
  BT       L008001E0
  MOV      #1, RD18
  MOV.Q    @(R15, 360), RQ17
  ADD      #1, RQ17
  MOV.L    RD18, @(R15, 236)
  MOV.Q    RQ17, @(R15, 360)

L008001E0:

L008001DF:
  MOV.Q    @(R15, 360), RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   #37, RD14
  BT       L008001E1
  CMP/EQ   #37, RD12
  BT       L008001E2
  MOV.L    @(R15, 328), RD2
  BRA      L00E00424

L008001E2:
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ16
  MOV      RQ16, RQ11
  MOV      RQ13, RQ10
  CMPQ/EQ  RQ10, RQ11
  BT       L008001E3
  MOV.Q    @(R15, 336), RQ16
  MOV      RQ16, RQ13
  ADD      #1, RQ16
  MOV.B    @RQ13, RD14
  EXTU.B   RD14, RD12
  MOV.Q    RQ16, @(R15, 336)
  BRA      L008001E4

L008001E3:
  MOV.Q    @(R15, 344), RQ4
  BSR      fgetc
  MOV      RD2, RD12

L008001E4:
  MOV      #0, RD16
  MOV.L    RD16, @(R15, 224)
  BRA      L008001E5

L008001E1:
  MOV.Q    @(R15, 360), RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   #108, RD14
  BT       L008001E6
  MOV      #1, RD16
  MOV      #1, RD17
  MOV.L    RD17, @(R15, 224)
  MOV.L    RD16, @(R15, 232)
  BRA      L008001E7

L008001E6:
  MOV.Q    @(R15, 360), RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   #104, RD14
  BT       L008001E8
  MOV      #1, RD16
  MOV      #1, RD17
  MOV.L    RD17, @(R15, 224)
  MOV.L    RD16, @(R15, 228)
  BRA      L008001E9

L008001E8:
  MOV      #0, RD16
  MOV.Q    @(R15, 360), RQ17
  MOV.B    @RQ17, RD14
  MOV.L    RD16, @(R15, 224)
  CMP/EQ   #115, RD14
  BT       L008001EA
  MOV.L    @(R15, 236), RD16
  CMP/EQ   #0, RD16
  BT       L008001EB
  MOV.Q    @(R15, 352), RQ4
  BSR      __va64_arg_l
  MOV      R2, RQ13
  MOV      RQ13, RQ25
  MOV.Q    RQ25, @(R15, 320)

L008001EB:

L008001EC:
  CMP/PZ   RD12
  BT       L008001ED
  MOV      RD12, RD4
  BSR      isspace
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L008001ED
  BRA      L008001EE

L008001EE:
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ16
  MOV      RQ16, RQ11
  MOV      RQ13, RQ10
  CMPQ/EQ  RQ10, RQ11
  BT       L008001EF
  MOV.Q    @(R15, 336), RQ16
  MOV      RQ16, RQ13
  ADD      #1, RQ16
  MOV.B    @RQ13, RD14
  EXTU.B   RD14, RD12
  MOV.Q    RQ16, @(R15, 336)
  BRA      L008001F0

L008001EF:
  MOV.Q    @(R15, 344), RQ4
  BSR      fgetc
  MOV      RD2, RD12

L008001F0:
  BRA      L008001EC

L008001ED:
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ16
  MOV      RQ16, RQ11
  MOV      RQ13, RQ10
  CMPQ/EQ  RQ10, RQ11
  BT       L008001F1
  CMP/EQ   #-1, RD12
  BT       L008001F1
  BRA      L008001F2

L008001F1:
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ16
  MOV      RQ16, RQ11
  MOV      RQ13, RQ10
  CMPQ/EQ  RQ10, RQ11
  BT       L008001F3
  CMP/EQ   RD12
  BT       L008001F3
  BRA      L008001F2

L008001F2:
  MOV      #1, RD16
  MOV.L    RD16, @(R15, 332)
  MOV.L    @(R15, 236), RD17
  CMP/EQ   #0, RD17
  BT       L008001F4
  MOV.Q    @(R15, 320), RQ16
  MOV      #0, RD17
  MOV.B    RD17, @RQ16

L008001F4:
  BRA      L008001F5

L008001F3:

L008001F6:
  MOV      RD12, RD4
  BSR      isspace
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L008001F7
  BRA      L008001F8

L008001F7:
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ16
  MOV      RQ16, RQ11
  MOV      RQ13, RQ10
  CMPQ/EQ  RQ10, RQ11
  BT       L008001F9
  CMP/EQ   #-1, RD12
  BT       L008001F9
  BRA      L008001FA

L008001F9:
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ16
  MOV      RQ16, RQ11
  MOV      RQ13, RQ10
  CMPQ/EQ  RQ10, RQ11
  BT       L008001FB
  CMP/EQ   RD12
  BT       L008001FB
  BRA      L008001FA

L008001FA:
  MOV      #1, RD16
  MOV.L    RD16, @(R15, 332)
  BRA      L008001F8

L008001FB:
  MOV.L    @(R15, 236), RD16
  CMP/EQ   #0, RD16
  BT       L008001FC
  EXTS.B   RD12, RD14
  MOV.Q    @(R15, 320), RQ16
  MOV      RQ16, RQ13
  ADD      #1, RQ16
  MOV.B    RD14, @RQ13
  MOV.Q    RQ16, @(R15, 320)

L008001FC:
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ16
  MOV      RQ16, RQ11
  MOV      RQ13, RQ10
  CMPQ/EQ  RQ10, RQ11
  BT       L008001FD
  MOV.Q    @(R15, 336), RQ16
  MOV      RQ16, RQ13
  ADD      #1, RQ16
  MOV.B    @RQ13, RD14
  EXTU.B   RD14, RD12
  MOV.Q    RQ16, @(R15, 336)
  BRA      L008001FE

L008001FD:
  MOV.Q    @(R15, 344), RQ4
  BSR      fgetc
  MOV      RD2, RD12

L008001FE:

L008001FF:
  BRA      L008001F6

L008001F8:
  MOV.L    @(R15, 236), RD16
  CMP/EQ   #0, RD16
  BT       L00800200
  MOV.Q    @(R15, 320), RQ16
  MOV      #0, RD17
  MOV.B    RD17, @RQ16

L00800200:
  MOV.L    @(R15, 328), RD16
  ADD      #1, RD16
  MOV.L    RD16, @(R15, 328)

L008001F5:
  BRA      L00800201

L008001EA:
  MOV.Q    @(R15, 360), RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   #91, RD14
  BT       L00800202
  MOV      #0, RD14
  MOV      RD14, RD16
  MOV      #0, RQ17
  EXTS.L   RQ17, RQ17
  MOV      RQ17, RQ18
  MOV.Q    RQ18, @(R15, 184)
  MOV.Q    RQ17, @(R15, 56)
  MOV.L    RD16, @(R15, 220)
  MOV.L    @(R15, 236), RD16
  CMP/EQ   #0, RD16
  BT       L00800203
  MOV.Q    @(R15, 352), RQ4
  BSR      __va64_arg_l
  MOV      R2, RQ13
  MOV      RQ13, RQ25
  MOV.Q    RQ25, @(R15, 320)

L00800203:
  MOV.Q    @(R15, 360), RQ16
  ADD      #1, RQ16
  MOV.B    @RQ16, RD14
  MOV.Q    RQ16, @(R15, 360)
  CMP/EQ   #94, RD14
  BT       L00800204
  MOV      #1, RD16
  MOV.Q    @(R15, 360), RQ17
  ADD      #1, RQ17
  MOV.Q    RQ17, @(R15, 360)
  MOV.L    RD16, @(R15, 220)

L00800204:
  MOV.Q    @(R15, 360), RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   RD14
  BT       L00800205
  BRA      L008001DA

L00800205:
  MOV.Q    @(R15, 360), RQ26
  MOV      RQ26, RQ25
  ADD      #1, RQ26
  MOV.Q    RQ26, @(R15, 360)
  MOV.Q    RQ25, @(R15, 208)
  MOV      RQ26, RQ4
  MOV      #93, RD5
  BSR      strchr
  MOV      RQ2, RQ25
  MOV      #0, RQ13
  MOV      RQ25, RQ11
  MOV      RQ13, RQ10
  MOV.Q    RQ25, @(R15, 200)
  CMPQ/EQ  RQ10, RQ11
  BT       L00800206
  MOV.L    @(R15, 328), RD2
  BRA      L00E00424

L00800206:
  MOV.Q    @(R15, 200), RQ16
  MOV.Q    @(R15, 208), RQ17
  SUB      RQ16, RQ17, RD14
  EXTS.L   RD14, RQ18
  MOV.Q    RQ18, @(R15, 192)

L00800207:
  MOV.Q    @(R15, 208), RQ4
  MOV      RD12, RD5
  MOV.Q    @(R15, 192), RQ6
  BSR      memchr
  MOV      RQ2, RQ13
  MOV      #0, RQ11
  MOV      RQ13, RQ7
  EXTS.L   RD7
  MOV      RD7, RD14
  MOV      RQ11, RQ7
  EXTS.L   RD7
  MOV      RD7, RD25
  CMP/EQ   RD25, RD14
  MOVNT    RQ7
  MOV      RQ7, RD26
  MOV.L    RD26, @(R15, 216)
  MOV.L    RD25, @(R15, 52)
  CMP/EQ   #0, RD26
  BT       L00800208
  MOV.L    @(R15, 220), RD16
  CMP/EQ   #0, RD16
  BT       L00800208
  BRA      L00800209

L00800209:
  BRA      L0080020A

L00800208:
  MOV.L    @(R15, 216), RD16
  CMP/EQ   #0, RD16
  BT       L0080020B
  MOV.L    @(R15, 220), RD16
  CMP/EQ   #0, RD16
  BT       L0080020B
  BRA      L0080020C

L0080020C:
  BRA      L0080020A

L0080020B:
  MOV.L    @(R15, 236), RD16
  CMP/EQ   #0, RD16
  BT       L0080020D
  EXTS.B   RD12, RD14
  MOV.Q    @(R15, 320), RQ16
  MOV      RQ16, RQ13
  ADD      #1, RQ16
  MOV.B    RD14, @RQ13
  MOV.Q    RQ16, @(R15, 320)

L0080020D:
  MOV.Q    @(R15, 184), RQ16
  ADD      #1, RQ16
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ17
  MOV      RQ17, RQ11
  MOV      RQ13, RQ10
  MOV.Q    RQ16, @(R15, 184)
  CMPQ/EQ  RQ10, RQ11
  BT       L0080020E
  MOV.Q    @(R15, 336), RQ16
  MOV      RQ16, RQ13
  ADD      #1, RQ16
  MOV.B    @RQ13, RD14
  EXTU.B   RD14, RD12
  MOV.Q    RQ16, @(R15, 336)
  BRA      L0080020F

L0080020E:
  MOV.Q    @(R15, 344), RQ4
  BSR      fgetc
  MOV      RD2, RD12

L0080020F:
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ16
  MOV      RQ16, RQ11
  MOV      RQ13, RQ10
  CMPQ/EQ  RQ10, RQ11
  BT       L00800210
  CMP/EQ   #-1, RD12
  BT       L00800210
  BRA      L00800211

L00800210:
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ16
  MOV      RQ16, RQ11
  MOV      RQ13, RQ10
  CMPQ/EQ  RQ10, RQ11
  BT       L00800212
  CMP/EQ   RD12
  BT       L00800212
  BRA      L00800211

L00800211:
  BRA      L0080020A

L00800212:
  BRA      L00800207

L0080020A:
  MOV.Q    @(R15, 184), RQ16
  MOV      #0, RQ17
  CMPQ/HI  RQ17, RQ16
  BT       L00800213
  MOV.L    @(R15, 236), RD16
  CMP/EQ   #0, RD16
  BT       L00800214
  MOV.Q    @(R15, 320), RQ16
  MOV      RQ16, RQ13
  ADD      #1, RQ16
  MOV      #0, RD17
  MOV.B    RD17, @RQ13
  MOV.Q    RQ16, @(R15, 320)

L00800214:
  MOV.L    @(R15, 328), RD16
  ADD      #1, RD16
  MOV.L    RD16, @(R15, 328)
  BRA      L00800215

L00800213:
  BRA      L008001DA

L00800215:
  MOV.Q    @(R15, 200), RQ16
  ADD      RQ16, #1, RQ17
  MOV.Q    RQ17, @(R15, 360)
  BRA      L00800216

L00800202:
  MOV.Q    @(R15, 360), RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   #99, RD14
  BT       L00800217
  MOV.L    @(R15, 236), RD16
  CMP/EQ   #0, RD16
  BT       L00800218
  MOV.Q    @(R15, 352), RQ4
  BSR      __va64_arg_l
  MOV      R2, RQ13
  MOV      RQ13, RQ25
  MOV.Q    RQ25, @(R15, 320)

L00800218:
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ16
  MOV      RQ16, RQ11
  MOV      RQ13, RQ10
  CMPQ/EQ  RQ10, RQ11
  BT       L00800219
  CMP/EQ   #-1, RD12
  BT       L00800219
  BRA      L0080021A

L00800219:
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ16
  MOV      RQ16, RQ11
  MOV      RQ13, RQ10
  CMPQ/EQ  RQ10, RQ11
  BT       L0080021B
  CMP/EQ   RD12
  BT       L0080021B
  BRA      L0080021A

L0080021A:
  MOV      #1, RD16
  MOV.L    RD16, @(R15, 332)
  BRA      L0080021C

L0080021B:
  MOV.L    @(R15, 236), RD16
  CMP/EQ   #0, RD16
  BT       L0080021D
  MOV.Q    @(R15, 320), RQ16
  MOV.B    RD12, @RQ16

L0080021D:
  MOV.L    @(R15, 328), RD16
  ADD      #1, RD16
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ17
  MOV      RQ17, RQ11
  MOV      RQ13, RQ10
  MOV.L    RD16, @(R15, 328)
  CMPQ/EQ  RQ10, RQ11
  BT       L0080021E
  MOV.Q    @(R15, 336), RQ16
  MOV      RQ16, RQ13
  ADD      #1, RQ16
  MOV.B    @RQ13, RD14
  EXTU.B   RD14, RD12
  MOV.Q    RQ16, @(R15, 336)
  BRA      L0080021F

L0080021E:
  MOV.Q    @(R15, 344), RQ4
  BSR      fgetc
  MOV      RD2, RD12

L0080021F:

L0080021C:
  BRA      L00800220

L00800217:
  MOV.Q    @(R15, 360), RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   #110, RD14
  BT       L00800221
  MOV.Q    @(R15, 352), RQ4
  BSR      __va64_arg_l
  MOV      R2, RQ13
  MOV      RQ13, RQ25
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ26
  MOV      RQ26, RQ11
  MOV      RQ13, RQ10
  MOV.Q    RQ25, @(R15, 304)
  CMPQ/EQ  RQ10, RQ11
  BT       L00800222
  MOV.Q    @(R15, 344), RQ4
  BSR      ftell
  MOV      RQ2, RQ25
  MOV      RQ25, RQ26
  MOV.Q    @(R15, 248), RQ27
  SUB      RQ26, RQ27, RQ28
  EXTU.L   RQ28, RD14
  MOV.Q    @(R15, 304), RQ29
  MOVU.L   RD14, @RQ29
  MOV.Q    RQ28, @(R15, 40)
  MOV.Q    RQ26, @(R15, 32)
  MOV.Q    RQ25, @(R15, 56)
  BRA      L00800223

L00800222:
  MOV.Q    @(R15, 240), RQ16
  MOV.Q    @(R15, 336), RQ17
  SUB      RQ16, RQ17, RD14
  MOV.Q    @(R15, 304), RQ18
  MOVU.L   RD14, @RQ18

L00800223:
  BRA      L00800224

L00800221:
  MOV.Q    @(R15, 360), RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   #100, RD14
  BT       L00800225
  MOV.Q    @(R15, 360), RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   #117, RD14
  BT       L00800225
  BRA      L00800226

L00800226:
  MOV.Q    @(R15, 360), RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   #120, RD14
  BT       L00800225
  BRA      L00800227

L00800227:
  MOV.Q    @(R15, 360), RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   #111, RD14
  BT       L00800225
  BRA      L00800228

L00800228:
  MOV.Q    @(R15, 360), RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   #112, RD14
  BT       L00800225
  BRA      L00800229

L00800229:
  MOV.Q    @(R15, 360), RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   #105, RD14
  BT       L00800225
  BRA      L0080022A

L00800225:
  MOV      #0, RD14
  MOV      RD14, RD16
  MOV      #0, RQ17
  EXTS.L   RQ17, RQ17
  MOV      RQ17, RQ18
  MOV      #0, RD14
  MOV      RD14, RD19
  MOV      #10, RD14
  MOV      RD14, RD20
  MOV      #0, RD14
  EXTS.L   RD14, RQ21
  MOV.Q    @(R15, 360), RQ22
  MOV.B    @RQ22, RD14
  MOV.Q    RQ21, @(R15, 184)
  MOV.L    RD20, @(R15, 160)
  MOV.L    RD19, @(R15, 164)
  MOV.Q    RQ18, @(R15, 168)
  MOV.Q    RQ17, @(R15, 56)
  MOV.L    RD16, @(R15, 180)
  CMP/EQ   #120, RD14
  BT       L0080022B
  MOV      #16, RD16
  MOV.L    RD16, @(R15, 160)
  BRA      L0080022C

L0080022B:
  MOV.Q    @(R15, 360), RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   #112, RD14
  BT       L0080022D
  MOV      #16, RD16
  MOV.L    RD16, @(R15, 160)
  BRA      L0080022E

L0080022D:
  MOV.Q    @(R15, 360), RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   #111, RD14
  BT       L0080022F
  MOV      #8, RD16
  MOV.L    RD16, @(R15, 160)
  BRA      L00800230

L0080022F:
  MOV.Q    @(R15, 360), RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   #105, RD14
  BT       L00800231
  MOV      #0, RD16
  MOV.L    RD16, @(R15, 160)

L00800231:

L00800230:

L0080022E:

L0080022C:
  MOV.L    @(R15, 236), RD16
  CMP/EQ   #0, RD16
  BT       L00800232
  MOV.Q    @(R15, 360), RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   #100, RD14
  BT       L00800233
  MOV.Q    @(R15, 360), RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   #105, RD14
  BT       L00800233
  BRA      L00800234

L00800233:
  MOV.L    @(R15, 232), RD16
  CMP/EQ   #0, RD16
  BT       L00800235
  MOV.Q    @(R15, 352), RQ4
  BSR      __va64_arg_l
  MOV      R2, RQ13
  MOV      RQ13, RQ25
  MOV.Q    RQ25, @(R15, 296)
  BRA      L00800236

L00800235:
  MOV.L    @(R15, 228), RD16
  CMP/EQ   #0, RD16
  BT       L00800237
  MOV.Q    @(R15, 352), RQ4
  BSR      __va64_arg_l
  MOV      R2, RQ13
  MOV      RQ13, RQ25
  MOV.Q    RQ25, @(R15, 280)
  BRA      L00800238

L00800237:
  MOV.Q    @(R15, 352), RQ4
  BSR      __va64_arg_l
  MOV      R2, RQ13
  MOV      RQ13, RQ25
  MOV.Q    RQ25, @(R15, 312)

L00800238:

L00800236:
  BRA      L00800239

L00800234:
  MOV.L    @(R15, 232), RD16
  CMP/EQ   #0, RD16
  BT       L0080023A
  MOV.Q    @(R15, 352), RQ4
  BSR      __va64_arg_l
  MOV      R2, RQ13
  MOV      RQ13, RQ25
  MOV.Q    RQ25, @(R15, 288)
  BRA      L0080023B

L0080023A:
  MOV.L    @(R15, 228), RD16
  CMP/EQ   #0, RD16
  BT       L0080023C
  MOV.Q    @(R15, 352), RQ4
  BSR      __va64_arg_l
  MOV      R2, RQ13
  MOV      RQ13, RQ25
  MOV.Q    RQ25, @(R15, 272)
  BRA      L0080023D

L0080023C:
  MOV.Q    @(R15, 352), RQ4
  BSR      __va64_arg_l
  MOV      R2, RQ13
  MOV      RQ13, RQ25
  MOV.Q    RQ25, @(R15, 304)

L0080023D:

L0080023B:

L00800239:

L00800232:

L0080023E:
  CMP/PZ   RD12
  BT       L0080023F
  MOV      RD12, RD4
  BSR      isspace
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L0080023F
  BRA      L00800240

L00800240:
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ16
  MOV      RQ16, RQ11
  MOV      RQ13, RQ10
  CMPQ/EQ  RQ10, RQ11
  BT       L00800241
  MOV.Q    @(R15, 336), RQ16
  MOV      RQ16, RQ13
  ADD      #1, RQ16
  MOV.B    @RQ13, RD14
  EXTU.B   RD14, RD12
  MOV.Q    RQ16, @(R15, 336)
  BRA      L00800242

L00800241:
  MOV.Q    @(R15, 344), RQ4
  BSR      fgetc
  MOV      RD2, RD12

L00800242:
  BRA      L0080023E

L0080023F:
  CMP/EQ   #45, RD12
  BT       L00800243
  MOV      #1, RD16
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ17
  MOV      RQ17, RQ11
  MOV      RQ13, RQ10
  MOV.L    RD16, @(R15, 180)
  CMPQ/EQ  RQ10, RQ11
  BT       L00800244
  MOV.Q    @(R15, 336), RQ16
  MOV      RQ16, RQ13
  ADD      #1, RQ16
  MOV.B    @RQ13, RD14
  EXTU.B   RD14, RD12
  MOV.Q    RQ16, @(R15, 336)
  BRA      L00800245

L00800244:
  MOV.Q    @(R15, 344), RQ4
  BSR      fgetc
  MOV      RD2, RD12

L00800245:
  BRA      L00800246

L00800243:
  CMP/EQ   #43, RD12
  BT       L00800247
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ16
  MOV      RQ16, RQ11
  MOV      RQ13, RQ10
  CMPQ/EQ  RQ10, RQ11
  BT       L00800248
  MOV.Q    @(R15, 336), RQ16
  MOV      RQ16, RQ13
  ADD      #1, RQ16
  MOV.B    @RQ13, RD14
  EXTU.B   RD14, RD12
  MOV.Q    RQ16, @(R15, 336)
  BRA      L00800249

L00800248:
  MOV.Q    @(R15, 344), RQ4
  BSR      fgetc
  MOV      RD2, RD12

L00800249:

L00800247:

L00800246:
  MOV.L    @(R15, 160), RD16
  CMP/EQ   #0, RD16
  BT       L0080024A
  MOV      #1, RD16
  MOV.L    RD16, @(R15, 164)

L0080024A:

L0080024B:
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ16
  MOV      RQ16, RQ11
  MOV      RQ13, RQ10
  CMPQ/EQ  RQ10, RQ11
  BT       L0080024C
  CMP/EQ   #-1, RD12
  BT       L0080024C
  BRA      L0080024D

L0080024C:
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ16
  MOV      RQ16, RQ11
  MOV      RQ13, RQ10
  CMPQ/EQ  RQ10, RQ11
  BT       L0080024E
  CMP/EQ   RD12
  BT       L0080024E
  BRA      L0080024D

L0080024E:
  EXTU.B   RD12, RD14
  MOV      RD14, RD4
  BSR      isdigit
  MOV      RD2, RD25
  MOV.L    RD25, @(R15, 52)
  CMP/EQ   #0, RD25
  BT       L0080024F
  MOV.L    @(R15, 160), RD16
  CMP/EQ   #0, RD16
  BT       L00800250
  CMP/EQ   #48, RD12
  BT       L00800251
  MOV      #8, RD16
  MOV.L    RD16, @(R15, 160)
  BRA      L00800252

L00800251:
  MOV      #10, RD16
  MOV      #0, RD17
  MOV.L    RD17, @(R15, 164)
  MOV.L    RD16, @(R15, 160)

L00800252:

L00800250:
  MOV.L    @(R15, 160), RD25
  EXTS.L   RQ25, RQ25
  MOV.Q    @(R15, 168), RQ26
  MOV      RQ25, RQ5
  MOV      RQ26, RQ4
  BSR      __umulsq
  MOV      RQ2, RQ26
  SUB      RD12, #48, RD14
  MOV      RQ26, RQ25
  EXTS.L   RD14, RQ26
  ADD      RQ25, RQ26, RQ27
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ28
  MOV      RQ28, RQ11
  MOV      RQ13, RQ10
  MOV.Q    RQ27, @(R15, 168)
  MOV.Q    RQ26, @(R15, 56)
  MOV.Q    RQ25, @(R15, 40)
  CMPQ/EQ  RQ10, RQ11
  BT       L00800253
  MOV.Q    @(R15, 336), RQ16
  MOV      RQ16, RQ13
  ADD      #1, RQ16
  MOV.B    @RQ13, RD14
  EXTU.B   RD14, RD12
  MOV.Q    RQ16, @(R15, 336)
  BRA      L00800254

L00800253:
  MOV.Q    @(R15, 344), RQ4
  BSR      fgetc
  MOV      RD2, RD12

L00800254:
  BRA      L00800255

L0080024F:
  EXTU.B   RD12, RD14
  MOV      RD14, RD4
  BSR      isalpha
  MOV      RD2, RD25
  MOV.L    RD25, @(R15, 52)
  CMP/EQ   #0, RD25
  BT       L00800256
  CMP/EQ   #88, RD12
  BT       L00800257
  CMP/EQ   #120, RD12
  BT       L00800257
  BRA      L00800258

L00800257:
  MOV.L    @(R15, 160), RD16
  CMP/EQ   #0, RD16
  BT       L00800259
  MOV.L    @(R15, 160), RD16
  CMP/EQ   #8, RD16
  BT       L0080025A
  MOV.L    @(R15, 164), RD16
  CMP/EQ   #0, RD16
  BT       L0080025A
  BRA      L00800259

L00800259:
  MOV      #16, RD16
  MOV      #0, RD17
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ18
  MOV      RQ18, RQ11
  MOV      RQ13, RQ10
  MOV.L    RD17, @(R15, 164)
  MOV.L    RD16, @(R15, 160)
  CMPQ/EQ  RQ10, RQ11
  BT       L0080025B
  MOV.Q    @(R15, 336), RQ16
  MOV      RQ16, RQ13
  ADD      #1, RQ16
  MOV.B    @RQ13, RD14
  EXTU.B   RD14, RD12
  MOV.Q    RQ16, @(R15, 336)
  BRA      L0080025C

L0080025B:
  MOV.Q    @(R15, 344), RQ4
  BSR      fgetc
  MOV      RD2, RD12

L0080025C:
  BRA      L0080025D

L0080025A:
  MOV.L    @(R15, 160), RD16
  CMP/EQ   #16, RD16
  BT       L0080025E
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ16
  MOV      RQ16, RQ11
  MOV      RQ13, RQ10
  CMPQ/EQ  RQ10, RQ11
  BT       L0080025F
  MOV.Q    @(R15, 336), RQ16
  MOV      RQ16, RQ13
  ADD      #1, RQ16
  MOV.B    @RQ13, RD14
  EXTU.B   RD14, RD12
  MOV.Q    RQ16, @(R15, 336)
  BRA      L00800260

L0080025F:
  MOV.Q    @(R15, 344), RQ4
  BSR      fgetc
  MOV      RD2, RD12

L00800260:
  BRA      L00800261

L0080025E:
  BRA      L0080024D

L00800261:

L0080025D:
  BRA      L00800262

L00800258:
  MOV.L    @(R15, 160), RD16
  CMP/GT   #10, RD16
  BT       L00800263
  BRA      L0080024D
  BRA      L00800264

L00800263:
  MOV.L    @(R15, 160), RD25
  EXTS.L   RQ25, RQ25
  MOV.Q    @(R15, 168), RQ26
  MOV      RQ25, RQ5
  MOV      RQ26, RQ4
  BSR      __umulsq
  MOV      RQ2, RQ26
  EXTU.B   RD12, RD14
  MOV.Q    RQ26, @(R15, 56)
  MOV.Q    RQ25, @(R15, 40)
  MOV      RD14, RD4
  BSR      toupper
  MOV      RD2, RD25
  SUB      RD25, #65, RD14
  EXTS.L   RD14, RQ27
  ADD      RQ26, RQ27, RQ28
  ADD      RQ28, #10, RQ29
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ26
  MOV      RQ26, RQ11
  MOV      RQ13, RQ10
  MOV.Q    RQ29, @(R15, 168)
  MOV.Q    RQ28, @(R15, 40)
  MOV.Q    RQ27, @(R15, 32)
  MOV.L    RD25, @(R15, 52)
  CMPQ/EQ  RQ10, RQ11
  BT       L00800265
  MOV.Q    @(R15, 336), RQ16
  MOV      RQ16, RQ13
  ADD      #1, RQ16
  MOV.B    @RQ13, RD14
  EXTU.B   RD14, RD12
  MOV.Q    RQ16, @(R15, 336)
  BRA      L00800266

L00800265:
  MOV.Q    @(R15, 344), RQ4
  BSR      fgetc
  MOV      RD2, RD12

L00800266:

L00800264:

L00800262:
  BRA      L00800267

L00800256:
  BRA      L0080024D

L00800267:

L00800255:
  MOV.Q    @(R15, 184), RQ16
  ADD      #1, RQ16
  MOV.Q    RQ16, @(R15, 184)
  BRA      L0080024B

L0080024D:
  MOV.Q    @(R15, 184), RQ16
  MOV      #0, RQ17
  CMPQ/EQ  RQ17, RQ16
  BT       L00800268
  BRA      L008001DA

L00800268:
  MOV.L    @(R15, 236), RD16
  CMP/EQ   #0, RD16
  BT       L00800269
  MOV.Q    @(R15, 360), RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   #100, RD14
  BT       L0080026A
  MOV.Q    @(R15, 360), RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   #105, RD14
  BT       L0080026A
  BRA      L0080026B

L0080026A:
  MOV.L    @(R15, 180), RD16
  CMP/EQ   #0, RD16
  BT       L0080026C
  MOV.Q    @(R15, 168), RQ16
  NEG      RQ16, RQ17
  MOV      RQ17, RQ18
  MOV.Q    RQ18, @(R15, 152)
  MOV.Q    RQ17, @(R15, 56)
  BRA      L0080026D

L0080026C:
  MOV.Q    @(R15, 168), RQ16
  MOV.Q    RQ16, @(R15, 152)

L0080026D:
  MOV.L    @(R15, 232), RD16
  CMP/EQ   #0, RD16
  BT       L0080026E
  MOV.Q    @(R15, 296), RQ16
  MOV.Q    @(R15, 152), RQ17
  MOV.Q    RQ17, @RQ16
  BRA      L0080026F

L0080026E:
  MOV.L    @(R15, 228), RD16
  CMP/EQ   #0, RD16
  BT       L00800270
  MOV.L    @(R15, 152), RD14
  EXTS.W   RD14, RD14
  MOV.Q    @(R15, 280), RQ16
  MOV.W    RD14, @RQ16
  BRA      L00800271

L00800270:
  MOV.L    @(R15, 152), RD14
  EXTS.L   RD14, RD14
  MOV.Q    @(R15, 312), RQ16
  MOV.L    RD14, @RQ16

L00800271:

L0080026F:
  BRA      L00800272

L0080026B:
  MOV.L    @(R15, 232), RD16
  CMP/EQ   #0, RD16
  BT       L00800273
  MOV.Q    @(R15, 288), RQ16
  MOV.Q    @(R15, 168), RQ17
  MOV.Q    RQ17, @RQ16
  BRA      L00800274

L00800273:
  MOV.L    @(R15, 228), RD16
  CMP/EQ   #0, RD16
  BT       L00800275
  MOV.L    @(R15, 168), RD14
  EXTU.W   RD14, RD14
  MOV.Q    @(R15, 272), RQ16
  MOVU.W   RD14, @RQ16
  BRA      L00800276

L00800275:
  MOV.L    @(R15, 168), RD14
  EXTU.L   RD14, RD14
  MOV.Q    @(R15, 304), RQ16
  MOVU.L   RD14, @RQ16

L00800276:

L00800274:

L00800272:

L00800269:
  MOV.L    @(R15, 328), RD16
  ADD      #1, RD16
  MOV.L    RD16, @(R15, 328)
  BRA      L00800277

L0080022A:
  MOV.Q    @(R15, 360), RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   #101, RD14
  BT       L00800278
  MOV.Q    @(R15, 360), RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   #102, RD14
  BT       L00800278
  BRA      L00800279

L00800279:
  MOV.Q    @(R15, 360), RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   #103, RD14
  BT       L00800278
  BRA      L0080027A

L0080027A:
  MOV.Q    @(R15, 360), RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   #69, RD14
  BT       L00800278
  BRA      L0080027B

L0080027B:
  MOV.Q    @(R15, 360), RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   #71, RD14
  BT       L00800278
  BRA      L0080027C

L00800278:
  MOV.L    @(R15, 236), RD16
  CMP/EQ   #0, RD16
  BT       L0080027D
  MOV.L    @(R15, 232), RD16
  CMP/EQ   #0, RD16
  BT       L0080027E
  MOV.Q    @(R15, 352), RQ4
  BSR      __va64_arg_l
  MOV      R2, RQ13
  MOV      RQ13, RQ25
  MOV.Q    RQ25, @(R15, 264)
  BRA      L0080027F

L0080027E:
  MOV.Q    @(R15, 352), RQ4
  BSR      __va64_arg_l
  MOV      R2, RQ13
  MOV      RQ13, RQ25
  MOV.Q    RQ25, @(R15, 256)

L0080027F:

L0080027D:
  MOV      #0, RD25
  MOV      #0, RD26
  MOV      #0, RD27
  MOV      #0, RD28
  MOV      #0, RD29
  MOV      #0, RD30
  MOV      #0, RD31
  MOV      #0, RD8
  MOV      #0, RD9
  MOV      #0, RD7
  MOV.L    RD7, @(R15, 112)
  MOV      #0, DR15
  FMOV.D   DR15, @(R15, 104)
  MOV.L    RD9, @(R15, 116)
  MOV.L    RD8, @(R15, 120)
  MOV.L    RD31, @(R15, 124)
  MOV.L    RD30, @(R15, 128)
  MOV.L    RD29, @(R15, 132)
  MOV.L    RD28, @(R15, 136)
  MOV.L    RD27, @(R15, 140)
  MOV.L    RD26, @(R15, 144)
  MOV.L    RD25, @(R15, 148)

L00800280:
  CMP/PZ   RD12
  BT       L00800281
  MOV      RD12, RD4
  BSR      isspace
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L00800281
  BRA      L00800282

L00800282:
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ16
  MOV      RQ16, RQ11
  MOV      RQ13, RQ10
  CMPQ/EQ  RQ10, RQ11
  BT       L00800283
  MOV.Q    @(R15, 336), RQ16
  MOV      RQ16, RQ13
  ADD      #1, RQ16
  MOV.B    @RQ13, RD14
  EXTU.B   RD14, RD12
  MOV.Q    RQ16, @(R15, 336)
  BRA      L00800284

L00800283:
  MOV.Q    @(R15, 344), RQ4
  BSR      fgetc
  MOV      RD2, RD12

L00800284:
  BRA      L00800280

L00800281:
  CMP/EQ   #45, RD12
  BT       L00800285
  MOV      #1, RD16
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ17
  MOV      RQ17, RQ11
  MOV      RQ13, RQ10
  MOV.L    RD16, @(R15, 148)
  CMPQ/EQ  RQ10, RQ11
  BT       L00800286
  MOV.Q    @(R15, 336), RQ16
  MOV      RQ16, RQ13
  ADD      #1, RQ16
  MOV.B    @RQ13, RD14
  EXTU.B   RD14, RD12
  MOV.Q    RQ16, @(R15, 336)
  BRA      L00800287

L00800286:
  MOV.Q    @(R15, 344), RQ4
  BSR      fgetc
  MOV      RD2, RD12

L00800287:
  BRA      L00800288

L00800285:
  CMP/EQ   #43, RD12
  BT       L00800289
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ16
  MOV      RQ16, RQ11
  MOV      RQ13, RQ10
  CMPQ/EQ  RQ10, RQ11
  BT       L0080028A
  MOV.Q    @(R15, 336), RQ16
  MOV      RQ16, RQ13
  ADD      #1, RQ16
  MOV.B    @RQ13, RD14
  EXTU.B   RD14, RD12
  MOV.Q    RQ16, @(R15, 336)
  BRA      L0080028B

L0080028A:
  MOV.Q    @(R15, 344), RQ4
  BSR      fgetc
  MOV      RD2, RD12

L0080028B:

L00800289:

L00800288:

L0080028C:
  CMP/PL   RD12
  BT       L0080028D
  CMP/EQ   #46, RD12
  BT       L0080028E
  MOV.L    @(R15, 140), RD16
  CMP/EQ   #0, RD16
  BT       L0080028E
  BRA      L0080028F

L0080028F:
  MOV.L    @(R15, 136), RD16
  CMP/EQ   #0, RD16
  BT       L0080028E
  BRA      L00800290

L00800290:
  MOV      #1, RD16
  MOV.L    RD16, @(R15, 140)
  BRA      L00800291

L0080028E:
  MOV      RD12, RD4
  BSR      isdigit
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L00800292
  MOV.L    @(R15, 136), RD16
  CMP/EQ   #0, RD16
  BT       L00800293
  MOV.L    @(R15, 128), RD16
  ADD      #1, RD16
  MOV.L    @(R15, 116), RD17
  MOV      RD17, RD14
  MOV      #10, RD18
  MUL.L    RD18, RD14, RD14
  SUB      RD12, #48, RD19
  ADD      RD14, RD19, RD17
  MOV.L    RD19, @(R15, 52)
  MOV.L    RD17, @(R15, 116)
  MOV.L    RD16, @(R15, 128)
  BRA      L00800294

L00800293:
  MOV.L    @(R15, 132), RD16
  ADD      #1, RD16
  MOV.L    RD16, @(R15, 132)
  MOV.L    @(R15, 140), RD17
  CMP/EQ   #0, RD17
  BT       L00800295
  MOV.L    @(R15, 124), RD16
  ADD      #1, RD16
  MOV.L    RD16, @(R15, 124)

L00800295:
  CMP/EQ   #48, RD12
  BT       L00800296
  FMOV.D   @(R15, 104), DR14
  MOV      #0, DR13
  FCMP/EQ  DR13, DR14
  BT       L00800296
  BRA      L00800297

L00800297:
  MOV.L    @(R15, 120), RD16
  ADD      #1, RD16
  MOV.L    RD16, @(R15, 120)
  BRA      L00800298

L00800296:

L00800299:
  MOV.L    @(R15, 120), RD16
  CMP/GT   #0, RD16
  BT       L0080029A
  MOV      #0, DR12
  FMOV.D   @(R15, 104), DR11
  FMUL     DR12, DR11
  MOV.L    @(R15, 120), RD16
  ADD      #-1, RD16
  FMOV.D   DR11, @(R15, 104)
  MOV.L    RD16, @(R15, 120)
  BRA      L00800299

L0080029A:
  FMOV.D   @(R15, 104), DR10
  MOV      #0, DR9
  FMUL     DR9, DR10
  SUB      RD12, #48, RD14
  FMOV     DR10, DR11
  EXTS.L   RD14
  MOV      RD14, R0
  FLDCI    DR10
  FMOV     DR11, DR12
  FADD     DR10, DR12
  FMOV.D   DR12, @(R15, 104)
  FMOV.D   DR11, @(R15, 8)
  FMOV.D   DR10, @(R15, 16)

L00800298:

L00800294:
  BRA      L0080029B

L00800292:
  CMP/EQ   #101, RD12
  BT       L0080029C
  CMP/EQ   #69, RD12
  BT       L0080029C
  BRA      L0080029D

L0080029C:
  MOV.L    @(R15, 136), RD16
  CMP/EQ   #0, RD16
  BT       L0080029D
  BRA      L0080029E

L0080029E:
  MOV      #1, RD16
  MOV.L    RD16, @(R15, 136)
  BRA      L0080029F

L0080029D:
  CMP/EQ   #43, RD12
  BT       L008002A0
  CMP/EQ   #45, RD12
  BT       L008002A0
  BRA      L008002A1

L008002A0:
  MOV.L    @(R15, 136), RD16
  CMP/EQ   #1, RD16
  BT       L008002A1
  BRA      L008002A2

L008002A2:
  MOV.L    @(R15, 128), RD16
  CMP/EQ   #0, RD16
  BT       L008002A1
  BRA      L008002A3

L008002A3:
  MOV.L    @(R15, 112), RD16
  CMP/EQ   #0, RD16
  BT       L008002A1
  BRA      L008002A4

L008002A4:
  MOV      #1, RD16
  MOV.L    RD16, @(R15, 112)
  CMP/EQ   #45, RD12
  BT       L008002A5
  MOV      #1, RD16
  MOV.L    RD16, @(R15, 144)

L008002A5:
  BRA      L008002A6

L008002A1:
  BRA      L0080028D

L008002A6:

L0080029F:

L0080029B:

L00800291:
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ16
  MOV      RQ16, RQ11
  MOV      RQ13, RQ10
  CMPQ/EQ  RQ10, RQ11
  BT       L008002A7
  MOV.Q    @(R15, 336), RQ16
  MOV      RQ16, RQ13
  ADD      #1, RQ16
  MOV.B    @RQ13, RD14
  EXTU.B   RD14, RD12
  MOV.Q    RQ16, @(R15, 336)
  BRA      L008002A8

L008002A7:
  MOV.Q    @(R15, 344), RQ4
  BSR      fgetc
  MOV      RD2, RD12

L008002A8:
  BRA      L0080028C

L0080028D:
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ16
  MOV      RQ16, RQ11
  MOV      RQ13, RQ10
  CMPQ/EQ  RQ10, RQ11
  BT       L008002A9
  CMP/EQ   #-1, RD12
  BT       L008002A9
  BRA      L008002AA

L008002A9:
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ16
  MOV      RQ16, RQ11
  MOV      RQ13, RQ10
  CMPQ/EQ  RQ10, RQ11
  BT       L008002AB
  CMP/EQ   RD12
  BT       L008002AB
  BRA      L008002AA

L008002AA:
  MOV      #1, RD16
  MOV.L    RD16, @(R15, 332)

L008002AB:
  MOV.L    @(R15, 132), RD16
  CMP/EQ   #0, RD16
  BT       L008002AC
  MOV.L    @(R15, 136), RD16
  CMP/EQ   #0, RD16
  BT       L008002AD
  MOV.L    @(R15, 128), RD16
  CMP/EQ   #0, RD16
  BT       L008002AD
  BRA      L008002AC

L008002AC:
  MOV.L    @(R15, 328), RD2
  BRA      L00E00424

L008002AD:
  MOV.L    @(R15, 144), RD16
  CMP/EQ   #0, RD16
  BT       L008002AE
  MOV.L    @(R15, 116), RD16
  NEG      RD16, RD16
  MOV.L    RD16, @(R15, 116)

L008002AE:
  MOV.L    @(R15, 120), RD16
  MOV.L    @(R15, 124), RD17
  SUB      RD16, RD17, RD14
  MOV.L    @(R15, 116), RD18
  ADD      RD14, RD18
  MOV.L    RD18, @(R15, 116)
  CMP/EQ   #0, RD18
  BT       L008002AF
  FMOV.D   @(R15, 104), DR9
  MOV      #0, DR10
  FCMP/EQ  DR10, DR9
  BT       L008002AF
  BRA      L008002B0

L008002B0:
  MOV      #0, RD16
  MOV.L    RD16, @(R15, 144)
  MOV.L    @(R15, 116), RD16
  MOV      #0, RD17
  CMP/GE   RD17, RD16
  BT       L008002B1
  MOV.L    @(R15, 116), RD16
  NEG      RD16, RD16
  MOV      #1, RD17
  MOV.L    RD17, @(R15, 144)
  MOV.L    RD16, @(R15, 116)

L008002B1:
  MOV      #0, DR9
  FMOV.D   DR9, @(R15, 96)

L008002B2:
  MOV.L    @(R15, 116), RD16
  TST      #1, RD16
  BT       L008002B3
  MOV.L    @(R15, 144), RD16
  CMP/EQ   #0, RD16
  BT       L008002B4
  FMOV.D   @(R15, 96), DR9
  FMOV.D   @(R15, 104), DR10
  FDIV     DR9, DR10
  FMOV.D   DR10, @(R15, 104)
  BRA      L008002B5

L008002B4:
  FMOV.D   @(R15, 96), DR9
  FMOV.D   @(R15, 104), DR10
  FMUL     DR9, DR10
  FMOV.D   DR10, @(R15, 104)

L008002B5:

L008002B3:
  MOV.L    @(R15, 116), RD16
  SHAD     #-1, RD16
  MOV.L    RD16, @(R15, 116)
  CMP/EQ   #0, RD16
  BT       L008002B6
  BRA      L008002B7

L008002B6:
  FMOV.D   @(R15, 96), DR9
  FMOV.D   @(R15, 96), DR10
  FMOV     DR9, DR11
  FMUL     DR10, DR11
  FMOV.D   DR11, @(R15, 96)
  FMOV.D   DR10, @(R15, 8)
  FMOV.D   DR9, @(R15, 16)

L008002B8:
  BRA      L008002B2

L008002B7:

L008002AF:
  MOV.L    @(R15, 148), RD16
  CMP/EQ   #0, RD16
  BT       L008002B9
  FMOV.D   @(R15, 104), DR9
  FNEG     DR9
  FMOV.D   DR9, @(R15, 104)

L008002B9:
  MOV.L    @(R15, 236), RD16
  CMP/EQ   #0, RD16
  BT       L008002BA
  MOV.L    @(R15, 232), RD16
  CMP/EQ   #0, RD16
  BT       L008002BB
  MOV.Q    @(R15, 264), RQ16
  FMOV.D   @(R15, 104), DR9
  FMOV.D   DR9, @RQ16
  BRA      L008002BC

L008002BB:
  FMOV.D   @(R15, 104), DR9
  FMOV     DR9, FR10
  MOV.Q    @(R15, 256), RQ16
  FMOV.S   FR10, @RQ16

L008002BC:

L008002BA:
  MOV.L    @(R15, 328), RD16
  ADD      #1, RD16
  MOV.L    RD16, @(R15, 328)

L0080027C:

L00800277:

L00800224:

L00800220:

L00800216:

L00800201:

L008001E9:

L008001E7:

L008001E5:
  BRA      L008002BD

L008001DE:
  MOV.Q    @(R15, 360), RQ25
  MOV.B    @RQ25, RD14
  EXTU.B   RD14, RD26
  MOV.L    RD26, @(R15, 52)
  MOV      RD26, RD4
  BSR      isspace
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L008002BE

L008002BF:
  CMP/PZ   RD12
  BT       L008002C0
  MOV      RD12, RD4
  BSR      isspace
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L008002C0
  BRA      L008002C1

L008002C1:
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ16
  MOV      RQ16, RQ11
  MOV      RQ13, RQ10
  CMPQ/EQ  RQ10, RQ11
  BT       L008002C2
  MOV.Q    @(R15, 336), RQ16
  MOV      RQ16, RQ13
  ADD      #1, RQ16
  MOV.B    @RQ13, RD14
  EXTU.B   RD14, RD12
  MOV.Q    RQ16, @(R15, 336)
  BRA      L008002C3

L008002C2:
  MOV.Q    @(R15, 344), RQ4
  BSR      fgetc
  MOV      RD2, RD12

L008002C3:
  BRA      L008002BF

L008002C0:
  BRA      L008002C4

L008002BE:
  MOV.Q    @(R15, 360), RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   RD14, RD12
  BT       L008002C5
  MOV.L    @(R15, 328), RD2
  BRA      L00E00424

L008002C5:
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ16
  MOV      RQ16, RQ11
  MOV      RQ13, RQ10
  CMPQ/EQ  RQ10, RQ11
  BT       L008002C6
  MOV.Q    @(R15, 336), RQ16
  MOV      RQ16, RQ13
  ADD      #1, RQ16
  MOV.B    @RQ13, RD14
  EXTU.B   RD14, RD12
  MOV.Q    RQ16, @(R15, 336)
  BRA      L008002C7

L008002C6:
  MOV.Q    @(R15, 344), RQ4
  BSR      fgetc
  MOV      RD2, RD12

L008002C7:

L008002C4:

L008002BD:

L008001DC:
  MOV.Q    @(R15, 360), RQ16
  ADD      #1, RQ16
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ17
  MOV      RQ17, RQ11
  MOV      RQ13, RQ10
  MOV.Q    RQ16, @(R15, 360)
  CMPQ/EQ  RQ10, RQ11
  BT       L008002C8
  CMP/EQ   #-1, RD12
  BT       L008002C8
  BRA      L008002C9

L008002C8:
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ16
  MOV      RQ16, RQ11
  MOV      RQ13, RQ10
  CMPQ/EQ  RQ10, RQ11
  BT       L008002CA
  CMP/EQ   RD12
  BT       L008002CA
  BRA      L008002C9

L008002C9:
  MOV      #1, RD16
  MOV.L    RD16, @(R15, 332)

L008002CA:
  BRA      L008001D9

L008001DA:
  MOV      #0, RQ13
  MOV.Q    @(R15, 344), RQ16
  MOV      RQ16, RQ11
  MOV      RQ13, RQ10
  CMPQ/EQ  RQ10, RQ11
  BT       L008002CB
  MOV      RD12, RD4
  MOV.Q    @(R15, 344), RQ5
  BSR      ungetc

L008002CB:
  MOV.L    @(R15, 328), RD2

L00E00424:
  ADD      #528, R15
  BRA      L00C00092

fopen2:
  PUSH     LR
  BSR      L00C00037
  ADD      #-104, R15
  BSR      checkMode
  MOV.L    TU3CD6E720/err, RD14
  CMP/EQ   RD14
  BT       L00800005
  MOV.Q    TU3CD6E720/myfile, RQ13
  ADD      RQ13, #112, RQ12
  MOV.Q    RQ12, @R15
  MOV      RQ12, RQ4
  MOV.Q    TU3CD6E720/modus, RQ5
  BSR      strcpy
  BSR      osfopen
  MOV.L    TU3CD6E720/err, RD11
  CMP/EQ   RD11
  BT       L00800006
  MOV      __userFiles, RQ10
  MOV.L    TU3CD6E720/spareSpot, RD9
  MOV.Q    TU3CD6E720/myfile, RQ11
  MOV.Q    RQ11, @(RQ10, RD9)
  MOV.L    RD9, @(RQ11, 16)
  BSR      fopen3
  BRA      L00800007

L00800006:
  MOV      L00C00099, RQ4
  BSR      tk_puts

L00800007:
  BRA      L00800008

L00800005:
  MOV      L00C0009A, RQ4
  BSR      tk_puts

L00800008:

L00E00425:
  ADD      #104, R15
  BRA      L00C00038

fopen3:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-88, R15
  MOV      #6152, RQ13
  EXTS.L   RQ13, RQ13
  MOV      RQ13, RQ4
  BSR      malloc
  MOV      RQ2, RQ14
  MOV.Q    TU3CD6E720/myfile, RQ12
  MOV.Q    RQ14, @(RQ12, 80)
  MOV.Q    @(RQ12, 80), RQ14
  MOV      #0, RQ11
  MOV      RQ14, RQ10
  MOV      RQ11, RQ9
  MOV.Q    RQ11, @(R15, 40)
  MOV.Q    RQ10, @(R15, 32)
  MOV.Q    RQ9, @(R15, 24)
  CMPQ/EQ  RQ9, RQ10
  BT       L00800009
  MOV      L00C0009B, RQ4
  BSR      tk_puts
  MOV      #1, RD8
  MOV.L    RD8, TU3CD6E720/err
  BRA      L0080000A

L00800009:
  MOV      #0, RD31
  MOV.Q    TU3CD6E720/myfile, RQ30
  MOV.L    RD31, @(RQ30, 100)
  MOV.Q    @(RQ30, 80), RQ14
  ADD      RQ14, #2, RQ29
  MOV.Q    RQ29, @(RQ30, 32)
  MOV.Q    @(RQ30, 32), RQ14
  MOV      RQ14, RQ29
  ADD      RQ14, #1, RQ28
  MOV.Q    RQ28, @(RQ30, 32)
  MOV.B    RD31, @RQ29
  MOV.Q    @(RQ30, 32), RQ14
  MOV      RQ14, RQ29
  ADD      RQ14, #1, RQ28
  MOV.Q    RQ28, @(RQ30, 32)
  MOV.B    RD31, @RQ29
  MOV      #6144, RQ13
  EXTS.L   RQ13, RQ13
  MOV.Q    RQ13, @(RQ30, 40)
  MOV.L    RD31, @(RQ30, 8)
  MOV.L    RD31, @(RQ30, 88)
  MOV.Q    @(RQ30, 32), RQ14
  MOV.Q    @(RQ30, 40), RQ13
  LEA.B    @(RQ14, RQ13), RQ29
  MOV.Q    RQ29, @(RQ30, 56)
  MOV.Q    @(RQ30, 56), RQ14
  MOV      #10, RD27
  MOV.B    RD27, @RQ14
  MOV.Q    @(RQ30, 56), RQ14
  MOV.Q    RQ14, @(RQ30, 48)
  MOV.Q    @(RQ30, 40), RQ13
  MOV      RQ13, RQ26
  NEG      RQ26, RQ13
  MOV      RQ13, RQ26
  MOV.Q    RQ26, @(RQ30, 24)
  MOV.L    RD31, @(RQ30, 64)
  MOV.L    RD31, @(RQ30, 68)
  MOV      #-1, RD25
  MOV.L    RD25, @(RQ30, 72)
  MOV.L    RD31, @(RQ30, 96)
  MOV.Q    RQ29, @(R15, 40)
  MOV.Q    RQ28, @(R15, 32)
  MOV.L    TU3CD6E720/inreopen, RD7
  CMP/EQ   RD7
  BT       L0080000B
  MOV      #0, RD6
  MOV.Q    TU3CD6E720/myfile, RQ5
  MOV.L    RD6, @(RQ5, 104)

L0080000B:
  MOV      #1, RD4
  MOV.Q    TU3CD6E720/myfile, RQ3
  MOV.L    RD4, @(RQ3, 108)
  MOV.L    @(RQ3, 12), RD2
  MOV.L    RD2, @(R15, 12)
  CMP/EQ   RD2
  BT       L0080000C
  MOV      #1, RD23
  MOV.Q    TU3CD6E720/myfile, RQ22
  MOV.L    RD23, @(RQ22, 4)
  BRA      L0080000D

L0080000C:
  MOV      #0, RD21
  MOV.Q    TU3CD6E720/myfile, RQ20
  MOV.L    RD21, @(RQ20, 4)

L0080000D:
  MOV      #2, RD19
  MOV.Q    TU3CD6E720/myfile, RQ18
  MOV.L    RD19, @(RQ18, 92)
  MOV.L    TU3CD6E720/modeType, RD17
  MOV      #8, RD16
  CMP/GE   RD16, RD17
  BT       L0080000E
  MOV.L    TU3CD6E720/modeType, RD16
  CMP/EQ   #2, RD16
  BT       L0080000F
  MOV.L    TU3CD6E720/modeType, RD16
  CMP/EQ   #3, RD16
  BT       L00800010
  MOV.L    TU3CD6E720/modeType, RD16
  CMP/EQ   #5, RD16
  BT       L00800011
  MOV.L    TU3CD6E720/modeType, RD16
  CMP/EQ   #6, RD16
  BT       L00800012
  BRA      L00800013

L0080000E:
  MOV.L    TU3CD6E720/modeType, RD16
  CMP/EQ   #8, RD16
  BT       L00800014
  MOV.L    TU3CD6E720/modeType, RD16
  CMP/EQ   #9, RD16
  BT       L00800015
  MOV.L    TU3CD6E720/modeType, RD16
  CMP/EQ   #11, RD16
  BT       L00800016
  MOV.L    TU3CD6E720/modeType, RD16
  CMP/EQ   #12, RD16
  BT       L00800017
  BRA      L00800013

L0080000F:

L00800010:

L00800011:

L00800012:

L00800014:

L00800015:

L00800016:

L00800017:
  MOV      #0, RQ13
  EXTS.L   RQ13, RQ13
  MOV.Q    TU3CD6E720/myfile, RQ16
  MOV.Q    RQ13, @(RQ16, 24)
  MOV.Q    @(RQ16, 32), RQ14
  MOV.Q    RQ14, @(RQ16, 48)
  MOV      #1, RD17
  MOV.L    RD17, @(RQ16, 92)
  BRA      L00800018

L00800013:

L00800018:
  MOV.L    TU3CD6E720/modeType, RD16
  CMP/EQ   #7, RD16
  BT       L00800019
  MOV.L    TU3CD6E720/modeType, RD16
  CMP/EQ   #8, RD16
  BT       L0080001A
  MOV.L    TU3CD6E720/modeType, RD16
  CMP/EQ   #10, RD16
  BT       L0080001B
  MOV.L    TU3CD6E720/modeType, RD16
  CMP/EQ   #11, RD16
  BT       L0080001C
  MOV.L    TU3CD6E720/modeType, RD16
  CMP/EQ   #12, RD16
  BT       L0080001D
  BRA      L0080001E

L00800019:

L0080001A:

L0080001B:

L0080001C:

L0080001D:
  MOV      #1, RD16
  MOV.Q    TU3CD6E720/myfile, RQ17
  MOV.L    RD16, @(RQ17, 96)
  BRA      L0080001E

L0080001E:

L0080000A:

L00E00426:
  ADD      #88, R15
  BRA      L00C00027

findSpareSpot:
  PUSH     LR
  BSR      L00C0008B
  ADD      #-104, R15
  MOV      #0, RD14

L0080001F:
  CMP/GE   #253, RD14
  BT       L00800020
  MOV      __userFiles, RQ13
  EXTS.L   RD14
  MOV.Q    @(RQ13, RD14), RQ12
  MOV      #0, RQ11
  MOV      RQ12, RQ10
  MOV      RQ11, RQ9
  MOV.Q    RQ10, @(R15, 8)
  MOV.Q    RQ9, @R15
  CMPQ/EQ  RQ9, RQ10
  BT       L00800021
  BRA      L00800020

L00800021:

L00800022:
  ADD      #1, RD14
  BRA      L0080001F

L00800020:
  CMP/EQ   #253, RD14
  BT       L00800023
  MOV      #1, RD8
  MOV.L    RD8, TU3CD6E720/err
  MOV      L00C0009C, RQ4
  BSR      tk_puts
  BRA      L00800024

L00800023:
  MOV      RD14, RD31
  MOV.L    RD31, TU3CD6E720/spareSpot

L00800024:

L00E00427:
  ADD      #104, R15
  BRA      L00C0008C

checkMode:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-40, R15
  MOV      #3, RQ13
  EXTS.L   RQ13, RQ13
  MOV.Q    TU3CD6E720/modus, RQ4
  MOV      L00C0009D, RQ5
  MOV      RQ13, RQ6
  BSR      strncmp
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L00800025
  MOV      #10, RD12
  MOV.L    RD12, TU3CD6E720/modeType
  BRA      L00800026

L00800025:
  MOV      #3, RQ13
  EXTS.L   RQ13, RQ13
  MOV.Q    TU3CD6E720/modus, RQ4
  MOV      L00C0009E, RQ5
  MOV      RQ13, RQ6
  BSR      strncmp
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L00800027
  MOV      #10, RD11
  MOV.L    RD11, TU3CD6E720/modeType
  BRA      L00800028

L00800027:
  MOV      #3, RQ13
  EXTS.L   RQ13, RQ13
  MOV.Q    TU3CD6E720/modus, RQ4
  MOV      L00C0009F, RQ5
  MOV      RQ13, RQ6
  BSR      strncmp
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L00800029
  MOV      #11, RD10
  MOV.L    RD10, TU3CD6E720/modeType
  BRA      L0080002A

L00800029:
  MOV      #3, RQ13
  EXTS.L   RQ13, RQ13
  MOV.Q    TU3CD6E720/modus, RQ4
  MOV      L00C000A0, RQ5
  MOV      RQ13, RQ6
  BSR      strncmp
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L0080002B
  MOV      #11, RD9
  MOV.L    RD9, TU3CD6E720/modeType
  BRA      L0080002C

L0080002B:
  MOV      #3, RQ13
  EXTS.L   RQ13, RQ13
  MOV.Q    TU3CD6E720/modus, RQ4
  MOV      L00C000A1, RQ5
  MOV      RQ13, RQ6
  BSR      strncmp
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L0080002D
  MOV      #12, RD8
  MOV.L    RD8, TU3CD6E720/modeType
  BRA      L0080002E

L0080002D:
  MOV      #3, RQ13
  EXTS.L   RQ13, RQ13
  MOV.Q    TU3CD6E720/modus, RQ4
  MOV      L00C000A2, RQ5
  MOV      RQ13, RQ6
  BSR      strncmp
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L0080002F
  MOV      #12, RD31
  MOV.L    RD31, TU3CD6E720/modeType
  BRA      L00800030

L0080002F:
  MOV      #2, RQ13
  EXTS.L   RQ13, RQ13
  MOV.Q    TU3CD6E720/modus, RQ4
  MOV      L00C000A3, RQ5
  MOV      RQ13, RQ6
  BSR      strncmp
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L00800031
  MOV      #7, RD30
  MOV.L    RD30, TU3CD6E720/modeType
  BRA      L00800032

L00800031:
  MOV      #2, RQ13
  EXTS.L   RQ13, RQ13
  MOV.Q    TU3CD6E720/modus, RQ4
  MOV      L00C000A4, RQ5
  MOV      RQ13, RQ6
  BSR      strncmp
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L00800033
  MOV      #8, RD29
  MOV.L    RD29, TU3CD6E720/modeType
  BRA      L00800034

L00800033:
  MOV      #2, RQ13
  EXTS.L   RQ13, RQ13
  MOV.Q    TU3CD6E720/modus, RQ4
  MOV      L00C000A5, RQ5
  MOV      RQ13, RQ6
  BSR      strncmp
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L00800035
  MOV      #9, RD28
  MOV.L    RD28, TU3CD6E720/modeType
  BRA      L00800036

L00800035:
  MOV      #2, RQ13
  EXTS.L   RQ13, RQ13
  MOV.Q    TU3CD6E720/modus, RQ4
  MOV      L00C0006A, RQ5
  MOV      RQ13, RQ6
  BSR      strncmp
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L00800037
  MOV      #4, RD27
  MOV.L    RD27, TU3CD6E720/modeType
  BRA      L00800038

L00800037:
  MOV      #2, RQ13
  EXTS.L   RQ13, RQ13
  MOV.Q    TU3CD6E720/modus, RQ4
  MOV      L00C000A6, RQ5
  MOV      RQ13, RQ6
  BSR      strncmp
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L00800039
  MOV      #5, RD26
  MOV.L    RD26, TU3CD6E720/modeType
  BRA      L0080003A

L00800039:
  MOV      #2, RQ13
  EXTS.L   RQ13, RQ13
  MOV.Q    TU3CD6E720/modus, RQ4
  MOV      L00C000A7, RQ5
  MOV      RQ13, RQ6
  BSR      strncmp
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L0080003B
  MOV      #6, RD25
  MOV.L    RD25, TU3CD6E720/modeType
  BRA      L0080003C

L0080003B:
  MOV      #1, RQ13
  EXTS.L   RQ13, RQ13
  MOV.Q    TU3CD6E720/modus, RQ4
  MOV      L00C000A8, RQ5
  MOV      RQ13, RQ6
  BSR      strncmp
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L0080003D
  MOV      #1, RD25
  MOV.L    RD25, TU3CD6E720/modeType
  BRA      L0080003E

L0080003D:
  MOV      #1, RQ13
  EXTS.L   RQ13, RQ13
  MOV.Q    TU3CD6E720/modus, RQ4
  MOV      L00C000A9, RQ5
  MOV      RQ13, RQ6
  BSR      strncmp
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L0080003F
  MOV      #2, RD25
  MOV.L    RD25, TU3CD6E720/modeType
  BRA      L00800040

L0080003F:
  MOV      #1, RQ13
  EXTS.L   RQ13, RQ13
  MOV.Q    TU3CD6E720/modus, RQ4
  MOV      L00C000AA, RQ5
  MOV      RQ13, RQ6
  BSR      strncmp
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L00800041
  MOV      #3, RD25
  MOV.L    RD25, TU3CD6E720/modeType
  BRA      L00800042

L00800041:
  MOV      #1, RD25
  MOV.L    RD25, TU3CD6E720/err
  BRA      L00E00428

L00800042:

L00800040:

L0080003E:

L0080003C:

L0080003A:

L00800038:

L00800036:

L00800034:

L00800032:

L00800030:

L0080002E:

L0080002C:

L0080002A:

L00800028:

L00800026:
  MOV.L    TU3CD6E720/modeType, RD7
  CMP/EQ   #4, RD7
  BT       L00800043
  MOV.L    TU3CD6E720/modeType, RD6
  CMP/EQ   #5, RD6
  BT       L00800043
  BRA      L00800044

L00800044:
  MOV.L    TU3CD6E720/modeType, RD5
  CMP/EQ   #6, RD5
  BT       L00800043
  BRA      L00800045

L00800045:
  MOV.L    TU3CD6E720/modeType, RD4
  CMP/EQ   #10, RD4
  BT       L00800043
  BRA      L00800046

L00800046:
  MOV.L    TU3CD6E720/modeType, RD3
  CMP/EQ   #11, RD3
  BT       L00800043
  BRA      L00800047

L00800047:
  MOV.L    TU3CD6E720/modeType, RD2
  CMP/EQ   #12, RD2
  BT       L00800043
  BRA      L00800048

L00800043:
  MOV      #0, RD23
  MOV.Q    TU3CD6E720/myfile, RQ22
  MOV.L    RD23, @(RQ22, 12)
  BRA      L00800049

L00800048:
  MOV      #1, RD21
  MOV.Q    TU3CD6E720/myfile, RQ20
  MOV.L    RD21, @(RQ20, 12)

L00800049:
  MOV      #0, RD19
  MOV.Q    TU3CD6E720/myfile, RQ18
  MOV.L    RD19, @(RQ18, 12)

L00E00428:
  ADD      #40, R15
  BRA      L00C00027

osfopen:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-56, R15
  MOV.L    TU3CD6E720/modeType, RD13
  CMP/EQ   #1, RD13
  BT       L0080004A
  MOV      #0, RD12
  MOV.L    RD12, @(R15, 28)
  BRA      L0080004B

L0080004A:
  MOV.L    TU3CD6E720/modeType, RD11
  CMP/EQ   #4, RD11
  BT       L0080004C
  MOV      #0, RD10
  MOV.L    RD10, @(R15, 28)
  BRA      L0080004D

L0080004C:
  MOV.L    TU3CD6E720/modeType, RD9
  CMP/EQ   #2, RD9
  BT       L0080004E
  MOV.L    TU3CD6E720/modeType, RD8
  CMP/EQ   #5, RD8
  BT       L0080004E
  BRA      L0080004F

L0080004E:
  MOV      #1, RD31
  MOV.L    RD31, @(R15, 28)
  BRA      L00800050

L0080004F:
  MOV      #2, RD30
  MOV      #1, RD29
  MOV.L    RD30, @(R15, 28)
  MOV.L    RD29, TU3CD6E720/err
  BSR      __get_errno
  MOV      RQ2, RQ14
  MOV      #2, RD28
  MOV.L    RD28, @RQ14
  MOV      L00C000AB, RQ4
  MOV.L    TU3CD6E720/modeType, RD5
  BSR      tk_printf
  BRA      L00E00429

L00800050:

L0080004D:

L0080004B:
  MOV      #24, RQ14
  ADD      R15, RQ14
  MOV      RQ14, RQ7
  EXTS.L   RD7
  MOV      RD7, RD27
  MOV.L    RD27, @(R15, 12)
  MOV.Q    TU3CD6E720/fnm, RQ4
  MOV.L    @(R15, 28), RD5
  MOV      RD27, RD6
  BSR      __open
  MOV      RD2, RD26
  MOV.Q    TU3CD6E720/myfile, RQ25
  MOV.L    RD26, @RQ25
  MOV.L    RD26, @(R15, 8)
  MOV.L    @(R15, 24), RD28
  MOV.L    @(R15, 24), RD29
  CMP/EQ   #0, RD29
  BT       L00800051
  MOV      #1, RD25
  MOV.Q    TU3CD6E720/myfile, RQ26
  MOV.L    @RQ26, RD27
  MOV.L    RD27, @(R15, 12)
  MOV.L    RD25, TU3CD6E720/err
  BSR      __get_errno
  MOV      RQ2, RQ14
  MOV.L    RD27, @RQ14
  MOV      L00C000AC, RQ4
  BSR      tk_puts

L00800051:

L00E00429:
  ADD      #56, R15
  BRA      L00C00027

fwriteSlow:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-136, R15
  MOV.Q    RQ4, @(R15, 104)
  MOV.Q    RQ5, @(R15, 96)
  MOV.Q    RQ6, @(R15, 88)
  MOV      RQ7, RQ13
  MOV.Q    RQ20, @(R15, 72)
  MOV.Q    RQ21, @(R15, 64)
  MOV.Q    @(RQ13, 48), RQ14
  MOV.Q    @(RQ13, 56), RQ9
  MOV.Q    RQ9, @(R15, 40)
  CMPQ/EQ  RQ9, RQ14
  BT       L0080009D
  MOV.Q    @(RQ13, 24), RQ12
  MOV.Q    @(RQ13, 56), RQ14
  MOV.Q    @(RQ13, 32), RQ8
  SUB      RQ14, RQ8, RD10
  EXTS.L   RD10, RQ31
  ADD      RQ12, RQ31, RQ11
  MOV.Q    RQ11, @(RQ13, 24)
  MOV.Q    @(RQ13, 32), RQ14
  MOV.Q    RQ14, @(RQ13, 48)
  MOV      #1, RD30
  MOV.L    RD30, @(RQ13, 92)
  MOV.Q    RQ8, @(R15, 40)
  MOV.Q    RQ31, @(R15, 8)

L0080009D:
  MOV.L    @(RQ13, 12), RD10
  CMP/EQ   RD10
  BT       L0080009E
  MOV.L    @(RQ13, 76), RD10
  CMP/EQ   #2, RD10
  BT       L0080009E
  BRA      L0080009F

L0080009E:
  MOV      #56, RQ14
  ADD      R15, RQ14
  MOV.Q    @(R15, 104), RQ4
  MOV      RQ13, RQ5
  MOV.Q    @(R15, 72), RQ6
  MOV      RQ14, RQ7
  BSR      fwriteSlowT
  BRA      L008000A0

L0080009F:
  MOV      #56, RQ14
  ADD      R15, RQ14
  MOV.Q    @(R15, 104), RQ4
  MOV      RQ13, RQ5
  MOV.Q    @(R15, 72), RQ6
  MOV      RQ14, RQ7
  BSR      fwriteSlowB

L008000A0:
  MOV.Q    @(R15, 88), RQ29
  CMPQ/EQ  #1, RQ29
  BT       L008000A1
  MOV.Q    @(R15, 56), RQ12
  MOV.Q    @(R15, 96), RQ11
  CMPQ/EQ  RQ11, RQ12
  BT       L008000A2
  MOV      #1, RQ12
  EXTS.L   RQ12, RQ12
  MOV.Q    @(R15, 64), RQ28
  MOV.Q    RQ12, @RQ28
  BRA      L008000A3

L008000A2:
  MOV      #0, RQ12
  EXTS.L   RQ12, RQ12
  MOV.Q    @(R15, 64), RQ27
  MOV.Q    RQ12, @RQ27

L008000A3:
  BRA      L008000A4

L008000A1:
  MOV.Q    @(R15, 96), RQ26
  CMPQ/EQ  #1, RQ26
  BT       L008000A5
  MOV.Q    @(R15, 64), RQ25
  MOV.Q    @(R15, 56), RQ7
  MOV.Q    RQ7, @RQ25
  BRA      L008000A6

L008000A5:
  MOV.Q    @(R15, 56), RQ11
  MOV.Q    @(R15, 96), RQ25
  MOV      RQ11, RQ12
  MOV      RQ25, RQ5
  MOV      RQ12, RQ4
  BSR      __sdivsq
  MOV      RQ2, RQ12
  MOV      RQ12, RQ11
  MOV.Q    @(R15, 64), RQ26
  MOV.Q    RQ11, @RQ26
  MOV.Q    RQ25, @(R15, 8)

L008000A6:

L008000A4:

L00E0042A:
  ADD      #136, R15
  BRA      L00C00027

fwriteSlowT:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-184, R15
  MOV.Q    RQ4, @(R15, 152)
  MOV.Q    RQ5, @(R15, 144)
  MOV.Q    RQ6, @(R15, 136)
  MOV.Q    RQ7, @(R15, 128)
  MOV      #0, RQ14
  EXTS.L   RQ14, RQ14
  MOV.Q    @(R15, 128), RQ31
  MOV.Q    RQ14, @RQ31
  MOV.Q    @(R15, 152), RQ29
  MOV      RQ29, RQ30
  MOV      RQ30, RQ8
  MOV      RQ8, RQ10
  SUB      RQ10, RQ30, RD12
  EXTS.L   RD12, RQ14
  MOV.Q    @(R15, 136), RQ28
  MOV      RQ14, RQ27
  SUB      RQ28, RQ27, RQ26
  MOV      RQ26, RQ14
  MOV.Q    RQ30, @(R15, 120)
  MOV.Q    RQ28, @(R15, 56)
  MOV.Q    RQ27, @(R15, 48)
  MOV      RQ10, RQ4
  MOV      #10, RD5
  MOV      RQ14, RQ6
  BSR      memchr
  MOV      RQ2, RQ13
  MOV      RQ13, RQ8

L008000A7:
  MOV      #0, RQ13
  MOV      RQ8, RQ11
  MOV      RQ13, RQ25
  MOV.Q    RQ25, @(R15, 24)
  CMPQ/EQ  RQ25, RQ11
  BT       L008000A8
  SUB      RQ8, RQ10, RD12
  EXTS.L   RD12, RQ9
  MOV      #0, RD25
  MOV.L    RD25, @(R15, 108)

L008000A9:
  MOV.L    @(R15, 108), RD7
  CMP/EQ   RD7
  BT       L008000AA
  MOV.Q    @(R15, 144), RQ25
  MOV.Q    @(RQ25, 56), RQ13
  MOV.Q    @(RQ25, 48), RQ11
  SUB      RQ13, RQ11, RD12
  EXTS.L   RD12, RQ26
  MOV      RQ9, RQ14
  MOV      RQ26, RQ27
  MOV.Q    RQ27, @(R15, 56)
  MOV.Q    RQ26, @(R15, 112)
  CMPQ/GT  RQ14, RQ27
  BT       L008000AB
  MOV.Q    @(R15, 144), RQ25
  MOV.Q    @(RQ25, 48), RQ13
  MOV      RQ13, RQ4
  MOV      RQ10, RQ5
  MOV      RQ9, RQ6
  BSR      memcpy
  MOV.Q    @(RQ25, 48), RQ13
  LEA.B    @(RQ13, RQ9), RQ11
  MOV.Q    RQ11, @(RQ25, 48)
  MOV.Q    @(R15, 128), RQ6
  MOV.Q    @RQ6, RQ14
  MOV      RQ14, RQ5
  MOV      RQ9, RQ4
  ADD      RQ5, RQ4, RQ3
  MOV      RQ3, RQ14
  MOV.Q    RQ14, @RQ6
  MOV      #1, RD2
  MOV.Q    RQ5, @(R15, 48)
  MOV.Q    RQ4, @(R15, 16)
  MOV.Q    RQ3, @(R15, 56)
  MOV.L    RD2, @(R15, 108)
  BRA      L008000AC

L008000AB:
  MOV.Q    @(R15, 144), RQ25
  MOV.Q    @(RQ25, 48), RQ13
  MOV      RQ13, RQ4
  MOV      RQ10, RQ5
  MOV.Q    @(R15, 112), RQ6
  BSR      memcpy
  MOV.Q    @(R15, 112), RQ26
  LEA.B    @(RQ10, RQ26), RQ10
  SUB      RQ26, RQ9
  MOV      #92, RQ13
  ADD      R15, RQ13
  MOV.Q    @(RQ25, 40), RQ14
  MOV.Q    @(RQ25, 32), RQ11
  MOV.L    @RQ25, RD12
  MOV      RD12, RD4
  MOV      RQ11, RQ5
  MOV      RQ14, RQ6
  MOV      RQ13, RQ7
  BSR      __write
  MOV      RD2, RD27
  EXTS.L   RD27, RQ28
  MOV.Q    RQ28, @(R15, 96)
  MOV.L    RD27, @(R15, 12)
  MOV.L    @(R15, 92), RD29
  MOV.L    @(R15, 92), RD30
  CMP/EQ   #0, RD30
  BT       L008000AD
  MOV      #1, RD23
  MOV.Q    @(R15, 144), RQ22
  MOV.L    RD23, @(RQ22, 64)
  BRA      L00E0042B
  BRA      L008000AE

L008000AD:
  MOV.Q    @(R15, 128), RQ21
  MOV.Q    @RQ21, RQ14
  MOV      RQ14, RQ20
  MOV.Q    @(R15, 112), RQ19
  ADD      RQ20, RQ19, RQ18
  MOV      RQ18, RQ14
  MOV.Q    RQ14, @RQ21
  MOV.Q    @(R15, 144), RQ17
  MOV.Q    @(RQ17, 32), RQ13
  MOV.Q    RQ13, @(RQ17, 48)
  MOV.Q    @(RQ17, 24), RQ14
  MOV      RQ14, RQ20
  MOV.Q    @(R15, 96), RQ19
  ADD      RQ20, RQ19, RQ18
  MOV      RQ18, RQ14
  MOV.Q    RQ14, @(RQ17, 24)
  MOV.Q    RQ20, @(R15, 48)
  MOV.Q    RQ19, @(R15, 16)
  MOV.Q    RQ18, @(R15, 56)

L008000AE:

L008000AC:
  BRA      L008000A9

L008000AA:
  MOV.Q    @(R15, 144), RQ16
  MOV.Q    @(RQ16, 56), RQ13
  MOV.Q    @(RQ16, 48), RQ11
  SUB      RQ13, RQ11, RD12
  EXTS.L   RD12, RQ17
  MOV.Q    RQ17, @(R15, 112)
  MOV      #3, RD18
  CMPQ/HS  RD18, RQ17
  BT       L008000AF
  MOV      #92, RQ13
  ADD      R15, RQ13
  MOV.Q    @(R15, 144), RQ25
  MOV.Q    @(RQ25, 48), RQ11
  MOV.Q    @(RQ25, 32), RQ26
  SUB      RQ11, RQ26, RD12
  EXTS.L   RD12, RQ14
  MOV.Q    @(RQ25, 32), RQ11
  MOV.L    @RQ25, RD12
  MOV.Q    RQ26, @(R15, 24)
  MOV      RD12, RD4
  MOV      RQ11, RQ5
  MOV      RQ14, RQ6
  MOV      RQ13, RQ7
  BSR      __write
  MOV      RD2, RD27
  EXTS.L   RD27, RQ28
  MOV.Q    RQ28, @(R15, 96)
  MOV.L    RD27, @(R15, 12)
  MOV.L    @(R15, 92), RD29
  MOV.L    @(R15, 92), RD30
  CMP/EQ   #0, RD30
  BT       L008000B0
  MOV      #1, RD25
  MOV.Q    @(R15, 144), RQ26
  MOV.L    RD25, @(RQ26, 64)
  BSR      __get_errno
  MOV      RQ2, RQ13
  MOV.L    @(R15, 96), RD12
  EXTS.L   RD12, RD12
  MOV.L    RD12, @RQ13
  BRA      L00E0042B

L008000B0:
  MOV.Q    @(R15, 144), RQ16
  MOV.Q    @(RQ16, 32), RQ13
  MOV.Q    RQ13, @(RQ16, 48)
  MOV.Q    @(RQ16, 24), RQ14
  MOV      RQ14, RQ17
  MOV.Q    @(R15, 96), RQ18
  ADD      RQ17, RQ18, RQ19
  MOV      RQ19, RQ14
  MOV.Q    RQ14, @(RQ16, 24)
  MOV.Q    RQ19, @(R15, 56)
  MOV.Q    RQ18, @(R15, 16)
  MOV.Q    RQ17, @(R15, 48)

L008000AF:
  MOV.Q    @(R15, 144), RQ25
  MOV.Q    @(RQ25, 48), RQ13
  MOV      #1, RQ14
  EXTS.L   RQ14, RQ14
  MOV      RQ13, RQ4
  MOV      L00C000B0, RQ5
  MOV      RQ14, RQ6
  BSR      memcpy
  MOV.Q    @(RQ25, 48), RQ13
  ADD      RQ13, #1, RQ11
  MOV.Q    RQ11, @(RQ25, 48)
  MOV.Q    @(R15, 128), RQ26
  MOV.Q    @RQ26, RQ14
  ADD      RQ14, #1, RQ27
  MOV.Q    RQ27, @RQ26
  ADD      RQ8, #1, RQ10
  MOV.Q    @(R15, 120), RQ28
  SUB      RQ10, RQ28, RD12
  EXTS.L   RD12, RQ14
  MOV.Q    @(R15, 136), RQ29
  MOV      RQ14, RQ30
  SUB      RQ29, RQ30, RQ27
  MOV      RQ27, RQ14
  MOV.Q    RQ30, @(R15, 16)
  MOV.Q    RQ29, @(R15, 48)
  MOV.Q    RQ27, @(R15, 56)
  MOV      RQ10, RQ4
  MOV      #10, RD5
  MOV      RQ14, RQ6
  BSR      memchr
  MOV      RQ2, RQ13
  MOV      RQ13, RQ8
  BRA      L008000A7

L008000A8:
  MOV.Q    @(R15, 144), RQ16
  MOV.L    @(RQ16, 76), RD12
  CMP/EQ   #2, RD12
  BT       L008000B1
  MOV.Q    @(R15, 144), RQ16
  MOV.Q    @(RQ16, 48), RQ13
  MOV.Q    @(RQ16, 32), RQ11
  CMPQ/EQ  RQ11, RQ13
  BT       L008000B1
  BRA      L008000B2

L008000B2:
  MOV.Q    @(R15, 120), RQ16
  CMPQ/EQ  RQ16, RQ10
  BT       L008000B1
  BRA      L008000B3

L008000B3:
  MOV      #92, RQ13
  ADD      R15, RQ13
  MOV.Q    @(R15, 144), RQ25
  MOV.Q    @(RQ25, 48), RQ11
  MOV.Q    @(RQ25, 32), RQ26
  SUB      RQ11, RQ26, RD12
  EXTS.L   RD12, RQ14
  MOV.Q    @(RQ25, 32), RQ11
  MOV.L    @RQ25, RD12
  MOV.Q    RQ26, @(R15, 24)
  MOV      RD12, RD4
  MOV      RQ11, RQ5
  MOV      RQ14, RQ6
  MOV      RQ13, RQ7
  BSR      __write
  MOV      RD2, RD27
  EXTS.L   RD27, RQ28
  MOV.Q    RQ28, @(R15, 96)
  MOV.L    RD27, @(R15, 12)
  MOV.L    @(R15, 92), RD29
  MOV.L    @(R15, 92), RD30
  CMP/EQ   #0, RD30
  BT       L008000B4
  MOV      #1, RD25
  MOV.Q    @(R15, 144), RQ26
  MOV.L    RD25, @(RQ26, 64)
  BSR      __get_errno
  MOV      RQ2, RQ13
  MOV.L    @(R15, 96), RD12
  EXTS.L   RD12, RD12
  MOV.L    RD12, @RQ13
  BRA      L00E0042B

L008000B4:
  MOV.Q    @(R15, 144), RQ16
  MOV.Q    @(RQ16, 32), RQ13
  MOV.Q    RQ13, @(RQ16, 48)
  MOV.Q    @(RQ16, 24), RQ14
  MOV      RQ14, RQ17
  MOV.Q    @(R15, 96), RQ18
  ADD      RQ17, RQ18, RQ19
  MOV      RQ19, RQ14
  MOV.Q    RQ14, @(RQ16, 24)
  MOV.Q    RQ19, @(R15, 48)
  MOV.Q    RQ18, @R15
  MOV.Q    RQ17, @(R15, 16)

L008000B1:
  MOV.Q    @(R15, 128), RQ16
  MOV.Q    @RQ16, RQ14
  MOV.Q    @(R15, 136), RQ17
  SUB      RQ17, RQ14, RQ9

L008000B5:
  MOV      #0, RQ16
  CMPQ/EQ  RQ16, RQ9
  BT       L008000B6
  MOV.Q    @(R15, 144), RQ16
  MOV.Q    @(RQ16, 56), RQ13
  MOV.Q    @(RQ16, 48), RQ11
  SUB      RQ13, RQ11, RD12
  EXTS.L   RD12, RQ17
  MOV      RQ9, RQ14
  MOV      RQ17, RQ18
  MOV.Q    RQ18, @(R15, 48)
  MOV.Q    RQ17, @(R15, 112)
  CMPQ/GT  RQ14, RQ18
  BT       L008000B7
  MOV.Q    @(R15, 144), RQ25
  MOV.Q    @(RQ25, 48), RQ13
  MOV      RQ13, RQ4
  MOV      RQ10, RQ5
  MOV      RQ9, RQ6
  BSR      memcpy
  MOV.Q    @(RQ25, 48), RQ13
  LEA.B    @(RQ13, RQ9), RQ11
  MOV.Q    RQ11, @(RQ25, 48)
  MOV.Q    @(R15, 128), RQ26
  MOV.Q    @RQ26, RQ14
  MOV      RQ14, RQ27
  MOV      RQ9, RQ28
  ADD      RQ27, RQ28, RQ29
  MOV      RQ29, RQ14
  MOV.Q    RQ14, @RQ26
  MOV.Q    RQ29, @(R15, 48)
  MOV.Q    RQ28, @R15
  MOV.Q    RQ27, @(R15, 16)
  BRA      L008000B8

L008000B7:
  MOV.Q    @(R15, 144), RQ25
  MOV.Q    @(RQ25, 48), RQ13
  MOV      RQ13, RQ4
  MOV      RQ10, RQ5
  MOV.Q    @(R15, 112), RQ6
  BSR      memcpy
  MOV      #92, RQ13
  ADD      R15, RQ13
  MOV.Q    @(RQ25, 40), RQ14
  MOV.Q    @(RQ25, 32), RQ11
  MOV.L    @RQ25, RD12
  MOV      RD12, RD4
  MOV      RQ11, RQ5
  MOV      RQ14, RQ6
  MOV      RQ13, RQ7
  BSR      __write
  MOV      RD2, RD26
  EXTS.L   RD26, RQ27
  MOV.Q    RQ27, @(R15, 96)
  MOV.L    RD26, @(R15, 12)
  MOV.L    @(R15, 92), RD28
  MOV.L    @(R15, 92), RD29
  CMP/EQ   #0, RD29
  BT       L008000B9
  MOV      #1, RD25
  MOV.Q    @(R15, 144), RQ26
  MOV.L    RD25, @(RQ26, 64)
  BSR      __get_errno
  MOV      RQ2, RQ13
  MOV.L    @(R15, 96), RD12
  EXTS.L   RD12, RD12
  MOV.L    RD12, @RQ13
  BRA      L00E0042B
  BRA      L008000BA

L008000B9:
  MOV.Q    @(R15, 128), RQ16
  MOV.Q    @RQ16, RQ14
  MOV      RQ14, RQ17
  MOV.Q    @(R15, 112), RQ18
  ADD      RQ17, RQ18, RQ19
  MOV      RQ19, RQ14
  MOV.Q    RQ14, @RQ16
  MOV.Q    @(R15, 144), RQ20
  MOV.Q    @(RQ20, 32), RQ13
  MOV.Q    RQ13, @(RQ20, 48)
  MOV.Q    RQ19, @(R15, 48)
  MOV.Q    RQ18, @R15
  MOV.Q    RQ17, @(R15, 16)

L008000BA:
  MOV.Q    @(R15, 144), RQ16
  MOV.Q    @(RQ16, 24), RQ14
  MOV      RQ14, RQ17
  MOV.Q    @(R15, 96), RQ18
  ADD      RQ17, RQ18, RQ19
  MOV      RQ19, RQ14
  MOV.Q    RQ14, @(RQ16, 24)
  MOV.Q    @(R15, 112), RQ20
  LEA.B    @(RQ10, RQ20), RQ10
  MOV.Q    RQ19, @(R15, 48)
  MOV.Q    RQ18, @R15
  MOV.Q    RQ17, @(R15, 16)

L008000B8:
  MOV.Q    @(R15, 128), RQ16
  MOV.Q    @RQ16, RQ14
  MOV.Q    @(R15, 136), RQ17
  SUB      RQ17, RQ14, RQ9
  BRA      L008000B5

L008000B6:
  MOV.Q    @(R15, 144), RQ16
  MOV.L    @(RQ16, 76), RD12
  CMP/EQ   #3, RD12
  BT       L008000BB
  MOV.Q    @(R15, 144), RQ16
  MOV.Q    @(RQ16, 48), RQ13
  MOV.Q    @(RQ16, 32), RQ11
  CMPQ/EQ  RQ11, RQ13
  BT       L008000BB
  BRA      L008000BC

L008000BC:
  MOV      #92, RQ13
  ADD      R15, RQ13
  MOV.Q    @(R15, 144), RQ25
  MOV.Q    @(RQ25, 48), RQ11
  MOV.Q    @(RQ25, 32), RQ26
  SUB      RQ11, RQ26, RD12
  EXTS.L   RD12, RQ14
  MOV.Q    @(RQ25, 32), RQ11
  MOV.L    @RQ25, RD12
  MOV.Q    RQ26, @(R15, 24)
  MOV      RD12, RD4
  MOV      RQ11, RQ5
  MOV      RQ14, RQ6
  MOV      RQ13, RQ7
  BSR      __write
  MOV      RD2, RD27
  EXTS.L   RD27, RQ28
  MOV.Q    RQ28, @(R15, 96)
  MOV.L    RD27, @(R15, 12)
  MOV.L    @(R15, 92), RD29
  MOV.L    @(R15, 92), RD30
  CMP/EQ   #0, RD30
  BT       L008000BD
  MOV      #1, RD25
  MOV.Q    @(R15, 144), RQ26
  MOV.L    RD25, @(RQ26, 64)
  BSR      __get_errno
  MOV      RQ2, RQ13
  MOV.L    @(R15, 96), RD12
  EXTS.L   RD12, RD12
  MOV.L    RD12, @RQ13
  BRA      L00E0042B

L008000BD:
  MOV.Q    @(R15, 144), RQ16
  MOV.Q    @(RQ16, 32), RQ13
  MOV.Q    RQ13, @(RQ16, 48)
  MOV.Q    @(RQ16, 24), RQ14
  MOV      RQ14, RQ17
  MOV.Q    @(R15, 96), RQ18
  ADD      RQ17, RQ18, RQ19
  MOV      RQ19, RQ14
  MOV.Q    RQ14, @(RQ16, 24)
  MOV.Q    RQ19, @(R15, 48)
  MOV.Q    RQ18, @R15
  MOV.Q    RQ17, @(R15, 16)

L008000BB:

L00E0042B:
  ADD      #184, R15
  BRA      L00C00027

fwriteSlowB:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-168, R15
  MOV.Q    RQ4, @(R15, 136)
  MOV      RQ5, RQ13
  MOV.Q    RQ6, @(R15, 120)
  MOV.Q    RQ7, @(R15, 112)
  MOV.Q    @(RQ13, 56), RQ14
  MOV.Q    @(RQ13, 48), RQ12
  SUB      RQ14, RQ12, RD11
  EXTS.L   RD11, RQ10
  MOV.Q    @(R15, 120), RQ9
  MOV      RQ10, RQ8
  MOV.Q    RQ9, @(R15, 64)
  MOV.Q    RQ8, @(R15, 56)
  CMPQ/GT  RQ9, RQ8
  BT       L008000BE
  MOV.Q    @(RQ13, 48), RQ14
  MOV      RQ14, RQ4
  MOV.Q    @(R15, 136), RQ5
  MOV.Q    @(R15, 120), RQ6
  BSR      memcpy
  MOV.Q    @(R15, 112), RQ31
  MOV.Q    @(R15, 120), RQ30
  MOV.Q    RQ30, @RQ31
  MOV.Q    @(RQ13, 48), RQ14
  LEA.B    @(RQ14, RQ30), RQ12
  MOV.Q    RQ12, @(RQ13, 48)
  BRA      L00E0042C

L008000BE:
  MOV.Q    @(RQ13, 48), RQ14
  MOV      RQ14, RQ4
  MOV.Q    @(R15, 136), RQ5
  MOV      RQ10, RQ6
  BSR      memcpy
  MOV      #100, RQ14
  ADD      R15, RQ14
  MOV.Q    @(RQ13, 40), RQ29
  MOV.Q    @(RQ13, 32), RQ12
  MOV.L    @RQ13, RD11
  MOV.Q    RQ29, @(R15, 64)
  MOV      RD11, RD4
  MOV      RQ12, RQ5
  MOV      RQ29, RQ6
  MOV      RQ14, RQ7
  BSR      __write
  MOV      RD2, RD28
  EXTS.L   RD28, RQ27
  MOV.L    RD28, @(R15, 52)
  MOV.Q    RQ27, @(R15, 104)
  MOV.L    @(R15, 100), RD26
  MOV.L    @(R15, 100), RD25
  CMP/EQ   #0, RD25
  BT       L008000BF
  MOV      #1, RD25
  MOV.L    RD25, @(RQ13, 64)
  BSR      __get_errno
  MOV      RQ2, RQ14
  MOV.L    @(R15, 104), RD11
  EXTS.L   RD11, RD11
  MOV.L    RD11, @RQ14
  BRA      L00E0042C

L008000BF:
  MOV.Q    @(R15, 112), RQ7
  MOV.Q    RQ10, @RQ7
  MOV.Q    @(RQ13, 32), RQ14
  MOV.Q    RQ14, @(RQ13, 48)
  MOV.Q    @(RQ13, 24), RQ6
  MOV      RQ6, RQ5
  MOV.Q    @(R15, 104), RQ4
  ADD      RQ5, RQ4, RQ3
  MOV      RQ3, RQ6
  MOV.Q    RQ6, @(RQ13, 24)
  MOV.Q    @(RQ13, 40), RQ6
  MOV.Q    @(R15, 120), RQ3
  MOV      RQ6, RQ5
  MOV.Q    RQ6, @(R15, 64)
  MOV.Q    RQ5, @(R15, 40)
  MOV.Q    RQ4, @(R15, 32)
  MOV.Q    RQ3, @(R15, 56)
  CMPQ/GT  RQ5, RQ3
  BT       L008000C0
  MOV      #1, RD25
  MOV.L    RD25, @(RQ13, 4)
  MOV      #100, RQ14
  ADD      R15, RQ14
  MOV.Q    @(R15, 112), RQ26
  MOV.Q    @RQ26, RQ27
  MOV.Q    @(R15, 120), RQ28
  MOV      RQ27, RQ29
  SUB      RQ28, RQ29, RQ30
  MOV.Q    @RQ26, RQ27
  MOV.Q    @(R15, 136), RQ31
  LEA.B    @(RQ31, RQ27), RQ12
  MOV.L    @RQ13, RD11
  MOV      RQ30, RQ27
  MOV.Q    RQ30, @(R15, 56)
  MOV.Q    RQ29, @(R15, 32)
  MOV.Q    RQ28, @(R15, 40)
  MOV.Q    RQ27, @(R15, 64)
  MOV      RD11, RD4
  MOV      RQ12, RQ5
  MOV      RQ27, RQ6
  MOV      RQ14, RQ7
  BSR      __write
  MOV      RD2, RD8
  EXTS.L   RD8, RQ9
  MOV.Q    RQ9, @(R15, 104)
  MOV.L    RD8, @(R15, 52)
  MOV.L    @(R15, 100), RD25
  MOV.L    @(R15, 100), RD26
  CMP/EQ   #0, RD26
  BT       L008000C1
  MOV      #1, RD25
  MOV.L    RD25, @(RQ13, 64)
  BSR      __get_errno
  MOV      RQ2, RQ14
  MOV.L    @(R15, 104), RD11
  EXTS.L   RD11, RD11
  MOV.L    RD11, @RQ14
  BRA      L00E0042C

L008000C1:
  MOV.Q    @(R15, 112), RQ2
  MOV.Q    @RQ2, RQ23
  MOV      RQ23, RQ22
  MOV.Q    @(R15, 104), RQ21
  ADD      RQ22, RQ21, RQ20
  MOV      RQ20, RQ23
  MOV.Q    RQ23, @RQ2
  MOV.Q    @(RQ13, 24), RQ23
  MOV      RQ23, RQ22
  MOV.Q    @(R15, 104), RQ21
  ADD      RQ22, RQ21, RQ20
  MOV      RQ20, RQ23
  MOV.Q    RQ23, @(RQ13, 24)
  MOV.Q    RQ23, @(R15, 64)
  MOV.Q    RQ22, @(R15, 32)
  MOV.Q    RQ21, @(R15, 24)
  MOV.Q    RQ20, @(R15, 40)
  BRA      L008000C2

L008000C0:
  MOV.Q    @(R15, 112), RQ25
  MOV.Q    @RQ25, RQ26
  MOV.Q    @(R15, 120), RQ27
  MOV      RQ26, RQ28
  SUB      RQ27, RQ28, RQ29
  MOV.Q    @RQ25, RQ26
  MOV.Q    @(R15, 136), RQ30
  LEA.B    @(RQ30, RQ26), RQ14
  MOV.Q    @(RQ13, 32), RQ12
  MOV      RQ29, RQ26
  MOV.Q    RQ29, @(R15, 40)
  MOV.Q    RQ28, @(R15, 24)
  MOV.Q    RQ27, @(R15, 32)
  MOV.Q    RQ26, @(R15, 64)
  MOV      RQ12, RQ4
  MOV      RQ14, RQ5
  MOV      RQ26, RQ6
  BSR      memcpy
  MOV.Q    @(RQ13, 48), RQ14
  MOV.Q    @RQ25, RQ26
  MOV.Q    @(R15, 120), RQ28
  MOV      RQ26, RQ19
  SUB      RQ28, RQ19, RQ27
  LEA.B    @(RQ14, RQ27), RQ12
  MOV.Q    RQ12, @(RQ13, 48)
  MOV.Q    @(R15, 120), RQ18
  MOV.Q    RQ18, @RQ25
  MOV.Q    RQ28, @(R15, 24)
  MOV.Q    RQ27, @(R15, 32)
  MOV.Q    RQ26, @(R15, 64)
  MOV.Q    RQ19, @(R15, 16)

L008000C2:
  MOV.Q    @(RQ13, 24), RQ17
  MOV.Q    @(R15, 112), RQ16
  MOV.Q    @RQ16, RQ18
  MOV      RQ17, RQ19
  MOV      RQ18, RQ20
  ADD      RQ19, RQ20, RQ21
  MOV      RQ21, RQ17
  MOV.Q    RQ17, @(RQ13, 24)
  MOV.Q    RQ21, @(R15, 24)
  MOV.Q    RQ19, @(R15, 16)
  MOV.Q    RQ18, @(R15, 32)
  MOV.Q    RQ17, @(R15, 64)

L00E0042C:
  ADD      #168, R15
  BRA      L00C00027

freadSlowT:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-168, R15
  MOV.Q    RQ4, @(R15, 136)
  MOV.Q    RQ5, @(R15, 128)
  MOV.Q    RQ6, @(R15, 120)
  MOV.Q    RQ7, @(R15, 112)
  MOV      #0, RD12
  MOV      RD12, RD11
  MOV      #0, RQ13
  EXTS.L   RQ13, RQ13
  MOV.Q    @(R15, 112), RQ10
  MOV.Q    RQ13, @RQ10
  MOV.L    RD11, @(R15, 108)

L00800075:
  MOV.L    @(R15, 108), RD9
  CMP/EQ   RD9
  BT       L00800076
  MOV.Q    @(R15, 128), RQ8
  MOV.Q    @(RQ8, 48), RQ14
  MOV.Q    @(RQ8, 56), RQ31
  MOV.Q    RQ31, @(R15, 32)
  CMPQ/EQ  RQ31, RQ14
  BT       L00800077
  MOV      #60, RQ14
  ADD      R15, RQ14
  MOV.Q    @(R15, 128), RQ30
  MOV.Q    @(RQ30, 40), RQ13
  MOV.Q    @(RQ30, 32), RQ29
  MOV.L    @RQ30, RD12
  MOV.Q    RQ29, @(R15, 32)
  MOV      RD12, RD4
  MOV      RQ29, RQ5
  MOV      RQ13, RQ6
  MOV      RQ14, RQ7
  BSR      __read
  MOV      RD2, RD28
  EXTS.L   RD28, RQ27
  MOV.L    RD28, @(R15, 28)
  MOV.Q    RQ27, @(R15, 64)
  MOV.L    @(R15, 60), RD26
  MOV.L    @(R15, 60), RD25
  CMP/EQ   #0, RD25
  BT       L00800078
  BSR      __get_errno
  MOV      RQ2, RQ14
  MOV.L    @(R15, 64), RD12
  EXTS.L   RD12, RD12
  MOV.L    RD12, @RQ14
  MOV      #0, RQ13
  EXTS.L   RQ13, RQ13
  MOV      RQ13, RQ25
  MOV      #1, RD26
  MOV.Q    @(R15, 128), RQ27
  MOV.L    RD26, @(RQ27, 64)
  MOV.Q    RQ25, @(R15, 64)

L00800078:
  MOV.Q    @(R15, 64), RQ7
  MOV      #0, RQ6
  CMPQ/EQ  RQ6, RQ7
  BT       L00800079
  MOV      #1, RD5
  MOV.Q    @(R15, 128), RQ4
  MOV.L    RD5, @(RQ4, 68)
  BRA      L00800076

L00800079:
  MOV.Q    @(R15, 128), RQ3
  MOV.Q    @(RQ3, 24), RQ13
  MOV.Q    @(RQ3, 48), RQ14
  MOV.Q    @(RQ3, 32), RQ2
  SUB      RQ14, RQ2, RD12
  EXTS.L   RD12, RQ23
  ADD      RQ13, RQ23, RQ22
  MOV.Q    RQ22, @(RQ3, 24)
  MOV.Q    @(RQ3, 32), RQ14
  MOV.Q    @(R15, 64), RQ21
  LEA.B    @(RQ14, RQ21), RQ2
  MOV.Q    RQ2, @(RQ3, 56)
  MOV.Q    @(RQ3, 56), RQ14
  MOV      #10, RD20
  MOV.B    RD20, @RQ14
  MOV.Q    @(RQ3, 32), RQ14
  MOV.Q    RQ14, @(RQ3, 48)
  MOV.Q    RQ2, @(R15, 32)
  MOV.Q    RQ23, @(R15, 8)
  MOV.Q    RQ22, @(R15, 16)

L00800077:
  MOV.Q    @(R15, 128), RQ25
  MOV.Q    @(RQ25, 56), RQ14
  MOV.Q    @(RQ25, 48), RQ26
  SUB      RQ14, RQ26, RD12
  EXTS.L   RD12, RQ13
  ADD      RQ13, #1, RQ27
  MOV.Q    @(R15, 112), RQ28
  MOV.Q    @RQ28, RQ13
  MOV.Q    @(R15, 120), RQ29
  SUB      RQ29, RQ13, RQ30
  MOV.Q    @(RQ25, 48), RQ14
  MOV.Q    RQ30, @(R15, 88)
  MOV.Q    RQ27, @(R15, 96)
  MOV.Q    RQ26, @(R15, 32)
  MOV      RQ14, RQ4
  MOV      #10, RD5
  MOV      RQ27, RQ6
  BSR      memchr
  MOV      RQ2, RQ31
  MOV.Q    @(RQ25, 48), RQ14
  SUB      RQ31, RQ14, RD12
  EXTS.L   RD12, RQ8
  MOV      RQ30, RQ13
  MOV      RQ8, RQ9
  MOV.Q    RQ9, @(R15, 16)
  MOV.Q    RQ8, @(R15, 72)
  MOV.Q    RQ31, @(R15, 80)
  CMPQ/GT  RQ13, RQ9
  BT       L0080007A
  MOV.Q    @(R15, 128), RQ25
  MOV.Q    @(RQ25, 48), RQ14
  MOV.Q    @(R15, 112), RQ26
  MOV.Q    @RQ26, RQ13
  MOV.Q    @(R15, 136), RQ27
  LEA.B    @(RQ27, RQ13), RQ28
  MOV.Q    RQ28, @(R15, 32)
  MOV      RQ28, RQ4
  MOV      RQ14, RQ5
  MOV.Q    @(R15, 88), RQ6
  BSR      memcpy
  MOV.Q    @(RQ25, 48), RQ14
  MOV.Q    @(R15, 88), RQ19
  LEA.B    @(RQ14, RQ19), RQ28
  MOV.Q    RQ28, @(RQ25, 48)
  MOV.Q    @RQ26, RQ13
  MOV      RQ13, RQ18
  MOV      RQ19, RQ17
  ADD      RQ18, RQ17, RQ16
  MOV      RQ16, RQ13
  MOV.Q    RQ13, @RQ26
  MOV.Q    RQ28, @(R15, 32)
  MOV.Q    RQ18, @(R15, 8)
  MOV.Q    RQ17, @R15
  MOV.Q    RQ16, @(R15, 16)
  BRA      L0080007B

L0080007A:
  MOV.Q    @(R15, 128), RQ25
  MOV.Q    @(RQ25, 48), RQ14
  MOV.Q    @(R15, 112), RQ26
  MOV.Q    @RQ26, RQ13
  MOV.Q    @(R15, 136), RQ27
  LEA.B    @(RQ27, RQ13), RQ28
  MOV.Q    RQ28, @(R15, 32)
  MOV      RQ28, RQ4
  MOV      RQ14, RQ5
  MOV.Q    @(R15, 72), RQ6
  BSR      memcpy
  MOV.Q    @(RQ25, 48), RQ14
  MOV.Q    @(R15, 72), RQ29
  LEA.B    @(RQ14, RQ29), RQ28
  MOV.Q    RQ28, @(RQ25, 48)
  MOV.Q    @RQ26, RQ13
  MOV      RQ13, RQ30
  MOV      RQ29, RQ31
  ADD      RQ30, RQ31, RQ8
  MOV      RQ8, RQ13
  MOV.Q    RQ13, @RQ26
  MOV.Q    @(RQ25, 56), RQ14
  MOV.Q    RQ8, @(R15, 16)
  MOV.Q    RQ31, @R15
  MOV.Q    RQ30, @(R15, 8)
  MOV.Q    RQ28, @(R15, 32)
  MOV.Q    @(R15, 80), RQ9
  CMPQ/EQ  RQ14, RQ9
  BT       L0080007C
  MOV.Q    @(R15, 128), RQ16
  MOV.Q    @(RQ16, 48), RQ14
  ADD      RQ14, #-1, RQ17
  MOV.B    @RQ17, RD12
  MOV.Q    RQ17, @(R15, 32)
  CMP/EQ   #13, RD12
  BT       L0080007D
  MOV.Q    @(R15, 112), RQ16
  MOV.Q    @RQ16, RQ13
  MOV.Q    @(R15, 136), RQ17
  LEA.B    @(RQ17, RQ13), RQ14
  ADD      RQ14, #-1, RQ18
  MOV      #10, RD19
  MOV.B    RD19, @RQ18
  MOV.Q    @(R15, 128), RQ20
  MOV.Q    @(RQ20, 48), RQ14
  ADD      RQ14, #1, RQ18
  MOV.Q    RQ18, @(RQ20, 48)
  MOV.Q    RQ18, @(R15, 32)
  BRA      L0080007E

L0080007D:
  MOV.Q    @(R15, 88), RQ13
  MOV.Q    @(R15, 72), RQ16
  MOV.Q    RQ16, @(R15, 16)
  CMPQ/EQ  RQ16, RQ13
  BT       L0080007F
  MOV.Q    @(R15, 112), RQ16
  MOV.Q    @RQ16, RQ13
  MOV.Q    @(R15, 136), RQ17
  LEA.B    @(RQ17, RQ13), RQ14
  MOV      #10, RD18
  MOV.B    RD18, @RQ14
  MOV.Q    @RQ16, RQ13
  ADD      RQ13, #1, RQ19
  MOV.Q    RQ19, @RQ16
  MOV.Q    @(R15, 128), RQ20
  MOV.Q    @(RQ20, 48), RQ14
  ADD      RQ14, #1, RQ21
  MOV.Q    RQ21, @(RQ20, 48)
  MOV.Q    RQ21, @(R15, 32)
  MOV.Q    RQ19, @(R15, 16)

L0080007F:

L0080007E:
  BRA      L00800080

L0080007C:
  MOV.Q    @(R15, 128), RQ16
  MOV.Q    @(RQ16, 48), RQ14
  ADD      RQ14, #-1, RQ17
  MOV.B    @RQ17, RD12
  MOV.Q    RQ17, @(R15, 32)
  CMP/EQ   #13, RD12
  BT       L00800081
  MOV.Q    @(R15, 112), RQ16
  MOV.Q    @RQ16, RQ13
  SUB      RQ13, #1, RQ17
  MOV.Q    RQ17, @RQ16
  MOV.Q    RQ17, @(R15, 16)

L00800081:

L00800080:

L0080007B:
  MOV.Q    @(R15, 112), RQ16
  MOV.Q    @RQ16, RQ13
  MOV      RQ13, RQ17
  MOV.Q    @(R15, 120), RQ18
  MOV.Q    RQ18, @(R15, 8)
  MOV.Q    RQ17, @(R15, 16)
  CMPQ/EQ  RQ18, RQ17
  BT       L00800082
  MOV      #1, RD16
  MOV.L    RD16, @(R15, 108)

L00800082:
  BRA      L00800075

L00800076:

L00E0042D:
  ADD      #168, R15
  BRA      L00C00027

freadSlowB:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-152, R15
  MOV.Q    RQ4, @(R15, 120)
  MOV      RQ5, RQ12
  MOV.Q    RQ6, @(R15, 104)
  MOV.Q    RQ7, @(R15, 96)
  MOV.Q    @(RQ12, 56), RQ13
  MOV.Q    @(RQ12, 48), RQ10
  SUB      RQ13, RQ10, RD9
  EXTS.L   RD9, RQ31
  MOV.Q    @(RQ12, 48), RQ13
  MOV.Q    RQ31, @(R15, 88)
  MOV.Q    @(R15, 120), RQ4
  MOV      RQ13, RQ5
  MOV      RQ31, RQ6
  BSR      memcpy
  MOV.Q    @(R15, 96), RQ30
  MOV.Q    RQ31, @RQ30
  MOV.Q    @(RQ12, 24), RQ14
  MOV.Q    @(RQ12, 56), RQ13
  MOV.Q    @(RQ12, 32), RQ10
  SUB      RQ13, RQ10, RD9
  EXTS.L   RD9, RQ11
  ADD      RQ14, RQ11, RQ29
  MOV.Q    RQ29, @(RQ12, 24)
  MOV.Q    @(RQ12, 40), RQ14
  MOV.Q    @(R15, 104), RQ29
  MOV      RQ14, RQ11
  MOV.Q    RQ29, @(R15, 32)
  CMPQ/GT  RQ29, RQ11
  BT       L00800083
  MOV.Q    @(RQ12, 56), RQ13
  MOV.Q    RQ13, @(RQ12, 48)
  MOV      #1, RD28
  MOV.L    RD28, @(RQ12, 4)
  MOV      #84, RQ13
  ADD      R15, RQ13
  MOV.Q    @(R15, 96), RQ27
  MOV.Q    @RQ27, RQ14
  MOV.Q    @(R15, 104), RQ11
  MOV      RQ14, RQ26
  SUB      RQ11, RQ26, RQ25
  MOV.Q    @RQ27, RQ14
  MOV.Q    @(R15, 120), RQ29
  LEA.B    @(RQ29, RQ14), RQ10
  MOV.L    @RQ12, RD9
  MOV      RQ25, RQ14
  MOV.Q    RQ26, @(R15, 16)
  MOV.Q    RQ25, @(R15, 32)
  MOV      RD9, RD4
  MOV      RQ10, RQ5
  MOV      RQ14, RQ6
  MOV      RQ13, RQ7
  BSR      __read
  MOV      RD2, RD30
  EXTS.L   RD30, RQ8
  MOV.L    RD30, @(R15, 12)
  MOV.L    @(R15, 84), RD31
  MOV.L    @(R15, 84), RD28
  CMP/EQ   #0, RD28
  BT       L00800084
  BSR      __get_errno
  MOV      RQ2, RQ13
  EXTS.L   RQ8, RD9
  MOV.L    RD9, @RQ13
  MOV      #0, RQ14
  EXTS.L   RQ14, RQ14
  MOV      RQ14, RQ8
  MOV      #1, RD25
  MOV.L    RD25, @(RQ12, 64)
  BRA      L00800085

L00800084:
  MOV.Q    @(R15, 96), RQ7
  MOV.Q    @RQ7, RQ14
  MOV.Q    @(R15, 104), RQ6
  MOV      RQ14, RQ5
  SUB      RQ6, RQ5, RQ11
  MOV      RQ8, RQ14
  MOV      RQ11, RQ6
  MOV.Q    RQ6, @(R15, 16)
  MOV.Q    RQ5, @R15
  CMPQ/EQ  RQ6, RQ14
  BT       L00800086
  MOV      #1, RD4
  MOV.L    RD4, @(RQ12, 68)

L00800086:

L00800085:
  MOV.Q    @(R15, 96), RQ3
  MOV.Q    @RQ3, RQ14
  MOV      RQ14, RQ2
  MOV      RQ8, RQ23
  ADD      RQ2, RQ23, RQ11
  MOV      RQ11, RQ14
  MOV.Q    RQ14, @RQ3
  MOV.Q    @(RQ12, 24), RQ14
  MOV      RQ14, RQ2
  MOV      RQ8, RQ23
  ADD      RQ2, RQ23, RQ11
  MOV      RQ11, RQ14
  MOV.Q    RQ14, @(RQ12, 24)
  MOV.Q    RQ2, @(R15, 16)
  MOV.Q    RQ23, @R15
  BRA      L00800087

L00800083:
  MOV.Q    @(RQ12, 32), RQ13
  MOV.Q    RQ13, @(RQ12, 48)
  MOV      #84, RQ13
  ADD      R15, RQ13
  MOV.Q    @(RQ12, 40), RQ14
  MOV.Q    @(RQ12, 32), RQ10
  MOV.L    @RQ12, RD9
  MOV      RD9, RD4
  MOV      RQ10, RQ5
  MOV      RQ14, RQ6
  MOV      RQ13, RQ7
  BSR      __read
  MOV      RD2, RD25
  EXTS.L   RD25, RQ8
  MOV.Q    @(R15, 96), RQ26
  MOV.Q    @RQ26, RQ14
  MOV.Q    @(R15, 104), RQ27
  SUB      RQ27, RQ14, RQ28
  MOV.Q    RQ28, @(R15, 72)
  MOV.L    RD25, @(R15, 12)
  MOV.L    @(R15, 84), RD29
  MOV.L    @(R15, 84), RD30
  CMP/EQ   #0, RD30
  BT       L00800088
  BSR      __get_errno
  MOV      RQ2, RQ13
  EXTS.L   RQ8, RD9
  MOV.L    RD9, @RQ13
  MOV      #0, RQ14
  EXTS.L   RQ14, RQ14
  MOV      RQ14, RQ8
  MOV      #1, RD25
  MOV.L    RD25, @(RQ12, 64)
  BRA      L00800089

L00800088:
  MOV      RQ8, RQ14
  MOV.Q    @(R15, 72), RQ11
  CMPQ/GT  RQ14, RQ11
  BT       L0080008A
  MOV      #1, RD22
  MOV.L    RD22, @(RQ12, 68)

L0080008A:

L00800089:
  MOV.Q    @(RQ12, 32), RQ13
  LEA.B    @(RQ13, RQ8), RQ10
  MOV.Q    RQ10, @(RQ12, 56)
  MOV.Q    @(RQ12, 56), RQ13
  MOV      #10, RD21
  MOV.B    RD21, @RQ13
  MOV.Q    @(RQ12, 56), RQ13
  MOV.Q    @(RQ12, 48), RQ10
  SUB      RQ13, RQ10, RD9
  EXTS.L   RD9, RQ20
  MOV      RQ20, RQ14
  MOV.Q    @(R15, 72), RQ11
  MOV.Q    RQ20, @(R15, 88)
  CMPQ/GT  RQ11, RQ14
  BT       L0080008B
  MOV.Q    @(R15, 72), RQ19
  MOV.Q    RQ19, @(R15, 88)

L0080008B:
  MOV.Q    @(RQ12, 48), RQ13
  MOV.Q    @(R15, 96), RQ25
  MOV.Q    @RQ25, RQ14
  MOV.Q    @(R15, 120), RQ26
  LEA.B    @(RQ26, RQ14), RQ10
  MOV      RQ10, RQ4
  MOV      RQ13, RQ5
  MOV.Q    @(R15, 88), RQ6
  BSR      memcpy
  MOV.Q    @(RQ12, 48), RQ13
  MOV.Q    @(R15, 88), RQ18
  LEA.B    @(RQ13, RQ18), RQ10
  MOV.Q    RQ10, @(RQ12, 48)
  MOV.Q    @RQ25, RQ14
  MOV      RQ14, RQ17
  MOV      RQ18, RQ16
  ADD      RQ17, RQ16, RQ11
  MOV      RQ11, RQ14
  MOV.Q    RQ14, @RQ25
  MOV.Q    RQ17, @(R15, 16)
  MOV.Q    RQ16, @R15

L00800087:

L00E0042E:
  ADD      #152, R15
  BRA      L00C00027

examine:
  PUSH     LR
  BSR      L00C0007A
  ADD      #-552, R15
  MOV.Q    RQ4, @(R15, 272)
  MOV.Q    RQ5, @(R15, 264)
  MOV.Q    RQ6, @(R15, 256)
  MOV.Q    RQ7, @(R15, 248)
  MOV.L    RD20, @(R15, 244)
  MOV      #284, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 144)
  MOV      #0, RD14
  MOV      RD14, RD9
  MOV      #0, RD14
  MOV      RD14, RD8
  MOV      #0, RD14
  MOV      RD14, RD31
  MOV      #0, RD14
  MOV      RD14, RD30
  MOV      #0, RD14
  MOV      RD14, RD29
  MOV      #0, RD14
  MOV      RD14, RD28
  MOV      #0, RD14
  MOV      RD14, RD27
  MOV      #-1, RD14
  MOV      RD14, RD26
  MOV      #0, RD14
  MOV      RD14, RD25
  MOV      #0, RD14
  MOV.L    RD25, @(R15, 208)
  MOV      RD14, RD25
  MOV      #0, RD14
  MOV.L    RD26, @(R15, 212)
  MOV      RD14, RD26
  MOV.L    RD27, @(R15, 216)
  MOV.Q    @(R15, 272), RQ27
  MOV.Q    @RQ27, RQ13
  MOV      RQ13, RQ11
  MOV      #0, RD27
  MOV.L    RD9, @(R15, 240)
  MOV.L    RD8, @(R15, 236)
  MOV.L    RD31, @(R15, 232)
  MOV.L    RD30, @(R15, 228)
  MOV.L    RD29, @(R15, 224)
  MOV.L    RD28, @(R15, 220)
  MOV.L    RD27, @(R15, 196)
  MOV.L    RD26, @(R15, 200)
  MOV.L    RD25, @(R15, 204)

L008000EC:
  MOV.L    @(R15, 196), RD7
  CMP/EQ   RD7
  BT       L008000ED
  MOV.B    @RQ11, RD14
  CMP/EQ   #45, RD14
  BT       L008000EE
  CMP/EQ   #43, RD14
  BT       L008000EF
  CMP/EQ   #32, RD14
  BT       L008000F0
  CMP/EQ   #35, RD14
  BT       L008000F1
  CMP/EQ   #48, RD14
  BT       L008000F2
  CMP/EQ   #42, RD14
  BT       L008000F3
  BRA      L008000F4

L008000EE:
  MOV      #1, RD25
  MOV.L    RD25, @(R15, 236)
  BRA      L008000F5

L008000EF:
  MOV      #1, RD25
  MOV.L    RD25, @(R15, 232)
  BRA      L008000F5

L008000F0:
  MOV      #1, RD25
  MOV.L    RD25, @(R15, 228)
  BRA      L008000F5

L008000F1:
  MOV      #1, RD25
  MOV.L    RD25, @(R15, 224)
  BRA      L008000F5

L008000F2:
  MOV      #1, RD25
  MOV.L    RD25, @(R15, 220)
  BRA      L008000F5

L008000F3:
  MOV.Q    @(R15, 248), RQ25
  MOV      RQ25, RQ26
  MOV.Q    RQ26, @(R15, 72)
  MOV.Q    @(R15, 72), RQ4
  BSR      __va64_arg_i
  MOV      R2, RD14
  MOV      RD14, RD27
  MOV.L    RD27, @(R15, 216)
  MOV      #0, RD28
  CMP/GE   RD28, RD27
  BT       L008000F6
  MOV      #1, RD25
  MOV.L    @(R15, 216), RD26
  NEG      RD26, RD26
  MOV.L    RD26, @(R15, 216)
  MOV.L    RD25, @(R15, 236)

L008000F6:
  BRA      L008000F5

L008000F4:
  MOV      #1, RD25
  MOV.L    RD25, @(R15, 196)
  BRA      L008000F5

L008000F5:
  MOV.L    @(R15, 196), RD6
  CMP/EQ   RD6
  BT       L008000F7
  ADD      #1, RQ11
  BRA      L008000F8

L008000F7:
  MOV.L    @(R15, 228), RD5
  CMP/EQ   RD5
  BT       L008000F9
  MOV.L    @(R15, 232), RD4
  CMP/EQ   RD4
  BT       L008000F9
  BRA      L008000FA

L008000FA:
  MOV      #0, RD3
  MOV.L    RD3, @(R15, 228)

L008000F9:
  MOV.L    @(R15, 236), RD2
  CMP/EQ   RD2
  BT       L008000FB
  MOV      #0, RD23
  MOV.L    RD23, @(R15, 220)

L008000FB:

L008000F8:
  BRA      L008000EC

L008000ED:
  MOV.B    @RQ11, RD14
  EXTU.B   RD14, RD10
  MOV      RD10, RD4
  BSR      isdigit
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L008000FC

L008000FD:
  MOV.B    @RQ11, RD14
  EXTU.B   RD14, RD10
  MOV      RD10, RD4
  BSR      isdigit
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L008000FE
  MOV.L    @(R15, 216), RD22
  MOV      RD22, RD14
  MOV      #10, RD21
  MUL.L    RD21, RD14, RD14
  MOV.B    @RQ11, RD10
  SUB      RD10, #48, RD20
  ADD      RD14, RD20, RD22
  ADD      #1, RQ11
  MOV.L    RD22, @(R15, 216)
  MOV.L    RD20, @(R15, 64)
  BRA      L008000FD

L008000FE:

L008000FC:
  MOV.B    @RQ11, RD14
  CMP/EQ   #46, RD14
  BT       L008000FF
  ADD      #1, RQ11
  MOV.B    @RQ11, RD14
  CMP/EQ   #42, RD14
  BT       L00800100
  MOV.Q    @(R15, 248), RQ25
  MOV      RQ25, RQ26
  MOV.Q    RQ26, @(R15, 72)
  MOV.Q    @(R15, 72), RQ4
  BSR      __va64_arg_i
  MOV      R2, RD14
  MOV      RD14, RD27
  ADD      #1, RQ11
  MOV.L    RD27, @(R15, 212)
  BRA      L00800101

L00800100:
  MOV      #0, RD19
  MOV.L    RD19, @(R15, 212)

L00800102:
  MOV.B    @RQ11, RD14
  EXTU.B   RD14, RD10
  MOV      RD10, RD4
  BSR      isdigit
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L00800103
  MOV.L    @(R15, 212), RD18
  MOV      RD18, RD14
  MOV      #10, RD17
  MUL.L    RD17, RD14, RD14
  MOV.B    @RQ11, RD10
  SUB      RD10, #48, RD16
  ADD      RD14, RD16, RD18
  ADD      #1, RQ11
  MOV.L    RD18, @(R15, 212)
  MOV.L    RD16, @(R15, 64)
  BRA      L00800102

L00800103:

L00800101:

L008000FF:
  MOV.B    @RQ11, RD14
  CMP/EQ   #104, RD14
  BT       L00800104
  MOV.B    @RQ11, RD14
  CMP/EQ   #108, RD14
  BT       L00800105
  MOV      #1, RD16
  MOV.L    RD16, @(R15, 204)
  BRA      L00800106

L00800105:
  MOV.B    @RQ11, RD14
  CMP/EQ   #76, RD14
  BT       L00800107
  MOV      #1, RD16
  MOV.L    RD16, @(R15, 204)
  BRA      L00800108

L00800107:
  ADD      #-1, RQ11

L00800108:

L00800106:

L00800104:
  ADD      #1, RQ11
  MOV.B    @RQ11, RD14
  MOV      RD14, RD25
  MOV.L    RD25, @(R15, 200)
  MOV      L00C000B1, RQ4
  MOV      RD25, RD5
  BSR      strchr
  MOV      RQ2, RQ13
  MOV      #0, RQ25
  MOV      RQ13, RQ26
  MOV      RQ25, RQ27
  MOV.Q    RQ27, @(R15, 40)
  MOV.Q    RQ26, @(R15, 48)
  MOV.Q    RQ25, @(R15, 56)
  CMPQ/EQ  RQ27, RQ26
  BT       L00800109
  MOV.L    @(R15, 200), RD16
  CMP/EQ   #0, RD16
  BT       L00800109
  BRA      L0080010A

L0080010A:
  MOV.L    @(R15, 212), RD16
  MOV      #0, RD17
  CMP/GE   RD17, RD16
  BT       L0080010B
  MOV      #1, RD16
  MOV.L    RD16, @(R15, 212)

L0080010B:
  MOV.L    @(R15, 204), RD16
  CMP/EQ   #0, RD16
  BT       L0080010C
  MOV.Q    @(R15, 248), RQ25
  MOV      RQ25, RQ26
  MOV.Q    RQ26, @(R15, 72)
  MOV.Q    @(R15, 72), RQ4
  BSR      __va64_arg_l
  MOV      R2, RQ27
  MOV      RQ27, RQ28
  MOV.Q    RQ28, @(R15, 184)
  MOV.Q    RQ27, @(R15, 32)
  BRA      L0080010D

L0080010C:
  MOV.L    @(R15, 208), RD16
  CMP/EQ   #0, RD16
  BT       L0080010E
  MOV.Q    @(R15, 248), RQ25
  MOV      RQ25, RQ26
  MOV.Q    RQ26, @(R15, 72)
  MOV.Q    @(R15, 72), RQ4
  BSR      __va64_arg_i
  MOV      R2, RD14
  EXTS.W   RD14, RD10
  MOV      RD10, RD27
  MOV.L    RD27, @(R15, 180)
  MOV.L    @(R15, 200), RD28
  CMP/EQ   #117, RD28
  BT       L0080010F
  MOV.L    @(R15, 180), RD14
  EXTU.W   RD14, RD14
  EXTU.W   RD14, RQ16
  MOV      RQ16, RQ17
  MOV.Q    RQ17, @(R15, 184)
  MOV.Q    RQ16, @(R15, 32)
  BRA      L00800110

L0080010F:
  MOV.L    @(R15, 180), RD16
  EXTS.W   RQ16, RQ16
  MOV      RQ16, RQ17
  MOV.Q    RQ17, @(R15, 184)
  MOV.Q    RQ16, @(R15, 32)

L00800110:
  BRA      L00800111

L0080010E:
  MOV.Q    @(R15, 248), RQ25
  MOV      RQ25, RQ26
  MOV.Q    RQ26, @(R15, 72)
  MOV.Q    @(R15, 72), RQ4
  BSR      __va64_arg_i
  MOV      R2, RD14
  MOV      RD14, RD27
  MOV.L    RD27, @(R15, 176)
  MOV.L    @(R15, 200), RD28
  CMP/EQ   #117, RD28
  BT       L00800112
  MOV.L    @(R15, 176), RD16
  EXTU.L   RQ16, RQ16
  MOV      RQ16, RQ17
  MOV.Q    RQ17, @(R15, 184)
  MOV.Q    RQ16, @(R15, 32)
  BRA      L00800113

L00800112:
  MOV.L    @(R15, 176), RD16
  EXTS.L   RQ16, RQ16
  MOV      RQ16, RQ17
  MOV.Q    RQ17, @(R15, 184)
  MOV.Q    RQ16, @(R15, 32)

L00800113:

L00800111:

L0080010D:
  MOV.Q    @(R15, 184), RQ16
  MOV.Q    RQ16, @(R15, 168)
  MOV.Q    @(R15, 184), RQ16
  MOV      #0, RQ17
  CMPQ/GT  RQ16, RQ17
  BT       L00800114
  MOV.L    @(R15, 200), RD16
  CMP/EQ   #100, RD16
  BT       L00800115
  MOV.L    @(R15, 200), RD16
  CMP/EQ   #105, RD16
  BT       L00800115
  BRA      L00800114

L00800115:
  MOV      #1, RD16
  MOV.Q    @(R15, 184), RQ17
  NEG      RQ17, RQ18
  MOV      RQ18, RQ19
  MOV.Q    RQ19, @(R15, 168)
  MOV.Q    RQ18, @(R15, 32)
  MOV.L    RD16, @(R15, 124)
  BRA      L00800116

L00800114:
  MOV      #0, RD16
  MOV.L    RD16, @(R15, 124)

L00800116:
  MOV.L    @(R15, 200), RD16
  CMP/EQ   #88, RD16
  BT       L00800117
  MOV.L    @(R15, 200), RD16
  CMP/EQ   #120, RD16
  BT       L00800117
  BRA      L00800118

L00800118:
  MOV.L    @(R15, 200), RD16
  CMP/EQ   #112, RD16
  BT       L00800117
  BRA      L00800119

L00800117:
  MOV      #16, RD16
  MOV.L    RD16, @(R15, 132)
  BRA      L0080011A

L00800119:
  MOV.L    @(R15, 200), RD16
  CMP/EQ   #111, RD16
  BT       L0080011B
  MOV      #8, RD16
  MOV.L    RD16, @(R15, 132)
  BRA      L0080011C

L0080011B:
  MOV      #10, RD16
  MOV.L    RD16, @(R15, 132)

L0080011C:

L0080011A:
  MOV.L    @(R15, 200), RD16
  CMP/EQ   #112, RD16
  BT       L0080011D
  MOV      #8, RD16
  MOV.L    RD16, @(R15, 212)

L0080011D:
  MOV      #0, RD12
  MOV      #64, RD16
  MOV.L    RD16, @(R15, 108)

L0080011E:
  MOV.Q    @(R15, 168), RQ16
  MOV      #0, RQ17
  CMPQ/HI  RQ17, RQ16
  BT       L0080011F
  MOV.L    @(R15, 108), RD16
  MOV      RD16, RD14
  ADD      #-1, RD16
  MOV.L    RD16, @(R15, 108)
  CMP/PL   RD14
  BT       L0080011F
  BRA      L00800120

L00800120:
  MOV.L    @(R15, 132), RD25
  EXTS.L   RQ25, RQ25
  MOV.Q    @(R15, 168), RQ26
  MOV      RQ25, RQ5
  MOV      RQ26, RQ4
  BSR      __umodsq
  MOV      RQ2, RQ26
  EXTS.L   RQ26, RD27
  MOV.L    RD27, @(R15, 136)
  MOV.Q    RQ26, @(R15, 32)
  MOV.Q    RQ25, @(R15, 24)
  MOV      #10, RD25
  CMP/GE   RD25, RD27
  BT       L00800121
  MOV.L    @(R15, 136), RD16
  ADD      RD16, #48, RD14
  EXTS.B   RD14, RD10
  MOV      #284, RQ17
  ADD      R15, RQ17
  EXTS.L   RD12
  MOV.B    RD10, @(RQ17, RD12)
  BRA      L00800122

L00800121:
  MOV.L    @(R15, 200), RD16
  CMP/EQ   #88, RD16
  BT       L00800123
  MOV.L    @(R15, 200), RD16
  CMP/EQ   #112, RD16
  BT       L00800123
  BRA      L00800124

L00800123:
  MOV.L    @(R15, 136), RD16
  SUB      RD16, #10, RD14
  ADD      RD14, #65, RD10
  EXTS.B   RD10, RD14
  MOV      #284, RQ17
  ADD      R15, RQ17
  EXTS.L   RD12
  MOV.B    RD14, @(RQ17, RD12)
  BRA      L00800125

L00800124:
  MOV.L    @(R15, 136), RD16
  SUB      RD16, #10, RD14
  ADD      RD14, #97, RD10
  EXTS.B   RD10, RD14
  MOV      #284, RQ17
  ADD      R15, RQ17
  EXTS.L   RD12
  MOV.B    RD14, @(RQ17, RD12)

L00800125:

L00800122:
  ADD      #1, RD12
  MOV.Q    @(R15, 168), RQ25
  MOV.L    @(R15, 132), RD26
  EXTS.L   RQ26, RQ26
  MOV      RQ25, RQ27
  MOV      RQ26, RQ5
  MOV      RQ27, RQ4
  BSR      __udivsq
  MOV      RQ2, RQ27
  MOV.Q    RQ27, @(R15, 168)
  MOV.Q    RQ26, @(R15, 24)
  MOV.Q    RQ25, @(R15, 32)
  BRA      L0080011E

L0080011F:

L00800126:
  MOV.L    @(R15, 212), RD16
  CMP/GT   RD12, RD16
  BT       L00800127
  MOV      #284, RQ16
  ADD      R15, RQ16
  MOV      #48, RD17
  EXTS.L   RD12
  MOV.B    RD17, @(RQ16, RD12)
  ADD      #1, RD12
  BRA      L00800126

L00800127:
  MOV.L    @(R15, 124), RD16
  CMP/EQ   #0, RD16
  BT       L00800128
  MOV      RD12, RD14
  ADD      #1, RD12
  MOV      #284, RQ16
  ADD      R15, RQ16
  MOV      #45, RD17
  EXTS.L   RD14
  MOV.B    RD17, @(RQ16, RD14)
  BRA      L00800129

L00800128:
  MOV.L    @(R15, 232), RD16
  CMP/EQ   #0, RD16
  BT       L0080012A
  MOV      RD12, RD14
  ADD      #1, RD12
  MOV      #284, RQ16
  ADD      R15, RQ16
  MOV      #43, RD17
  EXTS.L   RD14
  MOV.B    RD17, @(RQ16, RD14)

L0080012A:

L00800129:
  MOV.L    @(R15, 220), RD16
  CMP/EQ   #0, RD16
  BT       L0080012B
  MOV      #48, RD16
  MOV.L    RD16, @(R15, 128)
  BRA      L0080012C

L0080012B:
  MOV      #32, RD16
  MOV.L    RD16, @(R15, 128)

L0080012C:
  MOV      RD12, RD16
  MOV.L    RD16, @(R15, 140)
  MOV.L    @(R15, 236), RD16
  CMP/EQ   #0, RD16
  BT       L0080012D

L0080012E:
  MOV.L    @(R15, 140), RD16
  MOV.L    @(R15, 216), RD17
  CMP/GT   RD16, RD17
  BT       L0080012F
  MOV      #0, RQ13
  MOV.Q    @(R15, 264), RQ17
  MOV      RQ17, RQ16
  MOV      RQ13, RQ18
  MOV.Q    RQ18, @(R15, 48)
  MOV.Q    RQ16, @(R15, 56)
  CMPQ/EQ  RQ18, RQ16
  BT       L00800130
  MOV.L    @(R15, 128), RD14
  EXTS.B   RD14, RD14
  MOV.Q    @(R15, 256), RQ16
  MOV      RQ16, RQ13
  ADD      #1, RQ16
  MOV.B    RD14, @RQ13
  MOV.Q    RQ16, @(R15, 256)
  BRA      L00800131

L00800130:
  MOV.L    @(R15, 128), RD4
  MOV.Q    @(R15, 264), RQ5
  BSR      fputc

L00800131:
  MOV.L    @(R15, 240), RD16
  ADD      #1, RD16
  MOV.L    @(R15, 140), RD17
  ADD      #1, RD17
  MOV.L    RD17, @(R15, 140)
  MOV.L    RD16, @(R15, 240)
  BRA      L0080012E

L0080012F:

L0080012D:
  MOV.L    @(R15, 224), RD16
  CMP/EQ   #0, RD16
  BT       L00800132
  MOV.L    @(R15, 200), RD14
  EXTU.B   RD14, RD14
  MOV      RD14, RD4
  BSR      toupper
  MOV      RD2, RD10
  CMP/EQ   #88, RD10
  BT       L00800132
  BRA      L00800133

L00800133:
  MOV      #0, RQ13
  MOV.Q    @(R15, 264), RQ17
  MOV      RQ17, RQ16
  MOV      RQ13, RQ18
  MOV.Q    RQ18, @(R15, 48)
  MOV.Q    RQ16, @(R15, 56)
  CMPQ/EQ  RQ18, RQ16
  BT       L00800134
  MOV      #48, RD14
  MOV.Q    @(R15, 256), RQ16
  MOV      RQ16, RQ13
  ADD      #1, RQ16
  MOV.B    RD14, @RQ13
  MOV.Q    RQ16, @(R15, 256)
  BRA      L00800135

L00800134:
  MOV      #48, RD4
  MOV.Q    @(R15, 264), RQ5
  BSR      fputc

L00800135:
  MOV      #0, RQ13
  MOV.Q    @(R15, 264), RQ17
  MOV      RQ17, RQ16
  MOV      RQ13, RQ18
  MOV.Q    RQ18, @(R15, 48)
  MOV.Q    RQ16, @(R15, 56)
  CMPQ/EQ  RQ18, RQ16
  BT       L00800136
  MOV      #120, RD14
  MOV.Q    @(R15, 256), RQ16
  MOV      RQ16, RQ13
  ADD      #1, RQ16
  MOV.B    RD14, @RQ13
  MOV.Q    RQ16, @(R15, 256)
  BRA      L00800137

L00800136:
  MOV      #120, RD4
  MOV.Q    @(R15, 264), RQ5
  BSR      fputc

L00800137:
  MOV.L    @(R15, 240), RD16
  ADD      #2, RD16
  MOV.L    RD16, @(R15, 240)

L00800132:
  ADD      #-1, RD12

L00800138:
  CMP/PZ   RD12
  BT       L00800139
  MOV      #0, RQ13
  MOV.Q    @(R15, 264), RQ17
  MOV      RQ17, RQ16
  MOV      RQ13, RQ18
  MOV.Q    RQ18, @(R15, 48)
  MOV.Q    RQ16, @(R15, 56)
  CMPQ/EQ  RQ18, RQ16
  BT       L0080013A
  MOV      #284, RQ16
  ADD      R15, RQ16
  EXTS.L   RD12
  MOV.B    @(RQ16, RD12), RD14
  EXTS.B   RD14, RD10
  MOV.Q    @(R15, 256), RQ17
  MOV      RQ17, RQ13
  ADD      #1, RQ17
  MOV.B    RD10, @RQ13
  MOV.Q    RQ17, @(R15, 256)
  BRA      L0080013B

L0080013A:
  MOV      #284, RQ25
  ADD      R15, RQ25
  EXTS.L   RD12
  MOV.B    @(RQ25, RD12), RD14
  MOV      RD14, RD4
  MOV.Q    @(R15, 264), RQ5
  BSR      fputc

L0080013B:
  MOV.L    @(R15, 240), RD16
  ADD      #1, RD16
  ADD      #-1, RD12
  MOV.L    RD16, @(R15, 240)
  BRA      L00800138

L00800139:
  MOV.L    @(R15, 236), RD16
  CMP/EQ   #0, RD16
  BT       L0080013C

L0080013D:
  MOV.L    @(R15, 140), RD16
  MOV.L    @(R15, 216), RD17
  CMP/GT   RD16, RD17
  BT       L0080013E
  MOV      #0, RQ13
  MOV.Q    @(R15, 264), RQ17
  MOV      RQ17, RQ16
  MOV      RQ13, RQ18
  MOV.Q    RQ18, @(R15, 48)
  MOV.Q    RQ16, @(R15, 56)
  CMPQ/EQ  RQ18, RQ16
  BT       L0080013F
  MOV.L    @(R15, 128), RD14
  EXTS.B   RD14, RD14
  MOV.Q    @(R15, 256), RQ16
  MOV      RQ16, RQ13
  ADD      #1, RQ16
  MOV.B    RD14, @RQ13
  MOV.Q    RQ16, @(R15, 256)
  BRA      L00800140

L0080013F:
  MOV.L    @(R15, 128), RD4
  MOV.Q    @(R15, 264), RQ5
  BSR      fputc

L00800140:
  MOV.L    @(R15, 240), RD16
  ADD      #1, RD16
  MOV.L    @(R15, 140), RD17
  ADD      #1, RD17
  MOV.L    RD17, @(R15, 140)
  MOV.L    RD16, @(R15, 240)
  BRA      L0080013D

L0080013E:

L0080013C:
  BRA      L00800141

L00800109:
  MOV      L00C00094, RQ4
  MOV.L    @(R15, 200), RD5
  BSR      strchr
  MOV      RQ2, RQ13
  MOV      #0, RQ25
  MOV      RQ13, RQ26
  MOV      RQ25, RQ27
  MOV.Q    RQ27, @(R15, 40)
  MOV.Q    RQ26, @(R15, 48)
  MOV.Q    RQ25, @(R15, 56)
  CMPQ/EQ  RQ27, RQ26
  BT       L00800142
  MOV.L    @(R15, 200), RD16
  CMP/EQ   #0, RD16
  BT       L00800142
  BRA      L00800143

L00800143:
  MOV.L    @(R15, 212), RD16
  MOV      #0, RD17
  CMP/GE   RD17, RD16
  BT       L00800144
  MOV      #6, RD16
  MOV.L    RD16, @(R15, 212)

L00800144:
  MOV.Q    @(R15, 248), RQ25
  MOV      RQ25, RQ26
  MOV.Q    RQ26, @(R15, 72)
  MOV.Q    @(R15, 72), RQ4
  BSR      __va64_arg_d
  FMOV.D   DR2, @(R15, 16)
  FMOV.D   @(R15, 16), DR7
  FMOV.D   DR7, @(R15, 160)
  MOV.L    @(R15, 200), RD14
  EXTS.B   RD14, RD14
  MOV.L    @(R15, 216), RD27
  EXTS.L   RQ27, RQ27
  MOV.Q    RQ27, @(R15, 32)
  FMOV.D   @(R15, 160), DR4
  MOV      RD14, RD4
  MOV      RQ27, RQ5
  MOV.L    @(R15, 212), RD6
  MOV      #284, RQ7
  ADD      R15, RQ7
  BSR      dblcvt
  MOV      #284, RQ4
  ADD      R15, RQ4
  BSR      strlen
  MOV      RQ2, RQ28
  MOV      #0, RQ13
  MOV.Q    @(R15, 264), RQ30
  MOV      RQ30, RQ29
  MOV      RQ13, RQ31
  MOV.Q    RQ31, @(R15, 48)
  MOV.Q    RQ29, @(R15, 56)
  MOV.Q    RQ28, @(R15, 112)
  CMPQ/EQ  RQ31, RQ29
  BT       L00800145
  MOV.Q    @(R15, 256), RQ4
  MOV      #284, RQ5
  ADD      R15, RQ5
  MOV.Q    @(R15, 112), RQ6
  BSR      memcpy
  MOV.Q    @(R15, 112), RQ16
  MOV.Q    @(R15, 256), RQ17
  LEA.B    @(RQ17, RQ16), RQ17
  MOV.Q    RQ17, @(R15, 256)
  BRA      L00800146

L00800145:
  MOV      #284, RQ4
  ADD      R15, RQ4
  MOV.Q    @(R15, 264), RQ5
  BSR      fputs

L00800146:
  MOV.L    @(R15, 240), RD16
  EXTS.L   RQ16, RQ16
  MOV.Q    @(R15, 112), RQ17
  ADD      RQ16, RQ17, RQ18
  EXTS.L   RQ18, RD19
  MOV.L    RD19, @(R15, 240)
  MOV.Q    RQ18, @(R15, 32)
  MOV.Q    RQ16, @(R15, 24)
  BRA      L00800147

L00800142:
  MOV.L    @(R15, 200), RD16
  CMP/EQ   #115, RD16
  BT       L00800148
  MOV.L    @(R15, 212), RD16
  MOV      #0, RD17
  CMP/GE   RD17, RD16
  BT       L00800149
  MOV      #1, RD16
  MOV.L    RD16, @(R15, 212)

L00800149:
  MOV.Q    @(R15, 248), RQ25
  MOV      RQ25, RQ26
  MOV.Q    RQ26, @(R15, 72)
  MOV.Q    @(R15, 72), RQ4
  BSR      __va64_arg_l
  MOV      R2, RQ13
  MOV      RQ13, RQ27
  MOV      #32, RD28
  MOV.L    RD28, @(R15, 128)
  MOV.Q    RQ27, @(R15, 152)
  MOV.L    @(R15, 212), RD29
  CMP/GT   #1, RD29
  BT       L0080014A
  MOV.L    @(R15, 212), RD25
  EXTS.L   RQ25, RQ25
  MOV.Q    RQ25, @(R15, 32)
  MOV.Q    @(R15, 152), RQ4
  MOV      #0, RD5
  MOV      RQ25, RQ6
  BSR      memchr
  MOV      RQ2, RQ26
  MOV      #0, RQ13
  MOV      RQ26, RQ27
  MOV      RQ13, RQ28
  MOV.Q    RQ28, @(R15, 48)
  MOV.Q    RQ27, @(R15, 56)
  MOV.Q    RQ26, @(R15, 96)
  CMPQ/EQ  RQ28, RQ27
  BT       L0080014B
  MOV.Q    @(R15, 96), RQ16
  MOV.Q    @(R15, 152), RQ17
  SUB      RQ16, RQ17, RD14
  MOV      RD14, RD18
  MOV.L    RD18, @(R15, 120)
  BRA      L0080014C

L0080014B:
  MOV.L    @(R15, 212), RD17
  MOV      RD17, RD16
  MOV.L    RD16, @(R15, 120)

L0080014C:
  BRA      L0080014D

L0080014A:
  MOV.Q    @(R15, 152), RQ4
  BSR      strlen
  MOV      RQ2, RQ25
  EXTS.L   RQ25, RD26
  MOV.L    RD26, @(R15, 120)
  MOV.Q    RQ25, @(R15, 32)

L0080014D:
  MOV.L    @(R15, 236), RD16
  CMP/EQ   #0, RD16
  BT       L0080014E
  MOV.L    @(R15, 120), RD16
  MOV.L    @(R15, 216), RD17
  CMP/GT   RD16, RD17
  BT       L0080014F
  MOV.L    @(R15, 216), RD16
  MOV.L    @(R15, 120), RD17
  SUB      RD16, RD17, RD14
  MOV.L    @(R15, 240), RD18
  ADD      RD14, RD18
  MOV      #0, RD12
  MOV.L    RD18, @(R15, 240)

L00800150:
  MOV.L    @(R15, 216), RD16
  MOV.L    @(R15, 120), RD17
  SUB      RD16, RD17, RD14
  CMP/GT   RD12, RD14
  BT       L00800151
  MOV      #0, RQ13
  MOV.Q    @(R15, 264), RQ17
  MOV      RQ17, RQ16
  MOV      RQ13, RQ18
  MOV.Q    RQ18, @(R15, 48)
  MOV.Q    RQ16, @(R15, 56)
  CMPQ/EQ  RQ18, RQ16
  BT       L00800152
  MOV.L    @(R15, 128), RD14
  EXTS.B   RD14, RD14
  MOV.Q    @(R15, 256), RQ16
  MOV      RQ16, RQ13
  ADD      #1, RQ16
  MOV.B    RD14, @RQ13
  MOV.Q    RQ16, @(R15, 256)
  BRA      L00800153

L00800152:
  MOV.L    @(R15, 128), RD4
  MOV.Q    @(R15, 264), RQ5
  BSR      fputc

L00800153:

L00800154:
  ADD      #1, RD12
  BRA      L00800150

L00800151:

L0080014F:

L0080014E:
  MOV      #0, RD12

L00800155:
  MOV.L    @(R15, 120), RD16
  CMP/GT   RD12, RD16
  BT       L00800156
  MOV      #0, RQ13
  MOV.Q    @(R15, 264), RQ17
  MOV      RQ17, RQ16
  MOV      RQ13, RQ18
  MOV.Q    RQ18, @(R15, 48)
  MOV.Q    RQ16, @(R15, 56)
  CMPQ/EQ  RQ18, RQ16
  BT       L00800157
  MOV.Q    @(R15, 152), RQ16
  EXTS.L   RD12
  MOV.B    @(RQ16, RD12), RD14
  EXTS.B   RD14, RD10
  MOV.Q    @(R15, 256), RQ17
  MOV      RQ17, RQ13
  ADD      #1, RQ17
  MOV.B    RD10, @RQ13
  MOV.Q    RQ17, @(R15, 256)
  BRA      L00800158

L00800157:
  MOV.Q    @(R15, 152), RQ25
  EXTS.L   RD12
  MOV.B    @(RQ25, RD12), RD14
  MOV      RD14, RD4
  MOV.Q    @(R15, 264), RQ5
  BSR      fputc

L00800158:

L00800159:
  ADD      #1, RD12
  BRA      L00800155

L00800156:
  MOV.L    @(R15, 120), RD16
  MOV.L    @(R15, 240), RD17
  ADD      RD16, RD17
  MOV.L    RD17, @(R15, 240)
  MOV.L    @(R15, 236), RD18
  CMP/EQ   #0, RD18
  BT       L0080015A
  MOV.L    @(R15, 120), RD16
  MOV.L    @(R15, 216), RD17
  CMP/GT   RD16, RD17
  BT       L0080015B
  MOV.L    @(R15, 216), RD16
  MOV.L    @(R15, 120), RD17
  SUB      RD16, RD17, RD14
  MOV.L    @(R15, 240), RD18
  ADD      RD14, RD18
  MOV      #0, RD12
  MOV.L    RD18, @(R15, 240)

L0080015C:
  MOV.L    @(R15, 216), RD16
  MOV.L    @(R15, 120), RD17
  SUB      RD16, RD17, RD14
  CMP/GT   RD12, RD14
  BT       L0080015D
  MOV      #0, RQ13
  MOV.Q    @(R15, 264), RQ17
  MOV      RQ17, RQ16
  MOV      RQ13, RQ18
  MOV.Q    RQ18, @(R15, 48)
  MOV.Q    RQ16, @(R15, 56)
  CMPQ/EQ  RQ18, RQ16
  BT       L0080015E
  MOV.L    @(R15, 128), RD14
  EXTS.B   RD14, RD14
  MOV.Q    @(R15, 256), RQ16
  MOV      RQ16, RQ13
  ADD      #1, RQ16
  MOV.B    RD14, @RQ13
  MOV.Q    RQ16, @(R15, 256)
  BRA      L0080015F

L0080015E:
  MOV.L    @(R15, 128), RD4
  MOV.Q    @(R15, 264), RQ5
  BSR      fputc

L0080015F:

L00800160:
  ADD      #1, RD12
  BRA      L0080015C

L0080015D:

L0080015B:

L0080015A:

L00800148:

L00800147:

L00800141:
  MOV.Q    @(R15, 272), RQ16
  MOV.Q    RQ11, @RQ16
  MOV.L    @(R15, 240), RD2

L00E0042F:
  ADD      #552, R15
  BRA      L00C00085

tk_puts:
  PUSH     LR
  BSR      L00C00032
  ADD      #-136, R15
  MOV      RQ4, RQ11
  MOV      RQ11, RQ14

L008007B9:
  MOV.B    @RQ14, RD13
  CMP/EQ   RD13
  BT       L008007BA
  MOV      RQ14, RQ12
  ADD      #1, RQ14
  MOV.B    @RQ12, RD13
  MOV      RD13, RD4
  BSR      tk_putc
  BRA      L008007B9

L008007BA:

L00E00430:
  ADD      #136, R15
  BRA      L00C00033

isdigit:
  PUSH     LR
  BSR      L00C00032
  ADD      #-120, R15
  MOV.L    RD4, @(R15, 12)
  BSR      __cytpe_init
  MOV.Q    __isbuf, RQ12
  MOV.L    @(R15, 12), RD11
  MOVU.W   @(RQ12, RD11), RD14
  AND      RD14, #8, RD13
  MOV      RD13, RD2

L00E00431:
  ADD      #120, R15
  BRA      L00C00033

toupper:
  PUSH     LR
  BSR      L00C0001A
  ADD      #-128, R15
  MOV.L    RD4, @(R15, 12)
  BSR      __cytpe_init
  MOV.Q    __toup, RQ13
  MOV.L    @(R15, 12), RD12
  MOV.W    @(RQ13, RD12), RD14
  MOV      RD14, RD2

L00E00432:
  ADD      #128, R15
  BRA      L00C0001D

isspace:
  PUSH     LR
  BSR      L00C00032
  ADD      #-120, R15
  MOV.L    RD4, @(R15, 12)
  BSR      __cytpe_init
  MOV.Q    __isbuf, RQ12
  MOV.L    @(R15, 12), RD11
  MOVU.W   @(RQ12, RD11), RD14
  AND      RD14, #256, RD13
  MOV      RD13, RD2

L00E00433:
  ADD      #120, R15
  BRA      L00C00033

isalpha:
  PUSH     LR
  BSR      L00C00032
  ADD      #-120, R15
  MOV.L    RD4, @(R15, 12)
  BSR      __cytpe_init
  MOV.Q    __isbuf, RQ12
  MOV.L    @(R15, 12), RD11
  MOVU.W   @(RQ12, RD11), RD14
  AND      RD14, #2, RD13
  MOV      RD13, RD2

L00E00434:
  ADD      #120, R15
  BRA      L00C00033

stricmp:
  PUSH     LR
  BSR      L00C0008B
  ADD      #-104, R15
  MOV.Q    RQ4, @(R15, 24)
  MOV.Q    RQ5, @(R15, 16)
  MOV.Q    @(R15, 24), RQ10
  MOV      RQ10, RQ13
  MOV.Q    @(R15, 16), RQ9
  MOV      RQ9, RQ12

L00800379:
  MOVU.B   @RQ13, RD14
  CMP/EQ   RD14
  BT       L0080037A
  MOVU.B   @RQ13, RD14
  MOV      RD14, RD4
  BSR      toupper
  MOV      RD2, RD11
  MOVU.B   @RQ12, RD14
  MOV      RD14, RD4
  BSR      toupper
  MOV      RD2, RD8
  MOV.L    RD8, @(R15, 12)
  CMP/GT   RD11, RD8
  BT       L0080037B
  MOV      #-1, RD2
  BRA      L00E00435
  BRA      L0080037C

L0080037B:
  MOV.L    @(R15, 12), RD31
  CMP/GT   RD31, RD11
  BT       L0080037D
  MOV      #1, RD2
  BRA      L00E00435

L0080037D:

L0080037C:
  ADD      #1, RQ13
  ADD      #1, RQ12
  BRA      L00800379

L0080037A:
  MOVU.B   @RQ12, RD14
  CMP/EQ   RD14
  BT       L0080037E
  MOV      #0, RD2
  BRA      L00E00435
  BRA      L0080037F

L0080037E:
  MOV      #-1, RD2
  BRA      L00E00435

L0080037F:

L00E00435:
  ADD      #104, R15
  BRA      L00C0008C

strnicmp:
  PUSH     LR
  BSR      L00C00053
  ADD      #-104, R15
  MOV.Q    RQ4, @(R15, 56)
  MOV.Q    RQ5, @(R15, 48)
  MOV.Q    RQ6, @(R15, 40)
  MOV      #0, RQ12
  EXTS.L   RQ12, RQ12
  MOV      RQ12, RQ13
  MOV.Q    @(R15, 56), RQ8
  MOV      RQ8, RQ11
  MOV.Q    @(R15, 48), RQ31
  MOV      RQ31, RQ9

L00800380:
  MOV      RQ13, RQ12
  MOV.Q    @(R15, 40), RQ30
  MOV.Q    RQ30, @(R15, 16)
  CMPQ/GT  RQ12, RQ30
  BT       L00800381
  MOVU.B   @(RQ11, RQ13), RD14
  MOV      RD14, RD4
  BSR      toupper
  MOV      RD2, RD10
  MOVU.B   @(RQ9, RQ13), RD14
  MOV      RD14, RD4
  BSR      toupper
  MOV      RD2, RD29
  MOV.L    RD29, @(R15, 36)
  CMP/GT   RD10, RD29
  BT       L00800382
  MOV      #-1, RD2
  BRA      L00E00436
  BRA      L00800383

L00800382:
  MOVU.B   @(RQ11, RQ13), RD14
  MOV      RD14, RD4
  BSR      toupper
  MOV      RD2, RD28
  MOVU.B   @(RQ9, RQ13), RD14
  MOV.L    RD28, @(R15, 8)
  MOV      RD14, RD4
  BSR      toupper
  MOV      RD2, RD27
  MOV.L    RD27, @(R15, 4)
  CMP/GT   RD27, RD28
  BT       L00800384
  MOV      #1, RD2
  BRA      L00E00436
  BRA      L00800385

L00800384:
  MOVU.B   @(RQ11, RQ13), RD14
  CMP/EQ   RD14
  BT       L00800386
  MOV      #0, RD2
  BRA      L00E00436

L00800386:

L00800385:

L00800383:
  ADD      #1, RQ13
  BRA      L00800380

L00800381:
  MOV      #0, RD2

L00E00436:
  ADD      #104, R15
  BRA      L00C00054

__sigdfl:
  PUSH     LR
  BSR      L00C00032
  ADD      #-120, R15
  MOV      RD4, RD13
  MOV      __sigdfl, RQ12
  MOV      RQ12, RQ14
  MOV      handlers, RQ11
  EXTS.L   RD13
  MOV.Q    RQ14, @(RQ11, RD13)
  CMP/EQ   #1, RD13
  BT       L00800468
  MOV      #1, RD4
  BSR      exit

L00800468:

L00E00437:
  ADD      #120, R15
  BRA      L00C00033

__sigerr:
  ADD      #-160, R15
  MOV.L    RD4, @(R15, 12)

L00E00438:
  ADD      #160, R15
  RTS     

__sigign:
  ADD      #-160, R15
  MOV.L    RD4, @(R15, 12)

L00E00439:
  ADD      #160, R15
  RTS     

raise:
  PUSH     LR
  BSR      L00C0001A
  ADD      #-144, R15
  MOV      RD4, RD13
  MOV      handlers, RQ12
  EXTS.L   RD13
  MOV.Q    @(RQ12, RD13), RQ14
  MOV      RD13, RD4
  JSR      RQ14
  MOV      #0, RD2

L00E0043A:
  ADD      #144, R15
  BRA      L00C0001D

__allocmem:
  PUSH     LR
  BSR      L00C0001A
  ADD      #-144, R15
  MOV.Q    RQ4, @(R15, 24)
  MOV.Q    RQ5, @(R15, 16)
  MOV.L    @(R15, 24), RD13
  EXTS.L   RD13, RD13
  MOV      RD13, RD4
  BSR      TKMM_Malloc
  MOV      RQ2, RQ14
  MOV.Q    @(R15, 16), RQ12
  MOV.Q    RQ14, @RQ12

L00E0043B:
  ADD      #144, R15
  BRA      L00C0001D

__freemem:
  ADD      #-160, R15
  MOV.Q    RQ4, @(R15, 8)

L00E0043C:
  ADD      #160, R15
  RTS     

__exec:
  ADD      #-160, R15
  MOV.Q    RQ4, @(R15, 8)
  MOV.Q    RQ5, @R15

L00E0043D:
  ADD      #160, R15
  RTS     

__exit:
  PUSH     LR
  BSR      L00C0003D
  ADD      #-128, R15
  MOV.L    RD4, @(R15, 44)
  MOV      #0, RD12

L00800411:
  CMP/GE   #253, RD12
  BT       L00800412
  MOV      __userFiles, RQ9
  EXTS.L   RD12
  MOV.Q    @(RQ9, RD12), RQ14
  MOV      #0, RQ13
  MOV      RQ14, RQ11
  MOV      RQ13, RQ10
  CMPQ/EQ  RQ10, RQ11
  BT       L00800413
  MOV      __userFiles, RQ8
  EXTS.L   RD12
  MOV.Q    @(RQ8, RD12), RQ14
  MOV      RQ14, RQ4
  BSR      fclose

L00800413:

L00800414:
  ADD      #1, RD12
  BRA      L00800411

L00800412:
  BSR      __get_stdout
  MOV      RQ2, RQ14
  MOV.Q    @RQ14, RQ13
  MOV      #0, RQ14
  MOV      RQ13, RQ11
  MOV      RQ14, RQ10
  CMPQ/EQ  RQ10, RQ11
  BT       L00800415
  BSR      __get_stdout
  MOV      RQ2, RQ14
  MOV.Q    @RQ14, RQ13
  MOV      RQ13, RQ4
  BSR      fflush

L00800415:
  BSR      __get_stderr
  MOV      RQ2, RQ14
  MOV.Q    @RQ14, RQ13
  MOV      #0, RQ14
  MOV      RQ13, RQ11
  MOV      RQ14, RQ10
  CMPQ/EQ  RQ10, RQ11
  BT       L00800416
  BSR      __get_stderr
  MOV      RQ2, RQ14
  MOV.Q    @RQ14, RQ13
  MOV      RQ13, RQ4
  BSR      fflush

L00800416:
  MOV.L    @(R15, 44), RD4
  BSR      __exita

L00800417:
  BRA      L00800417

L00800418:

L00E0043E:
  ADD      #128, R15
  BRA      L00C0003E

main:
  PUSH     LR
  BSR      L00C00032
  ADD      #-120, R15
  MOV.L    RD4, @(R15, 12)
  MOV.Q    RQ5, @R15
  MOV.L    @(R15, 12), RD13
  MOV      RD13, RD14
  MOV.Q    @R15, RQ11
  MOV      RQ11, RQ12
  MOV.L    RD14, myargc
  MOV.Q    RQ12, myargv
  BSR      D_DoomMain
  MOV      #0, RD2

L00E0043F:
  ADD      #120, R15
  BRA      L00C00033

__exita:
  PUSH     R14
  PUSH     R13
  ADD      #-144, R15
  MOV.L    RD4, @(R15, 12)
  MOV      #-1, RQ14
  MOV      #0x0000FFFF, RQ14
  MOV      #-1, RD13
  MOV.L    RD13, @RQ14

L00E00440:
  ADD      #144, R15
  POP      RQ13
  POP      RQ14
  RTS     

__get_cmdline:
  ADD      #-144, R15
  MOV      L00C000C5, RQ2

L00E00441:
  ADD      #144, R15
  RTS     

__get_cmdenv:
  PUSH     R14
  ADD      #-152, R15
  MOV      #0, RQ14
  MOV      RQ14, RQ2

L00E00442:
  ADD      #152, R15
  POP      RQ14
  RTS     

__start:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-104, R15
  MOV      L00C000CD, RQ4
  BSR      tk_puts
  BSR      __start_early
  BSR      __get_cmdline
  MOV      RQ2, RQ13
  BSR      __get_cmdenv
  MOV      RQ2, RQ8
  MOV.Q    RQ8, @(R15, 56)
  MOV      L00C000CF, RQ4
  BSR      tk_puts
  BSR      __init_stdin
  BSR      __get_stdin
  MOV      RQ2, RQ14
  MOV.Q    @RQ14, RQ11
  MOV      RQ11, RQ12
  BSR      __get_stdout
  MOV      RQ2, RQ14
  MOV.Q    @RQ14, RQ11
  MOV      RQ11, RQ10
  BSR      __get_stderr
  MOV      RQ2, RQ14
  MOV.Q    @RQ14, RQ11
  MOV      RQ11, RQ9
  MOV      #0, RD31
  MOV.L    RD31, @RQ12
  MOV      #1, RD30
  MOV.L    RD30, @RQ10
  MOV      #2, RD29
  MOV.L    RD29, @RQ9
  MOV.L    RD31, @(RQ12, 4)
  MOV.L    RD31, @(RQ12, 8)
  MOV.L    RD30, @(RQ12, 12)
  MOV.L    RD31, @(RQ12, 16)
  EXTS.L   RD31, RQ28
  MOV.Q    RQ28, @(RQ12, 24)
  MOV.L    RD29, @(RQ12, 76)
  MOV      TU3CD6E720/buffer1, RQ27
  MOV.Q    RQ27, @(RQ12, 80)
  MOV.Q    @(RQ12, 80), RQ14
  ADD      RQ14, #2, RQ11
  MOV.Q    RQ11, @(RQ12, 32)
  MOV.Q    @(RQ12, 32), RQ14
  MOV      RQ14, RQ11
  ADD      RQ14, #1, RQ26
  MOV.Q    RQ26, @(RQ12, 32)
  MOV.B    RD31, @RQ11
  MOV.Q    @(RQ12, 32), RQ14
  MOV      RQ14, RQ11
  ADD      RQ14, #1, RQ26
  MOV.Q    RQ26, @(RQ12, 32)
  MOV.B    RD31, @RQ11
  MOV      #6144, RQ28
  EXTS.L   RQ28, RQ28
  MOV.Q    RQ28, @(RQ12, 40)
  MOV.Q    @(RQ12, 32), RQ14
  MOV.Q    @(RQ12, 40), RQ28
  LEA.B    @(RQ14, RQ28), RQ11
  MOV.Q    RQ11, @(RQ12, 56)
  MOV.Q    @(RQ12, 56), RQ14
  MOV      #10, RD25
  MOV.B    RD25, @RQ14
  MOV.L    RD31, @(RQ12, 88)
  MOV.Q    @(RQ12, 56), RQ14
  MOV.Q    RQ14, @(RQ12, 48)
  MOV.Q    @(RQ12, 40), RQ28
  NEG      RQ28, RQ8
  MOV.Q    RQ8, @(RQ12, 24)
  MOV.L    RD29, @(RQ12, 92)
  MOV      #-1, RD31
  MOV.L    RD31, @(RQ12, 72)
  MOV      #0, RD30
  MOV.L    RD30, @(RQ12, 96)
  MOV.L    RD30, @(RQ12, 100)
  MOV      #1, RD27
  MOV.L    RD27, @(RQ12, 104)
  MOV.L    RD27, @(RQ12, 108)
  MOV.L    RD30, @(RQ10, 4)
  MOV.L    RD30, @(RQ10, 8)
  MOV.L    RD27, @(RQ10, 12)
  MOV.L    RD29, @(RQ10, 76)
  MOV      TU3CD6E720/buffer2, RQ31
  MOV.Q    RQ31, @(RQ10, 80)
  MOV.Q    @(RQ10, 80), RQ14
  MOV.Q    RQ14, @(RQ10, 32)
  MOV.Q    @(RQ10, 32), RQ14
  MOV      RQ14, RQ11
  ADD      RQ14, #1, RQ26
  MOV.Q    RQ26, @(RQ10, 32)
  MOV.B    RD30, @RQ11
  MOV.Q    @(RQ10, 32), RQ14
  MOV      RQ14, RQ11
  ADD      RQ14, #1, RQ26
  MOV.Q    RQ26, @(RQ10, 32)
  MOV.B    RD30, @RQ11
  MOV      #6144, RQ28
  EXTS.L   RQ28, RQ28
  MOV.Q    RQ28, @(RQ10, 40)
  MOV.Q    @(RQ10, 32), RQ14
  MOV.Q    @(RQ10, 40), RQ28
  LEA.B    @(RQ14, RQ28), RQ11
  MOV.Q    RQ11, @(RQ10, 56)
  MOV.Q    @(RQ10, 56), RQ14
  MOV.B    RD25, @RQ14
  MOV.L    RD30, @(RQ10, 88)
  MOV.Q    @(RQ10, 32), RQ14
  MOV.Q    RQ14, @(RQ10, 48)
  EXTS.L   RD30, RQ28
  MOV.Q    RQ28, @(RQ10, 24)
  MOV.L    RD27, @(RQ10, 92)
  MOV.L    RD30, @(RQ10, 96)
  MOV.L    RD30, @(RQ10, 100)
  MOV.L    RD27, @(RQ10, 104)
  MOV.L    RD27, @(RQ10, 108)
  MOV.L    RD30, @(RQ9, 4)
  MOV.L    RD30, @(RQ9, 8)
  MOV.L    RD27, @(RQ9, 12)
  MOV.L    RD29, @(RQ9, 76)
  MOV      TU3CD6E720/buffer3, RQ31
  MOV.Q    RQ31, @(RQ9, 80)
  MOV.Q    @(RQ9, 80), RQ14
  MOV.Q    RQ14, @(RQ9, 32)
  MOV.Q    @(RQ9, 32), RQ14
  MOV      RQ14, RQ11
  ADD      RQ14, #1, RQ26
  MOV.Q    RQ26, @(RQ9, 32)
  MOV.B    RD30, @RQ11
  MOV.Q    @(RQ9, 32), RQ14
  MOV      RQ14, RQ11
  ADD      RQ14, #1, RQ26
  MOV.Q    RQ26, @(RQ9, 32)
  MOV.B    RD30, @RQ11
  MOV      #6144, RQ28
  EXTS.L   RQ28, RQ28
  MOV.Q    RQ28, @(RQ9, 40)
  MOV.Q    @(RQ9, 32), RQ14
  MOV.Q    @(RQ9, 40), RQ28
  LEA.B    @(RQ14, RQ28), RQ11
  MOV.Q    RQ11, @(RQ9, 56)
  MOV.Q    @(RQ9, 56), RQ14
  MOV.B    RD25, @RQ14
  MOV.L    RD30, @(RQ9, 88)
  MOV.Q    @(RQ9, 32), RQ14
  MOV.Q    RQ14, @(RQ9, 48)
  EXTS.L   RD30, RQ28
  MOV.Q    RQ28, @(RQ9, 24)
  MOV.L    RD27, @(RQ9, 92)
  MOV.L    RD30, @(RQ9, 96)
  MOV.L    RD30, @(RQ9, 100)
  MOV.L    RD27, @(RQ9, 104)
  MOV.L    RD27, @(RQ9, 108)
  MOV      #0, RD29
  MOV.L    RD29, @(R15, 76)
  MOV.Q    RQ26, @(R15, 16)

L008003F9:
  MOV.L    @(R15, 76), RD7
  CMP/GE   #253, RD7
  BT       L008003FA
  MOV      #0, RQ14
  MOV      __userFiles, RQ6
  MOV.L    @(R15, 76), RD5
  MOV.Q    RQ14, @(RQ6, RD5)

L008003FB:
  MOV.L    @(R15, 76), RD4
  ADD      #1, RD4
  MOV.L    RD4, @(R15, 76)
  BRA      L008003F9

L008003FA:
  MOV      L00C000D0, RQ4
  BSR      tk_puts
  MOV.B    @RQ13, RD3
  MOV.L    RD3, @(R15, 4)
  CMP/EQ   #34, RD3
  BT       L008003FC
  ADD      #1, RQ13
  MOV      argv, RQ2
  MOV.Q    RQ13, @RQ2

L008003FD:
  MOV      #0, RQ23
  CMPQ/EQ  RQ23, RQ13
  BT       L008003FE
  MOV.B    @RQ13, RD22
  MOV.L    RD22, @(R15, 4)
  CMP/EQ   #34, RD22
  BT       L008003FE
  BRA      L008003FF

L008003FF:
  ADD      #1, RQ13
  BRA      L008003FD

L008003FE:
  MOV      RQ13, RQ14
  ADD      #1, RQ13
  MOV      #0, RD21
  MOV.B    RD21, @RQ14
  BRA      L00800400

L008003FC:
  MOV      argv, RQ25
  MOV.Q    RQ13, @RQ25
  MOV      RQ13, RQ4
  MOV      #32, RD5
  BSR      strchr
  MOV      RQ2, RQ13
  MOV      #0, RQ14
  MOV      RQ13, RQ11
  MOV      RQ14, RQ26
  MOV.Q    RQ26, @(R15, 16)
  CMPQ/EQ  RQ26, RQ11
  BT       L00800401
  MOV      L00C000C5, RQ20
  MOV      RQ20, RQ13
  BRA      L00800402

L00800401:
  MOV      #0, RD19
  MOV.B    RD19, @RQ13
  ADD      #1, RQ13

L00800402:

L00800400:
  MOV      L00C000D2, RQ4
  BSR      tk_puts

L00800403:
  MOV.B    @RQ13, RD18
  MOV.L    RD18, @(R15, 4)
  CMP/EQ   #32, RD18
  BT       L00800404
  ADD      #1, RQ13
  BRA      L00800403

L00800404:
  MOV.B    @RQ13, RD17
  MOV.L    RD17, @(R15, 4)
  CMP/EQ   #0, RD17
  BT       L00800405
  MOV      #0, RQ14
  MOV      argv, RQ16
  MOV.Q    RQ14, @(RQ16, 8)
  MOV      #1, RD17
  MOV.L    RD17, @(R15, 72)
  BRA      L00800406

L00800405:
  MOV      #1, RD16
  MOV.L    RD16, @(R15, 76)

L00800407:
  MOV.L    @(R15, 76), RD16
  MOV      #50, RD17
  CMP/GE   RD17, RD16
  BT       L00800408
  MOV      #32, RD16
  MOV      RD16, RD17
  MOV.B    @RQ13, RD16
  MOV.L    RD17, @(R15, 52)
  MOV.L    RD16, @(R15, 4)
  CMP/EQ   #34, RD16
  BT       L00800409
  ADD      #1, RQ13
  MOV      #34, RD16
  MOV      RD16, RD17
  MOV.L    RD17, @(R15, 52)
  MOV.L    RD16, @(R15, 4)

L00800409:
  MOV      argv, RQ16
  MOV.L    @(R15, 76), RD17
  EXTS.L   RD17
  MOV.Q    RQ13, @(RQ16, RD17)
  ADD      #1, RD17
  MOV.L    RD17, @(R15, 76)
  MOV      #50, RD18
  CMP/GE   RD18, RD17
  BT       L0080040A
  BRA      L00800408

L0080040A:
  MOV      RQ13, RQ4
  MOV.L    @(R15, 52), RD5
  BSR      strchr
  MOV      RQ2, RQ13
  MOV      #0, RQ14
  MOV      RQ13, RQ11
  MOV      RQ14, RQ25
  MOV.Q    RQ25, @(R15, 16)
  CMPQ/EQ  RQ25, RQ11
  BT       L0080040B
  BRA      L00800408
  BRA      L0080040C

L0080040B:
  MOV      #0, RD16
  MOV.B    RD16, @RQ13
  ADD      #1, RQ13

L0080040D:
  MOV.B    @RQ13, RD16
  MOV.L    RD16, @(R15, 4)
  CMP/EQ   #32, RD16
  BT       L0080040E
  ADD      #1, RQ13
  BRA      L0080040D

L0080040E:
  MOV.B    @RQ13, RD16
  MOV.L    RD16, @(R15, 4)
  CMP/EQ   #0, RD16
  BT       L0080040F
  BRA      L00800408

L0080040F:

L0080040C:

L00800410:
  BRA      L00800407

L00800408:
  MOV      #0, RQ14
  MOV      argv, RQ16
  MOV.L    @(R15, 76), RD17
  EXTS.L   RD17
  MOV.Q    RQ14, @(RQ16, RD17)
  MOV      RD17, RD18
  MOV.L    RD18, @(R15, 72)

L00800406:
  MOV      L00C000D3, RQ4
  BSR      tk_puts
  BSR      __start_late
  MOV      L00C000D5, RQ4
  BSR      tk_puts
  MOV.L    @(R15, 72), RD4
  MOV      argv, RQ5
  BSR      main
  MOV      RD2, RD25
  MOV.L    RD25, @(R15, 68)
  MOV      RD25, RD4
  BSR      __exit
  MOV      RD25, RD2

L00E00443:
  ADD      #104, R15
  BRA      L00C00027

__start_early:
  PUSH     LR
  ADD      #-136, R15
  BSR      TKMM_Init

L00E00444:
  ADD      #136, R15
  RET     

__start_late:
  ADD      #-144, R15

L00E00445:
  ADD      #144, R15
  RTS     

__datetime:
  PUSH     LR
  BSR      L00C00037
  ADD      #-104, R15
  MOV.Q    RQ4, @(R15, 8)
  BSR      TK_GetTimeMs
  MOV      RD2, RD11
  MOV.Q    @(R15, 8), RQ10
  MOV      RQ10, RQ14
  MOV      RD11, RD13
  MOV      #0x1999999A, RD7
  MOV      #6553, RD7
  DMULS    RD7, RD13
  STS      R1, RD13
  MOV      RD13, RD12
  MOV      #0x028F5C29, RD7
  MOV      #655, RD7
  DMULS    RD7, RD12
  STS      R1, RD6
  MOV      #100, RD7
  DMULU    RD7, RD6
  STS      R0, RD6
  SUB      RD6, RD12
  MOV.L    RD12, @(RQ14, 24)
  MOV      RD11, RD13
  MOV      #0x00418938, RD7
  MOV      #65, RD7
  DMULS    RD7, RD13
  STS      R1, RD13
  MOV      RD13, RD12
  MOV      #0x04444445, RD7
  MOV      #1092, RD7
  DMULS    RD7, RD12
  STS      R1, RD6
  MOV      #60, RD7
  DMULU    RD7, RD6
  STS      R0, RD6
  SUB      RD6, RD12
  MOV.L    RD12, @(RQ14, 20)
  MOV      RD11, RD13
  MOV      #0x0001179F, RD7
  MOV      #1, RD7
  DMULS    RD7, RD13
  STS      R1, RD13
  MOV      RD13, RD12
  MOV      #0x04444445, RD7
  MOV      #1092, RD7
  DMULS    RD7, RD12
  STS      R1, RD6
  MOV      #60, RD7
  DMULU    RD7, RD6
  STS      R0, RD6
  SUB      RD6, RD12
  MOV.L    RD12, @(RQ14, 16)
  MOV      #0, RD9
  MOV.L    RD9, @(RQ14, 12)
  MOV.L    RD9, @(RQ14, 8)
  MOV.L    RD9, @(RQ14, 4)
  MOV.L    RD9, @RQ14

L00E00446:
  ADD      #104, R15
  BRA      L00C00038

TK_GetTimeMs:
  PUSH     LR
  BSR      L00C0008B
  ADD      #-104, R15
  MOV      #0xA000E000, RQ14
  MOV      #0x0000A000, RQ14
  MOVU.L   @RQ14, RD12
  MOVU.L   @(RQ14, 4), RD13
  EXTU.L   RD13, RQ11
  MOV      RQ11, RQ10
  MOV      #32, RD9
  SHLD.Q   RQ10, RD9, RQ10
  MOV      RQ10, RQ11
  EXTU.L   RD12, RQ10
  OR       RQ11, RQ10, RQ8
  MOV      RQ8, RQ10
  MOV      RQ10, RQ11
  MOV      #10, RD31
  SHLR.Q   RQ11, RD31, RQ11
  EXTS.L   RQ11, RD9
  MOV.L    RD9, @(R15, 20)
  MOV.Q    RQ8, @(R15, 24)
  MOV      RD9, RD2

L00E00447:
  ADD      #104, R15
  BRA      L00C0008C

__assert:
  PUSH     LR
  PUSH     R14
  PUSH     R13
  ADD      #-168, R15
  MOV.Q    RQ4, @(R15, 40)
  MOV.Q    RQ5, @(R15, 32)
  MOV.L    RD6, @(R15, 28)
  BSR      __get_stderr
  MOV      RQ2, RQ14
  MOV.Q    @RQ14, RQ13
  MOV.Q    RQ13, @(R15, 8)
  MOV      RQ13, RQ4
  MOV      L00C000DA, RQ5
  MOV.Q    @(R15, 40), RQ6
  MOV.Q    @(R15, 32), RQ7
  MOV.L    @(R15, 28), RD20
  BSR      fprintf
  BSR      abort
  MOV      #0, RD2

L00E00448:
  ADD      #168, R15
  POP      RQ13
  POP      RQ14
  RET     

__cytpe_init:
  PUSH     LR
  BSR      L00C00037
  ADD      #-616, R15
  MOV.Q    __isbuf, RQ14
  MOV      #0, RQ13
  CMPQ/EQ  RQ13, RQ14
  BT       L0080046E
  BRA      L00E00449

L0080046E:
  MOV      TU3CD6E720/__isbufR, RQ12
  MOV      #1, RD11
  LEA.W    @(RQ12, RD11), RQ10
  MOV      RQ10, RQ9
  MOV      TU3CD6E720/__tolowR, RQ7
  LEA.W    @(RQ7, RD11), RQ10
  MOV      RQ10, RQ6
  MOV      TU3CD6E720/__toupR, RQ5
  LEA.W    @(RQ5, RD11), RQ10
  MOV      RQ10, RQ4
  MOV.Q    RQ9, __isbuf
  MOV.Q    RQ6, __tolow
  MOV.Q    RQ4, __toup

L00E00449:
  ADD      #616, R15
  BRA      L00C00038

fabs:
  PUSH     FR15
  PUSH     FR14
  PUSH     FR13
  ADD      #-200, R15
  FMOV.D   DR4, @(R15, 8)
  FMOV.D   @(R15, 8), DR15
  MOV      #0, DR14
  FCMP/GT  DR15, DR14
  BT       L00800471
  FMOV.D   @(R15, 8), DR13
  FNEG     DR13
  FMOV.D   DR13, @(R15, 8)

L00800471:
  FMOV.D   @(R15, 8), DR2

L00E0044A:
  ADD      #200, R15
  POP      FR13
  POP      FR14
  POP      FR15
  RTS     

L00C000E1:
  PUSH     R31
  PUSH     R30
  PUSH     R29
  PUSH     R28
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  PUSH     R9
  PUSH     R8
  PUSH     FR15
  PUSH     FR14
  PUSH     FR13
  PUSH     FR12
  PUSH     FR11
  PUSH     FR10
  PUSH     FR9
  RTS     

frexp:
  PUSH     LR
  BSR      L00C000E1
  ADD      #-120, R15
  FMOV.D   DR4, @(R15, 48)
  MOV.Q    RQ4, @(R15, 40)
  FMOV.D   @(R15, 48), DR15
  MOV      #0, DR14
  FCMP/EQ  DR14, DR15
  BT       L008004A1
  MOV      #0, RQ14
  MOV      RQ14, RQ11
  MOV.Q    RQ11, @(R15, 40)
  MOV      #0, DR2
  BRA      L00E0044B

L008004A1:
  FMOV.D   @(R15, 48), DR13
  MOV      #32, RQ10
  ADD      R15, RQ10
  FMOV.D   DR13, @RQ10
  MOV      RQ10, RQ14
  MOVU.W   @RQ14, RD13
  SHAD     RD13, #-8, RD12
  AND      RD12, #127, RD13
  SUB      RD13, #64, RD12
  SHAD     RD12, #2, RD13
  MOV.Q    @(R15, 40), RQ9
  MOV.L    RD13, @RQ9
  MOV      RQ10, RQ14
  MOVU.W   @RQ14, RD13
  AND      RD13, #33023, RD12
  MOV      RQ10, RQ14
  MOVU.W   RD12, @RQ14
  MOV      RQ10, RQ14
  MOVU.W   @RQ14, RD13
  OR       RD13, #16384, RD12
  MOV      RQ10, RQ14
  MOVU.W   RD12, @RQ14

L008004A2:
  MOV      #32, RQ8
  ADD      R15, RQ8
  FMOV.D   @RQ8, DR12
  FMOV.D   DR12, @(R15, 8)
  FMOV     DR12, DR4
  BSR      fabs
  FMOV     DR2, DR11
  FMOV.D   DR11, @R15
  MOV      #0, DR10
  FCMP/GT  DR11, DR10
  BT       L008004A3
  MOV      #32, RQ31
  ADD      R15, RQ31
  FMOV.D   @RQ31, DR9
  FMOV.D   DR9, @(R15, 8)
  MOV      #0, DR10
  FCMP/EQ  DR10, DR9
  BT       L008004A3
  BRA      L008004A4

L008004A4:
  MOV      #32, RQ30
  ADD      R15, RQ30
  FMOV.D   @RQ30, DR9
  FMOV     DR9, DR10
  MOV      #0, DR11
  FMUL     DR11, DR10
  FMOV.D   DR10, @RQ30
  MOV.Q    @(R15, 40), RQ29
  MOV.L    @RQ29, RD13
  SUB      RD13, #1, RD12
  MOV.L    RD12, @RQ29
  FMOV.D   DR10, @R15
  FMOV.D   DR9, @(R15, 8)
  BRA      L008004A2

L008004A3:
  MOV      #32, RQ28
  ADD      R15, RQ28
  FMOV.D   @RQ28, DR9
  FMOV.D   DR9, @(R15, 8)
  FMOV     DR9, DR2

L00E0044B:
  ADD      #120, R15

L00C000E2:
  POP      FR9
  POP      FR10
  POP      FR11
  POP      FR12
  POP      FR13
  POP      FR14
  POP      FR15
  POP      RQ8
  POP      RQ9
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  POP      RQ28
  POP      RQ29
  POP      RQ30
  POP      RQ31
  RET     

L00C000E4:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  PUSH     R9
  PUSH     R8
  PUSH     FR15
  PUSH     FR14
  PUSH     FR13
  PUSH     FR12
  PUSH     FR11
  PUSH     FR10
  PUSH     FR9
  RTS     

pow:
  PUSH     LR
  BSR      L00C000E4
  ADD      #-168, R15
  FMOV.D   DR4, @(R15, 72)
  FMOV.D   DR5, @(R15, 64)
  MOV      #0, RD12
  FMOV.D   @(R15, 64), DR15
  FSTCI    DR15
  MOV      R0, RD13
  MOV      RD13, RD14
  EXTS.L   RD14
  MOV      RD14, R0
  FLDCI    DR14
  FMOV     DR14, DR13
  FMOV.D   DR14, @(R15, 24)
  FMOV.D   DR13, @(R15, 48)
  FCMP/EQ  DR15, DR13
  BT       L00800495
  FMOV.D   @(R15, 72), DR12
  FMOV.D   DR12, @(R15, 40)
  FMOV.D   @(R15, 64), DR11
  MOV      #0, DR10
  FCMP/GT  DR11, DR10
  BT       L00800496
  MOV      #1, RD12
  NEG      RD14, RD14

L00800496:
  FMOV.D   @(R15, 64), DR9
  MOV      #0, DR10
  FCMP/EQ  DR10, DR9
  BT       L00800497
  MOV      #0, DR2
  BRA      L00E0044C

L00800497:
  ADD      #-1, RD14
  MOV      #256, RD11
  MOV.L    RD11, @(R15, 60)

L00800498:
  CMP/PL   RD14
  BT       L00800499
  MOV.L    @(R15, 60), RD10
  MOV      RD10, RD13
  ADD      #-1, RD10
  MOV.L    RD10, @(R15, 60)
  CMP/PL   RD13
  BT       L00800499
  BRA      L0080049A

L0080049A:
  FMOV.D   @(R15, 72), DR9
  FMOV.D   @(R15, 40), DR10
  FMUL     DR9, DR10
  ADD      #-1, RD14
  FMOV.D   DR10, @(R15, 40)
  BRA      L00800498

L00800499:
  CMP/EQ   RD12
  BT       L0080049B
  MOV      #0, DR9
  FMOV.D   @(R15, 40), DR10
  FMOV     DR9, DR11
  FDIV     DR10, DR11
  FMOV.D   DR11, @(R15, 40)
  FMOV.D   DR10, @(R15, 16)
  FMOV.D   DR9, @(R15, 24)

L0080049B:
  FMOV.D   @(R15, 40), DR2
  BRA      L00E0044C

L00800495:
  FMOV.D   @(R15, 72), DR9
  MOV      #0, DR10
  FCMP/GT  DR9, DR10
  BT       L0080049C
  BSR      __get_errno
  MOV      RQ2, RQ9
  MOV      #33, RD8
  MOV.L    RD8, @RQ9
  MOV.Q    RQ9, @(R15, 8)
  MOV      #0, DR2
  BRA      L00E0044C

L0080049C:
  FMOV.D   @(R15, 64), DR9
  MOV      #0, DR10
  FCMP/EQ  DR10, DR9
  BT       L0080049D
  MOV      #0, DR2
  BRA      L00E0044C

L0080049D:
  FMOV.D   @(R15, 72), DR4
  BSR      log
  FMOV     DR2, DR9
  FMOV.D   @(R15, 64), DR10
  FMUL     DR9, DR10
  FMOV.D   DR10, @(R15, 16)
  FMOV.D   DR9, @(R15, 24)
  FMOV     DR10, DR4
  BSR      exp
  FMOV     DR2, DR9
  FMOV.D   DR9, @(R15, 24)
  FMOV     DR9, DR2

L00E0044C:
  ADD      #168, R15

L00C000E7:
  POP      FR9
  POP      FR10
  POP      FR11
  POP      FR12
  POP      FR13
  POP      FR14
  POP      FR15
  POP      RQ8
  POP      RQ9
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  RET     

L00C000E8:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     FR15
  PUSH     FR14
  PUSH     FR13
  PUSH     FR12
  PUSH     FR11
  PUSH     FR10
  PUSH     FR9
  RTS     

exp:
  PUSH     LR
  BSR      L00C000E8
  ADD      #-168, R15
  FMOV.D   DR4, @(R15, 40)
  MOV      #2, RD13
  FMOV.D   @(R15, 40), DR15
  FMOV.D   @(R15, 40), DR14
  MOV      #256, RD14
  FMOV.D   DR15, @(R15, 32)
  FMOV.D   DR14, @(R15, 24)

L0080048D:
  MOV      RD14, RD12
  ADD      #-1, RD14
  MOV.L    RD12, @(R15, 12)
  CMP/PL   RD12
  BT       L0080048E
  EXTS.L   RD13
  MOV      RD13, R0
  FLDCI    DR13
  FMOV     DR13, DR12
  FMOV.D   @(R15, 32), DR13
  FMOV.D   @(R15, 40), DR11
  FMUL     DR11, DR13
  FMOV     DR13, DR10
  FDIV     DR12, DR10
  FMOV.D   @(R15, 24), DR13
  FADD     DR10, DR13
  FMOV.D   DR13, @R15
  FMOV.D   DR12, @(R15, 16)
  FMOV.D   DR10, @(R15, 32)
  FMOV.D   @(R15, 24), DR9
  FCMP/EQ  DR13, DR9
  BT       L0080048F
  BRA      L0080048E

L0080048F:
  FMOV.D   @(R15, 32), DR9
  FMOV.D   @(R15, 24), DR10
  FADD     DR9, DR10
  ADD      #1, RD13
  FMOV.D   DR10, @(R15, 24)
  BRA      L0080048D

L0080048E:
  MOV      #0, DR9
  FMOV.D   @(R15, 24), DR10
  FADD     DR9, DR10
  FMOV.D   DR10, @(R15, 24)
  FMOV     DR10, DR2

L00E0044D:
  ADD      #168, R15

L00C000E9:
  POP      FR9
  POP      FR10
  POP      FR11
  POP      FR12
  POP      FR13
  POP      FR14
  POP      FR15
  POP      RQ12
  POP      RQ13
  POP      RQ14
  RET     

L00C000EA:
  PUSH     R31
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  PUSH     R9
  PUSH     R8
  PUSH     FR15
  PUSH     FR14
  PUSH     FR13
  PUSH     FR12
  PUSH     FR11
  PUSH     FR10
  PUSH     FR9
  RTS     

log:
  PUSH     LR
  BSR      L00C000EA
  ADD      #-176, R15
  FMOV.D   DR4, @(R15, 88)
  FMOV.D   @(R15, 88), DR15
  MOV      #0, DR14
  FCMP/GT  DR14, DR15
  BT       L00800490
  BSR      __get_errno
  MOV      RQ2, RQ14
  MOV      #33, RD13
  MOV.L    RD13, @RQ14
  MOV      #0x75D49490, DR2
  BRA      L00E0044E

L00800490:
  FMOV.D   @(R15, 88), DR13
  MOV      #0, DR12
  FCMP/EQ  DR12, DR13
  BT       L00800491
  MOV      #0, DR2
  BRA      L00E0044E

L00800491:
  MOV      #76, RQ14
  ADD      R15, RQ14
  FMOV.D   @(R15, 88), DR4
  MOV      RQ14, RQ4
  BSR      frexp
  FMOV     DR2, DR11
  FMOV     DR11, DR10
  MOV      #0, DR9
  FMUL     DR9, DR10
  FMOV     DR10, DR11
  FSUB     DR9, DR11
  MOV.L    RD12, @(R15, 76)
  MOV      #2, RD11
  FMOV     DR11, DR12
  FMOV     DR11, DR13
  MOV      #256, RD10
  FMOV.D   DR13, @(R15, 64)
  FMOV.D   DR12, @(R15, 56)
  FMOV.D   DR11, @(R15, 40)
  FMOV.D   DR10, @(R15, 24)
  MOV.L    RD11, @(R15, 84)
  MOV.L    RD10, @(R15, 80)

L00800492:
  MOV.L    @(R15, 80), RD8
  MOV      RD8, RD9
  ADD      #-1, RD8
  MOV.L    RD9, @(R15, 20)
  MOV.L    RD8, @(R15, 80)
  CMP/PL   RD9
  BT       L00800493
  MOV.L    @(R15, 84), RD31
  EXTS.L   RD31
  MOV      RD31, R0
  FLDCI    DR9
  FMOV     DR9, DR10
  FMOV.D   @(R15, 64), DR9
  FMOV.D   @(R15, 40), DR11
  FMUL     DR11, DR9
  FMOV     DR9, DR12
  FNEG     DR12
  FMOV     DR12, DR9
  FDIV     DR10, DR9
  FMOV.D   @(R15, 56), DR13
  FADD     DR9, DR13
  FMOV.D   DR13, @(R15, 8)
  FMOV.D   DR12, @(R15, 64)
  FMOV.D   DR10, @(R15, 48)
  FMOV.D   DR9, @(R15, 24)
  FMOV.D   @(R15, 56), DR14
  FCMP/EQ  DR13, DR14
  BT       L00800494
  BRA      L00800493

L00800494:
  FMOV.D   @(R15, 64), DR9
  FMOV.D   @(R15, 48), DR10
  FDIV     DR10, DR9
  FMOV.D   @(R15, 56), DR11
  FADD     DR9, DR11
  MOV.L    @(R15, 84), RD7
  ADD      #1, RD7
  FMOV.D   DR11, @(R15, 56)
  FMOV.D   DR9, @(R15, 24)
  MOV.L    RD7, @(R15, 84)
  BRA      L00800492

L00800493:
  MOV.L    @(R15, 76), RD6
  EXTS.L   RD6
  MOV      RD6, R0
  FLDCI    DR9
  FMOV     DR9, DR10
  MOV      #0xFEFA39F0, DR11
  FMUL     DR11, DR10
  FMOV.D   @(R15, 56), DR12
  FADD     DR10, DR12
  FMOV.D   DR12, @(R15, 56)
  FMOV.D   DR10, @(R15, 8)
  FMOV.D   DR9, @(R15, 24)
  FMOV     DR12, DR2

L00E0044E:
  ADD      #176, R15

L00C000EB:
  POP      FR9
  POP      FR10
  POP      FR11
  POP      FR12
  POP      FR13
  POP      FR14
  POP      FR15
  POP      RQ8
  POP      RQ9
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  POP      RQ31
  RET     

__mmDef:
  ADD      #-160, R15
  MOV.Q    RQ4, @(R15, 8)

L00E0044F:
  ADD      #160, R15
  RTS     

__mmInit:
  PUSH     R14
  PUSH     R13
  ADD      #-144, R15
  MOV.Q    RQ4, @(R15, 8)
  MOV      #0, RQ14
  MOV.Q    @(R15, 8), RQ13
  MOV.Q    RQ14, @RQ13
  MOV      #0, RQ14
  MOV.Q    RQ14, @(RQ13, 8)

L00E00450:
  ADD      #144, R15
  POP      RQ13
  POP      RQ14
  RTS     

__mmTerm:
  ADD      #-160, R15
  MOV.Q    RQ4, @(R15, 8)

L00E00451:
  ADD      #160, R15
  RTS     

__mmSupply:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-120, R15
  MOV.Q    RQ4, @(R15, 88)
  MOV.Q    RQ5, @(R15, 80)
  MOV.Q    RQ6, @(R15, 72)
  MOV.Q    @(R15, 80), RQ7
  EXTS.L   RD7
  MOV      RD7, RD12
  AND      RD12, #7, RD8
  CMP/EQ   RD8
  BT       L008004AA
  MOV.Q    @(R15, 80), RQ7
  EXTS.L   RD7
  MOV      RD7, RD12
  AND      RD12, #7, RD8
  MOV      #8, RD31
  SUB      RD31, RD8, RD12
  MOV.Q    @(R15, 72), RQ30
  EXTS.L   RD12, RQ29
  SUB      RQ30, RQ29, RQ28
  MOV.Q    @(R15, 80), RQ7
  EXTS.L   RD7
  MOV      RD7, RD12
  AND      RD12, #7, RD8
  SUB      RD31, RD8, RD12
  MOV.Q    @(R15, 80), RQ27
  EXTS.L   RD12
  LEA.B    @(RQ27, RD12), RQ27
  MOV.Q    RQ30, @(R15, 48)
  MOV.Q    RQ28, @(R15, 72)
  MOV.Q    RQ27, @(R15, 80)

L008004AA:
  MOV.Q    @(R15, 72), RQ26
  AND      RQ26, #7, RQ25
  MOV.Q    RQ25, @(R15, 48)
  MOV      #0, RQ7
  CMPQ/EQ  RQ7, RQ25
  BT       L008004AB
  MOV.Q    @(R15, 72), RQ6
  AND      RQ6, #7, RQ5
  SUB      RQ5, RQ6
  MOV.Q    RQ6, @(R15, 72)
  MOV.Q    RQ5, @(R15, 48)

L008004AB:
  MOV.Q    @(R15, 88), RQ4
  MOV.Q    @RQ4, RQ11
  MOV      #0, RQ3
  MOV.Q    RQ3, @(R15, 64)

L008004AC:
  MOV      #0, RQ13
  MOV      RQ11, RQ10
  MOV      RQ13, RQ9
  CMPQ/EQ  RQ9, RQ10
  BT       L008004AD
  MOV.Q    @(R15, 80), RQ2
  CMPQ/GT  RQ2, RQ11
  BT       L008004AD
  BRA      L008004AE

L008004AE:
  MOV      RQ11, RQ23
  MOV.Q    @RQ11, RQ11
  MOV.Q    RQ23, @(R15, 64)
  BRA      L008004AC

L008004AD:
  MOV.Q    @(R15, 80), RQ22
  MOV      RQ22, RQ14
  MOV.Q    @(R15, 64), RQ21
  MOV.Q    RQ21, @(RQ14, 8)
  MOV.Q    RQ11, @RQ14
  MOV      #0, RQ13
  MOV      RQ21, RQ10
  MOV      RQ13, RQ9
  CMPQ/EQ  RQ9, RQ10
  BT       L008004AF
  MOV.Q    @(R15, 64), RQ20
  MOV.Q    RQ14, @RQ20
  BRA      L008004B0

L008004AF:
  MOV.Q    @(R15, 88), RQ19
  MOV.Q    RQ14, @RQ19

L008004B0:
  MOV      #0, RQ13
  MOV      RQ11, RQ10
  MOV      RQ13, RQ9
  CMPQ/EQ  RQ9, RQ10
  BT       L008004B1
  MOV.Q    RQ14, @(RQ11, 8)

L008004B1:
  MOV      #1, RD18
  MOV.L    RD18, @(RQ14, 32)
  MOV.Q    @(R15, 72), RQ17
  MOV.Q    RQ17, @(RQ14, 40)
  MOV      #0, RD16
  MOV.L    RD16, @(RQ14, 48)
  MOV.Q    @(R15, 88), RQ19
  MOV.Q    @(RQ19, 8), RQ13
  MOV.Q    RQ13, @(RQ14, 16)
  MOV.Q    @(RQ14, 16), RQ13
  MOV      #0, RQ10
  MOV      RQ13, RQ9
  MOV      RQ10, RQ20
  MOV.Q    RQ20, @(R15, 8)
  CMPQ/EQ  RQ20, RQ9
  BT       L008004B2
  MOV.Q    @(RQ14, 16), RQ13
  MOV.Q    RQ14, @(RQ13, 24)

L008004B2:
  MOV      #0, RQ13
  MOV.Q    RQ13, @(RQ14, 24)
  MOV.Q    @(R15, 88), RQ16
  MOV.Q    RQ14, @(RQ16, 8)

L00E00452:
  ADD      #120, R15
  BRA      L00C00027

__mmAlloc:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-136, R15
  MOV.Q    RQ4, @(R15, 104)
  MOV      RQ5, RQ8
  MOV.L    RD6, @(R15, 92)
  MOV      RQ8, RQ9
  ADD      #64, RQ8
  AND      RQ8, #127, RQ10
  MOV      #0, RQ31
  CMPQ/EQ  RQ31, RQ10
  BT       L008004B3
  SHLD.Q   RQ8, #-7, RQ10
  ADD      RQ10, #1, RQ30
  SHLD.Q   RQ30, #7, RQ8
  MOV.Q    RQ30, @(R15, 64)

L008004B3:
  MOV      RQ8, RQ10
  MOV      RQ9, RQ29
  MOV.Q    RQ29, @(R15, 64)
  CMPQ/GT  RQ10, RQ29
  BT       L008004B4
  MOV      #0, RQ14
  MOV      RQ14, RQ2
  BRA      L00E00453

L008004B4:
  MOV.Q    @(R15, 104), RQ28
  MOV.Q    @(RQ28, 8), RQ13

L008004B5:
  MOV      #0, RQ14
  MOV      RQ13, RQ11
  MOV      RQ14, RQ27
  MOV.Q    RQ27, @(R15, 40)
  CMPQ/EQ  RQ27, RQ11
  BT       L008004B6
  MOV.Q    @(RQ13, 40), RQ10
  MOV      RQ10, RQ26
  MOV      RQ8, RQ25
  MOV.Q    RQ26, @(R15, 64)
  MOV.Q    RQ25, @(R15, 32)
  CMPQ/GT  RQ26, RQ25
  BT       L008004B7
  MOV.L    @(RQ13, 48), RD7
  MOV.L    RD7, @(R15, 28)
  CMP/EQ   RD7
  BT       L008004B8
  MOV      #0, RQ14
  MOV      #0, RD25
  MOV.B    RD25, @RQ14
  MOV      #1, RD4
  BSR      exit

L008004B8:
  MOV.Q    @(RQ13, 40), RQ10
  MOV      RQ10, RQ6
  MOV      RQ8, RQ5
  SUB      RQ6, RQ5, RQ4
  MOV.Q    RQ6, @(R15, 32)
  MOV.Q    RQ5, @(R15, 16)
  MOV.Q    RQ4, @(R15, 64)
  MOV      #128, RQ3
  CMPQ/GT  RQ4, RQ3
  BT       L008004B9
  LEA.B    @(RQ13, RQ8), RQ14
  MOV      RQ14, RQ12
  MOV.Q    @RQ13, RQ14
  MOV.Q    RQ14, @RQ12
  MOV.Q    @RQ12, RQ14
  MOV      #0, RQ11
  MOV      RQ14, RQ2
  MOV      RQ11, RQ23
  MOV.Q    RQ2, @(R15, 40)
  MOV.Q    RQ23, @(R15, 8)
  CMPQ/EQ  RQ23, RQ2
  BT       L008004BA
  MOV.Q    @RQ12, RQ14
  MOV.Q    RQ12, @(RQ14, 8)

L008004BA:
  MOV.Q    RQ13, @(RQ12, 8)
  MOV.Q    RQ12, @RQ13
  MOV      #0, RD22
  MOV.L    RD22, @(RQ12, 32)
  MOV.Q    @(RQ13, 40), RQ10
  MOV      RQ10, RQ21
  MOV      RQ8, RQ20
  SUB      RQ21, RQ20, RQ19
  MOV      RQ19, RQ10
  MOV.Q    RQ10, @(RQ12, 40)
  MOV.L    RD22, @(RQ12, 48)
  MOV.Q    RQ8, @(RQ13, 40)
  MOV.Q    @(RQ13, 16), RQ14
  MOV.Q    RQ14, @(RQ12, 16)
  MOV.Q    @(RQ13, 24), RQ14
  MOV.Q    RQ14, @(RQ12, 24)
  MOV.Q    @(RQ12, 16), RQ14
  MOV      #0, RQ11
  MOV      RQ14, RQ18
  MOV      RQ11, RQ17
  MOV.Q    RQ21, @(R15, 32)
  MOV.Q    RQ20, @(R15, 16)
  MOV.Q    RQ19, @(R15, 64)
  MOV.Q    RQ18, @(R15, 40)
  MOV.Q    RQ17, @(R15, 8)
  CMPQ/EQ  RQ17, RQ18
  BT       L008004BB
  MOV.Q    @(RQ12, 16), RQ14
  MOV.Q    RQ12, @(RQ14, 24)

L008004BB:
  MOV.Q    @(RQ12, 24), RQ14
  MOV      #0, RQ11
  MOV      RQ14, RQ16
  MOV      RQ11, RQ17
  MOV.Q    RQ17, @(R15, 8)
  MOV.Q    RQ16, @(R15, 40)
  CMPQ/EQ  RQ17, RQ16
  BT       L008004BC
  MOV.Q    @(RQ12, 24), RQ14
  MOV.Q    RQ12, @(RQ14, 16)
  BRA      L008004BD

L008004BC:
  MOV.Q    @(R15, 104), RQ16
  MOV.Q    @(RQ16, 8), RQ14
  CMPQ/EQ  RQ13, RQ14
  BT       L008004BE
  MOV      #0, RQ14
  MOV      #0, RD25
  MOV.B    RD25, @RQ14
  MOV      #1, RD4
  BSR      exit
  BRA      L008004BF

L008004BE:
  MOV.Q    @(R15, 104), RQ16
  MOV.Q    RQ12, @(RQ16, 8)

L008004BF:

L008004BD:
  BRA      L008004C0

L008004B9:
  MOV.Q    @(RQ13, 16), RQ14
  MOV      #0, RQ11
  MOV      RQ14, RQ16
  MOV      RQ11, RQ17
  MOV.Q    RQ17, @(R15, 8)
  MOV.Q    RQ16, @(R15, 40)
  CMPQ/EQ  RQ17, RQ16
  BT       L008004C1
  MOV.Q    @(RQ13, 24), RQ14
  MOV.Q    @(RQ13, 16), RQ11
  MOV.Q    RQ14, @(RQ11, 24)

L008004C1:
  MOV.Q    @(RQ13, 24), RQ14
  MOV      #0, RQ11
  MOV      RQ14, RQ16
  MOV      RQ11, RQ17
  MOV.Q    RQ17, @(R15, 8)
  MOV.Q    RQ16, @(R15, 40)
  CMPQ/EQ  RQ17, RQ16
  BT       L008004C2
  MOV.Q    @(RQ13, 16), RQ14
  MOV.Q    @(RQ13, 24), RQ11
  MOV.Q    RQ14, @(RQ11, 16)
  BRA      L008004C3

L008004C2:
  MOV.Q    @(R15, 104), RQ16
  MOV.Q    @(RQ16, 8), RQ14
  CMPQ/EQ  RQ13, RQ14
  BT       L008004C4
  MOV      #0, RQ14
  MOV      #0, RD25
  MOV.B    RD25, @RQ14
  MOV      #1, RD4
  BSR      exit
  BRA      L008004C5

L008004C4:
  MOV.Q    @(RQ13, 16), RQ14
  MOV.Q    @(R15, 104), RQ16
  MOV.Q    RQ14, @(RQ16, 8)

L008004C5:

L008004C3:

L008004C0:
  MOV      #0, RQ14
  MOV.Q    RQ14, @(RQ13, 16)
  MOV      #0, RQ14
  MOV.Q    RQ14, @(RQ13, 24)
  MOV      #23130, RD16
  MOV.L    RD16, @(RQ13, 48)
  MOV.L    @(R15, 92), RD17
  MOV.L    RD17, @(RQ13, 52)
  BRA      L008004B6

L008004B7:
  MOV.Q    @(RQ13, 16), RQ13
  BRA      L008004B5

L008004B6:
  MOV      #0, RQ14
  MOV      RQ13, RQ11
  MOV      RQ14, RQ16
  MOV.Q    RQ16, @(R15, 40)
  CMPQ/EQ  RQ16, RQ11
  BT       L008004C6
  MOV      RQ13, RQ2
  BRA      L00E00453
  BRA      L008004C7

L008004C6:
  ADD      RQ13, #64, RQ14
  MOV      RQ14, RQ16
  ADD      RQ16, #-8, RQ14
  MOV.Q    RQ9, @RQ14
  ADD      RQ13, #64, RQ14
  MOV.Q    RQ16, @(R15, 80)
  MOV      RQ14, RQ2
  BRA      L00E00453

L008004C7:

L00E00453:
  ADD      #136, R15
  BRA      L00C00027

__mmFree:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-136, R15
  MOV.Q    RQ4, @(R15, 104)
  MOV.Q    RQ5, @(R15, 96)
  MOV      #0, RD8
  MOV      RD8, RD31
  MOV      #0, RD8
  MOV      RD8, RD30
  MOV.Q    @(R15, 96), RQ29
  ADD      RQ29, #-64, RQ14
  MOV      RQ14, RQ13
  MOV.L    @(RQ13, 48), RD8
  MOV.L    RD31, @(R15, 92)
  MOV.L    RD30, @(R15, 88)
  CMP/EQ   #23130, RD8
  BT       L008004C8
  MOV      #0, RQ14
  MOV      #0, RD28
  MOV.B    RD28, @RQ14
  MOV      #1, RD4
  BSR      exit

L008004C8:
  MOV      #0, RD27
  MOV.L    RD27, @(RQ13, 48)
  MOV.Q    @(RQ13, 8), RQ9
  MOV.Q    @RQ13, RQ11
  MOV.L    @(RQ13, 32), RD8
  CMP/EQ   RD8
  BT       L008004C9
  MOV      #0, RQ14
  MOV      RQ9, RQ12
  MOV      RQ14, RQ10
  CMPQ/EQ  RQ10, RQ12
  BT       L008004C9
  BRA      L008004CA

L008004CA:
  MOV.L    @(RQ9, 48), RD8
  CMP/EQ   RD8
  BT       L008004C9
  BRA      L008004CB

L008004CB:
  MOV.Q    @(RQ9, 40), RQ26
  MOV.Q    @(RQ13, 40), RQ25
  MOV      RQ26, RQ7
  MOV      RQ25, RQ6
  ADD      RQ7, RQ6, RQ5
  MOV      RQ5, RQ26
  MOV.Q    RQ26, @(RQ9, 40)
  MOV.Q    @RQ13, RQ14
  MOV.Q    RQ14, @RQ9
  MOV.Q    @RQ9, RQ14
  MOV      #0, RQ12
  MOV      RQ14, RQ10
  MOV      RQ12, RQ4
  MOV.Q    RQ26, @(R15, 48)
  MOV.Q    RQ25, @(R15, 40)
  MOV.Q    RQ7, @(R15, 24)
  MOV.Q    RQ6, @(R15, 16)
  MOV.Q    RQ5, @(R15, 32)
  MOV.Q    RQ4, @(R15, 8)
  CMPQ/EQ  RQ4, RQ10
  BT       L008004CC
  MOV.Q    @RQ9, RQ14
  MOV.Q    RQ9, @(RQ14, 8)

L008004CC:
  MOV      #1, RD3
  MOV.L    RD3, @(R15, 92)

L008004C9:
  MOV      #0, RQ14
  MOV      RQ11, RQ12
  MOV      RQ14, RQ10
  CMPQ/EQ  RQ10, RQ12
  BT       L008004CD
  MOV.L    @(RQ11, 48), RD8
  CMP/EQ   RD8
  BT       L008004CD
  BRA      L008004CE

L008004CE:
  MOV.L    @(RQ11, 32), RD8
  CMP/EQ   RD8
  BT       L008004CD
  BRA      L008004CF

L008004CF:
  MOV      #1, RD2
  MOV.L    RD2, @(R15, 88)

L008004CD:
  MOV.L    @(R15, 88), RD23
  CMP/EQ   #0, RD23
  BT       L008004D0
  MOV.L    @(R15, 92), RD22
  CMP/EQ   #0, RD22
  BT       L008004D0
  BRA      L008004D1

L008004D1:
  MOV.Q    @(RQ13, 40), RQ21
  MOV.Q    @(RQ11, 40), RQ20
  MOV      RQ21, RQ19
  MOV      RQ20, RQ18
  ADD      RQ19, RQ18, RQ17
  MOV      RQ17, RQ21
  MOV.Q    RQ21, @(RQ13, 40)
  MOV.Q    @RQ11, RQ14
  MOV.Q    RQ14, @RQ13
  MOV.Q    @RQ13, RQ14
  MOV      #0, RQ12
  MOV      RQ14, RQ10
  MOV      RQ12, RQ16
  MOV.Q    RQ21, @(R15, 48)
  MOV.Q    RQ20, @(R15, 40)
  MOV.Q    RQ19, @(R15, 24)
  MOV.Q    RQ18, @(R15, 16)
  MOV.Q    RQ17, @(R15, 32)
  MOV.Q    RQ16, @(R15, 8)
  CMPQ/EQ  RQ16, RQ10
  BT       L008004D2
  MOV.Q    @RQ13, RQ14
  MOV.Q    RQ13, @(RQ14, 8)

L008004D2:
  MOV.Q    @(RQ11, 16), RQ14
  MOV.Q    RQ14, @(RQ13, 16)
  MOV.Q    @(RQ13, 16), RQ14
  MOV      #0, RQ12
  MOV      RQ14, RQ10
  MOV      RQ12, RQ16
  MOV.Q    RQ16, @(R15, 8)
  CMPQ/EQ  RQ16, RQ10
  BT       L008004D3
  MOV.Q    @(RQ13, 16), RQ14
  MOV.Q    RQ13, @(RQ14, 24)

L008004D3:
  MOV.Q    @(RQ11, 24), RQ14
  MOV.Q    RQ14, @(RQ13, 24)
  MOV.Q    @(RQ13, 24), RQ14
  MOV      #0, RQ12
  MOV      RQ14, RQ10
  MOV      RQ12, RQ16
  MOV.Q    RQ16, @(R15, 8)
  CMPQ/EQ  RQ16, RQ10
  BT       L008004D4
  MOV.Q    @(RQ13, 24), RQ14
  MOV.Q    RQ13, @(RQ14, 16)
  BRA      L008004D5

L008004D4:
  MOV.Q    @(R15, 104), RQ16
  MOV.Q    @(RQ16, 8), RQ14
  CMPQ/EQ  RQ11, RQ14
  BT       L008004D6
  MOV      #0, RQ14
  MOV      #0, RD25
  MOV.B    RD25, @RQ14
  MOV      #1, RD4
  BSR      exit
  BRA      L008004D7

L008004D6:
  MOV.Q    @(R15, 104), RQ16
  MOV.Q    RQ13, @(RQ16, 8)

L008004D7:

L008004D5:
  BRA      L008004D8

L008004D0:
  MOV.L    @(R15, 88), RD16
  CMP/EQ   #0, RD16
  BT       L008004D9
  MOV.L    @(R15, 92), RD16
  CMP/EQ   #0, RD16
  BT       L008004D9
  BRA      L008004DA

L008004DA:
  MOV.Q    @(RQ11, 16), RQ14
  MOV      #0, RQ12
  MOV      RQ14, RQ10
  MOV      RQ12, RQ16
  MOV.Q    RQ16, @(R15, 8)
  CMPQ/EQ  RQ16, RQ10
  BT       L008004DB
  MOV.Q    @(RQ11, 24), RQ14
  MOV.Q    @(RQ11, 16), RQ12
  MOV.Q    RQ14, @(RQ12, 24)

L008004DB:
  MOV.Q    @(RQ11, 24), RQ14
  MOV      #0, RQ12
  MOV      RQ14, RQ10
  MOV      RQ12, RQ16
  MOV.Q    RQ16, @(R15, 8)
  CMPQ/EQ  RQ16, RQ10
  BT       L008004DC
  MOV.Q    @(RQ11, 16), RQ14
  MOV.Q    @(RQ11, 24), RQ12
  MOV.Q    RQ14, @(RQ12, 16)
  BRA      L008004DD

L008004DC:
  MOV.Q    @(R15, 104), RQ16
  MOV.Q    @(RQ16, 8), RQ14
  CMPQ/EQ  RQ11, RQ14
  BT       L008004DE
  MOV      #0, RQ14
  MOV      #0, RD25
  MOV.B    RD25, @RQ14
  MOV      #1, RD4
  BSR      exit
  BRA      L008004DF

L008004DE:
  MOV.Q    @(RQ11, 16), RQ14
  MOV.Q    @(R15, 104), RQ16
  MOV.Q    RQ14, @(RQ16, 8)
  MOV      #0, RQ14
  MOV.Q    @(RQ11, 16), RQ12
  MOV.Q    RQ14, @(RQ12, 24)

L008004DF:

L008004DD:
  MOV.Q    @(RQ9, 40), RQ16
  MOV.Q    @(RQ11, 40), RQ17
  MOV      RQ16, RQ18
  MOV      RQ17, RQ19
  ADD      RQ18, RQ19, RQ20
  MOV      RQ20, RQ16
  MOV.Q    RQ16, @(RQ9, 40)
  MOV.Q    @RQ11, RQ14
  MOV.Q    RQ14, @RQ9
  MOV.Q    @RQ9, RQ14
  MOV      #0, RQ12
  MOV      RQ14, RQ10
  MOV      RQ12, RQ21
  MOV.Q    RQ21, @(R15, 8)
  MOV.Q    RQ20, @(R15, 32)
  MOV.Q    RQ19, @(R15, 16)
  MOV.Q    RQ18, @(R15, 24)
  MOV.Q    RQ17, @(R15, 40)
  MOV.Q    RQ16, @(R15, 48)
  CMPQ/EQ  RQ21, RQ10
  BT       L008004E0
  MOV.Q    @RQ9, RQ14
  MOV.Q    RQ9, @(RQ14, 8)

L008004E0:

L008004D9:

L008004D8:
  MOV.L    @(R15, 88), RD16
  CMP/EQ   #0, RD16
  BT       L008004E1
  MOV      #0, RQ14
  MOV.Q    RQ14, @(RQ11, 16)
  MOV      #0, RQ14
  MOV.Q    RQ14, @(RQ11, 24)

L008004E1:
  MOV.L    @(R15, 92), RD16
  CMP/EQ   #0, RD16
  BT       L008004E2
  MOV      #0, RQ14
  MOV.Q    RQ14, @(RQ13, 16)
  MOV      #0, RQ14
  MOV.Q    RQ14, @(RQ13, 24)

L008004E2:
  MOV.L    @(R15, 92), RD16
  CMP/EQ   #0, RD16
  BT       L008004E3
  MOV.L    @(R15, 88), RD16
  CMP/EQ   #0, RD16
  BT       L008004E3
  BRA      L008004E4

L008004E4:
  MOV.Q    @(R15, 104), RQ16
  MOV.Q    @(RQ16, 8), RQ14
  MOV.Q    RQ14, @(RQ13, 16)
  MOV.Q    RQ13, @(RQ16, 8)
  MOV      #0, RQ14
  MOV.Q    RQ14, @(RQ13, 24)
  MOV.Q    @(RQ13, 16), RQ14
  MOV      #0, RQ12
  MOV      RQ14, RQ10
  MOV      RQ12, RQ17
  MOV.Q    RQ17, @(R15, 8)
  CMPQ/EQ  RQ17, RQ10
  BT       L008004E5
  MOV.Q    @(RQ13, 16), RQ14
  MOV.Q    RQ13, @(RQ14, 24)

L008004E5:

L008004E3:

L00E00454:
  ADD      #136, R15
  BRA      L00C00027

__mmFId:
  PUSH     LR
  BSR      L00C0008B
  ADD      #-120, R15
  MOV      RQ4, RQ8
  MOV.L    RD5, @(R15, 36)
  MOV.Q    @RQ8, RQ14
  MOV      #0, RQ9

L008004E6:
  MOV      #0, RQ13
  MOV      RQ14, RQ11
  MOV      RQ13, RQ10
  CMPQ/EQ  RQ10, RQ11
  BT       L008004E7
  MOV.L    @(RQ14, 52), RD12
  MOV.L    @(R15, 36), RD31
  CMP/EQ   RD31, RD12
  BT       L008004E8
  MOV.L    @(RQ14, 48), RD12
  CMP/EQ   RD12
  BT       L008004E8
  BRA      L008004E9

L008004E9:
  ADD      RQ14, #64, RQ13
  MOV      RQ8, RQ4
  MOV      RQ13, RQ5
  BSR      __mmFree
  MOV      #0, RQ13
  MOV      RQ9, RQ11
  MOV      RQ13, RQ10
  CMPQ/EQ  RQ10, RQ11
  BT       L008004EA
  MOV      RQ9, RQ14

L008004EA:

L008004E8:
  MOV      RQ14, RQ9
  MOV.Q    @RQ14, RQ14
  BRA      L008004E6

L008004E7:

L00E00455:
  ADD      #120, R15
  BRA      L00C0008C

__mmMaxSize:
  PUSH     LR
  BSR      L00C00049
  ADD      #-120, R15
  MOV.Q    RQ4, @(R15, 56)
  MOV      #0, RQ12
  EXTS.L   RQ12, RQ12
  MOV      RQ12, RQ14
  MOV.Q    @(R15, 56), RQ11
  MOV.Q    @(RQ11, 8), RQ13

L008004EB:
  MOV      #0, RQ10
  MOV      RQ13, RQ9
  MOV      RQ10, RQ8
  MOV.Q    RQ9, @(R15, 32)
  MOV.Q    RQ8, @(R15, 24)
  CMPQ/EQ  RQ8, RQ9
  BT       L008004EC
  MOV.Q    @(RQ13, 40), RQ12
  MOV      RQ12, RQ31
  MOV      RQ14, RQ30
  MOV.Q    RQ31, @(R15, 16)
  MOV.Q    RQ30, @(R15, 8)
  CMPQ/GT  RQ30, RQ31
  BT       L008004ED
  MOV.Q    @(RQ13, 40), RQ14

L008004ED:
  MOV.Q    @RQ13, RQ13
  BRA      L008004EB

L008004EC:
  MOV      #0, RQ29
  CMPQ/EQ  RQ29, RQ14
  BT       L008004EE
  SUB      #64, RQ14

L008004EE:
  MOV      RQ14, RQ2

L00E00456:
  ADD      #120, R15
  BRA      L00C0004A

__mmRealloc:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-152, R15
  MOV.Q    RQ4, @(R15, 120)
  MOV.Q    RQ5, @(R15, 112)
  MOV.Q    RQ6, @(R15, 104)
  MOV.Q    @(R15, 104), RQ31
  MOV.Q    @(R15, 104), RQ30
  ADD      #64, RQ30
  AND      RQ30, #127, RQ12
  MOV.Q    RQ31, @(R15, 88)
  MOV.Q    RQ30, @(R15, 104)
  MOV      #0, RQ29
  CMPQ/EQ  RQ29, RQ12
  BT       L008004EF
  MOV.Q    @(R15, 104), RQ28
  SHLD.Q   RQ28, #-7, RQ12
  ADD      RQ12, #1, RQ10
  SHLD.Q   RQ10, #7, RQ28
  MOV.Q    RQ28, @(R15, 104)

L008004EF:
  MOV.Q    @(R15, 104), RQ12
  MOV.Q    @(R15, 88), RQ10
  CMPQ/GT  RQ12, RQ10
  BT       L008004F0
  MOV      #-1, RD2
  BRA      L00E00457

L008004F0:
  MOV      #0, RQ14
  MOV.Q    @(R15, 112), RQ27
  CMPQ/EQ  RQ14, RQ27
  BT       L008004F1
  MOV      #-1, RD2
  BRA      L00E00457

L008004F1:
  MOV.Q    @(R15, 112), RQ26
  ADD      RQ26, #-64, RQ14
  MOV      RQ14, RQ25
  MOV.L    @(RQ25, 48), RD7
  MOV.Q    RQ25, @(R15, 96)
  MOV.L    RD7, @(R15, 52)
  CMP/EQ   #23130, RD7
  BT       L008004F2
  MOV      #0, RQ14
  MOV      #0, RD25
  MOV.B    RD25, @RQ14
  MOV      #1, RD4
  BSR      exit

L008004F2:
  MOV.Q    @(R15, 96), RQ6
  MOV.Q    @(RQ6, 40), RQ12
  MOV      RQ12, RQ10
  MOV.Q    @(R15, 104), RQ5
  MOV.Q    RQ5, @(R15, 40)
  CMPQ/GT  RQ10, RQ5
  BT       L008004F3
  MOV.Q    @(R15, 96), RQ4
  MOV.Q    @RQ4, RQ13
  MOV      #0, RQ14
  MOV      RQ13, RQ11
  MOV      RQ14, RQ8
  CMPQ/EQ  RQ8, RQ11
  BT       L008004F4
  MOV.L    @(RQ13, 48), RD3
  MOV.L    RD3, @(R15, 52)
  CMP/EQ   RD3
  BT       L008004F4
  BRA      L008004F5

L008004F5:
  MOV.L    @(RQ13, 32), RD2
  MOV.L    RD2, @(R15, 52)
  CMP/EQ   RD2
  BT       L008004F4
  BRA      L008004F6

L008004F6:
  MOV.Q    @(RQ13, 40), RQ12
  MOV.Q    @(R15, 96), RQ23
  MOV.Q    @(RQ23, 40), RQ10
  MOV      RQ12, RQ22
  MOV      RQ10, RQ21
  ADD      RQ22, RQ21, RQ20
  MOV      RQ20, RQ12
  MOV.Q    @(R15, 104), RQ10
  MOV.Q    RQ22, @(R15, 16)
  MOV.Q    RQ21, @(R15, 8)
  MOV.Q    RQ20, @(R15, 40)
  CMPQ/GT  RQ12, RQ10
  BT       L008004F4
  BRA      L008004F7

L008004F7:
  MOV.Q    @(R15, 96), RQ19
  MOV.Q    @(RQ19, 40), RQ12
  MOV.Q    @(RQ13, 40), RQ10
  MOV      RQ12, RQ18
  MOV      RQ10, RQ17
  ADD      RQ18, RQ17, RQ16
  MOV.Q    @(R15, 104), RQ20
  ADD      RQ20, #128, RQ12
  MOV      RQ16, RQ10
  MOV      RQ12, RQ18
  MOV.Q    RQ18, @(R15, 16)
  MOV.Q    RQ17, @(R15, 8)
  MOV.Q    RQ16, @(R15, 40)
  CMPQ/GT  RQ10, RQ18
  BT       L008004F8
  MOV.Q    @(RQ13, 16), RQ14
  MOV      #0, RQ11
  MOV      RQ14, RQ8
  MOV      RQ11, RQ16
  MOV.Q    RQ16, @R15
  CMPQ/EQ  RQ16, RQ8
  BT       L008004F9
  MOV.Q    @(RQ13, 24), RQ14
  MOV.Q    @(RQ13, 16), RQ11
  MOV.Q    RQ14, @(RQ11, 24)

L008004F9:
  MOV.Q    @(RQ13, 24), RQ14
  MOV      #0, RQ11
  MOV      RQ14, RQ8
  MOV      RQ11, RQ16
  MOV.Q    RQ16, @R15
  CMPQ/EQ  RQ16, RQ8
  BT       L008004FA
  MOV.Q    @(RQ13, 16), RQ14
  MOV.Q    @(RQ13, 24), RQ11
  MOV.Q    RQ14, @(RQ11, 16)
  BRA      L008004FB

L008004FA:
  MOV.Q    @(R15, 120), RQ16
  MOV.Q    @(RQ16, 8), RQ14
  CMPQ/EQ  RQ13, RQ14
  BT       L008004FC
  MOV      #0, RQ14
  MOV      #0, RD25
  MOV.B    RD25, @RQ14
  MOV      #1, RD4
  BSR      exit
  BRA      L008004FD

L008004FC:
  MOV.Q    @(RQ13, 16), RQ14
  MOV.Q    @(R15, 120), RQ16
  MOV.Q    RQ14, @(RQ16, 8)

L008004FD:

L008004FB:
  MOV.Q    @(R15, 96), RQ16
  MOV.Q    @(RQ16, 40), RQ12
  MOV.Q    @(RQ13, 40), RQ10
  MOV      RQ12, RQ17
  MOV      RQ10, RQ18
  ADD      RQ17, RQ18, RQ19
  MOV      RQ19, RQ12
  MOV.Q    RQ12, @(RQ16, 40)
  MOV.Q    @RQ13, RQ14
  MOV.Q    RQ14, @RQ16
  MOV.Q    @RQ16, RQ14
  MOV      #0, RQ11
  MOV      RQ14, RQ8
  MOV      RQ11, RQ20
  MOV.Q    RQ20, @R15
  MOV.Q    RQ19, @(R15, 40)
  MOV.Q    RQ18, @(R15, 8)
  MOV.Q    RQ17, @(R15, 16)
  CMPQ/EQ  RQ20, RQ8
  BT       L008004FE
  MOV.Q    @(R15, 96), RQ16
  MOV.Q    @RQ16, RQ14
  MOV.Q    RQ16, @(RQ14, 8)

L008004FE:
  BRA      L008004FF

L008004F8:
  MOV.Q    @(R15, 96), RQ16
  MOV.Q    @(R15, 104), RQ17
  LEA.B    @(RQ16, RQ17), RQ14
  MOV      RQ14, RQ9
  MOV      #0, RD18
  MOV.L    RD18, @(RQ9, 48)
  MOV.L    RD18, @(RQ9, 32)
  MOV.Q    @(RQ16, 40), RQ12
  MOV.Q    @(RQ13, 40), RQ10
  MOV      RQ12, RQ19
  MOV      RQ10, RQ20
  ADD      RQ19, RQ20, RQ21
  MOV      RQ17, RQ10
  SUB      RQ21, RQ10, RQ12
  MOV      RQ12, RQ10
  MOV.Q    RQ10, @(RQ9, 40)
  MOV.Q    RQ16, @(RQ9, 8)
  MOV.Q    RQ9, @RQ16
  MOV.Q    @RQ13, RQ14
  MOV.Q    RQ14, @RQ9
  MOV.Q    @RQ9, RQ14
  MOV      #0, RQ11
  MOV      RQ14, RQ8
  MOV      RQ11, RQ22
  MOV.Q    RQ22, @R15
  MOV.Q    RQ21, @(R15, 40)
  MOV.Q    RQ20, @(R15, 8)
  MOV.Q    RQ19, @(R15, 16)
  CMPQ/EQ  RQ22, RQ8
  BT       L00800500
  MOV.Q    @RQ9, RQ14
  MOV.Q    RQ9, @(RQ14, 8)

L00800500:
  MOV.Q    @(RQ13, 16), RQ14
  MOV.Q    RQ14, @(RQ9, 16)
  MOV.Q    @(RQ9, 16), RQ14
  MOV      #0, RQ11
  MOV      RQ14, RQ8
  MOV      RQ11, RQ16
  MOV.Q    RQ16, @R15
  CMPQ/EQ  RQ16, RQ8
  BT       L00800501
  MOV.Q    @(RQ9, 16), RQ14
  MOV.Q    RQ9, @(RQ14, 24)

L00800501:
  MOV.Q    @(RQ13, 24), RQ14
  MOV.Q    RQ14, @(RQ9, 24)
  MOV.Q    @(RQ9, 24), RQ14
  MOV      #0, RQ11
  MOV      RQ14, RQ8
  MOV      RQ11, RQ16
  MOV.Q    RQ16, @R15
  CMPQ/EQ  RQ16, RQ8
  BT       L00800502
  MOV.Q    @(RQ9, 24), RQ14
  MOV.Q    RQ9, @(RQ14, 16)
  BRA      L00800503

L00800502:
  MOV.Q    @(R15, 120), RQ16
  MOV.Q    @(RQ16, 8), RQ14
  CMPQ/EQ  RQ13, RQ14
  BT       L00800504
  MOV      #0, RQ14
  MOV      #0, RD25
  MOV.B    RD25, @RQ14
  MOV      #1, RD4
  BSR      exit
  BRA      L00800505

L00800504:
  MOV.Q    @(R15, 120), RQ16
  MOV.Q    RQ9, @(RQ16, 8)

L00800505:

L00800503:
  MOV.Q    @(R15, 104), RQ16
  MOV.Q    @(R15, 96), RQ17
  MOV.Q    RQ16, @(RQ17, 40)

L008004FF:
  BRA      L00800506

L008004F4:
  MOV      #-1, RD2
  BRA      L00E00457

L00800506:
  BRA      L00800507

L008004F3:
  MOV.Q    @(R15, 104), RQ16
  ADD      RQ16, #128, RQ12
  MOV.Q    @(R15, 96), RQ17
  MOV.Q    @(RQ17, 40), RQ10
  MOV      RQ12, RQ18
  MOV      RQ10, RQ19
  MOV.Q    RQ19, @(R15, 16)
  MOV.Q    RQ18, @(R15, 40)
  CMPQ/GT  RQ19, RQ18
  BT       L00800508
  MOV.Q    @(R15, 96), RQ16
  MOV.Q    @(R15, 104), RQ17
  LEA.B    @(RQ16, RQ17), RQ14
  MOV      RQ14, RQ13
  MOV.Q    @RQ16, RQ14
  MOV.Q    RQ14, @RQ13
  MOV.Q    @RQ13, RQ14
  MOV      #0, RQ11
  MOV      RQ14, RQ8
  MOV      RQ11, RQ18
  MOV.Q    RQ18, @R15
  CMPQ/EQ  RQ18, RQ8
  BT       L00800509
  MOV.Q    @RQ13, RQ14
  MOV.Q    RQ13, @(RQ14, 8)

L00800509:
  MOV.Q    @(R15, 96), RQ16
  MOV.Q    RQ16, @(RQ13, 8)
  MOV.Q    RQ13, @RQ16
  MOV      #0, RD17
  MOV.L    RD17, @(RQ13, 32)
  MOV.Q    @(RQ16, 40), RQ12
  MOV      RQ12, RQ18
  MOV.Q    @(R15, 104), RQ19
  SUB      RQ18, RQ19, RQ10
  MOV      RQ10, RQ12
  MOV.Q    RQ12, @(RQ13, 40)
  MOV.L    RD17, @(RQ13, 48)
  MOV.Q    @(R15, 104), RQ20
  MOV.Q    RQ20, @(RQ16, 40)
  MOV.Q    @RQ13, RQ9
  MOV      #0, RQ14
  MOV      RQ9, RQ11
  MOV      RQ14, RQ8
  MOV.Q    RQ19, @(R15, 16)
  MOV.Q    RQ18, @(R15, 40)
  CMPQ/EQ  RQ8, RQ11
  BT       L0080050A
  MOV.L    @(RQ9, 48), RD16
  MOV.L    RD16, @(R15, 52)
  CMP/EQ   #0, RD16
  BT       L0080050A
  BRA      L0080050B

L0080050B:
  MOV.L    @(RQ9, 32), RD16
  MOV.L    RD16, @(R15, 52)
  CMP/EQ   #0, RD16
  BT       L0080050A
  BRA      L0080050C

L0080050C:
  MOV.Q    @(RQ13, 40), RQ12
  MOV.Q    @(RQ9, 40), RQ10
  MOV      RQ12, RQ16
  MOV      RQ10, RQ17
  ADD      RQ16, RQ17, RQ18
  MOV      RQ18, RQ12
  MOV.Q    RQ12, @(RQ13, 40)
  MOV.Q    @RQ9, RQ14
  MOV.Q    RQ14, @RQ13
  MOV.Q    @RQ13, RQ14
  MOV      #0, RQ11
  MOV      RQ14, RQ8
  MOV      RQ11, RQ19
  MOV.Q    RQ19, @R15
  MOV.Q    RQ18, @(R15, 40)
  MOV.Q    RQ17, @(R15, 8)
  MOV.Q    RQ16, @(R15, 16)
  CMPQ/EQ  RQ19, RQ8
  BT       L0080050D
  MOV.Q    @RQ13, RQ14
  MOV.Q    RQ13, @(RQ14, 8)

L0080050D:
  MOV.Q    @(RQ9, 16), RQ14
  MOV.Q    RQ14, @(RQ13, 16)
  MOV.Q    @(RQ13, 16), RQ14
  MOV      #0, RQ11
  MOV      RQ14, RQ8
  MOV      RQ11, RQ16
  MOV.Q    RQ16, @R15
  CMPQ/EQ  RQ16, RQ8
  BT       L0080050E
  MOV.Q    @(RQ13, 16), RQ14
  MOV.Q    RQ13, @(RQ14, 24)

L0080050E:
  MOV.Q    @(RQ9, 24), RQ14
  MOV.Q    RQ14, @(RQ13, 24)
  MOV.Q    @(RQ13, 24), RQ14
  MOV      #0, RQ11
  MOV      RQ14, RQ8
  MOV      RQ11, RQ16
  MOV.Q    RQ16, @R15
  CMPQ/EQ  RQ16, RQ8
  BT       L0080050F
  MOV.Q    @(RQ13, 24), RQ14
  MOV.Q    RQ13, @(RQ14, 16)
  BRA      L00800510

L0080050F:
  MOV.Q    @(R15, 120), RQ16
  MOV.Q    @(RQ16, 8), RQ14
  CMPQ/EQ  RQ9, RQ14
  BT       L00800511
  MOV      #0, RQ14
  MOV      #0, RD25
  MOV.B    RD25, @RQ14
  MOV      #1, RD4
  BSR      exit
  BRA      L00800512

L00800511:
  MOV.Q    @(R15, 120), RQ16
  MOV.Q    RQ13, @(RQ16, 8)

L00800512:

L00800510:
  BRA      L00800513

L0080050A:
  MOV.Q    @(R15, 120), RQ16
  MOV.Q    @(RQ16, 8), RQ14
  MOV.Q    RQ14, @(RQ13, 16)
  MOV.Q    @(RQ13, 16), RQ14
  MOV      #0, RQ11
  MOV      RQ14, RQ8
  MOV      RQ11, RQ17
  MOV.Q    RQ17, @R15
  CMPQ/EQ  RQ17, RQ8
  BT       L00800514
  MOV.Q    @(RQ13, 16), RQ14
  MOV.Q    RQ13, @(RQ14, 24)

L00800514:
  MOV      #0, RQ14
  MOV.Q    RQ14, @(RQ13, 24)
  MOV.Q    @(R15, 120), RQ16
  MOV.Q    RQ13, @(RQ16, 8)

L00800513:

L00800508:

L00800507:
  MOV.Q    @(R15, 96), RQ16
  ADD      RQ16, #64, RQ14
  MOV      RQ14, RQ17
  ADD      RQ17, #-8, RQ14
  MOV.Q    @(R15, 88), RQ18
  MOV.Q    RQ18, @RQ14
  MOV.Q    RQ17, @(R15, 80)
  MOV      #0, RD2

L00E00457:
  ADD      #152, R15
  BRA      L00C00027

TKMM_Malloc:
  PUSH     LR
  BSR      L00C0000E
  ADD      #-64, R15
  MOV      RD4, RD11
  MOV      #0x00010000, RD8
  MOV      #1, RD8
  CMP/GE   RD8, RD11
  BT       L00800530
  MOV      RD11, RD4
  BSR      TKMM_MMList_Malloc
  MOV      RQ2, RQ13
  MOV      #0, RQ31
  CMPQ/EQ  RQ31, RQ13
  BT       L00800531
  MOV      L00C000F9, RQ4
  MOV      RD11, RD5
  BSR      tk_printf

L00800531:
  MOV      RQ13, RQ2
  BRA      L00E00458

L00800530:
  ADD      RD11, #32, RD30
  ADD      RD30, #4095, RD29
  SHAD     RD29, #-12, RD9
  MOV      RD9, RD4
  BSR      TKMM_AllocPages
  MOV      RD2, RD10
  CMP/PZ   RD10
  BT       L00800532
  MOV      L00C000FB, RQ4
  MOV      RD11, RD5
  BSR      tk_printf
  MOV      #0, RQ14
  MOV      RQ14, RQ2
  BRA      L00E00458

L00800532:
  MOV      RD10, RD4
  BSR      TKMM_PageToPointer
  MOV      RQ2, RQ13
  MOV      #0, RQ28
  CMPQ/EQ  RQ28, RQ13
  BT       L00800533
  MOV      L00C000FD, RQ4
  MOV      RD11, RD5
  BSR      tk_printf
  MOV      #0, RQ14
  MOV      RQ14, RQ2
  BRA      L00E00458

L00800533:
  MOV      RQ13, RQ12
  MOV      #0, RQ14
  MOV.Q    RQ14, @RQ12
  MOV      #0, RQ14
  MOV.Q    RQ14, @(RQ12, 8)
  EXTU.W   RD9, RD9
  MOV.W    RD9, @(RQ12, 16)
  MOV      #5, RD27
  EXTU.B   RD27, RD27
  MOV.B    RD27, @(RQ12, 18)
  MOV      #4095, RQ26
  TST.Q    RQ26, RQ13
  BT       L00800534
  BRK     

L00800534:
  ADD      RQ12, #24, RQ14
  MOV      L00C000FE, RQ4
  MOV      RQ12, RQ5
  MOV      RQ14, RQ6
  MOV      RD11, RD7
  BSR      tk_printf
  ADD      RQ12, #24, RQ14
  MOV      RQ14, RQ2

L00E00458:
  ADD      #64, R15
  BRA      L00C00018

TKMM_MMList_Malloc:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-88, R15
  MOV      RD4, RD11
  CMP/PL   RD11
  BT       L00800540
  MOV      #0, RQ14
  MOV      RQ14, RQ2
  BRA      L00E00459

L00800540:
  CMP/GE   #64, RD11
  BT       L00800541
  MOV      #64, RD11

L00800541:
  MOV      #0, RQ14
  ADD      RQ14, #24, RQ31
  MOV      RQ31, RQ7
  EXTS.L   RD7
  MOV      RD7, RD12
  ADD      RD11, RD12, RD9
  ADD      RD9, #15, RD12
  AND      RD12, #-8, RD11
  MOV.Q    RQ31, @(R15, 8)
  MOV      RD11, RD4
  BSR      TKMM_SizeToFxiU
  MOV      RD2, RD10
  TST      #-256, RD10
  BT       L00800542
  BRK     

L00800542:
  MOV      tkmm_mmlist_freelist, RQ30
  EXTS.L   RD10
  MOV.Q    @(RQ30, RD10), RQ13
  MOV      #0, RQ29
  CMPQ/EQ  RQ29, RQ13
  BT       L00800543
  ADD      RQ13, #24, RQ14
  MOV.Q    @RQ14, RQ28
  MOV      tkmm_mmlist_freelist, RQ27
  EXTS.L   RD10
  MOV.Q    RQ28, @(RQ27, RD10)
  MOV.B    @(RQ13, 18), RD12
  EXTU.B   RD12, RD12
  AND      RD12, #-2, RD9
  EXTU.B   RD9, RD9
  MOV.B    RD9, @(RQ13, 18)
  ADD      RQ13, #24, RQ14
  MOV.Q    RQ28, @(R15, 8)
  MOV      RQ14, RQ2
  BRA      L00E00459

L00800543:
  MOV      RD10, RD4
  BSR      TKMM_FxiToSize
  MOV      RD2, RD8
  MOV      RD8, RD4
  BSR      TKMM_MMList_AllocBrk
  MOV      RQ2, RQ26
  MOV.Q    RQ26, @(R15, 40)
  MOV      #0, RQ25
  CMPQ/EQ  RQ25, RQ26
  BT       L00800544
  MOV      L00C00109, RQ4
  MOV      RD8, RD5
  BSR      tk_printf
  BRK     
  MOV      #0, RQ14
  MOV      RQ14, RQ2
  BRA      L00E00459

L00800544:
  MOV.Q    @(R15, 40), RQ25
  EXTS.L   RD8
  LEA.B    @(RQ25, RD8), RQ26
  MOV      RQ25, RQ13
  MOV.Q    @(R15, 48), RQ27
  ADD      RQ27, #24, RQ14
  SUB      RQ26, RQ14, RD28
  MOV.L    RD28, @(R15, 28)
  MOV.Q    RQ26, @(R15, 32)
  MOV      RD28, RD4
  BSR      TKMM_SizeToFxiD
  MOV      RD2, RD12
  EXTU.W   RD12, RD12
  MOV.W    RD12, @(RQ13, 16)
  MOV      #3, RD29
  EXTU.B   RD29, RD29
  MOV.B    RD29, @(RQ13, 18)
  MOV      #90, RD30
  EXTU.B   RD30, RD30
  MOV.B    RD30, @(RQ13, 19)
  MOV.Q    RQ26, @RQ13
  MOV      #0, RQ14
  MOV.Q    RQ14, @(RQ13, 8)
  MOV.B    @(RQ13, 18), RD12
  EXTU.B   RD12, RD12
  AND      RD12, #-2, RD9
  EXTU.B   RD9, RD9
  MOV.B    RD9, @(RQ13, 18)
  ADD      RQ13, #24, RQ14
  MOV      RQ14, RQ2

L00E00459:
  ADD      #88, R15
  BRA      L00C00027

TKMM_FindFreePages:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-56, R15
  MOV.L    RD4, @(R15, 28)
  MOV.L    @(R15, 28), RD8
  CMP/PL   RD8
  BT       L00800515
  MOV      #-1, RD2
  BRA      L00E0045A

L00800515:
  MOV.L    tkmm_pagerov, RD31
  MOV      RD31, RD14
  MOV.L    tkmm_maxpage, RD30
  MOV      RD30, RD10

L00800516:
  CMP/GT   RD14, RD10
  BT       L00800517
  SHAD     RD14, #-3, RD13
  MOV      tkmm_pagebmp, RQ29
  MOVU.B   @(RQ29, RD13), RD12
  AND      RD14, #7, RD13
  MOV      #1, RD28
  SHAD     RD28, RD13, RD11
  TST      RD11, RD12
  BT       L00800518

L00800519:
  SHAD     RD14, #-3, RD13
  MOV      tkmm_pagebmp, RQ27
  MOVU.B   @(RQ27, RD13), RD12
  CMP/EQ   #255, RD12
  BT       L0080051A
  CMP/GT   RD14, RD10
  BT       L0080051A
  BRA      L0080051B

L0080051B:
  ADD      RD14, #8, RD13
  AND      RD13, #-8, RD14
  BRA      L00800519
  BRA      L00800519

L0080051A:

L0080051C:
  SHAD     RD14, #-3, RD13
  MOV      tkmm_pagebmp, RQ26
  MOVU.B   @(RQ26, RD13), RD12
  AND      RD14, #7, RD13
  MOV      #1, RD25
  SHAD     RD25, RD13, RD11
  TST      RD11, RD12
  BT       L0080051D
  CMP/GT   RD14, RD10
  BT       L0080051D
  BRA      L0080051E

L0080051E:
  ADD      #1, RD14
  BRA      L0080051C

L0080051D:
  BRA      L00800516

L00800518:
  MOV      RD14, RD7
  MOV.L    @(R15, 28), RD6
  ADD      RD7, RD6, RD9
  MOV.L    RD7, @(R15, 24)
  CMP/GT   RD10, RD9
  BT       L0080051F
  BRA      L00800517

L0080051F:

L00800520:
  SHAD     RD14, #-3, RD13
  MOV      tkmm_pagebmp, RQ5
  MOVU.B   @(RQ5, RD13), RD12
  AND      RD14, #7, RD13
  MOV      #1, RD4
  SHAD     RD4, RD13, RD11
  TST      RD11, RD12
  BT       L00800521
  CMP/GT   RD14, RD9
  BT       L00800521
  BRA      L00800522

L00800522:
  ADD      #1, RD14
  BRA      L00800520

L00800521:
  CMP/GT   RD14, RD9
  BT       L00800523
  MOV      RD9, RD3
  MOV.L    RD3, tkmm_pagerov
  MOV.L    @(R15, 24), RD2
  BRA      L00E0045A

L00800523:
  BRA      L00800516

L00800517:
  MOV.L    tkmm_pagerov, RD2
  CMP/EQ   RD2
  BT       L00800524
  MOV      #0, RD25
  MOV.L    RD25, tkmm_pagerov
  MOV.L    @(R15, 28), RD4
  BSR      TKMM_FindFreePages
  MOV      RD2, RD14
  MOV      RD14, RD2
  BRA      L00E0045A

L00800524:
  MOV      #-1, RD2

L00E0045A:
  ADD      #56, R15
  BRA      L00C00027

TKMM_AllocPages:
  PUSH     LR
  BSR      L00C0004E
  ADD      #-96, R15
  MOV.L    RD4, @(R15, 28)
  MOV.L    @(R15, 28), RD4
  BSR      TKMM_FindFreePages
  MOV      RD2, RD12
  CMP/PZ   RD12
  BT       L00800525
  MOV      #-1, RD2
  BRA      L00E0045B

L00800525:
  MOV      RD12, RD14
  MOV.L    @(R15, 28), RD9
  ADD      RD14, RD9, RD10
  MOV.L    RD10, @(R15, 24)

L00800526:
  MOV.L    @(R15, 24), RD8
  CMP/GT   RD14, RD8
  BT       L00800527
  SHAD     RD14, #-3, RD13
  MOV      tkmm_pagebmp, RQ31
  MOVU.B   @(RQ31, RD13), RD11
  AND      RD14, #7, RD13
  MOV      #1, RD7
  SHAD     RD7, RD13, RD30
  OR       RD11, RD30, RD13
  SHAD     RD14, #-3, RD11
  MOVU.B   RD13, @(RQ31, RD11)
  ADD      #1, RD14
  BRA      L00800526

L00800527:
  MOV      RD12, RD2

L00E0045B:
  ADD      #96, R15
  BRA      L00C0004F

TKMM_PageToPointer:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  ADD      #-144, R15
  MOV.L    RD4, @(R15, 28)
  MOV      #0x10000000, RQ14
  MOV      #4096, RQ14
  MOV.L    @(R15, 28), RD11
  SHAD     RD11, #12, RD13
  LEA.B    @(RQ14, RD13), RQ12
  MOV      RQ12, RQ2

L00E0045C:
  ADD      #144, R15
  BRA      L00C00035

TKMM_PageAlloc:
  PUSH     LR
  BSR      L00C00072
  ADD      #-128, R15
  MOV.L    RD4, @(R15, 28)
  MOV.L    @(R15, 28), RD10
  ADD      RD10, #4095, RD12
  SHAD     RD12, #-12, RD11
  MOV      RD11, RD4
  BSR      TKMM_AllocPages
  MOV      RD2, RD14
  CMP/PZ   RD14
  BT       L0080052B
  MOV      #0, RQ10
  MOV.Q    RQ10, @(R15, 8)
  MOV      RQ10, RQ2
  BRA      L00E0045D

L0080052B:
  MOV      RD14, RD4
  BSR      TKMM_PageToPointer
  MOV      RQ2, RQ13
  MOV      RQ13, RQ2

L00E0045D:
  ADD      #128, R15
  BRA      L00C00074

TKMM_Init:
  PUSH     LR
  BSR      L00C00053
  ADD      #-56, R15
  MOV.L    init, RD13
  CMP/EQ   RD13
  BT       L0080052C
  BRA      L00E0045E

L0080052C:
  MOV      #1, RD12
  MOV      #0x10000000, RD11
  MOV      #4096, RD11
  MOV      #0x18000000, RD10
  MOV      #6144, RD10
  MOV.L    RD12, init
  MOV.L    RD11, tkmm_pagebase
  MOV.L    RD10, tkmm_pageend
  MOV.Q    tk_ird_imgbuf, RQ9
  MOV      #0, RQ8
  CMPQ/EQ  RQ8, RQ9
  BT       L0080052D
  MOV.Q    tk_ird_imgbuf, RQ7
  EXTU.L   RD7
  MOV      RD7, RD14
  MOV.L    tkmm_pageend, RD31
  CMP/HI   RD14, RD31
  BT       L0080052E
  MOV.Q    tk_ird_imgbuf, RQ7
  EXTU.L   RD7
  MOV      RD7, RD14
  MOV.L    tkmm_pagebase, RD30
  CMP/HI   RD30, RD14
  BT       L0080052E
  BRA      L0080052F

L0080052F:
  MOV.Q    tk_ird_imgbuf, RQ7
  EXTU.L   RD7
  MOV      RD7, RD29
  MOV.L    RD29, tkmm_pageend

L0080052E:

L0080052D:
  MOV.L    tkmm_pageend, RD28
  MOV.L    tkmm_pagebase, RD27
  SUB      RD28, RD27, RD14
  SHLD     RD14, #-12, RD29
  MOV.L    RD29, tkmm_maxpage
  BSR      TKMM_MMList_Init

L00E0045E:
  ADD      #56, R15
  BRA      L00C00054

TKMM_MMList_Init:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  ADD      #-136, R15
  MOV      #0, RD14

L00800548:
  CMP/GE   #256, RD14
  BT       L00800549
  MOV      #0, RQ13
  MOV      tkmm_mmlist_freelist, RQ12
  EXTS.L   RD14
  MOV.Q    RQ13, @(RQ12, RD14)

L0080054A:
  ADD      #1, RD14
  BRA      L00800548

L00800549:

L00E0045F:
  ADD      #136, R15
  BRA      L00C0005F

TKMM_MMList_AllocBrk:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-40, R15
  MOV      RD4, RD13
  MOV      #0x00010000, RD12
  MOV      #1, RD12
  CMP/GE   RD12, RD13
  BT       L00800537
  MOV      L00C00112, RQ4
  BSR      tk_puts
  MOV      RD13, RD4
  BSR      TKMM_PageAlloc
  MOV      RQ2, RQ14
  MOV      RQ14, RQ2
  BRA      L00E00460

L00800537:
  ADD      RD13, #15, RD11
  AND      RD11, #-16, RD13
  MOV.Q    tkmm_mmlist_brkbuf, RQ10
  MOV      #0, RQ9
  CMPQ/EQ  RQ9, RQ10
  BT       L00800538
  MOV.Q    tkmm_mmlist_brkpos, RQ8
  EXTS.L   RD13
  LEA.B    @(RQ8, RD13), RQ31
  MOV.Q    RQ31, @R15
  MOV.Q    tkmm_mmlist_brkend, RQ30
  CMPQ/GT  RQ30, RQ31
  BT       L00800538
  BRA      L00800539

L00800539:
  MOV      L00C00113, RQ4
  BSR      tk_puts
  MOV      #0, RQ29
  MOV.Q    RQ29, tkmm_mmlist_brkbuf

L00800538:
  MOV.Q    tkmm_mmlist_brkbuf, RQ28
  MOV      #0, RQ27
  CMPQ/EQ  RQ27, RQ28
  BT       L0080053A
  MOV      L00C00114, RQ4
  BSR      tk_puts
  MOV      #0x00100000, RD4
  MOV      #16, RD4
  BSR      TKMM_PageAlloc
  MOV      RQ2, RQ26
  ADD      RQ26, #1048576, RQ25
  MOV      RQ26, RQ27
  MOV.Q    RQ27, tkmm_mmlist_brkpos
  MOV.Q    RQ26, tkmm_mmlist_brkbuf
  MOV.Q    RQ25, tkmm_mmlist_brkend

L0080053A:
  MOV.Q    tkmm_mmlist_brkpos, RQ7
  MOV      RQ7, RQ14
  EXTS.L   RD13
  LEA.B    @(RQ14, RD13), RQ7
  MOV.Q    RQ7, tkmm_mmlist_brkpos
  MOV      RQ14, RQ2

L00E00460:
  ADD      #40, R15
  BRA      L00C00027

TKMM_SizeToFxiU:
  PUSH     LR
  BSR      L00C00037
  ADD      #-104, R15
  MOV      RD4, RD11
  MOV      RD11, RD14
  MOV      #0, RD13

L0080053B:
  CMP/GE   #16, RD14
  BT       L0080053C
  ADD      RD14, #1, RD12
  SHAD     RD12, #-1, RD14
  ADD      #1, RD13
  BRA      L0080053B

L0080053C:
  AND      RD14, #7, RD12
  SHAD     RD13, #3, RD10
  OR       RD12, RD10, RD9
  MOV.L    RD9, @(R15, 8)
  MOV      RD9, RD2

L00E00461:
  ADD      #104, R15
  BRA      L00C00038

TKMM_SizeToFxiD:
  PUSH     LR
  BSR      L00C00037
  ADD      #-104, R15
  MOV      RD4, RD9
  MOV      RD9, RD14
  MOV      #0, RD13

L0080053D:
  CMP/GE   #16, RD14
  BT       L0080053E
  SHAD     #-1, RD14
  ADD      #1, RD13
  BRA      L0080053D

L0080053E:
  AND      RD14, #7, RD12
  SHAD     RD13, #3, RD11
  OR       RD12, RD11, RD10
  MOV      RD10, RD2

L00E00462:
  ADD      #104, R15
  BRA      L00C00038

TKMM_FxiToSize:
  PUSH     LR
  BSR      L00C00037
  ADD      #-120, R15
  MOV      RD4, RD14
  MOV      #8, RD12
  CMP/GE   RD12, RD14
  BT       L0080053F
  MOV      RD14, RD2
  BRA      L00E00463

L0080053F:
  AND      RD14, #7, RD13
  OR       RD13, #8, RD11
  SHAD     RD14, #-3, RD10
  SHAD     RD11, RD10, RD9
  MOV.L    RD11, @(R15, 20)
  MOV.L    RD10, @(R15, 24)
  MOV.L    RD9, @(R15, 16)
  MOV      RD9, RD2

L00E00463:
  ADD      #120, R15
  BRA      L00C00038

TKSPI_ChipSel:
  PUSH     LR
  BSR      L00C00044
  ADD      #-80, R15
  MOV.L    RD4, @(R15, 28)
  MOV      #0xA000E030, RQ13
  MOV      #0x0000A000, RQ13
  MOVU.L   @RQ13, RD14
  MOV      RD14, RD12

L0080054B:
  TST      #2, RD12
  BT       L0080054C
  MOV      #0xA000E030, RQ13
  MOV      #0x0000A000, RQ13
  MOVU.L   @RQ13, RD14
  MOV      RD14, RD12
  BRA      L0080054B

L0080054C:
  MOV.L    @(R15, 28), RD11
  CMP/EQ   RD11
  BT       L0080054D
  MOV.L    @(R15, 28), RD10
  CMP/EQ   #1, RD10
  BT       L0080054E
  MOV.L    tkspi_ctl_speed, RD8
  MOV      RD8, RD9
  MOV.L    RD9, tkspi_ctl_status
  BRA      L0080054F

L0080054E:
  MOV.L    @(R15, 28), RD31
  CMP/EQ   #2, RD31
  BT       L00800550
  MOV.L    tkspi_ctl_speed, RD30
  OR       RD30, #4, RD14
  OR       RD14, #1, RD29
  MOV.L    RD29, tkspi_ctl_status

L00800550:

L0080054F:
  BRA      L00800551

L0080054D:
  MOV      #1, RD28
  MOV.L    RD28, tkspi_ctl_status

L00800551:
  MOV      #0xA000E030, RQ13
  MOV      #0x0000A000, RQ13
  MOV.L    tkspi_ctl_status, RD7
  MOVU.L   RD7, @RQ13

L00E00464:
  ADD      #80, R15
  BRA      L00C00045

TKSPI_SetSpeed:
  PUSH     R14
  ADD      #-152, R15
  MOV.L    RD4, @(R15, 12)
  MOV.L    @(R15, 12), RD14
  CMP/EQ   RD14
  BT       L00800552
  MOV      #0xF8000000, RD14
  MOV      #-2048, RD14
  MOV.L    RD14, tkspi_ctl_speed
  BRA      L00800553

L00800552:
  MOV      #0, RD14
  MOV.L    RD14, tkspi_ctl_speed

L00800553:

L00E00465:
  ADD      #152, R15
  POP      RQ14
  RTS     

TKSPI_XchByte:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  ADD      #-136, R15
  MOV.L    RD4, @(R15, 28)
  MOV      #0xA000E034, RQ14
  MOV      #0x0000A000, RQ14
  MOV.L    @(R15, 28), RD11
  MOVU.L   RD11, @RQ14
  MOV.L    tkspi_ctl_status, RD10
  OR       RD10, #2, RD13
  MOV      #0xA000E030, RQ14
  MOV      #0x0000A000, RQ14
  MOVU.L   RD13, @RQ14
  MOV      #0xA000E030, RQ14
  MOV      #0x0000A000, RQ14
  MOVU.L   @RQ14, RD13
  MOV      RD13, RD12

L00800554:
  TST      #2, RD12
  BT       L00800555
  MOV      #0xA000E030, RQ14
  MOV      #0x0000A000, RQ14
  MOVU.L   @RQ14, RD13
  MOV      RD13, RD12
  BRA      L00800554

L00800555:
  MOV      #0xA000E034, RQ14
  MOV      #0x0000A000, RQ14
  MOVU.L   @RQ14, RD13
  MOV      RD13, RD12
  EXTU.B   RD12, RD13
  EXTU.B   RD13, RD10
  MOV.L    RD10, @(R15, 8)
  MOV      RD10, RD2

L00E00466:
  ADD      #136, R15
  BRA      L00C00047

TKSPI_DelayUSec:
  ADD      #-160, R15
  MOV.L    RD4, @(R15, 12)

L00E00467:
  ADD      #160, R15
  RTS     

TKSPI_ReadData:
  PUSH     LR
  BSR      L00C0004E
  ADD      #-112, R15
  MOV.Q    RQ4, @(R15, 40)
  MOV.L    RD5, @(R15, 36)
  MOV      #0x00010000, RD12
  MOV      #1, RD12

L00800558:
  CMP/EQ   RD12
  BT       L00800559
  MOV      #255, RD14
  MOV      RD14, RD4
  BSR      TKSPI_XchByte
  MOV      RD2, RD13
  CMP/EQ   #255, RD13
  BT       L0080055A
  BRA      L00800559

L0080055A:
  MOV      #10, RD4
  BSR      TKSPI_DelayUSec
  ADD      #-1, RD12
  BRA      L00800558

L00800559:
  CMP/EQ   #254, RD13
  BT       L0080055B
  MOV      L00C0011D, RQ4
  MOV      RD13, RD5
  BSR      printf
  MOV      #-1, RD2
  BRA      L00E00468

L0080055B:
  MOV.Q    @(R15, 40), RQ9
  MOV      RQ9, RQ10
  MOV.L    @(R15, 36), RD8
  MOV      RD8, RD11
  MOV.Q    RQ10, @(R15, 24)

L0080055C:
  MOV      RD11, RD14
  ADD      #-1, RD11
  CMP/PL   RD14
  BT       L0080055D
  MOV      #255, RD14
  MOV      RD14, RD4
  BSR      TKSPI_XchByte
  MOV      RD2, RD13
  MOV.Q    @(R15, 24), RQ30
  MOV      RQ30, RQ31
  ADD      #1, RQ30
  MOVU.B   RD13, @RQ31
  MOV.Q    RQ30, @(R15, 24)
  BRA      L0080055C

L0080055D:
  MOV      #255, RD14
  MOV      RD14, RD4
  BSR      TKSPI_XchByte
  MOV      #255, RD14
  MOV      RD14, RD4
  BSR      TKSPI_XchByte
  MOV      #0, RD2

L00E00468:
  ADD      #112, R15
  BRA      L00C0004F

TKSPI_WaitReady:
  PUSH     LR
  BSR      L00C0001A
  ADD      #-128, R15
  MOV      #0x00010000, RD14
  MOV      #1, RD14

L0080055E:
  CMP/EQ   RD14
  BT       L0080055F
  MOV      #255, RD12
  MOV      RD12, RD4
  BSR      TKSPI_XchByte
  MOV      RD2, RD13
  CMP/EQ   #255, RD13
  BT       L00800560
  MOV      #1, RD2
  BRA      L00E00469

L00800560:
  MOV      #10, RD4
  BSR      TKSPI_DelayUSec
  ADD      #-1, RD14
  BRA      L0080055E

L0080055F:
  MOV      #0, RD2

L00E00469:
  ADD      #128, R15
  BRA      L00C0001D

TKSPI_WriteData:
  PUSH     LR
  BSR      L00C0003D
  ADD      #-112, R15
  MOV.Q    RQ4, @(R15, 24)
  MOV.L    RD5, @(R15, 20)
  BSR      TKSPI_WaitReady
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L00800561
  MOV      #-1, RD2
  BRA      L00E0046A

L00800561:
  MOV      #254, RD14
  MOV      RD14, RD4
  BSR      TKSPI_XchByte
  MOV      RD2, RD11
  MOV.Q    @(R15, 24), RQ10
  MOV      RQ10, RQ12
  MOV.L    @(R15, 20), RD9
  MOV      RD9, RD13

L00800562:
  MOV      RD13, RD14
  ADD      #-1, RD13
  CMP/PL   RD14
  BT       L00800563
  MOV      RQ12, RQ8
  ADD      #1, RQ12
  MOVU.B   @RQ8, RD14
  EXTU.B   RD14, RD9
  MOV.L    RD9, @(R15, 4)
  MOV      RD9, RD4
  BSR      TKSPI_XchByte
  MOV      RD2, RD11
  BRA      L00800562

L00800563:
  MOV      #255, RD8
  MOV.L    RD8, @(R15, 4)
  MOV      RD8, RD4
  BSR      TKSPI_XchByte
  MOV      #255, RD8
  MOV.L    RD8, @(R15, 4)
  MOV      RD8, RD4
  BSR      TKSPI_XchByte
  MOV      #0, RD2

L00E0046A:
  ADD      #112, R15
  BRA      L00C0003E

TKSPI_Deselect:
  PUSH     LR
  ADD      #-136, R15
  MOV      #1, RD4
  BSR      TKSPI_ChipSel

L00E0046B:
  ADD      #136, R15
  RET     

TKSPI_Select:
  PUSH     LR
  PUSH     R14
  ADD      #-144, R15
  MOV      #0, RD4
  BSR      TKSPI_ChipSel
  BSR      TKSPI_WaitReady
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L00800564
  MOV      #1, RD2
  BRA      L00E0046C

L00800564:
  BSR      TKSPI_Deselect
  MOV      #0, RD2

L00E0046C:
  ADD      #144, R15
  POP      RQ14
  RET     

TKSPI_PowerOff:
  PUSH     LR
  ADD      #-136, R15
  BSR      TKSPI_Select
  BSR      TKSPI_Deselect

L00E0046D:
  ADD      #136, R15
  RET     

TKSPI_SendCmd:
  PUSH     LR
  BSR      L00C0000E
  ADD      #-64, R15
  MOV      RD4, RD11
  MOV.L    RD5, @(R15, 24)
  CMP/EQ   #55, RD11
  BT       L00800565
  MOV.L    @(R15, 24), RD9
  CMP/EQ   RD9
  BT       L00800565
  BRA      L00800566

L00800566:
  BRK     

L00800565:
  TST      #128, RD11
  BT       L00800567
  MOV      RD11, RD13
  MOV      #127, RD14
  AND      RD13, RD14, RD11
  MOV      #55, RD13
  MOV      RD13, RD4
  MOV      #0, RD5
  BSR      TKSPI_SendCmd
  MOV      RD2, RD10
  CMP/GT   #1, RD10
  BT       L00800568
  MOV      RD10, RD2
  BRA      L00E0046E

L00800568:

L00800567:
  BSR      TKSPI_Deselect
  BSR      TKSPI_Select
  MOV      RD2, RD13
  CMP/EQ   RD13
  BT       L00800569
  MOV      L00C00124, RQ4
  BSR      printf
  MOV      #255, RD13
  MOV      RD13, RD2
  BRA      L00E0046E

L00800569:
  OR       RD11, #64, RD13
  EXTU.B   RD13, RD14
  MOV      RD14, RD4
  BSR      TKSPI_XchByte
  MOV.L    @(R15, 24), RD8
  SHLD     RD8, #-24, RD14
  EXTU.B   RD14, RD12
  MOV      RD12, RD4
  BSR      TKSPI_XchByte
  SHLD     RD8, #-16, RD14
  EXTU.B   RD14, RD12
  MOV      RD12, RD4
  BSR      TKSPI_XchByte
  SHLD     RD8, #-8, RD14
  EXTU.B   RD14, RD12
  MOV      RD12, RD4
  BSR      TKSPI_XchByte
  EXTU.B   RD8, RD14
  MOV      RD14, RD4
  BSR      TKSPI_XchByte
  MOV      #1, RD14
  MOV      RD14, RD31
  MOV.L    RD31, @(R15, 16)
  CMP/EQ   RD11
  BT       L0080056A
  MOV      #149, RD14
  MOV      RD14, RD30
  MOV.L    RD30, @(R15, 16)

L0080056A:
  CMP/EQ   #8, RD11
  BT       L0080056B
  MOV      #135, RD14
  MOV      RD14, RD29
  MOV.L    RD29, @(R15, 16)

L0080056B:
  MOV.L    @(R15, 16), RD4
  BSR      TKSPI_XchByte
  CMP/EQ   #12, RD11
  BT       L0080056C
  MOV      #255, RD14
  MOV      RD14, RD4
  BSR      TKSPI_XchByte

L0080056C:
  MOV      #10, RD14
  MOV      RD14, RD28
  MOV.L    RD28, @(R15, 20)

L0080056D:
  MOV.L    @(R15, 20), RD27
  CMP/EQ   #0, RD27
  BT       L0080056E
  MOV      #255, RD14
  MOV      RD14, RD4
  BSR      TKSPI_XchByte
  MOV      RD2, RD10
  TST      #128, RD10
  BT       L0080056F
  BRA      L0080056E

L0080056F:
  MOV.L    @(R15, 20), RD26
  ADD      #-1, RD26
  MOV.L    RD26, @(R15, 20)
  BRA      L0080056D

L0080056E:
  MOV      RD10, RD2

L00E0046E:
  ADD      #64, R15
  BRA      L00C00018

TKSPI_ReadSectors:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-104, R15
  MOV.Q    RQ4, @(R15, 72)
  MOV.Q    RQ5, @(R15, 64)
  MOV.L    RD6, @(R15, 60)
  MOV.Q    @(R15, 64), RQ14
  MOV      #32, RD13
  SHAR.Q   RQ14, RD13, RQ14
  MOV.Q    RQ14, @(R15, 24)
  MOV      #0, RQ12
  CMPQ/EQ  RQ12, RQ14
  BT       L00800570
  BRK     

L00800570:
  MOV.Q    @(R15, 72), RQ10
  MOV      RQ10, RQ11
  MOV.Q    @(R15, 64), RQ9
  MOV.L    @(R15, 60), RD31
  MOV      RD31, RD8
  MOV.Q    RQ11, @(R15, 48)
  MOV.Q    RQ9, @(R15, 40)
  MOV.L    RD8, @(R15, 36)

L00800571:
  MOV.L    @(R15, 36), RD30
  CMP/GT   #0, RD30
  BT       L00800572
  MOV.Q    @(R15, 40), RQ29
  MOV      RQ29, RQ28
  MOV      #32, RD27
  SHLR.Q   RQ28, RD27, RQ28
  EXTS.L   RQ28, RD26
  MOV.Q    RQ28, @(R15, 24)
  MOV.L    RD26, @(R15, 32)
  CMP/EQ   #0, RD26
  BT       L00800573
  MOV      #55, RD25
  MOV.L    RD25, @(R15, 12)
  MOV      RD25, RD4
  MOV.L    @(R15, 32), RD5
  BSR      TKSPI_SendCmd

L00800573:
  MOV      #17, RD25
  MOV.L    @(R15, 40), RD26
  EXTU.L   RD26, RD26
  MOV.L    RD26, @(R15, 8)
  MOV.L    RD25, @(R15, 12)
  MOV      RD25, RD4
  MOV      RD26, RD5
  BSR      TKSPI_SendCmd
  MOV.Q    @(R15, 48), RQ4
  MOV      #512, RD5
  BSR      TKSPI_ReadData
  MOV.Q    @(R15, 48), RQ7
  ADD      #512, RQ7
  MOV.Q    @(R15, 40), RQ6
  ADD      #1, RQ6
  MOV.L    @(R15, 36), RD5
  ADD      #-1, RD5
  MOV.Q    RQ7, @(R15, 48)
  MOV.Q    RQ6, @(R15, 40)
  MOV.L    RD5, @(R15, 36)
  BRA      L00800571

L00800572:
  MOV      #0, RD2

L00E0046F:
  ADD      #104, R15
  BRA      L00C00027

TKSPI_WriteSectors:
  PUSH     LR
  BSR      L00C00053
  ADD      #-104, R15
  MOV.Q    RQ4, @(R15, 56)
  MOV.Q    RQ5, @(R15, 48)
  MOV.L    RD6, @(R15, 44)
  MOV.Q    @(R15, 56), RQ10
  MOV      RQ10, RQ13
  MOV.Q    @(R15, 48), RQ9
  MOV.L    @(R15, 44), RD8
  MOV      RD8, RD12
  MOV.Q    RQ9, @(R15, 32)

L00800574:
  CMP/PL   RD12
  BT       L00800575
  MOV.Q    @(R15, 32), RQ31
  MOV      RQ31, RQ30
  MOV      #32, RD29
  SHLR.Q   RQ30, RD29, RQ30
  EXTS.L   RQ30, RD11
  CMP/EQ   RD11
  BT       L00800576
  MOV      #55, RD14
  MOV      RD14, RD4
  MOV      RD11, RD5
  BSR      TKSPI_SendCmd

L00800576:
  MOV      #24, RD14
  MOV.L    @(R15, 32), RD28
  EXTU.L   RD28, RD28
  MOV.L    RD28, @(R15, 8)
  MOV      RD14, RD4
  MOV      RD28, RD5
  BSR      TKSPI_SendCmd
  MOV      RQ13, RQ4
  MOV      #512, RD5
  BSR      TKSPI_WriteData
  ADD      #512, RQ13
  MOV.Q    @(R15, 32), RQ27
  ADD      #1, RQ27
  ADD      #-1, RD12
  MOV.Q    RQ27, @(R15, 32)
  BRA      L00800574

L00800575:
  MOV      #0, RD2

L00E00470:
  ADD      #104, R15
  BRA      L00C00054

TKSPI_InitDevice:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-56, R15
  MOV      #28, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 16)
  MOV.L    init, RD11
  CMP/EQ   #1, RD11
  BT       L00800577
  MOV      #0, RD2
  BRA      L00E00471

L00800577:
  MOV      #1, RD10
  MOV.L    RD10, init
  MOV      #0, RD4
  BSR      TKSPI_SetSpeed
  MOV      #52, RD14
  MOV      RD14, RD4
  MOV      #1, RD5
  BSR      TKSPI_SendCmd
  MOV      #0, RD13

L00800578:
  MOV      #10, RD9
  CMP/HS   RD9, RD13
  BT       L00800579
  MOV      #255, RD14
  MOV      RD14, RD4
  BSR      TKSPI_XchByte

L0080057A:
  ADD      #1, RD13
  BRA      L00800578

L00800579:
  MOV      #0, RD14
  MOV      RD14, RD8
  MOV      #0, RD14
  MOV.L    RD8, @(R15, 8)
  MOV      RD14, RD4
  MOV      #0, RD5
  BSR      TKSPI_SendCmd
  MOV      RD2, RD13
  CMP/EQ   #1, RD13
  BT       L0080057B
  MOV      #8, RD14
  MOV      RD14, RD4
  MOV      #426, RD5
  BSR      TKSPI_SendCmd
  MOV      RD2, RD12
  CMP/EQ   #1, RD12
  BT       L0080057C
  MOV      #0, RD13

L0080057D:
  MOV      #4, RD31
  CMP/HS   RD31, RD13
  BT       L0080057E
  MOV      #255, RD30
  MOV.L    RD30, @R15
  MOV      RD30, RD4
  BSR      TKSPI_XchByte
  MOV      RD2, RD14
  MOV      #28, RQ29
  ADD      R15, RQ29
  EXTS.L   RD13
  MOVU.B   RD14, @(RQ29, RD13)

L0080057F:
  ADD      #1, RD13
  BRA      L0080057D

L0080057E:
  MOV      #28, RQ28
  ADD      R15, RQ28
  MOVU.B   @(RQ28, 2), RD14
  CMP/EQ   #1, RD14
  BT       L00800580
  MOV      #28, RQ27
  ADD      R15, RQ27
  MOVU.B   @(RQ27, 3), RD14
  CMP/EQ   #170, RD14
  BT       L00800580
  BRA      L00800581

L00800581:
  MOV      #0x00020000, RD13
  MOV      #2, RD13

L00800582:
  CMP/EQ   RD13
  BT       L00800583
  MOV      #169, RD14
  MOV      RD14, RD4
  MOV      #0x40000000, RD5
  MOV      #16384, RD5
  BSR      TKSPI_SendCmd
  MOV      RD2, RD12
  CMP/EQ   RD12
  BT       L00800584
  BRA      L00800583

L00800584:
  MOV      #10, RD4
  BSR      TKSPI_DelayUSec
  ADD      #-1, RD13
  BRA      L00800582

L00800583:
  MOV      #58, RD14
  MOV      RD14, RD4
  MOV      #0, RD5
  BSR      TKSPI_SendCmd
  MOV      RD2, RD12
  CMP/EQ   RD13
  BT       L00800585
  CMP/EQ   RD12
  BT       L00800585
  BRA      L00800586

L00800586:
  MOV      #0, RD13

L00800587:
  MOV      #4, RD26
  CMP/HS   RD26, RD13
  BT       L00800588
  MOV      #255, RD25
  MOV.L    RD25, @R15
  MOV      RD25, RD4
  BSR      TKSPI_XchByte
  MOV      RD2, RD14
  MOV      #28, RQ26
  ADD      R15, RQ26
  EXTS.L   RD13
  MOVU.B   RD14, @(RQ26, RD13)

L00800589:
  ADD      #1, RD13
  BRA      L00800587

L00800588:
  MOV      #28, RQ7
  ADD      R15, RQ7
  MOVU.B   @RQ7, RD6
  MOV.L    RD6, @R15
  TST      #64, RD6
  BT       L0080058A
  MOV      #67, RD14
  BRA      L0080058B

L0080058A:
  MOV      #3, RD14

L0080058B:
  EXTU.B   RD14, RD5
  MOV      RD5, RD4
  MOV.L    RD5, @R15
  MOV.L    RD4, @(R15, 8)

L00800585:

L00800580:
  BRA      L0080058C

L0080057C:
  MOV      #169, RD14
  MOV      RD14, RD4
  MOV      #0, RD5
  BSR      TKSPI_SendCmd
  MOV      RD2, RD12
  CMP/GT   #1, RD12
  BT       L0080058D
  MOV      #2, RD14
  MOV      RD14, RD3
  MOV      #169, RD14
  MOV      RD14, RD2
  MOV.L    RD3, @(R15, 8)
  MOV.L    RD2, @(R15, 12)
  BRA      L0080058E

L0080058D:
  MOV      #1, RD14
  MOV      RD14, RD23
  MOV      #1, RD14
  MOV      RD14, RD22
  MOV.L    RD23, @(R15, 8)
  MOV.L    RD22, @(R15, 12)

L0080058E:
  MOV      #0x00020000, RD13
  MOV      #2, RD13

L0080058F:
  CMP/EQ   RD13
  BT       L00800590
  MOV.L    @(R15, 12), RD4
  MOV      #0, RD5
  BSR      TKSPI_SendCmd
  MOV      RD2, RD12
  CMP/EQ   RD12
  BT       L00800591
  BRA      L00800590

L00800591:
  MOV      #10, RD4
  BSR      TKSPI_DelayUSec
  ADD      #-1, RD13
  BRA      L0080058F

L00800590:
  MOV      #16, RD14
  MOV      RD14, RD4
  MOV      #512, RD5
  BSR      TKSPI_SendCmd
  MOV      RD2, RD12
  CMP/EQ   RD13
  BT       L00800592
  CMP/EQ   RD12
  BT       L00800592
  BRA      L00800593

L00800592:
  MOV      #0, RD14
  MOV      RD14, RD21
  MOV.L    RD21, @(R15, 8)

L00800593:

L0080058C:
  BRA      L00800594

L0080057B:
  MOV      #0, RD14
  MOV      RD14, RD25
  MOV.L    RD25, @(R15, 8)
  MOV      L00C00128, RQ4
  MOV      RD13, RD5
  BSR      printf

L00800594:
  BSR      TKSPI_Deselect
  MOV.L    @(R15, 8), RD20
  CMP/EQ   #0, RD20
  BT       L00800595
  MOV      #1, RD4
  BSR      TKSPI_SetSpeed
  MOV.L    @(R15, 8), RD26
  MOV      RD26, RD25
  MOV.B    RD25, tkspi_init_ok
  MOV      L00C00129, RQ4
  MOV      RD26, RD5
  BSR      printf
  BRA      L00800596

L00800595:
  BSR      TKSPI_PowerOff
  MOV      #0, RD14
  MOV      RD14, RD25
  MOV.B    RD25, tkspi_init_ok
  MOV      L00C0012A, RQ4
  BSR      printf

L00800596:
  MOVU.B   tkspi_init_ok, RD2

L00E00471:
  ADD      #56, R15
  BRA      L00C00027

tkfat_setWord:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  ADD      #-136, R15
  MOV      RQ4, RQ13
  MOV      RD5, RD14
  MOVU.B   RD14, @RQ13
  SHAD     RD14, #-8, RD12
  MOVU.B   RD12, @(RQ13, 1)

L00E00472:
  ADD      #136, R15
  BRA      L00C0005F

tkfat_setDWord:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  ADD      #-136, R15
  MOV      RQ4, RQ12
  MOV      RD5, RD13
  MOVU.B   RD13, @RQ12
  SHLD     RD13, #-8, RD14
  MOVU.B   RD14, @(RQ12, 1)
  SHLD     RD13, #-16, RD14
  MOVU.B   RD14, @(RQ12, 2)
  SHLD     RD13, #-24, RD14
  MOVU.B   RD14, @(RQ12, 3)

L00E00473:
  ADD      #136, R15
  BRA      L00C0005F

TKFAT_GetSectorTempBuffer:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-4232, R15
  MOV.Q    RQ4, @(R15, 104)
  MOV.L    RD5, @(R15, 100)
  MOV.L    RD6, @(R15, 96)
  MOV.Q    @(R15, 104), RQ8
  MOV.L    @(RQ8, 4560), RD9
  ADD      RQ8, #972, RQ10
  ADD      RQ8, #1996, RQ31
  MOV.L    @(R15, 96), RD30
  EXTU.B   RD30, RD29
  MOV.L    @(RQ8, 4564), RD14
  MOVU.L   @(RQ10, RD14), RD13
  MOV      RD13, RD12
  MOV.L    @(R15, 100), RD28
  MOV      RD28, RD11
  MOV.Q    RQ31, @(R15, 80)
  MOV.L    RD29, @(R15, 60)
  CMP/EQ   RD11, RD12
  BT       L00800597
  MOV.Q    @(R15, 104), RQ27
  MOV.L    @(RQ27, 4568), RD14

L00800597:
  EXTS.L   RD14
  MOVU.L   @(RQ10, RD14), RD13
  MOV      RD13, RD12
  MOV.L    @(R15, 100), RD26
  MOV      RD26, RD11
  CMP/EQ   RD11, RD12
  BT       L00800598
  MOV      #0, RD14

L00800599:
  CMP/GT   RD14, RD9
  BT       L0080059A
  EXTS.L   RD14
  LEA.L    @(RQ10, RD14), RQ25
  MOVU.L   @RQ25, RD13
  MOV.L    @(R15, 100), RD7
  CMP/EQ   RD7, RD13
  MOVT     RD12
  MOVU.L   @(RQ25, 4), RD13
  CMP/EQ   RD7, RD13
  MOVT     RD11
  OR       RD12, RD11, RD13
  MOVU.L   @(RQ25, 8), RD12
  CMP/EQ   RD7, RD12
  MOVT     RD11
  OR       RD13, RD11, RD12
  MOVU.L   @(RQ25, 12), RD13
  CMP/EQ   RD7, RD13
  MOVT     RD11
  OR       RD12, RD11, RD6
  MOVU.L   @(RQ25, 16), RD13
  CMP/EQ   RD7, RD13
  MOVT     RD12
  MOVU.L   @(RQ25, 20), RD13
  CMP/EQ   RD7, RD13
  MOVT     RD11
  OR       RD12, RD11, RD13
  MOVU.L   @(RQ25, 24), RD12
  CMP/EQ   RD7, RD12
  MOVT     RD11
  OR       RD13, RD11, RD12
  MOVU.L   @(RQ25, 28), RD13
  CMP/EQ   RD7, RD13
  MOVT     RD11
  OR       RD12, RD11, RD5
  MOV.Q    RQ25, @(R15, 88)
  MOV.L    RD6, @(R15, 52)
  MOV.L    RD5, @(R15, 48)
  CMP/EQ   RD6
  BT       L0080059B
  BRA      L0080059A

L0080059B:
  MOV.L    @(R15, 48), RD4
  CMP/EQ   RD4
  BT       L0080059C
  ADD      #4, RD14
  BRA      L0080059A

L0080059C:

L0080059D:
  ADD      #8, RD14
  BRA      L00800599

L0080059A:

L0080059E:
  CMP/GT   RD14, RD9
  BT       L0080059F
  EXTS.L   RD14
  MOVU.L   @(RQ10, RD14), RD13
  MOV      RD13, RD12
  MOV.L    @(R15, 100), RD3
  MOV      RD3, RD11
  CMP/EQ   RD11, RD12
  BT       L008005A0
  BRA      L0080059F

L008005A0:

L008005A1:
  ADD      #1, RD14
  BRA      L0080059E

L0080059F:

L00800598:
  CMP/GT   RD14, RD9
  BT       L008005A2
  EXTS.L   RD14
  MOVU.L   @(RQ10, RD14), RD13
  MOV      RD13, RD12
  MOV.L    @(R15, 100), RD2
  MOV      RD2, RD11
  CMP/EQ   RD11, RD12
  BT       L008005A3
  MOV.Q    @(R15, 80), RQ23
  EXTS.L   RD14
  MOV.W    @(RQ23, RD14), RD13
  MOV.L    @(R15, 60), RD22
  CMP/EQ   RD22, RD13
  BT       L008005A3
  BRA      L008005A4

L008005A4:
  MOV.L    @(R15, 96), RD21
  TST      #512, RD21
  BT       L008005A5
  MOV.Q    @(R15, 104), RQ20
  ADD      RQ20, #1996, RQ19
  EXTS.L   RD14
  MOV.W    @(RQ19, RD14), RD13
  OR       RD13, #512, RD12
  ADD      RQ20, #1996, RQ19
  EXTS.L   RD14
  MOV.W    RD12, @(RQ19, RD14)
  MOV.Q    RQ19, @(R15, 24)

L008005A5:
  CMP/EQ   RD14
  BT       L008005A6
  MOV.Q    @(R15, 104), RQ18
  ADD      RQ18, #2512, RQ17
  EXTS.L   RD14
  MOV.Q    @(RQ17, RD14), RQ16
  MOV.Q    RQ17, @(R15, 24)
  MOV.Q    RQ16, @(R15, 64)
  MOV      RQ16, RQ2
  BRA      L00E00474

L008005A6:
  MOV      RD14, RD13
  MOV      #7, RD16
  MUL.L    RD16, RD13, RD13
  SHAD     RD13, #-3, RD17
  MOV.Q    @(R15, 104), RQ18
  ADD      RQ18, #2512, RQ19
  EXTS.L   RD14
  MOV.Q    @(RQ19, RD14), RQ20
  ADD      RQ18, #972, RQ19
  EXTS.L   RD14
  MOVU.L   @(RQ19, RD14), RD21
  ADD      RQ18, #1996, RQ19
  EXTS.L   RD14
  MOV.W    @(RQ19, RD14), RD22
  ADD      RQ18, #2512, RQ19
  EXTS.L   RD17
  MOV.Q    @(RQ19, RD17), RQ23
  ADD      RQ18, #2512, RQ19
  EXTS.L   RD14
  MOV.Q    RQ23, @(RQ19, RD14)
  ADD      RQ18, #972, RQ19
  EXTS.L   RD17
  MOVU.L   @(RQ19, RD17), RD13
  ADD      RQ18, #972, RQ19
  EXTS.L   RD14
  MOVU.L   RD13, @(RQ19, RD14)
  ADD      RQ18, #1996, RQ19
  EXTS.L   RD17
  MOV.W    @(RQ19, RD17), RD13
  ADD      RQ18, #1996, RQ19
  EXTS.L   RD14
  MOV.W    RD13, @(RQ19, RD14)
  ADD      RQ18, #2512, RQ19
  EXTS.L   RD17
  MOV.Q    RQ20, @(RQ19, RD17)
  ADD      RQ18, #972, RQ19
  EXTS.L   RD17
  MOVU.L   RD21, @(RQ19, RD17)
  ADD      RQ18, #1996, RQ19
  EXTS.L   RD17
  MOV.W    RD22, @(RQ19, RD17)
  MOV.L    @(RQ18, 4564), RD13
  MOV.Q    RQ23, @(R15, 16)
  MOV.L    RD22, @(R15, 56)
  MOV.L    RD21, @(R15, 76)
  MOV.Q    RQ20, @(R15, 64)
  MOV.Q    RQ19, @(R15, 24)
  MOV.L    RD17, @(R15, 52)
  CMP/EQ   RD13, RD17
  BT       L008005A7
  MOV.Q    @(R15, 104), RQ16
  MOV.L    @(RQ16, 4564), RD13
  MOV.L    RD13, @(RQ16, 4568)
  MOV.L    @(R15, 52), RD17
  MOV.L    RD17, @(RQ16, 4564)

L008005A7:
  MOV.Q    @(R15, 64), RQ2
  BRA      L00E00474

L008005A3:

L008005A2:
  MOV.Q    @(R15, 104), RQ16
  MOV.L    @(RQ16, 4560), RD13
  CMP/GE   #256, RD13
  BT       L008005A8
  MOV.Q    @(R15, 104), RQ25
  MOV.L    @(RQ25, 4560), RD13
  MOV      RD13, RD12
  ADD      RD13, #1, RD11
  MOV.L    RD11, @(RQ25, 4560)
  MOV      RD12, RD14
  MOV.L    @(R15, 60), RD26
  SHAD     RD26, #9, RD13
  EXTS.L   RD13, RQ27
  MOV.Q    RQ27, @(R15, 8)
  MOV      RQ27, RQ4
  BSR      malloc
  MOV      RQ2, RQ28
  ADD      RQ25, #2512, RQ29
  MOV.Q    RQ28, @(RQ29, RD14)
  ADD      RQ25, #972, RQ29
  MOV.L    @(R15, 100), RD30
  MOVU.L   RD30, @(RQ29, RD14)
  ADD      RQ25, #1996, RQ29
  MOV.W    RD26, @(RQ29, RD14)
  MOV.L    @(RQ25, 4564), RD12
  MOV.L    RD12, @(RQ25, 4568)
  MOV.L    RD14, @(RQ25, 4564)
  MOV.Q    RQ29, @(R15, 24)
  MOV.Q    RQ28, @(R15, 64)
  BRA      L008005A9

L008005A8:
  MOV      #255, RD14
  MOV.Q    @(R15, 104), RQ16
  ADD      RQ16, #1996, RQ17
  MOV.W    @(RQ17, RD14), RD12
  MOV.Q    RQ17, @(R15, 24)
  TST      #512, RD12
  BT       L008005AA
  MOV.Q    @(R15, 104), RQ25
  ADD      RQ25, #1996, RQ26
  MOV.W    @(RQ26, RD14), RD12
  EXTU.B   RD12, RD11
  ADD      RQ25, #972, RQ26
  MOVU.L   @(RQ26, RD14), RD12
  ADD      RQ25, #2512, RQ26
  MOV.Q    @(RQ26, RD14), RQ27
  EXTU.L   RD12, RQ28
  MOV.Q    RQ28, @(R15, 8)
  MOV.Q    RQ27, @(R15, 16)
  MOV.Q    RQ26, @(R15, 24)
  MOV      RQ27, RQ4
  MOV      RQ28, RQ5
  MOV      RD11, RD6
  BSR      TKSPI_WriteSectors

L008005AA:
  MOV.Q    @(R15, 104), RQ16
  ADD      RQ16, #1996, RQ17
  EXTS.L   RD14
  MOV.W    @(RQ17, RD14), RD11
  MOV.Q    RQ17, @(R15, 24)
  MOV.L    @(R15, 60), RD18
  CMP/EQ   RD11, RD18
  BT       L008005AB
  MOV.Q    @(R15, 104), RQ25
  ADD      RQ25, #2512, RQ26
  EXTS.L   RD14
  MOV.Q    @(RQ26, RD14), RQ27
  MOV.Q    RQ27, @(R15, 16)
  MOV.Q    RQ26, @(R15, 24)
  MOV      RQ27, RQ4
  BSR      free
  MOV.L    @(R15, 60), RD28
  SHAD     RD28, #9, RD11
  EXTS.L   RD11, RQ29
  MOV.Q    RQ29, @(R15, 8)
  MOV      RQ29, RQ4
  BSR      malloc
  MOV      RQ2, RQ26
  ADD      RQ25, #2512, RQ27
  EXTS.L   RD14
  MOV.Q    RQ26, @(RQ27, RD14)
  MOV.Q    RQ27, @(R15, 16)
  MOV.Q    RQ26, @(R15, 24)

L008005AB:
  MOV.Q    @(R15, 104), RQ16
  ADD      RQ16, #972, RQ17
  MOV.L    @(R15, 100), RD18
  EXTS.L   RD14
  MOVU.L   RD18, @(RQ17, RD14)
  ADD      RQ16, #1996, RQ17
  MOV.L    @(R15, 60), RD19
  EXTS.L   RD14
  MOV.W    RD19, @(RQ17, RD14)
  ADD      RQ16, #2512, RQ17
  EXTS.L   RD14
  MOV.Q    @(RQ17, RD14), RQ20
  MOV.L    @(RQ16, 4564), RD21
  MOV.L    RD21, @(RQ16, 4568)
  MOV.L    RD14, @(RQ16, 4564)
  MOV.Q    RQ20, @(R15, 64)
  MOV.Q    RQ17, @(R15, 24)

L008005A9:
  MOV.L    @(R15, 100), RD25
  EXTS.L   RQ25, RQ25
  MOV.Q    RQ25, @(R15, 8)
  MOV.Q    @(R15, 64), RQ4
  MOV      RQ25, RQ5
  MOV.L    @(R15, 60), RD6
  BSR      TKSPI_ReadSectors
  MOV.Q    @(R15, 64), RQ2

L00E00474:
  ADD      #4232, R15
  BRA      L00C00027

TKFAT_GetSectorStaticBuffer:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-600, R15
  MOV.Q    RQ4, @(R15, 56)
  MOV.L    RD5, @(R15, 52)
  MOV.L    RD6, @(R15, 48)
  MOV      #0, RD13

L008005AC:
  MOV.Q    @(R15, 56), RQ11
  MOV.L    @(RQ11, 968), RD14
  CMP/GT   RD13, RD14
  BT       L008005AD
  MOV.Q    @(R15, 56), RQ10
  ADD      RQ10, #72, RQ9
  EXTS.L   RD13
  MOVU.L   @(RQ9, RD13), RD14
  MOV      RD14, RD12
  MOV.L    @(R15, 52), RD31
  MOV      RD31, RD8
  MOV.Q    RQ9, @(R15, 24)
  MOV.L    RD8, @(R15, 16)
  CMP/EQ   RD8, RD12
  BT       L008005AE
  MOV.Q    @(R15, 56), RQ30
  ADD      RQ30, #456, RQ29
  EXTS.L   RD13
  MOV.Q    @(RQ29, RD13), RQ28
  MOV.Q    RQ29, @(R15, 24)
  MOV.Q    RQ28, @(R15, 8)
  MOV      RQ28, RQ2
  BRA      L00E00475

L008005AE:

L008005AF:
  ADD      #1, RD13
  BRA      L008005AC

L008005AD:
  MOV.Q    @(R15, 56), RQ27
  MOV.L    @(RQ27, 968), RD14
  MOV      RD14, RD12
  ADD      RD14, #1, RD26
  MOV.L    RD26, @(RQ27, 968)
  MOV      RD12, RD13
  MOV.L    @(R15, 48), RD25
  EXTU.B   RD25, RD14
  SHAD     RD14, #9, RD12
  EXTS.L   RD12, RQ28
  MOV.Q    RQ28, @R15
  MOV.L    RD26, @(R15, 16)
  MOV      RQ28, RQ4
  BSR      malloc
  MOV      RQ2, RQ29
  ADD      RQ27, #456, RQ30
  MOV.Q    RQ29, @(RQ30, RD13)
  ADD      RQ27, #72, RQ29
  MOV.L    @(R15, 52), RD31
  MOVU.L   RD31, @(RQ29, RD13)
  EXTS.L   RD31, RQ28
  MOV      RQ28, RQ8
  EXTU.B   RD25, RD14
  ADD      RQ27, #456, RQ29
  MOV.Q    @(RQ29, RD13), RQ30
  MOV.Q    RQ8, @(R15, 40)
  MOV.Q    RQ30, @(R15, 8)
  MOV.Q    RQ29, @(R15, 24)
  MOV.Q    RQ28, @R15
  MOV      RQ30, RQ4
  MOV      RQ8, RQ5
  MOV      RD14, RD6
  BSR      TKSPI_ReadSectors
  ADD      RQ27, #456, RQ29
  MOV.Q    @(RQ29, RD13), RQ30
  MOV.Q    RQ30, @(R15, 8)
  MOV.Q    RQ29, @(R15, 24)
  MOV      RQ30, RQ2

L00E00475:
  ADD      #600, R15
  BRA      L00C00027

TKFAT_GetSectorTempFatBuffer:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-4488, R15
  MOV.Q    RQ4, @(R15, 360)
  MOV.L    RD5, @(R15, 356)
  MOV.L    RD6, @(R15, 352)
  MOV.Q    @(R15, 360), RQ8
  MOV.L    @(RQ8, 8160), RD9
  ADD      RQ8, #4572, RQ10
  ADD      RQ8, #5596, RQ31
  ADD      RQ8, #6112, RQ30
  MOV.L    @(R15, 352), RD29
  EXTU.B   RD29, RD28
  MOV.L    @(RQ8, 8164), RD14
  MOVU.L   @(RQ10, RD14), RD13
  MOV      RD13, RD12
  MOV.L    @(R15, 356), RD27
  MOV      RD27, RD11
  MOV.Q    RQ31, @(R15, 336)
  MOV.Q    RQ30, @(R15, 328)
  MOV.L    RD28, @(R15, 308)
  CMP/EQ   RD11, RD12
  BT       L008005B0
  MOV.Q    @(R15, 360), RQ26
  MOV.L    @(RQ26, 8168), RD14

L008005B0:
  EXTS.L   RD14
  MOVU.L   @(RQ10, RD14), RD13
  MOV      RD13, RD12
  MOV.L    @(R15, 356), RD25
  MOV      RD25, RD11
  CMP/EQ   RD11, RD12
  BT       L008005B1
  MOV      #0, RD14

L008005B2:
  CMP/GT   RD14, RD9
  BT       L008005B3
  EXTS.L   RD14
  LEA.L    @(RQ10, RD14), RQ7
  MOVU.L   @RQ7, RD13
  MOV.L    @(R15, 356), RD6
  CMP/EQ   RD6, RD13
  MOVT     RD12
  MOVU.L   @(RQ7, 4), RD13
  CMP/EQ   RD6, RD13
  MOVT     RD11
  OR       RD12, RD11, RD13
  MOVU.L   @(RQ7, 8), RD12
  CMP/EQ   RD6, RD12
  MOVT     RD11
  OR       RD13, RD11, RD12
  MOVU.L   @(RQ7, 12), RD13
  CMP/EQ   RD6, RD13
  MOVT     RD11
  OR       RD12, RD11, RD5
  MOVU.L   @(RQ7, 16), RD13
  CMP/EQ   RD6, RD13
  MOVT     RD12
  MOVU.L   @(RQ7, 20), RD13
  CMP/EQ   RD6, RD13
  MOVT     RD11
  OR       RD12, RD11, RD13
  MOVU.L   @(RQ7, 24), RD12
  CMP/EQ   RD6, RD12
  MOVT     RD11
  OR       RD13, RD11, RD12
  MOVU.L   @(RQ7, 28), RD13
  CMP/EQ   RD6, RD13
  MOVT     RD11
  OR       RD12, RD11, RD4
  MOV.Q    RQ7, @(R15, 344)
  MOV.L    RD5, @(R15, 300)
  MOV.L    RD4, @(R15, 296)
  CMP/EQ   RD5
  BT       L008005B4
  BRA      L008005B3

L008005B4:
  MOV.L    @(R15, 296), RD3
  CMP/EQ   RD3
  BT       L008005B5
  ADD      #4, RD14
  BRA      L008005B3

L008005B5:

L008005B6:
  ADD      #8, RD14
  BRA      L008005B2

L008005B3:

L008005B7:
  CMP/GT   RD14, RD9
  BT       L008005B8
  EXTS.L   RD14
  MOVU.L   @(RQ10, RD14), RD13
  MOV      RD13, RD12
  MOV.L    @(R15, 356), RD2
  MOV      RD2, RD11
  CMP/EQ   RD11, RD12
  BT       L008005B9
  BRA      L008005B8

L008005B9:

L008005BA:
  ADD      #1, RD14
  BRA      L008005B7

L008005B8:

L008005B1:
  CMP/GT   RD14, RD9
  BT       L008005BB
  EXTS.L   RD14
  MOVU.L   @(RQ10, RD14), RD13
  MOV      RD13, RD12
  MOV.L    @(R15, 356), RD23
  MOV      RD23, RD11
  CMP/EQ   RD11, RD12
  BT       L008005BC
  MOV.Q    @(R15, 336), RQ22
  EXTS.L   RD14
  MOV.W    @(RQ22, RD14), RD13
  MOV.L    @(R15, 308), RD21
  CMP/EQ   RD21, RD13
  BT       L008005BC
  BRA      L008005BD

L008005BD:
  MOV.L    @(R15, 352), RD20
  TST      #512, RD20
  BT       L008005BE
  MOV.Q    @(R15, 360), RQ19
  ADD      RQ19, #5596, RQ18
  EXTS.L   RD14
  MOV.W    @(RQ18, RD14), RD13
  OR       RD13, #512, RD12
  ADD      RQ19, #5596, RQ18
  EXTS.L   RD14
  MOV.W    RD12, @(RQ18, RD14)
  MOV.Q    RQ18, @(R15, 272)

L008005BE:
  CMP/EQ   RD14
  BT       L008005BF
  MOV.Q    @(R15, 328), RQ17
  EXTS.L   RD14
  MOV.Q    @(RQ17, RD14), RQ16
  MOV.Q    RQ16, @(R15, 312)
  MOV      RQ16, RQ2
  BRA      L00E00476

L008005BF:
  MOV      RD14, RD13
  MOV      #7, RD16
  MUL.L    RD16, RD13, RD13
  SHAD     RD13, #-3, RD17
  MOV.Q    @(R15, 328), RQ18
  EXTS.L   RD14
  MOV.Q    @(RQ18, RD14), RQ19
  EXTS.L   RD14
  MOVU.L   @(RQ10, RD14), RD20
  MOV.Q    @(R15, 336), RQ21
  EXTS.L   RD14
  MOV.W    @(RQ21, RD14), RD22
  EXTS.L   RD17
  MOV.Q    @(RQ18, RD17), RQ23
  EXTS.L   RD14
  MOV.Q    RQ23, @(RQ18, RD14)
  EXTS.L   RD17
  MOVU.L   @(RQ10, RD17), RD13
  EXTS.L   RD14
  MOVU.L   RD13, @(RQ10, RD14)
  EXTS.L   RD17
  MOV.W    @(RQ21, RD17), RD13
  EXTS.L   RD14
  MOV.W    RD13, @(RQ21, RD14)
  EXTS.L   RD17
  MOV.Q    RQ19, @(RQ18, RD17)
  EXTS.L   RD17
  MOVU.L   RD20, @(RQ10, RD17)
  EXTS.L   RD17
  MOV.W    RD22, @(RQ21, RD17)
  MOV.Q    @(R15, 360), RQ2
  MOV.L    @(RQ2, 8164), RD13
  MOV.Q    RQ23, @(R15, 272)
  MOV.L    RD22, @(R15, 304)
  MOV.L    RD20, @(R15, 324)
  MOV.Q    RQ19, @(R15, 312)
  MOV.L    RD17, @(R15, 300)
  CMP/EQ   RD13, RD17
  BT       L008005C0
  MOV.Q    @(R15, 360), RQ16
  MOV.L    @(RQ16, 8164), RD13
  MOV.L    RD13, @(RQ16, 8168)
  MOV.L    @(R15, 300), RD17
  MOV.L    RD17, @(RQ16, 8164)

L008005C0:
  MOV.Q    @(R15, 312), RQ2
  BRA      L00E00476

L008005BC:

L008005BB:
  MOV.Q    @(R15, 360), RQ16
  MOV.L    @(RQ16, 8160), RD13
  CMP/GE   #256, RD13
  BT       L008005C1
  MOV.Q    @(R15, 360), RQ25
  MOV.L    @(RQ25, 8160), RD13
  MOV      RD13, RD12
  ADD      RD13, #1, RD11
  MOV.L    RD11, @(RQ25, 8160)
  MOV      RD12, RD14
  MOV.L    @(R15, 308), RD26
  SHAD     RD26, #9, RD13
  EXTS.L   RD13, RQ27
  MOV.Q    RQ27, @(R15, 264)
  MOV      RQ27, RQ4
  BSR      malloc
  MOV      RQ2, RQ28
  MOV.Q    @(R15, 328), RQ29
  MOV.Q    RQ28, @(RQ29, RD14)
  MOV.L    @(R15, 356), RD30
  MOVU.L   RD30, @(RQ10, RD14)
  MOV.Q    @(R15, 336), RQ31
  MOV.W    RD26, @(RQ31, RD14)
  MOV.L    @(RQ25, 8164), RD12
  MOV.L    RD12, @(RQ25, 8168)
  MOV.L    RD14, @(RQ25, 8164)
  MOV.Q    RQ28, @(R15, 312)
  BRA      L008005C2

L008005C1:
  MOV      #255, RD14
  MOV.Q    @(R15, 360), RQ16
  ADD      RQ16, #5596, RQ17
  MOV.W    @(RQ17, RD14), RD12
  MOV.Q    RQ17, @(R15, 272)
  TST      #512, RD12
  BT       L008005C3
  MOV.Q    @(R15, 360), RQ25
  ADD      RQ25, #5596, RQ26
  MOV.W    @(RQ26, RD14), RD12
  EXTU.B   RD12, RD11
  ADD      RQ25, #4572, RQ26
  MOVU.L   @(RQ26, RD14), RD12
  ADD      RQ25, #6112, RQ26
  MOV.Q    @(RQ26, RD14), RQ27
  EXTU.L   RD12, RQ28
  MOV.Q    RQ28, @(R15, 264)
  MOV.Q    RQ27, @(R15, 256)
  MOV.Q    RQ26, @(R15, 272)
  MOV      RQ27, RQ4
  MOV      RQ28, RQ5
  MOV      RD11, RD6
  BSR      TKSPI_WriteSectors

L008005C3:
  MOV.Q    @(R15, 360), RQ16
  ADD      RQ16, #5596, RQ17
  EXTS.L   RD14
  MOV.W    @(RQ17, RD14), RD11
  MOV.Q    RQ17, @(R15, 272)
  MOV.L    @(R15, 308), RD18
  CMP/EQ   RD11, RD18
  BT       L008005C4
  MOV.Q    @(R15, 360), RQ25
  ADD      RQ25, #6112, RQ26
  EXTS.L   RD14
  MOV.Q    @(RQ26, RD14), RQ27
  MOV.Q    RQ27, @(R15, 256)
  MOV.Q    RQ26, @(R15, 272)
  MOV      RQ27, RQ4
  BSR      free
  MOV.L    @(R15, 308), RD28
  SHAD     RD28, #9, RD11
  EXTS.L   RD11, RQ29
  MOV.Q    RQ29, @(R15, 264)
  MOV      RQ29, RQ4
  BSR      malloc
  MOV      RQ2, RQ26
  ADD      RQ25, #6112, RQ27
  EXTS.L   RD14
  MOV.Q    RQ26, @(RQ27, RD14)
  MOV.Q    RQ27, @(R15, 256)
  MOV.Q    RQ26, @(R15, 272)

L008005C4:
  MOV.Q    @(R15, 360), RQ16
  ADD      RQ16, #4572, RQ17
  MOV.L    @(R15, 356), RD18
  EXTS.L   RD14
  MOVU.L   RD18, @(RQ17, RD14)
  ADD      RQ16, #5596, RQ17
  MOV.L    @(R15, 308), RD19
  EXTS.L   RD14
  MOV.W    RD19, @(RQ17, RD14)
  ADD      RQ16, #6112, RQ17
  EXTS.L   RD14
  MOV.Q    @(RQ17, RD14), RQ20
  MOV.L    @(RQ16, 8164), RD21
  MOV.L    RD21, @(RQ16, 8168)
  MOV.L    RD14, @(RQ16, 8164)
  MOV.Q    RQ20, @(R15, 312)
  MOV.Q    RQ17, @(R15, 272)

L008005C2:
  MOV.L    @(R15, 356), RD25
  EXTS.L   RQ25, RQ25
  MOV.Q    RQ25, @(R15, 264)
  MOV.Q    @(R15, 312), RQ4
  MOV      RQ25, RQ5
  MOV.L    @(R15, 308), RD6
  BSR      TKSPI_ReadSectors
  MOV.Q    @(R15, 312), RQ2

L00E00476:
  ADD      #4488, R15
  BRA      L00C00027

tkfat_fstnameforfsty:
  PUSH     LR
  BSR      L00C0003D
  ADD      #-96, R15
  MOV      RD4, RD13
  CMP/EQ   RD13
  BT       L008005C8
  CMP/EQ   #6, RD13
  BT       L008005C9
  CMP/EQ   #11, RD13
  BT       L008005CA
  CMP/EQ   #12, RD13
  BT       L008005CB
  BRA      L008005CC

L008005C8:
  MOV      L00C00131, RQ12
  MOV      RQ12, RQ14
  BRA      L008005CD

L008005C9:
  MOV      L00C00132, RQ11
  MOV      RQ11, RQ14
  BRA      L008005CD

L008005CA:
  MOV      L00C00133, RQ10
  MOV      RQ10, RQ14
  BRA      L008005CD

L008005CB:
  MOV      L00C00133, RQ9
  MOV      RQ9, RQ14
  BRA      L008005CD

L008005CC:
  MOV      L00C00134, RQ8
  MOV      RQ8, RQ14
  BRA      L008005CD

L008005CD:
  MOV      RQ14, RQ2

L00E00477:
  ADD      #96, R15
  BRA      L00C0003E

TKFAT_ReadImageMBR:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-184, R15
  MOV.Q    RQ4, @(R15, 80)
  MOV      L00C00136, RQ4
  BSR      printf
  MOV.Q    @(R15, 80), RQ4
  MOV      #0, RD5
  MOV      #1, RD6
  BSR      TKFAT_GetSectorStaticBuffer
  MOV      RQ2, RQ14
  MOV.Q    @(R15, 80), RQ12
  MOV.Q    RQ14, @(RQ12, 48)
  MOV      #0, RD13

L008005CE:
  MOV      #4, RD11
  CMP/GE   RD11, RD13
  BT       L008005CF
  MOV.Q    @(R15, 80), RQ10
  MOV.Q    @(RQ10, 48), RQ14
  ADD      RQ14, #446, RQ9
  EXTS.L   RD13
  SHAD     RD13, #4, RQ8
  ADD      RQ9, RQ8
  MOV.B    @(RQ8, 4), RD31
  EXTU.B   RD31, RD31
  MOV.Q    @(RQ10, 48), RQ14
  ADD      RQ14, #446, RQ9
  EXTS.L   RD13
  SHAD     RD13, #4, RQ8
  ADD      RQ9, RQ8
  ADD      RQ8, #8, RQ14
  MOVU.L   @RQ14, RD30
  MOV      RD30, RD29
  MOV.Q    @(RQ10, 48), RQ14
  ADD      RQ14, #446, RQ9
  EXTS.L   RD13
  SHAD     RD13, #4, RQ8
  ADD      RQ9, RQ8
  ADD      RQ8, #12, RQ14
  MOVU.L   @RQ14, RD30
  MOV      RD30, RD28
  MOV.Q    RQ9, @(R15, 40)
  MOV.Q    RQ8, @(R15, 24)
  MOV.L    RD31, @(R15, 60)
  MOV.L    RD30, @(R15, 20)
  MOV.L    RD29, @(R15, 68)
  MOV.L    RD28, @(R15, 64)
  MOV      RD31, RD4
  BSR      tkfat_fstnameforfsty
  MOV      RQ2, RQ27
  MOV.Q    RQ27, @(R15, 72)
  MOV      L00C00137, RQ4
  MOV      RD29, RD5
  MOV      RD28, RD6
  MOV      RD31, RD7
  MOV      RQ27, RQ20
  BSR      printf

L008005D0:
  ADD      #1, RD13
  BRA      L008005CE

L008005CF:
  MOV.Q    @(R15, 80), RQ26
  MOV.Q    @(RQ26, 48), RQ14
  ADD      RQ14, #446, RQ25
  MOV      RQ25, RQ27
  MOV.B    @(RQ27, 4), RD28
  EXTU.B   RD28, RD28
  EXTU.B   RD28, RD28
  MOV.B    RD28, @(RQ26, 12)
  MOV.Q    @(RQ26, 48), RQ14
  ADD      RQ14, #446, RQ25
  MOV      RQ25, RQ27
  ADD      RQ27, #8, RQ14
  MOVU.L   @RQ14, RD28
  MOV.L    RD28, @(RQ26, 16)
  MOV.Q    @(RQ26, 48), RQ14
  ADD      RQ14, #446, RQ25
  MOV      RQ25, RQ27
  ADD      RQ27, #12, RQ14
  MOVU.L   @RQ14, RD28
  MOV.L    RD28, @(RQ26, 20)
  MOV.B    @(RQ26, 12), RD28
  EXTU.B   RD28, RD28
  MOV      #6, RD29
  CMP/EQ   RD29, RD28
  MOVT     RQ7
  MOV      RQ7, RD30
  EXTU.B   RD30, RD30
  MOV.B    RD30, @(RQ26, 13)
  MOV.B    @(RQ26, 12), RD28
  EXTU.B   RD28, RD28
  MOV.L    RD30, @(R15, 16)
  MOV.L    RD28, @(R15, 20)
  MOV.Q    RQ27, @(R15, 24)
  MOV.Q    RQ25, @(R15, 40)
  MOV      RD28, RD4
  BSR      tkfat_fstnameforfsty
  MOV      RQ2, RQ31
  MOV.B    @(RQ26, 12), RD28
  EXTU.B   RD28, RD28
  MOV.L    @(RQ26, 20), RD30
  MOV.L    @(RQ26, 16), RD8
  MOV.L    RD8, @(R15, 12)
  MOV.Q    RQ31, @(R15, 72)
  MOV.L    RD30, @(R15, 16)
  MOV.L    RD28, @(R15, 20)
  MOV      L00C00138, RQ4
  MOV      RD8, RD5
  MOV      RD30, RD6
  MOV      RD28, RD7
  MOV      RQ31, RQ20
  BSR      printf

L00E00478:
  ADD      #184, R15
  BRA      L00C00027

TKFAT_GetFatEntry:
  PUSH     LR
  BSR      L00C00044
  ADD      #-80, R15
  MOV      RQ4, RQ10
  MOV      RD5, RD8
  MOV      #0, RQ31
  CMPQ/EQ  RQ31, RQ10
  BT       L008005D1
  BRK     

L008005D1:
  MOV.B    @(RQ10, 13), RD14
  EXTU.B   RD14, RD14
  CMP/EQ   RD14
  BT       L008005D2
  MOV.L    @(RQ10, 24), RD14
  SHAD     RD8, #-8, RD12
  ADD      RD14, RD12, RD9
  MOV      RQ10, RQ4
  MOV      RD9, RD5
  MOV      #1, RD6
  BSR      TKFAT_GetSectorTempFatBuffer
  MOV      RQ2, RQ13
  EXTU.B   RD8, RD14
  SHAD     RD14, #1, RD12
  LEA.B    @(RQ13, RD12), RQ13
  MOVU.W   @RQ13, RD14
  MOV      RD14, RD11
  MOV      #0x0000FFF0, RD30
  CMP/GE   RD30, RD11
  BT       L008005D3
  SHAD     RD11, #16, RD14
  SHAD     RD14, #-16, RD11

L008005D3:
  MOV      RD11, RD2
  BRA      L00E00479

L008005D2:
  MOV.L    @(RQ10, 24), RD14
  SHAD     RD8, #-11, RD12
  SHAD     RD12, #4, RD29
  ADD      RD14, RD29, RD9
  MOV.L    @(RQ10, 8184), RD14
  CMP/EQ   RD9, RD14
  BT       L008005D4
  MOV.Q    @(RQ10, 8176), RQ13
  AND      RD8, #2047, RD14
  SHAD     RD14, #2, RD12
  LEA.B    @(RQ13, RD12), RQ13
  BRA      L008005D5

L008005D4:
  MOV      RQ10, RQ4
  MOV      RD9, RD5
  MOV      #16, RD6
  BSR      TKFAT_GetSectorTempFatBuffer
  MOV      RQ2, RQ13
  MOV.L    RD9, @(RQ10, 8184)
  MOV.Q    RQ13, @(RQ10, 8176)
  AND      RD8, #2047, RD14
  SHAD     RD14, #2, RD12
  LEA.B    @(RQ13, RD12), RQ13

L008005D5:
  MOVU.L   @RQ13, RD14
  MOV      RD14, RD11
  MOV      #0x0FFFFFF0, RD28
  MOV      #4095, RD28
  CMP/GE   RD28, RD11
  BT       L008005D6
  SHAD     RD11, #4, RD14
  SHAD     RD14, #-4, RD11

L008005D6:
  MOV      RD11, RD2

L00E00479:
  ADD      #80, R15
  BRA      L00C00045

TKFAT_SetFatEntry:
  PUSH     LR
  BSR      L00C00053
  ADD      #-72, R15
  MOV      RQ4, RQ10
  MOV      RD5, RD9
  MOV.L    RD6, @(R15, 16)
  MOV.B    @(RQ10, 13), RD14
  EXTU.B   RD14, RD14
  CMP/EQ   RD14
  BT       L008005D7
  MOV.L    @(RQ10, 24), RD14
  SHAD     RD9, #-8, RD13
  ADD      RD14, RD13, RD8
  MOV.L    @(RQ10, 28), RD14
  SHAD     RD9, #-8, RD13
  ADD      RD14, RD13, RD31
  MOV.L    RD8, @(R15, 12)
  MOV.L    RD31, @(R15, 8)
  MOV      RQ10, RQ4
  MOV      RD8, RD5
  MOV      #513, RD6
  BSR      TKFAT_GetSectorTempBuffer
  MOV      RQ2, RQ12
  EXTU.B   RD9, RD14
  SHAD     RD14, #1, RD13
  LEA.B    @(RQ12, RD13), RQ12
  MOV      RQ10, RQ4
  MOV      RD31, RD5
  MOV      #513, RD6
  BSR      TKFAT_GetSectorTempBuffer
  MOV      RQ2, RQ11
  EXTU.B   RD9, RD14
  SHAD     RD14, #1, RD13
  LEA.B    @(RQ11, RD13), RQ11
  MOV.L    @(R15, 16), RD30
  MOVU.B   RD30, @RQ12
  SHAD     RD30, #-8, RD14
  MOVU.B   RD14, @(RQ12, 1)
  MOVU.B   RD30, @RQ11
  SHAD     RD30, #-8, RD14
  MOVU.B   RD14, @(RQ11, 1)
  MOV      #0, RD2
  BRA      L00E0047A

L008005D7:
  MOV.L    @(RQ10, 24), RD14
  SHAD     RD9, #-7, RD13
  ADD      RD14, RD13, RD29
  MOV.L    @(RQ10, 28), RD14
  SHAD     RD9, #-7, RD13
  ADD      RD14, RD13, RD28
  MOV.L    RD29, @(R15, 12)
  MOV.L    RD28, @(R15, 8)
  MOV      RQ10, RQ4
  MOV      RD29, RD5
  MOV      #513, RD6
  BSR      TKFAT_GetSectorTempFatBuffer
  MOV      RQ2, RQ12
  AND      RD9, #127, RD14
  SHAD     RD14, #2, RD13
  LEA.B    @(RQ12, RD13), RQ12
  MOV      RQ10, RQ4
  MOV      RD28, RD5
  MOV      #513, RD6
  BSR      TKFAT_GetSectorTempFatBuffer
  MOV      RQ2, RQ11
  AND      RD9, #127, RD14
  SHAD     RD14, #2, RD13
  LEA.B    @(RQ11, RD13), RQ11
  MOV.L    @(R15, 16), RD27
  MOVU.B   RD27, @RQ12
  SHAD     RD27, #-8, RD14
  MOVU.B   RD14, @(RQ12, 1)
  SHAD     RD27, #-16, RD14
  MOVU.B   RD14, @(RQ12, 2)
  SHAD     RD27, #-24, RD14
  MOVU.B   RD14, @(RQ12, 3)
  MOVU.B   RD27, @RQ11
  SHAD     RD27, #-8, RD14
  MOVU.B   RD14, @(RQ11, 1)
  SHAD     RD27, #-16, RD14
  MOVU.B   RD14, @(RQ11, 2)
  SHAD     RD27, #-24, RD14
  MOVU.B   RD14, @(RQ11, 3)
  MOV      #0, RD2

L00E0047A:
  ADD      #72, R15
  BRA      L00C00054

TKFAT_GetClusterLBA:
  PUSH     LR
  BSR      L00C00037
  ADD      #-120, R15
  MOV      RQ4, RQ12
  MOV      RD5, RD9
  MOV.L    @(RQ12, 36), RD13
  SUB      RD9, #2, RD14
  MOV.B    @(RQ12, 14), RD11
  EXTU.B   RD11, RD11
  MUL.L    RD14, RD11, RD10
  ADD      RD13, RD10, RD14
  MOV      RD14, RD2

L00E0047B:
  ADD      #120, R15
  BRA      L00C00038

TKFAT_ReadImageFAT:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-168, R15
  MOV      RQ4, RQ12
  MOV.L    @(RQ12, 16), RD14
  MOV      RQ12, RQ4
  MOV      RD14, RD5
  MOV      #1, RD6
  BSR      TKFAT_GetSectorStaticBuffer
  MOV      RQ2, RQ13
  MOV.Q    RQ13, @(RQ12, 56)
  MOV.Q    @(RQ12, 56), RQ13
  MOV.Q    RQ13, @(RQ12, 64)
  MOV.Q    @(RQ12, 64), RQ13
  ADD      RQ13, #19, RQ11
  MOVU.W   @RQ11, RD14
  MOV      RD14, RD10
  MOV.Q    @(RQ12, 64), RQ13
  ADD      RQ13, #32, RQ11
  MOVU.L   @RQ11, RD14
  MOV      RD14, RD9
  MOV.Q    @(RQ12, 64), RQ13
  ADD      RQ13, #22, RQ11
  MOVU.W   @RQ11, RD14
  MOV      RD14, RD8
  MOV.Q    @(RQ12, 64), RQ13
  ADD      RQ13, #36, RQ11
  MOVU.L   @RQ11, RD14
  MOV      RD14, RD31
  MOV.Q    @(RQ12, 64), RQ13
  ADD      RQ13, #14, RQ11
  MOVU.W   @RQ11, RD14
  MOV      RD14, RD30
  MOV.Q    @(RQ12, 64), RQ13
  ADD      RQ13, #17, RQ11
  MOVU.W   @RQ11, RD14
  MOV      RD14, RD29
  MOV.Q    @(RQ12, 64), RQ13
  MOV.B    @(RQ13, 13), RD28
  EXTU.B   RD28, RD28
  MOV.Q    @(RQ12, 64), RQ13
  ADD      RQ13, #44, RQ11
  MOVU.L   @RQ11, RD14
  MOV      RD14, RD27
  MOV.Q    RQ11, @(R15, 40)
  MOV.L    RD10, @(R15, 76)
  MOV.L    RD9, @(R15, 72)
  MOV.L    RD8, @(R15, 68)
  MOV.L    RD31, @(R15, 64)
  MOV.L    RD30, @(R15, 80)
  MOV.L    RD29, @(R15, 96)
  MOV.L    RD28, @(R15, 88)
  MOV.L    RD27, @(R15, 92)
  CMP/EQ   RD10
  BT       L008005E2
  MOV.L    @(R15, 72), RD26
  CMP/EQ   #0, RD26
  BT       L008005E2
  BRA      L008005E3

L008005E3:
  MOV.Q    @(RQ12, 64), RQ13
  MOV.B    @(RQ13, 66), RD14
  EXTU.B   RD14, RD14
  CMP/EQ   #41, RD14
  BT       L008005E2
  BRA      L008005E4

L008005E4:
  MOV.L    @(R15, 76), RD25
  EXTU.L   RQ25, RQ25
  MOV      RQ25, RQ26
  MOV      #32, RD27
  SHAD.Q   RQ26, RD27, RQ26
  MOV      RQ26, RQ25
  MOV.L    @(R15, 72), RD26
  EXTU.L   RQ26, RQ26
  OR       RQ25, RQ26, RQ28
  MOV.L    @(R15, 68), RD25
  EXTU.L   RQ25, RQ25
  MOV      RQ25, RQ26
  SHAD.Q   RQ26, RD27, RQ26
  MOV      RQ26, RQ25
  MOV.L    @(R15, 64), RD26
  EXTU.L   RQ26, RQ26
  OR       RQ25, RQ26, RQ29
  MOV.Q    RQ29, @(R15, 112)
  MOV.Q    RQ28, @(R15, 120)
  MOV.Q    RQ26, @(R15, 24)
  MOV.Q    RQ25, @(R15, 32)
  BRA      L008005E5

L008005E2:
  MOV.L    @(R15, 76), RD7
  CMP/EQ   RD7
  BT       L008005E6
  MOV.L    @(R15, 76), RD6
  MOV      RD6, RD14
  BRA      L008005E7

L008005E6:
  MOV.L    @(R15, 72), RD5
  MOV      RD5, RD14

L008005E7:
  EXTU.L   RD14, RQ25
  MOV      RQ25, RQ26
  MOV.Q    RQ26, @(R15, 120)
  MOV.Q    RQ25, @(R15, 32)
  MOV.L    @(R15, 68), RD25
  CMP/EQ   #0, RD25
  BT       L008005E8
  MOV.L    @(R15, 68), RD4
  MOV      RD4, RD14
  BRA      L008005E9

L008005E8:
  MOV.L    @(R15, 64), RD3
  MOV      RD3, RD14

L008005E9:
  EXTU.L   RD14, RQ2
  MOV      RQ2, RQ23
  MOV.Q    RQ2, @(R15, 32)
  MOV.Q    RQ23, @(R15, 112)
  MOV.Q    @(R15, 120), RQ22
  MOV      #0, RQ21
  CMPQ/EQ  RQ21, RQ22
  BT       L008005EA
  MOV.Q    @(RQ12, 64), RQ13
  ADD      RQ13, #82, RQ20
  ADD      RQ20, #4, RQ13
  MOVU.L   @RQ13, RD14
  MOV      RD14, RD19
  MOV.Q    @(RQ12, 64), RQ13
  ADD      RQ13, #82, RQ20
  MOVU.L   @RQ20, RD14
  MOV      RD14, RD18
  EXTU.L   RD19, RQ17
  MOV      RQ17, RQ16
  MOV      #32, RD21
  SHAD.Q   RQ16, RD21, RQ16
  MOV      RQ16, RQ17
  EXTU.L   RD18, RQ16
  OR       RQ17, RQ16, RQ22
  MOV.Q    RQ22, @(R15, 120)
  MOV.Q    RQ20, @(R15, 40)
  MOV.L    RD19, @(R15, 76)
  MOV.L    RD18, @(R15, 72)
  MOV.Q    RQ17, @(R15, 32)
  MOV.Q    RQ16, @(R15, 24)

L008005EA:

L008005E5:
  MOV      #0, RD16
  MOV.L    RD16, @(R15, 100)
  MOV.L    @(R15, 96), RD16
  CMP/EQ   #0, RD16
  BT       L008005EB
  MOV.L    @(R15, 96), RD16
  SHAD     RD16, #5, RD14
  ADD      RD14, #511, RD17
  SHAD     RD17, #-9, RD18
  MOV.L    RD18, @(R15, 100)
  MOV.L    RD17, @(R15, 20)

L008005EB:
  MOV.L    @(R15, 88), RD16
  CMP/EQ   #0, RD16
  BT       L008005EC
  MOV.L    @(R15, 88), RD16
  SUB      RD16, #1, RD14
  TST      RD14, RD16
  BT       L008005EC
  BRA      L008005ED

L008005EC:
  MOV      L00C0013D, RQ4
  MOV.L    @(R15, 88), RD5
  BSR      printf
  BRA      L00E0047C
  BRA      L008005EE

L008005ED:
  MOV.L    @(R15, 88), RD17
  MOV      RD17, RD16
  MOV      #0, RD18
  MOV.L    RD18, @(R15, 84)
  MOV.L    RD16, @(R15, 60)

L008005EF:
  MOV.L    @(R15, 60), RD16
  CMP/GT   #1, RD16
  BT       L008005F0
  MOV.L    @(R15, 60), RD16
  SHAD     #-1, RD16
  MOV.L    @(R15, 84), RD17
  ADD      #1, RD17
  MOV.L    RD17, @(R15, 84)
  MOV.L    RD16, @(R15, 60)
  BRA      L008005EF

L008005F0:

L008005EE:
  MOV.Q    @(RQ12, 64), RQ13
  MOV.B    @(RQ13, 66), RD14
  EXTU.B   RD14, RD14
  CMP/EQ   #41, RD14
  BT       L008005F1
  MOV.L    @(R15, 92), RD16
  CMP/EQ   #0, RD16
  BT       L008005F1
  BRA      L008005F2

L008005F1:
  MOV      #1, RD16
  MOV.L    RD16, @(R15, 92)

L008005F2:
  MOV.L    @(RQ12, 16), RD14
  MOV.L    @(R15, 80), RD26
  ADD      RD14, RD26, RD25
  MOV.L    RD25, @(RQ12, 24)
  MOV.L    @(RQ12, 16), RD14
  ADD      RD14, RD26, RD25
  EXTS.L   RD25, RQ27
  MOV.Q    @(R15, 112), RQ28
  ADD      RQ27, RQ28, RQ29
  EXTS.L   RQ29, RD14
  MOV.L    RD14, @(RQ12, 28)
  MOV.L    @(RQ12, 16), RD14
  ADD      RD14, RD26, RD25
  MOV      RQ28, RQ29
  MOV      #2, RQ5
  MOV      RQ29, RQ4
  MOV      __smulsq, R3
  JSR      R3
  MOV      RQ2, RQ29
  EXTS.L   RD25, RQ30
  ADD      RQ30, RQ29, RQ27
  EXTS.L   RQ27, RD14
  MOV.L    RD14, @(RQ12, 32)
  MOV.L    @(RQ12, 32), RD14
  MOV.L    @(R15, 100), RD31
  ADD      RD14, RD31, RD25
  MOV.L    RD25, @(RQ12, 36)
  MOV.L    @(R15, 88), RD8
  EXTU.B   RD8, RD8
  MOV.B    RD8, @(RQ12, 14)
  MOV.L    @(R15, 84), RD9
  ADD      RD9, #9, RD14
  EXTU.B   RD14, RD14
  MOV.B    RD14, @(RQ12, 15)
  MOV.L    @(RQ12, 16), RD14
  EXTS.L   RD14, RQ27
  MOV.Q    @(R15, 120), RQ10
  ADD      RQ27, RQ10, RQ29
  MOV.L    @(RQ12, 36), RD14
  EXTS.L   RD14, RQ30
  SUB      RQ29, RQ30, RQ27
  MOV      RQ27, RQ29
  EXTS.L   RD9, RQ27
  SHAR.Q   RQ29, RQ27, RQ11
  EXTS.L   RQ11, RD14
  MOV.L    RD14, @(RQ12, 40)
  MOV.L    @(R15, 92), RD28
  MOV.L    RD28, @(RQ12, 44)
  MOV      #0, RD10
  EXTU.B   RD10, RD10
  MOV.B    RD10, @(RQ12, 13)
  MOV.Q    RQ11, @(R15, 104)
  MOV.Q    RQ29, @(R15, 32)
  MOV.Q    RQ27, @(R15, 24)
  MOV.L    RD25, @(R15, 20)
  MOV      #0x0000FFF5, RQ10
  CMPQ/GT  RQ10, RQ11
  BT       L008005F3
  MOV      #1, RD16
  EXTU.B   RD16, RD16
  MOV.B    RD16, @(RQ12, 13)

L008005F3:
  MOV.B    @(RQ12, 13), RD16
  EXTU.B   RD16, RD16
  MOV.L    RD16, @(R15, 20)
  CMP/EQ   #0, RD16
  BT       L008005F4
  MOV      #16, RD14
  BRA      L008005F5

L008005F4:
  MOV      #32, RD14

L008005F5:
  MOV      L00C0013F, RQ4
  MOV      RD14, RD5
  BSR      printf
  MOV.L    @(RQ12, 24), RD14
  SHAD     RD14, #9, RD25
  MOV.L    @(RQ12, 24), RD14
  MOV.L    RD25, @(R15, 20)
  MOV      L00C00140, RQ4
  MOV      RD14, RD5
  MOV      RD25, RD6
  BSR      printf
  MOV.L    @(RQ12, 28), RD14
  SHAD     RD14, #9, RD25
  MOV.L    @(RQ12, 28), RD14
  MOV.L    RD25, @(R15, 20)
  MOV      L00C00141, RQ4
  MOV      RD14, RD5
  MOV      RD25, RD6
  BSR      printf
  MOV.L    @(RQ12, 32), RD14
  SHAD     RD14, #9, RD25
  MOV.L    @(RQ12, 32), RD14
  MOV.L    RD25, @(R15, 20)
  MOV      L00C00142, RQ4
  MOV      RD14, RD5
  MOV      RD25, RD6
  BSR      printf
  MOV.L    @(RQ12, 36), RD14
  SHAD     RD14, #9, RD25
  MOV.L    @(RQ12, 36), RD14
  MOV.L    RD25, @(R15, 20)
  MOV      L00C00143, RQ4
  MOV      RD14, RD5
  MOV      RD25, RD6
  BSR      printf
  MOV.L    @(R15, 88), RD25
  SHAD     RD25, #9, RD14
  MOV      L00C00144, RQ4
  MOV      RD25, RD5
  MOV      RD14, RD6
  BSR      printf
  MOV.L    @(RQ12, 40), RD14
  MOV      L00C00145, RQ4
  MOV      RD14, RD5
  BSR      printf
  MOV.L    @(RQ12, 44), RD14
  MOV      L00C00146, RQ4
  MOV      RD14, RD5
  BSR      printf

L00E0047C:
  ADD      #168, R15
  BRA      L00C00027

TKFAT_AllocFreeCluster:
  PUSH     LR
  BSR      L00C00072
  ADD      #-112, R15
  MOV      RQ4, RQ12
  MOV.L    @(RQ12, 40), RD13
  MOV.L    @(RQ12, 8188), RD14
  MOV      #2, RD10
  CMP/GE   RD10, RD14
  BT       L008005F6
  CMP/GT   RD14, RD13
  BT       L008005F6
  BRA      L008005F7

L008005F6:
  MOV      #2, RD14

L008005F7:

L008005F8:
  CMP/GT   RD14, RD13
  BT       L008005F9
  MOV      RQ12, RQ4
  MOV      RD14, RD5
  BSR      TKFAT_GetFatEntry
  MOV      RD2, RD11
  CMP/EQ   RD11
  BT       L008005FA
  MOV      RQ12, RQ4
  MOV      RD14, RD5
  MOV      #0x0FFFFFFF, RD6
  MOV      #4095, RD6
  BSR      TKFAT_SetFatEntry
  MOV      RD14, RD2
  BRA      L00E0047D

L008005FA:

L008005FB:
  ADD      #1, RD14
  BRA      L008005F8

L008005F9:
  MOV      #-1, RD2

L00E0047D:
  ADD      #112, R15
  BRA      L00C00074

TKFAT_GetWalkCluster:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-262280, R15
  MOV      RQ4, RQ13
  MOV.L    RD5, @(R15, 100)
  MOV      RD6, RD10
  MOV.L    RD7, @(R15, 92)
  MOV.L    @(R15, 100), RD8
  CMP/EQ   RD8
  BT       L008005FC
  MOV      #-1, RD2
  BRA      L00E0047E

L008005FC:
  MOV      #0, RQ31
  CMPQ/EQ  RQ31, RQ13
  BT       L008005FD
  BRK     

L008005FD:
  MOV.L    @(RQ13, 8192), RD14
  MOV.L    @(R15, 100), RD30
  CMP/EQ   RD30, RD14
  BT       L008005FE
  MOV.L    @(RQ13, 8196), RD14
  CMP/GT   RD10, RD14
  BT       L008005FE
  BRA      L008005FF

L008005FF:
  MOV.L    @(RQ13, 8200), RD9
  MOV.L    @(RQ13, 8196), RD11
  SUB      RD10, RD11, RD29
  MOV.L    RD29, @(R15, 88)
  CMP/GT   #192, RD29
  BT       L00800600
  MOV.L    @(RQ13, 270348), RD14
  CMP/GT   RD14, RD10
  BT       L00800601
  SHAD     RD10, #-7, RD12
  ADD      RQ13, #8204, RQ28
  MOV.L    @(RQ28, RD12), RD9
  SHAD     RD12, #7, RD11
  SUB      RD10, RD11, RD27
  MOV.Q    RQ28, @(R15, 72)
  MOV.L    RD27, @(R15, 88)
  BRA      L00800602

L00800601:
  MOV.L    @(RQ13, 270348), RD14
  CMP/PL   RD14
  BT       L00800603
  MOV.L    @(RQ13, 270348), RD14
  SHAD     RD14, #-7, RD12
  ADD      RQ13, #8204, RQ26
  MOV.L    @(RQ26, RD12), RD9
  SHAD     RD12, #7, RD11
  SUB      RD10, RD11, RD25
  MOV.Q    RQ26, @(R15, 72)
  MOV.L    RD25, @(R15, 88)

L00800603:

L00800602:

L00800600:
  BRA      L00800604

L008005FE:
  MOV.L    @(RQ13, 8192), RD14
  MOV.L    @(R15, 100), RD7
  CMP/EQ   RD7, RD14
  BT       L00800605
  MOV.L    @(RQ13, 270348), RD14
  CMP/GT   RD14, RD10
  BT       L00800606
  SHAD     RD10, #-7, RD12
  ADD      RQ13, #8204, RQ6
  MOV.L    @(RQ6, RD12), RD9
  SHAD     RD12, #7, RD11
  SUB      RD10, RD11, RD5
  MOV.Q    RQ6, @(R15, 72)
  MOV.L    RD5, @(R15, 88)
  BRA      L00800607

L00800606:
  MOV.L    @(RQ13, 270348), RD14
  CMP/PL   RD14
  BT       L00800608
  MOV.L    @(RQ13, 270348), RD14
  SHAD     RD14, #-7, RD12
  ADD      RQ13, #8204, RQ4
  MOV.L    @(RQ4, RD12), RD9
  SHAD     RD12, #7, RD11
  SUB      RD10, RD11, RD3
  MOV.Q    RQ4, @(R15, 72)
  MOV.L    RD3, @(R15, 88)
  BRA      L00800609

L00800608:
  MOV.L    @(R15, 100), RD2
  MOV      RD2, RD9
  MOV      RD10, RD23
  MOV      #0, RD11
  MOV.L    RD23, @(R15, 88)

L00800609:

L00800607:
  BRA      L0080060A

L00800605:
  MOV      #-1, RD22
  MOV.L    RD22, @(RQ13, 270348)
  MOV.L    @(R15, 100), RD21
  MOV      RD21, RD9
  MOV      RD10, RD20
  MOV      #0, RD11
  MOV.L    RD20, @(R15, 88)

L0080060A:

L00800604:

L0080060B:
  MOV.L    @(R15, 88), RD19
  CMP/GT   #0, RD19
  BT       L0080060C
  MOV      RQ13, RQ4
  MOV      RD9, RD5
  BSR      TKFAT_GetFatEntry
  MOV      RD2, RD12
  CMP/PZ   RD12
  BT       L0080060D
  MOV.L    @(R15, 92), RD18
  CMP/EQ   #0, RD18
  BT       L0080060E
  MOV      #-1, RD2
  BRA      L00E0047E

L0080060E:
  MOV      RQ13, RQ4
  BSR      TKFAT_AllocFreeCluster
  MOV      RD2, RD12
  CMP/PL   RD12
  BT       L0080060F
  MOV      #-1, RD2
  BRA      L00E0047E

L0080060F:
  MOV      RQ13, RQ4
  MOV      RD9, RD5
  MOV      RD12, RD6
  BSR      TKFAT_SetFatEntry

L0080060D:
  TST      #127, RD11
  BT       L00800610
  MOV.L    @(RQ13, 270348), RD14
  CMP/GT   RD14, RD11
  BT       L00800610
  BRA      L00800611

L00800611:
  ADD      RQ13, #8204, RQ17
  SHAD     RD11, #-7, RD14
  MOV.L    RD9, @(RQ17, RD14)
  MOV.L    RD11, @(RQ13, 270348)
  MOV.Q    RQ17, @(R15, 72)

L00800610:
  MOV      RD12, RD9
  MOV.L    @(R15, 88), RD16
  ADD      #-1, RD16
  ADD      #1, RD11
  MOV.L    RD16, @(R15, 88)
  BRA      L0080060B

L0080060C:
  MOV.L    @(R15, 100), RD16
  MOV.L    RD16, @(RQ13, 8192)
  MOV.L    RD10, @(RQ13, 8196)
  MOV.L    RD9, @(RQ13, 8200)
  MOV      RD9, RD2

L00E0047E:
  ADD      #262280, R15
  BRA      L00C00027

TKFAT_GetClusterFileOffs:
  PUSH     LR
  BSR      L00C00049
  ADD      #-120, R15
  MOV      RQ4, RQ12
  MOV.L    RD5, @(R15, 52)
  MOV.L    RD6, @(R15, 48)
  MOV.L    RD7, @(R15, 44)
  MOV.Q    RQ20, @(R15, 32)
  MOV.Q    RQ21, @(R15, 24)
  MOV.B    @(RQ12, 15), RD14
  EXTU.B   RD14, RD14
  MOV.L    @(R15, 48), RD9
  SHAR     RD9, RD14, RD10
  MOV.B    @(RQ12, 15), RD14
  EXTU.B   RD14, RD14
  MOV      #1, RD8
  SHAD     RD8, RD14, RD11
  SUB      RD11, #1, RD14
  AND      RD9, RD14, RD31
  MOV.L    RD31, @(R15, 20)
  MOV      RQ12, RQ4
  MOV.L    @(R15, 52), RD5
  MOV      RD10, RD6
  MOV.L    @(R15, 44), RD7
  BSR      TKFAT_GetWalkCluster
  MOV      RD2, RD13
  CMP/PZ   RD13
  BT       L00800612
  MOV      #-1, RD2
  BRA      L00E0047F

L00800612:
  MOV.Q    @(R15, 32), RQ30
  MOV.L    RD13, @RQ30
  MOV.Q    @(R15, 24), RQ29
  MOV.L    @(R15, 20), RD7
  MOV.L    RD7, @RQ29
  MOV      #0, RD2

L00E0047F:
  ADD      #120, R15
  BRA      L00C0004A

TKFAT_ReadWriteSector:
  PUSH     LR
  BSR      L00C0003D
  ADD      #-144, R15
  MOV.Q    RQ4, @(R15, 56)
  MOV.L    RD5, @(R15, 52)
  MOV.L    RD6, @(R15, 48)
  MOV.L    RD7, @(R15, 44)
  MOV.Q    RQ20, @(R15, 32)
  MOV.L    RD21, @(R15, 28)
  MOV.L    @(R15, 44), RD13
  CMP/EQ   RD13
  BT       L00800613
  MOV.Q    @(R15, 56), RQ4
  MOV.L    @(R15, 52), RD5
  MOV      #513, RD6
  BSR      TKFAT_GetSectorTempBuffer
  MOV      RQ2, RQ14
  MOV.L    @(R15, 48), RD12
  LEA.B    @(RQ14, RD12), RQ11
  MOV.L    @(R15, 28), RD10
  EXTS.L   RQ10, RQ10
  MOV.Q    RQ11, @(R15, 16)
  MOV.Q    RQ10, @(R15, 8)
  MOV      RQ11, RQ4
  MOV.Q    @(R15, 32), RQ5
  MOV      RQ10, RQ6
  BSR      memcpy
  BRA      L00800614

L00800613:
  MOV.Q    @(R15, 56), RQ4
  MOV.L    @(R15, 52), RD5
  MOV      #1, RD6
  BSR      TKFAT_GetSectorTempBuffer
  MOV      RQ2, RQ14
  MOV.L    @(R15, 48), RD9
  LEA.B    @(RQ14, RD9), RQ8
  MOV.L    @(R15, 28), RD10
  EXTS.L   RQ10, RQ10
  MOV.Q    RQ10, @(R15, 8)
  MOV.Q    RQ8, @(R15, 16)
  MOV.Q    @(R15, 32), RQ4
  MOV      RQ8, RQ5
  MOV      RQ10, RQ6
  BSR      memcpy

L00800614:
  MOV      #0, RD2

L00E00480:
  ADD      #144, R15
  BRA      L00C0003E

TKFAT_ReadWriteCluster:
  PUSH     LR
  BSR      L00C00044
  ADD      #-112, R15
  MOV      RQ4, RQ12
  MOV.L    RD5, @(R15, 52)
  MOV.L    RD6, @(R15, 48)
  MOV.L    RD7, @(R15, 44)
  MOV.Q    RQ20, @(R15, 32)
  MOV.L    RD21, @(R15, 28)
  MOV      RQ12, RQ4
  MOV.L    @(R15, 52), RD5
  BSR      TKFAT_GetClusterLBA
  MOV      RD2, RD13
  MOV.L    @(R15, 44), RD10
  CMP/EQ   RD10
  BT       L00800615
  MOV.B    @(RQ12, 14), RD11
  EXTU.B   RD11, RD11
  OR       RD11, #512, RD9
  MOV.L    RD9, @(R15, 20)
  MOV      RQ12, RQ4
  MOV      RD13, RD5
  MOV      RD9, RD6
  BSR      TKFAT_GetSectorTempBuffer
  MOV      RQ2, RQ14
  MOV.L    @(R15, 48), RD8
  LEA.B    @(RQ14, RD8), RQ31
  MOV.L    @(R15, 28), RD30
  EXTS.L   RQ30, RQ30
  MOV.Q    RQ31, @(R15, 8)
  MOV.Q    RQ30, @R15
  MOV      RQ31, RQ4
  MOV.Q    @(R15, 32), RQ5
  MOV      RQ30, RQ6
  BSR      memcpy
  BRA      L00800616

L00800615:
  MOV.B    @(RQ12, 14), RD11
  EXTU.B   RD11, RD11
  MOV      RQ12, RQ4
  MOV      RD13, RD5
  MOV      RD11, RD6
  BSR      TKFAT_GetSectorTempBuffer
  MOV      RQ2, RQ14
  MOV.L    @(R15, 48), RD29
  EXTS.L   RD29
  LEA.B    @(RQ14, RD29), RQ28
  MOV.L    @(R15, 28), RD30
  EXTS.L   RQ30, RQ30
  MOV.Q    RQ30, @R15
  MOV.Q    RQ28, @(R15, 8)
  MOV.Q    @(R15, 32), RQ4
  MOV      RQ28, RQ5
  MOV      RQ30, RQ6
  BSR      memcpy

L00800616:
  MOV      #0, RD2

L00E00481:
  ADD      #112, R15
  BRA      L00C00045

TKFAT_ReadWriteClusterOffset:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-136, R15
  MOV      RQ4, RQ12
  MOV.L    RD5, @(R15, 100)
  MOV.L    RD6, @(R15, 96)
  MOV.L    RD7, @(R15, 92)
  MOV.Q    RQ20, @(R15, 80)
  MOV.L    RD21, @(R15, 76)
  MOV      #0, RQ31
  CMPQ/EQ  RQ31, RQ12
  BT       L00800617
  BRK     

L00800617:
  MOV.L    @(R15, 76), RD30
  SUB      RD30, #1, RD14
  MOV.L    @(R15, 96), RD28
  ADD      RD28, RD14, RD29
  MOV.L    RD29, @(R15, 48)
  MOV.B    @(RQ12, 15), RD14
  EXTU.B   RD14, RD14
  SHAR     RD28, RD14, RD27
  MOV.B    @(RQ12, 15), RD14
  EXTU.B   RD14, RD14
  MOV.L    @(R15, 48), RD25
  SHAR     RD25, RD14, RD26
  MOV.L    RD27, @(R15, 32)
  MOV.L    RD26, @(R15, 28)
  CMP/EQ   RD26, RD27
  BT       L00800618
  MOV      #52, RQ9
  ADD      R15, RQ9
  MOV      #60, RQ8
  ADD      R15, RQ8
  MOV      RQ12, RQ4
  MOV.L    @(R15, 100), RD5
  MOV.L    @(R15, 96), RD6
  MOV.L    @(R15, 92), RD7
  MOV      RQ8, RQ20
  MOV      RQ9, RQ21
  BSR      TKFAT_GetClusterFileOffs
  MOV      RD2, RD13
  CMP/PZ   RD13
  BT       L00800619
  MOV      RD13, RD2
  BRA      L00E00482

L00800619:
  MOV      RQ12, RQ4
  MOV.L    @(R15, 60), RD5
  MOV.L    @(R15, 52), RD6
  MOV.L    @(R15, 92), RD7
  MOV.Q    @(R15, 80), RQ20
  MOV.L    @(R15, 76), RD21
  BSR      TKFAT_ReadWriteCluster
  MOV      RD2, RD13
  MOV      RD13, RD2
  BRA      L00E00482

L00800618:
  MOV      #52, RQ9
  ADD      R15, RQ9
  MOV      #60, RQ8
  ADD      R15, RQ8
  MOV      RQ12, RQ4
  MOV.L    @(R15, 100), RD5
  MOV.L    @(R15, 96), RD6
  MOV.L    @(R15, 92), RD7
  MOV      RQ8, RQ20
  MOV      RQ9, RQ21
  BSR      TKFAT_GetClusterFileOffs
  MOV      RD2, RD13
  MOV      #48, RQ9
  ADD      R15, RQ9
  MOV      #56, RQ8
  ADD      R15, RQ8
  MOV.L    @(R15, 76), RD25
  SUB      RD25, #1, RD14
  MOV.L    @(R15, 96), RD27
  ADD      RD27, RD14, RD26
  MOV.L    RD26, @(R15, 32)
  MOV      RQ12, RQ4
  MOV.L    @(R15, 100), RD5
  MOV      RD26, RD6
  MOV.L    @(R15, 92), RD7
  MOV      RQ8, RQ20
  MOV      RQ9, RQ21
  BSR      TKFAT_GetClusterFileOffs
  MOV      RD2, RD28
  MOV.L    RD28, @(R15, 40)
  CMP/PZ   RD13
  BT       L0080061A
  MOV      RD13, RD2
  BRA      L00E00482

L0080061A:
  MOV.L    @(R15, 40), RD7
  CMP/PZ   RD7
  BT       L0080061B
  MOV.L    @(R15, 40), RD2
  BRA      L00E00482

L0080061B:
  MOV.B    @(RQ12, 15), RD14
  EXTU.B   RD14, RD14
  MOV      #1, RD26
  SHAD     RD26, RD14, RD25
  MOV.Q    @(R15, 80), RQ27
  MOV      RQ27, RQ10
  MOV.L    @(R15, 76), RD28
  EXTS.L   RD28
  LEA.B    @(RQ27, RD28), RQ29
  MOV.L    @(R15, 60), RD30
  MOV      RD30, RD11
  MOV.L    @(R15, 52), RD31
  SUB      RD25, RD31, RD14
  MOV.Q    RQ29, @(R15, 64)
  MOV.L    RD25, @(R15, 44)
  MOV      RQ12, RQ4
  MOV      RD11, RD5
  MOV.L    @(R15, 52), RD6
  MOV.L    @(R15, 92), RD7
  MOV      RQ10, RQ20
  MOV      RD14, RD21
  BSR      TKFAT_ReadWriteCluster
  MOV      RQ12, RQ4
  MOV      RD11, RD5
  MOV      RD26, RD6
  MOV.L    @(R15, 92), RD7
  BSR      TKFAT_GetWalkCluster
  MOV      RD2, RD11
  MOV.L    @(R15, 52), RD26
  SUB      RD25, RD26, RD14
  EXTS.L   RD14
  LEA.B    @(RQ10, RD14), RQ10

L0080061C:
  MOV.L    @(R15, 44), RD6
  LEA.B    @(RQ10, RD6), RQ9
  MOV.Q    @(R15, 64), RQ5
  CMPQ/GT  RQ5, RQ9
  BT       L0080061D
  MOV      RQ12, RQ4
  MOV      RD11, RD5
  MOV      #0, RD6
  MOV.L    @(R15, 92), RD7
  MOV      RQ10, RQ20
  MOV.L    @(R15, 44), RD21
  BSR      TKFAT_ReadWriteCluster
  MOV      RQ12, RQ4
  MOV      RD11, RD5
  MOV      #1, RD6
  MOV.L    @(R15, 92), RD7
  BSR      TKFAT_GetWalkCluster
  MOV      RD2, RD11
  CMP/PZ   RD11
  BT       L0080061E
  MOV      #-1, RD2
  BRA      L00E00482

L0080061E:
  MOV.L    @(R15, 44), RD4
  LEA.B    @(RQ10, RD4), RQ10
  BRA      L0080061C

L0080061D:
  MOV.Q    @(R15, 64), RQ3
  CMPQ/GT  RQ10, RQ3
  BT       L0080061F
  MOV.Q    @(R15, 64), RQ25
  SUB      RQ25, RQ10, RD14
  MOV      RQ12, RQ4
  MOV      RD11, RD5
  MOV      #0, RD6
  MOV.L    @(R15, 92), RD7
  MOV      RQ10, RQ20
  MOV      RD14, RD21
  BSR      TKFAT_ReadWriteCluster

L0080061F:
  MOV      #0, RD2

L00E00482:
  ADD      #136, R15
  BRA      L00C00027

TKFAT_ReadWriteDirEntOffset:
  PUSH     LR
  BSR      L00C00053
  ADD      #-120, R15
  MOV      RQ4, RQ13
  MOV      RD5, RD12
  MOV      RD6, RD10
  MOV.L    RD7, @(R15, 52)
  MOV.Q    RQ20, @(R15, 40)
  CMP/PL   RD12
  BT       L00800620
  MOV      #-1, RD2
  BRA      L00E00483

L00800620:
  MOV      #0, RQ8
  CMPQ/EQ  RQ8, RQ13
  BT       L00800621
  BRK     

L00800621:
  CMP/EQ   #1, RD12
  BT       L00800622
  CMP/PZ   RD10
  BT       L00800623
  MOV      #-1, RD2
  BRA      L00E00483

L00800623:
  MOV.Q    @(RQ13, 56), RQ11
  ADD      RQ11, #17, RQ31
  MOVU.W   @RQ31, RD14
  MOV      RD14, RD30
  MOV.L    RD30, @(R15, 36)
  CMP/GT   RD10, RD30
  BT       L00800624
  MOV      #-1, RD2
  BRA      L00E00483

L00800624:
  AND      RD10, #15, RD14
  SHAD     RD14, #5, RD29
  MOV.L    @(RQ13, 32), RD14
  SHAD     RD10, #-4, RD28
  ADD      RD14, RD28, RD27
  MOV.L    RD29, @(R15, 8)
  MOV.L    RD27, @R15
  MOV      RQ13, RQ4
  MOV      RD27, RD5
  MOV      RD29, RD6
  MOV.L    @(R15, 52), RD7
  MOV.Q    @(R15, 40), RQ20
  MOV      #32, RD21
  BSR      TKFAT_ReadWriteSector
  MOV      #0, RD2
  BRA      L00E00483

L00800622:
  SHAD     RD10, #5, RD14
  MOV      RQ13, RQ4
  MOV      RD12, RD5
  MOV      RD14, RD6
  MOV.L    @(R15, 52), RD7
  MOV.Q    @(R15, 40), RQ20
  MOV      #32, RD21
  BSR      TKFAT_ReadWriteClusterOffset
  MOV      RD2, RD9
  MOV      RD9, RD2

L00E00483:
  ADD      #120, R15
  BRA      L00C00054

tkfat_name2sfn:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-56, R15
  MOV.Q    RQ4, @(R15, 24)
  MOV.Q    RQ5, @(R15, 16)
  MOV      #0, RD12

L00800625:
  MOV      #11, RD8
  CMP/GE   RD8, RD12
  BT       L00800626
  MOV.Q    @(R15, 16), RQ31
  MOV      #32, RD30
  EXTS.L   RD12
  MOV.B    RD30, @(RQ31, RD12)

L00800627:
  ADD      #1, RD12
  BRA      L00800625

L00800626:
  MOV.Q    @(R15, 24), RQ29
  MOV      RQ29, RQ13
  MOV.Q    @(R15, 16), RQ28
  MOV      RQ28, RQ10
  MOV      #0, RD12

L00800628:
  MOV      #8, RD27
  CMP/GE   RD27, RD12
  BT       L00800629
  MOV.B    @RQ13, RD14
  CMP/EQ   RD14
  BT       L0080062A
  MOV.B    @RQ13, RD14
  CMP/EQ   #46, RD14
  BT       L0080062A
  BRA      L0080062B

L0080062A:
  BRA      L00800629

L0080062B:
  MOV      RQ13, RQ9
  ADD      #1, RQ13
  MOV.B    @RQ9, RD14
  MOV      RD14, RD11
  CMP/GE   #97, RD11
  BT       L0080062C
  CMP/GT   #122, RD11
  BT       L0080062C
  BRA      L0080062D

L0080062D:
  SUB      RD11, #97, RD14
  ADD      RD14, #65, RD11

L0080062C:
  MOV      RQ10, RQ9
  ADD      #1, RQ10
  MOV.B    RD11, @RQ9

L0080062E:
  ADD      #1, RD12
  BRA      L00800628

L00800629:
  MOV.B    @RQ13, RD14
  CMP/EQ   RD14
  BT       L0080062F
  MOV      #0, RD2
  BRA      L00E00484

L0080062F:
  MOV.B    @RQ13, RD14
  CMP/EQ   #46, RD14
  BT       L00800630
  MOV      #-1, RD2
  BRA      L00E00484

L00800630:
  ADD      #1, RQ13
  MOV.Q    @(R15, 16), RQ26
  ADD      RQ26, #8, RQ10
  MOV      #0, RD12

L00800631:
  MOV      #3, RD25
  CMP/GE   RD25, RD12
  BT       L00800632
  MOV.B    @RQ13, RD14
  CMP/EQ   RD14
  BT       L00800633
  MOV.B    @RQ13, RD14
  CMP/EQ   #46, RD14
  BT       L00800633
  BRA      L00800634

L00800633:
  BRA      L00800632

L00800634:
  MOV      RQ13, RQ9
  ADD      #1, RQ13
  MOV.B    @RQ9, RD14
  MOV      RD14, RD11
  CMP/GE   #97, RD11
  BT       L00800635
  CMP/GT   #122, RD11
  BT       L00800635
  BRA      L00800636

L00800636:
  SUB      RD11, #97, RD14
  ADD      RD14, #65, RD11

L00800635:
  MOV      RQ10, RQ9
  ADD      #1, RQ10
  MOV.B    RD11, @RQ9

L00800637:
  ADD      #1, RD12
  BRA      L00800631

L00800632:
  MOV.B    @RQ13, RD14
  CMP/EQ   RD14
  BT       L00800638
  MOV      #-1, RD2
  BRA      L00E00484

L00800638:
  MOV      #0, RD2

L00E00484:
  ADD      #56, R15
  BRA      L00C00027

tkfat_name2lfn:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-72, R15
  MOV.Q    RQ4, @(R15, 40)
  MOV.Q    RQ5, @(R15, 32)
  MOV      #0, RD10

L00800639:
  CMP/GE   #256, RD10
  BT       L0080063A
  MOV.Q    @(R15, 32), RQ31
  MOV      #0x0000FFFF, RD30
  EXTS.L   RD10
  MOVU.W   RD30, @(RQ31, RD10)

L0080063B:
  ADD      #1, RD10
  BRA      L00800639

L0080063A:
  MOV.Q    @(R15, 40), RQ29
  MOV      RQ29, RQ13
  MOV.Q    @(R15, 32), RQ28
  MOV      RQ28, RQ11
  ADD      RQ11, #512, RQ27
  MOV.Q    RQ27, @(R15, 24)

L0080063C:
  MOV.B    @RQ13, RD14
  CMP/EQ   RD14
  BT       L0080063D
  MOV.Q    @(R15, 24), RQ26
  CMPQ/GT  RQ11, RQ26
  BT       L0080063D
  BRA      L0080063E

L0080063E:
  MOV      RQ13, RQ8
  ADD      #1, RQ13
  MOV.B    @RQ8, RD14
  MOV      RD14, RD10
  TST      #128, RD10
  BT       L0080063F
  MOV      RQ11, RQ8
  ADD      #2, RQ11
  MOVU.W   RD10, @RQ8
  BRA      L0080063C

L0080063F:
  AND      RD10, #224, RD14
  CMP/EQ   #192, RD14
  BT       L00800640
  AND      RD10, #31, RD14
  SHAD     RD14, #6, RD12
  MOV      RQ13, RQ8
  ADD      #1, RQ13
  MOV.B    @RQ8, RD14
  AND      RD14, #63, RD9
  OR       RD12, RD9, RD25
  MOV.L    RD25, @(R15, 20)
  BRA      L00800641

L00800640:
  AND      RD10, #240, RD14
  CMP/EQ   #224, RD14
  BT       L00800642
  AND      RD10, #15, RD14
  SHAD     RD14, #12, RD12
  MOV.B    @RQ13, RD14
  AND      RD14, #63, RD9
  SHAD     RD9, #6, RD14
  OR       RD12, RD14, RD9
  MOV.B    @(RQ13, 1), RD14
  AND      RD14, #63, RD12
  OR       RD9, RD12, RD7
  ADD      #2, RQ13
  MOV.L    RD7, @(R15, 20)
  BRA      L00800643

L00800642:
  AND      RD10, #248, RD14
  CMP/EQ   #240, RD14
  BT       L00800644
  AND      RD10, #7, RD14
  SHAD     RD14, #18, RD12
  MOV.B    @RQ13, RD14
  AND      RD14, #63, RD9
  SHAD     RD9, #12, RD14
  OR       RD12, RD14, RD9
  MOV.B    @(RQ13, 1), RD14
  AND      RD14, #63, RD12
  SHAD     RD12, #6, RD14
  OR       RD9, RD14, RD12
  MOV.B    @(RQ13, 2), RD14
  AND      RD14, #63, RD9
  OR       RD12, RD9, RD6
  ADD      #3, RQ13
  MOV.L    RD6, @(R15, 20)

L00800644:

L00800643:

L00800641:
  MOV.L    @(R15, 20), RD5
  MOV      #0x00010000, RD4
  MOV      #1, RD4
  CMP/GE   RD4, RD5
  BT       L00800645
  MOV.L    @(R15, 20), RD3
  SHAD     RD3, #-10, RD14
  AND      RD14, #1023, RD12
  ADD      RD12, #55296, RD14
  MOV      RQ11, RQ8
  ADD      #2, RQ11
  MOVU.W   RD14, @RQ8
  AND      RD3, #1023, RD14
  ADD      RD14, #56320, RD12
  MOV      RQ11, RQ8
  ADD      #2, RQ11
  MOVU.W   RD12, @RQ8
  BRA      L00800646

L00800645:
  MOV      RQ11, RQ8
  ADD      #2, RQ11
  MOV.L    @(R15, 20), RD2
  MOVU.W   RD2, @RQ8

L00800646:
  BRA      L0080063C

L0080063D:
  MOV      #0, RD23
  MOVU.W   RD23, @RQ11
  MOV      RD23, RD2

L00E00485:
  ADD      #72, R15
  BRA      L00C00027

tkfat_matchlfn:
  PUSH     LR
  BSR      L00C0008B
  ADD      #-104, R15
  MOV.Q    RQ4, @(R15, 24)
  MOV.Q    RQ5, @(R15, 16)
  MOV.Q    @(R15, 24), RQ8
  MOV      RQ8, RQ11
  MOV.Q    @(R15, 16), RQ31
  MOV      RQ31, RQ10
  MOVU.W   @RQ11, RD14
  MOV      RD14, RD12
  MOVU.W   @RQ10, RD14
  MOV      RD14, RD13

L00800647:
  MOVU.W   @RQ11, RD14
  CMP/EQ   RD14
  BT       L00800648
  MOVU.W   @RQ10, RD14
  CMP/EQ   RD14
  BT       L00800648
  BRA      L00800649

L00800649:
  MOV      RQ11, RQ9
  ADD      #2, RQ11
  MOVU.W   @RQ9, RD14
  MOV      RD14, RD12
  MOV      RQ10, RQ9
  ADD      #2, RQ10
  MOVU.W   @RQ9, RD14
  MOV      RD14, RD13
  CMP/GE   #97, RD12
  BT       L0080064A
  CMP/GT   #122, RD12
  BT       L0080064A
  BRA      L0080064B

L0080064B:
  SUB      RD12, #97, RD14
  ADD      RD14, #65, RD12

L0080064A:
  CMP/GE   #97, RD13
  BT       L0080064C
  CMP/GT   #122, RD13
  BT       L0080064C
  BRA      L0080064D

L0080064D:
  SUB      RD13, #97, RD14
  ADD      RD14, #65, RD13

L0080064C:
  CMP/EQ   RD13, RD12
  BT       L0080064E
  BRA      L00800648

L0080064E:
  BRA      L00800647

L00800648:
  CMP/GT   RD13, RD12
  BT       L0080064F
  MOV      #1, RD2
  BRA      L00E00486

L0080064F:
  CMP/GT   RD12, RD13
  BT       L00800650
  MOV      #-1, RD2
  BRA      L00E00486

L00800650:
  MOV      #0, RD2

L00E00486:
  ADD      #104, R15
  BRA      L00C0008C

tkfat_lfnchecksum:
  PUSH     LR
  BSR      L00C0004E
  ADD      #-96, R15
  MOV.Q    RQ4, @(R15, 24)
  MOV.Q    @(R15, 24), RQ8
  MOV      RQ8, RQ12
  MOV      #0, RD14
  MOV      RD14, RD13
  MOV      #0, RD9

L00800651:
  MOV      #11, RD31
  CMP/GE   RD31, RD9
  BT       L00800652
  AND      RD13, #1, RD14
  SHAD     RD14, #7, RD11
  SHAD     RD13, #-1, RD14
  ADD      RD11, RD14, RD10
  MOV      RQ12, RQ30
  ADD      #1, RQ12
  MOVU.B   @RQ30, RD14
  EXTU.B   RD10, RD11
  EXTU.B   RD14, RD10
  ADD      RD11, RD10, RD13

L00800653:
  ADD      #1, RD9
  BRA      L00800651

L00800652:
  EXTU.B   RD13, RD14
  MOV      RD14, RD2

L00E00487:
  ADD      #96, R15
  BRA      L00C0004F

tkfat_asc2ucs:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  ADD      #-128, R15
  MOV      RD4, RD14
  TST      #128, RD14
  BT       L00800654
  MOV      RD14, RD2
  BRA      L00E00488

L00800654:
  TST      #96, RD14
  BT       L00800655
  MOV      RD14, RD2
  BRA      L00E00488

L00800655:
  AND      RD14, #31, RD13
  MOV      TU3CD6E720/tkfat_tuc52, RQ11
  EXTS.L   RD13
  MOV.L    @(RQ11, RD13), RD12
  MOV      RD12, RD2

L00E00488:
  ADD      #128, R15
  BRA      L00C00035

tkfat_memcmp11:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  ADD      #-136, R15
  MOV      RQ4, RQ12
  MOV      RQ5, RQ11
  MOVU.L   @RQ12, RD14
  MOVU.L   @RQ11, RD13
  CMP/EQ   RD13, RD14
  BT       L008006D0
  MOV      #-1, RD2
  BRA      L00E00489

L008006D0:
  MOVU.L   @(RQ12, 4), RD14
  MOVU.L   @(RQ11, 4), RD13
  CMP/EQ   RD13, RD14
  BT       L008006D1
  MOV      #-1, RD2
  BRA      L00E00489

L008006D1:
  MOVU.L   @(RQ12, 8), RD14
  AND      RD14, #16777215, RD13
  MOVU.L   @(RQ11, 8), RD14
  AND      RD14, #16777215, RD10
  CMP/EQ   RD10, RD13
  BT       L008006D2
  MOV      #-1, RD2
  BRA      L00E00489

L008006D2:
  MOV      #0, RD2

L00E00489:
  ADD      #136, R15
  BRA      L00C00047

TKFAT_LookupDirEntName:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-2952, R15
  MOV.Q    RQ4, @(R15, 336)
  MOV.L    RD5, @(R15, 332)
  MOV.Q    RQ6, @(R15, 320)
  MOV.Q    RQ7, @(R15, 312)
  MOV      #348, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 304)
  MOV      #380, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 288)
  MOV      #956, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 280)
  MOV      #1796, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 272)
  MOV      #2372, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 264)
  MOV      #348, RQ8
  ADD      R15, RQ8
  MOV      RQ8, RQ12
  MOV      RQ12, RQ31
  MOV      RQ8, RQ12
  MOV      RQ12, RQ9
  MOV.Q    RQ31, @(R15, 296)
  MOV      L00C00156, RQ4
  MOV.Q    @(R15, 312), RQ5
  MOV.L    @(R15, 332), RD6
  BSR      printf
  MOV      #-1, RD30
  MOV.L    RD30, @(R15, 252)
  MOV.Q    @(R15, 312), RQ4
  MOV      #2372, RQ5
  ADD      R15, RQ5
  BSR      tkfat_name2sfn
  MOV      RD2, RD29
  MOV.L    RD29, @(R15, 248)
  MOV      #0, RD28
  CMP/GE   RD28, RD29
  BT       L008006D3
  MOV      L00C00157, RQ4
  MOV      #2372, RQ5
  ADD      R15, RQ5
  BSR      printf
  MOV      #0, RD27
  MOV.L    RD27, @(R15, 248)

L008006D4:
  MOV.L    @(R15, 248), RD26
  MOV      #0x00010000, RD25
  MOV      #1, RD25
  CMP/GE   RD25, RD26
  BT       L008006D5
  MOV      #0, RD14
  MOV.Q    @(R15, 336), RQ4
  MOV.L    @(R15, 332), RD5
  MOV.L    @(R15, 248), RD6
  MOV      RD14, RD7
  MOV.Q    @(R15, 296), RQ20
  BSR      TKFAT_ReadWriteDirEntOffset
  MOV      RD2, RD11
  CMP/PZ   RD11
  BT       L008006D6
  BRA      L008006D5

L008006D6:
  MOV.Q    @(R15, 296), RQ7
  MOV      RQ7, RQ12
  MOVU.B   @RQ12, RD14
  CMP/EQ   RD14
  BT       L008006D7
  BRA      L008006D8

L008006D7:
  MOV.Q    @(R15, 296), RQ6
  MOV      RQ6, RQ12
  MOVU.B   @RQ12, RD14
  CMP/EQ   #229, RD14
  BT       L008006D9
  BRA      L008006D8

L008006D9:
  MOV.Q    @(R15, 296), RQ25
  MOV      RQ25, RQ12
  MOV      RQ12, RQ4
  MOV      #2372, RQ5
  ADD      R15, RQ5
  BSR      tkfat_memcmp11
  MOV      RD2, RD14
  TST      RD14, RD14
  MOVT     RD13
  MOV      RQ25, RQ12
  MOV      L00C00158, RQ4
  MOV      RQ12, RQ5
  MOV      #2372, RQ6
  ADD      R15, RQ6
  MOV      RD13, RD7
  BSR      printf
  CMP/EQ   RD13
  BT       L008006DA
  MOV      #348, RQ25
  ADD      R15, RQ25
  MOV.Q    @(R15, 320), RQ26
  MOV      RQ26, RQ7
  MOV.Q    @RQ25, RQ6
  MOV.Q    @(RQ25, 8), RQ5
  MOV.Q    RQ6, @RQ7
  MOV.Q    RQ5, @(RQ7, 8)
  MOV.Q    @(RQ25, 16), RQ6
  MOV.Q    @(RQ25, 24), RQ5
  MOV.Q    RQ6, @(RQ7, 16)
  MOV.Q    RQ5, @(RQ7, 24)
  MOV.Q    @(R15, 336), RQ27
  MOV.Q    RQ27, @(RQ26, 32)
  MOV.L    @(R15, 332), RD28
  MOV.L    RD28, @(RQ26, 40)
  MOV.L    @(R15, 248), RD29
  MOV.L    RD29, @(RQ26, 44)
  ADD      RQ26, #56, RQ12
  MOV      RQ12, RQ4
  MOV.Q    @(R15, 312), RQ5
  BSR      strcpy
  MOV      RD29, RD2
  BRA      L00E0048A

L008006DA:

L008006D8:
  MOV.L    @(R15, 248), RD5
  ADD      #1, RD5
  MOV.L    RD5, @(R15, 248)
  BRA      L008006D4

L008006D5:
  MOV      #-1, RD2
  BRA      L00E0048A

L008006D3:
  MOV.Q    @(R15, 312), RQ4
  MOV      #1796, RQ5
  ADD      R15, RQ5
  BSR      tkfat_name2lfn
  MOV      #0, RD4
  MOV.L    RD4, @(R15, 248)

L008006DB:
  MOV.L    @(R15, 248), RD3
  MOV      #0x00010000, RD2
  MOV      #1, RD2
  CMP/GE   RD2, RD3
  BT       L008006DC
  MOV      #0, RD14
  MOV.Q    @(R15, 336), RQ4
  MOV.L    @(R15, 332), RD5
  MOV.L    @(R15, 248), RD6
  MOV      RD14, RD7
  MOV.Q    @(R15, 296), RQ20
  BSR      TKFAT_ReadWriteDirEntOffset
  MOV      RD2, RD11
  CMP/PZ   RD11
  BT       L008006DD
  BRA      L008006DC

L008006DD:
  MOV.Q    @(R15, 296), RQ23
  MOV      RQ23, RQ12
  MOVU.B   @RQ12, RD14
  CMP/EQ   RD14
  BT       L008006DE
  BRA      L008006DF

L008006DE:
  MOV.Q    @(R15, 296), RQ22
  MOV      RQ22, RQ12
  MOVU.B   @RQ12, RD14
  CMP/EQ   #229, RD14
  BT       L008006E0
  BRA      L008006DF

L008006E0:
  MOV.Q    @(R15, 296), RQ21
  MOV.B    @(RQ21, 11), RD14
  EXTU.B   RD14, RD14
  CMP/EQ   #15, RD14
  BT       L008006E1
  MOV.B    @RQ9, RD14
  EXTU.B   RD14, RD14
  AND      RD14, #63, RD11
  MOV      #1, RD20
  CMP/GE   RD20, RD11
  BT       L008006E2
  CMP/GT   #20, RD11
  BT       L008006E2
  BRA      L008006E3

L008006E3:
  MOV.B    @(RQ9, 13), RD19
  EXTU.B   RD19, RD19
  MOV.L    RD19, @(R15, 256)
  MOV.L    @(R15, 252), RD18
  CMP/EQ   RD18, RD19
  BT       L008006E4
  MOV      #0, RD13

L008006E5:
  CMP/GE   #288, RD13
  BT       L008006E6
  MOV      #380, RQ17
  ADD      R15, RQ17
  MOV      #0x0000FFFF, RD16
  EXTS.L   RD13
  MOVU.W   RD16, @(RQ17, RD13)

L008006E7:
  ADD      #1, RD13
  BRA      L008006E5

L008006E6:
  MOV.L    @(R15, 256), RD17
  MOV      RD17, RD16
  MOV.L    RD16, @(R15, 252)

L008006E4:
  MOV.B    @RQ9, RD14
  EXTU.B   RD14, RD14
  AND      RD14, #31, RD11
  SUB      RD11, #1, RD14
  MOV      RD14, RD11
  MOV      #13, RD16
  MUL.L    RD16, RD11, RD11
  MOV.B    @(RQ9, 12), RD14
  EXTU.B   RD14, RD14
  CMP/EQ   #32, RD14
  BT       L008006E8
  MOV      #0, RD13

L008006E9:
  MOV      #10, RD16
  CMP/GE   RD16, RD13
  BT       L008006EA
  ADD      RQ9, #1, RQ12
  EXTS.L   RD13
  MOVU.B   @(RQ12, RD13), RD14
  MOV      RD14, RD4
  BSR      tkfat_asc2ucs
  MOV      RD2, RD10
  MOV      RD11, RD14
  ADD      RD14, RD13, RD25
  MOV      #380, RQ26
  ADD      R15, RQ26
  EXTS.L   RD25
  MOVU.W   RD10, @(RQ26, RD25)
  MOV.L    RD25, @(R15, 228)

L008006EB:
  ADD      #1, RD13
  BRA      L008006E9

L008006EA:
  MOV      #0, RD13

L008006EC:
  MOV      #12, RD16
  CMP/GE   RD16, RD13
  BT       L008006ED
  ADD      RQ9, #14, RQ12
  EXTS.L   RD13
  MOVU.B   @(RQ12, RD13), RD14
  MOV      RD14, RD4
  BSR      tkfat_asc2ucs
  MOV      RD2, RD10
  ADD      RD11, #5, RD14
  ADD      RD14, RD13, RD25
  MOV      #380, RQ26
  ADD      R15, RQ26
  EXTS.L   RD25
  MOVU.W   RD10, @(RQ26, RD25)
  MOV.L    RD25, @(R15, 228)

L008006EE:
  ADD      #1, RD13
  BRA      L008006EC

L008006ED:
  MOV      #0, RD13

L008006EF:
  MOV      #4, RD16
  CMP/GE   RD16, RD13
  BT       L008006F0
  ADD      RQ9, #28, RQ12
  EXTS.L   RD13
  MOVU.B   @(RQ12, RD13), RD14
  MOV      RD14, RD4
  BSR      tkfat_asc2ucs
  MOV      RD2, RD10
  ADD      RD11, #11, RD14
  ADD      RD14, RD13, RD25
  MOV      #380, RQ26
  ADD      R15, RQ26
  EXTS.L   RD25
  MOVU.W   RD10, @(RQ26, RD25)
  MOV.L    RD25, @(R15, 228)

L008006F1:
  ADD      #1, RD13
  BRA      L008006EF

L008006F0:
  MOV.B    @RQ9, RD14
  EXTU.B   RD14, RD14
  TST      #64, RD14
  BT       L008006F2
  ADD      RD11, #26, RD14
  MOV      #380, RQ16
  ADD      R15, RQ16
  MOV      #0, RD17
  EXTS.L   RD14
  MOVU.W   RD17, @(RQ16, RD14)

L008006F2:
  BRA      L008006F3

L008006E8:
  MOV.B    @(RQ9, 12), RD14
  EXTU.B   RD14, RD14
  CMP/EQ   RD14
  BT       L008006F4
  MOV      #0, RD13

L008006F5:
  MOV      #5, RD16
  CMP/GE   RD16, RD13
  BT       L008006F6
  ADD      RQ9, #1, RQ12
  SHAD     RD13, #1, RD14
  LEA.B    @(RQ12, RD14), RQ16
  MOVU.W   @RQ16, RD14
  MOV      RD11, RD10
  ADD      RD10, RD13, RD17
  MOV      #380, RQ18
  ADD      R15, RQ18
  EXTS.L   RD17
  MOVU.W   RD14, @(RQ18, RD17)
  MOV.L    RD17, @(R15, 228)
  MOV.Q    RQ16, @(R15, 216)

L008006F7:
  ADD      #1, RD13
  BRA      L008006F5

L008006F6:
  MOV      #0, RD13

L008006F8:
  MOV      #6, RD16
  CMP/GE   RD16, RD13
  BT       L008006F9
  ADD      RQ9, #14, RQ12
  SHAD     RD13, #1, RD14
  LEA.B    @(RQ12, RD14), RQ16
  MOVU.W   @RQ16, RD14
  ADD      RD11, #5, RD10
  ADD      RD10, RD13, RD17
  MOV      #380, RQ18
  ADD      R15, RQ18
  EXTS.L   RD17
  MOVU.W   RD14, @(RQ18, RD17)
  MOV.L    RD17, @(R15, 228)
  MOV.Q    RQ16, @(R15, 216)

L008006FA:
  ADD      #1, RD13
  BRA      L008006F8

L008006F9:
  MOV      #0, RD13

L008006FB:
  MOV      #2, RD16
  CMP/GE   RD16, RD13
  BT       L008006FC
  ADD      RQ9, #28, RQ12
  SHAD     RD13, #1, RD14
  LEA.B    @(RQ12, RD14), RQ16
  MOVU.W   @RQ16, RD14
  ADD      RD11, #11, RD10
  ADD      RD10, RD13, RD17
  MOV      #380, RQ18
  ADD      R15, RQ18
  EXTS.L   RD17
  MOVU.W   RD14, @(RQ18, RD17)
  MOV.L    RD17, @(R15, 228)
  MOV.Q    RQ16, @(R15, 216)

L008006FD:
  ADD      #1, RD13
  BRA      L008006FB

L008006FC:
  MOV.B    @RQ9, RD14
  EXTU.B   RD14, RD14
  TST      #64, RD14
  BT       L008006FE
  ADD      RD11, #13, RD14
  MOV      #380, RQ16
  ADD      R15, RQ16
  MOV      #0, RD17
  EXTS.L   RD14
  MOVU.W   RD17, @(RQ16, RD14)

L008006FE:

L008006F4:

L008006F3:

L008006E2:
  MOV.B    @RQ9, RD14
  EXTU.B   RD14, RD14
  AND      RD14, #63, RD11
  CMP/GE   #33, RD11
  BT       L008006FF
  CMP/GT   #61, RD11
  BT       L008006FF
  BRA      L00800700

L00800700:
  MOV.B    @(RQ9, 13), RD16
  EXTU.B   RD16, RD16
  MOV.L    RD16, @(R15, 256)
  MOV.L    @(R15, 252), RD17
  CMP/EQ   RD17, RD16
  BT       L00800701
  MOV      #0, RD13

L00800702:
  CMP/GE   #288, RD13
  BT       L00800703
  MOV      #956, RQ16
  ADD      R15, RQ16
  MOV      #0x0000FFFF, RD17
  EXTS.L   RD13
  MOVU.W   RD17, @(RQ16, RD13)

L00800704:
  ADD      #1, RD13
  BRA      L00800702

L00800703:
  MOV.L    @(R15, 256), RD17
  MOV      RD17, RD16
  MOV.L    RD16, @(R15, 252)

L00800701:
  MOV.B    @RQ9, RD14
  EXTU.B   RD14, RD14
  AND      RD14, #31, RD11
  SUB      RD11, #1, RD14
  MOV      RD14, RD11
  MOV      #13, RD16
  MUL.L    RD16, RD11, RD11
  MOV.B    @(RQ9, 12), RD14
  EXTU.B   RD14, RD14
  CMP/EQ   #32, RD14
  BT       L00800705
  MOV      #0, RD13

L00800706:
  MOV      #10, RD16
  CMP/GE   RD16, RD13
  BT       L00800707
  ADD      RQ9, #1, RQ12
  EXTS.L   RD13
  MOVU.B   @(RQ12, RD13), RD14
  MOV      RD14, RD4
  BSR      tkfat_asc2ucs
  MOV      RD2, RD10
  MOV      RD11, RD14
  ADD      RD14, RD13, RD25
  MOV      #956, RQ26
  ADD      R15, RQ26
  EXTS.L   RD25
  MOVU.W   RD10, @(RQ26, RD25)
  MOV.L    RD25, @(R15, 228)

L00800708:
  ADD      #1, RD13
  BRA      L00800706

L00800707:
  MOV      #0, RD13

L00800709:
  MOV      #12, RD16
  CMP/GE   RD16, RD13
  BT       L0080070A
  ADD      RQ9, #14, RQ12
  EXTS.L   RD13
  MOVU.B   @(RQ12, RD13), RD14
  MOV      RD14, RD4
  BSR      tkfat_asc2ucs
  MOV      RD2, RD10
  ADD      RD11, #5, RD14
  ADD      RD14, RD13, RD25
  MOV      #956, RQ26
  ADD      R15, RQ26
  EXTS.L   RD25
  MOVU.W   RD10, @(RQ26, RD25)
  MOV.L    RD25, @(R15, 228)

L0080070B:
  ADD      #1, RD13
  BRA      L00800709

L0080070A:
  MOV      #0, RD13

L0080070C:
  MOV      #4, RD16
  CMP/GE   RD16, RD13
  BT       L0080070D
  ADD      RQ9, #28, RQ12
  EXTS.L   RD13
  MOVU.B   @(RQ12, RD13), RD14
  MOV      RD14, RD4
  BSR      tkfat_asc2ucs
  MOV      RD2, RD10
  ADD      RD11, #11, RD14
  ADD      RD14, RD13, RD25
  MOV      #956, RQ26
  ADD      R15, RQ26
  EXTS.L   RD25
  MOVU.W   RD10, @(RQ26, RD25)
  MOV.L    RD25, @(R15, 228)

L0080070E:
  ADD      #1, RD13
  BRA      L0080070C

L0080070D:
  MOV.B    @RQ9, RD14
  EXTU.B   RD14, RD14
  TST      #64, RD14
  BT       L0080070F
  ADD      RD11, #26, RD14
  MOV      #956, RQ16
  ADD      R15, RQ16
  MOV      #0, RD17
  EXTS.L   RD14
  MOVU.W   RD17, @(RQ16, RD14)

L0080070F:
  BRA      L00800710

L00800705:
  MOV.B    @(RQ9, 12), RD14
  EXTU.B   RD14, RD14
  CMP/EQ   RD14
  BT       L00800711
  MOV      #0, RD13

L00800712:
  MOV      #5, RD16
  CMP/GE   RD16, RD13
  BT       L00800713
  ADD      RQ9, #1, RQ12
  SHAD     RD13, #1, RD14
  LEA.B    @(RQ12, RD14), RQ16
  MOVU.W   @RQ16, RD14
  MOV      RD11, RD10
  ADD      RD10, RD13, RD17
  MOV      #956, RQ18
  ADD      R15, RQ18
  EXTS.L   RD17
  MOVU.W   RD14, @(RQ18, RD17)
  MOV.L    RD17, @(R15, 228)
  MOV.Q    RQ16, @(R15, 216)

L00800714:
  ADD      #1, RD13
  BRA      L00800712

L00800713:
  MOV      #0, RD13

L00800715:
  MOV      #6, RD16
  CMP/GE   RD16, RD13
  BT       L00800716
  ADD      RQ9, #14, RQ12
  SHAD     RD13, #1, RD14
  LEA.B    @(RQ12, RD14), RQ16
  MOVU.W   @RQ16, RD14
  ADD      RD11, #5, RD10
  ADD      RD10, RD13, RD17
  MOV      #956, RQ18
  ADD      R15, RQ18
  EXTS.L   RD17
  MOVU.W   RD14, @(RQ18, RD17)
  MOV.L    RD17, @(R15, 228)
  MOV.Q    RQ16, @(R15, 216)

L00800717:
  ADD      #1, RD13
  BRA      L00800715

L00800716:
  MOV      #0, RD13

L00800718:
  MOV      #2, RD16
  CMP/GE   RD16, RD13
  BT       L00800719
  ADD      RQ9, #28, RQ12
  SHAD     RD13, #1, RD14
  LEA.B    @(RQ12, RD14), RQ16
  MOVU.W   @RQ16, RD14
  ADD      RD11, #11, RD10
  ADD      RD10, RD13, RD17
  MOV      #956, RQ18
  ADD      R15, RQ18
  EXTS.L   RD17
  MOVU.W   RD14, @(RQ18, RD17)
  MOV.L    RD17, @(R15, 228)
  MOV.Q    RQ16, @(R15, 216)

L0080071A:
  ADD      #1, RD13
  BRA      L00800718

L00800719:
  MOV.B    @RQ9, RD14
  EXTU.B   RD14, RD14
  TST      #64, RD14
  BT       L0080071B
  ADD      RD11, #13, RD14
  MOV      #956, RQ16
  ADD      R15, RQ16
  MOV      #0, RD17
  EXTS.L   RD14
  MOVU.W   RD17, @(RQ16, RD14)

L0080071B:

L00800711:

L00800710:

L008006FF:
  BRA      L008006DF

L008006E1:
  MOV.Q    @(R15, 296), RQ25
  MOV      RQ25, RQ12
  MOV      RQ12, RQ4
  BSR      tkfat_lfnchecksum
  MOV      RD2, RD26
  MOV.L    RD26, @(R15, 260)
  MOV.L    @(R15, 256), RD27
  CMP/EQ   RD27, RD26
  BT       L0080071C
  MOV      #380, RQ4
  ADD      R15, RQ4
  MOV      #1796, RQ5
  ADD      R15, RQ5
  BSR      tkfat_matchlfn
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L0080071C
  BRA      L0080071D

L0080071D:
  MOV.Q    @(R15, 296), RQ25
  MOV      RQ25, RQ26
  MOV.Q    @(R15, 320), RQ27
  MOV      RQ27, RQ7
  MOV.Q    @RQ26, RQ6
  MOV.Q    @(RQ26, 8), RQ5
  MOV.Q    RQ6, @RQ7
  MOV.Q    RQ5, @(RQ7, 8)
  MOV.Q    @(RQ26, 16), RQ6
  MOV.Q    @(RQ26, 24), RQ5
  MOV.Q    RQ6, @(RQ7, 16)
  MOV.Q    RQ5, @(RQ7, 24)
  MOV.Q    @(R15, 336), RQ28
  MOV.Q    RQ28, @(RQ27, 32)
  MOV.L    @(R15, 332), RD29
  MOV.L    RD29, @(RQ27, 40)
  MOV.L    @(R15, 248), RD30
  MOV.L    RD30, @(RQ27, 44)
  ADD      RQ27, #56, RQ12
  MOV      RQ12, RQ4
  MOV.Q    @(R15, 312), RQ5
  BSR      strcpy
  MOV      RD30, RD2
  BRA      L00E0048A

L0080071C:

L008006DF:
  MOV.L    @(R15, 248), RD16
  ADD      #1, RD16
  MOV.L    RD16, @(R15, 248)
  BRA      L008006DB

L008006DC:
  MOV      #-1, RD2

L00E0048A:
  ADD      #2952, R15
  BRA      L00C00027

TKFAT_LookupDirEntPathR:
  PUSH     LR
  BSR      L00C00049
  ADD      #-1704, R15
  MOV.Q    RQ4, @(R15, 304)
  MOV.L    RD5, @(R15, 300)
  MOV.Q    RQ6, @(R15, 288)
  MOV.Q    RQ7, @(R15, 280)
  MOV      #312, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 272)
  MOV      #1392, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 264)
  MOV.Q    @(R15, 280), RQ9
  MOV      RQ9, RQ13
  MOV      #1392, RQ8
  ADD      R15, RQ8
  MOV      RQ8, RQ10

L0080074D:
  MOV.B    @RQ13, RD14
  CMP/EQ   RD14
  BT       L0080074E
  MOV.B    @RQ13, RD14
  CMP/EQ   #47, RD14
  BT       L0080074E
  BRA      L0080074F

L0080074F:
  MOV      RQ13, RQ11
  ADD      #1, RQ13
  MOV.B    @RQ11, RD14
  MOV      RQ10, RQ11
  ADD      #1, RQ10
  MOV.B    RD14, @RQ11
  BRA      L0080074D

L0080074E:
  MOV      RQ10, RQ11
  ADD      #1, RQ10
  MOV      #0, RD31
  MOV.B    RD31, @RQ11
  MOV.B    @RQ13, RD14
  CMP/EQ   #47, RD14
  BT       L00800750
  ADD      #1, RQ13

L00800750:
  MOV.B    @RQ13, RD14
  CMP/EQ   RD14
  BT       L00800751
  MOV      #312, RQ30
  ADD      R15, RQ30
  MOV      RQ30, RQ11
  MOV.Q    @(R15, 304), RQ4
  MOV.L    @(R15, 300), RD5
  MOV      RQ11, RQ6
  MOV      #1392, RQ7
  ADD      R15, RQ7
  BSR      TKFAT_LookupDirEntName
  MOV      RD2, RD12
  CMP/PZ   RD12
  BT       L00800752
  MOV      L00C0015A, RQ4
  MOV      #1392, RQ5
  ADD      R15, RQ5
  BSR      printf
  MOV      RD12, RD2
  BRA      L00E0048B

L00800752:
  MOV      #312, RQ29
  ADD      R15, RQ29
  MOV.L    @(RQ29, 40), RD14
  MOV.Q    @(R15, 304), RQ4
  MOV      RD14, RD5
  MOV.Q    @(R15, 288), RQ6
  MOV      RQ13, RQ7
  BSR      TKFAT_LookupDirEntPathR
  MOV      RD2, RD12
  MOV      RD12, RD2
  BRA      L00E0048B

L00800751:
  MOV.Q    @(R15, 304), RQ4
  MOV.L    @(R15, 300), RD5
  MOV.Q    @(R15, 288), RQ6
  MOV      #1392, RQ7
  ADD      R15, RQ7
  BSR      TKFAT_LookupDirEntName
  MOV      RD2, RD12
  MOV      RD12, RD2

L00E0048B:
  ADD      #1704, R15
  BRA      L00C0004A

TKFAT_LookupDirEntPath:
  PUSH     LR
  BSR      L00C00072
  ADD      #-128, R15
  MOV      RQ4, RQ11
  MOV.Q    RQ5, @(R15, 16)
  MOV.Q    RQ6, @(R15, 8)
  MOV.B    @(RQ11, 13), RD12
  EXTU.B   RD12, RD12
  CMP/EQ   RD12
  BT       L00800753
  MOV      #1, RD14
  BRA      L00800754

L00800753:
  MOV      #2, RD14

L00800754:
  MOV      RD14, RD10
  MOV      RQ11, RQ4
  MOV      RD10, RD5
  MOV.Q    @(R15, 16), RQ6
  MOV.Q    @(R15, 8), RQ7
  BSR      TKFAT_LookupDirEntPathR
  MOV      RD2, RD13
  MOV      RD13, RD2

L00E0048C:
  ADD      #128, R15
  BRA      L00C00074

TKFAT_UpdateDirEnt:
  PUSH     LR
  BSR      L00C0003D
  ADD      #-128, R15
  MOV      RQ4, RQ13
  MOV      RQ13, RQ14
  MOV.L    @(RQ13, 44), RD9
  MOV.L    @(RQ13, 40), RD10
  MOV.Q    @(RQ13, 32), RQ11
  MOV      #1, RD8
  MOV      RQ11, RQ4
  MOV      RD10, RD5
  MOV      RD9, RD6
  MOV      RD8, RD7
  MOV      RQ14, RQ20
  BSR      TKFAT_ReadWriteDirEntOffset
  MOV      RD2, RD12
  MOV      RD12, RD2

L00E0048D:
  ADD      #128, R15
  BRA      L00C0003E

TKFAT_GetDirEntCluster:
  PUSH     LR
  BSR      L00C00037
  ADD      #-168, R15
  MOV.Q    RQ4, @(R15, 32)
  MOV.Q    @(R15, 32), RQ13
  MOV.L    @(RQ13, 48), RD14
  EXTU.L   RD14, RD14
  CMP/EQ   RD14
  BT       L00800755
  MOV.Q    @(R15, 32), RQ12
  MOV.L    @(RQ12, 48), RD14
  EXTU.L   RD14, RD14
  MOV      RD14, RD2
  BRA      L00E0048E

L00800755:
  MOV.Q    @(R15, 32), RQ11
  MOV      RQ11, RQ10
  ADD      RQ10, #26, RQ9
  MOVU.W   @RQ9, RD14
  MOV      RD14, RD7
  MOV      RQ11, RQ10
  ADD      RQ10, #20, RQ9
  MOVU.W   @RQ9, RD14
  MOV      RD14, RD6
  SHAD     RD6, #16, RD14
  OR       RD7, RD14, RD5
  EXTU.L   RD5, RD5
  MOV.L    RD5, @(RQ11, 48)
  MOV.L    RD7, @(R15, 28)
  MOV.L    RD6, @(R15, 24)
  MOV.L    RD5, @(R15, 20)
  MOV      RD5, RD2

L00E0048E:
  ADD      #168, R15
  BRA      L00C00038

TKFAT_SetDirEntCluster:
  PUSH     LR
  BSR      L00C00072
  ADD      #-176, R15
  MOV.Q    RQ4, @(R15, 32)
  MOV.L    RD5, @(R15, 28)
  MOV.Q    @(R15, 32), RQ14
  MOV      RQ14, RQ13
  ADD      RQ13, #26, RQ12
  MOV.L    @(R15, 28), RD11
  EXTU.W   RD11, RD11
  MOV.Q    RQ13, @(R15, 16)
  MOV.Q    RQ12, @(R15, 8)
  MOV.L    RD11, @(R15, 4)
  MOV      RQ12, RQ4
  MOV      RD11, RD5
  BSR      tkfat_setWord
  MOV.L    @(R15, 28), RD10
  SHLD     RD10, #-16, RD11
  MOV      RQ14, RQ13
  ADD      RQ13, #20, RQ12
  EXTU.W   RD11, RD14
  MOV.L    RD14, @R15
  MOV.Q    RQ13, @(R15, 16)
  MOV.Q    RQ12, @(R15, 8)
  MOV.L    RD11, @(R15, 4)
  MOV      RQ12, RQ4
  MOV      RD14, RD5
  BSR      tkfat_setWord
  MOV.Q    @(R15, 32), RQ7
  EXTU.L   RD10, RD10
  MOV.L    RD10, @(RQ7, 48)
  MOV      #0, RD2

L00E0048F:
  ADD      #176, R15
  BRA      L00C00074

TKFAT_GetDirEntSize:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  ADD      #-184, R15
  MOV.Q    RQ4, @(R15, 32)
  MOV.Q    @(R15, 32), RQ13
  MOV.L    @(RQ13, 52), RD14
  EXTU.L   RD14, RD14
  CMP/EQ   RD14
  BT       L00800756
  MOV.Q    @(R15, 32), RQ12
  MOV.L    @(RQ12, 52), RD14
  EXTU.L   RD14, RD14
  MOV      RD14, RD2
  BRA      L00E00490

L00800756:
  MOV.Q    @(R15, 32), RQ11
  MOV      RQ11, RQ10
  ADD      RQ10, #28, RQ7
  MOVU.L   @RQ7, RD14
  MOV      RD14, RD6
  EXTU.L   RD6, RD6
  MOV.L    RD6, @(RQ11, 52)
  MOV.L    RD6, @(R15, 28)
  MOV      RD6, RD2

L00E00490:
  ADD      #184, R15
  BRA      L00C00047

TKFAT_SetDirEntSize:
  PUSH     LR
  BSR      L00C00032
  ADD      #-184, R15
  MOV.Q    RQ4, @(R15, 32)
  MOV.L    RD5, @(R15, 28)
  MOV.Q    @(R15, 32), RQ14
  MOV      RQ14, RQ13
  ADD      RQ13, #28, RQ12
  MOV.Q    RQ12, @(R15, 8)
  MOV      RQ12, RQ4
  MOV.L    @(R15, 28), RD5
  BSR      tkfat_setDWord
  MOV.L    @(R15, 28), RD11
  EXTU.L   RD11, RD11
  MOV.L    RD11, @(RQ14, 52)
  MOV      #0, RD2

L00E00491:
  ADD      #184, R15
  BRA      L00C00033

TKFAT_ReadWriteDirEntFile:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-104, R15
  MOV      RQ4, RQ13
  MOV      RD5, RD8
  MOV.L    RD6, @(R15, 64)
  MOV.Q    RQ7, @(R15, 56)
  MOV.L    RD20, @(R15, 52)
  MOV      #0, RQ31
  CMPQ/EQ  RQ31, RQ13
  BT       L00800757
  MOV.Q    @(RQ13, 32), RQ12
  MOV      #0, RQ30
  CMPQ/EQ  RQ30, RQ12
  BT       L00800757
  BRA      L00800758

L00800757:
  BRK     

L00800758:
  MOV.L    @(R15, 64), RD29
  CMP/EQ   #0, RD29
  BT       L00800759
  MOV      #0, RQ28
  MOV      #0, RD27
  MOV      #0, RD14
  MOV      RD14, RD9
  MOV.Q    RQ28, TU3CD6E720/tkfat_rdbuf_dee
  MOV.L    RD27, TU3CD6E720/tkfat_rdbuf_clid
  MOV      RQ13, RQ4
  BSR      TKFAT_GetDirEntCluster
  MOV      RD2, RD10
  MOV      RQ13, RQ4
  BSR      TKFAT_GetDirEntSize
  MOV      RD2, RD11
  CMP/EQ   RD10
  BT       L0080075A
  MOV.Q    @(RQ13, 32), RQ12
  MOV      RQ12, RQ4
  BSR      TKFAT_AllocFreeCluster
  MOV      RD2, RD10
  MOV      RQ13, RQ4
  MOV      RD10, RD5
  BSR      TKFAT_SetDirEntCluster
  MOV      #1, RD14
  MOV      RD14, RD9

L0080075A:
  MOV.L    @(R15, 52), RD26
  ADD      RD8, RD26, RD14
  CMP/HI   RD11, RD14
  BT       L0080075B
  MOV.L    @(R15, 52), RD25
  ADD      RD8, RD25, RD11
  MOV      RQ13, RQ4
  MOV      RD11, RD5
  BSR      TKFAT_SetDirEntSize
  MOV      #1, RD14
  MOV      RD14, RD9

L0080075B:
  CMP/EQ   RD9
  BT       L0080075C
  MOV      RQ13, RQ4
  BSR      TKFAT_UpdateDirEnt

L0080075C:
  MOV.Q    @(RQ13, 32), RQ12
  MOV      #1, RD14
  MOV      RQ12, RQ4
  MOV      RD10, RD5
  MOV      RD8, RD6
  MOV      RD14, RD7
  MOV.Q    @(R15, 56), RQ20
  MOV.L    @(R15, 52), RD21
  BSR      TKFAT_ReadWriteClusterOffset
  MOV      RD2, RD25
  MOV.L    RD25, @(R15, 40)
  MOV      RD25, RD2
  BRA      L00E00492

L00800759:
  AND      RD8, #-512, RD7
  MOV.L    @(R15, 52), RD6
  ADD      RD8, RD6, RD14
  AND      RD14, #-512, RD5
  MOV.L    RD7, @(R15, 48)
  MOV.L    RD5, @(R15, 44)
  CMP/EQ   RD5, RD7
  BT       L0080075D
  MOV.L    TU3CD6E720/tkfat_rdbuf_clid, RD4
  CMP/EQ   RD4
  BT       L0080075E
  MOV.L    @(RQ13, 48), RD14
  EXTU.L   RD14, RD14
  MOV      RD14, RD3
  MOV.L    TU3CD6E720/tkfat_rdbuf_clid, RD23
  MOV      RD23, RD2
  MOV.L    RD3, @(R15, 16)
  MOV.L    RD2, @(R15, 12)
  CMP/EQ   RD2, RD3
  BT       L0080075E
  BRA      L0080075F

L0080075F:
  MOV.L    @(R15, 48), RD22
  MOV.L    TU3CD6E720/tkfat_rdbuf_offs, RD21
  CMP/EQ   RD21, RD22
  BT       L0080075E
  BRA      L00800760

L00800760:
  AND      RD8, #511, RD14
  MOV      TU3CD6E720/tkfat_rdbuf_data, RQ25
  EXTS.L   RD14
  LEA.B    @(RQ25, RD14), RQ12
  MOV.L    @(R15, 52), RD26
  EXTS.L   RQ26, RQ26
  MOV.Q    RQ26, @R15
  MOV.Q    @(R15, 56), RQ4
  MOV      RQ12, RQ5
  MOV      RQ26, RQ6
  BSR      memcpy
  MOV.L    @(R15, 52), RD2
  BRA      L00E00492

L0080075E:

L0080075D:
  MOV      RQ13, RQ4
  BSR      TKFAT_GetDirEntCluster
  MOV      RD2, RD10
  MOV      RQ13, RQ4
  BSR      TKFAT_GetDirEntSize
  MOV      RD2, RD11
  CMP/HI   RD8, RD11
  BT       L00800761
  MOV      #0, RD2
  BRA      L00E00492

L00800761:
  MOV.L    @(R15, 52), RD19
  MOV      RD19, RD20
  ADD      RD8, RD19, RD14
  MOV.L    RD20, @(R15, 36)
  CMP/HI   RD11, RD14
  BT       L00800762
  SUB      RD11, RD8, RD18
  MOV.L    RD18, @(R15, 36)

L00800762:
  MOV.L    @(R15, 48), RD17
  MOV.L    @(R15, 44), RD16
  CMP/EQ   RD16, RD17
  BT       L00800763
  MOV.L    @(R15, 48), RD16
  ADD      RD16, #512, RD14
  CMP/HI   RD11, RD14
  BT       L00800763
  BRA      L00800764

L00800764:
  MOV.Q    @(RQ13, 32), RQ12
  MOV      #0, RD14
  MOV      RQ12, RQ4
  MOV      RD10, RD5
  MOV.L    @(R15, 48), RD6
  MOV      RD14, RD7
  MOV      TU3CD6E720/tkfat_rdbuf_data, RQ20
  MOV      #512, RD21
  BSR      TKFAT_ReadWriteClusterOffset
  MOV      RD2, RD25
  MOV.L    RD25, @(R15, 40)
  MOV      #0, RD26
  CMP/GE   RD26, RD25
  BT       L00800765
  MOV.L    @(R15, 40), RD2
  BRA      L00E00492

L00800765:
  MOV      RQ13, RQ25
  MOV.L    @(R15, 48), RD27
  MOV      RD27, RD26
  MOV.L    @(RQ13, 48), RD28
  EXTU.L   RD28, RD28
  AND      RD8, #511, RD14
  MOV      TU3CD6E720/tkfat_rdbuf_data, RQ29
  EXTS.L   RD14
  LEA.B    @(RQ29, RD14), RQ12
  MOV.L    @(R15, 52), RD30
  EXTS.L   RQ30, RQ30
  MOV.Q    RQ30, @R15
  MOV.L    RD28, TU3CD6E720/tkfat_rdbuf_clid
  MOV.L    RD26, TU3CD6E720/tkfat_rdbuf_offs
  MOV.Q    RQ25, TU3CD6E720/tkfat_rdbuf_dee
  MOV.Q    @(R15, 56), RQ4
  MOV      RQ12, RQ5
  MOV      RQ30, RQ6
  BSR      memcpy
  MOV.L    @(R15, 52), RD2
  BRA      L00E00492
  BRA      L00800766

L00800763:
  MOV.Q    @(RQ13, 32), RQ12
  MOV      #0, RD14
  MOV      RQ12, RQ4
  MOV      RD10, RD5
  MOV      RD8, RD6
  MOV      RD14, RD7
  MOV.Q    @(R15, 56), RQ20
  MOV.L    @(R15, 36), RD21
  BSR      TKFAT_ReadWriteClusterOffset
  MOV      RD2, RD25
  MOV.L    RD25, @(R15, 40)
  MOV      #0, RD26
  CMP/GE   RD26, RD25
  BT       L00800767
  MOV.L    @(R15, 40), RD2
  BRA      L00E00492

L00800767:

L00800766:
  MOV.L    @(R15, 36), RD2

L00E00492:
  ADD      #104, R15
  BRA      L00C00027

TKFAT_CreateSdFatContext:
  PUSH     LR
  PUSH     R14
  PUSH     R13
  ADD      #-136, R15
  MOV      #0x00042010, RQ13
  MOV      #4, RQ13
  EXTS.L   RQ13, RQ13
  MOV      RQ13, RQ4
  BSR      malloc
  MOV      RQ2, RQ14
  MOV      #0x00042010, RQ13
  MOV      #4, RQ13
  EXTS.L   RQ13, RQ13
  MOV      RQ14, RQ4
  MOV      #0, RD5
  MOV      RQ13, RQ6
  BSR      memset
  BSR      TKSPI_InitDevice
  MOV      RQ14, RQ4
  BSR      TKFAT_ReadImageMBR
  MOV      RQ14, RQ4
  BSR      TKFAT_ReadImageFAT
  MOV      RQ14, RQ2

L00E00493:
  ADD      #136, R15
  POP      RQ13
  POP      RQ14
  RET     

tk_vf_register:
  PUSH     R14
  PUSH     R13
  ADD      #-144, R15
  MOV.Q    RQ4, @(R15, 8)
  MOV.Q    tk_fsty_root, RQ14
  MOV.Q    @(R15, 8), RQ13
  MOV.Q    RQ14, @(RQ13, 8)
  MOV      RQ13, RQ14
  MOV.Q    RQ14, tk_fsty_root

L00E00494:
  ADD      #144, R15
  POP      RQ13
  POP      RQ14
  RTS     

tk_fat_init:
  PUSH     LR
  PUSH     R14
  PUSH     R13
  ADD      #-136, R15
  MOV      tk_vfile_fat_vt, RQ13
  MOV      RQ13, RQ14
  MOV      RQ14, RQ4
  BSR      tk_vf_register
  MOV      #1, RD2

L00E00495:
  ADD      #136, R15
  POP      RQ13
  POP      RQ14
  RET     

tk_mount_sdfat:
  PUSH     LR
  BSR      L00C00072
  ADD      #-112, R15
  BSR      TKFAT_CreateSdFatContext
  MOV      RQ2, RQ13
  BSR      tk_alloc_mount
  MOV      RQ2, RQ14
  MOV      tk_vfile_fat_vt, RQ11
  MOV      RQ11, RQ12
  MOV.Q    RQ12, @RQ14
  MOV.Q    RQ13, @(RQ14, 16)
  MOV.Q    tk_vf_mount, RQ10
  MOV.Q    RQ10, @(RQ14, 8)
  MOV      RQ14, RQ10
  MOV.Q    RQ10, tk_vf_mount

L00E00496:
  ADD      #112, R15
  BRA      L00C00074

tk_vfile_init:
  PUSH     LR
  PUSH     R14
  ADD      #-128, R15
  MOV.L    TU3CD6E720/tk_vf_init, RD14
  CMP/EQ   #1, RD14
  BT       L0080078B
  MOV      #0, RD2
  BRA      L00E00497

L0080078B:
  MOV      #1, RD14
  MOV.L    RD14, TU3CD6E720/tk_vf_init
  MOV      L00C00172, RQ4
  BSR      tk_puts
  BSR      tk_fat_init
  BSR      tk_mount_sdfat
  MOV      L00C00173, RQ4
  BSR      tk_puts
  MOV      #1, RD2

L00E00497:
  ADD      #128, R15
  POP      RQ14
  RET     

tk_alloc_file:
  PUSH     LR
  BSR      L00C00072
  ADD      #-112, R15
  MOV.Q    tk_vf_freelist, RQ12
  MOV      RQ12, RQ14
  MOV      #0, RQ11
  CMPQ/EQ  RQ11, RQ14
  BT       L0080078C
  MOV.Q    @(RQ14, 8), RQ10
  MOV      #72, RQ13
  EXTS.L   RQ13, RQ13
  MOV.Q    RQ10, tk_vf_freelist
  MOV      RQ14, RQ4
  MOV      #0, RD5
  MOV      RQ13, RQ6
  BSR      memset
  MOV      RQ14, RQ2
  BRA      L00E00498

L0080078C:
  MOV      #72, RQ13
  EXTS.L   RQ13, RQ13
  MOV      RQ13, RQ4
  BSR      malloc
  MOV      RQ2, RQ14
  MOV      #72, RQ13
  EXTS.L   RQ13, RQ13
  MOV      RQ14, RQ4
  MOV      #0, RD5
  MOV      RQ13, RQ6
  BSR      memset
  MOV      RQ14, RQ2

L00E00498:
  ADD      #112, R15
  BRA      L00C00074

tk_alloc_mount:
  PUSH     LR
  BSR      L00C00072
  ADD      #-112, R15
  MOV.Q    tk_mnt_freelist, RQ12
  MOV      RQ12, RQ14
  MOV      #0, RQ11
  CMPQ/EQ  RQ11, RQ14
  BT       L0080078D
  MOV.Q    @(RQ14, 16), RQ10
  MOV      #32, RQ13
  EXTS.L   RQ13, RQ13
  MOV.Q    RQ10, tk_mnt_freelist
  MOV      RQ14, RQ4
  MOV      #0, RD5
  MOV      RQ13, RQ6
  BSR      memset
  MOV      RQ14, RQ2
  BRA      L00E00499

L0080078D:
  MOV      #32, RQ13
  EXTS.L   RQ13, RQ13
  MOV      RQ13, RQ4
  BSR      malloc
  MOV      RQ2, RQ14
  MOV      #32, RQ13
  EXTS.L   RQ13, RQ13
  MOV      RQ14, RQ4
  MOV      #0, RD5
  MOV      RQ13, RQ6
  BSR      memset
  MOV      RQ14, RQ2

L00E00499:
  ADD      #112, R15
  BRA      L00C00074

tk_fat_mount:
  ADD      #-192, R15
  MOV.Q    RQ4, @(R15, 40)
  MOV.Q    RQ5, @(R15, 32)
  MOV.Q    RQ6, @(R15, 24)
  MOV.Q    RQ7, @(R15, 16)
  MOV.Q    RQ20, @(R15, 8)

L00E0049A:
  ADD      #192, R15
  RTS     

tk_fat_fopen:
  PUSH     LR
  BSR      L00C00053
  ADD      #-1432, R15
  MOV.Q    RQ4, @(R15, 304)
  MOV.Q    RQ5, @(R15, 296)
  MOV.Q    RQ6, @(R15, 288)
  MOV      #312, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 280)
  MOV      L00C00177, RQ4
  MOV.Q    @(R15, 296), RQ5
  BSR      tk_printf
  MOV.Q    @(R15, 304), RQ8
  MOV.Q    @(RQ8, 16), RQ9
  MOV      #312, RQ31
  ADD      R15, RQ31
  MOV      RQ31, RQ12
  MOV      RQ12, RQ10
  MOV      #1080, RQ13
  EXTS.L   RQ13, RQ13
  MOV      RQ10, RQ4
  MOV      #0, RD5
  MOV      RQ13, RQ6
  BSR      memset
  MOV      RQ9, RQ4
  MOV      RQ10, RQ5
  MOV.Q    @(R15, 296), RQ6
  BSR      TKFAT_LookupDirEntPath
  MOV      RD2, RD30
  MOV.L    RD30, @(R15, 276)
  MOV      #0, RD29
  CMP/GE   RD29, RD30
  BT       L0080078E
  MOV      L00C00178, RQ4
  MOV.Q    @(R15, 296), RQ5
  BSR      tk_printf
  MOV      #0, RQ12
  MOV      RQ12, RQ2
  BRA      L00E0049B

L0080078E:
  MOV      #1080, RQ13
  EXTS.L   RQ13, RQ13
  MOV      RQ13, RQ4
  BSR      malloc
  MOV      RQ2, RQ11
  MOV      #1080, RQ13
  EXTS.L   RQ13, RQ13
  MOV      RQ11, RQ4
  MOV      RQ10, RQ5
  MOV      RQ13, RQ6
  BSR      memcpy
  MOV.Q    RQ9, @(RQ11, 32)
  BSR      tk_alloc_file
  MOV      RQ2, RQ14
  MOV      tk_vfile_fat_vt, RQ28
  MOV      RQ28, RQ12
  MOV.Q    RQ12, @RQ14
  MOV.Q    RQ9, @(RQ14, 8)
  MOV.Q    RQ11, @(RQ14, 16)
  MOV      #0, RD27
  EXTU.L   RD27, RD27
  MOV.L    RD27, @(RQ14, 64)
  MOV      RQ11, RQ4
  BSR      TKFAT_GetDirEntSize
  MOV      RD2, RD29
  EXTU.L   RD29, RD29
  MOV.L    RD29, @(RQ14, 68)
  MOV.L    RD29, @(R15, 252)
  MOV      RQ14, RQ2

L00E0049B:
  ADD      #1432, R15
  BRA      L00C00054

tk_fat_fread:
  PUSH     LR
  BSR      L00C0003D
  ADD      #-128, R15
  MOV.Q    RQ4, @(R15, 40)
  MOV      RD5, RD10
  MOV      RD6, RD9
  MOV      RQ7, RQ12
  MUL.L    RD10, RD9, RD14
  MOV.L    @(RQ12, 64), RD13
  EXTU.L   RD13, RD13
  MOV.Q    @(RQ12, 16), RQ8
  MOV      #0, RD11
  MOV      RQ8, RQ4
  MOV      RD13, RD5
  MOV      RD11, RD6
  MOV.Q    @(R15, 40), RQ7
  MOV      RD14, RD20
  BSR      TKFAT_ReadWriteDirEntFile
  MOV      RD2, RD14
  CMP/PL   RD14
  BT       L00800798
  MOV.L    @(RQ12, 64), RD13
  EXTU.L   RD13, RD13
  ADD      RD13, RD14, RD11
  EXTU.L   RD11, RD11
  MOV.L    RD11, @(RQ12, 64)

L00800798:
  MOV      RD14, RD2

L00E0049C:
  ADD      #128, R15
  BRA      L00C0003E

tk_fat_fwrite:
  PUSH     LR
  BSR      L00C0003D
  ADD      #-128, R15
  MOV.Q    RQ4, @(R15, 40)
  MOV      RD5, RD10
  MOV      RD6, RD9
  MOV      RQ7, RQ12
  MUL.L    RD10, RD9, RD14
  MOV.L    @(RQ12, 64), RD13
  EXTU.L   RD13, RD13
  MOV.Q    @(RQ12, 16), RQ8
  MOV      #1, RD11
  MOV      RQ8, RQ4
  MOV      RD13, RD5
  MOV      RD11, RD6
  MOV.Q    @(R15, 40), RQ7
  MOV      RD14, RD20
  BSR      TKFAT_ReadWriteDirEntFile
  MOV      RD2, RD14
  CMP/PL   RD14
  BT       L00800799
  MOV.L    @(RQ12, 64), RD13
  EXTU.L   RD13, RD13
  ADD      RD13, RD14, RD11
  EXTU.L   RD11, RD11
  MOV.L    RD11, @(RQ12, 64)

L00800799:
  MOV      RD14, RD2

L00E0049D:
  ADD      #128, R15
  BRA      L00C0003E

tk_fat_fseek:
  PUSH     LR
  BSR      L00C0008B
  ADD      #-104, R15
  MOV      RQ4, RQ13
  MOV      RD5, RD10
  MOV      RD6, RD11
  CMP/EQ   RD11
  BT       L0080078F
  EXTU.L   RD10, RD10
  MOV.L    RD10, @(RQ13, 64)
  MOV.L    @(RQ13, 64), RD14
  EXTU.L   RD14, RD14
  BRA      L00800790
  MOV      #0, RD9
  EXTU.L   RD9, RD9
  MOV.L    RD9, @(RQ13, 64)

L00800790:
  MOV.L    @(RQ13, 64), RD14
  EXTU.L   RD14, RD14
  MOV.L    @(RQ13, 68), RD12
  EXTU.L   RD12, RD12
  CMP/HI   RD12, RD14
  BT       L00800791
  MOV.L    @(RQ13, 68), RD14
  EXTU.L   RD14, RD14
  EXTU.L   RD14, RD14
  MOV.L    RD14, @(RQ13, 64)

L00800791:
  MOV.L    @(RQ13, 64), RD14
  EXTU.L   RD14, RD14
  MOV      RD14, RD2
  BRA      L00E0049E

L0080078F:
  CMP/EQ   #1, RD11
  BT       L00800792
  MOV.L    @(RQ13, 64), RD14
  EXTU.L   RD14, RD14
  ADD      RD14, RD10, RD12
  EXTU.L   RD12, RD12
  MOV.L    RD12, @(RQ13, 64)
  MOV.L    @(RQ13, 64), RD14
  EXTU.L   RD14, RD14
  BRA      L00800793
  MOV      #0, RD8
  EXTU.L   RD8, RD8
  MOV.L    RD8, @(RQ13, 64)

L00800793:
  MOV.L    @(RQ13, 64), RD14
  EXTU.L   RD14, RD14
  MOV.L    @(RQ13, 68), RD12
  EXTU.L   RD12, RD12
  CMP/HI   RD12, RD14
  BT       L00800794
  MOV.L    @(RQ13, 68), RD14
  EXTU.L   RD14, RD14
  EXTU.L   RD14, RD14
  MOV.L    RD14, @(RQ13, 64)

L00800794:
  MOV.L    @(RQ13, 64), RD14
  EXTU.L   RD14, RD14
  MOV      RD14, RD2
  BRA      L00E0049E

L00800792:
  CMP/EQ   #2, RD11
  BT       L00800795
  MOV.L    @(RQ13, 68), RD14
  EXTU.L   RD14, RD14
  ADD      RD14, RD10, RD12
  EXTU.L   RD12, RD12
  MOV.L    RD12, @(RQ13, 64)
  MOV.L    @(RQ13, 64), RD14
  EXTU.L   RD14, RD14
  BRA      L00800796
  MOV      #0, RD31
  EXTU.L   RD31, RD31
  MOV.L    RD31, @(RQ13, 64)

L00800796:
  MOV.L    @(RQ13, 64), RD14
  EXTU.L   RD14, RD14
  MOV.L    @(RQ13, 68), RD12
  EXTU.L   RD12, RD12
  CMP/HI   RD12, RD14
  BT       L00800797
  MOV.L    @(RQ13, 68), RD14
  EXTU.L   RD14, RD14
  EXTU.L   RD14, RD14
  MOV.L    RD14, @(RQ13, 64)

L00800797:
  MOV.L    @(RQ13, 64), RD14
  EXTU.L   RD14, RD14
  MOV      RD14, RD2
  BRA      L00E0049E

L00800795:
  MOV      #-1, RD2

L00E0049E:
  ADD      #104, R15
  BRA      L00C0008C

tk_fat_ftell:
  PUSH     R14
  PUSH     R13
  ADD      #-144, R15
  MOV      RQ4, RQ13
  MOV.L    @(RQ13, 64), RD14
  EXTU.L   RD14, RD14
  MOV      RD14, RD2

L00E0049F:
  ADD      #144, R15
  POP      RQ13
  POP      RQ14
  RTS     

tk_fat_fclose:
  PUSH     LR
  PUSH     R14
  PUSH     R13
  ADD      #-136, R15
  MOV      RQ4, RQ13
  MOV.Q    @(RQ13, 16), RQ14
  MOV      RQ14, RQ4
  BSR      free
  MOV      #0, RD2

L00E004A0:
  ADD      #136, R15
  POP      RQ13
  POP      RQ14
  RET     

tk_fat_fgetc:
  PUSH     LR
  PUSH     R14
  PUSH     R13
  ADD      #-152, R15
  MOV.Q    RQ4, @(R15, 16)
  MOV      #28, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 8)
  MOV      #28, RQ4
  ADD      R15, RQ4
  MOV      #1, RD5
  MOV      #1, RD6
  MOV.Q    @(R15, 16), RQ7
  BSR      tk_fat_fread
  MOV      #28, RQ14
  ADD      R15, RQ14
  MOVU.B   @RQ14, RD13
  MOV.L    RD13, @(R15, 4)
  MOV      RD13, RD2

L00E004A1:
  ADD      #152, R15
  POP      RQ13
  POP      RQ14
  RET     

tk_fat_fputc:
  PUSH     LR
  PUSH     R14
  PUSH     R13
  ADD      #-152, R15
  MOV      RD4, RD14
  MOV.Q    RQ5, @(R15, 16)
  MOV      #28, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 8)
  EXTU.B   RD14, RD14
  MOV      #28, RQ13
  ADD      R15, RQ13
  MOVU.B   RD14, @RQ13
  MOV      RQ13, RQ4
  MOV      #1, RD5
  MOV      #1, RD6
  MOV.Q    @(R15, 16), RQ7
  BSR      tk_fat_fwrite
  MOV      RD14, RD2

L00E004A2:
  ADD      #152, R15
  POP      RQ13
  POP      RQ14
  RET     

tk_fopen:
  PUSH     LR
  BSR      L00C0004E
  ADD      #-128, R15
  MOV      RQ4, RQ13
  MOV.Q    RQ5, @(R15, 48)
  BSR      tk_vfile_init
  MOV.B    @RQ13, RD14
  CMP/EQ   #46, RD14
  BT       L0080079A
  MOV.B    @(RQ13, 1), RD14
  CMP/EQ   #47, RD14
  BT       L0080079A
  BRA      L0080079B

L0080079B:
  ADD      #2, RQ13

L0080079A:
  MOV.B    @RQ13, RD14
  CMP/EQ   #47, RD14
  BT       L0080079C
  ADD      #1, RQ13

L0080079C:
  MOV.Q    tk_vf_mount, RQ8
  MOV      RQ8, RQ11

L0080079D:
  MOV      #0, RQ31
  CMPQ/EQ  RQ31, RQ11
  BT       L0080079E
  MOV.Q    @RQ11, RQ12
  MOV.Q    @(RQ12, 24), RQ10
  MOV      RQ11, RQ4
  MOV      RQ13, RQ5
  MOV.Q    @(R15, 48), RQ6
  JSR      RQ10
  MOV      RQ2, RQ12
  MOV      RQ12, RQ9
  MOV      #0, RQ30
  CMPQ/EQ  RQ30, RQ9
  BT       L0080079F
  MOV      RQ9, RQ2
  BRA      L00E004A3

L0080079F:
  MOV.Q    @(RQ11, 8), RQ11
  BRA      L0080079D

L0080079E:
  MOV      L00C00181, RQ4
  MOV      RQ13, RQ5
  BSR      tk_printf
  MOV      #0, RQ12
  MOV      RQ12, RQ2

L00E004A3:
  ADD      #128, R15
  BRA      L00C0004F

tk_fread:
  PUSH     LR
  BSR      L00C0008B
  ADD      #-152, R15
  MOV      RQ4, RQ10
  MOV      RD5, RD9
  MOV      RD6, RD8
  MOV      RQ7, RQ12
  MOV.Q    @RQ12, RQ13
  MOV.Q    @(RQ13, 64), RQ14
  MOV      #0, RQ31
  CMPQ/EQ  RQ31, RQ14
  BT       L008007A0
  MOV.Q    @RQ12, RQ13
  MOV.Q    @(RQ13, 64), RQ14
  MOV      RQ10, RQ4
  MOV      RD9, RD5
  MOV      RD8, RD6
  MOV      RQ12, RQ7
  JSR      RQ14
  MOV      RD2, RD11
  MOV      RD11, RD2
  BRA      L00E004A4

L008007A0:
  MOV      #-1, RD2

L00E004A4:
  ADD      #152, R15
  BRA      L00C0008C

tk_fwrite:
  PUSH     LR
  BSR      L00C0008B
  ADD      #-152, R15
  MOV      RQ4, RQ10
  MOV      RD5, RD9
  MOV      RD6, RD8
  MOV      RQ7, RQ12
  MOV.Q    @RQ12, RQ13
  MOV.Q    @(RQ13, 72), RQ14
  MOV      #0, RQ31
  CMPQ/EQ  RQ31, RQ14
  BT       L008007A1
  MOV.Q    @RQ12, RQ13
  MOV.Q    @(RQ13, 72), RQ14
  MOV      RQ10, RQ4
  MOV      RD9, RD5
  MOV      RD8, RD6
  MOV      RQ12, RQ7
  JSR      RQ14
  MOV      RD2, RD11
  MOV      RD11, RD2
  BRA      L00E004A5

L008007A1:
  MOV      #-1, RD2

L00E004A5:
  ADD      #152, R15
  BRA      L00C0008C

tk_fseek:
  PUSH     LR
  BSR      L00C0003D
  ADD      #-144, R15
  MOV      RQ4, RQ12
  MOV      RD5, RD10
  MOV      RD6, RD9
  MOV.Q    @RQ12, RQ13
  MOV.Q    @(RQ13, 80), RQ14
  MOV      #0, RQ8
  CMPQ/EQ  RQ8, RQ14
  BT       L008007A2
  MOV.Q    @RQ12, RQ13
  MOV.Q    @(RQ13, 80), RQ14
  MOV      RQ12, RQ4
  MOV      RD10, RD5
  MOV      RD9, RD6
  JSR      RQ14
  MOV      RD2, RD11
  MOV      RD11, RD2
  BRA      L00E004A6

L008007A2:
  MOV      #-1, RD2

L00E004A6:
  ADD      #144, R15
  BRA      L00C0003E

tk_ftell:
  PUSH     LR
  BSR      L00C00072
  ADD      #-144, R15
  MOV      RQ4, RQ12
  MOV.Q    @RQ12, RQ13
  MOV.Q    @(RQ13, 88), RQ14
  MOV      #0, RQ10
  CMPQ/EQ  RQ10, RQ14
  BT       L008007A3
  MOV.Q    @RQ12, RQ13
  MOV.Q    @(RQ13, 88), RQ14
  MOV      RQ12, RQ4
  JSR      RQ14
  MOV      RD2, RD11
  MOV      RD11, RD2
  BRA      L00E004A7

L008007A3:
  MOV      #-1, RD2

L00E004A7:
  ADD      #144, R15
  BRA      L00C00074

tk_fclose:
  PUSH     LR
  BSR      L00C00072
  ADD      #-144, R15
  MOV      RQ4, RQ12
  MOV.Q    @RQ12, RQ13
  MOV.Q    @(RQ13, 96), RQ14
  MOV      #0, RQ10
  CMPQ/EQ  RQ10, RQ14
  BT       L008007A4
  MOV.Q    @RQ12, RQ13
  MOV.Q    @(RQ13, 96), RQ14
  MOV      RQ12, RQ4
  JSR      RQ14
  MOV      RD2, RD11
  MOV      RD11, RD2
  BRA      L00E004A8

L008007A4:
  MOV      #-1, RD2

L00E004A8:
  ADD      #144, R15
  BRA      L00C00074

_fcn_clz64:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-88, R15
  MOV.Q    RQ4, @(R15, 56)
  MOV      #0, RD13
  EXTU.L   RD13, RD13
  MOV      RD13, RD12
  MOV      #0, RD13
  EXTU.L   RD13, RD13
  MOV      RD13, RD11
  MOV      #0, RD13
  EXTU.L   RD13, RD13
  MOV      RD13, RD10
  MOV      #0, RQ9
  MOV      #0xFFFF0000, RQ9
  MOV      #-1, RQ9
  MOV      RQ9, RQ8
  MOV.Q    @(R15, 56), RQ31
  MOV      #0, RD14
  MOV.L    RD12, @(R15, 52)
  MOV.L    RD11, @(R15, 48)
  MOV.L    RD10, @(R15, 44)
  MOV.Q    RQ9, @(R15, 8)
  MOV.Q    RQ8, @(R15, 32)
  MOV.Q    RQ31, @(R15, 24)
  MOV      #0, RQ30
  MOV      #0xFFFF0000, RQ30
  MOV      #-1, RQ30
  TST.Q    RQ30, RQ31
  BT       L008007A7
  ADD      #32, RD14
  MOV      #32, RD29
  MOV.Q    @(R15, 24), RQ28
  SHLD.Q   RQ28, RD29, RQ28
  MOV.Q    RQ28, @(R15, 24)

L008007A7:
  MOV.Q    @(R15, 24), RQ27
  MOV      #0, RQ26
  MOV      #0, RQ26
  MOV      #0xFFFF0000, RQ26
  MOV      #-1, RQ26
  TST.Q    RQ26, RQ27
  BT       L008007A8
  ADD      #16, RD14
  MOV.Q    @(R15, 24), RQ25
  SHLD.Q   #16, RQ25
  MOV.Q    RQ25, @(R15, 24)

L008007A8:

L008007A9:
  MOV.Q    @(R15, 24), RQ7
  MOV      #0, RQ6
  MOV      #0, RQ6
  MOV      #0xFF000000, RQ6
  MOV      #-256, RQ6
  TST.Q    RQ6, RQ7
  BT       L008007AA
  ADD      #8, RD14
  MOV.Q    @(R15, 24), RQ5
  SHLD.Q   #8, RQ5
  MOV.Q    RQ5, @(R15, 24)
  BRA      L008007A9

L008007AA:

L008007AB:
  MOV.Q    @(R15, 24), RQ4
  MOV      #0, RQ3
  MOV      #0, RQ3
  MOV      #0x80000000, RQ3
  MOV      #-32768, RQ3
  TST.Q    RQ3, RQ4
  BT       L008007AC
  ADD      #1, RD14
  MOV.Q    @(R15, 24), RQ2
  MOV.Q    @(R15, 24), RQ23
  ADD      RQ2, RQ23, RQ22
  MOV.Q    RQ2, @(R15, 8)
  MOV.Q    RQ22, @(R15, 24)
  BRA      L008007AB

L008007AC:
  MOV      RD14, RD2

L00E004A9:
  ADD      #88, R15
  BRA      L00C00027

__udivdi3:
  PUSH     LR
  BSR      L00C0001F
  ADD      #-104, R15
  MOV.Q    RQ4, @(R15, 72)
  MOV.Q    RQ5, @(R15, 64)
  MOV.Q    @(R15, 64), RQ12
  MOV      #0, RQ11
  CMPQ/EQ  RQ11, RQ12
  BT       L008007AD
  MOV.Q    @(R15, 72), RQ10
  MOV      #0, RQ9
  CMPQ/EQ  RQ9, RQ10
  BT       L008007AD
  BRA      L008007AE

L008007AD:
  MOV      #0, RQ8
  EXTS.L   RQ8, RQ8
  MOV.Q    RQ8, @(R15, 32)
  MOV      RQ8, RQ2
  BRA      L00E004AA

L008007AE:
  MOV.Q    @(R15, 64), RQ4
  BSR      _fcn_clz64
  MOV      RD2, RD13
  MOV.Q    @(R15, 72), RQ4
  BSR      _fcn_clz64
  MOV      RD2, RD31
  SUB      RD13, RD31, RD30
  EXTU.B   RD30, RD14
  MOV.L    RD31, @(R15, 24)
  CMP/GE   #63, RD14
  BT       L008007AF
  CMP/EQ   #63, RD14
  BT       L008007B0
  MOV.Q    @(R15, 72), RQ2
  BRA      L00E004AA

L008007B0:
  MOV      #0, RQ29
  EXTS.L   RQ29, RQ29
  MOV.Q    RQ29, @(R15, 32)
  MOV      RQ29, RQ2
  BRA      L00E004AA

L008007AF:
  ADD      #1, RD14
  MOV      #64, RD28
  SUB      RD28, RD14, RD13
  MOV.Q    @(R15, 72), RQ27
  EXTS.L   RD13, RQ26
  SHAD.Q   RQ27, RQ26, RQ25
  MOV.Q    @(R15, 72), RQ27
  EXTU.B   RD14, RQ26
  MOV      RQ27, RQ29
  SHLR.Q   RQ29, RQ26, RQ29
  MOV      #0, RD30
  MOV.L    RD30, @(R15, 40)
  MOV.Q    RQ29, @(R15, 48)
  MOV.Q    RQ27, @(R15, 32)
  MOV.Q    RQ26, @(R15, 8)
  MOV.Q    RQ25, @(R15, 56)

L008007B1:
  MOV      RD14, RD13
  ADD      #-1, RD14
  CMP/EQ   RD13
  BT       L008007B2
  MOV.Q    @(R15, 48), RQ25
  SHLD.Q   RQ25, #1, RQ26
  MOV.Q    @(R15, 56), RQ27
  MOV      #63, RD28
  SHLR.Q   RQ27, RD28, RQ27
  OR       RQ26, RQ27, RQ25
  MOV.Q    @(R15, 56), RQ29
  SHLD.Q   RQ29, #1, RQ26
  MOV      RQ26, RQ27
  MOV.L    @(R15, 40), RD26
  EXTS.L   RQ26, RQ26
  OR       RQ27, RQ26, RQ29
  MOV.Q    @(R15, 64), RQ30
  SUB      RQ30, RQ25, RQ26
  MOV      #1, RQ31
  SUB      RQ26, RQ31, RQ27
  MOV      RQ27, RQ8
  MOV      RQ8, RQ26
  SHAR.Q   RQ26, RD28, RQ26
  EXTS.L   RQ26, RD9
  AND      RD9, #1, RD10
  EXTS.L   RD9, RQ27
  AND      RQ30, RQ27, RQ26
  SUB      RQ26, RQ25
  MOV.L    RD10, @(R15, 40)
  MOV.L    RD9, @(R15, 44)
  MOV.Q    RQ29, @(R15, 56)
  MOV.Q    RQ27, @(R15, 8)
  MOV.Q    RQ26, @(R15, 32)
  MOV.Q    RQ25, @(R15, 48)
  BRA      L008007B1

L008007B2:
  MOV.Q    @(R15, 56), RQ7
  SHLD.Q   RQ7, #1, RQ6
  MOV      RQ6, RQ5
  MOV.L    @(R15, 40), RD6
  EXTS.L   RQ6, RQ6
  OR       RQ5, RQ6, RQ7
  MOV.Q    RQ7, @(R15, 56)
  MOV.Q    RQ6, @(R15, 32)
  MOV.Q    RQ5, @(R15, 8)
  MOV      RQ7, RQ2

L00E004AA:
  ADD      #104, R15
  BRA      L00C00027

__sdivdi3:
  PUSH     LR
  BSR      L00C00037
  ADD      #-136, R15
  MOV.Q    RQ4, @(R15, 40)
  MOV.Q    RQ5, @(R15, 32)
  MOV.Q    @(R15, 40), RQ14
  MOV      #63, RD13
  SHAR.Q   RQ14, RD13, RQ14
  MOV.Q    @(R15, 32), RQ12
  SHAR.Q   RQ12, RD13, RQ12
  MOV.Q    @(R15, 40), RQ11
  XOR      RQ11, RQ14, RQ10
  SUB      RQ10, RQ14, RQ11
  MOV.Q    @(R15, 32), RQ9
  XOR      RQ9, RQ12, RQ10
  SUB      RQ10, RQ12, RQ9
  XOR      RQ12, RQ14
  MOV.Q    RQ14, @(R15, 24)
  MOV.Q    RQ12, @(R15, 16)
  MOV.Q    RQ11, @(R15, 40)
  MOV.Q    RQ10, @(R15, 8)
  MOV.Q    RQ9, @(R15, 32)
  MOV      RQ11, RQ4
  MOV      RQ9, RQ5
  BSR      __udivdi3
  MOV      RQ2, RQ10
  XOR      RQ10, RQ14, RQ13
  SUB      RQ13, RQ14, RQ10
  MOV.Q    RQ10, @(R15, 8)
  MOV      RQ10, RQ2

L00E004AB:
  ADD      #136, R15
  BRA      L00C00038

tk_putc:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  ADD      #-152, R15
  MOV.L    RD4, @(R15, 28)

L008007B3:
  MOV      #0xA000E018, RQ14
  MOV      #0x0000A000, RQ14
  MOVU.L   @RQ14, RD13
  TST      #8, RD13
  BT       L008007B4
  BRA      L008007B3

L008007B4:
  MOV      #0xA000E014, RQ14
  MOV      #0x0000A000, RQ14
  MOV.L    @(R15, 28), RD12
  MOVU.L   RD12, @RQ14

L00E004AC:
  ADD      #152, R15
  BRA      L00C0005F

tk_kbhit:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  ADD      #-136, R15
  MOV      #0xA000E018, RQ14
  MOV      #0x0000A000, RQ14
  MOVU.L   @RQ14, RD13
  AND      RD13, #1, RD12
  MOV      RD12, RD2

L00E004AD:
  ADD      #136, R15
  BRA      L00C0005F

tk_getch:
  PUSH     LR
  PUSH     R14
  PUSH     R13
  ADD      #-136, R15

L008007B5:
  MOV      #0xA000E018, RQ14
  MOV      #0x0000A000, RQ14
  MOVU.L   @RQ14, RD13
  TST      #1, RD13
  BT       L008007B6
  BSR      sleep_0
  BRA      L008007B5

L008007B6:
  MOV      #0xA000E010, RQ14
  MOV      #0x0000A000, RQ14
  MOVU.L   @RQ14, RD13
  MOV      RD13, RD2

L00E004AE:
  ADD      #136, R15
  POP      RQ13
  POP      RQ14
  RET     

tk_puts_n:
  PUSH     LR
  BSR      L00C00072
  ADD      #-128, R15
  MOV      RQ4, RQ12
  MOV      RD5, RD11
  MOV      RQ12, RQ14

L008007BB:
  MOV      RD11, RD13
  ADD      #-1, RD11
  CMP/EQ   RD13
  BT       L008007BC
  MOV      RQ14, RQ10
  ADD      #1, RQ14
  MOV.B    @RQ10, RD13
  MOV      RD13, RD4
  BSR      tk_putc
  BRA      L008007BB

L008007BC:

L00E004AF:
  ADD      #128, R15
  BRA      L00C00074

__moddi3:
  PUSH     LR
  BSR      L00C00072
  ADD      #-112, R15
  MOV      RD4, RD12
  MOV      RD5, RD13
  MOV      RD13, RD5
  MOV      RD12, RD4
  MOV      __udivsi3, R3
  JSR      R3
  MOV      RD2, RD14
  MUL.L    RD14, RD13, RD11
  SUB      RD12, RD11, RD10
  MOV      RD10, RD2

L00E004B0:
  ADD      #112, R15
  BRA      L00C00074

__umodsi3:
  PUSH     LR
  BSR      L00C00072
  ADD      #-112, R15
  MOV      RD4, RD12
  MOV      RD5, RD13
  MOV      RD13, RD5
  MOV      RD12, RD4
  BSR      __udivsi3
  MOV      RD2, RD14
  MUL.L    RD14, RD13, RD11
  SUB      RD12, RD11, RD10
  MOV      RD10, RD2

L00E004B1:
  ADD      #112, R15
  BRA      L00C00074

__smodsi3:
  PUSH     LR
  BSR      L00C00072
  ADD      #-112, R15
  MOV      RD4, RD12
  MOV      RD5, RD13
  MOV      RD13, RD5
  MOV      RD12, RD4
  MOV      __sdivsi3, R3
  JSR      R3
  MOV      RD2, RD14
  MUL.L    RD14, RD13, RD11
  SUB      RD12, RD11, RD10
  MOV      RD10, RD2

L00E004B2:
  ADD      #112, R15
  BRA      L00C00074

__shllli:
  PUSH     LR
  BSR      L00C00049
  ADD      #-104, R15
  MOV.Q    RQ4, @(R15, 40)
  MOV.L    RD5, @(R15, 36)
  MOV.L    @(R15, 36), RD13
  TST      #31, RD13
  BT       L008007C6
  MOV.L    @(R15, 36), RD12
  CMP/EQ   RD12
  BT       L008007C7
  MOV.Q    @(R15, 40), RQ2
  BRA      L00E004B3

L008007C7:
  MOV.L    @(R15, 36), RD11
  CMP/EQ   #32, RD11
  BT       L008007C8
  MOV      #0, RD10
  MOV      #24, RQ9
  ADD      R15, RQ9
  EXTU.L   RD10, RD10
  MOV.L    RD10, @RQ9
  MOV      #40, RQ8
  ADD      R15, RQ8
  MOV.L    @RQ8, RD14
  EXTU.L   RD14, RD14
  EXTU.L   RD14, RD14
  MOV.L    RD14, @(RQ9, 4)
  MOV.Q    @(R15, 24), RQ2
  BRA      L00E004B3

L008007C8:
  MOV      #0, RD31
  MOV      #24, RQ30
  ADD      R15, RQ30
  EXTU.L   RD31, RD31
  MOV.L    RD31, @RQ30
  EXTU.L   RD31, RD31
  MOV.L    RD31, @(RQ30, 4)
  MOV.Q    @(R15, 24), RQ2
  BRA      L00E004B3

L008007C6:
  MOV      #40, RQ29
  ADD      R15, RQ29
  MOV.L    @RQ29, RD14
  EXTU.L   RD14, RD14
  MOV.L    @(R15, 36), RD6
  SHLD     RD14, RD6, RD7
  MOV      #24, RQ5
  ADD      R15, RQ5
  EXTU.L   RD7, RD7
  MOV.L    RD7, @RQ5
  MOV.L    @(RQ29, 4), RD14
  EXTU.L   RD14, RD14
  SHLD     RD14, RD6, RD7
  MOV.L    @RQ29, RD14
  EXTU.L   RD14, RD14
  MOV      #32, RD3
  SUB      RD3, RD6, RD4
  SHLD     RD14, RD4, RD2
  OR       RD7, RD2, RD14
  EXTU.L   RD14, RD14
  MOV.L    RD14, @(RQ5, 4)
  MOV.Q    @(R15, 24), RQ2

L00E004B3:
  ADD      #104, R15
  BRA      L00C0004A

__shrlli:
  PUSH     LR
  BSR      L00C00049
  ADD      #-104, R15
  MOV.Q    RQ4, @(R15, 40)
  MOV.L    RD5, @(R15, 36)
  MOV.L    @(R15, 36), RD13
  TST      #31, RD13
  BT       L008007C9
  MOV.L    @(R15, 36), RD12
  CMP/EQ   RD12
  BT       L008007CA
  MOV.Q    @(R15, 40), RQ2
  BRA      L00E004B4

L008007CA:
  MOV.L    @(R15, 36), RD11
  CMP/EQ   #32, RD11
  BT       L008007CB
  MOV      #40, RQ10
  ADD      R15, RQ10
  MOV.L    @(RQ10, 4), RD14
  EXTU.L   RD14, RD14
  MOV      #24, RQ9
  ADD      R15, RQ9
  EXTU.L   RD14, RD14
  MOV.L    RD14, @RQ9
  MOV      #0, RD8
  EXTU.L   RD8, RD8
  MOV.L    RD8, @(RQ9, 4)
  MOV.Q    @(R15, 24), RQ2
  BRA      L00E004B4

L008007CB:
  MOV      #0, RD31
  MOV      #24, RQ30
  ADD      R15, RQ30
  EXTU.L   RD31, RD31
  MOV.L    RD31, @RQ30
  EXTU.L   RD31, RD31
  MOV.L    RD31, @(RQ30, 4)
  MOV.Q    @(R15, 24), RQ2
  BRA      L00E004B4

L008007C9:
  MOV      #40, RQ29
  ADD      R15, RQ29
  MOV.L    @(RQ29, 4), RD14
  EXTU.L   RD14, RD14
  MOV.L    @(R15, 36), RD6
  SHLR     RD14, RD6, RD7
  MOV      #24, RQ5
  ADD      R15, RQ5
  EXTU.L   RD7, RD7
  MOV.L    RD7, @(RQ5, 4)
  MOV.L    @RQ29, RD14
  EXTU.L   RD14, RD14
  SHLR     RD14, RD6, RD7
  MOV.L    @(RQ29, 4), RD14
  EXTU.L   RD14, RD14
  MOV      #32, RD3
  SUB      RD3, RD6, RD4
  SHLD     RD14, RD4, RD2
  OR       RD7, RD2, RD14
  EXTU.L   RD14, RD14
  MOV.L    RD14, @RQ5
  MOV.Q    @(R15, 24), RQ2

L00E004B4:
  ADD      #104, R15
  BRA      L00C0004A

L00C00190:
  PUSH     R31
  PUSH     R30
  PUSH     R29
  PUSH     R28
  PUSH     R27
  PUSH     R26
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  PUSH     R9
  PUSH     R8
  RTS     

__sarlli:
  PUSH     LR
  BSR      L00C00190
  ADD      #-80, R15
  MOV.Q    RQ4, @(R15, 40)
  MOV.L    RD5, @(R15, 36)
  MOV      #0, RD13
  MOV      #40, RQ12
  ADD      R15, RQ12
  MOV.L    @(RQ12, 4), RD14
  EXTU.L   RD14, RD14
  TST      #-2147483648, RD14
  BT       L008007CC
  MOV      #-1, RD13

L008007CC:
  MOV.L    @(R15, 36), RD11
  TST      #31, RD11
  BT       L008007CD
  MOV.L    @(R15, 36), RD10
  CMP/EQ   RD10
  BT       L008007CE
  MOV.Q    @(R15, 40), RQ2
  BRA      L00E004B5

L008007CE:
  MOV.L    @(R15, 36), RD9
  CMP/EQ   #32, RD9
  BT       L008007CF
  MOV      #40, RQ8
  ADD      R15, RQ8
  MOV.L    @(RQ8, 4), RD14
  EXTU.L   RD14, RD14
  MOV      #24, RQ31
  ADD      R15, RQ31
  EXTU.L   RD14, RD14
  MOV.L    RD14, @RQ31
  EXTU.L   RD13, RD13
  MOV.L    RD13, @(RQ31, 4)
  MOV.Q    @(R15, 24), RQ2
  BRA      L00E004B5

L008007CF:
  CMP/EQ   RD13
  BT       L008007D0
  MOV      #24, RQ30
  ADD      R15, RQ30
  EXTU.L   RD13, RD13
  MOV.L    RD13, @RQ30
  EXTU.L   RD13, RD13
  MOV.L    RD13, @(RQ30, 4)
  MOV.Q    @(R15, 24), RQ2
  BRA      L00E004B5

L008007D0:
  MOV      #0, RD29
  MOV      #24, RQ28
  ADD      R15, RQ28
  EXTU.L   RD29, RD29
  MOV.L    RD29, @RQ28
  EXTU.L   RD29, RD29
  MOV.L    RD29, @(RQ28, 4)
  MOV.Q    @(R15, 24), RQ2
  BRA      L00E004B5

L008007CD:
  MOV      #40, RQ27
  ADD      R15, RQ27
  MOV.L    @(RQ27, 4), RD14
  EXTU.L   RD14, RD14
  MOV.L    @(R15, 36), RD7
  SHLR     RD14, RD7, RD26
  MOV      #32, RD6
  SUB      RD6, RD7, RD14
  SHLD     RD13, RD14, RD5
  OR       RD26, RD5, RD14
  MOV      #24, RQ4
  ADD      R15, RQ4
  EXTU.L   RD14, RD14
  MOV.L    RD14, @(RQ4, 4)
  MOV.L    @RQ27, RD14
  EXTU.L   RD14, RD14
  SHLR     RD14, RD7, RD26
  MOV.L    @(RQ27, 4), RD14
  EXTU.L   RD14, RD14
  SUB      RD6, RD7, RD5
  SHLD     RD14, RD5, RD3
  OR       RD26, RD3, RD14
  EXTU.L   RD14, RD14
  MOV.L    RD14, @RQ4
  MOV.Q    @(R15, 24), RQ2

L00E004B5:
  ADD      #80, R15

L00C00191:
  POP      RQ8
  POP      RQ9
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  POP      RQ26
  POP      RQ27
  POP      RQ28
  POP      RQ29
  POP      RQ30
  POP      RQ31
  RET     

L00C00193:
  PUSH     R31
  PUSH     R30
  PUSH     R29
  PUSH     R28
  PUSH     R27
  PUSH     R26
  PUSH     R25
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  PUSH     R9
  PUSH     R8
  RTS     

__udivlli:
  PUSH     LR
  BSR      L00C00193
  ADD      #-104, R15
  MOV.Q    RQ4, @(R15, 72)
  MOV.Q    RQ5, @(R15, 64)
  MOV      #64, RQ11
  ADD      R15, RQ11
  MOV.L    @(RQ11, 4), RD14
  EXTU.L   RD14, RD14
  CMP/EQ   RD14
  BT       L008007D1
  MOV      #72, RQ10
  ADD      R15, RQ10
  MOV.L    @(RQ10, 4), RD14
  EXTU.L   RD14, RD14
  CMP/EQ   RD14
  BT       L008007D2
  MOV      #72, RQ9
  ADD      R15, RQ9
  MOV.L    @RQ9, RD14
  EXTU.L   RD14, RD14
  MOV      #64, RQ8
  ADD      R15, RQ8
  MOV.L    @RQ8, RD13
  EXTU.L   RD13, RD13
  MOV      RD13, RD5
  MOV      RD14, RD4
  BSR      __udivsi3
  MOV      RD2, RD12
  MOV      #48, RQ31
  ADD      R15, RQ31
  EXTU.L   RD12, RD12
  MOV.L    RD12, @RQ31
  MOV      #0, RD30
  EXTU.L   RD30, RD30
  MOV.L    RD30, @(RQ31, 4)
  MOV.Q    @(R15, 48), RQ2
  BRA      L00E004B6

L008007D2:
  MOV      #72, RQ29
  ADD      R15, RQ29
  MOV.L    @(RQ29, 4), RD14
  EXTU.L   RD14, RD14
  MOV      #64, RQ28
  ADD      R15, RQ28
  MOV.L    @RQ28, RD13
  EXTU.L   RD13, RD13
  MOV      RD13, RD5
  MOV      RD14, RD4
  BSR      __udivsi3
  MOV      RD2, RD12
  MOV      #48, RQ27
  ADD      R15, RQ27
  EXTU.L   RD12, RD12
  MOV.L    RD12, @(RQ27, 4)
  MOV.L    @RQ29, RD14
  EXTU.L   RD14, RD14
  MOV.L    @RQ28, RD13
  EXTU.L   RD13, RD13
  MOV      RD13, RD5
  MOV      RD14, RD4
  BSR      __udivsi3
  MOV      RD2, RD12
  EXTU.L   RD12, RD12
  MOV.L    RD12, @RQ27
  MOV.L    @(RQ29, 4), RD14
  EXTU.L   RD14, RD14
  MOV.L    @RQ28, RD13
  EXTU.L   RD13, RD13
  MOV      RD13, RD5
  MOV      RD14, RD4
  BSR      __umodsi3
  MOV      RD2, RD12
  MOV      #40, RQ26
  ADD      R15, RQ26
  EXTU.L   RD12, RD12
  MOV.L    RD12, @(RQ26, 4)
  MOV.L    @RQ29, RD14
  EXTU.L   RD14, RD14
  MOV.L    @RQ28, RD13
  EXTU.L   RD13, RD13
  MOV      RD13, RD5
  MOV      RD14, RD4
  BSR      __umodsi3
  MOV      RD2, RD12
  EXTU.L   RD12, RD12
  MOV.L    RD12, @RQ26
  MOV.L    @(RQ26, 4), RD14
  EXTU.L   RD14, RD14
  EXTU.L   RD14, RQ25
  MOV.L    @RQ28, RD14
  EXTU.L   RD14, RD14
  EXTU.L   RD14, RQ30
  MOV      RQ25, RQ31
  MOV      RQ30, RQ5
  MOV      RQ31, RQ4
  BSR      __umulsq
  MOV      RQ2, RQ31
  MOV      RQ27, RQ8
  MOV.Q    @RQ8, RQ25
  ADD      RQ25, RQ31, RQ30
  MOV      RQ27, RQ8
  MOV.Q    RQ30, @RQ8
  MOV.Q    RQ31, @(R15, 56)
  MOV.Q    @(R15, 48), RQ2
  BRA      L00E004B6

L008007D1:
  MOV      #72, RQ7
  ADD      R15, RQ7
  MOV.L    @(RQ7, 4), RD14
  EXTU.L   RD14, RD14
  CMP/EQ   RD14
  BT       L008007D3
  MOV      #0, RD6
  MOV      #48, RQ5
  ADD      R15, RQ5
  EXTU.L   RD6, RD6
  MOV.L    RD6, @RQ5
  EXTU.L   RD6, RD6
  MOV.L    RD6, @(RQ5, 4)
  MOV.Q    @(R15, 48), RQ2
  BRA      L00E004B6

L008007D3:
  MOV      #64, RQ4
  ADD      R15, RQ4
  MOV.L    @(RQ4, 4), RD14
  EXTU.L   RD14, RD14
  MOV      #72, RQ3
  ADD      R15, RQ3
  MOV.L    @(RQ3, 4), RD13
  EXTU.L   RD13, RD13
  CMP/HI   RD13, RD14
  BT       L008007D4
  MOV      #0, RD2
  MOV      #48, RQ23
  ADD      R15, RQ23
  EXTU.L   RD2, RD2
  MOV.L    RD2, @RQ23
  EXTU.L   RD2, RD2
  MOV.L    RD2, @(RQ23, 4)
  MOV.Q    @(R15, 48), RQ2
  BRA      L00E004B6

L008007D4:
  MOV      #72, RQ25
  ADD      R15, RQ25
  MOV.L    @(RQ25, 4), RD14
  EXTU.L   RD14, RD14
  MOV      #64, RQ26
  ADD      R15, RQ26
  MOV.L    @(RQ26, 4), RD13
  EXTU.L   RD13, RD13
  MOV      RD13, RD5
  MOV      RD14, RD4
  BSR      __udivsi3
  MOV      RD2, RD12
  MOV      #48, RQ27
  ADD      R15, RQ27
  EXTU.L   RD12, RD12
  MOV.L    RD12, @RQ27
  MOV      #0, RD28
  EXTU.L   RD28, RD28
  MOV.L    RD28, @(RQ27, 4)
  MOV.Q    @(R15, 48), RQ2

L00E004B6:
  ADD      #104, R15

L00C00194:
  POP      RQ8
  POP      RQ9
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  POP      RQ25
  POP      RQ26
  POP      RQ27
  POP      RQ28
  POP      RQ29
  POP      RQ30
  POP      RQ31
  RET     

__sdivlli:
  PUSH     LR
  BSR      L00C0008B
  ADD      #-120, R15
  MOV.Q    RQ4, @(R15, 40)
  MOV.Q    RQ5, @(R15, 32)
  MOV      #0, RD14
  MOV.Q    @(R15, 40), RQ13
  MOV.Q    RQ13, @(R15, 24)
  MOV      #0, RQ12
  MOV      #0, RQ12
  MOV      #0x80000000, RQ12
  MOV      #-32768, RQ12
  TST.Q    RQ12, RQ13
  BT       L008007D5
  MOV.Q    @(R15, 24), RQ11
  NEG      RQ11, RQ11
  MOV      #1, RD14
  MOV.Q    RQ11, @(R15, 24)

L008007D5:
  MOV.Q    @(R15, 40), RQ10
  MOV.Q    RQ10, @(R15, 16)
  MOV      #0, RQ9
  MOV      #0, RQ9
  MOV      #0x80000000, RQ9
  MOV      #-32768, RQ9
  TST.Q    RQ9, RQ10
  BT       L008007D6
  MOV.Q    @(R15, 16), RQ8
  NEG      RQ8, RQ8
  XOR      #1, RD14
  MOV.Q    RQ8, @(R15, 16)

L008007D6:
  MOV.Q    @(R15, 24), RQ31
  MOV.Q    @(R15, 16), RQ5
  MOV      RQ31, RQ4
  BSR      __udivsq
  MOV      RQ2, RQ31
  MOV.Q    RQ31, @(R15, 8)
  CMP/EQ   RD14
  BT       L008007D7
  MOV.Q    @(R15, 8), RQ7
  NEG      RQ7, RQ7
  MOV.Q    RQ7, @(R15, 8)

L008007D7:
  MOV.Q    @(R15, 8), RQ2

L00E004B7:
  ADD      #120, R15
  BRA      L00C0008C

__udivsq:
  PUSH     LR
  ADD      #-168, R15
  MOV.Q    RQ4, @(R15, 24)
  MOV.Q    RQ5, @(R15, 16)
  MOV.Q    @(R15, 24), RQ4
  MOV.Q    @(R15, 16), RQ5
  BSR      __udivdi3
  MOV.Q    RQ2, @(R15, 8)
  MOV.Q    @(R15, 8), RQ2

L00E004B8:
  ADD      #168, R15
  RET     

__sdivsq:
  PUSH     LR
  ADD      #-168, R15
  MOV.Q    RQ4, @(R15, 24)
  MOV.Q    RQ5, @(R15, 16)
  MOV.Q    @(R15, 24), RQ4
  MOV.Q    @(R15, 16), RQ5
  BSR      __sdivdi3
  MOV.Q    RQ2, @(R15, 8)
  MOV.Q    @(R15, 8), RQ2

L00E004B9:
  ADD      #168, R15
  RET     

__umodsq:
  PUSH     LR
  BSR      L00C00193
  ADD      #-104, R15
  MOV.Q    RQ4, @(R15, 72)
  MOV.Q    RQ5, @(R15, 64)
  MOV.L    @(R15, 72), RD14
  EXTU.L   RD14, RD14
  EXTU.L   RD14, RQ13
  MOV.Q    @(R15, 72), RQ12
  MOV.Q    RQ13, @(R15, 32)
  MOV.Q    RQ12, @(R15, 24)
  CMPQ/EQ  RQ12, RQ13
  BT       L008007D8
  MOV.L    @(R15, 64), RD14
  EXTU.L   RD14, RD14
  EXTU.L   RD14, RQ11
  MOV.Q    @(R15, 64), RQ10
  MOV.Q    RQ11, @(R15, 32)
  MOV.Q    RQ10, @(R15, 24)
  CMPQ/EQ  RQ10, RQ11
  BT       L008007D8
  BRA      L008007D9

L008007D9:
  MOV.Q    @(R15, 64), RQ9
  MOV      #1, RQ8
  SUB      RQ9, RQ8, RQ31
  AND      RQ9, RQ31, RQ30
  EXTU.L   RQ30, RD14
  MOV.Q    RQ31, @(R15, 32)
  MOV.Q    RQ30, @(R15, 24)
  CMP/EQ   RD14
  BT       L008007DA
  MOV.L    @(R15, 72), RD14
  EXTU.L   RD14, RD14
  MOV.Q    @(R15, 64), RQ29
  MOV      #1, RQ28
  SUB      RQ29, RQ28, RQ27
  EXTU.L   RQ27, RD26
  AND      RD14, RD26, RD25
  EXTU.L   RD25, RQ27
  MOV.Q    RQ27, @(R15, 32)
  MOV.L    RD26, @(R15, 20)
  MOV.L    RD25, @(R15, 16)
  MOV      RQ27, RQ2
  BRA      L00E004BA

L008007DA:
  MOV.L    @(R15, 72), RD14
  EXTU.L   RD14, RD14
  MOV.L    @(R15, 64), RD25
  EXTU.L   RD25, RD25
  MOV      RD25, RD5
  MOV      RD14, RD4
  BSR      __umodsi3
  MOV      RD2, RD26
  EXTU.L   RD26, RQ27
  MOV.Q    RQ27, @(R15, 32)
  MOV.L    RD26, @(R15, 16)
  MOV.L    RD25, @(R15, 20)
  MOV      RQ27, RQ2
  BRA      L00E004BA

L008007D8:
  MOV.Q    @(R15, 64), RQ7
  MOV      #1, RQ6
  SUB      RQ7, RQ6, RQ5
  AND      RQ7, RQ5, RQ4
  MOV.Q    RQ5, @(R15, 32)
  MOV.Q    RQ4, @(R15, 24)
  MOV      #0, RQ3
  CMPQ/EQ  RQ3, RQ4
  BT       L008007DB
  MOV.Q    @(R15, 64), RQ2
  MOV      #1, RQ23
  SUB      RQ2, RQ23, RQ22
  EXTU.L   RQ22, RD14
  EXTU.L   RD14, RQ21
  MOV.Q    @(R15, 72), RQ20
  AND      RQ20, RQ21, RQ22
  MOV.Q    RQ22, @(R15, 32)
  MOV.Q    RQ21, @(R15, 24)
  MOV      RQ22, RQ2
  BRA      L00E004BA

L008007DB:
  MOV.Q    @(R15, 72), RQ25
  MOV.Q    @(R15, 64), RQ5
  MOV      RQ25, RQ4
  BSR      __udivsq
  MOV      RQ2, RQ25
  MOV      RQ25, RQ26
  MOV.Q    @(R15, 64), RQ5
  MOV      RQ26, RQ4
  BSR      __umulsq
  MOV      RQ2, RQ26
  MOV.Q    @(R15, 72), RQ27
  SUB      RQ27, RQ26, RQ28
  MOV.Q    RQ28, @(R15, 48)
  MOV.Q    RQ26, @(R15, 32)
  MOV.Q    RQ25, @(R15, 56)
  MOV      #0, RQ29
  CMPQ/GT  RQ28, RQ29
  BT       L008007DC
  MOV.Q    @(R15, 64), RQ19
  MOV.Q    @(R15, 48), RQ18
  ADD      RQ19, RQ18
  MOV.Q    RQ18, @(R15, 48)

L008007DC:
  MOV.Q    @(R15, 48), RQ17
  MOV.Q    @(R15, 64), RQ16
  CMPQ/HI  RQ17, RQ16
  BT       L008007DD
  MOV.Q    @(R15, 64), RQ16
  MOV.Q    @(R15, 48), RQ17
  SUB      RQ16, RQ17
  MOV.Q    RQ17, @(R15, 48)
  CMPQ/HI  RQ17, RQ16
  BT       L008007DE
  MOV.L    @(R15, 64), RD14
  EXTU.L   RD14, RD14
  MOV.Q    @(R15, 64), RQ25
  MOV      #32, RD26
  SHLR.Q   RQ25, RD26, RQ25
  EXTU.L   RQ25, RD27
  MOV.L    @(R15, 72), RD28
  EXTU.L   RD28, RD28
  MOV.Q    @(R15, 72), RQ25
  SHLR.Q   RQ25, RD26, RQ25
  EXTU.L   RQ25, RD29
  MOV.L    RD29, @(R15, 12)
  MOV.L    RD28, @(R15, 16)
  MOV.L    RD27, @(R15, 20)
  MOV.Q    RQ25, @(R15, 32)
  MOV      L00C00196, RQ4
  MOV      RD29, RD5
  MOV      RD28, RD6
  MOV      RD27, RD7
  MOV      RD14, RD20
  BSR      tk_printf
  MOV.L    @(R15, 48), RD14
  EXTU.L   RD14, RD14
  MOV.Q    @(R15, 48), RQ25
  SHLR.Q   RQ25, RD26, RQ25
  EXTU.L   RQ25, RD27
  MOV.L    @(R15, 56), RD28
  EXTU.L   RD28, RD28
  MOV.Q    @(R15, 56), RQ25
  SHLR.Q   RQ25, RD26, RQ25
  EXTU.L   RQ25, RD29
  MOV.L    RD29, @(R15, 12)
  MOV.L    RD28, @(R15, 16)
  MOV.L    RD27, @(R15, 20)
  MOV.Q    RQ25, @(R15, 32)
  MOV      L00C00197, RQ4
  MOV      RD29, RD5
  MOV      RD28, RD6
  MOV      RD27, RD7
  MOV      RD14, RD20
  BSR      tk_printf
  BRK     
  MOV      #0, RQ25
  EXTS.L   RQ25, RQ25
  MOV.Q    RQ25, @(R15, 32)
  MOV      RQ25, RQ2
  BRA      L00E004BA

L008007DE:

L008007DD:
  MOV.Q    @(R15, 48), RQ2

L00E004BA:
  ADD      #104, R15
  BRA      L00C00194

L00C00198:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  PUSH     R9
  RTS     

tk_print_hex:
  PUSH     LR
  BSR      L00C00198
  ADD      #-104, R15
  MOV      RD4, RD12
  SHLD     RD12, #-28, RD14
  AND      RD14, #15, RD13
  MOV.Q    chrs, RQ11
  EXTS.L   RD13
  MOV.B    @(RQ11, RD13), RD14
  MOV      RD14, RD4
  BSR      tk_putc
  SHLD     RD12, #-24, RD14
  AND      RD14, #15, RD13
  MOV.Q    chrs, RQ10
  EXTS.L   RD13
  MOV.B    @(RQ10, RD13), RD14
  MOV      RD14, RD4
  BSR      tk_putc
  SHLD     RD12, #-20, RD14
  AND      RD14, #15, RD13
  MOV.Q    chrs, RQ9
  EXTS.L   RD13
  MOV.B    @(RQ9, RD13), RD14
  MOV      RD14, RD4
  BSR      tk_putc
  SHLD     RD12, #-16, RD14
  AND      RD14, #15, RD13
  MOV.Q    chrs, RQ11
  EXTS.L   RD13
  MOV.B    @(RQ11, RD13), RD14
  MOV      RD14, RD4
  BSR      tk_putc
  SHLD     RD12, #-12, RD14
  AND      RD14, #15, RD13
  MOV.Q    chrs, RQ10
  EXTS.L   RD13
  MOV.B    @(RQ10, RD13), RD14
  MOV      RD14, RD4
  BSR      tk_putc
  SHLD     RD12, #-8, RD14
  AND      RD14, #15, RD13
  MOV.Q    chrs, RQ9
  EXTS.L   RD13
  MOV.B    @(RQ9, RD13), RD14
  MOV      RD14, RD4
  BSR      tk_putc
  SHLD     RD12, #-4, RD14
  AND      RD14, #15, RD13
  MOV.Q    chrs, RQ11
  EXTS.L   RD13
  MOV.B    @(RQ11, RD13), RD14
  MOV      RD14, RD4
  BSR      tk_putc
  AND      RD12, #15, RD14
  MOV.Q    chrs, RQ10
  EXTS.L   RD14
  MOV.B    @(RQ10, RD14), RD13
  MOV      RD13, RD4
  BSR      tk_putc

L00E004BB:
  ADD      #104, R15

L00C0019A:
  POP      RQ9
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  RET     

tk_print_hex_n:
  PUSH     LR
  BSR      L00C00053
  ADD      #-56, R15
  MOV      RD4, RD11
  MOV      RD5, RD12
  CMP/GT   #7, RD12
  BT       L008007EC
  SHLD     RD11, #-28, RD14
  AND      RD14, #15, RD13
  MOV.Q    chrs, RQ10
  EXTS.L   RD13
  MOV.B    @(RQ10, RD13), RD14
  MOV      RD14, RD4
  BSR      tk_putc

L008007EC:
  CMP/GT   #6, RD12
  BT       L008007ED
  SHLD     RD11, #-24, RD14
  AND      RD14, #15, RD13
  MOV.Q    chrs, RQ9
  EXTS.L   RD13
  MOV.B    @(RQ9, RD13), RD14
  MOV      RD14, RD4
  BSR      tk_putc

L008007ED:
  CMP/GT   #5, RD12
  BT       L008007EE
  SHLD     RD11, #-20, RD14
  AND      RD14, #15, RD13
  MOV.Q    chrs, RQ8
  EXTS.L   RD13
  MOV.B    @(RQ8, RD13), RD14
  MOV      RD14, RD4
  BSR      tk_putc

L008007EE:
  CMP/GT   #4, RD12
  BT       L008007EF
  SHLD     RD11, #-16, RD14
  AND      RD14, #15, RD13
  MOV.Q    chrs, RQ31
  EXTS.L   RD13
  MOV.B    @(RQ31, RD13), RD14
  MOV      RD14, RD4
  BSR      tk_putc

L008007EF:
  CMP/GT   #3, RD12
  BT       L008007F0
  SHLD     RD11, #-12, RD14
  AND      RD14, #15, RD13
  MOV.Q    chrs, RQ30
  EXTS.L   RD13
  MOV.B    @(RQ30, RD13), RD14
  MOV      RD14, RD4
  BSR      tk_putc

L008007F0:
  CMP/GT   #2, RD12
  BT       L008007F1
  SHLD     RD11, #-8, RD14
  AND      RD14, #15, RD13
  MOV.Q    chrs, RQ29
  EXTS.L   RD13
  MOV.B    @(RQ29, RD13), RD14
  MOV      RD14, RD4
  BSR      tk_putc

L008007F1:
  CMP/GT   #1, RD12
  BT       L008007F2
  SHLD     RD11, #-4, RD14
  AND      RD14, #15, RD13
  MOV.Q    chrs, RQ28
  EXTS.L   RD13
  MOV.B    @(RQ28, RD13), RD14
  MOV      RD14, RD4
  BSR      tk_putc

L008007F2:
  CMP/PL   RD12
  BT       L008007F3
  AND      RD11, #15, RD14
  MOV.Q    chrs, RQ27
  EXTS.L   RD14
  MOV.B    @(RQ27, RD14), RD13
  MOV      RD13, RD4
  BSR      tk_putc

L008007F3:

L00E004BC:
  ADD      #56, R15
  BRA      L00C00054

tk_print_hex_genw:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  ADD      #-136, R15
  MOV      RD4, RD13
  MOV      #1, RD14

L008007F4:
  MOV      #16, RD12
  CMP/HS   RD12, RD13
  BT       L008007F5
  SHLD     #-4, RD13
  ADD      #1, RD14
  BRA      L008007F4

L008007F5:
  MOV      RD14, RD2

L00E004BD:
  ADD      #136, R15
  BRA      L00C0005F

tk_print_decimal:
  PUSH     LR
  BSR      L00C00193
  ADD      #-344, R15
  MOV      RD4, RD10
  MOV      #64, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 48)
  CMP/EQ   RD10
  BT       L008007F6
  MOV      #48, RD4
  BSR      tk_putc
  BRA      L00E004BE

L008007F6:
  CMP/EQ   #-2147483648, RD10
  BT       L008007F7
  MOV      L00C0019C, RQ4
  BSR      tk_puts
  BRA      L00E004BE

L008007F7:
  MOV      RD10, RD12
  MOV      #0, RD9
  CMP/PZ   RD12
  BT       L008007F8
  NEG      RD12, RD12
  MOV      #1, RD9

L008007F8:
  MOV      #64, RQ8
  ADD      R15, RQ8
  MOV      RQ8, RQ14
  ADD      RQ8, #18, RQ31
  MOV.Q    RQ31, @(R15, 40)
  CMP/EQ   RD12
  BT       L008007F9
  MOV      RQ14, RQ13
  ADD      #1, RQ14
  MOV      #48, RD30
  MOV.B    RD30, @RQ13

L008007F9:

L008007FA:
  CMP/PL   RD12
  BT       L008007FB
  MOV.Q    @(R15, 40), RQ29
  CMPQ/GT  RQ14, RQ29
  BT       L008007FB
  BRA      L008007FC

L008007FC:
  MOV      RD12, RD28
  MOV      #0x1999999A, RD7
  MOV      #6553, RD7
  DMULS    RD7, RD28
  STS      R1, RD6
  MOV      #10, RD7
  DMULU    RD7, RD6
  STS      R0, RD6
  SUB      RD6, RD28
  ADD      RD28, #48, RD11
  MOV      RQ14, RQ13
  ADD      #1, RQ14
  MOV.B    RD11, @RQ13
  MOV      #0x1999999A, RD7
  MOV      #6553, RD7
  DMULS    RD7, RD12
  STS      R1, RD12
  MOV.L    RD28, @(R15, 36)
  BRA      L008007FA

L008007FB:
  CMP/EQ   RD9
  BT       L008007FD
  MOV      RQ14, RQ13
  ADD      #1, RQ14
  MOV      #45, RD27
  MOV.B    RD27, @RQ13

L008007FD:
  MOV      #64, RQ26
  ADD      R15, RQ26
  MOV.B    @RQ26, RD11
  CMP/EQ   #45, RD11
  BT       L008007FE
  MOV      #-1, RQ13
  MOV      #0x0000FFFF, RQ13
  MOV      #-1, RD25
  MOV.L    RD25, @RQ13

L008007FE:

L008007FF:
  MOV      RQ14, RQ13
  MOV      #64, RQ6
  ADD      R15, RQ6
  MOV      RQ6, RQ7
  MOV.Q    RQ7, @(R15, 8)
  CMPQ/GT  RQ7, RQ13
  BT       L00800800
  ADD      #-1, RQ14
  MOV.B    @RQ14, RD11
  MOV      RD11, RD4
  BSR      tk_putc
  BRA      L008007FF

L00800800:

L00E004BE:
  ADD      #344, R15
  BRA      L00C00194

tk_print_decimal_n:
  PUSH     LR
  BSR      L00C00053
  ADD      #-344, R15
  MOV.L    RD4, @(R15, 44)
  MOV.L    RD5, @(R15, 40)
  MOV      #48, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 32)
  MOV.L    @(R15, 44), RD8
  MOV      RD8, RD13
  MOV      #0, RD9
  CMP/PZ   RD13
  BT       L00800801
  NEG      RD13, RD13
  MOV      #1, RD9

L00800801:
  MOV      #48, RQ31
  ADD      R15, RQ31
  MOV      RQ31, RQ14
  MOV.L    @(R15, 40), RD30
  MOV      RD30, RD12

L00800802:
  CMP/PL   RD12
  BT       L00800803
  MOV      RD13, RD29
  MOV      #0x1999999A, RD7
  MOV      #6553, RD7
  DMULS    RD7, RD29
  STS      R1, RD6
  MOV      #10, RD7
  DMULU    RD7, RD6
  STS      R0, RD6
  SUB      RD6, RD29
  ADD      RD29, #48, RD10
  MOV      RQ14, RQ11
  ADD      #1, RQ14
  MOV.B    RD10, @RQ11
  MOV      #0x1999999A, RD7
  MOV      #6553, RD7
  DMULS    RD7, RD13
  STS      R1, RD13
  ADD      #-1, RD12
  MOV.L    RD29, @(R15, 28)
  BRA      L00800802

L00800803:

L00800804:
  MOV      RQ14, RQ11
  MOV      #48, RQ27
  ADD      R15, RQ27
  MOV      RQ27, RQ28
  MOV.Q    RQ28, @(R15, 8)
  CMPQ/GT  RQ28, RQ11
  BT       L00800805
  ADD      #-1, RQ14
  MOV.B    @RQ14, RD10
  MOV      RD10, RD4
  BSR      tk_putc
  BRA      L00800804

L00800805:

L00E004BF:
  ADD      #344, R15
  BRA      L00C00054

tk_print_float:
  PUSH     LR
  BSR      L00C000E8
  ADD      #-152, R15
  FMOV.D   DR4, @(R15, 24)
  MOV      #0, RD14
  FMOV.D   @(R15, 24), DR15
  MOV      #0, DR14
  FCMP/GT  DR15, DR14
  BT       L00800806
  FMOV.D   @(R15, 24), DR13
  FNEG     DR13
  MOV      #1, RD14
  FMOV.D   DR13, @(R15, 24)

L00800806:
  FMOV.D   @(R15, 24), DR12
  FSTCI    DR12
  MOV      R0, RD13
  FMOV     DR12, FR11
  EXTS.L   RD13
  MOV      RD13, R0
  FLDCI    FR10
  FMOV     FR11, FR9
  FSUB     FR10, FR9
  FMOV     FR9, FR11
  MOV      #0x49742400, FR13
  FMUL     FR13, FR11
  FSTCI    FR11
  MOV      R0, RD12
  MOV.L    RD13, @(R15, 20)
  MOV.L    RD12, @(R15, 16)
  CMP/EQ   #-2147483648, RD13
  BT       L00800807
  MOV      L00C0019D, RQ4
  BSR      tk_puts
  BRA      L00E004C0

L00800807:
  CMP/EQ   RD14
  BT       L00800808
  MOV      #45, RD4
  BSR      tk_putc

L00800808:
  MOV.L    @(R15, 20), RD4
  BSR      tk_print_decimal
  MOV      #46, RD4
  BSR      tk_putc
  MOV.L    @(R15, 16), RD4
  MOV      #6, RD5
  BSR      tk_print_decimal_n

L00E004C0:
  ADD      #152, R15
  BRA      L00C000E9

irq_addTimerIrq:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  ADD      #-128, R15
  MOV.Q    RQ4, @(R15, 8)
  MOV.L    TU3CD6E720/n_irq_timer, RD13
  MOV      RD13, RD14
  ADD      #1, RD13
  MOV      irq_timer, RQ12
  MOV.Q    @(R15, 8), RQ11
  MOV.Q    RQ11, @(RQ12, RD14)
  MOV.L    RD13, TU3CD6E720/n_irq_timer

L00E004C1:
  ADD      #128, R15

L00C001A1:
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  RTS     

__isr_interrupt:
  PUSH     LR
  BSR      L00C00072
  ADD      #-112, R15
  MOV.L    RD4, @(R15, 12)
  MOV.L    @(R15, 12), RD13
  EXTU.W   RD13, RD13
  CMP/EQ   #49153, RD13
  BT       L00800875
  MOV      #0, RD14

L00800876:
  MOV.L    TU3CD6E720/n_irq_timer, RD11
  CMP/GT   RD14, RD11
  BT       L00800877
  MOV      irq_timer, RQ10
  EXTS.L   RD14
  MOV.Q    @(RQ10, RD14), RQ12
  JSR      RQ12

L00800878:
  ADD      #1, RD14
  BRA      L00800876

L00800877:
  MOV      #0, RD2
  BRA      L00E004C2

L00800875:
  MOV      L00C001A3, RQ4
  MOV.L    @(R15, 12), RD5
  BSR      tk_printf
  MOV      #0, RD2

L00E004C2:
  ADD      #112, R15
  BRA      L00C00074

_start:
  MOV      #17825776, R15
  BSR      zero_bss
  MOV      #57328, R0
  MOV      R0, SSP
  MOV      isr_table, R0
  MOV      R0, VBR
  BSR      __start
  BRA      _exit

_exit:
  MOV      #-1, R0
  JMP      R0
  NOP     

zero_bss:
  MOV      __bss_start, R2
  MOV      _end, R3
  MOV      #0, R4

zero_bss.L0:
  MOV.L    R4, @-R3
  CMP/HI   R2, R3
  BT       zero_bss.L0
  RTS     
  BRA      _exit

isr_except:
  BRK     

isr_inter:
  MOV      EXSR, R4
  BSR      __isr_interrupt
  RTE     

isr_table:
  BRA8B    _start
  BRA8B    isr_except
  BRA8B    isr_inter
  BRA8B    isr_inter

__umullq:
  DMULU    R4, R5
  MOV      R1, R3
  MOV      R0, R2
  SHLL32   R3
  ADD      R3, R2
  RTS     

__smullq:
  DMULS    R4, R5
  MOV      R1, R3
  MOV      R0, R2
  SHLL32   R3
  ADD      R3, R2
  RTS     

__umulsq:

__smulsq:
  MOV      R4, R6
  MOV      R5, R7
  SHLR32   R6
  SHLR32   R7
  DMULU    R4, R5
  MOV      R1, R3
  MOV      R0, R2
  DMULU    R4, R7
  ADD      R0, R3
  DMULU    R6, R5
  ADD      R0, R3
  SHLL32   R3
  ADD      R3, R2
  RTS     
  MOV      R4, R6
  MOV      R5, R7
  SHLR32   R6
  SHLR32   R7
  DMULU    R4, R5
  MOV      R1, R3
  MOV      R0, R2
  DMULU    R4, R7
  ADD      R0, R3
  DMULU    R6, R5
  ADD      R0, R3
  MOV      R3, R4
  SHLL32   R4
  ADD      R4, R2
  SHLR32   R3
  DMULU    R6, R7
  ADD      R0, R3
  RTS     
  MOV      R4, R6
  MOV      R5, R7
  SHLR32   R6
  SHLR32   R7
  DMULU    R4, R5
  MOV      R1, R3
  MOV      R0, R2
  DMULU    R4, R7
  ADD      R0, R3
  DMULU    R6, R5
  ADD      R0, R3
  MOV      R3, R4
  SHLL32   R4
  ADD      R4, R2
  SHLR32   R3
  DMULS    R6, R7
  ADD      R0, R3
  RTS     

__longj:
  MOV      R4, R1
  MOV      R5, R2
  MOV.Q    @(R4, 16), R0
  LDS      R0, LR
  MOV.Q    @R4, R0
  MOV.Q    @(R4, 8), R1
  MOV.Q    @(R4, 24), R3
  MOV.Q    @(R4, 40), R5
  MOV.Q    @(R4, 48), R6
  MOV.Q    @(R4, 56), R7
  MOV.Q    @(R4, 64), R8
  MOV.Q    @(R4, 72), R9
  MOV.Q    @(R4, 80), R10
  MOV.Q    @(R4, 88), R11
  MOV.Q    @(R4, 96), R12
  MOV.Q    @(R4, 104), R13
  MOV.Q    @(R4, 112), R14
  MOV.Q    @(R4, 120), R15
  MOV.Q    @(R4, 128), R16
  MOV.Q    @(R4, 136), R17
  MOV.Q    @(R4, 144), R18
  MOV.Q    @(R4, 152), R19
  MOV.Q    @(R4, 160), R20
  MOV.Q    @(R4, 168), R21
  MOV.Q    @(R4, 176), R22
  MOV.Q    @(R4, 184), R23
  MOV.Q    @(R4, 192), R24
  MOV.Q    @(R4, 200), R25
  MOV.Q    @(R4, 208), R26
  MOV.Q    @(R4, 216), R27
  MOV.Q    @(R4, 224), R28
  MOV.Q    @(R4, 232), R29
  MOV.Q    @(R4, 240), R30
  MOV.Q    @(R4, 248), R31
  RTS     
  NOP     

__setj:
  MOV.Q    R0, @R4
  MOV.Q    R1, @(R4, 8)
  MOV.Q    R2, @(R4, 16)
  MOV.Q    R3, @(R4, 24)
  MOV.Q    R4, @(R4, 32)
  MOV.Q    R5, @(R4, 40)
  MOV.Q    R6, @(R4, 48)
  MOV.Q    R7, @(R4, 56)
  MOV.Q    R8, @(R4, 64)
  MOV.Q    R9, @(R4, 72)
  MOV.Q    R10, @(R4, 80)
  MOV.Q    R11, @(R4, 88)
  MOV.Q    R12, @(R4, 96)
  MOV.Q    R13, @(R4, 104)
  MOV.Q    R14, @(R4, 112)
  MOV.Q    R15, @(R4, 120)
  MOV.Q    R16, @(R4, 128)
  MOV.Q    R17, @(R4, 136)
  MOV.Q    R18, @(R4, 144)
  MOV.Q    R19, @(R4, 152)
  MOV.Q    R20, @(R4, 160)
  MOV.Q    R21, @(R4, 168)
  MOV.Q    R22, @(R4, 176)
  MOV.Q    R23, @(R4, 184)
  MOV.Q    R24, @(R4, 192)
  MOV.Q    R25, @(R4, 200)
  MOV.Q    R26, @(R4, 208)
  MOV.Q    R27, @(R4, 216)
  MOV.Q    R28, @(R4, 224)
  MOV.Q    R29, @(R4, 232)
  MOV.Q    R30, @(R4, 240)
  MOV.Q    R31, @(R4, 248)
  STS      LR, R1
  MOV.Q    R1, @(R4, 16)
  MOV      #0, R2
  RTS     
  NOP     

__va64_arg_i:
  MOV.L    @(R4, 96), R2
  MOV      #64, R1
  CMP/GT   R2, R1
  BT       __va64_arg_i.L0
  MOV      R2, R3
  ADD      R4, R3
  MOV.Q    @R3, R6
  ADD      #8, R2
  MOV.L    R2, @(R4, 96)
  MOV      R6, R2
  RTS     
  NOP     

__va64_arg_i.L0:
  MOV.Q    @(R4, 112), R1
  MOV.L    @R1+, R6
  MOV.Q    R1, @(R4, 112)
  MOV      R6, R2
  RTS     
  NOP     

__va64_arg_l:
  MOV.L    @(R4, 96), R2
  MOV      #64, R1
  CMP/GT   R2, R1
  BT       __va64_arg_l.L0
  MOV      R2, R3
  ADD      R4, R3
  MOV.Q    @R3, R6
  ADD      #8, R2
  MOV.L    R2, @(R4, 96)
  MOV      R6, R2
  RTS     
  NOP     

__va64_arg_l.L0:
  MOV.Q    @(R4, 112), R1
  ADD      #7, R1
  AND      #-8, R1
  MOV.Q    @R1+, R6
  MOV.Q    R1, @(R4, 112)
  MOV      R6, R2
  RTS     
  NOP     

__va64_arg_x:
  MOV.L    @(R4, 96), R0
  MOV      #56, R1
  CMP/GT   R0, R1
  BT       __va64_arg_x.L0
  MOV      R0, R1
  ADD      R4, R1
  MOV.Q    @R1, R6
  ADD      #8, R1
  MOV.Q    @R1, R7
  ADD      #8, R1
  MOV.L    R1, @(R4, 96)
  MOV      R6, R2
  MOV      R7, R3
  RTS     
  NOP     

__va64_arg_x.L0:
  MOV.Q    @(R4, 112), R1
  ADD      #7, R1
  AND      #-8, R1
  MOV.Q    @R1+, R6
  MOV.Q    @R1+, R7
  MOV.Q    R1, @(R4, 112)
  MOV      R6, R2
  MOV      R7, R3
  RTS     
  NOP     

__va64_arg_f:
  MOV.L    @(R4, 104), R0
  MOV      #96, R1
  CMP/GT   R0, R1
  BT       __va64_arg_f.L0
  MOV      R4, R3
  ADD      R0, R3
  FMOV.D   @R3, FR2
  ADD      #8, R0
  MOV      R0, R1
  MOV.L    R1, @(R4, 104)
  RTS     
  NOP     

__va64_arg_f.L0:
  MOV.Q    @(R4, 112), R1
  FMOV.S   @R1, FR2
  ADD      #4, R1
  MOV.Q    R1, @(R4, 112)
  RTS     
  NOP     

__va64_arg_d:
  MOV.L    @(R4, 104), R0
  ADD      #7, R0
  MOV      #-8, R1
  AND      R1, R0
  MOV      #96, R1
  CMP/GT   R0, R1
  BT       __va64_arg_d.L0
  MOV      R4, R3
  ADD      R0, R3
  FMOV.D   @R3, FR2
  ADD      #8, R0
  MOV      R0, R1
  MOV.L    R1, @(R4, 104)
  RTS     
  NOP     

__va64_arg_d.L0:
  MOV.Q    @(R4, 112), R1
  FMOV.D   @R1, FR2
  ADD      #8, R1
  MOV.Q    R1, @(R4, 112)
  RTS     
  NOP     

__ldhf16:
  MOV      R4, R0
  FLDCH    FR2
  RTS     

__sthf16:
  FSTCH    FR4
  MOV      R0, R2
  RTS     

__memcpy32:
  MOV      R5, R3
  ADD      R6, R3

__memcpy32.L0:
  MOV.L    @R5, R1
  MOV.L    R1, @R4
  ADD      #4, R5
  ADD      #4, R4
  CMP/HI   R5, R3
  BT       __memcpy32.L0
  RTS     

__memcpy64:
  MOV      R5, R3
  ADD      R6, R3

__memcpy64.L0:
  MOV.Q    @R5, R1
  MOV.Q    R1, @R4
  ADD      #8, R5
  ADD      #8, R4
  CMP/HI   R5, R3
  BT       __memcpy64.L0
  RTS     

__memcpy128:
  MOV      R5, R3
  ADD      R6, R3

__memcpy128.L0:
  MOV.Q    @R5, R0
  MOV.Q    @(R5, 8), R1
  MOV.Q    R0, @R4
  MOV.Q    R1, @(R4, 8)
  ADD      #16, R5
  ADD      #16, R4
  CMP/HI   R5, R3
  BT       __memcpy128.L0
  RTS     

__memcpy8_16:
  MOV.Q    @R5, R0
  MOV.Q    @(R5, 8), R1
  MOV.Q    R0, @R4
  MOV.Q    R1, @(R4, 8)
  RTS     

sleep_0:
  SLEEP   
  RTS     

TK_EncBlock16P:
  MOV.Q    @R4, R18
  MOV.Q    @(R4, 640), R19
  MOV.Q    @(R4, 1280), R20
  MOV.Q    @(R4, 1920), R21
  MOV.Q    @R5, R22
  XOR      R2, R2
  CMPQ/EQ  R18, R22
  BT       TK_EncBlock16P.L0
  MOV.Q    @(R5, 640), R23
  MOV.Q    @(R5, 1280), R16
  MOV.Q    @(R5, 1920), R17
  CMPQ/EQ  R19, R23
  MOVNT    R2
  CMPQ/EQ  R20, R16
  MOVNT    R3
  OR       R3, R2
  CMPQ/EQ  R21, R17
  MOVNT    R3
  OR       R3, R2
  TST      R2, R2
  BT       TK_EncBlock16P.L0
  XOR      R2, R2
  RTS     

TK_EncBlock16P.L0:
  MOV.Q    R18, @R5
  MOV.Q    R19, @(R5, 640)
  MOV.Q    R20, @(R5, 1280)
  MOV.Q    R21, @(R5, 1920)
  MOV      #65535, R16
  MOV      #-1, R17
  SHLD     R18, #-16, R2
  CMP/GT   R16, R2
  CSELT    R16, R2, R16
  CMP/GT   R17, R2
  CSELT    R2, R17, R17
  SHLD.Q   R18, #-48, R2
  CMP/GT   R16, R2
  CSELT    R16, R2, R16
  CMP/GT   R17, R2
  CSELT    R2, R17, R17
  EXTU.W   R19, R2
  CMP/GT   R16, R2
  CSELT    R16, R2, R16
  CMP/GT   R17, R2
  CSELT    R2, R17, R17
  SHLD.Q   R19, #-32, R2
  EXTU.W   R2
  CMP/GT   R16, R2
  CSELT    R16, R2, R16
  CMP/GT   R17, R2
  CSELT    R2, R17, R17
  SHLD     R20, #-16, R2
  CMP/GT   R16, R2
  CSELT    R16, R2, R16
  CMP/GT   R17, R2
  CSELT    R2, R17, R17
  SHLD.Q   R20, #-48, R2
  CMP/GT   R16, R2
  CSELT    R16, R2, R16
  CMP/GT   R17, R2
  CSELT    R2, R17, R17
  EXTU.W   R21, R2
  CMP/GT   R16, R2
  CSELT    R16, R2, R16
  CMP/GT   R17, R2
  CSELT    R2, R17, R17
  SHLD.Q   R21, #-32, R2
  EXTU.W   R2
  CMP/GT   R16, R2
  CSELT    R16, R2, R16
  CMP/GT   R17, R2
  CSELT    R2, R17, R17
  ADD      R16, R17, R22
  SHAD     #-1, R22
  ADD      R16, R22, R4
  ADD      R17, R22, R5
  SHLR1    R4
  SHLR1    R5
  XOR      R1, R1
  EXTU.W   R18, R2
  CMP/GT   R22, R2
  ADDC     R3, R3
  CMP/GT   R5, R2
  ADDC     R3, R3
  CMP/GT   R2, R4
  ADDC     R1, R3
  SHLD     R18, #-16, R2
  CMP/GT   R22, R2
  ADDC     R3, R3
  CMP/GT   R5, R2
  ADDC     R3, R3
  CMP/GT   R2, R4
  ADDC     R1, R3
  SHLD.Q   R18, #-32, R2
  EXTU.W   R2
  CMP/GT   R22, R2
  ADDC     R3, R3
  CMP/GT   R5, R2
  ADDC     R3, R3
  CMP/GT   R2, R4
  ADDC     R1, R3
  SHLD.Q   R18, #-48, R2
  CMP/GT   R22, R2
  ADDC     R3, R3
  CMP/GT   R5, R2
  ADDC     R3, R3
  CMP/GT   R2, R4
  ADDC     R1, R3
  EXTU.W   R19, R2
  CMP/GT   R22, R2
  ADDC     R3, R3
  CMP/GT   R5, R2
  ADDC     R3, R3
  CMP/GT   R2, R4
  ADDC     R1, R3
  SHLD     R19, #-16, R2
  EXTU.W   R2
  CMP/GT   R22, R2
  ADDC     R3, R3
  CMP/GT   R5, R2
  ADDC     R3, R3
  CMP/GT   R2, R4
  ADDC     R1, R3
  SHLD.Q   R19, #-32, R2
  EXTU.W   R2
  CMP/GT   R22, R2
  ADDC     R3, R3
  CMP/GT   R5, R2
  ADDC     R3, R3
  CMP/GT   R2, R4
  ADDC     R1, R3
  SHLD.Q   R19, #-48, R2
  CMP/GT   R22, R2
  ADDC     R3, R3
  CMP/GT   R5, R2
  ADDC     R3, R3
  CMP/GT   R2, R4
  ADDC     R1, R3
  EXTU.W   R20, R2
  CMP/GT   R22, R2
  ADDC     R3, R3
  CMP/GT   R5, R2
  ADDC     R3, R3
  CMP/GT   R2, R4
  ADDC     R1, R3
  SHLD     R20, #-16, R2
  EXTU.W   R2
  CMP/GT   R22, R2
  ADDC     R3, R3
  CMP/GT   R5, R2
  ADDC     R3, R3
  CMP/GT   R2, R4
  ADDC     R1, R3
  SHLD.Q   R20, #-32, R2
  EXTU.W   R2
  CMP/GT   R22, R2
  ADDC     R3, R3
  CMP/GT   R5, R2
  ADDC     R3, R3
  CMP/GT   R2, R4
  ADDC     R1, R3
  SHLD.Q   R20, #-48, R2
  CMP/GT   R22, R2
  ADDC     R3, R3
  CMP/GT   R5, R2
  ADDC     R3, R3
  CMP/GT   R2, R4
  ADDC     R1, R3
  EXTU.W   R21, R2
  CMP/GT   R22, R2
  ADDC     R3, R3
  CMP/GT   R5, R2
  ADDC     R3, R3
  CMP/GT   R2, R4
  ADDC     R1, R3
  SHLD     R21, #-16, R2
  CMP/GT   R22, R2
  ADDC     R3, R3
  CMP/GT   R5, R2
  ADDC     R3, R3
  CMP/GT   R2, R4
  ADDC     R1, R3
  SHLD.Q   R21, #-32, R2
  EXTU.W   R2
  CMP/GT   R22, R2
  ADDC     R3, R3
  CMP/GT   R5, R2
  ADDC     R3, R3
  CMP/GT   R2, R4
  ADDC     R1, R3
  SHLD.Q   R21, #-48, R2
  CMP/GT   R22, R2
  ADDC     R3, R3
  CMP/GT   R5, R2
  ADDC     R3, R3
  CMP/GT   R2, R4
  ADDC     R1, R3
  MOV      R3, R2
  SHLR1    R2
  NOT      R2
  AND      #1431655765, R2
  XOR      R2, R3
  SHLD     #-1, R16
  SHLD     #-1, R17
  ADD      R16, #98304, R2
  SHLL16   R2
  SHLR1    R2
  OR       R17, R2
  MOV.L    R2, @R6
  MOV.L    R3, @R7
  MOV      #1, R2
  RTS     

__udivsi3:
  TST      R4, R4
  BT       __udivsi3.Ret_Zero
  TST      R5, R5
  BT       __udivsi3.Ret_Zero
  EXTU.L   R4, R16
  EXTU.L   R5, R17
  CLZ      R17, R18
  CLZ      R16, R2
  SUB      R2, R18
  EXTU.B   R18
  CMP/GT   #30, R18
  BT       __udivsi3.L0
  CMP/EQ   #31, R18
  BT       __udivsi3.Ret_R16
  BRA      __udivsi3.Ret_Zero

__udivsi3.L0:
  ADD      #1, R18
  MOV      #32, R2
  SUB      R18, R2
  SHLD     R16, R2, R4
  NEG      R18, R2
  SHLD     R16, R2, R5
  MOV      #0, R6
  TST      #-4, R18
  BT       __udivsi3.L1

__udivsi3.L4:
  ADD      #-3, R18
  SHLL1    R5
  SHLD     R4, #-31, R2
  SHLL1    R4
  OR       R2, R5
  OR       R6, R4
  CMP/GT   R5, R17
  MOVNT    R6
  SUB      R5, R17, R7
  CSELT    R5, R7, R5
  SHLL1    R5
  SHLD     R4, #-31, R2
  SHLL1    R4
  OR       R2, R5
  OR       R6, R4
  CMP/GT   R5, R17
  MOVNT    R6
  SUB      R5, R17, R7
  CSELT    R5, R7, R5
  SHLL1    R5
  SHLD     R4, #-31, R2
  SHLL1    R4
  OR       R2, R5
  OR       R6, R4
  CMP/GT   R5, R17
  MOVNT    R6
  SUB      R5, R17, R7
  CSELT    R5, R7, R5
  TST      #-4, R18
  BT       __udivsi3.L4

__udivsi3.L1:
  ADD      #-1, R18
  SHLL1    R5
  SHLD     R4, #-31, R2
  SHLL1    R4
  OR       R2, R5
  OR       R6, R4
  CMP/GT   R5, R17
  MOVNT    R6
  SUB      R5, R17, R7
  CSELT    R5, R7, R5
  TST      R18, R18
  BT       __udivsi3.L1

__udivsi3.L2:
  SHLL1    R4
  OR       R6, R4
  EXTU.L   R4, R2
  RTS     

__udivsi3.Ret_Zero:
  MOV      #0, R2
  RTS     

__udivsi3.Ret_R16:
  MOV      R16, R2
  RTS     

__sdivsi3:
  PUSH     LR
  PUSH     R14
  SHAD     R4, #-31, R6
  SHAD     R5, #-31, R7
  XOR      R4, R6, R2
  XOR      R5, R7, R3
  SUB      R2, R6, R4
  SUB      R3, R7, R5
  XOR      R7, R6, R14
  BSR      __udivsi3
  XOR      R2, R14, R3
  SUB      R3, R14, R2
  POP      R14
  RET     

I_Init:
  PUSH     LR
  ADD      #-136, R15
  BSR      I_InitSound

L00E004C3:
  ADD      #136, R15
  RET     

I_ZoneBase:
  PUSH     LR
  BSR      L00C00072
  ADD      #-128, R15
  MOV.Q    RQ4, @(R15, 24)
  MOV.L    mb_used, RD12
  SHAD     RD12, #10, RD14
  SHAD     RD14, #10, RD11
  MOV.Q    @(R15, 24), RQ10
  MOV.L    RD11, @RQ10
  MOV.L    @RQ10, RD14
  EXTS.L   RD14, RQ12
  MOV.Q    RQ12, @R15
  MOV      RQ12, RQ4
  BSR      malloc
  MOV      RQ2, RQ13
  MOV      RQ13, RQ2

L00E004C4:
  ADD      #128, R15
  BRA      L00C00074

I_GetTime:
  PUSH     LR
  PUSH     R14
  PUSH     FR15
  PUSH     FR14
  PUSH     FR13
  PUSH     FR12
  ADD      #-176, R15
  BSR      TK_GetTimeMs
  MOV      RD2, RD14
  EXTS.L   RD14
  MOV      RD14, R0
  FLDCI    FR15
  MOV      #0x1EB851E8, DR14
  FMOV     DR14, FR13
  FMOV     FR15, FR12
  FMUL     FR13, FR12
  FSTCI    FR12
  MOV      R0, RD14
  MOV      RD14, RD2

L00E004C5:
  ADD      #176, R15
  POP      FR12
  POP      FR13
  POP      FR14
  POP      FR15
  POP      RQ14
  RET     

I_StartFrame:
  ADD      #-144, R15

L00E004C6:
  ADD      #144, R15
  RTS     

I_StartTic:
  ADD      #-144, R15

L00E004C7:
  ADD      #144, R15
  RTS     

I_BaseTiccmd:
  PUSH     R14
  PUSH     R13
  ADD      #-144, R15
  MOV      emptycmd, RQ13
  MOV      RQ13, RQ14
  MOV      RQ14, RQ2

L00E004C8:
  ADD      #144, R15
  POP      RQ13
  POP      RQ14
  RTS     

I_Quit:
  PUSH     LR
  ADD      #-136, R15
  BSR      I_ShutdownGraphics
  MOV      #0, RD4
  BSR      exit

L00E004C9:
  ADD      #136, R15
  RET     

L00C001D8:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  RTS     

I_AllocLow:
  PUSH     LR
  BSR      L00C001D8
  ADD      #-144, R15
  MOV.L    RD4, @(R15, 28)
  MOV.L    @(R15, 28), RD14
  EXTS.L   RQ14, RQ14
  MOV      RQ14, RQ4
  BSR      malloc
  MOV      RQ2, RQ13
  MOV      RQ13, RQ12
  MOV.L    @(R15, 28), RD14
  EXTS.L   RQ14, RQ14
  MOV      RQ12, RQ4
  MOV      #0, RD5
  MOV      RQ14, RQ6
  BSR      memset
  MOV      RQ12, RQ2

L00E004CA:
  ADD      #144, R15

L00C001D9:
  POP      RQ12
  POP      RQ13
  POP      RQ14
  RET     

I_Tactile:
  ADD      #-160, R15
  MOV.L    RD4, @(R15, 12)
  MOV.L    RD5, @(R15, 8)
  MOV.L    RD6, @(R15, 4)

L00E004CB:
  ADD      #160, R15
  RTS     

I_Error:
  PUSH     LR
  PUSH     R14
  PUSH     R13
  ADD      #-280, R15
  MOV.Q    RQ4, @(R15, 24)
  MOV      #32, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 16)
  MOV.Q    RQ4, @RQ3
  MOV.Q    RQ5, @(RQ3, 8)
  MOV.Q    RQ6, @(RQ3, 16)
  MOV.Q    RQ7, @(RQ3, 24)
  MOV.Q    RQ20, @(RQ3, 32)
  MOV.Q    RQ21, @(RQ3, 40)
  MOV.Q    RQ22, @(RQ3, 48)
  MOV.Q    RQ23, @(RQ3, 56)
  FMOV.D   DR4, @(RQ3, 64)
  FMOV.D   DR5, @(RQ3, 72)
  FMOV.D   DR6, @(RQ3, 80)
  FMOV.D   DR7, @(RQ3, 88)
  MOV      #8, R0
  MOV.L    R0, @(R3, 96)
  MOV      #64, R0
  MOV.L    R0, @(R3, 104)
  MOV      #304, R1
  ADD      R15, R1
  MOV.Q    R1, @(R3, 112)
  BSR      __get_stderr
  MOV      RQ2, RQ14
  MOV.Q    @RQ14, RQ13
  MOV.Q    RQ13, @R15
  MOV      RQ13, RQ4
  MOV      L00C001DC, RQ5
  BSR      fprintf
  BSR      __get_stderr
  MOV      RQ2, RQ14
  MOV.Q    @RQ14, RQ13
  MOV.Q    RQ13, @R15
  MOV      RQ13, RQ4
  MOV.Q    @(R15, 24), RQ5
  MOV      #32, RQ6
  ADD      R15, RQ6
  BSR      vfprintf
  BSR      __get_stderr
  MOV      RQ2, RQ14
  MOV.Q    @RQ14, RQ13
  MOV.Q    RQ13, @R15
  MOV      RQ13, RQ4
  MOV      L00C000B0, RQ5
  BSR      fprintf
  BRK     
  BSR      __get_stderr
  MOV      RQ2, RQ14
  MOV.Q    @RQ14, RQ13
  MOV.Q    RQ13, @R15
  MOV      RQ13, RQ4
  BSR      fflush
  BSR      I_ShutdownGraphics
  MOV      #-1, RD4
  BSR      exit

L00E004CC:
  ADD      #280, R15
  POP      RQ13
  POP      RQ14
  RET     

L00C001DF:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  RTS     

FixedMul:
  PUSH     LR
  BSR      L00C001DF
  ADD      #-136, R15
  MOV.L    RD4, @(R15, 28)
  MOV.L    RD5, @(R15, 24)
  MOV.L    @(R15, 28), RD4
  MOV.L    @(R15, 24), RD5
  BSR      __smullq
  MOV      RQ2, RQ13
  MOV      RQ13, RQ12
  SHAD.Q   RQ12, #-16, RQ11
  EXTS.L   RQ11, RD14
  MOV.Q    RQ13, @(R15, 16)
  MOV      RD14, RD2

L00E004CD:
  ADD      #136, R15

L00C001E0:
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  RET     

FixedDiv:
  PUSH     LR
  BSR      L00C00072
  ADD      #-112, R15
  MOV.L    RD4, @(R15, 12)
  MOV.L    RD5, @(R15, 8)
  MOV.L    @(R15, 12), RD4
  BSR      abs
  MOV      RD2, RD14
  SHAD     RD14, #-14, RD13
  MOV.L    RD13, @R15
  MOV.L    @(R15, 8), RD4
  BSR      abs
  MOV      RD2, RD14
  CMP/GT   RD13, RD14
  BT       L00800F48
  MOV.L    @(R15, 12), RD11
  MOV.L    @(R15, 8), RD10
  XOR      RD11, RD10, RD12
  MOV.L    RD12, @R15
  CMP/PZ   RD12
  BT       L00800F49
  MOV      #0x80000000, RD14
  MOV      #-32768, RD14
  BRA      L00800F4A

L00800F49:
  MOV      #0x7FFFFFFF, RD14
  MOV      #32767, RD14

L00800F4A:
  MOV      RD14, RD2
  BRA      L00E004CE

L00800F48:
  MOV.L    @(R15, 12), RD4
  MOV.L    @(R15, 8), RD5
  BSR      FixedDiv2
  MOV      RD2, RD14
  MOV      RD14, RD2

L00E004CE:
  ADD      #112, R15
  BRA      L00C00074

FixedDiv2:
  PUSH     LR
  PUSH     R14
  PUSH     R13
  ADD      #-168, R15
  MOV.L    RD4, @(R15, 44)
  MOV.L    RD5, @(R15, 40)
  MOV.L    @(R15, 44), RD14
  EXTS.L   RQ14, RQ14
  SHAD.Q   RQ14, #16, RQ13
  MOV.L    @(R15, 40), RD14
  EXTS.L   RQ14, RQ14
  MOV.Q    RQ13, @(R15, 32)
  MOV      RQ14, RQ5
  MOV.Q    @(R15, 32), RQ4
  BSR      __sdivsq
  MOV.Q    RQ2, @(R15, 32)
  MOV.L    @(R15, 32), RD7
  EXTS.L   RD7, RD7
  MOV.L    RD7, @(R15, 12)
  MOV.L    @(R15, 12), RD2

L00E004CF:
  ADD      #168, R15
  POP      RQ13
  POP      RQ14
  RET     

SlopeDiv:
  PUSH     LR
  BSR      L00C00198
  ADD      #-104, R15
  MOV.L    RD4, @(R15, 12)
  MOV      RD5, RD12
  MOV      #512, RD10
  CMP/HS   RD10, RD12
  BT       L00800A54
  MOV      #2048, RD2
  BRA      L00E004D0

L00800A54:
  MOV.L    @(R15, 12), RD9
  SHLD     RD9, #3, RD14
  SHLD     RD12, #-8, RD11
  MOV      RD11, RD5
  MOV      RD14, RD4
  BSR      __udivsi3
  MOV      RD2, RD13
  CMP/HI   #2048, RD13
  BT       L00800A55
  MOV      RD13, RD14
  BRA      L00800A56

L00800A55:
  MOV      #2048, RD14

L00800A56:
  MOV      RD14, RD2

L00E004D0:
  ADD      #104, R15
  BRA      L00C0019A

L00C002C0:
  PUSH     R31
  PUSH     R30
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  PUSH     R9
  PUSH     R8
  RTS     

EV_FindMObjForTid:
  PUSH     LR
  BSR      L00C002C0
  ADD      #-160, R15
  MOV.L    RD4, @(R15, 72)
  MOV      thinkercap, RQ12
  MOV.Q    @(RQ12, 8), RQ14
  MOV.Q    @(RQ12, 8), RQ14

L00801C26:
  MOV      thinkercap, RQ11
  MOV      RQ11, RQ13
  CMPQ/EQ  RQ13, RQ14
  BT       L00801C28
  ADD      RQ14, #16, RQ10
  MOV.Q    @RQ10, RQ13
  MOV      P_MobjThinker, RQ8
  MOV      RQ8, RQ9
  MOV      RQ13, RQ31
  MOV      RQ9, RQ30
  MOV.Q    RQ31, @(R15, 32)
  MOV.Q    RQ30, @(R15, 24)
  CMPQ/EQ  RQ30, RQ31
  BT       L00801C29
  BRA      L00801C27

L00801C29:
  MOV      RQ14, RQ7
  ADD      RQ7, #204, RQ6
  MOV.W    @RQ6, RD5
  EXTS.W   RD5, RD4
  MOV.L    @(R15, 72), RD2
  MOV      RD2, RD3
  MOV.Q    RQ7, @(R15, 64)
  MOV.L    RD4, @(R15, 8)
  MOV.L    RD3, @(R15, 4)
  CMP/EQ   RD3, RD4
  BT       L00801C2A
  MOV.Q    @(R15, 64), RQ2
  BRA      L00E004D1

L00801C2A:

L00801C27:
  MOV.Q    @(RQ14, 8), RQ14
  BRA      L00801C26

L00801C28:
  MOV      #0, RQ13
  MOV      RQ13, RQ2

L00E004D1:
  ADD      #160, R15

L00C002C3:
  POP      RQ8
  POP      RQ9
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  POP      RQ30
  POP      RQ31
  RET     

L00C002C5:
  PUSH     R31
  PUSH     R30
  PUSH     R29
  PUSH     R28
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  PUSH     R9
  PUSH     R8
  RTS     

EV_FindMObjForTid2:
  PUSH     LR
  BSR      L00C002C5
  ADD      #-160, R15
  MOV.Q    RQ4, @(R15, 80)
  MOV.L    RD5, @(R15, 76)
  MOV.Q    @(R15, 80), RQ12
  MOV.Q    @RQ12, RQ14
  MOV      RQ14, RQ13
  MOV      #0, RQ11
  CMPQ/EQ  RQ11, RQ13
  BT       L00801C2B
  MOV      thinkercap, RQ10
  MOV.Q    @(RQ10, 8), RQ13

L00801C2B:

L00801C2C:
  MOV      thinkercap, RQ9
  MOV      RQ9, RQ14
  CMPQ/EQ  RQ14, RQ13
  BT       L00801C2E
  ADD      RQ13, #16, RQ8
  MOV.Q    @RQ8, RQ14
  MOV      P_MobjThinker, RQ30
  MOV      RQ30, RQ31
  MOV      RQ14, RQ29
  MOV      RQ31, RQ28
  MOV.Q    RQ29, @(R15, 32)
  MOV.Q    RQ28, @(R15, 24)
  CMPQ/EQ  RQ28, RQ29
  BT       L00801C2F
  BRA      L00801C2D

L00801C2F:
  MOV      RQ13, RQ7
  ADD      RQ7, #204, RQ6
  MOV.W    @RQ6, RD5
  EXTS.W   RD5, RD4
  MOV.L    @(R15, 76), RD2
  MOV      RD2, RD3
  MOV.Q    RQ7, @(R15, 64)
  MOV.L    RD4, @(R15, 8)
  MOV.L    RD3, @(R15, 4)
  CMP/EQ   RD3, RD4
  BT       L00801C30
  MOV.Q    @(RQ13, 8), RQ14
  MOV.Q    @(R15, 80), RQ23
  MOV.Q    RQ14, @RQ23
  MOV.Q    @(R15, 64), RQ2
  BRA      L00E004D2

L00801C30:

L00801C2D:
  MOV.Q    @(RQ13, 8), RQ13
  BRA      L00801C2C

L00801C2E:
  MOV      #0, RQ14
  MOV      RQ14, RQ2

L00E004D2:
  ADD      #160, R15

L00C002C6:
  POP      RQ8
  POP      RQ9
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  POP      RQ28
  POP      RQ29
  POP      RQ30
  POP      RQ31
  RET     

NetUpdate:
  PUSH     LR
  BSR      L00C00193
  ADD      #-168, R15
  BSR      I_GetTime
  MOV      RD2, RD14
  MOV.L    ticdup, RD5
  MOV      RD14, RD4
  BSR      __sdivsi3
  MOV      RD2, RD10
  MOV.L    gametime, RD9
  SUB      RD10, RD9, RD12
  MOV      RD10, RD9
  MOV.L    RD9, gametime
  CMP/PL   RD12
  BT       L00800C27
  BRA      L00800C28

L00800C27:
  MOV.L    skiptics, RD8
  CMP/GT   RD12, RD8
  BT       L00800C29
  MOV.L    skiptics, RD31
  SUB      RD31, RD12
  MOV      #0, RD31
  MOV.L    RD31, skiptics
  BRA      L00800C2A

L00800C29:
  MOV.L    skiptics, RD30
  SUB      RD12, RD30
  MOV      #0, RD12
  MOV.L    RD30, skiptics

L00800C2A:
  MOV.L    consoleplayer, RD29
  MOV.Q    netbuffer, RQ28
  EXTU.B   RD29, RD29
  MOV.B    RD29, @(RQ28, 6)
  MOV.L    ticdup, RD5
  MOV.L    gametic, RD4
  BSR      __sdivsi3
  MOV      RD2, RD27
  MOV      #0, RD13
  MOV.L    RD27, @(R15, 36)

L00800C2B:
  CMP/GT   RD13, RD12
  BT       L00800C2D
  BSR      I_StartTic
  BSR      D_ProcessEvents
  MOV.L    maketic, RD26
  MOV.L    @(R15, 36), RD25
  SUB      RD26, RD25, RD14
  MOV      #5, RD7
  CMP/GE   RD7, RD14
  BT       L00800C2E
  BRA      L00800C2D

L00800C2E:
  MOV.L    maketic, RD25
  MOV      RD25, RD14
  MOV      #0x15555556, RD7
  MOV      #5461, RD7
  DMULS    RD7, RD14
  STS      R1, RD6
  MOV      #12, RD7
  DMULU    RD7, RD6
  STS      R0, RD6
  SUB      RD6, RD14
  MOV      localcmds, RQ26
  EXTS.L   RD14
  LEA.Q    @(RQ26, RD14), RQ27
  MOV.Q    RQ27, @(R15, 24)
  MOV      RQ27, RQ4
  BSR      G_BuildTiccmd
  MOV.L    maketic, RD6
  ADD      #1, RD6
  MOV.L    RD6, maketic

L00800C2C:
  ADD      #1, RD13
  BRA      L00800C2B

L00800C2D:
  MOV.L    singletics, RD5
  CMP/EQ   RD5
  BT       L00800C2F
  BRA      L00E004D3

L00800C2F:
  MOV      #0, RD13

L00800C30:
  MOV.Q    doomcom, RQ4
  MOV.W    @(RQ4, 16), RD14
  MOV      RD13, RD11
  EXTS.W   RD14, RD3
  MOV.L    RD3, @(R15, 16)
  CMP/GT   RD11, RD3
  BT       L00800C32
  MOV      nodeingame, RQ2
  EXTS.L   RD13
  MOV.L    @(RQ2, RD13), RD14
  CMP/EQ   RD14
  BT       L00800C33
  MOV      resendto, RQ23
  EXTS.L   RD13
  MOV.L    @(RQ23, RD13), RD14
  MOV      RD14, RD22
  MOV.Q    netbuffer, RQ21
  EXTU.B   RD14, RD14
  MOV.B    RD14, @(RQ21, 5)
  MOV.L    maketic, RD20
  SUB      RD20, RD22, RD14
  EXTU.B   RD14, RD14
  MOV.B    RD14, @(RQ21, 7)
  MOV.B    @(RQ21, 7), RD14
  EXTU.B   RD14, RD14
  MOV.L    RD22, @(R15, 40)
  CMP/GT   #12, RD14
  BT       L00800C34
  MOV      L00C002D6, RQ4
  BSR      I_Error

L00800C34:
  MOV.Q    doomcom, RQ19
  MOV.W    @(RQ19, 20), RD14
  MOV.L    maketic, RD18
  SUB      RD18, RD14, RD11
  MOV      resendto, RQ17
  EXTS.L   RD13
  MOV.L    RD11, @(RQ17, RD13)
  MOV      #0, RD16
  MOV.L    RD16, @(R15, 44)

L00800C35:
  MOV.Q    netbuffer, RQ16
  MOV.B    @(RQ16, 7), RD14
  EXTU.B   RD14, RD14
  MOV.L    @(R15, 44), RD17
  MOV      RD17, RD11
  EXTU.B   RD14, RD18
  MOV.L    RD18, @(R15, 16)
  CMP/GT   RD11, RD18
  BT       L00800C37
  MOV.L    @(R15, 40), RD25
  MOV.L    @(R15, 44), RD26
  ADD      RD25, RD26, RD14
  MOV      RD14, RD11
  MOV      #0x15555556, RD7
  MOV      #5461, RD7
  DMULS    RD7, RD11
  STS      R1, RD6
  MOV      #12, RD7
  DMULU    RD7, RD6
  STS      R0, RD6
  SUB      RD6, RD11
  MOV      localcmds, RQ27
  EXTS.L   RD11
  LEA.Q    @(RQ27, RD11), RQ28
  MOV.Q    netbuffer, RQ29
  ADD      RQ29, #8, RQ30
  EXTS.L   RD26
  LEA.Q    @(RQ30, RD26), RQ7
  MOV.Q    @RQ28, RQ6
  MOV.Q    RQ6, @RQ7
  MOV.Q    RQ30, @(R15, 24)

L00800C36:
  MOV.L    @(R15, 44), RD16
  ADD      #1, RD16
  MOV.L    RD16, @(R15, 44)
  BRA      L00800C35

L00800C37:
  MOV      remoteresend, RQ16
  EXTS.L   RD13
  MOV.L    @(RQ16, RD13), RD14
  CMP/EQ   RD14
  BT       L00800C38
  MOV      nettics, RQ25
  EXTS.L   RD13
  MOV.L    @(RQ25, RD13), RD14
  MOV.Q    netbuffer, RQ26
  EXTU.B   RD14, RD14
  MOV.B    RD14, @(RQ26, 4)
  MOV      RD13, RD4
  MOV      #0x40000000, RD5
  MOV      #16384, RD5
  BSR      HSendPacket
  BRA      L00800C39

L00800C38:
  MOV      #0, RD25
  MOV.Q    netbuffer, RQ26
  EXTU.B   RD25, RD25
  MOV.B    RD25, @(RQ26, 4)
  MOV      RD13, RD4
  MOV      RD25, RD5
  BSR      HSendPacket

L00800C39:

L00800C33:

L00800C31:
  ADD      #1, RD13
  BRA      L00800C30

L00800C32:

L00800C28:
  BSR      GetPackets

L00E004D3:
  ADD      #168, R15
  BRA      L00C00194

TryRunTics:
  PUSH     LR
  BSR      L00C00193
  ADD      #-280, R15
  BSR      I_GetTime
  MOV      RD2, RD14
  MOV.L    ticdup, RD5
  MOV      RD14, RD4
  BSR      __sdivsi3
  MOV      RD2, RD31
  MOV.L    oldentertics, RD30
  SUB      RD31, RD30, RD9
  MOV      RD31, RD30
  MOV.L    RD31, @(R15, 60)
  MOV.L    RD30, oldentertics
  BSR      NetUpdate
  MOV      #0x7FFFFFFF, RD10
  MOV      #32767, RD10
  MOV      #0, RD29
  MOV      #0, RD13
  MOV.L    RD29, @(R15, 52)

L00800C74:
  MOV.Q    doomcom, RQ28
  MOV.W    @(RQ28, 16), RD14
  MOV      RD13, RD12
  EXTS.W   RD14, RD8
  CMP/GT   RD12, RD8
  BT       L00800C76
  MOV      nodeingame, RQ27
  EXTS.L   RD13
  MOV.L    @(RQ27, RD13), RD14
  CMP/EQ   RD14
  BT       L00800C77
  MOV.L    @(R15, 52), RD26
  ADD      #1, RD26
  MOV      nettics, RQ25
  EXTS.L   RD13
  MOV.L    @(RQ25, RD13), RD14
  MOV.L    RD26, @(R15, 52)
  CMP/GT   RD14, RD10
  BT       L00800C78
  MOV      nettics, RQ7
  EXTS.L   RD13
  MOV.L    @(RQ7, RD13), RD10

L00800C78:

L00800C77:

L00800C75:
  ADD      #1, RD13
  BRA      L00800C74

L00800C76:
  MOV.L    ticdup, RD5
  MOV.L    gametic, RD4
  BSR      __sdivsi3
  MOV      RD2, RD14
  SUB      RD10, RD14, RD25
  SUB      RD25, #1, RD14
  MOV.L    RD25, @(R15, 56)
  CMP/GT   RD9, RD14
  BT       L00800C79
  ADD      RD9, #1, RD11
  BRA      L00800C7A

L00800C79:
  MOV.L    @(R15, 56), RD6
  CMP/GT   RD9, RD6
  BT       L00800C7B
  MOV      RD9, RD11
  BRA      L00800C7C

L00800C7B:
  MOV.L    @(R15, 56), RD5
  MOV      RD5, RD11

L00800C7C:

L00800C7A:
  MOV      #1, RD4
  CMP/GE   RD4, RD11
  BT       L00800C7D
  MOV      #1, RD11

L00800C7D:
  MOV.L    frameon, RD3
  ADD      #1, RD3
  MOV.L    RD3, frameon
  MOV.Q    debugfile, RQ2
  MOV      #0, RQ23
  CMPQ/EQ  RQ23, RQ2
  BT       L00800C7E
  MOV.Q    debugfile, RQ4
  MOV      L00C002DF, RQ5
  MOV      RD9, RD6
  MOV.L    @(R15, 56), RD7
  MOV      RD11, RD20
  BSR      fprintf

L00800C7E:
  MOV.L    demoplayback, RD22
  CMP/EQ   #0, RD22
  BT       L00800C7F
  MOV      #0, RD13

L00800C80:
  MOV      #4, RD21
  CMP/GE   RD21, RD13
  BT       L00800C82
  MOV      playeringame, RQ20
  EXTS.L   RD13
  MOV.L    @(RQ20, RD13), RD14
  CMP/EQ   RD14
  BT       L00800C83
  BRA      L00800C82

L00800C83:

L00800C81:
  ADD      #1, RD13
  BRA      L00800C80

L00800C82:
  MOV      #4, RD19
  CMP/GE   RD19, RD13
  BT       L00800C84
  BRA      L00E004D4

L00800C84:
  MOV.L    consoleplayer, RD18
  CMP/EQ   RD13, RD18
  BT       L00800C85
  MOV      nettics, RQ17
  MOV.L    @RQ17, RD14
  MOV      nodeforplayer, RQ16
  EXTS.L   RD13
  MOV.L    @(RQ16, RD13), RD12
  MOV.L    @(RQ17, RD12), RD8
  CMP/GT   RD8, RD14
  BT       L00800C86
  MOV.L    gametime, RD16
  ADD      #-1, RD16
  MOV.L    RD16, gametime

L00800C86:
  MOV      nodeforplayer, RQ16
  EXTS.L   RD13
  MOV.L    @(RQ16, RD13), RD14
  MOV      nettics, RQ17
  MOV.L    @(RQ17, RD14), RD12
  MOV.L    oldnettics, RD18
  CMP/GT   RD12, RD18
  MOVT     RD14
  MOV.L    frameon, RD19
  AND      RD19, #3, RD12
  MOV      frameskip, RQ20
  EXTS.L   RD12
  MOV.L    RD14, @(RQ20, RD12)
  MOV.L    @RQ17, RD18
  MOV.L    @RQ20, RD14
  MOV.L    RD18, oldnettics
  CMP/EQ   RD14
  BT       L00800C87
  MOV      frameskip, RQ16
  MOV.L    @(RQ16, 4), RD14
  CMP/EQ   RD14
  BT       L00800C87
  BRA      L00800C8A

L00800C8A:
  MOV      frameskip, RQ16
  MOV.L    @(RQ16, 8), RD14
  CMP/EQ   RD14
  BT       L00800C87
  BRA      L00800C89

L00800C89:
  MOV      frameskip, RQ16
  MOV.L    @(RQ16, 12), RD14
  CMP/EQ   RD14
  BT       L00800C87
  BRA      L00800C88

L00800C88:
  MOV      #1, RD16
  MOV.L    RD16, skiptics

L00800C87:

L00800C85:

L00800C7F:

L00800C8C:
  MOV.L    ticdup, RD5
  MOV.L    gametic, RD4
  BSR      __sdivsi3
  MOV      RD2, RD14
  ADD      RD14, RD11, RD12
  CMP/GT   RD10, RD12
  BT       L00800C8D
  BSR      NetUpdate
  MOV      #0x7FFFFFFF, RD10
  MOV      #32767, RD10
  MOV      #0, RD13

L00800C8E:
  MOV.Q    doomcom, RQ16
  MOV.W    @(RQ16, 16), RD14
  MOV      RD13, RD12
  EXTS.W   RD14, RD8
  CMP/GT   RD12, RD8
  BT       L00800C90
  MOV      nodeingame, RQ16
  EXTS.L   RD13
  MOV.L    @(RQ16, RD13), RD14
  CMP/EQ   RD14
  BT       L00800C91
  MOV      nettics, RQ16
  EXTS.L   RD13
  MOV.L    @(RQ16, RD13), RD14
  CMP/GT   RD14, RD10
  BT       L00800C91
  BRA      L00800C92

L00800C92:
  MOV      nettics, RQ16
  EXTS.L   RD13
  MOV.L    @(RQ16, RD13), RD10

L00800C91:

L00800C8F:
  ADD      #1, RD13
  BRA      L00800C8E

L00800C90:
  MOV.L    ticdup, RD5
  MOV.L    gametic, RD4
  BSR      __sdivsi3
  MOV      RD2, RD14
  CMP/GT   RD10, RD14
  BT       L00800C94
  MOV      L00C002E5, RQ4
  BSR      I_Error

L00800C94:
  BSR      I_GetTime
  MOV      RD2, RD14
  MOV.L    ticdup, RD5
  MOV      RD14, RD4
  BSR      __sdivsi3
  MOV      RD2, RD12
  MOV.L    @(R15, 60), RD25
  SUB      RD12, RD25, RD14
  CMP/GE   #20, RD14
  BT       L00800C95
  BSR      M_Ticker
  BRA      L00E004D4

L00800C95:
  BRA      L00800C8C

L00800C8D:

L00800C96:
  MOV      RD11, RD14
  ADD      #-1, RD11
  CMP/EQ   RD14
  BT       L00800C97
  MOV      #0, RD13

L00800C98:
  MOV.L    ticdup, RD16
  CMP/GT   RD13, RD16
  BT       L00800C9A
  MOV.L    ticdup, RD5
  MOV.L    gametic, RD4
  BSR      __sdivsi3
  MOV      RD2, RD14
  CMP/GT   RD10, RD14
  BT       L00800C9B
  MOV      L00C002E7, RQ4
  BSR      I_Error

L00800C9B:
  MOV.L    advancedemo, RD16
  CMP/EQ   #0, RD16
  BT       L00800C9C
  BSR      D_DoAdvanceDemo

L00800C9C:
  BSR      M_Ticker
  BSR      G_Ticker
  MOV.L    gametic, RD16
  ADD      #1, RD16
  MOV.L    ticdup, RD17
  SUB      RD17, #1, RD14
  MOV.L    RD16, gametic
  CMP/EQ   RD14, RD13
  BT       L00800C9D
  MOV.L    ticdup, RD5
  MOV.L    gametic, RD4
  BSR      __sdivsi3
  MOV      RD2, RD14
  MOV      RD14, RD25
  MOV      #0x15555556, RD7
  MOV      #5461, RD7
  DMULS    RD7, RD25
  STS      R1, RD6
  MOV      #12, RD7
  DMULU    RD7, RD6
  STS      R0, RD6
  SUB      RD6, RD25
  MOV      #0, RD26
  MOV.L    RD26, @(R15, 32)
  MOV.L    RD25, @(R15, 36)

L00800C9E:
  MOV.L    @(R15, 32), RD16
  MOV      #4, RD17
  CMP/GE   RD17, RD16
  BT       L00800CA0
  MOV      netcmds, RQ25
  MOV.L    @(R15, 32), RD26
  EXTS.L   RD26
  MOV      #96, RD7
  MUL.L    RD26, RD7
  STS      R0, RQ27
  ADD      RQ25, RQ27
  MOV.L    @(R15, 36), RD28
  EXTS.L   RD28
  LEA.Q    @(RQ27, RD28), RQ29
  MOV      RQ29, RQ30
  MOV      #0, RD31
  EXTU.B   RD31, RD31
  MOV.B    RD31, @(RQ30, 6)
  MOV.B    @(RQ30, 7), RD14
  EXTU.B   RD14, RD14
  MOV.Q    RQ30, @(R15, 40)
  TST      #128, RD14
  BT       L00800CA1
  MOV      #0, RD16
  MOV.Q    @(R15, 40), RQ17
  EXTU.B   RD16, RD16
  MOV.B    RD16, @(RQ17, 7)

L00800CA1:

L00800C9F:
  MOV.L    @(R15, 32), RD16
  ADD      #1, RD16
  MOV.L    RD16, @(R15, 32)
  BRA      L00800C9E

L00800CA0:

L00800C9D:

L00800C99:
  ADD      #1, RD13
  BRA      L00800C98

L00800C9A:
  BSR      NetUpdate
  BRA      L00800C96

L00800C97:

L00E004D4:
  ADD      #280, R15
  BRA      L00C00194

I_InitSound:
  PUSH     LR
  BSR      L00C00193
  ADD      #-120, R15
  BSR      __get_stderr
  MOV      RQ2, RQ14
  MOV.Q    @RQ14, RQ12
  MOV      RQ12, RQ4
  MOV      L00C003D4, RQ5
  BSR      fprintf
  MOV      #11025, RD13
  BSR      __get_stderr
  MOV      RQ2, RQ14
  MOV.Q    @RQ14, RQ12
  MOV      RQ12, RQ4
  MOV      L00C003D5, RQ5
  BSR      fprintf
  BSR      __get_stderr
  MOV      RQ2, RQ14
  MOV.Q    @RQ14, RQ12
  MOV      RQ12, RQ4
  MOV      L00C003D4, RQ5
  BSR      fprintf
  MOV      #1, RD13

L00800983:
  CMP/GE   #109, RD13
  BT       L00800985
  MOV      S_sfx, RQ11
  EXTS.L   RD13
  MOV      #48, RD7
  MUL.L    RD13, RD7
  STS      R0, RQ10
  ADD      RQ11, RQ10
  MOV.Q    @(RQ10, 16), RQ14
  MOV.Q    RQ10, @(R15, 24)
  MOV      #0, RQ9
  CMPQ/EQ  RQ9, RQ14
  BT       L00800986
  MOV      lengths, RQ8
  EXTS.L   RD13
  LEA.L    @(RQ8, RD13), RQ14
  MOV      S_sfx, RQ31
  EXTS.L   RD13
  MOV      #48, RD7
  MUL.L    RD13, RD7
  STS      R0, RQ30
  ADD      RQ31, RQ30
  MOV.Q    @RQ30, RQ12
  MOV.Q    RQ30, @(R15, 24)
  MOV      RQ12, RQ4
  MOV      RQ14, RQ5
  BSR      getsfx
  MOV      RQ2, RQ29
  MOV      S_sfx, RQ28
  EXTS.L   RD13
  MOV      #48, RD7
  MUL.L    RD13, RD7
  STS      R0, RQ30
  ADD      RQ28, RQ30
  MOV.Q    RQ29, @(RQ30, 32)
  MOV.Q    RQ30, @(R15, 24)
  MOV.Q    RQ29, @(R15, 16)
  BRA      L00800987

L00800986:
  MOV      S_sfx, RQ27
  EXTS.L   RD13
  MOV      #48, RD7
  MUL.L    RD13, RD7
  STS      R0, RQ26
  ADD      RQ27, RQ26
  MOV.Q    @(RQ26, 16), RQ14
  MOV.Q    @(RQ14, 32), RQ12
  EXTS.L   RD13
  MOV      #48, RD7
  MUL.L    RD13, RD7
  STS      R0, RQ26
  ADD      RQ27, RQ26
  MOV.Q    RQ12, @(RQ26, 32)
  EXTS.L   RD13
  MOV      #48, RD7
  MUL.L    RD13, RD7
  STS      R0, RQ26
  ADD      RQ27, RQ26
  MOV.Q    @(RQ26, 16), RQ14
  MOV      RQ14, RD25
  SUB      RQ27, RD25
  MOV      #0x05555556, RD7
  MOV      #1365, RD7
  DMULS    RD7, RD25
  STS      R1, RD7
  MOV      RD7, RD25
  MOV      RD25, RD28
  MOV      #0x05555556, RD7
  MOV      #1365, RD7
  DMULS    RD7, RD28
  STS      R1, RD7
  MOV      RD7, RD28
  MOV      lengths, RQ29
  EXTS.L   RD28
  MOV.L    @(RQ29, RD28), RD25
  EXTS.L   RD13
  MOV.L    RD25, @(RQ29, RD13)
  MOV.Q    RQ26, @(R15, 24)

L00800987:

L00800984:
  ADD      #1, RD13
  BRA      L00800983

L00800985:
  BSR      __get_stderr
  MOV      RQ2, RQ14
  MOV.Q    @RQ14, RQ12
  MOV      RQ12, RQ4
  MOV      L00C003D8, RQ5
  BSR      fprintf
  MOV      #0, RD13

L00800988:
  MOV      #2048, RD7
  CMP/GE   RD7, RD13
  BT       L0080098A
  MOV      mixbuffer, RQ6
  MOV      #0, RD5
  EXTS.L   RD13
  MOV.W    RD5, @(RQ6, RD13)

L00800989:
  ADD      #1, RD13
  BRA      L00800988

L0080098A:
  BSR      __get_stderr
  MOV      RQ2, RQ14
  MOV.Q    @RQ14, RQ12
  MOV      RQ12, RQ4
  MOV      L00C003DA, RQ5
  BSR      fprintf

L00E004D5:
  ADD      #120, R15
  BRA      L00C00194

I_UpdateSound:
  PUSH     LR
  BSR      L00C00193
  ADD      #-72, R15
  MOV      mixbuffer, RQ31
  MOV      RQ31, RQ8
  ADD      RQ31, #2, RQ30
  MOV      #2, RD29
  SHAD     RD29, #9, RD10
  LEA.W    @(RQ31, RD10), RQ28
  MOV.Q    RQ30, @(R15, 40)
  MOV.L    RD29, @(R15, 28)
  MOV.Q    RQ28, @(R15, 32)

L00800965:
  MOV.Q    @(R15, 32), RQ27
  CMPQ/EQ  RQ27, RQ8
  BT       L00800966
  MOV      #0, RD14
  MOV      #0, RD13
  MOV      #0, RD11

L00800967:
  MOV      #8, RD26
  CMP/GE   RD26, RD11
  BT       L00800969
  MOV      channels, RQ25
  EXTS.L   RD11
  MOV.Q    @(RQ25, RD11), RQ9
  MOV      #0, RQ7
  CMPQ/EQ  RQ7, RQ9
  BT       L0080096A
  MOV      channels, RQ6
  EXTS.L   RD11
  MOV.Q    @(RQ6, RD11), RQ9
  MOVU.B   @RQ9, RD10
  MOV      RD10, RD12
  MOV      channelleftvol_lookup, RQ5
  EXTS.L   RD11
  MOV.Q    @(RQ5, RD11), RQ9
  MOV.L    @(RQ9, RD12), RD10
  ADD      RD10, RD14
  MOV      channelrightvol_lookup, RQ4
  EXTS.L   RD11
  MOV.Q    @(RQ4, RD11), RQ9
  MOV.L    @(RQ9, RD12), RD10
  ADD      RD10, RD13
  MOV      channelstepremainder, RQ3
  EXTS.L   RD11
  MOVU.L   @(RQ3, RD11), RD10
  MOV      channelstep, RQ2
  EXTS.L   RD11
  MOVU.L   @(RQ2, RD11), RD23
  ADD      RD10, RD23, RD22
  EXTS.L   RD11
  MOVU.L   RD22, @(RQ3, RD11)
  EXTS.L   RD11
  MOV.Q    @(RQ6, RD11), RQ9
  EXTS.L   RD11
  MOVU.L   @(RQ3, RD11), RD10
  SHLD     RD10, #-16, RD23
  LEA.B    @(RQ9, RD23), RQ21
  EXTS.L   RD11
  MOV.Q    RQ21, @(RQ6, RD11)
  EXTS.L   RD11
  MOVU.L   @(RQ3, RD11), RD10
  EXTU.W   RD10, RD23
  EXTS.L   RD11
  MOVU.L   RD23, @(RQ3, RD11)
  EXTS.L   RD11
  MOV.Q    @(RQ6, RD11), RQ9
  MOV      channelsend, RQ20
  EXTS.L   RD11
  MOV.Q    @(RQ20, RD11), RQ21
  MOV.Q    RQ21, @R15
  CMPQ/GT  RQ9, RQ21
  BT       L0080096B
  MOV      #0, RQ9
  MOV      channels, RQ19
  EXTS.L   RD11
  MOV.Q    RQ9, @(RQ19, RD11)

L0080096B:

L0080096A:

L00800968:
  ADD      #1, RD11
  BRA      L00800967

L00800969:
  CMP/GT   #32767, RD14
  BT       L0080096C
  MOV      #32767, RD18
  MOV.W    RD18, @RQ8
  BRA      L0080096D

L0080096C:
  MOV      #-32768, RD17
  CMP/GE   RD17, RD14
  BT       L0080096E
  MOV      #-32768, RD16
  MOV.W    RD16, @RQ8
  BRA      L0080096F

L0080096E:
  MOV.W    RD14, @RQ8

L0080096F:

L0080096D:
  CMP/GT   #32767, RD13
  BT       L00800970
  MOV.Q    @(R15, 40), RQ16
  MOV      #32767, RD17
  MOV.W    RD17, @RQ16
  BRA      L00800971

L00800970:
  MOV      #-32768, RD16
  CMP/GE   RD16, RD13
  BT       L00800972
  MOV.Q    @(R15, 40), RQ16
  MOV      #-32768, RD17
  MOV.W    RD17, @RQ16
  BRA      L00800973

L00800972:
  MOV.Q    @(R15, 40), RQ16
  MOV.W    RD13, @RQ16

L00800973:

L00800971:
  MOV.L    @(R15, 28), RD16
  EXTS.L   RD16
  LEA.W    @(RQ8, RD16), RQ8
  MOV.Q    @(R15, 40), RQ17
  EXTS.L   RD16
  LEA.W    @(RQ17, RD16), RQ17
  MOV.Q    RQ17, @(R15, 40)
  BRA      L00800965

L00800966:

L00E004D6:
  ADD      #72, R15
  BRA      L00C00194

I_SubmitSound:
  PUSH     LR
  ADD      #-136, R15
  MOV      #0, RD4
  BSR      I_SubmitSound2

L00E004D7:
  ADD      #136, R15
  RET     

L00C003E5:
  PUSH     R31
  PUSH     R30
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  PUSH     R9
  PUSH     R8
  PUSH     FR15
  PUSH     FR14
  PUSH     FR13
  PUSH     FR12
  PUSH     FR11
  PUSH     FR10
  PUSH     FR9
  RTS     

I_SetChannels:
  PUSH     LR
  BSR      L00C003E5
  ADD      #-136, R15
  MOV      steptable, RQ9
  ADD      RQ9, #512, RQ11
  MOV      RQ11, RQ10
  MOV      #-128, RD14

L0080095C:
  CMP/GE   #128, RD14
  BT       L0080095E
  EXTS.L   RD14
  MOV      RD14, R0
  FLDCI    FR15
  MOV      #0, DR14
  FMOV     DR14, FR13
  FMOV     FR15, FR12
  FMUL     FR13, FR12
  FMOV     FR12, DR11
  FMOV.D   DR11, @(R15, 24)
  MOV      #0, DR4
  FMOV     DR11, DR5
  BSR      pow
  FMOV     DR2, DR10
  FMOV     DR10, DR11
  MOV      #0, DR9
  FMUL     DR9, DR11
  FSTCI    DR11
  MOV      R0, RD13
  EXTS.L   RD14
  MOV.L    RD13, @(RQ10, RD14)
  FMOV.D   DR11, @(R15, 24)
  FMOV.D   DR10, @(R15, 32)

L0080095D:
  ADD      #1, RD14
  BRA      L0080095C

L0080095E:
  MOV      #0, RD14

L0080095F:
  CMP/GE   #128, RD14
  BT       L00800961
  MOV      #0, RD12

L00800962:
  CMP/GE   #256, RD12
  BT       L00800964
  SUB      RD12, #128, RD13
  MUL.L    RD14, RD13, RD8
  SHAD     RD8, #8, RD13
  MOV      RD13, RD8
  MOV      #0x02040811, RD7
  MOV      #516, RD7
  DMULS    RD7, RD8
  STS      R1, RD8
  SHAD     RD14, #8, RD13
  ADD      RD13, RD12, RD31
  MOV      vol_lookup, RQ30
  EXTS.L   RD31
  MOV.L    RD8, @(RQ30, RD31)

L00800963:
  ADD      #1, RD12
  BRA      L00800962

L00800964:

L00800960:
  ADD      #1, RD14
  BRA      L0080095F

L00800961:

L00E004D8:
  ADD      #136, R15

L00C003E8:
  POP      FR9
  POP      FR10
  POP      FR11
  POP      FR12
  POP      FR13
  POP      FR14
  POP      FR15
  POP      RQ8
  POP      RQ9
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  POP      RQ30
  POP      RQ31
  RET     

I_GetSfxLumpNum:
  PUSH     LR
  PUSH     R14
  PUSH     R13
  ADD      #-184, R15
  MOV.Q    RQ4, @(R15, 24)
  MOV      #32, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 16)
  MOV.Q    @(R15, 24), RQ14
  MOV.Q    @RQ14, RQ13
  MOV.Q    RQ13, @(R15, 8)
  MOV      #32, RQ4
  ADD      R15, RQ4
  MOV      L00C003EA, RQ5
  MOV      RQ13, RQ6
  BSR      sprintf
  MOV      #32, RQ4
  ADD      R15, RQ4
  BSR      W_GetNumForName
  MOV      RD2, RD13
  MOV.L    RD13, @(R15, 4)
  MOV      RD13, RD2

L00E004D9:
  ADD      #184, R15
  POP      RQ13
  POP      RQ14
  RET     

L00C003ED:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  RTS     

I_StartSound:
  PUSH     LR
  BSR      L00C003ED
  ADD      #-128, R15
  MOV      RD4, RD13
  MOV.L    RD5, @(R15, 24)
  MOV.L    RD6, @(R15, 20)
  MOV.L    RD7, @(R15, 16)
  MOV.L    RD20, @(R15, 12)
  MOV      #0, RD12
  MOV      steptable, RQ11
  MOV.L    @(R15, 16), RD10
  MOV.L    @(RQ11, RD10), RD14
  MOV.L    RD12, @(R15, 12)
  MOV      RD13, RD4
  MOV.L    @(R15, 24), RD5
  MOV      RD14, RD6
  MOV.L    @(R15, 20), RD7
  BSR      addsfx
  MOV      RD2, RD13
  MOV      RD13, RD2

L00E004DA:
  ADD      #128, R15

L00C003EF:
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  RET     

I_StopSound:
  PUSH     R14
  ADD      #-152, R15
  MOV      RD4, RD14
  MOV      #0, RD14

L00E004DB:
  ADD      #152, R15
  POP      RQ14
  RTS     

I_SoundIsPlaying:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  ADD      #-136, R15
  MOV.L    RD4, @(R15, 12)
  MOV.L    gametic, RD13
  MOV.L    @(R15, 12), RD12
  CMP/GT   RD13, RD12
  MOVT     RD14
  MOV      RD14, RD2

L00E004DC:
  ADD      #136, R15

L00C003F2:
  POP      RQ12
  POP      RQ13
  POP      RQ14
  RTS     

I_UpdateSoundParams:
  ADD      #-160, R15
  MOV.L    RD4, @(R15, 12)
  MOV.L    RD5, @(R15, 8)
  MOV.L    RD6, @(R15, 4)
  MOV.L    RD7, @R15
  MOV      #0, RD7
  MOV.L    RD7, @R15
  MOV      #0, RD7
  MOV.L    RD7, @(R15, 4)
  MOV      #0, RD7
  MOV.L    RD7, @(R15, 8)
  MOV      #0, RD7
  MOV.L    RD7, @(R15, 12)

L00E004DD:
  ADD      #160, R15
  RTS     

I_InitMusic:
  PUSH     LR
  BSR      L00C002C0
  ADD      #-96, R15
  BSR      SMus_Init
  MOV      L00C003F6, RQ4
  MOV      #1, RD5
  BSR      W_CacheLumpName
  MOV      RQ2, RQ11
  MOV.Q    RQ11, genmidi
  MOV.Q    genmidi, RQ10
  MOV      #0, RQ9
  CMPQ/EQ  RQ9, RQ10
  BT       L00800A47
  MOV      #0, RD13

L00800A48:
  CMP/GE   #175, RD13
  BT       L00800A4A
  MOV.Q    genmidi, RQ8
  ADD      RQ8, #8, RQ31
  MOV      RD13, RD14
  MOV      #36, RD30
  MUL.L    RD30, RD14, RD14
  EXTS.L   RD14
  LEA.B    @(RQ31, RD14), RQ9
  MOV      RQ9, RQ12
  MOVU.L   @RQ12, RD14
  MOV      RD13, RD4
  MOV      #0, RD5
  MOV      RD14, RD6
  BSR      SMus_SetFmRegisterData
  MOVU.L   @(RQ12, 4), RD14
  MOV      RD13, RD4
  MOV      #1, RD5
  MOV      RD14, RD6
  BSR      SMus_SetFmRegisterData
  MOVU.L   @(RQ12, 8), RD14
  MOV      RD13, RD4
  MOV      #2, RD5
  MOV      RD14, RD6
  BSR      SMus_SetFmRegisterData
  MOVU.L   @(RQ12, 12), RD14
  MOV      RD13, RD4
  MOV      #3, RD5
  MOV      RD14, RD6
  BSR      SMus_SetFmRegisterData
  MOVU.L   @(RQ12, 16), RD14
  MOV      RD13, RD4
  MOV      #4, RD5
  MOV      RD14, RD6
  BSR      SMus_SetFmRegisterData
  MOVU.L   @(RQ12, 20), RD14
  MOV      RD13, RD4
  MOV      #5, RD5
  MOV      RD14, RD6
  BSR      SMus_SetFmRegisterData
  MOVU.L   @(RQ12, 24), RD14
  MOV      RD13, RD4
  MOV      #6, RD5
  MOV      RD14, RD6
  BSR      SMus_SetFmRegisterData
  MOVU.L   @(RQ12, 28), RD14
  MOV      RD13, RD4
  MOV      #7, RD5
  MOV      RD14, RD6
  BSR      SMus_SetFmRegisterData
  MOVU.L   @(RQ12, 32), RD14
  MOV      RD13, RD4
  MOV      #8, RD5
  MOV      RD14, RD6
  BSR      SMus_SetFmRegisterData

L00800A49:
  ADD      #1, RD13
  BRA      L00800A48

L00800A4A:

L00800A47:

L00E004DE:
  ADD      #96, R15
  BRA      L00C002C3

I_SetMusicVolume:
  PUSH     R14
  PUSH     R13
  ADD      #-144, R15
  MOV.L    RD4, @(R15, 12)
  MOV.L    @(R15, 12), RD13
  MOV      RD13, RD14
  MOV.L    RD14, snd_MusicVolume

L00E004DF:
  ADD      #144, R15
  POP      RQ13
  POP      RQ14
  RTS     

I_PauseSong:
  ADD      #-160, R15
  MOV.L    RD4, @(R15, 12)
  MOV      #1, RD7
  MOV.L    RD7, i_mus_pause

L00E004E0:
  ADD      #160, R15
  RTS     

I_ResumeSong:
  ADD      #-160, R15
  MOV.L    RD4, @(R15, 12)
  MOV      #0, RD7
  MOV.L    RD7, i_mus_pause

L00E004E1:
  ADD      #160, R15
  RTS     

L00C003FF:
  PUSH     R31
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  PUSH     R9
  PUSH     R8
  RTS     

I_RegisterSong:
  PUSH     LR
  BSR      L00C003FF
  ADD      #-88, R15
  MOV      RQ4, RQ12
  MOV      #0, RQ11
  CMPQ/EQ  RQ11, RQ12
  BT       L00800A4B
  MOV      #0, RD2
  BRA      L00E004E2

L00800A4B:
  MOV      #1, RD14

L00800A4C:
  CMP/GE   #32, RD14
  BT       L00800A4E
  MOV      #1, RD10
  SHAD     RD10, RD14, RD13
  MOV.L    i_mus_song_mask, RD9
  TST      RD13, RD9
  BT       L00800A4F
  BRA      L00800A4E

L00800A4F:

L00800A4D:
  ADD      #1, RD14
  BRA      L00800A4C

L00800A4E:
  MOV      i_mus_songs, RQ8
  EXTS.L   RD14
  MOV.Q    RQ12, @(RQ8, RD14)
  MOV      #1, RD31
  SHAD     RD31, RD14, RD13
  MOV.L    i_mus_song_mask, RD7
  OR       RD13, RD7
  MOVU.W   @(RQ12, 4), RD13
  MOV      i_mus_song_len, RQ6
  EXTS.L   RD14
  MOV.L    RD13, @(RQ6, RD14)
  MOVU.W   @(RQ12, 6), RD13
  MOV      i_mus_song_ofs, RQ5
  EXTS.L   RD14
  MOV.L    RD13, @(RQ5, RD14)
  MOV.L    RD7, i_mus_song_mask
  MOV      RD14, RD2

L00E004E2:
  ADD      #88, R15

L00C00404:
  POP      RQ8
  POP      RQ9
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  POP      RQ31
  RET     

I_PlaySong:
  PUSH     LR
  BSR      L00C00190
  ADD      #-64, R15
  MOV      RD4, RD12
  MOV.L    RD5, @(R15, 24)
  BSR      I_InitMusic
  MOV      i_mus_song_ofs, RQ11
  EXTS.L   RD12
  MOV.L    @(RQ11, RD12), RD13
  MOV      i_mus_song_len, RQ10
  EXTS.L   RD12
  MOV.L    @(RQ10, RD12), RD9
  MOV      i_mus_songs, RQ8
  EXTS.L   RD12
  MOV.Q    @(RQ8, RD12), RQ31
  LEA.B    @(RQ31, RD13), RQ14
  MOV.L    RD9, @(R15, 20)
  MOV      L00C00406, RQ4
  MOV      RD13, RD5
  MOV      RD9, RD6
  BSR      printf
  MOV      RQ14, RQ30
  EXTS.L   RD9
  LEA.B    @(RQ14, RD9), RQ29
  MOV      RQ14, RQ28
  MOV      #0, RD27
  MOV.Q    RQ30, i_smus_css
  MOV.Q    RQ29, i_smus_cse
  MOV.Q    RQ28, i_smus_cs
  MOV.L    RD27, i_smus_tt
  BSR      SMus_SilenceAll
  MOV      RD12, RD26
  MOV      #0, RD27
  MOV.L    RD27, i_mus_curpos
  MOV.L    RD26, i_mus_curhandle

L00E004E3:
  ADD      #64, R15
  BRA      L00C00191

I_StopSong:
  ADD      #-160, R15
  MOV.L    RD4, @(R15, 12)
  MOV      #0, RD7
  MOV.L    RD7, i_mus_curhandle
  MOV      #0, RD7
  MOV.L    RD7, i_mus_curpos

L00E004E4:
  ADD      #160, R15
  RTS     

I_UnRegisterSong:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  ADD      #-120, R15
  MOV.L    RD4, @(R15, 12)
  MOV      #1, RD12
  MOV.L    @(R15, 12), RD11
  SHAD     RD12, RD11, RD14
  NOT      RD14, RD13
  MOV.L    i_mus_song_mask, RD10
  AND      RD13, RD10
  MOV.L    RD10, i_mus_song_mask

L00E004E5:
  ADD      #120, R15

L00C00410:
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  RTS     

I_InitGraphics:
  PUSH     LR
  BSR      L00C001D8
  ADD      #-128, R15
  MOV      #0x0001F400, RQ14
  MOV      #1, RQ14
  EXTS.L   RQ14, RQ14
  MOV      RQ14, RQ4
  BSR      malloc
  MOV      RQ2, RQ13
  MOV      screens, RQ12
  MOV.Q    RQ13, @RQ12
  MOV      #0x0001F400, RQ14
  MOV      #1, RQ14
  EXTS.L   RQ14, RQ14
  MOV      RQ14, RQ4
  BSR      malloc
  MOV      RQ2, RQ12
  MOV.Q    RQ12, vid_lastscreen
  BSR      IN_Init

L00E004E6:
  ADD      #128, R15
  BRA      L00C001D9

I_ShutdownGraphics:
  ADD      #-144, R15

L00E004E7:
  ADD      #144, R15
  RTS     

I_SetPalette:
  PUSH     LR
  BSR      L00C00193
  ADD      #-56, R15
  MOV.Q    RQ4, @(R15, 24)
  MOV      #0, RD11

L008008FF:
  CMP/GE   #256, RD11
  BT       L00800901
  MOV      RD11, RD14
  MOV      #3, RD31
  MUL.L    RD31, RD14, RD14
  MOV      RD14, RD13
  MOV.Q    @(R15, 24), RQ30
  EXTS.L   RD13
  MOVU.B   @(RQ30, RD13), RD10
  MOV      RD11, RD14
  MUL.L    RD31, RD14, RD14
  ADD      RD14, #1, RD13
  EXTS.L   RD13
  MOVU.B   @(RQ30, RD13), RD9
  MOV      RD11, RD14
  MUL.L    RD31, RD14, RD14
  ADD      RD14, #2, RD13
  EXTS.L   RD13
  MOVU.B   @(RQ30, RD13), RD8
  MOV      RD10, RD14
  MOV      #77, RD29
  MUL.L    RD29, RD14, RD14
  MOV      RD9, RD13
  MOV      #150, RD28
  MUL.L    RD28, RD13, RD13
  ADD      RD14, RD13, RD12
  MOV      RD8, RD14
  MOV      #29, RD27
  MUL.L    RD27, RD14, RD14
  ADD      RD12, RD14, RD13
  ADD      RD13, #127, RD14
  SHAD     RD14, #-8, RD26
  MOV      RD10, RD14
  MOV      #-43, RD25
  MUL.L    RD25, RD14, RD14
  MOV      RD9, RD13
  MOV      #85, RD31
  MUL.L    RD31, RD13, RD13
  SUB      RD14, RD13, RD12
  SHAD     RD8, #7, RD14
  ADD      RD12, RD14, RD13
  ADD      RD13, #127, RD14
  SHAD     RD14, #-8, RD13
  ADD      RD13, #128, RD25
  SHAD     RD10, #7, RD14
  MOV      RD9, RD13
  MOV      #107, RD31
  MUL.L    RD31, RD13, RD13
  SUB      RD14, RD13, RD12
  MOV      RD8, RD14
  MOV      #21, RD31
  MUL.L    RD31, RD14, RD14
  SUB      RD12, RD14, RD13
  ADD      RD13, #127, RD14
  SHAD     RD14, #-8, RD13
  ADD      RD13, #128, RD31
  ADD      RD26, #2, RD14
  MOV.L    RD31, @(R15, 12)
  MOV.L    RD26, @(R15, 20)
  MOV.L    RD25, @(R15, 16)
  MOV      RD14, RD4
  BSR      vid_clamp255
  MOV      RD2, RD13
  SHAD     RD13, #-2, RD26
  ADD      RD25, #4, RD14
  MOV.L    RD26, @(R15, 20)
  MOV      RD14, RD4
  BSR      vid_clamp255
  MOV      RD2, RD13
  SHAD     RD13, #-3, RD25
  ADD      RD31, #4, RD14
  MOV.L    RD25, @(R15, 16)
  MOV      RD14, RD4
  BSR      vid_clamp255
  MOV      RD2, RD13
  SHAD     RD13, #-3, RD31
  SHAD     RD26, #10, RD14
  SHAD     RD31, #5, RD13
  OR       RD14, RD13, RD12
  OR       RD12, RD25, RD14
  MOV      d_8to16table, RQ30
  EXTS.L   RD11
  MOVU.W   RD14, @(RQ30, RD11)
  MOV.L    RD31, @(R15, 12)

L00800900:
  ADD      #1, RD11
  BRA      L008008FF

L00800901:

L00E004E8:
  ADD      #56, R15
  BRA      L00C00194

I_UpdateNoBlit:
  ADD      #-144, R15

L00E004E9:
  ADD      #144, R15
  RTS     

I_FinishUpdate:
  PUSH     LR
  BSR      L00C00193
  ADD      #-72, R15
  MOV      #0xA00A0000, RQ31
  MOV      #0x0000A00A, RQ31
  MOV.L    vid_frnum, RD30
  ADD      #1, RD30
  MOV      screens, RQ29
  MOV.Q    @RQ29, RQ28
  MOV      RQ28, RQ11
  MOV.Q    vid_lastscreen, RQ27
  MOV      RQ27, RQ10
  MOV      RQ31, RQ13
  MOV      #0, RD12
  MOV      #0, RD26
  MOV.Q    RQ31, @(R15, 40)
  MOV.L    RD30, vid_frnum
  MOV.L    RD26, @(R15, 32)

L00800929:
  MOV.L    @(R15, 32), RD25
  MOV      #25, RD7
  CMP/GE   RD7, RD25
  BT       L0080092B
  MOV      #-1, RQ25
  EXTS.L   RQ25, RQ25
  MOV      RQ25, RQ26
  MOV.Q    RQ26, @(R15, 24)
  MOV.Q    RQ25, @R15
  MOV      #0, RQ27
  CMPQ/EQ  RQ27, RQ26
  BT       L0080092C
  ADD      #5120, RQ11
  ADD      #5120, RQ10
  ADD      #1280, RQ13
  BRA      L0080092A

L0080092C:
  MOV      RQ11, RQ9
  MOV      RQ10, RQ8
  MOV      #0, RD6
  MOV.L    RD6, @(R15, 36)

L0080092D:
  MOV.L    @(R15, 36), RD5
  CMP/GE   #40, RD5
  BT       L0080092F
  MOV      RQ9, RQ4
  MOV      RQ8, RQ5
  MOV      RQ13, RQ6
  BSR      VID_ConGfx_EncBlock16Q
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L00800930
  MOV      i_scr_bnc, RQ4
  MOV      #6, RD3
  EXTS.L   RD12
  MOVU.B   RD3, @(RQ4, RD12)
  BRA      L00800931

L00800930:
  MOV      i_scr_bnc, RQ2
  EXTS.L   RD12
  MOVU.B   @(RQ2, RD12), RD14
  CMP/PL   RD14
  BT       L00800932
  ADD      #-1, RD14
  CMP/EQ   RD14
  BT       L00800933
  MOV      RQ9, RQ4
  MOV      RQ8, RQ5
  MOV      RQ13, RQ6
  BSR      VID_ConGfx_EncBlock16HQ

L00800933:
  MOV      i_scr_bnc, RQ23
  EXTS.L   RD12
  MOVU.B   RD14, @(RQ23, RD12)

L00800932:

L00800931:
  ADD      #1, RD12
  ADD      #32, RQ13
  ADD      #16, RQ9
  ADD      #16, RQ8

L0080092E:
  MOV.L    @(R15, 36), RD22
  ADD      #1, RD22
  MOV.L    RD22, @(R15, 36)
  BRA      L0080092D

L0080092F:
  ADD      #5120, RQ11
  ADD      #5120, RQ10

L0080092A:
  MOV.L    @(R15, 32), RD21
  ADD      #1, RD21
  MOV.L    RD21, @(R15, 32)
  BRA      L00800929

L0080092B:
  MOV.Q    @(R15, 40), RQ25
  MOV.L    vid_frnum, RD26
  MOVU.L   RD26, @(RQ25, 32400)
  BSR      IN_Commands
  MOV      dirtybox, RQ4
  BSR      M_ClearBox
  BSR      R_ClearCZBuf
  MOV      #0, RD20
  MOV.L    RD20, @(R15, 20)

L00800934:
  MOV.L    @(R15, 20), RD19
  MOV      #25, RD18
  CMP/GE   RD18, RD19
  BT       L00800936
  MOV      #0, RQ17
  EXTS.L   RQ17, RQ17
  MOV      r_colmask, RQ16
  MOV.L    @(R15, 20), RD18
  EXTS.L   RD18
  MOV.Q    RQ17, @(RQ16, RD18)
  MOV.Q    RQ17, @R15

L00800935:
  MOV.L    @(R15, 20), RD16
  ADD      #1, RD16
  MOV.L    RD16, @(R15, 20)
  BRA      L00800934

L00800936:

L00E004EA:
  ADD      #72, R15
  BRA      L00C00194

I_WaitVBL:
  ADD      #-160, R15
  MOV.L    RD4, @(R15, 12)

L00E004EB:
  ADD      #160, R15
  RTS     

I_ReadScreen:
  PUSH     LR
  BSR      L00C001D8
  ADD      #-144, R15
  MOV.Q    RQ4, @(R15, 24)
  MOV      screens, RQ12
  MOV.Q    @RQ12, RQ14
  MOV      #0x0001F400, RQ13
  MOV      #1, RQ13
  EXTS.L   RQ13, RQ13
  MOV.Q    @(R15, 24), RQ4
  MOV      RQ14, RQ5
  MOV      RQ13, RQ6
  BSR      memcpy

L00E004EC:
  ADD      #144, R15
  BRA      L00C001D9

I_InitNetwork:
  PUSH     LR
  BSR      L00C001DF
  ADD      #-120, R15
  MOV      #144, RQ14
  EXTS.L   RQ14, RQ14
  MOV      RQ14, RQ4
  BSR      malloc
  MOV      RQ2, RQ13
  MOV      #144, RQ14
  EXTS.L   RQ14, RQ14
  MOV.Q    RQ13, doomcom
  MOV.Q    doomcom, RQ4
  MOV      #0, RD5
  MOV      RQ14, RQ6
  BSR      memset
  MOV      #1, RD12
  MOV.Q    doomcom, RQ11
  MOV.W    RD12, @(RQ11, 18)
  MOV      #0, RD13
  MOV.W    RD13, @(RQ11, 20)
  MOV      #0, RD13
  MOV      #0x12345678, RQ14
  MOV      #4660, RQ14
  MOV.Q    RQ14, @RQ11
  MOV.W    RD12, @(RQ11, 16)
  MOV.W    RD12, @(RQ11, 34)
  MOV      #0, RD12
  MOV.W    RD12, @(RQ11, 22)
  MOV.W    RD12, @(RQ11, 32)
  MOV.L    RD13, netgame

L00E004ED:
  ADD      #120, R15
  BRA      L00C001E0

I_NetCmd:
  ADD      #-144, R15

L00E004EE:
  ADD      #144, R15
  RTS     

R_GetColumn:
  PUSH     LR
  BSR      L00C00190
  ADD      #-64, R15
  MOV      RD4, RD12
  MOV      RD5, RD11
  MOV.Q    texturewidthmask, RQ31
  EXTS.L   RD12
  MOV.L    @(RQ31, RD12), RD13
  AND      RD13, RD11
  MOV.Q    texturecolumnlump, RQ30
  EXTS.L   RD12
  MOV.Q    @(RQ30, RD12), RQ14
  EXTS.L   RD11
  MOV.W    @(RQ14, RD11), RD9
  MOV.Q    texturecolumnofs, RQ29
  EXTS.L   RD12
  MOV.Q    @(RQ29, RD12), RQ14
  EXTS.L   RD11
  MOVU.W   @(RQ14, RD11), RD8
  CMP/PL   RD9
  BT       L00801D98
  MOV      RD9, RD4
  MOV      #101, RD5
  BSR      W_CacheLumpNum
  MOV      RQ2, RQ14
  LEA.B    @(RQ14, RD8), RQ10
  MOV      RQ10, RQ2
  BRA      L00E004EF

L00801D98:
  MOV.Q    texturecomposite, RQ28
  EXTS.L   RD12
  MOV.Q    @(RQ28, RD12), RQ14
  MOV      #0, RQ27
  CMPQ/EQ  RQ27, RQ14
  BT       L00801D99
  MOV      RD12, RD4
  BSR      R_GenerateComposite

L00801D99:
  MOV.Q    texturecomposite, RQ26
  EXTS.L   RD12
  MOV.Q    @(RQ26, RD12), RQ14
  EXTS.L   RD8
  LEA.B    @(RQ14, RD8), RQ10
  MOV      RQ10, RQ2

L00E004EF:
  ADD      #64, R15
  BRA      L00C00191

R_InitData:
  PUSH     LR
  ADD      #-136, R15
  BSR      R_InitTextures
  MOV      L00C00459, RQ4
  BSR      printf
  BSR      R_InitFlats
  MOV      L00C0045B, RQ4
  BSR      printf
  BSR      R_InitSpriteLumps
  MOV      L00C0045D, RQ4
  BSR      printf
  BSR      R_InitColormaps
  MOV      L00C0045F, RQ4
  BSR      printf

L00E004F0:
  ADD      #136, R15
  RET     

R_PrecacheLevel:
  PUSH     LR
  BSR      L00C00193
  ADD      #-392, R15
  MOV.L    demoplayback, RD10
  CMP/EQ   RD10
  BT       L00801DE0
  BRA      L00E004F1

L00801DE0:
  MOV.L    numflats, RQ12
  EXTS.L   RQ12, RQ12
  MOV      RQ12, RQ4
  BSR      malloc
  MOV      RQ2, RQ11
  MOV.L    numflats, RQ12
  EXTS.L   RQ12, RQ12
  MOV      RQ11, RQ4
  MOV      #0, RD5
  MOV      RQ12, RQ6
  BSR      memset
  MOV      #0, RD14

L00801DE1:
  MOV.L    numsectors, RD9
  CMP/GT   RD14, RD9
  BT       L00801DE3
  MOV.Q    sectors, RQ8
  EXTS.L   RD14
  SHAD     RD14, #7, RQ31
  ADD      RQ8, RQ31
  MOV.W    @(RQ31, 8), RD13
  MOV      #1, RD30
  MOV.B    RD30, @(RQ11, RD13)
  EXTS.L   RD14
  SHAD     RD14, #7, RQ31
  ADD      RQ8, RQ31
  MOV.W    @(RQ31, 10), RD13
  MOV.B    RD30, @(RQ11, RD13)

L00801DE2:
  ADD      #1, RD14
  BRA      L00801DE1

L00801DE3:
  MOV      #0, RD29
  MOV      #0, RD14
  MOV.L    RD29, flatmemory

L00801DE4:
  MOV.L    numflats, RD28
  CMP/GT   RD14, RD28
  BT       L00801DE6
  EXTS.L   RD14
  MOV.B    @(RQ11, RD14), RD13
  CMP/EQ   RD13
  BT       L00801DE7
  MOV.L    firstflat, RD26
  ADD      RD26, RD14, RD27
  MOV.Q    lumpinfo, RQ25
  EXTS.L   RD27
  MOV      #20, RD7
  MUL.L    RD27, RD7
  STS      R0, RQ28
  ADD      RQ25, RQ28
  MOV.L    @(RQ28, 16), RD13
  MOV.L    flatmemory, RD29
  ADD      RD13, RD29
  MOV.L    RD29, flatmemory
  MOV.Q    RQ28, @(R15, 88)
  MOV.L    RD27, @(R15, 148)
  MOV      RD27, RD4
  MOV      #101, RD5
  BSR      W_CacheLumpNum

L00801DE7:

L00801DE5:
  ADD      #1, RD14
  BRA      L00801DE4

L00801DE6:
  MOV.L    numtextures, RQ12
  EXTS.L   RQ12, RQ12
  MOV      RQ12, RQ4
  BSR      malloc
  MOV      RQ2, RQ25
  MOV.L    numtextures, RQ12
  EXTS.L   RQ12, RQ12
  MOV.Q    RQ25, @(R15, 168)
  MOV      RQ25, RQ4
  MOV      #0, RD5
  MOV      RQ12, RQ6
  BSR      memset
  MOV      #0, RD14

L00801DE8:
  MOV.L    numsides, RD7
  CMP/GT   RD14, RD7
  BT       L00801DEA
  MOV.Q    sides, RQ25
  EXTS.L   RD14
  MOV      #24, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ26
  ADD      RQ25, RQ26
  MOV.W    @(RQ26, 8), RD13
  MOV.Q    @(R15, 168), RQ27
  MOV      #1, RD28
  MOV.B    RD28, @(RQ27, RD13)
  EXTS.L   RD14
  MOV      #24, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ26
  ADD      RQ25, RQ26
  MOV.W    @(RQ26, 12), RD13
  MOV.B    RD28, @(RQ27, RD13)
  EXTS.L   RD14
  MOV      #24, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ26
  ADD      RQ25, RQ26
  MOV.W    @(RQ26, 10), RD13
  MOV.B    RD28, @(RQ27, RD13)

L00801DE9:
  ADD      #1, RD14
  BRA      L00801DE8

L00801DEA:
  MOV.Q    @(R15, 168), RQ6
  MOV.L    skytexture, RD5
  MOV      #1, RD4
  MOV.B    RD4, @(RQ6, RD5)
  MOV      #0, RD3
  MOV      #0, RD14
  MOV.L    RD3, texturememory

L00801DEB:
  MOV.L    numtextures, RD2
  CMP/GT   RD14, RD2
  BT       L00801DED
  MOV.Q    @(R15, 168), RQ23
  EXTS.L   RD14
  MOV.B    @(RQ23, RD14), RD13
  CMP/EQ   RD13
  BT       L00801DEE
  BRA      L00801DEC

L00801DEE:
  MOV.Q    textures, RQ22
  EXTS.L   RD14
  MOV.Q    @(RQ22, RD14), RQ21
  MOV      #0, RD20
  MOV.Q    RQ21, @(R15, 136)
  MOV.L    RD20, @(R15, 156)

L00801DEF:
  MOV.Q    @(R15, 136), RQ19
  MOV.W    @(RQ19, 12), RD13
  MOV.L    @(R15, 156), RD17
  MOV      RD17, RD18
  EXTS.W   RD13, RD16
  MOV.L    RD18, @(R15, 76)
  MOV.L    RD16, @(R15, 72)
  CMP/GT   RD18, RD16
  BT       L00801DF1
  MOV.Q    @(R15, 136), RQ25
  ADD      RQ25, #16, RQ26
  MOV.L    @(R15, 156), RD27
  EXTS.L   RD27
  MOV      #12, RD7
  MUL.L    RD27, RD7
  STS      R0, RQ28
  ADD      RQ26, RQ28
  MOV.L    @(RQ28, 8), RD29
  MOV.Q    lumpinfo, RQ30
  MOV      #20, RD7
  MUL.L    RD29, RD7
  STS      R0, RQ31
  ADD      RQ30, RQ31
  MOV.L    @(RQ31, 16), RD13
  MOV.L    texturememory, RD8
  ADD      RD13, RD8
  MOV.L    RD8, texturememory
  MOV.Q    RQ31, @(R15, 88)
  MOV.L    RD29, @(R15, 148)
  MOV.Q    RQ26, @(R15, 64)
  MOV      RD29, RD4
  MOV      #101, RD5
  BSR      W_CacheLumpNum

L00801DF0:
  MOV.L    @(R15, 156), RD16
  ADD      #1, RD16
  MOV.L    RD16, @(R15, 156)
  BRA      L00801DEF

L00801DF1:

L00801DEC:
  ADD      #1, RD14
  BRA      L00801DEB

L00801DED:
  MOV.L    numsprites, RQ12
  EXTS.L   RQ12, RQ12
  MOV      RQ12, RQ4
  BSR      malloc
  MOV      RQ2, RQ25
  MOV.L    numsprites, RQ12
  EXTS.L   RQ12, RQ12
  MOV.Q    RQ25, @(R15, 160)
  MOV      RQ25, RQ4
  MOV      #0, RD5
  MOV      RQ12, RQ6
  BSR      memset
  MOV      thinkercap, RQ16
  MOV.Q    @(RQ16, 8), RQ17
  MOV.Q    RQ17, @(R15, 128)

L00801DF2:
  MOV      thinkercap, RQ17
  MOV      RQ17, RQ16
  MOV.Q    RQ16, @(R15, 64)
  MOV.Q    @(R15, 128), RQ18
  CMPQ/EQ  RQ16, RQ18
  BT       L00801DF4
  MOV.Q    @(R15, 128), RQ16
  ADD      RQ16, #16, RQ17
  MOV.Q    @RQ17, RQ18
  MOV      P_MobjThinker, RQ20
  MOV      RQ20, RQ19
  MOV      RQ18, RQ21
  MOV      RQ19, RQ22
  MOV.Q    RQ22, @(R15, 16)
  MOV.Q    RQ21, @(R15, 24)
  MOV.Q    RQ19, @(R15, 32)
  MOV.Q    RQ18, @(R15, 64)
  CMPQ/EQ  RQ22, RQ21
  BT       L00801DF5
  MOV.Q    @(R15, 128), RQ16
  MOV.L    @(RQ16, 60), RD13
  MOV.Q    @(R15, 160), RQ17
  MOV      #1, RD18
  MOV.B    RD18, @(RQ17, RD13)

L00801DF5:

L00801DF3:
  MOV.Q    @(R15, 128), RQ16
  MOV.Q    @(RQ16, 8), RQ16
  MOV.Q    RQ16, @(R15, 128)
  BRA      L00801DF2

L00801DF4:
  MOV      #0, RD16
  MOV      #0, RD14
  MOV.L    RD16, spritememory

L00801DF6:
  MOV.L    numsprites, RD16
  CMP/GT   RD14, RD16
  BT       L00801DF8
  MOV.Q    @(R15, 160), RQ16
  EXTS.L   RD14
  MOV.B    @(RQ16, RD14), RD13
  CMP/EQ   RD13
  BT       L00801DF9
  BRA      L00801DF7

L00801DF9:
  MOV      #0, RD16
  MOV.L    RD16, @(R15, 156)

L00801DFA:
  MOV.Q    sprites, RQ16
  EXTS.L   RD14
  SHAD     RD14, #4, RQ17
  ADD      RQ16, RQ17
  MOV.L    @RQ17, RD13
  MOV.Q    RQ17, @R15
  MOV.L    @(R15, 156), RD18
  CMP/GT   RD18, RD13
  BT       L00801DFC
  MOV.Q    sprites, RQ25
  EXTS.L   RD14
  SHAD     RD14, #4, RQ26
  ADD      RQ25, RQ26
  MOV.Q    @(RQ26, 8), RQ27
  MOV.L    @(R15, 156), RD28
  EXTS.L   RD28
  MOV      #28, RD7
  MUL.L    RD28, RD7
  STS      R0, RQ29
  ADD      RQ27, RQ29
  MOV      RQ29, RQ30
  MOV      #0, RD31
  MOV.L    RD31, @(R15, 152)
  MOV.Q    RQ30, @(R15, 120)
  MOV.Q    RQ29, @(R15, 32)
  MOV.Q    RQ27, @(R15, 64)
  MOV.Q    RQ26, @R15

L00801DFD:
  MOV.L    @(R15, 152), RD16
  MOV      #8, RD17
  CMP/GE   RD17, RD16
  BT       L00801DFF
  MOV.Q    @(R15, 120), RQ25
  ADD      RQ25, #4, RQ26
  MOV.L    @(R15, 152), RD27
  EXTS.L   RD27
  MOV.W    @(RQ26, RD27), RD13
  MOV.L    firstspritelump, RD29
  ADD      RD29, RD13, RD28
  MOV.Q    lumpinfo, RQ30
  EXTS.L   RD28
  MOV      #20, RD7
  MUL.L    RD28, RD7
  STS      R0, RQ31
  ADD      RQ30, RQ31
  MOV.L    @(RQ31, 16), RD13
  MOV.L    spritememory, RD8
  ADD      RD13, RD8
  MOV.L    RD8, spritememory
  MOV.Q    RQ31, @(R15, 88)
  MOV.L    RD28, @(R15, 148)
  MOV.Q    RQ26, @(R15, 64)
  MOV      RD28, RD4
  MOV      #101, RD5
  BSR      W_CacheLumpNum

L00801DFE:
  MOV.L    @(R15, 152), RD16
  ADD      #1, RD16
  MOV.L    RD16, @(R15, 152)
  BRA      L00801DFD

L00801DFF:

L00801DFB:
  MOV.L    @(R15, 156), RD16
  ADD      #1, RD16
  MOV.L    RD16, @(R15, 156)
  BRA      L00801DFA

L00801DFC:

L00801DF7:
  ADD      #1, RD14
  BRA      L00801DF6

L00801DF8:

L00E004F1:
  ADD      #392, R15
  BRA      L00C00194

R_FlatNumForName:
  PUSH     LR
  BSR      L00C00198
  ADD      #-136, R15
  MOV.Q    RQ4, @(R15, 24)
  MOV      #36, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 16)
  MOV.Q    @(R15, 24), RQ4
  MOV      L00C0046A, RQ5
  BSR      strcmp
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L00801DD8
  MOV.L    skyflatnum, RD2
  BRA      L00E004F2

L00801DD8:
  MOV.Q    @(R15, 24), RQ4
  BSR      W_CheckNumForName
  MOV      RD2, RD13
  CMP/EQ   #-1, RD13
  BT       L00801DD9
  MOV      #36, RQ12
  ADD      R15, RQ12
  MOV      #0, RD11
  MOV.B    RD11, @(RQ12, 8)
  MOV      #8, RQ10
  EXTS.L   RQ10, RQ10
  MOV.Q    RQ10, @R15
  MOV      RQ12, RQ4
  MOV.Q    @(R15, 24), RQ5
  MOV      RQ10, RQ6
  BSR      memcpy
  MOV      L00C0046C, RQ4
  MOV      RQ12, RQ5
  BSR      I_Error

L00801DD9:
  MOV.L    firstflat, RD9
  SUB      RD13, RD9, RD14
  MOV      RD14, RD2

L00E004F2:
  ADD      #136, R15
  BRA      L00C0019A

R_TextureNumForName:
  PUSH     LR
  BSR      L00C001D8
  ADD      #-160, R15
  MOV.Q    RQ4, @(R15, 24)
  MOV      #36, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 16)
  MOV      #8, RQ14
  EXTS.L   RQ14, RQ14
  MOV.Q    RQ14, @R15
  MOV      #36, RQ4
  ADD      R15, RQ4
  MOV.Q    @(R15, 24), RQ5
  MOV      RQ14, RQ6
  BSR      memcpy
  MOV      #36, RQ13
  ADD      R15, RQ13
  MOV      #0, RD12
  MOV.B    RD12, @(RQ13, 8)
  MOV      RQ13, RQ4
  BSR      R_CheckTextureNumForName
  MOV      RD2, RD14
  MOV.L    RD14, @(R15, 12)
  CMP/EQ   #-1, RD14
  BT       L00801DDF
  MOV      #0, RD2
  BRA      L00E004F3

L00801DDF:
  MOV.L    @(R15, 12), RD2

L00E004F3:
  ADD      #160, R15
  BRA      L00C001D9

L00C0046F:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  PUSH     R9
  PUSH     R8
  RTS     

R_CheckTextureNumForName:
  PUSH     LR
  BSR      L00C0046F
  ADD      #-128, R15
  MOV.Q    RQ4, @(R15, 32)
  MOV.Q    @(R15, 32), RQ12
  MOV.B    @RQ12, RD13
  CMP/EQ   #45, RD13
  BT       L00801DDA
  MOV      #0, RD2
  BRA      L00E004F4

L00801DDA:
  MOV      #0, RD14

L00801DDB:
  MOV.L    numtextures, RD11
  CMP/GT   RD14, RD11
  BT       L00801DDD
  MOV.Q    textures, RQ10
  EXTS.L   RD14
  MOV.Q    @(RQ10, RD14), RQ9
  MOV      RQ9, RQ8
  MOV      #8, RQ11
  EXTS.L   RQ11, RQ11
  MOV.Q    RQ11, @R15
  MOV.Q    RQ8, @(R15, 8)
  MOV      RQ8, RQ4
  MOV.Q    @(R15, 32), RQ5
  MOV      RQ11, RQ6
  BSR      strnicmp
  MOV      RD2, RD13
  CMP/EQ   RD13
  BT       L00801DDE
  MOV      RD14, RD2
  BRA      L00E004F4

L00801DDE:

L00801DDC:
  ADD      #1, RD14
  BRA      L00801DDB

L00801DDD:
  MOV      #-1, RD2

L00E004F4:
  ADD      #128, R15

L00C00470:
  POP      RQ8
  POP      RQ9
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  RET     

L00C00472:
  PUSH     R31
  PUSH     R30
  PUSH     R29
  PUSH     R28
  PUSH     R27
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  PUSH     R9
  PUSH     R8
  RTS     

R_ColormapForLump:
  PUSH     LR
  BSR      L00C00472
  ADD      #-72, R15
  MOV.L    RD4, @(R15, 28)
  MOV.L    RD5, @(R15, 24)
  MOV.L    n_colormaps_alt, RD11
  CMP/GT   #1, RD11
  BT       L00801DD1
  MOV      #0, RQ14
  MOV      RQ14, RQ2
  BRA      L00E004F5

L00801DD1:
  MOV      colormaps_aidx, RQ10
  MOV.L    @RQ10, RD12
  MOV.L    @(R15, 28), RD9
  CMP/GT   RD12, RD9
  BT       L00801DD2
  MOV      #0, RQ14
  MOV      RQ14, RQ2
  BRA      L00E004F5

L00801DD2:
  MOV      #0, RD13

L00801DD3:
  MOV.L    n_colormaps_alt, RD8
  CMP/GT   RD13, RD8
  BT       L00801DD5
  MOV      colormaps_aidx, RQ31
  EXTS.L   RD13
  MOV.L    @(RQ31, RD13), RD12
  MOV.L    @(R15, 28), RD30
  CMP/GT   RD12, RD30
  BT       L00801DD6
  BRA      L00801DD5

L00801DD6:

L00801DD4:
  ADD      #1, RD13
  BRA      L00801DD3

L00801DD5:
  MOV.L    n_colormaps_alt, RD29
  CMP/GT   RD13, RD29
  BT       L00801DD7
  MOV      colormaps_alt, RQ28
  EXTS.L   RD13
  MOV.Q    @(RQ28, RD13), RQ27
  MOV.L    @(R15, 24), RD7
  SHAD     RD7, #8, RD12
  LEA.W    @(RQ27, RD12), RQ14
  MOV.Q    RQ27, @(R15, 16)
  MOV      RQ14, RQ2
  BRA      L00E004F5

L00801DD7:
  MOV      #0, RQ14
  MOV      RQ14, RQ2

L00E004F5:
  ADD      #72, R15

L00C00476:
  POP      RQ8
  POP      RQ9
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  POP      RQ27
  POP      RQ28
  POP      RQ29
  POP      RQ30
  POP      RQ31
  RET     

V_Init:
  PUSH     LR
  BSR      L00C002C5
  ADD      #-80, R15
  MOV      #0x0007D000, RD4
  MOV      #7, RD4
  BSR      I_AllocLow
  MOV      RQ2, RQ14
  MOV      RQ14, RQ12
  MOV      #0, RD13
  MOV.Q    RQ12, @(R15, 24)

L0080230B:
  MOV      #4, RD11
  CMP/GE   RD11, RD13
  BT       L0080230D
  MOV      RD13, RD10
  MOV      #320, RD9
  MUL.L    RD9, RD10, RD10
  MOV      RD10, RD8
  MOV      #200, RD31
  MUL.L    RD31, RD8, RD8
  MOV.Q    @(R15, 24), RQ30
  EXTS.L   RD8
  LEA.W    @(RQ30, RD8), RQ14
  MOV      screens, RQ29
  EXTS.L   RD13
  MOV.Q    RQ14, @(RQ29, RD13)

L0080230C:
  ADD      #1, RD13
  BRA      L0080230B

L0080230D:
  MOV      #0x0001F400, RD4
  MOV      #1, RD4
  BSR      I_AllocLow
  MOV      RQ2, RQ14
  MOV      RQ14, RQ28
  MOV.Q    RQ28, screens_zbuf

L00E004F6:
  ADD      #80, R15
  BRA      L00C002C6

V_CopyRect:
  PUSH     LR
  BSR      L00C00193
  ADD      #-104, R15
  MOV.L    RD4, @(R15, 76)
  MOV.L    RD5, @(R15, 72)
  MOV.L    RD6, @(R15, 68)
  MOV.L    RD7, @(R15, 64)
  MOV.L    RD20, @(R15, 60)
  MOV.L    RD21, @(R15, 56)
  MOV      RD22, RD13
  MOV.L    RD23, @(R15, 48)
  MOV.L    @(R15, 76), RD12
  CMP/PZ   RD12
  BT       L008022BC
  MOV.L    @(R15, 76), RD11
  MOV.L    @(R15, 64), RD10
  ADD      RD11, RD10, RD14
  CMP/GT   #320, RD14
  BT       L008022BC
  BRA      L008022C4

L008022C4:
  MOV.L    @(R15, 72), RD9
  CMP/PZ   RD9
  BT       L008022BC
  BRA      L008022C3

L008022C3:
  MOV.L    @(R15, 72), RD8
  MOV.L    @(R15, 60), RD31
  ADD      RD8, RD31, RD14
  CMP/GT   #200, RD14
  BT       L008022BC
  BRA      L008022C2

L008022C2:
  MOV.L    @(R15, 56), RD30
  MOV      #0, RD29
  CMP/GE   RD29, RD30
  BT       L008022BC
  BRA      L008022C1

L008022C1:
  MOV.L    @(R15, 56), RD28
  MOV.L    @(R15, 64), RD27
  ADD      RD28, RD27, RD14
  CMP/GT   #320, RD14
  BT       L008022BC
  BRA      L008022C0

L008022C0:
  CMP/PZ   RD13
  BT       L008022BC
  BRA      L008022BF

L008022BF:
  MOV.L    @(R15, 60), RD26
  ADD      RD13, RD26, RD14
  CMP/GT   #200, RD14
  BT       L008022BC
  BRA      L008022BE

L008022BE:
  MOV.L    @(R15, 68), RD25
  CMP/HI   #4, RD25
  BT       L008022BC
  BRA      L008022BD

L008022BD:
  MOV.L    @(R15, 48), RD7
  CMP/HI   #4, RD7
  BT       L008022BC
  BRA      L008022BB

L008022BC:
  MOV      L00C0047B, RQ4
  BSR      I_Error

L008022BB:
  MOV.L    @(R15, 56), RD4
  MOV      RD13, RD5
  MOV.L    @(R15, 64), RD6
  MOV.L    @(R15, 60), RD7
  BSR      V_MarkRect
  MOV      screens, RQ6
  MOV.L    @(R15, 68), RD5
  MOV.Q    @(RQ6, RD5), RQ4
  MOV.L    @(R15, 72), RD3
  MOV      RD3, RD14
  MOV      #320, RD2
  MUL.L    RD2, RD14, RD14
  EXTS.L   RD14
  LEA.W    @(RQ4, RD14), RQ23
  MOV.L    @(R15, 76), RD22
  EXTS.L   RD22
  LEA.W    @(RQ23, RD22), RQ21
  MOV.L    @(R15, 48), RD20
  EXTS.L   RD20
  MOV.Q    @(RQ6, RD20), RQ4
  MOV      RD13, RD14
  MUL.L    RD2, RD14, RD14
  EXTS.L   RD14
  LEA.W    @(RQ4, RD14), RQ23
  MOV.L    @(R15, 56), RD19
  LEA.W    @(RQ23, RD19), RQ18
  MOV.Q    RQ21, @(R15, 40)
  MOV.Q    RQ18, @(R15, 32)

L008022C6:
  MOV.L    @(R15, 60), RD17
  CMP/GT   #0, RD17
  BT       L008022C8
  MOV.L    @(R15, 64), RD25
  SHAD     RD25, #1, RD14
  EXTS.L   RD14, RQ26
  MOV.Q    RQ26, @R15
  MOV.Q    @(R15, 32), RQ4
  MOV.Q    @(R15, 40), RQ5
  MOV      RQ26, RQ6
  BSR      memcpy
  MOV.Q    @(R15, 40), RQ16
  ADD      #640, RQ16
  MOV.Q    @(R15, 32), RQ27
  ADD      #640, RQ27
  MOV.Q    RQ27, @(R15, 32)
  MOV.Q    RQ16, @(R15, 40)

L008022C7:
  MOV.L    @(R15, 60), RD16
  ADD      #-1, RD16
  MOV.L    RD16, @(R15, 60)
  BRA      L008022C6

L008022C8:

L00E004F7:
  ADD      #104, R15
  BRA      L00C00194

V_DrawPatch:
  PUSH     LR
  ADD      #-168, R15
  MOV.L    RD4, @(R15, 28)
  MOV.L    RD5, @(R15, 24)
  MOV.L    RD6, @(R15, 20)
  MOV.Q    RQ7, @(R15, 8)
  MOV.L    @(R15, 28), RD4
  MOV.L    @(R15, 24), RD5
  MOV.L    @(R15, 20), RD6
  MOV.Q    @(R15, 8), RQ7
  BSR      V_DrawPatchDirect

L00E004F8:
  ADD      #168, R15
  RET     

V_DrawPatchDirect:
  PUSH     LR
  BSR      L00C003ED
  ADD      #-128, R15
  MOV.L    RD4, @(R15, 28)
  MOV.L    RD5, @(R15, 24)
  MOV.L    RD6, @(R15, 20)
  MOV      RQ7, RQ12
  MOV.L    n_colormaps_alt, RD11
  CMP/GT   #1, RD11
  BT       L008022E8
  MOV.L    @(R15, 28), RD4
  MOV.L    @(R15, 24), RD5
  MOV.L    @(R15, 20), RD6
  MOV      RQ12, RQ7
  MOV.Q    colormaps, RQ20
  BSR      V_DrawPatchCmap
  BRA      L00E004F9

L008022E8:
  MOV      RQ12, RQ4
  BSR      W_GetNumForCache
  MOV      RD2, RD13
  MOV      RD13, RD4
  MOV      #0, RD5
  BSR      R_ColormapForLump
  MOV      RQ2, RQ14
  MOV      #0, RQ10
  CMPQ/EQ  RQ10, RQ14
  BT       L008022E9
  MOV.L    @(R15, 28), RD4
  MOV.L    @(R15, 24), RD5
  MOV.L    @(R15, 20), RD6
  MOV      RQ12, RQ7
  MOV      RQ14, RQ20
  BSR      V_DrawPatchCmap
  BRA      L008022EA

L008022E9:
  MOV.L    @(R15, 28), RD4
  MOV.L    @(R15, 24), RD5
  MOV.L    @(R15, 20), RD6
  MOV      RQ12, RQ7
  MOV.Q    colormaps, RQ20
  BSR      V_DrawPatchCmap

L008022EA:

L00E004F9:
  ADD      #128, R15
  BRA      L00C003EF

V_DrawPatchDirectName:
  PUSH     LR
  BSR      L00C001DF
  ADD      #-136, R15
  MOV.L    RD4, @(R15, 28)
  MOV.L    RD5, @(R15, 24)
  MOV.L    RD6, @(R15, 20)
  MOV.Q    RQ7, @(R15, 8)
  MOV.Q    @(R15, 8), RQ4
  BSR      W_GetNumForName
  MOV      RD2, RD14
  MOV      RD14, RD4
  MOV      #101, RD5
  BSR      W_CacheLumpNum
  MOV      RQ2, RQ12
  MOV      RD14, RD4
  MOV      #0, RD5
  BSR      R_ColormapForLump
  MOV      RQ2, RQ13
  MOV      #0, RQ11
  CMPQ/EQ  RQ11, RQ13
  BT       L008022EB
  MOV.L    @(R15, 28), RD4
  MOV.L    @(R15, 24), RD5
  MOV.L    @(R15, 20), RD6
  MOV      RQ12, RQ7
  MOV      RQ13, RQ20
  BSR      V_DrawPatchCmap
  BRA      L008022EC

L008022EB:
  MOV.L    @(R15, 28), RD4
  MOV.L    @(R15, 24), RD5
  MOV.L    @(R15, 20), RD6
  MOV      RQ12, RQ7
  BSR      V_DrawPatchDirect

L008022EC:

L00E004FA:
  ADD      #136, R15
  BRA      L00C001E0

V_MarkRect:
  PUSH     LR
  BSR      L00C0046F
  ADD      #-112, R15
  MOV      RD4, RD12
  MOV      RD5, RD13
  MOV.L    RD6, @(R15, 20)
  MOV.L    RD7, @(R15, 16)
  MOV      dirtybox, RQ4
  MOV      RD12, RD5
  MOV      RD13, RD6
  BSR      M_AddToBox
  MOV.L    @(R15, 16), RD11
  ADD      RD13, RD11, RD14
  SUB      RD14, #1, RD10
  MOV.L    @(R15, 20), RD9
  ADD      RD12, RD9, RD14
  SUB      RD14, #1, RD8
  MOV.L    RD10, @(R15, 8)
  MOV.L    RD8, @(R15, 4)
  MOV      dirtybox, RQ4
  MOV      RD8, RD5
  MOV      RD10, RD6
  BSR      M_AddToBox

L00E004FB:
  ADD      #112, R15
  BRA      L00C00470

I_GetTimeMs:
  PUSH     LR
  PUSH     R14
  ADD      #-144, R15
  BSR      TK_GetTimeMs
  MOV      RD2, RD14
  MOV      RD14, RD2

L00E004FC:
  ADD      #144, R15
  POP      RQ14
  RET     

vid_clamp255:
  PUSH     R14
  ADD      #-152, R15
  MOV      RD4, RD14
  CMP/PZ   RD14
  BT       L008008FD
  MOV      #0, RD2
  BRA      L00E004FD

L008008FD:
  CMP/GT   #255, RD14
  BT       L008008FE
  MOV      #255, RD2
  BRA      L00E004FD

L008008FE:
  MOV      RD14, RD2

L00E004FD:
  ADD      #152, R15
  POP      RQ14
  RTS     

IN_Init:
  ADD      #-144, R15

L00E004FE:
  ADD      #144, R15
  RTS     

Key_Event:
  PUSH     LR
  BSR      L00C003FF
  ADD      #-104, R15
  MOV      RD4, RD13
  MOV.L    RD5, @(R15, 24)
  CMP/GE   #131, RD13
  BT       L0080090C
  CMP/EQ   #127, RD13
  BT       L00800903
  CMP/EQ   #128, RD13
  BT       L00800904
  CMP/EQ   #129, RD13
  BT       L00800905
  CMP/EQ   #130, RD13
  BT       L00800906

L0080090C:
  CMP/EQ   #131, RD13
  BT       L00800907
  CMP/EQ   #132, RD13
  BT       L00800909
  CMP/EQ   #133, RD13
  BT       L00800908
  CMP/EQ   #134, RD13
  BT       L0080090A
  BRA      L0080090B

L00800903:
  MOV      #127, RD14
  BRA      L00800902

L00800904:
  MOV      #173, RD14
  BRA      L00800902

L00800905:
  MOV      #175, RD14
  BRA      L00800902

L00800906:
  MOV      #172, RD14
  BRA      L00800902

L00800907:
  MOV      #174, RD14
  BRA      L00800902

L00800908:
  MOV      #157, RD14
  BRA      L00800902

L00800909:
  MOV      #184, RD14
  BRA      L00800902

L0080090A:
  MOV      #182, RD14
  BRA      L00800902

L0080090B:
  MOV      RD13, RD14
  BRA      L00800902

L00800902:
  MOV.L    @(R15, 24), RD12
  CMP/EQ   RD12
  BT       L0080090D
  MOV      #0, RD11
  MOV      #8, RQ10
  ADD      R15, RQ10
  MOV.L    RD11, @RQ10
  MOV.L    RD14, @(RQ10, 4)
  MOV      RQ10, RQ9
  MOV.Q    RQ9, @R15
  MOV      RQ9, RQ4
  BSR      D_PostEvent
  MOV      RD11, RD2
  BRA      L00E004FF

L0080090D:
  MOV      #1, RD8
  MOV      #8, RQ31
  ADD      R15, RQ31
  MOV.L    RD8, @RQ31
  MOV.L    RD14, @(RQ31, 4)
  MOV      RQ31, RQ9
  MOV.Q    RQ9, @R15
  MOV      RQ9, RQ4
  BSR      D_PostEvent
  MOV      #0, RD2

L00E004FF:
  ADD      #104, R15
  BRA      L00C00404

D_PostEvent:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  ADD      #-144, R15
  MOV.Q    RQ4, @(R15, 24)
  MOV.Q    @(R15, 24), RQ14
  MOV      RQ14, RQ13
  MOV      events, RQ12
  MOV.L    eventhead, RD11
  SHAD     RD11, #4, RQ7
  ADD      RQ12, RQ7
  MOV.Q    @RQ13, RQ6
  MOV.Q    @(RQ13, 8), RQ5
  MOV.Q    RQ6, @RQ7
  MOV.Q    RQ5, @(RQ7, 8)
  ADD      #1, RD11
  AND      #63, RD11
  MOV.L    RD11, eventhead

L00E00500:
  ADD      #144, R15
  BRA      L00C001A1

IN_Commands:
  PUSH     LR
  BSR      L00C0046F
  ADD      #-96, R15

L0080090E:
  BSR      tk_kbhit
  MOV      RD2, RD13
  CMP/EQ   RD13
  BT       L0080090F
  BSR      tk_getch
  MOV      RD2, RD14
  CMP/EQ   #127, RD14
  BT       L00800911
  CMP/EQ   #255, RD14
  BT       L00800912
  CMP/EQ   #128, RD14
  BT       L00800913
  BRA      L00800914

L00800911:
  BSR      tk_getch
  MOV      RD2, RD14
  MOV      #1, RD12
  MOV.L    RD12, @(R15, 12)
  BRA      L00800910

L00800912:
  BSR      tk_getch
  MOV      RD2, RD14
  MOV      #0, RD11
  MOV.L    RD11, @(R15, 12)
  BRA      L00800910

L00800913:
  BSR      tk_getch
  MOV      RD2, RD14
  SHAD     RD14, #8, RD13
  BSR      tk_getch
  MOV      RD2, RD10
  OR       RD13, RD10, RD14
  MOV.L    RD10, @(R15, 4)
  TST      #32768, RD14
  BT       L00800915
  MOV      #0, RD13
  BRA      L00800916

L00800915:
  MOV      #1, RD13

L00800916:
  MOV      RD13, RD9
  MOV.L    RD9, @(R15, 12)
  BRA      L00800910

L00800914:
  TST      #128, RD14
  BT       L00800917
  MOV      #0, RD13
  BRA      L00800918

L00800917:
  MOV      #1, RD13

L00800918:
  MOV      RD13, RD8
  AND      #127, RD14
  MOV.L    RD8, @(R15, 12)
  BRA      L00800910

L00800910:
  CMP/EQ   #8, RD14
  BT       L0080091A
  CMP/EQ   #153, RD14
  BT       L0080091B
  CMP/EQ   #154, RD14
  BT       L0080091C
  CMP/EQ   #155, RD14
  BT       L0080091D
  CMP/EQ   #157, RD14
  BT       L0080091E
  CMP/EQ   #158, RD14
  BT       L0080091F
  CMP/EQ   #159, RD14
  BT       L00800920
  BRA      L00800921

L0080091A:
  MOV      #127, RD14
  BRA      L00800919

L0080091B:
  MOV      #255, RD14
  BRA      L00800919

L0080091C:
  MOV      #239, RD14
  BRA      L00800919

L0080091D:
  MOV      #240, RD14
  BRA      L00800919

L0080091E:
  MOV      #200, RD14
  BRA      L00800919

L0080091F:
  MOV      #201, RD14
  BRA      L00800919

L00800920:
  MOV      #202, RD14
  BRA      L00800919

L00800921:
  BRA      L00800919

L00800919:
  CMP/GE   #256, RD14
  BT       L00800922
  BRA      L0080090E

L00800922:
  MOV      RD14, RD4
  MOV.L    @(R15, 12), RD5
  BSR      Key_Event
  BRA      L0080090E

L0080090F:

L00E00501:
  ADD      #96, R15
  BRA      L00C00470

VID_ConGfx_EncBlock16Q:
  PUSH     LR
  BSR      L00C00190
  ADD      #-96, R15
  MOV.Q    RQ4, @(R15, 56)
  MOV.Q    RQ5, @(R15, 48)
  MOV.Q    RQ6, @(R15, 40)
  MOV.Q    @(R15, 56), RQ31
  MOV      RQ31, RQ10
  ADD      RQ31, #2560, RQ8
  MOV.Q    @(R15, 48), RQ30
  MOV      RQ30, RQ11
  ADD      RQ30, #2560, RQ9
  MOV.Q    @(R15, 40), RQ29
  ADD      RQ29, #28, RQ12
  ADD      RQ29, #12, RQ28
  MOV      RQ11, RQ27
  MOV      RQ10, RQ26
  MOV.Q    RQ28, @(R15, 24)
  MOV.Q    RQ27, @(R15, 16)
  MOV.Q    RQ26, @(R15, 8)
  MOV      RQ26, RQ4
  MOV      RQ27, RQ5
  MOV      RQ28, RQ6
  MOV      RQ12, RQ7
  BSR      TK_EncBlock16P
  MOV      RD2, RD13
  ADD      RQ29, #24, RQ12
  ADD      RQ29, #8, RQ28
  ADD      RQ11, #8, RQ27
  ADD      RQ10, #8, RQ26
  MOV.Q    RQ28, @(R15, 24)
  MOV.Q    RQ27, @(R15, 16)
  MOV.Q    RQ26, @(R15, 8)
  MOV      RQ26, RQ4
  MOV      RQ27, RQ5
  MOV      RQ28, RQ6
  MOV      RQ12, RQ7
  BSR      TK_EncBlock16P
  MOV      RD2, RD14
  SHAD     RD14, #1, RD31
  OR       RD31, RD13
  ADD      RQ29, #20, RQ12
  ADD      RQ29, #4, RQ28
  MOV      RQ9, RQ27
  MOV      RQ8, RQ26
  MOV.L    RD31, @R15
  MOV.Q    RQ28, @(R15, 24)
  MOV.Q    RQ27, @(R15, 16)
  MOV.Q    RQ26, @(R15, 8)
  MOV      RQ26, RQ4
  MOV      RQ27, RQ5
  MOV      RQ28, RQ6
  MOV      RQ12, RQ7
  BSR      TK_EncBlock16P
  MOV      RD2, RD14
  SHAD     RD14, #2, RD31
  OR       RD31, RD13
  ADD      RQ29, #16, RQ12
  MOV      RQ29, RQ28
  ADD      RQ9, #8, RQ27
  ADD      RQ8, #8, RQ26
  MOV.L    RD31, @R15
  MOV.Q    RQ28, @(R15, 24)
  MOV.Q    RQ27, @(R15, 16)
  MOV.Q    RQ26, @(R15, 8)
  MOV      RQ26, RQ4
  MOV      RQ27, RQ5
  MOV      RQ28, RQ6
  MOV      RQ12, RQ7
  BSR      TK_EncBlock16P
  MOV      RD2, RD14
  SHAD     RD14, #3, RD31
  OR       RD31, RD13
  MOV.L    RD31, @R15
  MOV      RD13, RD2

L00E00502:
  ADD      #96, R15
  BRA      L00C00191

VID_ConGfx_EncBlock16HP:
  PUSH     LR
  BSR      L00C00193
  ADD      #-232, R15
  MOV.Q    RQ4, @(R15, 200)
  MOV.Q    RQ5, @(R15, 192)
  MOV.Q    RQ6, @(R15, 184)
  MOV.Q    RQ7, @(R15, 176)
  MOV.Q    @(R15, 200), RQ31
  MOV      RQ31, RQ30
  MOV.Q    @RQ30, RQ29
  MOV      RQ29, RQ28
  ADD      RQ31, #640, RQ30
  MOV.Q    @RQ30, RQ29
  MOV      RQ29, RQ27
  ADD      RQ31, #1280, RQ30
  MOV.Q    @RQ30, RQ29
  MOV      RQ29, RQ26
  ADD      RQ31, #1920, RQ30
  MOV.Q    @RQ30, RQ29
  MOV      RQ29, RQ25
  MOV      #0x00010000, RD11
  MOV      #1, RD11
  MOV      #-1, RD12
  EXTU.W   RQ28, RD14
  SHLD.Q   RQ28, #-16, RQ29
  EXTU.W   RQ29, RD13
  MOV.Q    RQ29, @(R15, 40)
  MOV.Q    RQ28, @(R15, 136)
  MOV.Q    RQ27, @(R15, 128)
  MOV.Q    RQ26, @(R15, 120)
  MOV.Q    RQ25, @(R15, 112)
  CMP/GT   RD11, RD14
  CSELT    RD11, RD14, RD11
  CMP/GT   RD12, RD14
  CSELT    RD14, RD12, RD12
  MOV      RQ28, RQ29
  MOV      #32, RD7
  SHLR.Q   RQ29, RD7, RQ29
  EXTU.W   RQ29, RD14
  MOV.Q    RQ29, @(R15, 40)
  CMP/GT   RD11, RD13
  CSELT    RD11, RD13, RD11
  CMP/GT   RD12, RD13
  CSELT    RD13, RD12, RD12
  MOV      RQ28, RQ29
  MOV      #48, RD6
  SHLR.Q   RQ29, RD6, RQ29
  EXTU.W   RQ29, RD13
  MOV.Q    RQ29, @(R15, 40)
  CMP/GT   RD11, RD14
  CSELT    RD11, RD14, RD11
  CMP/GT   RD12, RD14
  CSELT    RD14, RD12, RD12
  CMP/GT   RD11, RD13
  CSELT    RD11, RD13, RD11
  CMP/GT   RD12, RD13
  CSELT    RD13, RD12, RD12
  EXTU.W   RQ27, RD14
  SHLD.Q   RQ27, #-16, RQ29
  EXTU.W   RQ29, RD13
  MOV.Q    RQ29, @(R15, 40)
  CMP/GT   RD11, RD14
  CSELT    RD11, RD14, RD11
  CMP/GT   RD12, RD14
  CSELT    RD14, RD12, RD12
  MOV      RQ27, RQ29
  SHLR.Q   RQ29, RD7, RQ29
  EXTU.W   RQ29, RD14
  MOV.Q    RQ29, @(R15, 40)
  CMP/GT   RD11, RD13
  CSELT    RD11, RD13, RD11
  CMP/GT   RD12, RD13
  CSELT    RD13, RD12, RD12
  MOV      RQ27, RQ29
  SHLR.Q   RQ29, RD6, RQ29
  EXTU.W   RQ29, RD13
  MOV.Q    RQ29, @(R15, 40)
  CMP/GT   RD11, RD14
  CSELT    RD11, RD14, RD11
  CMP/GT   RD12, RD14
  CSELT    RD14, RD12, RD12
  CMP/GT   RD11, RD13
  CSELT    RD11, RD13, RD11
  CMP/GT   RD12, RD13
  CSELT    RD13, RD12, RD12
  EXTU.W   RQ26, RD14
  SHLD.Q   RQ26, #-16, RQ29
  EXTU.W   RQ29, RD13
  MOV.Q    RQ29, @(R15, 40)
  CMP/GT   RD11, RD14
  CSELT    RD11, RD14, RD11
  CMP/GT   RD12, RD14
  CSELT    RD14, RD12, RD12
  MOV      RQ26, RQ29
  SHLR.Q   RQ29, RD7, RQ29
  EXTU.W   RQ29, RD14
  MOV.Q    RQ29, @(R15, 40)
  CMP/GT   RD11, RD13
  CSELT    RD11, RD13, RD11
  CMP/GT   RD12, RD13
  CSELT    RD13, RD12, RD12
  MOV      RQ26, RQ29
  SHLR.Q   RQ29, RD6, RQ29
  EXTU.W   RQ29, RD13
  MOV.Q    RQ29, @(R15, 40)
  CMP/GT   RD11, RD14
  CSELT    RD11, RD14, RD11
  CMP/GT   RD12, RD14
  CSELT    RD14, RD12, RD12
  CMP/GT   RD11, RD13
  CSELT    RD11, RD13, RD11
  CMP/GT   RD12, RD13
  CSELT    RD13, RD12, RD12
  EXTU.W   RQ25, RD14
  SHLD.Q   RQ25, #-16, RQ29
  EXTU.W   RQ29, RD13
  MOV.Q    RQ29, @(R15, 40)
  CMP/GT   RD11, RD14
  CSELT    RD11, RD14, RD11
  CMP/GT   RD12, RD14
  CSELT    RD14, RD12, RD12
  MOV      RQ25, RQ29
  SHLR.Q   RQ29, RD7, RQ29
  EXTU.W   RQ29, RD14
  MOV.Q    RQ29, @(R15, 40)
  CMP/GT   RD11, RD13
  CSELT    RD11, RD13, RD11
  CMP/GT   RD12, RD13
  CSELT    RD13, RD12, RD12
  MOV      RQ25, RQ29
  SHLR.Q   RQ29, RD6, RQ29
  EXTU.W   RQ29, RD13
  MOV.Q    RQ29, @(R15, 40)
  CMP/GT   RD11, RD14
  CSELT    RD11, RD14, RD11
  CMP/GT   RD12, RD14
  CSELT    RD14, RD12, RD12
  CMP/GT   RD11, RD13
  CSELT    RD11, RD13, RD11
  CMP/GT   RD12, RD13
  CSELT    RD13, RD12, RD12
  MOV      #0, RD5
  MOV      #0, RD4
  EXTU.W   RQ28, RD14
  MOV      RQ28, RQ29
  SHLR.Q   RQ29, RD7, RQ29
  EXTU.W   RQ29, RD13
  AND      RD14, #31, RD10
  ADD      RD10, RD5
  AND      RD14, #992, RD10
  ADD      RD10, RD4
  AND      RD13, #31, RD10
  ADD      RD10, RD5
  AND      RD13, #992, RD10
  ADD      RD10, RD4
  SHLD.Q   RQ27, #-16, RQ29
  EXTU.W   RQ29, RD14
  MOV      RQ27, RQ29
  SHLR.Q   RQ29, RD6, RQ29
  EXTU.W   RQ29, RD13
  AND      RD14, #31, RD10
  ADD      RD10, RD5
  AND      RD14, #992, RD10
  ADD      RD10, RD4
  AND      RD13, #31, RD10
  ADD      RD10, RD5
  AND      RD13, #992, RD10
  ADD      RD10, RD4
  EXTU.W   RQ26, RD14
  MOV      RQ26, RQ29
  SHLR.Q   RQ29, RD7, RQ29
  EXTU.W   RQ29, RD13
  AND      RD14, #31, RD10
  ADD      RD10, RD5
  AND      RD14, #992, RD10
  ADD      RD10, RD4
  AND      RD13, #31, RD10
  ADD      RD10, RD5
  AND      RD13, #992, RD10
  ADD      RD10, RD4
  SHLD.Q   RQ25, #-16, RQ29
  EXTU.W   RQ29, RD14
  MOV      RQ25, RQ29
  SHLR.Q   RQ29, RD6, RQ29
  EXTU.W   RQ29, RD13
  AND      RD14, #31, RD10
  ADD      RD10, RD5
  AND      RD14, #992, RD10
  ADD      RD10, RD4
  AND      RD13, #31, RD10
  ADD      RD10, RD5
  AND      RD13, #992, RD10
  ADD      RD10, RD4
  ADD      RD5, #4, RD10
  SHAD     RD10, #-3, RD5
  ADD      RD4, #64, RD10
  SHAD     RD10, #-3, RD4
  MOV.Q    RQ29, @(R15, 40)
  MOV.L    RD5, @(R15, 68)
  MOV.L    RD4, @(R15, 64)
  MOV      #0, RD3
  MOV      #31, RD2
  CMP/GT   RD5, RD3
  CSELT    RD3, RD5, RD5
  CMP/GT   RD5, RD2
  CSELT    RD5, RD2, RD5
  MOV.L    RD5, @(R15, 68)
  MOV      #992, RD23
  CMP/GT   RD4, RD3
  CSELT    RD3, RD4, RD4
  CMP/GT   RD4, RD23
  CSELT    RD4, RD23, RD4
  MOV      RD11, RD22
  MOV      RD12, RD21
  ADD      RD22, RD21, RD10
  SHAD     RD10, #-1, RD9
  ADD      RD22, RD9, RD10
  SHAD     RD10, #-1, RD11
  ADD      RD21, RD9, RD10
  SHAD     RD10, #-1, RD12
  AND      RD22, #31, RD10
  ADD      RD10, RD5, RD8
  ADD      RD8, #1, RD10
  SHAD     RD10, #-1, RD20
  AND      RD22, #992, RD10
  ADD      RD10, RD4, RD8
  ADD      RD8, #16, RD10
  SHAD     RD10, #-1, RD19
  MOV.L    RD4, @(R15, 64)
  MOV.L    RD22, @(R15, 172)
  MOV.L    RD21, @(R15, 168)
  MOV.L    RD20, @(R15, 108)
  MOV.L    RD19, @(R15, 104)
  CMP/GT   RD20, RD3
  CSELT    RD3, RD20, RD20
  CMP/GT   RD20, RD2
  CSELT    RD20, RD2, RD20
  MOV.L    RD20, @(R15, 108)
  CMP/GT   RD19, RD3
  CSELT    RD3, RD19, RD19
  CMP/GT   RD19, RD23
  CSELT    RD19, RD23, RD19
  AND      RD22, #64512, RD10
  AND      RD19, #992, RD8
  OR       RD10, RD8, RD18
  AND      RD20, #31, RD10
  OR       RD18, RD10, RD22
  AND      RD21, #31, RD10
  ADD      RD10, RD5, RD8
  ADD      RD8, #1, RD10
  SHAD     RD10, #-1, RD20
  AND      RD21, #992, RD10
  ADD      RD10, RD4, RD8
  ADD      RD8, #16, RD10
  SHAD     RD10, #-1, RD19
  MOV.L    RD22, @(R15, 172)
  MOV.L    RD20, @(R15, 108)
  MOV.L    RD19, @(R15, 104)
  MOV.L    RD18, @(R15, 28)
  CMP/GT   RD20, RD3
  CSELT    RD3, RD20, RD20
  CMP/GT   RD20, RD2
  CSELT    RD20, RD2, RD20
  MOV.L    RD20, @(R15, 108)
  CMP/GT   RD19, RD3
  CSELT    RD3, RD19, RD19
  CMP/GT   RD19, RD23
  CSELT    RD19, RD23, RD19
  MOV.L    @(R15, 168), RD31
  AND      RD31, #64512, RD10
  MOV.L    @(R15, 104), RD31
  AND      RD31, #992, RD8
  OR       RD10, RD8, RD25
  MOV.L    @(R15, 108), RD31
  AND      RD31, #31, RD10
  OR       RD25, RD10, RD31
  MOV      #0, RD30
  EXTU.W   RQ28, RD14
  SHLD.Q   RQ28, #-16, RQ29
  EXTU.W   RQ29, RD13
  SHLD     RD30, #2, RD10
  CMP/GT   RD9, RD14
  MOVT     RD8
  SHAD     RD8, #1, RD25
  OR       RD10, RD25, RD30
  CMP/GT   RD12, RD14
  MOVT     RD10
  CMP/GT   RD14, RD11
  MOVT     RD8
  OR       RD10, RD8, RD25
  OR       RD25, RD30
  MOV      RQ28, RQ29
  MOV      #32, RD28
  SHLR.Q   RQ29, RD28, RQ29
  EXTU.W   RQ29, RD14
  SHLD     RD30, #2, RD10
  CMP/GT   RD9, RD13
  MOVT     RD8
  SHAD     RD8, #1, RD25
  OR       RD10, RD25, RD30
  CMP/GT   RD12, RD13
  MOVT     RD10
  CMP/GT   RD13, RD11
  MOVT     RD8
  OR       RD10, RD8, RD25
  OR       RD25, RD30
  MOV.Q    @(R15, 136), RQ29
  MOV      #48, RD28
  SHLR.Q   RQ29, RD28, RQ29
  EXTU.W   RQ29, RD13
  SHLD     RD30, #2, RD10
  CMP/GT   RD9, RD14
  MOVT     RD8
  SHAD     RD8, #1, RD25
  OR       RD10, RD25, RD30
  CMP/GT   RD12, RD14
  MOVT     RD10
  CMP/GT   RD14, RD11
  MOVT     RD8
  OR       RD10, RD8, RD25
  OR       RD25, RD30
  SHLD     RD30, #2, RD10
  CMP/GT   RD9, RD13
  MOVT     RD8
  SHAD     RD8, #1, RD25
  OR       RD10, RD25, RD30
  CMP/GT   RD12, RD13
  MOVT     RD10
  CMP/GT   RD13, RD11
  MOVT     RD8
  OR       RD10, RD8, RD25
  OR       RD25, RD30
  EXTU.W   RQ27, RD14
  SHLD.Q   RQ27, #-16, RQ29
  EXTU.W   RQ29, RD13
  SHLD     RD30, #2, RD10
  CMP/GT   RD9, RD14
  MOVT     RD8
  SHAD     RD8, #1, RD25
  OR       RD10, RD25, RD30
  CMP/GT   RD12, RD14
  MOVT     RD10
  CMP/GT   RD14, RD11
  MOVT     RD8
  OR       RD10, RD8, RD25
  OR       RD25, RD30
  MOV      RQ27, RQ29
  MOV      #32, RD28
  SHLR.Q   RQ29, RD28, RQ29
  EXTU.W   RQ29, RD14
  SHLD     RD30, #2, RD10
  CMP/GT   RD9, RD13
  MOVT     RD8
  SHAD     RD8, #1, RD25
  OR       RD10, RD25, RD30
  CMP/GT   RD12, RD13
  MOVT     RD10
  CMP/GT   RD13, RD11
  MOVT     RD8
  OR       RD10, RD8, RD25
  OR       RD25, RD30
  MOV      RQ27, RQ29
  MOV      #48, RD28
  SHLR.Q   RQ29, RD28, RQ29
  EXTU.W   RQ29, RD13
  SHLD     RD30, #2, RD10
  CMP/GT   RD9, RD14
  MOVT     RD8
  SHAD     RD8, #1, RD25
  OR       RD10, RD25, RD30
  CMP/GT   RD12, RD14
  MOVT     RD10
  CMP/GT   RD14, RD11
  MOVT     RD8
  OR       RD10, RD8, RD25
  OR       RD25, RD30
  SHLD     RD30, #2, RD10
  CMP/GT   RD9, RD13
  MOVT     RD8
  SHAD     RD8, #1, RD25
  OR       RD10, RD25, RD30
  CMP/GT   RD12, RD13
  MOVT     RD10
  CMP/GT   RD13, RD11
  MOVT     RD8
  OR       RD10, RD8, RD25
  OR       RD25, RD30
  EXTU.W   RQ26, RD14
  SHLD.Q   RQ26, #-16, RQ29
  EXTU.W   RQ29, RD13
  SHLD     RD30, #2, RD10
  CMP/GT   RD9, RD14
  MOVT     RD8
  SHAD     RD8, #1, RD25
  OR       RD10, RD25, RD30
  CMP/GT   RD12, RD14
  MOVT     RD10
  CMP/GT   RD14, RD11
  MOVT     RD8
  OR       RD10, RD8, RD25
  OR       RD25, RD30
  MOV      RQ26, RQ29
  MOV      #32, RD28
  SHLR.Q   RQ29, RD28, RQ29
  EXTU.W   RQ29, RD14
  SHLD     RD30, #2, RD10
  CMP/GT   RD9, RD13
  MOVT     RD8
  SHAD     RD8, #1, RD25
  OR       RD10, RD25, RD30
  CMP/GT   RD12, RD13
  MOVT     RD10
  CMP/GT   RD13, RD11
  MOVT     RD8
  OR       RD10, RD8, RD25
  OR       RD25, RD30
  MOV      RQ26, RQ29
  MOV      #48, RD28
  SHLR.Q   RQ29, RD28, RQ29
  EXTU.W   RQ29, RD13
  SHLD     RD30, #2, RD10
  CMP/GT   RD9, RD14
  MOVT     RD8
  SHAD     RD8, #1, RD25
  OR       RD10, RD25, RD30
  CMP/GT   RD12, RD14
  MOVT     RD10
  CMP/GT   RD14, RD11
  MOVT     RD8
  OR       RD10, RD8, RD25
  OR       RD25, RD30
  SHLD     RD30, #2, RD10
  CMP/GT   RD9, RD13
  MOVT     RD8
  SHAD     RD8, #1, RD25
  OR       RD10, RD25, RD30
  CMP/GT   RD12, RD13
  MOVT     RD10
  CMP/GT   RD13, RD11
  MOVT     RD8
  OR       RD10, RD8, RD25
  OR       RD25, RD30
  MOV.L    @(R15, 112), RD14
  EXTU.W   RD14, RD14
  MOV.Q    @(R15, 112), RQ28
  SHLD.Q   RQ28, #-16, RQ29
  EXTU.W   RQ29, RD13
  SHLD     RD30, #2, RD10
  CMP/GT   RD9, RD14
  MOVT     RD8
  SHAD     RD8, #1, RD25
  OR       RD10, RD25, RD30
  CMP/GT   RD12, RD14
  MOVT     RD10
  CMP/GT   RD14, RD11
  MOVT     RD8
  OR       RD10, RD8, RD25
  OR       RD25, RD30
  MOV      RQ28, RQ29
  MOV      #32, RD28
  SHLR.Q   RQ29, RD28, RQ29
  EXTU.W   RQ29, RD14
  SHLD     RD30, #2, RD10
  CMP/GT   RD9, RD13
  MOVT     RD8
  SHAD     RD8, #1, RD25
  OR       RD10, RD25, RD30
  CMP/GT   RD12, RD13
  MOVT     RD10
  CMP/GT   RD13, RD11
  MOVT     RD8
  OR       RD10, RD8, RD25
  OR       RD25, RD30
  MOV.Q    @(R15, 112), RQ29
  MOV      #48, RD28
  SHLR.Q   RQ29, RD28, RQ29
  EXTU.W   RQ29, RD13
  SHLD     RD30, #2, RD10
  CMP/GT   RD9, RD14
  MOVT     RD8
  SHAD     RD8, #1, RD25
  OR       RD10, RD25, RD30
  CMP/GT   RD12, RD14
  MOVT     RD10
  CMP/GT   RD14, RD11
  MOVT     RD8
  OR       RD10, RD8, RD25
  OR       RD25, RD30
  SHLD     RD30, #2, RD10
  CMP/GT   RD9, RD13
  MOVT     RD8
  SHAD     RD8, #1, RD25
  OR       RD10, RD25, RD30
  CMP/GT   RD12, RD13
  MOVT     RD10
  CMP/GT   RD13, RD11
  MOVT     RD8
  OR       RD10, RD8, RD25
  OR       RD25, RD30
  SHLD     RD30, #-1, RD10
  NOT      RD10, RD8
  AND      RD8, #1431655765, RD10
  XOR      RD10, RD30
  MOV.Q    @(R15, 136), RQ26
  MOV      #31, RQ28
  AND      RQ26, RQ28, RQ29
  SHLD.Q   RQ27, #-16, RQ28
  MOV      #31, RQ27
  AND      RQ28, RQ27, RQ26
  ADD      RQ29, RQ26, RQ28
  MOV.Q    @(R15, 120), RQ27
  MOV.L    RD31, @(R15, 168)
  MOV      #31, RQ31
  AND      RQ27, RQ31, RQ29
  ADD      RQ28, RQ29, RQ26
  MOV.Q    @(R15, 112), RQ31
  SHLD.Q   RQ31, #-16, RQ29
  MOV      #31, RQ31
  AND      RQ29, RQ31, RQ28
  ADD      RQ26, RQ28, RQ29
  MOV      RQ29, RQ26
  MOV      RQ26, RQ28
  MOV      #2, RD31
  SHLR.Q   RQ28, RD31, RQ28
  EXTS.L   RQ28, RD31
  MOV.Q    @(R15, 136), RQ29
  MOV      #32, RD27
  SHLR.Q   RQ29, RD27, RQ29
  MOV      #31, RQ27
  AND      RQ29, RQ27, RQ28
  MOV.Q    @(R15, 128), RQ29
  MOV      #48, RD27
  SHLR.Q   RQ29, RD27, RQ29
  MOV      #31, RQ27
  AND      RQ29, RQ27, RQ26
  ADD      RQ28, RQ26, RQ29
  MOV.Q    @(R15, 120), RQ28
  MOV      #32, RD27
  SHLR.Q   RQ28, RD27, RQ28
  MOV      #31, RQ27
  AND      RQ28, RQ27, RQ26
  ADD      RQ29, RQ26, RQ28
  MOV.Q    @(R15, 112), RQ29
  MOV      #48, RD27
  SHLR.Q   RQ29, RD27, RQ29
  MOV      #31, RQ27
  AND      RQ29, RQ27, RQ26
  ADD      RQ28, RQ26, RQ29
  MOV      RQ29, RQ26
  MOV      RQ26, RQ28
  MOV      #2, RD27
  SHLR.Q   RQ28, RD27, RQ28
  EXTS.L   RQ28, RD27
  MOV.L    RD25, @(R15, 28)
  MOV.Q    @(R15, 136), RQ25
  MOV.L    RD30, @(R15, 156)
  MOV      #31, RQ30
  AND      RQ25, RQ30, RQ29
  MOV      RQ25, RQ28
  MOV      #32, RD30
  SHLR.Q   RQ28, RD30, RQ28
  MOV      #31, RQ30
  AND      RQ28, RQ30, RQ26
  ADD      RQ29, RQ26, RQ28
  MOV.Q    @(R15, 128), RQ30
  SHLD.Q   RQ30, #-16, RQ29
  MOV      #31, RQ30
  AND      RQ29, RQ30, RQ26
  ADD      RQ28, RQ26, RQ29
  MOV.Q    @(R15, 128), RQ28
  MOV      #48, RD30
  SHLR.Q   RQ28, RD30, RQ28
  MOV      #31, RQ30
  AND      RQ28, RQ30, RQ26
  ADD      RQ29, RQ26, RQ28
  MOV      RQ28, RQ26
  MOV      RQ26, RQ29
  MOV      #2, RD30
  SHLR.Q   RQ29, RD30, RQ29
  EXTS.L   RQ29, RD30
  MOV.Q    @(R15, 120), RQ25
  MOV.L    RD31, @(R15, 100)
  MOV      #31, RQ31
  AND      RQ25, RQ31, RQ29
  MOV      RQ25, RQ28
  MOV      #32, RD31
  SHLR.Q   RQ28, RD31, RQ28
  MOV      #31, RQ31
  AND      RQ28, RQ31, RQ26
  ADD      RQ29, RQ26, RQ28
  MOV.Q    @(R15, 112), RQ31
  SHLD.Q   RQ31, #-16, RQ29
  MOV      #31, RQ31
  AND      RQ29, RQ31, RQ26
  ADD      RQ28, RQ26, RQ29
  MOV.Q    @(R15, 112), RQ28
  MOV      #48, RD31
  SHLR.Q   RQ28, RD31, RQ28
  MOV      #31, RQ31
  AND      RQ28, RQ31, RQ26
  ADD      RQ29, RQ26, RQ28
  MOV      RQ28, RQ26
  MOV      RQ26, RQ29
  MOV      #2, RD31
  SHLR.Q   RQ29, RD31, RQ29
  EXTS.L   RQ29, RD31
  MOV.Q    @(R15, 136), RQ25
  MOV.L    RD27, @(R15, 92)
  MOV      #992, RQ27
  AND      RQ25, RQ27, RQ29
  MOV.Q    @(R15, 128), RQ27
  SHLD.Q   RQ27, #-16, RQ28
  MOV      #992, RQ27
  AND      RQ28, RQ27, RQ26
  ADD      RQ29, RQ26, RQ28
  MOV.Q    @(R15, 120), RQ27
  MOV      #992, RQ25
  AND      RQ27, RQ25, RQ29
  ADD      RQ28, RQ29, RQ26
  MOV.Q    @(R15, 112), RQ27
  SHLD.Q   RQ27, #-16, RQ29
  AND      RQ29, RQ25, RQ28
  ADD      RQ26, RQ28, RQ29
  MOV      RQ29, RQ26
  MOV      RQ26, RQ28
  MOV      #2, RD25
  SHLR.Q   RQ28, RD25, RQ28
  EXTS.L   RQ28, RD27
  MOV.Q    @(R15, 136), RQ29
  MOV      #32, RD25
  SHLR.Q   RQ29, RD25, RQ29
  MOV      #992, RQ25
  AND      RQ29, RQ25, RQ28
  MOV.Q    @(R15, 128), RQ29
  MOV      #48, RD25
  SHLR.Q   RQ29, RD25, RQ29
  MOV      #992, RQ25
  AND      RQ29, RQ25, RQ26
  ADD      RQ28, RQ26, RQ29
  MOV.Q    @(R15, 120), RQ28
  MOV      #32, RD25
  SHLR.Q   RQ28, RD25, RQ28
  MOV      #992, RQ25
  AND      RQ28, RQ25, RQ26
  ADD      RQ29, RQ26, RQ28
  MOV.Q    @(R15, 112), RQ29
  MOV      #48, RD25
  SHLR.Q   RQ29, RD25, RQ29
  MOV      #992, RQ25
  AND      RQ29, RQ25, RQ26
  ADD      RQ28, RQ26, RQ29
  MOV      RQ29, RQ26
  MOV      RQ26, RQ28
  MOV      #2, RD25
  SHLR.Q   RQ28, RD25, RQ28
  EXTS.L   RQ28, RD25
  MOV.L    RD30, @(R15, 84)
  MOV.Q    @(R15, 136), RQ30
  MOV.L    RD31, @(R15, 76)
  MOV      #992, RQ31
  AND      RQ30, RQ31, RQ29
  MOV      RQ30, RQ28
  MOV      #32, RD31
  SHLR.Q   RQ28, RD31, RQ28
  MOV      #992, RQ31
  AND      RQ28, RQ31, RQ26
  ADD      RQ29, RQ26, RQ28
  MOV.Q    @(R15, 128), RQ31
  SHLD.Q   RQ31, #-16, RQ29
  MOV      #992, RQ31
  AND      RQ29, RQ31, RQ26
  ADD      RQ28, RQ26, RQ29
  MOV.Q    @(R15, 128), RQ28
  MOV      #48, RD31
  SHLR.Q   RQ28, RD31, RQ28
  MOV      #992, RQ31
  AND      RQ28, RQ31, RQ26
  ADD      RQ29, RQ26, RQ28
  MOV      RQ28, RQ26
  MOV      RQ26, RQ29
  MOV      #2, RD31
  SHLR.Q   RQ29, RD31, RQ29
  EXTS.L   RQ29, RD31
  MOV.Q    @(R15, 120), RQ30
  MOV.L    RD27, @(R15, 96)
  MOV      #992, RQ27
  AND      RQ30, RQ27, RQ29
  MOV      RQ30, RQ28
  MOV      #32, RD27
  SHLR.Q   RQ28, RD27, RQ28
  MOV      #992, RQ30
  AND      RQ28, RQ30, RQ26
  ADD      RQ29, RQ26, RQ28
  MOV.Q    @(R15, 112), RQ27
  SHLD.Q   RQ27, #-16, RQ29
  AND      RQ29, RQ30, RQ26
  ADD      RQ28, RQ26, RQ29
  MOV      RQ27, RQ28
  MOV      #48, RD30
  SHLR.Q   RQ28, RD30, RQ28
  MOV      #992, RQ30
  AND      RQ28, RQ30, RQ26
  ADD      RQ29, RQ26, RQ28
  MOV      RQ28, RQ26
  MOV      RQ26, RQ29
  MOV      #2, RD30
  SHLR.Q   RQ29, RD30, RQ29
  EXTS.L   RQ29, RD30
  MOV.L    @(R15, 96), RD27
  AND      #960, RD27
  AND      #960, RD25
  AND      #960, RD31
  AND      #960, RD30
  MOV.L    RD25, @(R15, 88)
  MOV.L    @(R15, 100), RD25
  MOV.L    @(R15, 92), RD26
  SUB      RD25, RD26, RD10
  MOV.L    RD31, @(R15, 80)
  MOV.L    RD30, @(R15, 72)
  MOV.Q    RQ29, @(R15, 40)
  MOV.L    RD27, @(R15, 96)
  MOV.L    RD19, @(R15, 104)
  MOV      RD10, RD4
  BSR      abs
  MOV      RD2, RD8
  MOV.L    @(R15, 84), RD31
  MOV.L    @(R15, 76), RD27
  SUB      RD31, RD27, RD10
  MOV      RD10, RD4
  BSR      abs
  MOV      RD2, RD28
  ADD      RD8, RD28, RD10
  MOV.L    @(R15, 96), RD31
  MOV.L    @(R15, 88), RD30
  SUB      RD31, RD30, RD8
  MOV.L    RD28, @(R15, 28)
  MOV      RD8, RD4
  BSR      abs
  MOV      RD2, RD28
  MOV.L    @(R15, 80), RD31
  MOV.L    @(R15, 72), RD30
  SUB      RD31, RD30, RD8
  MOV.L    RD28, @(R15, 28)
  MOV      RD8, RD4
  BSR      abs
  MOV      RD2, RD31
  ADD      RD28, RD31, RD8
  SHAD     RD8, #-5, RD28
  ADD      RD10, RD28, RD25
  MOV.L    @(R15, 168), RD26
  MOV.L    @(R15, 172), RD27
  SUB      RD26, RD27, RD10
  SHAD     RD10, #-8, RD8
  MOV.L    RD31, @(R15, 4)
  MOV.L    RD28, @(R15, 28)
  MOV.L    RD25, @(R15, 68)
  CMP/GT   RD8, RD25
  BT       L00800924
  MOV.L    @(R15, 172), RD26
  MOV      RD26, RD25
  MOV.L    @(R15, 168), RD27
  MOV      RD27, RD26
  MOV      RD25, RD27
  MOV.L    @(R15, 156), RD28
  NOT      RD28, RD28
  MOV.L    @(R15, 164), RD29
  AND      #64512, RD29
  MOV.L    @(R15, 160), RD30
  AND      #64512, RD30
  MOV.L    @(R15, 84), RD31
  MOV.L    RD25, @(R15, 60)
  MOV.L    @(R15, 76), RD25
  SUB      RD31, RD25, RD10
  MOV.L    RD30, @(R15, 160)
  MOV.L    RD29, @(R15, 164)
  MOV.L    RD28, @(R15, 156)
  MOV.L    RD27, @(R15, 168)
  MOV.L    RD26, @(R15, 172)
  MOV      RD10, RD4
  BSR      abs
  MOV      RD2, RD8
  MOV.L    @(R15, 100), RD29
  MOV.L    @(R15, 92), RD30
  SUB      RD29, RD30, RD10
  MOV      RD10, RD4
  BSR      abs
  MOV      RD2, RD26
  MOV.L    RD26, @(R15, 28)
  CMP/GT   RD26, RD8
  BT       L00800925
  MOV.L    @(R15, 84), RD17
  MOV.L    @(R15, 164), RD16
  OR       RD17, RD16
  MOV.L    @(R15, 76), RD18
  OR       RD18, #32, RD10
  MOV.L    @(R15, 160), RD19
  OR       RD10, RD19
  MOV.L    RD19, @(R15, 160)
  MOV.L    RD16, @(R15, 164)
  BRA      L00800926

L00800925:
  MOV.L    @(R15, 100), RD16
  MOV.L    @(R15, 164), RD17
  OR       RD16, RD17
  MOV.L    @(R15, 92), RD18
  MOV.L    @(R15, 160), RD19
  OR       RD18, RD19
  MOV.L    RD19, @(R15, 160)
  MOV.L    RD17, @(R15, 164)

L00800926:
  MOV.L    @(R15, 80), RD25
  MOV.L    @(R15, 72), RD26
  SUB      RD25, RD26, RD10
  MOV      RD10, RD4
  BSR      abs
  MOV      RD2, RD8
  MOV.L    @(R15, 96), RD27
  MOV.L    @(R15, 88), RD28
  SUB      RD27, RD28, RD10
  MOV      RD10, RD4
  BSR      abs
  MOV      RD2, RD29
  MOV.L    RD29, @(R15, 28)
  CMP/GT   RD29, RD8
  BT       L00800927
  MOV.L    @(R15, 80), RD16
  OR       RD16, #32, RD10
  MOV.L    @(R15, 164), RD17
  OR       RD10, RD17
  MOV.L    @(R15, 72), RD18
  MOV.L    @(R15, 160), RD19
  OR       RD18, RD19
  MOV.L    RD19, @(R15, 160)
  MOV.L    RD17, @(R15, 164)
  BRA      L00800928

L00800927:
  MOV.L    @(R15, 96), RD16
  MOV.L    @(R15, 164), RD17
  OR       RD16, RD17
  MOV.L    @(R15, 88), RD18
  MOV.L    @(R15, 160), RD19
  OR       RD18, RD19
  MOV.L    RD19, @(R15, 160)
  MOV.L    RD17, @(R15, 164)

L00800928:

L00800924:
  MOV.L    @(R15, 172), RD16
  SHAD     RD16, #-1, RD17
  MOV.L    @(R15, 168), RD18
  SHAD     RD18, #-1, RD19
  SHAD     RD17, #15, RD10
  OR       RD10, #-1073741824, RD8
  OR       RD8, RD19, RD20
  MOV.L    @(R15, 156), RD22
  MOV      RD22, RD21
  MOV.Q    @(R15, 184), RQ23
  MOVU.L   RD20, @RQ23
  MOV.Q    @(R15, 176), RQ2
  MOVU.L   RD21, @RQ2
  MOV.L    RD21, @(R15, 148)
  MOV.L    RD20, @(R15, 152)
  MOV.L    RD19, @(R15, 164)
  MOV.L    RD17, @(R15, 160)
  MOV      #1, RD2

L00E00503:
  ADD      #232, R15
  BRA      L00C00194

VID_ConGfx_EncBlock16HQ:
  PUSH     LR
  BSR      L00C002C5
  ADD      #-112, R15
  MOV      RQ4, RQ9
  MOV      RQ5, RQ8
  MOV.Q    RQ6, @(R15, 40)
  MOV      RQ9, RQ12
  ADD      RQ9, #2560, RQ10
  MOV      RQ8, RQ13
  ADD      RQ8, #2560, RQ11
  MOV.Q    @(R15, 40), RQ31
  ADD      RQ31, #28, RQ14
  ADD      RQ31, #12, RQ30
  MOV      RQ13, RQ29
  MOV      RQ12, RQ28
  MOV.Q    RQ30, @(R15, 24)
  MOV.Q    RQ29, @(R15, 16)
  MOV.Q    RQ28, @(R15, 8)
  MOV      RQ28, RQ4
  MOV      RQ29, RQ5
  MOV      RQ30, RQ6
  MOV      RQ14, RQ7
  BSR      VID_ConGfx_EncBlock16HP
  ADD      RQ31, #24, RQ14
  ADD      RQ31, #8, RQ30
  ADD      RQ13, #8, RQ29
  ADD      RQ12, #8, RQ28
  MOV.Q    RQ30, @(R15, 24)
  MOV.Q    RQ29, @(R15, 16)
  MOV.Q    RQ28, @(R15, 8)
  MOV      RQ28, RQ4
  MOV      RQ29, RQ5
  MOV      RQ30, RQ6
  MOV      RQ14, RQ7
  BSR      VID_ConGfx_EncBlock16HP
  ADD      RQ31, #20, RQ14
  ADD      RQ31, #4, RQ30
  MOV      RQ11, RQ29
  MOV      RQ10, RQ28
  MOV.Q    RQ30, @(R15, 24)
  MOV.Q    RQ29, @(R15, 16)
  MOV.Q    RQ28, @(R15, 8)
  MOV      RQ28, RQ4
  MOV      RQ29, RQ5
  MOV      RQ30, RQ6
  MOV      RQ14, RQ7
  BSR      VID_ConGfx_EncBlock16HP
  ADD      RQ31, #16, RQ14
  MOV      RQ31, RQ30
  ADD      RQ11, #8, RQ29
  ADD      RQ10, #8, RQ28
  MOV.Q    RQ30, @(R15, 24)
  MOV.Q    RQ29, @(R15, 16)
  MOV.Q    RQ28, @(R15, 8)
  MOV      RQ28, RQ4
  MOV      RQ29, RQ5
  MOV      RQ30, RQ6
  MOV      RQ14, RQ7
  BSR      VID_ConGfx_EncBlock16HP
  MOV      #1, RD2

L00E00504:
  ADD      #112, R15
  BRA      L00C002C6

M_ClearBox:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  ADD      #-136, R15
  MOV      RQ4, RQ14
  MOV      #0x80000000, RD13
  MOV      #-32768, RD13
  MOV.L    RD13, @(RQ14, 12)
  MOV.L    RD13, @RQ14
  MOV      #0x7FFFFFFF, RD12
  MOV      #32767, RD12
  MOV.L    RD12, @(RQ14, 8)
  MOV.L    RD12, @(RQ14, 4)

L00E00505:
  ADD      #136, R15
  BRA      L00C003F2

L00C00487:
  PUSH     R31
  PUSH     R30
  PUSH     R29
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  PUSH     R9
  PUSH     R8
  RTS     

R_ClearCZBuf:
  PUSH     LR
  BSR      L00C00487
  ADD      #-72, R15
  MOV      #0, RD14

L00801E06:
  CMP/GE   #512, RD14
  BT       L00801E08
  MOV      dc_czbuf_sc, RQ13
  MOV      #0x7FFFFFFF, RD12
  MOV      #32767, RD12
  EXTS.L   RD14
  MOV.L    RD12, @(RQ13, RD14)

L00801E07:
  ADD      #1, RD14
  BRA      L00801E06

L00801E08:
  MOVU.B   r_usezbuff, RD11
  CMP/EQ   RD11
  BT       L00801E09
  MOV      #0, RD14

L00801E0A:
  MOV      #0x0000FA00, RD10
  CMP/GE   RD10, RD14
  BT       L00801E0C
  MOV.Q    screens_zbuf, RQ9
  MOV      #0x0000FFFF, RD8
  EXTS.L   RD14
  MOVU.W   RD8, @(RQ9, RD14)

L00801E0B:
  ADD      #1, RD14
  BRA      L00801E0A

L00801E0C:

L00801E09:
  MOV.L    r_nseenpolyobj, RD31
  CMP/EQ   #0, RD31
  BT       L00801E0D
  MOV      #0, RD30
  MOV      RD30, RD29
  MOV.B    RD29, r_usezbuff

L00801E0D:
  MOV      #0, RD29
  MOV.L    RD29, r_nseenpolyobj

L00E00506:
  ADD      #72, R15

L00C0048B:
  POP      RQ8
  POP      RQ9
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  POP      RQ29
  POP      RQ30
  POP      RQ31
  RET     

Z_Init:
  PUSH     LR
  BSR      L00C002C0
  ADD      #-160, R15
  MOV      #44, RQ14
  ADD      R15, RQ14
  MOV      RQ14, RQ4
  BSR      I_ZoneBase
  MOV      RQ2, RQ13
  MOV      RQ13, RQ12
  MOV.L    @(R15, 44), RD11
  MOV.L    RD11, @RQ12
  MOV      RQ12, RQ14
  ADD      RQ14, #56, RQ13
  MOV      RQ13, RQ10
  ADD      RQ12, #8, RQ9
  MOV.Q    RQ13, @(RQ9, 32)
  ADD      RQ12, #8, RQ9
  MOV.Q    RQ13, @(RQ9, 24)
  MOV      RQ12, RQ14
  ADD      RQ12, #8, RQ9
  MOV.Q    RQ14, @(RQ9, 8)
  ADD      RQ12, #8, RQ9
  MOV      #1, RD8
  MOV.L    RD8, @(RQ9, 16)
  MOV.Q    RQ10, @(RQ12, 48)
  ADD      RQ12, #8, RQ14
  MOV.Q    RQ14, @(RQ10, 24)
  MOV.Q    RQ14, @(RQ10, 32)
  MOV      #0, RQ14
  MOV.Q    RQ14, @(RQ10, 8)
  MOV.L    @RQ12, RD31
  SUB      RD31, #56, RD30
  MOV.L    RD30, @RQ10
  MOV.Q    RQ12, mainzone
  MOV.Q    RQ10, @(R15, 48)

L00E00507:
  ADD      #160, R15
  BRA      L00C002C3

Z_Malloc:
  PUSH     LR
  BSR      L00C00193
  ADD      #-72, R15
  MOV      RD4, RD11
  MOV.L    RD5, @(R15, 40)
  MOV.Q    RQ6, @(R15, 32)
  SHAD     #1, RD11
  ADD      RD11, #3, RD9
  AND      RD9, #-4, RD11
  ADD      #40, RD11
  MOV.Q    mainzone, RQ31
  MOV.Q    @(RQ31, 48), RQ14
  MOV.Q    @(RQ14, 32), RQ13
  MOV.Q    @(RQ13, 8), RQ30
  MOV.Q    RQ30, @R15
  MOV      #0, RQ29
  CMPQ/EQ  RQ29, RQ30
  BT       L0080258E
  MOV.Q    @(RQ14, 32), RQ14

L0080258E:
  MOV      RQ14, RQ12
  MOV.Q    @(RQ14, 32), RQ28
  MOV.Q    RQ28, @(R15, 24)

L0080258F:
  MOV.Q    @(R15, 24), RQ27
  CMPQ/EQ  RQ27, RQ12
  BT       L00802591
  MOV      L00C0048F, RQ4
  MOV      RD11, RD5
  BSR      I_Error

L00802591:
  MOV.Q    @(RQ12, 8), RQ13
  MOV      #0, RQ26
  CMPQ/EQ  RQ26, RQ13
  BT       L00802592
  MOV.L    @(RQ12, 16), RD9
  CMP/GE   #100, RD9
  BT       L00802593
  MOV.Q    @(RQ12, 24), RQ13
  MOV      RQ13, RQ12
  MOV      RQ13, RQ14
  BRA      L00802594

L00802593:
  MOV.Q    @(RQ14, 32), RQ14
  ADD      RQ12, #40, RQ13
  MOV      RQ13, RQ4
  BSR      Z_Free
  MOV.Q    @(RQ14, 24), RQ14
  MOV.Q    @(RQ14, 24), RQ12

L00802594:
  BRA      L00802595

L00802592:
  MOV.Q    @(RQ12, 24), RQ12

L00802595:
  MOV.Q    @(RQ14, 8), RQ13
  MOV      #0, RQ25
  CMPQ/EQ  RQ25, RQ13
  BT       L0080258F
  MOV.L    @RQ14, RD9
  CMP/GT   RD9, RD11
  BT       L0080258F
  BRA      L00802596

L00802596:

L00802590:
  MOV.L    @RQ14, RD9
  SUB      RD9, RD11, RD8
  CMP/GT   #64, RD8
  BT       L00802598
  EXTS.L   RD11
  LEA.B    @(RQ14, RD11), RQ13
  MOV      RQ13, RQ10
  MOV.L    RD8, @RQ10
  MOV      #0, RQ13
  MOV.Q    RQ13, @(RQ10, 8)
  MOV      #0, RD7
  MOV.L    RD7, @(RQ10, 16)
  MOV.Q    RQ14, @(RQ10, 32)
  MOV.Q    @(RQ14, 24), RQ13
  MOV.Q    RQ13, @(RQ10, 24)
  MOV.Q    @(RQ10, 24), RQ13
  MOV.Q    RQ10, @(RQ13, 32)
  MOV.Q    RQ10, @(RQ14, 24)
  MOV.L    RD11, @RQ14

L00802598:
  MOV.Q    @(R15, 32), RQ6
  MOV      #0, RQ5
  CMPQ/EQ  RQ5, RQ6
  BT       L00802599
  MOV.Q    @(R15, 32), RQ4
  MOV.Q    RQ4, @(RQ14, 8)
  ADD      RQ14, #40, RQ13
  MOV.Q    RQ13, @RQ4
  BRA      L0080259A

L00802599:
  MOV.L    @(R15, 40), RD3
  CMP/GE   #100, RD3
  BT       L0080259B
  MOV      L00C00491, RQ4
  BSR      I_Error

L0080259B:
  MOV      #2, RQ13
  MOV.Q    RQ13, @(RQ14, 8)

L0080259A:
  MOV.L    @(R15, 40), RD2
  MOV.L    RD2, @(RQ14, 16)
  MOV.Q    @(RQ14, 24), RQ13
  MOV.Q    mainzone, RQ23
  MOV.Q    RQ13, @(RQ23, 48)
  MOV      #0x001D4A11, RD22
  MOV      #29, RD22
  MOV.L    RD22, @(RQ14, 20)
  ADD      RQ14, #40, RQ13
  MOV      RQ13, RQ2

L00E00508:
  ADD      #72, R15
  BRA      L00C00194

Z_Free:
  PUSH     LR
  BSR      L00C00193
  ADD      #-72, R15
  MOV      RQ4, RQ9
  MOV      #0, RQ8
  CMPQ/EQ  RQ8, RQ9
  BT       L00802587
  BRA      L00E00509

L00802587:
  ADD      RQ9, #-40, RQ14
  MOV      RQ14, RQ13
  MOV.L    @(RQ13, 20), RD11
  CMP/EQ   #1919505, RD11
  BT       L00802588
  MOV      L00C00492, RQ4
  BSR      I_Error

L00802588:
  MOV.Q    @(RQ13, 8), RQ14
  MOV      #256, RQ10
  CMPQ/GT  RQ10, RQ14
  BT       L00802589
  MOV.Q    @(RQ13, 8), RQ14
  MOV      #0, RQ10
  MOV.Q    RQ10, @RQ14

L00802589:
  MOV      #0, RQ14
  MOV.Q    RQ14, @(RQ13, 8)
  MOV      #0, RD31
  MOV.L    RD31, @(RQ13, 16)
  MOV.L    RD31, @(RQ13, 20)
  MOV.Q    @(RQ13, 32), RQ12
  MOV.Q    @(RQ12, 8), RQ14
  MOV      #0, RQ30
  CMPQ/EQ  RQ30, RQ14
  BT       L0080258A
  MOV.L    @RQ12, RD11
  MOV.L    @RQ13, RD29
  ADD      RD11, RD29, RD28
  MOV.L    RD28, @RQ12
  MOV.Q    @(RQ13, 24), RQ14
  MOV.Q    RQ14, @(RQ12, 24)
  MOV.Q    @(RQ12, 24), RQ14
  MOV.Q    RQ12, @(RQ14, 32)
  MOV.Q    mainzone, RQ27
  MOV.Q    @(RQ27, 48), RQ14
  MOV.L    RD29, @(R15, 12)
  MOV.L    RD28, @(R15, 8)
  CMPQ/EQ  RQ14, RQ13
  BT       L0080258B
  MOV.Q    mainzone, RQ26
  MOV.Q    RQ12, @(RQ26, 48)

L0080258B:
  MOV      RQ12, RQ13

L0080258A:
  MOV.Q    @(RQ13, 24), RQ12
  MOV.Q    @(RQ12, 8), RQ14
  MOV      #0, RQ25
  CMPQ/EQ  RQ25, RQ14
  BT       L0080258C
  MOV.L    @RQ13, RD11
  MOV.L    @RQ12, RD7
  ADD      RD11, RD7, RD6
  MOV.L    RD6, @RQ13
  MOV.Q    @(RQ12, 24), RQ14
  MOV.Q    RQ14, @(RQ13, 24)
  MOV.Q    @(RQ13, 24), RQ14
  MOV.Q    RQ13, @(RQ14, 32)
  MOV.Q    mainzone, RQ5
  MOV.Q    @(RQ5, 48), RQ14
  MOV.L    RD7, @(R15, 12)
  MOV.L    RD6, @(R15, 8)
  CMPQ/EQ  RQ14, RQ12
  BT       L0080258D
  MOV.Q    mainzone, RQ4
  MOV.Q    RQ13, @(RQ4, 48)

L0080258D:

L0080258C:

L00E00509:
  ADD      #72, R15
  BRA      L00C00194

Z_FreeTags:
  PUSH     LR
  BSR      L00C00472
  ADD      #-120, R15
  MOV.L    RD4, @(R15, 36)
  MOV.L    RD5, @(R15, 32)
  MOV.Q    mainzone, RQ12
  ADD      RQ12, #8, RQ11
  MOV.Q    @(RQ11, 24), RQ14

L0080259C:
  MOV.Q    mainzone, RQ10
  ADD      RQ10, #8, RQ13
  CMPQ/EQ  RQ13, RQ14
  BT       L0080259E
  MOV.Q    @(RQ14, 24), RQ9
  MOV.Q    @(RQ14, 8), RQ13
  MOV.Q    RQ9, @(R15, 24)
  MOV      #0, RQ8
  CMPQ/EQ  RQ8, RQ13
  BT       L0080259F
  BRA      L0080259D

L0080259F:
  MOV.L    @(RQ14, 16), RD31
  MOV.L    RD31, @(R15, 4)
  MOV.L    @(R15, 36), RD30
  CMP/GT   RD31, RD30
  BT       L008025A0
  MOV.L    @(RQ14, 16), RD29
  MOV.L    RD29, @(R15, 4)
  MOV.L    @(R15, 32), RD28
  CMP/GT   RD28, RD29
  BT       L008025A0
  BRA      L008025A1

L008025A1:
  ADD      RQ14, #40, RQ13
  MOV      RQ13, RQ4
  BSR      Z_Free

L008025A0:

L0080259D:
  MOV.Q    @(R15, 24), RQ27
  MOV      RQ27, RQ14
  BRA      L0080259C

L0080259E:

L00E0050A:
  ADD      #120, R15
  BRA      L00C00476

Z_CheckHeap:
  PUSH     LR
  BSR      L00C002C0
  ADD      #-144, R15
  MOV.Q    mainzone, RQ11
  ADD      RQ11, #8, RQ10
  MOV.Q    @(RQ10, 24), RQ14

L008025B8:
  MOV.Q    @(RQ14, 24), RQ13
  MOV.Q    mainzone, RQ9
  ADD      RQ9, #8, RQ12
  CMPQ/EQ  RQ12, RQ13
  BT       L008025BB
  BRA      L008025BA

L008025BB:
  MOV.L    @RQ14, RD8
  LEA.B    @(RQ14, RD8), RQ13
  MOV.Q    @(RQ14, 24), RQ12
  CMPQ/EQ  RQ12, RQ13
  BT       L008025BC
  MOV      L00C00495, RQ4
  BSR      I_Error

L008025BC:
  MOV.Q    @(RQ14, 24), RQ13
  MOV.Q    @(RQ13, 32), RQ12
  CMPQ/EQ  RQ14, RQ12
  BT       L008025BD
  MOV      L00C00496, RQ4
  BSR      I_Error

L008025BD:
  MOV.Q    @(RQ14, 8), RQ13
  MOV      #0, RQ31
  CMPQ/EQ  RQ31, RQ13
  BT       L008025BE
  MOV.Q    @(RQ14, 24), RQ13
  MOV.Q    @(RQ13, 8), RQ12
  MOV      #0, RQ30
  CMPQ/EQ  RQ30, RQ12
  BT       L008025BE
  BRA      L008025BF

L008025BF:
  MOV      L00C00497, RQ4
  BSR      I_Error

L008025BE:

L008025B9:
  MOV.Q    @(RQ14, 24), RQ14
  BRA      L008025B8

L008025BA:

L00E0050B:
  ADD      #144, R15
  BRA      L00C002C3

Z_ChangeTag2:
  PUSH     LR
  BSR      L00C003FF
  ADD      #-104, R15
  MOV      RQ4, RQ12
  MOV.L    RD5, @(R15, 20)
  MOV      #0, RQ10
  CMPQ/EQ  RQ10, RQ12
  BT       L008025C1
  BRA      L00E0050C

L008025C1:
  ADD      RQ12, #-40, RQ14
  MOV      RQ14, RQ13
  MOV.L    @(RQ13, 20), RD11
  CMP/EQ   #1919505, RD11
  BT       L008025C2
  MOV      L00C00499, RQ4
  BSR      I_Error

L008025C2:
  MOV.L    @(R15, 20), RD9
  CMP/GE   #100, RD9
  BT       L008025C3
  MOV.Q    @(RQ13, 8), RQ14
  MOV      #256, RQ8
  CMPQ/GT  RQ14, RQ8
  BT       L008025C3
  BRA      L008025C4

L008025C4:
  MOV      L00C0049A, RQ4
  BSR      I_Error

L008025C3:
  MOV.L    @(R15, 20), RD31
  MOV.L    RD31, @(RQ13, 16)

L00E0050C:
  ADD      #104, R15
  BRA      L00C00404

M_CheckParm:
  PUSH     LR
  BSR      L00C003ED
  ADD      #-128, R15
  MOV.Q    RQ4, @(R15, 24)
  MOV      #1, RD14

L00800F3C:
  MOV.L    myargc, RD11
  CMP/GT   RD14, RD11
  BT       L00800F3E
  MOV.Q    myargv, RQ10
  EXTS.L   RD14
  MOV.Q    @(RQ10, RD14), RQ12
  MOV.Q    @(R15, 24), RQ4
  MOV      RQ12, RQ5
  BSR      stricmp
  MOV      RD2, RD13
  CMP/EQ   RD13
  BT       L00800F3F
  MOV      RD14, RD2
  BRA      L00E0050D

L00800F3F:

L00800F3D:
  ADD      #1, RD14
  BRA      L00800F3C

L00800F3E:
  MOV      #0, RD2

L00E0050D:
  ADD      #128, R15
  BRA      L00C003EF

M_WriteFile:
  PUSH     LR
  BSR      L00C0046F
  ADD      #-128, R15
  MOV.Q    RQ4, @(R15, 40)
  MOV.Q    RQ5, @(R15, 32)
  MOV.L    RD6, @(R15, 28)
  MOV.Q    @(R15, 40), RQ4
  MOV      L00C000A6, RQ5
  BSR      fopen
  MOV      RQ2, RQ14
  MOV      #0, RQ12
  CMPQ/EQ  RQ12, RQ14
  BT       L00800F11
  MOV      #0, RD2
  BRA      L00E0050E

L00800F11:
  MOV      #1, RQ11
  EXTS.L   RQ11, RQ11
  MOV.L    @(R15, 28), RD10
  EXTS.L   RQ10, RQ10
  MOV.Q    RQ11, @(R15, 8)
  MOV.Q    RQ10, @R15
  MOV.Q    @(R15, 32), RQ4
  MOV      RQ11, RQ5
  MOV      RQ10, RQ6
  MOV      RQ14, RQ7
  BSR      fwrite
  MOV      RQ2, RQ13
  EXTS.L   RQ13, RD9
  MOV.L    RD9, @(R15, 24)
  MOV      RQ14, RQ4
  BSR      fclose
  MOV.L    @(R15, 28), RD8
  CMP/GT   RD9, RD8
  BT       L00800F12
  MOV      #0, RD2
  BRA      L00E0050E

L00800F12:
  MOV      #1, RD2

L00E0050E:
  ADD      #128, R15
  BRA      L00C00470

M_ReadFile:
  PUSH     LR
  BSR      L00C002C5
  ADD      #-112, R15
  MOV.Q    RQ4, @(R15, 56)
  MOV.Q    RQ5, @(R15, 48)
  MOV.Q    @(R15, 56), RQ4
  MOV      L00C0006A, RQ5
  BSR      fopen
  MOV      RQ2, RQ13
  MOV      #0, RQ11
  CMPQ/EQ  RQ11, RQ13
  BT       L00800F13
  MOV      L00C0049E, RQ4
  MOV.Q    @(R15, 56), RQ5
  BSR      I_Error

L00800F13:
  MOV      #0, RQ14
  EXTS.L   RQ14, RQ14
  MOV      RQ13, RQ4
  MOV      RQ14, RQ5
  MOV      #2, RD6
  BSR      fseek
  MOV      RQ13, RQ4
  BSR      ftell
  MOV      RQ2, RQ14
  EXTS.L   RQ14, RD12
  MOV      #0, RQ14
  EXTS.L   RQ14, RQ14
  MOV      RQ13, RQ4
  MOV      RQ14, RQ5
  MOV      #0, RD6
  BSR      fseek
  MOV      #0, RQ10
  MOV.Q    RQ10, @(R15, 16)
  MOV      RD12, RD4
  MOV      #1, RD5
  MOV      RQ10, RQ6
  BSR      Z_Malloc
  MOV      RQ2, RQ9
  MOV      #1, RQ8
  EXTS.L   RQ8, RQ8
  EXTS.L   RD12, RQ31
  MOV.Q    RQ9, @(R15, 40)
  MOV.Q    RQ8, @(R15, 8)
  MOV.Q    RQ31, @R15
  MOV      RQ9, RQ4
  MOV      RQ8, RQ5
  MOV      RQ31, RQ6
  MOV      RQ13, RQ7
  BSR      fread
  MOV      RQ2, RQ14
  EXTS.L   RQ14, RD30
  MOV.L    RD30, @(R15, 36)
  MOV      RQ13, RQ4
  BSR      fclose
  CMP/GT   RD30, RD12
  BT       L00800F14
  MOV      L00C0049E, RQ4
  MOV.Q    @(R15, 56), RQ5
  BSR      I_Error

L00800F14:
  MOV.Q    @(R15, 48), RQ29
  MOV.Q    @(R15, 40), RQ28
  MOV.Q    RQ28, @RQ29
  MOV      RD12, RD2

L00E0050F:
  ADD      #112, R15
  BRA      L00C002C6

M_ScreenShot:
  ADD      #-144, R15

L00E00510:
  ADD      #144, R15
  RTS     

M_LoadDefaults:
  PUSH     LR
  BSR      L00C00193
  ADD      #-328, R15
  MOV      #92, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 72)
  MOV      #172, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 64)
  MOV      #37, RD11
  MOV      #0, RD13
  MOV.L    RD11, numdefaults

L00800F1D:
  MOV.L    numdefaults, RD10
  CMP/GT   RD13, RD10
  BT       L00800F1F
  MOV      defaults, RQ9
  EXTS.L   RD13
  SHAD     RD13, #5, RQ8
  ADD      RQ9, RQ8
  MOV.Q    @(RQ8, 16), RQ31
  EXTS.L   RD13
  SHAD     RD13, #5, RQ8
  ADD      RQ9, RQ8
  MOV.Q    @(RQ8, 8), RQ12
  EXTS.L   RQ31, RD14
  MOV.L    RD14, @RQ12
  MOV.Q    RQ8, @(R15, 40)
  MOV.Q    RQ31, @(R15, 32)

L00800F1E:
  ADD      #1, RD13
  BRA      L00800F1D

L00800F1F:
  MOV      L00C004A3, RQ4
  BSR      M_CheckParm
  MOV      RD2, RD13
  CMP/EQ   RD13
  BT       L00800F20
  MOV.L    myargc, RD30
  SUB      RD30, #1, RD14
  CMP/GT   RD13, RD14
  BT       L00800F20
  BRA      L00800F21

L00800F21:
  ADD      RD13, #1, RD14
  MOV.Q    myargv, RQ29
  EXTS.L   RD14
  MOV.Q    @(RQ29, RD14), RQ28
  MOV.Q    RQ28, defaultfile
  MOV      L00C004A5, RQ4
  MOV.Q    defaultfile, RQ5
  BSR      printf
  BRA      L00800F23

L00800F20:
  MOV      basedefault, RQ26
  MOV      RQ26, RQ27
  MOV.Q    RQ27, defaultfile

L00800F23:
  MOV.Q    defaultfile, RQ4
  MOV      L00C000A8, RQ5
  BSR      fopen
  MOV      RQ2, RQ25
  MOV.Q    RQ25, @(R15, 80)
  MOV      #0, RQ26
  CMPQ/EQ  RQ26, RQ25
  BT       L00800F24

L00800F25:
  MOV.Q    @(R15, 80), RQ7
  MOV.L    @(RQ7, 68), RD14
  CMP/EQ   RD14
  BT       L00800F26
  MOV      #0, RD25
  MOV.L    RD25, @(R15, 48)
  MOV.Q    @(R15, 80), RQ4
  MOV      L00C004A6, RQ5
  MOV      #92, RQ6
  ADD      R15, RQ6
  MOV      #172, RQ7
  ADD      R15, RQ7
  BSR      fscanf
  MOV      RD2, RD14
  CMP/EQ   #2, RD14
  BT       L00800F27
  MOV      #172, RQ6
  ADD      R15, RQ6
  MOV.B    @RQ6, RD14
  CMP/EQ   #34, RD14
  BT       L00800F28
  MOV      #1, RD25
  MOV.L    RD25, @(R15, 48)
  MOV      #172, RQ4
  ADD      R15, RQ4
  BSR      strlen
  MOV      RQ2, RQ25
  EXTS.L   RQ25, RD26
  EXTS.L   RD26, RQ25
  MOV.L    RD26, @(R15, 88)
  MOV.Q    RQ25, @(R15, 32)
  MOV      RQ25, RQ4
  BSR      malloc
  MOV      RQ2, RQ12
  MOV      RQ12, RQ27
  SUB      RD26, #1, RD14
  MOV      #172, RQ25
  ADD      R15, RQ25
  MOV      #0, RD28
  EXTS.L   RD14
  MOV.B    RD28, @(RQ25, RD14)
  ADD      RQ25, #1, RQ12
  MOV.Q    RQ27, @(R15, 56)
  MOV      RQ27, RQ4
  MOV      RQ12, RQ5
  BSR      strcpy
  BRA      L00800F29

L00800F28:
  MOV      #172, RQ5
  ADD      R15, RQ5
  MOV.B    @RQ5, RD14
  CMP/EQ   #48, RD14
  BT       L00800F2A
  MOV      #172, RQ4
  ADD      R15, RQ4
  MOV.B    @(RQ4, 1), RD14
  CMP/EQ   #120, RD14
  BT       L00800F2A
  BRA      L00800F2B

L00800F2B:
  MOV      #52, RQ12
  ADD      R15, RQ12
  MOV      #172, RQ25
  ADD      R15, RQ25
  ADD      RQ25, #2, RQ26
  MOV.Q    RQ26, @(R15, 8)
  MOV      RQ26, RQ4
  MOV      L00C004A7, RQ5
  MOV      RQ12, RQ6
  BSR      sscanf
  BRA      L00800F2D

L00800F2A:
  MOV      #52, RQ12
  ADD      R15, RQ12
  MOV      #172, RQ4
  ADD      R15, RQ4
  MOV      L00C004A8, RQ5
  MOV      RQ12, RQ6
  BSR      sscanf

L00800F2D:

L00800F29:
  MOV      #0, RD13

L00800F2E:
  MOV.L    numdefaults, RD3
  CMP/GT   RD13, RD3
  BT       L00800F30
  MOV      defaults, RQ25
  EXTS.L   RD13
  SHAD     RD13, #5, RQ26
  ADD      RQ25, RQ26
  MOV.Q    @RQ26, RQ12
  MOV.Q    RQ26, @(R15, 40)
  MOV      #92, RQ4
  ADD      R15, RQ4
  MOV      RQ12, RQ5
  BSR      strcmp
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L00800F31
  MOV.L    @(R15, 48), RD2
  CMP/EQ   RD2
  BT       L00800F32
  MOV      defaults, RQ23
  EXTS.L   RD13
  SHAD     RD13, #5, RQ22
  ADD      RQ23, RQ22
  MOV.Q    @(RQ22, 8), RQ12
  MOV.L    @(R15, 52), RD21
  MOV.L    RD21, @RQ12
  MOV.Q    RQ22, @(R15, 40)
  BRA      L00800F33

L00800F32:
  MOV      defaults, RQ20
  EXTS.L   RD13
  SHAD     RD13, #5, RQ19
  ADD      RQ20, RQ19
  MOV.Q    @(RQ19, 8), RQ12
  MOV.Q    @(R15, 56), RQ18
  MOV.Q    RQ18, @RQ12
  MOV.Q    RQ19, @(R15, 40)

L00800F33:
  BRA      L00800F30

L00800F31:

L00800F2F:
  ADD      #1, RD13
  BRA      L00800F2E

L00800F30:

L00800F27:
  BRA      L00800F25

L00800F26:
  MOV.Q    @(R15, 80), RQ4
  BSR      fclose

L00800F24:

L00E00511:
  ADD      #328, R15
  BRA      L00C00194

W_InitMultipleFiles:
  PUSH     LR
  BSR      L00C002C5
  ADD      #-80, R15
  MOV      RQ4, RQ12
  MOV      #0, RD11
  MOV      #1, RQ14
  EXTS.L   RQ14, RQ14
  MOV.L    RD11, numlumps
  MOV      RQ14, RQ4
  BSR      malloc
  MOV      RQ2, RQ10
  MOV.Q    RQ10, lumpinfo

L008020EA:
  MOV.Q    @RQ12, RQ13
  MOV      #0, RQ9
  CMPQ/EQ  RQ9, RQ13
  BT       L008020EC
  MOV.Q    @RQ12, RQ13
  MOV      RQ13, RQ4
  BSR      W_AddFile

L008020EB:
  ADD      #8, RQ12
  BRA      L008020EA

L008020EC:
  MOV.L    numlumps, RD8
  CMP/EQ   RD8
  BT       L008020ED
  MOV      L00C004AD, RQ4
  BSR      I_Error

L008020ED:
  MOV.L    numlumps, RD31
  SHAD     RD31, #3, RD30
  EXTS.L   RD30, RQ14
  MOV.L    RD30, @(R15, 20)
  MOV      RQ14, RQ4
  BSR      malloc
  MOV      RQ2, RQ29
  MOV.Q    RQ29, lumpcache
  MOV.Q    lumpcache, RQ28
  MOV      #0, RQ8
  CMPQ/EQ  RQ8, RQ28
  BT       L008020EE
  MOV      L00C004AE, RQ4
  BSR      I_Error

L008020EE:
  MOV.L    @(R15, 20), RD14
  EXTS.L   RQ14, RQ14
  MOV.Q    lumpcache, RQ4
  MOV      #0, RD5
  MOV      RQ14, RQ6
  BSR      memset

L00E00512:
  ADD      #80, R15
  BRA      L00C002C6

W_Reload:
  PUSH     LR
  BSR      L00C00193
  ADD      #-104, R15
  MOV.Q    reloadname, RQ10
  MOV      #0, RQ9
  CMPQ/EQ  RQ9, RQ10
  BT       L008020E4
  BRA      L00E00513

L008020E4:
  MOV.Q    reloadname, RQ4
  MOV      L00C0006A, RQ5
  BSR      w_open
  MOV      RD2, RD14
  MOV      RD14, RD8
  MOV.L    RD8, @(R15, 44)
  CMP/EQ   #-1, RD14
  BT       L008020E5
  MOV      L00C004B2, RQ4
  MOV.Q    reloadname, RQ5
  BSR      I_Error

L008020E5:
  MOV      #64, RQ31
  ADD      R15, RQ31
  MOV      RQ31, RQ13
  MOV.L    @(R15, 44), RD4
  MOV      RQ13, RQ5
  MOV      #12, RD6
  BSR      w_read
  MOV.L    @(RQ31, 4), RD30
  MOV.L    @(RQ31, 8), RD14
  MOV.L    RD14, @(RQ31, 8)
  SHAD     RD30, #4, RD29
  EXTS.L   RD29, RQ28
  MOV.L    RD30, @(R15, 60)
  MOV.L    RD29, @(R15, 40)
  MOV.Q    RQ28, @(R15, 16)
  MOV      RQ28, RQ4
  BSR      malloc
  MOV      RQ2, RQ12
  MOV.L    @(RQ31, 8), RD14
  MOV.L    @(R15, 44), RD4
  MOV      RD14, RD5
  MOV      #0, RD6
  BSR      w_lseek
  MOV.L    @(R15, 44), RD4
  MOV      RQ12, RQ5
  MOV      RD29, RD6
  BSR      w_read
  MOV.Q    lumpinfo, RQ27
  MOV.L    reloadlump, RD26
  EXTS.L   RD26
  MOV      #20, RD7
  MUL.L    RD26, RD7
  STS      R0, RQ13
  ADD      RQ27, RQ13
  MOV      RQ13, RQ25
  MOV      RD26, RD11
  MOV.Q    RQ25, @(R15, 48)

L008020E6:
  MOV.L    reloadlump, RD7
  MOV.L    @(R15, 60), RD6
  ADD      RD7, RD6, RD14
  MOV      RD11, RD5
  MOV      RD14, RD4
  MOV.L    RD5, @(R15, 12)
  MOV.L    RD4, @(R15, 8)
  CMP/HI   RD5, RD4
  BT       L008020E8
  MOV.Q    lumpcache, RQ3
  EXTS.L   RD11
  MOV.Q    @(RQ3, RD11), RQ13
  MOV      #0, RQ2
  CMPQ/EQ  RQ2, RQ13
  BT       L008020E9
  MOV.Q    lumpcache, RQ25
  EXTS.L   RD11
  MOV.Q    @(RQ25, RD11), RQ13
  MOV      RQ13, RQ4
  BSR      Z_Free

L008020E9:
  MOV.L    @RQ12, RD14
  MOV.Q    @(R15, 48), RQ23
  MOV.L    RD14, @(RQ23, 12)
  MOV.L    @(RQ12, 4), RD14
  MOV.L    RD14, @(RQ23, 16)

L008020E7:
  MOV      RD11, RD14
  ADD      #1, RD11
  MOV.Q    @(R15, 48), RQ22
  MOV      RQ22, RQ13
  ADD      #20, RQ22
  MOV      RQ12, RQ13
  ADD      #16, RQ12
  MOV.Q    RQ22, @(R15, 48)
  BRA      L008020E6

L008020E8:
  MOV.L    @(R15, 44), RD4
  BSR      w_close

L00E00513:
  ADD      #104, R15
  BRA      L00C00194

W_CheckNumForName:
  PUSH     LR
  PUSH     R14
  ADD      #-144, R15
  MOV.Q    RQ4, @(R15, 8)
  MOV.L    numlumps, RD4
  MOV.Q    @(R15, 8), RQ5
  BSR      W_CheckNumForNameBase
  MOV      RD2, RD14
  MOV      RD14, RD2

L00E00514:
  ADD      #144, R15
  POP      RQ14
  RET     

W_GetNumForName:
  PUSH     LR
  PUSH     R14
  ADD      #-144, R15
  MOV.Q    RQ4, @(R15, 8)
  MOV.Q    @(R15, 8), RQ4
  BSR      W_CheckNumForName
  MOV      RD2, RD14
  CMP/EQ   #-1, RD14
  BT       L008020F4
  MOV      L00C004B8, RQ4
  MOV.Q    @(R15, 8), RQ5
  BSR      printf
  MOV      #-1, RD2
  BRA      L00E00515

L008020F4:
  MOV      RD14, RD2

L00E00515:
  ADD      #144, R15
  POP      RQ14
  RET     

W_LumpLength:
  PUSH     LR
  BSR      L00C001DF
  ADD      #-152, R15
  MOV.L    RD4, @(R15, 24)
  MOV.L    @(R15, 24), RD14
  MOV.L    numlumps, RD13
  CMP/GT   RD14, RD13
  BT       L008020F6
  MOV      L00C004BA, RQ4
  MOV.L    @(R15, 24), RD5
  BSR      I_Error

L008020F6:
  MOV.Q    lumpinfo, RQ12
  MOV.L    @(R15, 24), RD11
  MOV      #20, RD7
  MUL.L    RD11, RD7
  STS      R0, RQ13
  ADD      RQ12, RQ13
  MOV.L    @(RQ13, 16), RD14
  MOV.L    RD14, @(R15, 12)
  MOV      RD14, RD2

L00E00516:
  ADD      #152, R15
  BRA      L00C001E0

W_ReadLump:
  PUSH     LR
  BSR      L00C0046F
  ADD      #-112, R15
  MOV      RD4, RD10
  MOV.Q    RQ5, @(R15, 16)
  MOV.L    numlumps, RD9
  CMP/GT   RD10, RD9
  BT       L008020F7
  MOV      L00C004BC, RQ4
  MOV      RD10, RD5
  BSR      I_Error

L008020F7:
  MOV.Q    lumpinfo, RQ8
  EXTS.L   RD10
  MOV      #20, RD7
  MUL.L    RD10, RD7
  STS      R0, RQ13
  ADD      RQ8, RQ13
  MOV.L    @(RQ13, 8), RD14
  CMP/EQ   #-1, RD14
  BT       L008020F8
  MOV.Q    reloadname, RQ4
  MOV      L00C0006A, RQ5
  BSR      w_open
  MOV      RD2, RD14
  MOV      RD14, RD12
  CMP/EQ   #-1, RD14
  BT       L008020F9
  MOV      L00C004BD, RQ4
  MOV.Q    reloadname, RQ5
  BSR      I_Error

L008020F9:
  BRA      L008020FA

L008020F8:
  MOV.L    @(RQ13, 8), RD12

L008020FA:
  MOV.L    @(RQ13, 12), RD14
  MOV      RD12, RD4
  MOV      RD14, RD5
  MOV      #0, RD6
  BSR      w_lseek
  MOV.L    @(RQ13, 16), RD14
  MOV      RD12, RD4
  MOV.Q    @(R15, 16), RQ5
  MOV      RD14, RD6
  BSR      w_read
  MOV      RD2, RD11
  MOV.L    @(RQ13, 16), RD14
  CMP/GT   RD11, RD14
  BT       L008020FB
  MOV.L    @(RQ13, 16), RD14
  MOV      L00C004BE, RQ4
  MOV      RD11, RD5
  MOV      RD14, RD6
  MOV      RD10, RD7
  BSR      I_Error

L008020FB:
  MOV.L    @(RQ13, 8), RD14
  CMP/EQ   #-1, RD14
  BT       L008020FC
  MOV      RD12, RD4
  BSR      w_close

L008020FC:

L00E00517:
  ADD      #112, R15
  BRA      L00C00470

W_GetNumForCache:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  ADD      #-120, R15
  MOV.Q    RQ4, @(R15, 8)
  MOV      #0, RD14

L00802103:
  MOV.L    numlumps, RD12
  CMP/GT   RD14, RD12
  BT       L00802105
  MOV.Q    lumpcache, RQ11
  EXTS.L   RD14
  MOV.Q    @(RQ11, RD14), RQ13
  MOV.Q    @(R15, 8), RQ10
  CMPQ/EQ  RQ10, RQ13
  BT       L00802106
  MOV      RD14, RD2
  BRA      L00E00518

L00802106:

L00802104:
  ADD      #1, RD14
  BRA      L00802103

L00802105:
  MOV      #-1, RD2

L00E00518:
  ADD      #120, R15
  BRA      L00C00410

W_CacheLumpNum:
  PUSH     LR
  BSR      L00C00193
  ADD      #-72, R15
  MOV      RD4, RD13
  MOV.L    RD5, @(R15, 40)
  CMP/PZ   RD13
  BT       L008020FD
  MOV      #0, RQ14
  MOV      RQ14, RQ2
  BRA      L00E00519

L008020FD:
  MOV      RD13, RD12
  MOV.L    numlumps, RD10
  MOV      RD10, RD11
  CMP/HI   RD12, RD11
  BT       L008020FE
  MOV      L00C004BF, RQ4
  MOV      RD13, RD5
  BSR      I_Error

L008020FE:
  MOV.Q    lumpcache, RQ9
  EXTS.L   RD13
  MOV.Q    @(RQ9, RD13), RQ14
  MOV      #0, RQ8
  CMPQ/EQ  RQ8, RQ14
  BT       L008020FF
  MOV.Q    lumpcache, RQ31
  EXTS.L   RD13
  LEA.Q    @(RQ31, RD13), RQ14
  MOV      RD13, RD4
  BSR      W_LumpLength
  MOV      RD2, RD12
  MOV      RD12, RD4
  MOV.L    @(R15, 40), RD5
  MOV      RQ14, RQ6
  BSR      Z_Malloc
  MOV      RQ2, RQ30
  MOV.Q    lumpcache, RQ29
  EXTS.L   RD13
  MOV.Q    @(RQ29, RD13), RQ14
  MOV.Q    RQ30, @(R15, 32)
  MOV      RD13, RD4
  MOV      RQ14, RQ5
  BSR      W_ReadLump
  BRA      L00802100

L008020FF:
  MOV.Q    lumpcache, RQ28
  EXTS.L   RD13
  MOV.Q    @(RQ28, RD13), RQ14
  MOV      #0, RQ27
  CMPQ/EQ  RQ27, RQ14
  BT       L00802101
  MOV.Q    lumpcache, RQ26
  EXTS.L   RD13
  MOV.Q    @(RQ26, RD13), RQ14
  ADD      RQ14, #-40, RQ25
  MOV.L    @(RQ25, 20), RD12
  CMP/EQ   #1919505, RD12
  BT       L00802102
  MOV      L00C004C0, RQ4
  MOV      #600, RD5
  BSR      I_Error

L00802102:
  MOV.Q    lumpcache, RQ25
  EXTS.L   RD13
  MOV.Q    @(RQ25, RD13), RQ14
  MOV      RQ14, RQ4
  MOV.L    @(R15, 40), RD5
  BSR      Z_ChangeTag2

L00802101:

L00802100:
  MOV.Q    lumpcache, RQ7
  EXTS.L   RD13
  MOV.Q    @(RQ7, RD13), RQ14
  MOV      RQ14, RQ2

L00E00519:
  ADD      #72, R15
  BRA      L00C00194

W_CacheLumpName:
  PUSH     LR
  PUSH     R14
  PUSH     R13
  ADD      #-152, R15
  MOV.Q    RQ4, @(R15, 24)
  MOV.L    RD5, @(R15, 20)
  MOV.Q    @(R15, 24), RQ4
  BSR      W_GetNumForName
  MOV      RD2, RD14
  MOV      RD14, RD4
  MOV.L    @(R15, 20), RD5
  BSR      W_CacheLumpNum
  MOV      RQ2, RQ13
  MOV      RQ13, RQ2

L00E0051A:
  ADD      #152, R15
  POP      RQ13
  POP      RQ14
  RET     

W_CacheLumpNameBase:
  PUSH     LR
  PUSH     R14
  PUSH     R13
  ADD      #-152, R15
  MOV.L    RD4, @(R15, 28)
  MOV.Q    RQ5, @(R15, 16)
  MOV.L    RD6, @(R15, 12)
  MOV.L    @(R15, 28), RD4
  MOV.Q    @(R15, 16), RQ5
  BSR      W_GetNumForNameBase
  MOV      RD2, RD14
  MOV      RD14, RD4
  MOV.L    @(R15, 12), RD5
  BSR      W_CacheLumpNum
  MOV      RQ2, RQ13
  MOV      RQ13, RQ2

L00E0051B:
  ADD      #152, R15
  POP      RQ13
  POP      RQ14
  RET     

getsfx:
  PUSH     LR
  BSR      L00C00193
  ADD      #-168, R15
  MOV.Q    RQ4, @(R15, 96)
  MOV.Q    RQ5, @(R15, 88)
  MOV      #104, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 48)
  MOV      #104, RQ4
  ADD      R15, RQ4
  MOV      L00C003EA, RQ5
  MOV.Q    @(R15, 96), RQ6
  BSR      sprintf
  MOV      #104, RQ4
  ADD      R15, RQ4
  BSR      W_CheckNumForName
  MOV      RD2, RD14
  CMP/EQ   #-1, RD14
  BT       L00800937
  MOV      L00C004C6, RQ4
  BSR      W_CheckNumForName
  MOV      RD2, RD14
  CMP/EQ   #-1, RD14
  BT       L00800938
  MOV      L00C004C6, RQ4
  BSR      W_GetNumForName
  MOV      RD2, RD13
  BRA      L00800939

L00800938:
  MOV      L00C004C7, RQ4
  BSR      W_CheckNumForName
  MOV      RD2, RD14
  CMP/EQ   #-1, RD14
  BT       L0080093A
  MOV      L00C004C7, RQ4
  BSR      W_GetNumForName
  MOV      RD2, RD13
  BRA      L0080093B

L0080093A:
  MOV      L00C004C6, RQ4
  BSR      W_GetNumForName
  MOV      RD2, RD13

L0080093B:

L00800939:
  BRA      L0080093C

L00800937:
  MOV      #104, RQ4
  ADD      R15, RQ4
  BSR      W_GetNumForName
  MOV      RD2, RD13

L0080093C:
  MOV      RD13, RD4
  BSR      W_LumpLength
  MOV      RD2, RD12
  MOV.L    RD12, @(R15, 64)
  MOV      RD13, RD4
  MOV      #1, RD5
  BSR      W_CacheLumpNum
  MOV      RQ2, RQ11
  MOV      RQ11, RQ10
  SUB      RD12, #8, RD14
  ADD      RD14, #511, RD9
  SHAD     RD9, #-9, RD14
  SHAD     RD14, #9, RD8
  ADD      RD8, #16, RD14
  MOV      #0, RQ31
  MOV.Q    RQ11, @(R15, 32)
  MOV.Q    RQ10, @(R15, 80)
  MOV.L    RD8, @(R15, 60)
  MOV.Q    RQ31, @(R15, 16)
  MOV      RD14, RD4
  MOV      #1, RD5
  MOV      RQ31, RQ6
  BSR      Z_Malloc
  MOV      RQ2, RQ11
  MOV      RQ11, RQ30
  EXTS.L   RD12, RQ29
  MOV.Q    RQ11, @(R15, 32)
  MOV.Q    RQ30, @(R15, 72)
  MOV.Q    RQ29, @(R15, 8)
  MOV      RQ30, RQ4
  MOV      RQ10, RQ5
  MOV      RQ29, RQ6
  BSR      memcpy
  MOV      RD12, RD28
  MOV.L    RD28, @(R15, 68)

L0080093D:
  MOV.L    @(R15, 60), RD27
  ADD      RD27, #8, RD14
  MOV.L    @(R15, 68), RD26
  CMP/GT   RD26, RD14
  BT       L0080093F
  MOV.Q    @(R15, 72), RQ25
  MOV.L    @(R15, 68), RD7
  MOV      #128, RD6
  MOVU.B   RD6, @(RQ25, RD7)

L0080093E:
  MOV.L    @(R15, 68), RD5
  ADD      #1, RD5
  MOV.L    RD5, @(R15, 68)
  BRA      L0080093D

L0080093F:
  MOV.Q    @(R15, 80), RQ4
  BSR      Z_Free
  MOV.Q    @(R15, 88), RQ4
  MOV.L    @(R15, 60), RD3
  MOV.L    RD3, @RQ4
  MOV.Q    @(R15, 72), RQ2
  ADD      RQ2, #8, RQ23
  MOV.Q    RQ23, @(R15, 32)
  MOV      RQ23, RQ2

L00E0051C:
  ADD      #168, R15
  BRA      L00C00194

addsfx:
  PUSH     LR
  BSR      L00C00193
  ADD      #-131208, R15
  MOV.L    RD4, @(R15, 60)
  MOV.L    RD5, @(R15, 56)
  MOV.L    RD6, @(R15, 52)
  MOV.L    RD7, @(R15, 48)
  MOV      #-1, RD14
  MOV      RD14, RD9
  MOV.L    gametic, RD8
  MOV      RD8, RD14
  MOV      RD14, RD10
  MOV      #0, RD14
  MOV      RD14, RD31
  MOV.L    RD31, @(R15, 44)
  MOV.L    @(R15, 60), RD30
  CMP/EQ   #10, RD30
  BT       L00800941
  MOV.L    @(R15, 60), RD29
  CMP/EQ   #11, RD29
  BT       L00800941
  BRA      L00800945

L00800945:
  MOV.L    @(R15, 60), RD28
  CMP/EQ   #12, RD28
  BT       L00800941
  BRA      L00800944

L00800944:
  MOV.L    @(R15, 60), RD27
  CMP/EQ   #13, RD27
  BT       L00800941
  BRA      L00800943

L00800943:
  MOV.L    @(R15, 60), RD26
  CMP/EQ   #22, RD26
  BT       L00800941
  BRA      L00800942

L00800942:
  MOV.L    @(R15, 60), RD25
  CMP/EQ   #1, RD25
  BT       L00800941
  BRA      L00800940

L00800941:
  MOV      #0, RD13

L00800947:
  MOV      #8, RD7
  CMP/GE   RD7, RD13
  BT       L00800949
  MOV      channels, RQ6
  EXTS.L   RD13
  MOV.Q    @(RQ6, RD13), RQ12
  MOV      #0, RQ5
  CMPQ/EQ  RQ5, RQ12
  BT       L0080094A
  MOV      channelids, RQ4
  EXTS.L   RD13
  MOV.L    @(RQ4, RD13), RD14
  MOV.L    @(R15, 60), RD3
  CMP/EQ   RD3, RD14
  BT       L0080094A
  BRA      L0080094B

L0080094B:
  MOV      #0, RQ12
  MOV      channels, RQ2
  EXTS.L   RD13
  MOV.Q    RQ12, @(RQ2, RD13)
  BRA      L00800949

L0080094A:

L00800948:
  ADD      #1, RD13
  BRA      L00800947

L00800949:

L00800940:
  MOV      #0, RD13

L0080094D:
  MOV      #8, RD23
  CMP/GE   RD23, RD13
  BT       L0080094F
  MOV      channels, RQ22
  EXTS.L   RD13
  MOV.Q    @(RQ22, RD13), RQ12
  MOV      #0, RQ21
  CMPQ/EQ  RQ21, RQ12
  BT       L0080094F
  BRA      L00800950

L00800950:
  MOV      channelstart, RQ20
  EXTS.L   RD13
  MOV.L    @(RQ20, RD13), RD14
  CMP/GT   RD14, RD10
  BT       L00800952
  MOV      RD13, RD19
  MOV      channelstart, RQ18
  EXTS.L   RD13
  MOV.L    @(RQ18, RD13), RD10
  MOV.L    RD19, @(R15, 44)

L00800952:

L0080094E:
  ADD      #1, RD13
  BRA      L0080094D

L0080094F:
  CMP/EQ   #8, RD13
  BT       L00800953
  MOV.L    @(R15, 44), RD17
  MOV      RD17, RD11
  BRA      L00800954

L00800953:
  MOV      RD13, RD11

L00800954:
  MOV      S_sfx, RQ25
  MOV.L    @(R15, 60), RD26
  EXTS.L   RD26
  MOV      #48, RD7
  MUL.L    RD26, RD7
  STS      R0, RQ27
  ADD      RQ25, RQ27
  MOV.Q    @(RQ27, 32), RQ12
  MOV      channels, RQ28
  EXTS.L   RD11
  MOV.Q    RQ12, @(RQ28, RD11)
  EXTS.L   RD11
  MOV.Q    @(RQ28, RD11), RQ12
  MOV      lengths, RQ29
  EXTS.L   RD26
  MOV.L    @(RQ29, RD26), RD14
  LEA.B    @(RQ12, RD14), RQ30
  MOV      channelsend, RQ31
  EXTS.L   RD11
  MOV.Q    RQ30, @(RQ31, RD11)
  MOVU.W   handlenums, RD8
  CMP/EQ   RD8
  BT       L00800955
  MOV      #100, RD14
  MOV      RD14, RD16
  MOV.W    RD16, handlenums

L00800955:
  MOVU.W   handlenums, RD16
  MOV      RD16, RD14
  ADD      #1, RD16
  MOV      RD14, RD9
  MOV      channelhandles, RQ17
  EXTS.L   RD11
  MOV.L    RD14, @(RQ17, RD11)
  MOV      channelstep, RQ18
  MOV.L    @(R15, 52), RD19
  EXTS.L   RD11
  MOVU.L   RD19, @(RQ18, RD11)
  MOV      channelstepremainder, RQ20
  MOV      #0, RD21
  EXTS.L   RD11
  MOVU.L   RD21, @(RQ20, RD11)
  MOV      channelstart, RQ22
  MOV.L    gametic, RD23
  EXTS.L   RD11
  MOV.L    RD23, @(RQ22, RD11)
  MOV.L    @(R15, 48), RD2
  ADD      #1, RD2
  MOV.L    @(R15, 56), RD3
  MUL.L    RD3, RD2, RD14
  MUL.L    RD14, RD2, RD4
  SHAD     RD4, #-16, RD14
  SUB      RD3, RD14, RD5
  SUB      #257, RD2
  MUL.L    RD3, RD2, RD14
  MUL.L    RD14, RD2, RD4
  SHAD     RD4, #-16, RD14
  SUB      RD3, RD14, RD6
  MOV.L    RD6, @(R15, 40)
  MOV.L    RD5, @(R15, 36)
  MOV.L    RD2, @(R15, 48)
  MOV.W    RD16, handlenums
  CMP/PZ   RD6
  BT       L00800957
  MOV.L    @(R15, 40), RD16
  CMP/GT   #127, RD16
  BT       L00800957
  BRA      L00800956

L00800957:
  MOV      L00C004C9, RQ4
  BSR      I_Error

L00800956:
  MOV.L    @(R15, 36), RD16
  MOV      #0, RD17
  CMP/GE   RD17, RD16
  BT       L0080095A
  MOV.L    @(R15, 36), RD16
  CMP/GT   #127, RD16
  BT       L0080095A
  BRA      L00800959

L0080095A:
  MOV      L00C004CA, RQ4
  BSR      I_Error

L00800959:
  MOV.L    @(R15, 36), RD16
  SHAD     RD16, #8, RD14
  MOV      vol_lookup, RQ17
  LEA.L    @(RQ17, RD14), RQ12
  MOV      channelleftvol_lookup, RQ18
  EXTS.L   RD11
  MOV.Q    RQ12, @(RQ18, RD11)
  MOV.L    @(R15, 40), RD19
  SHAD     RD19, #8, RD14
  LEA.L    @(RQ17, RD14), RQ12
  MOV      channelrightvol_lookup, RQ20
  EXTS.L   RD11
  MOV.Q    RQ12, @(RQ20, RD11)
  MOV      channelids, RQ21
  MOV.L    @(R15, 60), RD22
  EXTS.L   RD11
  MOV.L    RD22, @(RQ21, RD11)
  MOV      RD9, RD2

L00E0051D:
  ADD      #131208, R15
  BRA      L00C00194

I_MusicSubmit:
  ADD      #-144, R15

L00E0051E:
  ADD      #144, R15
  RTS     

L00C004CE:
  PUSH     R31
  PUSH     R30
  PUSH     R29
  PUSH     R28
  PUSH     R27
  PUSH     R26
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  PUSH     R9
  PUSH     R8
  PUSH     FR15
  PUSH     FR14
  PUSH     FR13
  PUSH     FR12
  PUSH     FR11
  PUSH     FR10
  RTS     

I_SubmitSound2:
  PUSH     LR
  BSR      L00C004CE
  ADD      #-96, R15
  MOV.L    RD4, @(R15, 44)
  MOV      #0xC636B098, DR15
  FSTCI    DR15
  MOV      R0, RD14
  MOV      RD14, RD12
  BSR      TK_GetTimeMs
  MOV      R2, RD9
  MOV.L    TU0ADE00C6/iss_lastms, RD31
  SUB      RD9, RD31, RD8
  MOV      RD9, RD31
  SHAD     RD8, #4, RD10
  MOV.L    RD9, TU0ADE00C6/iss_curms
  MOV.L    RD8, @(R15, 36)
  MOV.L    RD31, TU0ADE00C6/iss_lastms
  CMP/PZ   RD10
  BT       L00800974
  MOV      #0, RD10

L00800974:
  CMP/GT   RD12, RD10
  BT       L00800975
  MOV      RD12, RD10

L00800975:
  MOV.L    snd_MusicVolume, RD29
  MOV      RD29, RD30
  MOV      #0xC636B098, DR14
  FSTCI    DR14
  MOV      R0, RD14
  MOV      RD14, RD12
  MOV      #0, RD11
  MOV.L    RD30, @(R15, 32)

L00800976:
  CMP/GT   RD11, RD12
  BT       L00800978
  EXTS.L   RD11
  MOV      RD11, R0
  FLDCI    FR13
  MOV      #0xF6FEFAC0, DR12
  FMOV     DR12, FR11
  FMOV     FR13, FR10
  FMUL     FR11, FR10
  FSTCI    FR10
  MOV      R0, RD28
  SHAD     RD28, #1, RD14
  MOV      RD14, RD13
  MOV      mixbuffer, RQ27
  MOV.W    @(RQ27, RD13), RD14
  SHAD     RD11, #1, RD13
  MOV      RD13, RD26
  MOV      mixbuf2, RQ7
  EXTS.L   RD26
  MOV.W    RD14, @(RQ7, RD26)
  SHAD     RD28, #1, RD14
  ADD      RD14, #1, RD13
  EXTS.L   RD13
  MOV.W    @(RQ27, RD13), RD14
  SHAD     RD11, #1, RD13
  ADD      RD13, #1, RD26
  EXTS.L   RD26
  MOV.W    RD14, @(RQ7, RD26)
  MOV.L    RD28, @(R15, 40)

L00800977:
  ADD      #1, RD11
  BRA      L00800976

L00800978:
  CMP/PL   RD10
  BT       L00800979
  MOV.L    @(R15, 44), RD6
  TST      #1, RD6
  BT       L0080097A
  MOV      mixbuf2, RQ4
  MOV      RD12, RD5
  MOV      RD12, RD6
  BSR      SoundDev_WriteStereoSamples2
  BRA      L0080097B

L0080097A:
  MOV      mixbuf2, RQ4
  MOV      RD10, RD5
  MOV      RD12, RD6
  BSR      SoundDev_WriteStereoSamples2

L0080097B:
  BSR      SoundDev_Submit

L00800979:
  BSR      I_MusicSubmit

L00E0051F:
  ADD      #96, R15

L00C004D2:
  POP      FR10
  POP      FR11
  POP      FR12
  POP      FR13
  POP      FR14
  POP      FR15
  POP      RQ8
  POP      RQ9
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  POP      RQ26
  POP      RQ27
  POP      RQ28
  POP      RQ29
  POP      RQ30
  POP      RQ31
  RET     

SoundDev_WriteStereoSamples2:
  PUSH     LR
  BSR      L00C00487
  ADD      #-88, R15
  MOV.Q    RQ4, @(R15, 24)
  MOV.L    RD5, @(R15, 20)
  MOV.L    RD6, @(R15, 16)
  MOV.L    dma_pos, RD10
  ADD      RD10, #512, RD14
  AND      RD14, #8191, RD11
  MOV      #0, RD12

L0080098B:
  MOV.L    @(R15, 16), RD9
  CMP/GT   RD12, RD9
  BT       L0080098D
  SHAD     RD12, #1, RD14
  MOV      RD14, RD13
  MOV.Q    @(R15, 24), RQ8
  MOV.W    @(RQ8, RD13), RD14
  SHAD     RD11, #1, RD13
  MOV      RD13, RD31
  MOV      dma_buffer, RQ30
  EXTS.L   RD31
  MOV.W    RD14, @(RQ30, RD31)
  SHAD     RD12, #1, RD14
  ADD      RD14, #1, RD13
  EXTS.L   RD13
  MOV.W    @(RQ8, RD13), RD14
  SHAD     RD11, #1, RD13
  ADD      RD13, #1, RD31
  EXTS.L   RD31
  MOV.W    RD14, @(RQ30, RD31)
  ADD      RD11, #1, RD14
  AND      RD14, #8191, RD11

L0080098C:
  ADD      #1, RD12
  BRA      L0080098B

L0080098D:
  MOV.L    dma_pos, RD29
  MOV.L    @(R15, 20), RD7
  ADD      RD29, RD7, RD14
  AND      RD14, #8191, RD29
  MOV.L    RD29, dma_pos

L00E00520:
  ADD      #88, R15
  BRA      L00C0048B

SoundDev_Submit:
  PUSH     LR
  ADD      #-136, R15
  BSR      SNDDMA_Submit

L00E00521:
  ADD      #136, R15
  RET     

SNDDMA_Submit:
  PUSH     LR
  BSR      L00C00193
  ADD      #-104, R15
  BSR      SNDDMA_GetDMAPos
  MOV      RD2, RD10
  BSR      SNDDMA_GetDevDMAPos
  MOV      RD2, RD31
  ADD      RD31, #500, RD30
  MOV.L    RD31, @(R15, 68)
  MOV.L    RD30, @(R15, 64)
  MOV.L    TU0ADE00B8/dmarov, RD29
  MOV      #0, RD28
  CMP/GE   RD28, RD29
  BT       L008009C6
  MOV      RD10, RD27
  MOV      RD10, RD26
  MOV.L    @(R15, 64), RD7
  MOV      RD7, RD25
  MOV.L    RD27, olddma
  MOV.L    RD26, TU0ADE00B8/wbufrov
  MOV.L    RD25, TU0ADE00B8/dmarov

L008009C6:
  MOV      dma_buffer, RQ5
  MOV      RQ5, RQ6
  MOV      #0xA0080000, RQ4
  MOV      #0x0000A008, RQ4
  MOV.L    olddma, RD3
  MOV      RD3, RD8
  SUB      RD10, RD8, RD12
  MOV      RD10, RD8
  MOV.Q    RQ6, @(R15, 72)
  MOV.Q    RQ4, TU0ADE00B8/snd_dmabuf
  CMP/PZ   RD12
  BT       L008009C7
  ADD      #8192, RD12

L008009C7:
  CMP/PZ   RD12
  BT       L008009C8
  BRA      L00E00522

L008009C8:
  MOV.L    TU0ADE00B8/dmarov, RD2
  MOV.L    @(R15, 64), RD23
  SUB      RD2, RD23, RD11
  SHAD     RD11, #18, RD14
  SHAD     RD14, #-18, RD11
  SHAD     RD11, #-31, RD14
  XOR      RD14, RD11
  CMP/GT   #2200, RD11
  BT       L008009C9
  MOV.L    @(R15, 64), RD21
  MOV      RD21, RD22
  MOV.L    RD22, TU0ADE00B8/dmarov

L008009C9:
  SHAD     RD8, #-7, RD20
  AND      RD8, #127, RD14
  ADD      RD12, RD14, RD13
  ADD      RD13, #255, RD14
  SHAD     RD14, #-7, RD19
  MOV      #0, RD18
  MOV      #0, RD9
  MOV.L    RD20, @(R15, 52)
  MOV.L    RD19, @(R15, 48)
  MOV.L    RD18, TU0ADE00B8/snd_dmapred

L008009CA:
  MOV.L    @(R15, 48), RD17
  CMP/GT   RD9, RD17
  BT       L008009CC
  MOV      #0, RD16
  MOV.L    @(R15, 52), RD17
  ADD      RD17, RD9, RD14
  AND      RD14, #63, RD18
  ADD      RD17, RD9, RD14
  SHAD     RD14, #7, RD13
  AND      RD13, #8191, RD19
  MOV      #0, RD20
  MOV.L    RD20, @(R15, 36)
  MOV.L    RD19, @(R15, 40)
  MOV.L    RD18, @(R15, 44)
  MOV.L    RD16, TU0ADE00B8/snd_dmapred

L008009CD:
  MOV.L    @(R15, 36), RD16
  MOV      #128, RD17
  CMP/GE   RD17, RD16
  BT       L008009CF
  MOV.L    @(R15, 40), RD16
  MOV.L    @(R15, 36), RD17
  ADD      RD16, RD17, RD14
  SHAD     RD14, #1, RD13
  MOV      RD13, RD14
  MOV.Q    @(R15, 72), RQ18
  MOV.W    @(RQ18, RD14), RD19
  ADD      RD16, RD17, RD14
  SHAD     RD14, #1, RD13
  ADD      RD13, #1, RD14
  EXTS.L   RD14
  MOV.W    @(RQ18, RD14), RD20
  MOV.L    RD20, @(R15, 56)
  MOV.L    RD19, @(R15, 60)
  MOV      #-4096, RD21
  CMP/GE   RD21, RD19
  BT       L008009D0
  MOV      #0, RD16
  MOV.L    RD16, @(R15, 60)

L008009D0:
  MOV.L    @(R15, 60), RD16
  CMP/GT   #4096, RD16
  BT       L008009D1
  MOV      #0, RD16
  MOV.L    RD16, @(R15, 60)

L008009D1:
  MOV.L    @(R15, 56), RD16
  MOV      #-4096, RD17
  CMP/GE   RD17, RD16
  BT       L008009D2
  MOV      #0, RD16
  MOV.L    RD16, @(R15, 56)

L008009D2:
  MOV.L    @(R15, 56), RD16
  CMP/GT   #4096, RD16
  BT       L008009D3
  MOV      #0, RD16
  MOV.L    RD16, @(R15, 56)

L008009D3:
  MOV.L    @(R15, 36), RD16
  SHAD     RD16, #1, RD14
  MOV      RD14, RD13
  MOV      TU0ADE00B8/snd_tsblk, RQ17
  MOV.L    @(R15, 60), RD18
  MOV.W    RD18, @(RQ17, RD13)
  SHAD     RD16, #1, RD14
  ADD      RD14, #1, RD13
  MOV.L    @(R15, 56), RD19
  EXTS.L   RD13
  MOV.W    RD19, @(RQ17, RD13)

L008009CE:
  MOV.L    @(R15, 36), RD16
  ADD      #1, RD16
  MOV.L    RD16, @(R15, 36)
  BRA      L008009CD

L008009CF:
  MOV      TU0ADE00B8/snd_dmapred, RQ25
  MOV.L    @(R15, 44), RD26
  SHAD     RD26, #4, RD14
  MOV.Q    TU0ADE00B8/snd_dmabuf, RQ27
  LEA.L    @(RQ27, RD14), RQ28
  MOV.Q    RQ28, @(R15, 8)
  MOV.Q    RQ25, @(R15, 16)
  MOV      TU0ADE00B8/snd_tsblk, RQ4
  MOV      RQ28, RQ5
  MOV      RQ25, RQ6
  BSR      Sblkau_EncodeBlock

L008009CB:
  ADD      #1, RD9
  BRA      L008009CA

L008009CC:
  MOV      RD10, RD16
  MOV.L    TU0ADE00B8/wbufrov, RD17
  ADD      RD12, RD17
  MOV.L    TU0ADE00B8/dmarov, RD18
  ADD      RD12, RD18
  MOV.L    RD18, TU0ADE00B8/dmarov
  MOV.L    RD17, TU0ADE00B8/wbufrov
  MOV.L    RD16, olddma

L00E00522:
  ADD      #104, R15
  BRA      L00C00194

Sblkau_SampToPred9:
  PUSH     LR
  BSR      L00C0046F
  ADD      #-96, R15
  MOV      RD4, RD10
  CMP/PZ   RD10
  BT       L0080098E
  MOV      RD10, RD14
  MOV      #0, RD9
  BRA      L0080098F

L0080098E:
  NEG      RD10, RD14
  MOV      #1, RD9

L0080098F:
  MOV      #0, RD13
  SHAD     #-2, RD14
  CMP/GE   #64, RD14
  BT       L00800990

L00800991:
  CMP/GE   #64, RD14
  BT       L00800992
  ADD      #1, RD13
  SHAD     #-1, RD14
  BRA      L00800991

L00800992:
  CMP/GT   #7, RD13
  BT       L00800993
  MOV      #7, RD13
  MOV      #63, RD14

L00800993:
  BRA      L00800994

L00800990:
  SHAD     #-1, RD14

L00800994:
  SHAD     RD9, #8, RD12
  SHAD     RD13, #5, RD11
  OR       RD12, RD11, RD8
  AND      RD14, #31, RD12
  OR       RD8, RD12, RD11
  MOV      RD11, RD2

L00E00523:
  ADD      #96, R15
  BRA      L00C00470

Sblkau_StepForSamp:
  PUSH     LR
  BSR      L00C0046F
  ADD      #-96, R15
  MOV.L    RD4, @(R15, 12)
  MOV.L    @(R15, 12), RD11
  MOV      RD11, RD13
  CMP/PZ   RD13
  BT       L00800998
  NEG      RD13, RD13

L00800998:
  MOV      #1, RD14
  MOV      TU0ADE00B8/sblkau_rangetab, RQ10
  MOV.L    @(RQ10, RD14), RD12

L00800999:
  SHAD     RD12, #-1, RD9
  MOV.L    RD9, @(R15, 8)
  CMP/GT   RD9, RD13
  BT       L0080099A
  ADD      #1, RD14
  MOV      TU0ADE00B8/sblkau_rangetab, RQ8
  EXTS.L   RD14
  MOV.L    @(RQ8, RD14), RD12
  BRA      L00800999

L0080099A:
  MOV      RD14, RD2

L00E00524:
  ADD      #96, R15
  BRA      L00C00470

Sblkau_DeltaForSamp:
  PUSH     LR
  BSR      L00C0046F
  ADD      #-96, R15
  MOV.L    RD4, @(R15, 12)
  MOV      RD5, RD11
  MOV.L    @(R15, 12), RD9
  AND      RD9, #63, RD14
  MOV      TU0ADE00B8/sblkau_rangetab, RQ8
  EXTS.L   RD14
  MOV.L    @(RQ8, RD14), RD12
  TST      #4, RD11
  BT       L0080099B
  AND      RD11, #3, RD14
  SHAD     RD14, #1, RD13
  ADD      RD13, #1, RD14
  MUL.L    RD12, RD14, RD13
  NEG      RD13, RD14
  MOV      RD14, RD10
  MOV      #0x24924925, RD7
  MOV      #9362, RD7
  DMULS    RD7, RD10
  STS      R1, RD10
  BRA      L0080099C

L0080099B:
  AND      RD11, #3, RD14
  SHAD     RD14, #1, RD13
  ADD      RD13, #1, RD14
  MUL.L    RD12, RD14, RD13
  MOV      RD13, RD10
  MOV      #0x24924925, RD7
  MOV      #9362, RD7
  DMULS    RD7, RD10
  STS      R1, RD10

L0080099C:
  MOV      RD10, RD2

L00E00525:
  ADD      #96, R15
  BRA      L00C00470

Sblkau_SampForDelta:
  PUSH     LR
  BSR      L00C00190
  ADD      #-64, R15
  MOV.L    RD4, @(R15, 28)
  MOV.L    RD5, @(R15, 24)
  MOV.L    RD6, @(R15, 20)
  MOV.L    @(R15, 28), RD9
  AND      RD9, #63, RD13
  MOV      TU0ADE00B8/sblkau_rangetab, RQ8
  EXTS.L   RD13
  MOV.L    @(RQ8, RD13), RD11
  CMP/EQ   RD11
  BT       L0080099D
  BRK     

L0080099D:
  CMP/EQ   RD11
  BT       L0080099E
  MOV      #1, RD11

L0080099E:
  MOV.L    @(R15, 20), RD31
  MOV.L    @(R15, 24), RD30
  SUB      RD31, RD30, RD12
  MOV      #0, RD10
  CMP/PZ   RD12
  BT       L0080099F
  MOV      #4, RD10
  NEG      RD12, RD12

L0080099F:
  SHAD     RD12, #2, RD13
  MOV      RD11, RD5
  MOV      RD13, RD4
  BSR      __sdivsi3
  MOV      RD2, RD14
  EXTU.W   RD14, RD13
  MOV      RD14, RD29
  EXTU.W   RD13, RD28
  MOV.L    RD29, @(R15, 12)
  MOV.L    RD28, @(R15, 8)
  CMP/EQ   RD28, RD29
  BT       L008009A0
  BRK     

L008009A0:
  MOV      #0, RD27
  MOV      #3, RD26
  CMP/GT   RD14, RD27
  CSELT    RD27, RD14, RD14
  CMP/GT   RD14, RD26
  CSELT    RD14, RD26, RD14
  OR       RD10, RD14
  MOV      RD14, RD2

L00E00526:
  ADD      #64, R15
  BRA      L00C00191

Sblkau_AdjustStepForSamp:
  PUSH     LR
  BSR      L00C00198
  ADD      #-104, R15
  MOV      RD4, RD13
  MOV.L    RD5, @(R15, 8)
  MOV.L    @(R15, 8), RD12
  AND      RD12, #3, RD14
  CMP/EQ   RD14
  BT       L008009A1
  ADD      #-1, RD13

L008009A1:
  MOV.L    @(R15, 8), RD11
  AND      RD11, #3, RD14
  CMP/EQ   #3, RD14
  BT       L008009A2
  ADD      #2, RD13

L008009A2:
  MOV      #1, RD10
  MOV      #62, RD9
  CMP/GT   RD13, RD10
  CSELT    RD10, RD13, RD13
  CMP/GT   RD13, RD9
  CSELT    RD13, RD9, RD13
  MOV      RD13, RD2

L00E00527:
  ADD      #104, R15
  BRA      L00C0019A

Sblkau_EncodeBlock:
  PUSH     LR
  BSR      L00C00193
  ADD      #-1672, R15
  MOV.Q    RQ4, @(R15, 360)
  MOV.Q    RQ5, @(R15, 352)
  MOV.Q    RQ6, @(R15, 344)
  MOV      #368, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 336)
  MOV      #880, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 328)
  MOV      #1392, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 320)
  MOV      #1520, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 312)
  MOV.Q    @(R15, 344), RQ8
  MOVU.L   @RQ8, RD14
  MOV      RD14, RD10
  MOV      #0, RD11

L008009A3:
  CMP/GE   #128, RD11
  BT       L008009A5
  SHAD     RD11, #1, RD14
  MOV      RD14, RD13
  MOV.Q    @(R15, 360), RQ31
  MOV.W    @(RQ31, RD13), RD9
  SHAD     RD11, #1, RD14
  ADD      RD14, #1, RD13
  EXTS.L   RD13
  MOV.W    @(RQ31, RD13), RD30
  ADD      RD9, RD30, RD14
  SHAD     RD14, #-1, RD13
  MOV      #368, RQ29
  ADD      R15, RQ29
  EXTS.L   RD11
  MOV.L    RD13, @(RQ29, RD11)
  SUB      RD9, RD30, RD14
  SHAD     RD14, #-1, RD13
  MOV      #880, RQ28
  ADD      R15, RQ28
  EXTS.L   RD11
  MOV.L    RD13, @(RQ28, RD11)
  MOV.L    RD30, @(R15, 208)

L008009A4:
  ADD      #1, RD11
  BRA      L008009A3

L008009A5:
  MOV      #0, RQ27
  EXTS.L   RQ27, RQ27
  MOV      RQ27, RQ26
  MOV      #0, RD11
  MOV.Q    RQ27, @(R15, 192)
  MOV.Q    RQ26, @(R15, 304)

L008009A6:
  CMP/GE   #32, RD11
  BT       L008009A8
  SHAD     RD11, #2, RD14
  MOV      RD14, RD13
  MOV      #880, RQ25
  ADD      R15, RQ25
  MOV.L    @(RQ25, RD13), RD14
  SHAD     RD11, #2, RD13
  ADD      RD13, #1, RD12
  EXTS.L   RD12
  MOV.L    @(RQ25, RD12), RD13
  ADD      RD14, RD13, RD12
  SHAD     RD11, #2, RD14
  ADD      RD14, #2, RD13
  EXTS.L   RD13
  MOV.L    @(RQ25, RD13), RD14
  ADD      RD12, RD14, RD13
  SHAD     RD11, #2, RD14
  ADD      RD14, #3, RD12
  EXTS.L   RD12
  MOV.L    @(RQ25, RD12), RD14
  ADD      RD13, RD14, RD12
  SHAD     RD12, #-2, RD9
  MOV      #1392, RQ26
  ADD      R15, RQ26
  EXTS.L   RD11
  MOV.L    RD9, @(RQ26, RD11)
  MUL.L    RD9, RD9, RD14
  MOV.Q    @(R15, 304), RQ27
  EXTS.L   RD14, RQ28
  ADD      RQ27, RQ28, RQ29
  MOV.Q    RQ29, @(R15, 304)
  MOV.Q    RQ28, @(R15, 176)
  MOV.Q    RQ27, @(R15, 192)

L008009A7:
  ADD      #1, RD11
  BRA      L008009A6

L008009A8:
  MOV      #0, RD11

L008009A9:
  CMP/GE   #32, RD11
  BT       L008009AB
  SHAD     RD11, #2, RD14
  MOV      RD14, RD13
  MOV      #368, RQ7
  ADD      R15, RQ7
  MOV.L    @(RQ7, RD13), RD14
  SHAD     RD11, #2, RD13
  ADD      RD13, #1, RD12
  EXTS.L   RD12
  MOV.L    @(RQ7, RD12), RD13
  ADD      RD14, RD13, RD12
  SHAD     RD11, #2, RD14
  ADD      RD14, #2, RD13
  EXTS.L   RD13
  MOV.L    @(RQ7, RD13), RD14
  ADD      RD12, RD14, RD13
  SHAD     RD11, #2, RD14
  ADD      RD14, #3, RD12
  EXTS.L   RD12
  MOV.L    @(RQ7, RD12), RD14
  ADD      RD13, RD14, RD12
  SHAD     RD12, #-2, RD9
  MOV      #1520, RQ6
  ADD      R15, RQ6
  EXTS.L   RD11
  MOV.L    RD9, @(RQ6, RD11)

L008009AA:
  ADD      #1, RD11
  BRA      L008009A9

L008009AB:
  MOV      #0, RD25
  MOV.L    RD25, @(R15, 216)
  CMP/EQ   #0, RD25
  BT       L008009AC
  TST      #65536, RD10
  BT       L008009AD
  MOV      #0, RD10

L008009AD:
  CMP/EQ   RD10
  BT       L008009AE
  MOV      #1392, RQ25
  ADD      R15, RQ25
  MOV.L    @RQ25, RD26
  MOV.Q    @(R15, 360), RQ27
  MOV.W    @RQ27, RD14
  MOV.W    @(RQ27, 2), RD13
  ADD      RD14, RD13, RD12
  SHAD     RD12, #-1, RD14
  SUB      RD14, RD26, RD28
  MOV.L    RD28, @(R15, 240)
  MOV.L    RD26, @(R15, 236)
  MOV      RD28, RD4
  BSR      Sblkau_StepForSamp
  MOV      RD2, RD29
  MOV.L    @(RQ25, 4), RD14
  MOV.L    @RQ25, RD13
  SUB      RD14, RD13, RD12
  MOV.L    RD29, @(R15, 232)
  MOV      RD12, RD4
  BSR      Sblkau_StepForSamp
  MOV      RD2, RD30
  MOV.L    RD30, @(R15, 228)
  MOV      RD28, RD4
  BSR      Sblkau_SampToPred9
  MOV      RD2, RD14
  SHAD     RD14, #7, RD13
  SHAD     RD29, #1, RD14
  EXTU.W   RD13, RD12
  EXTU.W   RD14, RD13
  OR       RD12, RD13, RD31
  MOV.L    RD31, @(R15, 248)
  MOV      RD26, RD4
  BSR      Sblkau_SampToPred9
  MOV      RD2, RD14
  SHAD     RD14, #7, RD13
  SHAD     RD30, #1, RD14
  EXTU.W   RD13, RD12
  EXTU.W   RD14, RD13
  OR       RD12, RD13, RD8
  SHAD     RD8, #16, RD14
  OR       RD14, RD31, RD10
  MOV.L    RD8, @(R15, 244)

L008009AE:
  MOV      #0, RD11

L008009AF:
  CMP/GE   #32, RD11
  BT       L008009B1
  SHAD     RD11, #2, RD14
  MOV      RD14, RD13
  MOV      #368, RQ5
  ADD      R15, RQ5
  MOV.L    @(RQ5, RD13), RD14
  SHAD     RD11, #2, RD13
  ADD      RD13, #1, RD12
  EXTS.L   RD12
  MOV.L    @(RQ5, RD12), RD13
  ADD      RD14, RD13, RD12
  SHAD     RD11, #2, RD14
  ADD      RD14, #2, RD13
  EXTS.L   RD13
  MOV.L    @(RQ5, RD13), RD14
  ADD      RD12, RD14, RD13
  SHAD     RD11, #2, RD14
  ADD      RD14, #3, RD12
  EXTS.L   RD12
  MOV.L    @(RQ5, RD12), RD14
  ADD      RD13, RD14, RD12
  SHAD     RD12, #-2, RD9
  MOV      #1392, RQ4
  ADD      R15, RQ4
  EXTS.L   RD11
  MOV.L    RD9, @(RQ4, RD11)

L008009B0:
  ADD      #1, RD11
  BRA      L008009AF

L008009B1:
  MOV      #1, RD3
  MOV      sblkau_stat, RQ2
  MOV.L    @(RQ2, 4), RD14
  ADD      RD14, #1, RD13
  MOV.L    RD13, @(RQ2, 4)
  MOV.L    RD3, @(R15, 216)
  BRA      L008009B2

L008009AC:
  TST      #65536, RD10
  BT       L008009B3
  MOV      #0, RD10

L008009B3:
  CMP/EQ   RD10
  BT       L008009B4
  MOV.Q    @(R15, 360), RQ25
  MOV.W    @RQ25, RD14
  MOV.W    @(RQ25, 2), RD13
  ADD      RD14, RD13, RD12
  SHAD     RD12, #-1, RD26
  MOV.W    @RQ25, RD14
  MOV.W    @(RQ25, 2), RD13
  SUB      RD14, RD13, RD12
  SHAD     RD12, #-1, RD27
  MOV      #368, RQ28
  ADD      R15, RQ28
  MOV.L    @(RQ28, 4), RD14
  MOV.L    @RQ28, RD13
  SUB      RD14, RD13, RD12
  MOV.L    RD27, @(R15, 236)
  MOV.L    RD26, @(R15, 240)
  MOV      RD12, RD4
  BSR      Sblkau_StepForSamp
  MOV      RD2, RD29
  MOV      #1392, RQ30
  ADD      R15, RQ30
  MOV.L    @(RQ30, 4), RD14
  MOV.L    @RQ30, RD13
  SUB      RD14, RD13, RD12
  MOV.L    RD29, @(R15, 232)
  MOV      RD12, RD4
  BSR      Sblkau_StepForSamp
  MOV      RD2, RD31
  MOV.L    RD31, @(R15, 228)
  MOV      RD26, RD4
  BSR      Sblkau_SampToPred9
  MOV      RD2, RD14
  SHAD     RD14, #7, RD13
  SHAD     RD29, #1, RD14
  EXTU.W   RD13, RD12
  EXTU.W   RD14, RD13
  OR       RD12, RD13, RD8
  MOV.L    RD8, @(R15, 248)
  MOV      RD27, RD4
  BSR      Sblkau_SampToPred9
  MOV      RD2, RD14
  SHAD     RD14, #7, RD13
  SHAD     RD31, #1, RD14
  EXTU.W   RD13, RD12
  EXTU.W   RD14, RD13
  OR       RD12, RD13, RD8
  SHAD     RD8, #16, RD14
  MOV.L    @(R15, 248), RD31
  OR       RD14, RD31, RD10
  MOV.L    RD8, @(R15, 244)

L008009B4:
  MOV      sblkau_stat, RQ23
  MOV.L    @RQ23, RD14
  ADD      RD14, #1, RD13
  MOV.L    RD13, @RQ23

L008009B2:
  MOV      #0, RD22
  MOV      #0, RD21
  MOV      #0, RD20
  MOV.L    @(R15, 236), RD18
  MOV      RD18, RD19
  MOV.L    @(R15, 228), RD16
  MOV      RD16, RD17
  MOV      #0, RD11
  MOV.L    RD22, @(R15, 260)
  MOV.L    RD21, @(R15, 256)
  MOV.L    RD20, @(R15, 252)
  MOV.L    RD19, @(R15, 224)
  MOV.L    RD17, @(R15, 220)

L008009B5:
  CMP/GE   #32, RD11
  BT       L008009B7
  MOV      #1392, RQ25
  ADD      R15, RQ25
  EXTS.L   RD11
  MOV.L    @(RQ25, RD11), RD14
  MOV.L    @(R15, 220), RD4
  MOV.L    @(R15, 224), RD5
  MOV      RD14, RD6
  BSR      Sblkau_SampForDelta
  MOV      RD2, RD26
  MOV.L    RD26, @(R15, 212)
  MOV.L    @(R15, 220), RD4
  MOV      RD26, RD5
  BSR      Sblkau_DeltaForSamp
  MOV      RD2, RD14
  MOV.L    @(R15, 224), RD27
  ADD      RD14, RD27
  MOV.L    RD27, @(R15, 224)
  MOV.L    @(R15, 220), RD4
  MOV      RD26, RD5
  BSR      Sblkau_AdjustStepForSamp
  MOV      RD2, RD28
  EXTS.W   RD27, RD14
  EXTS.W   RD14, RD13
  MOV      RD27, RD12
  MOV.L    RD28, @(R15, 220)
  CMP/EQ   RD12, RD13
  BT       L008009B8
  BRK     

L008009B8:
  MOV      #1392, RQ16
  ADD      R15, RQ16
  MOV.L    @(R15, 224), RD17
  EXTS.L   RD11
  MOV.L    RD17, @(RQ16, RD11)
  MOV.L    @(R15, 256), RD18
  SHLD     RD18, #29, RD14
  MOV.L    @(R15, 260), RD19
  SHLD     RD19, #-3, RD13
  OR       RD14, RD13, RD19
  MOV.L    @(R15, 252), RD20
  SHLD     RD20, #29, RD14
  SHLD     RD18, #-3, RD13
  OR       RD14, RD13, RD18
  MOV.L    @(R15, 212), RD21
  SHAD     RD21, #29, RD14
  SHLD     RD20, #-3, RD13
  OR       RD14, RD13, RD20
  MOV.L    RD20, @(R15, 252)
  MOV.L    RD19, @(R15, 260)
  MOV.L    RD18, @(R15, 256)

L008009B6:
  ADD      #1, RD11
  BRA      L008009B5

L008009B7:
  MOV.L    @(R15, 224), RD17
  MOV      RD17, RD16
  MOV.L    @(R15, 220), RD19
  MOV      RD19, RD18
  MOV.L    RD18, @(R15, 228)
  MOV.L    RD16, @(R15, 236)
  MOV.L    @(R15, 216), RD20
  CMP/EQ   #0, RD20
  BT       L008009B9
  MOV      #0, RD11

L008009BA:
  CMP/GE   #128, RD11
  BT       L008009BC
  SHAD     RD11, #-2, RD14
  MOV      #1392, RQ16
  ADD      R15, RQ16
  MOV.L    @(RQ16, RD14), RD17
  SHAD     RD11, #1, RD14
  MOV      RD14, RD13
  MOV.Q    @(R15, 360), RQ18
  MOV.W    @(RQ18, RD13), RD14
  SHAD     RD11, #1, RD13
  ADD      RD13, #1, RD12
  EXTS.L   RD12
  MOV.W    @(RQ18, RD12), RD13
  ADD      RD14, RD13, RD12
  SHAD     RD12, #-1, RD9
  SUB      RD9, RD17, RD14
  MOV      #368, RQ19
  ADD      R15, RQ19
  EXTS.L   RD11
  MOV.L    RD14, @(RQ19, RD11)
  MOV.L    RD17, @(R15, 208)

L008009BB:
  ADD      #1, RD11
  BRA      L008009BA

L008009BC:

L008009B9:
  MOV      #0, RQ16
  EXTS.L   RQ16, RQ16
  MOV      RQ16, RQ17
  MOV      #0, RQ16
  EXTS.L   RQ16, RQ16
  MOV      RQ16, RQ18
  MOV      #0, RQ16
  EXTS.L   RQ16, RQ16
  MOV      RQ16, RQ19
  MOV      #0, RQ16
  EXTS.L   RQ16, RQ16
  MOV      RQ16, RQ20
  MOV      #0, RQ16
  EXTS.L   RQ16, RQ16
  MOV      RQ16, RQ21
  MOV      #0, RQ16
  EXTS.L   RQ16, RQ16
  MOV      RQ16, RQ22
  MOV.L    @(R15, 240), RD2
  MOV      RD2, RD23
  MOV.L    @(R15, 232), RD4
  MOV      RD4, RD3
  MOV      #0, RD11
  MOV.L    RD3, @(R15, 220)
  MOV.L    RD23, @(R15, 224)
  MOV.Q    RQ22, @(R15, 264)
  MOV.Q    RQ21, @(R15, 272)
  MOV.Q    RQ20, @(R15, 280)
  MOV.Q    RQ19, @(R15, 288)
  MOV.Q    RQ18, @(R15, 296)
  MOV.Q    RQ17, @(R15, 304)
  MOV.Q    RQ16, @(R15, 192)

L008009BD:
  CMP/GE   #128, RD11
  BT       L008009BF
  MOV      #368, RQ25
  ADD      R15, RQ25
  EXTS.L   RD11
  MOV.L    @(RQ25, RD11), RD14
  MOV.L    @(R15, 220), RD4
  MOV.L    @(R15, 224), RD5
  MOV      RD14, RD6
  BSR      Sblkau_SampForDelta
  MOV      RD2, RD26
  MOV.L    RD26, @(R15, 212)
  MOV.L    @(R15, 220), RD4
  MOV      RD26, RD5
  BSR      Sblkau_DeltaForSamp
  MOV      RD2, RD14
  MOV.L    @(R15, 224), RD27
  ADD      RD14, RD27
  MOV.L    RD27, @(R15, 224)
  MOV.L    @(R15, 220), RD4
  MOV      RD26, RD5
  BSR      Sblkau_AdjustStepForSamp
  MOV      RD2, RD28
  EXTS.W   RD27, RD14
  EXTS.W   RD14, RD13
  MOV      RD27, RD12
  MOV.L    RD28, @(R15, 220)
  CMP/EQ   RD12, RD13
  BT       L008009C0
  BRK     

L008009C0:
  MOV.L    @(R15, 212), RD16
  TST      #-8, RD16
  BT       L008009C1
  BRK     

L008009C1:
  MOV.Q    @(R15, 296), RQ16
  MOV      #61, RD17
  SHLD.Q   RQ16, RD17, RQ16
  MOV.Q    @(R15, 304), RQ18
  SHLD.Q   RQ18, #-3, RQ19
  OR       RQ16, RQ19, RQ18
  MOV.Q    @(R15, 288), RQ16
  SHLD.Q   RQ16, RD17, RQ16
  MOV.Q    @(R15, 296), RQ20
  SHLD.Q   RQ20, #-3, RQ19
  OR       RQ16, RQ19, RQ20
  MOV.Q    @(R15, 280), RQ16
  SHLD.Q   RQ16, RD17, RQ16
  MOV.Q    @(R15, 288), RQ21
  SHLD.Q   RQ21, #-3, RQ19
  OR       RQ16, RQ19, RQ21
  MOV.Q    @(R15, 272), RQ16
  SHLD.Q   RQ16, RD17, RQ16
  MOV.Q    @(R15, 280), RQ22
  SHLD.Q   RQ22, #-3, RQ19
  OR       RQ16, RQ19, RQ22
  MOV.Q    @(R15, 264), RQ16
  SHLD.Q   RQ16, RD17, RQ16
  MOV.Q    @(R15, 272), RQ23
  SHLD.Q   RQ23, #-3, RQ19
  OR       RQ16, RQ19, RQ23
  MOV.L    @(R15, 212), RD16
  EXTS.L   RQ16, RQ16
  MOV      RQ16, RQ19
  SHLD.Q   RQ19, RD17, RQ19
  MOV.Q    @(R15, 264), RQ2
  SHLD.Q   RQ2, #-3, RQ16
  OR       RQ19, RQ16, RQ2
  MOV.Q    RQ2, @(R15, 264)
  MOV.Q    RQ23, @(R15, 272)
  MOV.Q    RQ22, @(R15, 280)
  MOV.Q    RQ21, @(R15, 288)
  MOV.Q    RQ20, @(R15, 296)
  MOV.Q    RQ19, @(R15, 176)
  MOV.Q    RQ18, @(R15, 304)
  MOV.Q    RQ16, @(R15, 192)

L008009BE:
  ADD      #1, RD11
  BRA      L008009BD

L008009BF:
  MOV.L    @(R15, 224), RD17
  MOV      RD17, RD16
  MOV.L    @(R15, 220), RD19
  MOV      RD19, RD18
  MOV.L    RD18, @(R15, 232)
  MOV.L    RD16, @(R15, 240)
  MOV.L    @(R15, 216), RD20
  CMP/EQ   #0, RD20
  BT       L008009C2
  OR       #65536, RD10
  BRA      L008009C3

L008009C2:
  AND      #-65537, RD10

L008009C3:
  MOV.Q    @(R15, 352), RQ25
  MOVU.L   RD10, @RQ25
  MOV.L    @(R15, 260), RD26
  MOVU.L   RD26, @(RQ25, 4)
  MOV.L    @(R15, 256), RD27
  MOVU.L   RD27, @(RQ25, 8)
  MOV.L    @(R15, 252), RD28
  MOVU.L   RD28, @(RQ25, 12)
  MOV.L    @(R15, 304), RD14
  EXTU.L   RD14, RD14
  MOVU.L   RD14, @(RQ25, 16)
  MOV.Q    @(R15, 304), RQ29
  MOV      #32, RD30
  SHLR.Q   RQ29, RD30, RQ29
  EXTU.L   RQ29, RD14
  MOVU.L   RD14, @(RQ25, 20)
  MOV.L    @(R15, 296), RD14
  EXTU.L   RD14, RD14
  MOVU.L   RD14, @(RQ25, 24)
  MOV.Q    @(R15, 296), RQ29
  SHLR.Q   RQ29, RD30, RQ29
  EXTU.L   RQ29, RD14
  MOVU.L   RD14, @(RQ25, 28)
  MOV.L    @(R15, 288), RD14
  EXTU.L   RD14, RD14
  MOVU.L   RD14, @(RQ25, 32)
  MOV.Q    @(R15, 288), RQ29
  SHLR.Q   RQ29, RD30, RQ29
  EXTU.L   RQ29, RD14
  MOVU.L   RD14, @(RQ25, 36)
  MOV.L    @(R15, 280), RD14
  EXTU.L   RD14, RD14
  MOVU.L   RD14, @(RQ25, 40)
  MOV.Q    @(R15, 280), RQ29
  SHLR.Q   RQ29, RD30, RQ29
  EXTU.L   RQ29, RD14
  MOVU.L   RD14, @(RQ25, 44)
  MOV.L    @(R15, 272), RD14
  EXTU.L   RD14, RD14
  MOVU.L   RD14, @(RQ25, 48)
  MOV.Q    @(R15, 272), RQ29
  SHLR.Q   RQ29, RD30, RQ29
  EXTU.L   RQ29, RD14
  MOVU.L   RD14, @(RQ25, 52)
  MOV.L    @(R15, 264), RD14
  EXTU.L   RD14, RD14
  MOVU.L   RD14, @(RQ25, 56)
  MOV.Q    @(R15, 264), RQ29
  SHLR.Q   RQ29, RD30, RQ29
  EXTU.L   RQ29, RD14
  MOVU.L   RD14, @(RQ25, 60)
  MOV.Q    RQ29, @(R15, 192)
  MOV.L    @(R15, 240), RD4
  BSR      Sblkau_SampToPred9
  MOV      RD2, RD14
  SHAD     RD14, #7, RD13
  MOV.L    @(R15, 232), RD31
  SHAD     RD31, #1, RD14
  EXTU.W   RD13, RD12
  EXTU.W   RD14, RD13
  OR       RD12, RD13, RD8
  MOV.L    RD8, @(R15, 248)
  MOV.L    @(R15, 236), RD4
  BSR      Sblkau_SampToPred9
  MOV      RD2, RD14
  SHAD     RD14, #7, RD13
  MOV.L    @(R15, 228), RD26
  SHAD     RD26, #1, RD14
  EXTU.W   RD13, RD12
  EXTU.W   RD14, RD13
  OR       RD12, RD13, RD25
  SHAD     RD25, #16, RD14
  OR       RD14, RD8, RD10
  MOV.L    RD25, @(R15, 244)
  MOV.L    @(R15, 216), RD27
  CMP/EQ   #0, RD27
  BT       L008009C4
  OR       #65536, RD10
  BRA      L008009C5

L008009C4:
  AND      #-65537, RD10

L008009C5:
  MOV.Q    @(R15, 344), RQ16
  MOVU.L   RD10, @RQ16

L00E00528:
  ADD      #1672, R15
  BRA      L00C00194

SNDDMA_GetDevDMAPos:
  ADD      #-144, R15
  MOV.L    TU0ADE00B8/wbufrov, RD2

L00E00529:
  ADD      #144, R15
  RTS     

SNDDMA_GetDMAPos:
  ADD      #-144, R15
  MOV.L    dma_pos, RD2

L00E0052A:
  ADD      #144, R15
  RTS     

I_SMus_Tick:
  PUSH     LR
  BSR      L00C0046F
  ADD      #-96, R15
  MOV.L    i_smus_tt, RD14
  CMP/PL   RD14
  BT       L00800A42
  MOV.L    i_smus_tt, RD13
  ADD      #-1, RD13
  MOV.L    RD13, i_smus_tt
  MOV      #1, RD2
  BRA      L00E0052B

L00800A42:
  MOV      #0, RD12
  MOV      #5, RD11
  MOV.L    RD12, i_smus_tt
  MOV.L    RD11, @(R15, 12)

L00800A43:
  MOV.L    i_smus_tt, RD10
  CMP/EQ   RD10
  BT       L00800A44
  MOV.L    @(R15, 12), RD8
  MOV      RD8, RD9
  ADD      #-1, RD8
  MOV.L    RD9, @(R15, 8)
  MOV.L    RD8, @(R15, 12)
  CMP/PL   RD9
  BT       L00800A44
  BRA      L00800A45

L00800A45:
  BSR      I_SMus_ParseEvent
  BRA      L00800A43

L00800A44:
  MOV.L    @(R15, 12), RD7
  MOV      #0, RD6
  CMP/GT   RD6, RD7
  MOVT     RD5
  MOV.L    RD5, @(R15, 8)
  MOV      RD5, RD2

L00E0052B:
  ADD      #96, R15
  BRA      L00C00470

smus_timer_irq:
  PUSH     LR
  BSR      L00C001D8
  ADD      #-128, R15
  MOV.L    smus_irq_tt, RD14
  CMP/PL   RD14
  BT       L008009D4
  MOV.L    smus_irq_tt, RD13
  ADD      #-1, RD13
  MOV.L    RD13, smus_irq_tt
  MOV      #0, RD2
  BRA      L00E0052C

L008009D4:
  BSR      I_SMus_Tick
  MOV      RD2, RD12
  MOV.L    RD12, @(R15, 12)
  CMP/PL   RD12
  BT       L008009D5
  MOV      #7, RD12
  MOV.L    RD12, smus_irq_tt

L008009D5:
  MOV      #0, RD2

L00E0052C:
  ADD      #128, R15
  BRA      L00C001D9

L00C004F7:
  PUSH     R31
  PUSH     R30
  PUSH     R29
  PUSH     R28
  PUSH     R27
  PUSH     R26
  PUSH     R25
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  PUSH     R9
  PUSH     R8
  PUSH     FR15
  PUSH     FR14
  PUSH     FR13
  PUSH     FR12
  PUSH     FR11
  PUSH     FR10
  PUSH     FR9
  RTS     

SMus_Init:
  PUSH     LR
  BSR      L00C004F7
  ADD      #-112, R15
  MOV.L    TU0ADE0084/smus_isinit, RD12
  CMP/EQ   RD12
  BT       L008009D6
  MOV      #0, RD2
  BRA      L00E0052D

L008009D6:
  MOV      #1, RD11
  MOV      smus_timer_irq, RQ9
  MOV      RQ9, RQ10
  MOV.L    RD11, TU0ADE0084/smus_isinit
  MOV.Q    RQ10, @(R15, 56)
  MOV      RQ10, RQ4
  BSR      irq_addTimerIrq
  MOV      #0xA0081800, RQ8
  MOV      #0x0000A008, RQ8
  MOV      #0, RD14
  MOV.Q    RQ8, smus_regs

L008009D7:
  CMP/GE   #128, RD14
  BT       L008009D9
  SHAD     RD14, #-1, RD13
  MOV      #63, RD30
  SUB      RD30, RD13, RD31
  MOV      smus_noteatt, RQ29
  EXTS.L   RD14
  MOVU.B   RD31, @(RQ29, RD14)
  SUB      RD14, #69, RD13
  EXTS.L   RD13
  MOV      RD13, R0
  FLDCI    FR15
  MOV      #0x55555558, DR14
  FMOV     DR14, FR13
  FMOV     FR15, FR12
  FMUL     FR13, FR12
  MOV      #2, RD28
  EXTS.L   RD28
  MOV      RD28, R0
  FLDCI    DR11
  FMOV     FR12, DR10
  FMOV.S   FR15, @(R15, 40)
  FMOV.S   FR13, @(R15, 36)
  FMOV.D   DR11, @(R15, 16)
  FMOV.D   DR10, @(R15, 8)
  FMOV     DR11, DR4
  FMOV     DR10, DR5
  BSR      pow
  FMOV     DR2, DR9
  FMOV     DR9, FR15
  MOV      #0, DR14
  FMOV     DR14, FR13
  FMOV     FR15, FR14
  FMUL     FR13, FR14
  FMOV     FR14, FR12
  MOV      #0x3783126F, FR11
  FMUL     FR11, FR12
  FMOV     FR12, FR15
  MOV      #0x49800000, FR11
  FMUL     FR11, FR15
  FMOV     FR15, FR11
  MOV      #0x3F000000, FR15
  FMOV     FR11, FR13
  FADD     FR15, FR13
  FSTCI    FR13
  MOV      R0, RD27
  FMOV.S   FR15, @(R15, 40)
  FMOV.S   FR14, @(R15, 76)
  FMOV.S   FR13, @(R15, 36)
  FMOV.S   FR12, @(R15, 72)
  FMOV.D   DR9, @(R15, 24)
  MOV.L    RD27, @(R15, 68)
  MOV      #0x00100000, RD26
  MOV      #16, RD26
  CMP/GE   RD26, RD27
  BT       L008009DA
  MOV      #0x000FFFFF, RD25
  MOV      #15, RD25
  MOV.L    RD25, @(R15, 68)

L008009DA:
  MOV.L    @(R15, 68), RD7
  MOV      #1, RD6
  CMP/GE   RD6, RD7
  BT       L008009DB
  MOV      #1, RD25
  MOV.L    RD25, @(R15, 68)

L008009DB:
  MOV      smus_notediv, RQ5
  MOV.L    @(R15, 68), RD4
  EXTS.L   RD14
  MOV.L    RD4, @(RQ5, RD14)

L008009D8:
  ADD      #1, RD14
  BRA      L008009D7

L008009D9:
  MOV      #0, RD2

L00E0052D:
  ADD      #112, R15

L00C004F8:
  POP      FR9
  POP      FR10
  POP      FR11
  POP      FR12
  POP      FR13
  POP      FR14
  POP      FR15
  POP      RQ8
  POP      RQ9
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  POP      RQ25
  POP      RQ26
  POP      RQ27
  POP      RQ28
  POP      RQ29
  POP      RQ30
  POP      RQ31
  RET     

SMus_SetFmRegisterData:
  PUSH     LR
  BSR      L00C00198
  ADD      #-120, R15
  MOV      RD4, RD12
  MOV.L    RD5, @(R15, 24)
  MOV.L    RD6, @(R15, 20)
  SHAD     RD12, #4, RD14
  MOV.L    @(R15, 24), RD11
  ADD      RD14, RD11, RD13
  MOV      smus_fmregdata, RQ10
  MOV.L    @(R15, 20), RD9
  EXTS.L   RD13
  MOVU.L   RD9, @(RQ10, RD13)
  MOV      #0, RD2

L00E0052E:
  ADD      #120, R15
  BRA      L00C0019A

SMus_SilenceAll:
  PUSH     LR
  BSR      L00C002C5
  ADD      #-64, R15
  MOV.Q    smus_regs, RQ11
  MOV      #0, RQ10
  CMPQ/EQ  RQ10, RQ11
  BT       L008009DC
  MOV      #0, RD2
  BRA      L00E0052F

L008009DC:
  MOV      #0, RD14

L008009DD:
  CMP/GE   #16, RD14
  BT       L008009DF
  SHAD     RD14, #2, RD13
  MOV      RD13, RD12
  MOV.Q    smus_regs, RQ9
  MOV      #0, RD8
  MOVU.L   RD8, @(RQ9, RD12)
  SHAD     RD14, #2, RD13
  ADD      RD13, #1, RD12
  EXTS.L   RD12
  MOVU.L   RD8, @(RQ9, RD12)
  SHAD     RD14, #2, RD13
  ADD      RD13, #2, RD12
  EXTS.L   RD12
  MOVU.L   RD8, @(RQ9, RD12)
  SHAD     RD14, #2, RD13
  ADD      RD13, #3, RD12
  EXTS.L   RD12
  MOVU.L   RD8, @(RQ9, RD12)

L008009DE:
  ADD      #1, RD14
  BRA      L008009DD

L008009DF:
  MOV      #0, RD14

L008009E0:
  CMP/GE   #16, RD14
  BT       L008009E2
  MOV      smus_chanvol, RQ31
  MOV      #127, RD30
  EXTS.L   RD14
  MOVU.B   RD30, @(RQ31, RD14)
  MOV      smus_chanpbl, RQ29
  MOV      #128, RD28
  EXTS.L   RD14
  MOVU.B   RD28, @(RQ29, RD14)
  MOV      smus_vnflg, RQ7
  MOV      #0, RD6
  EXTS.L   RD14
  MOVU.B   RD6, @(RQ7, RD14)
  MOV      smus_chanvn, RQ5
  MOV      #255, RD4
  EXTS.L   RD14
  MOVU.B   RD4, @(RQ5, RD14)

L008009E1:
  ADD      #1, RD14
  BRA      L008009E0

L008009E2:
  MOV      #0, RD2

L00E0052F:
  ADD      #64, R15
  BRA      L00C002C6

SMus_NoteOff:
  PUSH     LR
  BSR      L00C00193
  ADD      #-56, R15
  MOV.L    RD4, @(R15, 28)
  MOV.L    RD5, @(R15, 24)
  MOV.L    RD6, @(R15, 20)
  MOV      smus_chanvn, RQ11
  MOV.L    @(R15, 28), RD10
  MOVU.B   @(RQ11, RD10), RD13
  CMP/EQ   #255, RD13
  BT       L008009E3
  MOV      #0, RD2
  BRA      L00E00530

L008009E3:
  MOV      smus_channt, RQ9
  MOV.L    @(R15, 28), RD8
  MOVU.B   @(RQ9, RD8), RD14
  MOV.L    @(R15, 24), RD31
  CMP/EQ   RD31, RD14
  BT       L008009E4
  MOV      #0, RD2
  BRA      L00E00530

L008009E4:
  MOV      smus_chanvn, RQ30
  MOV.L    @(R15, 28), RD29
  MOV      #255, RD28
  EXTS.L   RD29
  MOVU.B   RD28, @(RQ30, RD29)
  TST      #16, RD13
  BT       L008009E5
  AND      RD13, #7, RD14
  MOV      RD14, RD12
  AND      RD13, #7, RD14
  OR       RD14, #8, RD27
  MOV.L    RD27, @(R15, 16)
  BRA      L008009E6

L008009E5:
  AND      RD13, #15, RD12
  MOV      RD12, RD26
  MOV.L    RD26, @(R15, 16)

L008009E6:
  TST      #16, RD13
  BT       L008009E7
  SHAD     RD12, #2, RD14
  MOV      RD14, RD25
  MOV.Q    smus_regs, RQ7
  MOV      #0, RD6
  EXTS.L   RD25
  MOVU.L   RD6, @(RQ7, RD25)
  MOV.L    @(R15, 16), RD5
  SHAD     RD5, #2, RD14
  MOV      RD14, RD25
  EXTS.L   RD25
  MOVU.L   RD6, @(RQ7, RD25)
  MOV      smus_vnflg, RQ4
  EXTS.L   RD12
  MOVU.B   RD6, @(RQ4, RD12)
  MOVU.B   RD6, @(RQ4, RD5)
  MOV.L    RD25, @(R15, 8)
  BRA      L008009E8

L008009E7:
  SHAD     RD12, #2, RD14
  MOV      RD14, RD3
  MOV.Q    smus_regs, RQ2
  MOV      #0, RD23
  MOVU.L   RD23, @(RQ2, RD3)
  MOV      smus_vnflg, RQ22
  EXTS.L   RD12
  MOVU.B   RD23, @(RQ22, RD12)
  MOV.L    RD3, @(R15, 8)

L008009E8:
  MOV      #0, RD2

L00E00530:
  ADD      #56, R15
  BRA      L00C00194

SMus_FindFreeVoice:
  PUSH     LR
  BSR      L00C002C5
  ADD      #-64, R15
  MOV.L    RD4, @(R15, 12)
  MOV.L    @(R15, 12), RD12
  TST      #1, RD12
  BT       L008009E9
  MOV      #0, RD14

L008009EA:
  MOV      #5, RD11
  CMP/GE   RD11, RD14
  BT       L008009EC
  MOV      smus_vnflg, RQ10
  EXTS.L   RD14
  MOVU.B   @(RQ10, RD14), RD13
  CMP/EQ   RD13
  BT       L008009ED
  ADD      RD14, #8, RD13
  MOV      smus_vnflg, RQ9
  EXTS.L   RD13
  MOVU.B   @(RQ9, RD13), RD8
  MOV.L    RD8, @(R15, 4)
  CMP/EQ   RD8
  BT       L008009ED
  BRA      L008009EE

L008009EE:
  OR       RD14, #16, RD13
  MOV      RD13, RD2
  BRA      L00E00531

L008009ED:

L008009EB:
  ADD      #1, RD14
  BRA      L008009EA

L008009EC:

L008009E9:
  MOV      #0, RD14

L008009F0:
  MOV      #5, RD31
  CMP/GE   RD31, RD14
  BT       L008009F2
  MOV      smus_vnflg, RQ30
  EXTS.L   RD14
  MOVU.B   @(RQ30, RD14), RD13
  CMP/EQ   RD13
  BT       L008009F3
  MOV      RD14, RD2
  BRA      L00E00531

L008009F3:

L008009F1:
  ADD      #1, RD14
  BRA      L008009F0

L008009F2:
  MOV      #8, RD14

L008009F4:
  MOV      #13, RD29
  CMP/GE   RD29, RD14
  BT       L008009F6
  MOV      smus_vnflg, RQ28
  EXTS.L   RD14
  MOVU.B   @(RQ28, RD14), RD13
  CMP/EQ   RD13
  BT       L008009F7
  MOV      RD14, RD2
  BRA      L00E00531

L008009F7:

L008009F5:
  ADD      #1, RD14
  BRA      L008009F4

L008009F6:
  MOV      #-1, RD2

L00E00531:
  ADD      #64, R15
  BRA      L00C002C6

SMus_NoteOn:
  PUSH     LR
  BSR      L00C00193
  ADD      #-152, R15
  MOV.L    RD4, @(R15, 124)
  MOV.L    RD5, @(R15, 120)
  MOV.L    RD6, @(R15, 116)
  MOV.L    @(R15, 120), RD31
  MOV      RD31, RD9
  MOV      smus_chanprg, RQ30
  MOV.L    @(R15, 124), RD29
  EXTS.L   RD29
  MOVU.B   @(RQ30, RD29), RD28
  MOV.L    RD28, @(R15, 84)
  CMP/EQ   #8, RD29
  BT       L008009F8
  MOV.L    @(R15, 84), RD27
  ADD      #128, RD27
  MOV.L    RD27, @(R15, 84)

L008009F8:
  MOV.L    @(R15, 84), RD26
  SHAD     RD26, #4, RD14
  MOV      smus_fmregdata, RQ25
  LEA.L    @(RQ25, RD14), RQ7
  MOV      RQ7, RQ8
  MOVU.L   @(RQ7, 16), RD14
  SHLD     RD14, #-16, RD13
  EXTS.W   RD13, RD6
  MOVU.B   @(RQ8, 7), RD14
  EXTU.B   RD14, RD13
  MOV      #255, RD14
  AND      RD13, RD14, RD5
  MOVU.B   @(RQ8, 8), RD14
  EXTU.B   RD14, RD13
  MOV      #255, RD14
  AND      RD13, RD14, RD4
  MOVU.B   @(RQ8, 9), RD14
  EXTU.B   RD14, RD13
  MOV      #255, RD14
  AND      RD13, RD14, RD3
  MOVU.B   @(RQ8, 10), RD14
  EXTU.B   RD14, RD13
  MOV      #255, RD14
  AND      RD13, RD14, RD2
  MOVU.B   @(RQ8, 14), RD14
  EXTU.B   RD14, RD13
  MOV      #255, RD14
  AND      RD13, RD14, RD23
  MOVU.B   @(RQ8, 15), RD14
  EXTU.B   RD14, RD13
  MOV      #255, RD14
  AND      RD13, RD14, RD22
  MOVU.B   @(RQ8, 16), RD14
  EXTU.B   RD14, RD13
  MOV      #255, RD14
  AND      RD13, RD14, RD21
  MOV      smus_chanvn, RQ20
  MOV.L    @(R15, 124), RD19
  EXTS.L   RD19
  MOVU.B   @(RQ20, RD19), RD10
  MOV.Q    RQ7, @(R15, 104)
  MOV.L    RD6, @(R15, 56)
  MOV.L    RD5, @(R15, 44)
  MOV.L    RD4, @(R15, 36)
  MOV.L    RD3, @(R15, 32)
  MOV.L    RD2, @(R15, 16)
  MOV.L    RD23, @(R15, 40)
  MOV.L    RD22, @(R15, 24)
  MOV.L    RD21, @(R15, 28)
  CMP/EQ   #255, RD10
  BT       L008009F9
  MOV      #-1, RD10

L008009F9:
  CMP/PZ   RD10
  BT       L008009FA
  MOV      #0, RD25
  MOV.L    RD25, @(R15, 52)
  MOV.L    @(R15, 16), RD25
  TST      #1, RD25
  BT       L008009FC
  MOV.L    @(R15, 44), RD18
  MOV.L    @(R15, 40), RD17
  CMP/EQ   RD17, RD18
  BT       L008009FC
  BRA      L008009FB

L008009FC:
  MOV.L    @(R15, 52), RD16
  OR       #1, RD16
  MOV.L    RD16, @(R15, 52)

L008009FB:
  MOV.L    @(R15, 52), RD4
  BSR      SMus_FindFreeVoice
  MOV      RD2, RD10

L008009FA:
  CMP/PZ   RD10
  BT       L008009FE
  MOV      smus_chanvn, RQ16
  MOV.L    @(R15, 124), RD17
  MOV      #255, RD18
  EXTS.L   RD17
  MOVU.B   RD18, @(RQ16, RD17)
  MOV      #0, RD2
  BRA      L00E00532

L008009FE:
  MOV      smus_channt, RQ16
  MOV.L    @(R15, 124), RD17
  EXTS.L   RD17
  MOVU.B   RD9, @(RQ16, RD17)
  TST      #16, RD10
  BT       L008009FF
  MOV      smus_chanvn, RQ16
  MOV.L    @(R15, 124), RD17
  EXTS.L   RD17
  MOVU.B   RD10, @(RQ16, RD17)
  AND      RD10, #7, RD18
  AND      RD10, #7, RD14
  OR       RD14, #8, RD19
  MOV.L    RD19, @(R15, 88)
  MOV.L    RD18, @(R15, 92)
  BRA      L00800A00

L008009FF:
  MOV      smus_chanvn, RQ16
  MOV.L    @(R15, 124), RD17
  EXTS.L   RD17
  MOVU.B   RD10, @(RQ16, RD17)
  AND      RD10, #7, RD18
  MOV      RD18, RD19
  MOV.L    RD19, @(R15, 88)
  MOV.L    RD18, @(R15, 92)

L00800A00:
  MOV.L    @(R15, 56), RD16
  ADD      RD16, RD9
  CMP/PZ   RD9
  BT       L00800A01
  MOV      #0, RD9

L00800A01:
  SUB      RD9, #96, RD14
  SHAD     RD14, #8, RD13
  MOV      RD13, RD25
  MOV      #0x15555556, RD7
  MOV      #5461, RD7
  DMULS    RD7, RD25
  STS      R1, RD7
  MOV      RD7, RD25
  MOV      smus_chanvol, RQ26
  MOV.L    @(R15, 124), RD27
  EXTS.L   RD27
  MOVU.B   @(RQ26, RD27), RD14
  MOV.L    @(R15, 116), RD28
  MUL.L    RD14, RD28, RD13
  SHAD     RD13, #-7, RD29
  MOV.L    snd_MusicVolume, RD30
  MUL.L    RD29, RD30, RD14
  SHAD     RD14, #-4, RD29
  MOV.L    @(R15, 32), RD31
  AND      RD31, #63, RD14
  MOV      #63, RD13
  EXTU.B   RD14, RD11
  SUB      RD13, RD11, RD12
  MOV.L    @(R15, 28), RD26
  AND      RD26, #63, RD14
  MOV      #63, RD13
  EXTU.B   RD14, RD11
  SUB      RD13, RD11, RD28
  MOV      smus_chanpbl, RQ31
  MOVU.B   @(RQ31, RD27), RD30
  MOV.L    RD30, @(R15, 60)
  MOV.L    RD29, @(R15, 64)
  MOV.L    RD28, @(R15, 20)
  MOV.L    RD25, @(R15, 48)
  CMP/EQ   #0, RD30
  BT       L00800A02
  MOV.L    @(R15, 60), RD16
  SHAD     RD16, #-3, RD14
  SUB      RD14, #16, RD16
  MOV.L    RD16, @(R15, 60)

L00800A02:
  MOV.L    @(R15, 120), RD16
  MOV.L    @(R15, 60), RD17
  ADD      RD16, RD17, RD9
  MOV      #0, RD18
  MOV      #127, RD19
  CMP/GT   RD9, RD18
  CSELT    RD18, RD9, RD9
  CMP/GT   RD9, RD19
  CSELT    RD9, RD19, RD9
  MOV      #5, RD20
  MOV.L    RD20, @(R15, 80)
  MOV.L    @(R15, 124), RD21
  CMP/EQ   #8, RD21
  BT       L00800A03
  MOV      #7, RD16
  MOV.L    RD16, @(R15, 80)

L00800A03:
  MOV.L    @(R15, 44), RD16
  MOV.L    @(R15, 40), RD17
  CMP/EQ   RD17, RD16
  BT       L00800A04
  MOV.L    @(R15, 44), RD17
  MOV      RD17, RD16
  MOV.L    RD16, @(R15, 80)

L00800A04:
  MOV.L    @(R15, 16), RD16
  TST      #1, RD16
  BT       L00800A05
  MOV      RD12, RD14
  SHLD     RD14, #-1, RD12
  MOV.L    @(R15, 20), RD16
  MOV.L    @(R15, 64), RD17
  MUL.L    RD16, RD17, RD14
  EXTU.B   RD14, RD13
  SHLD     RD13, #-6, RD16
  MOV.L    @(R15, 16), RD18
  SHAD     RD18, #-1, RD14
  AND      RD14, #7, RD13
  MOV.L    RD16, @(R15, 20)
  MOV      #4, RD19
  CMP/GE   RD19, RD13
  BT       L00800A10
  CMP/EQ   RD13
  BT       L00800A07
  CMP/EQ   #1, RD13
  BT       L00800A08
  CMP/EQ   #2, RD13
  BT       L00800A09
  CMP/EQ   #3, RD13
  BT       L00800A0A

L00800A10:
  CMP/EQ   #4, RD13
  BT       L00800A0B
  CMP/EQ   #5, RD13
  BT       L00800A0C
  CMP/EQ   #6, RD13
  BT       L00800A0D
  CMP/EQ   #7, RD13
  BT       L00800A0E
  BRA      L00800A0F

L00800A07:
  MOV      #0, RD14
  MOV      RD14, RD12
  BRA      L00800A06

L00800A08:
  MOV      RD12, RD14
  SHLD     RD14, #-4, RD12
  BRA      L00800A06

L00800A09:
  MOV      RD12, RD14
  SHLD     RD14, #-3, RD12
  BRA      L00800A06

L00800A0A:
  MOV      RD12, RD14
  SHLD     RD14, #-2, RD12
  BRA      L00800A06

L00800A0B:
  MOV      RD12, RD14
  SHLD     RD14, #-1, RD12
  BRA      L00800A06

L00800A0C:
  BRA      L00800A06

L00800A0D:
  MOV      RD12, RD14
  SHLD     RD14, #1, RD12
  BRA      L00800A06

L00800A0E:
  MOV      RD12, RD14
  SHLD     RD14, #2, RD12
  BRA      L00800A06

L00800A0F:

L00800A06:
  MOV      #0, RD16
  MOV      #127, RD17
  CMP/GT   RD12, RD16
  CSELT    RD16, RD12, RD14
  CMP/GT   RD14, RD17
  CSELT    RD14, RD17, RD14
  EXTU.B   RD14, RD12
  BRA      L00800A11

L00800A05:
  MOV.L    @(R15, 64), RD16
  MUL.L    RD12, RD16, RD14
  EXTU.B   RD14, RD11
  SHLD     RD11, #-6, RD12
  MOV.L    @(R15, 20), RD17
  MUL.L    RD17, RD16, RD14
  EXTU.B   RD14, RD11
  SHLD     RD11, #-6, RD17
  MOV.L    @(R15, 44), RD19
  MOV      RD19, RD18
  MOV.L    RD18, @(R15, 80)
  MOV.L    RD17, @(R15, 20)

L00800A11:
  MOV.L    @(R15, 64), RD14
  EXTU.B   RD14, RD14
  MOV      RD14, RD12
  MOV.L    @(R15, 64), RD14
  EXTU.B   RD14, RD14
  MOV      RD14, RD16
  MOV.L    RD16, @(R15, 20)
  MOV      #0, RD17
  MOV      #127, RD18
  CMP/GT   RD12, RD17
  CSELT    RD17, RD12, RD14
  CMP/GT   RD14, RD18
  CSELT    RD14, RD18, RD14
  EXTU.B   RD14, RD12
  CMP/GT   RD16, RD17
  CSELT    RD17, RD16, RD14
  CMP/GT   RD14, RD18
  CSELT    RD14, RD18, RD14
  EXTU.B   RD14, RD16
  MOV.L    RD16, @(R15, 20)
  TST      #16, RD10
  BT       L00800A12
  MOV      smus_notediv, RQ16
  EXTS.L   RD9
  MOV.L    @(RQ16, RD9), RD17
  MOV      smus_noteatt, RQ18
  MOV.L    @(R15, 20), RD19
  EXTS.L   RD19
  MOVU.B   @(RQ18, RD19), RD20
  MOVU.B   @(RQ18, RD12), RD21
  SHAD     RD20, #22, RD14
  OR       RD17, RD14, RD11
  MOV.L    @(R15, 40), RD22
  AND      RD22, #7, RD14
  SHAD     RD14, #28, RD23
  OR       RD11, RD23, RD2
  SHAD     RD21, #22, RD14
  OR       RD17, RD14, RD11
  MOV.L    @(R15, 44), RD3
  AND      RD3, #7, RD14
  SHAD     RD14, #28, RD23
  OR       RD11, RD23, RD4
  MOV.L    RD4, @(R15, 96)
  MOV.L    RD2, @(R15, 100)
  MOV.L    RD21, @(R15, 68)
  MOV.L    RD20, @(R15, 72)
  MOV.L    RD17, @(R15, 76)
  MOV.L    @(R15, 16), RD5
  TST      #1, RD5
  BT       L00800A13
  MOV.L    @(R15, 100), RD16
  OR       #2097152, RD16
  MOV      smus_vnflg, RQ17
  MOV.L    @(R15, 92), RD18
  EXTS.L   RD18
  MOVU.B   @(RQ17, RD18), RD14
  OR       RD14, #1, RD11
  EXTS.L   RD18
  MOVU.B   RD11, @(RQ17, RD18)
  MOV.L    @(R15, 88), RD19
  MOVU.B   @(RQ17, RD19), RD14
  OR       RD14, #2, RD11
  MOVU.B   RD11, @(RQ17, RD19)
  MOV.L    RD16, @(R15, 100)

L00800A13:
  MOV.L    @(R15, 92), RD16
  SHAD     RD16, #2, RD14
  MOV      RD14, RD11
  MOV.Q    smus_regs, RQ17
  MOV.L    @(R15, 100), RD18
  MOVU.L   RD18, @(RQ17, RD11)
  MOV.L    @(R15, 88), RD19
  SHAD     RD19, #2, RD14
  MOV      RD14, RD11
  MOV.L    @(R15, 96), RD20
  MOVU.L   RD20, @(RQ17, RD11)
  BRA      L00800A14

L00800A12:
  MOV      smus_notediv, RQ16
  EXTS.L   RD9
  MOV.L    @(RQ16, RD9), RD17
  MOV      smus_noteatt, RQ18
  MOV.L    @(R15, 64), RD19
  EXTS.L   RD19
  MOVU.B   @(RQ18, RD19), RD20
  SHAD     RD20, #22, RD14
  OR       RD17, RD14, RD11
  MOV.L    @(R15, 80), RD21
  SHAD     RD21, #28, RD14
  OR       RD11, RD14, RD22
  MOV      smus_vnflg, RQ23
  MOV.L    @(R15, 92), RD2
  MOVU.B   @(RQ23, RD2), RD14
  OR       RD14, #2, RD11
  MOVU.B   RD11, @(RQ23, RD2)
  SHAD     RD2, #2, RD14
  MOV      RD14, RD11
  MOV.Q    smus_regs, RQ3
  MOVU.L   RD22, @(RQ3, RD11)
  MOV.L    RD22, @(R15, 100)
  MOV.L    RD20, @(R15, 72)
  MOV.L    RD17, @(R15, 76)

L00800A14:
  MOV      #0, RD2

L00E00532:
  ADD      #152, R15
  BRA      L00C00194

SMus_PitchBlend:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  ADD      #-128, R15
  MOV.L    RD4, @(R15, 12)
  MOV      RD5, RD13
  SHAD     RD13, #-6, RD14
  MOV      smus_chanpbl, RQ12
  MOV.L    @(R15, 12), RD11
  MOVU.B   RD14, @(RQ12, RD11)
  MOV      #0, RD2

L00E00533:
  ADD      #128, R15
  BRA      L00C001A1

SMus_Controller:
  PUSH     LR
  BSR      L00C002C0
  ADD      #-80, R15
  MOV.L    RD4, @(R15, 12)
  MOV.L    RD5, @(R15, 8)
  MOV.L    RD6, @(R15, 4)
  MOV.L    @(R15, 8), RD14
  CMP/EQ   #7, RD14
  BT       L00800A15
  MOV      smus_chanvol, RQ13
  MOV.L    @(R15, 12), RD12
  MOV.L    @(R15, 4), RD11
  MOVU.B   RD11, @(RQ13, RD12)
  MOV      #1, RD2
  BRA      L00E00534

L00800A15:
  MOV.L    @(R15, 8), RD10
  CMP/EQ   #10, RD10
  BT       L00800A16
  MOV      smus_chanpan, RQ9
  MOV.L    @(R15, 12), RD8
  MOV.L    @(R15, 4), RD31
  MOVU.B   RD31, @(RQ9, RD8)
  MOV      #1, RD2
  BRA      L00E00534

L00800A16:
  MOV.L    @(R15, 8), RD30
  CMP/EQ   #1, RD30
  BT       L00800A17
  MOV      smus_chanmod, RQ7
  MOV.L    @(R15, 12), RD6
  MOV.L    @(R15, 4), RD5
  MOVU.B   RD5, @(RQ7, RD6)
  MOV      #1, RD2
  BRA      L00E00534

L00800A17:
  MOV      L00C004FE, RQ4
  MOV.L    @(R15, 12), RD5
  MOV.L    @(R15, 8), RD6
  MOV.L    @(R15, 4), RD7
  BSR      printf
  MOV      #0, RD2

L00E00534:
  ADD      #80, R15
  BRA      L00C002C3

SMus_ProgramChange:
  PUSH     LR
  BSR      L00C001D8
  ADD      #-128, R15
  MOV      RD4, RD14
  MOV      RD5, RD13
  MOV      L00C00500, RQ4
  MOV      RD14, RD5
  MOV      RD13, RD6
  BSR      printf
  MOV      smus_chanprg, RQ12
  EXTS.L   RD14
  MOVU.B   RD13, @(RQ12, RD14)
  MOV      #0, RD2

L00E00535:
  ADD      #128, R15
  BRA      L00C001D9

I_SMus_ParseVLI:
  PUSH     LR
  BSR      L00C003FF
  ADD      #-104, R15
  MOV.Q    RQ4, @(R15, 24)
  MOV.Q    @(R15, 24), RQ8
  MOV.Q    @RQ8, RQ12
  MOV      RQ12, RQ13
  MOV      RQ13, RQ12
  ADD      #1, RQ13
  MOVU.B   @RQ12, RD14
  MOV      RD14, RD10
  MOV      #0, RD11

L00800A18:
  TST      #128, RD10
  BT       L00800A19
  SHAD     RD11, #7, RD14
  AND      RD10, #127, RD9
  OR       RD14, RD9, RD11
  MOV      RQ13, RQ12
  ADD      #1, RQ13
  MOVU.B   @RQ12, RD14
  MOV      RD14, RD10
  BRA      L00800A18

L00800A19:
  SHAD     RD11, #7, RD14
  AND      RD10, #127, RD9
  OR       RD14, RD9, RD11
  MOV.Q    @(R15, 24), RQ31
  MOV.Q    RQ13, @RQ31
  MOV      RD11, RD2

L00E00536:
  ADD      #104, R15
  BRA      L00C00404

I_SMus_ParseEvent:
  PUSH     LR
  BSR      L00C00193
  ADD      #-72, R15
  MOV.Q    i_smus_cs, RQ10
  MOV      RQ10, RQ11
  MOV.Q    RQ11, @(R15, 40)
  MOV.Q    @(R15, 40), RQ8
  MOV      RQ8, RQ9
  MOV.Q    @(R15, 40), RQ31
  ADD      #1, RQ31
  MOV.Q    RQ31, @(R15, 40)
  MOVU.B   @RQ9, RD30
  MOV      RD30, RD29
  MOV      RD29, RD28
  SHAD     RD28, #-4, RD30
  AND      RD30, #7, RD27
  AND      RD28, #15, RD13
  MOV.Q    @(R15, 40), RQ26
  MOV      RQ26, RQ9
  MOV.Q    @(R15, 40), RQ25
  ADD      #1, RQ25
  MOV.Q    RQ25, @(R15, 40)
  MOVU.B   @RQ9, RD30
  MOV      RD30, RD14
  MOV.Q    RQ9, @(R15, 16)
  MOV.L    RD30, @(R15, 12)
  MOV.L    RD29, @(R15, 28)
  MOV.L    RD28, @(R15, 36)
  MOV.L    RD27, @(R15, 32)
  CMP/EQ   #0, RD27
  BT       L00800A1A
  MOV.L    @(R15, 32), RD7
  CMP/EQ   #2, RD7
  BT       L00800A1A
  BRA      L00800A1D

L00800A1D:
  MOV.L    @(R15, 32), RD6
  CMP/EQ   #3, RD6
  BT       L00800A1A
  BRA      L00800A1C

L00800A1C:
  MOV.L    @(R15, 32), RD5
  CMP/EQ   #7, RD5
  BT       L00800A1A
  BRA      L00800A1B

L00800A1B:
  MOV.L    @(R15, 32), RD4
  CMP/EQ   #1, RD4
  BT       L00800A1F
  TST      #128, RD14
  BT       L00800A20
  AND      #127, RD14
  MOV.Q    @(R15, 40), RQ2
  MOV      RQ2, RQ3
  MOV.Q    @(R15, 40), RQ23
  ADD      #1, RQ23
  MOV.Q    RQ23, @(R15, 40)
  MOVU.B   @RQ3, RD22
  MOV      RD22, RD12
  MOV.Q    RQ3, @(R15, 16)
  MOV.L    RD22, @(R15, 12)
  BRA      L00800A21

L00800A20:
  MOV.L    ld1, RD21
  MOV      RD21, RD12

L00800A21:
  MOV      RD14, RD20
  MOV      RD12, RD19
  MOV.L    RD20, ld0
  MOV.L    RD19, ld1
  BRA      L00800A22

L00800A1F:
  MOV.Q    @(R15, 40), RQ17
  MOV      RQ17, RQ18
  MOV.Q    @(R15, 40), RQ16
  ADD      #1, RQ16
  MOV.Q    RQ16, @(R15, 40)
  MOVU.B   @RQ18, RD19
  MOV      RD19, RD12
  MOV.L    RD19, @(R15, 12)
  MOV.Q    RQ18, @(R15, 16)

L00800A22:
  BRA      L00800A23

L00800A1A:
  MOV      #0, RD12

L00800A23:
  MOV.L    @(R15, 28), RD16
  TST      #128, RD16
  BT       L00800A24
  MOV      #40, RQ25
  ADD      R15, RQ25
  MOV.Q    RQ25, @(R15, 16)
  MOV      RQ25, RQ4
  BSR      I_SMus_ParseVLI
  MOV      RD2, RD26
  MOV.L    i_smus_tt, RD27
  ADD      RD26, RD27
  MOV.L    RD27, i_smus_tt
  MOV.L    RD26, @(R15, 24)

L00800A24:
  MOV.Q    @(R15, 40), RQ17
  MOV      RQ17, RQ16
  MOV.Q    RQ16, i_smus_cs
  MOV.Q    i_smus_cs, RQ18
  MOV.Q    i_smus_cse, RQ19
  CMPQ/GT  RQ18, RQ19
  BT       L00800A25
  BSR      SMus_SilenceAll
  MOV.Q    i_smus_css, RQ17
  MOV      RQ17, RQ16
  MOV      #1, RD18
  MOV.L    RD18, i_smus_tt
  MOV.Q    RQ16, i_smus_cs
  MOV      #1, RD2
  BRA      L00E00537

L00800A25:
  MOV.L    @(R15, 32), RD16
  CMP/EQ   #0, RD16
  BT       L00800A27
  MOV.L    @(R15, 32), RD16
  CMP/EQ   #1, RD16
  BT       L00800A28
  MOV.L    @(R15, 32), RD16
  CMP/EQ   #2, RD16
  BT       L00800A29
  MOV.L    @(R15, 32), RD16
  CMP/EQ   #3, RD16
  BT       L00800A2A
  MOV.L    @(R15, 32), RD16
  CMP/EQ   #4, RD16
  BT       L00800A2B
  MOV.L    @(R15, 32), RD16
  CMP/EQ   #6, RD16
  BT       L00800A2C
  MOV.L    @(R15, 32), RD16
  CMP/EQ   #7, RD16
  BT       L00800A2D
  BRA      L00800A2E

L00800A27:
  MOV      RD13, RD4
  MOV      RD14, RD5
  MOV      RD12, RD6
  BSR      SMus_NoteOff
  BRA      L00800A26

L00800A28:
  MOV      RD13, RD4
  MOV      RD14, RD5
  MOV      RD12, RD6
  BSR      SMus_NoteOn
  BRA      L00800A26

L00800A29:
  SHAD     RD14, #6, RD25
  MOV.L    RD25, @(R15, 12)
  MOV      RD13, RD4
  MOV      RD25, RD5
  BSR      SMus_PitchBlend
  BRA      L00800A26

L00800A2A:
  CMP/EQ   #10, RD14
  BT       L00800A30
  CMP/EQ   #11, RD14
  BT       L00800A31
  CMP/EQ   #12, RD14
  BT       L00800A32
  CMP/EQ   #13, RD14
  BT       L00800A33
  CMP/EQ   #14, RD14
  BT       L00800A34
  BRA      L00800A2F

L00800A30:
  MOV      RD13, RD4
  MOV      #120, RD5
  MOV      #1, RD6
  BSR      SMus_Controller
  BRA      L00800A2F

L00800A31:
  MOV      RD13, RD4
  MOV      #123, RD5
  MOV      #1, RD6
  BSR      SMus_Controller
  BRA      L00800A2F

L00800A32:
  MOV      RD13, RD4
  MOV      #126, RD5
  MOV      #1, RD6
  BSR      SMus_Controller
  BRA      L00800A2F

L00800A33:
  MOV      RD13, RD4
  MOV      #127, RD5
  MOV      #1, RD6
  BSR      SMus_Controller
  BRA      L00800A2F

L00800A34:
  MOV      RD13, RD4
  MOV      #121, RD5
  MOV      #1, RD6
  BSR      SMus_Controller
  BRA      L00800A2F

L00800A2F:
  BRA      L00800A26

L00800A2B:
  MOV      #5, RD16
  CMP/GE   RD16, RD14
  BT       L00800A41
  CMP/EQ   RD14
  BT       L00800A36
  CMP/EQ   #1, RD14
  BT       L00800A37
  CMP/EQ   #2, RD14
  BT       L00800A38
  CMP/EQ   #3, RD14
  BT       L00800A39
  CMP/EQ   #4, RD14
  BT       L00800A3A

L00800A41:
  CMP/EQ   #5, RD14
  BT       L00800A3B
  CMP/EQ   #6, RD14
  BT       L00800A3C
  CMP/EQ   #7, RD14
  BT       L00800A3D
  CMP/EQ   #8, RD14
  BT       L00800A3E
  CMP/EQ   #9, RD14
  BT       L00800A3F
  BRA      L00800A40

L00800A36:
  MOV      RD13, RD4
  MOV      RD12, RD5
  BSR      SMus_ProgramChange
  BRA      L00800A35

L00800A37:
  MOV      RD13, RD4
  MOV      #0, RD5
  MOV      RD12, RD6
  BSR      SMus_Controller
  BRA      L00800A35

L00800A38:
  MOV      RD13, RD4
  MOV      #1, RD5
  MOV      RD12, RD6
  BSR      SMus_Controller
  BRA      L00800A35

L00800A39:
  MOV      RD13, RD4
  MOV      #7, RD5
  MOV      RD12, RD6
  BSR      SMus_Controller
  BRA      L00800A35

L00800A3A:
  MOV      RD13, RD4
  MOV      #10, RD5
  MOV      RD12, RD6
  BSR      SMus_Controller
  BRA      L00800A35

L00800A3B:
  MOV      RD13, RD4
  MOV      #11, RD5
  MOV      RD12, RD6
  BSR      SMus_Controller
  BRA      L00800A35

L00800A3C:
  MOV      RD13, RD4
  MOV      #91, RD5
  MOV      RD12, RD6
  BSR      SMus_Controller
  BRA      L00800A35

L00800A3D:
  MOV      RD13, RD4
  MOV      #93, RD5
  MOV      RD12, RD6
  BSR      SMus_Controller
  BRA      L00800A35

L00800A3E:
  MOV      RD13, RD4
  MOV      #64, RD5
  MOV      RD12, RD6
  BSR      SMus_Controller
  BRA      L00800A35

L00800A3F:
  MOV      RD13, RD4
  MOV      #67, RD5
  MOV      RD12, RD6
  BSR      SMus_Controller
  BRA      L00800A35

L00800A40:

L00800A35:
  BRA      L00800A26

L00800A2C:
  BRA      L00800A26

L00800A2D:
  BRA      L00800A26

L00800A2E:
  MOV      L00C00504, RQ4
  MOV.L    @(R15, 32), RD5
  BSR      printf

L00800A26:
  MOV      #0, RD2

L00E00537:
  ADD      #72, R15
  BRA      L00C00194

S_Init:
  PUSH     LR
  BSR      L00C00193
  ADD      #-152, R15
  MOV.L    RD4, @(R15, 52)
  MOV.L    RD5, @(R15, 48)
  BSR      __get_stderr
  MOV      RQ2, RQ13
  MOV.Q    @RQ13, RQ12
  MOV      RQ12, RQ4
  MOV      L00C0051C, RQ5
  MOV.L    @(R15, 52), RD6
  BSR      fprintf
  BSR      I_SetChannels
  MOV.L    @(R15, 52), RD4
  BSR      S_SetSfxVolume
  MOV.L    @(R15, 48), RD4
  BSR      S_SetMusicVolume
  MOV.L    numChannels, RD10
  MOV      RD10, RD11
  MOV      #24, RD9
  MUL.L    RD9, RD11, RD11
  MOV      #0, RQ12
  MOV.L    RD11, @(R15, 28)
  MOV      RD11, RD4
  MOV      #1, RD5
  MOV      RQ12, RQ6
  BSR      Z_Malloc
  MOV      RQ2, RQ13
  MOV      RQ13, RQ8
  MOV      #0, RD14
  MOV.Q    RQ8, TU0ADE0BF8/s_channels

L00802508:
  MOV.L    numChannels, RD31
  CMP/GT   RD14, RD31
  BT       L0080250A
  MOV.Q    TU0ADE0BF8/s_channels, RQ30
  EXTS.L   RD14
  MOV      #24, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ29
  ADD      RQ30, RQ29
  MOV      #0, RQ13
  MOV.Q    RQ13, @RQ29

L00802509:
  ADD      #1, RD14
  BRA      L00802508

L0080250A:
  MOV      #0, RD28
  MOV      #1, RD14
  MOV.L    RD28, TU0ADE0BF8/mus_paused

L0080250B:
  CMP/GE   #109, RD14
  BT       L0080250D
  MOV      S_sfx, RQ27
  EXTS.L   RD14
  MOV      #48, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ26
  ADD      RQ27, RQ26
  MOV      #-1, RD25
  MOV.L    RD25, @(RQ26, 40)
  EXTS.L   RD14
  MOV      #48, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ26
  ADD      RQ27, RQ26
  MOV.L    RD25, @(RQ26, 44)

L0080250C:
  ADD      #1, RD14
  BRA      L0080250B

L0080250D:

L00E00538:
  ADD      #152, R15
  BRA      L00C00194

S_Start:
  PUSH     LR
  BSR      L00C00193
  ADD      #-88, R15
  MOV      #0, RD13

L0080250E:
  MOV.L    numChannels, RD12
  CMP/GT   RD13, RD12
  BT       L00802510
  MOV.Q    TU0ADE0BF8/s_channels, RQ11
  EXTS.L   RD13
  MOV      #24, RD7
  MUL.L    RD13, RD7
  STS      R0, RQ10
  ADD      RQ11, RQ10
  MOV.Q    @RQ10, RQ11
  MOV.Q    RQ11, @(R15, 16)
  MOV      #0, RQ9
  CMPQ/EQ  RQ9, RQ11
  BT       L00802511
  MOV      RD13, RD4
  BSR      S_StopChannel

L00802511:

L0080250F:
  ADD      #1, RD13
  BRA      L0080250E

L00802510:
  MOV      #0, RD8
  MOV.L    RD8, TU0ADE0BF8/mus_paused
  MOV.L    gamemode, RD31
  CMP/EQ   #2, RD31
  BT       L00802512
  MOV.L    gamemap, RD30
  ADD      RD30, #33, RD14
  SUB      RD14, #1, RD29
  MOV.L    RD29, @(R15, 36)
  BRA      L00802513

L00802512:
  MOV.L    gameepisode, RD28
  MOV      #4, RD27
  CMP/GE   RD27, RD28
  BT       L00802514
  MOV.L    gameepisode, RD26
  SUB      RD26, #1, RD14
  MOV      RD14, RD25
  MOV      #9, RD7
  MUL.L    RD7, RD25, RD25
  ADD      RD25, #1, RD14
  MOV.L    gamemap, RD6
  ADD      RD14, RD6, RD25
  SUB      RD25, #1, RD5
  MOV.L    RD5, @(R15, 36)
  BRA      L00802515

L00802514:
  MOV.L    gamemap, RD4
  SUB      RD4, #1, RD14
  MOV      spmus, RQ3
  EXTS.L   RD14
  MOV.L    @(RQ3, RD14), RD2
  MOV.L    RD2, @(R15, 36)

L00802515:

L00802513:
  MOV.L    @(R15, 36), RD4
  MOV      #1, RD5
  BSR      S_ChangeMusic
  MOV      #15, RD23
  MOV.L    RD23, TU0ADE0BF8/nextcleanup

L00E00539:
  ADD      #88, R15
  BRA      L00C00194

S_StartSound:
  PUSH     LR
  ADD      #-152, R15
  MOV.Q    RQ4, @(R15, 8)
  MOV.L    RD5, @(R15, 4)
  MOV.Q    @(R15, 8), RQ4
  MOV.L    @(R15, 4), RD5
  MOV.L    snd_SfxVolume, RD6
  BSR      S_StartSoundAtVolume

L00E0053A:
  ADD      #152, R15
  RET     

S_StartSoundAtVolume:
  PUSH     LR
  BSR      L00C00193
  ADD      #-488, R15
  MOV.Q    RQ4, @(R15, 112)
  MOV.L    RD5, @(R15, 108)
  MOV.L    RD6, @(R15, 104)
  MOV.Q    @(R15, 112), RQ12
  MOV      RQ12, RQ13
  MOV.Q    RQ13, @(R15, 64)
  MOV.L    @(R15, 108), RD11
  MOV      #1, RD10
  CMP/GE   RD10, RD11
  BT       L00802517
  MOV.L    @(R15, 108), RD9
  CMP/GT   #109, RD9
  BT       L00802517
  BRA      L00802516

L00802517:
  MOV      L00C00529, RQ4
  MOV.L    @(R15, 108), RD5
  BSR      I_Error

L00802516:
  MOV      S_sfx, RQ8
  MOV.L    @(R15, 108), RD31
  EXTS.L   RD31
  MOV      #48, RD7
  MUL.L    RD31, RD7
  STS      R0, RQ30
  ADD      RQ8, RQ30
  MOV      RQ30, RQ29
  MOV.Q    @(RQ29, 16), RQ30
  MOV.Q    RQ30, @(R15, 56)
  MOV.Q    RQ29, @(R15, 80)
  MOV      #0, RQ28
  CMPQ/EQ  RQ28, RQ30
  BT       L00802519
  MOV.Q    @(R15, 80), RQ27
  MOV.L    @(RQ27, 24), RD26
  MOV.L    RD26, @(R15, 92)
  MOV.L    @(RQ27, 12), RD25
  MOV.L    @(RQ27, 28), RD14
  MOV.L    @(R15, 104), RD7
  ADD      RD14, RD7
  MOV.L    RD7, @(R15, 104)
  MOV.L    RD25, @(R15, 88)
  MOV.L    @(R15, 104), RD6
  MOV      #1, RD5
  CMP/GE   RD5, RD6
  BT       L0080251A
  BRA      L00E0053B

L0080251A:
  MOV.L    @(R15, 104), RD4
  MOV.L    snd_SfxVolume, RD3
  CMP/GT   RD3, RD4
  BT       L0080251B
  MOV.L    snd_SfxVolume, RD23
  MOV      RD23, RD2
  MOV.L    RD2, @(R15, 104)

L0080251B:
  BRA      L0080251C

L00802519:
  MOV      #128, RD25
  MOV.L    RD25, @(R15, 92)
  MOV      #64, RD25
  MOV.L    RD25, @(R15, 88)

L0080251C:
  MOV.Q    @(R15, 64), RQ22
  MOV      #0, RQ21
  CMPQ/EQ  RQ21, RQ22
  BT       L0080251D
  MOV      players, RQ25
  MOV.L    consoleplayer, RD26
  EXTS.L   RD26
  MOV      #320, RD7
  MUL.L    RD26, RD7
  STS      R0, RQ27
  ADD      RQ25, RQ27
  MOV.Q    @RQ27, RQ28
  MOV.Q    RQ28, @(R15, 56)
  MOV.Q    RQ27, @(R15, 40)
  MOV.Q    @(R15, 64), RQ29
  CMPQ/EQ  RQ28, RQ29
  BT       L0080251D
  BRA      L0080251E

L0080251E:
  MOV      #92, RQ25
  ADD      R15, RQ25
  MOV      #96, RQ26
  ADD      R15, RQ26
  MOV      #104, RQ27
  ADD      R15, RQ27
  MOV      players, RQ28
  MOV.L    consoleplayer, RD29
  EXTS.L   RD29
  MOV      #320, RD7
  MUL.L    RD29, RD7
  STS      R0, RQ30
  ADD      RQ28, RQ30
  MOV.Q    @RQ30, RQ31
  MOV.Q    RQ31, @(R15, 16)
  MOV.Q    RQ30, @(R15, 40)
  MOV.Q    RQ27, @(R15, 24)
  MOV.Q    RQ26, @(R15, 32)
  MOV.Q    RQ25, @(R15, 56)
  MOV      RQ31, RQ4
  MOV.Q    @(R15, 64), RQ5
  MOV      RQ27, RQ6
  MOV      RQ26, RQ7
  MOV      RQ25, RQ20
  BSR      S_AdjustSoundParams
  MOV      RD2, RD8
  MOV.Q    @(R15, 64), RQ9
  MOV.L    @(RQ9, 24), RD14
  MOV      players, RQ10
  MOV.L    consoleplayer, RD11
  MOV      #320, RD7
  MUL.L    RD11, RD7
  STS      R0, RQ30
  ADD      RQ10, RQ30
  MOV.Q    @RQ30, RQ25
  MOV.L    @(RQ25, 24), RD12
  MOV.L    RD12, @(R15, 12)
  MOV.L    RD8, @(R15, 100)
  MOV.Q    RQ30, @(R15, 40)
  MOV.Q    RQ25, @(R15, 56)
  CMP/EQ   RD12, RD14
  BT       L00802520
  MOV.Q    @(R15, 64), RQ25
  MOV.L    @(RQ25, 28), RD14
  MOV      players, RQ26
  MOV.L    consoleplayer, RD27
  MOV      #320, RD7
  MUL.L    RD27, RD7
  STS      R0, RQ28
  ADD      RQ26, RQ28
  MOV.Q    @RQ28, RQ29
  MOV.L    @(RQ29, 28), RD30
  MOV.L    RD30, @(R15, 12)
  MOV.Q    RQ29, @(R15, 56)
  MOV.Q    RQ28, @(R15, 40)
  CMP/EQ   RD30, RD14
  BT       L00802520
  BRA      L00802521

L00802521:
  MOV      #128, RD20
  MOV.L    RD20, @(R15, 96)

L00802520:
  MOV.L    @(R15, 100), RD19
  CMP/EQ   #0, RD19
  BT       L00802523
  BRA      L00E0053B

L00802523:
  BRA      L00802524

L0080251D:
  MOV      #128, RD18
  MOV.L    RD18, @(R15, 96)

L00802524:
  MOV.L    @(R15, 108), RD17
  MOV      #10, RD16
  CMP/GE   RD16, RD17
  BT       L00802525
  MOV.L    @(R15, 108), RD16
  CMP/GT   #13, RD16
  BT       L00802525
  BRA      L00802526

L00802526:
  BSR      M_Random
  MOV      RD2, RD14
  AND      RD14, #15, RD25
  MOV      #8, RD26
  SUB      RD26, RD25, RD14
  MOV.L    @(R15, 92), RD27
  ADD      RD14, RD27
  MOV.L    RD27, @(R15, 92)
  MOV.L    RD25, @(R15, 12)
  MOV.L    @(R15, 92), RD25
  MOV.L    @(R15, 92), RD28
  MOV      #0, RD29
  CMP/GE   RD29, RD28
  BT       L00802528
  MOV      #0, RD16
  MOV.L    RD16, @(R15, 92)
  BRA      L00802529

L00802528:
  MOV.L    @(R15, 92), RD16
  CMP/GT   #255, RD16
  BT       L0080252A
  MOV      #255, RD16
  MOV.L    RD16, @(R15, 92)

L0080252A:

L00802529:
  BRA      L0080252B

L00802525:
  MOV.L    @(R15, 108), RD16
  CMP/EQ   #32, RD16
  BT       L0080252C
  MOV.L    @(R15, 108), RD16
  CMP/EQ   #87, RD16
  BT       L0080252C
  BRA      L0080252D

L0080252D:
  BSR      M_Random
  MOV      RD2, RD14
  AND      RD14, #31, RD25
  MOV      #16, RD26
  SUB      RD26, RD25, RD14
  MOV.L    @(R15, 92), RD27
  ADD      RD14, RD27
  MOV.L    RD27, @(R15, 92)
  MOV.L    RD25, @(R15, 12)
  MOV.L    @(R15, 92), RD25
  MOV.L    @(R15, 92), RD28
  MOV      #0, RD29
  CMP/GE   RD29, RD28
  BT       L0080252F
  MOV      #0, RD16
  MOV.L    RD16, @(R15, 92)
  BRA      L00802530

L0080252F:
  MOV.L    @(R15, 92), RD16
  CMP/GT   #255, RD16
  BT       L00802531
  MOV      #255, RD16
  MOV.L    RD16, @(R15, 92)

L00802531:

L00802530:

L0080252C:

L0080252B:
  MOV.Q    @(R15, 64), RQ4
  BSR      S_StopSound
  MOV.Q    @(R15, 64), RQ4
  MOV.Q    @(R15, 80), RQ5
  BSR      S_getChannel
  MOV      RD2, RD25
  MOV.L    RD25, @(R15, 76)
  MOV      #0, RD26
  CMP/GE   RD26, RD25
  BT       L00802532
  BRA      L00E0053B

L00802532:
  MOV.Q    @(R15, 80), RQ16
  MOV.L    @(RQ16, 44), RD14
  CMP/PZ   RD14
  BT       L00802533
  MOV.Q    @(R15, 80), RQ4
  BSR      I_GetSfxLumpNum
  MOV      RD2, RD14
  MOV.Q    @(R15, 80), RQ25
  MOV.L    RD14, @(RQ25, 44)

L00802533:
  MOV.Q    @(R15, 80), RQ16
  MOV.Q    @(RQ16, 32), RQ17
  MOV.Q    RQ17, @(R15, 56)
  MOV      #0, RQ18
  CMPQ/EQ  RQ18, RQ17
  BT       L00802534
  BSR      __get_stderr
  MOV      RQ2, RQ25
  MOV.Q    @RQ25, RQ26
  MOV.Q    RQ26, @(R15, 32)
  MOV.Q    RQ25, @(R15, 56)
  MOV      RQ26, RQ4
  MOV      L00C0052E, RQ5
  BSR      fprintf

L00802534:
  MOV.Q    @(R15, 80), RQ16
  MOV.L    @(RQ16, 40), RD14
  MOV      RD14, RD17
  ADD      RD14, #1, RD18
  MOV.L    RD18, @(RQ16, 40)
  MOV.L    RD17, @(R15, 12)
  MOV      #0, RD19
  CMP/GE   RD19, RD17
  BT       L00802535
  MOV      #1, RD16
  MOV.Q    @(R15, 80), RQ17
  MOV.L    RD16, @(RQ17, 40)

L00802535:
  MOV.L    @(R15, 108), RD4
  MOV.L    @(R15, 104), RD5
  MOV.L    @(R15, 96), RD6
  MOV.L    @(R15, 92), RD7
  MOV.L    @(R15, 88), RD20
  BSR      I_StartSound
  MOV      RD2, RD14
  MOV.Q    TU0ADE0BF8/s_channels, RQ25
  MOV.L    @(R15, 76), RD26
  EXTS.L   RD26
  MOV      #24, RD7
  MUL.L    RD26, RD7
  STS      R0, RQ27
  ADD      RQ25, RQ27
  MOV.L    RD14, @(RQ27, 16)

L00E0053B:
  ADD      #488, R15
  BRA      L00C00194

S_StopSound:
  PUSH     LR
  BSR      L00C003FF
  ADD      #-120, R15
  MOV.Q    RQ4, @(R15, 16)
  MOV      #0, RD14

L00802536:
  MOV.L    numChannels, RD13
  CMP/GT   RD14, RD13
  BT       L00802538
  MOV.Q    TU0ADE0BF8/s_channels, RQ12
  EXTS.L   RD14
  MOV      #24, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ11
  ADD      RQ12, RQ11
  MOV.Q    @RQ11, RQ12
  MOV.Q    RQ12, @R15
  MOV.Q    RQ11, @(R15, 8)
  MOV      #0, RQ10
  CMPQ/EQ  RQ10, RQ12
  BT       L00802539
  MOV.Q    TU0ADE0BF8/s_channels, RQ9
  EXTS.L   RD14
  MOV      #24, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ8
  ADD      RQ9, RQ8
  MOV.Q    @(RQ8, 8), RQ9
  MOV.Q    RQ9, @R15
  MOV.Q    RQ8, @(R15, 8)
  MOV.Q    @(R15, 16), RQ31
  CMPQ/EQ  RQ31, RQ9
  BT       L00802539
  BRA      L0080253A

L0080253A:
  MOV      RD14, RD4
  BSR      S_StopChannel
  BRA      L00802538

L00802539:

L00802537:
  ADD      #1, RD14
  BRA      L00802536

L00802538:

L00E0053C:
  ADD      #120, R15
  BRA      L00C00404

S_StartMusic:
  PUSH     LR
  ADD      #-152, R15
  MOV.L    RD4, @(R15, 12)
  MOV.L    @(R15, 12), RD4
  MOV      #0, RD5
  BSR      S_ChangeMusic

L00E0053D:
  ADD      #152, R15
  RET     

S_ChangeMusic:
  PUSH     LR
  BSR      L00C00472
  ADD      #-88, R15
  MOV.L    RD4, @(R15, 32)
  MOV.L    RD5, @(R15, 28)
  MOV      #36, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 16)
  MOV.L    @(R15, 32), RD12
  CMP/PL   RD12
  BT       L00802558
  MOV.L    @(R15, 32), RD11
  CMP/GE   #68, RD11
  BT       L00802558
  BRA      L00802557

L00802558:
  MOV      L00C00530, RQ4
  MOV.L    @(R15, 32), RD5
  BSR      I_Error
  BRA      L0080255A

L00802557:
  MOV      S_music, RQ10
  MOV.L    @(R15, 32), RD9
  SHAD     RD9, #5, RQ8
  ADD      RQ10, RQ8
  MOV      RQ8, RQ13
  MOV.Q    RQ8, @(R15, 8)

L0080255A:
  MOV.Q    TU0ADE0BF8/mus_playing, RQ31
  CMPQ/EQ  RQ13, RQ31
  BT       L0080255B
  BRA      L00E0053E

L0080255B:
  BSR      S_StopMusic
  MOV.L    gamemode, RD30
  CMP/EQ   #4, RD30
  BT       L0080255C
  MOV.L    @(RQ13, 8), RD14
  CMP/PL   RD14
  BT       L0080255D
  MOV.Q    @RQ13, RQ29
  MOV.Q    RQ29, @(R15, 8)
  MOV      #36, RQ4
  ADD      R15, RQ4
  MOV      L00C00533, RQ5
  MOV      RQ29, RQ6
  BSR      sprintf
  MOV      #36, RQ4
  ADD      R15, RQ4
  BSR      W_GetNumForName
  MOV      RD2, RD14
  MOV.L    RD14, @(RQ13, 8)

L0080255D:

L0080255C:
  MOV.L    @(RQ13, 8), RD14
  CMP/PL   RD14
  BT       L0080255E
  MOV.Q    @RQ13, RQ28
  MOV.Q    RQ28, @(R15, 8)
  MOV      #36, RQ4
  ADD      R15, RQ4
  MOV      L00C00534, RQ5
  MOV      RQ28, RQ6
  BSR      sprintf
  MOV      #36, RQ4
  ADD      R15, RQ4
  BSR      W_GetNumForName
  MOV      RD2, RD14
  MOV.L    RD14, @(RQ13, 8)

L0080255E:
  MOV.L    @(RQ13, 8), RD14
  MOV      RD14, RD4
  MOV      #3, RD5
  BSR      W_CacheLumpNum
  MOV      RQ2, RQ27
  MOV.Q    RQ27, @(RQ13, 16)
  MOV.Q    @(RQ13, 16), RQ27
  MOV.Q    RQ27, @(R15, 8)
  MOV      RQ27, RQ4
  BSR      I_RegisterSong
  MOV      RD2, RD14
  MOV.L    RD14, @(RQ13, 24)
  MOV.L    @(RQ13, 24), RD14
  MOV      RD14, RD4
  MOV.L    @(R15, 28), RD5
  BSR      I_PlaySong
  MOV      RQ13, RQ7
  MOV.Q    RQ7, TU0ADE0BF8/mus_playing

L00E0053E:
  ADD      #88, R15
  BRA      L00C00476

S_StopMusic:
  PUSH     LR
  BSR      L00C00193
  ADD      #-56, R15
  MOV.Q    TU0ADE0BF8/mus_playing, RQ12
  MOV      #0, RQ11
  CMPQ/EQ  RQ11, RQ12
  BT       L0080255F
  MOV.L    TU0ADE0BF8/mus_paused, RD10
  CMP/EQ   RD10
  BT       L00802560
  MOV.Q    TU0ADE0BF8/mus_playing, RQ9
  MOV.L    @(RQ9, 24), RD13
  MOV      RD13, RD4
  BSR      I_ResumeSong

L00802560:
  MOV.Q    TU0ADE0BF8/mus_playing, RQ8
  MOV.L    @(RQ8, 24), RD13
  MOV      RD13, RD4
  BSR      I_StopSong
  MOV.Q    TU0ADE0BF8/mus_playing, RQ31
  MOV.L    @(RQ31, 24), RD13
  MOV      RD13, RD4
  BSR      I_UnRegisterSong
  MOV.Q    TU0ADE0BF8/mus_playing, RQ30
  MOV.Q    @(RQ30, 16), RQ14
  MOV      #0, RQ29
  CMPQ/EQ  RQ29, RQ14
  BT       L00802561
  MOV.Q    TU0ADE0BF8/mus_playing, RQ28
  MOV.Q    @(RQ28, 16), RQ14
  MOV      #0, RQ27
  CMPQ/EQ  RQ27, RQ14
  BT       L00802562
  MOV.Q    TU0ADE0BF8/mus_playing, RQ26
  MOV.Q    @(RQ26, 16), RQ14
  ADD      RQ14, #-40, RQ25
  MOV.L    @(RQ25, 20), RD13
  CMP/EQ   #1919505, RD13
  BT       L00802563
  MOV      L00C00535, RQ4
  MOV      #702, RD5
  BSR      I_Error

L00802563:
  MOV.Q    TU0ADE0BF8/mus_playing, RQ25
  MOV.Q    @(RQ25, 16), RQ14
  MOV      RQ14, RQ4
  MOV      #101, RD5
  BSR      Z_ChangeTag2

L00802562:

L00802561:
  MOV      #0, RQ14
  MOV.Q    TU0ADE0BF8/mus_playing, RQ7
  MOV.Q    RQ14, @(RQ7, 16)
  MOV      #0, RQ14
  MOV      RQ14, RQ7
  MOV.Q    RQ7, TU0ADE0BF8/mus_playing

L0080255F:

L00E0053F:
  ADD      #56, R15
  BRA      L00C00194

S_PauseSound:
  PUSH     LR
  BSR      L00C003ED
  ADD      #-112, R15
  MOV.Q    TU0ADE0BF8/mus_playing, RQ14
  MOV      #0, RQ13
  CMPQ/EQ  RQ13, RQ14
  BT       L0080253C
  MOV.L    TU0ADE0BF8/mus_paused, RD12
  CMP/EQ   RD12
  BT       L0080253C
  BRA      L0080253D

L0080253D:
  MOV.Q    TU0ADE0BF8/mus_playing, RQ11
  MOV.L    @(RQ11, 24), RD10
  MOV.L    RD10, @(R15, 12)
  MOV      RD10, RD4
  BSR      I_PauseSong
  MOV      #1, RD12
  MOV.L    RD12, TU0ADE0BF8/mus_paused

L0080253C:

L00E00540:
  ADD      #112, R15
  BRA      L00C003EF

S_ResumeSound:
  PUSH     LR
  BSR      L00C003ED
  ADD      #-112, R15
  MOV.Q    TU0ADE0BF8/mus_playing, RQ14
  MOV      #0, RQ13
  CMPQ/EQ  RQ13, RQ14
  BT       L0080253F
  MOV.L    TU0ADE0BF8/mus_paused, RD12
  CMP/EQ   RD12
  BT       L0080253F
  BRA      L00802540

L00802540:
  MOV.Q    TU0ADE0BF8/mus_playing, RQ11
  MOV.L    @(RQ11, 24), RD10
  MOV.L    RD10, @(R15, 12)
  MOV      RD10, RD4
  BSR      I_ResumeSong
  MOV      #0, RD12
  MOV.L    RD12, TU0ADE0BF8/mus_paused

L0080253F:

L00E00541:
  ADD      #112, R15
  BRA      L00C003EF

S_UpdateSounds:
  PUSH     LR
  BSR      L00C00193
  ADD      #-120, R15
  MOV.Q    RQ4, @(R15, 88)
  MOV.Q    @(R15, 88), RQ11
  MOV      RQ11, RQ12
  MOV      #0, RD10
  MOV.Q    RQ12, @(R15, 48)
  MOV.L    RD10, @(R15, 80)

L00802542:
  MOV.L    @(R15, 80), RD9
  MOV.L    numChannels, RD8
  CMP/GT   RD9, RD8
  BT       L00802544
  MOV.Q    TU0ADE0BF8/s_channels, RQ31
  MOV.L    @(R15, 80), RD30
  EXTS.L   RD30
  MOV      #24, RD7
  MUL.L    RD30, RD7
  STS      R0, RQ14
  ADD      RQ31, RQ14
  MOV      RQ14, RQ13
  MOV.Q    @RQ13, RQ31
  MOV.Q    @RQ13, RQ14
  MOV.Q    RQ31, @(R15, 56)
  MOV      #0, RQ29
  CMPQ/EQ  RQ29, RQ14
  BT       L00802545
  MOV.L    @(RQ13, 16), RD28
  MOV.L    RD28, @(R15, 36)
  MOV      RD28, RD4
  BSR      I_SoundIsPlaying
  MOV      RD2, RD27
  MOV.L    RD27, @(R15, 32)
  CMP/EQ   #0, RD27
  BT       L00802546
  MOV.L    snd_SfxVolume, RD25
  MOV      RD25, RD26
  MOV.L    RD26, @(R15, 76)
  MOV      #128, RD27
  MOV.L    RD27, @(R15, 68)
  MOV      #128, RD28
  MOV.L    RD28, @(R15, 72)
  MOV.Q    @(R15, 56), RQ29
  MOV.Q    @(RQ29, 16), RQ14
  MOV      #0, RQ30
  CMPQ/EQ  RQ30, RQ14
  BT       L00802547
  MOV.Q    @(R15, 56), RQ7
  MOV.L    @(RQ7, 24), RD6
  MOV.L    RD6, @(R15, 68)
  MOV.L    @(RQ7, 28), RD5
  MOV.L    @(R15, 76), RD4
  ADD      RD5, RD4
  MOV.L    RD4, @(R15, 76)
  MOV.L    RD5, @(R15, 36)
  MOV.L    @(R15, 76), RD3
  MOV      #1, RD2
  CMP/GE   RD2, RD3
  BT       L00802548
  MOV.L    @(R15, 80), RD4
  BSR      S_StopChannel
  BRA      L00802543
  BRA      L00802549

L00802548:
  MOV.L    @(R15, 76), RD23
  MOV.L    snd_SfxVolume, RD22
  CMP/GT   RD22, RD23
  BT       L0080254A
  MOV.L    snd_SfxVolume, RD20
  MOV      RD20, RD21
  MOV.L    RD21, @(R15, 76)

L0080254A:

L00802549:

L00802547:
  MOV.Q    @(RQ13, 8), RQ14
  MOV      #0, RQ19
  CMPQ/EQ  RQ19, RQ14
  BT       L0080254B
  MOV.Q    @(RQ13, 8), RQ14
  MOV.Q    @(R15, 88), RQ18
  CMPQ/EQ  RQ14, RQ18
  BT       L0080254B
  BRA      L0080254C

L0080254C:
  MOV      #68, RQ14
  ADD      R15, RQ14
  MOV      #72, RQ25
  ADD      R15, RQ25
  MOV      #76, RQ26
  ADD      R15, RQ26
  MOV.Q    @(RQ13, 8), RQ27
  MOV.Q    RQ27, @(R15, 8)
  MOV.Q    RQ26, @(R15, 16)
  MOV.Q    RQ25, @(R15, 24)
  MOV.Q    @(R15, 48), RQ4
  MOV      RQ27, RQ5
  MOV      RQ26, RQ6
  MOV      RQ25, RQ7
  MOV      RQ14, RQ20
  BSR      S_AdjustSoundParams
  MOV      RD2, RD25
  MOV.L    RD25, @(R15, 84)
  CMP/EQ   #0, RD25
  BT       L0080254E
  MOV.L    @(R15, 80), RD4
  BSR      S_StopChannel
  BRA      L0080254F

L0080254E:
  MOV.L    @(RQ13, 16), RD25
  MOV.L    RD25, @(R15, 36)
  MOV      RD25, RD4
  MOV.L    @(R15, 76), RD5
  MOV.L    @(R15, 72), RD6
  MOV.L    @(R15, 68), RD7
  BSR      I_UpdateSoundParams

L0080254F:

L0080254B:
  BRA      L00802550

L00802546:
  MOV.L    @(R15, 80), RD4
  BSR      S_StopChannel

L00802550:

L00802545:

L00802543:
  MOV.L    @(R15, 80), RD17
  ADD      #1, RD17
  MOV.L    RD17, @(R15, 80)
  BRA      L00802542

L00802544:

L00E00542:
  ADD      #120, R15
  BRA      L00C00194

S_SetMusicVolume:
  PUSH     LR
  PUSH     R14
  PUSH     R13
  ADD      #-136, R15
  MOV      RD4, RD14
  CMP/PZ   RD14
  BT       L00802552
  CMP/GT   #127, RD14
  BT       L00802552
  BRA      L00802551

L00802552:
  MOV      L00C00539, RQ4
  MOV      RD14, RD5
  BSR      I_Error

L00802551:
  MOV      #127, RD4
  BSR      I_SetMusicVolume
  MOV      RD14, RD4
  BSR      I_SetMusicVolume
  MOV      RD14, RD13
  MOV.L    RD13, snd_MusicVolume

L00E00543:
  ADD      #136, R15
  POP      RQ13
  POP      RQ14
  RET     

S_SetSfxVolume:
  PUSH     LR
  PUSH     R14
  PUSH     R13
  ADD      #-136, R15
  MOV      RD4, RD14
  CMP/PZ   RD14
  BT       L00802555
  CMP/GT   #127, RD14
  BT       L00802555
  BRA      L00802554

L00802555:
  MOV      L00C0053A, RQ4
  MOV      RD14, RD5
  BSR      I_Error

L00802554:
  MOV      RD14, RD13
  MOV.L    RD13, snd_SfxVolume

L00E00544:
  ADD      #136, R15
  POP      RQ13
  POP      RQ14
  RET     

F_StartCast:
  PUSH     LR
  PUSH     R31
  PUSH     R30
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  PUSH     R9
  PUSH     R8
  ADD      #-39096, R15
  MOV      #-1, RD13
  MOV      #0, RD12
  MOV      castorder, RQ11
  SHAD     RD12, #4, RQ10
  ADD      RQ11, RQ10
  MOV.L    @(RQ10, 8), RD9
  MOV      mobjinfo, RQ8
  MOV      #92, RD7
  MUL.L    RD9, RD7
  STS      R0, RQ31
  ADD      RQ8, RQ31
  MOV.L    @(RQ31, 12), RD9
  MOV      states, RQ30
  MOV      #40, RD7
  MUL.L    RD9, RD7
  STS      R0, RQ11
  ADD      RQ30, RQ11
  MOV      RQ11, RQ8
  MOV.L    @(RQ8, 8), RD30
  MOV      #0, RD7
  MOV.L    RD7, castdeath
  MOV      #2, RD7
  MOV.L    RD7, finalestage
  MOV      #0, RD7
  MOV.L    RD7, castframes
  MOV      #0, RD7
  MOV.L    RD7, castonmelee
  MOV      #0, RD7
  MOV.L    RD7, castattacking
  MOV.L    RD13, wipegamestate
  MOV.L    RD12, castnum
  MOV.Q    RQ8, caststate
  MOV.L    RD30, casttics
  MOV      #63, RD4
  MOV      #1, RD5
  BSR      S_ChangeMusic

L00E00545:
  ADD      #39096, R15
  POP      RQ8
  POP      RQ9
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ30
  POP      RQ31
  RET     

F_CastTicker:
  PUSH     LR
  BSR      L00C00193
  ADD      #-39048, R15
  MOV.L    casttics, RD11
  ADD      #-1, RD11
  MOV.L    RD11, casttics
  MOV.L    casttics, RD10
  CMP/PL   RD10
  BT       L00800A8F
  BRA      L00E00546

L00800A8F:
  MOV.Q    caststate, RQ9
  MOV.L    @(RQ9, 8), RD14
  CMP/EQ   #-1, RD14
  BT       L00800A91
  MOV.Q    caststate, RQ8
  MOV.L    @(RQ8, 24), RD14
  CMP/EQ   RD14
  BT       L00800A91
  BRA      L00800A90

L00800A91:
  MOV.L    castnum, RD31
  ADD      #1, RD31
  MOV      #0, RD30
  MOV      castorder, RQ29
  EXTS.L   RD31
  SHAD     RD31, #4, RQ28
  ADD      RQ29, RQ28
  MOV.Q    @RQ28, RQ27
  MOV      #0, RQ26
  MOV      RQ27, RQ25
  MOV      RQ26, RQ8
  MOV.Q    RQ8, @(R15, 200)
  MOV.L    RD31, castnum
  MOV.L    RD30, castdeath
  MOV.Q    RQ28, @(R15, 232)
  MOV.Q    RQ27, @(R15, 224)
  MOV.Q    RQ26, @(R15, 216)
  MOV.Q    RQ25, @(R15, 208)
  CMPQ/EQ  RQ8, RQ25
  BT       L00800A93
  MOV      #0, RD25
  MOV.L    RD25, castnum

L00800A93:
  MOV      castorder, RQ25
  MOV.L    castnum, RD26
  EXTS.L   RD26
  SHAD     RD26, #4, RQ27
  ADD      RQ25, RQ27
  MOV.L    @(RQ27, 8), RD14
  MOV      mobjinfo, RQ28
  MOV      #92, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ29
  ADD      RQ28, RQ29
  MOV.L    @(RQ29, 16), RD14
  MOV.Q    RQ29, @(R15, 192)
  MOV.Q    RQ27, @(R15, 232)
  CMP/EQ   RD14
  BT       L00800A94
  MOV      castorder, RQ25
  MOV.L    castnum, RD26
  EXTS.L   RD26
  SHAD     RD26, #4, RQ27
  ADD      RQ25, RQ27
  MOV.L    @(RQ27, 8), RD14
  MOV      mobjinfo, RQ28
  MOV      #92, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ29
  ADD      RQ28, RQ29
  MOV.L    @(RQ29, 16), RD14
  MOV      #0, RQ30
  MOV.Q    RQ30, @(R15, 224)
  MOV.Q    RQ29, @(R15, 192)
  MOV.Q    RQ27, @(R15, 232)
  MOV      RQ30, RQ4
  MOV      RD14, RD5
  BSR      S_StartSound

L00800A94:
  MOV      castorder, RQ25
  MOV.L    castnum, RD26
  EXTS.L   RD26
  SHAD     RD26, #4, RQ27
  ADD      RQ25, RQ27
  MOV.L    @(RQ27, 8), RD14
  MOV      mobjinfo, RQ28
  MOV      #92, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ29
  ADD      RQ28, RQ29
  MOV.L    @(RQ29, 12), RD14
  MOV      states, RQ30
  MOV      #40, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ31
  ADD      RQ30, RQ31
  MOV      RQ31, RQ8
  MOV      #0, RD9
  MOV.L    RD9, castframes
  MOV.Q    RQ8, caststate
  MOV.Q    RQ31, @(R15, 224)
  MOV.Q    RQ29, @(R15, 192)
  MOV.Q    RQ27, @(R15, 232)
  BRA      L00800A95

L00800A90:
  MOV      states, RQ25
  MOV      #154, RD26
  EXTS.L   RD26
  MOV      #40, RD7
  MUL.L    RD26, RD7
  STS      R0, RQ27
  ADD      RQ25, RQ27
  MOV.Q    caststate, RQ29
  MOV      RQ29, RQ28
  MOV      RQ27, RQ30
  MOV.Q    RQ30, @(R15, 208)
  MOV.Q    RQ28, @(R15, 216)
  MOV.Q    RQ27, @(R15, 224)
  CMPQ/EQ  RQ30, RQ28
  BT       L00800A96
  BRA      L00800A97

L00800A96:
  MOV.Q    caststate, RQ25
  MOV.L    @(RQ25, 24), RD13
  MOV      states, RQ26
  MOV      #40, RD7
  MUL.L    RD13, RD7
  STS      R0, RQ27
  ADD      RQ26, RQ27
  MOV      RQ27, RQ25
  MOV.L    castframes, RD28
  ADD      #1, RD28
  MOV.L    RD28, castframes
  MOV.Q    RQ27, @(R15, 224)
  MOV.Q    RQ25, caststate
  CMP/GE   #454, RD13
  BT       L00800AB4
  CMP/GE   #340, RD13
  BT       L00800AB5
  CMP/EQ   #154, RD13
  BT       L00800A99
  CMP/EQ   #185, RD13
  BT       L00800A9A
  CMP/EQ   #218, RD13
  BT       L00800A9B
  CMP/EQ   #256, RD13
  BT       L00800A9C
  CMP/EQ   #336, RD13
  BT       L00800A9D
  CMP/EQ   #338, RD13
  BT       L00800A9E
  BRA      L00800AB3

L00800AB5:
  CMP/EQ   #340, RD13
  BT       L00800A9F
  CMP/EQ   #377, RD13
  BT       L00800AA2
  CMP/EQ   #380, RD13
  BT       L00800AA1
  CMP/EQ   #383, RD13
  BT       L00800AA0
  CMP/EQ   #417, RD13
  BT       L00800AA3
  CMP/EQ   #418, RD13
  BT       L00800AA4
  CMP/EQ   #419, RD13
  BT       L00800AA5
  BRA      L00800AB3

L00800AB4:
  CMP/GE   #616, RD13
  BT       L00800AB6
  CMP/EQ   #454, RD13
  BT       L00800AA6
  CMP/EQ   #486, RD13
  BT       L00800AA7
  CMP/EQ   #505, RD13
  BT       L00800AAA
  CMP/EQ   #538, RD13
  BT       L00800AA8
  CMP/EQ   #567, RD13
  BT       L00800AA9
  CMP/EQ   #590, RD13
  BT       L00800AAB
  BRA      L00800AB3

L00800AB6:
  CMP/EQ   #616, RD13
  BT       L00800AAC
  CMP/EQ   #617, RD13
  BT       L00800AAD
  CMP/EQ   #648, RD13
  BT       L00800AAE
  CMP/EQ   #685, RD13
  BT       L00800AAF
  CMP/EQ   #687, RD13
  BT       L00800AB0
  CMP/EQ   #689, RD13
  BT       L00800AB1
  CMP/EQ   #710, RD13
  BT       L00800AB2
  BRA      L00800AB3

L00800A99:
  MOV      #4, RD12
  BRA      L00800A98

L00800A9A:
  MOV      #1, RD12
  BRA      L00800A98

L00800A9B:
  MOV      #2, RD12
  BRA      L00800A98

L00800A9C:
  MOV      #54, RD12
  BRA      L00800A98

L00800A9D:
  MOV      #56, RD12
  BRA      L00800A98

L00800A9E:
  MOV      #53, RD12
  BRA      L00800A98

L00800A9F:
  MOV      #107, RD12
  BRA      L00800A98

L00800AA0:

L00800AA1:

L00800AA2:
  MOV      #16, RD12
  BRA      L00800A98

L00800AA3:

L00800AA4:

L00800AA5:
  MOV      #2, RD12
  BRA      L00800A98

L00800AA6:
  MOV      #55, RD12
  BRA      L00800A98

L00800AA7:
  MOV      #52, RD12
  BRA      L00800A98

L00800AA8:

L00800AA9:

L00800AAA:
  MOV      #16, RD12
  BRA      L00800A98

L00800AAB:
  MOV      #51, RD12
  BRA      L00800A98

L00800AAC:

L00800AAD:
  MOV      #2, RD12
  BRA      L00800A98

L00800AAE:
  MOV      #8, RD12
  BRA      L00800A98

L00800AAF:

L00800AB0:

L00800AB1:
  MOV      #14, RD12
  BRA      L00800A98

L00800AB2:
  MOV      #51, RD12
  BRA      L00800A98

L00800AB3:
  MOV      #0, RD12
  BRA      L00800A98

L00800A98:
  CMP/EQ   RD12
  BT       L00800AB7
  MOV      #0, RQ25
  MOV.Q    RQ25, @(R15, 224)
  MOV      RQ25, RQ4
  MOV      RD12, RD5
  BSR      S_StartSound

L00800AB7:

L00800A95:
  MOV.L    castframes, RD7
  CMP/EQ   #12, RD7
  BT       L00800AB8
  MOV      #1, RD6
  MOV.L    RD6, castattacking
  MOV.L    castonmelee, RD5
  CMP/EQ   RD5
  BT       L00800AB9
  MOV      castorder, RQ25
  MOV.L    castnum, RD26
  EXTS.L   RD26
  SHAD     RD26, #4, RQ27
  ADD      RQ25, RQ27
  MOV.L    @(RQ27, 8), RD14
  MOV      mobjinfo, RQ28
  MOV      #92, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ29
  ADD      RQ28, RQ29
  MOV.L    @(RQ29, 40), RD14
  MOV      states, RQ30
  MOV      #40, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ31
  ADD      RQ30, RQ31
  MOV      RQ31, RQ8
  MOV.Q    RQ8, caststate
  MOV.Q    RQ31, @(R15, 224)
  MOV.Q    RQ29, @(R15, 192)
  MOV.Q    RQ27, @(R15, 232)
  BRA      L00800ABA

L00800AB9:
  MOV      castorder, RQ25
  MOV.L    castnum, RD26
  EXTS.L   RD26
  SHAD     RD26, #4, RQ27
  ADD      RQ25, RQ27
  MOV.L    @(RQ27, 8), RD14
  MOV      mobjinfo, RQ28
  MOV      #92, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ29
  ADD      RQ28, RQ29
  MOV.L    @(RQ29, 44), RD14
  MOV      states, RQ30
  MOV      #40, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ31
  ADD      RQ30, RQ31
  MOV      RQ31, RQ8
  MOV.Q    RQ8, caststate
  MOV.Q    RQ31, @(R15, 224)
  MOV.Q    RQ29, @(R15, 192)
  MOV.Q    RQ27, @(R15, 232)

L00800ABA:
  MOV.L    castonmelee, RD25
  XOR      #1, RD25
  MOV      states, RQ26
  MOV      #0, RD27
  EXTS.L   RD27
  MOV      #40, RD7
  MUL.L    RD27, RD7
  STS      R0, RQ28
  ADD      RQ26, RQ28
  MOV.Q    caststate, RQ30
  MOV      RQ30, RQ29
  MOV      RQ28, RQ31
  MOV.Q    RQ31, @(R15, 208)
  MOV.Q    RQ29, @(R15, 216)
  MOV.Q    RQ28, @(R15, 224)
  MOV.L    RD25, castonmelee
  CMPQ/EQ  RQ31, RQ29
  BT       L00800ABB
  MOV.L    castonmelee, RD4
  CMP/EQ   RD4
  BT       L00800ABC
  MOV      castorder, RQ25
  MOV.L    castnum, RD26
  EXTS.L   RD26
  SHAD     RD26, #4, RQ27
  ADD      RQ25, RQ27
  MOV.L    @(RQ27, 8), RD14
  MOV      mobjinfo, RQ28
  MOV      #92, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ29
  ADD      RQ28, RQ29
  MOV.L    @(RQ29, 40), RD14
  MOV      states, RQ30
  MOV      #40, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ31
  ADD      RQ30, RQ31
  MOV      RQ31, RQ8
  MOV.Q    RQ8, caststate
  MOV.Q    RQ31, @(R15, 224)
  MOV.Q    RQ29, @(R15, 192)
  MOV.Q    RQ27, @(R15, 232)
  BRA      L00800ABD

L00800ABC:
  MOV      castorder, RQ25
  MOV.L    castnum, RD26
  EXTS.L   RD26
  SHAD     RD26, #4, RQ27
  ADD      RQ25, RQ27
  MOV.L    @(RQ27, 8), RD14
  MOV      mobjinfo, RQ28
  MOV      #92, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ29
  ADD      RQ28, RQ29
  MOV.L    @(RQ29, 44), RD14
  MOV      states, RQ30
  MOV      #40, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ31
  ADD      RQ30, RQ31
  MOV      RQ31, RQ8
  MOV.Q    RQ8, caststate
  MOV.Q    RQ31, @(R15, 224)
  MOV.Q    RQ29, @(R15, 192)
  MOV.Q    RQ27, @(R15, 232)

L00800ABD:

L00800ABB:

L00800AB8:
  MOV.L    castattacking, RD3
  CMP/EQ   RD3
  BT       L00800ABE
  MOV.L    castframes, RD2
  CMP/EQ   #24, RD2
  BT       L00800AC0
  MOV      castorder, RQ25
  MOV.L    castnum, RD26
  EXTS.L   RD26
  SHAD     RD26, #4, RQ27
  ADD      RQ25, RQ27
  MOV.L    @(RQ27, 8), RD14
  MOV      mobjinfo, RQ28
  MOV      #92, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ29
  ADD      RQ28, RQ29
  MOV.L    @(RQ29, 12), RD14
  MOV      states, RQ30
  MOV      #40, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ31
  ADD      RQ30, RQ31
  MOV.Q    caststate, RQ9
  MOV      RQ9, RQ8
  MOV      RQ31, RQ10
  MOV.Q    RQ10, @(R15, 208)
  MOV.Q    RQ8, @(R15, 216)
  MOV.Q    RQ31, @(R15, 224)
  MOV.Q    RQ29, @(R15, 192)
  MOV.Q    RQ27, @(R15, 232)
  CMPQ/EQ  RQ10, RQ8
  BT       L00800AC0
  BRA      L00800ABF

L00800AC0:

L00800A97:
  MOV      #0, RD25
  MOV      #0, RD26
  MOV      castorder, RQ27
  MOV.L    castnum, RD28
  EXTS.L   RD28
  SHAD     RD28, #4, RQ29
  ADD      RQ27, RQ29
  MOV.L    @(RQ29, 8), RD14
  MOV      mobjinfo, RQ30
  MOV      #92, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ31
  ADD      RQ30, RQ31
  MOV.L    @(RQ31, 12), RD14
  MOV      states, RQ8
  MOV      #40, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ9
  ADD      RQ8, RQ9
  MOV      RQ9, RQ10
  MOV.Q    RQ10, caststate
  MOV.Q    RQ9, @(R15, 224)
  MOV.Q    RQ31, @(R15, 192)
  MOV.Q    RQ29, @(R15, 232)
  MOV.L    RD26, castframes
  MOV.L    RD25, castattacking

L00800ABF:

L00800ABE:
  MOV.Q    caststate, RQ23
  MOV.L    @(RQ23, 8), RD22
  MOV.L    RD22, casttics
  MOV.L    casttics, RD21
  CMP/EQ   #-1, RD21
  BT       L00800AC2
  MOV      #15, RD20
  MOV.L    RD20, casttics

L00800AC2:

L00E00546:
  ADD      #39048, R15
  BRA      L00C00194

F_CastResponder:
  PUSH     LR
  BSR      L00C00193
  ADD      #-168, R15
  MOV.Q    RQ4, @(R15, 40)
  MOV.Q    @(R15, 40), RQ13
  MOV.L    @RQ13, RD14
  CMP/EQ   RD14
  BT       L00800AC3
  MOV      #0, RD2
  BRA      L00E00547

L00800AC3:
  MOV.L    castdeath, RD12
  CMP/EQ   RD12
  BT       L00800AC4
  MOV      #1, RD2
  BRA      L00E00547

L00800AC4:
  MOV      #1, RD11
  MOV      castorder, RQ10
  MOV.L    castnum, RD9
  SHAD     RD9, #4, RQ8
  ADD      RQ10, RQ8
  MOV.L    @(RQ8, 8), RD14
  MOV      mobjinfo, RQ31
  MOV      #92, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ30
  ADD      RQ31, RQ30
  MOV.L    @(RQ30, 48), RD14
  MOV      states, RQ29
  MOV      #40, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ28
  ADD      RQ29, RQ28
  MOV      RQ28, RQ27
  MOV.L    @(RQ27, 8), RD26
  MOV      #0, RD25
  MOV      #0, RD12
  SHAD     RD9, #4, RQ8
  ADD      RQ10, RQ8
  MOV.L    @(RQ8, 8), RD14
  MOV      #92, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ30
  ADD      RQ31, RQ30
  MOV.L    @(RQ30, 56), RD14
  MOV.L    RD12, castattacking
  MOV.L    RD11, castdeath
  MOV.Q    RQ8, @(R15, 16)
  MOV.Q    RQ30, @(R15, 8)
  MOV.Q    RQ28, @R15
  MOV.Q    RQ27, caststate
  MOV.L    RD26, casttics
  MOV.L    RD25, castframes
  CMP/EQ   RD14
  BT       L00800AC5
  MOV      castorder, RQ25
  MOV.L    castnum, RD26
  EXTS.L   RD26
  SHAD     RD26, #4, RQ27
  ADD      RQ25, RQ27
  MOV.L    @(RQ27, 8), RD14
  MOV      mobjinfo, RQ28
  MOV      #92, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ29
  ADD      RQ28, RQ29
  MOV.L    @(RQ29, 56), RD14
  MOV      #0, RQ30
  MOV.Q    RQ30, @R15
  MOV.Q    RQ29, @(R15, 8)
  MOV.Q    RQ27, @(R15, 16)
  MOV      RQ30, RQ4
  MOV      RD14, RD5
  BSR      S_StartSound

L00800AC5:
  MOV      #1, RD2

L00E00547:
  ADD      #168, R15
  BRA      L00C00194

F_CastDrawer:
  PUSH     LR
  BSR      L00C002C5
  ADD      #-128, R15
  MOV      L00C0055F, RQ4
  MOV      #101, RD5
  BSR      W_CacheLumpName
  MOV      RQ2, RQ14
  MOV      #0, RD4
  MOV      #0, RD5
  MOV      #0, RD6
  MOV      RQ14, RQ7
  BSR      V_DrawPatch
  MOV      castorder, RQ12
  MOV.L    castnum, RD11
  SHAD     RD11, #4, RQ10
  ADD      RQ12, RQ10
  MOV.Q    @RQ10, RQ14
  MOV      RQ14, RQ4
  BSR      F_CastPrint
  MOV.Q    caststate, RQ9
  MOV.L    @RQ9, RD13
  MOV.Q    sprites, RQ8
  SHAD     RD13, #4, RQ14
  ADD      RQ8, RQ14
  MOV      RQ14, RQ31
  MOV.Q    @(RQ31, 8), RQ14
  MOV.L    @(RQ9, 4), RD13
  AND      RD13, #32767, RD30
  EXTS.L   RD30
  MOV      #28, RD7
  MUL.L    RD30, RD7
  STS      R0, RQ29
  ADD      RQ14, RQ29
  MOV      RQ29, RQ28
  ADD      RQ28, #4, RQ14
  MOV.W    @RQ14, RD11
  ADD      RQ28, #20, RQ14
  MOVU.B   @RQ14, RD13
  MOV      RD13, RD12
  MOV.L    firstspritelump, RD10
  ADD      RD11, RD10, RD13
  MOV.L    RD12, @(R15, 48)
  MOV.L    RD11, @(R15, 52)
  MOV.Q    RQ31, @(R15, 64)
  MOV.Q    RQ28, @(R15, 56)
  MOV      RD13, RD4
  MOV      #101, RD5
  BSR      W_CacheLumpNum
  MOV      RQ2, RQ28
  MOV.Q    RQ28, @(R15, 40)
  CMP/EQ   RD12
  BT       L00800AD2
  MOV      #160, RD4
  MOV      #170, RD5
  MOV      #0, RD6
  MOV.Q    @(R15, 40), RQ7
  BSR      V_DrawPatchFlipped
  BRA      L00800AD3

L00800AD2:
  MOV      #160, RD4
  MOV      #170, RD5
  MOV      #0, RD6
  MOV.Q    @(R15, 40), RQ7
  BSR      V_DrawPatch

L00800AD3:

L00E00548:
  ADD      #128, R15
  BRA      L00C002C6

F_StartFinale:
  PUSH     LR
  BSR      L00C00193
  ADD      #-24, R15
  MOV      #0, RD14
  MOV      #2, RD13
  MOV      #0, RD12
  MOV      #0, RD11
  MOV.L    RD14, gameaction
  MOV.L    RD13, gamestate
  MOV.L    RD12, viewactive
  MOV.L    RD11, automapactive
  MOV.L    gamemode, RD10
  CMP/EQ   RD10
  BT       L00800A58
  MOV.L    gamemode, RD9
  CMP/EQ   #1, RD9
  BT       L00800A59
  MOV.L    gamemode, RD8
  CMP/EQ   #3, RD8
  BT       L00800A5A
  MOV.L    gamemode, RD31
  CMP/EQ   #2, RD31
  BT       L00800A5B
  BRA      L00800A5C

L00800A58:

L00800A59:

L00800A5A:
  MOV      #31, RD4
  MOV      #1, RD5
  BSR      S_ChangeMusic
  MOV.L    gameepisode, RD30
  CMP/EQ   #1, RD30
  BT       L00800A5E
  MOV.L    gameepisode, RD29
  CMP/EQ   #2, RD29
  BT       L00800A5F
  MOV.L    gameepisode, RD28
  CMP/EQ   #3, RD28
  BT       L00800A60
  MOV.L    gameepisode, RD27
  CMP/EQ   #4, RD27
  BT       L00800A61
  BRA      L00800A62

L00800A5E:
  MOV      L00C00563, RQ25
  MOV      RQ25, RQ26
  MOV.Q    e1text, RQ6
  MOV      RQ6, RQ7
  MOV.Q    RQ26, finaleflat
  MOV.Q    RQ7, finaletext
  BRA      L00800A5D

L00800A5F:
  MOV      L00C00564, RQ4
  MOV      RQ4, RQ5
  MOV.Q    e2text, RQ2
  MOV      RQ2, RQ3
  MOV.Q    RQ5, finaleflat
  MOV.Q    RQ3, finaletext
  BRA      L00800A5D

L00800A60:
  MOV      L00C00565, RQ22
  MOV      RQ22, RQ23
  MOV.Q    e3text, RQ20
  MOV      RQ20, RQ21
  MOV.Q    RQ23, finaleflat
  MOV.Q    RQ21, finaletext
  BRA      L00800A5D

L00800A61:
  MOV      L00C00566, RQ18
  MOV      RQ18, RQ19
  MOV.Q    e4text, RQ16
  MOV      RQ16, RQ17
  MOV.Q    RQ19, finaleflat
  MOV.Q    RQ17, finaletext
  BRA      L00800A5D

L00800A62:
  BRA      L00800A5D

L00800A5D:
  BRA      L00800A57

L00800A5B:
  MOV      #65, RD4
  MOV      #1, RD5
  BSR      S_ChangeMusic
  MOV.L    gamemap, RD16
  CMP/EQ   #6, RD16
  BT       L00800A64
  MOV.L    gamemap, RD16
  CMP/EQ   #11, RD16
  BT       L00800A65
  MOV.L    gamemap, RD16
  CMP/EQ   #20, RD16
  BT       L00800A66
  MOV.L    gamemap, RD16
  CMP/EQ   #30, RD16
  BT       L00800A67
  MOV.L    gamemap, RD16
  CMP/EQ   #15, RD16
  BT       L00800A68
  MOV.L    gamemap, RD16
  CMP/EQ   #31, RD16
  BT       L00800A69
  BRA      L00800A6A

L00800A64:
  MOV      L00C00567, RQ17
  MOV      RQ17, RQ16
  MOV.Q    c1text, RQ19
  MOV      RQ19, RQ18
  MOV.Q    RQ18, finaletext
  MOV.Q    RQ16, finaleflat
  BRA      L00800A63

L00800A65:
  MOV      L00C00568, RQ17
  MOV      RQ17, RQ16
  MOV.Q    c2text, RQ19
  MOV      RQ19, RQ18
  MOV.Q    RQ18, finaletext
  MOV.Q    RQ16, finaleflat
  BRA      L00800A63

L00800A66:
  MOV      L00C00569, RQ17
  MOV      RQ17, RQ16
  MOV.Q    c3text, RQ19
  MOV      RQ19, RQ18
  MOV.Q    RQ18, finaletext
  MOV.Q    RQ16, finaleflat
  BRA      L00800A63

L00800A67:
  MOV      L00C0056A, RQ17
  MOV      RQ17, RQ16
  MOV.Q    c4text, RQ19
  MOV      RQ19, RQ18
  MOV.Q    RQ18, finaletext
  MOV.Q    RQ16, finaleflat
  BRA      L00800A63

L00800A68:
  MOV      L00C0056B, RQ17
  MOV      RQ17, RQ16
  MOV.Q    c5text, RQ19
  MOV      RQ19, RQ18
  MOV.Q    RQ18, finaletext
  MOV.Q    RQ16, finaleflat
  BRA      L00800A63

L00800A69:
  MOV      L00C0056C, RQ17
  MOV      RQ17, RQ16
  MOV.Q    c6text, RQ19
  MOV      RQ19, RQ18
  MOV.Q    RQ18, finaletext
  MOV.Q    RQ16, finaleflat
  BRA      L00800A63

L00800A6A:
  BRA      L00800A63

L00800A63:
  BRA      L00800A57

L00800A5C:
  MOV      #65, RD4
  MOV      #1, RD5
  BSR      S_ChangeMusic
  MOV      L00C0056D, RQ17
  MOV      RQ17, RQ16
  MOV.Q    c1text, RQ19
  MOV      RQ19, RQ18
  MOV.Q    RQ18, finaletext
  MOV.Q    RQ16, finaleflat
  BRA      L00800A57

L00800A57:
  MOV      #0, RD16
  MOV      #0, RD17
  MOV.L    RD17, finalecount
  MOV.L    RD16, finalestage

L00E00549:
  ADD      #24, R15
  BRA      L00C00194

F_Responder:
  PUSH     LR
  PUSH     R14
  PUSH     R13
  ADD      #-136, R15
  MOV.Q    RQ4, @(R15, 8)
  MOV.L    finalestage, RD13
  CMP/EQ   #2, RD13
  BT       L00800A6B
  MOV.Q    @(R15, 8), RQ4
  BSR      F_CastResponder
  MOV      RD2, RD14
  MOV      RD14, RD2
  BRA      L00E0054A

L00800A6B:
  MOV      #0, RD2

L00E0054A:
  ADD      #136, R15
  POP      RQ13
  POP      RQ14
  RET     

F_Ticker:
  PUSH     LR
  BSR      L00C00193
  ADD      #-392, R15
  MOV.L    gamemode, RD12
  CMP/EQ   #2, RD12
  BT       L00800A6C
  MOV.L    finalecount, RD11
  CMP/GT   #50, RD11
  BT       L00800A6C
  BRA      L00800A6D

L00800A6D:
  MOV      #0, RD14

L00800A6F:
  MOV      #4, RD10
  CMP/GE   RD10, RD14
  BT       L00800A71
  MOV      players, RQ9
  EXTS.L   RD14
  MOV      #320, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ8
  ADD      RQ9, RQ8
  ADD      RQ8, #12, RQ31
  MOV.B    @(RQ31, 7), RD30
  EXTU.B   RD30, RD30
  MOV.L    RD30, @(R15, 28)
  CMP/EQ   #0, RD30
  BT       L00800A72
  BRA      L00800A71

L00800A72:

L00800A70:
  ADD      #1, RD14
  BRA      L00800A6F

L00800A71:
  MOV      #4, RD29
  CMP/GE   RD29, RD14
  BT       L00800A73
  MOV.L    gamemap, RD28
  CMP/EQ   #30, RD28
  BT       L00800A74
  BSR      F_StartCast
  BRA      L00800A75

L00800A74:
  MOV      #8, RD27
  MOV.L    RD27, gameaction

L00800A75:

L00800A73:

L00800A6C:
  MOV.L    finalecount, RD26
  ADD      #1, RD26
  MOV.L    RD26, finalecount
  MOV.L    finalestage, RD25
  CMP/EQ   #2, RD25
  BT       L00800A76
  BSR      F_CastTicker
  BRA      L00E0054B

L00800A76:
  MOV.L    gamemode, RD7
  CMP/EQ   #2, RD7
  BT       L00800A77
  BRA      L00E0054B

L00800A77:
  MOV.L    finalestage, RD6
  CMP/EQ   RD6
  BT       L00800A78
  MOV.Q    finaletext, RQ4
  BSR      strlen
  MOV      RQ2, RQ13
  MOV      #3, RQ7
  MUL.L    RQ13, RQ7, RQ25
  ADD      RQ25, #250, RQ13
  MOV.L    finalecount, RQ25
  EXTS.L   RQ25, RQ25
  MOV      RQ13, RQ26
  MOV.Q    RQ26, @R15
  MOV.Q    RQ25, @(R15, 8)
  CMPQ/HI  RQ26, RQ25
  BT       L00800A78
  BRA      L00800A79

L00800A79:
  MOV      #0, RD5
  MOV      #1, RD4
  MOV      #-1, RD3
  MOV.L    RD5, finalecount
  MOV.L    RD4, finalestage
  MOV.L    RD3, wipegamestate
  MOV.L    gameepisode, RD2
  CMP/EQ   #3, RD2
  BT       L00800A7B
  MOV      #30, RD4
  BSR      S_StartMusic

L00800A7B:

L00800A78:

L00E0054B:
  ADD      #392, R15
  BRA      L00C00194

HU_Init:
  PUSH     LR
  BSR      L00C003FF
  ADD      #-120, R15
  MOV      #36, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 24)
  MOV      french_shiftxform, RQ9
  MOV      RQ9, RQ10
  MOV.Q    RQ10, shiftxform
  MOV      L00C00573, RQ4
  BSR      W_CheckNumForName
  MOV      RD2, RD14
  CMP/PZ   RD14
  BT       L0080243D
  MOV      #33, RD12
  MOV      #0, RD13

L0080243E:
  CMP/GE   #63, RD13
  BT       L00802440
  MOV      RD12, RD14
  ADD      #1, RD12
  MOV      #36, RQ4
  ADD      R15, RQ4
  MOV      L00C00574, RQ5
  MOV      RD14, RD6
  BSR      sprintf
  MOV      #36, RQ4
  ADD      R15, RQ4
  MOV      #1, RD5
  BSR      W_CacheLumpName
  MOV      RQ2, RQ11
  MOV      hu_font, RQ8
  EXTS.L   RD13
  MOV.Q    RQ11, @(RQ8, RD13)

L0080243F:
  ADD      #1, RD13
  BRA      L0080243E

L00802440:
  BRA      L00802441

L0080243D:
  MOV      L00C00576, RQ4
  BSR      W_CheckNumForName
  MOV      RD2, RD14
  CMP/PZ   RD14
  BT       L00802442
  MOV      #1, RD12
  MOV      #0, RD13

L00802443:
  CMP/GE   #59, RD13
  BT       L00802445
  MOV      RD12, RD14
  ADD      #1, RD12
  MOV      #36, RQ4
  ADD      R15, RQ4
  MOV      L00C00577, RQ5
  MOV      RD14, RD6
  BSR      sprintf
  MOV      #36, RQ4
  ADD      R15, RQ4
  MOV      #1, RD5
  BSR      W_CacheLumpName
  MOV      RQ2, RQ11
  MOV      hu_font, RQ31
  EXTS.L   RD13
  MOV.Q    RQ11, @(RQ31, RD13)

L00802444:
  ADD      #1, RD13
  BRA      L00802443

L00802445:

L00802442:

L00802441:

L00E0054C:
  ADD      #120, R15
  BRA      L00C00404

HU_Start:
  PUSH     LR
  BSR      L00C00193
  ADD      #-632, R15
  MOV.L    TU0ADE0A50/headsupactive, RD13
  CMP/EQ   RD13
  BT       L00802446
  BSR      HU_Stop

L00802446:
  MOV      players, RQ12
  MOV.L    consoleplayer, RD11
  MOV      #320, RD7
  MUL.L    RD11, RD7
  STS      R0, RQ10
  ADD      RQ12, RQ10
  MOV      RQ10, RQ9
  MOV      #0, RD8
  MOV      #0, RD31
  MOV      #0, RD30
  MOV      #0, RD29
  MOV      TU0ADE0A50/message_on, RQ10
  MOV      TU0ADE0A50/w_message, RQ27
  MOV      RQ27, RQ28
  MOV.Q    RQ10, @(R15, 40)
  MOV.Q    RQ9, TU0ADE0A50/plr
  MOV.L    RD8, TU0ADE0A50/message_on
  MOV.L    RD31, message_dontfuckwithme
  MOV.L    RD30, TU0ADE0A50/message_nottobefuckedwith
  MOV.L    RD29, chat_on
  MOV.Q    RQ28, @(R15, 32)
  MOV      RQ28, RQ4
  MOV      #0, RD5
  MOV      #0, RD6
  MOV      #1, RD7
  MOV      hu_font, RQ20
  MOV      #33, RD21
  MOV      RQ10, RQ22
  BSR      HUlib_initSText
  MOV      hu_font, RQ26
  MOV.Q    @RQ26, RQ10
  MOV.W    @(RQ10, 2), RD14
  MOV      #167, RD13
  SUB      RD13, RD14, RD25
  MOV      TU0ADE0A50/w_title, RQ27
  MOV      RQ27, RQ10
  MOV.Q    RQ10, @(R15, 40)
  MOV.L    RD25, @(R15, 24)
  MOV      RQ10, RQ4
  MOV      #0, RD5
  MOV      RD25, RD6
  MOV      hu_font, RQ7
  MOV      #33, RD20
  BSR      HUlib_initTextLine
  MOV.L    gamemode, RD7
  CMP/EQ   RD7
  BT       L00802448
  MOV.L    gamemode, RD6
  CMP/EQ   #1, RD6
  BT       L00802449
  MOV.L    gamemode, RD5
  CMP/EQ   #3, RD5
  BT       L0080244A
  MOV.L    gamemode, RD4
  CMP/EQ   #4, RD4
  BT       L0080244B
  MOV.L    gamemode, RD3
  CMP/EQ   #2, RD3
  BT       L0080244C
  BRA      L0080244D

L00802448:

L00802449:

L0080244A:

L0080244B:
  MOV.L    gameepisode, RD2
  SUB      RD2, #1, RD14
  MOV      RD14, RD23
  MOV      #9, RD22
  MUL.L    RD22, RD23, RD23
  MOV.L    gamemap, RD21
  ADD      RD23, RD21, RD14
  SUB      RD14, #1, RD23
  MOV      mapnames, RQ20
  EXTS.L   RD23
  MOV.Q    @(RQ20, RD23), RQ19
  MOV.L    RD23, @(R15, 24)
  MOV.Q    RQ19, @(R15, 48)
  BRA      L00802447

L0080244C:

L0080244D:
  MOV.L    gamemap, RD18
  SUB      RD18, #1, RD14
  MOV      mapnames2, RQ17
  EXTS.L   RD14
  MOV.Q    @(RQ17, RD14), RQ16
  MOV.Q    RQ16, @(R15, 48)
  BRA      L00802447

L00802447:

L0080244E:
  MOV.Q    @(R15, 48), RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   RD14
  BT       L0080244F
  MOV.Q    @(R15, 48), RQ26
  MOV      RQ26, RQ25
  ADD      #1, RQ26
  MOV.B    @RQ25, RD14
  MOV      TU0ADE0A50/w_title, RQ27
  MOV      RQ27, RQ25
  EXTS.B   RD14, RD28
  MOV.L    RD28, @(R15, 24)
  MOV.Q    RQ26, @(R15, 48)
  MOV.Q    RQ25, @(R15, 40)
  MOV      RQ25, RQ4
  MOV      RD28, RD5
  BSR      HUlib_addCharToTextLine
  BRA      L0080244E

L0080244F:
  MOV      chat_on, RQ25
  MOV      hu_font, RQ26
  MOV.Q    @RQ26, RQ27
  MOV.W    @(RQ27, 2), RD28
  ADD      RD28, #1, RD29
  SHAD     RD29, #0, RD28
  MOV      RD28, RD29
  MOV      TU0ADE0A50/w_chat, RQ30
  MOV      RQ30, RQ27
  MOV.L    RD29, @(R15, 20)
  MOV.L    RD28, @(R15, 24)
  MOV.Q    RQ27, @(R15, 32)
  MOV.Q    RQ25, @(R15, 40)
  MOV      RQ27, RQ4
  MOV      #0, RD5
  MOV      RD29, RD6
  MOV      hu_font, RQ7
  MOV      #33, RD20
  MOV      RQ25, RQ21
  BSR      HUlib_initIText
  MOV      #0, RD31
  MOV.L    RD31, @(R15, 60)

L00802450:
  MOV.L    @(R15, 60), RD16
  MOV      #4, RD17
  CMP/GE   RD17, RD16
  BT       L00802452
  MOV      TU0ADE0A50/always_off, RQ25
  MOV      TU0ADE0A50/w_inputbuffer, RQ26
  MOV.L    @(R15, 60), RD27
  EXTS.L   RD27
  MOV      #136, RD7
  MUL.L    RD27, RD7
  STS      R0, RQ28
  ADD      RQ26, RQ28
  MOV      #0, RQ29
  MOV.Q    RQ29, @(R15, 8)
  MOV.Q    RQ28, @(R15, 32)
  MOV.Q    RQ25, @(R15, 40)
  MOV      RQ28, RQ4
  MOV      #0, RD5
  MOV      #0, RD6
  MOV      RQ29, RQ7
  MOV      #0, RD20
  MOV      RQ25, RQ21
  BSR      HUlib_initIText

L00802451:
  MOV.L    @(R15, 60), RD16
  ADD      #1, RD16
  MOV.L    RD16, @(R15, 60)
  BRA      L00802450

L00802452:
  MOV      #1, RD16
  MOV.L    RD16, TU0ADE0A50/headsupactive

L00E0054D:
  ADD      #632, R15
  BRA      L00C00194

HU_Responder:
  PUSH     LR
  BSR      L00C00193
  ADD      #-296, R15
  MOV.Q    RQ4, @(R15, 64)
  MOV      #0, RD14
  MOV      RD14, RD9
  MOV      #0, RD8
  MOV      #0, RD11
  MOV.L    RD9, @(R15, 52)
  MOV.L    RD8, @(R15, 48)

L0080247A:
  MOV      #4, RD31
  CMP/GE   RD31, RD11
  BT       L0080247C
  MOV      playeringame, RQ30
  EXTS.L   RD11
  MOV.L    @(RQ30, RD11), RD14
  MOV.L    @(R15, 48), RD29
  ADD      RD14, RD29
  MOV.L    RD29, @(R15, 48)

L0080247B:
  ADD      #1, RD11
  BRA      L0080247A

L0080247C:
  MOV.Q    @(R15, 64), RQ28
  MOV.L    @(RQ28, 4), RD14
  CMP/EQ   #182, RD14
  BT       L0080247D
  MOV.Q    @(R15, 64), RQ27
  MOV.L    @RQ27, RD14
  MOV      #0, RD26
  CMP/EQ   RD26, RD14
  MOVT     RQ7
  MOV      RQ7, RD25
  MOV.L    RD25, shiftdown
  MOV      RD26, RD2
  BRA      L00E0054E
  BRA      L0080247E

L0080247D:
  MOV.Q    @(R15, 64), RQ7
  MOV.L    @(RQ7, 4), RD14
  CMP/EQ   #184, RD14
  BT       L00802480
  MOV.Q    @(R15, 64), RQ6
  MOV.L    @(RQ6, 4), RD14
  CMP/EQ   #184, RD14
  BT       L00802480
  BRA      L0080247F

L00802480:
  MOV.Q    @(R15, 64), RQ25
  MOV.L    @RQ25, RD14
  MOV      #0, RD26
  CMP/EQ   RD26, RD14
  MOVT     RQ7
  MOV      RQ7, RD27
  MOV.L    RD27, altdown
  MOV      RD26, RD2
  BRA      L00E0054E

L0080247F:

L0080247E:
  MOV.Q    @(R15, 64), RQ5
  MOV.L    @RQ5, RD14
  CMP/EQ   RD14
  BT       L00802482
  MOV      #0, RD2
  BRA      L00E0054E

L00802482:
  MOV.L    chat_on, RD4
  CMP/EQ   RD4
  BT       L00802483
  MOV.Q    @(R15, 64), RQ3
  MOV.L    @(RQ3, 4), RD14
  CMP/EQ   #13, RD14
  BT       L00802484
  MOV      #1, RD2
  MOV      #140, RD23
  MOV      #1, RD22
  MOV.L    RD2, TU0ADE0A50/message_on
  MOV.L    RD23, TU0ADE0A50/message_counter
  MOV.L    RD22, @(R15, 52)
  BRA      L00802485

L00802484:
  MOV.L    netgame, RD21
  CMP/EQ   #0, RD21
  BT       L00802486
  MOV.Q    @(R15, 64), RQ20
  MOV.L    @(RQ20, 4), RD14
  CMP/EQ   #116, RD14
  BT       L00802486
  BRA      L00802487

L00802487:
  MOV      #1, RD25
  MOV      #1, RD26
  MOV      TU0ADE0A50/w_chat, RQ28
  MOV      RQ28, RQ27
  MOV.Q    RQ27, @(R15, 32)
  MOV.L    RD26, @(R15, 52)
  MOV.L    RD25, chat_on
  MOV      RQ27, RQ4
  BSR      HUlib_resetIText
  MOV      #5, RD14
  MOV      RD14, RD4
  BSR      HU_queueChatChar
  BRA      L00802489

L00802486:
  MOV.L    netgame, RD19
  CMP/EQ   #0, RD19
  BT       L0080248A
  MOV.L    @(R15, 48), RD18
  CMP/GT   #2, RD18
  BT       L0080248A
  BRA      L0080248B

L0080248B:
  MOV      #0, RD11

L0080248D:
  MOV      #4, RD17
  CMP/GE   RD17, RD11
  BT       L0080248F
  MOV.Q    @(R15, 64), RQ16
  MOV.L    @(RQ16, 4), RD14
  MOV      destination_keys, RQ17
  EXTS.L   RD11
  MOV.B    @(RQ17, RD11), RD10
  CMP/EQ   RD10, RD14
  BT       L00802490
  MOV      playeringame, RQ16
  EXTS.L   RD11
  MOV.L    @(RQ16, RD11), RD14
  CMP/EQ   RD14
  BT       L00802491
  MOV.L    consoleplayer, RD16
  CMP/EQ   RD16, RD11
  BT       L00802491
  BRA      L00802492

L00802492:
  MOV      #1, RD25
  MOV      #1, RD26
  MOV      TU0ADE0A50/w_chat, RQ28
  MOV      RQ28, RQ27
  MOV.Q    RQ27, @(R15, 32)
  MOV.L    RD26, @(R15, 52)
  MOV.L    RD25, chat_on
  MOV      RQ27, RQ4
  BSR      HUlib_resetIText
  ADD      RD11, #1, RD14
  EXTS.B   RD14, RD10
  MOV      RD10, RD4
  BSR      HU_queueChatChar
  BRA      L0080248F
  BRA      L00802494

L00802491:
  MOV.L    consoleplayer, RD16
  CMP/EQ   RD16, RD11
  BT       L00802495
  MOV.L    num_nobrainers, RD16
  ADD      #1, RD16
  MOV.L    RD16, num_nobrainers
  MOV.L    num_nobrainers, RD17
  MOV      #3, RD18
  CMP/GE   RD18, RD17
  BT       L00802496
  MOV      L00C00593, RQ16
  MOV.Q    TU0ADE0A50/plr, RQ17
  MOV.Q    RQ16, @(RQ17, 224)
  BRA      L00802497

L00802496:
  MOV.L    num_nobrainers, RD16
  MOV      #6, RD17
  CMP/GE   RD17, RD16
  BT       L00802498
  MOV      L00C00594, RQ16
  MOV.Q    TU0ADE0A50/plr, RQ17
  MOV.Q    RQ16, @(RQ17, 224)
  BRA      L00802499

L00802498:
  MOV.L    num_nobrainers, RD16
  MOV      #9, RD17
  CMP/GE   RD17, RD16
  BT       L0080249A
  MOV      L00C00595, RQ16
  MOV.Q    TU0ADE0A50/plr, RQ17
  MOV.Q    RQ16, @(RQ17, 224)
  BRA      L0080249B

L0080249A:
  MOV.L    num_nobrainers, RD16
  MOV      #32, RD17
  CMP/GE   RD17, RD16
  BT       L0080249C
  MOV      L00C00596, RQ16
  MOV.Q    TU0ADE0A50/plr, RQ17
  MOV.Q    RQ16, @(RQ17, 224)
  BRA      L0080249D

L0080249C:
  MOV      L00C00597, RQ16
  MOV.Q    TU0ADE0A50/plr, RQ17
  MOV.Q    RQ16, @(RQ17, 224)

L0080249D:

L0080249B:

L00802499:

L00802497:

L00802495:

L00802494:

L00802490:

L0080248E:
  ADD      #1, RD11
  BRA      L0080248D

L0080248F:

L0080248A:

L00802489:

L00802485:
  BRA      L0080249E

L00802483:
  MOV.Q    @(R15, 64), RQ16
  MOV.L    @(RQ16, 4), RD10
  EXTU.B   RD10, RD13
  MOV      RD13, RD12
  MOV.L    altdown, RD17
  CMP/EQ   #0, RD17
  BT       L0080249F
  MOV      RD12, RD10
  MOV      #48, RD13
  SUB      RD10, RD13, RD12
  CMP/GT   #9, RD12
  BT       L008024A0
  MOV      #0, RD2
  BRA      L00E0054E

L008024A0:
  MOV      chat_macros, RQ25
  EXTS.L   RD12
  MOV.Q    @(RQ25, RD12), RQ26
  MOV      #13, RD10
  MOV.Q    RQ26, @(R15, 56)
  MOV      RD10, RD4
  BSR      HU_queueChatChar

L008024A1:
  MOV.Q    @(R15, 56), RQ16
  MOV.B    @RQ16, RD10
  CMP/EQ   RD10
  BT       L008024A2
  MOV.Q    @(R15, 56), RQ26
  MOV      RQ26, RQ25
  ADD      #1, RQ26
  MOV.B    @RQ25, RD10
  EXTS.B   RD10, RD13
  MOV.Q    RQ26, @(R15, 56)
  MOV.Q    RQ25, @(R15, 32)
  MOV      RD13, RD4
  BSR      HU_queueChatChar
  BRA      L008024A1

L008024A2:
  MOV      #13, RD13
  MOV      RD13, RD4
  BSR      HU_queueChatChar
  MOV      #0, RD25
  MOV      chat_macros, RQ26
  EXTS.L   RD12
  MOV.Q    @(RQ26, RD12), RQ27
  MOV.Q    RQ27, @(R15, 32)
  MOV.L    RD25, chat_on
  MOV      lastmessage, RQ4
  MOV      RQ27, RQ5
  BSR      strcpy
  MOV      lastmessage, RQ28
  MOV.Q    TU0ADE0A50/plr, RQ29
  MOV.Q    RQ28, @(RQ29, 224)
  MOV      #1, RD30
  MOV.L    RD30, @(R15, 52)
  BRA      L008024A3

L0080249F:
  MOV      RD12, RD4
  BSR      ForeignTranslation
  MOV      RD2, RD13
  EXTU.B   RD13, RD12
  MOV.L    shiftdown, RD25
  CMP/EQ   #0, RD25
  BT       L008024A5
  CMP/GE   #97, RD12
  BT       L008024A4
  CMP/GT   #122, RD12
  BT       L008024A4
  BRA      L008024A5

L008024A5:
  MOV.Q    shiftxform, RQ16
  EXTS.L   RD12
  MOV.B    @(RQ16, RD12), RD13
  EXTU.B   RD13, RD17
  MOV      RD17, RD12

L008024A4:
  MOV      TU0ADE0A50/w_chat, RQ26
  MOV      RQ26, RQ25
  MOV.Q    RQ25, @(R15, 32)
  MOV      RQ25, RQ4
  MOV      RD12, RD5
  BSR      HUlib_keyInIText
  MOV      RD2, RD27
  MOV.L    RD27, @(R15, 52)
  CMP/EQ   #0, RD27
  BT       L008024A8
  EXTS.B   RD12, RD13
  MOV      RD13, RD4
  BSR      HU_queueChatChar

L008024A8:
  CMP/EQ   #13, RD12
  BT       L008024A9
  MOV      #0, RD16
  MOV      TU0ADE0A50/w_chat, RQ17
  MOV      RQ17, RQ18
  MOV.L    @(RQ18, 104), RD13
  MOV.Q    RQ18, @(R15, 8)
  MOV.L    RD16, chat_on
  CMP/EQ   RD13
  BT       L008024AA
  MOV      TU0ADE0A50/w_chat, RQ25
  MOV      RQ25, RQ26
  ADD      RQ26, #20, RQ27
  MOV.Q    RQ27, @(R15, 32)
  MOV.Q    RQ26, @(R15, 8)
  MOV      lastmessage, RQ4
  MOV      RQ27, RQ5
  BSR      strcpy
  MOV      lastmessage, RQ28
  MOV.Q    TU0ADE0A50/plr, RQ29
  MOV.Q    RQ28, @(RQ29, 224)

L008024AA:
  BRA      L008024AB

L008024A9:
  CMP/EQ   #27, RD12
  BT       L008024AC
  MOV      #0, RD16
  MOV.L    RD16, chat_on

L008024AC:

L008024AB:

L008024A3:

L0080249E:
  MOV.L    @(R15, 52), RD2

L00E0054E:
  ADD      #296, R15
  BRA      L00C00194

HU_Ticker:
  PUSH     LR
  BSR      L00C00193
  ADD      #-1416, R15
  MOV.Q    TU0ADE0A50/plr, RQ10
  MOV      #0, RQ9
  CMPQ/EQ  RQ9, RQ10
  BT       L00802454
  BRA      L00E0054F

L00802454:
  MOV.L    TU0ADE0A50/message_counter, RD8
  CMP/EQ   RD8
  BT       L00802455
  MOV.L    TU0ADE0A50/message_counter, RD31
  ADD      #-1, RD31
  MOV.L    RD31, TU0ADE0A50/message_counter
  MOV.L    TU0ADE0A50/message_counter, RD30
  CMP/EQ   #0, RD30
  BT       L00802455
  BRA      L00802456

L00802456:
  MOV      #0, RD29
  MOV      #0, RD28
  MOV.L    RD29, TU0ADE0A50/message_on
  MOV.L    RD28, TU0ADE0A50/message_nottobefuckedwith

L00802455:
  MOV.L    showMessages, RD27
  CMP/EQ   #0, RD27
  BT       L00802459
  MOV.L    message_dontfuckwithme, RD26
  CMP/EQ   #0, RD26
  BT       L00802459
  BRA      L00802458

L00802459:
  MOV.Q    TU0ADE0A50/plr, RQ25
  MOV.Q    @(RQ25, 224), RQ14
  MOV      #0, RQ7
  CMPQ/EQ  RQ7, RQ14
  BT       L0080245D
  MOV.L    TU0ADE0A50/message_nottobefuckedwith, RD6
  CMP/EQ   RD6
  BT       L0080245D
  BRA      L0080245C

L0080245D:
  MOV.Q    TU0ADE0A50/plr, RQ5
  MOV.Q    @(RQ5, 224), RQ14
  MOV      #0, RQ4
  CMPQ/EQ  RQ4, RQ14
  BT       L0080245B
  MOV.L    message_dontfuckwithme, RD3
  CMP/EQ   RD3
  BT       L0080245B
  BRA      L0080245C

L0080245C:
  MOV.Q    TU0ADE0A50/plr, RQ25
  MOV.Q    @(RQ25, 224), RQ14
  MOV      TU0ADE0A50/w_message, RQ27
  MOV      RQ27, RQ26
  MOV      #0, RQ28
  MOV.Q    RQ28, @(R15, 248)
  MOV.Q    RQ26, @(R15, 256)
  MOV      RQ26, RQ4
  MOV      RQ28, RQ5
  MOV      RQ14, RQ6
  BSR      HUlib_addMessageToSText
  MOV      #0, RQ14
  MOV.Q    TU0ADE0A50/plr, RQ2
  MOV.Q    RQ14, @(RQ2, 224)
  MOV      #1, RD23
  MOV      #140, RD22
  MOV.L    message_dontfuckwithme, RD20
  MOV      RD20, RD21
  MOV      #0, RD20
  MOV.L    RD23, TU0ADE0A50/message_on
  MOV.L    RD22, TU0ADE0A50/message_counter
  MOV.L    RD21, TU0ADE0A50/message_nottobefuckedwith
  MOV.L    RD20, message_dontfuckwithme

L0080245B:

L00802458:
  MOV.L    netgame, RD19
  CMP/EQ   #0, RD19
  BT       L00802460
  MOV      #0, RD13

L00802461:
  MOV      #4, RD18
  CMP/GE   RD18, RD13
  BT       L00802463
  MOV      playeringame, RQ17
  EXTS.L   RD13
  MOV.L    @(RQ17, RD13), RD12
  CMP/EQ   RD12
  BT       L00802464
  BRA      L00802462

L00802464:
  MOV.L    consoleplayer, RD16
  CMP/EQ   RD16, RD13
  BT       L00802465
  MOV      players, RQ25
  EXTS.L   RD13
  MOV      #320, RD7
  MUL.L    RD13, RD7
  STS      R0, RQ26
  ADD      RQ25, RQ26
  ADD      RQ26, #12, RQ27
  MOV.B    @(RQ27, 6), RD12
  EXTU.B   RD12, RD12
  EXTS.B   RD12, RD28
  MOV      RD28, RD11
  MOV.L    RD28, @(R15, 220)
  MOV.Q    RQ27, @(R15, 224)
  MOV.Q    RQ26, @(R15, 232)
  CMP/EQ   RD12
  BT       L00802465
  BRA      L00802466

L00802466:
  CMP/GT   #5, RD11
  BT       L00802468
  MOV      TU0ADE0A50/chat_dest, RQ16
  EXTS.L   RD13
  MOV.B    RD11, @(RQ16, RD13)
  BRA      L00802469

L00802468:
  CMP/GE   #97, RD11
  BT       L0080246A
  CMP/GT   #122, RD11
  BT       L0080246A
  BRA      L0080246B

L0080246B:
  EXTU.B   RD11, RD12
  MOV.Q    shiftxform, RQ16
  MOV.B    @(RQ16, RD12), RD17
  EXTS.B   RD17, RD11
  MOV.L    RD17, @(R15, 220)

L0080246A:
  MOV      TU0ADE0A50/w_inputbuffer, RQ25
  EXTS.L   RD13
  MOV      #136, RD7
  MUL.L    RD13, RD7
  STS      R0, RQ14
  ADD      RQ25, RQ14
  EXTU.B   RD11, RD12
  MOV      RQ14, RQ4
  MOV      RD12, RD5
  BSR      HUlib_keyInIText
  MOV      RD2, RD26
  MOV.L    RD26, @(R15, 276)
  CMP/EQ   #0, RD26
  BT       L0080246D
  CMP/EQ   #13, RD11
  BT       L0080246D
  BRA      L0080246E

L0080246E:
  MOV      TU0ADE0A50/w_inputbuffer, RQ25
  EXTS.L   RD13
  MOV      #136, RD7
  MUL.L    RD13, RD7
  STS      R0, RQ26
  ADD      RQ25, RQ26
  MOV      RQ26, RQ27
  MOV.L    @(RQ27, 104), RD12
  MOV.Q    RQ27, @(R15, 200)
  MOV.Q    RQ26, @(R15, 208)
  CMP/EQ   RD12
  BT       L00802470
  MOV      TU0ADE0A50/chat_dest, RQ16
  EXTS.L   RD13
  MOV.B    @(RQ16, RD13), RD12
  MOV.L    consoleplayer, RD17
  ADD      RD17, #1, RD18
  MOV.L    RD18, @(R15, 220)
  CMP/EQ   RD18, RD12
  BT       L00802471
  MOV      TU0ADE0A50/chat_dest, RQ16
  EXTS.L   RD13
  MOV.B    @(RQ16, RD13), RD12
  CMP/EQ   #5, RD12
  BT       L00802471
  BRA      L00802470

L00802471:
  MOV      TU0ADE0A50/w_inputbuffer, RQ25
  EXTS.L   RD13
  MOV      #136, RD7
  MUL.L    RD13, RD7
  STS      R0, RQ26
  ADD      RQ25, RQ26
  MOV      RQ26, RQ27
  ADD      RQ27, #20, RQ14
  MOV      player_names, RQ28
  EXTS.L   RD13
  MOV.Q    @(RQ28, RD13), RQ29
  MOV      TU0ADE0A50/w_message, RQ31
  MOV      RQ31, RQ30
  MOV.Q    RQ30, @(R15, 248)
  MOV.Q    RQ29, @(R15, 256)
  MOV.Q    RQ27, @(R15, 200)
  MOV.Q    RQ26, @(R15, 208)
  MOV      RQ30, RQ4
  MOV      RQ29, RQ5
  MOV      RQ14, RQ6
  BSR      HUlib_addMessageToSText
  MOV      #1, RD8
  MOV      #1, RD9
  MOV      #140, RD10
  MOV.L    RD10, TU0ADE0A50/message_counter
  MOV.L    RD9, TU0ADE0A50/message_on
  MOV.L    RD8, TU0ADE0A50/message_nottobefuckedwith
  MOV.L    gamemode, RD16
  CMP/EQ   #2, RD16
  BT       L00802474
  MOV      #0, RQ14
  MOV      RQ14, RQ4
  MOV      #108, RD5
  BSR      S_StartSound
  BRA      L00802475

L00802474:
  MOV      #0, RQ14
  MOV      RQ14, RQ4
  MOV      #87, RD5
  BSR      S_StartSound

L00802475:

L00802470:
  MOV      TU0ADE0A50/w_inputbuffer, RQ25
  EXTS.L   RD13
  MOV      #136, RD7
  MUL.L    RD13, RD7
  STS      R0, RQ14
  ADD      RQ25, RQ14
  MOV      RQ14, RQ4
  BSR      HUlib_resetIText

L0080246D:

L00802469:
  MOV      players, RQ25
  EXTS.L   RD13
  MOV      #320, RD7
  MUL.L    RD13, RD7
  STS      R0, RQ26
  ADD      RQ25, RQ26
  ADD      RQ26, #12, RQ27
  MOV      #0, RD28
  EXTU.B   RD28, RD28
  MOV.B    RD28, @(RQ27, 6)
  MOV.Q    RQ27, @(R15, 224)
  MOV.Q    RQ26, @(R15, 232)

L00802465:

L00802462:
  ADD      #1, RD13
  BRA      L00802461

L00802463:

L00802460:

L00E0054F:
  ADD      #1416, R15
  BRA      L00C00194

HU_Drawer:
  PUSH     LR
  BSR      L00C003ED
  ADD      #-112, R15
  MOV      TU0ADE0A50/w_message, RQ13
  MOV      RQ13, RQ14
  MOV      RQ14, RQ4
  BSR      HUlib_drawSText
  MOV      TU0ADE0A50/w_chat, RQ12
  MOV      RQ12, RQ14
  MOV      RQ14, RQ4
  BSR      HUlib_drawIText
  MOV.L    automapactive, RD11
  CMP/EQ   RD11
  BT       L00802453
  MOV      TU0ADE0A50/w_title, RQ10
  MOV      RQ10, RQ14
  MOV      RQ14, RQ4
  MOV      #0, RD5
  BSR      HUlib_drawTextLine

L00802453:

L00E00550:
  ADD      #112, R15
  BRA      L00C003EF

HU_dequeueChatChar:
  PUSH     LR
  BSR      L00C003FF
  ADD      #-88, R15
  MOV.L    TU0ADE0A50/head, RD14
  MOV.L    TU0ADE0A50/tail, RD13
  CMP/EQ   RD13, RD14
  BT       L00802478
  MOV      TU0ADE0A50/chatchars, RQ12
  MOV.L    TU0ADE0A50/tail, RD11
  MOV.B    @(RQ12, RD11), RD10
  ADD      RD11, #1, RD9
  AND      RD9, #127, RD11
  MOV.L    RD11, TU0ADE0A50/tail
  MOV.L    RD10, @(R15, 12)
  MOV.L    RD9, @(R15, 8)
  BRA      L00802479

L00802478:
  MOV      #0, RD8
  MOV      RD8, RD31
  MOV.L    RD8, @(R15, 8)
  MOV.L    RD31, @(R15, 12)

L00802479:
  MOV.L    @(R15, 12), RD2

L00E00551:
  ADD      #88, R15
  BRA      L00C00404

HU_Erase:
  PUSH     LR
  BSR      L00C001DF
  ADD      #-120, R15
  MOV      TU0ADE0A50/w_message, RQ13
  MOV      RQ13, RQ14
  MOV      RQ14, RQ4
  BSR      HUlib_eraseSText
  MOV      TU0ADE0A50/w_chat, RQ12
  MOV      RQ12, RQ14
  MOV      RQ14, RQ4
  BSR      HUlib_eraseIText
  MOV      TU0ADE0A50/w_title, RQ11
  MOV      RQ11, RQ14
  MOV      RQ14, RQ4
  BSR      HUlib_eraseTextLine

L00E00552:
  ADD      #120, R15
  BRA      L00C001E0

F_TextWrite:
  PUSH     LR
  BSR      L00C00193
  ADD      #-88, R15
  MOV.Q    finaleflat, RQ4
  MOV      #101, RD5
  BSR      W_CacheLumpName
  MOV      RQ2, RQ31
  MOV      screens, RQ30
  MOV.Q    @RQ30, RQ29
  MOV      #0, RD9
  MOV.Q    RQ31, @(R15, 56)
  MOV.Q    RQ29, @(R15, 48)

L00800A7C:
  CMP/GE   #200, RD9
  BT       L00800A7E
  MOV      #0, RD28
  MOV.L    RD28, @(R15, 44)

L00800A7F:
  MOV.L    @(R15, 44), RD27
  MOV      #5, RD26
  CMP/GE   RD26, RD27
  BT       L00800A81
  MOV      #0, RD12

L00800A82:
  CMP/GE   #64, RD12
  BT       L00800A84
  AND      RD9, #63, RD14
  SHAD     RD14, #6, RD8
  MOV.Q    @(R15, 56), RQ25
  LEA.B    @(RQ25, RD8), RQ11
  EXTS.L   RD12
  MOVU.B   @(RQ11, RD12), RD14
  MOV.Q    colormaps, RQ7
  MOVU.W   @(RQ7, RD14), RD8
  MOV.Q    @(R15, 48), RQ6
  EXTS.L   RD12
  MOVU.W   RD8, @(RQ6, RD12)

L00800A83:
  ADD      #1, RD12
  BRA      L00800A82

L00800A84:
  MOV.Q    @(R15, 48), RQ5
  ADD      #128, RQ5
  MOV.Q    RQ5, @(R15, 48)

L00800A80:
  MOV.L    @(R15, 44), RD4
  ADD      #1, RD4
  MOV.L    RD4, @(R15, 44)
  BRA      L00800A7F

L00800A81:

L00800A7D:
  ADD      #1, RD9
  BRA      L00800A7C

L00800A7E:
  MOV      #0, RD4
  MOV      #0, RD5
  MOV      #320, RD6
  MOV      #200, RD7
  BSR      V_MarkRect
  MOV      #10, RD10
  MOV      #10, RD25
  MOV.Q    finaletext, RQ27
  MOV      RQ27, RQ26
  MOV.L    finalecount, RD28
  SUB      RD28, #10, RD14
  MOV      RD14, RD29
  MOV      #0x55555556, RD7
  MOV      #21845, RD7
  DMULS    RD7, RD29
  STS      R1, RD7
  MOV      RD7, RD29
  MOV.L    RD29, @(R15, 40)
  MOV.Q    RQ26, @(R15, 32)
  MOV.L    RD25, @(R15, 28)
  MOV      #0, RD25
  CMP/GE   RD25, RD29
  BT       L00800A85
  MOV      #0, RD3
  MOV.L    RD3, @(R15, 40)

L00800A85:

L00800A86:
  MOV.L    @(R15, 40), RD2
  CMP/EQ   RD2
  BT       L00800A88
  MOV.Q    @(R15, 32), RQ23
  MOV      RQ23, RQ11
  ADD      #1, RQ23
  MOV.B    @RQ11, RD14
  MOV      RD14, RD13
  MOV.Q    RQ23, @(R15, 32)
  CMP/EQ   RD13
  BT       L00800A89
  BRA      L00800A88

L00800A89:
  CMP/EQ   #10, RD13
  BT       L00800A8A
  MOV      #10, RD10
  MOV.L    @(R15, 28), RD22
  ADD      #11, RD22
  MOV.L    RD22, @(R15, 28)
  BRA      L00800A87

L00800A8A:
  MOV      RD13, RD4
  BSR      toupper
  MOV      RD2, RD14
  SUB      RD14, #33, RD13
  CMP/PZ   RD13
  BT       L00800A8C
  CMP/GT   #63, RD13
  BT       L00800A8C
  BRA      L00800A8B

L00800A8C:
  ADD      #4, RD10
  BRA      L00800A87

L00800A8B:
  MOV      hu_font, RQ21
  EXTS.L   RD13
  MOV.Q    @(RQ21, RD13), RQ11
  MOV.W    @RQ11, RD12
  ADD      RD10, RD12, RD14
  CMP/GT   #320, RD14
  BT       L00800A8E
  BRA      L00800A88

L00800A8E:
  MOV      hu_font, RQ25
  EXTS.L   RD13
  MOV.Q    @(RQ25, RD13), RQ11
  MOV      RD10, RD4
  MOV.L    @(R15, 28), RD5
  MOV      #0, RD6
  MOV      RQ11, RQ7
  BSR      V_DrawPatch
  ADD      RD12, RD10

L00800A87:
  MOV.L    @(R15, 40), RD20
  ADD      #-1, RD20
  MOV.L    RD20, @(R15, 40)
  BRA      L00800A86

L00800A88:

L00E00553:
  ADD      #88, R15
  BRA      L00C00194

F_CastPrint:
  PUSH     LR
  BSR      L00C00193
  ADD      #-56, R15
  MOV.Q    RQ4, @(R15, 24)
  MOV.Q    @(R15, 24), RQ8
  MOV      RQ8, RQ12
  MOV      #0, RD10

L00800AC6:
  MOV      #0, RQ31
  CMPQ/EQ  RQ31, RQ12
  BT       L00800AC7
  MOV      RQ12, RQ11
  ADD      #1, RQ12
  MOV.B    @RQ11, RD13
  MOV      RD13, RD14
  CMP/EQ   RD14
  BT       L00800AC8
  BRA      L00800AC7

L00800AC8:
  MOV      RD14, RD4
  BSR      toupper
  MOV      RD2, RD13
  SUB      RD13, #33, RD14
  CMP/PZ   RD14
  BT       L00800ACA
  CMP/GT   #63, RD14
  BT       L00800ACA
  BRA      L00800AC9

L00800ACA:
  ADD      #4, RD10
  BRA      L00800AC6

L00800AC9:
  MOV      hu_font, RQ30
  EXTS.L   RD14
  MOV.Q    @(RQ30, RD14), RQ11
  MOV.W    @RQ11, RD29
  ADD      RD29, RD10
  MOV.L    RD29, @(R15, 20)
  BRA      L00800AC6

L00800AC7:
  SHAD     RD10, #-1, RD13
  MOV      #160, RD28
  SUB      RD28, RD13, RD9
  MOV.Q    @(R15, 24), RQ27
  MOV      RQ27, RQ12

L00800ACC:
  MOV      #0, RQ26
  CMPQ/EQ  RQ26, RQ12
  BT       L00800ACD
  MOV      RQ12, RQ11
  ADD      #1, RQ12
  MOV.B    @RQ11, RD13
  MOV      RD13, RD14
  CMP/EQ   RD14
  BT       L00800ACE
  BRA      L00800ACD

L00800ACE:
  MOV      RD14, RD4
  BSR      toupper
  MOV      RD2, RD13
  SUB      RD13, #33, RD14
  CMP/PZ   RD14
  BT       L00800AD0
  CMP/GT   #63, RD14
  BT       L00800AD0
  BRA      L00800ACF

L00800AD0:
  ADD      #4, RD9
  BRA      L00800ACC

L00800ACF:
  MOV      hu_font, RQ25
  EXTS.L   RD14
  MOV.Q    @(RQ25, RD14), RQ11
  MOV.W    @RQ11, RD26
  EXTS.L   RD14
  MOV.Q    @(RQ25, RD14), RQ11
  MOV.L    RD26, @(R15, 20)
  MOV      RD9, RD4
  MOV      #180, RD5
  MOV      #0, RD6
  MOV      RQ11, RQ7
  BSR      V_DrawPatch
  ADD      RD26, RD9
  BRA      L00800ACC

L00800ACD:

L00E00554:
  ADD      #56, R15
  BRA      L00C00194

V_DrawPatchFlipped:
  PUSH     LR
  BSR      L00C00193
  ADD      #-120, R15
  MOV      RD4, RD9
  MOV      RD5, RD8
  MOV.L    RD6, @(R15, 84)
  MOV      RQ7, RQ11
  MOV.W    @(RQ11, 6), RD14
  SUB      RD14, RD8
  MOV.W    @(RQ11, 4), RD14
  SUB      RD14, RD9
  CMP/PZ   RD9
  BT       L008022DB
  MOV.W    @RQ11, RD14
  ADD      RD9, RD14, RD12
  CMP/GT   #320, RD12
  BT       L008022DB
  BRA      L008022DE

L008022DE:
  CMP/PZ   RD8
  BT       L008022DB
  BRA      L008022DD

L008022DD:
  MOV.W    @(RQ11, 2), RD14
  ADD      RD8, RD14, RD12
  CMP/GT   #200, RD12
  BT       L008022DB
  BRA      L008022DC

L008022DC:
  MOV.L    @(R15, 84), RD31
  CMP/HI   #4, RD31
  BT       L008022DB
  BRA      L008022DA

L008022DB:
  BSR      __get_stderr
  MOV      RQ2, RQ13
  MOV.Q    @RQ13, RQ30
  MOV.Q    RQ30, @(R15, 8)
  MOV      RQ30, RQ4
  MOV      L00C005BF, RQ5
  MOV      RD9, RD6
  MOV      RD8, RD7
  BSR      fprintf
  MOV      L00C005C0, RQ4
  BSR      I_Error

L008022DA:
  MOV.L    @(R15, 84), RD29
  CMP/EQ   #0, RD29
  BT       L008022E0
  MOV.W    @(RQ11, 2), RD14
  MOV.W    @RQ11, RD12
  MOV      RD9, RD4
  MOV      RD8, RD5
  MOV      RD12, RD6
  MOV      RD14, RD7
  BSR      V_MarkRect

L008022E0:
  MOV      #0, RD28
  MOV      screens, RQ27
  MOV.L    @(R15, 84), RD26
  EXTS.L   RD26
  MOV.Q    @(RQ27, RD26), RQ13
  MOV      RD8, RD14
  MOV      #320, RD25
  MUL.L    RD25, RD14, RD14
  EXTS.L   RD14
  LEA.W    @(RQ13, RD14), RQ29
  EXTS.L   RD9
  LEA.W    @(RQ29, RD9), RQ30
  MOV.W    @RQ11, RD31
  MOV.L    RD31, @(R15, 36)
  MOV.Q    RQ30, @(R15, 56)
  MOV.Q    RQ29, @(R15, 8)
  MOV.L    RD28, @(R15, 64)

L008022E1:
  MOV.L    @(R15, 64), RD7
  MOV.L    @(R15, 36), RD6
  CMP/GT   RD7, RD6
  BT       L008022E3
  ADD      RQ11, #8, RQ13
  MOV.L    @(R15, 36), RD5
  SUB      RD5, #1, RD14
  MOV.L    @(R15, 64), RD4
  SUB      RD14, RD4, RD12
  EXTS.L   RD12
  MOV.L    @(RQ13, RD12), RD14
  LEA.B    @(RQ11, RD14), RQ13
  MOV      RQ13, RQ10

L008022E4:
  MOV.B    @RQ10, RD14
  EXTU.B   RD14, RD14
  CMP/EQ   #255, RD14
  BT       L008022E5
  ADD      RQ10, #3, RQ3
  MOV.B    @RQ10, RD14
  EXTU.B   RD14, RD14
  MOV      RD14, RD12
  MOV      #320, RD2
  MUL.L    RD2, RD12, RD12
  MOV.Q    @(R15, 56), RQ23
  EXTS.L   RD12
  LEA.W    @(RQ23, RD12), RQ22
  MOV.B    @(RQ10, 1), RD21
  EXTU.B   RD21, RD21
  MOV.Q    RQ3, @(R15, 40)
  MOV.Q    RQ22, @(R15, 48)
  MOV.L    RD21, @(R15, 68)

L008022E6:
  MOV.L    @(R15, 68), RD20
  MOV      RD20, RD14
  ADD      #-1, RD20
  MOV.L    RD20, @(R15, 68)
  CMP/EQ   RD14
  BT       L008022E7
  MOV.Q    @(R15, 40), RQ19
  MOV      RQ19, RQ13
  ADD      #1, RQ19
  MOVU.B   @RQ13, RD14
  MOV.Q    @(R15, 48), RQ18
  MOVU.W   RD14, @RQ18
  ADD      #640, RQ18
  MOV.Q    RQ19, @(R15, 40)
  MOV.Q    RQ18, @(R15, 48)
  BRA      L008022E6

L008022E7:
  MOV.B    @(RQ10, 1), RD14
  EXTU.B   RD14, RD14
  LEA.B    @(RQ10, RD14), RQ13
  ADD      RQ13, #4, RQ17
  MOV      RQ17, RQ10
  MOV.Q    RQ17, @(R15, 8)
  BRA      L008022E4

L008022E5:

L008022E2:
  MOV      RD9, RD14
  ADD      #1, RD9
  MOV.L    @(R15, 64), RD16
  MOV      RD16, RD14
  ADD      #1, RD16
  MOV.Q    @(R15, 56), RQ17
  MOV      RQ17, RQ13
  ADD      #2, RQ17
  MOV.Q    RQ17, @(R15, 56)
  MOV.L    RD16, @(R15, 64)
  BRA      L008022E1

L008022E3:

L00E00555:
  ADD      #120, R15
  BRA      L00C00194

F_DrawPatchCol:
  PUSH     LR
  BSR      L00C00193
  ADD      #-120, R15
  MOV.L    RD4, @(R15, 92)
  MOV.Q    RQ5, @(R15, 80)
  MOV.L    RD6, @(R15, 76)
  MOV.Q    @(R15, 80), RQ13
  ADD      RQ13, #8, RQ12
  MOV.L    @(R15, 76), RD11
  MOV.L    @(RQ12, RD11), RD14
  LEA.B    @(RQ13, RD14), RQ12
  MOV      RQ12, RQ10
  MOV      screens, RQ9
  MOV.Q    @RQ9, RQ12
  MOV.L    @(R15, 92), RD8
  LEA.W    @(RQ12, RD8), RQ31
  MOV.Q    RQ12, @(R15, 24)
  MOV.Q    RQ10, @(R15, 64)
  MOV.Q    RQ31, @(R15, 40)

L00800AD4:
  MOV.Q    @(R15, 64), RQ30
  MOV.B    @RQ30, RD14
  EXTU.B   RD14, RD14
  CMP/EQ   #255, RD14
  BT       L00800AD5
  MOV.Q    @(R15, 64), RQ29
  ADD      RQ29, #3, RQ28
  MOV.B    @RQ29, RD14
  EXTU.B   RD14, RD14
  MOV      RD14, RD27
  MOV      #320, RD26
  MUL.L    RD26, RD27, RD27
  MOV.Q    @(R15, 40), RQ25
  EXTS.L   RD27
  LEA.W    @(RQ25, RD27), RQ7
  MOV.B    @(RQ29, 1), RD6
  EXTU.B   RD6, RD6
  MOV.Q    RQ28, @(R15, 56)
  MOV.L    RD27, @(R15, 16)
  MOV.Q    RQ7, @(R15, 48)
  MOV.L    RD6, @(R15, 36)

L00800AD6:
  MOV.L    @(R15, 36), RD5
  MOV      RD5, RD14
  ADD      #-1, RD5
  MOV.L    RD5, @(R15, 36)
  CMP/EQ   RD14
  BT       L00800AD7
  MOV.Q    @(R15, 56), RQ3
  MOV      RQ3, RQ4
  ADD      #1, RQ3
  MOVU.B   @RQ4, RD14
  MOV.Q    colormaps, RQ2
  MOVU.W   @(RQ2, RD14), RD23
  MOV.Q    @(R15, 48), RQ22
  MOVU.W   RD23, @RQ22
  ADD      #640, RQ22
  MOV.Q    RQ4, @(R15, 24)
  MOV.Q    RQ3, @(R15, 56)
  MOV.L    RD23, @(R15, 16)
  MOV.Q    RQ22, @(R15, 48)
  BRA      L00800AD6

L00800AD7:
  MOV.Q    @(R15, 64), RQ21
  MOV.B    @(RQ21, 1), RD14
  EXTU.B   RD14, RD14
  LEA.B    @(RQ21, RD14), RQ20
  ADD      RQ20, #4, RQ19
  MOV      RQ19, RQ21
  MOV.Q    RQ21, @(R15, 64)
  MOV.Q    RQ20, @(R15, 24)
  BRA      L00800AD4

L00800AD5:

L00E00556:
  ADD      #120, R15
  BRA      L00C00194

F_BunnyScroll:
  PUSH     LR
  BSR      L00C00193
  ADD      #-88, R15
  MOV      #52, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 32)
  MOV      L00C005C3, RQ4
  MOV      #50, RD5
  BSR      W_CacheLumpName
  MOV      RQ2, RQ11
  MOV      L00C005C4, RQ4
  MOV      #50, RD5
  BSR      W_CacheLumpName
  MOV      RQ2, RQ10
  MOV.Q    RQ10, @(R15, 40)
  MOV      #0, RD4
  MOV      #0, RD5
  MOV      #320, RD6
  MOV      #200, RD7
  BSR      V_MarkRect
  MOV.L    finalecount, RD9
  SUB      RD9, #230, RD14
  SHAD     RD14, #-1, RD8
  MOV      #320, RD31
  SUB      RD31, RD8, RD12
  MOV.L    RD8, @(R15, 20)
  CMP/GT   #320, RD12
  BT       L00800AD8
  MOV      #320, RD12

L00800AD8:
  CMP/PZ   RD12
  BT       L00800AD9
  MOV      #0, RD12

L00800AD9:
  MOV      #0, RD13

L00800ADA:
  CMP/GE   #320, RD13
  BT       L00800ADC
  ADD      RD13, RD12, RD14
  CMP/GE   #320, RD14
  BT       L00800ADD
  ADD      RD13, RD12, RD14
  MOV      RD13, RD4
  MOV      RQ11, RQ5
  MOV      RD14, RD6
  BSR      F_DrawPatchCol
  BRA      L00800ADE

L00800ADD:
  ADD      RD13, RD12, RD14
  SUB      RD14, #320, RD30
  MOV.L    RD30, @(R15, 20)
  MOV      RD13, RD4
  MOV.Q    @(R15, 40), RQ5
  MOV      RD30, RD6
  BSR      F_DrawPatchCol

L00800ADE:

L00800ADB:
  ADD      #1, RD13
  BRA      L00800ADA

L00800ADC:
  MOV.L    finalecount, RD29
  MOV      #1130, RD28
  CMP/GE   RD28, RD29
  BT       L00800ADF
  BRA      L00E00557

L00800ADF:
  MOV.L    finalecount, RD27
  MOV      #1180, RD26
  CMP/GE   RD26, RD27
  BT       L00800AE0
  MOV      L00C005C5, RQ4
  MOV      #101, RD5
  BSR      W_CacheLumpName
  MOV      RQ2, RQ25
  MOV.Q    RQ25, @(R15, 8)
  MOV      #108, RD4
  MOV      #68, RD5
  MOV      #0, RD6
  MOV      RQ25, RQ7
  BSR      V_DrawPatch
  MOV      #0, RD26
  MOV.L    RD26, laststage
  BRA      L00E00557

L00800AE0:
  MOV.L    finalecount, RD25
  SUB      RD25, #1180, RD14
  MOV      RD14, RD26
  MOV      #0x33333334, RD7
  MOV      #13107, RD7
  DMULS    RD7, RD26
  STS      R1, RD7
  MOV      RD7, RD26
  MOV.L    RD26, @(R15, 28)
  CMP/GT   #6, RD26
  BT       L00800AE1
  MOV      #6, RD7
  MOV.L    RD7, @(R15, 28)

L00800AE1:
  MOV.L    @(R15, 28), RD6
  MOV.L    laststage, RD5
  CMP/GT   RD5, RD6
  BT       L00800AE2
  MOV      #0, RQ25
  MOV.Q    RQ25, @(R15, 8)
  MOV      RQ25, RQ4
  MOV      #1, RD5
  BSR      S_StartSound
  MOV.L    @(R15, 28), RD3
  MOV      RD3, RD4
  MOV.L    RD4, laststage

L00800AE2:
  MOV      #52, RQ4
  ADD      R15, RQ4
  MOV      L00C005C7, RQ5
  MOV.L    @(R15, 28), RD6
  BSR      sprintf
  MOV      #52, RQ4
  ADD      R15, RQ4
  MOV      #101, RD5
  BSR      W_CacheLumpName
  MOV      RQ2, RQ25
  MOV.Q    RQ25, @(R15, 8)
  MOV      #108, RD4
  MOV      #68, RD5
  MOV      #0, RD6
  MOV      RQ25, RQ7
  BSR      V_DrawPatch

L00E00557:
  ADD      #88, R15
  BRA      L00C00194

F_Drawer:
  PUSH     LR
  BSR      L00C003FF
  ADD      #-88, R15
  MOV.L    finalestage, RD13
  CMP/EQ   #2, RD13
  BT       L00800AE3
  BSR      F_CastDrawer
  BRA      L00E00558

L00800AE3:
  MOV.L    finalestage, RD12
  CMP/EQ   RD12
  BT       L00800AE4
  BSR      F_TextWrite
  BRA      L00800AE5

L00800AE4:
  MOV.L    gameepisode, RD11
  CMP/EQ   #1, RD11
  BT       L00800AE7
  MOV.L    gameepisode, RD10
  CMP/EQ   #2, RD10
  BT       L00800AE8
  MOV.L    gameepisode, RD9
  CMP/EQ   #3, RD9
  BT       L00800AE9
  MOV.L    gameepisode, RD8
  CMP/EQ   #4, RD8
  BT       L00800AEA
  BRA      L00800AE6

L00800AE7:
  MOV.L    gamemode, RD31
  CMP/EQ   #3, RD31
  BT       L00800AEB
  MOV      L00C005C9, RQ4
  MOV      #101, RD5
  BSR      W_CacheLumpName
  MOV      RQ2, RQ14
  MOV      #0, RD4
  MOV      #0, RD5
  MOV      #0, RD6
  MOV      RQ14, RQ7
  BSR      V_DrawPatch
  BRA      L00800AEC

L00800AEB:
  MOV      L00C005CA, RQ4
  MOV      #101, RD5
  BSR      W_CacheLumpName
  MOV      RQ2, RQ14
  MOV      #0, RD4
  MOV      #0, RD5
  MOV      #0, RD6
  MOV      RQ14, RQ7
  BSR      V_DrawPatch

L00800AEC:
  BRA      L00800AE6

L00800AE8:
  MOV      L00C005CB, RQ4
  MOV      #101, RD5
  BSR      W_CacheLumpName
  MOV      RQ2, RQ14
  MOV      #0, RD4
  MOV      #0, RD5
  MOV      #0, RD6
  MOV      RQ14, RQ7
  BSR      V_DrawPatch
  BRA      L00800AE6

L00800AE9:
  BSR      F_BunnyScroll
  BRA      L00800AE6

L00800AEA:
  MOV      L00C005CC, RQ4
  MOV      #101, RD5
  BSR      W_CacheLumpName
  MOV      RQ2, RQ14
  MOV      #0, RD4
  MOV      #0, RD5
  MOV      #0, RD6
  MOV      RQ14, RQ7
  BSR      V_DrawPatch
  BRA      L00800AE6

L00800AE6:

L00800AE5:

L00E00558:
  ADD      #88, R15
  BRA      L00C00404

M_Random:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  ADD      #-136, R15
  MOV.L    rndindex, RD13
  ADD      RD13, #1, RD14
  EXTU.B   RD14, RD13
  MOV      rndtable, RQ12
  MOVU.B   @(RQ12, RD13), RD14
  MOV.L    RD13, rndindex
  MOV      RD14, RD2

L00E00559:
  ADD      #136, R15
  BRA      L00C003F2

P_Random:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  ADD      #-136, R15
  MOV.L    prndindex, RD13
  ADD      RD13, #1, RD14
  EXTU.B   RD14, RD13
  MOV      rndtable, RQ12
  MOVU.B   @(RQ12, RD13), RD14
  MOV.L    RD13, prndindex
  MOV      RD14, RD2

L00E0055A:
  ADD      #136, R15
  BRA      L00C003F2

M_ClearRandom:
  ADD      #-144, R15
  MOV      #0, RD7
  MOV.L    RD7, prndindex
  MOV      #0, RD7
  MOV.L    RD7, rndindex

L00E0055B:
  ADD      #144, R15
  RTS     

wipe_StartScreen:
  PUSH     LR
  PUSH     R14
  PUSH     R13
  ADD      #-136, R15
  MOV.L    RD4, @(R15, 12)
  MOV.L    RD5, @(R15, 8)
  MOV.L    RD6, @(R15, 4)
  MOV.L    RD7, @R15
  MOV      screens, RQ14
  MOV.Q    @(RQ14, 16), RQ13
  MOV.Q    RQ13, TU0ADE001F/wipe_scr_start
  MOV.Q    TU0ADE001F/wipe_scr_start, RQ4
  BSR      I_ReadScreen
  MOV      #0, RD2

L00E0055C:
  ADD      #136, R15
  POP      RQ13
  POP      RQ14
  RET     

wipe_EndScreen:
  PUSH     LR
  PUSH     R14
  PUSH     R13
  ADD      #-136, R15
  MOV.L    RD4, @(R15, 12)
  MOV.L    RD5, @(R15, 8)
  MOV.L    RD6, @(R15, 4)
  MOV.L    RD7, @R15
  MOV      screens, RQ14
  MOV.Q    @(RQ14, 24), RQ13
  MOV.Q    RQ13, TU0ADE001F/wipe_scr_end
  MOV.Q    TU0ADE001F/wipe_scr_end, RQ4
  BSR      I_ReadScreen
  MOV.L    @(R15, 12), RD4
  MOV.L    @(R15, 8), RD5
  MOV      #0, RD6
  MOV.L    @(R15, 4), RD7
  MOV.L    @R15, RD20
  MOV.Q    TU0ADE001F/wipe_scr_start, RQ21
  BSR      V_DrawBlockScr
  MOV      #0, RD2

L00E0055D:
  ADD      #136, R15
  POP      RQ13
  POP      RQ14
  RET     

wipe_ScreenWipe:
  PUSH     LR
  BSR      L00C00193
  ADD      #-120, R15
  MOV.L    RD4, @(R15, 92)
  MOV.L    RD5, @(R15, 88)
  MOV.L    RD6, @(R15, 84)
  MOV.L    RD7, @(R15, 80)
  MOV.L    RD20, @(R15, 76)
  MOV.L    RD21, @(R15, 72)
  MOV      #96, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 56)
  MOV      wipe_initColorXForm, RQ13
  MOV      RQ13, RQ14
  MOV      #96, RQ12
  ADD      R15, RQ12
  MOV.Q    RQ14, @RQ12
  MOV      wipe_doColorXForm, RQ11
  MOV      RQ11, RQ14
  MOV.Q    RQ14, @(RQ12, 8)
  MOV      wipe_exitColorXForm, RQ10
  MOV      RQ10, RQ14
  MOV.Q    RQ14, @(RQ12, 16)
  MOV      wipe_initMelt, RQ9
  MOV      RQ9, RQ14
  MOV.Q    RQ14, @(RQ12, 24)
  MOV      wipe_doMelt, RQ8
  MOV      RQ8, RQ14
  MOV.Q    RQ14, @(RQ12, 32)
  MOV      wipe_exitMelt, RQ31
  MOV      RQ31, RQ14
  MOV.Q    RQ14, @(RQ12, 40)
  MOV.L    TU0ADE001F/go, RD30
  CMP/EQ   #0, RD30
  BT       L00800B14
  MOV      #1, RD29
  MOV      screens, RQ28
  MOV.Q    @RQ28, RQ27
  MOV.L    @(R15, 92), RD25
  MOV      RD25, RD26
  MOV      #3, RD30
  MUL.L    RD30, RD26, RD26
  MOV      #96, RQ31
  ADD      R15, RQ31
  EXTS.L   RD26
  MOV.Q    @(RQ31, RD26), RQ14
  MOV      RQ14, RQ8
  MOV.Q    RQ8, @(R15, 32)
  MOV.L    RD29, TU0ADE001F/go
  MOV.Q    RQ27, TU0ADE001F/wipe_scr
  MOV.L    RD26, @(R15, 44)
  MOV.L    @(R15, 80), RD4
  MOV.L    @(R15, 76), RD5
  MOV.L    @(R15, 72), RD6
  JSR      RQ8

L00800B14:
  MOV      #0, RD4
  MOV      #0, RD5
  MOV.L    @(R15, 80), RD6
  MOV.L    @(R15, 76), RD7
  BSR      V_MarkRect
  MOV.L    @(R15, 92), RD26
  MOV      RD26, RD25
  MOV      #3, RD27
  MUL.L    RD27, RD25, RD25
  ADD      RD25, #1, RD28
  MOV      #96, RQ29
  ADD      R15, RQ29
  EXTS.L   RD28
  MOV.Q    @(RQ29, RD28), RQ14
  MOV      RQ14, RQ30
  MOV.Q    RQ30, @(R15, 32)
  MOV.L    RD28, @(R15, 28)
  MOV.L    RD25, @(R15, 44)
  MOV.L    @(R15, 80), RD4
  MOV.L    @(R15, 76), RD5
  MOV.L    @(R15, 72), RD6
  MOV      RQ30, R1
  JSR      R1
  MOV      RD2, RD31
  MOV.L    RD31, @(R15, 68)
  CMP/EQ   #0, RD31
  BT       L00800B15
  MOV      #0, RD25
  MOV.L    @(R15, 92), RD27
  MOV      RD27, RD26
  MOV      #3, RD28
  MUL.L    RD28, RD26, RD26
  ADD      RD26, #2, RD29
  MOV      #96, RQ30
  ADD      R15, RQ30
  EXTS.L   RD29
  MOV.Q    @(RQ30, RD29), RQ14
  MOV      RQ14, RQ31
  MOV.Q    RQ31, @(R15, 32)
  MOV.L    RD29, @(R15, 28)
  MOV.L    RD26, @(R15, 44)
  MOV.L    RD25, TU0ADE001F/go
  MOV.L    @(R15, 80), RD4
  MOV.L    @(R15, 76), RD5
  MOV.L    @(R15, 72), RD6
  MOV      RQ31, R1
  JSR      R1

L00800B15:
  MOV.L    TU0ADE001F/go, RD25
  TST      RD25, RD25
  MOVT     RQ7
  MOV      RQ7, RD26
  MOV.L    RD26, @(R15, 44)
  MOV      RD26, RD2

L00E0055E:
  ADD      #120, R15
  BRA      L00C00194

wipe_shittyColMajorXform:
  PUSH     LR
  BSR      L00C00193
  ADD      #-88, R15
  MOV.Q    RQ4, @(R15, 56)
  MOV      RD5, RD8
  MOV.L    RD6, @(R15, 48)
  MOV.L    @(R15, 48), RD31
  MUL.L    RD8, RD31, RD14
  SHAD     RD14, #1, RD13
  MOV      #0, RQ9
  MOV      RD13, RD4
  MOV      #1, RD5
  MOV      RQ9, RQ6
  BSR      Z_Malloc
  MOV      RQ2, RQ10
  MOV      RQ10, RQ30
  MOV      #0, RD11
  MOV.Q    RQ30, @(R15, 40)

L00800AED:
  MOV.L    @(R15, 48), RD29
  CMP/GT   RD11, RD29
  BT       L00800AEF
  MOV      #0, RD12

L00800AF0:
  CMP/GT   RD12, RD8
  BT       L00800AF2
  MUL.L    RD11, RD8, RD14
  ADD      RD14, RD12, RD13
  MOV.Q    @(R15, 56), RQ28
  EXTS.L   RD13
  MOV.W    @(RQ28, RD13), RD14
  MOV.L    @(R15, 48), RD27
  MUL.L    RD12, RD27, RD13
  ADD      RD13, RD11, RD26
  MOV.Q    @(R15, 40), RQ25
  EXTS.L   RD26
  MOV.W    RD14, @(RQ25, RD26)

L00800AF1:
  ADD      #1, RD12
  BRA      L00800AF0

L00800AF2:

L00800AEE:
  ADD      #1, RD11
  BRA      L00800AED

L00800AEF:
  MOV.L    @(R15, 48), RD25
  MUL.L    RD8, RD25, RD14
  SHAD     RD14, #1, RD13
  EXTS.L   RD13, RQ26
  MOV.Q    RQ26, @R15
  MOV.Q    @(R15, 56), RQ4
  MOV.Q    @(R15, 40), RQ5
  MOV      RQ26, RQ6
  BSR      memcpy
  MOV.Q    @(R15, 40), RQ4
  BSR      Z_Free

L00E0055F:
  ADD      #88, R15
  BRA      L00C00194

wipe_initColorXForm:
  PUSH     LR
  BSR      L00C003ED
  ADD      #-128, R15
  MOV      RD4, RD11
  MOV      RD5, RD10
  MOV.L    RD6, @(R15, 20)
  MUL.L    RD11, RD10, RD14
  SHAD     RD14, #1, RD13
  EXTS.L   RD13, RQ12
  MOV.Q    TU0ADE001F/wipe_scr, RQ4
  MOV.Q    TU0ADE001F/wipe_scr_start, RQ5
  MOV      RQ12, RQ6
  BSR      memcpy
  MOV      #0, RD2

L00E00560:
  ADD      #128, R15
  BRA      L00C003EF

wipe_doColorXForm:
  PUSH     LR
  BSR      L00C00472
  ADD      #-72, R15
  MOV.L    RD4, @(R15, 28)
  MOV.L    RD5, @(R15, 24)
  MOV.L    RD6, @(R15, 20)
  MOV      #0, RD9
  MOV.Q    TU0ADE001F/wipe_scr, RQ8
  MOV      RQ8, RQ13
  MOV.Q    TU0ADE001F/wipe_scr_end, RQ31
  MOV      RQ31, RQ12

L00800AF3:
  MOV.L    @(R15, 28), RD30
  MOV.L    @(R15, 24), RD29
  MUL.L    RD30, RD29, RD14
  MOV.Q    TU0ADE001F/wipe_scr, RQ28
  EXTS.L   RD14
  LEA.W    @(RQ28, RD14), RQ27
  MOV.Q    RQ27, @(R15, 8)
  CMPQ/EQ  RQ27, RQ13
  BT       L00800AF4
  MOVU.W   @RQ13, RD14
  MOVU.W   @RQ12, RD11
  CMP/EQ   RD11, RD14
  BT       L00800AF5
  MOVU.W   @RQ13, RD14
  MOVU.W   @RQ12, RD11
  CMP/GT   RD11, RD14
  BT       L00800AF6
  MOVU.W   @RQ13, RD14
  MOV.L    @(R15, 20), RD7
  SUB      RD14, RD7, RD10
  MOVU.W   @RQ12, RD14
  CMP/GT   RD10, RD14
  BT       L00800AF7
  MOVU.W   @RQ12, RD14
  MOVU.W   RD14, @RQ13
  BRA      L00800AF8

L00800AF7:
  MOVU.W   RD10, @RQ13

L00800AF8:
  MOV      #1, RD9
  BRA      L00800AF9

L00800AF6:
  MOVU.W   @RQ13, RD14
  MOVU.W   @RQ12, RD11
  CMP/GT   RD14, RD11
  BT       L00800AFA
  MOVU.W   @RQ13, RD14
  MOV.L    @(R15, 20), RD6
  ADD      RD14, RD6, RD10
  MOVU.W   @RQ12, RD14
  CMP/GT   RD14, RD10
  BT       L00800AFB
  MOVU.W   @RQ12, RD14
  MOVU.W   RD14, @RQ13
  BRA      L00800AFC

L00800AFB:
  MOVU.W   RD10, @RQ13

L00800AFC:
  MOV      #1, RD9

L00800AFA:

L00800AF9:

L00800AF5:
  ADD      #2, RQ13
  ADD      #2, RQ12
  BRA      L00800AF3

L00800AF4:
  TST      RD9, RD9
  MOVT     RD14
  MOV      RD14, RD2

L00E00561:
  ADD      #72, R15
  BRA      L00C00476

wipe_exitColorXForm:
  ADD      #-160, R15
  MOV.L    RD4, @(R15, 12)
  MOV.L    RD5, @(R15, 8)
  MOV.L    RD6, @(R15, 4)
  MOV      #0, RD2

L00E00562:
  ADD      #160, R15
  RTS     

wipe_initMelt:
  PUSH     LR
  BSR      L00C00193
  ADD      #-88, R15
  MOV.L    RD4, @(R15, 60)
  MOV.L    RD5, @(R15, 56)
  MOV.L    RD6, @(R15, 52)
  MOV.L    @(R15, 60), RD9
  MOV.L    @(R15, 56), RD8
  MUL.L    RD9, RD8, RD14
  SHAD     RD14, #1, RD11
  EXTS.L   RD11, RQ10
  MOV.Q    TU0ADE001F/wipe_scr, RQ4
  MOV.Q    TU0ADE001F/wipe_scr_start, RQ5
  MOV      RQ10, RQ6
  BSR      memcpy
  SHAD     RD9, #-1, RD14
  MOV.Q    TU0ADE001F/wipe_scr_start, RQ31
  MOV      RQ31, RQ12
  MOV      RQ12, RQ4
  MOV      RD14, RD5
  MOV      RD8, RD6
  BSR      wipe_shittyColMajorXform
  SHAD     RD9, #-1, RD14
  MOV.Q    TU0ADE001F/wipe_scr_end, RQ30
  MOV      RQ30, RQ12
  MOV      RQ12, RQ4
  MOV      RD14, RD5
  MOV      RD8, RD6
  BSR      wipe_shittyColMajorXform
  SHAD     RD9, #2, RD14
  MOV      #0, RQ29
  MOV.Q    RQ29, @(R15, 16)
  MOV      RD14, RD4
  MOV      #1, RD5
  MOV      RQ29, RQ6
  BSR      Z_Malloc
  MOV      RQ2, RQ12
  MOV      RQ12, RQ28
  MOV.Q    RQ28, TU0ADE001F/y
  BSR      M_Random
  MOV      RD2, RD14
  AND      RD14, #15, RD27
  NEG      RD27, RD14
  MOV.Q    TU0ADE001F/y, RQ26
  MOV.L    RD14, @RQ26
  MOV      #1, RD13
  MOV.L    RD27, @(R15, 12)

L00800AFD:
  MOV.L    @(R15, 60), RD25
  CMP/GT   RD13, RD25
  BT       L00800AFF
  BSR      M_Random
  MOV      RD2, RD14
  MOV      RD14, RD25
  MOV      #0x55555556, RD7
  MOV      #21845, RD7
  DMULS    RD7, RD25
  STS      R1, RD6
  MOV      #3, RD7
  DMULU    RD7, RD6
  STS      R0, RD6
  SUB      RD6, RD25
  SUB      RD25, #1, RD26
  SUB      RD13, #1, RD14
  MOV.Q    TU0ADE001F/y, RQ27
  EXTS.L   RD14
  MOV.L    @(RQ27, RD14), RD25
  ADD      RD25, RD26, RD14
  EXTS.L   RD13
  MOV.L    RD14, @(RQ27, RD13)
  EXTS.L   RD13
  MOV.L    @(RQ27, RD13), RD14
  MOV.L    RD26, @(R15, 48)
  MOV.L    RD25, @(R15, 12)
  CMP/PL   RD14
  BT       L00800B00
  MOV.Q    TU0ADE001F/y, RQ7
  MOV      #0, RD6
  EXTS.L   RD13
  MOV.L    RD6, @(RQ7, RD13)
  BRA      L00800B01

L00800B00:
  MOV.Q    TU0ADE001F/y, RQ5
  EXTS.L   RD13
  MOV.L    @(RQ5, RD13), RD14
  CMP/EQ   #-16, RD14
  BT       L00800B02
  MOV.Q    TU0ADE001F/y, RQ4
  MOV      #-15, RD3
  EXTS.L   RD13
  MOV.L    RD3, @(RQ4, RD13)

L00800B02:

L00800B01:

L00800AFE:
  ADD      #1, RD13
  BRA      L00800AFD

L00800AFF:
  MOV      #0, RD2

L00E00563:
  ADD      #88, R15
  BRA      L00C00194

wipe_doMelt:
  PUSH     LR
  BSR      L00C00193
  ADD      #-104, R15
  MOV.L    RD4, @(R15, 76)
  MOV.L    RD5, @(R15, 72)
  MOV.L    RD6, @(R15, 68)
  MOV      #1, RD14
  MOV      RD14, RD11
  MOV.L    @(R15, 76), RD10
  SHAD     #-1, RD10
  MOV.L    RD10, @(R15, 76)

L00800B03:
  MOV.L    @(R15, 68), RD9
  MOV      RD9, RD14
  ADD      #-1, RD9
  MOV.L    RD9, @(R15, 68)
  CMP/EQ   RD14
  BT       L00800B04
  MOV      #0, RD13

L00800B05:
  MOV.L    @(R15, 76), RD8
  CMP/GT   RD13, RD8
  BT       L00800B07
  MOV.Q    TU0ADE001F/y, RQ31
  EXTS.L   RD13
  MOV.L    @(RQ31, RD13), RD14
  CMP/PZ   RD14
  BT       L00800B08
  MOV.Q    TU0ADE001F/y, RQ30
  EXTS.L   RD13
  MOV.L    @(RQ30, RD13), RD14
  ADD      RD14, #1, RD12
  EXTS.L   RD13
  MOV.L    RD12, @(RQ30, RD13)
  MOV      #0, RD11
  BRA      L00800B09

L00800B08:
  MOV.Q    TU0ADE001F/y, RQ29
  EXTS.L   RD13
  MOV.L    @(RQ29, RD13), RD14
  MOV.L    @(R15, 72), RD28
  CMP/GT   RD14, RD28
  BT       L00800B0A
  MOV.Q    TU0ADE001F/y, RQ27
  EXTS.L   RD13
  MOV.L    @(RQ27, RD13), RD12
  CMP/GE   #16, RD12
  BT       L00800B0B
  MOV.Q    TU0ADE001F/y, RQ26
  EXTS.L   RD13
  MOV.L    @(RQ26, RD13), RD12
  ADD      RD12, #1, RD25
  MOV      RD25, RD14
  MOV.L    RD25, @(R15, 28)
  BRA      L00800B0C

L00800B0B:
  MOV      #8, RD14

L00800B0C:
  MOV      RD14, RD7
  MOV.Q    TU0ADE001F/y, RQ6
  EXTS.L   RD13
  MOV.L    @(RQ6, RD13), RD14
  ADD      RD14, RD7, RD12
  MOV.L    RD7, @(R15, 60)
  MOV.L    @(R15, 72), RD5
  CMP/GT   RD12, RD5
  BT       L00800B0D
  MOV.Q    TU0ADE001F/y, RQ4
  EXTS.L   RD13
  MOV.L    @(RQ4, RD13), RD14
  MOV.L    @(R15, 72), RD2
  SUB      RD2, RD14, RD3
  MOV.L    RD3, @(R15, 60)

L00800B0D:
  MOV.Q    TU0ADE001F/wipe_scr_end, RQ22
  MOV      RQ22, RQ23
  MOV.L    @(R15, 72), RD21
  MUL.L    RD13, RD21, RD14
  MOV.Q    TU0ADE001F/y, RQ20
  EXTS.L   RD13
  MOV.L    @(RQ20, RD13), RD12
  ADD      RD14, RD12, RD19
  EXTS.L   RD19
  LEA.W    @(RQ23, RD19), RQ18
  MOV      RQ18, RQ17
  MOV.Q    TU0ADE001F/wipe_scr, RQ16
  MOV      RQ16, RQ23
  EXTS.L   RD13
  MOV.L    @(RQ20, RD13), RD14
  MOV.L    @(R15, 76), RD2
  MUL.L    RD14, RD2, RD12
  ADD      RD12, RD13, RD14
  EXTS.L   RD14
  LEA.W    @(RQ23, RD14), RQ18
  MOV      RQ18, RQ3
  MOV      #0, RD4
  MOV.L    @(R15, 60), RD6
  MOV      RD6, RD5
  MOV.L    RD5, @(R15, 64)
  MOV.L    RD4, @(R15, 56)
  MOV.Q    RQ3, @(R15, 40)
  MOV.Q    RQ23, @(R15, 16)
  MOV.L    RD19, @(R15, 28)
  MOV.Q    RQ18, @(R15, 8)
  MOV.Q    RQ17, @(R15, 48)

L00800B0E:
  MOV.L    @(R15, 64), RD16
  CMP/EQ   #0, RD16
  BT       L00800B10
  MOV.Q    @(R15, 48), RQ17
  MOV      RQ17, RQ16
  ADD      #2, RQ17
  MOV.W    @RQ16, RD14
  MOV.Q    @(R15, 40), RQ18
  MOV.L    @(R15, 56), RD19
  EXTS.L   RD19
  MOV.W    RD14, @(RQ18, RD19)
  MOV.L    @(R15, 76), RD20
  ADD      RD20, RD19
  MOV.L    RD19, @(R15, 56)
  MOV.Q    RQ17, @(R15, 48)
  MOV.Q    RQ16, @(R15, 16)

L00800B0F:
  MOV.L    @(R15, 64), RD16
  ADD      #-1, RD16
  MOV.L    RD16, @(R15, 64)
  BRA      L00800B0E

L00800B10:
  MOV.Q    TU0ADE001F/y, RQ16
  EXTS.L   RD13
  MOV.L    @(RQ16, RD13), RD14
  MOV.L    @(R15, 60), RD17
  ADD      RD14, RD17, RD12
  EXTS.L   RD13
  MOV.L    RD12, @(RQ16, RD13)
  MOV.Q    TU0ADE001F/wipe_scr_start, RQ19
  MOV      RQ19, RQ18
  MOV.L    @(R15, 72), RD20
  MUL.L    RD13, RD20, RD14
  EXTS.L   RD14
  LEA.W    @(RQ18, RD14), RQ21
  MOV      RQ21, RQ22
  MOV.Q    TU0ADE001F/wipe_scr, RQ23
  MOV      RQ23, RQ18
  EXTS.L   RD13
  MOV.L    @(RQ16, RD13), RD14
  MOV.L    @(R15, 76), RD2
  MUL.L    RD14, RD2, RD12
  ADD      RD12, RD13, RD14
  EXTS.L   RD14
  LEA.W    @(RQ18, RD14), RQ21
  MOV      RQ21, RQ3
  MOV      #0, RD4
  EXTS.L   RD13
  MOV.L    @(RQ16, RD13), RD14
  SUB      RD20, RD14, RD5
  MOV.L    RD5, @(R15, 64)
  MOV.L    RD4, @(R15, 56)
  MOV.Q    RQ3, @(R15, 40)
  MOV.Q    RQ22, @(R15, 48)
  MOV.Q    RQ21, @(R15, 8)
  MOV.Q    RQ18, @(R15, 16)

L00800B11:
  MOV.L    @(R15, 64), RD16
  CMP/EQ   #0, RD16
  BT       L00800B13
  MOV.Q    @(R15, 48), RQ17
  MOV      RQ17, RQ16
  ADD      #2, RQ17
  MOV.W    @RQ16, RD14
  MOV.Q    @(R15, 40), RQ18
  MOV.L    @(R15, 56), RD19
  EXTS.L   RD19
  MOV.W    RD14, @(RQ18, RD19)
  MOV.L    @(R15, 76), RD20
  ADD      RD20, RD19
  MOV.L    RD19, @(R15, 56)
  MOV.Q    RQ17, @(R15, 48)
  MOV.Q    RQ16, @(R15, 16)

L00800B12:
  MOV.L    @(R15, 64), RD16
  ADD      #-1, RD16
  MOV.L    RD16, @(R15, 64)
  BRA      L00800B11

L00800B13:
  MOV      #0, RD11

L00800B0A:

L00800B09:

L00800B06:
  ADD      #1, RD13
  BRA      L00800B05

L00800B07:
  BRA      L00800B03

L00800B04:
  MOV      RD11, RD2

L00E00564:
  ADD      #104, R15
  BRA      L00C00194

wipe_exitMelt:
  PUSH     LR
  ADD      #-152, R15
  MOV.L    RD4, @(R15, 12)
  MOV.L    RD5, @(R15, 8)
  MOV.L    RD6, @(R15, 4)
  MOV.Q    TU0ADE001F/y, RQ4
  BSR      Z_Free
  MOV      #0, RD2

L00E00565:
  ADD      #152, R15
  RET     

V_DrawBlockScr:
  PUSH     LR
  BSR      L00C002C5
  ADD      #-112, R15
  MOV      RD4, RD11
  MOV      RD5, RD12
  MOV      RD6, RD8
  MOV      RD7, RD10
  MOV      RD20, RD13
  MOV.Q    RQ21, @(R15, 32)
  CMP/PZ   RD11
  BT       L008022F9
  ADD      RD11, RD10, RD14
  CMP/GT   #320, RD14
  BT       L008022F9
  BRA      L008022FC

L008022FC:
  CMP/PZ   RD12
  BT       L008022F9
  BRA      L008022FB

L008022FB:
  ADD      RD12, RD13, RD14
  CMP/GT   #200, RD14
  BT       L008022F9
  BRA      L008022FA

L008022FA:
  CMP/HI   #4, RD8
  BT       L008022F9
  BRA      L008022F8

L008022F9:
  MOV      L00C005E1, RQ4
  BSR      I_Error

L008022F8:
  MOV      RD11, RD4
  MOV      RD12, RD5
  MOV      RD10, RD6
  MOV      RD13, RD7
  BSR      V_MarkRect
  MOV      screens, RQ31
  EXTS.L   RD8
  MOV.Q    @(RQ31, RD8), RQ30
  MOV      RD12, RD14
  MOV      #320, RD29
  MUL.L    RD29, RD14, RD14
  EXTS.L   RD14
  LEA.W    @(RQ30, RD14), RQ28
  EXTS.L   RD11
  LEA.W    @(RQ28, RD11), RQ9

L008022FE:
  MOV      RD13, RD14
  ADD      #-1, RD13
  CMP/EQ   RD14
  BT       L008022FF
  SHAD     RD10, #1, RD14
  EXTS.L   RD14, RQ28
  MOV.Q    RQ28, @R15
  MOV      RQ9, RQ4
  MOV.Q    @(R15, 32), RQ5
  MOV      RQ28, RQ6
  BSR      memcpy
  MOV.Q    @(R15, 32), RQ7
  EXTS.L   RD10
  LEA.W    @(RQ7, RD10), RQ7
  ADD      #640, RQ9
  MOV.Q    RQ7, @(R15, 32)
  BRA      L008022FE

L008022FF:

L00E00566:
  ADD      #112, R15
  BRA      L00C002C6

M_Responder:
  PUSH     LR
  BSR      L00C00193
  ADD      #-472, R15
  MOV.Q    RQ4, @(R15, 64)
  MOV      #-1, RD13
  MOV.Q    @(R15, 64), RQ12
  MOV.L    @RQ12, RD14
  CMP/EQ   #3, RD14
  BT       L00800E7B
  BSR      I_GetTime
  MOV      RD2, RD14
  MOV.L    joywait, RD11
  CMP/GT   RD11, RD14
  BT       L00800E7B
  BRA      L00800E7C

L00800E7C:
  MOV.Q    @(R15, 64), RQ10
  MOV.L    @(RQ10, 12), RD14
  CMP/EQ   #-1, RD14
  BT       L00800E7E
  MOV      #173, RD13
  BSR      I_GetTime
  MOV      RD2, RD14
  ADD      RD14, #5, RD9
  MOV.L    RD9, joywait
  BRA      L00800E7F

L00800E7E:
  MOV.Q    @(R15, 64), RQ8
  MOV.L    @(RQ8, 12), RD14
  CMP/EQ   #1, RD14
  BT       L00800E80
  MOV      #175, RD13
  BSR      I_GetTime
  MOV      RD2, RD14
  ADD      RD14, #5, RD31
  MOV.L    RD31, joywait

L00800E80:

L00800E7F:
  MOV.Q    @(R15, 64), RQ30
  MOV.L    @(RQ30, 8), RD14
  CMP/EQ   #-1, RD14
  BT       L00800E81
  MOV      #172, RD13
  BSR      I_GetTime
  MOV      RD2, RD14
  ADD      RD14, #2, RD29
  MOV.L    RD29, joywait
  BRA      L00800E82

L00800E81:
  MOV.Q    @(R15, 64), RQ28
  MOV.L    @(RQ28, 8), RD14
  CMP/EQ   #1, RD14
  BT       L00800E83
  MOV      #174, RD13
  BSR      I_GetTime
  MOV      RD2, RD14
  ADD      RD14, #2, RD27
  MOV.L    RD27, joywait

L00800E83:

L00800E82:
  MOV.Q    @(R15, 64), RQ26
  MOV.L    @(RQ26, 4), RD14
  TST      #1, RD14
  BT       L00800E84
  MOV      #13, RD13
  BSR      I_GetTime
  MOV      RD2, RD14
  ADD      RD14, #5, RD25
  MOV.L    RD25, joywait

L00800E84:
  MOV.Q    @(R15, 64), RQ7
  MOV.L    @(RQ7, 4), RD14
  TST      #2, RD14
  BT       L00800E85
  MOV      #127, RD13
  BSR      I_GetTime
  MOV      RD2, RD14
  ADD      RD14, #5, RD25
  MOV.L    RD25, joywait

L00800E85:
  BRA      L00800E86

L00800E7B:
  MOV.Q    @(R15, 64), RQ6
  MOV.L    @RQ6, RD14
  CMP/EQ   #2, RD14
  BT       L00800E87
  BSR      I_GetTime
  MOV      RD2, RD14
  MOV.L    mousewait, RD25
  CMP/GT   RD25, RD14
  BT       L00800E87
  BRA      L00800E88

L00800E88:
  MOV.Q    @(R15, 64), RQ5
  MOV.L    @(RQ5, 12), RD14
  MOV.L    mousey, RD4
  ADD      RD14, RD4
  MOV.L    lasty, RD3
  SUB      RD3, #30, RD14
  MOV.L    RD4, mousey
  MOV.L    mousey, RD2
  CMP/GT   RD2, RD14
  BT       L00800E8A
  MOV      #175, RD13
  BSR      I_GetTime
  MOV      RD2, RD14
  ADD      RD14, #5, RD25
  MOV.L    lasty, RD26
  SUB      RD26, #30, RD14
  MOV      RD14, RD26
  MOV      RD14, RD27
  MOV.L    RD27, mousey
  MOV.L    RD26, lasty
  MOV.L    RD25, mousewait
  BRA      L00800E8B

L00800E8A:
  MOV.L    lasty, RD23
  ADD      RD23, #30, RD14
  MOV.L    mousey, RD22
  CMP/GT   RD14, RD22
  BT       L00800E8C
  MOV      #173, RD13
  BSR      I_GetTime
  MOV      RD2, RD14
  ADD      RD14, #5, RD25
  MOV.L    lasty, RD26
  ADD      RD26, #30, RD14
  MOV      RD14, RD26
  MOV      RD14, RD27
  MOV.L    RD27, mousey
  MOV.L    RD26, lasty
  MOV.L    RD25, mousewait

L00800E8C:

L00800E8B:
  MOV.Q    @(R15, 64), RQ21
  MOV.L    @(RQ21, 8), RD14
  MOV.L    mousex, RD20
  ADD      RD14, RD20
  MOV.L    lastx, RD19
  SUB      RD19, #30, RD14
  MOV.L    RD20, mousex
  MOV.L    mousex, RD18
  CMP/GT   RD18, RD14
  BT       L00800E8D
  MOV      #172, RD13
  BSR      I_GetTime
  MOV      RD2, RD14
  ADD      RD14, #5, RD25
  MOV.L    lastx, RD26
  SUB      RD26, #30, RD14
  MOV      RD14, RD26
  MOV      RD14, RD27
  MOV.L    RD27, mousex
  MOV.L    RD26, lastx
  MOV.L    RD25, mousewait
  BRA      L00800E8E

L00800E8D:
  MOV.L    lastx, RD17
  ADD      RD17, #30, RD14
  MOV.L    mousex, RD16
  CMP/GT   RD14, RD16
  BT       L00800E8F
  MOV      #174, RD13
  BSR      I_GetTime
  MOV      RD2, RD14
  ADD      RD14, #5, RD25
  MOV.L    lastx, RD26
  ADD      RD26, #30, RD14
  MOV      RD14, RD26
  MOV      RD14, RD27
  MOV.L    RD27, mousex
  MOV.L    RD26, lastx
  MOV.L    RD25, mousewait

L00800E8F:

L00800E8E:
  MOV.Q    @(R15, 64), RQ16
  MOV.L    @(RQ16, 4), RD14
  TST      #1, RD14
  BT       L00800E90
  MOV      #13, RD13
  BSR      I_GetTime
  MOV      RD2, RD14
  ADD      RD14, #15, RD25
  MOV.L    RD25, mousewait

L00800E90:
  MOV.Q    @(R15, 64), RQ16
  MOV.L    @(RQ16, 4), RD14
  TST      #2, RD14
  BT       L00800E91
  MOV      #127, RD13
  BSR      I_GetTime
  MOV      RD2, RD14
  ADD      RD14, #15, RD25
  MOV.L    RD25, mousewait

L00800E91:
  BRA      L00800E92

L00800E87:
  MOV.Q    @(R15, 64), RQ16
  MOV.L    @RQ16, RD14
  CMP/EQ   RD14
  BT       L00800E93
  MOV.Q    @(R15, 64), RQ16
  MOV.L    @(RQ16, 4), RD13

L00800E93:

L00800E92:

L00800E86:
  CMP/EQ   #-1, RD13
  BT       L00800E94
  MOV      #0, RD2
  BRA      L00E00567

L00800E94:
  MOV.L    saveStringEnter, RD16
  CMP/EQ   #0, RD16
  BT       L00800E95
  CMP/EQ   #127, RD13
  BT       L00800E97
  CMP/EQ   #27, RD13
  BT       L00800E98
  CMP/EQ   #13, RD13
  BT       L00800E99
  BRA      L00800E9A

L00800E97:
  MOV.L    saveCharIndex, RD16
  CMP/GT   #0, RD16
  BT       L00800E9B
  MOV.L    saveCharIndex, RD25
  ADD      #-1, RD25
  MOV      savegamestrings, RQ26
  MOV.L    saveSlot, RD27
  EXTS.L   RD27
  MOV      #24, RD7
  MUL.L    RD27, RD7
  STS      R0, RQ28
  ADD      RQ26, RQ28
  MOV      #0, RD29
  EXTS.L   RD25
  MOV.B    RD29, @(RQ28, RD25)
  MOV.Q    RQ28, @(R15, 48)
  MOV.L    RD25, saveCharIndex

L00800E9B:
  BRA      L00800E96

L00800E98:
  MOV      #0, RD25
  MOV      savegamestrings, RQ26
  MOV.L    saveSlot, RD27
  EXTS.L   RD27
  MOV      #24, RD7
  MUL.L    RD27, RD7
  STS      R0, RQ28
  ADD      RQ26, RQ28
  MOV      #0, RD29
  EXTS.L   RD29
  LEA.B    @(RQ28, RD29), RQ30
  MOV.Q    RQ30, @(R15, 40)
  MOV.Q    RQ28, @(R15, 48)
  MOV.L    RD25, saveStringEnter
  MOV      RQ30, RQ4
  MOV      saveOldString, RQ5
  BSR      strcpy
  BRA      L00800E96

L00800E99:
  MOV      #0, RD25
  MOV      savegamestrings, RQ26
  MOV.L    saveSlot, RD27
  EXTS.L   RD27
  MOV      #24, RD7
  MUL.L    RD27, RD7
  STS      R0, RQ28
  ADD      RQ26, RQ28
  MOV.B    @RQ28, RD14
  MOV.Q    RQ28, @(R15, 48)
  MOV.L    RD25, saveStringEnter
  CMP/EQ   RD14
  BT       L00800E9C
  MOV.L    saveSlot, RD4
  BSR      M_DoSave

L00800E9C:
  BRA      L00800E96

L00800E9A:
  MOV      RD13, RD4
  BSR      toupper
  MOV      RD2, RD13
  CMP/EQ   #32, RD13
  BT       L00800E9D
  SUB      RD13, #33, RD14
  CMP/PZ   RD14
  BT       L00800E9F
  SUB      RD13, #33, RD14
  CMP/GE   #63, RD14
  BT       L00800E9F
  BRA      L00800E9E

L00800E9F:
  BRA      L00800E96

L00800E9E:

L00800E9D:
  CMP/GE   #32, RD13
  BT       L00800EA1
  CMP/GT   #127, RD13
  BT       L00800EA1
  BRA      L00800EA4

L00800EA4:
  MOV.L    saveCharIndex, RD16
  MOV      #23, RD17
  CMP/GE   RD17, RD16
  BT       L00800EA1
  BRA      L00800EA3

L00800EA3:
  MOV      savegamestrings, RQ25
  MOV.L    saveSlot, RD26
  EXTS.L   RD26
  MOV      #24, RD7
  MUL.L    RD26, RD7
  STS      R0, RQ27
  ADD      RQ25, RQ27
  MOV.Q    RQ27, @(R15, 48)
  MOV      RQ27, RQ4
  BSR      M_StringWidth
  MOV      RD2, RD14
  CMP/GE   #176, RD14
  BT       L00800EA1
  BRA      L00800EA2

L00800EA2:
  MOV      savegamestrings, RQ25
  MOV.L    saveSlot, RD26
  EXTS.L   RD26
  MOV      #24, RD7
  MUL.L    RD26, RD7
  STS      R0, RQ27
  ADD      RQ25, RQ27
  MOV.L    saveCharIndex, RD28
  MOV      RD28, RD14
  ADD      #1, RD28
  MOV.B    RD13, @(RQ27, RD14)
  EXTS.L   RD26
  MOV      #24, RD7
  MUL.L    RD26, RD7
  STS      R0, RQ27
  ADD      RQ25, RQ27
  MOV      #0, RD29
  EXTS.L   RD28
  MOV.B    RD29, @(RQ27, RD28)
  MOV.L    RD28, saveCharIndex
  MOV.Q    RQ27, @(R15, 48)

L00800EA1:
  BRA      L00800E96

L00800E96:
  MOV      #1, RD2
  BRA      L00E00567

L00800E95:
  MOV.L    messageToPrint, RD16
  CMP/EQ   #0, RD16
  BT       L00800EA6
  MOV.L    messageNeedsInput, RD16
  CMP/EQ   #1, RD16
  BT       L00800EA7
  CMP/EQ   #32, RD13
  BT       L00800EA7
  CMP/EQ   #110, RD13
  BT       L00800EA7
  BRA      L00800EAC

L00800EAC:
  CMP/EQ   #121, RD13
  BT       L00800EA7
  BRA      L00800EAB

L00800EAB:
  CMP/EQ   #27, RD13
  BT       L00800EA7
  BRA      L00800EAA

L00800EAA:
  BRA      L00800EA8

L00800EA8:
  MOV      #0, RD2
  BRA      L00E00567

L00800EA7:
  MOV.L    messageLastMenuActive, RD17
  MOV      RD17, RD16
  MOV      #0, RD18
  MOV.L    RD18, messageToPrint
  MOV.L    RD16, menuactive
  MOV.Q    messageRoutine, RQ19
  MOV      #0, RQ20
  CMPQ/EQ  RQ20, RQ19
  BT       L00800EAE
  MOV      RD13, RD4
  MOV.Q    messageRoutine, RQ25
  MOV      RQ25, R1
  JSR      R1

L00800EAE:
  MOV      #0, RD25
  MOV      #0, RQ26
  MOV.Q    RQ26, @(R15, 48)
  MOV.L    RD25, menuactive
  MOV      RQ26, RQ4
  MOV      #24, RD5
  BSR      S_StartSound
  MOV      #1, RD2
  BRA      L00E00567

L00800EA6:
  MOV.L    devparm, RD16
  CMP/EQ   #0, RD16
  BT       L00800EAF
  CMP/EQ   #187, RD13
  BT       L00800EAF
  BRA      L00800EB0

L00800EB0:
  BSR      G_ScreenShot
  MOV      #1, RD2
  BRA      L00E00567

L00800EAF:
  MOV.L    menuactive, RD16
  CMP/EQ   #0, RD16
  BT       L00800EB2
  CMP/GE   #191, RD13
  BT       L00800EC2
  CMP/EQ   #45, RD13
  BT       L00800EB4
  CMP/EQ   #61, RD13
  BT       L00800EB5
  CMP/EQ   #187, RD13
  BT       L00800EB6
  CMP/EQ   #188, RD13
  BT       L00800EB7
  CMP/EQ   #189, RD13
  BT       L00800EB8
  CMP/EQ   #190, RD13
  BT       L00800EB9

L00800EC2:
  CMP/EQ   #191, RD13
  BT       L00800EBA
  CMP/EQ   #192, RD13
  BT       L00800EBB
  CMP/EQ   #193, RD13
  BT       L00800EBC
  CMP/EQ   #194, RD13
  BT       L00800EBD
  CMP/EQ   #195, RD13
  BT       L00800EBE
  CMP/EQ   #196, RD13
  BT       L00800EBF
  CMP/EQ   #215, RD13
  BT       L00800EC0
  BRA      L00800EC1

L00800EB4:
  MOV.L    automapactive, RD16
  CMP/EQ   #0, RD16
  BT       L00800EC4
  MOV.L    chat_on, RD16
  CMP/EQ   #0, RD16
  BT       L00800EC4
  BRA      L00800EC3

L00800EC4:
  MOV      #0, RD2
  BRA      L00E00567

L00800EC3:
  MOV      #0, RD4
  BSR      M_SizeDisplay
  MOV      #0, RQ25
  MOV.Q    RQ25, @(R15, 48)
  MOV      RQ25, RQ4
  MOV      #22, RD5
  BSR      S_StartSound
  MOV      #1, RD2
  BRA      L00E00567

L00800EB5:
  MOV.L    automapactive, RD16
  CMP/EQ   #0, RD16
  BT       L00800EC7
  MOV.L    chat_on, RD16
  CMP/EQ   #0, RD16
  BT       L00800EC7
  BRA      L00800EC6

L00800EC7:
  MOV      #0, RD2
  BRA      L00E00567

L00800EC6:
  MOV      #1, RD4
  BSR      M_SizeDisplay
  MOV      #0, RQ25
  MOV.Q    RQ25, @(R15, 48)
  MOV      RQ25, RQ4
  MOV      #22, RD5
  BSR      S_StartSound
  MOV      #1, RD2
  BRA      L00E00567

L00800EB6:
  BSR      M_StartControlPanel
  MOV.L    gamemode, RD16
  CMP/EQ   #3, RD16
  BT       L00800EC9
  MOV      ReadDef2, RQ17
  MOV      RQ17, RQ16
  MOV      RQ16, RQ18
  MOV.Q    RQ18, currentMenu
  MOV.Q    RQ16, @(R15, 48)
  BRA      L00800ECA

L00800EC9:
  MOV      ReadDef1, RQ17
  MOV      RQ17, RQ16
  MOV      RQ16, RQ18
  MOV.Q    RQ18, currentMenu
  MOV.Q    RQ16, @(R15, 48)

L00800ECA:
  MOV      #0, RD14
  MOV      RD14, RD25
  MOV      #0, RQ26
  MOV.Q    RQ26, @(R15, 48)
  MOV.W    RD25, itemOn
  MOV      RQ26, RQ4
  MOV      #23, RD5
  BSR      S_StartSound
  MOV      #1, RD2
  BRA      L00E00567

L00800EB7:
  BSR      M_StartControlPanel
  MOV      #0, RQ25
  MOV.Q    RQ25, @(R15, 48)
  MOV      RQ25, RQ4
  MOV      #23, RD5
  BSR      S_StartSound
  MOV      #0, RD4
  BSR      M_SaveGame
  MOV      #1, RD2
  BRA      L00E00567

L00800EB8:
  BSR      M_StartControlPanel
  MOV      #0, RQ25
  MOV.Q    RQ25, @(R15, 48)
  MOV      RQ25, RQ4
  MOV      #23, RD5
  BSR      S_StartSound
  MOV      #0, RD4
  BSR      M_LoadGame
  MOV      #1, RD2
  BRA      L00E00567

L00800EB9:
  BSR      M_StartControlPanel
  MOV      SoundDef, RQ26
  MOV      RQ26, RQ25
  MOV      RQ25, RQ27
  MOV      #0, RD14
  MOV      RD14, RD28
  MOV      #0, RQ25
  MOV.W    RD28, itemOn
  MOV.Q    RQ27, currentMenu
  MOV.Q    RQ25, @(R15, 48)
  MOV      RQ25, RQ4
  MOV      #23, RD5
  BSR      S_StartSound
  MOV      #1, RD2
  BRA      L00E00567

L00800EBA:
  MOV      #0, RD4
  BSR      M_ChangeDetail
  MOV      #0, RQ25
  MOV.Q    RQ25, @(R15, 48)
  MOV      RQ25, RQ4
  MOV      #23, RD5
  BSR      S_StartSound
  MOV      #1, RD2
  BRA      L00E00567

L00800EBB:
  MOV      #0, RQ25
  MOV.Q    RQ25, @(R15, 48)
  MOV      RQ25, RQ4
  MOV      #23, RD5
  BSR      S_StartSound
  BSR      M_QuickSave
  MOV      #1, RD2
  BRA      L00E00567

L00800EBC:
  MOV      #0, RQ25
  MOV.Q    RQ25, @(R15, 48)
  MOV      RQ25, RQ4
  MOV      #23, RD5
  BSR      S_StartSound
  MOV      #0, RD4
  BSR      M_EndGame
  MOV      #1, RD2
  BRA      L00E00567

L00800EBD:
  MOV      #0, RD4
  BSR      M_ChangeMessages
  MOV      #0, RQ25
  MOV.Q    RQ25, @(R15, 48)
  MOV      RQ25, RQ4
  MOV      #23, RD5
  BSR      S_StartSound
  MOV      #1, RD2
  BRA      L00E00567

L00800EBE:
  MOV      #0, RQ25
  MOV.Q    RQ25, @(R15, 48)
  MOV      RQ25, RQ4
  MOV      #23, RD5
  BSR      S_StartSound
  BSR      M_QuickLoad
  MOV      #1, RD2
  BRA      L00E00567

L00800EBF:
  MOV      #0, RQ25
  MOV.Q    RQ25, @(R15, 48)
  MOV      RQ25, RQ4
  MOV      #23, RD5
  BSR      S_StartSound
  MOV      #0, RD4
  BSR      M_QuitDOOM
  MOV      #1, RD2
  BRA      L00E00567

L00800EC0:
  MOV.L    usegamma, RD16
  ADD      #1, RD16
  MOV.L    RD16, usegamma
  MOV.L    usegamma, RD17
  CMP/GT   #4, RD17
  BT       L00800ECB
  MOV      #0, RD16
  MOV.L    RD16, usegamma

L00800ECB:
  MOV      gammamsg, RQ25
  MOV.L    usegamma, RD26
  EXTS.L   RD26
  MOV      #26, RD7
  MUL.L    RD26, RD7
  STS      R0, RQ27
  ADD      RQ25, RQ27
  MOV      players, RQ28
  MOV.L    consoleplayer, RD29
  EXTS.L   RD29
  MOV      #320, RD7
  MUL.L    RD29, RD7
  STS      R0, RQ30
  ADD      RQ28, RQ30
  MOV.Q    RQ27, @(RQ30, 224)
  MOV.Q    RQ27, @(R15, 48)
  MOV      L00C00605, RQ4
  MOV      #101, RD5
  BSR      W_CacheLumpName
  MOV      RQ2, RQ27
  MOV.Q    RQ27, @(R15, 48)
  MOV      RQ27, RQ4
  BSR      I_SetPalette
  MOV      #1, RD2
  BRA      L00E00567

L00800EC1:

L00800EB3:

L00800EB2:
  MOV.L    menuactive, RD16
  CMP/EQ   #0, RD16
  BT       L00800ECC
  CMP/EQ   #27, RD13
  BT       L00800ECD
  BSR      M_StartControlPanel
  MOV      #0, RQ25
  MOV.Q    RQ25, @(R15, 48)
  MOV      RQ25, RQ4
  MOV      #23, RD5
  BSR      S_StartSound
  MOV      #1, RD2
  BRA      L00E00567

L00800ECD:
  MOV      #0, RD2
  BRA      L00E00567

L00800ECC:
  CMP/EQ   #175, RD13
  BT       L00800ECF
  CMP/EQ   #173, RD13
  BT       L00800ED0
  CMP/EQ   #172, RD13
  BT       L00800ED1
  CMP/EQ   #174, RD13
  BT       L00800ED2
  CMP/EQ   #13, RD13
  BT       L00800ED3
  CMP/EQ   #27, RD13
  BT       L00800ED4
  CMP/EQ   #127, RD13
  BT       L00800ED5
  BRA      L00800ED6

L00800ECF:

L00800ED7:
  MOV.W    itemOn, RD16
  ADD      RD16, #1, RD14
  MOV.Q    currentMenu, RQ17
  MOV.W    @RQ17, RD18
  SUB      RD18, #1, RD19
  MOV.L    RD19, @(R15, 24)
  MOV.L    RD18, @(R15, 28)
  CMP/GT   RD19, RD14
  BT       L00800ED9
  MOV      #0, RD14
  MOV      RD14, RD16
  MOV.W    RD16, itemOn
  BRA      L00800EDA

L00800ED9:
  MOV.W    itemOn, RD16
  ADD      #1, RD16
  MOV.W    RD16, itemOn

L00800EDA:
  MOV      #0, RQ25
  MOV.Q    RQ25, @(R15, 48)
  MOV      RQ25, RQ4
  MOV      #19, RD5
  BSR      S_StartSound
  MOV.Q    currentMenu, RQ16
  MOV.Q    @(RQ16, 16), RQ25
  MOV.W    itemOn, RD17
  EXTS.L   RD17
  SHAD     RD17, #5, RQ18
  ADD      RQ25, RQ18
  MOV.W    @RQ18, RD14
  MOV.Q    RQ25, @(R15, 48)
  MOV.Q    RQ18, @(R15, 16)
  CMP/EQ   #-1, RD14
  BT       L00800ED7

L00800ED8:
  MOV      #1, RD2
  BRA      L00E00567

L00800ED0:

L00800EDB:
  MOV.W    itemOn, RD16
  CMP/EQ   #0, RD16
  BT       L00800EDD
  MOV.Q    currentMenu, RQ16
  MOV.W    @RQ16, RD14
  MOV      RD14, RD17
  MOV      #1, RD14
  SUB      RD17, RD14, RD18
  MOV.W    RD18, itemOn
  MOV.L    RD17, @(R15, 28)
  BRA      L00800EDE

L00800EDD:
  MOV.W    itemOn, RD16
  ADD      #-1, RD16
  MOV.W    RD16, itemOn

L00800EDE:
  MOV      #0, RQ25
  MOV.Q    RQ25, @(R15, 48)
  MOV      RQ25, RQ4
  MOV      #19, RD5
  BSR      S_StartSound
  MOV.Q    currentMenu, RQ16
  MOV.Q    @(RQ16, 16), RQ25
  MOV.W    itemOn, RD17
  EXTS.L   RD17
  SHAD     RD17, #5, RQ18
  ADD      RQ25, RQ18
  MOV.W    @RQ18, RD14
  MOV.Q    RQ25, @(R15, 48)
  MOV.Q    RQ18, @(R15, 16)
  CMP/EQ   #-1, RD14
  BT       L00800EDB

L00800EDC:
  MOV      #1, RD2
  BRA      L00E00567

L00800ED1:
  MOV.Q    currentMenu, RQ16
  MOV.Q    @(RQ16, 16), RQ17
  MOV.W    itemOn, RD18
  EXTS.L   RD18
  SHAD     RD18, #5, RQ19
  ADD      RQ17, RQ19
  MOV.Q    @(RQ19, 16), RQ17
  MOV.Q    RQ19, @(R15, 16)
  MOV.Q    RQ17, @(R15, 48)
  MOV      #0, RQ20
  CMPQ/EQ  RQ20, RQ17
  BT       L00800EDF
  MOV.Q    currentMenu, RQ16
  MOV.Q    @(RQ16, 16), RQ17
  MOV.W    itemOn, RD18
  EXTS.L   RD18
  SHAD     RD18, #5, RQ19
  ADD      RQ17, RQ19
  MOV.W    @RQ19, RD14
  MOV.Q    RQ19, @(R15, 16)
  MOV.Q    RQ17, @(R15, 48)
  CMP/EQ   #2, RD14
  BT       L00800EDF
  BRA      L00800EE0

L00800EE0:
  MOV      #0, RQ25
  MOV.Q    RQ25, @(R15, 48)
  MOV      RQ25, RQ4
  MOV      #22, RD5
  BSR      S_StartSound
  MOV.Q    currentMenu, RQ26
  MOV.Q    @(RQ26, 16), RQ25
  MOV.W    itemOn, RD27
  EXTS.L   RD27
  SHAD     RD27, #5, RQ28
  ADD      RQ25, RQ28
  MOV.Q    @(RQ28, 16), RQ25
  MOV.Q    RQ28, @(R15, 16)
  MOV.Q    RQ25, @(R15, 48)
  MOV      #0, RD4
  MOV      RQ25, R1
  JSR      R1

L00800EDF:
  MOV      #1, RD2
  BRA      L00E00567

L00800ED2:
  MOV.Q    currentMenu, RQ16
  MOV.Q    @(RQ16, 16), RQ17
  MOV.W    itemOn, RD18
  EXTS.L   RD18
  SHAD     RD18, #5, RQ19
  ADD      RQ17, RQ19
  MOV.Q    @(RQ19, 16), RQ17
  MOV.Q    RQ19, @(R15, 16)
  MOV.Q    RQ17, @(R15, 48)
  MOV      #0, RQ20
  CMPQ/EQ  RQ20, RQ17
  BT       L00800EE2
  MOV.Q    currentMenu, RQ16
  MOV.Q    @(RQ16, 16), RQ17
  MOV.W    itemOn, RD18
  EXTS.L   RD18
  SHAD     RD18, #5, RQ19
  ADD      RQ17, RQ19
  MOV.W    @RQ19, RD14
  MOV.Q    RQ19, @(R15, 16)
  MOV.Q    RQ17, @(R15, 48)
  CMP/EQ   #2, RD14
  BT       L00800EE2
  BRA      L00800EE3

L00800EE3:
  MOV      #0, RQ25
  MOV.Q    RQ25, @(R15, 48)
  MOV      RQ25, RQ4
  MOV      #22, RD5
  BSR      S_StartSound
  MOV.Q    currentMenu, RQ26
  MOV.Q    @(RQ26, 16), RQ25
  MOV.W    itemOn, RD27
  EXTS.L   RD27
  SHAD     RD27, #5, RQ28
  ADD      RQ25, RQ28
  MOV.Q    @(RQ28, 16), RQ25
  MOV.Q    RQ28, @(R15, 16)
  MOV.Q    RQ25, @(R15, 48)
  MOV      #1, RD4
  MOV      RQ25, R1
  JSR      R1

L00800EE2:
  MOV      #1, RD2
  BRA      L00E00567

L00800ED3:
  MOV.Q    currentMenu, RQ16
  MOV.Q    @(RQ16, 16), RQ17
  MOV.W    itemOn, RD18
  EXTS.L   RD18
  SHAD     RD18, #5, RQ19
  ADD      RQ17, RQ19
  MOV.Q    @(RQ19, 16), RQ17
  MOV.Q    RQ19, @(R15, 16)
  MOV.Q    RQ17, @(R15, 48)
  MOV      #0, RQ20
  CMPQ/EQ  RQ20, RQ17
  BT       L00800EE5
  MOV.Q    currentMenu, RQ16
  MOV.Q    @(RQ16, 16), RQ17
  MOV.W    itemOn, RD18
  EXTS.L   RD18
  SHAD     RD18, #5, RQ19
  ADD      RQ17, RQ19
  MOV.W    @RQ19, RD14
  MOV.Q    RQ19, @(R15, 16)
  MOV.Q    RQ17, @(R15, 48)
  CMP/EQ   RD14
  BT       L00800EE5
  BRA      L00800EE6

L00800EE6:
  MOV.W    itemOn, RD16
  MOV.Q    currentMenu, RQ17
  MOV.W    RD16, @(RQ17, 36)
  MOV.Q    @(RQ17, 16), RQ18
  SHAD     RD16, #5, RQ19
  ADD      RQ18, RQ19
  MOV.W    @RQ19, RD14
  MOV.Q    RQ19, @(R15, 16)
  MOV.Q    RQ18, @(R15, 48)
  CMP/EQ   #2, RD14
  BT       L00800EE8
  MOV.Q    currentMenu, RQ25
  MOV.Q    @(RQ25, 16), RQ26
  MOV.W    itemOn, RD27
  EXTS.L   RD27
  SHAD     RD27, #5, RQ28
  ADD      RQ26, RQ28
  MOV.Q    @(RQ28, 16), RQ26
  MOV.Q    RQ28, @(R15, 16)
  MOV.Q    RQ26, @(R15, 48)
  MOV      #1, RD4
  MOV      RQ26, R1
  JSR      R1
  MOV      #0, RQ26
  MOV.Q    RQ26, @(R15, 48)
  MOV      RQ26, RQ4
  MOV      #22, RD5
  BSR      S_StartSound
  BRA      L00800EE9

L00800EE8:
  MOV.Q    currentMenu, RQ25
  MOV.Q    @(RQ25, 16), RQ26
  MOV.W    itemOn, RD27
  EXTS.L   RD27
  SHAD     RD27, #5, RQ28
  ADD      RQ26, RQ28
  MOV.Q    @(RQ28, 16), RQ26
  MOV.Q    RQ28, @(R15, 16)
  MOV.Q    RQ26, @(R15, 48)
  MOV.W    itemOn, RD4
  MOV      RQ26, R1
  JSR      R1
  MOV      #0, RQ26
  MOV.Q    RQ26, @(R15, 48)
  MOV      RQ26, RQ4
  MOV      #1, RD5
  BSR      S_StartSound

L00800EE9:

L00800EE5:
  MOV      #1, RD2
  BRA      L00E00567

L00800ED4:
  MOV.W    itemOn, RD25
  MOV.Q    currentMenu, RQ26
  MOV.W    RD25, @(RQ26, 36)
  BSR      M_ClearMenus
  MOV      #0, RQ27
  MOV.Q    RQ27, @(R15, 48)
  MOV      RQ27, RQ4
  MOV      #24, RD5
  BSR      S_StartSound
  MOV      #1, RD2
  BRA      L00E00567

L00800ED5:
  MOV.W    itemOn, RD16
  MOV.Q    currentMenu, RQ17
  MOV.W    RD16, @(RQ17, 36)
  MOV.Q    @(RQ17, 8), RQ18
  MOV.Q    RQ18, @(R15, 48)
  MOV      #0, RQ19
  CMPQ/EQ  RQ19, RQ18
  BT       L00800EEA
  MOV.Q    currentMenu, RQ25
  MOV.Q    @(RQ25, 8), RQ25
  MOV.W    @(RQ25, 36), RD26
  MOV      #0, RQ27
  MOV.Q    RQ27, @(R15, 48)
  MOV.W    RD26, itemOn
  MOV.Q    RQ25, currentMenu
  MOV      RQ27, RQ4
  MOV      #23, RD5
  BSR      S_StartSound

L00800EEA:
  MOV      #1, RD2
  BRA      L00E00567

L00800ED6:
  MOV.W    itemOn, RD16
  ADD      RD16, #1, RD17
  MOV.L    RD17, @(R15, 60)

L00800EEB:
  MOV.Q    currentMenu, RQ16
  MOV.W    @RQ16, RD14
  MOV.L    @(R15, 60), RD18
  MOV      RD18, RD17
  EXTS.W   RD14, RD19
  MOV.L    RD19, @(R15, 24)
  MOV.L    RD17, @(R15, 28)
  CMP/GT   RD17, RD19
  BT       L00800EED
  MOV.Q    currentMenu, RQ16
  MOV.Q    @(RQ16, 16), RQ17
  MOV.L    @(R15, 60), RD18
  EXTS.L   RD18
  SHAD     RD18, #5, RQ19
  ADD      RQ17, RQ19
  MOV.B    @(RQ19, 24), RD14
  EXTS.B   RD14, RD20
  MOV      RD13, RD21
  MOV.L    RD21, @(R15, 24)
  MOV.L    RD20, @(R15, 28)
  MOV.Q    RQ19, @(R15, 16)
  MOV.Q    RQ17, @(R15, 48)
  CMP/EQ   RD21, RD20
  BT       L00800EEE
  MOV.L    @(R15, 60), RD14
  EXTS.W   RD14, RD14
  MOV      RD14, RD25
  MOV      #0, RQ26
  MOV.Q    RQ26, @(R15, 48)
  MOV.W    RD25, itemOn
  MOV      RQ26, RQ4
  MOV      #19, RD5
  BSR      S_StartSound
  MOV      #1, RD2
  BRA      L00E00567

L00800EEE:

L00800EEC:
  MOV.L    @(R15, 60), RD16
  ADD      #1, RD16
  MOV.L    RD16, @(R15, 60)
  BRA      L00800EEB

L00800EED:
  MOV      #0, RD16
  MOV.L    RD16, @(R15, 60)

L00800EEF:
  MOV.L    @(R15, 60), RD16
  MOV      RD16, RD14
  MOV.W    itemOn, RD17
  EXTS.W   RD17, RD17
  MOV.L    RD17, @(R15, 28)
  CMP/GT   RD17, RD14
  BT       L00800EF1
  MOV.Q    currentMenu, RQ16
  MOV.Q    @(RQ16, 16), RQ17
  MOV.L    @(R15, 60), RD18
  EXTS.L   RD18
  SHAD     RD18, #5, RQ19
  ADD      RQ17, RQ19
  MOV.B    @(RQ19, 24), RD14
  EXTS.B   RD14, RD20
  MOV      RD13, RD21
  MOV.L    RD21, @(R15, 24)
  MOV.L    RD20, @(R15, 28)
  MOV.Q    RQ19, @(R15, 16)
  MOV.Q    RQ17, @(R15, 48)
  CMP/EQ   RD21, RD20
  BT       L00800EF2
  MOV.L    @(R15, 60), RD14
  EXTS.W   RD14, RD14
  MOV      RD14, RD25
  MOV      #0, RQ26
  MOV.Q    RQ26, @(R15, 48)
  MOV.W    RD25, itemOn
  MOV      RQ26, RQ4
  MOV      #19, RD5
  BSR      S_StartSound
  MOV      #1, RD2
  BRA      L00E00567

L00800EF2:

L00800EF0:
  MOV.L    @(R15, 60), RD16
  ADD      #1, RD16
  MOV.L    RD16, @(R15, 60)
  BRA      L00800EEF

L00800EF1:
  BRA      L00800ECE

L00800ECE:
  MOV      #0, RD2

L00E00567:
  ADD      #472, R15
  BRA      L00C00194

M_Ticker:
  PUSH     LR
  BSR      L00C00198
  ADD      #-104, R15
  MOV.W    skullAnimCounter, RD14
  ADD      #-1, RD14
  MOV.W    RD14, skullAnimCounter
  MOV.W    skullAnimCounter, RD13
  CMP/PL   RD13
  BT       L00800F02
  MOV.W    whichSkull, RD11
  MOV      RD11, RD12
  MOV      #1, RD10
  XOR      RD12, RD10, RD11
  MOV      #8, RD12
  MOV      RD12, RD9
  MOV.W    RD11, whichSkull
  MOV.W    RD9, skullAnimCounter

L00800F02:

L00E00568:
  ADD      #104, R15
  BRA      L00C0019A

M_Drawer:
  PUSH     LR
  BSR      L00C00193
  ADD      #-408, R15
  MOV      #84, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 72)
  MOV      #0, RD8
  MOV.L    RD8, inhelpscreens
  MOV.L    messageToPrint, RD31
  CMP/EQ   #0, RD31
  BT       L00800EF4
  MOV      #0, RD10
  MOV.Q    messageString, RQ4
  BSR      M_StringHeight
  MOV      RD2, RD14
  SHAD     RD14, #-1, RD9
  MOV      #100, RD14
  EXTS.W   RD9, RD30
  SUB      RD14, RD30, RD29
  MOV.L    RD30, @(R15, 60)
  MOV.W    RD29, y

L00800EF5:
  MOV.Q    messageString, RQ28
  EXTS.L   RD10
  LEA.B    @(RQ28, RD10), RQ13
  MOV.B    @RQ13, RD14
  CMP/EQ   RD14
  BT       L00800EF6
  MOV      #0, RD14
  MOV      RD14, RD12

L00800EF7:
  MOV.Q    messageString, RQ27
  EXTS.L   RD10
  LEA.B    @(RQ27, RD10), RQ13
  MOV      RQ13, RQ4
  BSR      strlen
  MOV      RQ2, RQ11
  EXTS.W   RD12, RQ26
  MOV      RQ11, RQ25
  MOV.Q    RQ26, @(R15, 32)
  MOV.Q    RQ25, @(R15, 24)
  CMPQ/HI  RQ26, RQ25
  BT       L00800EF9
  MOV.Q    messageString, RQ7
  EXTS.L   RD10
  LEA.B    @(RQ7, RD10), RQ13
  EXTS.L   RD12
  LEA.B    @(RQ13, RD12), RQ6
  MOV.B    @RQ6, RD14
  CMP/EQ   #10, RD14
  BT       L00800EFA
  MOV      #40, RQ11
  EXTS.L   RQ11, RQ11
  MOV      #84, RQ4
  ADD      R15, RQ4
  MOV      #0, RD5
  MOV      RQ11, RQ6
  BSR      memset
  MOV.Q    messageString, RQ25
  EXTS.L   RD10
  LEA.B    @(RQ25, RD10), RQ13
  EXTS.W   RD12, RQ11
  MOV      #84, RQ4
  ADD      R15, RQ4
  MOV      RQ13, RQ5
  MOV      RQ11, RQ6
  BSR      strncpy
  ADD      RD12, #1, RD14
  ADD      RD14, RD10
  BRA      L00800EF9

L00800EFA:

L00800EF8:
  ADD      #1, RD12
  BRA      L00800EF7

L00800EF9:
  MOV.Q    messageString, RQ25
  EXTS.L   RD10
  LEA.B    @(RQ25, RD10), RQ13
  MOV      RQ13, RQ4
  BSR      strlen
  MOV      RQ2, RQ11
  EXTS.W   RD12, RQ26
  MOV      RQ11, RQ27
  MOV.Q    RQ27, @(R15, 24)
  MOV.Q    RQ26, @(R15, 32)
  CMPQ/EQ  RQ27, RQ26
  BT       L00800EFB
  MOV.Q    messageString, RQ25
  EXTS.L   RD10
  LEA.B    @(RQ25, RD10), RQ13
  MOV      #84, RQ4
  ADD      R15, RQ4
  MOV      RQ13, RQ5
  BSR      strcpy
  ADD      RD12, RD10

L00800EFB:
  MOV      #84, RQ4
  ADD      R15, RQ4
  BSR      M_StringWidth
  MOV      RD2, RD14
  SHAD     RD14, #-1, RD9
  MOV      #160, RD14
  EXTS.W   RD9, RD25
  SUB      RD14, RD25, RD26
  MOV.W    RD26, x
  MOV.L    RD25, @(R15, 60)
  MOV.W    x, RD4
  MOV.W    y, RD5
  MOV      #84, RQ6
  ADD      R15, RQ6
  BSR      M_WriteText
  MOV      hu_font, RQ5
  MOV.Q    @RQ5, RQ13
  MOV.W    @(RQ13, 2), RD14
  MOV.W    y, RD4
  ADD      RD14, RD4
  MOV.W    RD4, y
  BRA      L00800EF5

L00800EF6:
  BRA      L00E00569

L00800EF4:
  MOV.L    menuactive, RD3
  CMP/EQ   RD3
  BT       L00800EFC
  BRA      L00E00569

L00800EFC:
  MOV.Q    currentMenu, RQ2
  MOV.Q    @(RQ2, 24), RQ13
  MOV      #0, RQ23
  CMPQ/EQ  RQ23, RQ13
  BT       L00800EFD
  MOV.Q    currentMenu, RQ25
  MOV.Q    @(RQ25, 24), RQ13
  JSR      RQ13

L00800EFD:
  MOV.Q    currentMenu, RQ22
  MOV.W    @(RQ22, 32), RD21
  MOV.W    @(RQ22, 34), RD20
  MOV.W    @RQ22, RD19
  MOV      #0, RD14
  MOV      RD14, RD12
  MOV.W    RD21, x
  MOV.W    RD20, y
  MOV.L    RD19, @(R15, 80)

L00800EFE:
  MOV.L    @(R15, 80), RD18
  CMP/GT   RD12, RD18
  BT       L00800F00
  MOV.Q    currentMenu, RQ17
  MOV.Q    @(RQ17, 16), RQ13
  EXTS.L   RD12
  SHAD     RD12, #5, RQ16
  ADD      RQ13, RQ16
  ADD      RQ16, #2, RQ13
  MOV.B    @RQ13, RD14
  MOV.Q    RQ16, @(R15, 8)
  CMP/EQ   RD14
  BT       L00800F01
  MOV.Q    currentMenu, RQ25
  MOV.Q    @(RQ25, 16), RQ13
  EXTS.L   RD12
  SHAD     RD12, #5, RQ26
  ADD      RQ13, RQ26
  ADD      RQ26, #2, RQ13
  MOV.Q    RQ26, @(R15, 8)
  MOV.W    x, RD4
  MOV.W    y, RD5
  MOV      #0, RD6
  MOV      RQ13, RQ7
  BSR      V_DrawPatchDirectName

L00800F01:
  MOV.W    y, RD16
  MOV      RD16, RD14
  MOV      #16, RD9
  ADD      RD14, RD9, RD16
  MOV.W    RD16, y

L00800EFF:
  ADD      #1, RD12
  BRA      L00800EFE

L00800F00:
  MOV      skullName, RQ25
  MOV.W    whichSkull, RD26
  EXTS.L   RD26
  MOV      #9, RD7
  MUL.L    RD26, RD7
  STS      R0, RQ13
  ADD      RQ25, RQ13
  MOV.Q    currentMenu, RQ27
  MOV.W    @(RQ27, 34), RD14
  SUB      RD14, #5, RD9
  MOV.W    itemOn, RD28
  SHAD     RD28, #4, RD14
  ADD      RD9, RD14, RD29
  MOV.W    x, RD30
  ADD      RD30, #-32, RD14
  MOV.L    RD29, @(R15, 60)
  MOV      RD14, RD4
  MOV      RD29, RD5
  MOV      #0, RD6
  MOV      RQ13, RQ7
  BSR      V_DrawPatchDirectName

L00E00569:
  ADD      #408, R15
  BRA      L00C00194

M_Init:
  PUSH     LR
  BSR      L00C00193
  ADD      #-88, R15
  MOV      MainDef, RQ12
  MOV      RQ12, RQ13
  MOV      RQ13, RQ11
  MOV      #0, RD10
  MOV.W    @(RQ11, 36), RD9
  MOV      #0, RD14
  MOV      RD14, RD8
  MOV      #10, RD14
  MOV      RD14, RD31
  MOV.L    screenblocks, RD30
  SUB      RD30, #3, RD29
  MOV      #0, RD28
  MOV      #0, RQ27
  MOV      RD10, RD26
  MOV      #-1, RD25
  MOV.Q    RQ11, currentMenu
  MOV.L    RD10, menuactive
  MOV.W    RD9, itemOn
  MOV.W    RD8, whichSkull
  MOV.W    RD31, skullAnimCounter
  MOV.L    RD29, screenSize
  MOV.L    RD28, messageToPrint
  MOV.Q    RQ27, messageString
  MOV.L    RD26, messageLastMenuActive
  MOV.L    RD25, quickSaveSlot
  MOV.L    gamemode, RD25
  CMP/EQ   #2, RD25
  BT       L00800F04
  MOV.L    gamemode, RD7
  CMP/EQ   RD7
  BT       L00800F05
  MOV.L    gamemode, RD6
  CMP/EQ   #1, RD6
  BT       L00800F06
  MOV.L    gamemode, RD5
  CMP/EQ   #3, RD5
  BT       L00800F07
  BRA      L00800F08

L00800F04:
  MOV      MainMenu, RQ25
  ADD      RQ25, #160, RQ26
  ADD      RQ25, #128, RQ7
  MOV.Q    @RQ26, RQ6
  MOV.Q    @(RQ26, 8), RQ5
  MOV.Q    RQ6, @RQ7
  MOV.Q    RQ5, @(RQ7, 8)
  MOV.Q    @(RQ26, 16), RQ6
  MOV.Q    @(RQ26, 24), RQ5
  MOV.Q    RQ6, @(RQ7, 16)
  MOV.Q    RQ5, @(RQ7, 24)
  MOV      MainDef, RQ27
  MOV.W    @RQ27, RD14
  ADD      RD14, #-1, RD28
  MOV.W    RD28, @RQ27
  MOV.W    @(RQ27, 34), RD14
  ADD      RD14, #8, RD28
  MOV.W    RD28, @(RQ27, 34)
  MOV      RQ27, RQ13
  MOV      NewDef, RQ29
  MOV.Q    RQ13, @(RQ29, 8)
  MOV      M_DrawReadThis1, RQ30
  MOV      RQ30, RQ13
  MOV      ReadDef1, RQ31
  MOV.Q    RQ13, @(RQ31, 24)
  MOV      #330, RD8
  MOV.W    RD8, @(RQ31, 32)
  MOV      #165, RD9
  MOV.W    RD9, @(RQ31, 34)
  MOV      ReadMenu1, RQ10
  MOV      RQ10, RQ26
  MOV      M_FinishReadThis, RQ11
  MOV      RQ11, RQ13
  MOV.Q    RQ13, @(RQ26, 16)
  MOV.L    RD28, @(R15, 4)
  BRA      L00800F03

L00800F05:

L00800F06:
  MOV      EpiDef, RQ4
  MOV.W    @RQ4, RD14
  ADD      RD14, #-1, RD3
  MOV.W    RD3, @RQ4
  MOV.L    RD3, @(R15, 4)
  BRA      L00800F03

L00800F07:

L00800F08:
  BRA      L00800F03

L00800F03:

L00E0056A:
  ADD      #88, R15
  BRA      L00C00194

M_StartControlPanel:
  PUSH     LR
  BSR      L00C00198
  ADD      #-104, R15
  MOV.L    menuactive, RD14
  CMP/EQ   RD14
  BT       L00800EF3
  BRA      L00E0056B

L00800EF3:
  MOV      #1, RD13
  MOV      MainDef, RQ11
  MOV      RQ11, RQ12
  MOV      RQ12, RQ10
  MOV.W    @(RQ10, 36), RD9
  MOV.L    RD13, menuactive
  MOV.Q    RQ10, currentMenu
  MOV.W    RD9, itemOn

L00E0056B:
  ADD      #104, R15
  BRA      L00C0019A

G_DeathMatchSpawnPlayer:
  PUSH     LR
  BSR      L00C00193
  ADD      #-152, R15
  MOV.L    RD4, @(R15, 24)
  MOV.Q    deathmatch_p, RQ11
  MOV      deathmatchstarts, RQ10
  MOV      RQ11, RD13
  SUB      RQ10, RD13
  MOV      #0x0CCCCCCD, RD7
  MOV      #3276, RD7
  DMULS    RD7, RD13
  STS      R1, RD13
  MOV      #4, RD9
  CMP/GE   RD9, RD13
  BT       L00800D6E
  MOV      L00C0061D, RQ4
  MOV      RD13, RD5
  BSR      I_Error

L00800D6E:
  MOV      #0, RD12

L00800D6F:
  CMP/GE   #20, RD12
  BT       L00800D71
  BSR      P_Random
  MOV      RD2, RD14
  MOV      RD13, RD5
  MOV      RD14, RD4
  BSR      __smodsi3
  MOV      RD2, RD8
  MOV      deathmatchstarts, RQ31
  EXTS.L   RD8
  MOV      #20, RD7
  MUL.L    RD8, RD7
  STS      R0, RQ30
  ADD      RQ31, RQ30
  MOV.L    RD8, @(R15, 20)
  MOV.Q    RQ30, @(R15, 8)
  MOV.L    @(R15, 24), RD4
  MOV      RQ30, RQ5
  BSR      G_CheckSpot
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L00800D72
  MOV.L    @(R15, 24), RD29
  ADD      RD29, #1, RD14
  MOV      deathmatchstarts, RQ28
  MOV.L    @(R15, 20), RD27
  EXTS.L   RD27
  MOV      #20, RD7
  MUL.L    RD27, RD7
  STS      R0, RQ26
  ADD      RQ28, RQ26
  MOV.W    RD14, @(RQ26, 10)
  EXTS.L   RD27
  MOV      #20, RD7
  MUL.L    RD27, RD7
  STS      R0, RQ25
  ADD      RQ28, RQ25
  MOV.Q    RQ25, @(R15, 8)
  MOV      RQ25, RQ4
  BSR      P_SpawnPlayer
  BRA      L00E0056C

L00800D72:

L00800D70:
  ADD      #1, RD12
  BRA      L00800D6F

L00800D71:
  MOV      playerstarts, RQ25
  MOV.L    @(R15, 24), RD26
  EXTS.L   RD26
  MOV      #20, RD7
  MUL.L    RD26, RD7
  STS      R0, RQ27
  ADD      RQ25, RQ27
  MOV.Q    RQ27, @(R15, 8)
  MOV      RQ27, RQ4
  BSR      P_SpawnPlayer

L00E0056C:
  ADD      #152, R15
  BRA      L00C00194

G_InitNew:
  PUSH     LR
  BSR      L00C00193
  ADD      #-520, R15
  MOV.L    RD4, @(R15, 40)
  MOV.L    RD5, @(R15, 36)
  MOV.L    RD6, @(R15, 32)
  MOV.L    paused, RD13
  CMP/EQ   RD13
  BT       L00800DC4
  MOV      #0, RD12
  MOV.L    RD12, paused
  BSR      S_ResumeSound

L00800DC4:
  MOV.L    @(R15, 40), RD11
  CMP/GT   #4, RD11
  BT       L00800DC5
  MOV      #4, RD10
  MOV.L    RD10, @(R15, 40)

L00800DC5:
  MOV.L    @(R15, 36), RD9
  MOV      #1, RD8
  CMP/GE   RD8, RD9
  BT       L00800DC6
  MOV      #1, RD31
  MOV.L    RD31, @(R15, 36)

L00800DC6:
  MOV.L    gamemode, RD30
  CMP/EQ   #3, RD30
  BT       L00800DC7
  MOV.L    @(R15, 36), RD29
  CMP/GT   #4, RD29
  BT       L00800DC8
  MOV      #4, RD28
  MOV.L    RD28, @(R15, 36)

L00800DC8:
  BRA      L00800DC9

L00800DC7:
  MOV.L    gamemode, RD27
  CMP/EQ   #0, RD27
  BT       L00800DCA
  MOV.L    @(R15, 36), RD26
  CMP/GT   #1, RD26
  BT       L00800DCB
  MOV      #1, RD25
  MOV.L    RD25, @(R15, 36)

L00800DCB:
  BRA      L00800DCC

L00800DCA:
  MOV.L    @(R15, 36), RD7
  CMP/GT   #3, RD7
  BT       L00800DCD
  MOV      #3, RD25
  MOV.L    RD25, @(R15, 36)

L00800DCD:

L00800DCC:

L00800DC9:
  MOV.L    @(R15, 32), RD6
  MOV      #1, RD5
  CMP/GE   RD5, RD6
  BT       L00800DCE
  MOV      #1, RD25
  MOV.L    RD25, @(R15, 32)

L00800DCE:
  MOV.L    @(R15, 32), RD4
  CMP/GT   #9, RD4
  BT       L00800DCF
  MOV.L    gamemode, RD3
  CMP/EQ   #2, RD3
  BT       L00800DCF
  BRA      L00800DD0

L00800DD0:
  MOV      #9, RD2
  MOV.L    RD2, @(R15, 32)

L00800DCF:
  BSR      M_ClearRandom
  MOV.L    @(R15, 40), RD23
  CMP/EQ   #4, RD23
  BT       L00800DD3
  MOV.L    respawnparm, RD22
  CMP/EQ   #0, RD22
  BT       L00800DD3
  BRA      L00800DD2

L00800DD3:
  MOV      #1, RD21
  MOV.L    RD21, respawnmonsters
  BRA      L00800DD5

L00800DD2:
  MOV      #0, RD20
  MOV.L    RD20, respawnmonsters

L00800DD5:
  MOV.L    fastparm, RD19
  CMP/EQ   #0, RD19
  BT       L00800DD7
  MOV.L    @(R15, 40), RD18
  CMP/EQ   #4, RD18
  BT       L00800DD6
  MOV.L    gameskill, RD17
  CMP/EQ   #4, RD17
  BT       L00800DD6
  BRA      L00800DD7

L00800DD7:
  MOV      #477, RD14

L00800DDA:
  CMP/GT   #489, RD14
  BT       L00800DDC
  MOV      states, RQ25
  EXTS.L   RD14
  MOV      #40, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ26
  ADD      RQ25, RQ26
  MOV.L    @(RQ26, 8), RD27
  SHAD     RD27, #-1, RD28
  EXTS.L   RD14
  MOV      #40, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ26
  ADD      RQ25, RQ26
  MOV.L    RD28, @(RQ26, 8)
  MOV.L    RD28, @(R15, 16)
  MOV.L    RD27, @(R15, 20)
  MOV.Q    RQ26, @(R15, 24)

L00800DDB:
  ADD      #1, RD14
  BRA      L00800DDA

L00800DDC:
  MOV      mobjinfo, RQ16
  ADD      RQ16, #1472, RQ17
  MOV      #0x00140000, RD18
  MOV      #20, RD18
  MOV.L    RD18, @(RQ17, 60)
  ADD      RQ16, #2944, RQ17
  MOV.L    RD18, @(RQ17, 60)
  ADD      RQ16, #2852, RQ17
  MOV.L    RD18, @(RQ17, 60)
  MOV.Q    RQ17, @(R15, 8)
  BRA      L00800DDD

L00800DD6:
  MOV.L    @(R15, 40), RD16
  CMP/EQ   #4, RD16
  BT       L00800DDE
  MOV.L    gameskill, RD16
  CMP/EQ   #4, RD16
  BT       L00800DDE
  BRA      L00800DDF

L00800DDF:
  MOV      #477, RD14

L00800DE1:
  CMP/GT   #489, RD14
  BT       L00800DE3
  MOV      states, RQ25
  EXTS.L   RD14
  MOV      #40, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ26
  ADD      RQ25, RQ26
  MOV.L    @(RQ26, 8), RD27
  SHAD     RD27, #1, RD28
  EXTS.L   RD14
  MOV      #40, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ26
  ADD      RQ25, RQ26
  MOV.L    RD28, @(RQ26, 8)
  MOV.L    RD28, @(R15, 16)
  MOV.L    RD27, @(R15, 20)
  MOV.Q    RQ26, @(R15, 24)

L00800DE2:
  ADD      #1, RD14
  BRA      L00800DE1

L00800DE3:
  MOV      mobjinfo, RQ16
  ADD      RQ16, #1472, RQ17
  MOV      #0x000F0000, RD18
  MOV      #15, RD18
  MOV.L    RD18, @(RQ17, 60)
  ADD      RQ16, #2944, RQ17
  MOV      #0x000A0000, RD19
  MOV      #10, RD19
  MOV.L    RD19, @(RQ17, 60)
  ADD      RQ16, #2852, RQ17
  MOV.L    RD19, @(RQ17, 60)
  MOV.Q    RQ17, @(R15, 8)

L00800DDE:

L00800DDD:
  MOV      #0, RD14

L00800DE4:
  MOV      #4, RD16
  CMP/GE   RD16, RD14
  BT       L00800DE6
  MOV      players, RQ25
  EXTS.L   RD14
  MOV      #320, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ26
  ADD      RQ25, RQ26
  MOV      #2, RD27
  MOV.L    RD27, @(RQ26, 8)

L00800DE5:
  ADD      #1, RD14
  BRA      L00800DE4

L00800DE6:
  MOV      #1, RD16
  MOV      #0, RD17
  MOV      #0, RD18
  MOV      #0, RD19
  MOV      #1, RD20
  MOV.L    @(R15, 36), RD22
  MOV      RD22, RD21
  MOV.L    @(R15, 32), RD2
  MOV      RD2, RD23
  MOV.L    @(R15, 40), RD4
  MOV      RD4, RD3
  MOV      #1, RD20
  MOV.L    RD3, gameskill
  MOV.L    RD23, gamemap
  MOV.L    RD21, gameepisode
  MOV.L    RD20, viewactive
  MOV.L    RD19, automapactive
  MOV.L    RD18, demoplayback
  MOV.L    RD17, paused
  MOV.L    RD16, usergame
  MOV.L    gamemode, RD5
  CMP/EQ   #2, RD5
  BT       L00800DE7
  MOV      L00C00621, RQ4
  BSR      R_TextureNumForName
  MOV      R2, RD25
  MOV.L    RD25, skytexture
  MOV.L    gamemap, RD25
  MOV      #12, RD26
  CMP/GE   RD26, RD25
  BT       L00800DE8
  MOV      L00C00622, RQ4
  BSR      R_TextureNumForName
  MOV      R2, RD25
  MOV.L    RD25, skytexture
  BRA      L00800DE9

L00800DE8:
  MOV.L    gamemap, RD16
  MOV      #21, RD17
  CMP/GE   RD17, RD16
  BT       L00800DEA
  MOV      L00C00623, RQ4
  BSR      R_TextureNumForName
  MOV      R2, RD25
  MOV.L    RD25, skytexture

L00800DEA:

L00800DE9:
  BRA      L00800DEB

L00800DE7:
  MOV.L    @(R15, 36), RD16
  CMP/EQ   #1, RD16
  BT       L00800DED
  MOV.L    @(R15, 36), RD16
  CMP/EQ   #2, RD16
  BT       L00800DEE
  MOV.L    @(R15, 36), RD16
  CMP/EQ   #3, RD16
  BT       L00800DEF
  MOV.L    @(R15, 36), RD16
  CMP/EQ   #4, RD16
  BT       L00800DF0
  BRA      L00800DEC

L00800DED:
  MOV      L00C00622, RQ4
  BSR      R_TextureNumForName
  MOV      R2, RD25
  MOV.L    RD25, skytexture
  BRA      L00800DEC

L00800DEE:
  MOV      L00C00623, RQ4
  BSR      R_TextureNumForName
  MOV      R2, RD25
  MOV.L    RD25, skytexture
  BRA      L00800DEC

L00800DEF:
  MOV      L00C00621, RQ4
  BSR      R_TextureNumForName
  MOV      R2, RD25
  MOV.L    RD25, skytexture
  BRA      L00800DEC

L00800DF0:
  MOV      L00C00624, RQ4
  BSR      R_TextureNumForName
  MOV      R2, RD25
  MOV.L    RD25, skytexture
  BRA      L00800DEC

L00800DEC:

L00800DEB:
  BSR      G_DoLoadLevel

L00E0056D:
  ADD      #520, R15

L00C00626:
  POP      RQ8
  POP      RQ9
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  POP      RQ25
  POP      RQ26
  POP      RQ27
  POP      RQ28
  POP      RQ29
  POP      RQ30
  POP      RQ31
  RET     

G_DeferedInitNew:
  PUSH     LR
  BSR      L00C00198
  ADD      #-104, R15
  MOV.L    RD4, @(R15, 12)
  MOV.L    RD5, @(R15, 8)
  MOV.L    RD6, @(R15, 4)
  MOV.L    @(R15, 12), RD13
  MOV      RD13, RD14
  MOV.L    @(R15, 8), RD11
  MOV      RD11, RD12
  MOV.L    @(R15, 4), RD9
  MOV      RD9, RD10
  MOV      #2, RD13
  MOV.L    RD14, d_skill
  MOV.L    RD13, gameaction
  MOV.L    RD12, d_episode
  MOV.L    RD10, d_map

L00E0056E:
  ADD      #104, R15
  BRA      L00C0019A

G_DeferedPlayDemo:
  PUSH     R14
  PUSH     R13
  ADD      #-144, R15
  MOV.Q    RQ4, @(R15, 8)
  MOV.Q    @(R15, 8), RQ13
  MOV      RQ13, RQ14
  MOV      #5, RD13
  MOV.Q    RQ14, defdemoname
  MOV.L    RD13, gameaction

L00E0056F:
  ADD      #144, R15
  POP      RQ13
  POP      RQ14
  RTS     

G_LoadGame:
  PUSH     LR
  ADD      #-152, R15
  MOV.Q    RQ4, @(R15, 8)
  MOV      savename, RQ4
  MOV.Q    @(R15, 8), RQ5
  BSR      strcpy
  MOV      #3, RD7
  MOV.L    RD7, gameaction

L00E00570:
  ADD      #152, R15
  RET     

L00C00630:
  PUSH     R31
  PUSH     R30
  PUSH     R29
  PUSH     R28
  PUSH     R27
  PUSH     R26
  PUSH     R25
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  PUSH     R9
  PUSH     R8
  RTS     

G_DoLoadGame:
  PUSH     LR
  BSR      L00C00630
  ADD      #-104, R15
  MOV      #64, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 40)
  MOV      #0, RD11
  MOV      savebuffer, RQ13
  MOV.L    RD11, gameaction
  MOV      savename, RQ4
  MOV      RQ13, RQ5
  BSR      M_ReadFile
  MOV      RD2, RD12
  MOV.Q    savebuffer, RQ10
  ADD      RQ10, #24, RQ9
  MOV      #16, RQ8
  EXTS.L   RQ8, RQ8
  MOV.Q    RQ9, save_p
  MOV.Q    RQ8, @(R15, 24)
  MOV      #64, RQ4
  ADD      R15, RQ4
  MOV      #0, RD5
  MOV      RQ8, RQ6
  BSR      memset
  MOV      #64, RQ4
  ADD      R15, RQ4
  MOV      L00C00633, RQ5
  MOV      #110, RD6
  BSR      sprintf
  MOV.Q    save_p, RQ4
  MOV      #64, RQ5
  ADD      R15, RQ5
  BSR      strcmp
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L00800DB8
  BRA      L00E00571

L00800DB8:
  MOV.Q    save_p, RQ31
  ADD      #16, RQ31
  MOV      RQ31, RQ13
  ADD      #1, RQ31
  MOVU.B   @RQ13, RD14
  MOV      RD14, RD30
  MOV      RQ31, RQ13
  ADD      #1, RQ31
  MOVU.B   @RQ13, RD14
  MOV      RD14, RD29
  MOV      RQ31, RQ13
  ADD      #1, RQ31
  MOVU.B   @RQ13, RD14
  MOV      RD14, RD28
  MOV      #0, RD27
  MOV.Q    RQ31, save_p
  MOV.L    RD30, gameskill
  MOV.L    RD29, gameepisode
  MOV.L    RD28, gamemap
  MOV.L    RD27, @(R15, 60)

L00800DB9:
  MOV.L    @(R15, 60), RD26
  MOV      #4, RD25
  CMP/GE   RD25, RD26
  BT       L00800DBB
  MOV.Q    save_p, RQ7
  MOV      RQ7, RQ13
  ADD      #1, RQ7
  MOVU.B   @RQ13, RD14
  MOV      playeringame, RQ6
  MOV.L    @(R15, 60), RD5
  MOV.L    RD14, @(RQ6, RD5)
  MOV.Q    RQ7, save_p

L00800DBA:
  MOV.L    @(R15, 60), RD4
  ADD      #1, RD4
  MOV.L    RD4, @(R15, 60)
  BRA      L00800DB9

L00800DBB:
  MOV.L    gameskill, RD4
  MOV.L    gameepisode, RD5
  MOV.L    gamemap, RD6
  BSR      G_InitNew
  MOV.Q    save_p, RQ25
  MOV      RQ25, RQ13
  ADD      #1, RQ25
  MOVU.B   @RQ13, RD14
  MOV      RD14, RD26
  MOV      RQ25, RQ13
  ADD      #1, RQ25
  MOVU.B   @RQ13, RD14
  MOV      RD14, RD27
  MOV      RQ25, RQ13
  ADD      #1, RQ25
  MOVU.B   @RQ13, RD14
  MOV      RD14, RD28
  SHAD     RD26, #16, RD14
  SHAD     RD27, #8, RD29
  ADD      RD14, RD29, RD30
  ADD      RD30, RD28, RD31
  MOV.L    RD31, leveltime
  MOV.L    RD28, @(R15, 48)
  MOV.L    RD27, @(R15, 52)
  MOV.L    RD26, @(R15, 56)
  MOV.Q    RQ25, save_p
  BSR      P_UnArchivePlayers
  BSR      P_UnArchiveWorld
  BSR      P_UnArchiveThinkers
  BSR      P_UnArchiveSpecials
  MOV.Q    save_p, RQ3
  MOVU.B   @RQ3, RD14
  CMP/EQ   #29, RD14
  BT       L00800DBC
  MOV      L00C00638, RQ4
  BSR      I_Error

L00800DBC:
  MOV.Q    savebuffer, RQ4
  BSR      Z_Free
  MOV.L    setsizeneeded, RD2
  CMP/EQ   RD2
  BT       L00800DBD
  BSR      R_ExecuteSetViewSize

L00800DBD:
  BSR      R_FillBackScreen

L00E00571:
  ADD      #104, R15
  BRA      L00C00626

G_SaveGame:
  PUSH     LR
  PUSH     R14
  PUSH     R13
  ADD      #-136, R15
  MOV.L    RD4, @(R15, 12)
  MOV.Q    RQ5, @R15
  MOV.L    @(R15, 12), RD13
  MOV      RD13, RD14
  MOV.L    RD14, savegameslot
  MOV      savedescription, RQ4
  MOV.Q    @R15, RQ5
  BSR      strcpy
  MOV      #1, RD14
  MOV.L    RD14, sendsave

L00E00572:
  ADD      #136, R15
  POP      RQ13
  POP      RQ14
  RET     

G_RecordDemo:
  PUSH     LR
  BSR      L00C002C0
  ADD      #-96, R15
  MOV.Q    RQ4, @(R15, 24)
  MOV      #0, RD10
  MOV.L    RD10, usergame
  MOV      demoname, RQ4
  MOV.Q    @(R15, 24), RQ5
  BSR      strcpy
  MOV      demoname, RQ4
  MOV      L00C00642, RQ5
  BSR      strcat
  MOV      #0x00020000, RD11
  MOV      #2, RD11
  MOV      L00C00643, RQ4
  BSR      M_CheckParm
  MOV      RD2, RD13
  CMP/EQ   RD13
  BT       L00800DF4
  MOV.L    myargc, RD9
  SUB      RD9, #1, RD14
  CMP/GT   RD13, RD14
  BT       L00800DF4
  BRA      L00800DF5

L00800DF5:
  ADD      RD13, #1, RD14
  MOV.Q    myargv, RQ8
  EXTS.L   RD14
  MOV.Q    @(RQ8, RD14), RQ12
  MOV      RQ12, RQ4
  BSR      atoi
  MOV      RD2, RD14
  SHAD     RD14, #10, RD11

L00800DF4:
  MOV      #0, RQ12
  MOV      RD11, RD4
  MOV      #1, RD5
  MOV      RQ12, RQ6
  BSR      Z_Malloc
  MOV      RQ2, RQ31
  EXTS.L   RD11
  LEA.B    @(RQ31, RD11), RQ30
  MOV      #1, RD8
  MOV.L    RD8, demorecording
  MOV.Q    RQ31, demobuffer
  MOV.Q    RQ30, demoend

L00E00573:
  ADD      #96, R15
  BRA      L00C002C3

G_BeginRecording:
  PUSH     LR
  BSR      L00C00630
  ADD      #-56, R15
  MOV.Q    demobuffer, RQ13
  MOV      RQ13, RQ14
  MOV      RQ14, RQ12
  ADD      #1, RQ14
  MOV      #110, RD11
  MOVU.B   RD11, @RQ12
  MOV      RQ14, RQ12
  ADD      #1, RQ14
  MOV.L    gameskill, RD10
  MOVU.B   RD10, @RQ12
  MOV      RQ14, RQ12
  ADD      #1, RQ14
  MOV.L    gameepisode, RD9
  MOVU.B   RD9, @RQ12
  MOV      RQ14, RQ12
  ADD      #1, RQ14
  MOV.L    gamemap, RD8
  MOVU.B   RD8, @RQ12
  MOV      RQ14, RQ12
  ADD      #1, RQ14
  MOV.L    deathmatch, RD31
  MOVU.B   RD31, @RQ12
  MOV      RQ14, RQ12
  ADD      #1, RQ14
  MOV.L    respawnparm, RD30
  MOVU.B   RD30, @RQ12
  MOV      RQ14, RQ12
  ADD      #1, RQ14
  MOV.L    fastparm, RD29
  MOVU.B   RD29, @RQ12
  MOV      RQ14, RQ12
  ADD      #1, RQ14
  MOV.L    nomonsters, RD28
  MOVU.B   RD28, @RQ12
  MOV      RQ14, RQ12
  ADD      #1, RQ14
  MOV.L    consoleplayer, RD27
  MOVU.B   RD27, @RQ12
  MOV      #0, RD26
  MOV.Q    RQ14, demo_p
  MOV.Q    RQ12, @(R15, 16)
  MOV.L    RD26, @(R15, 28)

L00800DF7:
  MOV.L    @(R15, 28), RD25
  MOV      #4, RD7
  CMP/GE   RD7, RD25
  BT       L00800DF9
  MOV      playeringame, RQ6
  MOV.L    @(R15, 28), RD5
  MOV.L    @(RQ6, RD5), RD4
  MOV.Q    demo_p, RQ2
  MOV      RQ2, RQ3
  ADD      #1, RQ2
  MOVU.B   RD4, @RQ3
  MOV.Q    RQ3, @(R15, 16)
  MOV.Q    RQ2, demo_p

L00800DF8:
  MOV.L    @(R15, 28), RD23
  ADD      #1, RD23
  MOV.L    RD23, @(R15, 28)
  BRA      L00800DF7

L00800DF9:

L00E00574:
  ADD      #56, R15
  BRA      L00C00626

L00C00649:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  PUSH     R9
  RTS     

G_TimeDemo:
  PUSH     LR
  BSR      L00C00649
  ADD      #-104, R15
  MOV.Q    RQ4, @(R15, 8)
  MOV      L00C0064A, RQ4
  BSR      M_CheckParm
  MOV      R2, RD14
  MOV.L    RD14, nodrawers
  MOV      L00C0064B, RQ4
  BSR      M_CheckParm
  MOV      R2, RD13
  MOV      #1, RD12
  MOV      #1, RD11
  MOV.Q    @(R15, 8), RQ9
  MOV      RQ9, RQ10
  MOV      #5, RD14
  MOV.L    RD14, gameaction
  MOV.L    RD13, noblit
  MOV.L    RD12, timingdemo
  MOV.L    RD11, singletics
  MOV.Q    RQ10, defdemoname

L00E00575:
  ADD      #104, R15

L00C0064D:
  POP      RQ9
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  RET     

G_CheckDemoStatus:
  PUSH     LR
  BSR      L00C00630
  ADD      #-56, R15
  MOV.L    timingdemo, RD11
  CMP/EQ   RD11
  BT       L00800E02
  BSR      I_GetTime
  MOV      RD2, RD12
  MOV.L    starttime, RD10
  SUB      RD12, RD10, RD14
  MOV      L00C00650, RQ4
  MOV.L    gametic, RD5
  MOV      RD14, RD6
  BSR      I_Error

L00800E02:
  MOV.L    demoplayback, RD9
  CMP/EQ   RD9
  BT       L00800E03
  MOV.L    singledemo, RD8
  CMP/EQ   RD8
  BT       L00800E04
  BSR      I_Quit

L00800E04:
  MOV.Q    demobuffer, RQ31
  MOV      #0, RQ30
  CMPQ/EQ  RQ30, RQ31
  BT       L00800E05
  MOV.Q    demobuffer, RQ29
  MOV      RQ29, RQ13
  ADD      RQ13, #-40, RQ28
  MOV.L    @(RQ28, 20), RD14
  CMP/EQ   #1919505, RD14
  BT       L00800E06
  MOV      L00C00651, RQ4
  MOV      #1676, RD5
  BSR      I_Error

L00800E06:
  MOV.Q    demobuffer, RQ4
  MOV      #101, RD5
  BSR      Z_ChangeTag2

L00800E05:
  MOV      #0, RD27
  MOV      #0, RD26
  MOV      #0, RD25
  MOV      #0, RD28
  MOV      playeringame, RQ29
  MOV      #0, RD30
  MOV.L    RD30, @(RQ29, 12)
  MOV.L    RD30, @(RQ29, 8)
  MOV.L    RD30, @(RQ29, 4)
  MOV      #0, RD31
  MOV      #0, RD8
  MOV      #0, RD9
  MOV      #0, RD10
  MOV.L    RD10, consoleplayer
  MOV.L    RD9, nomonsters
  MOV.L    RD8, fastparm
  MOV.L    RD31, respawnparm
  MOV.L    RD28, deathmatch
  MOV.L    RD27, demoplayback
  MOV.L    RD26, netdemo
  MOV.L    RD25, netgame
  BSR      D_AdvanceDemo
  MOV      #1, RD2
  BRA      L00E00576

L00800E03:
  MOV.L    demorecording, RD7
  CMP/EQ   RD7
  BT       L00800E07
  MOV.Q    demo_p, RQ25
  MOV      RQ25, RQ13
  ADD      #1, RQ25
  MOV      #128, RD26
  MOVU.B   RD26, @RQ13
  MOV.Q    demobuffer, RQ27
  SUB      RQ25, RQ27, RD14
  MOV.Q    RQ25, demo_p
  MOV      demoname, RQ4
  MOV.Q    demobuffer, RQ5
  MOV      RD14, RD6
  BSR      M_WriteFile
  MOV.Q    demobuffer, RQ4
  BSR      Z_Free
  MOV      #0, RD28
  MOV.L    RD28, demorecording
  MOV      L00C00654, RQ4
  MOV      demoname, RQ5
  BSR      I_Error

L00800E07:
  MOV      #0, RD2

L00E00576:
  ADD      #56, R15
  BRA      L00C00626

G_ExitLevel:
  ADD      #-144, R15
  MOV      #0, RD7
  MOV.L    RD7, secretexit
  MOV      #6, RD7
  MOV.L    RD7, gameaction

L00E00577:
  ADD      #144, R15
  RTS     

G_SecretExitLevel:
  PUSH     LR
  PUSH     R14
  PUSH     R13
  ADD      #-136, R15
  MOV.L    gamemode, RD13
  CMP/EQ   #2, RD13
  BT       L00800D7B
  MOV      L00C00658, RQ4
  BSR      W_CheckNumForName
  MOV      RD2, RD14
  CMP/PZ   RD14
  BT       L00800D7B
  BRA      L00800D7C

L00800D7C:
  MOV      #0, RD13
  MOV.L    RD13, secretexit
  BRA      L00800D7E

L00800D7B:
  MOV      #1, RD13
  MOV.L    RD13, secretexit

L00800D7E:
  MOV      #6, RD13
  MOV.L    RD13, gameaction

L00E00578:
  ADD      #136, R15
  POP      RQ13
  POP      RQ14
  RET     

G_WorldDone:
  PUSH     LR
  BSR      L00C002C5
  ADD      #-384, R15
  MOV      #8, RD14
  MOV.L    RD14, gameaction
  MOV.L    secretexit, RD13
  CMP/EQ   RD13
  BT       L00800DAE
  MOV      players, RQ12
  MOV.L    consoleplayer, RD11
  MOV      #320, RD7
  MUL.L    RD11, RD7
  STS      R0, RQ10
  ADD      RQ12, RQ10
  MOV      #1, RD9
  MOV.L    RD9, @(RQ10, 312)

L00800DAE:
  MOV.L    gamemode, RD8
  CMP/EQ   #2, RD8
  BT       L00800DAF
  MOV.L    gamemap, RD31
  CMP/EQ   #15, RD31
  BT       L00800DB1
  MOV.L    gamemap, RD30
  CMP/EQ   #31, RD30
  BT       L00800DB2
  MOV.L    gamemap, RD29
  CMP/EQ   #6, RD29
  BT       L00800DB3
  MOV.L    gamemap, RD28
  CMP/EQ   #11, RD28
  BT       L00800DB4
  MOV.L    gamemap, RD7
  CMP/EQ   #20, RD7
  BT       L00800DB5
  MOV.L    gamemap, RD6
  CMP/EQ   #30, RD6
  BT       L00800DB6
  BRA      L00800DB0

L00800DB1:

L00800DB2:
  MOV.L    secretexit, RD5
  CMP/EQ   RD5
  BT       L00800DB7
  BRA      L00800DB0

L00800DB7:

L00800DB3:

L00800DB4:

L00800DB5:

L00800DB6:
  BSR      F_StartFinale
  BRA      L00800DB0

L00800DB0:

L00800DAF:

L00E00579:
  ADD      #384, R15
  BRA      L00C002C6

G_Ticker:
  PUSH     LR
  BSR      L00C00630
  ADD      #-552, R15
  MOV      #0, RD13

L00800D29:
  MOV      #4, RD12
  CMP/GE   RD12, RD13
  BT       L00800D2B
  MOV      playeringame, RQ11
  EXTS.L   RD13
  MOV.L    @(RQ11, RD13), RD14
  CMP/EQ   RD14
  BT       L00800D2C
  MOV      players, RQ10
  EXTS.L   RD13
  MOV      #320, RD7
  MUL.L    RD13, RD7
  STS      R0, RQ9
  ADD      RQ10, RQ9
  MOV.L    @(RQ9, 8), RD14
  MOV.Q    RQ9, @(R15, 56)
  CMP/EQ   #2, RD14
  BT       L00800D2C
  BRA      L00800D2D

L00800D2D:
  MOV      RD13, RD4
  BSR      G_DoReborn

L00800D2C:

L00800D2A:
  ADD      #1, RD13
  BRA      L00800D29

L00800D2B:

L00800D2F:
  MOV.L    gameaction, RD8
  CMP/EQ   RD8
  BT       L00800D30
  MOV.L    gameaction, RD31
  MOV      #5, RD30
  CMP/GE   RD30, RD31
  BT       L00800D3D
  MOV.L    gameaction, RD29
  CMP/EQ   #0, RD29
  BT       L00800D3B
  MOV.L    gameaction, RD28
  CMP/EQ   #1, RD28
  BT       L00800D32
  MOV.L    gameaction, RD27
  CMP/EQ   #2, RD27
  BT       L00800D33
  MOV.L    gameaction, RD26
  CMP/EQ   #3, RD26
  BT       L00800D34
  MOV.L    gameaction, RD25
  CMP/EQ   #4, RD25
  BT       L00800D35

L00800D3D:
  MOV.L    gameaction, RD7
  CMP/EQ   #5, RD7
  BT       L00800D36
  MOV.L    gameaction, RD6
  CMP/EQ   #6, RD6
  BT       L00800D37
  MOV.L    gameaction, RD5
  CMP/EQ   #7, RD5
  BT       L00800D38
  MOV.L    gameaction, RD4
  CMP/EQ   #8, RD4
  BT       L00800D39
  MOV.L    gameaction, RD3
  CMP/EQ   #9, RD3
  BT       L00800D3A
  BRA      L00800D3C

L00800D32:
  BSR      G_DoLoadLevel
  BRA      L00800D31

L00800D33:
  BSR      G_DoNewGame
  BRA      L00800D31

L00800D34:
  BSR      G_DoLoadGame
  BRA      L00800D31

L00800D35:
  BSR      G_DoSaveGame
  BRA      L00800D31

L00800D36:
  BSR      G_DoPlayDemo
  BRA      L00800D31

L00800D37:
  BSR      G_DoCompleted
  BRA      L00800D31

L00800D38:
  BSR      F_StartFinale
  BRA      L00800D31

L00800D39:
  BSR      G_DoWorldDone
  BRA      L00800D31

L00800D3A:
  BSR      M_ScreenShot
  MOV      #0, RD2
  MOV.L    RD2, gameaction
  BRA      L00800D31

L00800D3B:
  BRA      L00800D31

L00800D3C:

L00800D31:
  BRA      L00800D2F

L00800D30:
  MOV.L    ticdup, RD5
  MOV.L    gametic, RD4
  BSR      __sdivsi3
  MOV      RD2, RD14
  MOV      RD14, RD25
  MOV      #0x15555556, RD7
  MOV      #5461, RD7
  DMULS    RD7, RD25
  STS      R1, RD6
  MOV      #12, RD7
  DMULU    RD7, RD6
  STS      R0, RD6
  SUB      RD6, RD25
  MOV      #0, RD13
  MOV.L    RD25, @(R15, 84)

L00800D3E:
  MOV      #4, RD23
  CMP/GE   RD23, RD13
  BT       L00800D40
  MOV      playeringame, RQ22
  EXTS.L   RD13
  MOV.L    @(RQ22, RD13), RD14
  CMP/EQ   RD14
  BT       L00800D41
  MOV      players, RQ25
  EXTS.L   RD13
  MOV      #320, RD7
  MUL.L    RD13, RD7
  STS      R0, RQ26
  ADD      RQ25, RQ26
  ADD      RQ26, #12, RQ27
  MOV      RQ27, RQ28
  MOV      netcmds, RQ29
  EXTS.L   RD13
  MOV      #96, RD7
  MUL.L    RD13, RD7
  STS      R0, RQ27
  ADD      RQ29, RQ27
  MOV.L    @(R15, 84), RD30
  EXTS.L   RD30
  LEA.Q    @(RQ27, RD30), RQ31
  MOV      #8, RQ8
  EXTS.L   RQ8, RQ8
  MOV.Q    RQ8, @(R15, 32)
  MOV.Q    RQ31, @(R15, 40)
  MOV.Q    RQ28, @(R15, 72)
  MOV.Q    RQ27, @(R15, 48)
  MOV.Q    RQ26, @(R15, 56)
  MOV      RQ28, RQ4
  MOV      RQ31, RQ5
  MOV      RQ8, RQ6
  BSR      memcpy
  MOV.L    demoplayback, RD21
  CMP/EQ   #0, RD21
  BT       L00800D42
  MOV.Q    @(R15, 72), RQ4
  BSR      G_ReadDemoTiccmd

L00800D42:
  MOV.L    demorecording, RD20
  CMP/EQ   #0, RD20
  BT       L00800D43
  MOV.Q    @(R15, 72), RQ4
  BSR      G_WriteDemoTiccmd

L00800D43:
  MOV.Q    @(R15, 72), RQ19
  MOV.B    @RQ19, RD14
  CMP/GT   #50, RD14
  BT       L00800D44
  MOV.L    gametic, RD18
  TST      #31, RD18
  BT       L00800D44
  BRA      L00800D46

L00800D46:
  MOV.L    gametic, RD17
  SHAD     RD17, #-5, RD14
  AND      RD14, #3, RD16
  MOV.L    RD16, @(R15, 28)
  CMP/EQ   RD13, RD16
  BT       L00800D44
  BRA      L00800D45

L00800D45:
  MOV      player_names, RQ25
  EXTS.L   RD13
  MOV.Q    @(RQ25, RD13), RQ26
  MOV.Q    RQ26, @(R15, 48)
  MOV      TU0ADE0179/turbomessage, RQ4
  MOV      L00C00663, RQ5
  MOV      RQ26, RQ6
  BSR      sprintf
  MOV      players, RQ27
  MOV.L    consoleplayer, RD28
  EXTS.L   RD28
  MOV      #320, RD7
  MUL.L    RD28, RD7
  STS      R0, RQ29
  ADD      RQ27, RQ29
  MOV      TU0ADE0179/turbomessage, RQ30
  MOV.Q    RQ30, @(RQ29, 224)
  MOV.Q    RQ29, @(R15, 56)

L00800D44:
  MOV.L    netgame, RD16
  CMP/EQ   #0, RD16
  BT       L00800D48
  MOV.L    netdemo, RD16
  CMP/EQ   #0, RD16
  BT       L00800D48
  BRA      L00800D4A

L00800D4A:
  MOV.L    ticdup, RD5
  MOV.L    gametic, RD4
  BSR      __smodsi3
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L00800D48
  BRA      L00800D49

L00800D49:
  MOV.L    gametic, RD16
  CMP/GT   #12, RD16
  BT       L00800D4C
  MOV      consistancy, RQ25
  EXTS.L   RD13
  MOV      #24, RD7
  MUL.L    RD13, RD7
  STS      R0, RQ26
  ADD      RQ25, RQ26
  MOV.L    @(R15, 84), RD27
  EXTS.L   RD27
  MOV.W    @(RQ26, RD27), RD14
  MOV.Q    @(R15, 72), RQ28
  MOV.W    @(RQ28, 4), RD29
  MOV      RD14, RD30
  EXTS.W   RD29, RD31
  MOV.L    RD31, @(R15, 20)
  MOV.L    RD30, @(R15, 24)
  MOV.L    RD29, @(R15, 28)
  MOV.Q    RQ26, @(R15, 48)
  CMP/EQ   RD31, RD30
  BT       L00800D4C
  BRA      L00800D4D

L00800D4D:
  MOV      consistancy, RQ25
  EXTS.L   RD13
  MOV      #24, RD7
  MUL.L    RD13, RD7
  STS      R0, RQ26
  ADD      RQ25, RQ26
  MOV.L    @(R15, 84), RD27
  EXTS.L   RD27
  MOV.W    @(RQ26, RD27), RD14
  MOV.Q    @(R15, 72), RQ28
  MOV.W    @(RQ28, 4), RD29
  MOV.L    RD29, @(R15, 28)
  MOV.Q    RQ26, @(R15, 48)
  MOV      L00C00665, RQ4
  MOV      RD29, RD5
  MOV      RD14, RD6
  BSR      I_Error

L00800D4C:
  MOV      players, RQ25
  EXTS.L   RD13
  MOV      #320, RD7
  MUL.L    RD13, RD7
  STS      R0, RQ26
  ADD      RQ25, RQ26
  MOV.Q    @RQ26, RQ27
  MOV.Q    RQ27, @(R15, 48)
  MOV.Q    RQ26, @(R15, 56)
  MOV      #0, RQ28
  CMPQ/EQ  RQ28, RQ27
  BT       L00800D4F
  MOV      players, RQ25
  EXTS.L   RD13
  MOV      #320, RD7
  MUL.L    RD13, RD7
  STS      R0, RQ26
  ADD      RQ25, RQ26
  MOV.Q    @RQ26, RQ27
  MOV.L    @(RQ27, 24), RD14
  MOV      consistancy, RQ28
  EXTS.L   RD13
  MOV      #24, RD7
  MUL.L    RD13, RD7
  STS      R0, RQ27
  ADD      RQ28, RQ27
  MOV.L    @(R15, 84), RD29
  EXTS.L   RD29
  MOV.W    RD14, @(RQ27, RD29)
  MOV.Q    RQ27, @(R15, 48)
  MOV.Q    RQ26, @(R15, 56)
  BRA      L00800D50

L00800D4F:
  MOV      consistancy, RQ25
  EXTS.L   RD13
  MOV      #24, RD7
  MUL.L    RD13, RD7
  STS      R0, RQ26
  ADD      RQ25, RQ26
  MOV.L    @(R15, 84), RD27
  MOV.L    rndindex, RD28
  EXTS.L   RD27
  MOV.W    RD28, @(RQ26, RD27)
  MOV.Q    RQ26, @(R15, 48)

L00800D50:

L00800D48:

L00800D41:

L00800D3F:
  ADD      #1, RD13
  BRA      L00800D3E

L00800D40:
  MOV      #0, RD13

L00800D51:
  MOV      #4, RD16
  CMP/GE   RD16, RD13
  BT       L00800D53
  MOV      playeringame, RQ16
  EXTS.L   RD13
  MOV.L    @(RQ16, RD13), RD14
  CMP/EQ   RD14
  BT       L00800D54
  MOV      players, RQ25
  EXTS.L   RD13
  MOV      #320, RD7
  MUL.L    RD13, RD7
  STS      R0, RQ26
  ADD      RQ25, RQ26
  ADD      RQ26, #12, RQ27
  MOV.B    @(RQ27, 7), RD14
  EXTU.B   RD14, RD14
  MOV.Q    RQ27, @(R15, 8)
  MOV.Q    RQ26, @(R15, 56)
  TST      #128, RD14
  BT       L00800D55
  MOV      players, RQ25
  EXTS.L   RD13
  MOV      #320, RD7
  MUL.L    RD13, RD7
  STS      R0, RQ26
  ADD      RQ25, RQ26
  ADD      RQ26, #12, RQ27
  MOV.B    @(RQ27, 7), RD14
  EXTU.B   RD14, RD14
  AND      RD14, #3, RD28
  MOV.L    RD28, @(R15, 28)
  MOV.Q    RQ27, @(R15, 8)
  MOV.Q    RQ26, @(R15, 56)
  CMP/EQ   #1, RD28
  BT       L00800D57
  MOV.L    @(R15, 28), RD16
  CMP/EQ   #2, RD16
  BT       L00800D58
  BRA      L00800D56

L00800D57:
  MOV.L    paused, RD16
  XOR      #1, RD16
  MOV.L    RD16, paused
  MOV.L    paused, RD17
  CMP/EQ   #0, RD17
  BT       L00800D59
  BSR      S_PauseSound
  BRA      L00800D5A

L00800D59:
  BSR      S_ResumeSound

L00800D5A:
  BRA      L00800D56

L00800D58:
  MOV      savedescription, RQ16
  MOV.B    @RQ16, RD14
  CMP/EQ   RD14
  BT       L00800D5B
  MOV      savedescription, RQ4
  MOV      L00C00666, RQ5
  BSR      strcpy

L00800D5B:
  MOV      players, RQ25
  EXTS.L   RD13
  MOV      #320, RD7
  MUL.L    RD13, RD7
  STS      R0, RQ26
  ADD      RQ25, RQ26
  ADD      RQ26, #12, RQ27
  MOV.B    @(RQ27, 7), RD14
  EXTU.B   RD14, RD14
  AND      RD14, #28, RD28
  SHAD     RD28, #-2, RD29
  MOV      #4, RD30
  MOV.L    RD30, gameaction
  MOV.L    RD29, savegameslot
  MOV.L    RD28, @(R15, 28)
  MOV.Q    RQ27, @(R15, 8)
  MOV.Q    RQ26, @(R15, 56)
  BRA      L00800D56

L00800D56:

L00800D55:

L00800D54:

L00800D52:
  ADD      #1, RD13
  BRA      L00800D51

L00800D53:
  MOV.L    gamestate, RD16
  CMP/EQ   #0, RD16
  BT       L00800D5D
  MOV.L    gamestate, RD16
  CMP/EQ   #1, RD16
  BT       L00800D5E
  MOV.L    gamestate, RD16
  CMP/EQ   #2, RD16
  BT       L00800D5F
  MOV.L    gamestate, RD16
  CMP/EQ   #3, RD16
  BT       L00800D60
  BRA      L00800D5C

L00800D5D:
  BSR      P_Ticker
  BSR      ST_Ticker
  BSR      AM_Ticker
  BSR      HU_Ticker
  BRA      L00800D5C

L00800D5E:
  BSR      WI_Ticker
  BRA      L00800D5C

L00800D5F:
  BSR      F_Ticker
  BRA      L00800D5C

L00800D60:
  BSR      D_PageTicker
  BRA      L00800D5C

L00800D5C:

L00E0057A:
  ADD      #552, R15
  BRA      L00C00626

G_Responder:
  PUSH     LR
  BSR      L00C00630
  ADD      #-56, R15
  MOV      RQ4, RQ13
  MOV.L    gamestate, RD11
  CMP/EQ   RD11
  BT       L00800D04
  MOV.L    @RQ13, RD14
  CMP/EQ   RD14
  BT       L00800D04
  BRA      L00800D07

L00800D07:
  MOV.L    @(RQ13, 4), RD14
  CMP/EQ   #216, RD14
  BT       L00800D04
  BRA      L00800D06

L00800D06:
  MOV.L    singledemo, RD10
  CMP/EQ   RD10
  BT       L00800D05
  MOV.L    deathmatch, RD9
  CMP/EQ   RD9
  BT       L00800D05
  BRA      L00800D04

L00800D05:

L00800D0A:
  MOV.L    displayplayer, RD8
  ADD      #1, RD8
  MOV.L    RD8, displayplayer
  MOV.L    displayplayer, RD31
  CMP/EQ   #4, RD31
  BT       L00800D0C
  MOV      #0, RD30
  MOV.L    RD30, displayplayer

L00800D0C:
  MOV      playeringame, RQ29
  MOV.L    displayplayer, RD28
  EXTS.L   RD28
  MOV.L    @(RQ29, RD28), RD14
  CMP/EQ   RD14
  BT       L00800D0D
  MOV.L    displayplayer, RD27
  MOV.L    consoleplayer, RD26
  CMP/EQ   RD26, RD27
  BT       L00800D0D
  BRA      L00800D0A

L00800D0D:

L00800D0B:
  MOV      #1, RD2
  BRA      L00E0057B

L00800D04:
  MOV.L    gameaction, RD25
  CMP/EQ   #0, RD25
  BT       L00800D0F
  MOV.L    singledemo, RD7
  CMP/EQ   RD7
  BT       L00800D0F
  BRA      L00800D11

L00800D11:
  MOV.L    demoplayback, RD6
  CMP/EQ   RD6
  BT       L00800D10
  MOV.L    gamestate, RD5
  CMP/EQ   #3, RD5
  BT       L00800D10
  BRA      L00800D0F

L00800D10:
  MOV.L    @RQ13, RD14
  CMP/EQ   RD14
  BT       L00800D15
  MOV.L    @RQ13, RD14
  CMP/EQ   #2, RD14
  BT       L00800D16
  MOV.L    @(RQ13, 4), RD14
  CMP/EQ   RD14
  BT       L00800D16
  BRA      L00800D15

L00800D16:
  MOV.L    @RQ13, RD14
  CMP/EQ   #3, RD14
  BT       L00800D14
  MOV.L    @(RQ13, 4), RD14
  CMP/EQ   RD14
  BT       L00800D14
  BRA      L00800D15

L00800D15:
  BSR      M_StartControlPanel
  MOV      #1, RD2
  BRA      L00E0057B

L00800D14:
  MOV      #0, RD2
  BRA      L00E0057B

L00800D0F:
  MOV.L    gamestate, RD4
  CMP/EQ   RD4
  BT       L00800D1A
  MOV      RQ13, RQ4
  BSR      HU_Responder
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L00800D1B
  MOV      #1, RD2
  BRA      L00E0057B

L00800D1B:
  MOV      RQ13, RQ4
  BSR      ST_Responder
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L00800D1C
  MOV      #1, RD2
  BRA      L00E0057B

L00800D1C:
  MOV      RQ13, RQ4
  BSR      AM_Responder
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L00800D1D
  MOV      #1, RD2
  BRA      L00E0057B

L00800D1D:

L00800D1A:
  MOV.L    gamestate, RD3
  CMP/EQ   #2, RD3
  BT       L00800D1E
  MOV      RQ13, RQ4
  BSR      F_Responder
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L00800D1F
  MOV      #1, RD2
  BRA      L00E0057B

L00800D1F:

L00800D1E:
  MOV.L    @RQ13, RD14
  CMP/EQ   RD14
  BT       L00800D21
  CMP/EQ   #1, RD14
  BT       L00800D22
  CMP/EQ   #2, RD14
  BT       L00800D23
  CMP/EQ   #3, RD14
  BT       L00800D24
  BRA      L00800D25

L00800D21:
  MOV.L    @(RQ13, 4), RD14
  CMP/EQ   #255, RD14
  BT       L00800D26
  MOV      #1, RD2
  MOV.L    RD2, sendpause
  MOV      #1, RD2
  BRA      L00E0057B

L00800D26:
  MOV.L    @(RQ13, 4), RD14
  CMP/GE   #256, RD14
  BT       L00800D27
  MOV.L    @(RQ13, 4), RD14
  MOV      gamekeydown, RQ23
  MOV      #1, RD22
  MOV.L    RD22, @(RQ23, RD14)

L00800D27:
  MOV      #1, RD2
  BRA      L00E0057B

L00800D22:
  MOV.L    @(RQ13, 4), RD14
  CMP/GE   #256, RD14
  BT       L00800D28
  MOV.L    @(RQ13, 4), RD14
  MOV      gamekeydown, RQ21
  MOV      #0, RD20
  MOV.L    RD20, @(RQ21, RD14)

L00800D28:
  MOV      #0, RD2
  BRA      L00E0057B

L00800D23:
  MOV.L    @(RQ13, 4), RD14
  AND      RD14, #1, RD12
  MOV.Q    mousebuttons, RQ25
  MOV.L    RD12, @RQ25
  MOV.L    @(RQ13, 4), RD14
  AND      RD14, #2, RD12
  MOV.L    RD12, @(RQ25, 4)
  MOV.L    @(RQ13, 4), RD14
  AND      RD14, #4, RD12
  MOV.L    RD12, @(RQ25, 8)
  MOV.L    @(RQ13, 8), RD14
  MOV.L    mouseSensitivity, RD26
  ADD      RD26, #5, RD12
  MUL.L    RD14, RD12, RD27
  MOV      RD27, RD28
  MOV      #0x1999999A, RD7
  MOV      #6553, RD7
  DMULS    RD7, RD28
  STS      R1, RD7
  MOV      RD7, RD28
  MOV.L    @(RQ13, 12), RD14
  ADD      RD26, #5, RD12
  MUL.L    RD14, RD12, RD27
  MOV      RD27, RD29
  MOV      #0x1999999A, RD7
  MOV      #6553, RD7
  DMULS    RD7, RD29
  STS      R1, RD7
  MOV      RD7, RD29
  MOV.L    RD29, mousey
  MOV.L    RD28, mousex
  MOV      #1, RD2
  BRA      L00E0057B

L00800D24:
  MOV.L    @(RQ13, 4), RD14
  AND      RD14, #1, RD12
  MOV.Q    joybuttons, RQ19
  MOV.L    RD12, @RQ19
  MOV.L    @(RQ13, 4), RD14
  AND      RD14, #2, RD12
  MOV.L    RD12, @(RQ19, 4)
  MOV.L    @(RQ13, 4), RD14
  AND      RD14, #4, RD12
  MOV.L    RD12, @(RQ19, 8)
  MOV.L    @(RQ13, 4), RD14
  AND      RD14, #8, RD12
  MOV.L    RD12, @(RQ19, 12)
  MOV.L    @(RQ13, 8), RD18
  MOV.L    @(RQ13, 12), RD17
  MOV.L    RD18, joyxmove
  MOV.L    RD17, joyymove
  MOV      #1, RD2
  BRA      L00E0057B

L00800D25:
  BRA      L00800D20

L00800D20:
  MOV      #0, RD2

L00E0057B:
  ADD      #56, R15
  BRA      L00C00626

G_ScreenShot:
  ADD      #-144, R15
  MOV      #9, RD7
  MOV.L    RD7, gameaction

L00E0057C:
  ADD      #144, R15
  RTS     

WI_Ticker:
  PUSH     LR
  BSR      L00C002C0
  ADD      #-64, R15
  MOV.L    TU0ADE093B/bcnt, RD14
  ADD      #1, RD14
  MOV.L    RD14, TU0ADE093B/bcnt
  MOV.L    TU0ADE093B/bcnt, RD13
  CMP/EQ   #1, RD13
  BT       L0080221B
  MOV.L    gamemode, RD12
  CMP/EQ   #2, RD12
  BT       L0080221D
  MOV.L    gamemode, RD11
  CMP/EQ   #5, RD11
  BT       L0080221D
  BRA      L0080221C

L0080221D:
  MOV      #67, RD4
  MOV      #1, RD5
  BSR      S_ChangeMusic
  BRA      L0080221F

L0080221C:
  MOV      #28, RD4
  MOV      #1, RD5
  BSR      S_ChangeMusic

L0080221F:

L0080221B:
  BSR      WI_checkForAccelerate
  MOV.L    TU0ADE093B/state, RD10
  CMP/EQ   RD10
  BT       L00802221
  MOV.L    TU0ADE093B/state, RD9
  CMP/EQ   #1, RD9
  BT       L00802222
  MOV.L    TU0ADE093B/state, RD8
  CMP/EQ   #-1, RD8
  BT       L00802223
  BRA      L00802220

L00802221:
  MOV.L    deathmatch, RD31
  CMP/EQ   #0, RD31
  BT       L00802224
  BSR      WI_updateDeathmatchStats
  BRA      L00802225

L00802224:
  MOV.L    netgame, RD30
  CMP/EQ   #0, RD30
  BT       L00802226
  BSR      WI_updateNetgameStats
  BRA      L00802227

L00802226:
  BSR      WI_updateStats

L00802227:

L00802225:
  BRA      L00802220

L00802222:
  BSR      WI_updateShowNextLoc
  BRA      L00802220

L00802223:
  BSR      WI_updateNoState
  BRA      L00802220

L00802220:

L00E0057D:
  ADD      #64, R15
  BRA      L00C002C3

WI_Drawer:
  PUSH     LR
  BSR      L00C003ED
  ADD      #-96, R15
  MOV.L    TU0ADE093B/state, RD14
  CMP/EQ   RD14
  BT       L008022A5
  MOV.L    TU0ADE093B/state, RD13
  CMP/EQ   #1, RD13
  BT       L008022A6
  MOV.L    TU0ADE093B/state, RD12
  CMP/EQ   #-1, RD12
  BT       L008022A7
  BRA      L008022A4

L008022A5:
  MOV.L    deathmatch, RD11
  CMP/EQ   RD11
  BT       L008022A8
  BSR      WI_drawDeathmatchStats
  BRA      L008022A9

L008022A8:
  MOV.L    netgame, RD10
  CMP/EQ   RD10
  BT       L008022AA
  BSR      WI_drawNetgameStats
  BRA      L008022AB

L008022AA:
  BSR      WI_drawStats

L008022AB:

L008022A9:
  BRA      L008022A4

L008022A6:
  BSR      WI_drawShowNextLoc
  BRA      L008022A4

L008022A7:
  BSR      WI_drawNoState
  BRA      L008022A4

L008022A4:

L00E0057E:
  ADD      #96, R15
  BRA      L00C003EF

WI_Start:
  PUSH     LR
  PUSH     R14
  PUSH     R13
  ADD      #-136, R15
  MOV.Q    RQ4, @(R15, 8)
  MOV.Q    @(R15, 8), RQ4
  BSR      WI_initVariables
  BSR      WI_loadData
  MOV.L    deathmatch, RD14
  CMP/EQ   RD14
  BT       L008022B1
  BSR      WI_initDeathmatchStats
  BRA      L008022B2

L008022B1:
  MOV.L    netgame, RD13
  CMP/EQ   RD13
  BT       L008022B3
  BSR      WI_initNetgameStats
  BRA      L008022B4

L008022B3:
  BSR      WI_initStats

L008022B4:

L008022B2:

L00E0057F:
  ADD      #136, R15
  POP      RQ13
  POP      RQ14
  RET     

ST_Responder:
  PUSH     LR
  BSR      L00C00630
  ADD      #-456, R15
  MOV.Q    RQ4, @(R15, 96)
  MOV      #108, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 88)
  MOV.Q    @(R15, 96), RQ8
  MOV.L    @RQ8, RD11
  CMP/EQ   #1, RD11
  BT       L00802345
  MOV.Q    @(R15, 96), RQ31
  MOV.L    @(RQ31, 4), RD11
  AND      RD11, #-65536, RD10
  CMP/EQ   #1634533376, RD10
  BT       L00802345
  BRA      L00802346

L00802346:
  MOV.Q    @(R15, 96), RQ30
  MOV.L    @(RQ30, 4), RD11
  CMP/EQ   #1634559232, RD11
  BT       L00802349
  CMP/EQ   #1634564096, RD11
  BT       L0080234A
  BRA      L00802348

L00802349:
  MOV      #0, RD29
  MOV      #1, RD28
  MOV.L    RD29, TU0ADE0A99/st_gamestate
  MOV.L    RD28, TU0ADE0A99/st_firsttime
  BRA      L00802348

L0080234A:
  MOV      #1, RD27
  MOV.L    RD27, TU0ADE0A99/st_gamestate
  BRA      L00802348

L00802348:
  BRA      L0080234B

L00802345:
  MOV.Q    @(R15, 96), RQ26
  MOV.L    @RQ26, RD11
  CMP/EQ   RD11
  BT       L0080234C
  MOV.L    netgame, RD25
  CMP/EQ   #0, RD25
  BT       L0080234D
  MOV.Q    @(R15, 96), RQ25
  MOV.L    @(RQ25, 4), RD11
  MOV      cheat_god, RQ26
  MOV      RQ26, RQ14
  EXTS.B   RD11, RD9
  MOV      RQ14, RQ4
  MOV      RD9, RD5
  BSR      cht_CheckCheat
  MOV      RD2, RD10
  CMP/EQ   RD10
  BT       L0080234E
  MOV.Q    TU0ADE0A99/plyr, RQ7
  MOV.L    @(RQ7, 200), RD10
  XOR      RD10, #2, RD9
  MOV.L    RD9, @(RQ7, 200)
  MOV.L    @(RQ7, 200), RD10
  TST      #2, RD10
  BT       L0080234F
  MOV.Q    TU0ADE0A99/plyr, RQ6
  MOV.Q    @RQ6, RQ14
  MOV      #0, RQ5
  CMPQ/EQ  RQ5, RQ14
  BT       L00802350
  MOV.Q    TU0ADE0A99/plyr, RQ4
  MOV.Q    @RQ4, RQ14
  MOV      #100, RD3
  MOV.L    RD3, @(RQ14, 164)

L00802350:
  MOV      #100, RD2
  MOV.Q    TU0ADE0A99/plyr, RQ23
  MOV.L    RD2, @(RQ23, 36)
  MOV      L00C0068E, RQ22
  MOV.Q    RQ22, @(RQ23, 224)
  BRA      L00802351

L0080234F:
  MOV      L00C0068F, RQ21
  MOV.Q    TU0ADE0A99/plyr, RQ20
  MOV.Q    RQ21, @(RQ20, 224)

L00802351:
  BRA      L00802352

L0080234E:
  MOV.Q    @(R15, 96), RQ25
  MOV.L    @(RQ25, 4), RD10
  MOV      cheat_ammonokey, RQ26
  MOV      RQ26, RQ14
  EXTS.B   RD10, RD27
  MOV.L    RD27, @(R15, 48)
  MOV      RQ14, RQ4
  MOV      RD27, RD5
  BSR      cht_CheckCheat
  MOV      RD2, RD9
  CMP/EQ   RD9
  BT       L00802353
  MOV      #200, RD19
  MOV.Q    TU0ADE0A99/plyr, RQ18
  MOV.L    RD19, @(RQ18, 40)
  MOV      #2, RD17
  MOV.L    RD17, @(RQ18, 44)
  MOV      #0, RD13

L00802354:
  MOV      #9, RD16
  CMP/GE   RD16, RD13
  BT       L00802356
  MOV.Q    TU0ADE0A99/plyr, RQ16
  ADD      RQ16, #124, RQ14
  MOV      #1, RD17
  EXTS.L   RD13
  MOV.L    RD17, @(RQ14, RD13)

L00802355:
  ADD      #1, RD13
  BRA      L00802354

L00802356:
  MOV      #0, RD13

L00802357:
  MOV      #4, RD16
  CMP/GE   RD16, RD13
  BT       L00802359
  MOV.Q    TU0ADE0A99/plyr, RQ16
  ADD      RQ16, #176, RQ14
  EXTS.L   RD13
  MOV.L    @(RQ14, RD13), RD9
  ADD      RQ16, #160, RQ14
  EXTS.L   RD13
  MOV.L    RD9, @(RQ14, RD13)

L00802358:
  ADD      #1, RD13
  BRA      L00802357

L00802359:
  MOV      L00C00691, RQ16
  MOV.Q    TU0ADE0A99/plyr, RQ17
  MOV.Q    RQ16, @(RQ17, 224)
  BRA      L0080235A

L00802353:
  MOV.Q    @(R15, 96), RQ25
  MOV.L    @(RQ25, 4), RD9
  MOV      cheat_ammo, RQ26
  MOV      RQ26, RQ14
  EXTS.B   RD9, RD12
  MOV      RQ14, RQ4
  MOV      RD12, RD5
  BSR      cht_CheckCheat
  MOV      RD2, RD27
  MOV.L    RD27, @(R15, 48)
  CMP/EQ   #0, RD27
  BT       L0080235B
  MOV      #200, RD16
  MOV.Q    TU0ADE0A99/plyr, RQ17
  MOV.L    RD16, @(RQ17, 40)
  MOV      #2, RD18
  MOV.L    RD18, @(RQ17, 44)
  MOV      #0, RD13

L0080235C:
  MOV      #9, RD16
  CMP/GE   RD16, RD13
  BT       L0080235E
  MOV.Q    TU0ADE0A99/plyr, RQ16
  ADD      RQ16, #124, RQ14
  MOV      #1, RD17
  EXTS.L   RD13
  MOV.L    RD17, @(RQ14, RD13)

L0080235D:
  ADD      #1, RD13
  BRA      L0080235C

L0080235E:
  MOV      #0, RD13

L0080235F:
  MOV      #4, RD16
  CMP/GE   RD16, RD13
  BT       L00802361
  MOV.Q    TU0ADE0A99/plyr, RQ16
  ADD      RQ16, #176, RQ14
  EXTS.L   RD13
  MOV.L    @(RQ14, RD13), RD17
  ADD      RQ16, #160, RQ14
  EXTS.L   RD13
  MOV.L    RD17, @(RQ14, RD13)
  MOV.L    RD17, @(R15, 48)

L00802360:
  ADD      #1, RD13
  BRA      L0080235F

L00802361:
  MOV      #0, RD13

L00802362:
  MOV      #6, RD16
  CMP/GE   RD16, RD13
  BT       L00802364
  MOV.Q    TU0ADE0A99/plyr, RQ16
  ADD      RQ16, #72, RQ14
  MOV      #1, RD17
  EXTS.L   RD13
  MOV.L    RD17, @(RQ14, RD13)

L00802363:
  ADD      #1, RD13
  BRA      L00802362

L00802364:
  MOV      L00C00693, RQ16
  MOV.Q    TU0ADE0A99/plyr, RQ17
  MOV.Q    RQ16, @(RQ17, 224)
  BRA      L00802365

L0080235B:
  MOV.Q    @(R15, 96), RQ25
  MOV.L    @(RQ25, 4), RD26
  MOV      cheat_mus, RQ27
  MOV      RQ27, RQ14
  EXTS.B   RD26, RD28
  MOV.L    RD28, @(R15, 40)
  MOV.L    RD26, @(R15, 48)
  MOV      RQ14, RQ4
  MOV      RD28, RD5
  BSR      cht_CheckCheat
  MOV      RD2, RD12
  CMP/EQ   RD12
  BT       L00802366
  MOV      L00C00695, RQ25
  MOV.Q    TU0ADE0A99/plyr, RQ26
  MOV.Q    RQ25, @(RQ26, 224)
  MOV      cheat_mus, RQ27
  MOV      RQ27, RQ14
  MOV      RQ14, RQ4
  MOV      #108, RQ5
  ADD      R15, RQ5
  BSR      cht_GetParam
  MOV.L    gamemode, RD28
  CMP/EQ   #2, RD28
  BT       L00802367
  MOV      #108, RQ16
  ADD      R15, RQ16
  MOV.B    @RQ16, RD12
  SUB      RD12, #48, RD17
  MOV      RD17, RD12
  MOV      #10, RD18
  MUL.L    RD18, RD12, RD12
  ADD      RD12, #33, RD17
  MOV.B    @(RQ16, 1), RD12
  ADD      RD17, RD12, RD19
  SUB      RD19, #48, RD12
  SUB      RD12, #1, RD20
  MOV.B    @RQ16, RD12
  SUB      RD12, #48, RD17
  MOV      RD17, RD12
  MUL.L    RD18, RD12, RD12
  MOV.B    @(RQ16, 1), RD17
  ADD      RD12, RD17, RD19
  SUB      RD19, #48, RD12
  MOV.L    RD20, @(R15, 84)
  MOV.L    RD19, @(R15, 36)
  MOV.L    RD17, @(R15, 40)
  CMP/GT   #35, RD12
  BT       L00802368
  MOV      L00C00697, RQ16
  MOV.Q    TU0ADE0A99/plyr, RQ17
  MOV.Q    RQ16, @(RQ17, 224)
  BRA      L00802369

L00802368:
  MOV.L    @(R15, 84), RD4
  MOV      #1, RD5
  BSR      S_ChangeMusic

L00802369:
  BRA      L0080236A

L00802367:
  MOV      #108, RQ16
  ADD      R15, RQ16
  MOV.B    @RQ16, RD12
  SUB      RD12, #49, RD17
  MOV      RD17, RD12
  MOV      #9, RD18
  MUL.L    RD18, RD12, RD12
  ADD      RD12, #1, RD17
  MOV.B    @(RQ16, 1), RD12
  SUB      RD12, #49, RD19
  ADD      RD17, RD19, RD20
  MOV.B    @RQ16, RD12
  SUB      RD12, #49, RD17
  MOV      RD17, RD12
  MUL.L    RD18, RD12, RD12
  MOV.B    @(RQ16, 1), RD17
  ADD      RD12, RD17, RD19
  SUB      RD19, #49, RD12
  MOV.L    RD20, @(R15, 84)
  MOV.L    RD19, @(R15, 36)
  MOV.L    RD17, @(R15, 40)
  CMP/GT   #31, RD12
  BT       L0080236B
  MOV      L00C00697, RQ16
  MOV.Q    TU0ADE0A99/plyr, RQ17
  MOV.Q    RQ16, @(RQ17, 224)
  BRA      L0080236C

L0080236B:
  MOV.L    @(R15, 84), RD4
  MOV      #1, RD5
  BSR      S_ChangeMusic

L0080236C:

L0080236A:
  BRA      L0080236D

L00802366:
  MOV.Q    @(R15, 96), RQ25
  MOV.L    @(RQ25, 4), RD12
  MOV      cheat_noclip, RQ26
  MOV      RQ26, RQ14
  EXTS.B   RD12, RD27
  MOV.L    RD27, @(R15, 36)
  MOV      RQ14, RQ4
  MOV      RD27, RD5
  BSR      cht_CheckCheat
  MOV      RD2, RD28
  MOV.L    RD28, @(R15, 40)
  CMP/EQ   #0, RD28
  BT       L0080236F
  MOV.Q    @(R15, 96), RQ25
  MOV.L    @(RQ25, 4), RD26
  MOV      cheat_commercial_noclip, RQ27
  MOV      RQ27, RQ14
  EXTS.B   RD26, RD28
  MOV.L    RD28, @(R15, 32)
  MOV.L    RD26, @(R15, 40)
  MOV      RQ14, RQ4
  MOV      RD28, RD5
  BSR      cht_CheckCheat
  MOV      RD2, RD29
  MOV.L    RD29, @(R15, 36)
  CMP/EQ   #0, RD29
  BT       L0080236F
  BRA      L0080236E

L0080236F:
  MOV.Q    TU0ADE0A99/plyr, RQ16
  MOV.L    @(RQ16, 200), RD17
  XOR      RD17, #1, RD18
  MOV.L    RD18, @(RQ16, 200)
  MOV.L    @(RQ16, 200), RD17
  MOV.L    RD18, @(R15, 32)
  MOV.L    RD17, @(R15, 36)
  TST      #1, RD17
  BT       L00802371
  MOV      L00C0069A, RQ16
  MOV.Q    TU0ADE0A99/plyr, RQ17
  MOV.Q    RQ16, @(RQ17, 224)
  BRA      L00802372

L00802371:
  MOV      L00C0069B, RQ16
  MOV.Q    TU0ADE0A99/plyr, RQ17
  MOV.Q    RQ16, @(RQ17, 224)

L00802372:

L0080236E:

L0080236D:

L00802365:

L0080235A:

L00802352:
  MOV      #0, RD13

L00802373:
  MOV      #6, RD16
  CMP/GE   RD16, RD13
  BT       L00802375
  MOV.Q    @(R15, 96), RQ25
  MOV.L    @(RQ25, 4), RD26
  MOV      cheat_powerup, RQ27
  EXTS.L   RD13
  SHAD     RD13, #4, RQ14
  ADD      RQ27, RQ14
  EXTS.B   RD26, RD28
  MOV.L    RD28, @(R15, 28)
  MOV.L    RD26, @(R15, 36)
  MOV      RQ14, RQ4
  MOV      RD28, RD5
  BSR      cht_CheckCheat
  MOV      RD2, RD29
  MOV.L    RD29, @(R15, 32)
  CMP/EQ   #0, RD29
  BT       L00802376
  MOV.Q    TU0ADE0A99/plyr, RQ16
  ADD      RQ16, #48, RQ14
  EXTS.L   RD13
  MOV.L    @(RQ14, RD13), RD17
  MOV.L    RD17, @(R15, 32)
  CMP/EQ   #0, RD17
  BT       L00802377
  MOV.Q    TU0ADE0A99/plyr, RQ4
  MOV      RD13, RD5
  BSR      P_GivePower
  BRA      L00802378

L00802377:
  CMP/EQ   #1, RD13
  BT       L00802379
  MOV.Q    TU0ADE0A99/plyr, RQ16
  ADD      RQ16, #48, RQ14
  MOV      #1, RD17
  EXTS.L   RD13
  MOV.L    RD17, @(RQ14, RD13)
  BRA      L0080237A

L00802379:
  MOV.Q    TU0ADE0A99/plyr, RQ16
  ADD      RQ16, #48, RQ14
  MOV      #0, RD17
  EXTS.L   RD13
  MOV.L    RD17, @(RQ14, RD13)

L0080237A:

L00802378:
  MOV      L00C0069E, RQ16
  MOV.Q    TU0ADE0A99/plyr, RQ17
  MOV.Q    RQ16, @(RQ17, 224)

L00802376:

L00802374:
  ADD      #1, RD13
  BRA      L00802373

L00802375:
  MOV.Q    @(R15, 96), RQ25
  MOV.L    @(RQ25, 4), RD26
  MOV      cheat_powerup, RQ27
  MOV      #6, RD28
  EXTS.L   RD28
  SHAD     RD28, #4, RQ14
  ADD      RQ27, RQ14
  EXTS.B   RD26, RD29
  MOV.L    RD29, @(R15, 24)
  MOV.L    RD26, @(R15, 32)
  MOV      RQ14, RQ4
  MOV      RD29, RD5
  BSR      cht_CheckCheat
  MOV      RD2, RD30
  MOV.L    RD30, @(R15, 28)
  CMP/EQ   #0, RD30
  BT       L0080237B
  MOV      L00C0069F, RQ16
  MOV.Q    TU0ADE0A99/plyr, RQ17
  MOV.Q    RQ16, @(RQ17, 224)
  BRA      L0080237C

L0080237B:
  MOV.Q    @(R15, 96), RQ25
  MOV.L    @(RQ25, 4), RD26
  MOV      cheat_choppers, RQ27
  MOV      RQ27, RQ14
  EXTS.B   RD26, RD28
  MOV.L    RD28, @(R15, 20)
  MOV.L    RD26, @(R15, 28)
  MOV      RQ14, RQ4
  MOV      RD28, RD5
  BSR      cht_CheckCheat
  MOV      RD2, RD29
  MOV.L    RD29, @(R15, 24)
  CMP/EQ   #0, RD29
  BT       L0080237D
  MOV.Q    TU0ADE0A99/plyr, RQ16
  ADD      RQ16, #124, RQ14
  MOV      #1, RD17
  MOV.L    RD17, @(RQ14, 28)
  ADD      RQ16, #48, RQ14
  MOV.L    RD17, @RQ14
  MOV      L00C006A1, RQ18
  MOV.Q    RQ18, @(RQ16, 224)
  BRA      L0080237E

L0080237D:
  MOV.Q    @(R15, 96), RQ25
  MOV.L    @(RQ25, 4), RD26
  MOV      cheat_mypos, RQ27
  MOV      RQ27, RQ14
  EXTS.B   RD26, RD28
  MOV.L    RD28, @(R15, 16)
  MOV.L    RD26, @(R15, 24)
  MOV      RQ14, RQ4
  MOV      RD28, RD5
  BSR      cht_CheckCheat
  MOV      RD2, RD29
  MOV.L    RD29, @(R15, 20)
  CMP/EQ   #0, RD29
  BT       L0080237F
  MOV      players, RQ25
  MOV.L    consoleplayer, RD26
  EXTS.L   RD26
  MOV      #320, RD7
  MUL.L    RD26, RD7
  STS      R0, RQ27
  ADD      RQ25, RQ27
  MOV.Q    @RQ27, RQ14
  MOV.L    @(RQ14, 28), RD28
  EXTS.L   RD26
  MOV      #320, RD7
  MUL.L    RD26, RD7
  STS      R0, RQ27
  ADD      RQ25, RQ27
  MOV.Q    @RQ27, RQ14
  MOV.L    @(RQ14, 24), RD29
  EXTS.L   RD26
  MOV      #320, RD7
  MUL.L    RD26, RD7
  STS      R0, RQ27
  ADD      RQ25, RQ27
  MOV.Q    @RQ27, RQ14
  MOV.L    @(RQ14, 56), RD30
  EXTU.L   RD30, RD30
  MOV.L    RD30, @(R15, 4)
  MOV.L    RD29, @(R15, 16)
  MOV.L    RD28, @(R15, 20)
  MOV      #108, RQ4
  ADD      R15, RQ4
  MOV      L00C006A3, RQ5
  MOV      RD30, RD6
  MOV      RD29, RD7
  MOV      RD28, RD20
  BSR      sprintf
  MOV      #108, RQ31
  ADD      R15, RQ31
  MOV.Q    TU0ADE0A99/plyr, RQ8
  MOV.Q    RQ31, @(RQ8, 224)

L0080237F:

L0080237E:

L0080237C:

L0080234D:
  MOV.Q    @(R15, 96), RQ25
  MOV.L    @(RQ25, 4), RD26
  MOV      cheat_clev, RQ27
  MOV      RQ27, RQ14
  EXTS.B   RD26, RD28
  MOV.L    RD28, @(R15, 4)
  MOV.L    RD26, @(R15, 20)
  MOV      RQ14, RQ4
  MOV      RD28, RD5
  BSR      cht_CheckCheat
  MOV      RD2, RD29
  MOV.L    RD29, @(R15, 16)
  CMP/EQ   #0, RD29
  BT       L00802380
  MOV      cheat_clev, RQ25
  MOV      RQ25, RQ14
  MOV      RQ14, RQ4
  MOV      #108, RQ5
  ADD      R15, RQ5
  BSR      cht_GetParam
  MOV.L    gamemode, RD26
  CMP/EQ   #2, RD26
  BT       L00802381
  MOV      #0, RD16
  MOV      #108, RQ17
  ADD      R15, RQ17
  MOV.B    @RQ17, RD18
  SUB      RD18, #48, RD19
  MOV      RD19, RD18
  MOV      #10, RD20
  MUL.L    RD20, RD18, RD18
  MOV.B    @(RQ17, 1), RD19
  ADD      RD18, RD19, RD21
  SUB      RD21, #48, RD22
  MOV.L    RD22, @(R15, 76)
  MOV.L    RD19, @(R15, 4)
  MOV.L    RD18, @(R15, 16)
  MOV.L    RD16, @(R15, 80)
  BRA      L00802382

L00802381:
  MOV      #108, RQ16
  ADD      R15, RQ16
  MOV.B    @RQ16, RD17
  SUB      RD17, #48, RD18
  MOV.B    @(RQ16, 1), RD17
  SUB      RD17, #48, RD19
  MOV.L    RD19, @(R15, 76)
  MOV.L    RD18, @(R15, 80)
  MOV.L    RD17, @(R15, 16)

L00802382:
  MOV.L    @(R15, 80), RD16
  MOV      #1, RD17
  CMP/GE   RD17, RD16
  BT       L00802383
  MOV      #0, RD2
  BRA      L00E00580

L00802383:
  MOV.L    @(R15, 76), RD16
  MOV      #1, RD17
  CMP/GE   RD17, RD16
  BT       L00802384
  MOV      #0, RD2
  BRA      L00E00580

L00802384:
  MOV.L    gamemode, RD16
  CMP/EQ   #3, RD16
  BT       L00802385
  MOV.L    @(R15, 80), RD16
  CMP/GT   #4, RD16
  BT       L00802386
  MOV.L    @(R15, 76), RD16
  CMP/GT   #9, RD16
  BT       L00802386
  BRA      L00802385

L00802386:
  MOV      #0, RD2
  BRA      L00E00580

L00802385:
  MOV.L    gamemode, RD16
  CMP/EQ   #1, RD16
  BT       L00802389
  MOV.L    @(R15, 80), RD16
  CMP/GT   #3, RD16
  BT       L0080238A
  MOV.L    @(R15, 76), RD16
  CMP/GT   #9, RD16
  BT       L0080238A
  BRA      L00802389

L0080238A:
  MOV      #0, RD2
  BRA      L00E00580

L00802389:
  MOV.L    gamemode, RD16
  CMP/EQ   #0, RD16
  BT       L0080238D
  MOV.L    @(R15, 80), RD16
  CMP/GT   #1, RD16
  BT       L0080238E
  MOV.L    @(R15, 76), RD16
  CMP/GT   #9, RD16
  BT       L0080238E
  BRA      L0080238D

L0080238E:
  MOV      #0, RD2
  BRA      L00E00580

L0080238D:
  MOV.L    gamemode, RD16
  CMP/EQ   #2, RD16
  BT       L00802391
  MOV.L    @(R15, 80), RD16
  CMP/GT   #1, RD16
  BT       L00802392
  MOV.L    @(R15, 76), RD16
  CMP/GT   #34, RD16
  BT       L00802392
  BRA      L00802391

L00802392:
  MOV      #0, RD2
  BRA      L00E00580

L00802391:
  MOV      L00C006A5, RQ25
  MOV.Q    TU0ADE0A99/plyr, RQ26
  MOV.Q    RQ25, @(RQ26, 224)
  MOV.L    gameskill, RD4
  MOV.L    @(R15, 80), RD5
  MOV.L    @(R15, 76), RD6
  BSR      G_DeferedInitNew

L00802380:

L0080234C:

L0080234B:
  MOV      #0, RD2

L00E00580:
  ADD      #456, R15
  BRA      L00C00626

L00C006A6:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  RTS     

ST_Ticker:
  PUSH     LR
  BSR      L00C006A6
  ADD      #-104, R15
  MOV.Q    TU0ADE0A99/plyr, RQ14
  MOV      #0, RQ13
  CMPQ/EQ  RQ13, RQ14
  BT       L008023D3
  BRA      L00E00581

L008023D3:
  MOV.L    TU0ADE0A99/st_clock, RD12
  ADD      #1, RD12
  MOV.L    RD12, TU0ADE0A99/st_clock
  BSR      M_Random
  MOV      R2, RD11
  MOV.L    RD11, TU0ADE0A99/st_randomnumber
  BSR      ST_updateWidgets
  MOV.Q    TU0ADE0A99/plyr, RQ7
  MOV.L    @(RQ7, 36), RD6
  MOV.L    RD6, TU0ADE0A99/st_oldhealth

L00E00581:
  ADD      #104, R15

L00C006AB:
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  RET     

ST_Drawer:
  PUSH     LR
  BSR      L00C003FF
  ADD      #-88, R15
  MOV.L    RD4, @(R15, 12)
  MOV.L    RD5, @(R15, 8)
  MOV.L    @(R15, 12), RD13
  CMP/EQ   RD13
  BT       L008023F3
  MOV.L    automapactive, RD12
  CMP/EQ   RD12
  BT       L008023F3
  MOV      #0, RD14
  BRA      L008023F4

L008023F3:
  MOV      #1, RD14

L008023F4:
  MOV      RD14, RD11
  MOV.L    RD11, TU0ADE0A99/st_statusbaron
  MOV.L    TU0ADE0A99/st_firsttime, RD10
  CMP/EQ   RD10
  BT       L008023F7
  MOV.L    @(R15, 8), RD9
  CMP/EQ   RD9
  BT       L008023F7
  MOV      #0, RD14
  BRA      L008023F8

L008023F7:
  MOV      #1, RD14

L008023F8:
  MOV      RD14, RD8
  MOV.L    RD8, TU0ADE0A99/st_firsttime
  BSR      ST_doPaletteStuff
  MOV.L    TU0ADE0A99/st_firsttime, RD31
  CMP/EQ   #0, RD31
  BT       L008023FB
  BSR      ST_doRefresh
  BRA      L008023FC

L008023FB:
  BSR      ST_diffDraw

L008023FC:

L00E00582:
  ADD      #88, R15
  BRA      L00C00404

ST_Start:
  PUSH     LR
  PUSH     R14
  ADD      #-128, R15
  MOV.L    TU0ADE0A99/st_stopped, RD14
  CMP/EQ   RD14
  BT       L00802439
  BSR      ST_Stop

L00802439:
  BSR      ST_initData
  BSR      ST_createWidgets
  MOV      #0, RD14
  MOV.L    RD14, TU0ADE0A99/st_stopped

L00E00583:
  ADD      #128, R15
  POP      RQ14
  RET     

ST_Init:
  PUSH     LR
  BSR      L00C006A6
  ADD      #-120, R15
  MOV      #0, RD13
  MOV.L    RD13, TU0ADE0A99/veryfirsttime
  BSR      ST_loadData
  MOV      #0, RQ12
  MOV.Q    RQ12, @R15
  MOV      #20480, RD4
  MOV      #1, RD5
  MOV      RQ12, RQ6
  BSR      Z_Malloc
  MOV      RQ2, RQ14
  MOV      screens, RQ11
  MOV.Q    RQ14, @(RQ11, 32)

L00E00584:
  ADD      #120, R15
  BRA      L00C006AB

AM_Responder:
  PUSH     LR
  BSR      L00C00630
  ADD      #-72, R15
  MOV.Q    RQ4, @(R15, 40)
  MOV      #0, RD11
  MOV.L    automapactive, RD10
  CMP/EQ   RD10
  BT       L00800FA9
  MOV.Q    @(R15, 40), RQ9
  MOV.L    @RQ9, RD14
  CMP/EQ   RD14
  BT       L00800FAA
  MOV.Q    @(R15, 40), RQ8
  MOV.L    @(RQ8, 4), RD14
  CMP/EQ   #9, RD14
  BT       L00800FAA
  BRA      L00800FAB

L00800FAB:
  BSR      AM_Start
  MOV      #0, RD31
  MOV      #1, RD11
  MOV.L    RD31, viewactive

L00800FAA:
  BRA      L00800FAD

L00800FA9:
  MOV.Q    @(R15, 40), RQ30
  MOV.L    @RQ30, RD14
  CMP/EQ   RD14
  BT       L00800FAE
  MOV      #1, RD11
  MOV.Q    @(R15, 40), RQ29
  MOV.L    @(RQ29, 4), RD14
  CMP/GE   #103, RD14
  BT       L00800FBD
  CMP/EQ   #9, RD14
  BT       L00800FB6
  CMP/EQ   #45, RD14
  BT       L00800FB4
  CMP/EQ   #48, RD14
  BT       L00800FB7
  CMP/EQ   #61, RD14
  BT       L00800FB5
  CMP/EQ   #99, RD14
  BT       L00800FBB
  CMP/EQ   #102, RD14
  BT       L00800FB8

L00800FBD:
  CMP/EQ   #103, RD14
  BT       L00800FB9
  CMP/EQ   #109, RD14
  BT       L00800FBA
  CMP/EQ   #172, RD14
  BT       L00800FB1
  CMP/EQ   #173, RD14
  BT       L00800FB2
  CMP/EQ   #174, RD14
  BT       L00800FB0
  CMP/EQ   #175, RD14
  BT       L00800FB3
  BRA      L00800FBC

L00800FB0:
  MOV.L    TU0ADE02B5/followplayer, RD28
  CMP/EQ   #0, RD28
  BT       L00800FBE
  MOV      #0x00040000, RD4
  MOV      #4, RD4
  MOV.L    TU0ADE02B5/scale_ftom, RD5
  BSR      FixedMul
  MOV      RD2, RD12
  MOV      TU0ADE02B5/m_paninc, RQ27
  MOV.L    RD12, @RQ27
  BRA      L00800FBF

L00800FBE:
  MOV      #0, RD11

L00800FBF:
  BRA      L00800FAF

L00800FB1:
  MOV.L    TU0ADE02B5/followplayer, RD26
  CMP/EQ   #0, RD26
  BT       L00800FC0
  MOV      #0x00040000, RD4
  MOV      #4, RD4
  MOV.L    TU0ADE02B5/scale_ftom, RD5
  BSR      FixedMul
  MOV      RD2, RD12
  NEG      RD12, RD13
  MOV      TU0ADE02B5/m_paninc, RQ25
  MOV.L    RD13, @RQ25
  BRA      L00800FC1

L00800FC0:
  MOV      #0, RD11

L00800FC1:
  BRA      L00800FAF

L00800FB2:
  MOV.L    TU0ADE02B5/followplayer, RD7
  CMP/EQ   RD7
  BT       L00800FC2
  MOV      #0x00040000, RD4
  MOV      #4, RD4
  MOV.L    TU0ADE02B5/scale_ftom, RD5
  BSR      FixedMul
  MOV      RD2, RD12
  MOV      TU0ADE02B5/m_paninc, RQ25
  MOV.L    RD12, @(RQ25, 4)
  BRA      L00800FC3

L00800FC2:
  MOV      #0, RD11

L00800FC3:
  BRA      L00800FAF

L00800FB3:
  MOV.L    TU0ADE02B5/followplayer, RD6
  CMP/EQ   RD6
  BT       L00800FC4
  MOV      #0x00040000, RD4
  MOV      #4, RD4
  MOV.L    TU0ADE02B5/scale_ftom, RD5
  BSR      FixedMul
  MOV      RD2, RD12
  NEG      RD12, RD13
  MOV      TU0ADE02B5/m_paninc, RQ25
  MOV.L    RD13, @(RQ25, 4)
  BRA      L00800FC5

L00800FC4:
  MOV      #0, RD11

L00800FC5:
  BRA      L00800FAF

L00800FB4:
  MOV      #0x0000FAFA, RD5
  MOV      #0x0001051E, RD4
  MOV      #1, RD4
  MOV.L    RD5, TU0ADE02B5/mtof_zoommul
  MOV.L    RD4, TU0ADE02B5/ftom_zoommul
  BRA      L00800FAF

L00800FB5:
  MOV      #0x0001051E, RD3
  MOV      #1, RD3
  MOV      #0x0000FAFA, RD2
  MOV.L    RD3, TU0ADE02B5/mtof_zoommul
  MOV.L    RD2, TU0ADE02B5/ftom_zoommul
  BRA      L00800FAF

L00800FB6:
  MOV      #0, RD25
  MOV      #1, RD26
  MOV.L    RD26, viewactive
  MOV.L    RD25, bigstate
  BSR      AM_Stop
  BRA      L00800FAF

L00800FB7:
  MOV.L    bigstate, RD25
  TST      RD25, RD25
  MOVT     RQ7
  MOV      RQ7, RD25
  MOV.L    RD25, bigstate
  MOV.L    bigstate, RD26
  CMP/EQ   #0, RD26
  BT       L00800FC6
  BSR      AM_saveScaleAndLoc
  BSR      AM_minOutWindowScale
  BRA      L00800FC7

L00800FC6:
  BSR      AM_restoreScaleAndLoc

L00800FC7:
  BRA      L00800FAF

L00800FB8:
  MOV.L    TU0ADE02B5/followplayer, RD25
  TST      RD25, RD25
  MOVT     RQ7
  MOV      RQ7, RD25
  MOV      #0x7FFFFFFF, RD26
  MOV      #32767, RD26
  MOV      TU0ADE02B5/f_oldloc, RQ27
  MOV.L    RD26, @RQ27
  MOV.L    RD25, TU0ADE02B5/followplayer
  MOV.L    TU0ADE02B5/followplayer, RD28
  CMP/EQ   #0, RD28
  BT       L00800FC8
  MOV      L00C006C5, RQ22
  MOV      RQ22, RQ23
  MOV.Q    RQ23, @(R15, 16)
  BRA      L00800FC9

L00800FC8:
  MOV      L00C006C6, RQ20
  MOV      RQ20, RQ21
  MOV.Q    RQ21, @(R15, 16)

L00800FC9:
  MOV.Q    @(R15, 16), RQ19
  MOV.Q    TU0ADE02B5/plr, RQ18
  MOV.Q    RQ19, @(RQ18, 224)
  BRA      L00800FAF

L00800FB9:
  MOV.L    TU0ADE02B5/grid, RD25
  TST      RD25, RD25
  MOVT     RQ7
  MOV      RQ7, RD25
  MOV.L    RD25, TU0ADE02B5/grid
  MOV.L    TU0ADE02B5/grid, RD26
  CMP/EQ   #0, RD26
  BT       L00800FCA
  MOV      L00C006C9, RQ16
  MOV      RQ16, RQ17
  MOV.Q    RQ17, @(R15, 16)
  BRA      L00800FCB

L00800FCA:
  MOV      L00C006CA, RQ17
  MOV      RQ17, RQ16
  MOV.Q    RQ16, @(R15, 16)

L00800FCB:
  MOV.Q    @(R15, 16), RQ16
  MOV.Q    TU0ADE02B5/plr, RQ17
  MOV.Q    RQ16, @(RQ17, 224)
  BRA      L00800FAF

L00800FBA:
  MOV      buffer, RQ4
  MOV      L00C006CC, RQ5
  MOV      L00C006CD, RQ6
  MOV.L    TU0ADE02B5/markpointnum, RD7
  BSR      sprintf
  MOV      buffer, RQ25
  MOV.Q    TU0ADE02B5/plr, RQ26
  MOV.Q    RQ25, @(RQ26, 224)
  BSR      AM_addMark
  BRA      L00800FAF

L00800FBB:
  BSR      AM_clearMarks
  MOV      L00C006D1, RQ16
  MOV.Q    TU0ADE02B5/plr, RQ17
  MOV.Q    RQ16, @(RQ17, 224)
  BRA      L00800FAF

L00800FBC:
  MOV      #0, RD16
  MOV      #0, RD11
  MOV.L    RD16, cheatstate

L00800FAF:
  MOV.L    deathmatch, RD16
  CMP/EQ   #0, RD16
  BT       L00800FCC
  MOV.Q    @(R15, 40), RQ25
  MOV.L    @(RQ25, 4), RD12
  MOV      TU0ADE02B5/cheat_amap, RQ27
  MOV      RQ27, RQ26
  EXTS.B   RD12, RD28
  MOV.L    RD28, @(R15, 12)
  MOV.Q    RQ26, @(R15, 16)
  MOV      RQ26, RQ4
  MOV      RD28, RD5
  BSR      cht_CheckCheat
  MOV      RD2, RD13
  CMP/EQ   RD13
  BT       L00800FCC
  BRA      L00800FCD

L00800FCD:
  MOV      #0, RD11
  MOV.L    TU0ADE02B5/cheating, RD25
  ADD      RD25, #1, RD13
  MOV      RD13, RD25
  MOV      #0x55555556, RD7
  MOV      #21845, RD7
  DMULS    RD7, RD25
  STS      R1, RD6
  MOV      #3, RD7
  DMULU    RD7, RD6
  STS      R0, RD6
  SUB      RD6, RD25
  MOV.L    RD25, TU0ADE02B5/cheating

L00800FCC:
  BRA      L00800FCF

L00800FAE:
  MOV.Q    @(R15, 40), RQ16
  MOV.L    @RQ16, RD13
  CMP/EQ   #1, RD13
  BT       L00800FD0
  MOV      #0, RD11
  MOV.Q    @(R15, 40), RQ16
  MOV.L    @(RQ16, 4), RD13
  CMP/EQ   #174, RD13
  BT       L00800FD2
  CMP/EQ   #172, RD13
  BT       L00800FD3
  CMP/EQ   #173, RD13
  BT       L00800FD4
  CMP/EQ   #175, RD13
  BT       L00800FD5
  CMP/EQ   #45, RD13
  BT       L00800FD6
  CMP/EQ   #61, RD13
  BT       L00800FD7
  BRA      L00800FD1

L00800FD2:
  MOV.L    TU0ADE02B5/followplayer, RD16
  CMP/EQ   #0, RD16
  BT       L00800FD8
  MOV      #0, RD16
  MOV      TU0ADE02B5/m_paninc, RQ17
  MOV.L    RD16, @RQ17

L00800FD8:
  BRA      L00800FD1

L00800FD3:
  MOV.L    TU0ADE02B5/followplayer, RD16
  CMP/EQ   #0, RD16
  BT       L00800FD9
  MOV      #0, RD16
  MOV      TU0ADE02B5/m_paninc, RQ17
  MOV.L    RD16, @RQ17

L00800FD9:
  BRA      L00800FD1

L00800FD4:
  MOV.L    TU0ADE02B5/followplayer, RD16
  CMP/EQ   #0, RD16
  BT       L00800FDA
  MOV      #0, RD16
  MOV      TU0ADE02B5/m_paninc, RQ17
  MOV.L    RD16, @(RQ17, 4)

L00800FDA:
  BRA      L00800FD1

L00800FD5:
  MOV.L    TU0ADE02B5/followplayer, RD16
  CMP/EQ   #0, RD16
  BT       L00800FDB
  MOV      #0, RD16
  MOV      TU0ADE02B5/m_paninc, RQ17
  MOV.L    RD16, @(RQ17, 4)

L00800FDB:
  BRA      L00800FD1

L00800FD6:

L00800FD7:
  MOV      #0x00010000, RD16
  MOV      #1, RD16
  MOV      #0x00010000, RD17
  MOV      #1, RD17
  MOV.L    RD17, TU0ADE02B5/ftom_zoommul
  MOV.L    RD16, TU0ADE02B5/mtof_zoommul
  BRA      L00800FD1

L00800FD1:

L00800FD0:

L00800FCF:

L00800FAD:
  MOV      RD11, RD2

L00E00585:
  ADD      #72, R15
  BRA      L00C00626

AM_Ticker:
  PUSH     LR
  BSR      L00C0046F
  ADD      #-96, R15
  MOV.L    automapactive, RD13
  CMP/EQ   RD13
  BT       L00800FE5
  BRA      L00E00586

L00800FE5:
  MOV.L    TU0ADE02B5/amclock, RD12
  ADD      #1, RD12
  MOV.L    RD12, TU0ADE02B5/amclock
  MOV.L    TU0ADE02B5/followplayer, RD11
  CMP/EQ   RD11
  BT       L00800FE6
  BSR      AM_doFollowPlayer

L00800FE6:
  MOV.L    TU0ADE02B5/ftom_zoommul, RD10
  CMP/EQ   #65536, RD10
  BT       L00800FE7
  BSR      AM_changeWindowScale

L00800FE7:
  MOV      TU0ADE02B5/m_paninc, RQ9
  MOV.L    @RQ9, RD14
  CMP/EQ   RD14
  BT       L00800FE9
  MOV      TU0ADE02B5/m_paninc, RQ8
  MOV.L    @(RQ8, 4), RD14
  CMP/EQ   RD14
  BT       L00800FE9
  BRA      L00800FE8

L00800FE9:
  BSR      AM_changeWindowLoc

L00800FE8:

L00E00586:
  ADD      #96, R15
  BRA      L00C00470

L00C006DA:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  RTS     

AM_Drawer:
  PUSH     LR
  BSR      L00C006DA
  ADD      #-112, R15
  MOV.L    automapactive, RD14
  CMP/EQ   RD14
  BT       L00801082
  BRA      L00E00587

L00801082:
  MOV      #0, RD4
  BSR      AM_clearFB
  MOV.L    TU0ADE02B5/grid, RD13
  CMP/EQ   RD13
  BT       L00801083
  MOV      #104, RD4
  BSR      AM_drawGrid

L00801083:
  BSR      AM_drawWalls
  BSR      AM_drawPlayers
  MOV.L    TU0ADE02B5/cheating, RD12
  CMP/EQ   #2, RD12
  BT       L00801084
  MOV      #112, RD4
  MOV      #16, RD5
  BSR      AM_drawThings

L00801084:
  MOV      #96, RD4
  BSR      AM_drawCrosshair
  BSR      AM_drawMarks
  MOV.L    TU0ADE02B5/f_x, RD4
  MOV.L    TU0ADE02B5/f_y, RD5
  MOV.L    TU0ADE02B5/f_w, RD6
  MOV.L    TU0ADE02B5/f_h, RD7
  BSR      V_MarkRect

L00E00587:
  ADD      #112, R15

L00C006E6:
  POP      RQ12
  POP      RQ13
  POP      RQ14
  RET     

AM_Stop:
  PUSH     LR
  BSR      L00C006DA
  ADD      #-128, R15
  BSR      AM_unloadPics
  MOV      #0, RD13
  MOV      st_notify, RQ12
  MOV      RQ12, RQ14
  MOV.L    RD13, automapactive
  MOV      RQ14, RQ4
  BSR      ST_Responder
  MOV      #1, RD12
  MOV.L    RD12, TU0ADE02B5/stopped

L00E00588:
  ADD      #128, R15
  BRA      L00C006E6

P_SetupLevel:
  PUSH     LR
  BSR      L00C00630
  ADD      #-424, R15
  MOV.L    RD4, @(R15, 64)
  MOV.L    RD5, @(R15, 60)
  MOV.L    RD6, @(R15, 56)
  MOV.L    RD7, @(R15, 52)
  MOV      #68, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 40)
  MOV      #0, RD13
  MOV      wminfo, RQ12
  MOV.L    RD13, @(RQ12, 28)
  MOV      #0, RD11
  MOV      #0, RD10
  MOV      #0, RD9
  MOV      #180, RD8
  MOV.L    RD8, @(RQ12, 32)
  MOV      #0, RD31
  MOV.L    RD11, totalsecret
  MOV.L    RD10, totalitems
  MOV.L    RD9, totalkills
  MOV.L    RD31, @(R15, 48)

L0080193C:
  MOV.L    @(R15, 48), RD30
  MOV      #4, RD29
  CMP/GE   RD29, RD30
  BT       L0080193E
  MOV      players, RQ28
  MOV.L    @(R15, 48), RD27
  EXTS.L   RD27
  MOV      #320, RD7
  MUL.L    RD27, RD7
  STS      R0, RQ26
  ADD      RQ28, RQ26
  MOV      #0, RD25
  MOV.L    RD25, @(RQ26, 212)
  EXTS.L   RD27
  MOV      #320, RD7
  MUL.L    RD27, RD7
  STS      R0, RQ26
  ADD      RQ28, RQ26
  MOV.L    RD25, @(RQ26, 216)
  EXTS.L   RD27
  MOV      #320, RD7
  MUL.L    RD27, RD7
  STS      R0, RQ26
  ADD      RQ28, RQ26
  MOV.L    RD25, @(RQ26, 208)
  MOV.Q    RQ26, @(R15, 24)

L0080193D:
  MOV.L    @(R15, 48), RD7
  ADD      #1, RD7
  MOV.L    RD7, @(R15, 48)
  BRA      L0080193C

L0080193E:
  MOV      players, RQ25
  MOV.L    consoleplayer, RD26
  EXTS.L   RD26
  MOV      #320, RD7
  MUL.L    RD26, RD7
  STS      R0, RQ27
  ADD      RQ25, RQ27
  MOV      #1, RD28
  MOV.L    RD28, @(RQ27, 20)
  MOV.Q    RQ27, @(R15, 24)
  BSR      S_Start
  MOV      #50, RD4
  MOV      #99, RD5
  BSR      Z_FreeTags
  BSR      P_InitThinkers
  BSR      W_Reload
  MOV.L    gamemode, RD6
  CMP/EQ   #2, RD6
  BT       L00801940
  MOV.L    gamemode, RD5
  CMP/EQ   #5, RD5
  BT       L00801940
  BRA      L0080193F

L00801940:
  MOV.L    @(R15, 60), RD4
  MOV      #10, RD3
  CMP/GE   RD3, RD4
  BT       L00801942
  MOV      #68, RQ4
  ADD      R15, RQ4
  MOV      L00C006EC, RQ5
  MOV.L    @(R15, 60), RD6
  BSR      sprintf
  BRA      L00801943

L00801942:
  MOV      #68, RQ4
  ADD      R15, RQ4
  MOV      L00C006ED, RQ5
  MOV.L    @(R15, 60), RD6
  BSR      sprintf

L00801943:
  BRA      L00801944

L0080193F:
  MOV      #68, RQ2
  ADD      R15, RQ2
  MOV      #69, RD23
  MOV.B    RD23, @RQ2
  MOV.L    @(R15, 64), RD22
  ADD      RD22, #48, RD14
  MOV.B    RD14, @(RQ2, 1)
  MOV      #77, RD21
  MOV.B    RD21, @(RQ2, 2)
  MOV.L    @(R15, 60), RD20
  ADD      RD20, #48, RD14
  MOV.B    RD14, @(RQ2, 3)
  MOV      #0, RD19
  MOV.B    RD19, @(RQ2, 4)

L00801944:
  MOV      #68, RQ4
  ADD      R15, RQ4
  BSR      W_GetNumForName
  MOV      RD2, RD25
  MOV.L    RD25, @(R15, 36)
  MOV      #0, RD26
  CMP/GE   RD26, RD25
  BT       L00801945
  BRA      L00E00589

L00801945:
  MOV      #0, RD25
  MOV      #0, RD26
  MOV.L    RD26, p_map_hexenfmt
  MOV.L    RD25, leveltime
  BSR      R_FlushPolyObj
  MOV.L    @(R15, 36), RD25
  ADD      RD25, #10, RD14
  MOV      RD14, RD4
  BSR      P_LoadBlockMap
  ADD      RD25, #4, RD14
  MOV      RD14, RD4
  BSR      P_LoadVertexes
  ADD      RD25, #8, RD14
  MOV      RD14, RD4
  BSR      P_LoadSectors
  ADD      RD25, #3, RD14
  MOV      RD14, RD4
  BSR      P_LoadSideDefs
  ADD      RD25, #11, RD14
  MOV      RD14, RD4
  BSR      P_LoadBehavior
  ADD      RD25, #2, RD14
  MOV      RD14, RD4
  BSR      P_LoadLineDefs
  ADD      RD25, #6, RD14
  MOV      RD14, RD4
  BSR      P_LoadSubsectors
  ADD      RD25, #7, RD14
  MOV      RD14, RD4
  BSR      P_LoadNodes
  ADD      RD25, #5, RD14
  MOV      RD14, RD4
  BSR      P_LoadSegs
  ADD      RD25, #9, RD14
  MOV      RD14, RD4
  MOV      #50, RD5
  BSR      W_CacheLumpNum
  MOV      RQ2, RQ26
  MOV.Q    RQ26, rejectmatrix
  BSR      P_GroupLines
  MOV      #0, RD27
  MOV      deathmatchstarts, RQ29
  MOV      RQ29, RQ28
  ADD      RD25, #1, RD14
  MOV.Q    RQ28, deathmatch_p
  MOV.L    RD27, bodyqueslot
  MOV      RD14, RD4
  BSR      P_LoadThings
  MOV.L    deathmatch, RD18
  CMP/EQ   #0, RD18
  BT       L00801946
  MOV      #0, RD17
  MOV.L    RD17, @(R15, 48)

L00801947:
  MOV.L    @(R15, 48), RD16
  MOV      #4, RD17
  CMP/GE   RD17, RD16
  BT       L00801949
  MOV      playeringame, RQ16
  MOV.L    @(R15, 48), RD17
  EXTS.L   RD17
  MOV.L    @(RQ16, RD17), RD14
  CMP/EQ   RD14
  BT       L0080194A
  MOV      #0, RQ25
  MOV      players, RQ26
  MOV.L    @(R15, 48), RD27
  EXTS.L   RD27
  MOV      #320, RD7
  MUL.L    RD27, RD7
  STS      R0, RQ28
  ADD      RQ26, RQ28
  MOV.Q    RQ25, @RQ28
  MOV.Q    RQ28, @(R15, 24)
  MOV      RD27, RD4
  BSR      G_DeathMatchSpawnPlayer

L0080194A:

L00801948:
  MOV.L    @(R15, 48), RD16
  ADD      #1, RD16
  MOV.L    RD16, @(R15, 48)
  BRA      L00801947

L00801949:

L00801946:
  MOV      #0, RD25
  MOV      #0, RD26
  MOV.L    RD26, iquehead
  MOV.L    RD25, iquetail
  BSR      P_SpawnSpecials
  MOV.L    precache, RD16
  CMP/EQ   #0, RD16
  BT       L0080194B
  BSR      R_PrecacheLevel

L0080194B:

L00E00589:
  ADD      #424, R15
  BRA      L00C00626

P_Init:
  PUSH     LR
  PUSH     R14
  PUSH     R13
  ADD      #-136, R15
  BSR      P_InitSwitchList
  BSR      P_InitPicAnims
  MOV      sprnames, RQ13
  MOV      RQ13, RQ14
  MOV      RQ14, RQ4
  BSR      R_InitSprites

L00E0058A:
  ADD      #136, R15
  POP      RQ13
  POP      RQ14
  RET     

R_PointOnSide:
  PUSH     LR
  BSR      L00C00630
  ADD      #-72, R15
  MOV.L    RD4, @(R15, 44)
  MOV.L    RD5, @(R15, 40)
  MOV      RQ6, RQ12
  MOV.L    @(RQ12, 8), RD14
  CMP/EQ   RD14
  BT       L00801E83
  MOV.L    @RQ12, RD14
  MOV.L    @(R15, 44), RD11
  CMP/GT   RD14, RD11
  BT       L00801E84
  MOV.L    @(RQ12, 12), RD14
  MOV      #0, RD10
  CMP/GT   RD10, RD14
  MOVT     RD13
  MOV      RD13, RD2
  BRA      L00E0058B

L00801E84:
  MOV.L    @(RQ12, 12), RD14
  MOV      #0, RD9
  CMP/GE   RD9, RD14
  MOVNT    RD13
  MOV      RD13, RD2
  BRA      L00E0058B

L00801E83:
  MOV.L    @(RQ12, 12), RD14
  CMP/EQ   RD14
  BT       L00801E85
  MOV.L    @(RQ12, 4), RD14
  MOV.L    @(R15, 40), RD8
  CMP/GT   RD14, RD8
  BT       L00801E86
  MOV.L    @(RQ12, 8), RD14
  MOV      #0, RD31
  CMP/GE   RD31, RD14
  MOVNT    RD13
  MOV      RD13, RD2
  BRA      L00E0058B

L00801E86:
  MOV.L    @(RQ12, 8), RD14
  MOV      #0, RD30
  CMP/GT   RD30, RD14
  MOVT     RD13
  MOV      RD13, RD2
  BRA      L00E0058B

L00801E85:
  MOV.L    @RQ12, RD14
  MOV.L    @(R15, 44), RD28
  SUB      RD28, RD14, RD29
  MOV.L    @(RQ12, 4), RD14
  MOV.L    @(R15, 40), RD26
  SUB      RD26, RD14, RD27
  MOV.L    @(RQ12, 12), RD14
  MOV.L    @(RQ12, 8), RD13
  XOR      RD14, RD13, RD25
  XOR      RD25, RD29, RD14
  XOR      RD14, RD27, RD13
  MOV.L    RD29, @(R15, 28)
  MOV.L    RD27, @(R15, 24)
  TST      #-2147483648, RD13
  BT       L00801E87
  MOV.L    @(RQ12, 12), RD14
  MOV.L    @(R15, 28), RD7
  XOR      RD14, RD7, RD13
  TST      #-2147483648, RD13
  BT       L00801E88
  MOV      #1, RD2
  BRA      L00E0058B

L00801E88:
  MOV      #0, RD2
  BRA      L00E0058B

L00801E87:
  MOV.L    @(RQ12, 12), RD14
  SHAD     RD14, #-16, RD13
  MOV      RD13, RD4
  MOV.L    @(R15, 28), RD5
  BSR      FixedMul
  MOV      RD2, RD25
  MOV.L    @(RQ12, 8), RD14
  SHAD     RD14, #-16, RD13
  MOV.L    RD25, @(R15, 20)
  MOV.L    @(R15, 24), RD4
  MOV      RD13, RD5
  BSR      FixedMul
  MOV      RD2, RD26
  MOV.L    RD26, @(R15, 16)
  CMP/GT   RD26, RD25
  BT       L00801E89
  MOV      #0, RD2
  BRA      L00E0058B

L00801E89:
  MOV      #1, RD2

L00E0058B:
  ADD      #72, R15
  BRA      L00C00626

R_PointOnSegSide:
  PUSH     LR
  BSR      L00C00630
  ADD      #-72, R15
  MOV.L    RD4, @(R15, 44)
  MOV.L    RD5, @(R15, 40)
  MOV      RQ6, RQ10
  MOV.Q    @RQ10, RQ13
  MOV.L    @RQ13, RD9
  MOV.Q    @RQ10, RQ13
  MOV.L    @(RQ13, 4), RD8
  MOV.Q    @(RQ10, 8), RQ13
  MOV.L    @RQ13, RD14
  SUB      RD14, RD9, RD11
  MOV.Q    @(RQ10, 8), RQ13
  MOV.L    @(RQ13, 4), RD14
  SUB      RD14, RD8, RD12
  CMP/EQ   RD11
  BT       L00801E8A
  MOV.L    @(R15, 44), RD31
  CMP/GT   RD9, RD31
  BT       L00801E8B
  MOV      #0, RD30
  CMP/GT   RD30, RD12
  MOVT     RD14
  MOV      RD14, RD2
  BRA      L00E0058C

L00801E8B:
  MOV      #0, RD29
  CMP/GE   RD29, RD12
  MOVNT    RD14
  MOV      RD14, RD2
  BRA      L00E0058C

L00801E8A:
  CMP/EQ   RD12
  BT       L00801E8C
  MOV.L    @(R15, 40), RD28
  CMP/GT   RD8, RD28
  BT       L00801E8D
  MOV      #0, RD27
  CMP/GE   RD27, RD11
  MOVNT    RD14
  MOV      RD14, RD2
  BRA      L00E0058C

L00801E8D:
  MOV      #0, RD26
  CMP/GT   RD26, RD11
  MOVT     RD14
  MOV      RD14, RD2
  BRA      L00E0058C

L00801E8C:
  MOV.L    @(R15, 44), RD7
  SUB      RD7, RD9, RD25
  MOV.L    @(R15, 40), RD5
  SUB      RD5, RD8, RD6
  XOR      RD12, RD11, RD14
  XOR      RD14, RD25, RD4
  XOR      RD4, RD6, RD14
  MOV.L    RD25, @(R15, 28)
  MOV.L    RD6, @(R15, 24)
  TST      #-2147483648, RD14
  BT       L00801E8E
  MOV.L    @(R15, 28), RD3
  XOR      RD12, RD3, RD14
  TST      #-2147483648, RD14
  BT       L00801E8F
  MOV      #1, RD2
  BRA      L00E0058C

L00801E8F:
  MOV      #0, RD2
  BRA      L00E0058C

L00801E8E:
  SHAD     RD12, #-16, RD14
  MOV      RD14, RD4
  MOV.L    @(R15, 28), RD5
  BSR      FixedMul
  MOV      RD2, RD25
  SHAD     RD11, #-16, RD14
  MOV.L    RD25, @(R15, 20)
  MOV.L    @(R15, 24), RD4
  MOV      RD14, RD5
  BSR      FixedMul
  MOV      RD2, RD26
  MOV.L    RD26, @(R15, 16)
  CMP/GT   RD26, RD25
  BT       L00801E90
  MOV      #0, RD2
  BRA      L00E0058C

L00801E90:
  MOV      #1, RD2

L00E0058C:
  ADD      #72, R15
  BRA      L00C00626

R_PointToAngle:
  PUSH     LR
  BSR      L00C00630
  ADD      #-40, R15
  MOV      RD4, RD12
  MOV      RD5, RD13
  MOV.L    viewx, RD11
  SUB      RD11, RD12
  MOV.L    viewy, RD10
  SUB      RD10, RD13
  CMP/EQ   RD12
  BT       L00801E91
  CMP/EQ   RD13
  BT       L00801E91
  BRA      L00801E92

L00801E92:
  MOV      #0, RD2
  BRA      L00E0058D

L00801E91:
  CMP/PZ   RD12
  BT       L00801E94
  CMP/PZ   RD13
  BT       L00801E95
  CMP/GT   RD13, RD12
  BT       L00801E96
  MOV      RD13, RD4
  MOV      RD12, RD5
  BSR      SlopeDiv
  MOV      RD2, RD14
  MOV      tantoangle, RQ9
  EXTS.L   RD14
  MOVU.L   @(RQ9, RD14), RD8
  MOV.L    RD8, @R15
  MOV      RD8, RD2
  BRA      L00E0058D
  BRA      L00801E97

L00801E96:
  MOV      RD12, RD4
  MOV      RD13, RD5
  BSR      SlopeDiv
  MOV      RD2, RD14
  MOV      tantoangle, RQ31
  EXTS.L   RD14
  MOVU.L   @(RQ31, RD14), RD30
  MOV      #0x3FFFFFFF, RD29
  MOV      #16383, RD29
  SUB      RD29, RD30, RD14
  MOV.L    RD30, @R15
  MOV      RD14, RD2
  BRA      L00E0058D

L00801E97:
  BRA      L00801E98

L00801E95:
  NEG      RD13, RD13
  CMP/GT   RD13, RD12
  BT       L00801E99
  MOV      RD13, RD4
  MOV      RD12, RD5
  BSR      SlopeDiv
  MOV      RD2, RD14
  MOV      tantoangle, RQ28
  EXTS.L   RD14
  MOVU.L   @(RQ28, RD14), RD27
  NEG      RD27, RD14
  MOV.L    RD27, @R15
  MOV      RD14, RD2
  BRA      L00E0058D
  BRA      L00801E9A

L00801E99:
  MOV      RD12, RD4
  MOV      RD13, RD5
  BSR      SlopeDiv
  MOV      RD2, RD14
  MOV      tantoangle, RQ26
  EXTS.L   RD14
  MOVU.L   @(RQ26, RD14), RD25
  ADD      RD25, #-1073741824, RD14
  MOV.L    RD25, @R15
  MOV      RD14, RD2
  BRA      L00E0058D

L00801E9A:

L00801E98:
  BRA      L00801E9B

L00801E94:
  NEG      RD12, RD12
  CMP/PZ   RD13
  BT       L00801E9C
  CMP/GT   RD13, RD12
  BT       L00801E9D
  MOV      RD13, RD4
  MOV      RD12, RD5
  BSR      SlopeDiv
  MOV      RD2, RD14
  MOV      tantoangle, RQ25
  EXTS.L   RD14
  MOVU.L   @(RQ25, RD14), RD26
  MOV      #0x7FFFFFFF, RD27
  MOV      #32767, RD27
  SUB      RD27, RD26, RD14
  MOV.L    RD26, @R15
  MOV      RD14, RD2
  BRA      L00E0058D
  BRA      L00801E9E

L00801E9D:
  MOV      RD12, RD4
  MOV      RD13, RD5
  BSR      SlopeDiv
  MOV      RD2, RD14
  MOV      tantoangle, RQ25
  EXTS.L   RD14
  MOVU.L   @(RQ25, RD14), RD26
  ADD      RD26, #1073741824, RD14
  MOV.L    RD26, @R15
  MOV      RD14, RD2
  BRA      L00E0058D

L00801E9E:
  BRA      L00801E9F

L00801E9C:
  NEG      RD13, RD13
  CMP/GT   RD13, RD12
  BT       L00801EA0
  MOV      RD13, RD4
  MOV      RD12, RD5
  BSR      SlopeDiv
  MOV      RD2, RD14
  MOV      tantoangle, RQ25
  EXTS.L   RD14
  MOVU.L   @(RQ25, RD14), RD26
  ADD      RD26, #-2147483648, RD14
  MOV.L    RD26, @R15
  MOV      RD14, RD2
  BRA      L00E0058D
  BRA      L00801EA1

L00801EA0:
  MOV      RD12, RD4
  MOV      RD13, RD5
  BSR      SlopeDiv
  MOV      RD2, RD14
  MOV      tantoangle, RQ25
  EXTS.L   RD14
  MOVU.L   @(RQ25, RD14), RD26
  MOV      #0xBFFFFFFF, RD27
  MOV      #-16385, RD27
  SUB      RD27, RD26, RD14
  MOV.L    RD26, @R15
  MOV      RD14, RD2
  BRA      L00E0058D

L00801EA1:

L00801E9F:

L00801E9B:
  MOV      #0, RD2

L00E0058D:
  ADD      #40, R15
  BRA      L00C00626

L00C0071A:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  RTS     

R_PointToAngle2:
  PUSH     LR
  BSR      L00C0071A
  ADD      #-128, R15
  MOV.L    RD4, @(R15, 28)
  MOV.L    RD5, @(R15, 24)
  MOV.L    RD6, @(R15, 20)
  MOV.L    RD7, @(R15, 16)
  MOV.L    @(R15, 28), RD12
  MOV      RD12, RD13
  MOV.L    @(R15, 24), RD10
  MOV      RD10, RD11
  MOV.L    RD13, viewx
  MOV.L    RD11, viewy
  MOV.L    @(R15, 20), RD4
  MOV.L    @(R15, 16), RD5
  BSR      R_PointToAngle
  MOV      RD2, RD14
  MOV      RD14, RD2

L00E0058E:
  ADD      #128, R15

L00C0071B:
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  RET     

R_PointToDist:
  PUSH     LR
  BSR      L00C00472
  ADD      #-72, R15
  MOV.L    RD4, @(R15, 28)
  MOV.L    RD5, @(R15, 24)
  MOV.L    @(R15, 28), RD11
  MOV.L    viewx, RD10
  SUB      RD11, RD10, RD14
  MOV      RD14, RD4
  BSR      abs
  MOV      RD2, RD13
  MOV.L    @(R15, 24), RD9
  MOV.L    viewy, RD8
  SUB      RD9, RD8, RD14
  MOV      RD14, RD4
  BSR      abs
  MOV      RD2, RD12
  CMP/GT   RD13, RD12
  BT       L00801EA2
  MOV      RD13, RD31
  MOV      RD12, RD13
  MOV      RD31, RD12
  MOV.L    RD31, @(R15, 16)

L00801EA2:
  MOV      RD12, RD4
  MOV      RD13, RD5
  BSR      FixedDiv
  MOV      RD2, RD14
  SHAD     RD14, #-5, RD30
  MOV      tantoangle, RQ29
  EXTS.L   RD30
  MOVU.L   @(RQ29, RD30), RD14
  ADD      RD14, #1073741824, RD30
  SHLD     RD30, #-19, RD28
  MOV      finesine, RQ27
  EXTS.L   RD28
  MOV.L    @(RQ27, RD28), RD14
  MOV.L    RD28, @(R15, 20)
  MOV      RD13, RD4
  MOV      RD14, RD5
  BSR      FixedDiv
  MOV      RD2, RD31
  MOV.L    RD31, @(R15, 12)
  MOV      RD31, RD2

L00E0058F:
  ADD      #72, R15
  BRA      L00C00476

L00C0071E:
  PUSH     R31
  PUSH     R30
  PUSH     R29
  PUSH     R28
  PUSH     R27
  PUSH     R26
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  PUSH     R9
  PUSH     R8
  RTS     

R_ScaleFromGlobalAngle:
  PUSH     LR
  BSR      L00C0071E
  ADD      #-64, R15
  MOV      RD4, RD12
  MOV.L    viewangle, RD10
  SUB      RD12, RD10, RD14
  ADD      RD14, #1073741824, RD11
  MOV.L    rw_normalangle, RD9
  SUB      RD12, RD9, RD14
  ADD      RD14, #1073741824, RD8
  SHAD     RD11, #-19, RD14
  MOV      finesine, RQ31
  MOV.L    @(RQ31, RD14), RD30
  SHAD     RD8, #-19, RD14
  MOV.L    @(RQ31, RD14), RD29
  MOV.L    RD8, @(R15, 24)
  MOV.L    RD30, @(R15, 20)
  MOV.L    RD29, @(R15, 16)
  MOV.L    projection, RD4
  MOV      RD29, RD5
  BSR      FixedMul
  MOV      RD2, RD14
  MOV.L    detailshift, RD27
  SHAD     RD14, RD27, RD28
  MOV.L    RD28, @(R15, 12)
  MOV.L    rw_distance, RD4
  MOV      RD30, RD5
  BSR      FixedMul
  MOV      RD2, RD26
  SHAD     RD28, #-16, RD14
  MOV.L    RD26, @(R15, 8)
  CMP/GT   RD14, RD26
  BT       L00801EA3
  MOV.L    @(R15, 12), RD4
  MOV.L    @(R15, 8), RD5
  BSR      FixedDiv
  MOV      RD2, RD13
  CMP/GT   #4194304, RD13
  BT       L00801EA4
  MOV      #0x00400000, RD13
  MOV      #64, RD13
  BRA      L00801EA5

L00801EA4:
  CMP/GE   #256, RD13
  BT       L00801EA6
  MOV      #256, RD13

L00801EA6:

L00801EA5:
  BRA      L00801EA7

L00801EA3:
  MOV      #0x00400000, RD13
  MOV      #64, RD13

L00801EA7:
  MOV      RD13, RD2

L00E00590:
  ADD      #64, R15

L00C0071F:
  POP      RQ8
  POP      RQ9
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  POP      RQ26
  POP      RQ27
  POP      RQ28
  POP      RQ29
  POP      RQ30
  POP      RQ31
  RET     

L00C00721:
  PUSH     R31
  PUSH     R30
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  PUSH     R9
  PUSH     R8
  RTS     

R_PointInSubsector:
  PUSH     LR
  BSR      L00C00721
  ADD      #-96, R15
  MOV.L    RD4, @(R15, 28)
  MOV.L    RD5, @(R15, 24)
  MOV.L    numnodes, RD10
  CMP/EQ   RD10
  BT       L00801EDA
  MOV.Q    subsectors, RQ2
  BRA      L00E00591

L00801EDA:
  MOV.L    numnodes, RD9
  SUB      RD9, #1, RD13

L00801EDB:
  TST      #32768, RD13
  BT       L00801EDC
  MOV.Q    nodes, RQ8
  EXTS.L   RD13
  MOV      #52, RD7
  MUL.L    RD13, RD7
  STS      R0, RQ14
  ADD      RQ8, RQ14
  MOV      RQ14, RQ12
  MOV.L    @(R15, 28), RD4
  MOV.L    @(R15, 24), RD5
  MOV      RQ12, RQ6
  BSR      R_PointOnSide
  MOV      RD2, RD11
  ADD      RQ12, #48, RQ14
  EXTS.L   RD11
  MOVU.W   @(RQ14, RD11), RD13
  BRA      L00801EDB

L00801EDC:
  AND      RD13, #-32769, RD31
  MOV.Q    subsectors, RQ30
  EXTS.L   RD31
  SHAD     RD31, #4, RQ14
  ADD      RQ30, RQ14
  MOV      RQ14, RQ2

L00E00591:
  ADD      #96, R15

L00C00722:
  POP      RQ8
  POP      RQ9
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  POP      RQ30
  POP      RQ31
  RET     

R_RenderPlayerView:
  PUSH     LR
  PUSH     R14
  PUSH     R13
  ADD      #-136, R15
  MOV.Q    RQ4, @(R15, 8)
  MOV.Q    @(R15, 8), RQ4
  BSR      R_SetupFrame
  BSR      R_ClearClipSegs
  BSR      R_ClearDrawSegs
  BSR      R_ClearPlanes
  BSR      R_ClearSprites
  BSR      NetUpdate
  MOV.L    numnodes, RD13
  SUB      RD13, #1, RD14
  MOV      RD14, RD4
  BSR      R_RenderBSPNode
  BSR      NetUpdate
  BSR      R_DrawPlanes
  BSR      NetUpdate
  BSR      R_DrawMasked
  BSR      NetUpdate

L00E00592:
  ADD      #136, R15
  POP      RQ13
  POP      RQ14
  RET     

R_Init:
  PUSH     LR
  ADD      #-136, R15
  BSR      R_InitData
  MOV      L00C0072D, RQ4
  BSR      printf
  BSR      R_InitPointToAngle
  MOV      L00C0072F, RQ4
  BSR      printf
  BSR      R_InitTables
  MOV      L00C00731, RQ4
  BSR      printf
  MOV.L    screenblocks, RD4
  MOV.L    detailLevel, RD5
  BSR      R_SetViewSize
  BSR      R_InitPlanes
  MOV      L00C00735, RQ4
  BSR      printf
  BSR      R_InitLightTables
  MOV      L00C00737, RQ4
  BSR      printf
  BSR      R_InitSkyMap
  MOV      L00C00739, RQ4
  BSR      printf
  BSR      R_InitTranslationTables
  MOV      L00C0073B, RQ4
  BSR      printf
  MOV      #0, RD7
  MOV.L    RD7, framecount

L00E00593:
  ADD      #136, R15
  RET     

R_SetViewSize:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  ADD      #-120, R15
  MOV.L    RD4, @(R15, 12)
  MOV.L    RD5, @(R15, 8)
  MOV      #1, RD14
  MOV.L    @(R15, 12), RD12
  MOV      RD12, RD13
  MOV.L    @(R15, 8), RD10
  MOV      RD10, RD11
  MOV.L    RD14, setsizeneeded
  MOV.L    RD13, setblocks
  MOV.L    RD11, setdetail

L00E00594:
  ADD      #120, R15

L00C0073F:
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  RTS     

R_ClearClipSegs:
  PUSH     LR
  BSR      L00C00649
  ADD      #-104, R15
  MOV      solidsegs, RQ14
  MOV      RQ14, RQ13
  MOV      #0x80000001, RD12
  MOV      #-32768, RD12
  MOV.L    RD12, @RQ13
  MOV      RQ14, RQ13
  MOV      #-1, RD11
  MOV.L    RD11, @(RQ13, 4)
  ADD      RQ14, #8, RQ13
  MOV.L    viewwidth, RD10
  MOV.L    RD10, @RQ13
  ADD      RQ14, #8, RQ13
  MOV      #0x7FFFFFFF, RD9
  MOV      #32767, RD9
  MOV.L    RD9, @(RQ13, 4)
  ADD      RQ14, #16, RQ7
  MOV.Q    RQ7, newend

L00E00595:
  ADD      #104, R15
  BRA      L00C0064D

R_ClearDrawSegs:
  PUSH     R14
  PUSH     R13
  ADD      #-128, R15
  MOV      drawsegs, RQ13
  MOV      RQ13, RQ14
  MOV.Q    RQ14, ds_p

L00E00596:
  ADD      #128, R15
  POP      RQ13
  POP      RQ14
  RTS     

L00C0074E:
  PUSH     R31
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  PUSH     R9
  PUSH     R8
  RTS     

R_RenderBSPNode:
  PUSH     LR
  BSR      L00C0074E
  ADD      #-136, R15
  MOV      RD4, RD10
  TST      #32768, RD10
  BT       L00801D6B
  CMP/EQ   #-1, RD10
  BT       L00801D6C
  MOV      #0, RD4
  BSR      R_Subsector
  BRA      L00801D6D

L00801D6C:
  AND      RD10, #-32769, RD14
  MOV      RD14, RD4
  BSR      R_Subsector

L00801D6D:
  BRA      L00E00597

L00801D6B:
  MOV.Q    nodes, RQ9
  EXTS.L   RD10
  MOV      #52, RD7
  MUL.L    RD10, RD7
  STS      R0, RQ13
  ADD      RQ9, RQ13
  MOV      RQ13, RQ12
  MOV.L    viewx, RD4
  MOV.L    viewy, RD5
  MOV      RQ12, RQ6
  BSR      R_PointOnSide
  MOV      RD2, RD11
  ADD      RQ12, #48, RQ13
  EXTS.L   RD11
  MOVU.W   @(RQ13, RD11), RD14
  MOV      RD14, RD4
  BSR      R_RenderBSPNode
  ADD      RQ12, #16, RQ13
  XOR      RD11, #1, RD14
  EXTS.L   RD14
  SHAD     RD14, #4, RQ8
  ADD      RQ13, RQ8
  MOV.Q    RQ8, @(R15, 16)
  MOV      RQ8, RQ4
  BSR      R_CheckBBox
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L00801D6E
  ADD      RQ12, #48, RQ13
  XOR      RD11, #1, RD14
  EXTS.L   RD14
  MOVU.W   @(RQ13, RD14), RD31
  MOV.L    RD31, @(R15, 12)
  MOV      RD31, RD4
  BSR      R_RenderBSPNode

L00801D6E:

L00E00597:
  ADD      #136, R15

L00C00751:
  POP      RQ8
  POP      RQ9
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  POP      RQ31
  RET     

R_RenderMaskedSegRange:
  PUSH     LR
  BSR      L00C00630
  ADD      #-104, R15
  MOV.Q    RQ4, @(R15, 72)
  MOV.L    RD5, @(R15, 68)
  MOV.L    RD6, @(R15, 64)
  MOV.Q    @(R15, 72), RQ11
  MOV.Q    @RQ11, RQ10
  MOV.Q    @(RQ10, 40), RQ9
  MOV.Q    @(RQ10, 48), RQ8
  MOV.Q    @(RQ10, 24), RQ12
  MOV.W    @(RQ12, 12), RD14
  MOV.Q    texturetranslation, RQ31
  MOV.L    @(RQ31, RD14), RD30
  MOV.W    @(RQ9, 12), RD14
  SHAD     RD14, #-4, RD13
  MOV.L    extralight, RD28
  ADD      RD13, RD28, RD29
  MOV.Q    @RQ10, RQ12
  MOV.L    @(RQ12, 4), RD14
  MOV.Q    @(RQ10, 8), RQ12
  MOV.L    @(RQ12, 4), RD13
  MOV.Q    RQ10, curline
  MOV.Q    RQ9, frontsector
  MOV.Q    RQ8, backsector
  MOV.L    RD30, @(R15, 40)
  MOV.L    RD29, @(R15, 44)
  CMP/EQ   RD13, RD14
  BT       L00801F73
  MOV.L    @(R15, 44), RD27
  ADD      #-1, RD27
  MOV.L    RD27, @(R15, 44)
  BRA      L00801F74

L00801F73:
  MOV.Q    curline, RQ26
  MOV.Q    @RQ26, RQ12
  MOV.L    @RQ12, RD14
  MOV.Q    @(RQ26, 8), RQ12
  MOV.L    @RQ12, RD13
  CMP/EQ   RD13, RD14
  BT       L00801F75
  MOV.L    @(R15, 44), RD25
  ADD      #1, RD25
  MOV.L    RD25, @(R15, 44)

L00801F75:

L00801F74:
  MOV.L    @(R15, 44), RD7
  CMP/PZ   RD7
  BT       L00801F76
  MOV      scalelight, RQ6
  MOV      RQ6, RQ5
  MOV.Q    RQ5, walllights
  BRA      L00801F77

L00801F76:
  MOV.L    @(R15, 44), RD4
  CMP/GE   #16, RD4
  BT       L00801F78
  MOV      scalelight, RQ3
  ADD      RQ3, #5760, RQ2
  MOV.Q    RQ2, walllights
  BRA      L00801F79

L00801F78:
  MOV      scalelight, RQ23
  MOV.L    @(R15, 44), RD22
  EXTS.L   RD22
  MOV      RD22, RQ21
  SHLL1    RQ21
  ADD      RD22, RQ21
  SHAD     #7, RQ21
  ADD      RQ23, RQ21
  MOV.Q    RQ21, walllights

L00801F79:

L00801F77:
  MOV.Q    @(R15, 72), RQ20
  MOV.Q    @(RQ20, 72), RQ19
  MOV.L    @(RQ20, 24), RD18
  MOV.L    @(RQ20, 36), RD17
  MOV.L    @(RQ20, 16), RD14
  MOV.L    @(RQ20, 8), RD13
  MOV.L    @(R15, 68), RD21
  SUB      RD21, RD13, RD16
  MUL.L    RD16, RD18, RD13
  ADD      RD14, RD13, RD22
  MOV.Q    @(RQ20, 64), RQ23
  MOV.Q    @(RQ20, 56), RQ2
  MOV.Q    curline, RQ3
  MOV.Q    @(RQ3, 32), RQ12
  MOV.W    @(RQ12, 24), RD14
  MOV.Q    RQ2, mceilingclip
  MOV.Q    RQ23, mfloorclip
  MOV.L    RD22, spryscale
  MOV.Q    RQ19, maskedtexturecol
  MOV.L    RD18, rw_scalestep
  MOV.L    RD17, rw_zstep
  MOV.L    RD16, @(R15, 12)
  TST      #16, RD14
  BT       L00801F7A
  MOV.Q    frontsector, RQ16
  MOV.L    @RQ16, RD13
  MOV.Q    backsector, RQ17
  MOV.L    @RQ17, RD18
  MOV.L    RD18, @(R15, 12)
  CMP/GT   RD18, RD13
  BT       L00801F7B
  MOV.Q    frontsector, RQ16
  MOV.L    @RQ16, RD13
  MOV      RD13, RD14
  BRA      L00801F7C

L00801F7B:
  MOV.Q    backsector, RQ16
  MOV.L    @RQ16, RD13
  MOV      RD13, RD14

L00801F7C:
  MOV      RD14, RD16
  MOV.Q    textureheight, RQ17
  MOV.L    @(R15, 40), RD18
  EXTS.L   RD18
  MOV.L    @(RQ17, RD18), RD14
  ADD      RD16, RD14, RD13
  MOV.L    viewz, RD19
  SUB      RD13, RD19, RD16
  MOV.L    RD16, dc_texturemid
  BRA      L00801F7D

L00801F7A:
  MOV.Q    frontsector, RQ16
  MOV.L    @(RQ16, 4), RD13
  MOV.Q    backsector, RQ17
  MOV.L    @(RQ17, 4), RD18
  MOV.L    RD18, @(R15, 12)
  CMP/GT   RD13, RD18
  BT       L00801F7E
  MOV.Q    frontsector, RQ16
  MOV.L    @(RQ16, 4), RD13
  MOV      RD13, RD14
  BRA      L00801F7F

L00801F7E:
  MOV.Q    backsector, RQ16
  MOV.L    @(RQ16, 4), RD13
  MOV      RD13, RD14

L00801F7F:
  MOV      RD14, RD16
  MOV.L    viewz, RD17
  SUB      RD17, RD16
  MOV.L    RD16, dc_texturemid

L00801F7D:
  MOV.Q    curline, RQ16
  MOV.Q    @(RQ16, 24), RQ12
  MOV.L    @(RQ12, 4), RD14
  MOV.L    dc_texturemid, RD17
  ADD      RD14, RD17
  MOV.L    RD17, dc_texturemid
  MOV.Q    fixedcolormap, RQ18
  MOV      #0, RQ19
  CMPQ/EQ  RQ19, RQ18
  BT       L00801F80
  MOV.Q    fixedcolormap, RQ17
  MOV      RQ17, RQ16
  MOV.Q    RQ16, dc_colormap

L00801F80:
  MOV.Q    @(R15, 72), RQ16
  MOV.L    @(RQ16, 28), RD17
  MOV      #1, RD14
  MOV      RD14, RD18
  MOV.L    @(R15, 68), RD20
  MOV      RD20, RD19
  MOV.L    RD19, dc_x
  MOV.B    RD18, dc_isspr
  MOV.L    RD17, @(R15, 36)

L00801F81:
  MOV.L    dc_x, RD16
  MOV.L    @(R15, 64), RD17
  CMP/GT   RD17, RD16
  BT       L00801F83
  MOV.Q    maskedtexturecol, RQ16
  MOV.L    dc_x, RD17
  EXTS.L   RD17
  MOV.W    @(RQ16, RD17), RD14
  CMP/EQ   #32767, RD14
  BT       L00801F84
  MOV.Q    fixedcolormap, RQ16
  MOV      #0, RQ17
  CMPQ/EQ  RQ17, RQ16
  BT       L00801F85
  MOV.L    spryscale, RD16
  SHLD     RD16, #-12, RD17
  MOV.L    RD17, @(R15, 60)
  MOV      #48, RD18
  CMP/HS   RD18, RD17
  BT       L00801F86
  MOV      #47, RD16
  MOV.L    RD16, @(R15, 60)

L00801F86:
  MOV.Q    walllights, RQ16
  MOV.L    @(R15, 60), RD17
  EXTS.L   RD17
  MOV.Q    @(RQ16, RD17), RQ18
  MOV.Q    RQ18, dc_colormap

L00801F85:
  MOV.L    dc_texturemid, RD4
  MOV.L    spryscale, RD5
  BSR      FixedMul
  MOV      RD2, RD14
  MOV.L    centeryfrac, RD26
  SUB      RD26, RD14, RD25
  MOV.L    RD25, sprtopscreen
  MOV.L    spryscale, RD4
  BSR      M_SoftDivRcp
  MOV      R2, RD27
  MOV.L    spryscale, RD29
  MOV      RD29, RD28
  MOV.L    @(R15, 36), RD31
  MOV      RD31, RD30
  MOV.Q    maskedtexturecol, RQ8
  MOV.L    dc_x, RD9
  MOV.W    @(RQ8, RD9), RD14
  MOV.L    RD30, dc_zdist
  MOV.L    RD28, dc_scale
  MOV.L    RD27, dc_iscale
  MOV.L    @(R15, 40), RD4
  MOV      RD14, RD5
  BSR      R_GetColumn
  MOV      RQ2, RQ12
  ADD      RQ12, #-3, RQ10
  MOV      RQ10, RQ11
  MOV.Q    RQ11, @(R15, 48)
  MOV      RQ11, RQ4
  BSR      R_DrawMaskedColumn
  MOV.Q    maskedtexturecol, RQ25
  MOV.L    dc_x, RD26
  MOV      #32767, RD16
  EXTS.L   RD26
  MOV.W    RD16, @(RQ25, RD26)

L00801F84:
  MOV.L    rw_scalestep, RD16
  MOV.L    spryscale, RD17
  ADD      RD16, RD17
  MOV.L    rw_zstep, RD18
  MOV.L    @(R15, 36), RD19
  ADD      RD18, RD19
  MOV.L    RD19, @(R15, 36)
  MOV.L    RD17, spryscale

L00801F82:
  MOV.L    dc_x, RD16
  ADD      #1, RD16
  MOV.L    RD16, dc_x
  BRA      L00801F81

L00801F83:

L00E00598:
  ADD      #104, R15
  BRA      L00C00626

R_InitPlanes:
  ADD      #-144, R15

L00E00599:
  ADD      #144, R15
  RTS     

L00C00769:
  PUSH     R31
  PUSH     R30
  PUSH     R29
  PUSH     R28
  PUSH     R27
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  PUSH     R9
  PUSH     R8
  RTS     

R_ClearPlanes:
  PUSH     LR
  BSR      L00C00769
  ADD      #-56, R15
  MOV      #0, RD14

L00801EF7:
  MOV.L    viewwidth, RD11
  CMP/GT   RD14, RD11
  BT       L00801EF9
  MOV      floorclip, RQ10
  MOV.L    viewheight, RD9
  EXTS.L   RD14
  MOV.W    RD9, @(RQ10, RD14)
  MOV      ceilingclip, RQ8
  MOV      #-1, RD31
  EXTS.L   RD14
  MOV.W    RD31, @(RQ8, RD14)

L00801EF8:
  ADD      #1, RD14
  BRA      L00801EF7

L00801EF9:
  MOV      visplanes, RQ29
  MOV      RQ29, RQ30
  MOV      openings, RQ27
  MOV      RQ27, RQ28
  MOV      #800, RQ31
  EXTS.L   RQ31, RQ31
  MOV.Q    RQ31, @(R15, 8)
  MOV.Q    RQ30, lastvisplane
  MOV.Q    RQ28, lastopening
  MOV      cachedheight, RQ4
  MOV      #0, RD5
  MOV      RQ31, RQ6
  BSR      memset
  MOV.L    viewangle, RD8
  SUB      RD8, #1073741824, RD13
  SHLD     RD13, #-19, RD12
  MOV.Q    finecosine, RQ9
  MOV.L    @(RQ9, RD12), RD13
  MOV      RD13, RD4
  MOV.L    centerxfrac, RD5
  BSR      FixedDiv
  MOV      R2, RD10
  MOV      finesine, RQ11
  MOV.L    @(RQ11, RD12), RD13
  MOV.L    RD10, basexscale
  MOV      RD13, RD4
  MOV.L    centerxfrac, RD5
  BSR      FixedDiv
  MOV      RD2, RD27
  NEG      RD27, RD28
  MOV.L    RD28, baseyscale
  MOV.L    RD27, @R15

L00E0059A:
  ADD      #56, R15

L00C00770:
  POP      RQ8
  POP      RQ9
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  POP      RQ27
  POP      RQ28
  POP      RQ29
  POP      RQ30
  POP      RQ31
  RET     

R_MapPlane:
  PUSH     LR
  BSR      L00C00630
  ADD      #-72, R15
  MOV.L    RD4, @(R15, 44)
  MOV.L    RD5, @(R15, 40)
  MOV.L    RD6, @(R15, 36)
  MOV.L    @(R15, 36), RD12
  MOV.L    @(R15, 40), RD11
  CMP/GT   RD12, RD11
  BT       L00801EEE
  MOV.L    @(R15, 40), RD10
  CMP/PZ   RD10
  BT       L00801EEE
  BRA      L00801EF0

L00801EF0:
  MOV.L    @(R15, 36), RD9
  MOV.L    viewwidth, RD8
  CMP/GT   RD9, RD8
  BT       L00801EEE
  BRA      L00801EEF

L00801EEF:
  MOV.L    @(R15, 44), RD31
  MOV      RD31, RD14
  MOV.L    viewheight, RD30
  MOV      RD30, RD13
  CMP/HI   RD13, RD14
  BT       L00801EEE
  BRA      L00801EED

L00801EEE:
  MOV      L00C00772, RQ4
  MOV.L    @(R15, 40), RD5
  MOV.L    @(R15, 36), RD6
  MOV.L    @(R15, 44), RD7
  BSR      I_Error

L00801EED:
  MOV      cachedheight, RQ29
  MOV.L    @(R15, 44), RD28
  EXTS.L   RD28
  MOV.L    @(RQ29, RD28), RD14
  MOV.L    planeheight, RD27
  CMP/EQ   RD14, RD27
  BT       L00801EF2
  MOV      cachedheight, RQ26
  MOV.L    @(R15, 44), RD25
  MOV.L    planeheight, RD27
  EXTS.L   RD25
  MOV.L    RD27, @(RQ26, RD25)
  MOV      yslope, RQ28
  EXTS.L   RD25
  MOV.L    @(RQ28, RD25), RD14
  MOV.L    planeheight, RD4
  MOV      RD14, RD5
  BSR      FixedMul
  MOV      RD2, RD13
  MOV      cacheddistance, RQ29
  EXTS.L   RD25
  MOV.L    RD13, @(RQ29, RD25)
  MOV      RD13, RD30
  MOV.L    RD30, @(R15, 28)
  MOV      RD30, RD4
  MOV.L    basexscale, RD5
  BSR      FixedMul
  MOV      RD2, RD14
  MOV      cachedxstep, RQ31
  EXTS.L   RD25
  MOV.L    RD14, @(RQ31, RD25)
  MOV      RD14, RD8
  MOV.L    RD8, ds_xstep
  MOV      RD30, RD4
  MOV.L    baseyscale, RD5
  BSR      FixedMul
  MOV      RD2, RD14
  MOV      cachedystep, RQ9
  EXTS.L   RD25
  MOV.L    RD14, @(RQ9, RD25)
  MOV      RD14, RD10
  MOV.L    RD10, ds_ystep
  BRA      L00801EF3

L00801EF2:
  MOV      cacheddistance, RQ7
  MOV.L    @(R15, 44), RD6
  MOV.L    @(RQ7, RD6), RD5
  MOV      cachedxstep, RQ4
  MOV.L    @(RQ4, RD6), RD3
  MOV      cachedystep, RQ2
  MOV.L    @(RQ2, RD6), RD23
  MOV.L    RD5, @(R15, 28)
  MOV.L    RD3, ds_xstep
  MOV.L    RD23, ds_ystep

L00801EF3:
  MOV      distscale, RQ25
  MOV.L    @(R15, 40), RD26
  EXTS.L   RD26
  MOV.L    @(RQ25, RD26), RD14
  MOV.L    @(R15, 28), RD4
  MOV      RD14, RD5
  BSR      FixedMul
  MOV      RD2, RD27
  MOV      xtoviewangle, RQ28
  EXTS.L   RD26
  MOVU.L   @(RQ28, RD26), RD14
  MOV.L    viewangle, RD29
  ADD      RD29, RD14, RD13
  SHLD     RD13, #-19, RD30
  MOV.Q    finecosine, RQ31
  MOV.L    @(RQ31, RD30), RD14
  MOV.L    RD30, @(R15, 32)
  MOV.L    RD27, @(R15, 24)
  MOV      RD14, RD4
  MOV      RD27, RD5
  BSR      FixedMul
  MOV      RD2, RD13
  MOV.L    viewx, RD9
  ADD      RD9, RD13, RD8
  MOV.L    viewy, RD10
  NEG      RD10, RD14
  MOV      finesine, RQ11
  EXTS.L   RD30
  MOV.L    @(RQ11, RD30), RD13
  MOV.L    RD8, ds_xfrac
  MOV      RD13, RD4
  MOV      RD27, RD5
  BSR      FixedMul
  MOV      RD2, RD12
  SUB      RD14, RD12, RD25
  MOV.L    RD12, @(R15, 8)
  MOV.L    RD25, ds_yfrac
  MOV.Q    fixedcolormap, RQ28
  MOV      #0, RQ29
  CMPQ/EQ  RQ29, RQ28
  BT       L00801EF4
  MOV.Q    fixedcolormap, RQ21
  MOV      RQ21, RQ22
  MOV.Q    RQ22, ds_colormap
  BRA      L00801EF5

L00801EF4:
  MOV.L    @(R15, 28), RD20
  SHLD     RD20, #-20, RD19
  MOV.L    RD19, @(R15, 20)
  MOV      #128, RD18
  CMP/HS   RD18, RD19
  BT       L00801EF6
  MOV      #127, RD17
  MOV.L    RD17, @(R15, 20)

L00801EF6:
  MOV.Q    planezlight, RQ16
  MOV.L    @(R15, 20), RD17
  EXTS.L   RD17
  MOV.Q    @(RQ16, RD17), RQ18
  MOV.Q    RQ18, ds_colormap

L00801EF5:
  MOV.L    @(R15, 44), RD26
  MOV      RD26, RD25
  MOV.L    @(R15, 40), RD28
  MOV      RD28, RD27
  MOV.L    @(R15, 36), RD30
  MOV      RD30, RD29
  MOV.L    @(R15, 28), RD8
  MOV      RD8, RD31
  MOV.L    RD31, ds_z
  MOV.L    RD29, ds_x2
  MOV.L    RD27, ds_x1
  MOV.L    RD25, ds_y
  MOV.Q    spanfunc, RQ9
  JSR      RQ9

L00E0059B:
  ADD      #72, R15
  BRA      L00C00626

R_MakeSpans:
  PUSH     LR
  BSR      L00C00630
  ADD      #-56, R15
  MOV.L    RD4, @(R15, 28)
  MOV      RD5, RD14
  MOV      RD6, RD11
  MOV      RD7, RD10
  MOV      RD20, RD9

L00801F0D:
  CMP/GT   RD14, RD10
  BT       L00801F0E
  CMP/GT   RD11, RD14
  BT       L00801F0E
  BRA      L00801F0F

L00801F0F:
  MOV.L    @(R15, 28), RD8
  SUB      RD8, #1, RD12
  MOV      spanstart, RQ31
  EXTS.L   RD14
  MOV.L    @(RQ31, RD14), RD13
  MOV      RD14, RD4
  MOV      RD13, RD5
  MOV      RD12, RD6
  BSR      R_MapPlane
  ADD      #1, RD14
  BRA      L00801F0D

L00801F0E:

L00801F11:
  CMP/GT   RD9, RD11
  BT       L00801F12
  CMP/GT   RD11, RD14
  BT       L00801F12
  BRA      L00801F13

L00801F13:
  MOV.L    @(R15, 28), RD30
  SUB      RD30, #1, RD12
  MOV      spanstart, RQ29
  EXTS.L   RD11
  MOV.L    @(RQ29, RD11), RD13
  MOV      RD11, RD4
  MOV      RD13, RD5
  MOV      RD12, RD6
  BSR      R_MapPlane
  ADD      #-1, RD11
  BRA      L00801F11

L00801F12:

L00801F15:
  CMP/GT   RD10, RD14
  BT       L00801F16
  CMP/GT   RD9, RD10
  BT       L00801F16
  BRA      L00801F17

L00801F17:
  MOV      spanstart, RQ28
  MOV.L    @(R15, 28), RD27
  EXTS.L   RD10
  MOV.L    RD27, @(RQ28, RD10)
  ADD      #1, RD10
  BRA      L00801F15

L00801F16:

L00801F19:
  CMP/GT   RD11, RD9
  BT       L00801F1A
  CMP/GT   RD9, RD10
  BT       L00801F1A
  BRA      L00801F1B

L00801F1B:
  MOV      spanstart, RQ26
  MOV.L    @(R15, 28), RD25
  EXTS.L   RD9
  MOV.L    RD25, @(RQ26, RD9)
  ADD      #-1, RD9
  BRA      L00801F19

L00801F1A:

L00E0059C:
  ADD      #56, R15
  BRA      L00C00626

R_DrawPlanes:
  PUSH     LR
  BSR      L00C00630
  ADD      #-72, R15
  MOV.Q    ds_p, RQ9
  MOV      drawsegs, RQ8
  MOV      RQ9, RD14
  SUB      RQ8, RD14
  MOV      #0x03333334, RD7
  MOV      #819, RD7
  DMULS    RD7, RD14
  STS      R1, RD14
  CMP/GT   #2048, RD14
  BT       L00801F1D
  MOV.Q    ds_p, RQ31
  MOV      drawsegs, RQ30
  MOV      RQ31, RD14
  SUB      RQ30, RD14
  MOV      #0x03333334, RD7
  MOV      #819, RD7
  DMULS    RD7, RD14
  STS      R1, RD14
  MOV      L00C00783, RQ4
  MOV      RD14, RD5
  BSR      I_Error

L00801F1D:
  MOV.Q    lastvisplane, RQ29
  MOV      visplanes, RQ28
  MOV      RQ29, RD14
  SUB      RQ28, RD14
  MOV      #0x0062B2E5, RD7
  MOV      #98, RD7
  DMULS    RD7, RD14
  STS      R1, RD14
  CMP/GT   #512, RD14
  BT       L00801F1E
  MOV.Q    lastvisplane, RQ27
  MOV      visplanes, RQ26
  MOV      RQ27, RD14
  SUB      RQ26, RD14
  MOV      #0x0062B2E5, RD7
  MOV      #98, RD7
  DMULS    RD7, RD14
  STS      R1, RD14
  MOV      L00C00784, RQ4
  MOV      RD14, RD5
  BSR      I_Error

L00801F1E:
  MOV.Q    lastopening, RQ25
  MOV      openings, RQ7
  SUB      RQ25, RQ7, RD14
  SHAD.Q   #-1, RD14
  CMP/GT   #81920, RD14
  BT       L00801F1F
  MOV.Q    lastopening, RQ25
  MOV      openings, RQ26
  SUB      RQ25, RQ26, RD14
  SHAD.Q   #-1, RD14
  MOV      L00C00785, RQ4
  MOV      RD14, RD5
  BSR      I_Error

L00801F1F:
  MOV      visplanes, RQ6
  MOV      RQ6, RQ13

L00801F20:
  MOV.Q    lastvisplane, RQ5
  CMPQ/GT  RQ13, RQ5
  BT       L00801F22
  MOV.L    @(RQ13, 12), RD14
  MOV.L    @(RQ13, 16), RD11
  CMP/GT   RD11, RD14
  BT       L00801F23
  BRA      L00801F21

L00801F23:
  MOV.L    @(RQ13, 4), RD14
  MOV.L    skyflatnum, RD4
  CMP/EQ   RD4, RD14
  BT       L00801F24
  MOV.L    pspriteiscale, RD2
  MOV.L    detailshift, RD23
  SHAR     RD2, RD23, RD3
  MOV      #0x7FFFFFFF, RD22
  MOV      #32767, RD22
  MOV      #1, RD14
  MOV      RD14, RD21
  MOV.Q    colormaps, RQ19
  MOV      RQ19, RQ20
  MOV.L    skytexturemid, RD17
  MOV      RD17, RD18
  MOV.L    @(RQ13, 12), RD12
  MOV.L    RD3, dc_iscale
  MOV.L    RD22, dc_scale
  MOV.B    RD21, dc_isspr
  MOV.Q    RQ20, dc_colormap
  MOV.L    RD18, dc_texturemid

L00801F25:
  MOV.L    @(RQ13, 16), RD14
  CMP/GT   RD14, RD12
  BT       L00801F27
  ADD      RQ13, #21, RQ16
  EXTS.L   RD12
  MOVU.B   @(RQ16, RD12), RD17
  ADD      RQ13, #343, RQ16
  EXTS.L   RD12
  MOVU.B   @(RQ16, RD12), RD18
  MOV.L    RD18, dc_yh
  MOV.L    RD17, dc_yl
  MOV.Q    RQ16, @(R15, 24)
  MOV.L    dc_yl, RD17
  MOV.L    dc_yh, RD18
  CMP/GT   RD18, RD17
  BT       L00801F28
  MOV      xtoviewangle, RQ25
  EXTS.L   RD12
  MOVU.L   @(RQ25, RD12), RD14
  MOV.L    viewangle, RD26
  ADD      RD26, RD14, RD11
  SHLD     RD11, #-22, RD27
  MOV      RD12, RD28
  MOV.L    RD28, dc_x
  MOV.L    RD27, @(R15, 40)
  MOV.L    skytexture, RD4
  MOV      RD27, RD5
  BSR      R_GetColumn
  MOV      RQ2, RQ29
  MOV.Q    RQ29, dc_source
  MOV.Q    colfunc, RQ30
  MOV      RQ30, R1
  JSR      R1

L00801F28:

L00801F26:
  ADD      #1, RD12
  BRA      L00801F25

L00801F27:
  BRA      L00801F21

L00801F24:
  MOV.L    @(RQ13, 4), RD14
  MOV.Q    flattranslation, RQ25
  MOV.L    @(RQ25, RD14), RD11
  MOV.L    firstflat, RD26
  ADD      RD26, RD11, RD14
  MOV      RD14, RD4
  MOV      #1, RD5
  BSR      W_CacheLumpNum
  MOV      RQ2, RQ27
  MOV.L    @RQ13, RD14
  MOV.L    viewz, RD28
  SUB      RD14, RD28, RD11
  MOV.Q    RQ27, ds_source
  MOV      RD11, RD4
  BSR      abs
  MOV      R2, RD29
  MOV.L    @(RQ13, 8), RD14
  SHAD     RD14, #-4, RD11
  MOV.L    extralight, RD30
  ADD      RD11, RD30, RD10
  MOV.L    RD29, planeheight
  CMP/GE   #16, RD10
  BT       L00801F29
  MOV      #15, RD10

L00801F29:
  CMP/PZ   RD10
  BT       L00801F2A
  MOV      #0, RD10

L00801F2A:
  MOV      zlight, RQ16
  EXTS.L   RD10
  SHAD     RD10, #10, RQ17
  ADD      RQ16, RQ17
  ADD      RQ13, #21, RQ18
  MOV.L    @(RQ13, 16), RD14
  ADD      RD14, #1, RD11
  MOV      #255, RD19
  EXTS.L   RD11
  MOVU.B   RD19, @(RQ18, RD11)
  ADD      RQ13, #21, RQ18
  MOV.L    @(RQ13, 12), RD14
  SUB      RD14, #1, RD11
  EXTS.L   RD11
  MOVU.B   RD19, @(RQ18, RD11)
  MOV.L    @(RQ13, 16), RD14
  ADD      RD14, #1, RD20
  MOV.L    @(RQ13, 12), RD12
  MOV.L    RD20, @(R15, 44)
  MOV.Q    RQ18, @(R15, 24)
  MOV.Q    RQ17, planezlight

L00801F2B:
  MOV.L    @(R15, 44), RD16
  CMP/GT   RD16, RD12
  BT       L00801F2D
  ADD      RQ13, #343, RQ25
  EXTS.L   RD12
  MOVU.B   @(RQ25, RD12), RD14
  ADD      RQ13, #21, RQ25
  EXTS.L   RD12
  MOVU.B   @(RQ25, RD12), RD11
  ADD      RQ13, #343, RQ25
  SUB      RD12, #1, RD26
  EXTS.L   RD26
  MOVU.B   @(RQ25, RD26), RD27
  ADD      RQ13, #21, RQ25
  SUB      RD12, #1, RD26
  EXTS.L   RD26
  MOVU.B   @(RQ25, RD26), RD28
  MOV.L    RD28, @(R15, 12)
  MOV.L    RD27, @(R15, 16)
  MOV.Q    RQ25, @(R15, 24)
  MOV      RD12, RD4
  MOV      RD28, RD5
  MOV      RD27, RD6
  MOV      RD11, RD7
  MOV      RD14, RD20
  BSR      R_MakeSpans

L00801F2C:
  ADD      #1, RD12
  BRA      L00801F2B

L00801F2D:
  MOV.Q    ds_source, RQ16
  MOV      #0, RQ17
  CMPQ/EQ  RQ17, RQ16
  BT       L00801F2E
  MOV.Q    ds_source, RQ17
  MOV      RQ17, RQ16
  ADD      RQ16, #-40, RQ18
  MOV.L    @(RQ18, 20), RD14
  MOV.Q    RQ16, @(R15, 24)
  CMP/EQ   #1919505, RD14
  BT       L00801F2F
  MOV      L00C0078C, RQ4
  MOV      #457, RD5
  BSR      I_Error

L00801F2F:
  MOV.Q    ds_source, RQ4
  MOV      #101, RD5
  BSR      Z_ChangeTag2

L00801F2E:

L00801F21:
  ADD      #664, RQ13
  BRA      L00801F20

L00801F22:

L00E0059D:
  ADD      #72, R15
  BRA      L00C00626

R_FindPlane:
  PUSH     LR
  BSR      L00C00630
  ADD      #-376, R15
  MOV.L    RD4, @(R15, 28)
  MOV.L    RD5, @(R15, 24)
  MOV.L    RD6, @(R15, 20)
  MOV.L    @(R15, 24), RD12
  MOV.L    skyflatnum, RD11
  CMP/EQ   RD11, RD12
  BT       L00801EFA
  MOV      #0, RD10
  MOV      #0, RD9
  MOV.L    RD10, @(R15, 28)
  MOV.L    RD9, @(R15, 20)

L00801EFA:
  MOV      visplanes, RQ8
  MOV      RQ8, RQ14

L00801EFB:
  MOV.Q    lastvisplane, RQ31
  CMPQ/GT  RQ14, RQ31
  BT       L00801EFD
  MOV.L    @RQ14, RD13
  MOV.L    @(R15, 28), RD30
  CMP/EQ   RD13, RD30
  BT       L00801EFE
  MOV.L    @(RQ14, 4), RD13
  MOV.L    @(R15, 24), RD29
  CMP/EQ   RD13, RD29
  BT       L00801EFE
  BRA      L00801F00

L00801F00:
  MOV.L    @(RQ14, 8), RD13
  MOV.L    @(R15, 20), RD28
  CMP/EQ   RD13, RD28
  BT       L00801EFE
  BRA      L00801EFF

L00801EFF:
  BRA      L00801EFD

L00801EFE:

L00801EFC:
  ADD      #664, RQ14
  BRA      L00801EFB

L00801EFD:
  MOV.Q    lastvisplane, RQ27
  CMPQ/GT  RQ14, RQ27
  BT       L00801F02
  MOV      RQ14, RQ2
  BRA      L00E0059E

L00801F02:
  MOV.Q    lastvisplane, RQ26
  MOV      visplanes, RQ25
  MOV      RQ26, RD13
  SUB      RQ25, RD13
  MOV      #0x0062B2E5, RD7
  MOV      #98, RD7
  DMULS    RD7, RD13
  STS      R1, RD13
  CMP/EQ   #512, RD13
  BT       L00801F03
  MOV      L00C0078E, RQ4
  BSR      I_Error

L00801F03:
  MOV.Q    lastvisplane, RQ25
  ADD      #664, RQ25
  MOV.L    @(R15, 28), RD26
  MOV.L    RD26, @RQ14
  MOV.L    @(R15, 24), RD27
  MOV.L    RD27, @(RQ14, 4)
  MOV.L    @(R15, 20), RD28
  MOV.L    RD28, @(RQ14, 8)
  MOV      #320, RD29
  MOV.L    RD29, @(RQ14, 12)
  MOV      #-1, RD30
  MOV.L    RD30, @(RQ14, 16)
  ADD      RQ14, #21, RQ31
  EXTS.L   RD29, RQ8
  MOV.Q    RQ8, @R15
  MOV.Q    RQ31, @(R15, 8)
  MOV.Q    RQ25, lastvisplane
  MOV      RQ31, RQ4
  MOV      #255, RD5
  MOV      RQ8, RQ6
  BSR      memset
  MOV      RQ14, RQ2

L00E0059E:
  ADD      #376, R15
  BRA      L00C00626

R_CheckPlane:
  PUSH     LR
  BSR      L00C00630
  ADD      #-392, R15
  MOV      RQ4, RQ13
  MOV      RD5, RD10
  MOV.L    RD6, @(R15, 32)
  MOV.L    @(RQ13, 12), RD14
  CMP/GT   RD10, RD14
  BT       L00801F04
  MOV.L    @(RQ13, 12), RD9
  MOV      RD10, RD8
  BRA      L00801F05

L00801F04:
  MOV.L    @(RQ13, 12), RD8
  MOV      RD10, RD9

L00801F05:
  MOV.L    @(RQ13, 16), RD14
  MOV.L    @(R15, 32), RD31
  CMP/GT   RD14, RD31
  BT       L00801F06
  MOV.L    @(RQ13, 16), RD11
  MOV.L    @(R15, 32), RD29
  MOV      RD29, RD30
  MOV.L    RD30, @(R15, 28)
  BRA      L00801F07

L00801F06:
  MOV.L    @(RQ13, 16), RD28
  MOV.L    @(R15, 32), RD27
  MOV      RD27, RD11
  MOV.L    RD28, @(R15, 28)

L00801F07:
  MOV      RD9, RD12

L00801F08:
  CMP/GT   RD11, RD12
  BT       L00801F0A
  ADD      RQ13, #21, RQ26
  EXTS.L   RD12
  MOVU.B   @(RQ26, RD12), RD14
  MOV.Q    RQ26, @(R15, 16)
  CMP/EQ   #255, RD14
  BT       L00801F0B
  BRA      L00801F0A

L00801F0B:

L00801F09:
  ADD      #1, RD12
  BRA      L00801F08

L00801F0A:
  CMP/GT   RD11, RD12
  BT       L00801F0C
  MOV.L    RD8, @(RQ13, 12)
  MOV.L    @(R15, 28), RD25
  MOV.L    RD25, @(RQ13, 16)
  MOV      RQ13, RQ2
  BRA      L00E0059F

L00801F0C:
  MOV.L    @RQ13, RD14
  MOV.Q    lastvisplane, RQ25
  MOV.L    RD14, @RQ25
  MOV.L    @(RQ13, 4), RD14
  MOV.L    RD14, @(RQ25, 4)
  MOV.L    @(RQ13, 8), RD14
  MOV.L    RD14, @(RQ25, 8)
  MOV      RQ25, RQ26
  ADD      #664, RQ25
  MOV      RQ26, RQ13
  MOV.L    RD10, @(RQ13, 12)
  MOV.L    @(R15, 32), RD27
  MOV.L    RD27, @(RQ13, 16)
  ADD      RQ13, #21, RQ26
  MOV      #320, RQ28
  EXTS.L   RQ28, RQ28
  MOV.Q    RQ28, @(R15, 8)
  MOV.Q    RQ26, @(R15, 16)
  MOV.Q    RQ25, lastvisplane
  MOV      RQ26, RQ4
  MOV      #255, RD5
  MOV      RQ28, RQ6
  BSR      memset
  MOV      RQ13, RQ2

L00E0059F:
  ADD      #392, R15
  BRA      L00C00626

R_DrawMaskedColumn:
  PUSH     LR
  BSR      L00C00630
  ADD      #-72, R15
  MOV      RQ4, RQ11
  MOV.L    dc_texturemid, RD9
  MOV      RD9, RD12

L00802026:
  MOV.B    @RQ11, RD14
  EXTU.B   RD14, RD14
  CMP/EQ   #255, RD14
  BT       L00802028
  MOV.B    @RQ11, RD14
  EXTU.B   RD14, RD14
  MOV.L    spryscale, RD8
  MUL.L    RD8, RD14, RD13
  MOV.L    sprtopscreen, RD31
  ADD      RD31, RD13, RD10
  MOV.B    @(RQ11, 1), RD14
  EXTU.B   RD14, RD14
  MUL.L    RD8, RD14, RD13
  ADD      RD10, RD13, RD30
  ADD      RD10, #65536, RD14
  SUB      RD14, #1, RD13
  SHAD     RD13, #-16, RD29
  SUB      RD30, #1, RD14
  SHAD     RD14, #-16, RD28
  MOV.Q    mfloorclip, RQ27
  MOV.L    dc_x, RD26
  EXTS.L   RD26
  MOV.W    @(RQ27, RD26), RD14
  MOV.L    RD30, @(R15, 36)
  MOV.L    RD29, dc_yl
  MOV.L    RD28, dc_yh
  MOV.L    dc_yh, RD25
  CMP/GT   RD25, RD14
  BT       L00802029
  MOV.Q    mfloorclip, RQ7
  MOV.L    dc_x, RD6
  MOV.W    @(RQ7, RD6), RD14
  SUB      RD14, #1, RD5
  MOV.L    RD5, dc_yh

L00802029:
  MOV.Q    mceilingclip, RQ4
  MOV.L    dc_x, RD3
  MOV.W    @(RQ4, RD3), RD14
  MOV.L    dc_yl, RD2
  CMP/GT   RD14, RD2
  BT       L0080202A
  MOV.Q    mceilingclip, RQ23
  MOV.L    dc_x, RD22
  EXTS.L   RD22
  MOV.W    @(RQ23, RD22), RD14
  ADD      RD14, #1, RD21
  MOV.L    RD21, dc_yl

L0080202A:
  MOV.L    dc_yl, RD20
  MOV.L    dc_yh, RD19
  CMP/GT   RD19, RD20
  BT       L0080202B
  ADD      RQ11, #3, RQ25
  MOV.B    @RQ11, RD14
  EXTU.B   RD14, RD14
  SHAD     RD14, #16, RD13
  SUB      RD12, RD13, RD26
  MOV.L    RD26, dc_texturemid
  MOV.Q    RQ25, dc_source
  MOV.Q    colfunc, RQ25
  MOV      RQ25, R1
  JSR      R1

L0080202B:
  MOV.B    @(RQ11, 1), RD14
  EXTU.B   RD14, RD14
  LEA.B    @(RQ11, RD14), RQ18
  ADD      RQ18, #4, RQ17
  MOV      RQ17, RQ11

L00802027:
  BRA      L00802026

L00802028:
  MOV      RD12, RD16
  MOV.L    RD16, dc_texturemid

L00E005A0:
  ADD      #72, R15
  BRA      L00C00626

R_SortVisSprites:
  PUSH     LR
  BSR      L00C00630
  ADD      #-248, R15
  MOV      #64, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 40)
  MOV.Q    vissprite_p, RQ12
  MOV      vissprites, RQ11
  MOV      RQ12, RD10
  SUB      RQ11, RD10
  MOV      #0x03333334, RD7
  MOV      #819, RD7
  DMULS    RD7, RD10
  STS      R1, RD10
  MOV      #64, RQ9
  ADD      R15, RQ9
  MOV      RQ9, RQ14
  MOV.Q    RQ14, @RQ9
  MOV.Q    RQ14, @(RQ9, 8)
  MOV.L    RD10, @(R15, 56)
  CMP/EQ   RD10
  BT       L00802080
  BRA      L00E005A1

L00802080:
  MOV      vissprites, RQ8
  MOV      RQ8, RQ13

L00802081:
  MOV.Q    vissprite_p, RQ31
  CMPQ/GT  RQ13, RQ31
  BT       L00802083
  ADD      RQ13, #80, RQ14
  MOV.Q    RQ14, @(RQ13, 8)
  ADD      RQ13, #-80, RQ14
  MOV.Q    RQ14, @RQ13

L00802082:
  ADD      #80, RQ13
  BRA      L00802081

L00802083:
  MOV      #64, RQ30
  ADD      R15, RQ30
  MOV      RQ30, RQ14
  MOV      vissprites, RQ29
  MOV      RQ29, RQ28
  MOV.Q    RQ14, @RQ28
  MOV      #0, RD27
  EXTS.L   RD27
  MOV      #80, RD7
  MUL.L    RD27, RD7
  STS      R0, RQ14
  ADD      RQ29, RQ14
  MOV.Q    RQ14, @(RQ30, 8)
  MOV      RQ30, RQ14
  MOV.Q    vissprite_p, RQ26
  ADD      RQ26, #-80, RQ25
  MOV.Q    RQ14, @(RQ25, 8)
  ADD      RQ26, #-80, RQ14
  MOV.Q    RQ14, @RQ30
  MOV      vsprsortedhead, RQ31
  MOV      RQ31, RQ14
  MOV.Q    RQ14, @RQ31
  MOV.Q    RQ14, @(RQ31, 8)
  MOV      #0, RD8
  MOV.L    RD8, @(R15, 60)
  MOV.Q    RQ25, @(R15, 8)

L00802084:
  MOV.L    @(R15, 60), RD7
  MOV.L    @(R15, 56), RD6
  CMP/GT   RD7, RD6
  BT       L00802086
  MOV      #0x7FFFFFFF, RD25
  MOV      #32767, RD25
  MOV      #64, RQ26
  ADD      R15, RQ26
  MOV.Q    @(RQ26, 8), RQ13
  MOV.L    RD25, @(R15, 36)

L00802087:
  MOV      #64, RQ5
  ADD      R15, RQ5
  MOV      RQ5, RQ14
  CMPQ/EQ  RQ14, RQ13
  BT       L00802089
  MOV.L    @(RQ13, 48), RD4
  MOV.L    RD4, @(R15, 4)
  MOV.L    @(R15, 36), RD3
  CMP/GT   RD4, RD3
  BT       L0080208A
  MOV.L    @(RQ13, 48), RD2
  MOV      RQ13, RQ23
  MOV.L    RD2, @(R15, 36)
  MOV.Q    RQ23, @(R15, 48)

L0080208A:

L00802088:
  MOV.Q    @(RQ13, 8), RQ13
  BRA      L00802087

L00802089:
  MOV.Q    @(R15, 48), RQ22
  MOV.Q    @RQ22, RQ14
  MOV.Q    @(RQ22, 8), RQ21
  MOV.Q    RQ14, @RQ21
  MOV.Q    @(RQ22, 8), RQ14
  MOV.Q    @RQ22, RQ21
  MOV.Q    RQ14, @(RQ21, 8)
  MOV      vsprsortedhead, RQ20
  MOV      RQ20, RQ14
  MOV.Q    RQ14, @(RQ22, 8)
  MOV.Q    @RQ20, RQ14
  MOV.Q    RQ14, @RQ22
  MOV.Q    @RQ20, RQ14
  MOV.Q    RQ22, @(RQ14, 8)
  MOV.Q    RQ22, @RQ20
  MOV.Q    RQ21, @(R15, 8)

L00802085:
  MOV.L    @(R15, 60), RD19
  ADD      #1, RD19
  MOV.L    RD19, @(R15, 60)
  BRA      L00802084

L00802086:

L00E005A1:
  ADD      #248, R15
  BRA      L00C00626

R_AddSprites:
  PUSH     LR
  BSR      L00C00630
  ADD      #-40, R15
  MOV      RQ4, RQ11
  MOV.L    @(RQ11, 88), RD12
  MOV.L    validcount, RD10
  CMP/EQ   RD10, RD12
  BT       L00802055
  BRA      L00E005A2

L00802055:
  MOV.L    validcount, RD9
  MOV.L    RD9, @(RQ11, 88)
  MOV.W    @(RQ11, 12), RD12
  SHAD     RD12, #-4, RD8
  MOV.L    extralight, RD31
  ADD      RD8, RD31, RD14
  CMP/PZ   RD14
  BT       L00802056
  MOV      scalelight, RQ30
  MOV      RQ30, RQ29
  MOV      #0, RD28
  MOV.Q    RQ29, spritelights
  MOV.L    RD28, lightlevel
  BRA      L00802057

L00802056:
  CMP/GE   #16, RD14
  BT       L00802058
  MOV      scalelight, RQ27
  ADD      RQ27, #5760, RQ26
  MOV      #15, RD25
  MOV.Q    RQ26, spritelights
  MOV.L    RD25, lightlevel
  BRA      L00802059

L00802058:
  MOV      scalelight, RQ7
  EXTS.L   RD14
  MOV      RD14, RQ6
  SHLL1    RQ6
  ADD      RD14, RQ6
  SHAD     #7, RQ6
  ADD      RQ7, RQ6
  MOV      RD14, RD5
  MOV.Q    RQ6, spritelights
  MOV.L    RD5, lightlevel

L00802059:

L00802057:
  MOV.Q    @(RQ11, 96), RQ13

L0080205A:
  MOV      #0, RQ4
  CMPQ/EQ  RQ4, RQ13
  BT       L0080205C
  MOV      RQ13, RQ4
  BSR      R_ProjectSprite

L0080205B:
  MOV.Q    @(RQ13, 40), RQ13
  BRA      L0080205A

L0080205C:

L00E005A2:
  ADD      #40, R15
  BRA      L00C00626

R_InitSprites:
  PUSH     LR
  BSR      L00C006DA
  ADD      #-128, R15
  MOV.Q    RQ4, @(R15, 8)
  MOV      #0, RD14

L00802022:
  CMP/GE   #320, RD14
  BT       L00802024
  MOV      negonearray, RQ13
  MOV      #-1, RD12
  EXTS.L   RD14
  MOV.W    RD12, @(RQ13, RD14)

L00802023:
  ADD      #1, RD14
  BRA      L00802022

L00802024:
  MOV.Q    @(R15, 8), RQ4
  BSR      R_InitSpriteDefs

L00E005A3:
  ADD      #128, R15
  BRA      L00C006E6

R_ClearSprites:
  PUSH     R14
  PUSH     R13
  ADD      #-128, R15
  MOV      vissprites, RQ13
  MOV      RQ13, RQ14
  MOV.Q    RQ14, vissprite_p

L00E005A4:
  ADD      #128, R15
  POP      RQ13
  POP      RQ14
  RTS     

R_DrawMasked:
  PUSH     LR
  BSR      L00C00630
  ADD      #-56, R15
  BSR      R_SortVisSprites
  MOV.Q    vissprite_p, RQ10
  MOV      RQ10, RQ14
  MOV      vissprites, RQ9
  MOV      RQ9, RQ11
  CMPQ/GT  RQ11, RQ14
  BT       L008020BA
  MOV      vsprsortedhead, RQ8
  MOV.Q    @(RQ8, 8), RQ12

L008020BB:
  MOV      vsprsortedhead, RQ31
  MOV      RQ31, RQ14
  CMPQ/EQ  RQ14, RQ12
  BT       L008020BD
  MOV      RQ12, RQ4
  BSR      R_DrawSprite

L008020BC:
  MOV.Q    @(RQ12, 8), RQ12
  BRA      L008020BB

L008020BD:

L008020BA:
  MOV.Q    ds_p, RQ30
  ADD      RQ30, #-80, RQ13

L008020BE:
  MOV      RQ13, RQ14
  MOV      drawsegs, RQ29
  MOV      RQ29, RQ11
  CMPQ/GT  RQ14, RQ11
  BT       L008020C0
  MOV.Q    @(RQ13, 72), RQ14
  MOV      #0, RQ28
  CMPQ/EQ  RQ28, RQ14
  BT       L008020C1
  MOV.L    @(RQ13, 12), RD27
  MOV.L    @(RQ13, 8), RD26
  MOV.L    RD27, @(R15, 12)
  MOV.L    RD26, @(R15, 8)
  MOV      RQ13, RQ4
  MOV      RD26, RD5
  MOV      RD27, RD6
  BSR      R_RenderMaskedSegRange

L008020C1:

L008020BF:
  ADD      #-80, RQ13
  BRA      L008020BE

L008020C0:
  MOV.L    viewangleoffset, RD25
  CMP/EQ   #0, RD25
  BT       L008020C2
  BSR      R_DrawPlayerSprites

L008020C2:

L00E005A5:
  ADD      #56, R15
  BRA      L00C00626

r_int_clamp:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  ADD      #-136, R15
  MOV      RD4, RD14
  MOV      RD5, RD12
  MOV      RD6, RD13
  CMP/GT   RD14, RD12
  BT       L00802502
  CMP/GT   RD13, RD14
  BT       L00802503
  MOV      RD14, RD2
  BRA      L00E005A6

L00802503:
  MOV      RD13, RD2
  BRA      L00E005A6

L00802502:
  MOV      RD12, RD2

L00E005A6:
  ADD      #136, R15

L00C0079F:
  POP      RQ12
  POP      RQ13
  POP      RQ14
  RTS     

r_int_min:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  ADD      #-136, R15
  MOV      RD4, RD13
  MOV      RD5, RD12
  CMP/GT   RD12, RD13
  BT       L00802504
  MOV      RD13, RD14
  BRA      L00802505

L00802504:
  MOV      RD12, RD14

L00802505:
  MOV      RD14, RD2

L00E005A7:
  ADD      #136, R15
  BRA      L00C0079F

r_int_max:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  ADD      #-136, R15
  MOV      RD4, RD13
  MOV      RD5, RD12
  CMP/GT   RD12, RD13
  BT       L00802506
  MOV      RD13, RD14
  BRA      L00802507

L00802506:
  MOV      RD12, RD14

L00802507:
  MOV      RD14, RD2

L00E005A8:
  ADD      #136, R15
  BRA      L00C0079F

R_DrawColumn:
  PUSH     LR
  BSR      L00C00630
  ADD      #-72, R15
  MOV.L    dc_yh, RD8
  MOV.L    dc_yl, RD31
  SUB      RD8, RD31, RD14
  ADD      RD14, #1, RD9
  CMP/PL   RD9
  BT       L00801E11
  BRA      L00E005A9

L00801E11:
  MOV.Q    dc_source, RQ30
  MOV      RQ30, RQ10
  MOV      #0, RQ29
  CMPQ/EQ  RQ29, RQ10
  BT       L00801E12
  BRA      L00E005A9

L00801E12:
  MOVU.B   dc_isspr, RD28
  CMP/EQ   #0, RD28
  BT       L00801E13
  MOV      dc_czbuf_sc, RQ27
  MOV.L    dc_x, RD26
  EXTS.L   RD26
  MOV.L    @(RQ27, RD26), RD14
  MOV.L    dc_scale, RD25
  CMP/EQ   RD14, RD25
  BT       L00801E14
  MOV      dc_czbuf_yl, RQ7
  MOV.L    dc_x, RD6
  MOV.L    @(RQ7, RD6), RD14
  MOV.L    dc_yl, RD5
  CMP/GT   RD5, RD14
  BT       L00801E15
  MOV      dc_czbuf_yl, RQ4
  MOV.L    dc_x, RD3
  MOV.L    dc_yl, RD2
  MOV.L    RD2, @(RQ4, RD3)

L00801E15:
  MOV      dc_czbuf_yh, RQ23
  MOV.L    dc_x, RD22
  EXTS.L   RD22
  MOV.L    @(RQ23, RD22), RD14
  MOV.L    dc_yh, RD21
  CMP/GT   RD14, RD21
  BT       L00801E16
  MOV      dc_czbuf_yh, RQ20
  MOV.L    dc_x, RD19
  MOV.L    dc_yh, RD18
  EXTS.L   RD19
  MOV.L    RD18, @(RQ20, RD19)

L00801E16:
  BRA      L00801E17

L00801E14:
  MOV      dc_czbuf_sc, RQ17
  MOV.L    dc_x, RD16
  MOV.L    @(RQ17, RD16), RD14
  MOV.L    dc_scale, RD18
  CMP/GT   RD18, RD14
  BT       L00801E18
  MOV      dc_czbuf_yl, RQ16
  MOV.L    dc_x, RD17
  MOV.L    dc_yl, RD18
  EXTS.L   RD17
  MOV.L    RD18, @(RQ16, RD17)
  MOV      dc_czbuf_yh, RQ19
  MOV.L    dc_yh, RD20
  EXTS.L   RD17
  MOV.L    RD20, @(RQ19, RD17)
  MOV      dc_czbuf_sc, RQ21
  MOV.L    dc_scale, RD22
  EXTS.L   RD17
  MOV.L    RD22, @(RQ21, RD17)

L00801E18:

L00801E17:

L00801E13:
  MOV.Q    dc_colormap, RQ17
  MOV      RQ17, RQ16
  MOV      ylookup, RQ18
  MOV.L    dc_yl, RD19
  EXTS.L   RD19
  MOV.Q    @(RQ18, RD19), RQ20
  MOV      columnofs, RQ21
  MOV.L    dc_x, RD22
  EXTS.L   RD22
  MOV.L    @(RQ21, RD22), RD14
  LEA.W    @(RQ20, RD14), RQ11
  MOV.L    dc_iscale, RD23
  SHLD     RD23, #9, RD2
  MOV.L    centery, RD3
  SUB      RD19, RD3, RD14
  MUL.L    RD14, RD23, RD13
  MOV.L    dc_texturemid, RD4
  ADD      RD4, RD13, RD14
  SHLD     RD14, #9, RD12
  ADD      RD2, RD2, RD5
  ADD      RD5, RD2, RD6
  ADD      RD6, RD2, RD7
  MOV.L    RD7, @(R15, 24)
  MOV.L    RD6, @(R15, 28)
  MOV.L    RD5, @(R15, 32)
  MOV.L    RD2, @(R15, 36)
  MOV.Q    RQ16, @(R15, 40)

L00801E19:
  MOV      #8, RD16
  CMP/GE   RD16, RD9
  BT       L00801E1A
  SHLD     RD12, #-25, RD14
  MOVU.B   @(RQ10, RD14), RD13
  MOV.Q    @(R15, 40), RQ16
  MOVU.W   @(RQ16, RD13), RD14
  MOVU.W   RD14, @RQ11
  MOV.L    @(R15, 36), RD17
  ADD      RD12, RD17, RD14
  SHLD     RD14, #-25, RD13
  MOVU.B   @(RQ10, RD13), RD14
  MOVU.W   @(RQ16, RD14), RD13
  MOVU.W   RD13, @(RQ11, 640)
  MOV.L    @(R15, 32), RD18
  ADD      RD12, RD18, RD14
  SHLD     RD14, #-25, RD13
  MOVU.B   @(RQ10, RD13), RD14
  MOVU.W   @(RQ16, RD14), RD13
  MOVU.W   RD13, @(RQ11, 1280)
  MOV.L    @(R15, 28), RD19
  ADD      RD12, RD19, RD14
  SHLD     RD14, #-25, RD13
  MOVU.B   @(RQ10, RD13), RD14
  MOVU.W   @(RQ16, RD14), RD13
  MOVU.W   RD13, @(RQ11, 1920)
  MOV.L    @(R15, 24), RD20
  ADD      RD20, RD12
  SHLD     RD12, #-25, RD14
  MOVU.B   @(RQ10, RD14), RD13
  MOVU.W   @(RQ16, RD13), RD14
  MOVU.W   RD14, @(RQ11, 2560)
  ADD      RD12, RD17, RD14
  SHLD     RD14, #-25, RD13
  MOVU.B   @(RQ10, RD13), RD14
  MOVU.W   @(RQ16, RD14), RD13
  MOVU.W   RD13, @(RQ11, 3200)
  ADD      RD12, RD18, RD14
  SHLD     RD14, #-25, RD13
  MOVU.B   @(RQ10, RD13), RD14
  MOVU.W   @(RQ16, RD14), RD13
  MOVU.W   RD13, @(RQ11, 3840)
  ADD      RD12, RD19, RD14
  SHLD     RD14, #-25, RD13
  MOVU.B   @(RQ10, RD13), RD14
  MOVU.W   @(RQ16, RD14), RD13
  MOVU.W   RD13, @(RQ11, 4480)
  ADD      RD20, RD12
  ADD      #5120, RQ11
  SUB      #8, RD9
  BRA      L00801E19

L00801E1A:

L00801E1B:
  CMP/PL   RD9
  BT       L00801E1C
  SHLD     RD12, #-25, RD14
  MOVU.B   @(RQ10, RD14), RD13
  MOV.Q    @(R15, 40), RQ16
  MOVU.W   @(RQ16, RD13), RD14
  MOVU.W   RD14, @RQ11
  ADD      #640, RQ11
  MOV.L    @(R15, 36), RD17
  ADD      RD17, RD12
  ADD      #-1, RD9
  BRA      L00801E1B

L00801E1C:

L00E005A9:
  ADD      #72, R15
  BRA      L00C00626

R_DrawColumnLow:
  PUSH     LR
  BSR      L00C00630
  ADD      #-72, R15
  MOV.L    dc_yh, RD11
  MOV.L    dc_yl, RD10
  SUB      RD11, RD10, RD12
  CMP/PZ   RD12
  BT       L00801E22
  BRA      L00E005AA

L00801E22:
  MOV.L    dc_x, RD9
  MOV      RD9, RD14
  MOV      #320, RD8
  CMP/HS   RD8, RD14
  BT       L00801E24
  MOV.L    dc_yl, RD31
  MOV      #0, RD30
  CMP/GE   RD30, RD31
  BT       L00801E24
  BRA      L00801E25

L00801E25:
  MOV.L    dc_yh, RD29
  MOV      #200, RD28
  CMP/GE   RD28, RD29
  BT       L00801E24
  BRA      L00801E23

L00801E24:
  MOV      L00C007A8, RQ4
  MOV.L    dc_yl, RD5
  MOV.L    dc_yh, RD6
  MOV.L    dc_x, RD7
  BSR      I_Error

L00801E23:
  MOV.L    dc_x, RD27
  SHAD     #1, RD27
  MOV      ylookup, RQ26
  MOV.L    dc_yl, RD25
  EXTS.L   RD25
  MOV.Q    @(RQ26, RD25), RQ7
  MOV      columnofs, RQ6
  EXTS.L   RD27
  MOV.L    @(RQ6, RD27), RD14
  LEA.W    @(RQ7, RD14), RQ5
  EXTS.L   RD25
  MOV.Q    @(RQ26, RD25), RQ7
  ADD      RD27, #1, RD14
  EXTS.L   RD14
  MOV.L    @(RQ6, RD14), RD13
  LEA.W    @(RQ7, RD13), RQ4
  MOV.L    dc_iscale, RD2
  MOV      RD2, RD3
  MOV.L    centery, RD23
  SUB      RD25, RD23, RD14
  MUL.L    RD14, RD3, RD13
  MOV.L    dc_texturemid, RD21
  ADD      RD21, RD13, RD22
  MOV.L    RD27, dc_x
  MOV.Q    RQ5, @(R15, 40)
  MOV.Q    RQ4, @(R15, 32)
  MOV.L    RD3, @(R15, 24)
  MOV.L    RD22, @(R15, 28)

L00801E27:
  MOV.L    @(R15, 28), RD20
  SHAD     RD20, #-16, RD14
  AND      RD14, #127, RD13
  MOV.Q    dc_source, RQ19
  EXTS.L   RD13
  MOVU.B   @(RQ19, RD13), RD14
  MOV.Q    dc_colormap, RQ18
  MOVU.W   @(RQ18, RD14), RD13
  MOV.Q    @(R15, 40), RQ17
  MOVU.W   RD13, @RQ17
  MOV.Q    @(R15, 32), RQ16
  MOVU.W   RD13, @RQ16
  ADD      #640, RQ17
  ADD      #640, RQ16
  MOV.L    @(R15, 24), RD21
  ADD      RD21, RD20
  MOV      RD12, RD14
  ADD      #-1, RD12
  MOV.L    RD20, @(R15, 28)
  MOV.Q    RQ17, @(R15, 40)
  MOV.Q    RQ16, @(R15, 32)
  CMP/EQ   RD14
  BT       L00801E27

L00801E28:

L00E005AA:
  ADD      #72, R15
  BRA      L00C00626

R_DrawColumn_ZB:
  PUSH     LR
  BSR      L00C00630
  ADD      #-72, R15
  MOV.L    dc_yh, RD8
  MOV.L    dc_yl, RD31
  SUB      RD8, RD31, RD10
  CMP/PZ   RD10
  BT       L00801E1D
  BRA      L00E005AB

L00801E1D:
  MOV      ylookup, RQ30
  MOV.L    dc_yl, RD29
  EXTS.L   RD29
  MOV.Q    @(RQ30, RD29), RQ9
  MOV      columnofs, RQ28
  MOV.L    dc_x, RD27
  EXTS.L   RD27
  MOV.L    @(RQ28, RD27), RD14
  LEA.W    @(RQ9, RD14), RQ26
  MOV      ylookup_zb, RQ25
  EXTS.L   RD29
  MOV.Q    @(RQ25, RD29), RQ9
  EXTS.L   RD27
  MOV.L    @(RQ28, RD27), RD14
  LEA.W    @(RQ9, RD14), RQ11
  MOV.L    dc_iscale, RD8
  MOV      RD8, RD31
  MOV.L    centery, RD8
  SUB      RD29, RD8, RD14
  MUL.L    RD14, RD31, RD13
  MOV.L    dc_texturemid, RD8
  ADD      RD8, RD13, RD30
  MOV.L    dc_zdist, RD14
  EXTU.W   RD14, RD14
  SHLD     RD14, #-13, RD12
  MOV.L    RD31, @(R15, 32)
  MOV.L    RD30, @(R15, 36)
  MOV.Q    RQ26, @(R15, 40)
  MOV      RD12, RD4
  MOV      #0, RD5
  MOV      #0x0000FFFF, RD6
  BSR      r_int_clamp
  MOV      RD2, RD14
  EXTU.W   RD14, RD12
  MOV      #0, RD25
  MOV.L    RD25, @(R15, 28)

L00801E1E:
  MOVU.W   @RQ11, RD14
  EXTU.W   RD12, RD13
  MOV      RD14, RD7
  MOV.L    RD7, @(R15, 4)
  CMP/GT   RD13, RD7
  BT       L00801E20
  MOV.L    @(R15, 36), RD6
  SHAD     RD6, #-16, RD14
  AND      RD14, #127, RD13
  MOV.Q    dc_source, RQ5
  EXTS.L   RD13
  MOVU.B   @(RQ5, RD13), RD14
  MOV.Q    dc_colormap, RQ4
  MOVU.W   @(RQ4, RD14), RD13
  MOV.Q    @(R15, 40), RQ3
  MOVU.W   RD13, @RQ3
  MOVU.W   RD12, @RQ11
  BRA      L00801E21

L00801E20:
  MOV      #-1, RD2
  MOV.L    RD2, @(R15, 28)

L00801E21:
  MOV.Q    @(R15, 40), RQ23
  ADD      #640, RQ23
  ADD      #640, RQ11
  MOV.L    @(R15, 32), RD22
  MOV.L    @(R15, 36), RD21
  ADD      RD22, RD21
  MOV      RD10, RD14
  ADD      #-1, RD10
  MOV.Q    RQ23, @(R15, 40)
  MOV.L    RD21, @(R15, 36)
  CMP/EQ   RD14
  BT       L00801E1E

L00801E1F:

L00E005AB:
  ADD      #72, R15
  BRA      L00C00626

R_DrawFuzzColumn:
  PUSH     LR
  BSR      L00C00630
  ADD      #-72, R15
  MOV.L    dc_yl, RD13
  CMP/EQ   RD13
  BT       L00801E29
  MOV      #1, RD12
  MOV.L    RD12, dc_yl

L00801E29:
  MOV.L    viewheight, RD11
  SUB      RD11, #1, RD14
  MOV.L    dc_yh, RD10
  CMP/EQ   RD14, RD10
  BT       L00801E2A
  MOV.L    viewheight, RD9
  SUB      RD9, #2, RD8
  MOV.L    RD8, dc_yh

L00801E2A:
  MOV.L    dc_yh, RD30
  MOV.L    dc_yl, RD29
  SUB      RD30, RD29, RD31
  MOV.L    RD31, @(R15, 44)
  MOV      #0, RD28
  CMP/GE   RD28, RD31
  BT       L00801E2B
  BRA      L00E005AC

L00801E2B:
  MOV.L    dc_x, RD27
  MOV      RD27, RD14
  MOV      #320, RD26
  CMP/HS   RD26, RD14
  BT       L00801E2D
  MOV.L    dc_yl, RD25
  MOV      #0, RD7
  CMP/GE   RD7, RD25
  BT       L00801E2D
  BRA      L00801E2E

L00801E2E:
  MOV.L    dc_yh, RD6
  CMP/GE   #200, RD6
  BT       L00801E2D
  BRA      L00801E2C

L00801E2D:
  MOV      L00C007AC, RQ4
  MOV.L    dc_yl, RD5
  MOV.L    dc_yh, RD6
  MOV.L    dc_x, RD7
  BSR      I_Error

L00801E2C:
  MOV      ylookup, RQ5
  MOV.L    dc_yl, RD4
  MOV.Q    @(RQ5, RD4), RQ3
  MOV      columnofs, RQ2
  MOV.L    dc_x, RD23
  EXTS.L   RD23
  MOV.L    @(RQ2, RD23), RD14
  LEA.W    @(RQ3, RD14), RQ22
  MOV.L    dc_iscale, RD20
  MOV      RD20, RD21
  MOV.L    centery, RD19
  SUB      RD4, RD19, RD14
  MUL.L    RD14, RD21, RD18
  MOV.L    dc_texturemid, RD16
  ADD      RD16, RD18, RD17
  MOV.Q    RQ22, @(R15, 32)
  MOV.L    RD21, @(R15, 24)
  MOV.L    RD17, @(R15, 28)

L00801E30:
  MOV      fuzzoffset, RQ16
  MOV.L    fuzzpos, RD17
  EXTS.L   RD17
  MOV.L    @(RQ16, RD17), RD14
  MOV.Q    @(R15, 32), RQ18
  MOVU.W   @(RQ18, RD14), RD19
  SUB      #1024, RD19
  MOV.L    RD19, @(R15, 20)
  MOV      #528, RD20
  CMP/GE   RD20, RD19
  BT       L00801E32
  MOV      #528, RD16
  MOV.L    RD16, @(R15, 20)

L00801E32:
  MOV.Q    @(R15, 32), RQ16
  MOV.L    @(R15, 20), RD17
  MOVU.W   RD17, @RQ16
  MOV.L    fuzzpos, RD18
  ADD      #1, RD18
  MOV.L    RD18, fuzzpos
  MOV.L    fuzzpos, RD19
  CMP/EQ   #50, RD19
  BT       L00801E33
  MOV      #0, RD16
  MOV.L    RD16, fuzzpos

L00801E33:
  MOV.Q    @(R15, 32), RQ16
  ADD      #640, RQ16
  MOV.L    @(R15, 24), RD17
  MOV.L    @(R15, 28), RD18
  ADD      RD17, RD18
  MOV.L    @(R15, 44), RD19
  MOV      RD19, RD14
  ADD      #-1, RD19
  MOV.L    RD19, @(R15, 44)
  MOV.L    RD18, @(R15, 28)
  MOV.Q    RQ16, @(R15, 32)
  CMP/EQ   RD14
  BT       L00801E30

L00801E31:

L00E005AC:
  ADD      #72, R15
  BRA      L00C00626

R_DrawTranslatedColumn:
  PUSH     LR
  BSR      L00C00630
  ADD      #-72, R15
  MOV.L    dc_yh, RD11
  MOV.L    dc_yl, RD10
  SUB      RD11, RD10, RD12
  CMP/PZ   RD12
  BT       L00801E34
  BRA      L00E005AD

L00801E34:
  MOV.L    dc_x, RD9
  MOV      RD9, RD14
  MOV      #320, RD8
  CMP/HS   RD8, RD14
  BT       L00801E36
  MOV.L    dc_yl, RD31
  MOV      #0, RD30
  CMP/GE   RD30, RD31
  BT       L00801E36
  BRA      L00801E37

L00801E37:
  MOV.L    dc_yh, RD29
  MOV      #200, RD28
  CMP/GE   RD28, RD29
  BT       L00801E36
  BRA      L00801E35

L00801E36:
  MOV      L00C007A8, RQ4
  MOV.L    dc_yl, RD5
  MOV.L    dc_yh, RD6
  MOV.L    dc_x, RD7
  BSR      I_Error

L00801E35:
  MOV      ylookup, RQ27
  MOV.L    dc_yl, RD26
  EXTS.L   RD26
  MOV.Q    @(RQ27, RD26), RQ25
  MOV      columnofs, RQ7
  MOV.L    dc_x, RD6
  MOV.L    @(RQ7, RD6), RD14
  LEA.W    @(RQ25, RD14), RQ5
  MOV.L    dc_iscale, RD3
  MOV      RD3, RD4
  MOV.L    centery, RD2
  SUB      RD26, RD2, RD14
  MUL.L    RD14, RD4, RD13
  MOV.L    dc_texturemid, RD22
  ADD      RD22, RD13, RD23
  MOV.Q    RQ5, @(R15, 40)
  MOV.L    RD4, @(R15, 32)
  MOV.L    RD23, @(R15, 36)

L00801E39:
  MOV.L    @(R15, 36), RD21
  SHAD     RD21, #-16, RD14
  MOV.Q    dc_source, RQ20
  MOVU.B   @(RQ20, RD14), RD13
  MOV.Q    dc_translation, RQ19
  MOVU.B   @(RQ19, RD13), RD14
  MOV.Q    dc_colormap, RQ18
  MOVU.W   @(RQ18, RD14), RD13
  MOV.Q    @(R15, 40), RQ17
  MOVU.W   RD13, @RQ17
  ADD      #640, RQ17
  MOV.L    @(R15, 32), RD16
  ADD      RD16, RD21
  MOV      RD12, RD14
  ADD      #-1, RD12
  MOV.L    RD21, @(R15, 36)
  MOV.Q    RQ17, @(R15, 40)
  CMP/EQ   RD14
  BT       L00801E39

L00801E3A:

L00E005AD:
  ADD      #72, R15
  BRA      L00C00626

R_VideoErase:
  PUSH     LR
  BSR      L00C00649
  ADD      #-136, R15
  MOV.L    RD4, @(R15, 44)
  MOV.L    RD5, @(R15, 40)
  MOV.L    @(R15, 40), RD12
  SHAD     RD12, #1, RD13
  MOV      screens, RQ11
  MOV.Q    @(RQ11, 8), RQ14
  MOV.L    @(R15, 44), RD10
  LEA.W    @(RQ14, RD10), RQ9
  MOV.Q    @RQ11, RQ14
  LEA.W    @(RQ14, RD10), RQ12
  EXTS.L   RD13, RQ11
  MOV.Q    RQ12, @(R15, 8)
  MOV.Q    RQ11, @R15
  MOV.Q    RQ9, @(R15, 16)
  MOV      RQ12, RQ4
  MOV      RQ9, RQ5
  MOV      RQ11, RQ6
  BSR      memcpy

L00E005AE:
  ADD      #136, R15
  BRA      L00C0064D

R_DrawSpan:
  PUSH     LR
  BSR      L00C00630
  ADD      #-72, R15
  MOV.L    ds_xfrac, RD31
  SHAD     RD31, #10, RD14
  AND      RD14, #-65536, RD11
  MOV.L    ds_yfrac, RD30
  SHAD     RD30, #-6, RD14
  EXTU.W   RD14, RD29
  OR       RD11, RD29, RD13
  MOV.L    ds_xstep, RD28
  SHAD     RD28, #10, RD14
  AND      RD14, #-65536, RD11
  MOV.L    ds_ystep, RD27
  SHAD     RD27, #-6, RD14
  EXTU.W   RD14, RD29
  OR       RD11, RD29, RD26
  MOV.Q    ds_source, RQ7
  MOV      RQ7, RQ25
  MOV.Q    ds_colormap, RQ5
  MOV      RQ5, RQ6
  MOV      ylookup, RQ4
  MOV.L    ds_y, RD3
  MOV.Q    @(RQ4, RD3), RQ2
  MOV      columnofs, RQ23
  MOV.L    ds_x1, RD22
  EXTS.L   RD22
  MOV.L    @(RQ23, RD22), RD14
  LEA.W    @(RQ2, RD14), RQ8
  MOV.L    ds_x2, RD21
  SUB      RD21, RD22, RD14
  ADD      RD14, #1, RD20
  MOV.L    RD26, @(R15, 44)
  MOV.Q    RQ25, @(R15, 32)
  MOV.Q    RQ6, @(R15, 24)
  MOV.Q    RQ2, @R15
  MOV.L    RD20, @(R15, 20)

L00801E47:
  MOV.L    @(R15, 20), RD19
  MOV      #4, RD18
  CMP/HS   RD18, RD19
  BT       L00801E48
  SHLD     RD13, #-4, RD12
  AND      #4032, RD12
  SHLD     RD13, #-26, RD10
  OR       RD10, RD12, RD9
  MOV.L    @(R15, 44), RD17
  ADD      RD17, RD13
  MOV.Q    @(R15, 32), RQ16
  EXTS.L   RD9
  MOVU.B   @(RQ16, RD9), RD14
  MOV.Q    @(R15, 24), RQ18
  MOVU.W   @(RQ18, RD14), RD11
  MOVU.W   RD11, @RQ8
  SHLD     RD13, #-4, RD12
  AND      #4032, RD12
  SHLD     RD13, #-26, RD10
  OR       RD10, RD12, RD9
  ADD      RD17, RD13
  EXTS.L   RD9
  MOVU.B   @(RQ16, RD9), RD14
  MOVU.W   @(RQ18, RD14), RD11
  MOVU.W   RD11, @(RQ8, 2)
  SHLD     RD13, #-4, RD12
  AND      #4032, RD12
  SHLD     RD13, #-26, RD10
  OR       RD10, RD12, RD9
  ADD      RD17, RD13
  EXTS.L   RD9
  MOVU.B   @(RQ16, RD9), RD14
  MOVU.W   @(RQ18, RD14), RD11
  MOVU.W   RD11, @(RQ8, 4)
  SHLD     RD13, #-4, RD12
  AND      #4032, RD12
  SHLD     RD13, #-26, RD10
  OR       RD10, RD12, RD9
  ADD      RD17, RD13
  EXTS.L   RD9
  MOVU.B   @(RQ16, RD9), RD14
  MOVU.W   @(RQ18, RD14), RD11
  MOVU.W   RD11, @(RQ8, 6)
  MOV.L    @(R15, 20), RD19
  SUB      #4, RD19
  ADD      #8, RQ8
  MOV.L    RD19, @(R15, 20)
  BRA      L00801E47

L00801E48:

L00801E49:
  MOV.L    @(R15, 20), RD16
  CMP/EQ   #0, RD16
  BT       L00801E4A
  SHLD     RD13, #-4, RD12
  AND      #4032, RD12
  SHLD     RD13, #-26, RD10
  OR       RD10, RD12, RD9
  MOV.L    @(R15, 44), RD16
  ADD      RD16, RD13
  MOV.Q    @(R15, 32), RQ17
  EXTS.L   RD9
  MOVU.B   @(RQ17, RD9), RD14
  MOV.Q    @(R15, 24), RQ18
  MOVU.W   @(RQ18, RD14), RD11
  MOV      RQ8, RQ19
  ADD      #2, RQ8
  MOVU.W   RD11, @RQ19
  MOV.L    @(R15, 20), RD20
  ADD      #-1, RD20
  MOV.L    RD20, @(R15, 20)
  MOV.Q    RQ19, @R15
  BRA      L00801E49

L00801E4A:

L00E005AF:
  ADD      #72, R15
  BRA      L00C00626

R_DrawSpan_ZB:
  PUSH     LR
  BSR      L00C00630
  ADD      #-72, R15
  MOV.L    ds_xfrac, RD31
  MOV      RD31, RD8
  MOV.L    ds_yfrac, RD29
  MOV      RD29, RD30
  MOV      ylookup, RQ28
  MOV.L    ds_y, RD27
  EXTS.L   RD27
  MOV.Q    @(RQ28, RD27), RQ11
  MOV      columnofs, RQ26
  MOV.L    ds_x1, RD25
  EXTS.L   RD25
  MOV.L    @(RQ26, RD25), RD14
  LEA.W    @(RQ11, RD14), RQ9
  MOV      ylookup_zb, RQ31
  EXTS.L   RD27
  MOV.Q    @(RQ31, RD27), RQ11
  EXTS.L   RD25
  MOV.L    @(RQ26, RD25), RD14
  LEA.W    @(RQ11, RD14), RQ12
  MOV.L    ds_x2, RD31
  SUB      RD31, RD25, RD29
  MOV.L    ds_z, RD31
  SHAD     RD31, #-13, RD13
  MOV.L    RD30, @(R15, 44)
  MOV.L    RD29, @(R15, 40)
  MOV      RD13, RD4
  MOV      #0, RD5
  MOV      #0x0000FFFF, RD6
  BSR      r_int_clamp
  MOV      RD2, RD13

L00801E4B:
  MOV.L    @(R15, 44), RD7
  SHAD     RD7, #-10, RD14
  AND      RD14, #4032, RD10
  SHAD     RD8, #-16, RD14
  AND      RD14, #63, RD6
  ADD      RD10, RD6, RD5
  MOVU.W   @RQ12, RD14
  MOV.L    RD5, @(R15, 36)
  CMP/GT   RD14, RD13
  BT       L00801E4D
  MOV.Q    ds_source, RQ4
  MOV.L    @(R15, 36), RD3
  MOVU.B   @(RQ4, RD3), RD14
  MOV.Q    ds_colormap, RQ2
  MOVU.W   @(RQ2, RD14), RD10
  MOVU.W   RD10, @RQ9
  MOVU.W   RD13, @RQ12

L00801E4D:
  ADD      #2, RQ9
  ADD      #2, RQ12
  MOV.L    ds_xstep, RD23
  ADD      RD23, RD8
  MOV.L    ds_ystep, RD22
  MOV.L    @(R15, 44), RD21
  ADD      RD22, RD21
  MOV.L    @(R15, 40), RD20
  MOV      RD20, RD14
  ADD      #-1, RD20
  MOV.L    RD21, @(R15, 44)
  MOV.L    RD20, @(R15, 40)
  CMP/EQ   RD14
  BT       L00801E4B

L00801E4C:

L00E005B0:
  ADD      #72, R15
  BRA      L00C00626

R_DrawSpanLow:
  PUSH     LR
  BSR      L00C00769
  ADD      #-88, R15
  MOV.L    ds_x2, RD13
  MOV.L    ds_x1, RD12
  CMP/GT   RD13, RD12
  BT       L00801E4F
  MOV.L    ds_x1, RD11
  CMP/PZ   RD11
  BT       L00801E4F
  BRA      L00801E51

L00801E51:
  MOV.L    ds_x2, RD10
  CMP/GE   #320, RD10
  BT       L00801E4F
  BRA      L00801E50

L00801E50:
  MOV.L    ds_y, RD9
  MOV      RD9, RD14
  CMP/HI   #200, RD14
  BT       L00801E4F
  BRA      L00801E4E

L00801E4F:
  MOV      L00C007B6, RQ4
  MOV.L    ds_x1, RD5
  MOV.L    ds_x2, RD6
  MOV.L    ds_y, RD7
  BSR      I_Error

L00801E4E:
  MOV.L    ds_xfrac, RD31
  MOV      RD31, RD8
  MOV.L    ds_yfrac, RD29
  MOV      RD29, RD30
  MOV.L    ds_x1, RD28
  SHAD     #1, RD28
  MOV.L    ds_x2, RD27
  SHAD     #1, RD27
  MOV      ylookup, RQ7
  MOV.L    ds_y, RD6
  MOV.Q    @(RQ7, RD6), RQ5
  MOV      columnofs, RQ4
  EXTS.L   RD28
  MOV.L    @(RQ4, RD28), RD14
  LEA.W    @(RQ5, RD14), RQ3
  SUB      RD27, RD28, RD2
  MOV.L    RD8, @(R15, 44)
  MOV.L    RD30, @(R15, 40)
  MOV.L    RD28, ds_x1
  MOV.L    RD27, ds_x2
  MOV.Q    RQ5, @(R15, 8)
  MOV.Q    RQ3, @(R15, 32)
  MOV.L    RD2, @(R15, 28)

L00801E53:
  MOV.L    @(R15, 40), RD23
  SHAD     RD23, #-10, RD14
  AND      RD14, #4032, RD22
  MOV.L    @(R15, 44), RD21
  SHAD     RD21, #-16, RD14
  AND      RD14, #63, RD20
  ADD      RD22, RD20, RD19
  MOV.Q    ds_source, RQ18
  EXTS.L   RD19
  MOVU.B   @(RQ18, RD19), RD14
  MOV.Q    ds_colormap, RQ17
  MOVU.W   @(RQ17, RD14), RD22
  MOV.Q    @(R15, 32), RQ2
  MOV      RQ2, RQ16
  ADD      #2, RQ2
  MOVU.W   RD22, @RQ16
  EXTS.L   RD19
  MOVU.B   @(RQ18, RD19), RD14
  MOVU.W   @(RQ17, RD14), RD22
  MOV      RQ2, RQ16
  ADD      #2, RQ2
  MOVU.W   RD22, @RQ16
  MOV.L    ds_xstep, RD3
  ADD      RD3, RD21
  MOV.L    ds_ystep, RD4
  ADD      RD4, RD23
  MOV.L    @(R15, 28), RD5
  MOV      RD5, RD14
  ADD      #-1, RD5
  MOV.L    RD5, @(R15, 28)
  MOV.Q    RQ2, @(R15, 32)
  MOV.L    RD23, @(R15, 40)
  MOV.L    RD21, @(R15, 44)
  MOV.L    RD19, @(R15, 24)
  MOV.Q    RQ16, @(R15, 8)
  CMP/EQ   RD14
  BT       L00801E53

L00801E54:

L00E005B1:
  ADD      #88, R15
  BRA      L00C00770

R_InitBuffer:
  PUSH     LR
  BSR      L00C00630
  ADD      #-72, R15
  MOV.L    RD4, @(R15, 44)
  MOV.L    RD5, @(R15, 40)
  MOV      #320, RD12
  MOV.L    @(R15, 44), RD11
  SUB      RD12, RD11, RD13
  SHAD     RD13, #-1, RD10
  MOV      #0, RD14
  MOV.L    RD10, viewwindowx

L00801E55:
  MOV.L    @(R15, 44), RD9
  CMP/GT   RD14, RD9
  BT       L00801E57
  MOV.L    viewwindowx, RD8
  ADD      RD8, RD14, RD13
  MOV      columnofs, RQ31
  EXTS.L   RD14
  MOV.L    RD13, @(RQ31, RD14)

L00801E56:
  ADD      #1, RD14
  BRA      L00801E55

L00801E57:
  MOV.L    @(R15, 44), RD30
  CMP/EQ   #320, RD30
  BT       L00801E58
  MOV      #0, RD29
  MOV.L    RD29, viewwindowy
  BRA      L00801E59

L00801E58:
  MOV      #168, RD28
  MOV.L    @(R15, 40), RD27
  SUB      RD28, RD27, RD13
  SHAD     RD13, #-1, RD26
  MOV.L    RD26, viewwindowy

L00801E59:
  MOV      #0, RD14

L00801E5A:
  MOV.L    @(R15, 40), RD25
  CMP/GT   RD14, RD25
  BT       L00801E5C
  MOV      screens, RQ7
  MOV.Q    @RQ7, RQ6
  MOV.L    viewwindowy, RD5
  ADD      RD14, RD5, RD13
  MOV      RD13, RD4
  MOV      #320, RD3
  MUL.L    RD3, RD4, RD4
  EXTS.L   RD4
  LEA.W    @(RQ6, RD4), RQ2
  MOV      ylookup, RQ23
  EXTS.L   RD14
  MOV.Q    RQ2, @(RQ23, RD14)
  ADD      RD14, RD5, RD13
  MOV      RD13, RD4
  MUL.L    RD3, RD4, RD4
  MOV.Q    screens_zbuf, RQ22
  EXTS.L   RD4
  LEA.W    @(RQ22, RD4), RQ6
  MOV      ylookup_zb, RQ21
  EXTS.L   RD14
  MOV.Q    RQ6, @(RQ21, RD14)

L00801E5B:
  ADD      #1, RD14
  BRA      L00801E5A

L00801E5C:

L00E005B2:
  ADD      #72, R15
  BRA      L00C00626

L00C007B8:
  PUSH     R31
  PUSH     R30
  PUSH     R29
  PUSH     R28
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  PUSH     R9
  PUSH     R8
  RTS     

R_InitTranslationTables:
  PUSH     LR
  BSR      L00C007B8
  ADD      #-80, R15
  MOV      #0, RQ12
  MOV      #1023, RD4
  MOV      #1, RD5
  MOV      RQ12, RQ6
  BSR      Z_Malloc
  MOV      RQ2, RQ11
  MOV      RQ11, RQ10
  MOV      #255, RQ9
  ADD      RQ10, RQ9, RQ8
  MOV      #-256, RQ31
  AND      RQ8, RQ31, RQ10
  MOV      RQ10, RQ11
  MOV      #0, RD14
  MOV.Q    RQ11, translationtables

L00801E3B:
  CMP/GE   #256, RD14
  BT       L00801E3D
  CMP/GE   #112, RD14
  BT       L00801E3E
  CMP/GT   #127, RD14
  BT       L00801E3E
  BRA      L00801E3F

L00801E3F:
  AND      RD14, #15, RD13
  ADD      RD13, #96, RD30
  MOV.Q    translationtables, RQ29
  EXTS.L   RD14
  MOVU.B   RD30, @(RQ29, RD14)
  AND      RD14, #15, RD13
  ADD      RD13, #64, RD30
  ADD      RD14, #256, RD13
  EXTS.L   RD13
  MOVU.B   RD30, @(RQ29, RD13)
  AND      RD14, #15, RD13
  ADD      RD13, #32, RD30
  ADD      RD14, #512, RD13
  EXTS.L   RD13
  MOVU.B   RD30, @(RQ29, RD13)
  BRA      L00801E41

L00801E3E:
  ADD      RD14, #512, RD13
  MOV.Q    translationtables, RQ28
  EXTS.L   RD13
  MOVU.B   RD14, @(RQ28, RD13)
  ADD      RD14, #256, RD13
  EXTS.L   RD13
  MOVU.B   RD14, @(RQ28, RD13)
  EXTS.L   RD14
  MOVU.B   RD14, @(RQ28, RD14)

L00801E41:

L00801E3C:
  ADD      #1, RD14
  BRA      L00801E3B

L00801E3D:

L00E005B3:
  ADD      #80, R15

L00C007B9:
  POP      RQ8
  POP      RQ9
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  POP      RQ28
  POP      RQ29
  POP      RQ30
  POP      RQ31
  RET     

R_FillBackScreen:
  PUSH     LR
  BSR      L00C00630
  ADD      #-72, R15
  MOV      #48, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 32)
  MOV      #48, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 24)
  MOV.L    scaledviewwidth, RD31
  CMP/EQ   #320, RD31
  BT       L00801E5D
  BRA      L00E005B4

L00801E5D:
  MOV.L    gamemode, RD30
  CMP/EQ   #2, RD30
  BT       L00801E5E
  MOV      #48, RQ29
  ADD      R15, RQ29
  MOV      RQ29, RQ8
  BRA      L00801E5F

L00801E5E:
  MOV      #48, RQ28
  ADD      R15, RQ28
  MOV      RQ28, RQ8

L00801E5F:
  MOV      RQ8, RQ4
  MOV      #101, RD5
  BSR      W_CacheLumpName
  MOV      RQ2, RQ27
  MOV      screens, RQ26
  MOV.Q    @(RQ26, 8), RQ9
  MOV      #0, RD13
  MOV.Q    RQ27, @(R15, 40)

L00801E60:
  CMP/GE   #168, RD13
  BT       L00801E62
  MOV      #0, RD12

L00801E63:
  MOV      #5, RD25
  CMP/GE   RD25, RD12
  BT       L00801E65
  MOV      #0, RD10

L00801E66:
  CMP/GE   #64, RD10
  BT       L00801E68
  AND      RD13, #63, RD14
  SHAD     RD14, #6, RD7
  MOV.Q    @(R15, 40), RQ6
  LEA.B    @(RQ6, RD7), RQ5
  EXTS.L   RD10
  MOVU.B   @(RQ5, RD10), RD14
  EXTS.L   RD10
  MOVU.W   RD14, @(RQ9, RD10)
  MOV.L    RD7, @(R15, 16)
  MOV.Q    RQ5, @(R15, 8)

L00801E67:
  ADD      #1, RD10
  BRA      L00801E66

L00801E68:
  ADD      #128, RQ9

L00801E64:
  ADD      #1, RD12
  BRA      L00801E63

L00801E65:

L00801E61:
  ADD      #1, RD13
  BRA      L00801E60

L00801E62:
  MOV      L00C007BA, RQ4
  MOV      #101, RD5
  BSR      W_CacheLumpName
  MOV      RQ2, RQ11
  MOV      #0, RD12

L00801E69:
  MOV.L    scaledviewwidth, RD4
  CMP/GT   RD12, RD4
  BT       L00801E6B
  MOV.L    viewwindowy, RD25
  SUB      RD25, #8, RD14
  MOV.L    viewwindowx, RD27
  ADD      RD27, RD12, RD26
  MOV.L    RD26, @(R15, 16)
  MOV      RD26, RD4
  MOV      RD14, RD5
  MOV      #1, RD6
  MOV      RQ11, RQ7
  BSR      V_DrawPatch

L00801E6A:
  ADD      #8, RD12
  BRA      L00801E69

L00801E6B:
  MOV      L00C007BB, RQ4
  MOV      #101, RD5
  BSR      W_CacheLumpName
  MOV      RQ2, RQ11
  MOV      #0, RD12

L00801E6C:
  MOV.L    scaledviewwidth, RD3
  CMP/GT   RD12, RD3
  BT       L00801E6E
  MOV.L    viewwindowy, RD25
  MOV.L    viewheight, RD26
  ADD      RD25, RD26, RD14
  MOV.L    viewwindowx, RD28
  ADD      RD28, RD12, RD27
  MOV.L    RD27, @(R15, 16)
  MOV      RD27, RD4
  MOV      RD14, RD5
  MOV      #1, RD6
  MOV      RQ11, RQ7
  BSR      V_DrawPatch

L00801E6D:
  ADD      #8, RD12
  BRA      L00801E6C

L00801E6E:
  MOV      L00C007BC, RQ4
  MOV      #101, RD5
  BSR      W_CacheLumpName
  MOV      RQ2, RQ11
  MOV      #0, RD13

L00801E6F:
  MOV.L    viewheight, RD2
  CMP/GT   RD13, RD2
  BT       L00801E71
  MOV.L    viewwindowy, RD25
  ADD      RD25, RD13, RD14
  MOV.L    viewwindowx, RD26
  SUB      RD26, #8, RD27
  MOV.L    RD27, @(R15, 16)
  MOV      RD27, RD4
  MOV      RD14, RD5
  MOV      #1, RD6
  MOV      RQ11, RQ7
  BSR      V_DrawPatch

L00801E70:
  ADD      #8, RD13
  BRA      L00801E6F

L00801E71:
  MOV      L00C007BD, RQ4
  MOV      #101, RD5
  BSR      W_CacheLumpName
  MOV      RQ2, RQ11
  MOV      #0, RD13

L00801E72:
  MOV.L    viewheight, RD23
  CMP/GT   RD13, RD23
  BT       L00801E74
  MOV.L    viewwindowy, RD25
  ADD      RD25, RD13, RD14
  MOV.L    viewwindowx, RD27
  MOV.L    scaledviewwidth, RD28
  ADD      RD27, RD28, RD26
  MOV.L    RD26, @(R15, 16)
  MOV      RD26, RD4
  MOV      RD14, RD5
  MOV      #1, RD6
  MOV      RQ11, RQ7
  BSR      V_DrawPatch

L00801E73:
  ADD      #8, RD13
  BRA      L00801E72

L00801E74:
  MOV      L00C007BE, RQ4
  MOV      #101, RD5
  BSR      W_CacheLumpName
  MOV      RQ2, RQ25
  MOV.L    viewwindowy, RD26
  SUB      RD26, #8, RD14
  MOV.L    viewwindowx, RD27
  SUB      RD27, #8, RD28
  MOV.L    RD28, @(R15, 16)
  MOV.Q    RQ25, @(R15, 8)
  MOV      RD28, RD4
  MOV      RD14, RD5
  MOV      #1, RD6
  MOV      RQ25, RQ7
  BSR      V_DrawPatch
  MOV      L00C007BF, RQ4
  MOV      #101, RD5
  BSR      W_CacheLumpName
  MOV      RQ2, RQ25
  MOV.L    viewwindowy, RD29
  SUB      RD29, #8, RD14
  MOV.L    viewwindowx, RD30
  MOV.L    scaledviewwidth, RD31
  ADD      RD30, RD31, RD28
  MOV.L    RD28, @(R15, 16)
  MOV.Q    RQ25, @(R15, 8)
  MOV      RD28, RD4
  MOV      RD14, RD5
  MOV      #1, RD6
  MOV      RQ25, RQ7
  BSR      V_DrawPatch
  MOV      L00C007C0, RQ4
  MOV      #101, RD5
  BSR      W_CacheLumpName
  MOV      RQ2, RQ25
  MOV.L    viewwindowy, RD26
  MOV.L    viewheight, RD27
  ADD      RD26, RD27, RD14
  MOV.L    viewwindowx, RD29
  SUB      RD29, #8, RD28
  MOV.L    RD28, @(R15, 16)
  MOV.Q    RQ25, @(R15, 8)
  MOV      RD28, RD4
  MOV      RD14, RD5
  MOV      #1, RD6
  MOV      RQ25, RQ7
  BSR      V_DrawPatch
  MOV      L00C007C1, RQ4
  MOV      #101, RD5
  BSR      W_CacheLumpName
  MOV      RQ2, RQ25
  MOV.L    viewwindowy, RD30
  MOV.L    viewheight, RD31
  ADD      RD30, RD31, RD14
  MOV.L    viewwindowx, RD26
  MOV.L    scaledviewwidth, RD27
  ADD      RD26, RD27, RD28
  MOV.L    RD28, @(R15, 16)
  MOV.Q    RQ25, @(R15, 8)
  MOV      RD28, RD4
  MOV      RD14, RD5
  MOV      #1, RD6
  MOV      RQ25, RQ7
  BSR      V_DrawPatch

L00E005B4:
  ADD      #72, R15
  BRA      L00C00626

R_DrawViewBorder:
  PUSH     LR
  BSR      L00C0071E
  ADD      #-64, R15
  MOV.L    scaledviewwidth, RD11
  CMP/EQ   #320, RD11
  BT       L00801E75
  BRA      L00E005B5

L00801E75:
  MOV      #168, RD10
  MOV.L    viewheight, RD9
  SUB      RD10, RD9, RD14
  SHAD     RD14, #-1, RD8
  MOV      #320, RD31
  MOV.L    scaledviewwidth, RD30
  SUB      RD31, RD30, RD14
  SHAD     RD14, #-1, RD12
  MOV      RD8, RD14
  MUL.L    RD31, RD14, RD14
  ADD      RD14, RD12, RD13
  MOV.L    RD8, @(R15, 28)
  MOV      #0, RD4
  MOV      RD13, RD5
  BSR      R_VideoErase
  MOV.L    viewheight, RD29
  ADD      RD29, RD8, RD14
  MOV      RD14, RD13
  MUL.L    RD31, RD13, RD13
  SUB      RD13, RD12, RD28
  MOV      RD8, RD14
  MUL.L    RD31, RD14, RD14
  ADD      RD14, RD12, RD13
  MOV.L    RD28, @(R15, 24)
  MOV      RD28, RD4
  MOV      RD13, RD5
  BSR      R_VideoErase
  MOV      RD8, RD14
  MUL.L    RD31, RD14, RD14
  ADD      RD14, #320, RD13
  SUB      RD13, RD12, RD28
  SHAD     #1, RD12
  MOV      #1, RD27
  MOV.L    RD28, @(R15, 24)
  MOV.L    RD27, @(R15, 20)

L00801E76:
  MOV.L    @(R15, 20), RD26
  MOV.L    viewheight, RD7
  CMP/GT   RD26, RD7
  BT       L00801E78
  MOV.L    @(R15, 24), RD4
  MOV      RD12, RD5
  BSR      R_VideoErase
  MOV.L    @(R15, 24), RD6
  ADD      #320, RD6
  MOV.L    RD6, @(R15, 24)

L00801E77:
  MOV.L    @(R15, 20), RD5
  ADD      #1, RD5
  MOV.L    RD5, @(R15, 20)
  BRA      L00801E76

L00801E78:
  MOV      #0, RD4
  MOV      #0, RD5
  MOV      #320, RD6
  MOV      #168, RD7
  BSR      V_MarkRect

L00E005B5:
  ADD      #64, R15
  BRA      L00C0071F

D_AddFile:
  PUSH     LR
  BSR      L00C0074E
  ADD      #-104, R15
  MOV.Q    RQ4, @(R15, 24)
  MOV      #0, RD14

L00800B75:
  MOV      wadfiles, RQ10
  EXTS.L   RD14
  MOV.Q    @(RQ10, RD14), RQ11
  MOV      #0, RQ9
  CMPQ/EQ  RQ9, RQ11
  BT       L00800B77

L00800B76:
  ADD      #1, RD14
  BRA      L00800B75

L00800B77:
  MOV.Q    @(R15, 24), RQ4
  BSR      strlen
  MOV      RQ2, RQ12
  ADD      RQ12, #1, RQ8
  MOV.Q    RQ8, @R15
  MOV      RQ8, RQ4
  BSR      malloc
  MOV      RQ2, RQ13
  MOV      RQ13, RQ4
  MOV.Q    @(R15, 24), RQ5
  BSR      strcpy
  MOV      wadfiles, RQ31
  EXTS.L   RD14
  MOV.Q    RQ13, @(RQ31, RD14)

L00E005B6:
  ADD      #104, R15
  BRA      L00C00751

D_DoomMain:
  PUSH     LR
  BSR      L00C00630
  ADD      #-536, R15
  MOV      #72, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 64)
  MOV      #328, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 48)
  BSR      FindResponseFile
  BSR      IdentifyVersion
  MOV      #0, RQ12
  BSR      __get_stdout
  MOV      RQ2, RQ11
  MOV.Q    @RQ11, RQ10
  MOV.Q    RQ10, @(R15, 16)
  MOV      RQ10, RQ4
  MOV      RQ12, RQ5
  BSR      setbuf
  MOV      #0, RD9
  MOV.L    RD9, modifiedgame
  MOV      L00C007C7, RQ4
  BSR      M_CheckParm
  MOV      R2, RD8
  MOV.L    RD8, nomonsters
  MOV      L00C007C8, RQ4
  BSR      M_CheckParm
  MOV      R2, RD31
  MOV.L    RD31, respawnparm
  MOV      L00C007C9, RQ4
  BSR      M_CheckParm
  MOV      R2, RD30
  MOV.L    RD30, fastparm
  MOV      L00C007CA, RQ4
  BSR      M_CheckParm
  MOV      R2, RD29
  MOV.L    RD29, devparm
  MOV      L00C007CB, RQ4
  BSR      M_CheckParm
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L00800B9B
  MOV      #2, RD28
  MOV.L    RD28, deathmatch
  BRA      L00800B9C

L00800B9B:
  MOV      L00C007CC, RQ4
  BSR      M_CheckParm
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L00800B9D
  MOV      #1, RD27
  MOV.L    RD27, deathmatch

L00800B9D:

L00800B9C:
  MOV.L    gamemode, RD26
  CMP/EQ   #3, RD26
  BT       L00800B9F
  MOV.L    gamemode, RD25
  CMP/EQ   #0, RD25
  BT       L00800BA0
  MOV.L    gamemode, RD7
  CMP/EQ   #1, RD7
  BT       L00800BA1
  MOV.L    gamemode, RD6
  CMP/EQ   #2, RD6
  BT       L00800BA2
  BRA      L00800BA3

L00800B9F:
  MOV      title, RQ4
  MOV      L00C007CE, RQ5
  MOV      #1, RD6
  MOV      #10, RD7
  BSR      sprintf
  BRA      L00800B9E

L00800BA0:
  MOV      title, RQ4
  MOV      L00C007CF, RQ5
  MOV      #1, RD6
  MOV      #10, RD7
  BSR      sprintf
  BRA      L00800B9E

L00800BA1:
  MOV      title, RQ4
  MOV      L00C007D0, RQ5
  MOV      #1, RD6
  MOV      #10, RD7
  BSR      sprintf
  BRA      L00800B9E

L00800BA2:
  MOV      title, RQ4
  MOV      L00C007D1, RQ5
  MOV      #1, RD6
  MOV      #10, RD7
  BSR      sprintf
  BRA      L00800B9E

L00800BA3:
  MOV      title, RQ4
  MOV      L00C007D2, RQ5
  MOV      #1, RD6
  MOV      #10, RD7
  BSR      sprintf
  BRA      L00800B9E

L00800B9E:
  MOV      L00C007D3, RQ4
  MOV      title, RQ5
  BSR      printf
  MOV.L    devparm, RD5
  CMP/EQ   RD5
  BT       L00800BA4
  MOV      L00C007D4, RQ4
  BSR      printf

L00800BA4:
  MOV      L00C007D5, RQ4
  BSR      M_CheckParm
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L00800BA5
  MOV      L00C007D6, RQ4
  BSR      printf
  MOV      basedefault, RQ4
  MOV      L00C007D7, RQ5
  BSR      strcpy

L00800BA5:
  MOV      L00C007D8, RQ4
  BSR      M_CheckParm
  MOV      RD2, RD14
  MOV      RD14, RD13
  CMP/EQ   RD14
  BT       L00800BA6
  MOV      #200, RD14
  MOV      RD14, RD4
  MOV.L    myargc, RD3
  SUB      RD3, #1, RD14
  MOV.L    RD4, @(R15, 60)
  CMP/GT   RD13, RD14
  BT       L00800BA7
  ADD      RD13, #1, RD14
  MOV.Q    myargv, RQ25
  EXTS.L   RD14
  MOV.Q    @(RQ25, RD14), RQ12
  MOV      RQ12, RQ4
  BSR      atoi
  MOV      RD2, RD26
  MOV.L    RD26, @(R15, 60)

L00800BA7:
  MOV.L    @(R15, 60), RD2
  MOV      #10, RD23
  CMP/GE   RD23, RD2
  BT       L00800BA8
  MOV      #10, RD22
  MOV.L    RD22, @(R15, 60)

L00800BA8:
  MOV.L    @(R15, 60), RD21
  CMP/GT   #400, RD21
  BT       L00800BA9
  MOV      #400, RD20
  MOV.L    RD20, @(R15, 60)

L00800BA9:
  MOV      L00C007D9, RQ4
  MOV.L    @(R15, 60), RD5
  BSR      printf
  MOV      forwardmove, RQ25
  MOV.L    @RQ25, RD14
  MOV.L    @(R15, 60), RD27
  MUL.L    RD14, RD27, RD26
  MOV      RD26, RD14
  MOV      #0x028F5C29, RD7
  MOV      #655, RD7
  DMULS    RD7, RD14
  STS      R1, RD14
  MOV.L    RD14, @RQ25
  MOV.L    @(RQ25, 4), RD14
  MUL.L    RD14, RD27, RD26
  MOV      RD26, RD14
  MOV      #0x028F5C29, RD7
  MOV      #655, RD7
  DMULS    RD7, RD14
  STS      R1, RD14
  MOV.L    RD14, @(RQ25, 4)
  MOV      sidemove, RQ28
  MOV.L    @RQ28, RD14
  MUL.L    RD14, RD27, RD26
  MOV      RD26, RD14
  MOV      #0x028F5C29, RD7
  MOV      #655, RD7
  DMULS    RD7, RD14
  STS      R1, RD14
  MOV.L    RD14, @RQ28
  MOV.L    @(RQ28, 4), RD14
  MUL.L    RD14, RD27, RD26
  MOV      RD26, RD14
  MOV      #0x028F5C29, RD7
  MOV      #655, RD7
  DMULS    RD7, RD14
  STS      R1, RD14
  MOV.L    RD14, @(RQ28, 4)
  MOV.L    RD26, @(R15, 8)

L00800BA6:
  MOV      L00C007DC, RQ4
  BSR      M_CheckParm
  MOV      RD2, RD13
  CMP/EQ   RD13
  BT       L00800BAA
  MOV.Q    myargv, RQ19
  EXTS.L   RD13
  MOV.Q    @(RQ19, RD13), RQ12
  MOV      #112, RD18
  MOV.B    RD18, @(RQ12, 4)
  MOV.L    gamemode, RD17
  CMP/EQ   #0, RD17
  BT       L00800BAC
  MOV.L    gamemode, RD16
  CMP/EQ   #3, RD16
  BT       L00800BAD
  MOV.L    gamemode, RD16
  CMP/EQ   #1, RD16
  BT       L00800BAE
  MOV.L    gamemode, RD16
  CMP/EQ   #4, RD16
  BT       L00800BAF
  MOV.L    gamemode, RD16
  CMP/EQ   #2, RD16
  BT       L00800BB0
  BRA      L00800BB1

L00800BAC:

L00800BAD:

L00800BAE:

L00800BAF:
  ADD      RD13, #2, RD14
  MOV.Q    myargv, RQ25
  EXTS.L   RD14
  MOV.Q    @(RQ25, RD14), RQ12
  MOV.B    @RQ12, RD14
  ADD      RD13, #1, RD26
  EXTS.L   RD26
  MOV.Q    @(RQ25, RD26), RQ12
  MOV.B    @RQ12, RD26
  MOV.L    RD26, @(R15, 8)
  MOV      #72, RQ4
  ADD      R15, RQ4
  MOV      L00C007DD, RQ5
  MOV      RD26, RD6
  MOV      RD14, RD7
  BSR      sprintf
  ADD      RD13, #2, RD14
  MOV.Q    myargv, RQ27
  EXTS.L   RD14
  MOV.Q    @(RQ27, RD14), RQ12
  ADD      RD13, #1, RD14
  EXTS.L   RD14
  MOV.Q    @(RQ27, RD14), RQ11
  MOV      L00C007DE, RQ4
  MOV      RQ11, RQ5
  MOV      RQ12, RQ6
  BSR      printf
  BRA      L00800BAB

L00800BB0:

L00800BB1:
  ADD      RD13, #1, RD14
  MOV.Q    myargv, RQ25
  EXTS.L   RD14
  MOV.Q    @(RQ25, RD14), RQ12
  MOV      RQ12, RQ4
  BSR      atoi
  MOV      RD2, RD13
  MOV      #10, RD26
  CMP/GE   RD26, RD13
  BT       L00800BB2
  MOV      #72, RQ4
  ADD      R15, RQ4
  MOV      L00C007DF, RQ5
  MOV      RD13, RD6
  BSR      sprintf
  BRA      L00800BB3

L00800BB2:
  MOV      #72, RQ4
  ADD      R15, RQ4
  MOV      L00C007E0, RQ5
  MOV      RD13, RD6
  BSR      sprintf

L00800BB3:
  BRA      L00800BAB

L00800BAB:
  MOV      #72, RQ4
  ADD      R15, RQ4
  BSR      D_AddFile

L00800BAA:
  MOV      L00C007E1, RQ4
  BSR      M_CheckParm
  MOV      RD2, RD13
  CMP/EQ   RD13
  BT       L00800BB4
  MOV      #1, RD16
  MOV.L    RD16, modifiedgame

L00800BB5:
  ADD      #1, RD13
  MOV.L    myargc, RD16
  CMP/EQ   RD16, RD13
  BT       L00800BB6
  MOV.Q    myargv, RQ16
  EXTS.L   RD13
  MOV.Q    @(RQ16, RD13), RQ12
  MOV.B    @RQ12, RD14
  CMP/EQ   #45, RD14
  BT       L00800BB6
  BRA      L00800BB7

L00800BB7:
  MOV.Q    myargv, RQ25
  EXTS.L   RD13
  MOV.Q    @(RQ25, RD13), RQ12
  MOV      RQ12, RQ4
  BSR      D_AddFile
  BRA      L00800BB5

L00800BB6:

L00800BB4:
  MOV      L00C007E2, RQ4
  BSR      M_CheckParm
  MOV      RD2, RD13
  CMP/EQ   RD13
  BT       L00800BB9
  MOV      L00C007E3, RQ4
  BSR      M_CheckParm
  MOV      RD2, RD13

L00800BB9:
  CMP/EQ   RD13
  BT       L00800BBA
  MOV.L    myargc, RD16
  SUB      RD16, #1, RD14
  CMP/GT   RD13, RD14
  BT       L00800BBA
  BRA      L00800BBB

L00800BBB:
  ADD      RD13, #1, RD14
  MOV.Q    myargv, RQ25
  EXTS.L   RD14
  MOV.Q    @(RQ25, RD14), RQ12
  MOV      #72, RQ4
  ADD      R15, RQ4
  MOV      L00C007E4, RQ5
  MOV      RQ12, RQ6
  BSR      sprintf
  MOV      #72, RQ4
  ADD      R15, RQ4
  BSR      D_AddFile
  ADD      RD13, #1, RD14
  MOV.Q    myargv, RQ26
  EXTS.L   RD14
  MOV.Q    @(RQ26, RD14), RQ12
  MOV      L00C007E5, RQ4
  MOV      RQ12, RQ5
  BSR      printf

L00800BBA:
  MOV      #2, RD25
  MOV      #1, RD26
  MOV      #1, RD27
  MOV      #0, RD28
  MOV.L    RD28, autostart
  MOV.L    RD27, startmap
  MOV.L    RD26, startepisode
  MOV.L    RD25, startskill
  MOV      L00C007E6, RQ4
  BSR      M_CheckParm
  MOV      RD2, RD13
  CMP/EQ   RD13
  BT       L00800BBD
  MOV.L    myargc, RD16
  SUB      RD16, #1, RD14
  CMP/GT   RD13, RD14
  BT       L00800BBD
  BRA      L00800BBE

L00800BBE:
  ADD      RD13, #1, RD14
  MOV.Q    myargv, RQ16
  EXTS.L   RD14
  MOV.Q    @(RQ16, RD14), RQ12
  MOV.B    @RQ12, RD14
  SUB      RD14, #49, RD17
  MOV      #1, RD18
  MOV.L    RD18, autostart
  MOV.L    RD17, startskill

L00800BBD:
  MOV      L00C007E7, RQ4
  BSR      M_CheckParm
  MOV      RD2, RD13
  CMP/EQ   RD13
  BT       L00800BC0
  MOV.L    myargc, RD16
  SUB      RD16, #1, RD14
  CMP/GT   RD13, RD14
  BT       L00800BC0
  BRA      L00800BC1

L00800BC1:
  ADD      RD13, #1, RD14
  MOV.Q    myargv, RQ16
  EXTS.L   RD14
  MOV.Q    @(RQ16, RD14), RQ12
  MOV.B    @RQ12, RD14
  SUB      RD14, #48, RD17
  MOV      #1, RD18
  MOV      #1, RD19
  MOV.L    RD19, autostart
  MOV.L    RD18, startmap
  MOV.L    RD17, startepisode

L00800BC0:
  MOV      L00C007E8, RQ4
  BSR      M_CheckParm
  MOV      RD2, RD13
  CMP/EQ   RD13
  BT       L00800BC3
  MOV.L    myargc, RD16
  SUB      RD16, #1, RD14
  CMP/GT   RD13, RD14
  BT       L00800BC3
  BRA      L00800BC5

L00800BC5:
  MOV.L    deathmatch, RD16
  CMP/EQ   #0, RD16
  BT       L00800BC3
  BRA      L00800BC4

L00800BC4:
  ADD      RD13, #1, RD14
  MOV.Q    myargv, RQ25
  EXTS.L   RD14
  MOV.Q    @(RQ25, RD14), RQ12
  MOV      RQ12, RQ4
  BSR      atoi
  MOV      RD2, RD26
  MOV.L    RD26, @(R15, 56)
  MOV      L00C007E9, RQ4
  MOV      RD26, RD5
  BSR      printf
  CMP/GT   #1, RD26
  BT       L00800BC7
  MOV      L00C007EA, RQ4
  BSR      printf

L00800BC7:
  MOV      L00C007EB, RQ4
  BSR      printf

L00800BC3:
  MOV      L00C007EC, RQ4
  BSR      M_CheckParm
  MOV      RD2, RD13
  CMP/EQ   RD13
  BT       L00800BC8
  MOV.L    myargc, RD16
  SUB      RD16, #1, RD14
  CMP/GT   RD13, RD14
  BT       L00800BC8
  BRA      L00800BCA

L00800BCA:
  MOV.L    deathmatch, RD16
  CMP/EQ   #0, RD16
  BT       L00800BC8
  BRA      L00800BC9

L00800BC9:
  MOV      L00C007ED, RQ4
  BSR      printf

L00800BC8:
  MOV      L00C007EE, RQ4
  BSR      M_CheckParm
  MOV      RD2, RD13
  CMP/EQ   RD13
  BT       L00800BCC
  MOV.L    myargc, RD16
  SUB      RD16, #1, RD14
  CMP/GT   RD13, RD14
  BT       L00800BCC
  BRA      L00800BCD

L00800BCD:
  MOV.L    gamemode, RD16
  CMP/EQ   #2, RD16
  BT       L00800BCF
  ADD      RD13, #1, RD14
  MOV.Q    myargv, RQ25
  EXTS.L   RD14
  MOV.Q    @(RQ25, RD14), RQ12
  MOV      RQ12, RQ4
  BSR      atoi
  MOV      R2, RD26
  MOV.L    RD26, startmap
  BRA      L00800BD0

L00800BCF:
  ADD      RD13, #1, RD14
  MOV.Q    myargv, RQ16
  EXTS.L   RD14
  MOV.Q    @(RQ16, RD14), RQ12
  MOV.B    @RQ12, RD14
  SUB      RD14, #48, RD17
  ADD      RD13, #2, RD14
  EXTS.L   RD14
  MOV.Q    @(RQ16, RD14), RQ12
  MOV.B    @RQ12, RD14
  SUB      RD14, #48, RD18
  MOV.L    RD18, startmap
  MOV.L    RD17, startepisode

L00800BD0:
  MOV      #1, RD16
  MOV.L    RD16, autostart

L00800BCC:
  MOV      L00C007EF, RQ4
  BSR      printf
  BSR      V_Init
  MOV      L00C007F0, RQ4
  BSR      printf
  BSR      M_LoadDefaults
  MOV      L00C007F1, RQ4
  BSR      printf
  BSR      Z_Init
  MOV      L00C007F2, RQ4
  BSR      printf
  MOV      wadfiles, RQ4
  BSR      W_InitMultipleFiles
  MOV.L    modifiedgame, RD16
  CMP/EQ   #0, RD16
  BT       L00800BD1
  MOV.L    gamemode, RD16
  CMP/EQ   #0, RD16
  BT       L00800BD2
  MOV      L00C007F3, RQ4
  BSR      I_Error

L00800BD2:
  MOV.L    gamemode, RD16
  CMP/EQ   #1, RD16
  BT       L00800BD3
  MOV      #0, RD16
  MOV.L    RD16, @(R15, 44)

L00800BD4:
  MOV.L    @(R15, 44), RD16
  MOV      #23, RD17
  CMP/GE   RD17, RD16
  BT       L00800BD6
  MOV      #328, RQ25
  ADD      R15, RQ25
  MOV.L    @(R15, 44), RD26
  EXTS.L   RD26
  LEA.Q    @(RQ25, RD26), RQ12
  MOV      RQ12, RQ4
  BSR      W_CheckNumForName
  MOV      RD2, RD14
  CMP/PZ   RD14
  BT       L00800BD7
  MOV      L00C007F4, RQ4
  BSR      I_Error

L00800BD7:

L00800BD5:
  MOV.L    @(R15, 44), RD16
  ADD      #1, RD16
  MOV.L    RD16, @(R15, 44)
  BRA      L00800BD4

L00800BD6:

L00800BD3:

L00800BD1:
  MOV.L    modifiedgame, RD16
  CMP/EQ   #0, RD16
  BT       L00800BD8
  MOV      L00C007F5, RQ4
  BSR      printf
  BSR      __get_stdin
  MOV      RQ2, RQ12
  MOV.Q    @RQ12, RQ11
  MOV      RQ11, RQ4
  BSR      fgetc

L00800BD8:
  MOV.L    gamemode, RD16
  CMP/EQ   #0, RD16
  BT       L00800BDA
  MOV.L    gamemode, RD16
  CMP/EQ   #6, RD16
  BT       L00800BDB
  MOV.L    gamemode, RD16
  CMP/EQ   #1, RD16
  BT       L00800BDC
  MOV.L    gamemode, RD16
  CMP/EQ   #3, RD16
  BT       L00800BDD
  MOV.L    gamemode, RD16
  CMP/EQ   #2, RD16
  BT       L00800BDE
  BRA      L00800BDF

L00800BDA:

L00800BDB:
  MOV      L00C007F6, RQ4
  BSR      printf
  BRA      L00800BD9

L00800BDC:

L00800BDD:

L00800BDE:
  MOV      L00C007F7, RQ4
  BSR      printf
  BRA      L00800BD9

L00800BDF:
  BRA      L00800BD9

L00800BD9:
  MOV      L00C007F8, RQ4
  BSR      printf
  BSR      M_Init
  MOV      L00C007F9, RQ4
  BSR      printf
  BSR      R_Init
  MOV      L00C007FA, RQ4
  BSR      printf
  BSR      P_Init
  MOV      L00C007FB, RQ4
  BSR      printf
  BSR      I_Init
  MOV      L00C007FC, RQ4
  BSR      printf
  BSR      D_CheckNetGame
  MOV      L00C007FE, RQ4
  BSR      printf
  MOV.L    snd_SfxVolume, RD4
  MOV.L    snd_MusicVolume, RD5
  BSR      S_Init
  MOV      L00C007FF, RQ4
  BSR      printf
  BSR      HU_Init
  MOV      L00C00800, RQ4
  BSR      printf
  BSR      ST_Init
  MOV      L00C00801, RQ4
  BSR      M_CheckParm
  MOV      RD2, RD13
  CMP/EQ   RD13
  BT       L00800BE0
  MOV.L    myargc, RD16
  SUB      RD16, #1, RD14
  CMP/GT   RD13, RD14
  BT       L00800BE0
  BRA      L00800BE1

L00800BE1:
  ADD      RD13, #1, RD14
  MOV.Q    myargv, RQ25
  EXTS.L   RD14
  MOV.Q    @(RQ25, RD14), RQ12
  MOV      RQ12, RQ4
  BSR      G_RecordDemo
  MOV      #1, RD26
  MOV.L    RD26, autostart

L00800BE0:
  MOV      L00C007E2, RQ4
  BSR      M_CheckParm
  MOV      RD2, RD13
  CMP/EQ   RD13
  BT       L00800BE3
  MOV.L    myargc, RD16
  SUB      RD16, #1, RD14
  CMP/GT   RD13, RD14
  BT       L00800BE3
  BRA      L00800BE4

L00800BE4:
  MOV      #1, RD25
  ADD      RD13, #1, RD14
  MOV.Q    myargv, RQ26
  EXTS.L   RD14
  MOV.Q    @(RQ26, RD14), RQ12
  MOV.L    RD25, singledemo
  MOV      RQ12, RQ4
  BSR      G_DeferedPlayDemo
  BSR      D_DoomLoop

L00800BE3:
  MOV      L00C007E3, RQ4
  BSR      M_CheckParm
  MOV      RD2, RD13
  CMP/EQ   RD13
  BT       L00800BE6
  MOV.L    myargc, RD16
  SUB      RD16, #1, RD14
  CMP/GT   RD13, RD14
  BT       L00800BE6
  BRA      L00800BE7

L00800BE7:
  ADD      RD13, #1, RD14
  MOV.Q    myargv, RQ25
  EXTS.L   RD14
  MOV.Q    @(RQ25, RD14), RQ12
  MOV      RQ12, RQ4
  BSR      G_TimeDemo
  BSR      D_DoomLoop

L00800BE6:
  MOV      L00C00803, RQ4
  BSR      M_CheckParm
  MOV      RD2, RD13
  CMP/EQ   RD13
  BT       L00800BE9
  MOV.L    myargc, RD16
  SUB      RD16, #1, RD14
  CMP/GT   RD13, RD14
  BT       L00800BE9
  BRA      L00800BEA

L00800BEA:
  MOV      L00C007D5, RQ4
  BSR      M_CheckParm
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L00800BEC
  ADD      RD13, #1, RD14
  MOV.Q    myargv, RQ25
  EXTS.L   RD14
  MOV.Q    @(RQ25, RD14), RQ12
  MOV.B    @RQ12, RD14
  MOV      #72, RQ4
  ADD      R15, RQ4
  MOV      L00C00804, RQ5
  MOV      RD14, RD6
  BSR      sprintf
  BRA      L00800BED

L00800BEC:
  ADD      RD13, #1, RD14
  MOV.Q    myargv, RQ25
  EXTS.L   RD14
  MOV.Q    @(RQ25, RD14), RQ12
  MOV.B    @RQ12, RD14
  MOV      #72, RQ4
  ADD      R15, RQ4
  MOV      L00C00805, RQ5
  MOV      RD14, RD6
  BSR      sprintf

L00800BED:
  MOV      #72, RQ4
  ADD      R15, RQ4
  BSR      G_LoadGame

L00800BE9:
  BSR      W_Profile
  MOV.L    gameaction, RD16
  CMP/EQ   #3, RD16
  BT       L00800BEE
  MOV.L    autostart, RD16
  CMP/EQ   #0, RD16
  BT       L00800BF0
  MOV.L    netgame, RD16
  CMP/EQ   #0, RD16
  BT       L00800BF0
  BRA      L00800BEF

L00800BF0:
  MOV.L    startskill, RD4
  MOV.L    startepisode, RD5
  MOV.L    startmap, RD6
  BSR      G_InitNew
  BRA      L00800BF2

L00800BEF:
  BSR      D_StartTitle

L00800BF2:

L00800BEE:
  BSR      D_DoomLoop

L00E005B7:
  ADD      #536, R15
  BRA      L00C00626

D_PageTicker:
  PUSH     LR
  PUSH     R14
  PUSH     R13
  ADD      #-120, R15
  MOV.L    pagetic, RD14
  ADD      #-1, RD14
  MOV.L    RD14, pagetic
  MOV.L    pagetic, RD13
  CMP/PZ   RD13
  BT       L00800B61
  BSR      D_AdvanceDemo

L00800B61:

L00E005B8:
  ADD      #120, R15
  POP      RQ13
  POP      RQ14
  RET     

D_PageDrawer:
  PUSH     LR
  ADD      #-136, R15
  MOV      #0, RD4
  MOV      #0, RD5
  MOV      #0, RD6
  MOV.Q    pagename, RQ7
  BSR      V_DrawPatchDirectName

L00E005B9:
  ADD      #136, R15
  RET     

D_AdvanceDemo:
  ADD      #-144, R15
  MOV      #1, RD7
  MOV.L    RD7, advancedemo

L00E005BA:
  ADD      #144, R15
  RTS     

D_StartTitle:
  PUSH     LR
  ADD      #-136, R15
  MOV      #0, RD7
  MOV.L    RD7, gameaction
  MOV      #-1, RD7
  MOV.L    RD7, demosequence
  BSR      D_AdvanceDemo

L00E005BB:
  ADD      #136, R15
  RET     

D_DoomLoop:
  PUSH     LR
  BSR      L00C004F7
  ADD      #-528, R15
  MOV      #60, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 48)
  MOV.L    demorecording, RD13
  CMP/EQ   RD13
  BT       L00800B56
  BSR      G_BeginRecording

L00800B56:
  MOV      L00C0080C, RQ4
  BSR      M_CheckParm
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L00800B57
  MOV      #60, RQ4
  ADD      R15, RQ4
  MOV      L00C0080D, RQ5
  MOV.L    consoleplayer, RD6
  BSR      sprintf
  MOV      L00C0080E, RQ4
  MOV      #60, RQ5
  ADD      R15, RQ5
  BSR      printf
  MOV      #60, RQ4
  ADD      R15, RQ4
  MOV      L00C000A9, RQ5
  BSR      fopen
  MOV      RQ2, RQ12
  MOV.Q    RQ12, debugfile

L00800B57:
  BSR      I_InitGraphics
  MOV      #0, DR15
  MOV      d_snd_acctime, RQ7
  FMOV.D   @RQ7, DR14
  FADD     DR15, DR14
  MOV      d_snd_acctime, RQ7
  FMOV.D   DR14, @RQ7

L00800B58:
  BSR      I_GetTimeMs
  MOV      R2, RD11
  MOV.L    d_main_lasttime, RD9
  SUB      RD11, RD9, RD10
  MOV      RD11, RD9
  MOV.L    RD11, d_main_curtime
  MOV.L    RD10, @(R15, 56)
  MOV.L    RD9, d_main_lasttime
  BSR      I_StartFrame
  MOV.L    singletics, RD8
  CMP/EQ   RD8
  BT       L00800B5A
  BSR      I_StartTic
  BSR      D_ProcessEvents
  MOV      netcmds, RQ31
  MOV.L    consoleplayer, RD30
  EXTS.L   RD30
  MOV      #96, RD7
  MUL.L    RD30, RD7
  STS      R0, RQ29
  ADD      RQ31, RQ29
  MOV.L    maketic, RD28
  MOV      RD28, RD14
  MOV      #0x15555556, RD7
  MOV      #5461, RD7
  DMULS    RD7, RD14
  STS      R1, RD6
  MOV      #12, RD7
  DMULU    RD7, RD6
  STS      R0, RD6
  SUB      RD6, RD14
  EXTS.L   RD14
  LEA.Q    @(RQ29, RD14), RQ27
  MOV.Q    RQ29, @(R15, 32)
  MOV.Q    RQ27, @(R15, 24)
  MOV      RQ27, RQ4
  BSR      G_BuildTiccmd
  MOV.L    advancedemo, RD26
  CMP/EQ   #0, RD26
  BT       L00800B5B
  BSR      D_DoAdvanceDemo

L00800B5B:
  BSR      M_Ticker
  BSR      G_Ticker
  MOV.L    gametic, RD25
  ADD      #1, RD25
  MOV.L    maketic, RD7
  ADD      #1, RD7
  MOV.L    RD25, gametic
  MOV.L    RD7, maketic
  BRA      L00800B5C

L00800B5A:
  BSR      TryRunTics

L00800B5C:
  MOV      players, RQ25
  MOV.L    consoleplayer, RD26
  EXTS.L   RD26
  MOV      #320, RD7
  MUL.L    RD26, RD7
  STS      R0, RQ27
  ADD      RQ25, RQ27
  MOV.Q    @RQ27, RQ28
  MOV.Q    RQ28, @(R15, 32)
  MOV      RQ28, RQ4
  BSR      S_UpdateSounds
  BSR      D_Display
  MOV      d_snd_acctime, RQ7
  FMOV.D   @RQ7, DR13
  MOV.L    @(R15, 56), RD29
  EXTS.L   RD29
  MOV      RD29, R0
  FLDCI    DR12
  FMOV     DR13, DR11
  FADD     DR12, DR11
  FMOV.D   DR13, @(R15, 8)
  MOV      d_snd_acctime, RQ7
  FMOV.D   DR11, @RQ7
  CMP/GT   #72, RD29
  BT       L00800B5D

L00800B5E:
  MOV      d_snd_acctime, RQ7
  FMOV.D   @RQ7, DR10
  MOV      #0, DR9
  FCMP/GT  DR9, DR10
  BT       L00800B5F
  BSR      I_UpdateSound
  MOV      #1, RD4
  BSR      I_SubmitSound2
  MOV      #0xA4A8C158, DR9
  MOV      d_snd_acctime, RQ7
  FMOV.D   @RQ7, DR10
  FSUB     DR9, DR10
  MOV      d_snd_acctime, RQ7
  FMOV.D   DR10, @RQ7
  BRA      L00800B5E

L00800B5F:
  BRA      L00800B60

L00800B5D:
  MOV      #0, RD25
  EXTS.L   RD25
  MOV      RD25, R0
  FLDCI    DR9
  FMOV     DR9, DR10
  MOV      d_snd_acctime, RQ7
  FMOV.D   DR10, @RQ7
  FMOV.D   DR9, @(R15, 8)
  BSR      I_UpdateSound
  BSR      I_SubmitSound

L00800B60:
  BRA      L00800B58

L00800B59:

L00E005BC:
  ADD      #528, R15
  BRA      L00C004F8

D_CheckNetGame:
  PUSH     LR
  BSR      L00C00630
  ADD      #-56, R15
  MOV      #0, RD14

L00800C5B:
  MOV      #8, RD12
  CMP/GE   RD12, RD14
  BT       L00800C5D
  MOV      nodeingame, RQ11
  MOV      #0, RD10
  EXTS.L   RD14
  MOV.L    RD10, @(RQ11, RD14)
  MOV      nettics, RQ9
  EXTS.L   RD14
  MOV.L    RD10, @(RQ9, RD14)
  MOV      remoteresend, RQ8
  EXTS.L   RD14
  MOV.L    RD10, @(RQ8, RD14)
  MOV      resendto, RQ31
  EXTS.L   RD14
  MOV.L    RD10, @(RQ31, RD14)

L00800C5C:
  ADD      #1, RD14
  BRA      L00800C5B

L00800C5D:
  BSR      I_InitNetwork
  MOV.Q    doomcom, RQ30
  MOV.Q    @RQ30, RQ29
  MOV.Q    RQ29, @(R15, 24)
  MOV      #0x12345678, RQ28
  MOV      #4660, RQ28
  CMPQ/EQ  RQ28, RQ29
  BT       L00800C5E
  MOV      L00C00813, RQ4
  BSR      I_Error

L00800C5E:
  MOV.Q    doomcom, RQ27
  ADD      RQ27, #40, RQ26
  MOV      RQ26, RQ25
  MOV.W    @(RQ27, 32), RD13
  MOV      RD13, RD7
  MOV      RD13, RD6
  MOV.Q    RQ25, netbuffer
  MOV.L    RD7, displayplayer
  MOV.L    RD6, consoleplayer
  MOV.L    netgame, RD5
  CMP/EQ   RD5
  BT       L00800C5F
  BSR      D_ArbitrateNetStart

L00800C5F:
  MOV      L00C00815, RQ4
  MOV.L    startskill, RD5
  MOV.L    deathmatch, RD6
  MOV.L    startmap, RD7
  MOV.L    startepisode, RD20
  BSR      printf
  MOV.Q    doomcom, RQ25
  MOV.W    @(RQ25, 18), RD26
  SHAD     RD26, #1, RD13
  MOV      RD13, RD5
  MOV      #12, RD4
  BSR      __sdivsi3
  MOV      RD2, RD27
  SUB      RD27, #1, RD28
  MOV.L    RD28, maxsend
  MOV.L    RD27, @(R15, 8)
  MOV.L    RD26, ticdup
  MOV.L    maxsend, RD29
  MOV      #1, RD30
  CMP/GE   RD30, RD29
  BT       L00800C60
  MOV      #1, RD25
  MOV.L    RD25, maxsend

L00800C60:
  MOV      #0, RD14

L00800C61:
  MOV.Q    doomcom, RQ4
  MOV.W    @(RQ4, 34), RD13
  MOV      RD14, RD3
  EXTS.W   RD13, RD2
  MOV.L    RD3, @(R15, 8)
  MOV.L    RD2, @(R15, 4)
  CMP/GT   RD3, RD2
  BT       L00800C63
  MOV      playeringame, RQ23
  MOV      #1, RD22
  EXTS.L   RD14
  MOV.L    RD22, @(RQ23, RD14)

L00800C62:
  ADD      #1, RD14
  BRA      L00800C61

L00800C63:
  MOV      #0, RD14

L00800C64:
  MOV.Q    doomcom, RQ21
  MOV.W    @(RQ21, 16), RD13
  MOV      RD14, RD20
  EXTS.W   RD13, RD19
  MOV.L    RD20, @(R15, 8)
  MOV.L    RD19, @(R15, 4)
  CMP/GT   RD20, RD19
  BT       L00800C66
  MOV      nodeingame, RQ18
  MOV      #1, RD17
  EXTS.L   RD14
  MOV.L    RD17, @(RQ18, RD14)

L00800C65:
  ADD      #1, RD14
  BRA      L00800C64

L00800C66:
  MOV.Q    doomcom, RQ25
  MOV.W    @(RQ25, 16), RD13
  MOV.W    @(RQ25, 34), RD26
  MOV.L    consoleplayer, RD27
  ADD      RD27, #1, RD28
  MOV.L    RD28, @(R15, 4)
  MOV.L    RD26, @(R15, 8)
  MOV      L00C00817, RQ4
  MOV      RD28, RD5
  MOV      RD26, RD6
  MOV      RD13, RD7
  BSR      printf

L00E005BD:
  ADD      #56, R15
  BRA      L00C00626

D_ProcessEvents:
  PUSH     LR
  BSR      L00C0074E
  ADD      #-1208, R15
  MOV.L    gamemode, RD12
  CMP/EQ   #2, RD12
  BT       L00800B1C
  MOV      L00C00818, RQ4
  BSR      W_CheckNumForName
  MOV      RD2, RD14
  CMP/PZ   RD14
  BT       L00800B1C
  BRA      L00800B1D

L00800B1D:
  BRA      L00E005BE

L00800B1C:

L00800B1F:
  MOV.L    eventtail, RD11
  MOV.L    eventhead, RD10
  CMP/EQ   RD10, RD11
  BT       L00800B21
  MOV      events, RQ9
  MOV.L    eventtail, RD8
  SHAD     RD8, #4, RQ31
  ADD      RQ9, RQ31
  MOV      RQ31, RQ13
  MOV      RQ13, RQ4
  BSR      M_Responder
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L00800B22
  BRA      L00800B20

L00800B22:
  MOV      RQ13, RQ4
  BSR      G_Responder

L00800B20:
  MOV.L    eventtail, RD7
  ADD      #1, RD7
  AND      #63, RD7
  MOV.L    RD7, eventtail
  BRA      L00800B1F

L00800B21:

L00E005BE:
  ADD      #1208, R15
  BRA      L00C00751

G_BuildTiccmd:
  PUSH     LR
  BSR      L00C00630
  ADD      #-120, R15
  MOV.Q    RQ4, @(R15, 64)
  BSR      I_BaseTiccmd
  MOV      RQ2, RQ11
  MOV      #8, RQ10
  EXTS.L   RQ10, RQ10
  MOV.Q    RQ10, @(R15, 32)
  MOV.Q    @(R15, 64), RQ4
  MOV      RQ11, RQ5
  MOV      RQ10, RQ6
  BSR      memcpy
  MOV      consistancy, RQ9
  MOV.L    consoleplayer, RD8
  MOV      #24, RD7
  MUL.L    RD8, RD7
  STS      R0, RQ31
  ADD      RQ9, RQ31
  MOV.L    maketic, RD30
  MOV      RD30, RD14
  MOV      #0x15555556, RD7
  MOV      #5461, RD7
  DMULS    RD7, RD14
  STS      R1, RD6
  MOV      #12, RD7
  DMULU    RD7, RD6
  STS      R0, RD6
  SUB      RD6, RD14
  EXTS.L   RD14
  MOV.W    @(RQ31, RD14), RD13
  MOV.Q    @(R15, 64), RQ29
  MOV.W    RD13, @(RQ29, 4)
  MOV      gamekeydown, RQ28
  MOV.L    key_strafe, RD27
  EXTS.L   RD27
  MOV.L    @(RQ28, RD27), RD13
  CMP/EQ   RD13
  BT       L00800CAB
  MOV.Q    mousebuttons, RQ26
  MOV.L    mousebstrafe, RD25
  EXTS.L   RD25
  MOV.L    @(RQ26, RD25), RD13
  CMP/EQ   RD13
  BT       L00800CAB
  BRA      L00800CAD

L00800CAD:
  MOV.Q    joybuttons, RQ7
  MOV.L    joybstrafe, RD6
  MOV.L    @(RQ7, RD6), RD13
  CMP/EQ   RD13
  BT       L00800CAB
  MOV      #0, RD14
  BRA      L00800CAC

L00800CAB:
  MOV      #1, RD14

L00800CAC:
  MOV      RD14, RD5
  MOV      gamekeydown, RQ4
  MOV.L    key_speed, RD3
  MOV.L    @(RQ4, RD3), RD13
  MOV.L    RD5, @(R15, 56)
  CMP/EQ   RD13
  BT       L00800CB0
  MOV.Q    joybuttons, RQ2
  MOV.L    joybspeed, RD23
  EXTS.L   RD23
  MOV.L    @(RQ2, RD23), RD13
  CMP/EQ   RD13
  BT       L00800CB0
  MOV      #0, RD14
  BRA      L00800CB1

L00800CB0:
  MOV      #1, RD14

L00800CB1:
  MOV      RD14, RD22
  MOV      #0, RD12
  MOV      #0, RD21
  MOV.L    RD22, @(R15, 48)
  MOV.L    RD21, @(R15, 40)
  MOV.L    joyxmove, RD20
  MOV      #0, RD19
  CMP/GE   RD19, RD20
  BT       L00800CB5
  MOV.L    joyxmove, RD18
  CMP/GT   #0, RD18
  BT       L00800CB5
  BRA      L00800CB7

L00800CB7:
  MOV      gamekeydown, RQ17
  MOV.L    key_right, RD16
  MOV.L    @(RQ17, RD16), RD14
  CMP/EQ   RD14
  BT       L00800CB5
  BRA      L00800CB6

L00800CB6:
  MOV      gamekeydown, RQ16
  MOV.L    key_left, RD17
  EXTS.L   RD17
  MOV.L    @(RQ16, RD17), RD14
  CMP/EQ   RD14
  BT       L00800CB5
  BRA      L00800CB4

L00800CB5:
  MOV.L    ticdup, RD16
  MOV.L    turnheld, RD17
  ADD      RD16, RD17
  MOV.L    RD17, turnheld
  BRA      L00800CB9

L00800CB4:
  MOV      #0, RD16
  MOV.L    RD16, turnheld

L00800CB9:
  MOV.L    turnheld, RD16
  MOV      #6, RD17
  CMP/GE   RD17, RD16
  BT       L00800CBA
  MOV      #2, RD16
  MOV.L    RD16, @(R15, 44)
  BRA      L00800CBB

L00800CBA:
  MOV.L    @(R15, 48), RD17
  MOV      RD17, RD16
  MOV.L    RD16, @(R15, 44)

L00800CBB:
  MOV.L    @(R15, 56), RD16
  CMP/EQ   #0, RD16
  BT       L00800CBC
  MOV      gamekeydown, RQ16
  MOV.L    key_right, RD17
  EXTS.L   RD17
  MOV.L    @(RQ16, RD17), RD14
  CMP/EQ   RD14
  BT       L00800CBD
  MOV      sidemove, RQ16
  MOV.L    @(R15, 48), RD17
  EXTS.L   RD17
  MOV.L    @(RQ16, RD17), RD14
  ADD      RD14, RD12

L00800CBD:
  MOV      gamekeydown, RQ16
  MOV.L    key_left, RD17
  EXTS.L   RD17
  MOV.L    @(RQ16, RD17), RD14
  CMP/EQ   RD14
  BT       L00800CBE
  MOV      sidemove, RQ16
  MOV.L    @(R15, 48), RD17
  EXTS.L   RD17
  MOV.L    @(RQ16, RD17), RD14
  SUB      RD14, RD12

L00800CBE:
  MOV.L    joyxmove, RD16
  CMP/GT   #0, RD16
  BT       L00800CBF
  MOV      sidemove, RQ16
  MOV.L    @(R15, 48), RD17
  EXTS.L   RD17
  MOV.L    @(RQ16, RD17), RD14
  ADD      RD14, RD12

L00800CBF:
  MOV.L    joyxmove, RD16
  MOV      #0, RD17
  CMP/GE   RD17, RD16
  BT       L00800CC0
  MOV      sidemove, RQ16
  MOV.L    @(R15, 48), RD17
  EXTS.L   RD17
  MOV.L    @(RQ16, RD17), RD14
  SUB      RD14, RD12

L00800CC0:
  BRA      L00800CC1

L00800CBC:
  MOV      gamekeydown, RQ16
  MOV.L    key_right, RD17
  EXTS.L   RD17
  MOV.L    @(RQ16, RD17), RD14
  CMP/EQ   RD14
  BT       L00800CC2
  MOV.Q    @(R15, 64), RQ16
  MOV.W    @(RQ16, 2), RD14
  MOV      angleturn, RQ17
  MOV.L    @(R15, 44), RD18
  EXTS.L   RD18
  MOV.L    @(RQ17, RD18), RD13
  SUB      RD14, RD13, RD19
  MOV.W    RD19, @(RQ16, 2)
  MOV.L    RD19, @(R15, 12)

L00800CC2:
  MOV      gamekeydown, RQ16
  MOV.L    key_left, RD17
  EXTS.L   RD17
  MOV.L    @(RQ16, RD17), RD14
  CMP/EQ   RD14
  BT       L00800CC3
  MOV.Q    @(R15, 64), RQ16
  MOV.W    @(RQ16, 2), RD14
  MOV      angleturn, RQ17
  MOV.L    @(R15, 44), RD18
  EXTS.L   RD18
  MOV.L    @(RQ17, RD18), RD13
  ADD      RD14, RD13, RD19
  MOV.W    RD19, @(RQ16, 2)
  MOV.L    RD19, @(R15, 12)

L00800CC3:
  MOV.L    joyxmove, RD16
  CMP/GT   #0, RD16
  BT       L00800CC4
  MOV.Q    @(R15, 64), RQ16
  MOV.W    @(RQ16, 2), RD14
  MOV      angleturn, RQ17
  MOV.L    @(R15, 44), RD18
  EXTS.L   RD18
  MOV.L    @(RQ17, RD18), RD13
  SUB      RD14, RD13, RD19
  MOV.W    RD19, @(RQ16, 2)
  MOV.L    RD19, @(R15, 12)

L00800CC4:
  MOV.L    joyxmove, RD16
  MOV      #0, RD17
  CMP/GE   RD17, RD16
  BT       L00800CC5
  MOV.Q    @(R15, 64), RQ16
  MOV.W    @(RQ16, 2), RD14
  MOV      angleturn, RQ17
  MOV.L    @(R15, 44), RD18
  EXTS.L   RD18
  MOV.L    @(RQ17, RD18), RD13
  ADD      RD14, RD13, RD19
  MOV.W    RD19, @(RQ16, 2)
  MOV.L    RD19, @(R15, 12)

L00800CC5:

L00800CC1:
  MOV      gamekeydown, RQ16
  MOV.L    key_up, RD17
  EXTS.L   RD17
  MOV.L    @(RQ16, RD17), RD14
  CMP/EQ   RD14
  BT       L00800CC6
  MOV      forwardmove, RQ16
  MOV.L    @(R15, 48), RD17
  EXTS.L   RD17
  MOV.L    @(RQ16, RD17), RD14
  MOV.L    @(R15, 40), RD18
  ADD      RD14, RD18
  MOV.L    RD18, @(R15, 40)

L00800CC6:
  MOV      gamekeydown, RQ16
  MOV.L    key_down, RD17
  EXTS.L   RD17
  MOV.L    @(RQ16, RD17), RD14
  CMP/EQ   RD14
  BT       L00800CC7
  MOV      forwardmove, RQ16
  MOV.L    @(R15, 48), RD17
  EXTS.L   RD17
  MOV.L    @(RQ16, RD17), RD14
  MOV.L    @(R15, 40), RD18
  SUB      RD14, RD18
  MOV.L    RD18, @(R15, 40)

L00800CC7:
  MOV.L    joyymove, RD16
  MOV      #0, RD17
  CMP/GE   RD17, RD16
  BT       L00800CC8
  MOV      forwardmove, RQ16
  MOV.L    @(R15, 48), RD17
  EXTS.L   RD17
  MOV.L    @(RQ16, RD17), RD14
  MOV.L    @(R15, 40), RD18
  ADD      RD14, RD18
  MOV.L    RD18, @(R15, 40)

L00800CC8:
  MOV.L    joyymove, RD16
  CMP/GT   #0, RD16
  BT       L00800CC9
  MOV      forwardmove, RQ16
  MOV.L    @(R15, 48), RD17
  EXTS.L   RD17
  MOV.L    @(RQ16, RD17), RD14
  MOV.L    @(R15, 40), RD18
  SUB      RD14, RD18
  MOV.L    RD18, @(R15, 40)

L00800CC9:
  MOV      gamekeydown, RQ16
  MOV.L    key_straferight, RD17
  EXTS.L   RD17
  MOV.L    @(RQ16, RD17), RD14
  CMP/EQ   RD14
  BT       L00800CCA
  MOV      sidemove, RQ16
  MOV.L    @(R15, 48), RD17
  EXTS.L   RD17
  MOV.L    @(RQ16, RD17), RD14
  ADD      RD14, RD12

L00800CCA:
  MOV      gamekeydown, RQ16
  MOV.L    key_strafeleft, RD17
  EXTS.L   RD17
  MOV.L    @(RQ16, RD17), RD14
  CMP/EQ   RD14
  BT       L00800CCB
  MOV      sidemove, RQ16
  MOV.L    @(R15, 48), RD17
  EXTS.L   RD17
  MOV.L    @(RQ16, RD17), RD14
  SUB      RD14, RD12

L00800CCB:
  BSR      HU_dequeueChatChar
  MOV      RD2, RD14
  MOV.Q    @(R15, 64), RQ25
  EXTU.B   RD14, RD14
  MOV.B    RD14, @(RQ25, 6)
  MOV      gamekeydown, RQ26
  MOV.L    key_fire, RD27
  EXTS.L   RD27
  MOV.L    @(RQ26, RD27), RD14
  CMP/EQ   RD14
  BT       L00800CCD
  MOV.Q    mousebuttons, RQ16
  MOV.L    mousebfire, RD17
  EXTS.L   RD17
  MOV.L    @(RQ16, RD17), RD14
  CMP/EQ   RD14
  BT       L00800CCD
  BRA      L00800CCE

L00800CCE:
  MOV.Q    joybuttons, RQ16
  MOV.L    joybfire, RD17
  EXTS.L   RD17
  MOV.L    @(RQ16, RD17), RD14
  CMP/EQ   RD14
  BT       L00800CCD
  BRA      L00800CCC

L00800CCD:
  MOV.Q    @(R15, 64), RQ16
  MOV.B    @(RQ16, 7), RD14
  EXTU.B   RD14, RD14
  OR       RD14, #1, RD13
  EXTU.B   RD13, RD13
  MOV.B    RD13, @(RQ16, 7)

L00800CCC:
  MOV      gamekeydown, RQ16
  MOV.L    key_use, RD17
  EXTS.L   RD17
  MOV.L    @(RQ16, RD17), RD14
  CMP/EQ   RD14
  BT       L00800CD1
  MOV.Q    joybuttons, RQ16
  MOV.L    joybuse, RD17
  EXTS.L   RD17
  MOV.L    @(RQ16, RD17), RD14
  CMP/EQ   RD14
  BT       L00800CD1
  BRA      L00800CD0

L00800CD1:
  MOV.Q    @(R15, 64), RQ16
  MOV.B    @(RQ16, 7), RD14
  EXTU.B   RD14, RD14
  OR       RD14, #2, RD13
  EXTU.B   RD13, RD13
  MOV.B    RD13, @(RQ16, 7)
  MOV      #0, RD17
  MOV.L    RD17, dclicks

L00800CD0:
  MOV      #0, RD16
  MOV.L    RD16, @(R15, 60)

L00800CD3:
  MOV.L    @(R15, 60), RD16
  MOV      #8, RD17
  CMP/GE   RD17, RD16
  BT       L00800CD5
  MOV.L    @(R15, 60), RD16
  ADD      RD16, #49, RD14
  MOV      gamekeydown, RQ17
  EXTS.L   RD14
  MOV.L    @(RQ17, RD14), RD13
  CMP/EQ   RD13
  BT       L00800CD6
  MOV.Q    @(R15, 64), RQ16
  MOV.B    @(RQ16, 7), RD14
  EXTU.B   RD14, RD14
  OR       RD14, #4, RD13
  EXTU.B   RD13, RD13
  MOV.B    RD13, @(RQ16, 7)
  MOV.B    @(RQ16, 7), RD14
  EXTU.B   RD14, RD14
  MOV.L    @(R15, 60), RD17
  SHAD     RD17, #3, RD13
  OR       RD14, RD13, RD18
  EXTU.B   RD18, RD18
  MOV.B    RD18, @(RQ16, 7)
  MOV.L    RD18, @(R15, 12)
  BRA      L00800CD5

L00800CD6:

L00800CD4:
  MOV.L    @(R15, 60), RD16
  ADD      #1, RD16
  MOV.L    RD16, @(R15, 60)
  BRA      L00800CD3

L00800CD5:
  MOV.Q    mousebuttons, RQ16
  MOV.L    mousebforward, RD17
  EXTS.L   RD17
  MOV.L    @(RQ16, RD17), RD14
  CMP/EQ   RD14
  BT       L00800CD7
  MOV      forwardmove, RQ16
  MOV.L    @(R15, 48), RD17
  EXTS.L   RD17
  MOV.L    @(RQ16, RD17), RD14
  MOV.L    @(R15, 40), RD18
  ADD      RD14, RD18
  MOV.L    RD18, @(R15, 40)

L00800CD7:
  MOV.Q    mousebuttons, RQ16
  MOV.L    mousebforward, RD17
  EXTS.L   RD17
  MOV.L    @(RQ16, RD17), RD14
  MOV.L    dclickstate, RD18
  CMP/EQ   RD18, RD14
  BT       L00800CD8
  MOV.L    dclicktime, RD16
  CMP/GT   #1, RD16
  BT       L00800CD8
  BRA      L00800CD9

L00800CD9:
  MOV.Q    mousebuttons, RQ16
  MOV.L    mousebforward, RD17
  EXTS.L   RD17
  MOV.L    @(RQ16, RD17), RD18
  MOV.L    RD18, dclickstate
  MOV.L    dclickstate, RD19
  CMP/EQ   #0, RD19
  BT       L00800CDB
  MOV.L    dclicks, RD16
  ADD      #1, RD16
  MOV.L    RD16, dclicks

L00800CDB:
  MOV.L    dclicks, RD16
  CMP/EQ   #2, RD16
  BT       L00800CDC
  MOV.Q    @(R15, 64), RQ16
  MOV.B    @(RQ16, 7), RD14
  EXTU.B   RD14, RD14
  OR       RD14, #2, RD13
  EXTU.B   RD13, RD13
  MOV.B    RD13, @(RQ16, 7)
  MOV      #0, RD17
  MOV.L    RD17, dclicks
  BRA      L00800CDD

L00800CDC:
  MOV      #0, RD16
  MOV.L    RD16, dclicktime

L00800CDD:
  BRA      L00800CDE

L00800CD8:
  MOV.L    ticdup, RD16
  MOV.L    dclicktime, RD17
  ADD      RD16, RD17
  MOV.L    RD17, dclicktime
  MOV.L    dclicktime, RD18
  CMP/GT   #20, RD18
  BT       L00800CDF
  MOV      #0, RD16
  MOV      #0, RD17
  MOV.L    RD17, dclickstate
  MOV.L    RD16, dclicks

L00800CDF:

L00800CDE:
  MOV.Q    mousebuttons, RQ16
  MOV.L    mousebstrafe, RD17
  EXTS.L   RD17
  MOV.L    @(RQ16, RD17), RD13
  CMP/EQ   RD13
  BT       L00800CE0
  MOV.Q    joybuttons, RQ16
  MOV.L    joybstrafe, RD17
  EXTS.L   RD17
  MOV.L    @(RQ16, RD17), RD13
  CMP/EQ   RD13
  BT       L00800CE0
  MOV      #0, RD14
  BRA      L00800CE1

L00800CE0:
  MOV      #1, RD14

L00800CE1:
  MOV      RD14, RD16
  MOV.L    RD16, @(R15, 52)
  MOV.L    dclickstate2, RD17
  CMP/EQ   RD17, RD16
  BT       L00800CE4
  MOV.L    dclicktime2, RD16
  CMP/GT   #1, RD16
  BT       L00800CE4
  BRA      L00800CE5

L00800CE5:
  MOV.L    @(R15, 52), RD17
  MOV      RD17, RD16
  MOV.L    RD16, dclickstate2
  MOV.L    dclickstate2, RD18
  CMP/EQ   #0, RD18
  BT       L00800CE7
  MOV.L    dclicks2, RD16
  ADD      #1, RD16
  MOV.L    RD16, dclicks2

L00800CE7:
  MOV.L    dclicks2, RD16
  CMP/EQ   #2, RD16
  BT       L00800CE8
  MOV.Q    @(R15, 64), RQ16
  MOV.B    @(RQ16, 7), RD14
  EXTU.B   RD14, RD14
  OR       RD14, #2, RD13
  EXTU.B   RD13, RD13
  MOV.B    RD13, @(RQ16, 7)
  MOV      #0, RD17
  MOV.L    RD17, dclicks2
  BRA      L00800CE9

L00800CE8:
  MOV      #0, RD16
  MOV.L    RD16, dclicktime2

L00800CE9:
  BRA      L00800CEA

L00800CE4:
  MOV.L    ticdup, RD16
  MOV.L    dclicktime2, RD17
  ADD      RD16, RD17
  MOV.L    RD17, dclicktime2
  MOV.L    dclicktime2, RD18
  CMP/GT   #20, RD18
  BT       L00800CEB
  MOV      #0, RD16
  MOV      #0, RD17
  MOV.L    RD17, dclickstate2
  MOV.L    RD16, dclicks2

L00800CEB:

L00800CEA:
  MOV.L    mousey, RD16
  MOV.L    @(R15, 40), RD17
  ADD      RD16, RD17
  MOV.L    RD17, @(R15, 40)
  MOV.L    @(R15, 56), RD18
  CMP/EQ   #0, RD18
  BT       L00800CEC
  MOV.L    mousex, RD16
  SHAD     RD16, #1, RD14
  ADD      RD14, RD12
  BRA      L00800CED

L00800CEC:
  MOV.Q    @(R15, 64), RQ16
  MOV.W    @(RQ16, 2), RD14
  MOV.L    mousex, RD17
  SHAD     RD17, #3, RD13
  SUB      RD14, RD13, RD18
  MOV.W    RD18, @(RQ16, 2)
  MOV.L    RD18, @(R15, 12)

L00800CED:
  MOV      #0, RD16
  MOV      #0, RD17
  MOV      forwardmove, RQ18
  MOV.L    @(RQ18, 4), RD14
  MOV.L    RD17, mousex
  MOV.L    RD16, mousey
  MOV.L    @(R15, 40), RD19
  CMP/GT   RD14, RD19
  BT       L00800CEE
  MOV      forwardmove, RQ16
  MOV.L    @(RQ16, 4), RD17
  MOV.L    RD17, @(R15, 40)
  BRA      L00800CEF

L00800CEE:
  MOV      forwardmove, RQ16
  MOV.L    @(RQ16, 4), RD14
  NEG      RD14, RD13
  MOV.L    @(R15, 40), RD17
  CMP/GT   RD17, RD13
  BT       L00800CF0
  MOV      forwardmove, RQ16
  MOV.L    @(RQ16, 4), RD14
  NEG      RD14, RD17
  MOV.L    RD17, @(R15, 40)

L00800CF0:

L00800CEF:
  MOV      forwardmove, RQ16
  MOV.L    @(RQ16, 4), RD14
  CMP/GT   RD14, RD12
  BT       L00800CF1
  MOV      forwardmove, RQ16
  MOV.L    @(RQ16, 4), RD12
  BRA      L00800CF2

L00800CF1:
  MOV      forwardmove, RQ16
  MOV.L    @(RQ16, 4), RD14
  NEG      RD14, RD13
  CMP/GT   RD12, RD13
  BT       L00800CF3
  MOV      forwardmove, RQ16
  MOV.L    @(RQ16, 4), RD14
  NEG      RD14, RD12

L00800CF3:

L00800CF2:
  MOV.Q    @(R15, 64), RQ16
  MOV.B    @RQ16, RD14
  MOV.L    @(R15, 40), RD17
  ADD      RD14, RD17, RD13
  MOV.B    RD13, @RQ16
  MOV.B    @(RQ16, 1), RD14
  ADD      RD14, RD12, RD13
  MOV.B    RD13, @(RQ16, 1)
  MOV.L    sendpause, RD18
  CMP/EQ   #0, RD18
  BT       L00800CF4
  MOV      #0, RD16
  MOV      #129, RD17
  MOV.Q    @(R15, 64), RQ18
  EXTU.B   RD17, RD17
  MOV.B    RD17, @(RQ18, 7)
  MOV.L    RD16, sendpause

L00800CF4:
  MOV.L    sendsave, RD16
  CMP/EQ   #0, RD16
  BT       L00800CF5
  MOV      #0, RD16
  MOV.L    savegameslot, RD17
  SHAD     RD17, #2, RD14
  OR       RD14, #130, RD13
  MOV.Q    @(R15, 64), RQ18
  EXTU.B   RD13, RD13
  MOV.B    RD13, @(RQ18, 7)
  MOV.L    RD16, sendsave

L00800CF5:

L00E005BF:
  ADD      #120, R15
  BRA      L00C00626

D_DoAdvanceDemo:
  PUSH     LR
  BSR      L00C00630
  ADD      #-360, R15
  MOV      players, RQ13
  MOV.L    consoleplayer, RD12
  MOV      #320, RD7
  MUL.L    RD12, RD7
  STS      R0, RQ11
  ADD      RQ13, RQ11
  MOV      #0, RD10
  MOV.L    RD10, @(RQ11, 8)
  MOV      #0, RD9
  MOV      #0, RD8
  MOV      #0, RD31
  MOV      #0, RD30
  MOV.L    RD9, advancedemo
  MOV.L    RD8, usergame
  MOV.L    RD31, paused
  MOV.L    RD30, gameaction
  MOV.L    gamemode, RD29
  CMP/EQ   #3, RD29
  BT       L00800B62
  MOV.L    demosequence, RD28
  ADD      RD28, #1, RD14
  MOV      RD14, RD28
  MOV      #0x24924925, RD7
  MOV      #9362, RD7
  DMULS    RD7, RD28
  STS      R1, RD6
  MOV      #7, RD7
  DMULU    RD7, RD6
  STS      R0, RD6
  SUB      RD6, RD28
  MOV.L    RD28, demosequence
  BRA      L00800B63

L00800B62:
  MOV.L    demosequence, RD27
  ADD      RD27, #1, RD14
  MOV      RD14, RD27
  MOV      #0x2AAAAAAB, RD7
  MOV      #10922, RD7
  DMULS    RD7, RD27
  STS      R1, RD6
  MOV      #6, RD7
  DMULU    RD7, RD6
  STS      R0, RD6
  SUB      RD6, RD27
  MOV.L    RD27, demosequence

L00800B63:
  MOV.L    demosequence, RD26
  CMP/EQ   #0, RD26
  BT       L00800B65
  MOV.L    demosequence, RD25
  CMP/EQ   #1, RD25
  BT       L00800B66
  MOV.L    demosequence, RD7
  CMP/EQ   #2, RD7
  BT       L00800B67
  MOV.L    demosequence, RD6
  CMP/EQ   #3, RD6
  BT       L00800B68
  MOV.L    demosequence, RD5
  CMP/EQ   #4, RD5
  BT       L00800B69
  MOV.L    demosequence, RD4
  CMP/EQ   #5, RD4
  BT       L00800B6A
  MOV.L    demosequence, RD3
  CMP/EQ   #6, RD3
  BT       L00800B6B
  BRA      L00800B64

L00800B65:
  MOV.L    gamemode, RD2
  CMP/EQ   #2, RD2
  BT       L00800B6C
  MOV      #385, RD23
  MOV.L    RD23, pagetic
  BRA      L00800B6D

L00800B6C:
  MOV      #170, RD22
  MOV.L    RD22, pagetic

L00800B6D:
  MOV      #3, RD25
  MOV      L00C00832, RQ27
  MOV      RQ27, RQ26
  MOV.Q    RQ26, pagename
  MOV.L    RD25, gamestate
  MOV.Q    pagename, RQ4
  BSR      W_CheckNumForName
  MOV      RD2, RD14
  CMP/EQ   #-1, RD14
  BT       L00800B6E
  MOV      L00C00833, RQ20
  MOV      RQ20, RQ21
  MOV.Q    RQ21, pagename

L00800B6E:
  MOV.L    gamemode, RD19
  CMP/EQ   #2, RD19
  BT       L00800B6F
  MOV      #66, RD4
  BSR      S_StartMusic
  BRA      L00800B70

L00800B6F:
  MOV      #29, RD4
  BSR      S_StartMusic

L00800B70:
  BRA      L00800B64

L00800B66:
  MOV      L00C00834, RQ4
  BSR      G_DeferedPlayDemo
  BRA      L00800B64

L00800B67:
  MOV      #200, RD18
  MOV      #3, RD17
  MOV      L00C005C9, RQ19
  MOV      RQ19, RQ16
  MOV.L    RD18, pagetic
  MOV.L    RD17, gamestate
  MOV.Q    RQ16, pagename
  BRA      L00800B64

L00800B68:
  MOV      L00C00835, RQ4
  BSR      G_DeferedPlayDemo
  BRA      L00800B64

L00800B69:
  MOV      #3, RD16
  MOV.L    RD16, gamestate
  MOV.L    gamemode, RD16
  CMP/EQ   #2, RD16
  BT       L00800B71
  MOV      #385, RD25
  MOV      L00C00832, RQ27
  MOV      RQ27, RQ26
  MOV.Q    RQ26, pagename
  MOV.L    RD25, pagetic
  MOV      #66, RD4
  BSR      S_StartMusic
  BRA      L00800B72

L00800B71:
  MOV      #200, RD16
  MOV.L    RD16, pagetic
  MOV.L    gamemode, RD16
  CMP/EQ   #3, RD16
  BT       L00800B73
  MOV      L00C005C9, RQ17
  MOV      RQ17, RQ16
  MOV.Q    RQ16, pagename
  BRA      L00800B74

L00800B73:
  MOV      L00C005CA, RQ17
  MOV      RQ17, RQ16
  MOV.Q    RQ16, pagename

L00800B74:

L00800B72:
  BRA      L00800B64

L00800B6A:
  MOV      L00C00836, RQ4
  BSR      G_DeferedPlayDemo
  BRA      L00800B64

L00800B6B:
  MOV      L00C00837, RQ4
  BSR      G_DeferedPlayDemo
  BRA      L00800B64

L00800B64:

L00E005C0:
  ADD      #360, R15
  BRA      L00C00626

R_ExecuteSetViewSize:
  PUSH     LR
  BSR      L00C00630
  ADD      #-472, R15
  MOV      #0, RD10
  MOV.L    RD10, setsizeneeded
  MOV.L    setblocks, RD9
  CMP/EQ   #11, RD9
  BT       L00801EC5
  MOV      #320, RD8
  MOV      #200, RD31
  MOV.L    RD8, scaledviewwidth
  MOV.L    RD31, viewheight
  BRA      L00801EC6

L00801EC5:
  MOV.L    setblocks, RD30
  SHAD     RD30, #5, RD29
  MOV      RD30, RD14
  MOV      #168, RD28
  MUL.L    RD28, RD14, RD14
  MOV      RD14, RD11
  MOV      #0x1999999A, RD7
  MOV      #6553, RD7
  DMULS    RD7, RD11
  STS      R1, RD11
  AND      RD11, #-8, RD27
  MOV.L    RD29, scaledviewwidth
  MOV.L    RD27, viewheight

L00801EC6:
  MOV.L    setdetail, RD25
  MOV      RD25, RD26
  MOV.L    scaledviewwidth, RD6
  SHAR     RD6, RD26, RD7
  MOV.L    viewheight, RD5
  SHAD     RD5, #-1, RD4
  SHAD     RD7, #-1, RD3
  SHAD     RD3, #16, RD2
  SHAD     RD4, #16, RD23
  MOV      RD2, RD22
  MOV.L    RD26, detailshift
  MOV.L    RD7, viewwidth
  MOV.L    RD4, centery
  MOV.L    RD3, centerx
  MOV.L    RD2, centerxfrac
  MOV.L    RD23, centeryfrac
  MOV.L    RD22, projection
  MOV.L    detailshift, RD21
  CMP/EQ   #0, RD21
  BT       L00801EC7
  MOV      R_DrawColumn, RQ20
  MOV      RQ20, RQ12
  MOV      RQ12, RQ19
  MOV      RQ20, RQ12
  MOV      RQ12, RQ18
  MOV      R_DrawFuzzColumn, RQ17
  MOV      RQ17, RQ12
  MOV      RQ12, RQ16
  MOV      R_DrawTranslatedColumn, RQ21
  MOV      RQ21, RQ12
  MOV      RQ12, RQ22
  MOV      R_DrawSpan, RQ23
  MOV      RQ23, RQ12
  MOV      RQ12, RQ2
  MOV.Q    RQ2, spanfunc
  MOV.Q    RQ22, transcolfunc
  MOV.Q    RQ19, basecolfunc
  MOV.Q    RQ18, colfunc
  MOV.Q    RQ16, fuzzcolfunc
  BRA      L00801EC8

L00801EC7:
  MOV      R_DrawColumnLow, RQ16
  MOV      RQ16, RQ12
  MOV      RQ12, RQ17
  MOV      RQ16, RQ12
  MOV      RQ12, RQ18
  MOV      R_DrawFuzzColumn, RQ19
  MOV      RQ19, RQ12
  MOV      RQ12, RQ20
  MOV      R_DrawTranslatedColumn, RQ21
  MOV      RQ21, RQ12
  MOV      RQ12, RQ22
  MOV      R_DrawSpanLow, RQ23
  MOV      RQ23, RQ12
  MOV      RQ12, RQ2
  MOV.Q    RQ2, spanfunc
  MOV.Q    RQ22, transcolfunc
  MOV.Q    RQ20, fuzzcolfunc
  MOV.Q    RQ18, colfunc
  MOV.Q    RQ17, basecolfunc

L00801EC8:
  MOV.L    scaledviewwidth, RD4
  MOV.L    viewheight, RD5
  BSR      R_InitBuffer
  BSR      R_InitTextureMapping
  MOV.L    viewwidth, RD25
  SHAD     RD25, #16, RD14
  MOV      RD14, RD26
  MOV      #0x00CCCCCD, RD7
  MOV      #204, RD7
  DMULS    RD7, RD26
  STS      R1, RD7
  MOV      RD7, RD26
  MOV      RD25, RD5
  MOV      #0x01400000, RD4
  MOV      #320, RD4
  BSR      __sdivsi3
  MOV      RD2, RD27
  MOV      #0, RD13
  MOV.L    RD27, pspriteiscale
  MOV.L    RD26, pspritescale

L00801EC9:
  MOV.L    viewwidth, RD16
  CMP/GT   RD13, RD16
  BT       L00801ECB
  MOV      screenheightarray, RQ16
  MOV.L    viewheight, RD17
  EXTS.L   RD13
  MOV.W    RD17, @(RQ16, RD13)

L00801ECA:
  ADD      #1, RD13
  BRA      L00801EC9

L00801ECB:
  MOV      #0, RD13

L00801ECC:
  MOV.L    viewheight, RD16
  CMP/GT   RD13, RD16
  BT       L00801ECE
  MOV.L    viewheight, RD25
  SHAD     RD25, #-1, RD14
  SUB      RD13, RD14, RD11
  SHAD     RD11, #16, RD14
  ADD      RD14, #32768, RD26
  MOV.L    RD26, @(R15, 56)
  MOV      RD26, RD4
  BSR      abs
  MOV      RD2, RD26
  MOV.L    viewwidth, RD27
  MOV.L    detailshift, RD28
  SHAD     RD27, RD28, RD14
  SHAD     RD14, #-1, RD11
  SHAD     RD11, #16, RD14
  MOV.L    RD26, @(R15, 56)
  MOV      RD14, RD4
  MOV      RD26, RD5
  BSR      FixedDiv
  MOV      RD2, RD11
  MOV      yslope, RQ29
  EXTS.L   RD13
  MOV.L    RD11, @(RQ29, RD13)

L00801ECD:
  ADD      #1, RD13
  BRA      L00801ECC

L00801ECE:
  MOV      #0, RD13

L00801ECF:
  MOV.L    viewwidth, RD16
  CMP/GT   RD13, RD16
  BT       L00801ED1
  MOV      xtoviewangle, RQ25
  EXTS.L   RD13
  MOVU.L   @(RQ25, RD13), RD14
  SHLD     RD14, #-19, RD11
  MOV.Q    finecosine, RQ26
  MOV.L    @(RQ26, RD11), RD14
  MOV      RD14, RD4
  BSR      abs
  MOV      RD2, RD27
  MOV.L    RD27, @(R15, 60)
  MOV      #0x00010000, RD4
  MOV      #1, RD4
  MOV      RD27, RD5
  BSR      FixedDiv
  MOV      RD2, RD14
  MOV      distscale, RQ28
  EXTS.L   RD13
  MOV.L    RD14, @(RQ28, RD13)

L00801ED0:
  ADD      #1, RD13
  BRA      L00801ECF

L00801ED1:
  MOV      #0, RD13

L00801ED2:
  CMP/GE   #16, RD13
  BT       L00801ED4
  MOV      #15, RD16
  SUB      RD16, RD13, RD14
  SHAD     RD14, #1, RD11
  SHAD     RD11, #5, RD14
  SHAD     RD14, #-4, RD17
  MOV      #0, RD18
  MOV.L    RD18, @(R15, 52)
  MOV.L    RD17, @(R15, 44)

L00801ED5:
  MOV.L    @(R15, 52), RD16
  MOV      #48, RD17
  CMP/GE   RD17, RD16
  BT       L00801ED7
  MOV.L    @(R15, 52), RD25
  MOV      RD25, RD14
  MOV      #320, RD26
  MUL.L    RD26, RD14, RD14
  MOV.L    viewwidth, RD27
  MOV.L    detailshift, RD28
  SHAD     RD27, RD28, RD11
  MOV      RD11, RD5
  MOV      RD14, RD4
  BSR      __sdivsi3
  MOV      RD2, RD29
  SHAD     RD29, #-1, RD14
  MOV.L    @(R15, 44), RD31
  SUB      RD31, RD14, RD30
  MOV.L    RD30, @(R15, 48)
  MOV      #0, RD8
  CMP/GE   RD8, RD30
  BT       L00801ED8
  MOV      #0, RD16
  MOV.L    RD16, @(R15, 48)

L00801ED8:
  MOV.L    @(R15, 48), RD16
  MOV      #32, RD17
  CMP/GE   RD17, RD16
  BT       L00801ED9
  MOV      #31, RD16
  MOV.L    RD16, @(R15, 48)

L00801ED9:
  MOV.L    @(R15, 48), RD16
  SHAD     RD16, #8, RD14
  MOV.Q    colormaps, RQ17
  LEA.W    @(RQ17, RD14), RQ12
  MOV      scalelight, RQ18
  EXTS.L   RD13
  MOV      RD13, RQ19
  SHLL1    RQ19
  ADD      RD13, RQ19
  SHAD     #7, RQ19
  ADD      RQ18, RQ19
  MOV.L    @(R15, 52), RD20
  EXTS.L   RD20
  MOV.Q    RQ12, @(RQ19, RD20)

L00801ED6:
  MOV.L    @(R15, 52), RD16
  ADD      #1, RD16
  MOV.L    RD16, @(R15, 52)
  BRA      L00801ED5

L00801ED7:

L00801ED3:
  ADD      #1, RD13
  BRA      L00801ED2

L00801ED4:

L00E005C1:
  ADD      #472, R15
  BRA      L00C00626

D_Display:
  PUSH     LR
  BSR      L00C00630
  ADD      #-72, R15
  MOV.L    nodrawers, RD14
  CMP/EQ   RD14
  BT       L00800B23
  BRA      L00E005C2

L00800B23:
  MOV      #0, RD13
  MOV.L    RD13, @(R15, 20)
  MOV.L    setsizeneeded, RD12
  CMP/EQ   RD12
  BT       L00800B24
  BSR      R_ExecuteSetViewSize
  MOV      #-1, RD11
  MOV      #3, RD10
  MOV.L    RD11, oldgamestate
  MOV.L    RD10, borderdrawcount

L00800B24:
  MOV.L    gamestate, RD8
  MOV      RD8, RD9
  MOV.L    RD9, wipegamestate
  MOV.L    gamestate, RD31
  MOV.L    wipegamestate, RD30
  CMP/EQ   RD30, RD31
  BT       L00800B25
  MOV      #1, RD29
  MOV.L    RD29, @(R15, 24)
  MOV      #0, RD4
  MOV      #0, RD5
  MOV      #320, RD6
  MOV      #200, RD7
  BSR      wipe_StartScreen
  BRA      L00800B26

L00800B25:
  MOV      #0, RD28
  MOV.L    RD28, @(R15, 24)

L00800B26:
  MOV.L    gamestate, RD27
  CMP/EQ   #0, RD27
  BT       L00800B27
  MOV.L    gametic, RD26
  CMP/EQ   #0, RD26
  BT       L00800B27
  BRA      L00800B28

L00800B28:
  BSR      HU_Erase

L00800B27:
  MOV.L    gamestate, RD25
  CMP/EQ   #0, RD25
  BT       L00800B2B
  MOV.L    gamestate, RD7
  CMP/EQ   #1, RD7
  BT       L00800B2C
  MOV.L    gamestate, RD6
  CMP/EQ   #2, RD6
  BT       L00800B2D
  MOV.L    gamestate, RD5
  CMP/EQ   #3, RD5
  BT       L00800B2E
  BRA      L00800B2A

L00800B2B:
  MOV.L    gametic, RD4
  CMP/EQ   RD4
  BT       L00800B2F
  BRA      L00800B2A

L00800B2F:
  MOV.L    automapactive, RD3
  CMP/EQ   RD3
  BT       L00800B30
  BSR      AM_Drawer

L00800B30:
  MOV.L    @(R15, 24), RD2
  CMP/EQ   RD2
  BT       L00800B32
  MOV.L    viewheight, RD23
  CMP/EQ   #200, RD23
  BT       L00800B31
  MOV.L    fullscreen, RD22
  CMP/EQ   #0, RD22
  BT       L00800B31
  BRA      L00800B32

L00800B32:
  MOV      #1, RD21
  MOV.L    RD21, @(R15, 20)

L00800B31:
  MOV.L    inhelpscreensstate, RD20
  CMP/EQ   #0, RD20
  BT       L00800B35
  MOV.L    inhelpscreens, RD19
  CMP/EQ   #0, RD19
  BT       L00800B35
  BRA      L00800B36

L00800B36:
  MOV      #1, RD18
  MOV.L    RD18, @(R15, 20)

L00800B35:
  MOV.L    viewheight, RD25
  MOV      #200, RD26
  CMP/EQ   RD26, RD25
  MOVT     RQ7
  MOV      RQ7, RD27
  MOV.L    RD27, @(R15, 16)
  MOV      RD27, RD4
  MOV.L    @(R15, 20), RD5
  BSR      ST_Drawer
  MOV.L    viewheight, RD28
  CMP/EQ   RD26, RD28
  MOVT     RQ7
  MOV      RQ7, RD29
  MOV.L    RD29, fullscreen
  BRA      L00800B2A

L00800B2C:
  BSR      WI_Drawer
  BRA      L00800B2A

L00800B2D:
  BSR      F_Drawer
  BRA      L00800B2A

L00800B2E:
  BSR      D_PageDrawer
  BRA      L00800B2A

L00800B2A:
  BSR      I_UpdateNoBlit
  MOV.L    gamestate, RD17
  CMP/EQ   #0, RD17
  BT       L00800B38
  MOV.L    automapactive, RD16
  CMP/EQ   #0, RD16
  BT       L00800B38
  BRA      L00800B3A

L00800B3A:
  MOV.L    gametic, RD16
  CMP/EQ   #0, RD16
  BT       L00800B38
  BRA      L00800B39

L00800B39:
  MOV      players, RQ25
  MOV.L    displayplayer, RD26
  EXTS.L   RD26
  MOV      #320, RD7
  MUL.L    RD26, RD7
  STS      R0, RQ27
  ADD      RQ25, RQ27
  MOV.Q    RQ27, @(R15, 8)
  MOV      RQ27, RQ4
  BSR      R_RenderPlayerView

L00800B38:
  MOV.L    gamestate, RD16
  CMP/EQ   #0, RD16
  BT       L00800B3C
  MOV.L    gametic, RD16
  CMP/EQ   #0, RD16
  BT       L00800B3C
  BRA      L00800B3D

L00800B3D:
  BSR      HU_Drawer

L00800B3C:
  MOV.L    gamestate, RD16
  MOV.L    oldgamestate, RD17
  CMP/EQ   RD17, RD16
  BT       L00800B3F
  MOV.L    gamestate, RD16
  CMP/EQ   #0, RD16
  BT       L00800B3F
  BRA      L00800B40

L00800B40:
  MOV      L00C00605, RQ4
  MOV      #101, RD5
  BSR      W_CacheLumpName
  MOV      RQ2, RQ25
  MOV.Q    RQ25, @(R15, 8)
  MOV      RQ25, RQ4
  BSR      I_SetPalette

L00800B3F:
  MOV.L    gamestate, RD16
  CMP/EQ   #0, RD16
  BT       L00800B42
  MOV.L    oldgamestate, RD16
  CMP/EQ   #0, RD16
  BT       L00800B42
  BRA      L00800B43

L00800B43:
  MOV      #0, RD25
  MOV.L    RD25, viewactivestate
  BSR      R_FillBackScreen

L00800B42:
  MOV.L    gamestate, RD16
  CMP/EQ   #0, RD16
  BT       L00800B45
  MOV.L    automapactive, RD16
  CMP/EQ   #0, RD16
  BT       L00800B45
  BRA      L00800B47

L00800B47:
  MOV.L    scaledviewwidth, RD16
  CMP/EQ   #320, RD16
  BT       L00800B45
  BRA      L00800B46

L00800B46:
  MOV.L    menuactive, RD16
  CMP/EQ   #0, RD16
  BT       L00800B4A
  MOV.L    menuactivestate, RD16
  CMP/EQ   #0, RD16
  BT       L00800B4A
  BRA      L00800B4B

L00800B4B:
  MOV.L    viewactivestate, RD16
  CMP/EQ   #0, RD16
  BT       L00800B4A
  BRA      L00800B49

L00800B4A:
  MOV      #3, RD16
  MOV.L    RD16, borderdrawcount

L00800B49:
  MOV.L    borderdrawcount, RD16
  CMP/EQ   #0, RD16
  BT       L00800B4D
  BSR      R_DrawViewBorder
  MOV.L    borderdrawcount, RD16
  ADD      #-1, RD16
  MOV.L    RD16, borderdrawcount

L00800B4D:

L00800B45:
  MOV.L    menuactive, RD17
  MOV      RD17, RD16
  MOV.L    viewactive, RD19
  MOV      RD19, RD18
  MOV.L    inhelpscreens, RD21
  MOV      RD21, RD20
  MOV.L    gamestate, RD23
  MOV      RD23, RD22
  MOV      RD23, RD2
  MOV.L    RD2, oldgamestate
  MOV.L    RD22, wipegamestate
  MOV.L    RD20, inhelpscreensstate
  MOV.L    RD18, viewactivestate
  MOV.L    RD16, menuactivestate
  MOV.L    paused, RD3
  CMP/EQ   RD3
  BT       L00800B4E
  MOV.L    automapactive, RD16
  CMP/EQ   #0, RD16
  BT       L00800B4F
  MOV      #4, RD16
  MOV.L    RD16, @(R15, 32)
  BRA      L00800B50

L00800B4F:
  MOV.L    viewwindowy, RD16
  ADD      RD16, #4, RD17
  MOV.L    RD17, @(R15, 32)

L00800B50:
  MOV      L00C00840, RQ4
  MOV      #101, RD5
  BSR      W_CacheLumpName
  MOV      RQ2, RQ25
  MOV.L    scaledviewwidth, RD26
  SUB      RD26, #68, RD27
  SHAD     RD27, #-1, RD28
  MOV.L    viewwindowx, RD29
  ADD      RD29, RD28, RD27
  MOV.L    RD27, @(R15, 16)
  MOV.Q    RQ25, @(R15, 8)
  MOV      RD27, RD4
  MOV.L    @(R15, 32), RD5
  MOV      #0, RD6
  MOV      RQ25, RQ7
  BSR      V_DrawPatchDirect

L00800B4E:
  BSR      M_Drawer
  BSR      NetUpdate
  MOV.L    @(R15, 24), RD16
  CMP/EQ   #0, RD16
  BT       L00800B51
  BSR      I_FinishUpdate
  BRA      L00E005C2

L00800B51:
  MOV      #0, RD4
  MOV      #0, RD5
  MOV      #320, RD6
  MOV      #200, RD7
  BSR      wipe_EndScreen
  BSR      I_GetTime
  MOV      RD2, RD25
  SUB      RD25, #1, RD26
  MOV.L    RD26, @(R15, 36)
  MOV.L    RD25, @(R15, 16)

L00800B52:

L00800B54:
  BSR      I_GetTime
  MOV      RD2, RD25
  MOV.L    @(R15, 36), RD27
  SUB      RD25, RD27, RD26
  MOV.L    RD26, @(R15, 40)
  MOV.L    RD25, @(R15, 44)
  CMP/EQ   #0, RD26
  BT       L00800B54

L00800B55:
  MOV.L    @(R15, 44), RD26
  MOV      RD26, RD25
  MOV.L    RD25, @(R15, 36)
  MOV      #1, RD4
  MOV      #0, RD5
  MOV      #0, RD6
  MOV      #320, RD7
  MOV      #200, RD20
  MOV.L    @(R15, 40), RD21
  BSR      wipe_ScreenWipe
  MOV      RD2, RD27
  MOV.L    RD27, @(R15, 28)
  BSR      I_UpdateNoBlit
  BSR      M_Drawer
  BSR      I_FinishUpdate
  CMP/EQ   #0, RD27
  BT       L00800B52

L00800B53:

L00E005C2:
  ADD      #72, R15
  BRA      L00C00626

IdentifyVersion:
  PUSH     LR
  BSR      L00C00649
  ADD      #-184, R15
  MOV      L00C00090, RQ9
  MOV      RQ9, RQ11
  MOV      RQ11, RQ4
  BSR      strlen
  MOV      RQ2, RQ14
  ADD      RQ14, #1, RQ13
  ADD      RQ13, #9, RQ14
  ADD      RQ14, #1, RQ13
  MOV      RQ13, RQ4
  BSR      malloc
  MOV      RQ2, RQ10
  MOV      RQ10, RQ4
  MOV      L00C00841, RQ5
  MOV      RQ11, RQ6
  BSR      sprintf
  MOV      RQ11, RQ4
  BSR      strlen
  MOV      RQ2, RQ14
  ADD      RQ14, #1, RQ13
  ADD      RQ13, #8, RQ14
  ADD      RQ14, #1, RQ13
  MOV      RQ13, RQ4
  BSR      malloc
  MOV      RQ2, RQ9
  MOV.Q    RQ9, @(R15, 72)
  MOV      RQ9, RQ4
  MOV      L00C00842, RQ5
  MOV      RQ11, RQ6
  BSR      sprintf
  MOV      RQ11, RQ4
  BSR      strlen
  MOV      RQ2, RQ14
  ADD      RQ14, #1, RQ13
  ADD      RQ13, #8, RQ14
  ADD      RQ14, #1, RQ13
  MOV      RQ13, RQ4
  BSR      malloc
  MOV      RQ2, RQ9
  MOV.Q    RQ9, @(R15, 80)
  MOV      RQ9, RQ4
  MOV      L00C00843, RQ5
  MOV      RQ11, RQ6
  BSR      sprintf
  MOV      RQ11, RQ4
  BSR      strlen
  MOV      RQ2, RQ14
  ADD      RQ14, #1, RQ13
  ADD      RQ13, #9, RQ14
  ADD      RQ14, #1, RQ13
  MOV      RQ13, RQ4
  BSR      malloc
  MOV      RQ2, RQ9
  MOV.Q    RQ9, @(R15, 88)
  MOV      RQ9, RQ4
  MOV      L00C00844, RQ5
  MOV      RQ11, RQ6
  BSR      sprintf
  MOV      RQ11, RQ4
  BSR      strlen
  MOV      RQ2, RQ14
  ADD      RQ14, #1, RQ13
  ADD      RQ13, #12, RQ14
  ADD      RQ14, #1, RQ13
  MOV      RQ13, RQ4
  BSR      malloc
  MOV      RQ2, RQ9
  MOV.Q    RQ9, @(R15, 56)
  MOV      RQ9, RQ4
  MOV      L00C00845, RQ5
  MOV      RQ11, RQ6
  BSR      sprintf
  MOV      RQ11, RQ4
  BSR      strlen
  MOV      RQ2, RQ14
  ADD      RQ14, #1, RQ13
  ADD      RQ13, #9, RQ14
  ADD      RQ14, #1, RQ13
  MOV      RQ13, RQ4
  BSR      malloc
  MOV      RQ2, RQ9
  MOV.Q    RQ9, @(R15, 48)
  MOV      RQ9, RQ4
  MOV      L00C00846, RQ5
  MOV      RQ11, RQ6
  BSR      sprintf
  MOV      RQ11, RQ4
  BSR      strlen
  MOV      RQ2, RQ14
  ADD      RQ14, #1, RQ13
  ADD      RQ13, #10, RQ14
  ADD      RQ14, #1, RQ13
  MOV      RQ13, RQ4
  BSR      malloc
  MOV      RQ2, RQ9
  MOV.Q    RQ9, @(R15, 64)
  MOV      RQ9, RQ4
  MOV      L00C00847, RQ5
  MOV      RQ11, RQ6
  BSR      sprintf
  MOV      RQ11, RQ4
  BSR      strlen
  MOV      RQ2, RQ14
  ADD      RQ14, #1, RQ13
  ADD      RQ13, #10, RQ14
  ADD      RQ14, #1, RQ13
  MOV      RQ13, RQ4
  BSR      malloc
  MOV      RQ2, RQ9
  MOV.Q    RQ9, @(R15, 40)
  MOV      RQ9, RQ4
  MOV      L00C00848, RQ5
  MOV      RQ11, RQ6
  BSR      sprintf
  MOV      RQ11, RQ4
  BSR      strlen
  MOV      RQ2, RQ14
  ADD      RQ14, #1, RQ13
  ADD      RQ13, #10, RQ14
  ADD      RQ14, #1, RQ13
  MOV      RQ13, RQ4
  BSR      malloc
  MOV      RQ2, RQ9
  MOV.Q    RQ9, @(R15, 32)
  MOV      RQ9, RQ4
  MOV      L00C00849, RQ5
  MOV      RQ11, RQ6
  BSR      sprintf
  MOV      basedefault, RQ4
  MOV      L00C0084A, RQ5
  BSR      sprintf
  MOV      L00C0084B, RQ4
  BSR      M_CheckParm
  MOV      RD2, RD12
  CMP/EQ   RD12
  BT       L00800B78
  MOV      #0, RD9
  MOV      #1, RD7
  MOV.L    RD7, devparm
  MOV.L    RD9, gamemode
  MOV      L00C0084C, RQ4
  BSR      D_AddFile
  MOV      L00C0084D, RQ4
  BSR      D_AddFile
  MOV      L00C0084E, RQ4
  BSR      D_AddFile
  MOV      basedefault, RQ4
  MOV      L00C0084F, RQ5
  BSR      strcpy
  BRA      L00E005C3

L00800B78:
  MOV      L00C00850, RQ4
  BSR      M_CheckParm
  MOV      RD2, RD12
  CMP/EQ   RD12
  BT       L00800B79
  MOV      #1, RD9
  MOV      #1, RD7
  MOV.L    RD7, devparm
  MOV.L    RD9, gamemode
  MOV      L00C00851, RQ4
  BSR      D_AddFile
  MOV      L00C0084D, RQ4
  BSR      D_AddFile
  MOV      L00C00852, RQ4
  BSR      D_AddFile
  MOV      L00C0084E, RQ4
  BSR      D_AddFile
  MOV      basedefault, RQ4
  MOV      L00C0084F, RQ5
  BSR      strcpy
  BRA      L00E005C3

L00800B79:
  MOV      L00C00853, RQ4
  BSR      M_CheckParm
  MOV      RD2, RD12
  CMP/EQ   RD12
  BT       L00800B7A
  MOV      #2, RD9
  MOV      #1, RD7
  MOV.L    RD7, devparm
  MOV.L    RD9, gamemode
  MOV      L00C00854, RQ4
  BSR      D_AddFile
  MOV      L00C00855, RQ4
  BSR      D_AddFile
  MOV      L00C00856, RQ4
  BSR      D_AddFile
  MOV      basedefault, RQ4
  MOV      L00C0084F, RQ5
  BSR      strcpy
  BRA      L00E005C3

L00800B7A:
  MOV.Q    @(R15, 40), RQ4
  BSR      w_chkaccess
  MOV      RD2, RD12
  CMP/EQ   RD12
  BT       L00800B7B
  MOV      #4, RD9
  MOV.L    RD9, gamemode
  MOV.Q    @(R15, 88), RQ4
  BSR      D_AddFile
  MOV.Q    @(R15, 40), RQ4
  BSR      D_AddFile
  BRA      L00E005C3

L00800B7B:
  MOV.Q    @(R15, 32), RQ4
  BSR      w_chkaccess
  MOV      RD2, RD12
  CMP/EQ   RD12
  BT       L00800B7C
  MOV      #5, RD9
  MOV.L    RD9, gamemode
  MOV      RQ10, RQ4
  BSR      D_AddFile
  MOV.Q    @(R15, 32), RQ4
  BSR      D_AddFile
  BRA      L00E005C3

L00800B7C:
  MOV.Q    @(R15, 64), RQ4
  BSR      w_chkaccess
  MOV      RD2, RD12
  CMP/EQ   RD12
  BT       L00800B7D
  MOV      #2, RD9
  MOV      #1, RD7
  MOV.L    RD7, language
  MOV.L    RD9, gamemode
  MOV      L00C00858, RQ4
  BSR      printf
  MOV.Q    @(R15, 64), RQ4
  BSR      D_AddFile
  BRA      L00E005C3

L00800B7D:
  MOV      RQ10, RQ4
  BSR      w_chkaccess
  MOV      RD2, RD12
  CMP/EQ   RD12
  BT       L00800B7E
  MOV      #2, RD9
  MOV.L    RD9, gamemode
  MOV      RQ10, RQ4
  BSR      D_AddFile
  BRA      L00E005C3

L00800B7E:
  MOV.Q    @(R15, 80), RQ4
  BSR      w_chkaccess
  MOV      RD2, RD12
  CMP/EQ   RD12
  BT       L00800B7F
  MOV      #1, RD9
  MOV.L    RD9, gamemode
  MOV.Q    @(R15, 80), RQ4
  BSR      D_AddFile
  BRA      L00E005C3

L00800B7F:
  MOV.Q    @(R15, 88), RQ4
  BSR      w_chkaccess
  MOV      RD2, RD12
  CMP/EQ   RD12
  BT       L00800B80
  MOV      #0, RD9
  MOV.L    RD9, gamemode
  MOV.Q    @(R15, 88), RQ4
  BSR      D_AddFile
  BRA      L00E005C3

L00800B80:
  MOV      L00C00859, RQ4
  BSR      printf
  MOV      #6, RD9
  MOV.L    RD9, gamemode

L00E005C3:
  ADD      #184, R15
  BRA      L00C0064D

w_chkaccess:
  PUSH     LR
  PUSH     R14
  PUSH     R13
  ADD      #-136, R15
  MOV.Q    RQ4, @(R15, 8)
  MOV.Q    @(R15, 8), RQ4
  MOV      L00C0006A, RQ5
  BSR      fopen
  MOV      RQ2, RQ14
  MOV      #0, RQ13
  CMPQ/EQ  RQ13, RQ14
  BT       L008020C5
  MOV      RQ14, RQ4
  BSR      fclose
  MOV      #0, RD2
  BRA      L00E005C4

L008020C5:
  MOV      #-1, RD2

L00E005C4:
  ADD      #136, R15
  POP      RQ13
  POP      RQ14
  RET     

FindResponseFile:
  PUSH     LR
  BSR      L00C00630
  ADD      #-280, R15
  MOV      #96, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 56)
  MOV      #1, RD9

L00800B81:
  MOV.L    myargc, RD8
  CMP/GT   RD9, RD8
  BT       L00800B83
  MOV.Q    myargv, RQ31
  EXTS.L   RD9
  MOV.Q    @(RQ31, RD9), RQ13
  MOV.B    @RQ13, RD12
  CMP/EQ   #64, RD12
  BT       L00800B84
  MOV.Q    myargv, RQ30
  EXTS.L   RD9
  MOV.Q    @(RQ30, RD9), RQ13
  MOV      #1, RD29
  EXTS.L   RD29
  LEA.B    @(RQ13, RD29), RQ28
  MOV.Q    RQ28, @(R15, 24)
  MOV      RQ28, RQ4
  MOV      L00C0006A, RQ5
  BSR      fopen
  MOV      RQ2, RQ10
  MOV      #0, RQ27
  CMPQ/EQ  RQ27, RQ10
  BT       L00800B85
  MOV      L00C0085A, RQ4
  BSR      printf
  MOV      #1, RD4
  BSR      exit

L00800B85:
  MOV.Q    myargv, RQ26
  EXTS.L   RD9
  MOV.Q    @(RQ26, RD9), RQ13
  MOV      #1, RD25
  EXTS.L   RD25
  LEA.B    @(RQ13, RD25), RQ27
  MOV.Q    RQ27, @(R15, 24)
  MOV      L00C0085B, RQ4
  MOV      RQ27, RQ5
  BSR      printf
  MOV      #0, RQ11
  EXTS.L   RQ11, RQ11
  MOV      RQ10, RQ4
  MOV      RQ11, RQ5
  MOV      #2, RD6
  BSR      fseek
  MOV      RQ10, RQ4
  BSR      ftell
  MOV      RQ2, RQ11
  EXTS.L   RQ11, RD28
  MOV      #0, RQ11
  EXTS.L   RQ11, RQ11
  MOV.L    RD28, @(R15, 92)
  MOV      RQ10, RQ4
  MOV      RQ11, RQ5
  MOV      #0, RD6
  BSR      fseek
  EXTS.L   RD28, RQ11
  MOV      RQ11, RQ4
  BSR      malloc
  MOV      RQ2, RQ29
  EXTS.L   RD28, RQ11
  EXTS.L   RD25, RQ30
  MOV.Q    RQ30, @(R15, 8)
  MOV.Q    RQ29, @(R15, 64)
  MOV      RQ29, RQ4
  MOV      RQ11, RQ5
  MOV      RQ30, RQ6
  MOV      RQ10, RQ7
  BSR      fread
  MOV      RQ10, RQ4
  BSR      fclose
  MOV      #0, RD31
  ADD      RD9, #1, RD12
  MOV      RD12, RD14
  MOV.L    RD31, @(R15, 88)

L00800B86:
  MOV.L    myargc, RD7
  CMP/GT   RD14, RD7
  BT       L00800B88
  MOV.Q    myargv, RQ6
  EXTS.L   RD14
  MOV.Q    @(RQ6, RD14), RQ13
  MOV.L    @(R15, 88), RD5
  MOV      RD5, RD12
  ADD      #1, RD5
  MOV      #96, RQ4
  ADD      R15, RQ4
  MOV.Q    RQ13, @(RQ4, RD12)
  MOV.L    RD5, @(R15, 88)

L00800B87:
  ADD      #1, RD14
  BRA      L00800B86

L00800B88:
  MOV.Q    myargv, RQ25
  MOV.Q    @RQ25, RQ26
  MOV      #800, RQ11
  EXTS.L   RQ11, RQ11
  MOV.Q    RQ26, @(R15, 48)
  MOV      RQ11, RQ4
  BSR      malloc
  MOV      RQ2, RQ27
  MOV      #800, RQ11
  EXTS.L   RQ11, RQ11
  MOV.Q    RQ27, myargv
  MOV.Q    myargv, RQ4
  MOV      #0, RD5
  MOV      RQ11, RQ6
  BSR      memset
  MOV.Q    myargv, RQ3
  MOV.Q    RQ26, @RQ3
  MOV.Q    @(R15, 64), RQ23
  MOV      RQ23, RQ2
  MOV      #0, RD14
  MOV      #0, RD22
  ADD      #1, RD22
  MOV.Q    RQ2, @(R15, 72)
  MOV.L    RD22, @(R15, 84)

L00800B89:
  MOV.Q    @(R15, 72), RQ21
  EXTS.L   RD14
  LEA.B    @(RQ21, RD14), RQ13
  MOV.L    @(R15, 84), RD20
  MOV      RD20, RD12
  ADD      #1, RD20
  MOV.Q    myargv, RQ19
  MOV.Q    RQ13, @(RQ19, RD12)
  MOV.L    RD20, @(R15, 84)

L00800B8B:
  MOV.L    @(R15, 92), RD18
  CMP/GT   RD14, RD18
  BT       L00800B8C
  MOV.Q    @(R15, 72), RQ17
  EXTS.L   RD14
  LEA.B    @(RQ17, RD14), RQ13
  MOV.B    @RQ13, RD12
  CMP/GE   #33, RD12
  BT       L00800B8C
  MOV.Q    @(R15, 72), RQ16
  EXTS.L   RD14
  LEA.B    @(RQ16, RD14), RQ13
  MOV.B    @RQ13, RD12
  CMP/GT   #122, RD12
  BT       L00800B8C
  BRA      L00800B8D

L00800B8D:
  ADD      #1, RD14
  BRA      L00800B8B

L00800B8C:
  MOV.Q    @(R15, 72), RQ16
  EXTS.L   RD14
  LEA.B    @(RQ16, RD14), RQ13
  MOV      #0, RD17
  MOV.B    RD17, @RQ13

L00800B90:
  MOV.L    @(R15, 92), RD16
  CMP/GT   RD14, RD16
  BT       L00800B91
  MOV.Q    @(R15, 72), RQ16
  EXTS.L   RD14
  LEA.B    @(RQ16, RD14), RQ13
  MOV.B    @RQ13, RD12
  CMP/GT   #32, RD12
  BT       L00800B92
  MOV.Q    @(R15, 72), RQ16
  EXTS.L   RD14
  LEA.B    @(RQ16, RD14), RQ13
  MOV.B    @RQ13, RD12
  CMP/GT   #122, RD12
  BT       L00800B92
  BRA      L00800B91

L00800B92:
  ADD      #1, RD14
  BRA      L00800B90

L00800B91:
  MOV.L    @(R15, 92), RD16
  CMP/GT   RD14, RD16
  BT       L00800B89

L00800B8A:
  MOV      #0, RD14

L00800B95:
  MOV.L    @(R15, 88), RD16
  CMP/GT   RD14, RD16
  BT       L00800B97
  MOV      #96, RQ16
  ADD      R15, RQ16
  EXTS.L   RD14
  MOV.Q    @(RQ16, RD14), RQ13
  MOV.L    @(R15, 84), RD17
  MOV      RD17, RD12
  ADD      #1, RD17
  MOV.Q    myargv, RQ18
  MOV.Q    RQ13, @(RQ18, RD12)
  MOV.L    RD17, @(R15, 84)

L00800B96:
  ADD      #1, RD14
  BRA      L00800B95

L00800B97:
  MOV.L    @(R15, 84), RD26
  MOV      RD26, RD25
  MOV.L    RD25, myargc
  MOV      L00C0085C, RQ4
  MOV.L    myargc, RD5
  BSR      printf
  MOV      #1, RD14

L00800B98:
  MOV.L    myargc, RD16
  CMP/GT   RD14, RD16
  BT       L00800B9A
  MOV.Q    myargv, RQ25
  EXTS.L   RD14
  MOV.Q    @(RQ25, RD14), RQ13
  MOV      L00C007D3, RQ4
  MOV      RQ13, RQ5
  BSR      printf

L00800B99:
  ADD      #1, RD14
  BRA      L00800B98

L00800B9A:
  BRA      L00800B83

L00800B84:

L00800B82:
  ADD      #1, RD9
  BRA      L00800B81

L00800B83:

L00E005C5:
  ADD      #280, R15
  BRA      L00C00626

W_Profile:
  ADD      #-144, R15

L00E005C6:
  ADD      #144, R15
  RTS     

NetbufferSize:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  ADD      #-336, R15
  MOV      #0, RQ14
  ADD      RQ14, #8, RQ12
  MOV.Q    netbuffer, RQ11
  MOV.B    @(RQ11, 7), RD13
  EXTU.B   RD13, RD13
  LEA.Q    @(RQ12, RD13), RQ14
  MOV      RQ14, RQ7
  EXTS.L   RQ7, RD13
  MOV      RD13, RD2

L00E005C7:
  ADD      #336, R15

L00C00861:
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  RTS     

L00C00863:
  PUSH     R31
  PUSH     R30
  PUSH     R29
  PUSH     R14
  PUSH     R13
  PUSH     R12
  PUSH     R11
  PUSH     R10
  PUSH     R9
  PUSH     R8
  RTS     

NetbufferChecksum:
  PUSH     LR
  BSR      L00C00863
  ADD      #-88, R15
  MOV      #0x01234567, RD9
  MOV      #291, RD9
  BSR      NetbufferSize
  MOV      RD2, RD14
  MOV      #0, RQ12
  ADD      RQ12, #4, RQ8
  EXTS.L   RQ8, RD11
  SUB      RD14, RD11, RD10
  SHAD     RD10, #-2, RD31
  MOV      #0, RD13
  MOV.L    RD31, @(R15, 28)

L00800BF3:
  MOV.L    @(R15, 28), RD30
  CMP/GT   RD13, RD30
  BT       L00800BF5
  MOV.Q    netbuffer, RQ29
  ADD      RQ29, #4, RQ12
  EXTS.L   RD13
  MOVU.L   @(RQ12, RD13), RD14
  ADD      RD13, #1, RD11
  MUL.L    RD14, RD11, RD10
  ADD      RD10, RD9

L00800BF4:
  ADD      #1, RD13
  BRA      L00800BF3

L00800BF5:
  AND      RD9, #268435455, RD14
  MOV      RD14, RD2

L00E005C8:
  ADD      #88, R15

L00C00864:
  POP      RQ8
  POP      RQ9
  POP      RQ10
  POP      RQ11
  POP      RQ12
  POP      RQ13
  POP      RQ14
  POP      RQ29
  POP      RQ30
  POP      RQ31
  RET     

ExpandTics:
  PUSH     LR
  BSR      L00C00863
  ADD      #-72, R15
  MOV      RD4, RD11
  MOV.L    maketic, RD10
  EXTU.B   RD10, RD14
  SUB      RD11, RD14, RD12
  MOV      #-64, RD9
  CMP/GE   RD9, RD12
  BT       L00800BF6
  CMP/GT   #64, RD12
  BT       L00800BF6
  BRA      L00800BF7

L00800BF7:
  MOV.L    maketic, RD8
  AND      RD8, #-256, RD14
  ADD      RD14, RD11, RD13
  MOV      RD13, RD2
  BRA      L00E005C9

L00800BF6:
  CMP/GT   #64, RD12
  BT       L00800BF9
  MOV.L    maketic, RD31
  AND      RD31, #-256, RD14
  SUB      RD14, #256, RD13
  ADD      RD13, RD11, RD14
  MOV      RD14, RD2
  BRA      L00E005C9

L00800BF9:
  MOV      #-64, RD30
  CMP/GE   RD30, RD12
  BT       L00800BFA
  MOV.L    maketic, RD29
  AND      RD29, #-256, RD14
  ADD      RD14, #256, RD13
  ADD      RD13, RD11, RD14
  MOV      RD14, RD2
  BRA      L00E005C9

L00800BFA:
  MOV      L00C00866, RQ4
  MOV      RD11, RD5
  MOV.L    maketic, RD6
  BSR      I_Error
  MOV      #0, RD2

L00E005C9:
  ADD      #72, R15
  BRA      L00C00864

HSendPacket:
  PUSH     LR
  BSR      L00C00630
  ADD      #-184, R15
  MOV.L    RD4, @(R15, 52)
  MOV.L    RD5, @(R15, 48)
  BSR      NetbufferChecksum
  MOV      RD2, RD14
  MOV.L    @(R15, 48), RD12
  OR       RD14, RD12, RD13
  MOV.Q    netbuffer, RQ11
  EXTU.L   RD13, RD13
  MOV.L    RD13, @RQ11
  MOV.L    @(R15, 52), RD10
  CMP/EQ   RD10
  BT       L00800BFB
  MOV.Q    netbuffer, RQ9
  MOV      RQ9, RQ8
  MOV      RQ8, RQ31
  MOV      #1, RD30
  MOV      reboundstore, RQ7
  MOV.Q    @RQ31, RQ6
  MOV.Q    @(RQ31, 8), RQ5
  MOV.Q    RQ6, @RQ7
  MOV.Q    RQ5, @(RQ7, 8)
  MOV.Q    @(RQ31, 16), RQ6
  MOV.Q    @(RQ31, 24), RQ5
  MOV.Q    RQ6, @(RQ7, 16)
  MOV.Q    RQ5, @(RQ7, 24)
  MOV.Q    @(RQ31, 32), RQ6
  MOV.Q    @(RQ31, 40), RQ5
  MOV.Q    RQ6, @(RQ7, 32)
  MOV.Q    RQ5, @(RQ7, 40)
  MOV.Q    @(RQ31, 48), RQ6
  MOV.Q    @(RQ31, 56), RQ5
  MOV.Q    RQ6, @(RQ7, 48)
  MOV.Q    RQ5, @(RQ7, 56)
  MOV.Q    @(RQ31, 64), RQ6
  MOV.Q    @(RQ31, 72), RQ5
  MOV.Q    RQ6, @(RQ7, 64)
  MOV.Q    RQ5, @(RQ7, 72)
  MOV.Q    @(RQ31, 80), RQ6
  MOV.Q    @(RQ31, 88), RQ5
  MOV.Q    RQ6, @(RQ7, 80)
  MOV.Q    RQ5, @(RQ7, 88)
  MOV.Q    @(RQ31, 96), RQ6
  MOV.Q    RQ6, @(RQ7, 96)
  MOV.L    RD30, reboundpacket
  BRA      L00E005CA

L00800BFB:
  MOV.L    demoplayback, RD29
  CMP/EQ   #0, RD29
  BT       L00800BFC
  BRA      L00E005CA

L00800BFC:
  MOV.L    netgame, RD28
  CMP/EQ   #0, RD28
  BT       L00800BFD
  MOV      L00C00867, RQ4
  BSR      I_Error

L00800BFD:
  MOV      #1, RD27
  MOV.Q    doomcom, RQ26
  MOV.W    RD27, @(RQ26, 10)
  MOV.L    @(R15, 52), RD25
  MOV.W    RD25, @(RQ26, 12)
  BSR      NetbufferSize
  MOV      RD2, RD14
  MOV.Q    doomcom, RQ28
  MOV.W    RD14, @(RQ28, 14)
  MOV.Q    debugfile, RQ29
  MOV      #0, RQ30
  CMPQ/EQ  RQ30, RQ29
  BT       L00800BFE
  MOV.Q    netbuffer, RQ7
  MOV.L    @RQ7, RD14
  EXTU.L   RD14, RD14
  TST      #1073741824, RD14
  BT       L00800BFF
  MOV.Q    netbuffer, RQ25
  MOV.B    @(RQ25, 4), RD14
  EXTU.B   RD14, RD14
  MOV      RD14, RD4
  BSR      ExpandTics
  MOV      RD2, RD26
  MOV.L    RD26, @(R15, 40)
  BRA      L00800C00

L00800BFF:
  MOV      #-1, RD25
  MOV.L    RD25, @(R15, 40)

L00800C00:
  MOV.Q    doomcom, RQ25
  MOV.W    @(RQ25, 14), RD14
  MOV.Q    netbuffer, RQ26
  MOV.B    @(RQ26, 7), RD13
  EXTU.B   RD13, RD13
  MOV.B    @(RQ26, 5), RD27
  EXTU.B   RD27, RD27
  MOV.L    RD27, @(R15, 20)
  MOV      RD27, RD4
  BSR      ExpandTics
  MOV      RD2, RD28
  MOV.L    RD28, @(R15, 16)
  MOV.Q    debugfile, RQ4
  MOV      L00C00868, RQ5
  MOV      RD28, RD6
  MOV      RD13, RD7
  MOV.L    @(R15, 40), RD20
  MOV      RD14, RD21
  BSR      fprintf
  MOV      #0, RD6
  MOV.L    RD6, @(R15, 44)

L00800C01:
  MOV.Q    doomcom, RQ5
  MOV.W    @(RQ5, 14), RD14
  MOV.L    @(R15, 44), RD4
  MOV      RD4, RD13
  EXTS.W   RD14, RD3
  MOV.L    RD3, @(R15, 20)
  CMP/GT   RD13, RD3
  BT       L00800C03
  MOV.Q    netbuffer, RQ26
  MOV      RQ26, RQ25
  MOV.L    @(R15, 44), RD27
  EXTS.L   RD27
  MOVU.B   @(RQ25, RD27), RD14
  MOV.Q    debugfile, RQ4
  MOV      L00C00869, RQ5
  MOV      RD14, RD6
  BSR      fprintf

L00800C02:
  MOV.L    @(R15, 44), RD2
  ADD      #1, RD2
  MOV.L    RD2, @(R15, 44)
  BRA      L00800C01

L00800C03:
  MOV.Q    debugfile, RQ4
  MOV      L00C000B0, RQ5
  BSR      fprintf

L00800BFE:
  BSR      I_NetCmd

L00E005CA:
  ADD      #184, R15
  BRA      L00C00626

HGetPacket:
  PUSH     LR
  BSR      L00C00630
  ADD      #-56, R15
  MOV.L    reboundpacket, RD11
  CMP/EQ   RD11
  BT       L00800C04
  MOV.Q    netbuffer, RQ10
  MOV      reboundstore, RQ9
  MOV.Q    @RQ9, RQ7
  MOV.Q    @(RQ9, 8), RQ6
  MOV.Q    RQ7, @RQ10
  MOV.Q    RQ6, @(RQ10, 8)
  MOV.Q    @(RQ9, 16), RQ7
  MOV.Q    @(RQ9, 24), RQ6
  MOV.Q    RQ7, @(RQ10, 16)
  MOV.Q    RQ6, @(RQ10, 24)
  MOV.Q    @(RQ9, 32), RQ7
  MOV.Q    @(RQ9, 40), RQ6
  MOV.Q    RQ7, @(RQ10, 32)
  MOV.Q    RQ6, @(RQ10, 40)
  MOV.Q    @(RQ9, 48), RQ7
  MOV.Q    @(RQ9, 56), RQ6
  MOV.Q    RQ7, @(RQ10, 48)
  MOV.Q    RQ6, @(RQ10, 56)
  MOV.Q    @(RQ9, 64), RQ7
  MOV.Q    @(RQ9, 72), RQ6
  MOV.Q    RQ7, @(RQ10, 64)
  MOV.Q    RQ6, @(RQ10, 72)
  MOV.Q    @(RQ9, 80), RQ7
  MOV.Q    @(RQ9, 88), RQ6
  MOV.Q    RQ7, @(RQ10, 80)
  MOV.Q    RQ6, @(RQ10, 88)
  MOV.Q    @(RQ9, 96), RQ7
  MOV.Q    RQ7, @(RQ10, 96)
  MOV      #0, RD8
  MOV.Q    doomcom, RQ31
  MOV.W    RD8, @(RQ31, 12)
  MOV      #0, RD30
  MOV.L    RD30, reboundpacket
  MOV      #1, RD2
  BRA      L00E005CB

L00800C04:
  MOV.L    netgame, RD29
  CMP/EQ   #0, RD29
  BT       L00800C05
  MOV      #0, RD2
  BRA      L00E005CB

L00800C05:
  MOV.L    demoplayback, RD28
  CMP/EQ   #0, RD28
  BT       L00800C06
  MOV      #0, RD2
  BRA      L00E005CB

L00800C06:
  MOV      #2, RD27
  MOV.Q    doomcom, RQ26
  MOV.W    RD27, @(RQ26, 10)
  BSR      I_NetCmd
  MOV.Q    doomcom, RQ25
  MOV.W    @(RQ25, 12), RD14
  CMP/EQ   #-1, RD14
  BT       L00800C07
  MOV      #0, RD2
  BRA      L00E005CB

L00800C07:
  MOV.Q    doomcom, RQ25
  MOV.W    @(RQ25, 14), RD14
  BSR      NetbufferSize
  MOV      RD2, RD12
  EXTS.W   RD14, RD13
  MOV      RD12, RD26
  MOV.L    RD26, @(R15, 8)
  CMP/EQ   RD26, RD13
  BT       L00800C08
  MOV.Q    debugfile, RQ7
  MOV      #0, RQ6
  CMPQ/EQ  RQ6, RQ7
  BT       L00800C09
  MOV.Q    doomcom, RQ25
  MOV.W    @(RQ25, 14), RD14
  MOV.Q    debugfile, RQ4
  MOV      L00C0086B, RQ5
  MOV      RD14, RD6
  BSR      fprintf

L00800C09:
  MOV      #0, RD2
  BRA      L00E005CB

L00800C08:
  BSR      NetbufferChecksum
  MOV      RD2, RD14
  MOV.Q    netbuffer, RQ25
  MOV.L    @RQ25, RD12
  EXTU.L   RD12, RD12
  AND      RD12, #268435455, RD13
  CMP/EQ   RD13, RD14
  BT       L00800C0A
  MOV.Q    debugfile, RQ5
  MOV      #0, RQ4
  CMPQ/EQ  RQ4, RQ5
  BT       L00800C0B
  MOV.Q    debugfile, RQ4
  MOV      L00C0086C, RQ5
  BSR      fprintf

L00800C0B:
  MOV      #0, RD2
  BRA      L00E005CB

L00800C0A:
  MOV.Q    debugfile, RQ3
  MOV      #0, RQ2
  CMPQ/EQ  RQ2, RQ3
  BT       L00800C0C
  MOV.Q    netbuffer, RQ23
  MOV.L    @RQ23, RD14
  EXTU.L   RD14, RD14
  TST      #536870912, RD14
  BT       L00800C0D
  MOV.Q    debugfile, RQ4
  MOV      L00C0086D, RQ5
  BSR      fprintf
  BRA      L00800C0E

L00800C0D:
  MOV.Q    netbuffer, RQ22
  MOV.L    @RQ22, RD14
  EXTU.L   RD14, RD14
  TST      #1073741824, RD14
  BT       L00800C0F
  MOV.Q    netbuffer, RQ25
  MOV.B    @(RQ25, 4), RD14
  EXTU.B   RD14, RD14
  MOV      RD14, RD4
  BSR      ExpandTics
  MOV      RD2, RD26
  MOV.L    RD26, @(R15, 28)
  BRA      L00800C10

L00800C0F:
  MOV      #-1, RD21
  MOV.L    RD21, @(R15, 28)

L00800C10:
  MOV.Q    doomcom, RQ25
  MOV.W    @(RQ25, 14), RD14
  MOV.Q    netbuffer, RQ26
  MOV.B    @(RQ26, 7), RD12
  EXTU.B   RD12, RD12
  MOV.B    @(RQ26, 5), RD13
  EXTU.B   RD13, RD13
  MOV      RD13, RD4
  BSR      ExpandTics
  MOV      RD2, RD27
  MOV.Q    doomcom, RQ28
  MOV.W    @(RQ28, 12), RD13
  MOV.L    RD27, @(R15, 8)
  MOV.Q    debugfile, RQ4
  MOV      L00C0086E, RQ5
  MOV      RD13, RD6
  MOV      RD27, RD7
  MOV      RD12, RD20
  MOV.L    @(R15, 28), RD21
  MOV      RD14, RD22
  BSR      fprintf
  MOV      #0, RD20
  MOV.L    RD20, @(R15, 24)

L00800C11:
  MOV.Q    doomcom, RQ19
  MOV.W    @(RQ19, 14), RD14
  MOV.L    @(R15, 24), RD18
  MOV      RD18, RD12
  EXTS.W   RD14, RD13
  CMP/GT   RD12, RD13
  BT       L00800C13
  MOV.Q    netbuffer, RQ26
  MOV      RQ26, RQ25
  MOV.L    @(R15, 24), RD27
  EXTS.L   RD27
  MOVU.B   @(RQ25, RD27), RD14
  MOV.Q    debugfile, RQ4
  MOV      L00C00869, RQ5
  MOV      RD14, RD6
  BSR      fprintf

L00800C12:
  MOV.L    @(R15, 24), RD17
  ADD      #1, RD17
  MOV.L    RD17, @(R15, 24)
  BRA      L00800C11

L00800C13:
  MOV.Q    debugfile, RQ4
  MOV      L00C000B0, RQ5
  BSR      fprintf

L00800C0E:

L00800C0C:
  MOV      #1, RD2

L00E005CB:
  ADD      #56, R15
  BRA      L00C00626

GetPackets:
  PUSH     LR
  BSR      L00C00630
  ADD      #-616, R15

L00800C14:
  BSR      HGetPacket
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L00800C15
  MOV.Q    netbuffer, RQ11
  MOV.L    @RQ11, RD14
  EXTU.L   RD14, RD14
  TST      #536870912, RD14
  BT       L00800C16
  BRA      L00800C14

L00800C16:
  MOV.Q    netbuffer, RQ10
  MOV.B    @(RQ10, 6), RD14
  EXTU.B   RD14, RD14
  AND      RD14, #-129, RD9
  MOV.Q    doomcom, RQ8
  MOV.W    @(RQ8, 12), RD13
  MOV.B    @(RQ10, 5), RD14
  EXTU.B   RD14, RD14
  MOV.L    RD9, @(R15, 76)
  MOV      RD14, RD4
  BSR      ExpandTics
  MOV      RD2, RD31
  MOV.Q    netbuffer, RQ30
  MOV.B    @(RQ30, 7), RD14
  EXTU.B   RD14, RD14
  ADD      RD31, RD14, RD29
  MOV.L    @RQ30, RD14
  EXTU.L   RD14, RD14
  MOV.L    RD31, @(R15, 48)
  MOV.L    RD29, @(R15, 52)
  TST      #-2147483648, RD14
  BT       L00800C17
  MOV      nodeingame, RQ28
  MOV.L    @(RQ28, RD13), RD14
  CMP/EQ   RD14
  BT       L00800C18
  BRA      L00800C14

L00800C18:
  MOV      nodeingame, RQ27
  MOV      #0, RD26
  EXTS.L   RD13
  MOV.L    RD26, @(RQ27, RD13)
  MOV      playeringame, RQ25
  MOV.L    @(R15, 76), RD28
  EXTS.L   RD28
  MOV.L    RD26, @(RQ25, RD28)
  MOV      exitmsg, RQ4
  MOV      L00C00870, RQ5
  BSR      strcpy
  MOV      exitmsg, RQ29
  MOV.B    @(RQ29, 7), RD14
  ADD      RD14, RD28, RD12
  MOV.B    RD12, @(RQ29, 7)
  MOV      players, RQ30
  MOV.L    consoleplayer, RD31
  EXTS.L   RD31
  MOV      #320, RD7
  MUL.L    RD31, RD7
  STS      R0, RQ8
  ADD      RQ30, RQ8
  MOV.Q    RQ29, @(RQ8, 224)
  MOV.L    demorecording, RD9
  CMP/EQ   RD9
  BT       L00800C19
  BSR      G_CheckDemoStatus

L00800C19:
  BRA      L00800C14

L00800C17:
  MOV.Q    netbuffer, RQ7
  MOV.L    @RQ7, RD14
  EXTU.L   RD14, RD14
  TST      #268435456, RD14
  BT       L00800C1A
  MOV      L00C00871, RQ4
  BSR      I_Error

L00800C1A:
  MOV      nodeforplayer, RQ6
  MOV.L    @(R15, 76), RD5
  MOV.L    RD13, @(RQ6, RD5)
  MOV      resendcount, RQ4
  EXTS.L   RD13
  MOV.L    @(RQ4, RD13), RD14
  CMP/PL   RD14
  BT       L00800C1B
  MOV.Q    netbuffer, RQ3
  MOV.L    @RQ3, RD14
  EXTU.L   RD14, RD14
  TST      #1073741824, RD14
  BT       L00800C1B
  BRA      L00800C1C

L00800C1C:
  MOV.Q    netbuffer, RQ25
  MOV.B    @(RQ25, 4), RD14
  EXTU.B   RD14, RD14
  MOV      RD14, RD4
  BSR      ExpandTics
  MOV      RD2, RD12
  MOV      resendto, RQ26
  EXTS.L   RD13
  MOV.L    RD12, @(RQ26, RD13)
  MOV.Q    debugfile, RQ27
  MOV      #0, RQ28
  CMPQ/EQ  RQ28, RQ27
  BT       L00800C1E
  MOV      resendto, RQ25
  EXTS.L   RD13
  MOV.L    @(RQ25, RD13), RD14
  MOV.Q    debugfile, RQ4
  MOV      L00C00872, RQ5
  MOV      RD14, RD6
  BSR      fprintf

L00800C1E:
  MOV      resendcount, RQ2
  MOV      #10, RD23
  EXTS.L   RD13
  MOV.L    RD23, @(RQ2, RD13)
  BRA      L00800C1F

L00800C1B:
  MOV      resendcount, RQ22
  EXTS.L   RD13
  MOV.L    @(RQ22, RD13), RD14
  ADD      RD14, #-1, RD12
  EXTS.L   RD13
  MOV.L    RD12, @(RQ22, RD13)

L00800C1F:
  MOV      nettics, RQ21
  EXTS.L   RD13
  MOV.L    @(RQ21, RD13), RD14
  MOV.L    @(R15, 52), RD20
  CMP/EQ   RD14, RD20
  BT       L00800C20
  BRA      L00800C14

L00800C20:
  MOV      nettics, RQ19
  EXTS.L   RD13
  MOV.L    @(RQ19, RD13), RD14
  MOV.L    @(R15, 52), RD18
  CMP/GT   RD18, RD14
  BT       L00800C21
  MOV.Q    debugfile, RQ17
  MOV      #0, RQ16
  CMPQ/EQ  RQ16, RQ17
  BT       L00800C22
  MOV.Q    netbuffer, RQ25
  MOV.B    @(RQ25, 7), RD14
  EXTU.B   RD14, RD14
  MOV.Q    debugfile, RQ4
  MOV      L00C00873, RQ5
  MOV.L    @(R15, 48), RD6
  MOV      RD14, RD7
  BSR      fprintf

L00800C22:
  BRA      L00800C14

L00800C21:
  MOV      nettics, RQ16
  EXTS.L   RD13
  MOV.L    @(RQ16, RD13), RD14
  MOV.L    @(R15, 48), RD17
  CMP/GT   RD14, RD17
  BT       L00800C23
  MOV.Q    debugfile, RQ16
  MOV      #0, RQ17
  CMPQ/EQ  RQ17, RQ16
  BT       L00800C24
  MOV      nettics, RQ25
  EXTS.L   RD13
  MOV.L    @(RQ25, RD13), RD14
  MOV.Q    debugfile, RQ4
  MOV      L00C00874, RQ5
  MOV      RD13, RD6
  MOV.L    @(R15, 48), RD7
  MOV      RD14, RD20
  BSR      fprintf

L00800C24:
  MOV      remoteresend, RQ16
  MOV      #1, RD17
  EXTS.L   RD13
  MOV.L    RD17, @(RQ16, RD13)
  BRA      L00800C14

L00800C23:
  MOV      remoteresend, RQ16
  MOV      #0, RD17
  EXTS.L   RD13
  MOV.L    RD17, @(RQ16, RD13)
  MOV      nettics, RQ18
  EXTS.L   RD13
  MOV.L    @(RQ18, RD13), RD14
  MOV.L    @(R15, 48), RD20
  SUB      RD14, RD20, RD19
  MOV.Q    netbuffer, RQ21
  ADD      RQ21, #8, RQ22
  EXTS.L   RD19
  LEA.Q    @(RQ22, RD19), RQ23
  MOV      RQ23, RQ2
  MOV.Q    RQ2, @(R15, 64)
  MOV.Q    RQ23, @(R15, 8)
  MOV.Q    RQ22, @(R15, 16)
  MOV.L    RD19, @(R15, 44)

L00800C25:
  MOV      nettics, RQ16
  EXTS.L   RD13
  MOV.L    @(RQ16, RD13), RD14
  MOV.L    @(R15, 52), RD17
  CMP/GT   RD14, RD17
  BT       L00800C26
  MOV      netcmds, RQ25
  MOV.L    @(R15, 76), RD26
  EXTS.L   RD26
  MOV      #96, RD7
  MUL.L    RD26, RD7
  STS      R0, RQ27
  ADD      RQ25, RQ27
  MOV      nettics, RQ28
  EXTS.L   RD13
  MOV.L    @(RQ28, RD13), RD14
  MOV      RD14, RD12
  MOV      #0x15555556, RD7
  MOV      #5461, RD7
  DMULS    RD7, RD12
  STS      R1, RD6
  MOV      #12, RD7
  DMULU    RD7, RD6
  STS      R0, RD6
  SUB      RD6, RD12
  EXTS.L   RD12
  LEA.Q    @(RQ27, RD12), RQ29
  MOV      RQ29, RQ30
  EXTS.L   RD13
  MOV.L    @(RQ28, RD13), RD14
  ADD      RD14, #1, RD12
  EXTS.L   RD13
  MOV.L    RD12, @(RQ28, RD13)
  MOV.Q    @(R15, 64), RQ31
  MOV      RQ31, RQ8
  MOV.Q    @RQ8, RQ7
  MOV.Q    RQ7, @RQ30
  ADD      #8, RQ31
  MOV.Q    RQ31, @(R15, 64)
  MOV.Q    RQ30, @(R15, 56)
  MOV.Q    RQ29, @(R15, 8)
  MOV.Q    RQ27, @(R15, 16)
  BRA      L00800C25

L00800C26:
  BRA      L00800C14

L00800C15:

L00E005CC:
  ADD      #616, R15
  BRA      L00C00626

CheckAbort:
  PUSH     LR
  BSR      L00C00721
  ADD      #-1200, R15
  BSR      I_GetTime
  MOV      RD2, RD14
  ADD      RD14, #2, RD13
  MOV.L    RD13, @(R15, 100)

L00800C3A:
  BSR      I_GetTime
  MOV      RD2, RD14
  MOV.L    @(R15, 100), RD12
  CMP/GT   RD14, RD12
  BT       L00800C3B
  BSR      I_StartTic
  BRA      L00800C3A

L00800C3B:
  BSR      I_StartTic

L00800C3C:
  MOV.L    eventtail, RD11
  MOV.L    eventhead, RD10
  CMP/EQ   RD10, RD11
  BT       L00800C3E
  MOV      events, RQ9
  MOV.L    eventtail, RD8
  SHAD     RD8, #4, RQ31
  ADD      RQ9, RQ31
  MOV      RQ31, RQ30
  MOV.L    @RQ30, RD14
  MOV.Q    RQ30, @(R15, 104)
  CMP/EQ   RD14
  BT       L00800C3F
  MOV.Q    @(R15, 104), RQ7
  MOV.L    @(RQ7, 4), RD14
  CMP/EQ   #27, RD14
  BT       L00800C3F
  BRA      L00800C40

L00800C40:
  MOV      L00C00876, RQ4
  BSR      I_Error

L00800C3F:

L00800C3D:
  MOV.L    eventtail, RD6
  ADD      #1, RD6
  AND      #63, RD6
  MOV.L    RD6, eventtail
  BRA      L00800C3C

L00800C3E:

L00E005CD:
  ADD      #1200, R15
  BRA      L00C00722

D_ArbitrateNetStart:
  PUSH     LR
  BSR      L00C00630
  ADD      #-104, R15
  MOV      #48, RQ3
  ADD      R15, RQ3
  MOV.Q    RQ3, @(R15, 32)
  MOV      #1, RD12
  MOV      #32, RQ11
  EXTS.L   RQ11, RQ11
  MOV.L    RD12, autostart
  MOV.Q    RQ11, @(R15, 24)
  MOV      #48, RQ4
  ADD      R15, RQ4
  MOV      #0, RD5
  MOV      RQ11, RQ6
  BSR      memset
  MOV.Q    doomcom, RQ10
  MOV.W    @(RQ10, 32), RD14
  CMP/EQ   RD14
  BT       L00800C42
  MOV      L00C00877, RQ4
  BSR      printf

L00800C43:
  BSR      CheckAbort
  BSR      HGetPacket
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L00800C45
  BRA      L00800C43

L00800C45:
  MOV.Q    netbuffer, RQ9
  MOV.L    @RQ9, RD14
  EXTU.L   RD14, RD14
  TST      #536870912, RD14
  BT       L00800C46
  MOV.Q    netbuffer, RQ8
  MOV.B    @(RQ8, 6), RD14
  EXTU.B   RD14, RD14
  CMP/EQ   #110, RD14
  BT       L00800C47
  MOV      L00C00878, RQ4
  BSR      I_Error

L00800C47:
  MOV.Q    netbuffer, RQ31
  MOV.B    @(RQ31, 4), RD14
  EXTU.B   RD14, RD14
  AND      RD14, #15, RD30
  MOV.B    @(RQ31, 4), RD14
  EXTU.B   RD14, RD14
  AND      RD14, #192, RD13
  SHAD     RD13, #-6, RD29
  MOV.B    @(RQ31, 4), RD14
  EXTU.B   RD14, RD14
  AND      RD14, #32, RD13
  MOV      #0, RD28
  CMP/GT   RD28, RD13
  MOVT     RQ7
  MOV      RQ7, RD27
  MOV.B    @(RQ31, 4), RD14
  EXTU.B   RD14, RD14
  AND      RD14, #16, RD13
  CMP/GT   RD28, RD13
  MOVT     RQ7
  MOV      RQ7, RD26
  MOV.B    @(RQ31, 5), RD14
  EXTU.B   RD14, RD14
  AND      RD14, #63, RD25
  MOV.B    @(RQ31, 5), RD14
  EXTU.B   RD14, RD14
  SHLD     RD14, #-6, RD8
  MOV.L    RD8, startepisode
  MOV.L    RD30, startskill
  MOV.L    RD29, deathmatch
  MOV.L    RD27, nomonsters
  MOV.L    RD26, respawnparm
  MOV.L    RD25, startmap
  BRA      L00E005CE

L00800C46:
  BRA      L00800C43

L00800C44:
  BRA      L00800C48

L00800C42:
  MOV      L00C00879, RQ4
  BSR      printf

L00800C49:
  BSR      CheckAbort
  MOV      #0, RD25
  MOV.L    RD25, @(R15, 44)

L00800C4B:
  MOV.Q    doomcom, RQ25
  MOV.W    @(RQ25, 16), RD14
  MOV.L    @(R15, 44), RD26
  MOV      RD26, RD13
  EXTS.W   RD14, RD27
  MOV.L    RD27, @(R15, 12)
  CMP/GT   RD13, RD27
  BT       L00800C4D
  MOV.L    startskill, RD7
  MOV.Q    netbuffer, RQ6
  EXTU.B   RD7, RD7
  MOV.B    RD7, @(RQ6, 4)
  MOV.L    deathmatch, RD5
  CMP/EQ   RD5
  BT       L00800C4E
  MOV.Q    netbuffer, RQ4
  MOV.B    @(RQ4, 4), RD14
  EXTU.B   RD14, RD14
  MOV.L    deathmatch, RD3
  SHAD     RD3, #6, RD13
  OR       RD14, RD13, RD2
  EXTU.B   RD2, RD2
  MOV.B    RD2, @(RQ4, 4)
  MOV.L    RD2, @(R15, 12)

L00800C4E:
  MOV.L    nomonsters, RD23
  CMP/EQ   #0, RD23
  BT       L00800C4F
  MOV.Q    netbuffer, RQ22
  MOV.B    @(RQ22, 4), RD14
  EXTU.B   RD14, RD14
  OR       RD14, #32, RD13
  EXTU.B   RD13, RD13
  MOV.B    RD13, @(RQ22, 4)

L00800C4F:
  MOV.L    respawnparm, RD21
  CMP/EQ   #0, RD21
  BT       L00800C50
  MOV.Q    netbuffer, RQ20
  MOV.B    @(RQ20, 4), RD14
  EXTU.B   RD14, RD14
  OR       RD14, #16, RD13
  EXTU.B   RD13, RD13
  MOV.B    RD13, @(RQ20, 4)

L00800C50:
  MOV.L    startepisode, RD25
  SHAD     RD25, #6, RD14
  MOV.L    startmap, RD26
  ADD      RD14, RD26, RD13
  MOV.Q    netbuffer, RQ27
  EXTU.B   RD13, RD13
  MOV.B    RD13, @(RQ27, 5)
  MOV      #110, RD28
  EXTU.B   RD28, RD28
  MOV.B    RD28, @(RQ27, 6)
  MOV      #0, RD29
  EXTU.B   RD29, RD29
  MOV.B    RD29, @(RQ27, 7)
  MOV.L    @(R15, 44), RD4
  MOV      #0x20000000, RD5
  MOV      #8192, RD5
  BSR      HSendPacket

L00800C4C:
  MOV.L    @(R15, 44), RD19
  ADD      #1, RD19
  MOV.L    RD19, @(R15, 44)
  BRA      L00800C4B

L00800C4D:
  MOV      #10, RD18
  MOV.L    RD18, @(R15, 44)

L00800C51:
  MOV.L    @(R15, 44), RD17
  CMP/EQ   #0, RD17
  BT       L00800C53
  BSR      HGetPacket
  MOV      RD2, RD14
  CMP/EQ   RD14
  BT       L00800C53
  BRA      L00800C54

L00800C54:
  MOV.Q    netbuffer, RQ16
  MOV.B    @(RQ16, 6), RD14
  EXTU.B   RD14, RD14
  AND      RD14, #127, RD13
  MOV      #8, RD17
  CMP/GE   RD17, RD13
  BT       L00800C56
  MOV.Q    netbuffer, RQ16
  MOV.B    @(RQ16, 6), RD14
  EXTU.B   RD14, RD14
  AND      RD14, #127, RD13
  MOV      #48, RQ17
  ADD      R15, RQ17
  MOV      #1, RD18
  EXTS.L   RD13
  MOV.L    RD18, @(RQ17, RD13)

L00800C56:

L00800C52:
  MOV.L    @(R15, 44), RD16
  ADD      #-1, RD16
  MOV.L    RD16, @(R15, 44)
  BRA      L00800C51

L00800C53:
  MOV      #1, RD16
  MOV.L    RD16, @(R15, 44)

L00800C57:
  MOV.Q    doomcom, RQ16
  MOV.W    @(RQ16, 16), RD14
  MOV.L    @(R15, 44), RD17
  MOV      RD17, RD13
  EXTS.W   RD14, RD18
  MOV.L    RD18, @(R15, 12)
  CMP/GT   RD13, RD18
  BT       L00800C59
  MOV      #48, RQ16
  ADD      R15, RQ16
  MOV.L    @(R15, 44), RD17
  EXTS.L   RD17
  MOV.L    @(RQ16, RD17), RD14
  CMP/EQ   RD14
  BT       L00800C5A
  BRA      L00800C59

L00800C5A:

L00800C58:
  MOV.L    @(R15, 44), RD16
  ADD      #1, RD16
  MOV.L    RD16, @(R15, 44)
  BRA      L00800C57

L00800C59:
  MOV.Q    doomcom, RQ16
  MOV.W    @(RQ16, 16), RD14
  MOV.L    @(R15, 44), RD17
  MOV      RD17, RD13
  EXTS.W   RD14, RD18
  MOV.L    RD18, @(R15, 12)
  CMP/GT   RD13, RD18
  BT       L00800C49

L00800C4A:

L00800C48:

L00E005CE:
  ADD      #104, R15
  BRA      L00C00626

P_ArchivePlayers:
  PUSH     LR
  BSR      L00C00630
  ADD      #-168, R15
  MOV      #0, RD14

L00801C71:
  MOV      #4, RD12
  CMP/GE   RD12, RD14
  BT       L00801C73
  MOV      playeringame, RQ11
  EXTS.L   RD14
  MOV.L    @(RQ11, RD14), RD13
  CMP/EQ   RD13
  BT       L00801C74
  BRA      L00801C72

L00801C74:
  MOV.Q    save_p, RQ10
  MOV      #3, RQ9
  AND      RQ10, RQ9, RQ8
  MOV      #4, RQ31
  EXTS.L   RQ31, RQ31
  SUB      RQ31, RQ8, RQ10
  AND      RQ10, RQ9, RQ8
  MOV.Q    save_p, RQ30
  LEA.B    @(RQ30, RQ8), RQ30
  MOV      RQ30, RQ29
  MOV      players, RQ28
  EXTS.L   RD14
  MOV      #320, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ27
  ADD      RQ28, RQ27
  MOV      #320, RQ10
  EXTS.L   RQ10, RQ10
  MOV.Q    RQ10, @(R15, 40)
  MOV.Q    RQ30, save_p
  MOV.Q    RQ29, @(R15, 56)
  MOV.Q    RQ27, @(R15, 16)
  MOV      RQ29, RQ4
  MOV      RQ27, RQ5
  MOV      RQ10, RQ6
  BSR      memcpy
  MOV.Q    save_p, RQ26
  ADD      #320, RQ26
  MOV      #0, RD25
  MOV.Q    RQ26, save_p
  MOV.L    RD25, @(R15, 68)

L00801C75:
  MOV.L    @(R15, 68), RD7
  MOV      #2, RD6
  CMP/GE   RD6, RD7
  BT       L00801C77
  MOV.Q    @(R15, 56), RQ25
  ADD      RQ25, #264, RQ26
  MOV.L    @(R15, 68), RD27
  EXTS.L   RD27
  MOV      #24, RD7
  MUL.L    RD27, RD7
  STS      R0, RQ28
  ADD      RQ26, RQ28
  MOV.Q    @RQ28, RQ26
  MOV.Q    RQ28, @(R15, 8)
  MOV.Q    RQ26, @(R15, 16)
  MOV      #0, RQ29
  CMPQ/EQ  RQ29, RQ26
  BT       L00801C78
  MOV.Q    @(R15, 56), RQ25
  ADD      RQ25, #264, RQ26
  MOV.L    @(R15, 68), RD27
  EXTS.L   RD27
  MOV      #24, RD7
  MUL.L    RD27, RD7
  STS      R0, RQ28
  ADD      RQ26, RQ28
  MOV.Q    @RQ28, RQ26
  MOV      states, RQ29
  MOV      RQ26, RD13
  SUB      RQ29, RD13
  MOV      #0x06666667, RD7
  MOV      #1638, RD7
  DMULS    RD7, RD13
  STS      R1, RD13
  MOV      RQ13, RQ26
  ADD      RQ25, #264, RQ30
  EXTS.L   RD27
  MOV      #24, RD7
  MUL.L    RD27, RD7
  STS      R0, RQ28
  ADD      RQ30, RQ28
  MOV.Q    RQ26, @RQ28
  MOV.Q    RQ28, @(R15, 8)
  MOV.Q    RQ26, @(R15, 16)

L00801C78:

L00801C76:
  MOV.L    @(R15, 68), RD5
  ADD      #1, RD5
  MOV.L    RD5, @(R15, 68)
  BRA      L00801C75

L00801C77:

L00801C72:
  ADD      #1, RD14
  BRA      L00801C71

L00801C73:

L00E005CF:
  ADD      #168, R15
  BRA      L00C00626

P_UnArchivePlayers:
  PUSH     LR
  BSR      L00C00630
  ADD      #-39304, R15
  MOV      #0, RD14

L00801C79:
  MOV      #4, RD12
  CMP/GE   RD12, RD14
  BT       L00801C7B
  MOV      playeringame, RQ11
  EXTS.L   RD14
  MOV.L    @(RQ11, RD14), RD13
  CMP/EQ   RD13
  BT       L00801C7C
  BRA      L00801C7A

L00801C7C:
  MOV.Q    save_p, RQ10
  MOV      #3, RQ9
  AND      RQ10, RQ9, RQ8
  MOV      #4, RQ31
  EXTS.L   RQ31, RQ31
  SUB      RQ31, RQ8, RQ10
  AND      RQ10, RQ9, RQ8
  MOV.Q    save_p, RQ30
  LEA.B    @(RQ30, RQ8), RQ30
  MOV      players, RQ29
  EXTS.L   RD14
  MOV      #320, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ28
  ADD      RQ29, RQ28
  MOV      #320, RQ10
  EXTS.L   RQ10, RQ10
  MOV.Q    RQ10, @(R15, 192)
  MOV.Q    RQ30, save_p
  MOV.Q    RQ28, @(R15, 168)
  MOV      RQ28, RQ4
  MOV.Q    save_p, RQ5
  MOV      RQ10, RQ6
  BSR      memcpy
  MOV.Q    save_p, RQ27
  ADD      #320, RQ27
  MOV      #0, RQ28
  MOV      players, RQ26
  EXTS.L   RD14
  MOV      #320, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ25
  ADD      RQ26, RQ25
  MOV.Q    RQ28, @RQ25
  MOV      #0, RQ28
  EXTS.L   RD14
  MOV      #320, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ25
  ADD      RQ26, RQ25
  MOV.Q    RQ28, @(RQ25, 224)
  MOV      #0, RQ28
  EXTS.L   RD14
  MOV      #320, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ25
  ADD      RQ26, RQ25
  MOV.Q    RQ28, @(RQ25, 240)
  MOV      #0, RD11
  MOV.L    RD11, @(R15, 204)
  MOV.Q    RQ28, @(R15, 168)
  MOV.Q    RQ27, save_p
  MOV.Q    RQ25, @(R15, 160)

L00801C7D:
  MOV.L    @(R15, 204), RD7
  MOV      #2, RD6
  CMP/GE   RD6, RD7
  BT       L00801C7F
  MOV      players, RQ25
  EXTS.L   RD14
  MOV      #320, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ26
  ADD      RQ25, RQ26
  ADD      RQ26, #264, RQ27
  MOV.L    @(R15, 204), RD28
  EXTS.L   RD28
  MOV      #24, RD7
  MUL.L    RD28, RD7
  STS      R0, RQ29
  ADD      RQ27, RQ29
  MOV.Q    @RQ29, RQ27
  MOV.Q    RQ29, @(R15, 152)
  MOV.Q    RQ27, @(R15, 168)
  MOV.Q    RQ26, @(R15, 160)
  MOV      #0, RQ30
  CMPQ/EQ  RQ30, RQ27
  BT       L00801C80
  MOV      players, RQ25
  EXTS.L   RD14
  MOV      #320, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ26
  ADD      RQ25, RQ26
  ADD      RQ26, #264, RQ27
  MOV.L    @(R15, 204), RD28
  EXTS.L   RD28
  MOV      #24, RD7
  MUL.L    RD28, RD7
  STS      R0, RQ29
  ADD      RQ27, RQ29
  MOV.Q    @RQ29, RQ27
  MOV      states, RQ30
  MOV      #40, RD7
  MUL.L    RQ27, RD7
  STS      R0, RQ31
  ADD      RQ30, RQ31
  EXTS.L   RD14
  MOV      #320, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ26
  ADD      RQ25, RQ26
  ADD      RQ26, #264, RQ27
  EXTS.L   RD28
  MOV      #24, RD7
  MUL.L    RD28, RD7
  STS      R0, RQ29
  ADD      RQ27, RQ29
  MOV.Q    RQ31, @RQ29
  MOV.Q    RQ29, @(R15, 152)
  MOV.Q    RQ27, @(R15, 168)
  MOV.Q    RQ26, @(R15, 160)

L00801C80:

L00801C7E:
  MOV.L    @(R15, 204), RD5
  ADD      #1, RD5
  MOV.L    RD5, @(R15, 204)
  BRA      L00801C7D

L00801C7F:

L00801C7A:
  ADD      #1, RD14
  BRA      L00801C79

L00801C7B:

L00E005D0:
  ADD      #39304, R15
  BRA      L00C00626

P_ArchiveWorld:
  PUSH     LR
  BSR      L00C00630
  ADD      #-56, R15
  MOV.Q    save_p, RQ31
  MOV      RQ31, RQ14
  MOV      #0, RD10
  MOV.Q    sectors, RQ30
  MOV      RQ30, RQ11

L00801C81:
  MOV.L    numsectors, RD29
  CMP/GT   RD10, RD29
  BT       L00801C83
  MOV.L    @RQ11, RD12
  SHAD     RD12, #-16, RD8
  MOV      RQ14, RQ13
  ADD      #2, RQ14
  MOV.W    RD8, @RQ13
  MOV.L    @(RQ11, 4), RD12
  SHAD     RD12, #-16, RD8
  MOV      RQ14, RQ13
  ADD      #2, RQ14
  MOV.W    RD8, @RQ13
  MOV.W    @(RQ11, 8), RD12
  MOV      RQ14, RQ13
  ADD      #2, RQ14
  MOV.W    RD12, @RQ13
  MOV.W    @(RQ11, 10), RD12
  MOV      RQ14, RQ13
  ADD      #2, RQ14
  MOV.W    RD12, @RQ13
  MOV.W    @(RQ11, 12), RD12
  MOV      RQ14, RQ13
  ADD      #2, RQ14
  MOV.W    RD12, @RQ13
  MOV.W    @(RQ11, 14), RD12
  MOV      RQ14, RQ13
  ADD      #2, RQ14
  MOV.W    RD12, @RQ13
  MOV.W    @(RQ11, 16), RD12
  MOV      RQ14, RQ13
  ADD      #2, RQ14
  MOV.W    RD12, @RQ13

L00801C82:
  MOV      RD10, RD12
  ADD      #1, RD10
  MOV      RQ11, RQ13
  ADD      #128, RQ11
  BRA      L00801C81

L00801C83:
  MOV      #0, RD10
  MOV.Q    lines, RQ28
  MOV      RQ28, RQ9

L00801C84:
  MOV.L    numlines, RD27
  CMP/GT   RD10, RD27
  BT       L00801C86
  MOV.W    @(RQ9, 24), RD12
  MOV      RQ14, RQ13
  ADD      #2, RQ14
  MOV.W    RD12, @RQ13
  MOV.W    @(RQ9, 26), RD12
  MOV      RQ14, RQ13
  ADD      #2, RQ14
  MOV.W    RD12, @RQ13
  MOV.W    @(RQ9, 28), RD12
  MOV      RQ14, RQ13
  ADD      #2, RQ14
  MOV.W    RD12, @RQ13
  MOV      #0, RD26
  MOV.L    RD26, @(R15, 28)

L00801C87:
  MOV.L    @(R15, 28), RD25
  MOV      #2, RD7
  CMP/GE   RD7, RD25
  BT       L00801C89
  ADD      RQ9, #36, RQ13
  MOV.L    @(R15, 28), RD6
  MOV.W    @(RQ13, RD6), RD12
  CMP/EQ   #-1, RD12
  BT       L00801C8A
  BRA      L00801C88

L00801C8A:
  ADD      RQ9, #36, RQ13
  MOV.L    @(R15, 28), RD25
  EXTS.L   RD25
  MOV.W    @(RQ13, RD25), RD12
  MOV.Q    sides, RQ26
  MOV      #24, RD7
  MUL.L    RD12, RD7
  STS      R0, RQ13
  ADD      RQ26, RQ13
  MOV      RQ13, RQ27
  MOV.L    @RQ27, RD12
  SHAD     RD12, #-16, RD8
  MOV      RQ14, RQ13
  ADD      #2, RQ14
  MOV.W    RD8, @RQ13
  MOV.L    @(RQ27, 4), RD12
  SHAD     RD12, #-16, RD8
  MOV      RQ14, RQ13
  ADD      #2, RQ14
  MOV.W    RD8, @RQ13
  MOV.W    @(RQ27, 8), RD12
  MOV      RQ14, RQ13
  ADD      #2, RQ14
  MOV.W    RD12, @RQ13
  MOV.W    @(RQ27, 10), RD12
  MOV      RQ14, RQ13
  ADD      #2, RQ14
  MOV.W    RD12, @RQ13
  MOV.W    @(RQ27, 12), RD12
  MOV      RQ14, RQ13
  ADD      #2, RQ14
  MOV.W    RD12, @RQ13
  MOV.Q    RQ27, @(R15, 16)

L00801C88:
  MOV.L    @(R15, 28), RD5
  ADD      #1, RD5
  MOV.L    RD5, @(R15, 28)
  BRA      L00801C87

L00801C89:

L00801C85:
  MOV      RD10, RD12
  ADD      #1, RD10
  MOV      RQ9, RQ13
  ADD      #96, RQ9
  BRA      L00801C84

L00801C86:
  MOV      RQ14, RQ4
  MOV.Q    RQ4, save_p

L00E005D1:
  ADD      #56, R15
  BRA      L00C00626

P_UnArchiveWorld:
  PUSH     LR
  BSR      L00C00630
  ADD      #-56, R15
  MOV.Q    save_p, RQ31
  MOV      RQ31, RQ14
  MOV      #0, RD10
  MOV.Q    sectors, RQ30
  MOV      RQ30, RQ11

L00801C8B:
  MOV.L    numsectors, RD29
  CMP/GT   RD10, RD29
  BT       L00801C8D
  MOV      RQ14, RQ13
  ADD      #2, RQ14
  MOV.W    @RQ13, RD12
  SHAD     RD12, #16, RD8
  MOV.L    RD8, @RQ11
  MOV      RQ14, RQ13
  ADD      #2, RQ14
  MOV.W    @RQ13, RD12
  SHAD     RD12, #16, RD8
  MOV.L    RD8, @(RQ11, 4)
  MOV      RQ14, RQ13
  ADD      #2, RQ14
  MOV.W    @RQ13, RD12
  MOV.W    RD12, @(RQ11, 8)
  MOV      RQ14, RQ13
  ADD      #2, RQ14
  MOV.W    @RQ13, RD12
  MOV.W    RD12, @(RQ11, 10)
  MOV      RQ14, RQ13
  ADD      #2, RQ14
  MOV.W    @RQ13, RD12
  MOV.W    RD12, @(RQ11, 12)
  MOV      RQ14, RQ13
  ADD      #2, RQ14
  MOV.W    @RQ13, RD12
  MOV.W    RD12, @(RQ11, 14)
  MOV      RQ14, RQ13
  ADD      #2, RQ14
  MOV.W    @RQ13, RD12
  MOV.W    RD12, @(RQ11, 16)
  MOV      #0, RQ13
  MOV.Q    RQ13, @(RQ11, 104)
  MOV      #0, RQ13
  MOV.Q    RQ13, @(RQ11, 24)

L00801C8C:
  MOV      RD10, RD12
  ADD      #1, RD10
  MOV      RQ11, RQ13
  ADD      #128, RQ11
  BRA      L00801C8B

L00801C8D:
  MOV      #0, RD10
  MOV.Q    lines, RQ28
  MOV      RQ28, RQ9

L00801C8E:
  MOV.L    numlines, RD27
  CMP/GT   RD10, RD27
  BT       L00801C90
  MOV      RQ14, RQ13
  ADD      #2, RQ14
  MOV.W    @RQ13, RD12
  MOV.W    RD12, @(RQ9, 24)
  MOV      RQ14, RQ13
  ADD      #2, RQ14
  MOV.W    @RQ13, RD12
  MOV.W    RD12, @(RQ9, 26)
  MOV      RQ14, RQ13
  ADD      #2, RQ14
  MOV.W    @RQ13, RD12
  MOV.W    RD12, @(RQ9, 28)
  MOV      #0, RD26
  MOV.L    RD26, @(R15, 28)

L00801C91:
  MOV.L    @(R15, 28), RD25
  MOV      #2, RD7
  CMP/GE   RD7, RD25
  BT       L00801C93
  ADD      RQ9, #36, RQ13
  MOV.L    @(R15, 28), RD6
  MOV.W    @(RQ13, RD6), RD12
  CMP/EQ   #-1, RD12
  BT       L00801C94
  BRA      L00801C92

L00801C94:
  ADD      RQ9, #36, RQ13
  MOV.L    @(R15, 28), RD25
  EXTS.L   RD25
  MOV.W    @(RQ13, RD25), RD12
  MOV.Q    sides, RQ26
  MOV      #24, RD7
  MUL.L    RD12, RD7
  STS      R0, RQ13
  ADD      RQ26, RQ13
  MOV      RQ13, RQ27
  MOV      RQ14, RQ13
  ADD      #2, RQ14
  MOV.W    @RQ13, RD12
  SHAD     RD12, #16, RD8
  MOV.L    RD8, @RQ27
  MOV      RQ14, RQ13
  ADD      #2, RQ14
  MOV.W    @RQ13, RD12
  SHAD     RD12, #16, RD8
  MOV.L    RD8, @(RQ27, 4)
  MOV      RQ14, RQ13
  ADD      #2, RQ14
  MOV.W    @RQ13, RD12
  MOV.W    RD12, @(RQ27, 8)
  MOV      RQ14, RQ13
  ADD      #2, RQ14
  MOV.W    @RQ13, RD12
  MOV.W    RD12, @(RQ27, 10)
  MOV      RQ14, RQ13
  ADD      #2, RQ14
  MOV.W    @RQ13, RD12
  MOV.W    RD12, @(RQ27, 12)
  MOV.Q    RQ27, @(R15, 16)

L00801C92:
  MOV.L    @(R15, 28), RD5
  ADD      #1, RD5
  MOV.L    RD5, @(R15, 28)
  BRA      L00801C91

L00801C93:

L00801C8F:
  MOV      RD10, RD12
  ADD      #1, RD10
  MOV      RQ9, RQ13
  ADD      #96, RQ9
  BRA      L00801C8E

L00801C90:
  MOV      RQ14, RQ4
  MOV.Q    RQ4, save_p

L00E005D2:
  ADD      #56, R15
  BRA      L00C00626

P_ArchiveThinkers:
  PUSH     LR
  BSR      L00C00630
  ADD      #-104, R15
  MOV      thinkercap, RQ12
  MOV.Q    @(RQ12, 8), RQ13

L00801C95:
  MOV      thinkercap, RQ11
  MOV      RQ11, RQ14
  CMPQ/EQ  RQ14, RQ13
  BT       L00801C97
  ADD      RQ13, #16, RQ10
  MOV.Q    @RQ10, RQ14
  MOV      P_MobjThinker, RQ8
  MOV      RQ8, RQ9
  MOV      RQ14, RQ31
  MOV      RQ9, RQ30
  MOV.Q    RQ31, @(R15, 40)
  MOV.Q    RQ30, @(R15, 32)
  CMPQ/EQ  RQ30, RQ31
  BT       L00801C98
  MOV.Q    save_p, RQ29
  MOV      RQ29, RQ14
  ADD      #1, RQ29
  MOV      #1, RD28
  MOVU.B   RD28, @RQ14
  MOV      RQ29, RQ27
  MOV      #3, RQ26
  AND      RQ27, RQ26, RQ25
  MOV      #4, RQ30
  EXTS.L   RQ30, RQ30
  SUB      RQ30, RQ25, RQ27
  AND      RQ27, RQ26, RQ25
  LEA.B    @(RQ29, RQ25), RQ29
  MOV      RQ29, RQ31
  MOV      #232, RQ27
  EXTS.L   RQ27, RQ27
  MOV.Q    RQ31, @(R15, 72)
  MOV.Q    RQ29, save_p
  MOV.Q    RQ27, @(R15, 24)
  MOV      RQ31, RQ4
  MOV      RQ13, RQ5
  MOV      RQ27, RQ6
  BSR      memcpy
  MOV.Q    save_p, RQ8
  ADD      #232, RQ8
  MOV.Q    @(RQ31, 152), RQ14
  MOV      states, RQ9
  MOV      RQ14, RD10
  SUB      RQ9, RD10
  MOV      #0x06666667, RD7
  MOV      #1638, RD7
  DMULS    RD7, RD10
  STS      R1, RD10
  MOV      RQ10, RQ14
  MOV.Q    RQ14, @(RQ31, 152)
  MOV.Q    @(RQ31, 192), RQ14
  MOV.L    RD10, @(R15, 4)
  MOV.Q    RQ8, save_p
  MOV      #0, RQ11
  CMPQ/EQ  RQ11, RQ14
  BT       L00801C99
  MOV.Q    @(R15, 72), RQ25
  MOV.Q    @(RQ25, 192), RQ14
  MOV      players, RQ26
  MOV      RQ14, RD27
  SUB      RQ26, RD27
  MOV      #0x00CCCCCD, RD7
  MOV      #204, RD7
  DMULS    RD7, RD27
  STS      R1, RD7
  MOV      RD7, RD27
  ADD      RD27, #1, RD28
  MOV      RQ28, RQ14
  MOV.Q    RQ14, @(RQ25, 192)
  MOV.L    RD27, @(R15, 4)

L00801C99:
  BRA      L00801C96

L00801C98:

L00801C96:
  MOV.Q    @(RQ13, 8), RQ13
  BRA      L00801C95

L00801C97:
  MOV.Q    save_p, RQ7
  MOV      RQ7, RQ14
  ADD      #1, RQ7
  MOV      #0, RD6
  MOVU.B   RD6, @RQ14
  MOV.Q    RQ7, save_p

L00E005D3:
  ADD      #104, R15
  BRA      L00C00626

P_UnArchiveThinkers:
  PUSH     LR
  BSR      L00C00630
  ADD      #-152, R15
  MOV      thinkercap, RQ10
  MOV.Q    @(RQ10, 8), RQ11

L00801C9A:
  MOV      thinkercap, RQ9
  MOV      RQ9, RQ14
  CMPQ/EQ  RQ14, RQ11
  BT       L00801C9B
  MOV.Q    @(RQ11, 8), RQ8
  ADD      RQ11, #16, RQ31
  MOV.Q    @RQ31, RQ14
  MOV      P_MobjThinker, RQ30
  MOV      RQ30, RQ12
  MOV      RQ14, RQ29
  MOV      RQ12, RQ28
  MOV.Q    RQ8, @(R15, 88)
  MOV.Q    RQ31, @(R15, 72)
  MOV.Q    RQ29, @(R15, 56)
  MOV.Q    RQ28, @(R15, 48)
  CMPQ/EQ  RQ28, RQ29
  BT       L00801C9C
  MOV      RQ11, RQ4
  BSR      P_RemoveMobj
  BRA      L00801C9D

L00801C9C:
  MOV      RQ11, RQ4
  BSR      Z_Free

L00801C9D:
  MOV.Q    @(R15, 88), RQ27
  MOV      RQ27, RQ11
  BRA      L00801C9A

L00801C9B:
  BSR      P_InitThinkers

L00801C9E:
  MOV.Q    save_p, RQ26
  MOV      RQ26, RQ14
  ADD      #1, RQ26
  MOVU.B   @RQ14, RD25
  EXTU.B   RD25, RD27
  MOV      RD27, RD28
  MOV.L    RD28, @(R15, 100)
  MOV.Q    RQ26, save_p
  MOV.L    RD25, @(R15, 44)
  CMP/EQ   #0, RD28
  BT       L00801CA1
  MOV.L    @(R15, 100), RD7
  CMP/EQ   #1, RD7
  BT       L00801CA2
  BRA      L00801CA3

L00801CA1:
  BRA      L00E005D4

L00801CA2:
  MOV.Q    save_p, RQ25
  MOV      #3, RQ26
  AND      RQ25, RQ26, RQ27
  MOV      #4, RQ28
  EXTS.L   RQ28, RQ28
  SUB      RQ28, RQ27, RQ25
  AND      RQ25, RQ26, RQ27
  MOV.Q    save_p, RQ29
  LEA.B    @(RQ29, RQ27), RQ29
  MOV      #0, RQ14
  MOV.Q    RQ29, save_p
  MOV.Q    RQ25, @(R15, 32)
  MOV      #232, RD4
  MOV      #50, RD5
  MOV      RQ14, RQ6
  BSR      Z_Malloc
  MOV      RQ2, RQ13
  MOV      #232, RQ25
  EXTS.L   RQ25, RQ25
  MOV.Q    RQ25, @(R15, 32)
  MOV      RQ13, RQ4
  MOV.Q    save_p, RQ5
  MOV      RQ25, RQ6
  BSR      memcpy
  MOV.Q    save_p, RQ30
  ADD      #232, RQ30
  MOV.Q    @(RQ13, 152), RQ14
  MOV      states, RQ31
  MOV      #40, RD7
  MUL.L    RQ14, RD7
  STS      R0, RQ12
  ADD      RQ31, RQ12
  MOV.Q    RQ12, @(RQ13, 152)
  MOV      #0, RQ14
  MOV.Q    RQ14, @(RQ13, 176)
  MOV.Q    @(RQ13, 192), RQ14
  MOV.Q    RQ30, save_p
  MOV      #0, RQ8
  CMPQ/EQ  RQ8, RQ14
  BT       L00801CA4
  MOV.Q    @(RQ13, 192), RQ14
  MOV      #1, RQ25
  SUB      RQ14, RQ25, RQ26
  MOV      players, RQ27
  MOV      #320, RD7
  MUL.L    RQ26, RD7
  STS      R0, RQ14
  ADD      RQ27, RQ14
  MOV.Q    RQ14, @(RQ13, 192)
  MOV.Q    @(RQ13, 192), RQ14
  MOV.Q    RQ13, @RQ14
  MOV.Q    RQ26, @(R15, 32)

L00801CA4:
  MOV      RQ13, RQ4
  BSR      P_SetThingPosition
  MOV.L    @(RQ13, 128), RD25
  MOV      mobjinfo, RQ26
  EXTS.L   RD25
  MOV      #92, RD7
  MUL.L    RD25, RD7
  STS      R0, RQ14
  ADD      RQ26, RQ14
  MOV.Q    RQ14, @(RQ13, 136)
  MOV.Q    @(RQ13, 88), RQ14
  MOV.Q    @RQ14, RQ12
  MOV.L    @RQ12, RD25
  MOV.L    RD25, @(RQ13, 96)
  MOV.Q    @(RQ13, 88), RQ14
  MOV.Q    @RQ14, RQ12
  MOV.L    @(RQ12, 4), RD25
  MOV.L    RD25, @(RQ13, 100)
  MOV      P_MobjThinker, RQ27
  MOV      RQ27, RQ14
  MOV      RQ13, RQ28
  ADD      RQ28, #16, RQ29
  MOV.Q    RQ14, @RQ29
  MOV      RQ13, RQ14
  MOV.Q    RQ29, @(R15, 72)
  MOV.L    RD25, @(R15, 44)
  MOV      RQ14, RQ4
  BSR      P_AddThinker
  BRA      L00801CA0

L00801CA3:
  MOV      L00C00880, RQ4
  MOV.L    @(R15, 100), RD5
  BSR      I_Error

L00801CA0:
  BRA      L00801C9E

L00801C9F:

L00E005D4:
  ADD      #152, R15
  BRA      L00C00626

P_ArchiveSpecials:
  PUSH     LR
  BSR      L00C00630
  ADD      #-152, R15
  MOV      thinkercap, RQ11
  MOV.Q    @(RQ11, 8), RQ13

L00801CA5:
  MOV      thinkercap, RQ10
  MOV      RQ10, RQ14
  CMPQ/EQ  RQ14, RQ13
  BT       L00801CA7
  ADD      RQ13, #16, RQ9
  MOV.Q    @RQ9, RQ14
  MOV      #0, RQ8
  MOV      RQ14, RQ31
  MOV      RQ8, RQ30
  MOV.Q    RQ9, @(R15, 56)
  MOV.Q    RQ8, @(R15, 48)
  MOV.Q    RQ31, @(R15, 40)
  MOV.Q    RQ30, @(R15, 32)
  CMPQ/EQ  RQ30, RQ31
  BT       L00801CA8
  MOV      #0, RD12

L00801CA9:
  CMP/GE   #30, RD12
  BT       L00801CAB
  MOV      activeceilings, RQ29
  EXTS.L   RD12
  MOV.Q    @(RQ29, RD12), RQ14
  CMPQ/EQ  RQ13, RQ14
  BT       L00801CAC
  BRA      L00801CAB

L00801CAC:

L00801CAA:
  ADD      #1, RD12
  BRA      L00801CA9

L00801CAB:
  CMP/GE   #30, RD12
  BT       L00801CAD
  MOV.Q    save_p, RQ28
  MOV      RQ28, RQ14
  ADD      #1, RQ28
  MOV      #0, RD27
  MOVU.B   RD27, @RQ14
  MOV      RQ28, RQ26
  MOV      #3, RQ25
  AND      RQ26, RQ25, RQ29
  MOV      #4, RQ30
  EXTS.L   RQ30, RQ30
  SUB      RQ30, RQ29, RQ26
  AND      RQ26, RQ25, RQ29
  LEA.B    @(RQ28, RQ29), RQ28
  MOV      RQ28, RQ31
  MOV      #72, RQ26
  EXTS.L   RQ26, RQ26
  MOV.Q    RQ31, @(R15, 120)
  MOV.Q    RQ30, @(R15, 8)
  MOV.Q    RQ29, @(R15, 16)
  MOV.Q    RQ28, save_p
  MOV.Q    RQ26, @(R15, 24)
  MOV      RQ31, RQ4
  MOV      RQ13, RQ5
  MOV      RQ26, RQ6
  BSR      memcpy
  MOV.Q    save_p, RQ7
  ADD      #72, RQ7
  MOV.Q    @(RQ31, 32), RQ14
  MOV.Q    sectors, RQ6
  SUB      RQ14, RQ6, RD5
  SHAD.Q   #-7, RD5
  MOV      RQ5, RQ14
  MOV.Q    RQ14, @(RQ31, 32)
  MOV.Q    RQ7, save_p
  MOV.L    RD5, @(R15, 4)

L00801CAD:
  BRA      L00801CA6

L00801CA8:
  ADD      RQ13, #16, RQ4
  MOV.Q    @RQ4, RQ14
  MOV      T_MoveCeiling, RQ2
  MOV      RQ2, RQ3
  MOV      RQ14, RQ23
  MOV      RQ3, RQ22
  MOV.Q    RQ4, @(R15, 56)
  MOV.Q    RQ3, @(R15, 48)
  MOV.Q    RQ23, @(R15, 40)
  MOV.Q    RQ22, @(R15, 32)
  CMPQ/EQ  RQ22, RQ23
  BT       L00801CAE
  MOV.Q    save_p, RQ25
  MOV      RQ25, RQ14
  ADD      #1, RQ25
  MOV      #0, RD26
  MOVU.B   RD26, @RQ14
  MOV      RQ25, RQ27
  MOV      #3, RQ28
  AND      RQ27, RQ28, RQ29
  MOV      #4, RQ30
  EXTS.L   RQ30, RQ30
  SUB      RQ30, RQ29, RQ27
  AND      RQ27, RQ28, RQ29
  LEA.B    @(RQ25, RQ29), RQ25
  MOV      RQ25, RQ31
  MOV      #72, RQ27
  EXTS.L   RQ27, RQ27
  MOV.Q    RQ31, @(R15, 120)
  MOV.Q    RQ30, @(R15, 8)
  MOV.Q    RQ29, @(R15, 16)
  MOV.Q    RQ27, @(R15, 24)
  MOV.Q    RQ25, save_p
  MOV      RQ31, RQ4
  MOV      RQ13, RQ5
  MOV      RQ27, RQ6
  BSR      memcpy
  MOV.Q    save_p, RQ21
  ADD      #72, RQ21
  MOV.Q    @(RQ31, 32), RQ14
  MOV.Q    sectors, RQ20
  SUB      RQ14, RQ20, RD19
  SHAD.Q   #-7, RD19
  MOV      RQ19, RQ14
  MOV.Q    RQ14, @(RQ31, 32)
  MOV.Q    RQ21, save_p
  MOV.L    RD19, @(R15, 4)
  BRA      L00801CA6

L00801CAE:
  ADD      RQ13, #16, RQ18
  MOV.Q    @RQ18, RQ14
  MOV      T_VerticalDoor, RQ16
  MOV      RQ16, RQ17
  MOV      RQ14, RQ19
  MOV      RQ17, RQ20
  MOV.Q    RQ20, @(R15, 32)
  MOV.Q    RQ19, @(R15, 40)
  MOV.Q    RQ18, @(R15, 56)
  MOV.Q    RQ17, @(R15, 48)
  CMPQ/EQ  RQ20, RQ19
  BT       L00801CAF
  MOV.Q    save_p, RQ25
  MOV      RQ25, RQ14
  ADD      #1, RQ25
  MOV      #1, RD26
  MOVU.B   RD26, @RQ14
  MOV      RQ25, RQ27
  MOV      #3, RQ28
  AND      RQ27, RQ28, RQ29
  MOV      #4, RQ30
  EXTS.L   RQ30, RQ30
  SUB      RQ30, RQ29, RQ27
  AND      RQ27, RQ28, RQ29
  LEA.B    @(RQ25, RQ29), RQ25
  MOV      RQ25, RQ31
  MOV      #64, RQ27
  EXTS.L   RQ27, RQ27
  MOV.Q    RQ31, @(R15, 112)
  MOV.Q    RQ30, @(R15, 8)
  MOV.Q    RQ29, @(R15, 16)
  MOV.Q    RQ27, @(R15, 24)
  MOV.Q    RQ25, save_p
  MOV      RQ31, RQ4
  MOV      RQ13, RQ5
  MOV      RQ27, RQ6
  BSR      memcpy
  MOV.Q    save_p, RQ8
  ADD      #64, RQ8
  MOV.Q    @(RQ31, 32), RQ14
  MOV.Q    sectors, RQ9
  SUB      RQ14, RQ9, RD10
  SHAD.Q   #-7, RD10
  MOV      RQ10, RQ14
  MOV.Q    RQ14, @(RQ31, 32)
  MOV.L    RD10, @(R15, 4)
  MOV.Q    RQ8, save_p
  BRA      L00801CA6

L00801CAF:
  ADD      RQ13, #16, RQ16
  MOV.Q    @RQ16, RQ14
  MOV      T_MoveFloor, RQ18
  MOV      RQ18, RQ17
  MOV      RQ14, RQ19
  MOV      RQ17, RQ20
  MOV.Q    RQ20, @(R15, 32)
  MOV.Q    RQ19, @(R15, 40)
  MOV.Q    RQ17, @(R15, 48)
  MOV.Q    RQ16, @(R15, 56)
  CMPQ/EQ  RQ20, RQ19
  BT       L00801CB0
  MOV.Q    save_p, RQ25
  MOV      RQ25, RQ14
  ADD      #1, RQ25
  MOV      #2, RD26
  MOVU.B   RD26, @RQ14
  MOV      RQ25, RQ27
  MOV      #3, RQ28
  AND      RQ27, RQ28, RQ29
  MOV      #4, RQ30
  EXTS.L   RQ30, RQ30
  SUB      RQ30, RQ29, RQ27
  AND      RQ27, RQ28, RQ29
  LEA.B    @(RQ25, RQ29), RQ25
  MOV      RQ25, RQ31
  MOV      #64, RQ27
  EXTS.L   RQ27, RQ27
  MOV.Q    RQ31, @(R15, 104)
  MOV.Q    RQ30, @(R15, 8)
  MOV.Q    RQ29, @(R15, 16)
  MOV.Q    RQ27, @(R15, 24)
  MOV.Q    RQ25, save_p
  MOV      RQ31, RQ4
  MOV      RQ13, RQ5
  MOV      RQ27, RQ6
  BSR      memcpy
  MOV.Q    save_p, RQ8
  ADD      #64, RQ8
  MOV.Q    @(RQ31, 32), RQ14
  MOV.Q    sectors, RQ9
  SUB      RQ14, RQ9, RD10
  SHAD.Q   #-7, RD10
  MOV      RQ10, RQ14
  MOV.Q    RQ14, @(RQ31, 32)
  MOV.L    RD10, @(R15, 4)
  MOV.Q    RQ8, save_p
  BRA      L00801CA6

L00801CB0:
  ADD      RQ13, #16, RQ16
  MOV.Q    @RQ16, RQ14
  MOV      T_PlatRaise, RQ18
  MOV      RQ18, RQ17
  MOV      RQ14, RQ19
  MOV      RQ17, RQ20
  MOV.Q    RQ20, @(R15, 32)
  MOV.Q    RQ19, @(R15, 40)
  MOV.Q    RQ17, @(R15, 48)
  MOV.Q    RQ16, @(R15, 56)
  CMPQ/EQ  RQ20, RQ19
  BT       L00801CB1
  MOV.Q    save_p, RQ25
  MOV      RQ25, RQ14
  ADD      #1, RQ25
  MOV      #3, RD26
  MOVU.B   RD26, @RQ14
  MOV      RQ25, RQ27
  MOV      #3, RQ28
  AND      RQ27, RQ28, RQ29
  MOV      #4, RQ30
  EXTS.L   RQ30, RQ30
  SUB      RQ30, RQ29, RQ27
  AND      RQ27, RQ28, RQ29
  LEA.B    @(RQ25, RQ29), RQ25
  MOV      RQ25, RQ31
  MOV      #72, RQ27
  EXTS.L   RQ27, RQ27
  MOV.Q    RQ31, @(R15, 96)
  MOV.Q    RQ30, @(R15, 8)
  MOV.Q    RQ29, @(R15, 16)
  MOV.Q    RQ27, @(R15, 24)
  MOV.Q    RQ25, save_p
  MOV      RQ31, RQ4
  MOV      RQ13, RQ5
  MOV      RQ27, RQ6
  BSR      memcpy
  MOV.Q    save_p, RQ8
  ADD      #72, RQ8
  MOV.Q    @(RQ31, 24), RQ14
  MOV.Q    sectors, RQ9
  SUB      RQ14, RQ9, RD10
  SHAD.Q   #-7, RD10
  MOV      RQ10, RQ14
  MOV.Q    RQ14, @(RQ31, 24)
  MOV.L    RD10, @(R15, 4)
  MOV.Q    RQ8, save_p
  BRA      L00801CA6

L00801CB1:
  ADD      RQ13, #16, RQ16
  MOV.Q    @RQ16, RQ14
  MOV      T_LightFlash, RQ18
  MOV      RQ18, RQ17
  MOV      RQ14, RQ19
  MOV      RQ17, RQ20
  MOV.Q    RQ20, @(R15, 32)
  MOV.Q    RQ19, @(R15, 40)
  MOV.Q    RQ17, @(R15, 48)
  MOV.Q    RQ16, @(R15, 56)
  CMPQ/EQ  RQ20, RQ19
  BT       L00801CB2
  MOV.Q    save_p, RQ25
  MOV      RQ25, RQ14
  ADD      #1, RQ25
  MOV      #4, RD26
  MOVU.B   RD26, @RQ14
  MOV      RQ25, RQ27
  MOV      #3, RQ28
  AND      RQ27, RQ28, RQ29
  EXTS.L   RD26, RQ30
  SUB      RQ30, RQ29, RQ27
  AND      RQ27, RQ28, RQ29
  LEA.B    @(RQ25, RQ29), RQ25
  MOV      RQ25, RQ31
  MOV      #56, RQ27
  EXTS.L   RQ27, RQ27
  MOV.Q    RQ31, @(R15, 88)
  MOV.Q    RQ30, @(R15, 8)
  MOV.Q    RQ29, @(R15, 16)
  MOV.Q    RQ27, @(R15, 24)
  MOV.Q    RQ25, save_p
  MOV      RQ31, RQ4
  MOV      RQ13, RQ5
  MOV      RQ27, RQ6
  BSR      memcpy
  MOV.Q    save_p, RQ8
  ADD      #56, RQ8
  MOV.Q    @(RQ31, 24), RQ14
  MOV.Q    sectors, RQ9
  SUB      RQ14, RQ9, RD10
  SHAD.Q   #-7, RD10
  MOV      RQ10, RQ14
  MOV.Q    RQ14, @(RQ31, 24)
  MOV.L    RD10, @(R15, 4)
  MOV.Q    RQ8, save_p
  BRA      L00801CA6

L00801CB2:
  ADD      RQ13, #16, RQ16
  MOV.Q    @RQ16, RQ14
  MOV      T_StrobeFlash, RQ18
  MOV      RQ18, RQ17
  MOV      RQ14, RQ19
  MOV      RQ17, RQ20
  MOV.Q    RQ20, @(R15, 32)
  MOV.Q    RQ19, @(R15, 40)
  MOV.Q    RQ17, @(R15, 48)
  MOV.Q    RQ16, @(R15, 56)
  CMPQ/EQ  RQ20, RQ19
  BT       L00801CB3
  MOV.Q    save_p, RQ25
  MOV      RQ25, RQ14
  ADD      #1, RQ25
  MOV      #5, RD26
  MOVU.B   RD26, @RQ14
  MOV      RQ25, RQ27
  MOV      #3, RQ28
  AND      RQ27, RQ28, RQ29
  MOV      #4, RQ30
  EXTS.L   RQ30, RQ30
  SUB      RQ30, RQ29, RQ27
  AND      RQ27, RQ28, RQ29
  LEA.B    @(RQ25, RQ29), RQ25
  MOV      RQ25, RQ31
  MOV      #56, RQ27
  EXTS.L   RQ27, RQ27
  MOV.Q    RQ31, @(R15, 80)
  MOV.Q    RQ30, @(R15, 8)
  MOV.Q    RQ29, @(R15, 16)
  MOV.Q    RQ27, @(R15, 24)
  MOV.Q    RQ25, save_p
  MOV      RQ31, RQ4
  MOV      RQ13, RQ5
  MOV      RQ27, RQ6
  BSR      memcpy
  MOV.Q    save_p, RQ8
  ADD      #56, RQ8
  MOV.Q    @(RQ31, 24), RQ14
  MOV.Q    sectors, RQ9
  SUB      RQ14, RQ9, RD10
  SHAD.Q   #-7, RD10
  MOV      RQ10, RQ14
  MOV.Q    RQ14, @(RQ31, 24)
  MOV.L    RD10, @(R15, 4)
  MOV.Q    RQ8, save_p
  BRA      L00801CA6

L00801CB3:
  ADD      RQ13, #16, RQ16
  MOV.Q    @RQ16, RQ14
  MOV      T_Glow, RQ18
  MOV      RQ18, RQ17
  MOV      RQ14, RQ19
  MOV      RQ17, RQ20
  MOV.Q    RQ20, @(R15, 32)
  MOV.Q    RQ19, @(R15, 40)
  MOV.Q    RQ17, @(R15, 48)
  MOV.Q    RQ16, @(R15, 56)
  CMPQ/EQ  RQ20, RQ19
  BT       L00801CB4
  MOV.Q    save_p, RQ25
  MOV      RQ25, RQ14
  ADD      #1, RQ25
  MOV      #6, RD26
  MOVU.B   RD26, @RQ14
  MOV      RQ25, RQ27
  MOV      #3, RQ28
  AND      RQ27, RQ28, RQ29
  MOV      #4, RQ30
  EXTS.L   RQ30, RQ30
  SUB      RQ30, RQ29, RQ27
  AND      RQ27, RQ28, RQ29
  LEA.B    @(RQ25, RQ29), RQ25
  MOV      RQ25, RQ31
  MOV      #48, RQ27
  EXTS.L   RQ27, RQ27
  MOV.Q    RQ31, @(R15, 72)
  MOV.Q    RQ30, @(R15, 8)
  MOV.Q    RQ29, @(R15, 16)
  MOV.Q    RQ27, @(R15, 24)
  MOV.Q    RQ25, save_p
  MOV      RQ31, RQ4
  MOV      RQ13, RQ5
  MOV      RQ27, RQ6
  BSR      memcpy
  MOV.Q    save_p, RQ8
  ADD      #48, RQ8
  MOV.Q    @(RQ31, 24), RQ14
  MOV.Q    sectors, RQ9
  SUB      RQ14, RQ9, RD10
  SHAD.Q   #-7, RD10
  MOV      RQ10, RQ14
  MOV.Q    RQ14, @(RQ31, 24)
  MOV.L    RD10, @(R15, 4)
  MOV.Q    RQ8, save_p
  BRA      L00801CA6

L00801CB4:

L00801CA6:
  MOV.Q    @(RQ13, 8), RQ13
  BRA      L00801CA5

L00801CA7:
  MOV.Q    save_p, RQ16
  MOV      RQ16, RQ14
  ADD      #1, RQ16
  MOV      #7, RD17
  MOVU.B   RD17, @RQ14
  MOV.Q    RQ16, save_p

L00E005D5:
  ADD      #152, R15
  BRA      L00C00626

P_UnArchiveSpecials:
  PUSH     LR
  BSR      L00C00630
  ADD      #-168, R15

L00801CB5:
  MOV.Q    save_p, RQ11
  MOV      RQ11, RQ14
  ADD      #1, RQ11
  MOVU.B   @RQ14, RD10
  EXTU.B   RD10, RD9
  MOV      RD9, RD12
  MOV.Q    RQ11, save_p
  MOV      #4, RD8
  CMP/GE   RD8, RD12
  BT       L00801CC1
  CMP/EQ   RD12
  BT       L00801CB9
  CMP/EQ   #1, RD12
  BT       L00801CBA
  CMP/EQ   #2, RD12
  BT       L00801CBB
  CMP/EQ   #3, RD12
  BT       L00801CBC

L00801CC1:
  CMP/EQ   #4, RD12
  BT       L00801CBD
  CMP/EQ   #5, RD12
  BT       L00801CBE
  CMP/EQ   #6, RD12
  BT       L00801CBF
  CMP/EQ   #7, RD12
  BT       L00801CB8
  BRA      L00801CC0

L00801CB8:
  BRA      L00E005D6

L00801CB9:
  MOV.Q    save_p, RQ31
  MOV      #3, RQ30
  AND      RQ31, RQ30, RQ29
  MOV      #4, RQ28
  EXTS.L   RQ28, RQ28
  SUB      RQ28, RQ29, RQ31
  AND      RQ31, RQ30, RQ29
  MOV.Q    save_p, RQ27
  LEA.B    @(RQ27, RQ29), RQ27
  MOV      #0, RQ14
  MOV.Q    RQ31, @(R15, 48)
  MOV.Q    RQ29, @(R15, 40)
  MOV.Q    RQ28, @(R15, 32)
  MOV.Q    RQ27, save_p
  MOV      #72, RD4
  MOV      #50, RD5
  MOV      RQ14, RQ6
  BSR      Z_Malloc
  MOV      RQ2, RQ13
  MOV      #72, RQ31
  EXTS.L   RQ31, RQ31
  MOV.Q    RQ31, @(R15, 48)
  MOV      RQ13, RQ4
  MOV.Q    save_p, RQ5
  MOV      RQ31, RQ6
  BSR      memcpy
  MOV.Q    save_p, RQ26
  ADD      #72, RQ26
  MOV.Q    @(RQ13, 32), RQ14
  MOV.Q    sectors, RQ25
  SHAD     RQ14, #7, RQ7
  ADD      RQ25, RQ7
  MOV.Q    RQ7, @(RQ13, 32)
  MOV.Q    @(RQ13, 32), RQ14
  MOV.Q    RQ13, @(RQ14, 104)
  MOV      RQ13, RQ6
  ADD      RQ6, #16, RQ5
  MOV.Q    @RQ5, RQ14
  MOV.Q    RQ26, save_p
  MOV.Q    RQ7, @(R15, 24)
  MOV.Q    RQ6, @(R15, 16)
  MOV.Q    RQ5, @(R15, 8)
  MOV      #0, RQ4
  CMPQ/EQ  RQ4, RQ14
  BT       L00801CC2
  MOV      T_MoveCeiling, RQ3
  MOV      RQ3, RQ14
  MOV      RQ13, RQ2
  ADD      RQ2, #16, RQ23
  MOV.Q    RQ14, @RQ23
  MOV.Q    RQ2, @(R15, 16)
  MOV.Q    RQ23, @(R15, 8)

L00801CC2:
  MOV      RQ13, RQ14
  MOV      RQ14, RQ4
  BSR      P_AddThinker
  MOV      RQ13, RQ4
  BSR      P_AddActiveCeiling
  BRA      L00801CB7

L00801CBA:
  MOV.Q    save_p, RQ25
  MOV      #3, RQ26
  AND      RQ25, RQ26, RQ27
  MOV      #4, RQ28
  EXTS.L   RQ28, RQ28
  SUB      RQ28, RQ27, RQ25
  AND      RQ25, RQ26, RQ27
  MOV.Q    save_p, RQ29
  LEA.B    @(RQ29, RQ27), RQ29
  MOV      #0, RQ14
  MOV.Q    RQ29, save_p
  MOV.Q    RQ28, @(R15, 32)
  MOV.Q    RQ27, @(R15, 40)
  MOV.Q    RQ25, @(R15, 48)
  MOV      #64, RD4
  MOV      #50, RD5
  MOV      RQ14, RQ6
  BSR      Z_Malloc
  MOV      RQ2, RQ30
  MOV      #64, RQ25
  EXTS.L   RQ25, RQ25
  MOV.Q    RQ30, @(R15, 112)
  MOV.Q    RQ25, @(R15, 48)
  MOV      RQ30, RQ4
  MOV.Q    save_p, RQ5
  MOV      RQ25, RQ6
  BSR      memcpy
  MOV.Q    save_p, RQ31
  ADD      #64, RQ31
  MOV.Q    @(RQ30, 32), RQ14
  MOV.Q    sectors, RQ8
  SHAD     RQ14, #7, RQ9
  ADD      RQ8, RQ9
  MOV.Q    RQ9, @(RQ30, 32)
  MOV.Q    @(RQ30, 32), RQ14
  MOV.Q    RQ30, @(RQ14, 104)
  MOV      T_VerticalDoor, RQ10
  MOV      RQ10, RQ14
  MOV      RQ30, RQ11
  ADD      RQ11, #16, RQ29
  MOV.Q    RQ14, @RQ29
  MOV      RQ30, RQ14
  MOV.Q    RQ11, @(R15, 16)
  MOV.Q    RQ9, @(R15, 24)
  MOV.Q    RQ31, save_p
  MOV.Q    RQ29, @(R15, 8)
  MOV      RQ14, RQ4
  BSR      P_AddThinker
  BRA      L00801CB7

L00801CBB:
  MOV.Q    save_p, RQ25
  MOV      #3, RQ26
  AND      RQ25, RQ26, RQ27
  MOV      #4, RQ28
  EXTS.L   RQ28, RQ28
  SUB      RQ28, RQ27, RQ25
  AND      RQ25, RQ26, RQ27
  MOV.Q    save_p, RQ29
  LEA.B    @(RQ29, RQ27), RQ29
  MOV      #0, RQ14
  MOV.Q    RQ29, save_p
  MOV.Q    RQ28, @(R15, 32)
  MOV.Q    RQ27, @(R15, 40)
  MOV.Q    RQ25, @(R15, 48)
  MOV      #64, RD4
  MOV      #50, RD5
  MOV      RQ14, RQ6
  BSR      Z_Malloc
  MOV      RQ2, RQ30
  MOV      #64, RQ25
  EXTS.L   RQ25, RQ25
  MOV.Q    RQ30, @(R15, 104)
  MOV.Q    RQ25, @(R15, 48)
  MOV      RQ30, RQ4
  MOV.Q    save_p, RQ5
  MOV      RQ25, RQ6
  BSR      memcpy
  MOV.Q    save_p, RQ31
  ADD      #64, RQ31
  MOV.Q    @(RQ30, 32), RQ14
  MOV.Q    sectors, RQ8
  SHAD     RQ14, #7, RQ9
  ADD      RQ8, RQ9
  MOV.Q    RQ9, @(RQ30, 32)
  MOV.Q    @(RQ30, 32), RQ14
  MOV.Q    RQ30, @(RQ14, 104)
  MOV      T_MoveFloor, RQ10
  MOV      RQ10, RQ14
  MOV      RQ30, RQ11
  ADD      RQ11, #16, RQ29
  MOV.Q    RQ14, @RQ29
  MOV      RQ30, RQ14
  MOV.Q    RQ11, @(R15, 16)
  MOV.Q    RQ9, @(R15, 24)
  MOV.Q    RQ31, save_p
  MOV.Q    RQ29, @(R15, 8)
  MOV      RQ14, RQ4
  BSR      P_AddThinker
  BRA      L00801CB7

L00801CBC:
  MOV.Q    save_p, RQ25
  MOV      #3, RQ26
  AND      RQ25, RQ26, RQ27
  MOV      #4, RQ28
  EXTS.L   RQ28, RQ28
  SUB      RQ28, RQ27, RQ25
  AND      RQ25, RQ26, RQ27
  MOV.Q    save_p, RQ29
  LEA.B    @(RQ29, RQ27), RQ29
  MOV      #0, RQ14
  MOV.Q    RQ29, save_p
  MOV.Q    RQ28, @(R15, 32)
  MOV.Q    RQ27, @(R15, 40)
  MOV.Q    RQ25, @(R15, 48)
  MOV      #72, RD4
  MOV      #50, RD5
  MOV      RQ14, RQ6
  BSR      Z_Malloc
  MOV      RQ2, RQ30
  MOV      #72, RQ25
  EXTS.L   RQ25, RQ25
  MOV.Q    RQ30, @(R15, 96)
  MOV.Q    RQ25, @(R15, 48)
  MOV      RQ30, RQ4
  MOV.Q    save_p, RQ5
  MOV      RQ25, RQ6
  BSR      memcpy
  MOV.Q    save_p, RQ22
  ADD      #72, RQ22
  MOV.Q    @(RQ30, 24), RQ14
  MOV.Q    sectors, RQ21
  SHAD     RQ14, #7, RQ20
  ADD      RQ21, RQ20
  MOV.Q    RQ20, @(RQ30, 24)
  MOV.Q    @(RQ30, 24), RQ14
  MOV.Q    RQ30, @(RQ14, 104)
  MOV      RQ30, RQ19
  ADD      RQ19, #16, RQ18
  MOV.Q    @RQ18, RQ14
  MOV.Q    RQ22, save_p
  MOV.Q    RQ20, @(R15, 24)
  MOV.Q    RQ19, @(R15, 16)
  MOV.Q    RQ18, @(R15, 8)
  MOV      #0, RQ17
  CMPQ/EQ  RQ17, RQ14
  BT       L00801CC3
  MOV      T_PlatRaise, RQ16
  MOV      RQ16, RQ14
  MOV.Q    @(R15, 96), RQ17
  MOV      RQ17, RQ18
  ADD      RQ18, #16, RQ19
  MOV.Q    RQ14, @RQ19
  MOV.Q    RQ19, @(R15, 8)
  MOV.Q    RQ18, @(R15, 16)

L00801CC3:
  MOV.Q    @(R15, 96), RQ25
  MOV      RQ25, RQ14
  MOV      RQ14, RQ4
  BSR      P_AddThinker
  MOV      RQ25, RQ4
  BSR      P_AddActivePlat
  BRA      L00801CB7

L00801CBD:
  MOV.Q    save_p, RQ25
  MOV      #3, RQ26
  AND      RQ25, RQ26, RQ27
  MOV      #4, RQ28
  EXTS.L   RQ28, RQ28
  SUB      RQ28, RQ27, RQ25
  AND      RQ25, RQ26, RQ27
  MOV.Q    save_p, RQ29
  LEA.B    @(RQ29, RQ27), RQ29
  MOV      #0, RQ14
  MOV.Q    RQ29, save_p
  MOV.Q    RQ28, @(R15, 32)
  MOV.Q    RQ27, @(R15, 40)
  MOV.Q    RQ25, @(R15, 48)
  MOV      #56, RD4
  MOV      #50, RD5
  MOV      RQ14, RQ6
  BSR      Z_Malloc
  MOV      RQ2, RQ30
  MOV      #56, RQ25
  EXTS.L   RQ25, RQ25
  MOV.Q    RQ30, @(R15, 88)
  MOV.Q    RQ25, @(R15, 48)
  MOV      RQ30, RQ4
  MOV.Q    save_p, RQ5
  MOV      RQ25, RQ6
  BSR      memcpy
  MOV.Q    save_p, RQ31
  ADD      #56, RQ31
  MOV.Q    @(RQ30, 24), RQ14
  MOV.Q    sectors, RQ8
  SHAD     RQ14, #7, RQ9
  ADD      RQ8, RQ9
  MOV.Q    RQ9, @(RQ30, 24)
  MOV      T_LightFlash, RQ10
  MOV      RQ10, RQ14
  MOV      RQ30, RQ11
  ADD      RQ11, #16, RQ29
  MOV.Q    RQ14, @RQ29
  MOV      RQ30, RQ14
  MOV.Q    RQ11, @(R15, 16)
  MOV.Q    RQ9, @(R15, 24)
  MOV.Q    RQ31, save_p
  MOV.Q    RQ29, @(R15, 8)
  MOV      RQ14, RQ4
  BSR      P_AddThinker
  BRA      L00801CB7

L00801CBE:
  MOV.Q    save_p, RQ25
  MOV      #3, RQ26
  AND      RQ25, RQ26, RQ27
  MOV      #4, RQ28
  EXTS.L   RQ28, RQ28
  SUB      RQ28, RQ27, RQ25
  AND      RQ25, RQ26, RQ27
  MOV.Q    save_p, RQ29
  LEA.B    @(RQ29, RQ27), RQ29
  MOV      #0, RQ14
  MOV.Q    RQ29, save_p
  MOV.Q    RQ28, @(R15, 32)
  MOV.Q    RQ27, @(R15, 40)
  MOV.Q    RQ25, @(R15, 48)
  MOV      #56, RD4
  MOV      #50, RD5
  MOV      RQ14, RQ6
  BSR      Z_Malloc
  MOV      RQ2, RQ30
  MOV      #56, RQ25
  EXTS.L   RQ25, RQ25
  MOV.Q    RQ30, @(R15, 80)
  MOV.Q    RQ25, @(R15, 48)
  MOV      RQ30, RQ4
  MOV.Q    save_p, RQ5
  MOV      RQ25, RQ6
  BSR      memcpy
  MOV.Q    save_p, RQ31
  ADD      #56, RQ31
  MOV.Q    @(RQ30, 24), RQ14
  MOV.Q    sectors, RQ8
  SHAD     RQ14, #7, RQ9
  ADD      RQ8, RQ9
  MOV.Q    RQ9, @(RQ30, 24)
  MOV      T_StrobeFlash, RQ10
  MOV      RQ10, RQ14
  MOV      RQ30, RQ11
  ADD      RQ11, #16, RQ29
  MOV.Q    RQ14, @RQ29
  MOV      RQ30, RQ14
  MOV.Q    RQ11, @(R15, 16)
  MOV.Q    RQ9, @(R15, 24)
  MOV.Q    RQ31, save_p
  MOV.Q    RQ29, @(R15, 8)
  MOV      RQ14, RQ4
  BSR      P_AddThinker
  BRA      L00801CB7

L00801CBF:
  MOV.Q    save_p, RQ25
  MOV      #3, RQ26
  AND      RQ25, RQ26, RQ27
  MOV      #4, RQ28
  EXTS.L   RQ28, RQ28
  SUB      RQ28, RQ27, RQ25
  AND      RQ25, RQ26, RQ27
  MOV.Q    save_p, RQ29
  LEA.B    @(RQ29, RQ27), RQ29
  MOV      #0, RQ14
  MOV.Q    RQ29, save_p
  MOV.Q    RQ28, @(R15, 32)
  MOV.Q    RQ27, @(R15, 40)
  MOV.Q    RQ25, @(R15, 48)
  MOV      #48, RD4
  MOV      #50, RD5
  MOV      RQ14, RQ6
  BSR      Z_Malloc
  MOV      RQ2, RQ30
  MOV      #48, RQ25
  EXTS.L   RQ25, RQ25
  MOV.Q    RQ30, @(R15, 72)
  MOV.Q    RQ25, @(R15, 48)
  MOV      RQ30, RQ4
  MOV.Q    save_p, RQ5
  MOV      RQ25, RQ6
  BSR      memcpy
  MOV.Q    save_p, RQ31
  ADD      #48, RQ31
  MOV.Q    @(RQ30, 24), RQ14
  MOV.Q    sectors, RQ8
  SHAD     RQ14, #7, RQ9
  ADD      RQ8, RQ9
  MOV.Q    RQ9, @(RQ30, 24)
  MOV      T_Glow, RQ10
  MOV      RQ10, RQ14
  MOV      RQ30, RQ11
  ADD      RQ11, #16, RQ29
  MOV.Q    RQ14, @RQ29
  MOV      RQ30, RQ14
  MOV.Q    RQ11, @(R15, 16)
  MOV.Q    RQ9, @(R15, 24)
  MOV.Q    RQ31, save_p
  MOV.Q    RQ29, @(R15, 8)
  MOV      RQ14, RQ4
  BSR      P_AddThinker
  BRA      L00801CB7

L00801CC0:
  MOV      L00C0088C, RQ4
  MOV      RD12, RD5
  BSR      I_Error

L00801CB7:
  BRA      L00801CB5

L00801CB6:

L00E005D6:
  ADD      #168, R15
  BRA      L00C00626

P_Ticker:
  PUSH     LR
  BSR      L00C00630
  ADD      #-1672, R15
  MOV.L    paused, RD13
  CMP/EQ   RD13
  BT       L00801C61
  BRA      L00E005D7

L00801C61:
  MOV.L    netgame, RD12
  CMP/EQ   RD12
  BT       L00801C62
  MOV.L    menuactive, RD11
  CMP/EQ   RD11
  BT       L00801C62
  BRA      L00801C65

L00801C65:
  MOV.L    demoplayback, RD10
  CMP/EQ   RD10
  BT       L00801C62
  BRA      L00801C64

L00801C64:
  MOV      players, RQ9
  MOV.L    consoleplayer, RD8
  MOV      #320, RD7
  MUL.L    RD8, RD7
  STS      R0, RQ31
  ADD      RQ9, RQ31
  MOV.L    @(RQ31, 20), RD30
  MOV.L    RD30, @(R15, 36)
  CMP/EQ   #1, RD30
  BT       L00801C62
  BRA      L00801C63

L00801C63:
  BRA      L00E005D7

L00801C62:
  MOV      #0, RD14

L00801C67:
  MOV      #4, RD29
  CMP/GE   RD29, RD14
  BT       L00801C69
  MOV      playeringame, RQ28
  EXTS.L   RD14
  MOV.L    @(RQ28, RD14), RD27
  MOV.L    RD27, @(R15, 36)
  CMP/EQ   #0, RD27
  BT       L00801C6A
  MOV      players, RQ26
  EXTS.L   RD14
  MOV      #320, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ25
  ADD      RQ26, RQ25
  MOV.Q    RQ25, @(R15, 24)
  MOV      RQ25, RQ4
  BSR      P_PlayerThink

L00801C6A:

L00801C68:
  ADD      #1, RD14
  BRA      L00801C67

L00801C69:
  BSR      P_RunThinkers
  BSR      P_UpdateSpecials
  BSR      P_RespawnSpecials
  BSR      P_AcsTick
  MOV.L    leveltime, RD7
  ADD      #1, RD7
  MOV.L    RD7, leveltime

L00E005D7:
  ADD      #1672, R15
  BRA      L00C00626

P_InitThinkers:
  PUSH     R14
  PUSH     R13
  ADD      #-144, R15
  MOV      thinkercap, RQ13
  MOV      RQ13, RQ14
  MOV.Q    RQ14, @(RQ13, 8)
  MOV.Q    RQ14, @RQ13

L00E005D8:
  ADD      #144, R15
  POP      RQ13
  POP      RQ14
  RTS     

P_AddThinker:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  ADD      #-136, R15
  MOV.Q    RQ4, @(R15, 8)
  MOV      thinkercap, RQ13
  MOV.Q    @RQ13, RQ14
  MOV.Q    @(R15, 8), RQ12
  MOV.Q    RQ12, @(RQ14, 8)
  MOV      RQ13, RQ14
  MOV.Q    RQ14, @(RQ12, 8)
  MOV.Q    @RQ13, RQ14
  MOV.Q    RQ14, @RQ12
  MOV.Q    RQ12, @RQ13

L00E005D9:
  ADD      #136, R15
  BRA      L00C0079F

P_RemoveThinker:
  PUSH     R14
  PUSH     R13
  PUSH     R12
  ADD      #-152, R15
  MOV.Q    RQ4, @(R15, 24)
  MOV      #-1, RQ14
  MOV      #0x0000FFFF, RQ14
  MOV.Q    @(R15, 24), RQ13
  ADD      RQ13, #16, RQ12
  MOV.Q    RQ14, @RQ12

L00E005DA:
  ADD      #152, R15
  BRA      L00C0079F

P_SetupPsprites:
  PUSH     LR
  BSR      L00C0074E
  ADD      #-184, R15
  MOV.Q    RQ4, @(R15, 32)
  MOV      #0, RD14

L008017B2:
  MOV      #2, RD13
  CMP/GE   RD13, RD14
  BT       L008017B4
  MOV      #0, RQ12
  MOV.Q    @(R15, 32), RQ11
  ADD      RQ11, #264, RQ10
  EXTS.L   RD14
  MOV      #24, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ9
  ADD      RQ10, RQ9
  MOV.Q    RQ12, @RQ9

L008017B3:
  ADD      #1, RD14
  BRA      L008017B2

L008017B4:
  MOV.Q    @(R15, 32), RQ8
  MOV.L    @(RQ8, 116), RD31
  MOV.L    RD31, @(RQ8, 120)
  MOV      RQ8, RQ4
  BSR      P_BringUpWeapon

L00E005DB:
  ADD      #184, R15
  BRA      L00C00751

P_MovePsprites:
  PUSH     LR
  BSR      L00C00630
  ADD      #-200, R15
  MOV.Q    RQ4, @(R15, 48)
  MOV.Q    @(R15, 48), RQ12
  ADD      RQ12, #264, RQ11
  MOV      #0, RD10
  MOV      #24, RD7
  MUL.L    RD10, RD7
  STS      R0, RQ9
  ADD      RQ11, RQ9
  MOV      RQ9, RQ13
  MOV      #0, RD8
  MOV.Q    RQ11, @(R15, 24)
  MOV.L    RD8, @(R15, 44)

L008017B5:
  MOV.L    @(R15, 44), RD31
  MOV      #2, RD30
  CMP/GE   RD30, RD31
  BT       L008017B7
  MOV.Q    @RQ13, RQ29
  MOV      RQ29, RQ28
  MOV.Q    RQ29, @(R15, 24)
  MOV.Q    RQ28, @(R15, 32)
  MOV      #0, RQ27
  CMPQ/EQ  RQ27, RQ29
  BT       L008017B8
  MOV.L    @(RQ13, 8), RD14
  CMP/EQ   #-1, RD14
  BT       L008017B9
  MOV.L    @(RQ13, 8), RD14
  ADD      RD14, #-1, RD26
  MOV.L    RD26, @(RQ13, 8)
  MOV.L    @(RQ13, 8), RD14
  CMP/EQ   RD14
  BT       L008017BA
  MOV.Q    @RQ13, RQ25
  MOV.L    @(RQ25, 24), RD14
  MOV.Q    RQ25, @(R15, 24)
  MOV.Q    @(R15, 48), RQ4
  MOV.L    @(R15, 44), RD5
  MOV      RD14, RD6
  BSR      P_SetPsprite

L008017BA:

L008017B9:

L008017B8:

L008017B6:
  MOV.L    @(R15, 44), RD7
  MOV      RD7, RD14
  ADD      #1, RD7
  MOV      RQ13, RQ6
  ADD      #24, RQ13
  MOV.L    RD7, @(R15, 44)
  MOV.Q    RQ6, @(R15, 24)
  BRA      L008017B5

L008017B7:
  MOV.Q    @(R15, 48), RQ5
  ADD      RQ5, #264, RQ4
  MOV      RQ4, RQ3
  MOV.L    @(RQ3, 12), RD14
  ADD      RQ5, #264, RQ4
  ADD      RQ4, #24, RQ3
  MOV.L    RD14, @(RQ3, 12)
  ADD      RQ5, #264, RQ4
  MOV      RQ4, RQ3
  MOV.L    @(RQ3, 16), RD14
  ADD      RQ5, #264, RQ4
  ADD      RQ4, #24, RQ3
  MOV.L    RD14, @(RQ3, 16)
  MOV.Q    RQ4, @(R15, 24)

L00E005DC:
  ADD      #200, R15
  BRA      L00C00626

P_DropWeapon:
  PUSH     LR
  BSR      L00C006A6
  ADD      #-152, R15
  MOV.Q    RQ4, @(R15, 16)
  MOV.Q    @(R15, 16), RQ13
  MOV.L    @(RQ13, 116), RD14
  MOV      weaponinfo, RQ12
  MOV      #24, RD7
  MUL.L    RD14, RD7
  STS      R0, RQ11
  ADD      RQ12, RQ11
  MOV.L    @(RQ11, 8), RD14
  MOV      RQ13, RQ4
  MOV      #0, RD5
  MOV      RD14, RD6
  BSR      P_SetPsprite

L00E005DD:
  ADD      #152, R15
  BRA      L00C006AB

P_PlayerThink:
  PUSH     LR
  BSR      L00C00630
  ADD      #-88, R15
  MOV      RQ4, RQ12
  MOV.L    @(RQ12, 200), RD14
  TST      #1, RD14
  BT       L00801CEF
  MOV.Q    @RQ12, RQ13
  MOV.L    @(RQ13, 160), RD14
  OR       RD14, #4096, RD11
  MOV.Q    @RQ12, RQ13
  MOV.L    RD11, @(RQ13, 160)
  BRA      L00801CF0

L00801CEF:
  MOV.Q    @RQ12, RQ13
  MOV.L    @(RQ13, 160), RD14
  AND      RD14, #-4097, RD11
  MOV.Q    @RQ12, RQ13
  MOV.L    RD11, @(RQ13, 160)

L00801CF0:
  ADD      RQ12, #12, RQ13
  MOV      RQ13, RQ9
  MOV.Q    @RQ12, RQ13
  MOV.L    @(RQ13, 160), RD14
  TST      #128, RD14
  BT       L00801CF1
  MOV      #0, RD8
  MOV.W    RD8, @(RQ9, 2)
  MOV      #100, RD31
  MOV.B    RD31, @RQ9
  MOV.B    RD8, @(RQ9, 1)
  MOV.Q    @RQ12, RQ13
  MOV.L    @(RQ13, 160), RD14
  AND      RD14, #-129, RD11
  MOV.Q    @RQ12, RQ13
  MOV.L    RD11, @(RQ13, 160)

L00801CF1:
  MOV.L    @(RQ12, 8), RD14
  CMP/EQ   #1, RD14
  BT       L00801CF2
  MOV      RQ12, RQ4
  BSR      P_DeathThink
  BRA      L00E005DE

L00801CF2:
  MOV.Q    @RQ12, RQ13
  MOV.L    @(RQ13, 184), RD14
  CMP/EQ   RD14
  BT       L00801CF3
  MOV.Q    @RQ12, RQ13
  MOV.L    @(RQ13, 184), RD14
  ADD      RD14, #-1, RD11
  MOV.Q    @RQ12, RQ13
  MOV.L    RD11, @(RQ13, 184)
  BRA      L00801CF4

L00801CF3:
  MOV      RQ12, RQ4
  BSR      P_MovePlayer

L00801CF4:
  MOV      RQ12, RQ4
  BSR      P_CalcHeight
  MOV.Q    @RQ12, RQ13
  MOV.Q    @(RQ13, 88), RQ30
  MOV.Q    @RQ30, RQ13
  MOV.W    @(RQ13, 14), RD14
  CMP/EQ   RD14
  BT       L00801CF5
  MOV      RQ12, RQ4
  BSR      P_PlayerInSpecialSector

L00801CF5:
  MOV.B    @(RQ9, 7), RD14
  EXTU.B   RD14, RD14
  TST      #128, RD14
  BT       L00801CF6
  MOV      #0, RD29
  EXTU.B   RD29, RD29
  MOV.B    RD29, @(RQ9, 7)

L00801CF6:
  MOV.B    @(RQ9, 7), RD14
  EXTU.B   RD14, RD14
  TST      #4, RD14
  BT       L00801CF7
  MOV.B    @(RQ9, 7), RD14
  EXTU.B   RD14, RD14
  AND      RD14, #56, RD11
  SHAD     RD11, #-3, RD10
  CMP/EQ   RD10
  BT       L00801CF8
  ADD      RQ12, #124, RQ13
  MOV.L    @(RQ13, 28), RD14
  CMP/EQ   RD14
  BT       L00801CF8
  BRA      L00801CFA

L00801CFA:
  MOV.L    @(RQ12, 116), RD14
  CMP/EQ   #7, RD14
  BT       L00801CFC
  ADD      RQ12, #48, RQ13
  MOV.L    @(RQ13, 4), RD14
  CMP/EQ   RD14
  BT       L00801CFC
  BRA      L00801CF8

L00801CFC:
  BRA      L00801CF9

L00801CF9:
  MOV      #7, RD10

L00801CF8:
  MOV.L    gamemode, RD28
  CMP/EQ   #2, RD28
  BT       L00801CFE
  CMP/EQ   #2, RD10
  BT       L00801CFE
  BRA      L00801D01

L00801D01:
  ADD      RQ12, #124, RQ13
  MOV.L    @(RQ13, 32), RD14
  CMP/EQ   RD14
  BT       L00801CFE
  BRA      L00801D00

L00801D00:
  MOV.L    @(RQ12, 116), RD14
  CMP/EQ   #8, RD14
  BT       L00801CFE
  BRA      L00801CFF

L00801CFF:
  MOV      #8, RD10

L00801CFE:
  ADD      RQ12, #124, RQ13
  EXTS.L   RD10
  MOV.L    @(RQ13, RD10), RD14
  CMP/EQ   RD14
  BT       L00801D03
  MOV.L    @(RQ12, 116), RD14
  CMP/EQ   RD14, RD10
  BT       L00801D03
  BRA      L00801D04

L00801D04:
  CMP/EQ   #5, RD10
  BT       L00801D08
  CMP/EQ   #6, RD10
  BT       L00801D08
  BRA      L00801D07

L00801D08:
  MOV.L    gamemode, RD27
  CMP/EQ   #0, RD27
  BT       L00801D07
  BRA      L00801D06

L00801D07:
  MOV.L    RD10, @(RQ12, 120)

L00801D06:

L00801D03:

L00801CF7:
  MOV.B    @(RQ9, 7), RD14
  EXTU.B   RD14, RD14
  TST      #2, RD14
  BT       L00801D0A
  MOV.L    @(RQ12, 196), RD14
  CMP/EQ   RD14
  BT       L00801D0B
  MOV      RQ12, RQ4
  BSR      P_UseLines
  MOV      #1, RD26
  MOV.L    RD26, @(RQ12, 196)

L00801D0B:
  BRA      L00801D0C

L00801D0A:
  MOV      #0, RD25
  MOV.L    RD25, @(RQ12, 196)

L00801D0C:
  MOV      RQ12, RQ4
  BSR      P_MovePsprites
  ADD      RQ12, #48, RQ13
  MOV.L    @(RQ13, 4), RD14
  CMP/EQ   RD14
  BT       L00801D0D
  ADD      RQ12, #48, RQ13
  MOV.L    @(RQ13, 4), RD14
  ADD      RD14, #1, RD11
  ADD      RQ12, #48, RQ13
  MOV.L    RD11, @(RQ13, 4)

L00801D0D:
  ADD      RQ12, #48, RQ13
  MOV.L    @RQ13, RD14
  CMP/EQ   RD14
  BT       L00801D0E
  ADD      RQ12, #48, RQ13
  MOV.L    @RQ13, RD14
  ADD      RD14, #-1, RD11
  ADD      RQ12, #48, RQ13
  MOV.L    RD11, @RQ13

L00801D0E:
  ADD      RQ12, #48, RQ13
  MOV.L    @(RQ13, 8), RD14
  CMP/EQ   RD14
  BT       L00801D0F
  ADD      RQ12, #48, RQ13
  MOV.L    @(RQ13, 8), RD14
  ADD      RD14, #-1, RD11
  ADD      RQ12, #48, RQ13
  MOV.L    RD11, @(RQ13, 8)
  CMP/EQ   RD11
  BT       L00801D10
  MOV.Q    @RQ12, RQ13
  MOV.L    @(RQ13, 160), RD14
  AND      RD14, #-262145, RD11
  MOV.Q    @RQ12, RQ13
  MOV.L    RD11, @(RQ13, 160)

L00801D10:

L00801D0F:
  ADD      RQ12, #48, RQ13
  MOV.L    @(RQ13, 20), RD14
  CMP/EQ   RD14
  BT       L00801D11
  ADD      RQ12, #48, RQ13
  MOV.L    @(RQ13, 20), RD14
  ADD      RD14, #-1, RD11
  ADD      RQ12, #48, RQ13
  MOV.L    RD11, @(RQ13, 20)

L00801D11:
  ADD      RQ12, #48, RQ13
  MOV.L    @(RQ13, 12), RD14
  CMP/EQ   RD14
  BT       L00801D12
  ADD      RQ12, #48, RQ13
  MOV.L    @(RQ13, 12), RD14
  ADD      RD14, #-1, RD11
  ADD      RQ12, #48, RQ13
  MOV.L    RD11, @(RQ13, 12)

L00801D12:
  MOV.L    @(RQ12, 232), RD14
  CMP/EQ   RD14
  BT       L00801D13
  MOV.L    @(RQ12, 232), RD14
  ADD      RD14, #-1, RD11
  MOV.L    RD11, @(RQ12, 232)

L00801D13:
  MOV.L    @(RQ12, 236), RD14
  CMP/EQ   RD14
  BT       L00801D14
  MOV.L    @(RQ12, 236), RD14
  ADD      RD14, #-1, RD11
  MOV.L    RD11, @(RQ12, 236)

L00801D14:
  ADD      RQ12, #48, RQ13
  MOV.L    @RQ13, RD14
  CMP/EQ   RD14
  BT       L00801D15
  ADD      RQ12, #48, RQ13
  MOV.L    @RQ13, RD14
  CMP/GT   #128, RD14
  BT       L00801D17
  ADD      RQ12, #48, RQ13
  MOV.L    @RQ13, RD14
  TST      #8, RD14
  BT       L00801D17
  BRA      L00801D16

L00801D17:
  MOV      #32, RD7
  MOV.L    RD7, @(RQ12, 252)
  BRA      L00801D19

L00801D16:
  MOV      #0, RD6
  MOV.L    RD6, @(RQ12, 252)

L00801D19:
  BRA      L00801D1A

L00801D15:
  ADD      RQ12, #48, RQ13
  MOV.L    @(RQ13, 20), RD14
  CMP/EQ   RD14
  BT       L00801D1B
  TST      #8, RD14