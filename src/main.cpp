#include "mainapp.h"
#include <gtkmm/application.h>
#include <gtkmm/applicationwindow.h>

int main(int argc, char *argv[])
{
  auto app = Gtk::Application::create("io.github.Snaggly.PS2_Pnacher", Gtk::Application::Flags::NONE);
  MainApp mainApp;
  auto mainWindow = mainApp.window;

  app->signal_startup().connect([app, mainWindow]
                                {
    app->add_window(*mainWindow);
    mainWindow->show(); 
  });

  int status = app->run(argc, argv);

  return status;
}
