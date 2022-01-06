#ifndef STD_SETJMP_H
#define STD_SETJMP_H

typedef struct {
    long regs[128];
    int retval;
} jmp_buf[1];

int __setj(jmp_buf env);
#define setjmp(x) (__setj(x))
void longjmp(jmp_buf env, int val);

#endif
