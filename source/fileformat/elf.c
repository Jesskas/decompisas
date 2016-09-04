// TODO: ei_data specifies endianness. Perhaps use this rather than assume LE.

#include "elf.h"

void parseElf(FILE* fp, long fileSize) {

    // ELF Header
    unsigned char* buffer = (unsigned char*)malloc(0x05);
    fread(buffer, sizeof(unsigned char), 0x05, fp);

    if (buffer[4] == 1) {
        printf("Determined 32-bit.\n");
        if (fseek(fp, 0l, SEEK_SET) != 0) {
            printf("[ELF] Error seeking back to zero (parse).\n");
        }
        free(buffer);
        parseElf32(fp, fileSize);
    } else if (buffer[4] == 2) {
        printf("Determined 64-bit.\n");
        if (fseek(fp, 0l, SEEK_SET) != 0) {
            printf("[ELF] Error seeking back to zero (parse).\n");
        }
        free(buffer);
        parseElf64(fp, fileSize);
    } else {
        printf("Failed to determine 32-bit or 64-bit.\n");
        return;
    }
}

void parseElf32(FILE* fp, long fileSize)
{
    const int sizeOfElfHeader = 0x34;
    struct ELF_Header_32      elfHeader32 = {{ 0 }};

    unsigned char* buffer = (unsigned char*)malloc(sizeOfElfHeader);
    fread(buffer, sizeof(unsigned char), sizeOfElfHeader, fp);
    char mag_str[4] = { buffer[0], buffer[1], buffer[2], buffer[3] };

    // 32-bit ELF Header
    memcpy(elfHeader32.ei_mag, mag_str, 4);
    elfHeader32.ei_class =      buffer[0x04];
    elfHeader32.ei_data =       buffer[0x05];
    elfHeader32.ei_version =    buffer[0x06];
    elfHeader32.ei_osabi =      buffer[0x07];
    elfHeader32.ei_abiversion = buffer[0x08];
    // ei_pad currently unused
    elfHeader32.e_type =        *(uint16_t*)&buffer[0x10];
    elfHeader32.e_machine =     *(uint16_t*)&buffer[0x12];
    elfHeader32.e_version =     *(uint32_t*)&buffer[0x14];
    elfHeader32.e_entry =       *(uint32_t*)&buffer[0x18];
    elfHeader32.e_phoff =       *(uint32_t*)&buffer[0x1C];
    elfHeader32.e_shoff =       *(uint32_t*)&buffer[0x20];
    elfHeader32.e_flags =       *(uint32_t*)&buffer[0x24];
    elfHeader32.e_ehsize =      *(uint16_t*)&buffer[0x28];
    elfHeader32.e_phentsize =   *(uint16_t*)&buffer[0x2A];
    elfHeader32.e_phnum =       *(uint16_t*)&buffer[0x2C];
    elfHeader32.e_shentsize =   *(uint16_t*)&buffer[0x2E];
    elfHeader32.e_shnum =       *(uint16_t*)&buffer[0x30];
    elfHeader32.e_shstrndx =    *(uint16_t*)&buffer[0x32];
    free(buffer);

    // 32-bit Program Headers
    // if not there already, seek to program header offset found in elfHeader
    if (sizeOfElfHeader != elfHeader32.e_phoff) {
        if (fseek(fp, elfHeader32.e_phoff, SEEK_SET) != 0) {
            printf("[ELF] Error seeking to elfHeader32.e_phoff (parse).\n");
            // TODO: Clean exit in cases like this.
            exit(-1);
        }
    }
    if (elfHeader32.e_phentsize !=
            (unsigned short)sizeof(struct Program_Header_32)) {
        printf("[ELF Fatal Error] elfHeader32.e_phentsize == ");
        printf("sizeof(Program_Header_32)\n[Is file corrupt?]\n");
        // TODO: Clean exit in cases like this.
        exit(-1);
    }

    int i = 0;
    int sizeOfProgramHeaders = elfHeader32.e_phnum * elfHeader32.e_phentsize;
    buffer = (unsigned char*)malloc(sizeOfProgramHeaders);
    fread(buffer, sizeof(unsigned char), sizeOfProgramHeaders, fp);
    struct Program_Header_32* programHeaders =
        (struct Program_Header_32*)malloc(sizeOfProgramHeaders);

    for (i = 0; i < elfHeader32.e_phnum; i++) {
        int cur_phoff = (i * elfHeader32.e_phentsize);
        struct Program_Header_32 cur_programHeader = { 0 };

        cur_programHeader.p_type =          *(uint32_t*)&buffer[cur_phoff+0x00];
        cur_programHeader.p_offset =        *(uint32_t*)&buffer[cur_phoff+0x04];
        cur_programHeader.p_vaddr =         *(uint32_t*)&buffer[cur_phoff+0x08];
        cur_programHeader.p_paddr =         *(uint32_t*)&buffer[cur_phoff+0x0C];
        cur_programHeader.p_filesz =        *(uint32_t*)&buffer[cur_phoff+0x10];
        cur_programHeader.p_memsz =         *(uint32_t*)&buffer[cur_phoff+0x14];
        cur_programHeader.p_flags =         *(uint32_t*)&buffer[cur_phoff+0x18];
        cur_programHeader.p_align =         *(uint32_t*)&buffer[cur_phoff+0x1C];

        memcpy(&programHeaders[i], &cur_programHeader,
            elfHeader32.e_phentsize);
    }
    free(buffer);


    // 32-bit Section Headers
    if (fseek(fp, elfHeader32.e_shoff, SEEK_SET) != 0) {
        printf("[ELF] Error seeking to elfHeader32.e_shoff (parse).\n");
        // TODO: Clean exit in cases like this.
        free(programHeaders);
        exit(-1);
    }
    if (elfHeader32.e_shentsize !=
      (unsigned short)sizeof(struct Section_Header_32)) {
      printf("[ELF Fatal Error] elfHeader32.e_shentsize == ");
        printf("sizeof(Section_Header_32)\n[Is file corrupt?]\n");
        // TODO: Clean exit in cases like this.
        free(programHeaders);
        exit(-1);
    }

    int sizeOfSectionHeaders = elfHeader32.e_shnum * elfHeader32.e_shentsize;
    buffer = (unsigned char*)malloc(sizeOfSectionHeaders);
    fread(buffer, sizeof(unsigned char), sizeOfSectionHeaders, fp);
    struct Section_Header_32* sectionHeaders =
        (struct Section_Header_32*)malloc(sizeOfSectionHeaders);

    for (i = 0; i < elfHeader32.e_shnum; i++) {
        int cur_shoff = (i * elfHeader32.e_shentsize);
        struct Section_Header_32 cur_sectionHeader = { 0 };

        cur_sectionHeader.sh_name =         *(uint32_t*)&buffer[cur_shoff+0x00];
        cur_sectionHeader.sh_type =         *(uint32_t*)&buffer[cur_shoff+0x04];
        cur_sectionHeader.sh_flags =        *(uint32_t*)&buffer[cur_shoff+0x08];
        cur_sectionHeader.sh_address =      *(uint32_t*)&buffer[cur_shoff+0x0C];
        cur_sectionHeader.sh_offset =       *(uint32_t*)&buffer[cur_shoff+0x10];
        cur_sectionHeader.sh_size =         *(uint32_t*)&buffer[cur_shoff+0x14];
        cur_sectionHeader.sh_link =         *(uint32_t*)&buffer[cur_shoff+0x18];
        cur_sectionHeader.sh_info =         *(uint32_t*)&buffer[cur_shoff+0x1C];
        cur_sectionHeader.sh_addralign =    *(uint32_t*)&buffer[cur_shoff+0x20];
        cur_sectionHeader.sh_entsize =      *(uint32_t*)&buffer[cur_shoff+0x24];

        memcpy(&sectionHeaders[i], &cur_sectionHeader,
            elfHeader32.e_shentsize);
    }
    free(buffer);

    // Now, store the shstrtab
    int shstrtab_addr =
        sectionHeaders[elfHeader32.e_shstrndx].sh_offset;
    if (!shstrtab_addr || fseek(fp, shstrtab_addr, SEEK_SET) != 0) {
        printf("[ELF] Error seeking to shstrtab_addr (parse).\n");
        // TODO: Clean exit in cases like this.
        free(programHeaders);
        free(sectionHeaders);
        exit(-1);
    }
    int shstrtab_size = sectionHeaders[elfHeader32.e_shstrndx].sh_size;
    char* shstrtab = (char*)malloc(
      sizeof(char)*shstrtab_size);
    fread(shstrtab, sizeof(char), shstrtab_size, fp);

    // cleanup
    free(programHeaders);
    free(sectionHeaders);
    free(shstrtab);
}


void parseElf64(FILE* fp, long fileSize)
{
    const int sizeOfElfHeader = 0x40;
    struct ELF_Header_64      elfHeader64 = {{ 0 }};

    unsigned char* buffer = (unsigned char*)malloc(sizeOfElfHeader);
    fread(buffer, sizeof(unsigned char), sizeOfElfHeader, fp);
    char mag_str[4] = { buffer[0], buffer[1], buffer[2], buffer[3] };

    // 64-bit ELF Header
    memcpy(elfHeader64.ei_mag, mag_str, 4);
    elfHeader64.ei_class =      buffer[0x04];
    elfHeader64.ei_data =       buffer[0x05];
    elfHeader64.ei_version =    buffer[0x06];
    elfHeader64.ei_osabi =      buffer[0x07];
    elfHeader64.ei_abiversion = buffer[0x08];
    elfHeader64.e_type =        *(uint16_t*)&buffer[0x10];
    elfHeader64.e_machine =     *(uint16_t*)&buffer[0x12];
    elfHeader64.e_version =     *(uint32_t*)&buffer[0x14];
    /* 64-bit differences begin */
    elfHeader64.e_entry =       *(uint64_t*)&buffer[0x18];
    elfHeader64.e_phoff =       *(uint64_t*)&buffer[0x20];
    elfHeader64.e_shoff =       *(uint64_t*)&buffer[0x28];
    elfHeader64.e_flags =       *(uint32_t*)&buffer[0x30];
    elfHeader64.e_ehsize =      *(uint16_t*)&buffer[0x34];
    elfHeader64.e_phentsize =   *(uint16_t*)&buffer[0x36];
    elfHeader64.e_phnum =       *(uint16_t*)&buffer[0x38];
    elfHeader64.e_shentsize =   *(uint16_t*)&buffer[0x3A];
    elfHeader64.e_shnum =       *(uint16_t*)&buffer[0x3C];
    elfHeader64.e_shstrndx =    *(uint16_t*)&buffer[0x3E];
    free(buffer);


    // 64-bit Program Headers
    // if not there already, seek to program header offset found in elfHeader
    if (sizeOfElfHeader != elfHeader64.e_phoff) {
        if (fseek(fp, elfHeader64.e_phoff, SEEK_SET) != 0) {
            printf("[ELF] Error seeking to elfHeader64.e_phoff (parse).\n");
            // TODO: Clean exit in cases like this.
            exit(-1);
        }
    }
    if (elfHeader64.e_phentsize !=
            (unsigned short)sizeof(struct Program_Header_64)) {
        printf("[ELF Fatal Error] elfHeader64.e_phentsize == ");
        printf("sizeof(Program_Header_64)\n[Is file corrupt?]\n");
        // TODO: Clean exit in cases like this.
        exit(-1);
    }

    int i = 0;
    int sizeOfProgramHeaders = elfHeader64.e_phnum * elfHeader64.e_phentsize;
    buffer = (unsigned char*)malloc(sizeOfProgramHeaders);
    fread(buffer, sizeof(unsigned char), sizeOfProgramHeaders, fp);
    struct Program_Header_64* programHeaders =
        (struct Program_Header_64*)malloc(sizeOfProgramHeaders);

    for (i = 0; i < elfHeader64.e_phnum; i++) {
        int cur_phoff = (i * elfHeader64.e_phentsize);
        struct Program_Header_64 cur_programHeader = { 0 };

        cur_programHeader.p_type =          *(uint32_t*)&buffer[cur_phoff+0x00];
        cur_programHeader.p_offset =        *(uint32_t*)&buffer[cur_phoff+0x04];
        cur_programHeader.p_offset =        *(uint64_t*)&buffer[cur_phoff+0x08];
        cur_programHeader.p_vaddr =         *(uint64_t*)&buffer[cur_phoff+0x10];
        cur_programHeader.p_paddr =         *(uint64_t*)&buffer[cur_phoff+0x18];
        cur_programHeader.p_filesz =        *(uint64_t*)&buffer[cur_phoff+0x20];
        cur_programHeader.p_memsz =         *(uint64_t*)&buffer[cur_phoff+0x28];
        cur_programHeader.p_align =         *(uint64_t*)&buffer[cur_phoff+0x30];

        memcpy(&programHeaders[i], &cur_programHeader,
            elfHeader64.e_phentsize);
    }
    free(buffer);


    // 64-bit Section Headers
    if (fseek(fp, elfHeader64.e_shoff, SEEK_SET) != 0) {
        printf("[ELF] Error seeking to elfHeader64.e_shoff (parse).\n");
        // TODO: Clean exit in cases like this.
        free(programHeaders);
        exit(-1);
    }
    if (elfHeader64.e_shentsize !=
      (unsigned short)sizeof(struct Section_Header_64)) {
      printf("[ELF Fatal Error] elfHeader64.e_shentsize == ");
        printf("sizeof(Section_Header_64)\n[Is file corrupt?]\n");
        // TODO: Clean exit in cases like this.
        free(programHeaders);
        exit(-1);
    }

    int sizeOfSectionHeaders = elfHeader64.e_shnum * elfHeader64.e_shentsize;
    buffer = (unsigned char*)malloc(sizeOfSectionHeaders);
    fread(buffer, sizeof(unsigned char), sizeOfSectionHeaders, fp);
    struct Section_Header_64* sectionHeaders =
        (struct Section_Header_64*)malloc(sizeOfSectionHeaders);

    for (i = 0; i < elfHeader64.e_shnum; i++) {
        int cur_shoff = (i * elfHeader64.e_shentsize);
        struct Section_Header_64 cur_sectionHeader = { 0 };

        cur_sectionHeader.sh_name =         *(uint32_t*)&buffer[cur_shoff+0x00];
        cur_sectionHeader.sh_type =         *(uint32_t*)&buffer[cur_shoff+0x04];
        cur_sectionHeader.sh_flags =        *(uint64_t*)&buffer[cur_shoff+0x08];
        cur_sectionHeader.sh_address =      *(uint64_t*)&buffer[cur_shoff+0x10];
        cur_sectionHeader.sh_offset =       *(uint64_t*)&buffer[cur_shoff+0x18];
        cur_sectionHeader.sh_size =         *(uint64_t*)&buffer[cur_shoff+0x20];
        cur_sectionHeader.sh_link =         *(uint32_t*)&buffer[cur_shoff+0x28];
        cur_sectionHeader.sh_info =         *(uint32_t*)&buffer[cur_shoff+0x2C];
        cur_sectionHeader.sh_addralign =    *(uint64_t*)&buffer[cur_shoff+0x30];
        cur_sectionHeader.sh_entsize =      *(uint64_t*)&buffer[cur_shoff+0x38];

        memcpy(&sectionHeaders[i], &cur_sectionHeader,
            elfHeader64.e_shentsize);
    }
    free(buffer);

    // Now, store the shstrtab
    int shstrtab_addr =
        sectionHeaders[elfHeader64.e_shstrndx].sh_offset;
    if (!shstrtab_addr || fseek(fp, shstrtab_addr, SEEK_SET) != 0) {
        printf("[ELF] Error seeking to shstrtab_addr (parse).\n");
        // TODO: Clean exit in cases like this.
        free(programHeaders);
        free(sectionHeaders);
        exit(-1);
    }
    int shstrtab_size = sectionHeaders[elfHeader64.e_shstrndx].sh_size;
    char* shstrtab = (char*)malloc(
      sizeof(char)*shstrtab_size);
    fread(shstrtab, sizeof(char), shstrtab_size, fp);

    printSectionHeader64(sectionHeaders[27], shstrtab);

    // cleanup
    free(programHeaders);
    free(sectionHeaders);
    free(shstrtab);
}
