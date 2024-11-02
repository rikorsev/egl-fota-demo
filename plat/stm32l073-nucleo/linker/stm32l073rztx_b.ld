MEMORY
{
    IVT  (rx):  ORIGIN = 0x08020800, LENGTH = 256
    META (r):   ORIGIN = 0x08020900, LENGTH = 256
    FLASH (rx): ORIGIN = 0x08020A00, LENGTH = 67072
}

SECTIONS
{
    .info :
    {
        . = ALIGN(4);
        *(.info*)
        . = ALIGN(4);
    } >META
}