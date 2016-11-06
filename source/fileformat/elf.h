#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elfstructs.h"
#include "../disassembler/x86.h"
#include "../disassembler/arm.h"

#define MAKE_SHORT(x, y)        ((x << 8) | y)
#define MAKE_LONG(x, y, z, w)   ((x << 24) | (y << 16) | (z << 8) | (w))
#define MAKE_LP1(x, y, z, w)    (long long)((x<<24) | (y<<16) | (z<<8) | (w))
// TODO: Better method for 64-bit.

// parseElf is called from main; parseElf then detects
// if executable is 32 or 64 bit, and processes accordingly.
void parseElf(FILE* fp, long fileSize);
void parseElf32(FILE* fp, long fileSize);
void parseElf64(FILE* fp, long fileSize);
