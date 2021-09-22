# Assembler
## Index
<a src=#hi> [hello](#hi) </a>

## Overview
The final project for my university C lab programming course.

Receives an assembly file through the command line interface, and outputs an object file and if needed ext and ent files.

Made on ubuntu x64, ran successfully on ubuntu x32 and windows x64.

## Building the program

Included in the project are source files and a makefile.

To build on Linux simply run the command "make" in the kernel while in the project directory.

For windows there are a couple of options, see [this stack overflow thread](https://stackoverflow.com/questions/2532234/how-to-run-a-makefile-in-windows). 
The project was ran successfully on windows using minGW.

## Running the program

Included in the project are sample assembly (.asm) files with expected output in a subfolder.

### Example run:

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

### Creating an assembly file

If you wish to create your own file to run, here is a table of known commands:

//TODO

<span id=hi> 
    <h2> Overview </h2>
</span> 
