#include "pe.h"

void parseWin32(FILE* fp, long fileSize) {
    // TODO: Handle better! 0x400 seems to be enough for Windows.
    unsigned int width = 0x400;

    // TODO: Populate values in DOS_Header.
    //struct DOS_Header dos_Header = { 0 };
    struct COFFHeader coffHeader = { 0 };
    struct PEOptHeader peOptHeader = { 0 };

    // TODO: The structure here was originally taken from the print() functions
    // in main(), which, featured an actual loop. This is not a true loop.
    // refactor this.
    int endOfFile = 0;
    while (!endOfFile) {
        unsigned char* buffer = (unsigned char*)malloc(width);
        unsigned int amountRead =
            fread(buffer, sizeof(unsigned char), width, fp);
        if (amountRead < width) {
            endOfFile = 1;
        }

        int pePtr = buffer[60];
        printf("PE Header listed to be at offset %d (%02X)\n", pePtr, pePtr);

        if (amountRead < pePtr) { // TODO: Handle separately
            fprintf(stderr, "Invalid file (amountRead < pePtr)\n"); exit(-1);
        }

        const char peCheck[3] = { buffer[pePtr], buffer[pePtr+1] };

        if (strcmp(peCheck, "PE")) { // TODO: Handle separately
            fprintf(stderr, "Invalid file (peCheck != PE)\n"); exit(-1);
        }

        unsigned int coffOff = pePtr+4;

        coffHeader.Machine =                *(uint16_t*)&buffer[coffOff];
        coffHeader.NumberOfSections =       *(uint16_t*)&buffer[coffOff+0x02];
        coffHeader.TimeDateStamp =          *(uint32_t*)&buffer[coffOff+0x04];
        coffHeader.PointerToSymbolTable =   *(uint32_t*)&buffer[coffOff+0x08];
        coffHeader.NumberOfSymbols =        *(uint32_t*)&buffer[coffOff+0x0C];
        coffHeader.SizeOfOptionalHeader =   *(uint16_t*)&buffer[coffOff+0x10];
        coffHeader.Characteristics =        *(uint16_t*)&buffer[coffOff+0x12];

        // Get the PE Optional Header offset
        unsigned int peOff = coffOff + sizeof(struct COFFHeader);

        int filePeOptSigCheck =
            *(uint16_t*)&buffer[peOff];
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

        peOptHeader.signature =                 filePeOptSigCheck;
        peOptHeader.MajorLinkerVersion =        buffer[peOff+0x02];
        peOptHeader.MinorLinkerVersion =        buffer[peOff+0x03];
        peOptHeader.SizeOfCode =                *(uint32_t*)&buffer[peOff+0x04];
        peOptHeader.SizeOfInitializedData =     *(uint32_t*)&buffer[peOff+0x08];
        peOptHeader.SizeOfUninitializedData =   *(uint32_t*)&buffer[peOff+0x0C];
        peOptHeader.AddressOfEntryPoint =       *(uint32_t*)&buffer[peOff+0x10];
        peOptHeader.BaseOfCode =                *(uint32_t*)&buffer[peOff+0x14];
        peOptHeader.BaseOfData =                *(uint32_t*)&buffer[peOff+0x18];
        peOptHeader.ImageBase =                 *(uint32_t*)&buffer[peOff+0x1C];
        peOptHeader.SectionAlignment =          *(uint32_t*)&buffer[peOff+0x20];
        peOptHeader.FileAlignment =             *(uint32_t*)&buffer[peOff+0x24];
        peOptHeader.MajorOSVersion =            *(uint16_t*)&buffer[peOff+0x28];
        peOptHeader.MinorOSVersion =            *(uint16_t*)&buffer[peOff+0x2A];
        peOptHeader.MajorImageVersion =         *(uint16_t*)&buffer[peOff+0x2C];
        peOptHeader.MinorImageVersion =         *(uint16_t*)&buffer[peOff+0x2E];
        peOptHeader.MajorSubsystemVersion =     *(uint16_t*)&buffer[peOff+0x30];
        peOptHeader.MinorSubsystemVersion =     *(uint16_t*)&buffer[peOff+0x32];
        peOptHeader.Win32VersionValue =         *(uint32_t*)&buffer[peOff+0x34];
        peOptHeader.SizeOfImage =               *(uint32_t*)&buffer[peOff+0x38];
        peOptHeader.SizeOfHeaders =             *(uint32_t*)&buffer[peOff+0x3C];
        peOptHeader.Checksum =                  *(uint32_t*)&buffer[peOff+0x40];
        peOptHeader.Subsystem =                 *(uint16_t*)&buffer[peOff+0x44];
        peOptHeader.DLLCharacteristics =        *(uint16_t*)&buffer[peOff+0x46];
        peOptHeader.SizeOfStackReserve =        *(uint32_t*)&buffer[peOff+0x48];
        peOptHeader.SizeOfStackCommit =         *(uint32_t*)&buffer[peOff+0x4C];
        peOptHeader.SizeOfHeapReserve =         *(uint32_t*)&buffer[peOff+0x50];
        peOptHeader.SizeOfHeapCommit =          *(uint32_t*)&buffer[peOff+0x54];
        peOptHeader.LoaderFlags =               *(uint32_t*)&buffer[peOff+0x58];
        peOptHeader.NumberOfRvaAndSizes =       *(uint32_t*)&buffer[peOff+0x5C];

        unsigned int dataDirOffset = peOff + 96;

        peOptHeader.DataDirectory =
            (struct data_directory*)malloc(
                sizeof(struct data_directory)*peOptHeader.NumberOfRvaAndSizes);
        int i;
        for (i = 0; i < peOptHeader.NumberOfRvaAndSizes; i++) {
            struct data_directory data_dir = { 0 };
            data_dir.VirtualAddress = *(uint32_t*)&buffer[dataDirOffset+i*8];
            data_dir.Size =           *(uint32_t*)&buffer[dataDirOffset+i*8+4];

            peOptHeader.DataDirectory[i] = data_dir;
        }

        // Starts at the offset at which ".text" and other sections begin.
        unsigned int secOff = dataDirOffset +
            (peOptHeader.NumberOfRvaAndSizes)*sizeof(struct data_directory);

        struct FCSection* fileCodeSections =
            (struct FCSection*)malloc(
                sizeof(struct FCSection)*coffHeader.NumberOfSections);
        unsigned char* fileCodeSectionBuffers[4];

        for (i = 0; i < coffHeader.NumberOfSections; i++) {
            struct FCSection fileCodeSection = {{ 0 }};

            // SectionName
            char secName[8];
            int j;
            for (j = 0; j < 8; j++)
                secName[j] = buffer[secOff+i*0x28+j];
            memcpy(fileCodeSection.SectionName, secName, sizeof(char)*8);

            fileCodeSection.SectionSize =
                *(uint32_t*)&buffer[secOff+i*0x28+0x08];
            fileCodeSection.RVA =
                *(uint32_t*)&buffer[secOff+i*0x28+0x0C];
            fileCodeSection.PhysicalSize =
                *(uint32_t*)&buffer[secOff+i*0x28+0x10];
            fileCodeSection.PhysicalLocation =
                *(uint32_t*)&buffer[secOff+i*0x28+0x14];
            // TODO: fileCodeSection.Reserved.
            fileCodeSection.SectionFlags =
                *(uint32_t*)&buffer[secOff+i*0x28+0x24];

            fileCodeSections[i] = fileCodeSection;
        }
        free(buffer);


        for (i = 0; i < coffHeader.NumberOfSections; i++) {
            // TODO: Get to location finet's
            if (fseek(fp, 0l, SEEK_SET) != 0) {
                printf("Error seeking back to zero (parseWin32).\n");
            }

            if (fseek(fp, fileCodeSections[i].PhysicalLocation, SEEK_SET) != 0)
            {
                printf("Error seeking to PhysicalLocation.\n");
                printf("Cygwin!GCC Error? Dumping headers:\n");
                printCOFFHeader(coffHeader);
                printPEOptHeader(peOptHeader);
                printFCSection(fileCodeSections[i]);
                exit(-1);
            }

            unsigned int sectionSize = fileCodeSections[i].PhysicalSize;
            unsigned char* sectionBuffer = (unsigned char*)malloc(
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

        break;
    }
}
