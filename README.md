# TypedCygni

This is a simple compiler with runtime virtual machine, written in C++11.
The compiler is able to make type check and type inference, then generates the bytecodes for the virtual machine to execute.

## Example Code

```
module Program
{
	def Factorial(n: Int): Int
	{
		if (n == 0)
		{
			return 1
		}
		else
		{
			return n * Factorial(n - 1)
		}
	}

	def Add(x: Double, y: Double): Double
	{
		return x + y
	}
	
	def Multiply(x: Int, y: Int): Int
	{
        return x * y
    }
    
    def Fibonacci(n: Int): Int
    {
        if (n == 0)
        {
            return n
        }
        else if (n == 1)
        {
            return n
        }
        else
        {
            return Fibonacci(n - 1) + Fibonacci(n - 2)
        }
    }

    def Sum(n: Int): Int
    {
        var i = 1
        var sum = 0
        while (i <= n)
		{
			sum = sum + i
			i = i + 1
		}
		return sum
	}
	
	def Main(): Int
	{
		return Sum(100)
	}
}
```

Compiles to byte code:

module Program
- name = Program
- index = 0
- fields count = 0
- methods count = 1
- Constant Pool:
	1. Int: 0
	2. Int: 1
	3. Int: 1
- Method Factorial
	- flags: ACCESS_PUBLIC
	- code:
		locals = 1, args_size = 1
		
		0: PUSH_STACK_INT 0
		
		3: PUSH_CONSTANT_INT 0
		
		6: EQ_INT
		
		7: JUMP_IF_FALSE 14
		
		10: PUSH_CONSTANT_INT 1
		
		13: RETURN_INT
		
		14: PUSH_STACK_INT 0
		
		17: PUSH_STACK_INT 0
		
		20: PUSH_CONSTANT_INT 2
		
		23: SUB_INT
		
		24: INVOKE 0 0
		
		29: MUL_INT
		
		30: RETURN_INT
		
