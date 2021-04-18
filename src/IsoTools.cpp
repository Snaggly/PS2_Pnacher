#include "IsoTools.h"
#include <cstring>

using namespace PS2PNACHER;

IsoTools* IsoTools::readIsoFromFile(const char* path) {
	auto result = new IsoTools();
	try {
		result->iso = iso9660_open(path);
	
		if (result->iso){
			result->isoRootList = iso9660_ifs_readdir(result->iso, "/");
			_CDIO_LIST_FOREACH (result->isoListNode, result->isoRootList) {
				auto record = (iso9660_stat_t *) _cdio_list_node_data(result->isoListNode);
				if (record->type!=1)
					continue;
				IsoRecord newRecord;
				newRecord.filename = record->filename;
				newRecord.location = record->lsn;
				newRecord.size = record->size;
				result->rootDirFilesList.push_back(newRecord);
				//iso9660_name_translate(record->filename, newRecord.filename);
			}
		}
		else{
			throw std::exception();
		}
	}
	catch (std::exception &exc) {
		delete result;
		return nullptr;
	}
	
	return result;
}

const char* IsoTools::getApplicationID() {
	char* result;
	iso9660_ifs_get_application_id(iso, &result);
	return result;
}

IsoRecord* IsoTools::findElfFile() {
	IsoRecord* result = nullptr;
	if (rootDirFilesList.empty())
		return result;
	
	for (auto file : rootDirFilesList) {
		if (std::strlen(file.filename) != 13)
			continue;
		if ((file.filename[0] == 's' || file.filename[0] == 'S') &&
			file.filename[4] == '_' &&
			(file.filename[5] >= '0' && file.filename[5] <= '9') &&
			(file.filename[6] >= '0' && file.filename[6] <= '9') &&
			(file.filename[7] >= '0' && file.filename[7] <= '9') &&
			file.filename[8] == '.' &&
			(file.filename[9] >= '0' && file.filename[9] <= '9') &&
			(file.filename[10] >= '0' && file.filename[10] <= '9')
		) {
			result = &file;
			break;
		}
	}
	
	return result;
}

IsoTools::IsoTools()
{
}

IsoTools::~IsoTools()
{
}


