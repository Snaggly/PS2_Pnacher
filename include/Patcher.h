#pragma once

#include "ElfTools.h"
#include "PnachTools.h"
#include "IsoTools.h"
#include "ReturnStatus.h"
#include <giomm/file.h>
#include <string>
#include <filesystem>


namespace PS2PNACHER
{
	class Patcher
	{
	private:
		PnachTools* pnacher = nullptr;
		IsoTools* iso = nullptr;
  		IsoRecord* elfFile = nullptr;

	public:
		Patcher();
		~Patcher();
		void performPatch(sigc::slot<void (ReturnStatus)>&& handle_status);
		void addPnachFile(const Glib::RefPtr<Gio::File>& pnachFile, sigc::slot<void (ReturnStatus)>&& handle_status);
		void addIsoFile(const Glib::RefPtr<Gio::File>& isoFile, sigc::slot<void (ReturnStatus)>&& handle_status);
		Glib::RefPtr<Gio::File> isoFile;
		Glib::RefPtr<Gio::File> pnachFile;
		bool canPerformPatch(sigc::slot<void (ReturnStatus)>&& handle_status);
	};
}