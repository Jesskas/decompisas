// This is strictly for x86, 32-bit
// For x86_64, changes will need to be made, for example,
// this does not use 64-bit registers like rax.

#ifndef X86_H
#define X86_H

// x86.h
#include <stdio.h>
#include <stdint.h>

#define EAX 0x0 // AX/AL 000
#define ECX 0x1 // CX/CL 001
#define EDX 0x2 // DX/DL 010
#define EBX 0x3 // BX/BL 011
#define ESP 0x4 // SP/AH 100
#define EBP 0x5 // BP/CH 101
#define ESI 0x6 // SI/DH 110
#define EDI 0x7 // DI/BH 111
extern const char* reglist[8];
extern const char* reglist8l[8]; // specific list of the lower 8-bit variations
extern const char* reglist8h[8]; // specific list of the higher 8-bit variations
extern const char* reglist16[8]; // specific list of the 16-bit variations

// Addressing modes for Mod-Reg-R/M
#define INDIR_ADDR         0x00 // 0b00000000
#define INDIR_ADDR8        0x40 // 0b01000000 8-bit displacement added to val
#define INDIR_ADDR32       0x80 // 0b10000000 32-bit "
#define DIRECT_ADDR        0xC0 // 0b11000000

struct Instruction {
    uint32_t byteCounter;
    uint32_t prgmCounter;
    uint32_t relativeOff;         // offset for relative literals
    uint8_t numInstrBytes;        // num. of bytes in uint8_t instructionBytes[]
    uint8_t instructionBytes[64]; // 64 is overkill, but that allows worst cases

    uint8_t instructionPrefix[4]; // 1-4 byte instruction prefix
    uint8_t opcode;
    uint8_t opcode2;              // for two-byte opcodes
    uint8_t modRegRm;
    uint8_t scaleIndexBase;
    uint8_t disp_8;               // optional
    uint32_t disp_32;              // optional TODO: Handle 32/64bit
    uint8_t imm_8;                // optional
    uint16_t imm_16;              // optional
    uint32_t imm_32;              // optional TODO: Handle 32/64bit
    uint8_t rel_8;
    uint16_t rel_16;
    uint32_t rel_32;
};

void disassemble_x86(char* name, int RVA, const unsigned char* code,
    unsigned int codeSize);

// debug = print the bytes as well
void printInstruction(struct Instruction instr, int debug);

int parsePrefix(unsigned char byte);

#endif
