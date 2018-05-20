typedef struct th_context_s thContext;

typedef struct BIPRO_RegisterState_s BIPRO_RegisterState;

struct th_context_s {
int (*fcn)(void *ptr);
void *ptr;
int id;

void *hdl;
void *stktop;

//void *tls[1024];
void **tls;
int sz_tls;

void **rootstack;
int rootstackpos, szrootstack;
int *rootframe;
int rootframepos, szrootframe;
};

struct BIPRO_RegisterState_s {
int dummy;
#ifdef X86
u32 eax, ecx, edx, ebx;
u32 esp, ebp, esi, edi;
u32 eip, eflags;
#endif
#ifdef X86_64
u64 rax, rcx, rdx, rbx;
u64 rsp, rbp, rsi, rdi;
u64 r8, r9, r10, r11;
u64 r12, r13, r14, r15;
u64 rip, rflags;
#endif
};
