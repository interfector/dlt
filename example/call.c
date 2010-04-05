#include "hook.h"

INIT_HOOK;

HOOK_ASM(hook_call);
/* Function call */
asm("movl	$0x080489c0,%eax");
asm("call	*%eax");
/* Exits */
asm("movb	$0x1,%al");
asm("xorb	%bl,%bl");
asm("int	$0x80");

END_HOOK;

HOOK_SIZE;

HOOK(hook_call);
