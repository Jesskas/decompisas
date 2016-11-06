#include "x86.h"

// define extern declaration
const char* reglist[8] =
    { "eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi" };
const char* reglist8l[8] = // TODO: Confirm the latter four
    { "al", "cl", "dl", "bl", "ah", "ch", "dh", "bh" };
const char* reglist8h[8] = // TODO: Confirm the lattter four
    { "ah", "ch", "dh", "bh", "esp", "ebp", "esi", "edi" };
const char* reglist16[8] = // TODO: Confirm the lattter four
    { "ax", "cx", "dx", "bx", "esp", "ebp", "esi", "edi" };

// TODO: This assumes IA-32. Split into IA-32 and x64.
void disassemble_x86(char* name, int RVA, const unsigned char* code,
    unsigned int codeSize)
{
    unsigned int byteCounter = 0;
    unsigned int prgmCounter = 0;

    while (prgmCounter < 200 && byteCounter < codeSize) {
        printf("%s:0x%08X | ", name, byteCounter + RVA);
        int i = 0;  // for now... represents number of bytes in instruction
        struct Instruction instr = { 0 };

        uint8_t prefixByte = parsePrefix(code, byteCounter);
        while (prefixByte != 0) {
            instr.instructionBytes[i]   = prefixByte;
            instr.instructionPrefix[i]  = prefixByte;
            byteCounter++; i++;
            if (i == 4) break; // TODO: better error handling
            prefixByte = parsePrefix(code, byteCounter);
        }
        instr.byteCounter =         byteCounter;
        instr.prgmCounter =         prgmCounter;
        instr.relativeOff =         byteCounter + RVA;
        instr.opcode =              code[byteCounter];
        instr.instructionBytes[i++] = code[byteCounter];

// examples of two=byte opcodes, saved for later
//0f b6 00                movzx  eax,BYTE PTR [eax]
//0f b6 c0                movzx  eax,al
//0f af c2                imul   eax,edx

// TODO: Check whether SIB bytes are used in the simple early modRegRm's
// in each section; for example, 0x00. This doesn't check for SIB;
// but could one still exist?
// TODO: 16/32 addressing needs to be addressed, badly. specifically,
// the "BYTE/WORD/DWORD PTR" syntax.

        // Single-byte opcode
        if (code[byteCounter] != 0x0F) {
            switch (code[byteCounter]) {
                // ADD Opcodes (0b0000 0000)
                case 0x00: // add r/m8 r8
                    // but sometimes nop?
                    // 00 00 = add BYTE PTR [rax],al
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            byteCounter += 4;
                        }
                    }
                    byteCounter += 2; break;
                    // 00 03 = add BYTE PTR [rbx],al
                case 0x01: // add r/m16/32 r16/32
                    // 01 00 = add DWORD PTR [rax],eax
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            byteCounter += 4;
                        }
                    }
                    byteCounter += 2; break;
                case 0x02: // add r8 r/m8
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            byteCounter += 4;
                        }
                    }
                    byteCounter += 2; break;
                case 0x03: // add r16/32 r/m16/32
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            byteCounter += 4;
                        }
                    }
                    byteCounter += 2; break;
                case 0x04:
                    instr.imm_8               = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    byteCounter += 2; break;
                case 0x05:
                    instr.imm_32 = *(uint32_t*)&code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+2];
                    instr.instructionBytes[i++] = code[byteCounter+3];
                    instr.instructionBytes[i++] = code[byteCounter+4];
                    byteCounter += 5; break;
                case 0x06: // push es
                case 0x07: // pop es
                    byteCounter++; break;
                // OR Opcodes (0b0000 1000)
                case 0x08:
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            byteCounter += 4;
                        }
                    }
                    byteCounter += 2; break;
                case 0x09:
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            byteCounter += 4;
                        }
                    }
                    byteCounter += 2; break;
                case 0x0A:
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            byteCounter += 4;
                        }
                    }
                    byteCounter += 2; break;
                case 0x0B:
                    // 0B 00 = or eax,DWORD PTR [rax]
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            byteCounter += 4;
                        }
                    }
                    byteCounter += 2; break;
                case 0x0C:
                    instr.imm_8               = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    byteCounter += 2; break;
                case 0x0D:
                    instr.imm_32 = *(uint32_t*)&code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+2];
                    instr.instructionBytes[i++] = code[byteCounter+3];
                    instr.instructionBytes[i++] = code[byteCounter+4];
                    byteCounter += 5; break;
                case 0x0E: // PUSH CS
                    byteCounter++; break;
                // ADC Opcodes (0b0001 0000)
                case 0x10:
                    // 10 60 00 = adc BYTE PTR [ra]
                    // 10 00 = adc BYTE PTR [rax], alx+0x0],ah
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            byteCounter += 4;
                        }
                    }
                    byteCounter += 2; break;
                case 0x11:
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            byteCounter += 4;
                        }
                    }
                    byteCounter += 2; break;
                case 0x12:
                    // 12 00 = adc al,BYTE PTR [rax]
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            byteCounter += 4;
                        }
                    }
                    byteCounter += 2; break;
                case 0x13:
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            byteCounter += 4;
                        }
                    }
                    byteCounter += 2; break;
                case 0x14:
                    instr.imm_8               = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    byteCounter += 2; break;
                case 0x15:
                    instr.imm_32 = *(uint32_t*)&code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+2];
                    instr.instructionBytes[i++] = code[byteCounter+3];
                    instr.instructionBytes[i++] = code[byteCounter+4];
                    byteCounter += 5; break;
                case 0x16:
                case 0x17:
                    byteCounter++; break;
                // SBB Opcodes (0b0001 1000)
                case 0x18:
                    // 18 10 = sbb BYTE PTR [rax],dl
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            byteCounter += 4;
                        }
                    }
                    byteCounter += 2; break;
                case 0x19:
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            byteCounter += 4;
                        }
                    }
                    byteCounter += 2; break;
                case 0x1A:
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            byteCounter += 4;
                        }
                    }
                    byteCounter += 2; break;
                case 0x1B:
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            byteCounter += 4;
                        }
                    }
                    byteCounter += 2; break;
                case 0x1C:
                    instr.imm_8               = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    byteCounter += 2; break;
                case 0x1D:
                    instr.imm_32 = *(uint32_t*)&code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+2];
                    instr.instructionBytes[i++] = code[byteCounter+3];
                    instr.instructionBytes[i++] = code[byteCounter+4];
                    byteCounter += 5; break;
                case 0x1E: // PUSH DS
                case 0x1F: // POP DS
                    byteCounter++; break;
                // AND Opcodes (0b0010 0000)
                case 0x20:
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            byteCounter += 4;
                        }
                    }
                    byteCounter += 2; break;
                case 0x21:
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            byteCounter += 4;
                        }
                    }
                    byteCounter += 2; break;
                case 0x22:
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            byteCounter += 4;
                        }
                    }
                    byteCounter += 2; break;
                case 0x23:
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            byteCounter += 4;
                        }
                    }
                    byteCounter += 2; break;
                case 0x24:
                    instr.imm_8               = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    byteCounter += 2; break;
                case 0x25:
                    instr.imm_32 = *(uint32_t*)&code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+2];
                    instr.instructionBytes[i++] = code[byteCounter+3];
                    instr.instructionBytes[i++] = code[byteCounter+4];
                    byteCounter += 5; break;
                case 0x27: // DAA AL
                // SUB Opcodes (0b0010 1000)
                case 0x28:
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            byteCounter += 4;
                        }
                    }
                    byteCounter += 2; break;
                case 0x29:
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            byteCounter += 4;
                        }
                    }
                    byteCounter += 2; break;
                case 0x2A:
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            byteCounter += 4;
                        }
                    }
                    byteCounter += 2; break;
                case 0x2B:
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            byteCounter += 4;
                        }
                    }
                    byteCounter += 2; break;
                case 0x2C:
                    instr.imm_8               = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    byteCounter += 2; break;
                case 0x2D:
                    instr.imm_32 = *(uint32_t*)&code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+2];
                    instr.instructionBytes[i++] = code[byteCounter+3];
                    instr.instructionBytes[i++] = code[byteCounter+4];
                    byteCounter += 5; break;
                case 0x2F:
                    // TODO: Review
                    byteCounter++; break;
                // XOR Opcodes (0b0011 0000)
                case 0x30:
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            byteCounter += 4;
                        }
                    }
                    byteCounter += 2; break;
                case 0x31:
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            byteCounter += 4;
                        }
                    }
                    byteCounter += 2; break;
                case 0x32:
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            byteCounter += 4;
                        }
                    }
                    byteCounter += 2; break;
                case 0x33:
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            byteCounter += 4;
                        }
                    }
                    byteCounter += 2; break;
                case 0x34:
                    instr.imm_8               = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    byteCounter += 2; break;
                case 0x35:
                    instr.imm_32 = *(uint32_t*)&code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+2];
                    instr.instructionBytes[i++] = code[byteCounter+3];
                    instr.instructionBytes[i++] = code[byteCounter+4];
                    byteCounter += 5; break;
                case 0x37: // AAA AL AH
                // CMP Opcodes (0b0011 1000)
                case 0x38:
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            byteCounter += 4;
                        }
                    }
                    byteCounter += 2; break;
                case 0x39:
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            byteCounter += 4;
                        }
                    }
                    byteCounter += 2; break;
                case 0x3A:
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            byteCounter += 4;
                        }
                    }
                    byteCounter += 2; break;
                case 0x3B:
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            byteCounter += 4;
                        }
                    }
                    byteCounter += 2; break;
                case 0x3C:
                    instr.imm_8               = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    byteCounter += 2; break;
                case 0x3D:
                    instr.imm_32 = *(uint32_t*)&code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+2];
                    instr.instructionBytes[i++] = code[byteCounter+3];
                    instr.instructionBytes[i++] = code[byteCounter+4];
                    byteCounter += 5; break;
                case 0x3F: // AAS AL AH
                    byteCounter++; break;
                // INC Opcodes (0b0100 0000), skip through
                case 0x40:
                case 0x41:
                case 0x42:
                case 0x43:
                case 0x44:
                case 0x45:
                case 0x46:
                case 0x47:
                    byteCounter++; break;
                // DEC Opcodes (0b0100 1000), skip through
                case 0x48:
                case 0x49:
                case 0x4A:
                case 0x4B:
                case 0x4C:
                case 0x4D:
                case 0x4E:
                case 0x4F:
                    byteCounter++; break;
                // Push Opcodes (0b0101 0000), skip through
                case 0x50:
                case 0x51:
                case 0x52:
                case 0x53:
                case 0x54:
                case 0x55:
                case 0x56:
                case 0x57:
                    byteCounter++; break;
                // Pop Opcodes (0b0101 1000), skip through
                case 0x58:
                case 0x59:
                case 0x5A:
                case 0x5B:
                case 0x5C:
                case 0x5D:
                case 0x5E:
                case 0x5F:
                    byteCounter++; break;
                // ?? (0b0110 0000)
                case 0x60: // pusha ax cx dx  |  pushad eax ecx edx
                case 0x61: // popa di si bp   |  popad edi esi ebp
                case 0x62:
                case 0x63:
                //case 0x64:
                //case 0x65:
                //case 0x66:
                  // 66 90                   xchg   ax,ax
                //case 0x67:
                // ?? (0b0110 1000)
                case 0x68: // PUSH imm32
                    instr.imm_32 = *(uint32_t*)&code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+2];
                    instr.instructionBytes[i++] = code[byteCounter+3];
                    instr.instructionBytes[i++] = code[byteCounter+4];
                    byteCounter += 5; break;
                case 0x69:  // IMUL r16/32 r/m16/32 imm16/32
                    // 69  62  36  34 2f 6c 64    imul   esp,DWORD PTR [rdx+0x36],0x646c2f34
                    // 69  09      00 00 02 00    imul   ecx,DWORD PTR [rcx],0x20000
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR) == INDIR_ADDR) // 00
                        {
                            if ((instr.modRegRm & 0b00000111) == 0x7) { // 0b100
                                instr.scaleIndexBase = code[byteCounter+2];
                                instr.instructionBytes[i++] = code[byteCounter+2];

                                instr.imm_32 = *(uint32_t*)&code[byteCounter+3];
                                instr.instructionBytes[i++] = code[byteCounter+3];
                                instr.instructionBytes[i++] = code[byteCounter+4];
                                instr.instructionBytes[i++] = code[byteCounter+5];
                                instr.instructionBytes[i++] = code[byteCounter+6];

                                byteCounter += 6;
                            }
                            // Note: there is a special case for 0b101 missing
                        }
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8) // 01
                        {
                            if ((instr.modRegRm & 0b00000111) == 0x7) { // 0b100
                                instr.scaleIndexBase      = code[byteCounter+2];
                                instr.instructionBytes[i++] = code[byteCounter+2];
                                instr.disp_8              = code[byteCounter+3];
                                instr.instructionBytes[i++] = code[byteCounter+3];

                                instr.imm_32 = *(uint32_t*)&code[byteCounter+4];
                                instr.instructionBytes[i++] = code[byteCounter+4];
                                instr.instructionBytes[i++] = code[byteCounter+5];
                                instr.instructionBytes[i++] = code[byteCounter+6];
                                instr.instructionBytes[i++] = code[byteCounter+7];

                                byteCounter += 6;
                            } else {
                                instr.disp_8              = code[byteCounter+2];
                                instr.instructionBytes[i++] = code[byteCounter+2];

                                instr.imm_32 = *(uint32_t*)&code[byteCounter+3];
                                instr.instructionBytes[i++] = code[byteCounter+3];
                                instr.instructionBytes[i++] = code[byteCounter+4];
                                instr.instructionBytes[i++] = code[byteCounter+5];
                                instr.instructionBytes[i++] = code[byteCounter+6];

                                byteCounter += 5;
                            }

                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32)//10
                        {
                            if ((instr.modRegRm & 0b00000111) == 0x7) { // 0b100
                                instr.scaleIndexBase  = code[byteCounter+2];
                                instr.instructionBytes[i++] = code[byteCounter+2];
                                instr.disp_32 =*(uint32_t*)&code[byteCounter+3];
                                instr.instructionBytes[i++] = code[byteCounter+3];
                                instr.instructionBytes[i++] = code[byteCounter+4];
                                instr.instructionBytes[i++] = code[byteCounter+5];
                                instr.instructionBytes[i++] = code[byteCounter+6];

                                instr.imm_32 = *(uint32_t*)&code[byteCounter+7];
                                instr.instructionBytes[i++] = code[byteCounter+7];
                                instr.instructionBytes[i++] = code[byteCounter+8];
                                instr.instructionBytes[i++] = code[byteCounter+9];
                                instr.instructionBytes[i++]=code[byteCounter+10];

                                byteCounter += 9;
                            } else {
                                instr.disp_32 =*(uint32_t*)&code[byteCounter+2];
                                instr.instructionBytes[i++] = code[byteCounter+2];
                                instr.instructionBytes[i++] = code[byteCounter+3];
                                instr.instructionBytes[i++] = code[byteCounter+4];
                                instr.instructionBytes[i++] = code[byteCounter+5];

                                instr.imm_32 = *(uint32_t*)&code[byteCounter+6];
                                instr.instructionBytes[i++] = code[byteCounter+6];
                                instr.instructionBytes[i++] = code[byteCounter+7];
                                instr.instructionBytes[i++] = code[byteCounter+8];
                                instr.instructionBytes[i++] = code[byteCounter+9];

                                byteCounter += 8;
                            }
                        }
                    }
                    byteCounter += 2; break;
                    break;
                case 0x6A:    // PUSH imm8
                    instr.imm_8               = code[byteCounter+1];
                    byteCounter += 2; break;
                case 0x6B:    // IMUL r16/32 r/m16/32 imm8
                    break;
                case 0x6C:    // INS(B) m8 (DX)
                    // TODO: Review
                    byteCounter++; break;
                    break;
                case 0x6D:
                case 0x6E:
                case 0x6F:
                    byteCounter++; break;
                // Varying J Opcodes (0b0111 0000), all [j* rel8], skip through
                case 0x70:  // JO
                case 0x71:  // JNO
                case 0x72:  // JB / JNAE / JC
                case 0x73:  // JNB / JAE / JNC
                case 0x74:  // JZ / JE
                case 0x75:  // JNZ / JNE
                case 0x76:  // JBE / JNA
                case 0x77:  // JNBE / JA
                case 0x78:  // JS
                case 0x79:  // JNS
                case 0x7A:  // JP / JPE
                case 0x7B:  // JNP / JP0
                case 0x7C:  // JL / JNGE
                case 0x7D:  // JNL / JGE
                case 0x7E:  // JLE / JNG
                case 0x7F:  // JNLE / JG
                    instr.rel_8               = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    byteCounter += 2; break;
                // (0b1000 0000)
                // the exact operation depends on the opcode extension 0-7
                case 0x80: // * r/m8 imm8
                case 0x81: // * r/m16/32 imm16/32
                case 0x82: // * r/m8 imm8
                    byteCounter++; break;
                case 0x83: // * r/m16/32 imm8
                    // 83 ec 3c                sub    esp,0x3c
                    // 83 e0 01                and    eax,0x1
                    // 83 c4 3c                add    esp,0x3c
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];

                            instr.imm_8               = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            byteCounter += 2;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];

                            instr.imm_8               = code[byteCounter+6];
                            instr.instructionBytes[i++] = code[byteCounter+6];

                            byteCounter += 5;
                        }
                    } else {
                        instr.imm_8               = code[byteCounter+2];
                        instr.instructionBytes[i++] = code[byteCounter+2];
                        instr.numInstrBytes ++;      byteCounter ++;
                    }

                    byteCounter += 2; break;
                case 0x84:    // TEST r/m8 r8
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    byteCounter += 2; break;
                case 0x85:    // TEST r/m16/32 r16/32
                    // 85 C0 = test eax, eax
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    byteCounter += 2; break;
                case 0x86:    // XCHG r/m8 r8
                case 0x87:    // XCHG r/m16/32 r16/32
                    byteCounter++; break;
                // (0b1000 1000)
                case 0x88:    // MOV r/m8 r8
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            byteCounter += 4;
                        }
                    }
                    byteCounter += 2; break;
                case 0x89:    // MOV r/m16/32 r16/32
                    // 89 e5                   mov    ebp,esp
                    // 89 c2                   mov    edx,eax
                    // 89 45 fc                mov    DWORD PTR [ebp-0x4],eax
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            byteCounter += 4;
                        }
                    }
                    byteCounter += 2; break;
                case 0x8A:    // MOV r8 r/m8
                    byteCounter++; break;
                case 0x8B:
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            byteCounter += 4;
                        }
                    }
                    byteCounter += 2; break;
                case 0x8C:
                case 0x8D:
                    // 8d b4 26 00 00 00 00    lea    esi,[esi+eiz*1+0x0]
                    // 8d b6 00 00 00 00       lea    esi,[esi+0x0]
                    // 8d 74 26 00             lea    esi,[esi+eiz*1+0x0]
                    // 8d 44 24 2c             lea    eax,[esp+0x2c]
                    // 8d 95 78 ff ff ff       lea    edx,[ebp-0x88]
                    // 8d 45 0c                lea    eax,[ebp+0xc]
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR) == INDIR_ADDR) // 00
                        {
                            if ((instr.modRegRm & 0b00000111) == 0x7) { // 0b100
                                instr.scaleIndexBase = code[byteCounter+2];
                                instr.instructionBytes[i++] = code[byteCounter+2];
                                instr.numInstrBytes++;    byteCounter++;
                            }
                            // Note: there is a special case for 0b101 missing
                        }
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8) // 01
                        {
                            if ((instr.modRegRm & 0b00000111) == 0x7) { // 0b100
                                instr.scaleIndexBase      = code[byteCounter+2];
                                instr.instructionBytes[i++] = code[byteCounter+2];
                                instr.disp_8              = code[byteCounter+3];
                                instr.instructionBytes[i++] = code[byteCounter+3];
                                byteCounter += 2;
                            } else {
                                instr.disp_8              = code[byteCounter+2];
                                instr.instructionBytes[i++] = code[byteCounter+2];
                                byteCounter++;
                            }

                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32)//10
                        {
                            if ((instr.modRegRm & 0b00000111) == 0x7) { // 0b100
                                instr.scaleIndexBase  = code[byteCounter+2];
                                instr.instructionBytes[i++] = code[byteCounter+2];
                                instr.disp_32 =*(uint32_t*)&code[byteCounter+3];
                                instr.instructionBytes[i++] = code[byteCounter+3];
                                instr.instructionBytes[i++] = code[byteCounter+4];
                                instr.instructionBytes[i++] = code[byteCounter+5];
                                instr.instructionBytes[i++] = code[byteCounter+6];
                                byteCounter += 5;
                            } else {
                                instr.disp_32 =*(uint32_t*)&code[byteCounter+2];
                                instr.instructionBytes[i++] = code[byteCounter+2];
                                instr.instructionBytes[i++] = code[byteCounter+3];
                                instr.instructionBytes[i++] = code[byteCounter+4];
                                instr.instructionBytes[i++] = code[byteCounter+5];
                                byteCounter += 4;
                            }
                        }
                    }
                    byteCounter += 2; break;
                case 0x8E: // mov Sreg r/m16


                    break;
                case 0x8F: // pop r/m16/32
                //
                // 0x90 after an F3 prefix = "pause" opcode
                case 0x90: // nop
                    byteCounter++; break;
                // 90+r - xchg r16/32 eax
                // case 0x90:
                // case 0x91:
                // case 0x92:
                // case 0x93:
                // case 0x94:
                // case 0x95:
                // case 0x96:
                // case 0x97:
                // ???
                // case 0x98: cbw/cwde ax/eax, al/ax
                // case 0x99: cwd/cdq dx/edx, ax/eax
                // case 0x9A: callf ptr16:16/32
                // case 0x9B: fwait/wait
                // case 0x9C: pushf/pushfd flags/eflags
                // case 0x9D: popf/popfd flags/eflags
                // case 0x9E: sahf ah
                // case 0x9F: lahf ah
                // Mov Opcodes (variations) - not verified
                case 0xA0:
                    instr.disp_8              = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    byteCounter += 2; break;
                case 0xA1:
                    // a1 20 30 41 00 = mov eax, ds:0x413020
                    instr.disp_32 = *(uint32_t*)&code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+2];
                    instr.instructionBytes[i++] = code[byteCounter+3];
                    instr.instructionBytes[i++] = code[byteCounter+4];
                    byteCounter += 5; break;
                    break;
                case 0xA2:
                    instr.disp_8              = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    byteCounter += 2; break;
                case 0xA3:
                    // a3 44 d0 40 00          mov    ds:0x40d044,eax
                    instr.disp_32 = *(uint32_t*)&code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+2];
                    instr.instructionBytes[i++] = code[byteCounter+3];
                    instr.instructionBytes[i++] = code[byteCounter+4];
                    byteCounter += 5; break;
                case 0xA4:
                case 0xA5:
                case 0xA6:
                case 0xA7:
                    byteCounter++; break;
                case 0xA8:
                    instr.disp_8              = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    byteCounter += 2; break;
                case 0xA9:
                    instr.disp_32 = *(uint32_t*)&code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+2];
                    instr.instructionBytes[i++] = code[byteCounter+3];
                    instr.instructionBytes[i++] = code[byteCounter+4];
                    byteCounter += 5; break;
                case 0xAA:
                case 0xAB:
                case 0xAC:
                case 0xAD:
                case 0xAE:
                    byteCounter++; break;
                case 0xAF:
                    byteCounter++; break;
                // Mov Opcodes (0-7:r8,imm8  -  8-F:r16/32,imm16/32)
                case 0xB0:
                case 0xB1:
                case 0xB2:
                case 0xB3:
                case 0xB4:
                case 0xB5:
                case 0xB6:
                case 0xB7:
                    instr.imm_8 =   code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    byteCounter += 2; break;
                case 0xB8:
                case 0xB9:
                case 0xBA:
                case 0xBB:
                case 0xBC:
                case 0xBD:
                case 0xBE:
                case 0xBF:
                    instr.imm_32 =  *(uint32_t*)&code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+2];
                    instr.instructionBytes[i++] = code[byteCounter+3];
                    instr.instructionBytes[i++] = code[byteCounter+4];
                    byteCounter += 5; break;

                case 0xC0:
                case 0xC1:
                    // c1 e2 03                shl    edx,0x3
                    // c1 e0 03                shl    eax,0x3
                    byteCounter++; break;
                case 0xC2: // retn imm16
                    // C2 04 00 = ret 0x4
                    instr.imm_16 =  *(uint16_t*)&code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+2];
                    byteCounter += 3;
                case 0xC3: // retn
                    byteCounter++; break;
                case 0xC7: // mov r/m16/32
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];

                            instr.imm_32 = *(uint32_t*)&code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];
                            instr.instructionBytes[i++] = code[byteCounter+6];
                            byteCounter += 5;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = *(uint32_t*)&code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+2];
                            instr.instructionBytes[i++] = code[byteCounter+3];
                            instr.instructionBytes[i++] = code[byteCounter+4];
                            instr.instructionBytes[i++] = code[byteCounter+5];

                            instr.imm_32 = *(uint32_t*)&code[byteCounter+6];
                            instr.instructionBytes[i++] = code[byteCounter+6];
                            instr.instructionBytes[i++] = code[byteCounter+7];
                            instr.instructionBytes[i++] = code[byteCounter+8];
                            instr.instructionBytes[i++] = code[byteCounter+9];
                            byteCounter += 8;
                        }
                    }
                    byteCounter += 2; break;
                case 0xC9: // leave
                    byteCounter++; break;
                case 0xCC: // int 0x3? int3?
                    byteCounter++; break;
                case 0xDB: // Incomplete, likely the wrong method of handling
                    instr.modRegRm            = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    byteCounter += 2; break;
                case 0xE8: // CALL rel16/32
                    instr.rel_32 =  *(uint32_t*)&code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+2];
                    instr.instructionBytes[i++] = code[byteCounter+3];
                    instr.instructionBytes[i++] = code[byteCounter+4];
                    byteCounter += 5;
                    break;
                case 0xE9: // JMP rel16/32
                    // TODO: Evaluate accuracy of relative offsets.
                    instr.rel_32 =  *(uint32_t*)&code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+2];
                    instr.instructionBytes[i++] = code[byteCounter+3];
                    instr.instructionBytes[i++] = code[byteCounter+4];
                    byteCounter += 5; break;
                case 0xEA:
                    // JMPF	ptr16:16/32 (not implemented)
                    byteCounter++; break;
                case 0xEB:
                    // 402382:       eb 12                   jmp    402396 <_printInstruction+0x516>
                    // 402390:       eb 04                   jmp    402396 <_printInstruction+0x516>
                    instr.rel_8               = code[byteCounter+1];
                    instr.instructionBytes[i++] = code[byteCounter+1];
                    byteCounter += 2; break;
                case 0xEC:
                    byteCounter++; break;
                case 0xED:
                    byteCounter++; break;
                case 0xEE:
                    byteCounter++; break;
                case 0xEF:
                    byteCounter++; break;
                case 0xF3:
                    // f3 ab                   rep stos DWORD PTR es:[edi],eax
                default:
                    byteCounter++; break;
            }
            instr.numInstrBytes = i;
            printInstruction(instr, 1);
        }

        printf("\n");
        prgmCounter++;
    }

    printf("\n");
}

// return the number of bytes handled.
uint8_t parsePrefix(const unsigned char* code, unsigned int byteCounter)
{
    uint8_t currentPrefix = 0;
    switch (code[byteCounter]) {
        // First group
        case 0xF0: // LOCK
            currentPrefix = 0xF0; break;
        case 0xF2: // REPNE/REPNZ
            currentPrefix = 0xF2; break;
        case 0xF3: // REP or REPE/REPZ /* F3 A6 = repe cmpsb */
            currentPrefix = 0xF3; break;
        // Segment overrides:
        case 0x26: // ES
            currentPrefix = 0x26; break;
        case 0x2E: // CS
            currentPrefix = 0x2E; break;
        case 0x36: // SS
            currentPrefix = 0x36; break;
        case 0x3E: // DS
            currentPrefix = 0x3E; break;
        case 0x64: // FS
            currentPrefix = 0x64; break;
        case 0x65: // GS
            currentPrefix = 0x65; break;
        // Others:
        case 0x66: // "Mandatory" Prefix
            currentPrefix = 0x66; break;
        case 0x67: // 0x67 Prefix
            currentPrefix = 0x67; break;
        default:
            currentPrefix = 0; break;
    }
    return currentPrefix;
}

void printInstruction(struct Instruction instr, int debug)
{
    if (debug) {
        unsigned int bytesPrinted = 0;
        unsigned int i;

        for (i = 0; i < instr.numInstrBytes; i++) {
            printf("%02X ", instr.instructionBytes[i]);
            bytesPrinted++;
        }

        for (i = 0; i < 7 - bytesPrinted; i++)
            printf("   ");
        printf("| ");
    }

    // Print instruction prefix
    int prefixIndex = 0;
    for (prefixIndex = 0; instr.instructionPrefix[prefixIndex]; prefixIndex++) {
        switch (instr.instructionPrefix[prefixIndex]) {
            // First group
            case 0xF0: // LOCK
                printf("(lock) ");      break;
            case 0xF2: // REPNE/REPNZ
                printf("(repnz) ");     break;
            case 0xF3: // REP or REPE/REPZ /* F3 A6 = repe cmpsb */
                printf("(repe) ");      break;
            // Segment overrides:
            case 0x26: // ES
                printf("(es) ");        break;
            case 0x2E: // CS
                printf("(cs) ");        break;
            case 0x36: // SS
                printf("(ss) ");        break;
            case 0x3E: // DS
                printf("(ds) ");        break;
            case 0x64: // FS
                printf("(fs) ");        break;
            case 0x65: // GS
                printf("(gs) ");        break;
            // Others:
            case 0x66: // "Mandatory" Prefix
                printf("(mandatory) "); break;
            case 0x67: // 0x67 Prefix
                printf("(0x67) ");      break;
            default:
                break;
          }
    }

    int i = 0;
    switch (instr.opcode) {
        case 0x00:
            printf("add\t");
            printf("byte ptr ");
            for (i = 7; i >= 0; i--) {
                if ((instr.modRegRm & 0b00000111) == i) {
                    printf("[%s]", reglist[i]);
                    break;
                }
            }
            for (i = 7; i >= 0; i--) {
                if (((instr.modRegRm & 0b00111000) >> 3) == i) {
                    printf(", %s", reglist8l[i]);
                    break;
                }
            }
            break;
        case 0x01:
            printf("add\t");
            printf("dword ptr ");
            for (i = 7; i >= 0; i--) {
                if ((instr.modRegRm & 0b00000111) == i) {
                    printf("[%s]", reglist[i]);
                    break;
                }
            }
            for (i = 7; i >= 0; i--) {
                if (((instr.modRegRm & 0b00111000) >> 3) == i) {
                    printf(", %s", reglist[i]);
                    break;
                }
            }
            break;
        case 0x02:
            printf("add\t");
            for (i = 7; i >= 0; i--) {
                if (((instr.modRegRm & 0b00111000) >> 3) == i) {
                    printf("%s, ", reglist8l[i]);
                    break;
                }
            }
            printf("byte ptr ");
            for (i = 7; i >= 0; i--) {
                if ((instr.modRegRm & 0b00000111) == i) {
                    printf("[%s]", reglist[i]);
                    break;
                }
            }
            break;
        case 0x03:
            printf("add\t");
            for (i = 7; i >= 0; i--) {
                if (((instr.modRegRm & 0b00111000) >> 3) == i) {
                    printf("%s, ", reglist[i]);
                    break;
                }
            }
            printf("dword ptr ");
            for (i = 7; i >= 0; i--) {
                if ((instr.modRegRm & 0b00000111) == i) {
                    printf("[%s]", reglist[i]);
                    break;
                }
            }
            break;
        case 0x04:
            printf("add\t");
            printf("al, ");
            printf("0x%X", instr.imm_8);
            break;
        case 0x05:
            printf("add\t");
            printf("eax, ");
            printf("0x%X", instr.imm_32);
            break;
        // ...
        case 0x10:
            printf("adc\t");
            printf("byte ptr ");
            for (i = 7; i >= 0; i--) {
                if ((instr.modRegRm & 0b00000111) == i) {
                    printf("[%s]", reglist[i]);
                    break;
                }
            }
            for (i = 7; i >= 0; i--) {
                if (((instr.modRegRm & 0b00111000) >> 3) == i) {
                    printf(", %s", reglist8l[i]);
                    break;
                }
            }
            break;
        case 0x12:
            printf("adc\t");
            for (i = 7; i >= 0; i--) {
                if (((instr.modRegRm & 0b00111000) >> 3) == i) {
                    printf("%s, ", reglist8l[i]);
                    break;
                }
            }
            printf("byte ptr ");
            for (i = 7; i >= 0; i--) {
                if ((instr.modRegRm & 0b00000111) == i) {
                    printf("[%s]", reglist[i]);
                    break;
                }
            }
            break;
        case 0x14:
            printf("adc\t");
            for (i = 7; i >= 0; i--) {
                if ((instr.modRegRm & 0b00000111) == i) {
                    printf("%s, ", reglist8l[i]);
                    break;
                }
            }
            printf("0x%X", instr.imm_8);
            break;
        case 0x1A:
            printf("sbb\t");
            for (i = 7; i >= 0; i--) {
                if (((instr.modRegRm & 0b00111000) >> 3) == i) {
                    printf("%s, ", reglist8l[i]);
                    break;
                }
            }
            printf("byte ptr ");
            for (i = 7; i >= 0; i--) {
                if ((instr.modRegRm & 0b00000111) == i) {
                    printf("[%s]", reglist[i]);
                    break;
                }
              }
            break;
        case 0x20:
            printf("and\t");
            printf("byte ptr ");
            for (i = 7; i >= 0; i--) {
                if ((instr.modRegRm & 0b00000111) == i) {
                    printf("[%s]", reglist[i]);
                    break;
                }
            }
            for (i = 7; i >= 0; i--) {
                if (((instr.modRegRm & 0b00111000) >> 3) == i) {
                    printf(", %s", reglist8l[i]);
                    break;
                }
            }
            break;
        case 0x2F:
            // TODO: Review
            printf("das\t");
            break;
        case 0x30: // r/m8, r8
            printf("xor\t");
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR)
                printf("byte ptr ");
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) printf("[");
            for (i = 7; i >= 0; i--) {
                if ((instr.modRegRm & 0b00000111) == i) {
                    printf("%s", reglist8l[i]);
                    break;
                }
            }
            if (instr.disp_8) {
                if (instr.disp_8 & 0b10000000) // negative
                    printf("-0x%X", (int8_t)(-instr.disp_8));
                else
                    printf("+0x%X", instr.disp_8);
            }
            if (instr.disp_32) {
                if (instr.disp_32 & 0b10000000000000000000000000000000) // negative
                    printf("-0x%X", (int32_t)(-instr.disp_32));
                else
                    printf("+0x%X", instr.disp_32);
            }
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) printf("]");
            for (i = 7; i >= 0; i--) {
                if (((instr.modRegRm & 0b00111000) >> 3) == i) {
                    printf(", %s", reglist8l[i]);
                    break;
                }
            }
            break;
        case 0x31: // r/m16/32, r16/32
            printf("xor\t");
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR)
                printf("dword ptr ");
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) printf("[");
            for (i = 7; i >= 0; i--) {
                if ((instr.modRegRm & 0b00000111) == i) {
                    printf("%s", reglist[i]);
                    break;
                }
            }
            if (instr.disp_8) {
                if (instr.disp_8 & 0b10000000) // negative
                    printf("-0x%X", (int8_t)(-instr.disp_8));
                else
                    printf("+0x%X", instr.disp_8);
            }
            if (instr.disp_32) {
                if (instr.disp_32 & 0b1000000000000000000000000000000) // negative
                    printf("-0x%X", (int32_t)(-instr.disp_32));
                else
                    printf("+0x%X", instr.disp_32);
            }
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) printf("]");
            for (i = 7; i >= 0; i--) {
                if (((instr.modRegRm & 0b00111000) >> 3) == i) {
                    printf(", %s", reglist[i]);
                    break;
                }
            }
            break;
        case 0x32: // r8, r/m8
            printf("xor\t");
            for (i = 7; i >= 0; i--) {
                if (((instr.modRegRm & 0b00111000) >> 3) == i) {
                    printf("%s, ", reglist8l[i]);
                    break;
                }
            }
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR)
                printf("byte ptr ");
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) printf("[");
            for (i = 7; i >= 0; i--) {
                if ((instr.modRegRm & 0b00000111) == i) {
                    printf("%s", reglist8l[i]);
                    break;
                }
            }
            if (instr.disp_8) {
                if (instr.disp_8 & 0b1000000) // negative
                    printf("-0x%X", (int8_t)(-instr.disp_8));
                else
                    printf("+0x%X", instr.disp_8);
            }
            if (instr.disp_32) {
                if (instr.disp_32 & 0b100000000) // negative
                    printf("-0x%X", (int32_t)(-instr.disp_32));
                else
                    printf("+0x%X", instr.disp_32);
            }
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) printf("]");
            break;
        case 0x33: // r16/32, r/m16/32
            printf("xor\t");
            for (i = 7; i >= 0; i--) {
            		if (((instr.modRegRm & 0b00111000) >> 3) == i) {
            		    printf("%s, ", reglist[i]);
            		    break;
            		}
      	    }
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR)
                printf("dword ptr ");
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) printf("[");
            for (i = 7; i >= 0; i--) {
            		if ((instr.modRegRm & 0b00000111) == i) {
            		    printf("%s", reglist[i]);
            		    break;
            		}
            }
            if (instr.disp_8) {
                if (instr.disp_8 & 0b10000000) // negative
                    printf("-0x%X", (int8_t)(-instr.disp_8));
                else
                    printf("+0x%X", instr.disp_8);
            }
            if (instr.disp_32) {
                if (instr.disp_32 & 0b10000000000000000000000000000000) // negative
                    printf("-0x%X", (int32_t)(-instr.disp_32));
                else
                    printf("+0x%X", instr.disp_32);
            }
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) printf("]");
            break;
        case 0x34:
            printf("xor\t");
            printf("al, ");
            printf("0x%X", instr.imm_8);
            break;
        case 0x35:
            printf("xor\t");
            printf("eax, ");
            printf("0x%X", instr.imm_32);
            break;

        case 0x39:
            printf("cmp\t");
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR)
                printf("dword ptr ");
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) printf("[");
            for (i = 7; i >= 0; i--) {
                if ((instr.modRegRm & 0b00000111) == i) {
                    printf("%s", reglist[i]);
                    break;
                }
            }
            if (instr.disp_8) {
                if (instr.disp_8 & 0b10000000) // negative
                    printf("-0x%X", (int8_t)(-instr.disp_8));
                else
                    printf("+0x%X", instr.disp_8);
            }
            if (instr.disp_32) {
                if (instr.disp_32 & 0b10000000000000000000000000000000) // negative
                    printf("-0x%X", (int32_t)(-instr.disp_32));
                else
                    printf("+0x%X", instr.disp_32);
            }
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) printf("]");
            for (i = 7; i >= 0; i--) {
                if (((instr.modRegRm & 0b00111000) >> 3) == i) {
                    printf(", %s", reglist[i]);
                    break;
                }
            }
            break;

        case 0x3B:
            printf("cmp\t");
            for (i = 7; i >= 0; i--) {
            		if (((instr.modRegRm & 0b00111000) >> 3) == i) {
            		    printf("%s, ", reglist[i]);
            		    break;
            		}
      	    }
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR)
                printf("dword ptr ");
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) printf("[");
            for (i = 7; i >= 0; i--) {
            		if ((instr.modRegRm & 0b00000111) == i) {
            		    printf("%s", reglist[i]);
            		    break;
            		}
            }
            if (instr.disp_8) {
                if (instr.disp_8 & 0b10000000) // negative
                    printf("-0x%X", (int8_t)(-instr.disp_8));
                else
                    printf("+0x%X", instr.disp_8);
            }
            if (instr.disp_32) {
                if (instr.disp_32 & 0b10000000000000000000000000000000) // negative
                    printf("-0x%X", (int32_t)(-instr.disp_32));
                else
                    printf("+0x%X", instr.disp_32);
            }
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) printf("]");
            break;
        // INC Opcodes (0b0100 0000), skip through
        case 0x40:
        case 0x41:
        case 0x42:
        case 0x43:
        case 0x44:
        case 0x45:
        case 0x46:
        case 0x47:
            printf("inc\t");
            for (i = 7; i >= 0; i--) {
                if ((instr.opcode & i) == i) {
                    printf("%s", reglist[i]);
                    break;
                }
            }
            break;
        // DEC Opcodes (0b0100 1000), skip through
        case 0x48:
        case 0x49:
        case 0x4A:
        case 0x4B:
        case 0x4C:
        case 0x4D:
        case 0x4E:
        case 0x4F:
            printf("dec\t");
            for (i = 7; i >= 0; i--) {
                if ((instr.opcode & i) == i) {
                    printf("%s", reglist[i]);
                    break;
                }
            }
            break;
        // Push Opcodes
        case 0x50:
        case 0x51:
        case 0x52:
        case 0x53:
        case 0x54:
        case 0x55:
        case 0x56:
        case 0x57:
            printf("push\t");
            for (i = 7; i >= 0; i--) {
                if ((instr.opcode & i) == i) {
                    printf("%s", reglist[i]);
                    break;
                }
            }
            break;
        // Pop Opcodes
        case 0x58:
        case 0x59:
        case 0x5A:
        case 0x5B:
        case 0x5C:
        case 0x5D:
        case 0x5E:
        case 0x5F:
            printf("pop\t");
            for (i = 7; i >= 0; i--) {
                if ((instr.opcode & i) == i) {
                    printf("%s", reglist[i]);
                    break;
                }
            }
            break;
        case 0x68:
            printf("push\t");
            printf("0x%X", instr.imm_32);
            break;
        case 0x69:
            printf("imul\t");
            for (i = 7; i >= 0; i--) { // & 0b00111000
                if (((instr.modRegRm & 0b00111000) >> 3) == i) {
                    printf("%s, ", reglist[i]);
                    break;
                }
            }

            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR)
                printf("dword ptr ");
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) printf("[");
            for (i = 7; i >= 0; i--) { // & 0b00000111
                if (((instr.modRegRm & 0b00000111)) == i) {
                    printf("%s", reglist[i]);
                    break;
                }
            }
            if (instr.disp_8) {
                if (instr.disp_8 & 0b10000000) // negative
                    printf("-0x%X", (int8_t)(-instr.disp_8));
                else
                    printf("+0x%X", instr.disp_8);
            }
            if (instr.disp_32) {
                if (instr.disp_32 & 0b10000000000000000000000000000000) // negative
                    printf("-0x%X", (int32_t)(-instr.disp_32));
                else
                    printf("+0x%X", instr.disp_32);
            }
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) printf("]");
            printf(", 0x%X", instr.imm_32);
            break;
        case 0x6A:
            printf("push\t");
            printf("0x%X", instr.imm_8);
            break;

        case 0x6C:
            // TODO: Review
            // INS[B] 	m8 	DX
            // "Input from Port to String"
            printf("ins\t");
            printf("byte ptr es:[edi],dx");
            break;

        // Varying J Opcodes (0b0111 0000), all are [j* rel8]
        case 0x70:  // JO
            printf("j0\t");
            printf("0x%X",
                instr.rel_8 + instr.numInstrBytes + instr.relativeOff);
            break;
        case 0x71:  // JNO
            printf("jn0\t");
            printf("0x%X",
                instr.rel_8 + instr.numInstrBytes + instr.relativeOff);
            break;
        case 0x72:  // JB / JNAE / JC
            printf("jb\t");
            printf("0x%X",
                instr.rel_8 + instr.numInstrBytes + instr.relativeOff);
            break;
        case 0x73:  // JNB / JAE / JNC
            printf("jnb\t");
            printf("0x%X",
                instr.rel_8 + instr.numInstrBytes + instr.relativeOff);
            break;
        case 0x74:  // JZ / JE
            // TODO: Bug occurs where this isn't always accurate.
            // Reduplicate: some clenv-compiled hello world.
            printf("je\t"); // or, printf("jz\t");
            printf("0x%X",
                instr.rel_8 + instr.numInstrBytes + instr.relativeOff);
            break;
        case 0x75:  // JNZ / JNE
            printf("jne\t"); // or, printf("jnz\t");
            printf("0x%X",
                instr.rel_8 + instr.numInstrBytes + instr.relativeOff);
            break;
        case 0x76:  // JBE / JNA
            printf("jbe\t");
            printf("0x%X",
                instr.rel_8 + instr.numInstrBytes + instr.relativeOff);
            break;
        case 0x77:  // JNBE / JA
            printf("jnbe\t");
            printf("0x%X",
                instr.rel_8 + instr.numInstrBytes + instr.relativeOff);
            break;
        case 0x78:  // JS
            printf("js\t");
            printf("0x%X",
                instr.rel_8 + instr.numInstrBytes + instr.relativeOff);
            break;
        case 0x79:  // JNS
            printf("jns\t");
            printf("0x%X",
                instr.rel_8 + instr.numInstrBytes + instr.relativeOff);
            break;
        case 0x7A:  // JP / JPE
            printf("jp\t");
            printf("0x%X",
                instr.rel_8 + instr.numInstrBytes + instr.relativeOff);
            break;
        case 0x7B:  // JNP / JP0
            printf("jnp\t");
            printf("0x%X",
                instr.rel_8 + instr.numInstrBytes + instr.relativeOff);
            break;
        case 0x7C:  // JL / JNGE
            printf("jl\t");
            printf("0x%X",
                instr.rel_8 + instr.numInstrBytes + instr.relativeOff);
            break;
        case 0x7D:  // JNL / JGE
            printf("jnl\t");
            printf("0x%X",
                instr.rel_8 + instr.numInstrBytes + instr.relativeOff);
            break;
        case 0x7E:  // JLE / JNG
            printf("jle\t");
            printf("0x%X",
                instr.rel_8 + instr.numInstrBytes + instr.relativeOff);
            break;
        case 0x7F:  // JNLE / JG
            printf("rel+%i\n", instr.rel_8);
            printf("0x%X",
                instr.rel_8 + instr.numInstrBytes + instr.relativeOff);
            break;

        case 0x83: // r/m16/32, imm8
            switch ((instr.modRegRm & 0b00111000) >> 3) { // & 0b00111000
                case 0:
                    printf("add\t"); break;
                case 1:
                    printf("or\t"); break;
                case 2:
                    printf("adc\t"); break;
                case 3:
                    printf("sbb\t"); break;
                case 4:
                    printf("and\t"); break;
                case 5:
                    printf("sub\t"); break;
                case 6:
                    printf("xor\t"); break;
                case 7:
                    printf("cmp\t"); break;
            }
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR)
                printf("dword ptr ");
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) printf("[");
            for (i = 7; i >= 0; i--) { // & 0b00000111
                if (((instr.modRegRm & 0b00000111)) == i) {
                    printf("%s", reglist[i]);
                    break;
                }
            }
            if (instr.disp_8) {
                if (instr.disp_8 & 0b10000000) // negative
                    printf("-0x%X", (int8_t)(-instr.disp_8));
                else
                    printf("+0x%X", instr.disp_8);
            }
            if (instr.disp_32) {
                if (instr.disp_32 & 0b10000000000000000000000000000000) // negative
                    printf("-0x%X", (int32_t)(-instr.disp_32));
                else
                    printf("+0x%X", instr.disp_32);
            }
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) printf("]");
            printf(", 0x%X", instr.imm_8);
            break;
        case 0x84:    // TEST r/m8 r8
            printf("test\t");
            for (i = 7; i >= 0; i--) { // & 0b00000111
                if (((instr.modRegRm & 0b00000111)) == i) {
                    printf("%s, ", reglist8l[i]);
                    break;
                }
            }
            for (i = 7; i >= 0; i--) { // & 0b00111000
                if (((instr.modRegRm & 0b00111000) >> 3) == i) {
                    printf("%s", reglist8l[i]);
                    break;
                }
            }
            break;
        case 0x85:    // TEST r/m16/32 r16/32
            printf("test\t");
            for (i = 7; i >= 0; i--) { // & 0b00000111
                if (((instr.modRegRm & 0b00000111)) == i) {
                    printf("%s, ", reglist[i]);
                    break;
                }
            }
            for (i = 7; i >= 0; i--) { // & 0b00111000
                if (((instr.modRegRm & 0b00111000) >> 3) == i) {
                    printf("%s", reglist[i]);
                    break;
                }
            }
            break;
        case 0x88: // mov r/m8 r8
            printf("mov\t");
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR)
                printf("byte ptr ");
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) printf("[");
            for (i = 7; i >= 0; i--) { // & 0b00000111
                if (((instr.modRegRm & 0b00000111)) == i) {
                    printf("%s", reglist[i]);
                    break;
                }
            }
            if (instr.disp_8) {
                if (instr.disp_8 & 0b10000000) // negative
                    printf("-0x%X", (int8_t)(-instr.disp_8));
                else
                    printf("+0x%X", instr.disp_8);
            }
            if (instr.disp_32) {
                if (instr.disp_32 & 0b10000000000000000000000000000000) // negative
                    printf("-0x%X", (int32_t)(-instr.disp_32));
                else
                    printf("+0x%X", instr.disp_32);
            }
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) printf("]");
            for (i = 7; i >= 0; i--) { // & 0b00111000
                if (((instr.modRegRm & 0b00111000) >> 3) == i) {
                    printf(", %s", reglist8l[i]);
                    break;
                }
            }
            break;
        case 0x89: // mov r/m16/32 r16/32
            printf("mov\t");
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR)
                printf("dword ptr ");
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) printf("[");
            for (i = 7; i >= 0; i--) { // & 0b00000111
                if (((instr.modRegRm & 0b00000111)) == i) {
                    printf("%s", reglist[i]);
                    break;
                }
            }
            if (instr.disp_8) {
                if (instr.disp_8 & 0b10000000) // negative
                    printf("-0x%X", (int8_t)(-instr.disp_8));
                else
                    printf("+0x%X", instr.disp_8);
            }
            if (instr.disp_32) {
                if (instr.disp_32 & 0b10000000000000000000000000000000) // negative
                    printf("-0x%X", (int32_t)(-instr.disp_32));
                else
                    printf("+0x%X", instr.disp_32);
            }
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) printf("]");
            for (i = 7; i >= 0; i--) { // & 0b00111000
                if (((instr.modRegRm & 0b00111000) >> 3) == i) {
                    printf(", %s", reglist[i]);
                    break;
                }
            }
            break;
        case 0x8B: // mov r16/32 r/m16/32 (flipped order from 0x8A)
            printf("mov\t");
            for (i = 7; i >= 0; i--) { // & 0b00111000
                if (((instr.modRegRm & 0b00111000) >> 3) == i) {
                    printf("%s, ", reglist[i]);
                    break;
                }
            }
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR)
                printf("dword ptr ");
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) printf("[");
            for (i = 7; i >= 0; i--) { // & 0b00000111
                if (((instr.modRegRm & 0b00000111)) == i) {
                    printf("%s", reglist[i]);
                    break;
                }
            }
            if (instr.disp_8) {
                if (instr.disp_8 & 0b10000000) // negative
                    printf("-0x%X", (int8_t)(-instr.disp_8));
                else
                    printf("+0x%X", instr.disp_8);
            }
            if (instr.disp_32) {
                if (instr.disp_32 & 0b10000000000000000000000000000000) // negative
                    printf("-0x%X", (int32_t)(-instr.disp_32));
                else
                    printf("+0x%X", instr.disp_32);
            }
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) printf("]");
            break;
        case 0x8D:
            printf("lea\t");
            for (i = 7; i >= 0; i--) { // & 0b00111000
                if (((instr.modRegRm & 0b00111000) >> 3) == i) {
                    printf("%s, ", reglist[i]);
                    break;
                }
            }
            printf("[");
            if (instr.scaleIndexBase) {
                // TODO: False assumptions to remove later:
                // - the Index part of SIB is assumed to be a register
                for (i = 7; i >= 0; i--) { // & 0b00000111
                    if (((instr.modRegRm & 0b00000111)) == i) {
                        printf("%s+", reglist[i]);
                        break;
                    }
                }
                switch ((instr.scaleIndexBase & 0b11000000) >> 6) { // 0b11000000
                    case 0x01: // 0b01
                        printf("(");
                        for (i = 7; i >= 0; i--) { // & 0b00111000
                            if (((instr.modRegRm & 0b00111000) >> 3) == i) {
                                printf("%s", reglist[i]);
                                break;
                            }
                        }
                        printf("*2)");
                        break;
                    case 0x02: // 0b10
                        printf("(");
                        for (i = 7; i >= 0; i--) { // & 0b00111000
                            if (((instr.modRegRm & 0b00111000) >> 3) == i) {
                                printf("%s", reglist[i]);
                                break;
                            }
                        }
                        printf("*4)");
                        break;
                    case 0x03: // 0b11
                        printf("(");
                        for (i = 7; i >= 0; i--) { // & 0b00111000
                            if (((instr.modRegRm & 0b00111000) >> 3) == i) {
                                printf("%s", reglist[i]);
                                break;
                            }
                        }
                        printf("*8)");
                        break;
                    default:
                        for (i = 7; i >= 0; i--) { // & 0b00111000
                            if (((instr.modRegRm & 0b00111000) >> 3) == i) {
                                printf("%s", reglist[i]);
                                break;
                            }
                        }
                        break;
                }
                if (instr.disp_8) {
                    if (instr.disp_8 & 0b10000000) // negative
                        printf("-0x%X", -instr.disp_8);
                    else
                        printf("+0x%X", instr.disp_8);
                }
                if (instr.disp_32)  printf("+0x%X", instr.disp_32);
            } else {
                for (i = 7; i >= 0; i--) { // & 0b00000111
                    if (((instr.modRegRm & 0b00000111) & i) == i) {
                        printf("%s", reglist[i]);
                        break;
                    }
                }
                if (instr.disp_8) {
                    if (instr.disp_8 & 0b10000000) // negative
                        printf("-0x%X", -instr.disp_8);
                    else
                        printf("+0x%X", instr.disp_8);
                }
                if (instr.disp_32)  printf("+0x%X", instr.disp_32);
            }
            printf("]");
            break;

        case 0x90:
            printf("nop");
            break;

        case 0xA1:
            printf("mov\t");
            printf("eax, ");
            printf("ds:"); // TODO: I can't confirm the meaning of this
            printf("0x%X", instr.disp_32);
            break;

        case 0xAE:
            printf("scas\t");
            printf("al, ");
            // TODO: Review
            printf("byte ptr es:[edi]");
            break;

        // Mov, Imm8
        case 0xB0:
        case 0xB1:
        case 0xB2:
        case 0xB3:
        case 0xB4:
        case 0xB5:
        case 0xB6:
        case 0xB7:
            printf("mov\t");
            for (i = 7; i >= 0; i--) {
                if ((instr.opcode & i) == i) {
                    printf("%s, ", reglist8l[i]);
                    break;
                }
            }
            printf("0x%X", instr.imm_8);
            break;
        // Mov, Imm32
        case 0xB8:
        case 0xB9:
        case 0xBA:
        case 0xBB:
        case 0xBC:
        case 0xBD:
        case 0xBE:
        case 0xBF:
            printf("mov\t");
            for (i = 7; i >= 0; i--) {
                if ((instr.opcode & i) == i) {
                    printf("%s, ", reglist[i]);
                    break;
                }
                if (!i) printf("eax, ");
            }
            printf("0x%X", instr.imm_32);
            break;
        case 0xC2:
            printf("ret\t");
            printf("0x%04x", instr.imm_16);
            break;
        case 0xC3:
            printf("ret"); // or printf("retn");
            break;
        case 0xC7: // mov r/m16/32
            printf("mov\t");
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR)
                printf("dword ptr ");
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) printf("[");
            for (i = 7; i >= 0; i--) { // & 0b00000111
                if (((instr.modRegRm & 0b00000111)) == i) {
                    printf("%s", reglist[i]);
                    break;
                }
            }
            if (instr.disp_8) {

                if (instr.disp_8 & 0b10000000) // negative
                    printf("-0x%X", (int8_t)(-instr.disp_8));
                else
                    printf("+0x%X", instr.disp_8);
            }
            if (instr.disp_32) {
                if (instr.disp_32 & 0b10000000000000000000000000000000) // negative
                    printf("-0x%X", (int32_t)(-instr.disp_32));
                else
                    printf("+0x%X", instr.disp_32);
            }
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) printf("]");
            printf(", 0x%X", instr.imm_32);
            break;
        case 0xCC:
            printf("int\t0x03");
            break;
        case 0xDB: // again; incomplete and wrong way of handling
            switch (instr.modRegRm) {
                case 0xE2:
                    printf("fnclex");
                    break;
                default:
                    printf("0xDB opcode undefined");
                    break;
            }
            break;
        case 0xE8:
            printf("call\t");
            uint32_t calculatedRelative =
                instr.rel_32 + instr.numInstrBytes + instr.relativeOff;
            printf("0x%X", calculatedRelative);
            break;
        case 0xEE:
            // TODO: Review.
            printf("out\t");
            printf("dx, al");
            break;
        default:
            printf("undefined opcode");
            break;
    }


}
