// Naming conventions taken from
// https://en.wikibooks.org/wiki/X86_Disassembly/Windows_Executable_Files
// May modify later.
// TODO: Methods of displaying this data (prettyprinted)
// TODO: Data struct sizes are assumed, but may not be true for all compilers.
// ^ for example, short is not guaranteed to be 16 bits.

#include <stdio.h>

struct DOS_Header
{
    char* signature;
    short lastsize;
    short nblocks;
    short nreloc;
    short hdrsize;
    short minalloc;
    short maxalloc;
    void *ss;
    void *sp;
    short checksum;
    void *ip;
    void *cs;
    short relocpos;
    short noverlay;
    short reserved1[4];
    short oem_id;
    short oem_info;
    short reserved2[10];
    unsigned int e_lfanew;
};

struct COFFHeader
{
    short Machine;
    short NumberOfSections;
    unsigned int TimeDateStamp;
    unsigned int PointerToSymbolTable;
    unsigned int NumberOfSymbols;
    short SizeOfOptionalHeader;
    short Characteristics;
};

struct data_directory
{
    unsigned int VirtualAddress;
    unsigned int Size;
};

struct PEOptHeader
{
    short signature; //decimal number 267 for 32 bit, 523 for 64 bit, and 263 for a ROM image.
    char MajorLinkerVersion;
    char MinorLinkerVersion;
    unsigned int SizeOfCode;
    unsigned int SizeOfInitializedData;
    unsigned int SizeOfUninitializedData;
    unsigned int AddressOfEntryPoint;  //The RVA of the code entry point
    unsigned int BaseOfCode;
    unsigned int BaseOfData;
    /*The next 21 fields are an extension to the COFF optional header format*/
    unsigned int ImageBase;
    unsigned int SectionAlignment;
    unsigned int FileAlignment;
    short MajorOSVersion;
    short MinorOSVersion;
    short MajorImageVersion;
    short MinorImageVersion;
    short MajorSubsystemVersion;
    short MinorSubsystemVersion;
    unsigned int Win32VersionValue;
    unsigned int SizeOfImage;
    unsigned int SizeOfHeaders;
    unsigned int Checksum;
    short Subsystem;
    unsigned short DLLCharacteristics; // TODO: Changed to unsigned to fix bug
    unsigned int SizeOfStackReserve;
    unsigned int SizeOfStackCommit;
    unsigned int SizeOfHeapReserve;
    unsigned int SizeOfHeapCommit;
    unsigned int LoaderFlags;
    unsigned int NumberOfRvaAndSizes;
    struct data_directory* DataDirectory;
    //Can have any number of elements, matching the number in NumberOfRvaAndSizes.
};

//
// TODO: Consider putting this in a different file. My unofficial structs.
//
// Sizeof=0x28. rip.
struct FCSection {
    char SectionName[8];
    int SectionSize; // size o the section once it is loaded to memory
    int RVA; // location of section once it is loaded to memory
    int PhysicalSize; // of section on disk
    int PhysicalLocation; // of section on disk
    int Reserved[3]; // usually zero, used in object formats
    int SectionFlags;
};

void printDOSHeader(struct DOS_Header header);
void printCOFFHeader(struct COFFHeader header);
void printPEOptHeader(struct PEOptHeader header);
void printFCSection(struct FCSection section);
