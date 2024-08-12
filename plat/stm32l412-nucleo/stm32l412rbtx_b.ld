MEMORY
{
    META (r):  ORIGIN = 0x08018000, LENGTH = 256
    ROM  (rx): ORIGIN = 0x08018100, LENGTH = 32256
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
