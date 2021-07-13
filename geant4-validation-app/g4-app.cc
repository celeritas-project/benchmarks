//----------------------------------*-C++-*----------------------------------//
// Copyright 2021 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file g4-app.cc
//! \brief Geant4 validation app.
//---------------------------------------------------------------------------//
#include <fstream>

#include <G4RunManager.hh>
#include <G4UImanager.hh>
#include <G4VisExecutive.hh>
#include <G4UIExecutive.hh>
#include <G4GDMLParser.hh>

#include "src/DetectorConstruction.hh"
#include "src/PhysicsList.hh"
#include "src/ActionInitialization.hh"
#include "src/JsonReader.hh"
#include "src/HepMC3Reader.hh"

//---------------------------------------------------------------------------//
/*!
 * Small validation app. All options are selected via a json input file.
 *
 * - Load geometry (simplified cms).
 * - Load physics list.
 * - Run a simulation using the particle gun or a hepmc3 input.
 * - Store data into a ROOT output file.
 */
int main(int argc, char** argv)
{
    if (argc != 2)
    {
        // Print help message
        std::cout << argv[0] << " must be initialized with a json input file."
                  << std::endl;
        return EXIT_FAILURE;
    }

    // Parse json input file
    std::ifstream input_stream(argv[1]);
    JsonReader::construct(input_stream);
    const auto json = JsonReader::get_instance()->json();

    const std::string hepmc3_input
        = json.at("simulation").at("hepmc3").get<std::string>();

    if (!hepmc3_input.empty())
    {
        // Initialize HepMC3 reader
        HepMC3Reader::construct();
    }

    // Initialize run manager
    G4RunManager run_manager;
    run_manager.SetVerboseLevel(
        json.at("verbosity").at("RunManager").get<int>());

    // Initialize geometry
    run_manager.SetUserInitialization(new DetectorConstruction());

    // Initialize physics list
    run_manager.SetUserInitialization(new PhysicsList());

    // Initialize simulation
    run_manager.SetUserInitialization(new ActionInitialization());

    // Initialize user interface
    G4UImanager*    ui_manager = G4UImanager::GetUIpointer();
    G4UIExecutive*  user_interface;
    G4VisExecutive* vis_manager;
    ui_manager->ApplyCommand("/run/initialize");

    // Set up GUI
    bool gui_interface = json.at("GUI").get<bool>();
    if (gui_interface)
    {
        user_interface = new G4UIExecutive(argc, argv);
        vis_manager    = new G4VisExecutive();
        vis_manager->Initialize();
        std::string vis_macro = "/control/execute "
                                + json.at("vis_macro").get<std::string>();
        ui_manager->ApplyCommand(vis_macro);
    }

    // Set up the number of events to be passed to the UI manager
    std::string run_beamOn = "/run/beamOn ";

    if (hepmc3_input.empty())
    {
        // Use particle gun with the number of events defined in the json file
        run_beamOn += json.at("events").get<std::string>();
    }
    else
    {
        // Use HepMC3Reader to get the correct number of events
        run_beamOn += std::to_string(
            HepMC3Reader::get_instance()->number_of_events());
    }

    // Run events
    ui_manager->ApplyCommand(run_beamOn.c_str());

    const bool export_gdml = json.at("export_gdml").get<bool>();
    if (export_gdml)
    {
        // Export gdml
        G4GDMLParser parser;
        std::string  gdml_name  = "simple_cms.gdml";
        std::string  export_cmd = "/persistency/gdml/write " + gdml_name;
        ui_manager->ApplyCommand("/persistency/gdml/export_SD true");
        ui_manager->ApplyCommand("/persistency/gdml/export_Ecuts true");
        ui_manager->ApplyCommand(export_cmd);
    }

    if (gui_interface)
    {
        // Open GUI
        user_interface->SessionStart();
    }

    return EXIT_SUCCESS;
}
