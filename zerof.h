/*
 * Copyright (c) 2009 ... 2019 2020 2021
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

#ifndef REV_ZEROF_H

#define REV_ZEROF_H "$Id: zerof.h,v 2.6 2020/06/18 19:18:54 jmccue Exp $"

#define PROG_NAME "zerof"

typedef unsigned long counter;

struct s_file_info
  {
    FILE *fp;
    char *fname;
  } ;

struct s_work_area
  {
    struct s_file_info out;         /* default stdout        */
    struct s_file_info err;         /* default stderr        */
    int verbose;                    /* TRUE or FALSE      */
    int use_urand;                  /* TRUE or FALSE      */
    int use_random;                 /* TRUE or FALSE      */
    int overwrite;                  /* overrite character */
    int force;                      /* TRUE or FALSE */
    int iterations;                 /* passes        */
  } ;

#define CKARG_SIZE 256
#define USG_MSG_ZERO_1 "\toverwrite all data in a file with a NULL character\n"
#define USG_MSG_ZERO_2 "\tor a specific character.\n"

#endif /* END: REV_ZEROF_H */

/* END: zerof.h */
