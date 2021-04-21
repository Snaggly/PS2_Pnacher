#include "Patcher.h"

/***
 * Shamelessly copied and translated from:
 * https://github.com/jpmac26/PS2PatchElf/blob/master/PS2PatchElf/Program.cs
 * 
 * Original code written by jpmac26 and translated to C++ in this fasion by Snaggly
 **/

void PS2PNACHER::performPatch(std::fstream* file, PnachTools* pnacher, Elf* elf, uint64_t filelocation) {
	for (auto patch : pnacher->patches) {
		int8_t cheatDataSize = 1;
		switch (patch.length) {
			case _byte:
				cheatDataSize = 1;
				break;
			case _short:
				cheatDataSize = 2;
				break;
			case _word:
				cheatDataSize = 4;
				break;
			case _extended:
				if (patch.address & 0x20000000)
					cheatDataSize = 4;
				else if (patch.address & 0x10000000)
					cheatDataSize = 2;
				else
					cheatDataSize = 1;
				break;
		}
		
		uint32_t adjustedCheatAddress = patch.address & 0x0FFFFFFF;
		bool foundHeader = false;
		for (auto pHeader : elf->ProgramHeaders) {
			if (pHeader.VirtAddr <= adjustedCheatAddress && (pHeader.VirtAddr + pHeader.FileSize) > adjustedCheatAddress + cheatDataSize) {
				file->seekp(filelocation + pHeader.Offset + (adjustedCheatAddress - pHeader.VirtAddr), std::ios_base::beg);
				file->write((char*)&(patch.data), cheatDataSize);
				foundHeader = true;
				break;
			}
		}
		if (!foundHeader) {
			std::cout << "Could not patch at ";
			std::cout << std::hex << patch.address << std::endl;
		}
	}
}

