// x86_64-elf/include/link.h
#ifndef _LINK_H
#define _LINK_H

#include <elf.h>

#if UINTPTR_MAX > 0xffffffff
#define ElfW(type) Elf64_ ## type
#else
#define ElfW(type) Elf32_ ## type
#endif

#endif
