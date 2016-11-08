// Initial reference:
// http://tfpwn.com/blog/mach-o-file-format.html

#include <string.h>
#include <stdio.h>
#include <stdint.h>

/*
 * shorthand of <mach/vm_prot.h>, <mach/machine.h>
 */
typedef uint32_t	cpu_type_t;
typedef uint32_t	cpu_subtype_t;
typedef uint32_t	vm_prot_t;

// struct fat_header {
//     uint32_t magic;
//     uint32_t nfat_arch;
// };

typedef struct mach_header {
    uint32_t      magic;
    cpu_type_t    cputype;    // cpu specifier
    cpu_subtype_t cpusubtype; // machine specifier
    uint32_t      filetype;
    uint32_t      ncmds;
    uint32_t      sizeofcmds;
    uint32_t      flags;
} mach_header;

typedef struct mach_header_64 {
  	uint32_t	    magic;
  	cpu_type_t	  cputype;
  	cpu_subtype_t	cpusubtype;
  	uint32_t	    filetype;
  	uint32_t	    ncmds;
  	uint32_t	    sizeofcmds;
  	uint32_t	    flags;
  	uint32_t	    reserved;
} mach_header_64;

typedef struct segment_command {
    uint32_t      cmd;
    uint32_t      cmdsize;
    char          segname[16];
    uint32_t      vmaddr;
    uint32_t      vmsize;
    uint32_t      fileoff;
    uint32_t      filesize;
    uint32_t      maxprot;    // vm_prot_t
    uint32_t      initprot;   // vm_prot_t
    uint32_t      nsects;
    uint32_t      flags;
} segment_command;

typedef struct segment_command_64 {
    uint32_t      cmd;
    uint32_t      cmdsize;
    char          segname[16];
    uint64_t      vmaddr;
    uint64_t      vmsize;
    uint64_t      fileoff;
    uint64_t      filesize;
    uint32_t      maxprot;    // vm_prot_t
    uint32_t      initprot;   // vm_prot_t
    uint32_t      nsects;
    uint32_t      flags;
} segment_command_64;

// typedef struct load_command {
//     uint32_t cmd;
//     uint32_t cmdsize;
// } load_command;

typedef struct section {
    char        sectname[16];
    char        segname[16];
    uint32_t    addr;
    uint32_t    size;
    uint32_t    offset;
    uint32_t    align;
    uint32_t    reloff;
    uint32_t    nreloc;
    uint32_t    flags;
    uint32_t    reserved1;
    uint32_t    reserved2;
} section;

typedef struct section_64 {
    char        sectname[16];
    char        segname[16];
    uint64_t    addr;
    uint64_t    size;
    uint32_t    offset;
    uint32_t    align;
    uint32_t    reloff;
    uint32_t    nreloc;
    uint32_t    flags;
    uint32_t    reserved1;
    uint32_t    reserved2;
} section_64;

void printMachHeader(mach_header header);
void printMachHeader64(mach_header_64 header);
void printSegmentCommand(segment_command segment);
void printSegmentCommand64(segment_command_64 segment);
void printSection(section sect);
void printSection64(section_64 sect);
