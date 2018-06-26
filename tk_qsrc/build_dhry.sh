/opt/cross/bin/sh4-gcc -o dhry1.elf -DTIME -nostdinc -nostdlib -nostartfiles -fno-builtin -DARCH_SH4 -DARCH_SH4LE -Itk_clib/include -Ltk_clib -O3 -Wl,-nostdlib -Wl,-e,_start -Wl,-T,sh4elf.x tk_clib/crt0.o -lvxcore -Idhry dhry/dhry_1.c dhry/dhry_2.c

gcc -o dhry1n.elf -DTIME -Idhry dhry/dhry_1.c dhry/dhry_2.c -O3