/*
 * Copyright (c) 2003-2008 Stephen Williams (steve@icarus.com)
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

#include <assert.h>
#include <string.h>
#include "sys_priv.h"

PLI_UINT64 timerec_to_time64(const struct t_vpi_time*time)
{
      PLI_UINT64 tmp;

      tmp = time->high;
      tmp <<= 32;
      tmp |= (PLI_UINT64) time->low;

      return tmp;
}

/*
 * This routine returns 1 if the argument is a constant value,
 * otherwise it returns 0.
 */
unsigned is_constant_obj(vpiHandle obj)
{
      assert(obj);
      unsigned rtn = 0;

      switch(vpi_get(vpiType, obj)) {
	case vpiConstant:
	case vpiParameter:
	    rtn = 1;
	    break;
      }

      return rtn;
}

/*
 * This routine returns 1 if the argument supports has a numeric value,
 * otherwise it returns 0.
 */
unsigned is_numeric_obj(vpiHandle obj)
{
    assert(obj);
    unsigned rtn = 0;

    switch(vpi_get(vpiType, obj)) {
      case vpiConstant:
      case vpiParameter:
	  /* These cannot be a string constant. */
	if (vpi_get(vpiConstType, obj) != vpiStringConst) rtn = 1;
	break;

	/* These can have a valid numeric value. */
      case vpiIntegerVar:
      case vpiMemoryWord:
      case vpiNet:
      case vpiPartSelect:
      case vpiRealVar:
      case vpiReg:
      case vpiTimeVar:
	rtn = 1;;
	break;
    }

    return rtn;
}


/*
 * This routine returns 1 if the argument supports a valid string value,
 * otherwise it returns 0.
 */
unsigned is_string_obj(vpiHandle obj)
{
    assert(obj);
    unsigned rtn = 0;

    switch(vpi_get(vpiType, obj)) {
      case vpiConstant:
      case vpiParameter: {
	  /* These must be a string or binary constant. */
	PLI_INT32 ctype = vpi_get(vpiConstType, obj);
	if (ctype == vpiStringConst || ctype == vpiBinaryConst) rtn = 1;
	break;
      }

	/* These can have a valid string value. */
      case vpiIntegerVar:
      case vpiMemoryWord:
      case vpiNet:
      case vpiPartSelect:
      case vpiReg:
      case vpiTimeVar:
	rtn = 1;;
	break;
    }

    return rtn;
}


/*
 * Find the enclosing module.
 */
vpiHandle sys_func_module(vpiHandle obj)
{
      assert(obj);

      while (vpi_get(vpiType, obj) != vpiModule) {
	    obj = vpi_handle(vpiScope, obj);
	    assert(obj);
      }

      return obj;
}

/*
 * Standard compiletf routines.
 */

/* For system tasks/functions that do not take an argument. */
PLI_INT32 sys_no_arg_compiletf(PLI_BYTE8 *name)
{
      vpiHandle callh = vpi_handle(vpiSysTfCall, 0);
      vpiHandle argv = vpi_iterate(vpiArgument, callh);

      /* Make sure there are no arguments. */
      if (argv != 0) {
	    char msg [64];
	    snprintf(msg, 64, "ERROR: %s line %d:",
	             vpi_get_str(vpiFile, callh),
	             (int)vpi_get(vpiLineNo, callh));

	    unsigned argc = 0;
	    while (vpi_scan(argv)) argc += 1;

            vpi_printf("%s %s does not take an argument.\n", msg, name);
            vpi_printf("%*s Found %u extra argument%s.\n",
	               strlen(msg), " ", argc, argc == 1 ? "" : "s");
            vpi_control(vpiFinish, 1);
      }

      return 0;
}

/* For system tasks/functions that take a single numeric argument. */
PLI_INT32 sys_one_numeric_arg_compiletf(PLI_BYTE8 *name)
{
      vpiHandle callh = vpi_handle(vpiSysTfCall, 0);
      vpiHandle argv = vpi_iterate(vpiArgument, callh);

      /* Check that there is an argument and that it is numeric. */
      if (argv == 0) {
            vpi_printf("ERROR: %s line %d: ", vpi_get_str(vpiFile, callh),
                       (int)vpi_get(vpiLineNo, callh));
            vpi_printf("%s requires a single numeric argument.\n", name);
            vpi_control(vpiFinish, 1);
            return 0;
      }

      if (! is_numeric_obj(vpi_scan(argv))) {
            vpi_printf("ERROR: %s line %d: ", vpi_get_str(vpiFile, callh),
                       (int)vpi_get(vpiLineNo, callh));
            vpi_printf("%s's argument must be numeric.\n", name);
            vpi_control(vpiFinish, 1);
      }

      /* Make sure there are no extra arguments. */
      if (vpi_scan(argv) != 0) {
	    char msg [64];
	    snprintf(msg, 64, "ERROR: %s line %d:",
	             vpi_get_str(vpiFile, callh),
	             (int)vpi_get(vpiLineNo, callh));

	    unsigned argc = 1;
	    while (vpi_scan(argv)) argc += 1;

            vpi_printf("%s %s takes a single numeric argument.\n", msg, name);
            vpi_printf("%*s Found %u extra argument%s.\n",
	               strlen(msg), " ", argc, argc == 1 ? "" : "s");
            vpi_control(vpiFinish, 1);
      }

      return 0;
}

/* For system tasks/functions that take a single optional numeric argument. */
PLI_INT32 sys_one_opt_numeric_arg_compiletf(PLI_BYTE8 *name)
{
      vpiHandle callh = vpi_handle(vpiSysTfCall, 0);
      vpiHandle argv = vpi_iterate(vpiArgument, callh);

      /* The argument is optional so just return if none are found. */
      if (argv == 0) return 0;

      if (! is_numeric_obj(vpi_scan(argv))) {
            vpi_printf("ERROR: %s line %d: ", vpi_get_str(vpiFile, callh),
                       (int)vpi_get(vpiLineNo, callh));
            vpi_printf("%s's argument must be numeric.\n", name);
            vpi_control(vpiFinish, 1);
      }

      /* Make sure there are no extra arguments. */
      if (vpi_scan(argv) != 0) {
	    char msg [64];
	    snprintf(msg, 64, "ERROR: %s line %d:",
	             vpi_get_str(vpiFile, callh),
	             (int)vpi_get(vpiLineNo, callh));

	    unsigned argc = 1;
	    while (vpi_scan(argv)) argc += 1;

            vpi_printf("%s %s takes at most one numeric argument.\n",
	               msg, name);
            vpi_printf("%*s Found %u extra argument%s.\n",
	               strlen(msg), " ", argc, argc == 1 ? "" : "s");
            vpi_control(vpiFinish, 1);
      }

      return 0;
}

/* For system tasks/functions that take two numeric arguments. */
PLI_INT32 sys_two_numeric_args_compiletf(PLI_BYTE8 *name)
{
      vpiHandle callh = vpi_handle(vpiSysTfCall, 0);
      vpiHandle argv = vpi_iterate(vpiArgument, callh);
      vpiHandle arg;

      /* Check that there are two argument and that they are numeric. */
      if (argv == 0) {
            vpi_printf("ERROR: %s line %d: ", vpi_get_str(vpiFile, callh),
                       (int)vpi_get(vpiLineNo, callh));
            vpi_printf("%s requires two numeric arguments.\n", name);
            vpi_control(vpiFinish, 1);
            return 0;
      }

      if (! is_numeric_obj(vpi_scan(argv))) {
            vpi_printf("ERROR: %s line %d: ", vpi_get_str(vpiFile, callh),
                       (int)vpi_get(vpiLineNo, callh));
            vpi_printf("%s's first argument must be numeric.\n", name);
            vpi_control(vpiFinish, 1);
      }

      arg = vpi_scan(argv);
      if (! arg) {
            vpi_printf("ERROR: %s line %d: ", vpi_get_str(vpiFile, callh),
                       (int)vpi_get(vpiLineNo, callh));
            vpi_printf("%s requires a second (numeric) argument.\n", name);
            vpi_control(vpiFinish, 1);
            return 0;
      }

      if (! is_numeric_obj(arg)) {
            vpi_printf("ERROR: %s line %d: ", vpi_get_str(vpiFile, callh),
                       (int)vpi_get(vpiLineNo, callh));
            vpi_printf("%s's second argument must be numeric.\n", name);
            vpi_control(vpiFinish, 1);
      }

      /* Make sure there are no extra arguments. */
      if (vpi_scan(argv) != 0) {
	    char msg [64];
	    snprintf(msg, 64, "ERROR: %s line %d:",
	             vpi_get_str(vpiFile, callh),
	             (int)vpi_get(vpiLineNo, callh));

	    unsigned argc = 1;
	    while (vpi_scan(argv)) argc += 1;

            vpi_printf("%s %s takes two numeric arguments.\n", msg, name);
            vpi_printf("%*s Found %u extra argument%s.\n",
	               strlen(msg), " ", argc, argc == 1 ? "" : "s");
            vpi_control(vpiFinish, 1);
      }

      return 0;
}

/* For system tasks/functions that take a single string argument. */
PLI_INT32 sys_one_string_arg_compiletf(PLI_BYTE8 *name)
{
      vpiHandle callh = vpi_handle(vpiSysTfCall, 0);
      vpiHandle argv = vpi_iterate(vpiArgument, callh);

      /* Check that there is an argument and that it is a string. */
      if (argv == 0) {
            vpi_printf("ERROR: %s line %d: ", vpi_get_str(vpiFile, callh),
                       (int)vpi_get(vpiLineNo, callh));
            vpi_printf("%s requires a single string argument.\n", name);
            vpi_control(vpiFinish, 1);
            return 0;
      }
      if (! is_string_obj(vpi_scan(argv))) {
            vpi_printf("ERROR: %s line %d: ", vpi_get_str(vpiFile, callh),
                       (int)vpi_get(vpiLineNo, callh));
            vpi_printf("%s's argument must be a string.\n", name);
            vpi_control(vpiFinish, 1);
      }

      /* Make sure there are no extra arguments. */
      if (vpi_scan(argv) != 0) {
	    char msg [64];
	    snprintf(msg, 64, "ERROR: %s line %d:",
	             vpi_get_str(vpiFile, callh),
	             (int)vpi_get(vpiLineNo, callh));

	    unsigned argc = 1;
	    while (vpi_scan(argv)) argc += 1;

            vpi_printf("%s %s takes a single string argument.\n", msg, name);
            vpi_printf("%*s Found %u extra argument%s.\n",
	               strlen(msg), " ", argc, argc == 1 ? "" : "s");
            vpi_control(vpiFinish, 1);
      }

      return 0;
}