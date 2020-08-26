//----------------------------------*-C++-*----------------------------------//
// Copyright 2020 UT-Battelle, LLC, and other Celeritas developers.
// See the top-level COPYRIGHT file for details.
// SPDX-License-Identifier: (Apache-2.0 OR MIT)
//---------------------------------------------------------------------------//
//! \file SteppingAction.cc
//---------------------------------------------------------------------------//

#include <G4RunManager.hh>
#include <G4Step.hh>
#include <G4Event.hh>
#include <G4Run.hh>
#include <g4root.hh>

#include "SteppingAction.hh"

//---------------------------------------------------------------------------//
//! Default constructor and destructor
SteppingAction::SteppingAction(EventAction *event_action)
    : G4UserSteppingAction(), event_action_(event_action)
{
}

SteppingAction::~SteppingAction() = default;

//---------------------------------------------------------------------------//
/*!
 * Fetch data at every new simulation step and fill the ROOT trees

 * Step status is an enum defined in source/track/include/G4StepStatus.hh
 * 
 *  Status  Definition
 * --------------------------------------------------------------------
 *    0     Step reached the world boundary
 *    1     Step defined by a geometry boundary
 *    2     Step defined by a PreStepDoItVector
 *    3     Step defined by a AlongStepDoItVector
 *    4     Step defined by a PostStepDoItVector
 *    5     Step defined by the user Step limit in the logical volume
 *    6     Step defined by an exclusively forced PostStepDoIt process
 *    7     Step not defined yet
 * --------------------------------------------------------------------
 * 
 * Regarding the vertex tree:
 * Geant4 list of secondaries seems to be processed using the FILO rule: The 
 * first electron created will be the last entry in the vertex tree.
 */
void SteppingAction::UserSteppingAction(const G4Step *step)
{
    auto analysis_manager = G4AnalysisManager::Instance();

    //! Filling ntuple 0
    auto track = step->GetTrack();
    auto pdg = track->GetParticleDefinition()->GetPDGEncoding();
    auto process = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
    G4ThreeVector track_position = track->GetPosition();
    G4ThreeVector track_dir = track->GetMomentumDirection();
    G4int status = step->GetPreStepPoint()->GetStepStatus();
    G4double global_time = step->GetPostStepPoint()->GetGlobalTime();

    int id_0 = 0; // columnID increment for ntuple 0

    analysis_manager->FillNtupleIColumn(id_0, event_action_->event_id_);
    id_0++;
    analysis_manager->FillNtupleIColumn(id_0, track->GetTrackID());
    id_0++;
    analysis_manager->FillNtupleIColumn(id_0, track->GetParentID());
    id_0++;
    analysis_manager->FillNtupleIColumn(id_0, track->GetCurrentStepNumber());
    id_0++;
    analysis_manager->FillNtupleIColumn(id_0, pdg);
    id_0++;
    analysis_manager->FillNtupleSColumn(id_0, process);
    id_0++;
    analysis_manager->FillNtupleDColumn(id_0, step->GetTotalEnergyDeposit());
    id_0++;
    analysis_manager->FillNtupleDColumn(id_0, track->GetKineticEnergy());
    id_0++;
    analysis_manager->FillNtupleDColumn(id_0, step->GetStepLength());
    id_0++;
    analysis_manager->FillNtupleDColumn(id_0, track_position.x());
    id_0++;
    analysis_manager->FillNtupleDColumn(id_0, track_position.y());
    id_0++;
    analysis_manager->FillNtupleDColumn(id_0, track_position.z());
    id_0++;
    analysis_manager->FillNtupleDColumn(id_0, track_dir.x());
    id_0++;
    analysis_manager->FillNtupleDColumn(id_0, track_dir.y());
    id_0++;
    analysis_manager->FillNtupleDColumn(id_0, track_dir.z());
    id_0++;
    analysis_manager->FillNtupleIColumn(id_0, status);
    id_0++;
    analysis_manager->FillNtupleDColumn(id_0, global_time);

    analysis_manager->AddNtupleRow();

    //! Filling ntuple 1
    if (track->GetCurrentStepNumber() == 1)
    {
        G4ThreeVector vertex = track->GetVertexPosition();
        G4ThreeVector vertex_dir = track->GetVertexMomentumDirection();
        G4double vertex_E_k = step->GetTrack()->GetVertexKineticEnergy();

        int id_1 = 0; // columnID increment for ntuple 1

        analysis_manager->FillNtupleIColumn(1, id_1, pdg);
        id_1++;
        analysis_manager->FillNtupleDColumn(1, id_1, vertex_E_k);
        id_1++;
        analysis_manager->FillNtupleDColumn(1, id_1, vertex.x());
        id_1++;
        analysis_manager->FillNtupleDColumn(1, id_1, vertex.y());
        id_1++;
        analysis_manager->FillNtupleDColumn(1, id_1, vertex.z());
        id_1++;
        analysis_manager->FillNtupleDColumn(1, id_1, vertex_dir.x());
        id_1++;
        analysis_manager->FillNtupleDColumn(1, id_1, vertex_dir.y());
        id_1++;
        analysis_manager->FillNtupleDColumn(1, id_1, vertex_dir.z());

        analysis_manager->AddNtupleRow(1);
    }
}