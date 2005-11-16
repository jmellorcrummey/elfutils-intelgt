/* Common code for ebl reloc functions.
   Copyright (C) 2005 Red Hat, Inc.

   This program is Open Source software; you can redistribute it and/or
   modify it under the terms of the Open Software License version 1.0 as
   published by the Open Source Initiative.

   You should have received a copy of the Open Software License along
   with this program; if not, you may obtain a copy of the Open Software
   License version 1.0 from http://www.opensource.org/licenses/osl.php or
   by writing the Open Source Initiative c/o Lawrence Rosen, Esq.,
   3001 King Ranch Road, Ukiah, CA 95482.   */

#include "libebl_CPU.h"
#include <assert.h>

#define R_TYPE(name)	PASTE (RELOC_PREFIX, name)
#define PASTE(a, b)	PASTE_1 (a, b)
#define PASTE_1(a, b)	a##b
#define R_NAME(name)	R_NAME_1 (R_TYPE (name))
#define R_NAME_1(type)	#type

#define RELOC_TYPES		STRINGIFIED_PASTE (BACKEND, reloc.def)
#define STRINGIFIED_PASTE(a, b)	STRINGIFY (PASTE (a, b))
#define STRINGIFY(x)		STRINGIFY_1 (x)
#define STRINGIFY_1(x)		#x

/* Provide a table of reloc type names, in a PIC-friendly fashion.  */

static const struct EBLHOOK(reloc_nametable)
{
  char zero;
#define	RELOC_TYPE(type, uses) \
  char name_##type[sizeof R_NAME (type)];
#include RELOC_TYPES
#undef RELOC_TYPE
} EBLHOOK(reloc_nametable) =
  {
    '\0',
#define	RELOC_TYPE(type, uses) R_NAME (type),
#include RELOC_TYPES
#undef RELOC_TYPE
  };
#define reloc_namestr (&EBLHOOK(reloc_nametable).zero)

static const uint_fast16_t EBLHOOK(reloc_nameidx)[] =
{
#define	RELOC_TYPE(type, uses) \
  [R_TYPE (type)] = offsetof (struct EBLHOOK(reloc_nametable), name_##type),
#include RELOC_TYPES
#undef RELOC_TYPE
};
#define nreloc \
  ((int) (sizeof EBLHOOK(reloc_nameidx) / sizeof EBLHOOK(reloc_nameidx)[0]))

#define REL	(1 << (ET_REL - 1))
#define EXEC	(1 << (ET_EXEC - 1))
#define DYN	(1 << (ET_DYN - 1))
static const uint8_t EBLHOOK(reloc_valid)[] =
{
#define	RELOC_TYPE(type, uses) [R_TYPE (type)] = uses,
#include RELOC_TYPES
#undef RELOC_TYPE
};
#undef REL
#undef EXEC
#undef DYN

const char *
EBLHOOK(reloc_type_name) (int reloc,
			  char *buf __attribute__ ((unused)),
			  size_t len __attribute__ ((unused)))
{
  if (reloc >= 0 && reloc < nreloc && EBLHOOK(reloc_nameidx)[reloc] != 0)
    return &reloc_namestr[EBLHOOK(reloc_nameidx)[reloc]];
  return NULL;
}

bool
EBLHOOK(reloc_type_check) (int reloc)
{
  return reloc >= 0 && reloc < nreloc && EBLHOOK(reloc_nameidx)[reloc] != 0;
}

bool
EBLHOOK(reloc_valid_use) (Elf *elf, int reloc)
{
  uint8_t uses = EBLHOOK(reloc_valid)[reloc];

  GElf_Ehdr ehdr_mem;
  GElf_Ehdr *ehdr = gelf_getehdr (elf, &ehdr_mem);
  assert (ehdr != NULL);
  uint8_t type = ehdr->e_type;

  return type > ET_NONE && type < ET_CORE && (uses & (1 << (type - 1)));
}


bool
EBLHOOK(copy_reloc_p) (int reloc)
{
  return reloc == R_TYPE (COPY);
}

static void
EBLHOOK(init_reloc) (Ebl *ebl)
{
  ebl->reloc_type_name = EBLHOOK(reloc_type_name);
  ebl->reloc_type_check = EBLHOOK(reloc_type_check);
  ebl->reloc_valid_use = EBLHOOK(reloc_valid_use);
  ebl->copy_reloc_p = EBLHOOK(copy_reloc_p);
}
