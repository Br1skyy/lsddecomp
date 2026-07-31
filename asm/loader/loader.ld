SECTIONS
{
    .text 0x800027C0 : AT(0x800027C0) SUBALIGN(2)
    {
        FILL(0x00000000);
        loader_TEXT_START = .;
        build_loader/asm/loader/loader.s.o(.text);
        loader_TEXT_END = .;
        loader_TEXT_SIZE = ABSOLUTE(loader_TEXT_END - loader_TEXT_START);
    }
    .bss 0x80003148 : AT(0x80003148)
    {
        FILL(0x00000000);
        loader_BSS_START = .;
        build_loader/asm/loader/loader.s.o(.bss);
        loader_BSS_END = .;
        loader_BSS_SIZE = ABSOLUTE(loader_BSS_END - loader_BSS_START);
    }
    __romPos = ALIGN(0x800027C0 + SIZEOF(.text), 4);
    loader_ROM_END = __romPos;
    loader_VRAM_END = .;

    /DISCARD/ :
    {
        *(.header)
        *(.debug*) *(.comment*) *(.note*) *(.eh_frame*) *(.stab*) *(.mdebug*)
        *(.MIPS.abiflags) *(.reginfo) *(.gnu.attributes) *(.pdr)
    }
}
