## Expressions
- `BinaryExpression`: `+-*/%`, `and`, `or`, `>`, `<`, `>=`, `<=`, `==`, `!=`
    - left
    - right
- `ConstantExpression`: integer32, float32, integer64, float64, boolean, character, string
    - value
- `UnaryExpression`: `+`, `-`, `not`
    - operand
- `InvocationExpression`
    - function
    - arguments
- `IdentifierExpression`
    - identifier
- `MemberExpression`
    - object
    - memberName
- `NewExpression`
    - className
    - initializers
- `IsExpression`
    - expression
    - type
- `AsExpression`
    - expression
    - type

## Statements
- `BlockStatement`
    - statements
- `IfThenStatement`
    - condition
    - ifTrue
- `IfElseStatement`
    - condition
    - ifTrue
    - ifFalse
- `WhileStatement`
    - condition
    - body
- `AssignStatement`
    - left
    - value
- `TryStatement`
    - body
    - handlers
    - finally
- `ReturnStatement`
    - value
- `BreakStatement`

## Sample Code

```
class Point
{
    private x: Double
    private y: Double

    def GetX(): Double
    {
        return this.x
    }

    def GetY(): Double
    {
        return this.x
    }
}

module Point
{
    def New(_x: Double, _y: Double): Point
    {
        return new Point
        {
            x = _x,
            y = _y
        }
    }
}
```