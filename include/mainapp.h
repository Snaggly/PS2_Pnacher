#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <gtkmm/builder.h>
#include <gtkmm/window.h>
#include <gtkmm/button.h>
#include <gtkmm/filechooserbutton.h>
#include "PnachTools.h"
#include "IsoTools.h"

using namespace PS2PNACHER;

class MainApp
{

public:
  MainApp();
  virtual ~MainApp();
  Gtk::Window* window;

protected:
  void onPatchBtnClick();
  void onPnachFileBtnClick();
  void onIsoFileBtnClick();
  void getWidgets();
  void connectSignals();
  void showError(const char* message);

private:
  Gtk::FileChooserButton* pnachFileSelector;
  Gtk::FileChooserButton* isoFileSelector;
  Gtk::Button* patchBtn;
  PnachTools* pnacher = 0;
  IsoTools* iso = 0;
  
};

#endif