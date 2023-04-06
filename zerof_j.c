/*
 * Copyright (c) 1994 1995 1996 ... 2023 2024
 *     John McCue <jmccue@jmcunx.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

/*
 * zerof_j.c -- routines from lib j_lib2
 */

#ifndef _MSDOS
#include <sys/param.h>
#endif

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef _AIX
#define HAS_GETLINE 1
#endif
#ifdef __NetBSD_Version__
#include <sys/timeb.h>
#define HAS_GETLINE 1
#define BSD_TODAY 1
#endif
#ifdef OpenBSD
#include <sys/timeb.h>
#define HAS_GETLINE 1
#define BSD_TODAY 1
#endif
#ifdef __FreeBSD__
#include <sys/timeb.h>
#define HAS_GETLINE 1
#define BSD_TODAY 1
#endif
#ifdef linux
#define HAS_GETLINE 1
#endif

#include "zerof.h"

#define SIZE_GETLINE_BUF 256

/*
 * j2_f_exist() -- determines if a file exists
 */
int j2_f_exist(char *file_name)

{
  if (file_name == (char *) NULL)
    return((int) FALSE);

#ifdef _MSDOS
  if (access(file_name, 00) == -1)
    return (FALSE);
  else
    return (TRUE);
#else
  struct stat file_info;
  if (stat(file_name, &file_info) == 0)
    return (TRUE);
  else
    return (FALSE);
#endif

} /* j2_f_exist() */

/*
 * j2_clr_str() -- Clears a string with a char & ensure it ends with NULL
 */
long int j2_clr_str(char *s, char c, int size)
  
{
  if (s == (char *) NULL)
    return(0L);
  
  memset(s, (int) c, (size_t) size);
  s[size - 1 ] = JLIB2_CHAR_NULL;

  return((long int) strlen(s));

} /* j2_clr_str() */

/*
 * j2_file_typ() -- Determines File Type
 */
int j2_file_typ(char *file_name)

{

  struct stat file_stat;
  int file_type;

  if (stat(file_name, &file_stat))
    return(J2_CANT_GET);

  file_type = (int) (file_stat.st_mode & S_IFMT);

  switch (file_type)
    {
      case (int) S_IFREG:
	file_type = J2_REGULAR;
	break;
      case (int) S_IFDIR:
	      file_type = J2_DIRECTORY;
	      break;
#ifdef S_IFCHR
      case (int) S_IFCHR:
	      file_type = J2_CHAR_SPECIAL;
	      break;
#endif
#ifdef S_IFBLK
      case (int) S_IFBLK:
	      file_type = J2_BLOCK_SPECIAL;
	      break;
#endif
#ifdef S_IFLNK
      case (int) S_IFLNK:
	      file_type = J2_SYMB_LINK;
	      break;
#endif
#ifdef S_IFPIP
      case (int) S_IFPIP:
	      file_type = J2_PIPE;
	      break;
#endif
#ifdef S_IFSOCK
#if S_IFSOCK != S_IFPIP
      case (int) S_IFSOCK:
	      file_type = J2_SOCKET;
	      break;
#endif
#endif
#ifdef S_IFIFO
#if S_IFIFO != S_IFPIP
      case (int) S_IFIFO:
	      file_type = J2_FIFO;
	      break;
#endif
#endif
      default:
	      file_type = J2_UNKNOWN;
    }

  return(file_type);

} /* j2_file_typ() */

/*
 * j2_is_numr() -- determines if all characters are numeric
 */
int j2_is_numr(char *s)

{
  if (s == (char *) NULL)
    return((int) FALSE); /* NULL pointer */

  for ( ; (*s) != JLIB2_CHAR_NULL; s++)
    {
      if ( ! isdigit((int)(*s)) )
	return(FALSE);
    }

  return(TRUE);

} /* j2_is_numr() */

/*
 * j2_today() -- get todays date
 */
void j2_today(struct s_j2_datetime *dt)

{

  struct tm     *str_tm;
  
#ifdef BSD_TODAY
  struct timeval tp;
  struct timezone tzp;
#else
#ifdef linux
  struct timeval tp;
  struct timezone tzp;
#else
  struct timeb  time_buf;
#endif
#endif

#ifdef BSD_TODAY
  gettimeofday(&tp, &tzp);
  str_tm = localtime(&(tp.tv_sec));
#else
#ifdef linux
  gettimeofday(&tp, &tzp);
  str_tm = localtime(&(tp.tv_sec));
#else
  ftime(&time_buf);
  str_tm = localtime(&(time_buf.time));
#endif
#endif
  
  (str_tm->tm_mon)++;
  
  dt->yyyy     = str_tm->tm_year + 1900;
  dt->month    = str_tm->tm_mon;
  dt->dd       = str_tm->tm_mday;
  dt->yy       = str_tm->tm_year;
  dt->hh       = str_tm->tm_hour;
  dt->minutes  = str_tm->tm_min;
  dt->ss       = str_tm->tm_sec;
  dt->tm_isdst = str_tm->tm_isdst;

#ifdef BSD_TODAY
  dt->mil      = (int) (tp.tv_usec / 1000);
#else
#ifdef linux
  dt->mil      = (int) (tp.tv_usec / 1000);
#else
  dt->mil      = time_buf.millitm;
#endif
#endif

} /* j2_today() */
