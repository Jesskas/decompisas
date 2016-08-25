#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pestructs.h"
#include "../disassembler/x86.h"

#define MAKE_SHORT(x, y)        ((x << 8) | y)
#define MAKE_LONG(x, y, z, w)   ((x << 24) | (y << 16) | (z << 8) | (w))

void parseWin32(FILE* fp, long fileSize);
