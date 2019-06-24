# time gcc -o jx2vm.elf jx2vm.c -g -O2 -lm -DUSE_SDL -lSDL -mfpu=neon -mcpu=Cortex-A8 --warn-no-uninitialized
time clang -o jx2vm.elf jx2vm.c -g -O2 -lm -DUSE_SDL -lSDL -mfpu=neon -mcpu=Cortex-A8
