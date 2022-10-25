project('io.github.Snaggly.PS2_Pnacher', 
        ['cpp', 'c'], 
        version: '1.1', 
        meson_version: '>= 0.59.0', 
        default_options: ['warning_level=3', 'werror=false', 'cpp_std=c++20']
    )

gnome = import('gnome')

gtk = dependency('gtkmm-4.0')
glib = dependency('glib-2.0')
cdio = dependency('libcdio++')
iso = dependency('libiso9660++')

incDir = include_directories('include')
sources = files(
    'src/main.cpp',
    'src/mainapp.cpp',
    'src/ElfTools.cpp',
    'src/IsoTools.cpp',
    'src/Patcher.cpp',
    'src/PnachTools.cpp'
)


executable('io.github.Snaggly.PS2_Pnacher', sources, include_directories: incDir, dependencies: [gtk, glib, cdio, iso], install: true)
install_data('res/io.github.Snaggly.PS2_Pnacher.desktop', install_dir: 'share/applications')
install_data('res/io.github.Snaggly.PS2_Pnacher.svg', install_dir: 'share/icons/hicolor/scalable/apps')
install_data('res/io.github.Snaggly.PS2_Pnacher_gui.glade', install_dir: 'share/ps2pnacher')

gnome.post_install(gtk_update_icon_cache: true, update_desktop_database: true)