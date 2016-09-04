#ifndef ARM_H
#define ARM_H

// arm.h
#include <stdio.h>
#include <stdint.h>

uint8_t Instruction[4];

void disassemble_arm(char* name, int RVA, const unsigned char* code,
    unsigned int codeSize);

// debug = print the bytes as well
void printARMInstruction(uint8_t* instruction, int debug);

#endif ARM_H
