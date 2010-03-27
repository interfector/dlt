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


typedef struct MODULE   MODULE;
typedef struct HOOKED   HOOKED;

struct MODULE
{
    char    *   name;
    void    *   handle;
    MODULE  *   next;
};

struct HOOKED
{
    char * sym;
    int (*init_hook)();

    int size;
};

MODULE* module_init (void);
void module_append (MODULE *);
void module_free (MODULE *);
void module_delete (MODULE *);
void load_module (char *);
void load_hook(HOOKED*);

void die(int,char*,...);

#define USAGE "Usage: %s -f <file> -p <PID> [-v] [-h]\n"

#define VPRINT "dlt-"VERSION" Copyright (C) 2010 nex\n" \
		  "This program comes with ABSOLUTELY NO WARRANTY; for details type `show w'.\n" \
		  "This is free software, and you are welcome to redistribute it\n" \
	       "under certain conditions; type `show c' for details.\n"

int ElfSym_read(char*,char*);
int HookInject(HOOKED*,int,int);