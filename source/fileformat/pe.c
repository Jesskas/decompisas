#include "pe.h"

void parseWin32(FILE* fp, long fileSize) {
    //struct DOS_Header dos_Header = { 0 }; // TODO: Populate DOS_Header
    struct COFFHeader coffHeader = { 0 };
    struct PEOptHeader peOptHeader = { 0 };

    int sizeOfHeaders = 0x400; // TODO: This can still be done better!
    uint8_t* buffer = malloc(sizeOfHeaders);
    int amountRead = fread(buffer, sizeof(uint8_t), sizeOfHeaders, fp);

    int pePtr = buffer[60];
    printf("PE Header listed to be at offset %d (%02X)\n", pePtr, pePtr);

    if (amountRead < pePtr) { // TODO: Handle separately
        fprintf(stderr, "Invalid file (amountRead < pePtr)\n"); exit(-1);
    }

    const char peCheck[3] = { buffer[pePtr], buffer[pePtr+1] };

    if (strcmp(peCheck, "PE")) { // TODO: Handle separately
        fprintf(stderr, "Invalid file (peCheck != PE)\n"); exit(-1);
    }

    unsigned int coffOffset = pePtr+4;

    coffHeader.Machine              = *(uint16_t*)&buffer[coffOffset];
    coffHeader.NumberOfSections     = *(uint16_t*)&buffer[coffOffset+2];
    coffHeader.TimeDateStamp        = *(uint32_t*)&buffer[coffOffset+4];
    coffHeader.PointerToSymbolTable = *(uint32_t*)&buffer[coffOffset+8];
    coffHeader.NumberOfSymbols      = *(uint32_t*)&buffer[coffOffset+12];
    coffHeader.SizeOfOptionalHeader = *(uint16_t*)&buffer[coffOffset+16];
    coffHeader.Characteristics      = *(uint16_t*)&buffer[coffOffset+18];

    uint32_t peOptOffset = coffOffset + sizeof(struct COFFHeader);

    uint16_t filePeOptSigCheck = *(uint16_t*)&buffer[peOptOffset];
    if (filePeOptSigCheck != 267 && filePeOptSigCheck != 523
        && filePeOptSigCheck != 263) {
        fprintf(stderr, "Incompatible file (filePeOptSigCheck not \
          267, 523, or 263)\nThis may have been created in Cygwin,\
           which is not yet handled.\n");

        printf("Dumping COFFHeader:\n");
        printCOFFHeader(coffHeader);

        exit(-1);
    }

    printf("filePeOptSigCheck = %d (267=32b, 523=64b)\n", filePeOptSigCheck);

    peOptHeader.signature             = filePeOptSigCheck;
    peOptHeader.MajorLinkerVersion    = buffer[peOptOffset+2];
    peOptHeader.MinorLinkerVersion    = buffer[peOptOffset+3];
    peOptHeader.SizeOfCode            = *(uint32_t*)&buffer[peOptOffset+4];
    peOptHeader.SizeOfInitializedData = *(uint32_t*)&buffer[peOptOffset+8];
    peOptHeader.SizeOfUninitializedData=*(uint32_t*)&buffer[peOptOffset+12];
    peOptHeader.AddressOfEntryPoint   = *(uint32_t*)&buffer[peOptOffset+16];
    peOptHeader.BaseOfCode            = *(uint32_t*)&buffer[peOptOffset+20];
    peOptHeader.BaseOfData            = *(uint32_t*)&buffer[peOptOffset+24];
    peOptHeader.ImageBase             = *(uint32_t*)&buffer[peOptOffset+28];
    peOptHeader.SectionAlignment      = *(uint32_t*)&buffer[peOptOffset+32];
    peOptHeader.FileAlignment         = *(uint32_t*)&buffer[peOptOffset+36];
    peOptHeader.MajorOSVersion        = *(uint16_t*)&buffer[peOptOffset+40];
    peOptHeader.MinorOSVersion        = *(uint16_t*)&buffer[peOptOffset+42];
    peOptHeader.MajorImageVersion     = *(uint16_t*)&buffer[peOptOffset+44];
    peOptHeader.MinorImageVersion     = *(uint16_t*)&buffer[peOptOffset+46];
    peOptHeader.MajorSubsystemVersion = *(uint16_t*)&buffer[peOptOffset+48];
    peOptHeader.MinorSubsystemVersion = *(uint16_t*)&buffer[peOptOffset+50];
    peOptHeader.Win32VersionValue     = *(uint32_t*)&buffer[peOptOffset+52];
    peOptHeader.SizeOfImage           = *(uint32_t*)&buffer[peOptOffset+56];
    peOptHeader.SizeOfHeaders         = *(uint32_t*)&buffer[peOptOffset+60];
    peOptHeader.Checksum              = *(uint32_t*)&buffer[peOptOffset+64];
    peOptHeader.Subsystem             = *(uint16_t*)&buffer[peOptOffset+68];
    peOptHeader.DLLCharacteristics    = *(uint16_t*)&buffer[peOptOffset+70];
    peOptHeader.SizeOfStackReserve    = *(uint32_t*)&buffer[peOptOffset+72];
    peOptHeader.SizeOfStackCommit     = *(uint32_t*)&buffer[peOptOffset+76];
    peOptHeader.SizeOfHeapReserve     = *(uint32_t*)&buffer[peOptOffset+80];
    peOptHeader.SizeOfHeapCommit      = *(uint32_t*)&buffer[peOptOffset+84];
    peOptHeader.LoaderFlags           = *(uint32_t*)&buffer[peOptOffset+88];
    peOptHeader.NumberOfRvaAndSizes   = *(uint32_t*)&buffer[peOptOffset+92];

    unsigned int dataDirOffset = peOptOffset + 96;

    peOptHeader.DataDirectory = malloc(
            sizeof(struct data_directory)*peOptHeader.NumberOfRvaAndSizes);
    int i;
    for (i = 0; i < peOptHeader.NumberOfRvaAndSizes; i++) {
        struct data_directory data_dir = { 0 };
        data_dir.VirtualAddress = *(uint32_t*)&buffer[dataDirOffset+i*8];
        data_dir.Size           = *(uint32_t*)&buffer[dataDirOffset+i*8+4];

        peOptHeader.DataDirectory[i] = data_dir;
    }

    // Starts at the offset at which ".text" and other sections begin.
    unsigned int secOff = dataDirOffset +
        (peOptHeader.NumberOfRvaAndSizes)*sizeof(struct data_directory);

    struct FCSection* fileCodeSections =
        malloc(sizeof(struct FCSection)*coffHeader.NumberOfSections);
    unsigned char* fileCodeSectionBuffers[4];

    for (i = 0; i < coffHeader.NumberOfSections; i++) {
        struct FCSection fcSection = {{ 0 }};

        // SectionName
        char secName[8];
        int j;
        for (j = 0; j < 8; j++)
            secName[j] = buffer[secOff+i*0x28+j];
        memcpy(fcSection.SectionName, secName, sizeof(char)*8);

        fcSection.SectionSize       = *(uint32_t*)&buffer[secOff+i*0x28+8];
        fcSection.RVA               = *(uint32_t*)&buffer[secOff+i*0x28+12];
        fcSection.PhysicalSize      = *(uint32_t*)&buffer[secOff+i*0x28+16];
        fcSection.PhysicalLocation  = *(uint32_t*)&buffer[secOff+i*0x28+20];
        fcSection.Reserved[0]       = *(uint32_t*)&buffer[secOff+i*0x28+24];
        fcSection.Reserved[1]       = *(uint32_t*)&buffer[secOff+i*0x28+28];
        fcSection.Reserved[2]       = *(uint32_t*)&buffer[secOff+i*0x28+32];
        fcSection.SectionFlags      = *(uint32_t*)&buffer[secOff+i*0x28+36];

        fileCodeSections[i] = fcSection;
    }
    free(buffer);


    for (i = 0; i < coffHeader.NumberOfSections; i++) {
        // TODO: Get to location finet's
        if (fseek(fp, 0l, SEEK_SET) != 0) {
            printf("Error seeking back to zero (parseWin32).\n");
        }

        if (fseek(fp, fileCodeSections[i].PhysicalLocation, SEEK_SET) != 0) {
            printf("Error seeking to PhysicalLocation.\n");
            printf("Cygwin!GCC Error? Dumping headers:\n");
            printCOFFHeader(coffHeader);
            printPEOptHeader(peOptHeader);
            printFCSection(fileCodeSections[i]);
            exit(-1);
        }

        unsigned int sectionSize = fileCodeSections[i].PhysicalSize;
        unsigned char* sectionBuffer = malloc(
            sizeof(unsigned char)*sectionSize);

        unsigned int amountOfSectionRead = fread(sectionBuffer,
            sizeof(unsigned char), sectionSize, fp);
        if (amountOfSectionRead < sectionSize) {
            // TODO: Check and fail
            printf("amountOfSectionRead < sectionSize\n");
        }

        fileCodeSectionBuffers[i] = sectionBuffer;
    }

    printf("\nDisassembly below:\n\n");
    for (i = 0; i < coffHeader.NumberOfSections; i++) {
        long int executableFlag = 0x20000000; // later, turn into a define

        if (fileCodeSections[i].SectionFlags & executableFlag) {
            if (coffHeader.Machine ==0x8664 || coffHeader.Machine ==0x14c) {
                if (fileCodeSections[i].PhysicalSize != 0) {
                    disassemble_x86(fileCodeSections[i].SectionName,
                        fileCodeSections[i].RVA + peOptHeader.ImageBase,
                        fileCodeSectionBuffers[i],
                        fileCodeSections[i].PhysicalSize);
                } else {
                    printf("%s:0x%08X\t\t; empty executable code section\n",
                        fileCodeSections[i].SectionName,
                        fileCodeSections[i].RVA + peOptHeader.ImageBase);
                }
            } else {
              printf("This architecture is not yet handled.\n");
            }
        }
    }

    // TODO: Free all memory!!
    free(peOptHeader.DataDirectory);
    free(fileCodeSections);
    for (i = 0; i < coffHeader.NumberOfSections; i++) {
        free(fileCodeSectionBuffers[i]);
    }

}
