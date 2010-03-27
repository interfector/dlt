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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <dlt.h>

extern HOOKED * hooker;

extern char* hook_value;

void
die (int code,char* fmt,...)
{
	va_list v;

	va_start(v,fmt);

	vfprintf(stderr,fmt,v);

	va_end(v);

	exit(code);
}

void
banner ()
{
	printf("     _ _ _   \n"
		  "    | | | |  \n"
		  "  __| | | |_ \n"
		  " / _` | | __|\n"
	       "| (_| | | |_ \n"
		  " \\__,_|_|\\__|\n\n");
}

int
main (int argc, char **argv)
{
	int ch;
	char * module = NULL;
	int pid = -1;

	banner();

	while((ch = getopt(argc,argv,"vhf:p:")) > 0)
	{
		switch(ch)
		{
			case 'v':
				die(1,VPRINT);
				break;
			case 'h':
				die(1,USAGE,argv[0]);
				break;
			case 'f':
				module = strdup(optarg);
				break;
			case 'p':
				pid = atoi(optarg);
				break;
			default:
				break;
		}
	}

	if(pid == -1 || !module)
		die(1,USAGE,argv[0]);

	printf("@ PID:\t\t%d.\n",pid);
	printf("@ Module:\t%s.\n",module);

	load_module(module);

	ch = ElfSym_read(module,hooker->sym);

	if(hooker->size <= 0)
		die(1,"* Error: symbol size must be > 0.\n");

	printf("@ Hook: found at <%p>[0..%u] <%p>.\n",hooker->init_hook,hooker->size,hooker->init_hook+hooker->size);

	hooker->init_hook = (int (*)()) hook_value;

	if(HookInject(hooker,hooker->size,pid) < 0)
	{
		perror("ptrace");

		exit(1);
	}

	return 0;
}
