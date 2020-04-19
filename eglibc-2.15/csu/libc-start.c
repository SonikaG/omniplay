/* Copyright (C) 1998-2006, 2007, 2009 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA.  */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ldsodefs.h>
#include <bp-start.h>
#include <bp-sym.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

//function to replace strlen, got from https://stackoverflow.com/questions/22520413/c-strlen-implementation-in-one-line-of-code
int my_strlen(const char *str)
{
    if(str == NULL){
	return 0;
    }
    int len;
    for (len = 0; str[len]; (len)++);
}

//function to copy over part of one buffer into another
void copy (char *buf, char *input, int offset, int length){
    int i;
    for(i = 0; i < length; i++){
      buf[offset + i] = input[i];
    }
}
//swap, reverse and itoa from: https://www.techiedelight.com/implement-itoa-function-in-c/
// function to swap two numbers
void swap(char *x, char *y) {
    char t = *x; *x = *y; *y = t;
}

// function to reverse buffer[i..j]
char* reverse(char *buffer, int i, int j)
{
    while (i < j)
    swap(&buffer[i++], &buffer[j--]);

    return buffer;
}

// Iterative function to implement itoa() function in C
int my_itoa(int value, char* buffer, int base)
{
    // invalid input
    if (base < 2 || base > 32)
    return my_strlen(buffer);

    // consider absolute value of number
    int n = abs(value);

    int i = 0;
    while (n)
    {
	int r = n % base;

	if (r >= 10)
	buffer[i++] = 65 + (r - 10);
	else
	buffer[i++] = 48 + r;

	n = n / base;
    }

    // if number is 0
    if (i == 0)
    buffer[i++] = '0';

    // If base is 10 and value is negative, the resulting string
    // is preceded with a minus sign (-)
    // With any other base, value is always considered unsigned
    if (value < 0 && base == 10)
	buffer[i++] = '-';

    buffer[i] = '\0'; // null terminate string

   // reverse the string and return it
   reverse(buffer, 0, i - 1);
   return i;
}

//modified from https://stackoverflow.com/questions/16647278/minimal-implementation-of-sprintf-or-printf
int my_sprintf(char *buf, char const *fmt, va_list arg) {

    int int_temp;
    char char_temp;
    char *string_temp;
    double double_temp;

    char ch;
    int length = 0;

    char buffer[512];
    while ( ch = *fmt++) {
        if ( '%' == ch ) {
            switch (ch = *fmt++) {

                // %c: print out a character    
                case 'c':
                    char_temp = va_arg(arg, int);
		    copy(buf, &char_temp, length, 1);
                    length++;
                    break;

                // %s: print out a string       
                case 's':
                    string_temp = va_arg(arg, char *);
		    copy(buf, string_temp, length, my_strlen(string_temp));
                    length += my_strlen(string_temp);
                    break;

                // %d: print out an int         
                case 'd':
                    int_temp = va_arg(arg, int);
                    int size = my_itoa(int_temp, buffer, 10);
		    copy(buf, buffer, length, my_strlen(buffer));
                    length += my_strlen(buffer);
                    break;
            }
        }
        else {
	    copy(buf, &ch, length, 1);
            length++;
        }
    }
    buf[length] = '\0';
    return length;
}

int s_sprintf(char* buf, char const *fmt, ...){
  va_list arg;
  int length;
  va_start(arg, fmt);
  length=my_sprintf(buf, fmt, arg);
  va_end(arg);
  return length;
}

extern void __libc_init_first (int argc, char **argv, char **envp);
#ifndef SHARED
extern void __libc_csu_irel (void);
#endif

extern int __libc_multiple_libcs;

#include <tls.h>
#ifndef SHARED
# include <dl-osinfo.h>
extern void __pthread_initialize_minimal (void);
# ifndef THREAD_SET_STACK_GUARD
/* Only exported for architectures that don't store the stack guard canary
   in thread local area.  */
uintptr_t __stack_chk_guard attribute_relro;
# endif
#endif

#ifdef HAVE_PTR_NTHREADS
/* We need atomic operations.  */
# include <atomic.h>
#endif


#ifdef LIBC_START_MAIN
# ifdef LIBC_START_DISABLE_INLINE
#  define STATIC static
# else
#  define STATIC static inline __attribute__ ((always_inline))
# endif
#else
# define STATIC
# define LIBC_START_MAIN BP_SYM (__libc_start_main)
#endif

#ifdef MAIN_AUXVEC_ARG
/* main gets passed a pointer to the auxiliary.  */
# define MAIN_AUXVEC_DECL	, void *
# define MAIN_AUXVEC_PARAM	, auxvec
#else
# define MAIN_AUXVEC_DECL
# define MAIN_AUXVEC_PARAM
#endif

STATIC int LIBC_START_MAIN (int (*main) (int, char **, char **
					 MAIN_AUXVEC_DECL),
			    int argc,
			    char *__unbounded *__unbounded ubp_av,
#ifdef LIBC_START_MAIN_AUXVEC_ARG
			    ElfW(auxv_t) *__unbounded auxvec,
#endif
			    __typeof (main) init,
			    void (*fini) (void),
			    void (*rtld_fini) (void),
			    void *__unbounded stack_end)
     __attribute__ ((noreturn));


/* Note: the fini parameter is ignored here for shared library.  It
   is registered with __cxa_atexit.  This had the disadvantage that
   finalizers were called in more than one place.  */
STATIC int
LIBC_START_MAIN (int (*main) (int, char **, char ** MAIN_AUXVEC_DECL),
		 int argc, char *__unbounded *__unbounded ubp_av,
#ifdef LIBC_START_MAIN_AUXVEC_ARG
		 ElfW(auxv_t) *__unbounded auxvec,
#endif
		 __typeof (main) init,
		 void (*fini) (void),
		 void (*rtld_fini) (void), void *__unbounded stack_end)
{
#if __BOUNDED_POINTERS__
  char **argv;
#else
# define argv ubp_av
#endif

  /* Result of the 'main' function.  */
  int result;

  __libc_multiple_libcs = &_dl_starting_up && !_dl_starting_up;

#ifndef SHARED
  char *__unbounded *__unbounded ubp_ev = &ubp_av[argc + 1];

  INIT_ARGV_and_ENVIRON;

  /* Store the lowest stack address.  This is done in ld.so if this is
     the code for the DSO.  */
  __libc_stack_end = stack_end;

# ifdef HAVE_AUX_VECTOR
  /* First process the auxiliary vector since we need to find the
     program header to locate an eventually present PT_TLS entry.  */
#  ifndef LIBC_START_MAIN_AUXVEC_ARG
  ElfW(auxv_t) *__unbounded auxvec;
//  int fd = open("/home/sonika/Documents/omniplay/test/output.txt", O_WRONLY);
//  char buf[200];
//  s_sprintf(buf, "This is auxvec %s!\n", auxvec);
/*  s_sprintf(buf, "This is auxvec!\n"); 
  if(buf != NULL){
    write(fd, buf, my_strlen(buf)+1);
  }*/
  {
    char *__unbounded *__unbounded evp = ubp_ev;
    while (*evp++ != NULL)
      ;
    auxvec = (ElfW(auxv_t) *__unbounded) evp;
  }
//  s_sprintf(buf, "This is evp %s!\n", ubp_ev);
/*  s_sprintf(buf, "This is auxvec!\n");
  if(buf != NULL){
    write(fd, buf, my_strlen(buf)+1);
  }*/
#  endif
  _dl_aux_init (auxvec);
# endif
# ifdef DL_SYSDEP_OSCHECK
  if (!__libc_multiple_libcs)
    {
      /* This needs to run to initiliaze _dl_osversion before TLS
	 setup might check it.  */
      DL_SYSDEP_OSCHECK (__libc_fatal);
    }
# endif

  /* Performe IREL{,A} relocations.  */
  __libc_csu_irel ();

  /* Initialize the thread library at least a bit since the libgcc
     functions are using thread functions if these are available and
     we need to setup errno.  */
  __pthread_initialize_minimal ();

  /* Set up the stack checker's canary.  */
  uintptr_t stack_chk_guard = _dl_setup_stack_chk_guard (_dl_random);
# ifdef THREAD_SET_STACK_GUARD
  THREAD_SET_STACK_GUARD (stack_chk_guard);
# else
  __stack_chk_guard = stack_chk_guard;
# endif
#endif

  /* Register the destructor of the dynamic linker if there is any.  */
  if (__builtin_expect (rtld_fini != NULL, 1))
    __cxa_atexit ((void (*) (void *)) rtld_fini, NULL, NULL);

#ifndef SHARED
  /* Call the initializer of the libc.  This is only needed here if we
     are compiling for the static library in which case we haven't
     run the constructors in `_dl_start_user'.  */
  __libc_init_first (argc, argv, __environ);

  /* Register the destructor of the program, if any.  */
  if (fini)
    __cxa_atexit ((void (*) (void *)) fini, NULL, NULL);

  /* Some security at this point.  Prevent starting a SUID binary where
     the standard file descriptors are not opened.  We have to do this
     only for statically linked applications since otherwise the dynamic
     loader did the work already.  */
  if (__builtin_expect (__libc_enable_secure, 0))
    __libc_check_standard_fds ();
#endif

  /* Call the initializer of the program, if any.  */
#ifdef SHARED
  if (__builtin_expect (GLRO_dl_debug_mask & DL_DEBUG_IMPCALLS, 0))
    GLRO(dl_debug_printf) ("\ninitialize program: %s\n\n", argv[0]);
#endif
  if (init)
    (*init) (argc, argv, __environ MAIN_AUXVEC_PARAM);

#ifdef SHARED
  /* Auditing checkpoint: we have a new object.  */
  if (__builtin_expect (GLRO(dl_naudit) > 0, 0))
    {
      struct audit_ifaces *afct = GLRO(dl_audit);
      struct link_map *head = GL(dl_ns)[LM_ID_BASE]._ns_loaded;
      for (unsigned int cnt = 0; cnt < GLRO(dl_naudit); ++cnt)
	{
	  if (afct->preinit != NULL)
	    afct->preinit (&head->l_audit[cnt].cookie);

	  afct = afct->next;
	}
    }
#endif

#ifdef SHARED
  if (__builtin_expect (GLRO_dl_debug_mask & DL_DEBUG_IMPCALLS, 0))
    GLRO(dl_debug_printf) ("\ntransferring control: %s\n\n", argv[0]);
#endif

#ifdef HAVE_CLEANUP_JMP_BUF
  /* Memory for the cancellation buffer.  */
  struct pthread_unwind_buf unwind_buf;

  int not_first_call;
  not_first_call = setjmp ((struct __jmp_buf_tag *) unwind_buf.cancel_jmp_buf);
  if (__builtin_expect (! not_first_call, 1))
    {
      struct pthread *self = THREAD_SELF;

      /* Store old info.  */
      unwind_buf.priv.data.prev = THREAD_GETMEM (self, cleanup_jmp_buf);
      unwind_buf.priv.data.cleanup = THREAD_GETMEM (self, cleanup);

      /* Store the new cleanup handler info.  */
      THREAD_SETMEM (self, cleanup_jmp_buf, &unwind_buf);

      /* Run the program.  */
      result = main (argc, argv, __environ MAIN_AUXVEC_PARAM);
    }
  else
    {
      /* Remove the thread-local data.  */
# ifdef SHARED
      PTHFCT_CALL (ptr__nptl_deallocate_tsd, ());
# else
      extern void __nptl_deallocate_tsd (void) __attribute ((weak));
      __nptl_deallocate_tsd ();
# endif

      /* One less thread.  Decrement the counter.  If it is zero we
	 terminate the entire process.  */
      result = 0;
# ifdef SHARED
      unsigned int *ptr = __libc_pthread_functions.ptr_nthreads;
      PTR_DEMANGLE (ptr);
# else
      extern unsigned int __nptl_nthreads __attribute ((weak));
      unsigned int *const ptr = &__nptl_nthreads;
# endif

      if (! atomic_decrement_and_test (ptr))
	/* Not much left to do but to exit the thread, not the process.  */
	__exit_thread (0);
    }
#else
  /* Nothing fancy, just call the function.  */
  result = main (argc, argv, __environ MAIN_AUXVEC_PARAM);
#endif

  exit (result);
}
