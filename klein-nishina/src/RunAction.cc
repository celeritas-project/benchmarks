//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file RunAction.cc
//---------------------------------------------------------------------------//

#include <G4RunManager.hh>
#include <g4root.hh>

#include "RunAction.hh"

//---------------------------------------------------------------------------//
//! Default constructor and destructor
RunAction::RunAction() : G4UserRunAction() {}

RunAction::~RunAction() = default;

//---------------------------------------------------------------------------//
//! Create the root file and its ntuple structure
void RunAction::BeginOfRunAction(const G4Run* run)
{
    auto analysis_manager = G4AnalysisManager::Instance();
    analysis_manager->OpenFile("out.root");
    CreateRootNtuples();
}

//---------------------------------------------------------------------------//
//! Write the root file to disk
void RunAction::EndOfRunAction(const G4Run* /*run*/)
{
    G4AnalysisManager* analysis_manager = G4AnalysisManager::Instance();
    analysis_manager->Write();
    analysis_manager->CloseFile();
}

//---------------------------------------------------------------------------//
//! Create the root ntuple structure at RunAction::BeginOfRunAction()
//! Ntuples are filled in SteppingAction.cc: UserSteppingAction()
void RunAction::CreateRootNtuples()
{
    G4AnalysisManager* analysis_manager = G4AnalysisManager::Instance();

    // ntuple ID = 0
    analysis_manager->CreateNtuple("step", "step");
    analysis_manager->CreateNtupleIColumn("event");
    analysis_manager->CreateNtupleIColumn("track_id");
    analysis_manager->CreateNtupleIColumn("parent_id");
    analysis_manager->CreateNtupleIColumn("step_number");
    analysis_manager->CreateNtupleIColumn("pdg");
    analysis_manager->CreateNtupleSColumn("process");
    analysis_manager->CreateNtupleDColumn("energy_loss");
    analysis_manager->CreateNtupleDColumn("kinetic_energy");
    analysis_manager->CreateNtupleDColumn("length");
    analysis_manager->CreateNtupleDColumn("x");
    analysis_manager->CreateNtupleDColumn("y");
    analysis_manager->CreateNtupleDColumn("z");
    analysis_manager->CreateNtupleDColumn("dir_x");
    analysis_manager->CreateNtupleDColumn("dir_y");
    analysis_manager->CreateNtupleDColumn("dir_z");
    analysis_manager->CreateNtupleIColumn("status");
    analysis_manager->CreateNtupleDColumn("global_time");
    analysis_manager->FinishNtuple();

    // ntuple ID = 1
    analysis_manager->CreateNtuple("vertex", "vertex");
    analysis_manager->CreateNtupleIColumn("pdg");
    analysis_manager->CreateNtupleDColumn("kinetic_energy");
    analysis_manager->CreateNtupleDColumn("x");
    analysis_manager->CreateNtupleDColumn("y");
    analysis_manager->CreateNtupleDColumn("z");
    analysis_manager->CreateNtupleDColumn("dir_x");
    analysis_manager->CreateNtupleDColumn("dir_y");
    analysis_manager->CreateNtupleDColumn("dir_z");
    analysis_manager->FinishNtuple();
}