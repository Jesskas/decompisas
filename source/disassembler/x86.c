#include "x86.h"

// define extern declaration
const char* reglist[8] =
    { "eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi" };

// TODO: This assumes IA-32. Split into IA-32 and x64.
void disassemble_x86(char* name, int RVA, const unsigned char* code,
    unsigned int codeSize)
{
    unsigned int byteCounter = 0;
    unsigned int prgmCounter = 0;

    while (prgmCounter < 60 && byteCounter < codeSize) {
        printf("%s:0x%08X\t", name, byteCounter + RVA);
        if (parsePrefix(code[byteCounter])) byteCounter++;

        // Single-byte opcode
        if (code[byteCounter] != 0x0F) {
            struct Instruction instr = { 0 };
            instr.byteCounter = byteCounter;
            instr.prgmCounter = prgmCounter;
            instr.opcode = code[byteCounter];
            switch (code[byteCounter]) {
                case 0x33:
                    instr.modRegRm  = code[byteCounter+1];
                    byteCounter += 2; break;
                // Push Opcodes
                case 0x50:
                case 0x51:
                case 0x52:
                case 0x53:
                case 0x54:
                case 0x55:
                case 0x56:
                case 0x57:
                // Pop Opcodes
                case 0x58:
                case 0x59:
                case 0x5A:
                case 0x5B:
                case 0x5C:
                case 0x5D:
                case 0x5E:
                case 0x5F:
                    byteCounter++; break;
                // Others
                case 0x68:
                    instr.imm_32 =
                        MAKE_LONG(code[byteCounter+4], code[byteCounter+3],
                        code[byteCounter+2], code[byteCounter+1]);
                    byteCounter += 5; break;
                case 0x83:
                    instr.modRegRm  = code[byteCounter+1];
                    instr.imm_8     = code[byteCounter+2];
                    byteCounter += 3; break;
                case 0x8B:
                    instr.modRegRm  = code[byteCounter+1];
                    if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) {
                        if ((instr.modRegRm & INDIR_ADDR8) == INDIR_ADDR8)
                        {
                            instr.disp_8 = code[byteCounter+2];
                            byteCounter++;
                        }
                        if ((instr.modRegRm & INDIR_ADDR32) == INDIR_ADDR32) {
                            instr.disp_32 = MAKE_LONG(
                                code[byteCounter+5], code[byteCounter+4],
                                code[byteCounter+3], code[byteCounter+2]);
                            byteCounter++;
                        }
                    }

                    byteCounter += 2; break;
                // Mov Opcodes
                case 0xB0:
                case 0xB1:
                case 0xB2:
                case 0xB3:
                case 0xB4:
                case 0xB5:
                case 0xB6:
                case 0xB7:
                    instr.imm_8     = code[byteCounter+1];
                    byteCounter += 2; break;
                case 0xB8:
                case 0xB9:
                case 0xBA:
                case 0xBB:
                case 0xBC:
                case 0xBD:
                case 0xBE:
                case 0xBF:
                    instr.imm_32 =
                        MAKE_LONG(code[byteCounter+4], code[byteCounter+3],
                        code[byteCounter+2], code[byteCounter+1]);
                    byteCounter += 5; break;
                case 0xC3:
                    byteCounter++; break;
                case 0xCC:
                    byteCounter++; break;
                case 0xE8:
                    // TODO: Evaluate accuracy of relative offsets.
                    instr.rel_32 =
                        MAKE_LONG(code[byteCounter+4], code[byteCounter+3],
                        code[byteCounter+2], code[byteCounter+1]);
                    byteCounter += 5; break;
                default:
                    byteCounter++; break;
            }
            printInstruction(instr, 1);
        }

        printf("\n");
        prgmCounter++;
    }

    printf("\n");
}

int parsePrefix(unsigned char byte){
    switch (byte) {
        case 0xF0: // LOCK
            printf("(lock) ");
            break;
        case 0xF2: // REPNE/REPNZ
            printf("(repne/repnz) ");
            break;
        case 0xF3: // REP or REPE/REPZ
            printf("(rep/repe/repz) ");
            break;
        case 0x2D:
            printf("(CS segment override) ");
            break;
        case 0x36:
            printf("(SS segment override) ");
            break;
        case 0x3E:
            printf("(DS segment override/mandatory) ");
            break;
        case 0x26:
            printf("(ES segment override) ");
            break;
        case 0x64:
            printf("(FS segment override) ");
            break;
        case 0x65:
            printf("(GS segment override) ");
            break;
        case 0x2E:
            printf("(branch not taken/mandatory) ");
            break;
        //case 0x3E:
        //    break;
        case 0x66:
            printf("(manatory) ");
            break;

        case 0x67:
            printf("(0x67 prefix something) ");
            break;

        default:
            return 0;
    }

    return 1;
}

void printInstruction(struct Instruction instr, int debug)
{
    if (debug) {
        unsigned int bytesPrinted = 0;
        unsigned int i;
        for (i = 0; instr.instructionPrefix[i]; i++) {
            printf("%02X ", instr.instructionPrefix[i]);
            bytesPrinted++;
        }
        printf("%02X ", instr.opcode); bytesPrinted++;
        if (instr.opcode2) {
            printf("%02X ", instr.opcode2);
            bytesPrinted++;
        }
        if (instr.modRegRm) {
            printf("%02X ", instr.modRegRm);
            bytesPrinted++;
        }
        if (instr.scaleIndexBase) {
            printf("%02X ", instr.scaleIndexBase);
            bytesPrinted++;
        }
        if (instr.imm_8) {
            printf("%02X ", instr.imm_8); bytesPrinted++;
        }
        if (instr.imm_32) {
            printf("%02X ", (uint8_t)(instr.imm_32));
            printf("%02X ", (uint8_t)(instr.imm_32 >> 8));
            printf("%02X ", (uint8_t)(instr.imm_32 >> 16));
            printf("%02X ", (uint8_t)(instr.imm_32 >> 24));

            bytesPrinted += 4;
        }
        if (instr.rel_8) {
            printf("%02X ", instr.rel_8); bytesPrinted++;
        }
        if (instr.rel_32) {
            printf("%02X ", (uint8_t)(instr.rel_32));
            printf("%02X ", (uint8_t)(instr.rel_32 >> 8));
            printf("%02X ", (uint8_t)(instr.rel_32 >> 16));
            printf("%02X ", (uint8_t)(instr.rel_32 >> 24));

            bytesPrinted += 4;
        }
        if (instr.disp_8) {
            printf("%02X ", instr.disp_8); bytesPrinted++;
        }
        if (instr.disp_32) {
            printf("%02X ", (uint8_t)(instr.disp_32));
            printf("%02X ", (uint8_t)(instr.disp_32 >> 8));
            printf("%02X ", (uint8_t)(instr.disp_32 >> 16));
            printf("%02X ", (uint8_t)(instr.disp_32 >> 24));

            bytesPrinted += 4;
        }

        for (i = 0; i < 6 - bytesPrinted; i++)
            printf("   ");
        printf("| ");
    }

    // TODO: Print instruction prefix
    unsigned int i = 0;
    switch (instr.opcode) {
        case 0x33:
            printf("xor\t\t");
            if (~instr.modRegRm & ~(EAX << 3))
               printf("eax, ");
            if (~instr.modRegRm & ~EAX)
               printf("eax");
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
                if (!i) printf("eax");
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
            printf("pop\t\t");
            for (i = 7; i >= 0; i--) {
                if ((instr.opcode & i) == i) {
                    printf("%s", reglist[i]);
                    break;
                }
                if (!i) printf("eax");
            }
            break;
        case 0x68:
            printf("push\t");
            printf("0x%08X", instr.imm_32);
            break;
        case 0x83: // TODO: lol, this isn't necessarily add
            printf("add\t\t");
            if (instr.modRegRm & ESP)
                printf("esp, ");
            printf("0x%02X", instr.imm_8);
            break;
        case 0x8B:
            printf("mov\t\t");
            for (i = 7; i >= 0; i--) {
                if ((instr.modRegRm & (i << 3)) == (i << 3)) {
                    printf("%s, ", reglist[i]);
                    break;
                }
                if (!i) printf("eax, ");
            }
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) printf("[");
            for (i = 7; i >= 0; i--) {
                if ((instr.modRegRm & i) == i) {
                    printf("%s", reglist[i]);
                    break;
                }
                if (!i) printf("eax");
            }
            if (instr.disp_8)   printf("+0x%02X", instr.disp_8);
            if (instr.disp_32)  printf("+0x%08X", instr.disp_32);
            if ((instr.modRegRm & DIRECT_ADDR) != DIRECT_ADDR) printf("]");
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
            printf("mov\t\t");
            for (i = 7; i >= 0; i--) {
                if ((instr.opcode & i) == i) {
                    printf("%s", reglist[i]);
                    break;
                }
                if (!i) printf("eax, ");
            }
            printf("0x%02X", instr.imm_8);
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
            printf("mov\t\t");
            for (i = 7; i >= 0; i--) {
                if ((instr.opcode & i) == i) {
                    printf("%s, ", reglist[i]);
                    break;
                }
                if (!i) printf("eax, ");
            }
            printf("0x%08X", instr.imm_32);
            break;
        case 0xC3:
            printf("retn");
            break;
        case 0xCC:
            printf("int 0x03");
            break;
        case 0xE8:
            printf("call\t");
            printf("0x%08X", instr.rel_32);
            break;
        default:
            printf("undefined opcode");
            break;
    }


}