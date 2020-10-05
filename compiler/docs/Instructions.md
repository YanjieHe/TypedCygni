## Instructions

### Base Class

- Instruction
    - OpCode op
    - int length

### Subclasses of the Instruction Class

- ArithmeticInstruction

- CPInstruction: Constant Pool Intruction
    - uint16_t index

- PushConstantInstruction

- LoadInstruction: Load local variable to the stack
    - uint16_t index

- StoreInstruction: Store the value on the top of the stack to the local variable
    - uint16_t index

- InvokeInstruction: Invoke a function or a method
    - uint16_t cpIndex: constant pool index

- LoadMemberInstruction: Get a member of an object
    - uint16_t cpIndex: constant pool index

- StoreMemberInstruction: Get a member of an object
    - uint16_t cpIndex: constant pool index