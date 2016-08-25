// Rudimentary Reader for Compiled Files!
// argv[1] == filename
// argv[2] == what to do?
// - argv[2] == "-print" is print hex and ascii side-by-side
// - argv[2] == "-parse" is print hex and ascii side-by-side
// argv[3] == debug? "-d"
// I'm probably going to regret this troll commit later ayy

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileformat/pe.h"
#include "fileformat/elf.h"

void printHexWithAscii(FILE* fp, long fileSize, unsigned int width);
void parse(FILE* fp, long fileSize);

int main(int argc, char* argv[]) {
    FILE* fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        printf("File not found.\n");
        return -1;
    }

    printf("Reading file...\n\n");

    fseek(fp, SEEK_SET, 0);
    long fileSize = ftell(fp);
    if (fseek(fp, 0l, SEEK_SET) != 0) {
        printf("Error seeking back to zero (main).\n");
    }

    if (argv[2]) {
        unsigned int width = 16; // TODO: Parameterize this
        if (!strcmp(argv[2], "-print")) {
            printHexWithAscii(fp, fileSize, width);
        } else if (!strcmp(argv[2], "-parse")) {
            parse(fp, fileSize);
        }
    }

    printf("\nFile read.\n");
    return 0;
}

void printHexWithAscii(FILE* fp, long fileSize, unsigned int width) {
    printf("Offset\t 00 01 02 03  04 05 06 07  08 09 0A 0B  0C 0D 0E 0F\n");
    unsigned int dataRead = 0;

    int endOfFile = 0;
    while (!endOfFile) {
        unsigned char* buffer = (unsigned char*)malloc(width);
        if (fread(buffer, sizeof(unsigned char), width, fp) < width) {
            endOfFile = 1;
        }

        printf("%08X ", dataRead*width);
        int i;
        for (i = 0; i < width; i++) {
            printf("%02X ", buffer[i]);
            if (i==3 || i==7 || i==11) printf(" ");
        }
        printf(" ");

        for (i = 0; i < width; i++) {
            if (buffer[i] < 16) {
                printf(".");
            } else {
                printf("%c", buffer[i]);
            }
        }

        dataRead++;

        //if (dataRead == 128) break;

        printf("\n");
        free(buffer);
    }

    printf("\nFile read.\n");
}


void parse(FILE* fp, long fileSize) {
    unsigned char* buffer = (unsigned char*)malloc(0x06);
    fread(buffer, sizeof(unsigned char), 0x06, fp);

    if (buffer[0] == 'M' && buffer[1] == 'Z') {
        if (fseek(fp, 0l, SEEK_SET) != 0) {
            printf("Error seeking back to zero (parse).\n");
        }
        free(buffer);
        printf("MS-DOS header detected, decompiling Win32 program.\n");
        parseWin32(fp, fileSize);
    } else if (buffer[0] == 0x7F && buffer[1] == 'E' &&
        buffer[2] == 'L' && buffer[3] == 'F') {
        if (fseek(fp, 0l, SEEK_SET) != 0) {
            printf("Error seeking back to zero (parse).\n");
        }
        free(buffer);
        printf("ELF header detected, decompiling ELF program.\n");
        parseElf(fp, fileSize);
    } else {
        free(buffer);
        printf("Could not detect type of executable. Is this binary?\n");
    }
}
