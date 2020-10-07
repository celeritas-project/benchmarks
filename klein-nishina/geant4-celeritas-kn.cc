//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file geant4-celeritas-kn.cc
//! \brief Geant4 Klein-Nishina Compton scattering app
//---------------------------------------------------------------------------//

#include <G4RunManager.hh>
#include <G4UImanager.hh>
#include <G4VisExecutive.hh>
#include <G4UIExecutive.hh>

#include "src/DetectorConstruction.hh"
#include "src/PhysicsList.hh"
#include "src/ActionInitialization.hh"

//---------------------------------------------------------------------------//
/*!
 * Small experiment to compare G4 Klein-Nishina Compton with Celeritas demo 
 * app. The code
 * 
 * 1. Loads a simple geometry
 *    - 100 km aluminum cube
 * 
 * 2. Loads particle / physics lists:
 *    - Particles: gammas, electrons, and positrons 
 *    - Physics  : transport (all); Klein-Nishina Compton scattering (gammas)
 * 
 * 3. Uses gamma as the primary particle for the particle gun 
 * 
 * 4. Stores simulation data into a ROOT output file
 */
int main(int argc, char **argv)
{
    G4RunManager run_manager;
    run_manager.SetVerboseLevel(3);

    // Initialize the geometry
    auto detector = std::make_unique<DetectorConstruction>();
    run_manager.SetUserInitialization(detector.release());

    // Load the physics list
    auto physics_list = std::make_unique<PhysicsList>();
    run_manager.SetUserInitialization(physics_list.release());

    // Initialize simulation
    auto action_initialization = std::make_unique<ActionInitialization>();
    run_manager.SetUserInitialization(action_initialization.release());

    G4UImanager *ui_manager = G4UImanager::GetUIpointer();
    ui_manager->ApplyCommand("/run/initialize");

    // Initialize visualization manager
    //auto vis_manager = std::make_unique<G4VisExecutive>();
    //vis_manager->Initialize();

    // Set up interface
    //auto user_interface = std::make_unique<G4UIExecutive>(argc, argv);
    //ui_manager->ApplyCommand("/control/execute vis.mac");

    // Run events
    ui_manager->ApplyCommand("/run/beamOn 1");

    // Dump physics tables
    //ui_manager->ApplyCommand("/run/particle/storePhysicsTable tables/");
    //ui_manager->ApplyCommand("/run/particle/setStoredInAscii 1");

    // Open interface
    //user_interface->SessionStart();
}
