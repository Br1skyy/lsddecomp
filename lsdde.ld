SECTIONS
{
    __romPos = 0;
    header_ROM_START = __romPos;
    header_VRAM = ADDR(.header);
    .header : AT(header_ROM_START) SUBALIGN(2)
    {
        FILL(0x00000000);
        header_DATA_START = .;
        build/asm/lsdde/header.s.o(.data);
        header_DATA_END = .;
        header_DATA_SIZE = ABSOLUTE(header_DATA_END - header_DATA_START);
    }
    __romPos += SIZEOF(.header);
    header_ROM_END = __romPos;
    header_VRAM_END = .;

    main_ROM_START = __romPos;
    main_VRAM = ADDR(.main);
    .main 0x80010000 : AT(main_ROM_START) SUBALIGN(2)
    {
        FILL(0x00000000);
        main_RODATA_START = .;
        build/asm/lsdde/data/800.rodata.s.o(.rodata);
        build/asm/lsdde/data/A8C.rodata.s.o(.rodata);
        build/asm/lsdde/data/ABC.rodata.s.o(.rodata);
        build/asm/lsdde/data/F28.rodata.s.o(.rodata);
        build/asm/lsdde/data/FA4.rodata.s.o(.rodata);
        build/asm/lsdde/data/FD8.rodata.s.o(.rodata);
        build/asm/lsdde/data/1764.rodata.s.o(.rodata);
        build/asm/lsdde/data/1890.rodata.s.o(.rodata);
        build/asm/lsdde/data/19DC.rodata.s.o(.rodata);
        build/asm/lsdde/data/1A90.rodata.s.o(.rodata);
        build/asm/lsdde/data/1C0C.rodata.s.o(.rodata);
        build/asm/lsdde/data/1E28.rodata.s.o(.rodata);
        build/asm/lsdde/data/1EF4.rodata.s.o(.rodata);
        build/src/lsdde/DreamSys.c.o(.rodata);
        build/asm/lsdde/data/206C.rodata.s.o(.rodata);
        build/asm/lsdde/main.s.o(.rodata);
        build/asm/lsdde/psyq_2258.s.o(.rodata);
        build/asm/lsdde/8220.s.o(.rodata);
        build/asm/lsdde/psyq_rcpolyf3.s.o(.rodata);
        build/asm/lsdde/psyq_rcpolyf4.s.o(.rodata);
        build/asm/lsdde/psyq_rcpolyg3.s.o(.rodata);
        build/asm/lsdde/psyq_rcpolyft3.s.o(.rodata);
        build/asm/lsdde/psyq_rcpolygt3.s.o(.rodata);
        build/asm/lsdde/D294.s.o(.rodata);
        build/asm/lsdde/psyq_GsLinkObject4.s.o(.rodata);
        build/asm/lsdde/psyq_15d04.s.o(.rodata);
        build/asm/lsdde/psyq_PadInit.s.o(.rodata);
        build/asm/lsdde/1677C.s.o(.rodata);
        build/asm/lsdde/171E0.s.o(.rodata);
        build/asm/lsdde/179D8.s.o(.rodata);
        build/asm/lsdde/psyq_SpuSetMute.s.o(.rodata);
        build/asm/lsdde/2C054.s.o(.rodata);
        build/asm/lsdde/psyq_memset.s.o(.rodata);
        build/asm/lsdde/psyq_rand.s.o(.rodata);
        build/src/lsdde/StageGrid2.c.o(.rodata);
        build/asm/lsdde/class_39e08.s.o(.rodata);
        build/asm/lsdde/4cd08.s.o(.rodata);
        build/asm/lsdde/Entity.s.o(.rodata);
        build/asm/lsdde/55DD4.s.o(.rodata);
        *(.rodata)
        *(.rodata.str1.4)
        *(.rodata.cst4)
        . = ALIGN(., 4);
        main_RODATA_END = .;
        main_RODATA_SIZE = ABSOLUTE(main_RODATA_END - main_RODATA_START);
        main_TEXT_START = .;
        build/asm/lsdde/main.s.o(.text);
        build/asm/lsdde/cd_stage2.s.o(.text);
        build/asm/lsdde/psyq_2258.s.o(.text);
        build/asm/lsdde/8220.s.o(.text);
        build/asm/lsdde/psyq_rcpolyf3.s.o(.text);
        build/asm/lsdde/psyq_rcpolyf4.s.o(.text);
        build/asm/lsdde/psyq_rcpolyg3.s.o(.text);
        build/asm/lsdde/psyq_rcpolyft3.s.o(.text);
        build/asm/lsdde/psyq_rcpolygt3.s.o(.text);
        build/asm/lsdde/D294.s.o(.text);
        build/asm/lsdde/psyq_GsLinkObject4.s.o(.text);
        build/asm/lsdde/psyq_15d04.s.o(.text);
        build/asm/lsdde/psyq_PadInit.s.o(.text);
        build/asm/lsdde/1677C.s.o(.text);
        build/asm/lsdde/171E0.s.o(.text);
        build/asm/lsdde/179D8.s.o(.text);
        build/asm/lsdde/psyq_SpuSetMute.s.o(.text);
        build/asm/lsdde/2C054.s.o(.text);
        build/asm/lsdde/psyq_memset.s.o(.text);
        build/asm/lsdde/psyq_rand.s.o(.text);
        build/src/lsdde/StageGrid2.c.o(.text);
        build/asm/lsdde/class_39e08.s.o(.text);
        build/src/lsdde/DreamSys.c.o(.text);
        build/src/lsdde/System.c.o(.text);
        build/asm/lsdde/4cd08.s.o(.text);
        build/asm/lsdde/Entity.s.o(.text);
        build/asm/lsdde/55DD4.s.o(.text);
        build/asm/lsdde/stub_fns.s.o(.text);
        build/asm/lsdde/auto_gen_stubs.s.o(.text);
        build/asm/lsdde/DreamSys.s.o(.text);
        build/asm/lsdde/cdrom_handler.s.o(.text);
        build/asm/lsdde/5CAB4.s.o(.text);
        *(.text) *(.text.*)
        . = ALIGN(., 4);
        main_TEXT_END = .;
        main_TEXT_SIZE = ABSOLUTE(main_TEXT_END - main_TEXT_START);
        main_DATA_START = .;
        build/asm/lsdde/data/800.rodata.s.o(.data);
        build/asm/lsdde/data/A8C.rodata.s.o(.data);
        build/asm/lsdde/data/ABC.rodata.s.o(.data);
        build/asm/lsdde/data/F28.rodata.s.o(.data);
        build/asm/lsdde/data/FA4.rodata.s.o(.data);
        build/asm/lsdde/data/FD8.rodata.s.o(.data);
        build/asm/lsdde/data/1764.rodata.s.o(.data);
        build/asm/lsdde/data/1890.rodata.s.o(.data);
        build/asm/lsdde/data/19DC.rodata.s.o(.data);
        build/asm/lsdde/data/1A90.rodata.s.o(.data);
        build/asm/lsdde/data/1C0C.rodata.s.o(.data);
        build/asm/lsdde/data/1E28.rodata.s.o(.data);
        build/asm/lsdde/data/1EF4.rodata.s.o(.data);
        build/asm/lsdde/data/57070.data.s.o(.data);
        build/src/lsdde/StageGrid2.c.o(.data);
        build/asm/lsdde/class_39e08.s.o(.data);
        build/asm/lsdde/4cd08.s.o(.data);
        build/asm/lsdde/Entity.s.o(.data);
        build/asm/lsdde/55DD4.s.o(.data);
        build/asm/lsdde/data/76DC8.data.s.o(.data);
        build/asm/lsdde/data/DreamSys.data.s.o(.data);
        build/asm/lsdde/data/206C.rodata.s.o(.data);
        build/asm/lsdde/main.s.o(.data);
        build/asm/lsdde/psyq_2258.s.o(.data);
        build/asm/lsdde/8220.s.o(.data);
        build/asm/lsdde/psyq_rcpolyf3.s.o(.data);
        build/asm/lsdde/psyq_rcpolyf4.s.o(.data);
        build/asm/lsdde/psyq_rcpolyg3.s.o(.data);
        build/asm/lsdde/psyq_rcpolyft3.s.o(.data);
        build/asm/lsdde/psyq_rcpolygt3.s.o(.data);
        build/asm/lsdde/D294.s.o(.data);
        build/asm/lsdde/psyq_GsLinkObject4.s.o(.data);
        build/asm/lsdde/psyq_15d04.s.o(.data);
        build/asm/lsdde/psyq_PadInit.s.o(.data);
        build/asm/lsdde/1677C.s.o(.data);
        build/asm/lsdde/171E0.s.o(.data);
        build/asm/lsdde/179D8.s.o(.data);
        build/asm/lsdde/psyq_SpuSetMute.s.o(.data);
        build/asm/lsdde/2C054.s.o(.data);
        build/asm/lsdde/psyq_memset.s.o(.data);
        build/asm/lsdde/psyq_rand.s.o(.data);
        build/asm/lsdde/data/79528.data.s.o(.data);
        build/asm/lsdde/auto_gen_stubs.s.o(.data);
        *(.data) *(.data.*)
        . = ALIGN(., 4);
        main_DATA_END = .;
        main_DATA_SIZE = ABSOLUTE(main_DATA_END - main_DATA_START);
        main_SDATA_START = .;
        _gp = . + 0x8000;
        build/asm/lsdde/data/7B008.sdata.s.o(.sdata);
        build/asm/lsdde/data/DreamSys.sdata.s.o(.sdata);
        build/asm/lsdde/data/7B3F8.sdata.s.o(.sdata);
        . = ALIGN(., 4);
        main_SDATA_END = .;
        main_SDATA_SIZE = ABSOLUTE(main_SDATA_END - main_SDATA_START);
        main_SBSS_START = .;
        build/asm/lsdde/data/7B420.sbss.s.o(.sbss);
        build/asm/lsdde/data/7B468.sbss.s.o(.sbss);
        build/asm/lsdde/data/7B46C.sbss.s.o(.sbss);
        build/src/lsdde/DreamSys.c.o(.sbss);
        build/asm/lsdde/data/7B4CC.sbss.s.o(.sbss);
        . = ALIGN(., 4);
        main_SBSS_END = .;
        main_SBSS_SIZE = ABSOLUTE(main_SBSS_END - main_SBSS_START);
        main_BSS_START = .;
        build/asm/lsdde/data/800.rodata.s.o(.bss);
        build/asm/lsdde/data/A8C.rodata.s.o(.bss);
        build/asm/lsdde/data/ABC.rodata.s.o(.bss);
        build/asm/lsdde/data/F28.rodata.s.o(.bss);
        build/asm/lsdde/data/FA4.rodata.s.o(.bss);
        build/asm/lsdde/data/FD8.rodata.s.o(.bss);
        build/asm/lsdde/data/1764.rodata.s.o(.bss);
        build/asm/lsdde/data/1890.rodata.s.o(.bss);
        build/asm/lsdde/data/19DC.rodata.s.o(.bss);
        build/asm/lsdde/data/1A90.rodata.s.o(.bss);
        build/asm/lsdde/data/1C0C.rodata.s.o(.bss);
        build/asm/lsdde/data/1E28.rodata.s.o(.bss);
        build/asm/lsdde/data/1EF4.rodata.s.o(.bss);
        build/src/lsdde/DreamSys.c.o(.bss);
        build/asm/lsdde/data/206C.rodata.s.o(.bss);
        build/asm/lsdde/main.s.o(.bss);
        build/asm/lsdde/psyq_2258.s.o(.bss);
        build/asm/lsdde/8220.s.o(.bss);
        build/asm/lsdde/psyq_rcpolyf3.s.o(.bss);
        build/asm/lsdde/psyq_rcpolyf4.s.o(.bss);
        build/asm/lsdde/psyq_rcpolyg3.s.o(.bss);
        build/asm/lsdde/psyq_rcpolyft3.s.o(.bss);
        build/asm/lsdde/psyq_rcpolygt3.s.o(.bss);
        build/asm/lsdde/D294.s.o(.bss);
        build/asm/lsdde/psyq_GsLinkObject4.s.o(.bss);
        build/asm/lsdde/psyq_15d04.s.o(.bss);
        build/asm/lsdde/psyq_PadInit.s.o(.bss);
        build/asm/lsdde/1677C.s.o(.bss);
        build/asm/lsdde/171E0.s.o(.bss);
        build/asm/lsdde/179D8.s.o(.bss);
        build/asm/lsdde/psyq_SpuSetMute.s.o(.bss);
        build/asm/lsdde/2C054.s.o(.bss);
        build/asm/lsdde/psyq_memset.s.o(.bss);
        build/asm/lsdde/psyq_rand.s.o(.bss);
        build/src/lsdde/StageGrid2.c.o(.bss);
        build/asm/lsdde/class_39e08.s.o(.bss);
        build/asm/lsdde/4cd08.s.o(.bss);
        build/asm/lsdde/Entity.s.o(.bss);
        build/asm/lsdde/55DD4.s.o(.bss);
        *(.bss) *(.bss.*) *(COMMON)
        . = ALIGN(., 4);
        main_BSS_END = .;
        main_BSS_SIZE = ABSOLUTE(main_BSS_END - main_BSS_START);

        /* PS-EXE resident callback tables (must survive GAME.BIN overlay load) */
        . = ALIGN(., 16);
        CdCallbackTable = .;
        . += 16;  /* 4 callbacks * 4 bytes: data_ready, data_sync, ready, complete */
        . = ALIGN(., 16);
        VSyncCallbackTable = .;
        . += 64;  /* 16 entries * 4 bytes */
        . = ALIGN(., 16);
        IntrCallbackTable = .;
        . += 64;  /* 16 entries * 4 bytes */
    }
    __romPos += SIZEOF(.main);
    __romPos = ALIGN(__romPos, 4);
    . = ALIGN(., 4);
    main_ROM_END = __romPos;
    main_VRAM_END = .;

    /DISCARD/ :
    {
        *(.debug*) *(.comment*) *(.note*) *(.eh_frame*) *(.stab*) *(.mdebug*)
        *(.MIPS.abiflags) *(.reginfo) *(.gnu.attributes) *(.pdr)
    }
}

INCLUDE lsdde_defsyms.ld
