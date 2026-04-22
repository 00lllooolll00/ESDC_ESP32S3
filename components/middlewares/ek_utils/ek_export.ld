SECTIONS
{
    .ek_export :
    {
        . = ALIGN(4);
        _ek_export_fn_start = .;
        KEEP(*(SORT(.ek_export_fn*)))
        KEEP(*(SORT(.ek_export_fn*)))
        . = ALIGN(4);
        _ek_export_fn_end = .;
    } > flash
}

