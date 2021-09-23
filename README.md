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

The assembly language used in here was created specifically for the assembler project and is not meant to be linked or run.

Made on ubuntu x64, tested successfully on ubuntu x32 and windows 10 x64.

<hr/>
<span id=a2> <h2> Building </h2> </span> 

Included in the project are source files and a makefile.

To build on Linux simply run the command "make" in the kernel while in the project directory.

For windows there are a couple of options, see [this stack overflow thread](https://stackoverflow.com/questions/2532234/how-to-run-a-makefile-in-windows). 
The project was ran successfully on windows using minGW.

After runnning make, two new folders, "build" and "bin" will appear, the "build" folder includes all object files, and the "bin" folder includes the executable.

To delete all object files you can run the command "make clean"
in the command line prompt. This is a must if wanting to re-make, as the existence of object files will confuse the make.

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

Operands used in the language: 
* Registers ("Reg" in the param list), all of whom are presumed to have 32-bit width. There are 32 available registers (0 - 31), and are denoted with a '$', ie: "$1", "$23", etc.
* Immediate Integers ("Int_X" in the param list), the assumed size of the param is denoted in buts by the X in the type.
* Lblels ("Lbl" in the param list), whom can be user defiened or external. label names must begin with a letter and contain only alpha-numerical characters. Lblels can be no longer than 31 characters. Lblels are unique and can't be redefined. Any label name definition must be followed immediatly by a colon, followed by any legal operation or directive and its parameters. for example: _label-name_**:** *command* *param-list*
*  Strings ("Str" in the param list), denoted by intial and terminating quotation marks, with free text contained betwen. 

Types of statements available in the language:
* Operations, which are encoded instructions of code given to the machine to execute at run-time. The operation can optionally begin with a label definition.
* Directives, which instruct the assembler what is must do in regards to the code in the assembly file, creating relevant memory and file assignments before run. The operation can optionally begin with a label definition.
* Both of the above statements share the syntax: [_label_:] *command-name* *param1*, *param2*, ...
* Comments, which can only be written as a whole line (not from the middle), and begin with the character ';'. Example: ;this is a comment
* All line statements may not exceed 80 characters.

### Instruction sets

| Ops    | Parameters    | Function   | Example |
|--------------|-----------|------------|------------|
| add | Reg *lhs*, Reg *rhs*, Reg *dest* | Sums the content of *lhs* and *rhs* and stores in *dest* | add $1, $2, $3 |
| sub | Reg *lhs*, Reg *rhs*, Reg *dest* | Subtracts the content of *rhs* from *lhs* and stores in *dest* | sub $1, $2, $3 |
| and | Reg *lhs*, Reg *rhs*, Reg *dest* | Perform logical AND between *lhs* and *rhs*, and store result in *dest* | and $1, $2, $3 |
| or  | Reg *lhs*, Reg *rhs*, Reg *dest* | Perform logical OR between *lhs* and *rhs*, and store result in *dest* | or $1, $2, $3 |
| nor | Reg *lhs*, Reg *rhs*, Reg *dest* | Perform logical NOR between *lhs* and *rhs*, and store result in *dest* | nor $1, $2, $3 |
| move | Reg *src*, Reg *dest* | Replace the content of *dest* with the content of *src* | move $1, $2 |
| mvhi | Reg *src*, Reg *dest* | Replace the content of *dest*'s lowest bits (0-15) with the content of *src*'s highest bits | mvhi $1, $2 |
| mvlo | Reg *src*, Reg *dest* | Replace the content of *dest*'s highest bits (16-31) with the content of *src*'s lowest bits | mvlo $1, $2 |
| addi | Reg *lhs*, Int_16 *rhs*, Reg *dest* | See _add_, difference in *rhs* operand being literal integer | addi $1, 100, $3 |
| subi | Reg *lhs*, Int_16 *rhs*, Reg *dest* | See _sub_, difference in *rhs* operand being literal integer | subi $1, 100, $3 |
| andi | Reg *lhs*, Int_16 *rhs*, Reg *dest* | See _and_, difference in *rhs* operand being literal integer | andi $1, 100, $3 |
| ori | Reg *lhs*, Int_16 *rhs*, Reg *dest* | See _or_, difference in *rhs* operand being literal integer | ori $1, 100, $3 |
| nori | Reg *lhs*, Int_16 *rhs*, Reg *dest* | See _nor_, difference in *rhs* operand being literal integer | nori $1, 100, $3 |
| bne | Reg *lhs*, Reg *rhs*, Lbl *dest* | Compares the content of *lhs* and *rhs*, if content is equal jump to *dest* | bne $1, $2, MAIN |
| beq | Reg *lhs*, Reg *rhs*, Lbl *dest* | Compares the content of *lhs* and *rhs*, if content is not equal jump to *dest*  | beg $1, $2, MAIN |
| blt | Reg *lhs*, Reg *rhs*, Lbl *dest* | Compares the content of *lhs* and *rhs*, if *lhs*'s content is smaller than *rhs*'s, jump to *dest* | blt $1, $2, MAIN |
| bgt | Reg *lhs*, Reg *rhs*, Lbl *dest* | Compares the content of *lhs* and *rhs*, if *lhs*'s content is larger than *rhs*'s, jump to *dest* | bgt $1, $2, MAIN |
| lb | Reg *lhs*, Int_16 *rhs*, Reg *dest* | Sum the content of *lhs* register and *rhs* integer, and access the memory at the address indicated by sum, storing a byte of memory in *dest* | lb $1, 100, $2 |
| sb | Reg *lhs*, Int_16 *rhs*, Reg *dest* | Sum the content of *lhs* register and *rhs* integer, and access the memory at the address indicated by sum, storing in memory a byte's-worth of the lowest bits from *dest* | sb $1, 100, $2 |
| lw | Reg *lhs*, Int_16 *rhs*, Reg *dest* | Sum the content of *lhs* register and *rhs* integer, and access the memory at the address indicated by sum, storing four bytes byte of memory in *dest* | lw $1, 100, $2 |
| sw | Reg *lhs*, Int_16 *rhs*, Reg *dest* | Sum the content of *lhs* register and *rhs* integer, and access the memory at the address indicated by sum, storing in memory four bytes'-worth of the lowest bits from *dest* | sw $1, 100, $2 |
| lh | Reg *lhs*, Int_16 *rhs*, Reg *dest* | Sum the content of *lhs* register and *rhs* integer, and access the memory at the address indicated by sum, storing two bytes of memory in *dest* | lh $1, 100, $2 |
| sh | Reg *lhs*, Int_16 *rhs*, Reg *dest* | Sum the content of *lhs* register and *rhs* integer, and access the memory at the address indicated by sum, storing in memory two bytes'-worth of the lowest bits from *dest* | sh $1, 100, $2 |
| jmp | Lbl *dest*  | Jump to the address of *dest* | jmp MAIN |
| jmp | Reg *dest* | Jump to the address stored in register *dest* | jmp $1 |
| la | Lbl *src* | Copies to register $0 the address of the var *src* | la MAIN |
| call | Lbl *dest* | Jump to the address of *dest*, and stores the address of the operation following the call in register $0 | call MAIN |
| stop | NONE | Stops the program | stop

| Dirs    | Parameters    | Function   | Example |
|--------------|-----------|------------|------------|
| .db | Int_8 *num1*, *num2*, ... | Stores each num in the param list in memory in 8 bit-width space | .db 45, 67, 3, 0 |
| .dw | Int_32 *num1*, *num2*, ... | Stores each num in the param list in memory in 32 bit-width space | .dw 4598765, 6700, 3, 0 |
| .dh | Int_16 *num1*, *num2*, ... | Stores each num in the param list in memory in 16 bit-width space | .dh 450, 6786, 8 |
| .asciz | Str " *free text* " | Stores each letter in memory including terminating char | .asciz "hello world" |
| .entry | Lbl *arg* | Denotes arg to be an entry point to the program for external code to use, entry var must be declared in the assembly file | .entry MAIN |
| .extern | Lbl *arg* | Denotes arg to be external, declared outside the file scope | .extern VAR |