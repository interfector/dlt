#include "hook.h"
/*
asm(".LCLOL:");
asm(".string \"lol\"");
*/
INIT_HOOK;

HOOK_ASM(hook_asm);

asm(
    "push	$0x0a6c6f6c\n"
    "movb	$0x4,%al\n"
    "movb	$0x1,%bl\n"
    "movl	%esp,%ecx\n"
    "movb	$0x4,%dl\n"
    "int	$0x80\n"
    "movb	$0x1,%al\n"
    "xorb	%bl,%bl\n"
    "int	$0x80"
);

END_HOOK;

HOOK_SIZE;

HOOK(hook_asm);
