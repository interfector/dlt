/*
 * DLT , Dynamic Library Tracer , useful program to inject plugin into a process' memory.
 * Copyright (C) 2010 nex
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define INIT_HOOK        extern char init; extern char end; asm("init:")
#define HOOK_SIZE		unsigned int hook_size() { return (unsigned int)&end - (unsigned int)&init; }
#define HOOKFUNC(name)   int name (int argc,char *argv[])
#define END_HOOK         asm ("end:")
#define HOOK(x)		HOOKED hook = { #x, x }

#define HOOK_ASM(name)	extern int name (); asm(#name":");

typedef struct
{
    char*	sym;
    int	(*init_hook)();
    int	size;
} HOOKED;
