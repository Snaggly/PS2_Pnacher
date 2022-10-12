#include "mainapp.h"
#include "UI.h"
#include <cstring>
#include <iostream>
#include <filesystem>

using namespace PS2PNACHER;

void MainApp::onPatchBtnClick()
{
	if (!pnacher)
	{
		showMessage("No Pnach file selected!");
		return;
	}

	if (!iso)
	{
		showMessage("No ISO file selected!");
		return;
	}

	if (!elfFile)
	{
		showMessage("No ELF file selected!");
		return;
	}

	uint64_t elfLocation = (uint64_t)elfFile->location * 0x800;

	Elf elfHed;
	try{
		auto elfStr = std::fstream(isoFile->get_path(),
				std::ios::in | std::ios::out | std::ios::binary);
		{
			elfStr.seekp(elfLocation);
			elfHed = parseElfFile(&elfStr);
		}
	}catch(std::exception& exc) {
		showMessage("Patching failed! ELF could either not be parsed or written to!");
		return;
	}

	try{
		std::string backupFile = isoFile->get_path();
		backupFile.append(".bak");
		std::filesystem::copy_file(isoFile->get_path(), backupFile,
							std::filesystem::copy_options::overwrite_existing);
	}catch(std::exception& exc) {
		showMessage("Failed to create backup!");
		return;
	}

	try {
		auto file = std::fstream(isoFile->get_path(),
				std::ios::in | std::ios::out | std::ios::binary);
		{
			performPatch(&file, pnacher, &elfHed, elfLocation);
		}
	}catch(std::exception& exc) {
		showMessage("Patching ELF failed. Make sure to reuse the backup!");
		return;
	}

	showMessage("Successfully patched iso! Make sure to keep your backup.");
	resetBtn(pnachFileSelector, pnacher);
	resetBtn(isoFileSelector, iso);
}

void MainApp::onPnachFileBtnClick()
{
	fileChooser = Gtk::FileChooserNative::create(
		"Select Pnach file",
		*window,
		Gtk::FileChooser::Action::OPEN,
		"Select",
		"Cancel");

	auto filter = Gtk::FileFilter::create();
	filter->add_suffix("pnach");
	fileChooser->set_filter(filter);

	fileChooser->signal_response().connect([this](int response)
		{
		if (response == Gtk::ResponseType::ACCEPT)
		{
			if (pnacher != NULL)
				delete pnacher;
			pnachFile = fileChooser->get_file();
			if (std::filesystem::is_directory(pnachFile->get_path()))
			{
				return errorOut<PnachTools>("Directories are not supported!", pnachFileSelector, pnacher);
			}
			pnacher = PnachTools::parsePnachFile(pnachFile->get_path());
			if (!pnacher)
			{
				return errorOut<PnachTools>("Selected file could not be parsed!", pnachFileSelector, pnacher);
			}
			else
			{
				pnachFileSelector->set_label(pnachFile->get_basename());
			}
		} 
		});

	fileChooser->show();
}

void MainApp::onIsoFileBtnClick()
{
	fileChooser = Gtk::FileChooserNative::create(
		"Select ISO file",
		*window,
		Gtk::FileChooser::Action::OPEN,
		"Select",
		"Cancel");
	auto filter = Gtk::FileFilter::create();
	filter->add_suffix("iso");
	fileChooser->set_filter(filter);

	fileChooser->signal_response().connect([this](int response)
	{
	if (response == Gtk::ResponseType::ACCEPT)
	{
		isoFile = fileChooser->get_file();
		if (std::filesystem::is_directory(isoFile->get_path()))
		{
			return errorOut<IsoTools>("Directories are not supported!", isoFileSelector, iso);
		}
		delete iso;
		iso = IsoTools::readIsoFromFile(isoFile->get_path().c_str());
		if (!iso)
		{
			return errorOut<IsoTools>("Selected file is not an ISO!", isoFileSelector, iso);
		}

		if (std::strcmp(iso->getApplicationID(), "PLAYSTATION"))
		{
			return errorOut<IsoTools>("Selected ISO is not a PlayStation ISO!", isoFileSelector, iso);
		}

		delete elfFile;
		elfFile = iso->findElfFile();
		if (!elfFile)
		{
			return errorOut<IsoTools>("Couldn't locate Elf in PlayStation ISO!", isoFileSelector, iso);
		}
		isoFileSelector->set_label(isoFile->get_basename());
	} 
	});

	fileChooser->show();
}

void MainApp::getWidgets()
{
	auto lBuilder = Gtk::Builder::create_from_string(UI::ui_xml);
	window = lBuilder->get_widget<Gtk::Window>("window");
	pnachFileSelector = lBuilder->get_widget<Gtk::Button>("pnachFileSelector");
	isoFileSelector = lBuilder->get_widget<Gtk::Button>("isoFileSelector");
	patchBtn = lBuilder->get_widget<Gtk::Button>("patchBtn");
	createBackupCheck = lBuilder->get_widget<Gtk::CheckButton>("createBackupCeck");
}

void MainApp::connectSignals()
{
	patchBtn->signal_clicked().connect(sigc::mem_fun(*this, &MainApp::onPatchBtnClick));
	pnachFileSelector->signal_clicked().connect(sigc::mem_fun(*this, &MainApp::onPnachFileBtnClick));
	isoFileSelector->signal_clicked().connect(sigc::mem_fun(*this, &MainApp::onIsoFileBtnClick));
}

void MainApp::showMessage(std::string message)
{
	messagedialog = Glib::RefPtr<Gtk::MessageDialog>(new Gtk::MessageDialog(
		*window,
		"",
		false,
		Gtk::MessageType::INFO,
		Gtk::ButtonsType::OK,
		true));
	messagedialog->set_message(message);
	messagedialog->show();
	messagedialog->signal_response().connect([this](int)
	{ 
		messagedialog->destroy();
	});
}

template<typename T> void MainApp::resetBtn(Gtk::Button *btn, T *&t_ptr) {
	delete t_ptr;
	t_ptr = nullptr;
	btn->set_label(PS2PNACHER::UI::defaultNoFile);
}

template<typename T> void MainApp::errorOut(const std::string message, Gtk::Button *btn, T *&t_ptr) {
	resetBtn(btn, t_ptr);
	showMessage(message);
}

MainApp::MainApp()
{
	getWidgets();
	connectSignals();
}

MainApp::~MainApp()
{
	delete pnacher;
	delete iso;
	delete elfFile;

	delete window;
}
