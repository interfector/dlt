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
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <dlt.h>

HOOKED* hooker;

void
load_module (char * file)
{
	FILE* handle;
	char * error;
	HOOKED * hook;

	handle = dlopen (file, RTLD_LAZY);

	if (!handle)
	{
		fprintf (stderr, "Couldn't load %s: %s\n", file, dlerror ());
		exit(1);
	}

	dlerror();
	hook = (HOOKED *) dlsym (handle, "hook");
	hook->size = ((int (*)()) dlsym(handle,"hook_size"))();

	if ((error = dlerror ()))
	{
		fprintf (stderr, "Couldn't initialize the hook function.\n\t%s\n", error);
		exit(1);
	}

	load_hook (hook);
}

void
load_hook (HOOKED* hook)
{
	printf("@ SymLink:\t%s.\n",
			hook->sym);
	hooker = hook;
}
