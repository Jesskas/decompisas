// Initial reference:
// http://tfpwn.com/blog/mach-o-file-format.html

#include <string.h>
#include <stdint.h>

struct fat_header {
    uint32_t magic;
    uint32_t nfat_arch;
};

struct mach_header {
    uint32_t magic;
    uint32_t cputype;    // typedef as int in mach/machine.h
    uint32_t cpusubtype; // typedef as int in mach/machine.h
    uint32_t filetype;
    uint32_t ncmds;
    uint32_t sizeofcmds;
    uint32_t flags;
};

struct mach_header_64 {



};

struct load_command {
    uint32_t cmd;
    uint32_t cmdsize;
};
struct segment_command {
    uint32_t cmd;
    uint32_t cmdsize;
    char     segname[16];
    uint32_t vmaddr;
    uint32_t vmsize;
    uint32_t fileoff;
    uint32_t filesize;
    uint32_t maxprot;
    uint32_t initprot;
    uint32_t nsects;
    uint32_t flags;
};
struct section {
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
};

void printMachHeader32(struct mach_header header);
void printMachHeader64(struct mach_header_64 header);
