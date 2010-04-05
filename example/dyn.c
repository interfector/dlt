#include "hook.h"

#define ADDR "0xb80b9000" /* ADDRESS TO CHANGE */

INIT_HOOK;

HOOK_ASM(hook_dyn);
/*
asm("movl	$"ADDR",%eax");
asm("movl	$0x0a,(%eax)");
*/


/* Print to check */
asm("movb	$0x4,%al");
asm("movb	$0x1,%bl");
asm("movl	$"ADDR",%ecx");
asm("movb $4,%dl");
asm("int	$0x80");
/* Exits */
asm("movb	$0x1,%al");
asm("xorb	%bl,%bl");
asm("int	$0x80");

END_HOOK;

HOOK_SIZE;

HOOK(hook_dyn);
