# Types

## Basic Types

Int, Float, Long, Double, Boolean, Char, String

```
Int + Int -> Int
Float + Float -> Float
Long + Long -> Long
Double + Double -> Double

Int == Int -> Boolean
Float == Float -> Boolean
Long == Long -> Boolean
Double == Double -> Boolean
Boolean == Boolean -> Boolean
Char == Char -> Boolean
String == String -> Boolean
```

## Built-in Functions

```
Length("abc") -> 3
Length(Array[Int](1, 2, 3)) -> 3
As[Int](3.2) -> 3
Is[Int](3.2) -> false
```

## Object-Oriented System

```
class Base
{
    virtual def F(): Void
    {

    }
}

interface I1
{

}

interface I2
{

}

interface I3
{

}

class Inherited <: Base, I1, I2, I3
{
    override def F(): Void
    {

    }
}
```

## Types & Values

### Type

- `BasicType`
    - tag
- `FunctionType`
    - parameters
    - returnType
- `IdentifierType`
    - identifier
- `ArrayType`
    - elementType


### Value

- `BasicValue`
    - tag
- `ObjectValue`
    - unordered_map<string, Value*> fields
- `ArrayValue`
    - Value* elementValue
- `FunctionValue`
    - vector<Value*> parameters
    - Value* returnType
    - ObjectValue* self