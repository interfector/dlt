#include <stdio.h>
#include <stdlib.h>
#include "hook.h"

HOOKFUNC(hook_test)
{
	printf("Test 1!\n");

	exit(0);
}

int hook_size()
{
	return 30;
}

HOOK(hook_test);
