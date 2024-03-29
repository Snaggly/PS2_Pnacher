#pragma once

#include <string>

namespace PS2PNACHER
{
  namespace UI
  {
    const std::string ui_xml = R"(
<?xml version="1.0" encoding="UTF-8"?>
<!-- Generated with glade 3.38.2 -->
<!-- Edited by Snaggly to work with GTK4 -->
<interface>
  <requires lib="gtk+" version="4.0" />
  <object class="GtkWindow" id="window">
    <property name="can-focus">False</property>
    <property name="title" translatable="yes">PS2 Pnacher</property>
    <!--property name="window-position">center</property-->
    <property name="default-width">500</property>
    <property name="default-height">200</property>
    <property name="icon-name">io.github.Snaggly.PS2_Pnacher</property>
    <child>
      <object class="GtkBox" id="rootBox">
        <property name="visible">True</property>
        <property name="can-focus">False</property>
        <property name="margin-start">5</property>
        <property name="margin-end">5</property>
        <property name="margin-top">5</property>
        <property name="margin-bottom">5</property>
        <property name="orientation">vertical</property>
        <property name="spacing">10</property>
        <child>
          <object class="GtkLabel">
            <property name="visible">True</property>
            <property name="can-focus">False</property>
            <property name="label" translatable="yes">PS2 Pnacher</property>
            <attributes>
              <attribute name="weight" value="bold" />
              <attribute name="size" value="16384" />
            </attributes>
          </object>
          <!--packing>
            <property name="expand">False</property>
            <property name="fill">True</property>
            <property name="position">0</property>
          </packing-->
        </child>
        <child>
          <object class="GtkFrame">
            <property name="visible">True</property>
            <property name="can-focus">False</property>
            <property name="margin-start">10</property>
            <property name="margin-end">10</property>
            <property name="margin-top">10</property>
            <property name="margin-bottom">10</property>
            <!--property name="label-xalign">0</property>
            <property name="label-yalign">0</property>
            <property name="shadow-type">out</property-->
            <child>
              <object class="GtkBox">
                <property name="visible">True</property>
                <property name="can-focus">False</property>
                <property name="orientation">vertical</property>
                <child>
                  <object class="GtkBox">
                    <property name="visible">True</property>
                    <property name="can-focus">False</property>
                    <property name="margin-top">15</property>
                    <property name="margin-bottom">15</property>
                    <property name="spacing">10</property>
                    <property name="homogeneous">True</property>
                    <child>
                      <object class="GtkLabel">
                        <property name="visible">True</property>
                        <property name="can-focus">False</property>
                        <property name="label" translatable="yes">Select Pnach file: </property>
                      </object>
                      <!--packing>
                        <property name="expand">False</property>
                        <property name="fill">True</property>
                        <property name="position">0</property>
                      </packing-->
                    </child>
                    <child>
                      <!--object class="GtkFileChooserButton" id="pnachFileSelector"-->
                      <object class="GtkButton" id="pnachFileSelector">
                        <property name="visible">True</property>
                        <property name="can-focus">False</property>
                        <property name="margin-end">20</property>
                        <property name="label" translatable="yes">No file selected!</property>
                      </object>
                      <!--packing>
                        <property name="expand">False</property>
                        <property name="fill">True</property>
                        <property name="position">1</property>
                      </packing-->
                    </child>
                  </object>
                  <!--packing>
                    <property name="expand">False</property>
                    <property name="fill">True</property>
                    <property name="position">0</property>
                  </packing-->
                </child>
                <child>
                  <object class="GtkBox">
                    <property name="visible">True</property>
                    <property name="can-focus">False</property>
                    <property name="margin-bottom">15</property>
                    <property name="spacing">10</property>
                    <property name="homogeneous">True</property>
                    <child>
                      <object class="GtkLabel">
                        <property name="visible">True</property>
                        <property name="can-focus">False</property>
                        <property name="label" translatable="yes">Select PS2 ISO: </property>
                      </object>
                      <!--packing>
                        <property name="expand">False</property>
                        <property name="fill">True</property>
                        <property name="position">0</property>
                      </packing-->
                    </child>
                    <child>
                      <!--object class="GtkFileChooserButton" id="isoFileSelector"-->
                      <object class="GtkButton" id="isoFileSelector">
                        <property name="visible">True</property>
                        <property name="can-focus">False</property>
                        <property name="margin-end">20</property>
                        <property name="label" translatable="yes">No file selected!</property>
                      </object>
                      <!--packing>
                        <property name="expand">False</property>
                        <property name="fill">True</property>
                        <property name="position">1</property>
                      </packing-->
                    </child>
                  </object>
                  <!--packing>
                    <property name="expand">False</property>
                    <property name="fill">True</property>
                    <property name="position">1</property>
                  </packing-->
                </child>
                <child>
                  <object class="GtkCheckButton" id="createBackupCeck">
                    <property name="label" translatable="yes">Create Backup</property>
                    <property name="active">True</property>
                    <property name="visible">True</property>
                    <property name="can-focus">True</property>
                    <property name="receives-default">True</property>
                    <property name="halign">end</property>
                    <property name="margin-end">5</property>
                    <property name="margin-bottom">5</property>
                  </object>
                </child>
                <child>
                  <object class="GtkLabel" id="progressLabel">
                    <property name="label" translatable="no"></property>
                    <property name="visible">True</property>
                    <property name="can-focus">True</property>
                    <property name="halign">center</property>
                  </object>
                </child>
              </object>
            </child>
            <child type="label_item">
              <placeholder />
            </child>
          </object>
          <!--packing>
            <property name="expand">True</property>
            <property name="fill">False</property>
            <property name="position">1</property>
          </packing-->
        </child>
        <child>
          <object class="GtkButton" id="patchBtn">
            <property name="label" translatable="yes">Patch!</property>
            <property name="visible">True</property>
            <property name="can-focus">True</property>
            <property name="receives-default">True</property>
            <property name="halign">end</property>
            <property name="margin-end">30</property>
            <property name="margin-bottom">20</property>
          </object>
          <!--packing>
            <property name="expand">False</property>
            <property name="fill">True</property>
            <property name="pack-type">end</property>
            <property name="position">4</property>
          </packing-->
        </child>
      </object>
    </child>
  </object>
</interface>
        )";

    const std::string defaultNoFile = "No file selected!";
  }
}