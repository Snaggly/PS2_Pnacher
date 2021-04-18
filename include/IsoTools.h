#pragma once

#include <cdio/iso9660.h>
#include <vector>

namespace PS2PNACHER
{
	struct IsoRecord {
		const char* filename;
		uint32_t location;
		uint32_t size;
	};

	class IsoTools
	{
	public:
		~IsoTools();
		static IsoTools* readIsoFromFile(const char* path);
		const char* getApplicationID();
		IsoRecord* findElfFile();
		std::vector<IsoRecord> rootDirFilesList;
			
	private:
		IsoTools();
		CdioList_t *isoRootList;
		CdioListNode_t *isoListNode;
		iso9660_t *iso;
		iso9660_stat_t* record;
	};

}