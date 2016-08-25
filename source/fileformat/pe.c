#include "pe.h"

// TODO: Evaluate argument "width". I'm forcing it to be 1024 to avoid complications.
void parseWin32(FILE* fp, long fileSize) {
    unsigned int width = 0x400; // TODO: Handle better! 0x400 seems to be enough for Windows.

    //struct DOS_Header dos_Header = { 0 }; // TODO: Populate values in DOS_Header.
    struct COFFHeader coffHeader = { 0 };
    struct PEOptHeader peOptHeader = { 0 };

    // TODO: The structure here was originally taken from the print() functions
    // in main(), which, featured an actual loop. This is not a true loop.
    // refactor this.
    int endOfFile = 0;
    while (!endOfFile) {
        unsigned char* buffer = (unsigned char*)malloc(width);
        unsigned int amountRead = fread(buffer, sizeof(unsigned char), width, fp);
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

        unsigned int coffOffset = pePtr+4;

        // TODO: For debugging/detail functionality, implement printf methods of the struct
        coffHeader.Machine =
            MAKE_SHORT(buffer[coffOffset+1], buffer[coffOffset]);
        coffHeader.NumberOfSections =
            MAKE_SHORT(buffer[coffOffset+3], buffer[coffOffset+2]);
        coffHeader.TimeDateStamp =
            MAKE_LONG(buffer[coffOffset+7], buffer[coffOffset+6],
                buffer[coffOffset+5], buffer[coffOffset+4]);
        coffHeader.PointerToSymbolTable =
            MAKE_LONG(buffer[coffOffset+11], buffer[coffOffset+10],
                buffer[coffOffset+9], buffer[coffOffset+8]);
        coffHeader.NumberOfSymbols =
            MAKE_LONG(buffer[coffOffset+15], buffer[coffOffset+14],
                buffer[coffOffset+13], buffer[coffOffset+12]);
        coffHeader.SizeOfOptionalHeader =
            MAKE_SHORT(buffer[coffOffset+17], buffer[coffOffset+16]);
        coffHeader.Characteristics =
            MAKE_SHORT(buffer[coffOffset+19], buffer[coffOffset+18]);

        unsigned int peOptOffset = coffOffset + sizeof(struct COFFHeader);

        int filePeOptSigCheck =
            MAKE_SHORT(buffer[peOptOffset+1], buffer[peOptOffset]);
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

        peOptHeader.signature = filePeOptSigCheck;
        peOptHeader.MajorLinkerVersion = buffer[peOptOffset+2];
        peOptHeader.MinorLinkerVersion = buffer[peOptOffset+3];
        peOptHeader.SizeOfCode =
            MAKE_LONG(buffer[peOptOffset+7], buffer[peOptOffset+6],
                buffer[peOptOffset+5], buffer[peOptOffset+4]);
        peOptHeader.SizeOfInitializedData =
            MAKE_LONG(buffer[peOptOffset+11], buffer[peOptOffset+10],
                buffer[peOptOffset+9], buffer[peOptOffset+8]);
        peOptHeader.SizeOfUninitializedData =
            MAKE_LONG(buffer[peOptOffset+15], buffer[peOptOffset+14],
                buffer[peOptOffset+13], buffer[peOptOffset+12]);
        peOptHeader.AddressOfEntryPoint =
            MAKE_LONG(buffer[peOptOffset+19], buffer[peOptOffset+18],
                buffer[peOptOffset+17], buffer[peOptOffset+16]);
        peOptHeader.BaseOfCode =
            MAKE_LONG(buffer[peOptOffset+23], buffer[peOptOffset+22],
                buffer[peOptOffset+21], buffer[peOptOffset+20]);
        peOptHeader.BaseOfData =
            MAKE_LONG(buffer[peOptOffset+27], buffer[peOptOffset+22],
                buffer[peOptOffset+25], buffer[peOptOffset+24]);
        peOptHeader.ImageBase =
            MAKE_LONG(buffer[peOptOffset+31], buffer[peOptOffset+30],
                buffer[peOptOffset+29], buffer[peOptOffset+28]);
        peOptHeader.SectionAlignment =
            MAKE_LONG(buffer[peOptOffset+35], buffer[peOptOffset+34],
                buffer[peOptOffset+33], buffer[peOptOffset+32]);
        peOptHeader.FileAlignment =
            MAKE_LONG(buffer[peOptOffset+39], buffer[peOptOffset+38],
                buffer[peOptOffset+37], buffer[peOptOffset+36]);
        peOptHeader.MajorOSVersion =
            MAKE_SHORT(buffer[peOptOffset+41], buffer[peOptOffset+40]);
        peOptHeader.MinorOSVersion =
            MAKE_SHORT(buffer[peOptOffset+43], buffer[peOptOffset+42]);
        peOptHeader.MajorImageVersion =
            MAKE_SHORT(buffer[peOptOffset+45], buffer[peOptOffset+44]);
        peOptHeader.MinorImageVersion =
            MAKE_SHORT(buffer[peOptOffset+47], buffer[peOptOffset+46]);
        peOptHeader.MajorSubsystemVersion =
            MAKE_SHORT(buffer[peOptOffset+49], buffer[peOptOffset+48]);
        peOptHeader.MinorSubsystemVersion =
            MAKE_SHORT(buffer[peOptOffset+51], buffer[peOptOffset+50]);
        peOptHeader.Win32VersionValue =
            MAKE_LONG(buffer[peOptOffset+55], buffer[peOptOffset+54],
                buffer[peOptOffset+53], buffer[peOptOffset+52]);
        peOptHeader.SizeOfImage =
            MAKE_LONG(buffer[peOptOffset+59], buffer[peOptOffset+58],
                buffer[peOptOffset+57], buffer[peOptOffset+56]);
        peOptHeader.SizeOfHeaders =
            MAKE_LONG(buffer[peOptOffset+63], buffer[peOptOffset+62],
                buffer[peOptOffset+61], buffer[peOptOffset+60]);
        peOptHeader.Checksum =
            MAKE_LONG(buffer[peOptOffset+67], buffer[peOptOffset+66],
                buffer[peOptOffset+65], buffer[peOptOffset+64]);
        peOptHeader.Subsystem =
            MAKE_SHORT(buffer[peOptOffset+69], buffer[peOptOffset+68]);
        peOptHeader.DLLCharacteristics =
            MAKE_SHORT(buffer[peOptOffset+71], buffer[peOptOffset+70]);
        peOptHeader.SizeOfStackReserve =
            MAKE_LONG(buffer[peOptOffset+75], buffer[peOptOffset+74],
                buffer[peOptOffset+73], buffer[peOptOffset+72]);
        peOptHeader.SizeOfStackCommit =
            MAKE_LONG(buffer[peOptOffset+79], buffer[peOptOffset+78],
                buffer[peOptOffset+77], buffer[peOptOffset+76]);
        peOptHeader.SizeOfHeapReserve =
            MAKE_LONG(buffer[peOptOffset+83], buffer[peOptOffset+82],
                buffer[peOptOffset+81], buffer[peOptOffset+80]);
        peOptHeader.SizeOfHeapCommit =
            MAKE_LONG(buffer[peOptOffset+87], buffer[peOptOffset+86],
                buffer[peOptOffset+85], buffer[peOptOffset+84]);
        peOptHeader.LoaderFlags =
            MAKE_LONG(buffer[peOptOffset+91], buffer[peOptOffset+90],
                buffer[peOptOffset+89], buffer[peOptOffset+88]);
        peOptHeader.NumberOfRvaAndSizes =
            MAKE_LONG(buffer[peOptOffset+95], buffer[peOptOffset+94],
                buffer[peOptOffset+93], buffer[peOptOffset+92]);

        unsigned int dataDirOffset = peOptOffset + 96;

        peOptHeader.DataDirectory =
            (struct data_directory*)malloc(
                sizeof(struct data_directory)*peOptHeader.NumberOfRvaAndSizes);
        int i;
        for (i = 0; i < peOptHeader.NumberOfRvaAndSizes; i++) {
            struct data_directory data_dir = { 0 };
            data_dir.VirtualAddress =
                MAKE_LONG(buffer[dataDirOffset+i*8+3], buffer[dataDirOffset+i*8+2],
                    buffer[dataDirOffset+i*8+1], buffer[dataDirOffset+i*8]);
            data_dir.Size =
                MAKE_LONG(buffer[dataDirOffset+i*8+7], buffer[dataDirOffset+i*8+6],
                    buffer[dataDirOffset+i*8+5], buffer[dataDirOffset+i*8+4]);

            peOptHeader.DataDirectory[i] = data_dir;
        }

        // Starts at the offset at which ".text" and other sections begin.
        unsigned int sectionOffset = dataDirOffset +
            (peOptHeader.NumberOfRvaAndSizes)*sizeof(struct data_directory);

        struct FCSection* fileCodeSections =
            (struct FCSection*)malloc(sizeof(struct FCSection)*coffHeader.NumberOfSections);
        unsigned char* fileCodeSectionBuffers[4];

        for (i = 0; i < coffHeader.NumberOfSections; i++) {
            struct FCSection fileCodeSection = {{ 0 }};

            // SectionName
            char secName[8];
            int j;
            for (j = 0; j < 8; j++)
                secName[j] = buffer[sectionOffset+i*0x28+j];
            memcpy(fileCodeSection.SectionName, secName, sizeof(char)*8);

            fileCodeSection.SectionSize =
                MAKE_LONG(buffer[sectionOffset+i*0x28+11], buffer[sectionOffset+i*0x28+10],
                buffer[sectionOffset+i*0x28+9], buffer[sectionOffset+i*0x28+8]);
            fileCodeSection.RVA =
                MAKE_LONG(buffer[sectionOffset+i*0x28+15], buffer[sectionOffset+i*0x28+14],
                buffer[sectionOffset+i*0x28+13], buffer[sectionOffset+i*0x28+12]);
            fileCodeSection.PhysicalSize =
                MAKE_LONG(buffer[sectionOffset+i*0x28+19], buffer[sectionOffset+i*0x28+18],
                buffer[sectionOffset+i*0x28+17], buffer[sectionOffset+i*0x28+16]);
            fileCodeSection.PhysicalLocation =
                MAKE_LONG(buffer[sectionOffset+i*0x28+23], buffer[sectionOffset+i*0x28+22],
                buffer[sectionOffset+i*0x28+21], buffer[sectionOffset+i*0x28+20]);
            // TODO: fileCodeSection.Reserved.
            fileCodeSection.SectionFlags =
                MAKE_LONG(buffer[sectionOffset+i*0x28+39], buffer[sectionOffset+i*0x28+38],
                buffer[sectionOffset+i*0x28+37], buffer[sectionOffset+i*0x28+36]);

            fileCodeSections[i] = fileCodeSection;
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
