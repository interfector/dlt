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
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h> /* linux/user.h */
#include <dlt.h>

void MemInject(int,struct user_regs_struct,int*,int);

int
HookInject(HOOKED * hook,int pid)
{
	struct user_regs_struct reg;
       
	if(ptrace(PTRACE_ATTACH,pid,0,0) <0)
		return -1;

	wait ((int*) 0);

	ptrace(PTRACE_GETREGS,pid,0,&reg);
        
	printf("@ Writing EIP at 0x%.8lx.\n",reg.eip);

	MemInject(pid,reg,(int*)hook->init_hook,hook->size);

	ptrace (PTRACE_DETACH,pid,0,0);

	return 0;
}

void
MemInject(int pid,struct user_regs_struct regs,int* shellcode,int size)
{
	int i;
	
	for (i = 0; i < size; i++)
		ptrace (PTRACE_POKETEXT, pid, regs.eip + i, *(int*) (shellcode + i));   
}
