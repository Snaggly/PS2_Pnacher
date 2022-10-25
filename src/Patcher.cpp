#include "Patcher.h"

using namespace PS2PNACHER;
	
Patcher::Patcher()
{
}

Patcher::~Patcher()
{
	delete pnacher;
	delete iso;
	delete elfFile;
}

bool Patcher::canPerformPatch(sigc::slot<void (ReturnStatus)>&& handle_status)
{
	if (!pnacher)
	{
		handle_status(NO_PNATCH_FILE);
	}
	else if (!iso)
	{
		handle_status(NO_ISO_FILE);
	}
	else if (!elfFile)
	{
		handle_status(NO_ELF_FILE);
	}
	else
	{
		handle_status(OK);
	}
	
	return (pnacher && iso && elfFile);
}

void Patcher::addPnachFile(const Glib::RefPtr<Gio::File>& pnachFile, sigc::slot<void (ReturnStatus)>&& handle_status)
{
	if (pnacher)
	{
		delete pnacher;
		pnacher = nullptr;
	}
	
	if (std::filesystem::is_directory(pnachFile->get_path()))
	{
		handle_status(DIR_NO_SUPPORT);
		return;
	}
	
	pnacher = PnachTools::parsePnachFile(pnachFile->get_path());
	if (!pnacher)
	{
		handle_status(PARSING_FAILED);
	}
	else
	{
		this->pnachFile = pnachFile;
		handle_status(OK);
	}
}

void Patcher::addIsoFile(const Glib::RefPtr<Gio::File>& isoFile, sigc::slot<void (ReturnStatus)>&& handle_status)
{
	if (iso)
	{
		delete iso;
		iso = nullptr;
	}

	if (elfFile)
	{
		delete elfFile;
		elfFile = nullptr;
	}

	if (std::filesystem::is_directory(isoFile->get_path()))
	{
		handle_status(DIR_NO_SUPPORT);
		return;
	}
	
	iso = IsoTools::readIsoFromFile(isoFile->get_path().c_str());
	if (!iso)
	{
		handle_status(FILE_NOT_ISO);
		return;
	}

	if (std::string(iso->getApplicationID()).compare("PLAYSTATION"))
	{
		handle_status(FILE_NOT_PS2ISO);
		return;
	}

	if (elfFile)
		delete elfFile;
	elfFile = iso->findElfFile();
	if (!elfFile)
	{
		handle_status(ELF_NOT_FOUND);
		return;
	}

	this->isoFile = isoFile;
	handle_status(OK);
}

void Patcher::performPatch(sigc::slot<void (ReturnStatus)>&& handle_status) 
{
	if (!pnacher)
	{
		handle_status(NO_PNATCH_FILE);
		return;
	}
	else if (!iso)
	{
		handle_status(NO_ISO_FILE);
		return;
	}
	else if (!elfFile)
	{
		handle_status(NO_ELF_FILE);
		return;
	}

	uint64_t elfLocation = (uint64_t)elfFile->location * 0x800;

	Elf elfHed;
	try
	{
		auto elfStr = std::fstream(isoFile->get_path(), std::ios::in | std::ios::out | std::ios::binary);
		{
			elfStr.seekp(elfLocation);
			elfHed = parseElfFile(&elfStr);
		}
	} catch(std::exception& exc) 
	{
		handle_status(ELF_PARSING_FAILURE);
		return;
	}

	try
	{
		auto file = std::fstream(isoFile->get_path(), std::ios::in | std::ios::out | std::ios::binary);
		{
/***
 * Shamelessly copied and translated from:
 * https://github.com/jpmac26/PS2PatchElf/blob/master/PS2PatchElf/Program.cs
 * 
 * Original code written by jpmac26 and translated to C++ in this fasion by Snaggly
 **/
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
				for (auto pHeader : elfHed.ProgramHeaders) {
					if (pHeader.VirtAddr <= adjustedCheatAddress && (pHeader.VirtAddr + pHeader.FileSize) > adjustedCheatAddress + cheatDataSize) {
						file.seekp(elfLocation + pHeader.Offset + (adjustedCheatAddress - pHeader.VirtAddr), std::ios_base::beg);
						file.write((char*)&(patch.data), cheatDataSize);
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
	}
	catch(const std::exception& e)
	{
		handle_status(PATCH_WRITE_FAILURE);
		return;
	}

	delete pnacher;
	delete elfFile;
	delete iso;
	pnacher = nullptr;
	elfFile= nullptr;
	iso = nullptr;
	
	handle_status(OK);
}

