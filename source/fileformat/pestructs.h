// Naming conventions taken from
// https://en.wikibooks.org/wiki/X86_Disassembly/Windows_Executable_Files
// May modify later.
// TODO: Methods of displaying this data (prettyprinted)
// TODO: Data struct sizes are assumed, but may not be true for all compilers.
// ^ for example, short is not guaranteed to be 16 bits.

#include <stdio.h>
#include <stdint.h>

struct DOS_Header
{
    uint8_t* signature;
    uint16_t lastsize;
    uint16_t nblocks;
    uint16_t nreloc;
    uint16_t hdrsize;
    uint16_t minalloc;
    uint16_t maxalloc;
    void *ss;
    void *sp;
    uint16_t checksum;
    void *ip;
    void *cs;
    uint16_t relocpos;
    uint16_t noverlay;
    uint16_t reserved1[4];
    uint16_t oem_id;
    uint16_t oem_info;
    uint16_t reserved2[10];
    uint32_t e_lfanew;
};

struct COFFHeader
{
    uint16_t Machine;
    uint16_t NumberOfSections;
    uint32_t TimeDateStamp;
    uint32_t PointerToSymbolTable;
    uint32_t NumberOfSymbols;
    uint16_t SizeOfOptionalHeader;
    uint16_t Characteristics;
};

struct data_directory
{
    uint32_t VirtualAddress;
    uint32_t Size;
};

struct PEOptHeader
{
    uint16_t signature; //decimal number 267 for 32 bit, 523 for 64 bit, and 263 for a ROM image.
    uint8_t MajorLinkerVersion;
    uint8_t MinorLinkerVersion;
    uint32_t SizeOfCode;
    uint32_t SizeOfInitializedData;
    uint32_t SizeOfUninitializedData;
    uint32_t AddressOfEntryPoint;  //The RVA of the code entry point
    uint32_t BaseOfCode;
    uint32_t BaseOfData;
    /*The next 21 fields are an extension to the COFF optional header format*/
    uint32_t ImageBase;
    uint32_t SectionAlignment;
    uint32_t FileAlignment;
    uint16_t MajorOSVersion;
    uint16_t MinorOSVersion;
    uint16_t MajorImageVersion;
    uint16_t MinorImageVersion;
    uint16_t MajorSubsystemVersion;
    uint16_t MinorSubsystemVersion;
    uint32_t Win32VersionValue;
    uint32_t SizeOfImage;
    uint32_t SizeOfHeaders;
    uint32_t Checksum;
    uint16_t Subsystem;
    uint16_t DLLCharacteristics; // TODO: Changed to unsigned to fix bug
    uint32_t SizeOfStackReserve;
    uint32_t SizeOfStackCommit;
    uint32_t SizeOfHeapReserve;
    uint32_t SizeOfHeapCommit;
    uint32_t LoaderFlags;
    uint32_t NumberOfRvaAndSizes;
    struct data_directory* DataDirectory;
    //Can have any number of elements, matching the number in NumberOfRvaAndSizes.
};

//
// TODO: Consider putting this in a different file. My unofficial structs.
//
// Sizeof=0x28. rip.
struct FCSection {
    char SectionName[8];
    uint32_t SectionSize; // size o the section once it is loaded to memory
    uint32_t RVA; // location of section once it is loaded to memory
    uint32_t PhysicalSize; // of section on disk
    uint32_t PhysicalLocation; // of section on disk
    uint32_t Reserved[3]; // usually zero, used in object formats
    uint32_t SectionFlags;
};

void printDOSHeader(struct DOS_Header header);
void printCOFFHeader(struct COFFHeader header);
void printPEOptHeader(struct PEOptHeader header);
void printFCSection(struct FCSection section);
