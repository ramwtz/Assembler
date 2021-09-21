#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "file_manager.h"

/***    struct definition   ***/

struct FileStruct{
    FILE *filePointer;
    char fileName[FILENAME_MAX];
};

struct OutputFiles{
    FileStrctP obfsp;
    FileStrctP extfsp;
    FileStrctP entfsp;
};

/***    creators    ***/

/*  allocate outputFiles struct and then create all the possible output files   */
OutFilesP createOutputFiles(char* inputFileName) {
    OutFilesP ofp = (OutFilesP) calloc(1, sizeof(struct OutputFiles));
    if(!ofp) {
        printf("%s", "Unable to allocate");
        exit(EXIT_FAILURE);
    }
    ofp->obfsp = createFileStruct(inputFileName, "ob");
    ofp->extfsp = createFileStruct(inputFileName, "ext");
    ofp->entfsp = createFileStruct(inputFileName, "ent");

    return ofp;
}

/*  create a fileStruct, which includes a file pointer and the files name   */
FileStrctP createFileStruct(char* inputFileName, const char* extension) {
    struct FileStruct *fsp = (struct FileStruct *) calloc(1, sizeof(struct FileStruct));
    if(!fsp) {
        printf("%s", "Unable to allocate");
        exit(EXIT_FAILURE);
    }
    memset(fsp->fileName, 0, FILENAME_MAX);
    createFileName(fsp->fileName, inputFileName, extension);
    fsp->filePointer = createFile(fsp->fileName);

    return fsp;
}

void createFileName(char* dest, char* source, const char* fileExt) {
    strncpy(dest, source, strlen(source) - LENGTH_OF_SOURCE_EXTENSION);
    strcat(dest, fileExt);
}

FILE* createFile(char* fileName) {
    FILE *fp;

    fp = fopen(fileName, "w");
    if(fp == NULL){
       perror("unable to open\n");
        exit(EXIT_FAILURE);
    }

    return fp;
}

/***    getters     ***/

FILE *getFilePointer(FileStrctP fp) {
    return fp->filePointer;
}

char *getFileName(FileStrctP fp) {
    return fp->fileName;
}

FileStrctP getObFile(OutFilesP ofp) {
    return ofp->obfsp;
}

FileStrctP getExtFile(OutFilesP ofp) {
    return ofp->extfsp;
}

FileStrctP getEntFile(OutFilesP ofp) {
    return ofp->entfsp;
}

/***    print    ***/

/*  iteratively prints to file the hexadiceimal representation of each byte in the array, using
    the specified stylization   */
void printToObFile(FILE* obfp, byte* byteArr, int byteArrSize, int crntAdress) {
    int i, crntByte;
    
    for(i = 0; i < byteArrSize; i++) {
        crntByte = byteArr[i];
        if((i % TOTAL_BYTES_IN_ROW) == FIRST_BYTE_IN_ROW)
            fprintf(obfp, "%04d %02X ", crntAdress + i, crntByte);
        else if((i % TOTAL_BYTES_IN_ROW) == LAST_BYTE_IN_ROW)
            fprintf(obfp, "%02X\n", crntByte);
        else
            fprintf(obfp, "%02X ", crntByte);
    }
}

/*  prints to ext file each reference to an ext in the source code  */
void addReferenceToExt(FILE* extfp, char* str, int adrs) {
    fprintf(extfp, "%s %04d\n", str, adrs);
}

/*  adds any label defined in the source as entry to the ent output file   */
void addLabelToEnt(FILE *entfp, char* label, int address) {
    fprintf(entfp, "%s %04d\n", label, address);
}

/***    queries     ***/

/*  moves cursor to end of file and returns size, returns true if empty, false otherwise    */
boolean isFileEmpty(FILE* fp) {
    int size = 0;

    fseek(fp, -1, SEEK_END);
    size = ftell(fp);

    return !size;
}

/*  iterates over the row until finding the end, saving the number of chars parsed,
    and moving the cursor to the begining of the row, finally returning the length  */
int rowLength(FILE* fp) {
    int c, count = 0;

    while(TRUE) {
        c = fgetc(fp);
        if(c == '\n') {
            count++;
            break;
        }
        if(c == EOF)
            break;
        count++;
    }

    fseek(fp, -count, SEEK_CUR);
    
    return count;
}   

/***    Error Handling      ***/

Error checkInputFileName(char* fileName) {
    int length = strlen(fileName);
    char* tempP = fileName;
    tempP += length - 3;
    
    if(strcmp(tempP, ".as"))
        return er_illegal_file_type;
    return er_no_error;
}

Error checkInputFileContent(FILE* fp) {
    if(isFileEmpty(fp))
        return er_file_is_empty;
    return er_no_error;
}

/***    free, close, & delete    ***/

void removeExportFileIfEmpty(FileStrctP fsp) {
    int check = 0;
    char* fileName = getFileName(fsp);

    if(isFileEmpty(getFilePointer(fsp))) {
        if((check = remove(fileName)))
            printf("Unable to remove %s\n", fileName);
    }
}

void freeFileStrct(FileStrctP fsp) {
    fclose(fsp->filePointer);
    free(fsp);
}

void closeOutputFiles(OutFilesP ofp) {
    freeFileStrct(ofp->entfsp);
    freeFileStrct(ofp->extfsp);
    freeFileStrct(ofp->obfsp);
    free(ofp);
}