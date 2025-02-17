/* intelgt specific symbolic name handling.
   Copyright (C) 2002, 2005, 2018 Red Hat, Inc.
   This file is part of elfutils.
   Written by Ulrich Drepper <drepper@redhat.com>, 2002.

   This file is free software; you can redistribute it and/or modify
   it under the terms of either

     * the GNU Lesser General Public License as published by the Free
       Software Foundation; either version 3 of the License, or (at
       your option) any later version

   or

     * the GNU General Public License as published by the Free
       Software Foundation; either version 2 of the License, or (at
       your option) any later version

   or both in parallel, as here.

   elfutils is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received copies of the GNU General Public License and
   the GNU Lesser General Public License along with this program.  If
   not, see <http://www.gnu.org/licenses/>.  */

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <assert.h>
#include <elf.h>
#include <stddef.h>

#define BACKEND		intelgt_
#include "libebl_CPU.h"

/* Check whether machine flags are valid.  */
bool
intelgt_machine_flag_check (GElf_Word flags)
{
  /* no intelgt flags yet */
  return true;
}

/* Check for the simple reloc types.  */
Elf_Type
intelgt_reloc_simple_type (Ebl *ebl __attribute__ ((unused)), int type,
			  int *addsub)
{
  switch (type)
    {
    case R_ZE_SYM_ADDR:
      *addsub = 1;
      return ELF_T_XWORD;
    case R_ZE_SYM_ADDR_32:
      *addsub = 1;
      return ELF_T_WORD;
    case R_ZE_SYM_ADDR_32_HI:
    case R_ZE_PER_THREAD_PAYLOAD_OFFSET_32:
      /* don't know how to handle these */
    default:
      return ELF_T_NUM;
    }
}

/* Return symbolic representation of section type.  */
const char *
intelgt_section_type_name (int type,
			   char *buf __attribute__ ((unused)),
			   size_t len __attribute__ ((unused)))
{
  if (type == SHT_ZEBIN_SPIRV)
    return "ZEBIN_SPIRV";

  if (type == SHT_ZEBIN_ZEINFO)
    return "ZEBIN_ZEINFO";

  if (type == SHT_ZEBIN_GTPIN_INFO)
    return "ZEBIN_GTPIN_INFO";

  if (type == SHT_ZEBIN_VISAASM)
    return "ZEBIN_VISAASM";

  if (type == SHT_ZEBIN_MISC)
    return "ZEBIN_MISC";

  return NULL;
}
