#include "mainapp.h"
#include <gtkmm/messagedialog.h>
#include <cstring>
#include <iostream>

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
	
	
	try{
		auto elfStr = std::fstream(isoFileSelector->get_filename(), 
				std::ios::in | std::ios::out | std::ios::binary);
		{
			elfStr.seekp(elfFile->location * 0x800);
			auto elfHed = parseElfFile(&elfStr);
		}
	}catch(std::exception& exc) {
		showError("Patching failed! ELF could either not be parsed or written to!");
	}
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
	auto lBuilder = Gtk::Builder::create_from_file("gui.glade");
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