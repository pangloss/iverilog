#ifndef __globals_H
#define __globals_H
/*
 * Copyright (c) 2000 Stephen Williams (steve@icarus.com)
 *
 *    This source code is free software; you can redistribute it
 *    and/or modify it in source code form under the terms of the GNU
 *    General Public License as published by the Free Software
 *    Foundation; either version 2 of the License, or (at your option)
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */
#ifdef HAVE_CVS_IDENT
#ident "$Id: globals.h,v 1.18 2003/11/13 04:09:49 steve Exp $"
#endif

# include  <stddef.h>

  /* This is the base (i.e. -B<value>) of the Icarus Verilog files. */
extern const char*base;

  /* This is the path to the iconfig file sent to ivl. */
extern char* iconfig_path;
extern char* iconfig_common_path;

  /* Ths is the optional -M<dependfile> value, if one was supplied. */
extern const char*depfile;

  /* Ths is the optional -N<path> value, if one was supplied. */
extern const char*npath;

  /* This is the name of the output file that the user selected. */
extern const char*opath;

  /* This pointer is set if there were -s<value> parameters. */
extern char*start;

  /* This flag is true if the -S flag was used on the command line. */
extern int synth_flag;

  /* This is the name of the selected target. */
extern const char*targ;

  /* Perform variable substitutions on the string. */
extern char* substitutions(const char*str);

  /* Add the name to the list of source files. */
extern void process_file_name(const char*name);

  /* Add the name to the list of library directories. */
extern void process_library_switch(const char*name);
extern void process_library_nocase_switch(const char*name);
extern void process_library2_switch(const char*name);

  /* Add a new include file search directory */
extern void process_include_dir(const char*name);

  /* Add a new -D define. */
extern void process_define(const char*name);

  /* -v */
extern int verbose_flag;

extern char warning_flags[];

  /* -y and -Y flags from the command line. */
extern char* library_flags;
extern char* library_flags2;

extern const char*lookup_pattern(const char*key);

extern int build_string(char*out, size_t olen, const char*pattern);

/*
 * $Log: globals.h,v $
 * Revision 1.18  2003/11/13 04:09:49  steve
 *  Pass flags through the temporary config file.
 *
 * Revision 1.17  2003/11/01 04:21:57  steve
 *  Add support for a target static config file.
 *
 * Revision 1.16  2002/08/12 01:35:01  steve
 *  conditional ident string using autoconfig.
 *
 * Revision 1.15  2002/06/23 20:10:51  steve
 *  Variable substitution in command files.
 *
 * Revision 1.14  2002/05/28 20:40:37  steve
 *  ivl indexes the search path for libraries, and
 *  supports case insensitive module-to-file lookup.
 *
 * Revision 1.13  2002/05/28 00:50:40  steve
 *  Add the ivl -C flag for bulk configuration
 *  from the driver, and use that to run library
 *  modules through the preprocessor.
 *
 * Revision 1.12  2002/05/24 01:13:00  steve
 *  Support language generation flag -g.
 *
 * Revision 1.11  2002/04/04 05:26:13  steve
 *  Add dependency generation.
 *
 * Revision 1.10  2001/11/16 05:07:19  steve
 *  Add support for +libext+ in command files.
 *
 * Revision 1.9  2001/11/13 03:30:26  steve
 *  The +incdir+ plusarg can take multiple directores,
 *  and add initial support for +define+ in the command file.
 *
 * Revision 1.8  2001/11/12 18:47:32  steve
 *  Support +incdir in command files, and ignore other
 *  +args flags. Also ignore -a and -v flags.
 *
 * Revision 1.7  2001/11/12 01:26:36  steve
 *  More sophisticated command file parser.
 *
 * Revision 1.6  2001/10/23 00:37:30  steve
 *  The -s flag can now be repeated on the iverilog command.
 *
 * Revision 1.5  2001/10/20 23:02:40  steve
 *  Add automatic module libraries.
 *
 * Revision 1.4  2001/07/03 04:09:25  steve
 *  Generate verbuse status messages (Stephan Boettcher)
 *
 * Revision 1.3  2000/11/09 21:58:00  steve
 *  Remember to include the -S condition.
 *
 * Revision 1.2  2000/10/28 03:45:47  steve
 *  Use the conf file to generate the vvm ivl string.
 *
 * Revision 1.1  2000/10/08 22:36:56  steve
 *  iverilog with an iverilog.conf configuration file.
 *
 */
#endif
