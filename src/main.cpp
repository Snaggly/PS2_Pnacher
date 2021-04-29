#include "mainapp.h"
#include <gtkmm/application.h>

int main (int argc, char *argv[])
{
  auto app = Gtk::Application::create(argc, argv, "io.github.Snaggly.PS2_Pnacher");
  
  MainApp mainApp;

  return app->run(*mainApp.window);
}
