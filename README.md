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