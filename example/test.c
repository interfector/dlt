#include "hook.h"

INIT_HOOK;

HOOK_ASM(hook_test);

asm("movb	$0x4,%al");
asm("movb	$0x1,%bl");
asm("movl	$0x8049500,%ecx");
asm("movb $163,%dl");
asm("int	$0x80");

asm("exit:");
asm("movb	$0x1,%al");
asm("xorb	%bl,%bl");
asm("int	$0x80");

END_HOOK;

HOOK_SIZE;

HOOK(hook_test);
