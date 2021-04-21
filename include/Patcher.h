#pragma once

#include "ElfTools.h"
#include "PnachTools.h"

namespace PS2PNACHER
{

	void performPatch(std::fstream* file, PnachTools* pnacher, Elf* elf, uint64_t filelocation);

}

