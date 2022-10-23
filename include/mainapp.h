#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtkmm/builder.h>
#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/filechoosernative.h>
#include <sstream>
#include "Patcher.h"

using namespace PS2PNACHER;

class MainApp
{

public:
  MainApp();
  ~MainApp();
  Gtk::Window *window;

protected:
  void onPatchBtnClick();
  void onPnachFileBtnClick();
  void onIsoFileBtnClick();
  void getWidgets();
  void connectSignals();
  void resetBtn(Gtk::Button *btn);
  void errorOut(const std::string message, Gtk::Button *btn);
  void showMessage(std::string message);

private:
  void startPatch();
  void on_backupfile_click(int response_id);
  void on_isofile_click(int response_id);
  void on_pnachfile_click(int response_id);
  Gtk::Button* pnachFileSelector;
  Gtk::Button* isoFileSelector;
  Gtk::Button* patchBtn;
  Gtk::Label* progressLabel;
  Gtk::CheckButton *createBackupCheck;
  Glib::RefPtr<Gtk::MessageDialog> messagedialog;
  Glib::RefPtr<Gtk::FileChooserNative> fileChooser;
  Patcher* patcher;
};

#endif