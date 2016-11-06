// Naming conventions taken from
// https://en.wikipedia.org/wiki/Executable_and_Linkable_Format#File_header
// https://www.uclibc.org/docs/elf-64-gen.pdf
// maybe http://fluxius.handgrep.se/2011/10/20/the-art-of-elf-analysises-and-exploitations/
// May modify later.
// TODO: Methods of displaying this data (prettyprinted)
// TODO: Data struct sizes are assumed, but may not be true for all compilers.
// ^ for example, short is not guaranteed to be 16 bits.

// * for now, struct is designated as unused by ei_class = 0.

#include <stdio.h>
#include <string.h>

struct ELF_Header_32
{
    unsigned char ei_mag[4]; // = { 0x7F, 0x45, 0x4C, 0x46 };
    unsigned char ei_class;
    unsigned char ei_data;
    unsigned char ei_version;
    unsigned char ei_osabi;
    unsigned char ei_abiversion;
    unsigned char ei_pad[7];
    unsigned short e_type;
    unsigned short e_machine;
    unsigned int e_version;
    unsigned int e_entry;
    unsigned int e_phoff;
    unsigned int e_shoff;
    unsigned int e_flags;
    unsigned short e_ehsize;
    unsigned short e_phentsize;
    unsigned short e_phnum;
    unsigned short e_shentsize;
    unsigned short e_shnum;
    unsigned short e_shstrndx;
};

struct ELF_Header_64
{
    unsigned char ei_mag[4]; // = { 0x7F, 0x45, 0x4C, 0x46 };
    unsigned char ei_class;
    unsigned char ei_data;
    unsigned char ei_version;
    unsigned char ei_osabi;
    unsigned char ei_abiversion;
    unsigned char ei_pad[7];
    unsigned short e_type;
    unsigned short e_machine;
    unsigned int e_version;
    unsigned long long e_entry;
    unsigned long long e_phoff;
    unsigned long long e_shoff;
    unsigned int e_flags;
    unsigned short e_ehsize;
    unsigned short e_phentsize;
    unsigned short e_phnum;
    unsigned short e_shentsize;
    unsigned short e_shnum;
    unsigned short e_shstrndx;
};

struct Program_Header_32
{
    unsigned int p_type;
    unsigned int p_offset;
    unsigned int p_vaddr;
    unsigned int p_paddr;
    unsigned int p_filesz;
    unsigned int p_memsz;
    unsigned int p_flags;
    unsigned int p_align;
};

// Reference: http://wiki.osdev.org/ELF
struct Program_Header_64
{
    unsigned int p_type;
    unsigned int p_flags;
    unsigned long long p_offset;
    unsigned long long p_vaddr;
    unsigned long long p_paddr;
    unsigned long long p_filesz;
    unsigned long long p_memsz;
    unsigned long long p_align;
};

struct Section_Header_32
{
    unsigned int sh_name;
    unsigned int sh_type;
    unsigned int sh_flags;
    unsigned int sh_address;
    unsigned int sh_offset;
    unsigned int sh_size;
    unsigned int sh_link;
    unsigned int sh_info;
    unsigned int sh_addralign;
    unsigned int sh_entsize;
};

// TODO: I have no reference, I deduced this, may be incorrect.
struct Section_Header_64
{
  unsigned int sh_name;
  unsigned int sh_type;
  unsigned long long sh_flags;
  unsigned long long sh_address;
  unsigned long long sh_offset;
  unsigned long long sh_size;
  unsigned int sh_link;
  unsigned int sh_info;
  unsigned long long sh_addralign;
  unsigned long long sh_entsize;
};

void printElfHeader32(struct ELF_Header_32 header);
void printElfHeader64(struct ELF_Header_64 header);
void printProgramHeader32(struct Program_Header_32 header);
void printProgramHeader64(struct Program_Header_64 header);
void printSectionHeader32(struct Section_Header_32 header,
    char* shstrtab);
void printSectionHeader64(struct Section_Header_64 header,
    char* shstrtab);

// const char* elfSectionHeader32Name(struct ELF_Header_32 header,
//     char* shstrtab);
// const char* elfSectionHeader64Name(struct ELF_Header_64 header,
//     char* shstrtab);
