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

#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>
#include <dlt.h>

int pread(int fd, void *buf, int count, unsigned int offset);
int sym_header(int,char*,char*,Elf32_Shdr*,char*);

char* hook_value;

int
ElfSym_read (char * file,char * sym)
{
	int i,fd;
	char *buf = NULL;
	struct stat elf_stat;
	Elf32_Ehdr *elf_header = NULL;
	Elf32_Shdr *section_hdr = NULL;
	int size;
        
	if ((fd = open(file,O_RDWR)) < 0)
		return -1;
	
	if( fstat( fd, &elf_stat ) < 0 )
		return -1;

	size = elf_stat.st_size;
        
	buf = (char*)malloc(sizeof(char)*size+1);

	if (read(fd,buf,size) != size)
		return -1;

	elf_header = (Elf32_Ehdr *)buf;

	section_hdr = (Elf32_Shdr *)(buf + elf_header->e_shoff);
     
	for(i = 0; i < elf_header->e_shnum; i++)
		if((size = sym_header(fd,buf,sym,&section_hdr[i],(char *)(buf + section_hdr[section_hdr[i].sh_link].sh_offset))) != -1)
			return size;

	close(fd);

	free(buf);
	return 0;
}

int
sym_header (int fd,char* buf,char* syms,Elf32_Shdr *shdr,char * tbl)
{
	Elf32_Sym * sym = (Elf32_Sym*)(buf + shdr->sh_offset);
	int sym_size = shdr->sh_entsize;
	int cur = 0;

	if (shdr->sh_type == SHT_DYNSYM || shdr->sh_type == SHT_SYMTAB)
	{
		do{
			if(!strcmp(tbl+sym->st_name,syms))
			{
				hook_value = malloc(sym->st_size);

				pread(fd,hook_value,sym->st_size,sym->st_value);

				return sym->st_size;
			}

			cur += sym_size;
			sym++;
		} while(cur < shdr->sh_size);

	}

	return -1;
}
