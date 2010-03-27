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
 * shura thanks... <3
 * Thanks to shura for his code, included in bbot 
 * , downloadable at http://github.com/shurizzle/bbot
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <dlt.h>

/* Thanks to shura for his code, included in bbot 
 * , downloadable at http://github.com/shurizzle/bbot */

MODULE* modules = NULL;
HOOKED* hooker;

MODULE *
module_init (void)
{
    MODULE * newmodule = (MODULE *) calloc (1, sizeof (MODULE));
    newmodule->name = newmodule->handle = newmodule->next =NULL;
    return newmodule;
}

void
module_append (MODULE * to_append)
{
    MODULE * tmp = modules;
    if (modules == NULL)
    {
        modules = to_append;
        return;
    }
    while (tmp->next != NULL)
        tmp = tmp->next;
    tmp->next = to_append;
}

void
module_free (MODULE * to_free)
{
    free (to_free->name);
    free (to_free->handle);
    free (to_free);
}

void
module_delete (MODULE * to_del)
{
    MODULE * tmp = modules;

    if (tmp == to_del)
    {
        modules = modules->next;
        module_free (tmp);
    }

    while (tmp->next != to_del)
        tmp = tmp->next;

    tmp->next = to_del->next;
    module_free (to_del);
}

void
load_module (char * file)
{
    MODULE * module = module_init ();
    char * error, * x, i;
    HOOKED * hook;

    for (i = 0, x = file; *x; x++)
        if (*x == '/')
            i = x - file + 1;

    module->name = strdup (file + i);

    module->handle = dlopen (file, RTLD_LAZY);
    if (!module->handle)
    {
        fprintf (stderr, "Couldn't load %s: %s\n", file, dlerror ());
	   exit(1);
    }

    dlerror ();
    hook = (HOOKED *) dlsym (module->handle, "hook");
    hook->size = ((int (*)()) dlsym(module->handle,"hook_size"))();
    if ((error = dlerror ()))
    {
        fprintf (stderr, "Invalid module, couldn't found initialization\n\t%s\n", error);
	   exit(1);
    }

    load_hook (hook);
    module_append (module);
}

void
load_hook (HOOKED* hook)
{
	printf("@ SymLink:\t%s.\n",
			hook->sym);

	hooker = hook;
}
