/* Copyright (C) 1991, 1995, 1996, 1997, 2001, 2004, 2006
   Free Software Foundation, Inc.
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

#include <libioP.h>
#include <stdarg.h>
#include <stdio.h>

#undef printf

/* Write formatted output to stdout from the format string FORMAT.  */
/* VARARGS1 */
extern void* analysis_flag_add = NULL;

void set_analysis_flag_add (void *adr){
  analysis_flag_add = adr;
  sonika_print("test 2\n");
}

int
__printf (const char *format, ...)
{
  va_list arg;
  int done;
//    sonika_print("TEST!\n");
//  if(analysis_flag_add == NULL){
    va_start (arg, format);
    done = vfprintf (stdout, format, arg);
    va_end (arg);
    sonika_print("TEST!\n");
//  }
  /*else if(!(*(int *)analysis_flag_add)){
    va_start (arg, format);
    done = vfprintf (stdout, format, arg);
    va_end (arg);
 //  sonika_print("test 3!\n");
  }
  else{ 
    done = sonika_print("test!\n");
  }*/
  return done;
}

#undef _IO_printf
ldbl_strong_alias (__printf, printf);
/* This is for libg++.  */
ldbl_strong_alias (__printf, _IO_printf);
