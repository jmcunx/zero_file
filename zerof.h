/*
 * Copyright (c) 2009 ... 2023 2024
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

#define REV_ZEROF_H "INCLUDED"

#define PROG_NAME "zerof"

#ifndef JLIB2_CHAR_NULL
#define NO_JLIB 1
#define JLIB2_INT_NULL   ((int) '\0')
#define JLIB2_CHAR_NULL  ((char) '\0')
#define JLIB2_UCHAR_NULL ((unsigned char) '\0' )
#endif

#ifndef NULL
#define NULL '\0'
#endif
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef SSIZE_T
#define SSIZE_T ssize_t
#endif
#ifndef TAB_CHAR
#define TAB_CHAR 0x09
#endif

#ifdef __FreeBSD_version
#define RNUM long int
#define RAND random
#define SRAND srandom
#endif
#ifdef __Linux__
#define RNUM long int
#define RAND random
#define SRAND srandom
#endif
#ifdef __NetBSD_Version__
#define RNUM long int
#define RAND random
#define SRAND srandom
#endif
#ifdef OpenBSD
#define RNUM long int
#define RAND arc4random
#define SRAND srandom
#endif

#ifndef RAND
#define RNUM int
#define RAND rand
#define SRAND srand
#endif

#ifdef NO_JLIB
/*** values returned by file_type() ***/
#define J2_CANT_GET      0
#define J2_UNKNOWN       1
#define J2_DIRECTORY     2  /* S_IFDIR  */
#define J2_REGULAR       3  /* S_IFREG  */
#define J2_CHAR_SPECIAL  4  /* S_IFCHR  */
#define J2_BLOCK_SPECIAL 5  /* S_IFBLK  */
#define J2_PIPE          6  /* S_IFPIP  */
#define J2_SOCKET        7  /* S_IFSOCK */
#define J2_SYMB_LINK     8  /* S_IFLNK  */
#define J2_FIFO          9  /* S_IFIFO  */
#endif /* NO_JLIB */

#ifndef FILE_URANDOM
#define FILE_URANDOM "/dev/urandom"
#endif
#ifndef FILE_RANDOM
#define FILE_RANDOM  "/dev/random"
#endif

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

#ifdef NO_JLIB
struct s_j2_datetime
{
  int month;    /* Month,           01 --12             */
  int dd;       /* Day of Month,    01 -- [28|29|30|31] */
  int yy;       /* two digit year,  00 -- 99            */
  int yyyy;     /* four digit year, 0000 -- 9999        */
  int hh;       /* Hour of the day, 00 -- 23            */
  int minutes;  /* minutes,         00 -- 59            */
  int ss;       /* seconds,         00 -- 59            */
  int mil;      /* milliseconds,    000 -- 999          */
  int tm_isdst; /* Daylight ?  0 = no, > 0 = yes        */
} ;
#endif /* NO_JLIB */

/*** messages ***/
#define ARG_CHAR          'c'  /* Replace Character                  */
#define ARG_ERR           'e'  /* Output Error File                  */
#define ARG_FORCE         'f'  /* force create files                 */
#define ARG_HELP          'h'  /* Show Help                          */
#define ARG_ITERATIONS    'I'  /* Iterations                         */
#define ARG_OUT           'o'  /* Output File                        */
#define ARG_RANDOM        'r'  /* use /dev/urandom as input          */
#define ARG_URAND         'R'  /* use rand() to generate random data */
#define ARG_VERBOSE       'v'  /* Verbose                            */
#define ARG_VERSION       'V'  /* Show Version Information           */
#define LIT_ENV_03        "File does not exist"
#define LIT_ENV_04        "Cannot access"
#define LIT_INFO_01       "\tBuild: %s %s\n"
#define LIT_UNDEFINED     "undefined"
#define LIT_UNKNOWN       "unknown"
#define MSG_ERR_E000      "Try '%s %c%c' for more information\n"
#define MSG_ERR_E002      "ERROR E002: Cannot open '%s' for write, processing aborted\n"
#define MSG_ERR_E004R     "ERROR E004: value %d invalid value for %c%c\n"
#define MSG_ERR_E005      "ERROR E005: need one or more files to process\n"
#define MSG_ERR_E006      "ERROR E006: '%s' is an invalid value for %c%c\n"
#define MSG_ERR_E025      "ERROR E025: File %s cannot be created, already exists\n"
#define MSG_INFO_I001     "I001: %s -- Processing '%s'\n"
#define MSG_INFO_I002     "I002: %s -- Reads %ld : Writes %ld\n"
#define MSG_INFO_I039     "I039: %s -- Pass %d of %d\n"
#define MSG_WARN_W001     "W001: Bypass File '%s', type %s\n"
#define MSG_WARN_W002     "W002: Open Error Bypass File '%s' : %s\n"
#define MSG_WARN_W003     "W003: Write Error on File '%s' : %s\n"
#define MSG_WARN_W024     "W024: Will use a call to random(3) to get overwrite characters\n"
#define MSG_WARN_W025     "W025: Will now try %s to get overwrite characters\n"
#define SWITCH_CHAR       '-'
#define UPDATE_MODE       "r+"
#define USG_MSG_ARG_CHAR_1C      "\t%c%c x\t\t: overwrite data in file with character 'x',\n"
#define USG_MSG_ARG_CHAR_1D      "\t\t\t  If 'x' longer than 1 byte and numeric, \n"
#define USG_MSG_ARG_CHAR_1E      "\t\t\t  decimal value 'x' is converted to a character.\n"
#define USG_MSG_ARG_CHAR_1F      "\t\t\t  If not specified, NULL is used.\n"
#define USG_MSG_ARG_ERR          "\t%c%c file\t\t: Write errors to file 'file', default stderr\n"
#define USG_MSG_ARG_FORCE        "\t%c%c\t\t: force create of files when found\n"
#define USG_MSG_ARG_HELP         "\t%c%c\t\t: Show brief help and exit\n"
#define USG_MSG_ARG_ITERATIONS   "\t%c%c n\t\t: Iterations for overwrites\n"
#define USG_MSG_ARG_OUT          "\t%c%c file\t\t: Write output to file 'file', default stdout\n"
#define USG_MSG_ARG_RANDOM_1     "\t%c%c\t\t: Use %s or if missing, %s\n"
#define USG_MSG_ARG_RANDOM_2     "\t\t\t: to overrite data instead of using zeros\n"
#define USG_MSG_ARG_URAND        "\t%c%c\t\t: Use rand(3) to generate Random Data\n"
#define USG_MSG_ARG_VERBOSE_4    "\t%c%c\t\t: Show detail File Stats\n"
#define USG_MSG_ARG_VERSION      "\t%c%c\t\t: Show revision information and exit\n"
#define USG_MSG_OPTIONS          "Options\n"
#define USG_MSG_USAGE_3          "usage:\t%s [OPTIONS] FILE_1 [FILE_2 ...]\n"
#define USG_MSG_ZERO_1           "\toverwrite all data in a file with a NULL character\n"
#define USG_MSG_ZERO_2           "\tor a specific character.\n"
#ifdef NO_JLIB
#endif /* NO_JLIB */

/*** prototypes ***/
#ifdef NO_JLIB
void j2_today(struct s_j2_datetime *dt);
int j2_f_exist(char *file_name);
long int j2_clr_str(char *s, char c, int size);
int j2_file_typ(char *file_name);
int j2_is_numr(char *s);
#endif /* NO_JLIB */

#endif /* END: REV_ZEROF_H */

/* zerof.h */
