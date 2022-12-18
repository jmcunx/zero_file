/*
 * Copyright (c) 2009 ... 2021 2022
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

#ifndef _MSDOS
#include <sys/param.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#ifdef OpenBSD
#include <err.h>
#endif
#ifdef __FreeBSD_version
#include <err.h>
#endif
#ifdef __NetBSD_Version__
#include <err.h>
#endif

#include <j_lib2.h>
#include <j_lib2m.h>

#include "zerof.h"

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

#define SDATETIME 25

/*
 * init_finfo() -- initialize out file structure
 */
void init_finfo(struct s_file_info *f)

{

  f->fp    = (FILE *) NULL;
  f->fname = (char *) NULL;

} /* init_finfo() */

/*
 * close_out() -- close output
 */
void close_out(struct s_file_info *f)

{

  if (f->fname != (char *) NULL)
    {
      fclose(f->fp);
      free(f->fname);
      f->fname = (char *) NULL;
    }

} /* close_out() */

/*
 * open_out() -- open, save file name and check status
 */
int open_out(FILE *wfp, struct s_file_info *f, char *fname, int force)

{

  int errsave;

  if (fname == (char *) NULL)
    return((int) TRUE);

  if (force == (int) FALSE)
    {
      if ( j2_f_exist(fname) )
	{
	  fprintf(wfp, MSG_ERR_E025, fname);
	  return((int) FALSE);
	}
    }

  f->fp = fopen(fname, "w");
  errsave = errno;

  if (f->fp == (FILE *) NULL)
    {
      f->fp = stderr;  /* needs to be something */
      fprintf(wfp, MSG_ERR_E002, fname);
      fprintf(wfp, "\t%s\n", strerror(errsave));
      return((int) FALSE);
    }

  /*** success, save file name ***/
  if (f->fname != (char *) NULL)
    free(f->fname);
  f->fname = strdup(fname);
  return((int) TRUE);

} /* open_out() */

/*
 * bye() -- clean up and exit
 */
void bye(struct s_work_area *w, int exit_val)

{

  close_out(&(w->out));
  close_out(&(w->err));

  w->err.fp = stderr;
  w->out.fp = stderr;

  /*** Done ***/
  exit(exit_val);

} /* bye() */

/*
 * display_rev() -- displays Version Information
 */
int display_rev(struct s_work_area *w)

{

  fprintf(w->out.fp,"Revision:\n");

#ifdef J_LIB2_H
  fprintf(w->out.fp,"\t%s %s\n", LIT_INFO_02, j2_get_build());
#endif

#ifdef OSTYPE
  fprintf(w->out.fp,"\t%s\n", OSTYPE);
#endif

#ifdef PRETTY_NAME
  fprintf(w->out.fp,"\t%s\n",PRETTY_NAME);
#endif  /* PRETTY_NAME  */

  fprintf(w->out.fp, LIT_INFO_01, __DATE__, __TIME__);

  return(EXIT_FAILURE);

} /* display_rev() */

/*
 * display_help() -- displays help info
 */
int display_help(struct s_work_area *w)

{

  fprintf(w->out.fp, USG_MSG_USAGE_3, PROG_NAME);
  fprintf(w->out.fp, USG_MSG_ZERO_1);
  fprintf(w->out.fp, USG_MSG_ZERO_2);
  fprintf(w->out.fp, USG_MSG_OPTIONS);
  fprintf(w->out.fp, USG_MSG_ARG_CHAR_1C,  SWITCH_CHAR, ARG_CHAR);
  fprintf(w->out.fp, USG_MSG_ARG_CHAR_1D);
  fprintf(w->out.fp, USG_MSG_ARG_CHAR_1E);
  fprintf(w->out.fp, USG_MSG_ARG_CHAR_1F);
  fprintf(w->out.fp, USG_MSG_ARG_ERR,       SWITCH_CHAR, ARG_ERR);
  fprintf(w->out.fp, USG_MSG_ARG_FORCE,     SWITCH_CHAR, ARG_FORCE);

  fprintf(w->out.fp, USG_MSG_ARG_HELP,       SWITCH_CHAR, ARG_HELP);
  fprintf(w->out.fp, USG_MSG_ARG_ITERATIONS, SWITCH_CHAR, ARG_ITERATIONS);
  fprintf(w->out.fp, USG_MSG_ARG_OUT,        SWITCH_CHAR, ARG_OUT);
  fprintf(w->out.fp, USG_MSG_ARG_URAND,      SWITCH_CHAR, ARG_URAND);
  fprintf(w->out.fp, USG_MSG_ARG_RANDOM_1,   SWITCH_CHAR, ARG_RANDOM, 
                     FILE_URANDOM, FILE_RANDOM);
  fprintf(w->out.fp, USG_MSG_ARG_RANDOM_2);
  fprintf(w->out.fp, USG_MSG_ARG_VERSION,    SWITCH_CHAR, ARG_VERSION);
  fprintf(w->out.fp, USG_MSG_ARG_VERBOSE_4,  SWITCH_CHAR, ARG_VERBOSE);

  return(EXIT_FAILURE);

} /* display_help() */

/*
 * get_args() -- load arguments
 */
void get_args(int argc, char **argv, struct s_work_area *w)

{
  char ckarg[CKARG_SIZE];
  int opt;
  int cnv_char;
  char *err_file   = (char *) NULL;
  char *out_file   = (char *) NULL;
  int show_help    = (int) FALSE;
  int show_rev     = (int) FALSE;
  int ok;

  w->overwrite  = JLIB2_CHAR_NULL;
  w->verbose    = 0;
  w->use_urand  = (int) FALSE;
  w->use_random = (int) FALSE;
  w->force      = (int) FALSE;
  w->iterations = (int) 1;

  init_finfo(&(w->out));
  init_finfo(&(w->err));
  w->err.fp    = stderr;
  w->out.fp    = stdout;

  snprintf(ckarg, CKARG_SIZE, "%c%c%c%c%c%c%c:%c:%c:%c:", 
	  ARG_FORCE, 
	  ARG_HELP, 
	  ARG_VERSION, 
	  ARG_URAND,
	  ARG_RANDOM,
	  ARG_VERBOSE,
	  ARG_CHAR, 
	  ARG_ERR,
	  ARG_OUT,
	  ARG_ITERATIONS);

  while ((opt = getopt(argc, argv, ckarg)) != -1)
    {
      switch (opt)
	{
	  case ARG_HELP:
	    show_help = (int) TRUE;
	    break;
	  case ARG_VERSION:
	    show_rev = (int) TRUE;
	    break;
	  case ARG_FORCE:
	    w->force = (int) TRUE;
	    break;
	  case ARG_URAND:
	    w->use_urand = (int) TRUE;
	    break;
	  case ARG_RANDOM:
	    w->use_random = (int) TRUE;
	    break;
	  case ARG_CHAR:
	    if (strlen(optarg) > 1)
	      {
		if (j2_is_numr(optarg) != (int) TRUE)
		  {
		    fprintf(w->err.fp, MSG_ERR_E006, optarg,  SWITCH_CHAR, ARG_CHAR);
		    fprintf(w->err.fp, MSG_ERR_E000, PROG_NAME, SWITCH_CHAR, ARG_HELP);
		    bye(w, EXIT_FAILURE);
		  }
		cnv_char = atoi(optarg);
		w->overwrite = cnv_char;
	      }
	    else
	      w->overwrite = (*optarg);
	    break;
	  case ARG_ITERATIONS:
	    if (j2_is_numr(optarg) == (int) TRUE)
	      w->iterations = atoi(optarg);
	    else
	      {
		fprintf(w->err.fp, MSG_ERR_E006, optarg,  SWITCH_CHAR, ARG_ITERATIONS);
		fprintf(w->err.fp, MSG_ERR_E000, PROG_NAME, SWITCH_CHAR, ARG_HELP);
		bye(w, EXIT_FAILURE);
	      }
	    if (w->iterations < 1)
	      {
		fprintf(w->err.fp, MSG_ERR_E004R, w->iterations,  SWITCH_CHAR, ARG_ITERATIONS);
		fprintf(w->err.fp, MSG_ERR_E000, PROG_NAME, SWITCH_CHAR, ARG_HELP);
		bye(w, EXIT_FAILURE);
	      }
	    break;
	  case ARG_OUT:
	    out_file = optarg;
	    break;
	  case ARG_ERR:
	    err_file = optarg;
	    break;
	  case ARG_VERBOSE:
	    (w->verbose)++;
	    break;
	  default:
	    fprintf(w->err.fp, MSG_ERR_E000, PROG_NAME, SWITCH_CHAR, ARG_HELP);
	    bye(w, EXIT_FAILURE);
	    break;
	}
    }

  /*** if necessary - save stdout/err files and open ***/
  ok = open_out(stderr, &(w->err), err_file, w->force);
  if ( ok )
    ok = open_out(w->err.fp, &(w->out), out_file, w->force);
  if ( ! ok )
    {
      fprintf(w->err.fp, MSG_ERR_E000, PROG_NAME, SWITCH_CHAR, ARG_HELP);
      exit(EXIT_FAILURE);
    }

  /*** show help/rev ? ***/
  ok = EXIT_SUCCESS;
  if (show_help)
    ok = display_help(w);
  if (show_rev)
    ok = display_rev(w);
  if (ok != EXIT_SUCCESS)
    bye(w, EXIT_FAILURE);

} /* get_args() */

/*
 * ckfile() -- see if file type is OK
 */
int ckfile(FILE *msg_fp, char *fname)

{

  /*** first see if the file exists ***/
  if (access(fname, F_OK) != 0)
    {
      fprintf(msg_fp, MSG_WARN_W002, fname, LIT_ENV_03);
      return((int) FALSE);
    }

  /*** Check file type ***/
  switch (j2_file_typ(fname))
    {
      case J2_BLOCK_SPECIAL:
	fprintf(msg_fp, MSG_WARN_W001, fname, "J2_BLOCK_SPECIAL");
	return((int) FALSE);
	break;
      case J2_CANT_GET:
	fprintf(msg_fp, MSG_WARN_W001, fname, "J2_CANT_GET");
	return((int) FALSE);
	break;
      case J2_CHAR_SPECIAL:
	fprintf(msg_fp, MSG_WARN_W001, fname, "J2_CHAR_SPECIAL");
	return((int) FALSE);
	break;
      case J2_DIRECTORY:
	fprintf(msg_fp, MSG_WARN_W001, fname, "J2_DIRECTORY");
	return((int) FALSE);
	break;
      case J2_FIFO:
	fprintf(msg_fp, MSG_WARN_W001, fname, "J2_FIFO");
	return((int) FALSE);
	break;
      case J2_PIPE:
	fprintf(msg_fp, MSG_WARN_W001, fname, "J2_PIPE");
	return((int) FALSE);
	break;
      case J2_REGULAR:
	/* continue on */
	break;
      case J2_SOCKET:
	fprintf(msg_fp, MSG_WARN_W001, fname, "J2_SOCKET");
	return((int) FALSE);
	break;
      case J2_SYMB_LINK:
	fprintf(msg_fp, MSG_WARN_W001, fname, "J2_SYMB_LINK");
	return((int) FALSE);
	break;
      case J2_UNKNOWN:
	fprintf(msg_fp, MSG_WARN_W001, fname, LIT_UNKNOWN);
	return((int) FALSE);
	break;
      default:
	fprintf(msg_fp, MSG_WARN_W001, fname, LIT_UNDEFINED);
	return((int) FALSE);
	break;
    }

  /*** check file access ***/
  if (access(fname, (R_OK | W_OK)) != 0)
    {
      fprintf(msg_fp, MSG_WARN_W002, fname, LIT_ENV_04);
      return((int) FALSE);
    }

  return((int) TRUE);

} /* ckfile() */

/*
 * date_time_str() -- format a date time string
 */
char *date_time_str(void)
{

  static char date_time[SDATETIME];
  struct s_j2_datetime ds;

  j2_clr_str(date_time, JLIB2_CHAR_NULL, 50);

  j2_today(&ds);

  snprintf(date_time, SDATETIME, "%04d/%02d/%02d %02d:%02d:%02d.%03d",
           ds.yyyy, ds.month,   ds.dd, ds.hh,   ds.minutes, ds.ss, ds.mil);

  return(date_time);

} /* END: date_time_str() */

/*
 * get_seed() -- get a random number seed, try reading /dev/random or
 *               /dev/urandom first, otherwise use time()
 */
unsigned int get_seed(void)
{

  unsigned int s;
  FILE *fp;

  fp = fopen(FILE_URANDOM, "r");

  if (fp == (FILE *) NULL)
    fp = fopen(FILE_RANDOM, "r"); /* now try /dev/random */

  if (fp == (FILE *) NULL)
    return((unsigned int) time(0));

  s = (unsigned int) fgetc(fp);

  fclose(fp);

  if (s == (unsigned) 0)
    s = get_seed(); /* never want a 0 seed */

  return(s);

} /* get_seed() */

/*
 * get_rand() -- get a random number
 */
long int get_rand(void)
{
  static RNUM r = (RNUM) 0;

  if (r == (RNUM) 0)
    SRAND(get_seed());

  r = (int) RAND();

  return(r);

} /* get_rand() */

/*
 * open_random() -- Open a Random Device
 */
FILE *open_random(struct s_work_area *w)
{
  static FILE *fp = (FILE *) NULL;
  static int show_issues = (int) TRUE;

  fp = fopen(FILE_URANDOM, "r");
  if (fp != (FILE *) NULL)
    return(fp);
  if ((w->verbose > 0) && show_issues)
    {
      fprintf(w->err.fp, MSG_WARN_W002, FILE_URANDOM, strerror(errno));
      fprintf(w->err.fp, MSG_WARN_W025, FILE_RANDOM);
    }

  fp = fopen(FILE_RANDOM, "r"); /* now try /dev/urandom */
  if (fp != (FILE *) NULL)
    return(fp);

  if ((w->verbose > 0) && show_issues)
    {
      show_issues   = (int) FALSE;
      fprintf(w->err.fp, MSG_WARN_W002, FILE_RANDOM, strerror(errno));
      fprintf(w->err.fp, MSG_WARN_W024);
    }
  w->use_random = (int) FALSE;
  w->use_urand  = (int) TRUE;

  return(fp);

} /* open_random() */

/*
 * process_file() -- Override file data
 */
void process_file(char *fname, struct s_work_area *w)

{
  FILE *fp;
  FILE *fpr = (FILE *) NULL;
  int c;
  long pos_before, pos_after;
  counter byte_read  = (counter) 0;
  counter byte_write = (counter) 0;

  if ((w->use_random == (int) TRUE) && (w->use_urand == (int) FALSE))
    fpr = open_random(w);

  fp = fopen(fname, UPDATE_MODE);

  if (fp == (FILE *) NULL)
    {
      fprintf(w->err.fp, MSG_WARN_W002, fname, strerror(errno));
      return;
    }

  /*** override data ***/
  if (w->verbose > 0)
    fprintf(w->out.fp, MSG_INFO_I001, date_time_str(), fname);

  pos_before = ftell(fp);

  while ( (c = fgetc(fp)) != EOF )
    {
      byte_read++;
      pos_after = ftell(fp);
      fseek(fp, pos_before, SEEK_SET);
      fflush(fp);
      if (w->use_urand == (int) TRUE)
	w->overwrite = get_rand();
      else
	{
	  if (w->use_random == (int) TRUE)
	    w->overwrite = fgetc(fpr);
	}
      if (fputc(w->overwrite, fp) == EOF)
	fprintf(w->err.fp, MSG_WARN_W003, fname, strerror(errno));
      else
	byte_write++;
      fseek(fp, pos_after, SEEK_SET);
      pos_before = ftell(fp);
    }

  /*** DONE ***/
  fflush(fp);
  fclose(fp);
  if (fpr != (FILE *) NULL)
    fclose(fpr);

  if (w->verbose > 0)
    fprintf(w->out.fp, MSG_INFO_I002, date_time_str(), byte_read, byte_write);

} /* process_file() */

/*
 * main()
 */
int main(int argc, char **argv)

{
  struct s_work_area w;
  int i, p;

#ifdef OpenBSD
  if (pledge("stdio rpath wpath cpath",NULL) == -1)
    err(1,"pledge\n");
#endif

  get_args(argc, argv, &w);

  for (i = optind; i < argc; i++)
    {
      if ( ckfile(w.err.fp, argv[i]) )
	{
	  for (p = 0; p < w.iterations; p++)
	    {
	      if (w.verbose > 0)
		fprintf(w.out.fp, MSG_INFO_I039, date_time_str(), 
	      (p + 1), w.iterations);
	      process_file(argv[i], &w);
	    }
	}
    }

  if (i == optind)
    {
      fprintf(w.err.fp, MSG_ERR_E005);
      fprintf(w.err.fp, MSG_ERR_E000, PROG_NAME, SWITCH_CHAR, ARG_HELP);
      bye(&w, EXIT_FAILURE);
    }

  bye(&w, EXIT_SUCCESS);
  exit(EXIT_SUCCESS);

} /* main() */
