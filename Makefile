decompisas: source/main.c source/disassembler/x86.c source/fileformat/pe.c source/fileformat/elf.c source/fileformat/pestructs.c source/fileformat/elfstructs.c
	gcc -o decompisas source/fileformat/*.c source/disassembler/*.c source/main.c -Wall -I.

clean:
	rm decompisas
