#pragma once

#include <iostream>
#include <vector>
#include <cstdint>

namespace PS2PNACHER
{
	
	enum TypeLength {
		_byte, 
		_short, 
		_word, 
		_extended
	};
	
	struct PatchData {
		uint32_t address;
		TypeLength length;
		uint32_t data;
	};
	

	class PnachTools
	{
	private:
		PnachTools();
		
	public:
		~PnachTools();
		static PnachTools* parsePnachFile(std::string path);
		std::vector<PatchData> patches;
	};

}