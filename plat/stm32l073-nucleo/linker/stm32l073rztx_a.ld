MEMORY
{
    IVT  (rx):  ORIGIN = 0x08010000, LENGTH = 256
    META (r):   ORIGIN = 0x08010100, LENGTH = 256
    FLASH (rx): ORIGIN = 0x08010200, LENGTH = 67072
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
