#include "assembler.h"

/*  main assembles all files given in the command line input  */
int main(int argc, char* argv[]) {
    static int crntSource;
    
    /*  for each file passed in the command prompt, we call the assembleFile routine    */
    for(crntSource = 1; crntSource < argc; crntSource++) {
        assembleFile(argv[crntSource], crntSource);
    }

    return 0;
}