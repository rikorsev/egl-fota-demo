MEMORY
{
    INFO (r):  ORIGIN = 0x08010000, LENGTH = 256
    ROM  (rx): ORIGIN = 0x08010100, LENGTH = 32256
}

SECTIONS
{
    .info :
    {
        . = ALIGN(4);
        *(.info*)
        . = ALIGN(4);
    } >INFO
}
