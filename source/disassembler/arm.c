#include "arm.h"

void disassemble_ARM(char* name, int RVA, const unsigned char* code,
    unsigned int codeSize)
{
    unsigned int byteCounter = 0;
    unsigned int prgmCounter = 0;

    while (prgmCounter < 60 && byteCounter < codeSize) {
        printf("%s:0x%08X | ", name, byteCounter + RVA);
        uint8_t instr[4] = {
            code[byteCounter+0], code[byteCounter+1],
            code[byteCounter+2], code[byteCounter+3]
        };
        byteCounter += 4;

        printARMInstruction(instr, 1);

        printf("\n");
        prgmCounter++;
    }
    printf("\n");
}

// debug = print the bytes as well
void printARMInstruction(uint8_t* instruction, int debug)
{
    printf("%02X %02X %02X %02X | ",  instruction[0], instruction[1],
                                      instruction[2], instruction[3]);

}
