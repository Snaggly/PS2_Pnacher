#pragma once

#include <cstdint>
#include <vector>
#include <fstream>

/***
 * Shamelessly copied and translated from:
 * https://github.com/jpmac26/PS2PatchElf/blob/master/PS2PatchLib/ElfUtils.cs
 * 
 * Original code written by jpmac26 and translated to C++ in this fasion by Snaggly
 **/

namespace PS2PNACHER
{
	struct ProgramHeader {
		uint32_t Type;
		uint32_t Offset;
		uint32_t VirtAddr;
		uint32_t PhysAddr;
		uint32_t FileSize;
		uint32_t MemSize;
		uint32_t Flags;
		uint32_t Align;
	};
		
	struct Elf {
		struct ElfHeader {
			struct HeaderIdentifier {
				char Magic[4];
				uint8_t Class;
				uint8_t ByteOrder;
				uint8_t Version;
				uint8_t OSABI;
				uint8_t ABIVersion;
				uint8_t Padding[7];
			};
			HeaderIdentifier Identifier;
			uint16_t Type;
			uint16_t Machine;
			uint32_t Version;
			uint32_t Entry;
			uint32_t PHOffset;
			uint32_t SHOffset;
			uint32_t Flags;
			uint16_t EHSize;
			uint16_t PHEntrySize;
			uint16_t PHNum;
			uint16_t SHEntrySize;
			uint16_t SHNum;
			uint16_t SHStrTblIndex;
		};
		
		ElfHeader Header;
		std::vector<ProgramHeader> ProgramHeaders;
		
	};
	
	Elf parseElfFile(std::istream* file);
}