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
#include <memory.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h> /* linux/user.h */
#include <dlt.h>

void MemInject(int,long,void*,int);
void MemGet(int,long,long*,int);

#define WORD_SIZE 4

int
HookInject(HOOKED * hook,int pid)
{
	long * backup;
	struct user_regs_struct reg;
       
	if(ptrace(PTRACE_ATTACH,pid,0,0) <0)
		return -1;
		
	backup = malloc(hook->size);

	wait (NULL);

	ptrace(PTRACE_GETREGS,pid,0,&reg);
        
	printf("@ Writing EIP at 0x%.8lx.\n",reg.eip);

	MemGet(pid,reg.eip,backup,hook->size);
	MemInject(pid,reg.eip,hook->init_hook,hook->size);

	ptrace(PTRACE_SETREGS,pid,NULL,&reg);
	ptrace(PTRACE_CONT,pid,NULL,NULL);

	wait(NULL);

	printf("@ Restoring execution.\n");
	MemInject(pid,reg.eip,backup,hook->size);

	ptrace (PTRACE_DETACH,pid,0,0);

	return 0;
}

void MemGet(int child,long addr,long *str,int len)
{
	int i = 0;

	for(i=0;i < len;i++)
		str[i] = ptrace(PTRACE_PEEKDATA,child,addr + i * WORD_SIZE,NULL);
}

void MemInject(int child,long addr,void *vptr,int len)
{
	int count = 0;
	long word;

	while (count < len)
	{
		memcpy(&word , vptr+count , WORD_SIZE);
		word = ptrace(PTRACE_POKETEXT, child , addr+count , word);
		count += WORD_SIZE;
	}
}

/*
void
MemInject(int pid,struct user_regs_struct regs,int* shellcode,int size)
{
	int i;
	
	for (i = 0; i < size; i++)
		ptrace (PTRACE_POKETEXT, pid, regs.eip + i, *(int*) (shellcode + i));   
}
*/
