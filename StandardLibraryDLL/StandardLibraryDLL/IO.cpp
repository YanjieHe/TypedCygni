#include "stdafx.h" // use stdafx.h in Visual Studio 2017 and earlier
#include "IO.h"
#include <stdio.h>

void print_line_int(Value * argv, Value* out)
{
	printf("%d\n", argv[0].u.i32_v);
}
