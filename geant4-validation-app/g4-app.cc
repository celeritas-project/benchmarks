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

#include "src/DetectorConstruction.hh"
#include "src/PhysicsList.hh"
#include "src/ActionInitialization.hh"
#include "src/JsonReader.hh"

//---------------------------------------------------------------------------//
/*!
 * Small validation app. All options are selected via a json input file.
 *
 * - Load geometry (programmatically or via a gdml input)
 * - Load physics list.
 * - Run a simulation using the particle gun.
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

    // Initialize run manager
    G4RunManager run_manager;
    run_manager.SetVerboseLevel(
        json.at("verbosity").at("RunManager").get<int>());

    // Initialize geometry
    std::string gdml_input = json.at("gdml").get<std::string>();

    if (gdml_input.empty())
    {
        run_manager.SetUserInitialization(new DetectorConstruction());
    }

    else
    {
        run_manager.SetUserInitialization(new DetectorConstruction(gdml_input));
    }

    // Load physics list
    run_manager.SetUserInitialization(new PhysicsList());

    // Initialize simulation
    run_manager.SetUserInitialization(new ActionInitialization());

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

    // Run events
    std::string run_beamOn = "/run/beamOn "
                             + json.at("events").get<std::string>();
    ui_manager->ApplyCommand(run_beamOn.c_str());

    if (gui_interface)
    {
        // Open GUI
        user_interface->SessionStart();
    }

    return EXIT_SUCCESS;
}
