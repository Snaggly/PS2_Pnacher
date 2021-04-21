#include "ElfTools.h"

using namespace PS2PNACHER;

Elf PS2PNACHER::parseElfFile(std::istream* file) {
	Elf result;
	
	file->read(result.Header.Identifier.Magic, 4);
	if (result.Header.Identifier.Magic[1] != 'E' ||
		result.Header.Identifier.Magic[2] != 'L' ||
		result.Header.Identifier.Magic[3] != 'F') {
			throw std::exception();
		}
	
	file->read((char*)&(result.Header.Identifier.Class), 1);
	if (result.Header.Identifier.Class != 1) {
		throw std::exception();
	}
	file->read((char*)&(result.Header.Identifier.ByteOrder), 1);
	file->read((char*)&(result.Header.Identifier.Version), 1);
	file->read((char*)&(result.Header.Identifier.OSABI), 1);
	file->read((char*)&(result.Header.Identifier.ABIVersion), 1);
	file->read((char*)&(result.Header.Identifier.Padding), 7);
		
	file->read((char*)&(result.Header.Type), sizeof(uint16_t));
	file->read((char*)&(result.Header.Machine), sizeof(uint16_t));
	if (result.Header.Machine != 8) {
		throw std::exception();
	}
	file->read((char*)&(result.Header.Version), sizeof(uint32_t));
	file->read((char*)&(result.Header.Entry), sizeof(uint32_t));
	file->read((char*)&(result.Header.PHOffset), sizeof(uint32_t));
	file->read((char*)&(result.Header.SHOffset), sizeof(uint32_t));
	file->read((char*)&(result.Header.Flags), sizeof(uint32_t));
	file->read((char*)&(result.Header.EHSize), sizeof(uint16_t));
	file->read((char*)&(result.Header.PHEntrySize), sizeof(uint16_t));
	file->read((char*)&(result.Header.PHNum), sizeof(uint16_t));
	file->read((char*)&(result.Header.SHEntrySize), sizeof(uint16_t));
	file->read((char*)&(result.Header.SHNum), sizeof(uint16_t));
	file->read((char*)&(result.Header.SHStrTblIndex), sizeof(uint16_t));
	
	for (char i=0; i<result.Header.PHNum; ++i) {
		ProgramHeader ph;
		uint64_t test = file->tellg();
		file->read((char*)&(ph.Type), sizeof(uint32_t));
		file->read((char*)&(ph.Offset), sizeof(uint32_t));
		file->read((char*)&(ph.VirtAddr), sizeof(uint32_t));
		file->read((char*)&(ph.PhysAddr), sizeof(uint32_t));
		file->read((char*)&(ph.FileSize), sizeof(uint32_t));
		file->read((char*)&(ph.MemSize), sizeof(uint32_t));
		file->read((char*)&(ph.Flags), sizeof(uint32_t));
		file->read((char*)&(ph.Align), sizeof(uint32_t));
		
		result.ProgramHeaders.push_back(ph);
	}
		
	return result;
}

