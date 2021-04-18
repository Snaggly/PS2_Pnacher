#include "mainapp.h"
#include <gtkmm/application.h>

int main (int argc, char *argv[])
{
  auto app = Gtk::Application::create(argc, argv, "com.snaggly.ps2pnach");
  
  MainApp mainApp;

  return app->run(*mainApp.window);
}