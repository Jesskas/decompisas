#include "pestructs.h"

void printDOSHeader(struct DOS_Header header)
{
    printf("DOS Header:\n\
      \tsignature =\t%s\n\
      \tlastsize =\t0x%X\n\
      \tnblocks =\t0x%X\n\
      \tnreloc =\t0x%X\n\
      \thdrsize =\t0x%X\n\
      \tminalloc =\t0x%X\n\
      \tmaxalloc =\t0%X\n\
      \tss =\t[not implemented]\n\
      \tsp =\t[not implemented]\n\
      \tchecksum =\t0x%X\n\
      \tip =\t[not implemented]\n\
      \tcs =\t[not implemented]\n\
      \trelocpos =\t0x%X\n\
      \tnoverlay =\t0x%X\n\
      \treserved1 =\t0\n\
      \toem_id =\t0x%X\n\
      \toem_info =\t0x%X\n\
      \treserved2 =\t0\n\
      \te_lfanew =\t0x%X\n",
      header.signature, header.lastsize, header.nblocks,
      header.nreloc, header.hdrsize, header.minalloc,
      header.maxalloc, header.checksum, header.relocpos,
      header.noverlay, header.oem_id, header.oem_info,
      header.e_lfanew);
}

void printCOFFHeader(struct COFFHeader header)
{
    printf("COFF Header:\n\
      \tMachine \t\t= 0x%X\n\
      \tNumberOfSections \t= %d\n\
      \tTimeDateStamp \t\t= 0x%X\n\
      \tPointerToSymbolTable \t= 0x%X\n\
      \tNumberOfSymbols \t= 0x%X\n\
      \tSizeOfOptionalHeader \t= %d\n\
      \tCharacteristics \t= %d\n",
      header.Machine, header.NumberOfSections, header.TimeDateStamp,
      header.PointerToSymbolTable, header.NumberOfSymbols,
      header.SizeOfOptionalHeader,
      header.Characteristics);
}

void printPEOptHeader(struct PEOptHeader header)
{
    printf("PE Opt Header:\n\
      \tsignature \t\t= 0x%X\n\
      \tMajorLinkerVersion \t= 0x%X\n\
      \tMinorLinkerVersion \t= 0x%X\n\
      \tSizeOfCode \t\t= 0x%X\n\
      \tSizeOfInitializedData \t= 0x%X\n\
      \tSizeOfUninitializedData = 0x%X\n\
      \tAddressOfEntryPoint \t= 0x%X\n\
      \tBaseOfCode \t\t= 0x%X\n\
      \tBaseOfData \t\t= 0x%X\n\
      \tImageBase \t\t= 0x%X\n\
      \tSectionAlignment \t= 0x%X\n\
      \tFileAlignment \t\t= 0x%X\n\
      \tMajorOSVersion \t\t= 0x%x\n\
      \tMinorOSVersion \t\t= 0x%X\n\
      \tMajorImageVersion \t= 0x%X\n\
      \tMinorImageVersion \t= 0x%X\n\
      \tMajorSubsystemVersion \t= 0x%X\n\
      \tMinorSubsystemVersion \t= 0x%X\n\
      \tWin32VersionValue \t= 0x%X\n\
      \tSizeOfImage \t\t= 0x%X\n\
      \tSizeOfHeaders \t\t= 0x%X\n\
      \tChecksum \t\t= 0x%X\n\
      \tSubsystem \t\t= 0x%X\n\
      \tDLLCharacteristics \t= 0x%X\n\
      \tSizeOfStackReserve \t= 0x%X\n\
      \tSizeOfStackCommit \t= 0x%X\n\
      \tSizeOfHeapReserve \t= 0x%X\n\
      \tSizeOfHeapCommit \t= 0x%X\n\
      \tLoaderFlags \t\t= 0x%X\n\
      \tNumberOfRvaAndSizes \t= 0x%X\n",
      header.signature, header.MajorLinkerVersion, header.MinorLinkerVersion,
      header.SizeOfCode, header.SizeOfInitializedData,
      header.SizeOfUninitializedData, header.AddressOfEntryPoint,
      header.BaseOfCode, header.BaseOfData, header.ImageBase,
      header.SectionAlignment, header.FileAlignment,
      header.MajorOSVersion, header.MinorOSVersion,
      header.MajorImageVersion, header.MinorImageVersion,
      header.MajorSubsystemVersion, header.MinorSubsystemVersion,
      header.Win32VersionValue, header.SizeOfImage,
      header.SizeOfHeaders, header.Checksum, header.Subsystem,
      header.DLLCharacteristics, header.SizeOfStackReserve,
      header.SizeOfStackCommit, header.SizeOfHeapReserve,
      header.SizeOfHeapCommit, header.LoaderFlags, header.NumberOfRvaAndSizes);

      int i;
      for (i = 0; i < header.NumberOfRvaAndSizes; i++) {
          printf("\tdataDirectory[%d].VirtualAddress =\t0x%X\n\
                  \tdataDirectory[%d].Size =\t0x%X\n",
          i, header.DataDirectory[i].VirtualAddress,
          i, header.DataDirectory[i].Size);
      }
}

void printFCSection(struct FCSection section)
{
    printf("FCSection:\n\
      \tSectionName \t\t= %s\n\
      \tSectionSize \t\t= 0x%X\n\
      \tRVA \t\t\t= 0x%X\n\
      \tPhysicalSize \t\t= 0x%X\n\
      \tPhysicalLocation \t= 0x%X\n\
      \tReserved \t\t= 0x%X\n\
      \tSectionFlags \t\t= 0x%X\n",
      section.SectionName, section.SectionSize, section.RVA,
      section.PhysicalSize, section.PhysicalLocation, *section.Reserved,
      section.SectionFlags);
}
