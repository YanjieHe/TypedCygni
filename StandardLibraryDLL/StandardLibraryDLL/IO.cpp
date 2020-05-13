#include "stdafx.h" // use stdafx.h in Visual Studio 2017 and earlier
#include "IO.h"
#include <stdio.h>

int print_line_int(Value * env)
{
	printf("%d\n", env[0].u.i32_v);
	return 0;
}