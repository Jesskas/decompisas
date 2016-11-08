#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mach-ostructs.h"
#include "../disassembler/x86.h"

void parseMach32(FILE* fp, long fileSize);
void parseMach64(FILE* fp, long fileSize);
