namespace PS2PNACHER
{
    enum ReturnStatus
    {
        OK,
        DIR_NO_SUPPORT,
        FILE_NOT_ISO,
        FILE_NOT_PS2ISO,
        ELF_NOT_FOUND,
        PARSING_FAILED,
        NO_PNATCH_FILE,
        NO_ISO_FILE,
        NO_ELF_FILE,
        ELF_PARSING_FAILURE,
        PATCH_WRITE_FAILURE
    };
}