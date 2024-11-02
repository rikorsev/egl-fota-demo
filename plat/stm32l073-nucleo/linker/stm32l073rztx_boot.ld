MEMORY
{
    IVT   (rx): ORIGIN = 0x08000000, LENGTH = 256
    META  (r):  ORIGIN = 0x08000100, LENGTH = 256
    FLASH (rx): ORIGIN = 0x08000200, LENGTH = 65024
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