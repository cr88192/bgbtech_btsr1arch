  /* Don't change the order of the following elements.  'dl_loaded'
     must remain the first element.  Forever.  */
/* Non-shared code has no support for multiple namespaces.  */

#define SHARED
// #define PTHREAD_IN_LIBC

#ifdef SHARED
# define DL_NNS 16
#else
# define DL_NNS 1
#endif

struct link_namespaces
{
    struct link_map *_ns_loaded;
    unsigned int _ns_nloaded;
    struct r_scope_elem *_ns_main_searchlist;
    unsigned int _ns_global_scope_alloc;
    unsigned int _ns_global_scope_pending_adds;
    struct link_map *libc_map;
    struct unique_sym_table
    {
      __rtld_lock_define_recursive (, lock)
      struct unique_sym
      {
	uint32_t hashval;
	const char *name;
	const ElfW(Sym) *sym;
	const struct link_map *map;
      } *entries;
      size_t size;
      size_t n_elements;
      void (*free) (void *);
    } _ns_unique_sym_table;
    /* Keep track of changes to each namespace' list.  */
    struct r_debug_extended _ns_debug;
};

struct dtv_slotinfo_list
{
    size_t len;
    struct dtv_slotinfo_list *next;
    struct dtv_slotinfo
    {
      size_t gen;
      struct link_map *map;
    } slotinfo[];
};

struct rtld_global
{
	struct link_namespaces _dl_ns[DL_NNS];

  size_t _dl_nns;
  __rtld_lock_define_recursive (EXTERN, _dl_load_lock)
  __rtld_lock_define_recursive (EXTERN, _dl_load_write_lock)
  __rtld_lock_define_recursive (EXTERN, _dl_load_tls_lock)
  unsigned long long _dl_load_adds;
  struct link_map *_dl_initfirst;
  struct link_map *_dl_profile_map;
  unsigned long int _dl_num_relocations;
  unsigned long int _dl_num_cache_relocations;
  struct r_search_path_elem *_dl_all_dirs;
  struct link_map _dl_rtld_map;
#ifdef SHARED
  struct auditstate _dl_rtld_auditstate[DL_NNS];
#endif
#if !PTHREAD_IN_LIBC && defined SHARED \
    && defined __rtld_lock_default_lock_recursive
  void (*_dl_rtld_lock_recursive) (void *);
  void (*_dl_rtld_unlock_recursive) (void *);
#endif

#include <dl-procruntime.c>

#if !PTHREAD_IN_LIBC
  int (*_dl_make_stack_executable_hook) (void **);
#endif
  ElfW(Word) _dl_stack_flags;
  bool _dl_tls_dtv_gaps;
  size_t _dl_tls_max_dtv_idx;
  struct dtv_slotinfo_list  *_dl_tls_dtv_slotinfo_list;
  size_t _dl_tls_static_nelem;
  size_t _dl_tls_static_used;
  size_t _dl_tls_static_optional;
#define TLS_SLOTINFO_SURPLUS (62)
#define DTV_SURPLUS	(14)
  void *_dl_initial_dtv;
  size_t _dl_tls_generation;
#if !PTHREAD_IN_LIBC
  void (*_dl_init_static_tls) (struct link_map *);
#endif
  /* Scopes to free after next THREAD_GSCOPE_WAIT ().  */
  struct dl_scope_free_list
  {
    size_t count;
    void *list[50];
  } *_dl_scope_free_list;
#if PTHREAD_IN_LIBC
  /* List of active thread stacks, with memory managed by glibc.  */
  list_t _dl_stack_used;
  /* List of thread stacks that were allocated by the application.  */
  list_t _dl_stack_user;
  /* List of queued thread stacks.  */
  list_t _dl_stack_cache;
  /* Total size of all stacks in the cache (sum over stackblock_size).  */
  size_t _dl_stack_cache_actsize;
  /* We need to record what list operations we are going to do so
     that, in case of an asynchronous interruption due to a fork()
     call, we can correct for the work.  */
  uintptr_t _dl_in_flight_stack;
  /* Mutex protecting the stack lists.  */
  int _dl_stack_cache_lock;
#else
  /* The total number of thread IDs currently in use, or on the list of
     available thread IDs.  */
  int _dl_pthread_num_threads;
  /* Array of __pthread structures and its lock.  */
  struct __pthread **_dl_pthread_threads;
  __libc_rwlock_define (EXTERN, _dl_pthread_threads_lock)
#endif
#ifdef SHARED
};

# define __rtld_global_attribute__
# if IS_IN (rtld)
#  ifdef HAVE_SDATA_SECTION
#   define __rtld_local_attribute__ \
	    __attribute__ ((visibility ("hidden"), section (".sdata")))
#   undef __rtld_global_attribute__
#   define __rtld_global_attribute__ __attribute__ ((section (".sdata")))
#  else
#   define __rtld_local_attribute__ __attribute__ ((visibility ("hidden")))
#  endif
extern struct rtld_global _rtld_local __rtld_local_attribute__;
#  undef __rtld_local_attribute__
# endif
extern struct rtld_global _rtld_global __rtld_global_attribute__;
# undef __rtld_global_attribute__
#endif
#ifndef SHARED
# define GLRO(name) _##name
#else
# if IS_IN (rtld)
#  define GLRO(name) _rtld_local_ro._##name
# else
#  define GLRO(name) _rtld_global_ro._##name
# endif

struct rtld_global_ro
{
#endif
  /* If nonzero the appropriate debug information is printed.  */
  int _dl_debug_mask;
#define DL_DEBUG_LIBS	    (1 << 0)
#define DL_DEBUG_IMPCALLS   (1 << 1)
#define DL_DEBUG_BINDINGS   (1 << 2)
#define DL_DEBUG_SYMBOLS    (1 << 3)
#define DL_DEBUG_VERSIONS   (1 << 4)
#define DL_DEBUG_RELOC      (1 << 5)
#define DL_DEBUG_FILES      (1 << 6)
#define DL_DEBUG_STATISTICS (1 << 7)
#define DL_DEBUG_UNUSED	    (1 << 8)
#define DL_DEBUG_SCOPES	    (1 << 9)
/* These two are used only internally.  */
#define DL_DEBUG_HELP       (1 << 10)
  /* Platform name.  */
  const char *_dl_platform;
  size_t _dl_platformlen;
  /* Cached value of `getpagesize ()'.  */
  size_t _dl_pagesize;
  /* Cached value of `sysconf (_SC_MINSIGSTKSZ)'.  */
  size_t _dl_minsigstacksize;
  /* Do we read from ld.so.cache?  */
  int _dl_inhibit_cache;
  /* Copy of the content of `_dl_main_searchlist' at startup time.  */
  struct r_scope_elem _dl_initial_searchlist;
  /* CLK_TCK as reported by the kernel.  */
  int _dl_clktck;
  /* If nonzero print warnings messages.  */
  int _dl_verbose;
  /* File descriptor to write debug messages to.  */
  int _dl_debug_fd;
  /* Do we do lazy relocations?  */
  int _dl_lazy;
  /* Nonzero if runtime lookups should not update the .got/.plt.  */
  int _dl_bind_not;
  /* Nonzero if references should be treated as weak during runtime
     linking.  */
  int _dl_dynamic_weak;
  /* Default floating-point control word.  */
  fpu_control_t _dl_fpu_control;
  /* Mask for hardware capabilities that are available.  */
  uint64_t _dl_hwcap;
#ifdef HAVE_AUX_VECTOR
  /* Pointer to the auxv list supplied to the program at startup.  */
  ElfW(auxv_t) *_dl_auxv;
#endif
  /* Get architecture specific definitions.  */
#include <dl-procinfo.c>
  /* Names of shared object for which the RPATH should be ignored.  */
  const char *_dl_inhibit_rpath;
  /* Location of the binary.  */
  const char *_dl_origin_path;
  /* Size of the static TLS block.  */
  size_t _dl_tls_static_size;
  /* Alignment requirement of the static TLS block.  */
  size_t _dl_tls_static_align;
  /* Size of surplus space in the static TLS area for dynamically
     loaded modules with IE-model TLS or for TLSDESC optimization.
     See comments in elf/dl-tls.c where it is initialized.  */
  size_t _dl_tls_static_surplus;
  /* Name of the shared object to be profiled (if any).  */
  const char *_dl_profile;
  /* Filename of the output file.  */
  const char *_dl_profile_output;
  /* All search directories defined at startup.  This is assigned a
     non-NULL pointer by the ld.so startup code (after initialization
     to NULL), so this can also serve as an indicator whether a copy
     of ld.so is initialized and active.  See the rtld_active function
     below.  */
  struct r_search_path_elem *_dl_init_all_dirs;
#ifdef NEED_DL_SYSINFO
  /* Syscall handling improvements.  This is very specific to x86.  */
  uintptr_t _dl_sysinfo;
#endif
#ifdef NEED_DL_SYSINFO_DSO
  /* The vsyscall page is a virtual DSO pre-mapped by the kernel.
     This points to its ELF header.  */
  const ElfW(Ehdr) *_dl_sysinfo_dso;
  /* At startup time we set up the normal DSO data structure for it,
     and this points to it.  */
  struct link_map *_dl_sysinfo_map;
# define PROCINFO_DECL
# ifndef PROCINFO_CLASS
#  define PROCINFO_CLASS EXTERN
# endif
# include <dl-vdso-setup.c>
#endif
  /* Mask for more hardware capabilities that are available on some
     platforms.  */
  uint64_t _dl_hwcap2;
  uint64_t _dl_hwcap3;
  uint64_t _dl_hwcap4;
  enum dso_sort_algorithm _dl_dso_sort_algo;
#ifdef SHARED
  /* We add a function table to _rtld_global which is then used to
     call the function instead of going through the PLT.  The result
     is that we can avoid exporting the functions and we do not jump
     PLT relocations in libc.so.  */
  void (*_dl_debug_printf) (const char *, ...)
       __attribute__ ((__format__ (__printf__, 1, 2)));
  void (*_dl_mcount) (ElfW(Addr) frompc, ElfW(Addr) selfpc);
  lookup_t (*_dl_lookup_symbol_x) (const char *, struct link_map *,
				   const ElfW(Sym) **, struct r_scope_elem *[],
				   const struct r_found_version *, int, int,
				   struct link_map *);
  void *(*_dl_open) (const char *file, int mode, const void *caller_dlopen,
		     Lmid_t nsid, int argc, char *argv[], char *env[]);
  void (*_dl_close) (void *map);
  /* libdl in a secondary namespace (after dlopen) must use
     _dl_catch_error from the main namespace, so it has to be
     exported in some way.  */
  int (*_dl_catch_error) (const char **objname, const char **errstring,
			  bool *mallocedp, void (*operate) (void *),
			  void *args);
  /* libdl in a secondary namespace must use free from the base
     namespace.  */
  void (*_dl_error_free) (void *);
  void *(*_dl_tls_get_addr_soft) (struct link_map *);
  /* Called from __libc_shared to deallocate malloc'ed memory.  */
  void (*_dl_libc_freeres) (void);
  /* Implementation of _dl_find_object.  The public entry point is in
     libc, and this is patched by __rtld_static_init to support static
     dlopen.  */
  int (*_dl_find_object) (void *, struct dl_find_object *);
  /* Dynamic linker operations used after static dlopen.  */
  const struct dlfcn_hook *_dl_dlfcn_hook;
  /* List of auditing interfaces.  */
  struct audit_ifaces *_dl_audit;
  unsigned int _dl_naudit;
};
# define __rtld_global_attribute__
# if IS_IN (rtld)
#  define __rtld_local_attribute__ __attribute__ ((visibility ("hidden")))
extern struct rtld_global_ro _rtld_local_ro
    attribute_relro __rtld_local_attribute__;
extern struct rtld_global_ro _rtld_global_ro
    attribute_relro __rtld_global_attribute__;
#  undef __rtld_local_attribute__
# else
/* We cheat a bit here.  We declare the variable as as const even
   though it is at startup.  */
extern const struct rtld_global_ro _rtld_global_ro
    attribute_relro __rtld_global_attribute__;
# endif
# undef __rtld_global_attribute__
#endif
#undef EXTERN
#ifndef SHARED
/* dl-support.c defines these and initializes them early on.  */
extern const ElfW(Phdr) *_dl_phdr;
extern size_t _dl_phnum;
#endif
#if PTHREAD_IN_LIBC
/* This function changes the permissions of all stacks (not just those
   of the main stack).  */
int _dl_make_stacks_executable (void **stack_endp) attribute_hidden;
#else
/* This is the initial value of GL(dl_make_stack_executable_hook).
   A threads library can change it.  The ld.so implementation changes
   the permissions of the main stack only.  */
extern int _dl_make_stack_executable (void **stack_endp);
rtld_hidden_proto (_dl_make_stack_executable)
#endif
/* Variable pointing to the end of the stack (or close to it).  This value
   must be constant over the runtime of the application.  Some programs
   might use the variable which results in copy relocations on some
   platforms.  But this does not matter, ld.so can always use the local
   copy.  */
extern void *__libc_stack_end
#ifndef LIBC_STACK_END_NOT_RELRO
     attribute_relro
#endif
     ;
rtld_hidden_proto (__libc_stack_end)
/* Parameters passed to the dynamic linker.  */
extern int _dl_argc attribute_hidden attribute_relro;
extern char **_dl_argv attribute_relro;
rtld_hidden_proto (_dl_argv)
#define rtld_progname _dl_argv[0]
/* Flag set at startup and cleared when the last initializer has run.  */
extern int _dl_starting_up;
weak_extern (_dl_starting_up)
rtld_hidden_proto (_dl_starting_up)
/* Random data provided by the kernel.  */
extern void *_dl_random attribute_hidden attribute_relro;
/* Write message on the debug file descriptor.  The parameters are
   interpreted as for a `printf' call.  All the lines start with a
   tag showing the PID.  */
extern void _dl_debug_printf (const char *fmt, ...)
     __attribute__ ((__format__ (__printf__, 1, 2))) attribute_hidden;
/* Write message on the debug file descriptor.  The parameters are
   interpreted as for a `printf' call.  All the lines buf the first
   start with a tag showing the PID.  */
