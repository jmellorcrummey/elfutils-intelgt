/* Initialization of PPC specific backend library.
   Copyright (C) 2004, 2005, 2006, 2007, 2008, 2013 Red Hat, Inc.
   This file is part of elfutils.
   Written by Ulrich Drepper <drepper@redhat.com>, 2004.

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

#define BACKEND		intelgt_
#define RELOC_PREFIX	R_ZE_
#include "libebl_CPU.h"

/* This defines the common reloc hooks based on intelgt_reloc.def.  */
#include "common-reloc.c"


Ebl *
intelgt_init (Elf *elf __attribute__ ((unused)),
	      GElf_Half machine __attribute__ ((unused)),
	      Ebl *eh)
{
  intelgt_init_reloc (eh);

  HOOK (eh, machine_flag_check);
  HOOK (eh, reloc_simple_type);
  HOOK (eh, section_type_name);

  /* gcc/config/ #define DWARF_FRAME_REGISTERS.  */
  eh->frame_nregs = 0;

  return eh;
}
