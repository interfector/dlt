#include "hook.h"

#define ADDR "0xb7ffd000" /* ADDRESS TO CHANGE */

INIT_HOOK;

HOOK_ASM(hook_lol);

asm("mov	$0xb7fe6000,%eax");
asm("movb	$0xa,(%eax)");
/*
asm("movb	$0x1,%al");
asm("xorb	%bl,%bl");
asm("int	$0x80");
*/
END_HOOK;

HOOK_SIZE;

HOOK(hook_lol);
