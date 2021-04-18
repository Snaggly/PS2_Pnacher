#include "PnachTools.h"
#include <fstream>
#include <sstream>

namespace PS2PNACHER {
	
	std::string s_dumm;
	std::string s_address;
	std::string s_length;
	std::string s_data;
	
	PnachTools::PnachTools()
	{
	}

	PnachTools::~PnachTools()
	{
	}

	PnachTools* PnachTools::parsePnachFile(std::string filename) {
		auto result = new PnachTools();
		
		try {
			std::string currentLine;
			std::ifstream patchFile(filename);
			{
				while(std::getline(patchFile,currentLine))
				{
					s_address = "";
					s_length = "";
					s_data = "";
					
					if (!currentLine.rfind("patch", 0)) {
						std::stringstream scanner(currentLine);
						{
							std::getline(scanner,s_dumm,',');
							std::getline(scanner,s_dumm,',');
							std::getline(scanner,s_address,',');
							std::getline(scanner,s_length,',');
							std::getline(scanner,s_data,'/');
						}
						
						PatchData patchInst;
						patchInst.address = (uint32_t)std::stoul(s_address, 0, 16);
						patchInst.length = s_length;
						patchInst.data = (uint32_t)std::stoul(s_data, 0, 16);
						
						result->patches.push_back(patchInst);
					}
				}
			}
			
			if (result->patches.empty()) {
				throw std::exception();
			}
		}
		catch(std::exception &exc) {
			delete result;
			return nullptr;
		}
		
		return result;
	}
}


