// SPDX-License-Identifier: Zlib
//
// Copyright (C) 2023 Antonio Niño Díaz

#include <stddef.h>

#include <nds/system.h>

// Documented here: https://devkitpro.org/wiki/Homebrew_Menu

// When this function is called:
//
//   argvMagic:   ARGV_MAGIC
//   commandLine: A copy of the command line string, copied to the heap base.
//   length:      Length of the command line.
//   argc:        0
//   argv:        NULL
//   endARGV:     NULL
//
// It should fill the last three values. The `argv` pointer array must be placed
// right after the command line string in the heap. The first free address
// after `argv` must be placed in `endARGV`. The new start of the heap will be
// located at that address.
void build_argv(struct __argv *argstruct)
{
    char *p = argstruct->commandLine;
    size_t len = argstruct->length;

    // Make sure that the last argument has a NULL terminator.
    if (p[len - 1] != '\0')
        p[len] = '\0';
    else
        len--;

    char *end = p + len;

    // Get the first available address in the heap aligned to the size of a
    // pointer. `argv` will be placed there.
    size_t free_heap_base = (uintptr_t)p + len;
    size_t alignment = sizeof(char **);
    free_heap_base = (free_heap_base + alignment) & ~(alignment - 1);
    char **argv = (char **)free_heap_base;

    int argc = 0;

    while (p < end)
    {
        // This is either the start of a string or a NULL terminator. If there
        // are no arguments, allocate a NULL pointer anyway just in case.
        argv[argc++] = p;

        // Search end of the string
        while (*p != '\0')
            p++;

        // Move to start of next string
        p++;
    }

    argstruct->argv = argv;
    argstruct->argc = argc;
    argstruct->endARGV = &argv[argc]; // Address after the end of argv
}
