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
#include <nlohmann/json.hpp>

#include "src/DetectorConstruction.hh"
#include "src/PhysicsList.hh"
#include "src/ActionInitialization.hh"

//---------------------------------------------------------------------------//
/*!
 * Small validation app. All options are selected via a json input file.
 *
 * 1. Load geometry (programmatically or via a gdml input)
 * 2. Load physics list.
 * 3. Run a simulation using the particle gun.
 * 4. Store data into a ROOT output file.
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

    // >>> PARSE JSON INPUT
    std::ifstream  input_stream(argv[1]);
    nlohmann::json json = nlohmann::json::parse(input_stream);

    // >>> INITIALIZE RUN MANAGER
    G4RunManager run_manager;
    run_manager.SetVerboseLevel(0);

    // >>> CONSTRUCT GEOMETRY
    std::unique_ptr<DetectorConstruction> detector;
    std::string gdml_input = json.at("gdml").get<std::string>();
    if (!gdml_input.size())
    {
        detector = std::make_unique<DetectorConstruction>();
    }
    else
    {
        detector = std::make_unique<DetectorConstruction>(gdml_input);
    }
    run_manager.SetUserInitialization(detector.release());

    // >>> LOAD PHYSICS LIST
    auto physics_list = std::make_unique<PhysicsList>(json);
    run_manager.SetUserInitialization(physics_list.release());

    // >>> INITIALIZE SIMULATION
    auto action_initialization = std::make_unique<ActionInitialization>(json);
    run_manager.SetUserInitialization(action_initialization.release());

    // >>> INITIALIZE USER INTERFACE
    G4UImanager*    ui_manager = G4UImanager::GetUIpointer();
    G4UIExecutive*  user_interface;
    G4VisExecutive* vis_manager;
    ui_manager->ApplyCommand("/run/initialize");

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

    // >>> RUN EVENTS
    std::string run_beamOn = "/run/beamOn "
                             + json.at("events").get<std::string>();
    ui_manager->ApplyCommand(run_beamOn.c_str());

    if (gui_interface)
    {
        // Open interface
        user_interface->SessionStart();
    }

    return EXIT_SUCCESS;
}
