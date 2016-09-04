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
#include <stdint.h>

struct ELF_Header_32
{
    uint8_t ei_mag[4]; // = { 0x7F, 0x45, 0x4C, 0x46 };
    uint8_t ei_class;
    uint8_t ei_data;
    uint8_t ei_version;
    uint8_t ei_osabi;
    uint8_t ei_abiversion;
    uint8_t ei_pad[7];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint32_t e_entry;
    uint32_t e_phoff;
    uint32_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
};

struct ELF_Header_64
{
    uint8_t ei_mag[4]; // = { 0x7F, 0x45, 0x4C, 0x46 };
    uint8_t ei_class;
    uint8_t ei_data;
    uint8_t ei_version;
    uint8_t ei_osabi;
    uint8_t ei_abiversion;
    uint8_t ei_pad[7];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint64_t e_entry;
    uint64_t e_phoff;
    uint64_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
};

struct Program_Header_32
{
    uint32_t p_type;
    uint32_t p_offset;
    uint32_t p_vaddr;
    uint32_t p_paddr;
    uint32_t p_filesz;
    uint32_t p_memsz;
    uint32_t p_flags;
    uint32_t p_align;
};

// Reference: http://wiki.osdev.org/ELF
struct Program_Header_64
{
    uint32_t p_type;
    uint32_t p_flags;
    uint64_t p_offset;
    uint64_t p_vaddr;
    uint64_t p_paddr;
    uint64_t p_filesz;
    uint64_t p_memsz;
    uint64_t p_align;
};

struct Section_Header_32
{
    uint32_t sh_name;
    uint32_t sh_type;
    uint32_t sh_flags;
    uint32_t sh_address;
    uint32_t sh_offset;
    uint32_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint32_t sh_addralign;
    uint32_t sh_entsize;
};

// TODO: I have no reference, I deduced this, may be incorrect.
struct Section_Header_64
{
  uint32_t sh_name;
  uint32_t sh_type;
  uint64_t sh_flags;
  uint64_t sh_address;
  uint64_t sh_offset;
  uint64_t sh_size;
  uint32_t sh_link;
  uint32_t sh_info;
  uint64_t sh_addralign;
  uint64_t sh_entsize;
};

void printElfHeader32(struct ELF_Header_32 header);
void printElfHeader64(struct ELF_Header_64 header);
void printProgramHeader32(struct Program_Header_32 header);
void printProgramHeader64(struct Program_Header_64 header);
void printSectionHeader32(struct Section_Header_32 header,
    char* shstrtab);
void printSectionHeader64(struct Section_Header_64 header,
    char* shstrtab);
