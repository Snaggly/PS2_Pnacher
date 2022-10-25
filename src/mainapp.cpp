#include "mainapp.h"
#include "UI.h"
#include <cstring>
#include <iostream>
#include <filesystem>

using namespace PS2PNACHER;

void MainApp::on_backupfile_click(int response_id)
{
	if (response_id != Gtk::ResponseType::ACCEPT) return;

	auto backupFile = fileChooser->get_file();
	patcher->isoFile->copy_async(
		backupFile,
		[this] (goffset current_num_bytes, goffset total_num_bytes)
		{
			std::stringstream stream;
			stream << std::fixed << std::setprecision(2) << ((current_num_bytes / (float)total_num_bytes) * 100);
			progressLabel->set_label(stream.str().append(" %"));
		},
		[this] (Glib::RefPtr<Gio::AsyncResult> &result)
		{
			progressLabel->set_label("");
			try
			{
				if (patcher->isoFile->copy_finish(result))
				{
					startPatch();
				}
				else
				{
					showMessage("Failed to create backup!");
				}
			}
			catch (Glib::Error &glibError)
			{
				std::cout << glibError.what();
				showMessage("Failed to create backup!");
			}
		},
		Gio::File::CopyFlags::OVERWRITE
	);
}

void MainApp::on_isofile_click(int response_id)
{
	if (response_id == Gtk::ResponseType::ACCEPT)
	{
		patcher->addIsoFile(fileChooser->get_file(), [this] (auto status)
		{
			switch (status)
			{
			case DIR_NO_SUPPORT:
				return errorOut("Directories are not supported!", isoFileSelector);
			case FILE_NOT_ISO:
				return errorOut("Selected file is not an ISO!", isoFileSelector);
			case FILE_NOT_PS2ISO:
				return errorOut("Selected ISO is not a PlayStation ISO!", isoFileSelector);
			case ELF_NOT_FOUND:
				return errorOut("Couldn't locate Elf in PlayStation ISO!", isoFileSelector);
			case OK:
				isoFileSelector->set_label(patcher->isoFile->get_basename());
				break;
			default:
				return errorOut("Something unexpected happened!", isoFileSelector);
			}
		});
	}
}

void MainApp::on_pnachfile_click(int response_id)
{
	if (response_id == Gtk::ResponseType::ACCEPT)
	{
		patcher->addPnachFile(fileChooser->get_file(), [this] (auto status)
		{
			switch(status)
			{
			case DIR_NO_SUPPORT:
				return errorOut("Directories are not supported!", pnachFileSelector);
			case PARSING_FAILED:
				return errorOut("Selected file could not be parsed!", pnachFileSelector);
			case OK:
				pnachFileSelector->set_label(patcher->pnachFile->get_basename());
				break;
			default:
				return errorOut("Something unexpected happened!", pnachFileSelector);
			}
		});
	} 
}

void MainApp::startPatch()
{
	patcher->performPatch([this] (auto status)
	{
		switch (status)
		{
		case NO_PNATCH_FILE:
			return showMessage("No Pnach file selected!");
		case NO_ISO_FILE:
			return showMessage("No ISO file selected!");
		case NO_ELF_FILE:
			return showMessage("No ELF file selected!");
		case ELF_PARSING_FAILURE:
			return showMessage("Patching failed! ELF could either not be parsed or written to!");
		case PATCH_WRITE_FAILURE:
			return showMessage("Patching ELF failed. Make sure to reuse the backup!");
		case OK:
			resetBtn(pnachFileSelector);
			resetBtn(isoFileSelector);
			return showMessage("Successfully patched iso!");
		default:
			return errorOut("Something unexpected happened!", pnachFileSelector);
		}
	});
}

void MainApp::onPatchBtnClick()
{
	if (patcher->canPerformPatch([this] (auto status) 
	{
		switch (status)
		{
		case NO_PNATCH_FILE:
			return showMessage("No Pnach file selected!");
		case NO_ISO_FILE:
			return showMessage("No ISO file selected!");
		case NO_ELF_FILE:
			return showMessage("No ELF file selected!");
		}
	}))
	{
		if (createBackupCheck->get_active())
		{
			fileChooser = Gtk::FileChooserNative::create
			(
				"Create Backup",
				*window,
				Gtk::FileChooser::Action::SAVE,
				"Save",
				"Cancel"
			);

			fileChooser->set_current_folder(patcher->isoFile->get_parent());
			fileChooser->set_current_name(patcher->isoFile->get_basename().append(".bak"));

			fileChooser->signal_response().connect(sigc::mem_fun(*this, &MainApp::on_backupfile_click));
			fileChooser->show();
		}
		else
		{
			startPatch();
		}
	}
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

	fileChooser->signal_response().connect(sigc::mem_fun(*this, &MainApp::on_pnachfile_click));
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

	fileChooser->signal_response().connect(sigc::mem_fun(*this, &MainApp::on_isofile_click));

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
	progressLabel = lBuilder->get_widget<Gtk::Label>("progressLabel");
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

void MainApp::resetBtn(Gtk::Button *btn) {
	btn->set_label(UI::defaultNoFile);
}

void MainApp::errorOut(const std::string message, Gtk::Button *btn) {
	resetBtn(btn);
	showMessage(message);
}

MainApp::MainApp()
{
	getWidgets();
	connectSignals();
	patcher = new Patcher();
}

MainApp::~MainApp()
{
	delete patcher;
	delete window;
}
