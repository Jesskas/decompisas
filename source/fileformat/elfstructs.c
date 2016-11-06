#include "elfstructs.h"

void printElfHeader32(struct ELF_Header_32 header)
{
    printf("32-bit ELF Header:\n\
      \tei_mag =\t%c%c%c%c\n\
      \tei_class =\t%d\n\
      \tei_data =\t%d\n\
      \tei_version =\t%d\n\
      \tei_osabi =\t%d\n\
      \tei_abiversion =\t%d\n\
      \tei_pad =\t0\n\
      \te_type =\t%d\n\
      \te_machine =\t%d\n\
      \te_version =\t%d\n\
      \te_entry =\t0x%x\n\
      \te_phoff =\t0x%x\n\
      \te_shoff =\t0x%x\n\
      \te_flags =\t%d\n\
      \te_ehsize =\t%d\n\
      \te_phentsize =\t%d\n\
      \te_phnum =\t%d\n\
      \te_shentsize =\t%d\n\
      \te_shnum =\t%d\n\
      \te_shstrndx =\t%d\n",
      header.ei_mag[0], header.ei_mag[1], header.ei_mag[2],
      header.ei_mag[3], header.ei_class, header.ei_data,
      header.ei_version, header.ei_osabi, header.ei_abiversion,
      header.e_type, header.e_machine, header.e_version,
      header.e_entry, header.e_phoff, header.e_shoff, header.e_flags,
      header.e_ehsize, header.e_phentsize, header.e_phnum,
      header.e_shentsize, header.e_shnum, header.e_shstrndx);
}

// Same as 32-bit, but %lld for e_entry/e_phoff/e_shoff
void printElfHeader64(struct ELF_Header_64 header)
{
    printf("64-bit ELF Header:\n\
        \tei_mag =\t%c%c%c%c\n\
        \tei_class =\t%d\n\
        \tei_data =\t%d\n\
        \tei_version =\t%d\n\
        \tei_osabi =\t%d\n\
        \tei_abiversion =\t%d\n\
        \tei_pad =\t0\n\
        \te_type =\t%d\n\
        \te_machine =\t%d\n\
        \te_version =\t%d\n\
        \te_entry =\t0x%llX\n\
        \te_phoff =\t0x%llX\n\
        \te_shoff =\t0x%llX\n\
        \te_flags =\t%d\n\
        \te_ehsize =\t%d\n\
        \te_phentsize =\t%d\n\
        \te_phnum =\t%d\n\
        \te_shentsize =\t%d\n\
        \te_shnum =\t%d\n\
        \te_shstrndx =\t%d\n",
        header.ei_mag[0], header.ei_mag[1], header.ei_mag[2],
        header.ei_mag[3], header.ei_class, header.ei_data,
        header.ei_version, header.ei_osabi, header.ei_abiversion,
        header.e_type, header.e_machine, header.e_version,
        header.e_entry, header.e_phoff, header.e_shoff, header.e_flags,
        header.e_ehsize, header.e_phentsize, header.e_phnum,
        header.e_shentsize, header.e_shnum, header.e_shstrndx);
}

void printProgramHeader32(struct Program_Header_32 header)
{
    printf("Program Header:\n\
        \tp_type =\t0x%X\n\
        \tp_offset =\t0x%X\n\
        \tp_vaddr =\t0x%X\n\
        \tp_paddr =\t0x%X\n\
        \tp_filesz =\t0x%X\n\
        \tp_memsz =\t0x%X\n\
        \tp_flags =\t0x%X\n\
        \tp_align =\t0x%X\n",
        header.p_type, header.p_offset, header.p_vaddr,
        header.p_paddr, header.p_filesz, header.p_memsz,
        header.p_flags, header.p_align);
}

void printProgramHeader64(struct Program_Header_64 header)
{
    printf("Program Header:\n\
        \tp_type =\t0x%X\n\
        \tp_offset =\t0x%llX\n\
        \tp_vaddr =\t0x%llX\n\
        \tp_paddr =\t0x%llX\n\
        \tp_filesz =\t0x%llX\n\
        \tp_memsz =\t0x%llX\n\
        \tp_flags =\t0x%X\n\
        \tp_align =\t0x%llX\n",
        header.p_type, header.p_offset, header.p_vaddr,
        header.p_paddr, header.p_filesz, header.p_memsz,
        header.p_flags, header.p_align);
}

void printSectionHeader32(struct Section_Header_32 header,
    char* shstrtab)
{
    if (!shstrtab) {
        printf("Section Header:\n\
            \tsh_name(off.) =\t0x%X\n", header.sh_name);
    } else {
        // TODO: This behavior may be risky
        printf("Section Header:\n\
            \tsh_name =\t");
        int i;
        for (i = 0; shstrtab[header.sh_name+i] != '\0'; i++) {
            printf("%c", shstrtab[header.sh_name+i]);
        }
        printf("\n");
    }

    printf("\
        \tsh_type =\t0x%X\n\
        \tsh_flags =\t0x%X\n\
        \tsh_address =\t0x%X\n\
        \tsh_offset =\t0x%X\n\
        \tsh_size =\t0x%X\n\
        \tsh_link =\t0x%X\n\
        \tsh_info =\t0x%X\n\
        \tsh_addralign =\t0x%X\n\
        \tsh_entsize =\t0x%X\n",
        header.sh_type, header.sh_flags, header.sh_address,
        header.sh_offset, header.sh_size, header.sh_link, header.sh_info,
        header.sh_addralign, header.sh_entsize);
}

void printSectionHeader64(struct Section_Header_64 header,
    char* shstrtab)
{
    if (!shstrtab) {
        printf("Section Header:\n\
            \tsh_name(off.) =\t0x%X\n", header.sh_name);
    } else {
        // TODO: This behavior may be risky
        printf("Section Header:\n\
            \tsh_name =\t");
        int i;
        for (i = 0; shstrtab[header.sh_name+i] != '\0'; i++) {
            printf("%c", shstrtab[header.sh_name+i]);
        }
        printf("\n");
    }

    printf("\
        \tsh_type =\t0x%X\n\
        \tsh_flags =\t0x%llX\n\
        \tsh_address =\t0x%llX\n\
        \tsh_offset =\t0x%llX\n\
        \tsh_size =\t0x%llX\n\
        \tsh_link =\t0x%X\n\
        \tsh_info =\t0x%X\n\
        \tsh_addralign =\t0x%llX\n\
        \tsh_entsize =\t0x%llX\n",
        header.sh_type, header.sh_flags, header.sh_address,
        header.sh_offset, header.sh_size, header.sh_link, header.sh_info,
        header.sh_addralign, header.sh_entsize);
}

// const char* elfSectionHeader32Name(struct ELF_Header_32 header,
//     char* shstrtab)
// {
//  
// }
//
//
// const char* elfSectionHeader64Name(struct ELF_Header_64 header,
//     char* shstrtab)
// {
//
// }
