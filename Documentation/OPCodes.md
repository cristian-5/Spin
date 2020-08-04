
# List of OPCodes of the Virtual Processor.

## The Virtual Processor (VM):

The **Virtual Processor** is capable of executing 16 bytes instructions,
where 8 bytes are reserved for the **OPCode** plus some padding, and the
remaining 8 bytes are reserved for an eventual argument.
The strings should be stored in a separate string table, that can be
accessed using the **STR** instruction.
When an operation raises an error, a dictionary of indexes and errors
is consulted to find what exception to raise.

### Argument types:

- **value**: An 8 byte value. It can be a basic type or a pointer.
- **index**: A numeric 8 byte unsigned integer.
-  **type**: A value representing a type (1 byte), fit in 8 bytes.
-  *types**: A value representing two types (2 bytes), fit in 8 bytes.

| OPCode | Argument | Description                                                             |
|:------:|:--------:|-------------------------------------------------------------------------|
|   RST  |          | Rest for a cycle                                                        |
|   PSH  |   value  | Push a value onto the stack                                             |
|   STR  |   index  | Push the string at index position (string table) onto the stack         |
|   GET  |   index  | Get the value (at position index) and push it                           |
|   SET  |   index  | Set the value (at position index) with the value at stack top           |
|   SSF  |   index  | Set the stack frame to start at current stack position - index          |
|   GLF  |   index  | Get the value (at position index in the current frame) and push it      |
|   SLF  |   index  | Set the value (at position index) with the value at stack top           |
|   CTP  |          | Copy the top of the stack in a temporary register, popping              |
|   LTP  |          | Load the top of the stack with the temporary register, pushing          |
|   SWP  |          | Swap the two values specified with two indexes on top of the stack      |
|   ADD  |   types  | Add the last two stack values (considering types)                       |
|   SUB  |   types  | Subtract the last two stack values (considering types)                  |
|   MUL  |   types  | Multiply the last two stack values (considering types)                  |
|   DIV  |   types  | Divide the last two stack values (considering types)                    |
|   MOD  |   types  | Get modulus of the last two stack values (considering types)            |
|   NEG  |   type   | Negation of the last stack value (considering type)                     |
|   INV  |   type   | Inversion of the last stack value (considering type)                    |
|   SSC  |   index  | String subscription of the last stack value (at position index)         |
|   CCJ  |          | Conjugate the complex number on top of the stack                        |
|   VCJ  |          | Conjugate the vector space on top of the stack                          |
|   MCJ  |          | Conjugate the matrix on top of the stack                                |
|   PST  |          | Push true on top of the stack                                           |
|   PSF  |          | Push false on top of the stack                                          |
|   PSI  |          | Push infinity on top of the stack                                       |
|   PSU  |          | Push undefined on top of the stack                                      |
|   PEC  |          | Push an empty complex (0 + 0i) on top of the stack                      |
|   PES  |          | Push the empty string on top of the stack                               |
|   POP  |          | Pop a value off the stack                                               |
|   DSK  |   index  | Decrease the stack (popping) n values (n being index)                   |
|   JMP  |   index  | Relative forward jump by n instructions (n being index)                 |
|   JMB  |   index  | Relative backward jump by n instructions (n being index)                |
|   JIF  |   index  | Relative forward jump (if false is on top of the stack)                 |
|   JAF  |   index  | Relative forward jump (if false is on top of the stack), no pop         |
|   JIT  |   index  | Relative forward jump (if true is on top of the stack)                  |
|   JAT  |   index  | Relative forward jump (if true is on top of the stack), no pop          |
|   EQL  |   types  | Equal comparison of the last two stack values                           |
|   NEQ  |   types  | Not equal comparison of the last two stack values                       |
|   GRT  |   types  | Great comparison of the last two stack values                           |
|   LSS  |   types  | Less comparison of the last two stack values                            |
|   GEQ  |   types  | Great equal comparison of the last two stack values                     |
|   LEQ  |   types  | Less equal comparison of the last two stack values                      |
|   NOT  |          | Not operation performed on the last Boolean on the stack                |
|   BWA  |   types  | Bitwise and, of the last two values of the stack (considering types)    |
|   BWO  |   types  | Bitwise or, of the last two values of the stack (considering types)     |
|   BWX  |   types  | Bitwise xor, of the last two values of the stack (considering types)    |
|   CAL  |   index  | Absolute jump at address index, with preservation of ip                 |
|   RET  |          | Return to the preserved ip, stored with CAL, decrease stack frame       |
|   CST  |   types  | Casting between the last two values on the stack (considering types)    |
|   PRT  |   type   | Print the top of the stack (considering type), popping                  |
|   PRL  |   type   | Print with line ending the top of the stack (considering type), popping |
|   NLN  |          | Print new line                                                          |
|   HLT  |          | Halt program                                                            |

## Binary operations on the stack:

Every binary operation performed on the stack, pops two values off the stack,
executes the operation (considering the given types) and pushes the result back in.

## Unary operations on the stack:

Every unary operation performed on the stack, pops a value off the stack,
executes the operation (considering the given type) and pushes the result back in.

## Special operations:

The **JAF** and **JAT** operations do not pop the corresponding Boolean value
off the stack. They can be used for short circuit operations like *and* and *or*.

The **SET** and **SLF** instructions do not pop the last value of the stack.
This beacause the user might need to execute consecutive assignations.

## Routines' calls, frames and returns:

The current stack frame is located in the **base** register. When the user needs
to call a routine he pushes the parameters onto the stack, then performs an
**SSF** (set stack frame) operation, that always requires the number of parameters
of the routine as argument, in such a way that the stack frame will be set, including
those parameters. The user must then call the routine with the *CAL* instruction,
specifing the absolute address of the first instruction of the routine (generally
located at the end of the program). This operation will store the *ip* (instruction
pointer) on top of the call stack (which is inaccessible for the user).\
Before returning the user must always clear the variables and the parameters allocated
on the stack until that point. When **RET** is hit, the processor will restore the stack
frame, pop the last call *ip* and jump back.
