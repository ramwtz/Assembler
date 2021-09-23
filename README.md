# Assembler

<h2> Index </h2>

* [Overview](#a1)
* [Building](#a2)
* [Running](#a3)
* [Assembly Language](#a4)

<hr/>
<span id=a1> <h2> Overview </h2> </span> 

The final project for my university programming lab in C course.

Receives an assembly file through the command line interface, and outputs an object file and if needed ext and ent files.

The assembly language used in here was created specifically for the assembler project and is not ment to be linked or run.

Made on ubuntu x64, tested successfully on ubuntu x32 and windows x64.

<hr/>
<span id=a2> <h2> Building </h2> </span> 

Included in the project are source files and a makefile.

To build on Linux simply run the command "make" in the kernel while in the project directory.

For windows there are a couple of options, see [this stack overflow thread](https://stackoverflow.com/questions/2532234/how-to-run-a-makefile-in-windows). 
The project was ran successfully on windows using minGW.

<hr/>
<span id=a3> <h2> Running </h2> </span> 

Included in the project are sample assembly (.asm) files with expected output in a subfolder.

### Example:

#### sample1.asm

![Untitled](https://user-images.githubusercontent.com/83758958/134347802-7926e491-15bb-4121-950b-a1400d27299e.png)

After running *program_path*/Assembler    *sample_path*/sample1.asm,
You'll receive the following three files in the same directory as the sample1.asm file:

#### sample1.ob

![output](https://user-images.githubusercontent.com/83758958/134348457-f95ae0d9-fcca-4d39-aa51-8efbfabfb075.png)

#### sample1.ext

![output2](https://user-images.githubusercontent.com/83758958/134348825-6a3c6f0a-669c-4754-bfdb-e48555055920.png)

#### sample1.ent

![output3](https://user-images.githubusercontent.com/83758958/134348891-44627727-3e04-49a7-a99a-7357e7a52d6d.png)

<hr/>
<span id=a4> <h2> Assembly Language </h2> </span> 

Literal data types used in the language: 
* Registers ("Reg" in the param list), all of whom are presumed to have 32-bit width. There are 32 available registers (0 - 31), and are denoted with a '$', ie: "$1", "$23", etc.
* Immediate Integers ("Int_X" in the param list), the assumed size of the param is denoted in buts by the X in the type.
*  Variables ("Var" in the param list), whom can be user defiened labels or external variables. variable names must begin with a letter and contain only alpha-numerical characters. Labels can be no longer than 31 characters. Labels are unique and can't be redefined. Any label name definition must be followed immediatly by a colon, followed by any legal operation or directive and its parameters. for example: _label-name_**:** *command* *param-list*
*  Strings ("Str" in the param list), denoted by intial and terminating quotation marks, with free text contained betwen. 

Types of line statements available in the language:
* Operations, which are encoded instructions of code given to the machine to execute at run-time. The operation can optionally begin with a label definition.
* Directives, which instruct the assembler what is must do in regards to the code in the assembly file, creating relevant memory and file assignments before run. The operation can optionally begin with a label definition.
* Both of the above statements share the syntax: [_label_:] *command-name* *param1*, *param2*, ...
* Comments, which can only be written as a whole line (not from the middle), and begin with the character ';'. Example: ;this is a comment
* All line statements may not exceed 80 characters.

### Instruction sets

| Ops    | Parameters    | Function   |
|--------------|-----------|------------|
| add | Reg left, Reg right, Reg dest | Sums the content of left and right and stores in dest |
| sub | Reg left, Reg right, Reg dest | Subtracts the content of right from left and stores in dest |
| and | Reg left, Reg right, Reg dest | Perform logical AND between left and right, and store result in dest |
| or  | Reg left, Reg right, Reg dest | Perform logical OR between left and right, and store result in dest |
| nor | Reg left, Reg right, Reg dest | Perform logical NOR between left and right, and store result in dest |
| move | Reg src, Reg dest | Replace the content of dest with the content of src |
| mvhi | Reg src, Reg dest | Replace the content of dest's lowest bits (0-15) with the content of src's highest bits |
| mvlo | Reg src, Reg dest | Replace the content of dest's highest bits (16-31) with the content of src's lowest bits |
| addi | Reg left, Int_16 right, Reg dest | See _add_, difference in right operand being literal integer |
| subi | Reg left, Int_16 right, Reg dest | See _sub_, difference in right operand being literal integer |
| andi | Reg left, Int_16 right, Reg dest | See _and_, difference in right operand being literal integer |
| ori | Reg left, Int_16 right, Reg dest | See _or_, difference in right operand being literal integer |
| nori | Reg left, Int_16 right, Reg dest | See _nor_, difference in right operand being literal integer |
| bne | Reg left, Reg right, Var dest | Compares the content of left and right, if content is equal jump to dest  |
| beq | Reg left, Reg right, Var dest | Compares the content of left and right, if content is not equal jump to dest  |
| blt | Reg left, Reg right, Var dest | Compares the content of left and right, if left's content is smaller than right's, jump to dest |
| bgt | Reg left, Reg right, Var dest | Compares the content of left and right, if left's content is larger than right's, jump to dest |
| lb | Reg left, Int_16 right, Reg dest | Sum the content of left register and right integer, and access the memory at the address indicated by sum, storing a byte of memory in dest |
| sb | Reg left, Int_16 right, Reg dest | Sum the content of left register and right integer, and access the memory at the address indicated by sum, storing in memory a byte's-worth of the lowest bits from dest |
| lw | Reg left, Int_16 right, Reg dest | Sum the content of left register and right integer, and access the memory at the address indicated by sum, storing four bytes byte of memory in dest |
| sw | Reg left, Int_16 right, Reg dest | Sum the content of left register and right integer, and access the memory at the address indicated by sum, storing in memory four bytes'-worth of the lowest bits from dest |
| lh | Reg left, Int_16 right, Reg dest | Sum the content of left register and right integer, and access the memory at the address indicated by sum, storing two bytes of memory in dest |
| sh | Reg left, Int_16 right, Reg dest | Sum the content of left register and right integer, and access the memory at the address indicated by sum, storing in memory two bytes'-worth of the lowest bits from dest |
| jmp | Var dest OR Reg dest | Jump to the address of the variable dest, or to the address stored in register dest |
| la | Var src | Copies to register $0 the address of the var src |
| call | Var dest | Jump to the address of the variable dest, and stores the address of the operation following the call in register $0 |
| stop | NONE | Stops the program |

| Dirs    | Parameters    | Function   |
|--------------|-----------|------------|
| .db | Int_8 num1, num1, ... | Stores each num in the param list in memory in 8 bit-width space |
| .dw | Int_32 num1, num1, ... | Stores each num in the param list in memory in 32 bit-width space |
| .dh | Int_16 num1, num1, ... | Stores each num in the param list in memory in 16 bit-width space |
| .asciz | Str "*free text*" | Stores each letter in memory including terminating char |
| .entry | Var arg | Denotes arg to be an entry point to the program for external code to use, entry var must be declared in the assembly file |
| .extern | Var arg | Denotes arg to be an external variable, declared outside the file scope |