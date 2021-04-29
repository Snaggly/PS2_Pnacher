#include "mainapp.h"
#include <gtkmm/messagedialog.h>
#include <cstring>
#include <iostream>
#include <filesystem>

using namespace PS2PNACHER;

void MainApp::onPatchBtnClick() {
	if (!pnacher) {
		showError("No Pnach file selected!");
		return;
	}
	
	if (!iso) {
		showError("No ISO file selected!");
		return;
	}
	
	if (!elfFile) {
		showError("No ELF file selected!");
		return;
	}
	
	uint64_t elfLocation = elfFile->location * 0x800;
	
	Elf elfHed;
	try{
		auto elfStr = std::fstream(isoFileSelector->get_filename(), 
				std::ios::in | std::ios::out | std::ios::binary);
		{
			elfStr.seekp(elfLocation);
			elfHed = parseElfFile(&elfStr);
		}
	}catch(std::exception& exc) {
		showError("Patching failed! ELF could either not be parsed or written to!");
		return;
	}
		
	try{
		std::string backupFile = isoFileSelector->get_filename();
		backupFile.append(".bak");
	    std::filesystem::copy_file(isoFileSelector->get_filename(), backupFile, 
							std::filesystem::copy_options::overwrite_existing);
	}catch(std::exception& exc) {
		showError("Failed to create backup!");
		return;
	}
	
	try {
		auto file = std::fstream(isoFileSelector->get_filename(),
				std::ios::in | std::ios::out | std::ios::binary);
		{
			performPatch(&file, pnacher, &elfHed, elfLocation);
		}
	}catch(std::exception& exc) {
		showError("Patching ELF failed. Make sure to reuse the backup!");
		return;
	}
	
	showMessage("Successfully patched iso! Make sure to keep your backup.");
	pnachFileSelector->select_filename("");
	isoFileSelector->select_filename("");
}

void MainApp::onPnachFileBtnClick() {
	if (pnacher) {
		delete pnacher;
	}
	
	pnacher = PnachTools::parsePnachFile(pnachFileSelector->get_filename());
	if (!pnacher){
		pnachFileSelector->select_filename("");
		showError("Selected file could not be parsed!");
	}
}

void MainApp::onIsoFileBtnClick() {
	if (iso) {
		delete iso;
	}
	
	iso = IsoTools::readIsoFromFile(isoFileSelector->get_filename().c_str());
	if (!iso) {
		showError("Selected file is not an ISO!");
		isoFileSelector->select_filename("");
		return;
	}
	
	if (std::strcmp(iso->getApplicationID(),"PLAYSTATION")) {
		showError("Selected ISO is not a PlayStation ISO!");
		isoFileSelector->select_filename("");
		return;
	}
	
	elfFile = iso->findElfFile();
	if (!elfFile) {
		showError("Couldn´t locate Elf in PlayStation ISO!");
		isoFileSelector->select_filename("");
		return;
	}
}

void MainApp::getWidgets() {
	auto lBuilder = Gtk::Builder::create_from_file("/usr/share/ps2pnacher/io.github.Snaggly.PS2_Pnacher_gui.glade");
	lBuilder->get_widget("window", window);
	lBuilder->get_widget("pnachFileSelector", pnachFileSelector);
	lBuilder->get_widget("isoFileSelector", isoFileSelector);
	lBuilder->get_widget("patchBtn", patchBtn);
}

void MainApp::connectSignals() {
	patchBtn->signal_clicked().connect(sigc::mem_fun(*this, &MainApp::onPatchBtnClick));
	pnachFileSelector->signal_file_set().connect(sigc::mem_fun(*this, &MainApp::onPnachFileBtnClick));
	isoFileSelector->signal_file_set().connect(sigc::mem_fun(*this, &MainApp::onIsoFileBtnClick));
}

MainApp::MainApp()
{	
	getWidgets();
	connectSignals();
	
	window->show();
}

MainApp::~MainApp()
{
	delete pnacher;
	delete iso;
	delete elfFile;
}

void MainApp::showError(const char* message) {
	Gtk::MessageDialog errordialog = Gtk::MessageDialog(
			*window,
			message,
			false,
			Gtk::MESSAGE_ERROR,
			Gtk::BUTTONS_OK,
			true
		);
		errordialog.run();
}

void MainApp::showMessage(const char* message) {
	Gtk::MessageDialog messagedialog = Gtk::MessageDialog(
			*window,
			message,
			false,
			Gtk::MESSAGE_INFO,
			Gtk::BUTTONS_OK,
			true
		);
		messagedialog.run();
}
