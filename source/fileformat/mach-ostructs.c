#include "mach-ostructs.h"

void printMachHeader(mach_header header)
{
    printf("\t     magic\t= 0x%X\n \
            cputype\t= 0x%X\n \
            cpusubtype\t= 0x%X\n \
            filetype\t= 0x%X\n \
            ncmds\t= 0x%X\n \
            sizzeofcmds= 0x%X\n \
            flags\t= 0x%X\n", header.magic,
            header.cputype, header.cpusubtype,
            header.filetype, header.ncmds, header.sizeofcmds,
            header.flags);
}

void printSegmentCommand(segment_command segment)
{
    printf("\t     cmd\t= 0x%X\n \
            cmdsize\t= 0x%X\n \
            segname\t= %s\n \
            vmaddr\t= 0x%X\n \
            vmsize\t= 0x%X\n \
            fileoff\t= 0x%X\n \
            filesize\t= 0x%X\n \
            maxprot\t= 0x%X\n \
            initprot\t= 0x%X\n \
            nsects\t= 0x%X\n \
            flags\t= 0x%X\n", segment.cmd,
            segment.cmdsize, segment.segname,
            segment.vmaddr, segment.vmsize, segment.fileoff,
            segment.filesize, segment.maxprot, segment.initprot,
            segment.nsects, segment.flags);


}

void printSection(section sect)
{
    printf("\t     sectname\t= %s\n \
            segname\t= %s\n \
            addr\t= 0x%X\n \
            size\t= 0x%X\n \
            offset\t= 0x%X\n \
            align\t= 0x%X\n \
            reloff\t= 0x%X\n \
            nreloc\t= 0x%X\n \
            flags\t= 0x%X\n \
            reserved1\t= 0x%X\n \
            reserved2\t= 0x%X\n", sect.sectname,
            sect.segname, sect.addr,
            sect.size, sect.offset, sect.align,
            sect.reloff, sect.nreloc, sect.flags,
            sect.reserved1, sect.reserved2);

}
