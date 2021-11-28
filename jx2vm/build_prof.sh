time gcc -o jx2vm.elf jx2vm.c -g -pg -lm -DUSE_SDL -lSDL -Wuninitialized
# time clang -o jx2vm.elf jx2vm.c -g -O2 -lm
