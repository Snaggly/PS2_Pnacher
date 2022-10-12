#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtkmm/builder.h>
#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/filechoosernative.h>
#include "PnachTools.h"
#include "IsoTools.h"
#include "ElfTools.h"
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
  template<typename T> void resetBtn(Gtk::Button *btn, T *&t_ptr);
  template<typename T> void errorOut(const std::string message, Gtk::Button *btn, T *&t_ptr);
  void showMessage(std::string message);

private:
  Gtk::Button *pnachFileSelector;
  Gtk::Button *isoFileSelector;
  Gtk::Button *patchBtn;
  Gtk::CheckButton *createBackupCheck;
  Glib::RefPtr<Gtk::MessageDialog> messagedialog;
  Glib::RefPtr<Gtk::FileChooserNative> fileChooser;
  PnachTools *pnacher = 0;
  IsoTools *iso = 0;
  IsoRecord *elfFile = 0;
  Glib::RefPtr<Gio::File> isoFile;
  Glib::RefPtr<Gio::File> pnachFile;
};

#endif