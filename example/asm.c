#include "hook.h"

INIT_HOOK;

HOOK_ASM(hook_asm);

asm("jmp	str");
//asm("push	$0x0a6c6f6c");
asm("print:");
asm("movb	$0x4,%al");
asm("movb	$0x1,%bl");
asm("pop	%esi");
//asm("movl	%esp,%ecx");
asm("movl	%esi,%ecx");
asm("movb	$0x4,%dl");
asm("int	$0x80");
asm("movb	$0x1,%al");
asm("xorb	%bl,%bl");
asm("int	$0x80");

asm("str:");
asm("call	print");
asm(".string	\"LOL\"");

END_HOOK;

HOOK_SIZE;

HOOK(hook_asm);
