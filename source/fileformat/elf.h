#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elfstructs.h"
#include "../disassembler/x86.h"

// parseElf is called from main; parseElf then detects
// if executable is 32 or 64 bit, and processes accordingly.
void parseElf(FILE* fp, long fileSize);
void parseElf32(FILE* fp, long fileSize);
void parseElf64(FILE* fp, long fileSize);
