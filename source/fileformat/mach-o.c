#include "mach-o.h"

void parseMach32(FILE* fp, long fileSize)
{
    int segCounter = 0, sectCounter = 0, fileIndex = 0;

    mach_header machHeader = { 0 };

    uint8_t* buffer = malloc(sizeof(mach_header));
    fread(buffer, sizeof(uint8_t), sizeof(mach_header), fp);
    machHeader.magic      = *(uint32_t*)&buffer[0x00];
    machHeader.cputype    = *(uint32_t*)&buffer[0x04];
    machHeader.cpusubtype = *(uint32_t*)&buffer[0x08];
    machHeader.filetype   = *(uint32_t*)&buffer[0x0C];
    machHeader.ncmds      = *(uint32_t*)&buffer[0x10];
    machHeader.sizeofcmds = *(uint32_t*)&buffer[0x14];
    machHeader.flags      = *(uint32_t*)&buffer[0x18];
    free(buffer);
    printMachHeader(machHeader);
    fileIndex += sizeof(mach_header);

    // TODO: Review. 4?
    for (segCounter = 0; segCounter < 4; segCounter++) {
        segment_command segmentCmd = { 0 };

        buffer = malloc(sizeof(segment_command));
        fseek(fp, fileIndex, SEEK_SET);
        fread(buffer, sizeof(uint8_t), sizeof(segment_command), fp);
        segmentCmd.cmd        = *(uint32_t*)&buffer[0x00];
        segmentCmd.cmdsize    = *(uint32_t*)&buffer[0x04];
        strcpy(segmentCmd.segname, (char*)&buffer[0x08]);
        segmentCmd.vmaddr     = *(uint32_t*)&buffer[0x18];
        segmentCmd.vmsize     = *(uint32_t*)&buffer[0x1C];
        segmentCmd.fileoff    = *(uint32_t*)&buffer[0x20];
        segmentCmd.filesize   = *(uint32_t*)&buffer[0x24];
        segmentCmd.maxprot    = *(uint32_t*)&buffer[0x28];
        segmentCmd.initprot   = *(uint32_t*)&buffer[0x2C];
        segmentCmd.nsects     = *(uint32_t*)&buffer[0x30];
        segmentCmd.flags      = *(uint32_t*)&buffer[0x34];
        free(buffer);

        printf("\n");
        printSegmentCommand(segmentCmd);

        for (sectCounter = 0; sectCounter < segmentCmd.nsects; sectCounter++) {
            section sect = { 0 };

            buffer = malloc(sizeof(section));
            int seek = fileIndex + sizeof(segment_command)
                       + sectCounter * sizeof(section);
            fseek(fp, seek, SEEK_SET);
            fread(buffer, sizeof(uint8_t), sizeof(section), fp);
            strcpy(sect.sectname, (char*)&buffer[0x00]);
            strcpy(sect.segname, (char*)&buffer[0x10]);
            sect.addr       = *(uint32_t*)&buffer[0x20];
            sect.size       = *(uint32_t*)&buffer[0x24];
            sect.offset     = *(uint32_t*)&buffer[0x28];
            sect.align      = *(uint32_t*)&buffer[0x2C];
            sect.reloff     = *(uint32_t*)&buffer[0x30];
            sect.nreloc     = *(uint32_t*)&buffer[0x34];
            sect.flags      = *(uint32_t*)&buffer[0x38];
            sect.reserved1  = *(uint32_t*)&buffer[0x3C];
            sect.reserved2  = *(uint32_t*)&buffer[0x40];
            free(buffer);

            printf("\n");
            printSection(sect);

            // Print disassembly
            uint8_t* codeSection = malloc(sect.size);
            fseek(fp, sect.offset, SEEK_SET); // TODO: Error checking
            fread(codeSection, sizeof(uint8_t), sect.size, fp);
            disassemble_x86(
                sect.sectname,
                sect.addr,
                codeSection,
                sect.size);
            free(codeSection);
        }

        fileIndex += segmentCmd.cmdsize;
    }
}

void parseMach64(FILE* fp, long fileSize)
{
    int segCounter = 0, sectCounter = 0, fileIndex = 0;

    mach_header_64 machHeader = { 0 };

    uint8_t* buffer = malloc(sizeof(mach_header_64));
    fread(buffer, sizeof(uint8_t), sizeof(mach_header_64), fp);
    machHeader.magic      = *(uint32_t*)&buffer[0x00];
    machHeader.cputype    = *(uint32_t*)&buffer[0x04];
    machHeader.cpusubtype = *(uint32_t*)&buffer[0x08];
    machHeader.filetype   = *(uint32_t*)&buffer[0x0C];
    machHeader.ncmds      = *(uint32_t*)&buffer[0x10];
    machHeader.sizeofcmds = *(uint32_t*)&buffer[0x14];
    machHeader.flags      = *(uint32_t*)&buffer[0x18];
    machHeader.reserved   = *(uint32_t*)&buffer[0x1C];
    free(buffer);
    printMachHeader64(machHeader);
    fileIndex += sizeof(mach_header_64);

    // TODO: Review. 4?
    for (segCounter = 0; segCounter < 4; segCounter++) {
        segment_command_64 segmentCmd = { 0 };

        buffer = malloc(sizeof(segment_command_64));
        fseek(fp, fileIndex, SEEK_SET);
        fread(buffer, sizeof(uint8_t), sizeof(segment_command_64), fp);
        segmentCmd.cmd        = *(uint32_t*)&buffer[0x00];
        segmentCmd.cmdsize    = *(uint32_t*)&buffer[0x04];
        strcpy(segmentCmd.segname, (char*)&buffer[0x08]);
        segmentCmd.vmaddr     = *(uint32_t*)&buffer[0x18];
        segmentCmd.vmsize     = *(uint32_t*)&buffer[0x20];
        segmentCmd.fileoff    = *(uint32_t*)&buffer[0x28];
        segmentCmd.filesize   = *(uint32_t*)&buffer[0x30];
        segmentCmd.maxprot    = *(uint32_t*)&buffer[0x38];
        segmentCmd.initprot   = *(uint32_t*)&buffer[0x3C];
        segmentCmd.nsects     = *(uint32_t*)&buffer[0x40];
        segmentCmd.flags      = *(uint32_t*)&buffer[0x44];
        free(buffer);

        printf("\n");
        printSegmentCommand64(segmentCmd);

        for (sectCounter = 0; sectCounter < segmentCmd.nsects; sectCounter++) {
            section_64 sect = { 0 };

            buffer = malloc(sizeof(section_64));
            int seek = fileIndex + sizeof(segment_command_64)
                       + sectCounter * sizeof(section_64);
            fseek(fp, seek, SEEK_SET);
            fread(buffer, sizeof(uint8_t), sizeof(section_64), fp);
            strcpy(sect.sectname, (char*)&buffer[0x00]);
            strcpy(sect.segname, (char*)&buffer[0x10]);
            sect.addr       = *(uint32_t*)&buffer[0x20];
            sect.size       = *(uint32_t*)&buffer[0x28];
            sect.offset     = *(uint32_t*)&buffer[0x30];
            sect.align      = *(uint32_t*)&buffer[0x34];
            sect.reloff     = *(uint32_t*)&buffer[0x38];
            sect.nreloc     = *(uint32_t*)&buffer[0x3C];
            sect.flags      = *(uint32_t*)&buffer[0x40];
            sect.reserved1  = *(uint32_t*)&buffer[0x44];
            sect.reserved2  = *(uint32_t*)&buffer[0x48];
            free(buffer);

            printf("\n");
            printSection64(sect);

            // Print disassembly
            // TODO: At the time of writing this, I do not have a
            // x86_64 disassembler.
            // uint8_t* codeSection = malloc(sect.size);
            // fseek(fp, sect.offset, SEEK_SET); // TODO: Error checking
            // fread(codeSection, sizeof(uint8_t), sect.size, fp);
            // disassemble_x86(
            //     sect.sectname,
            //     sect.addr,
            //     codeSection,
            //     sect.size);
            // free(codeSection);
        }
        fileIndex += segmentCmd.cmdsize;
    }
}
