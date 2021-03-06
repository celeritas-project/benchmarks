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
#include <G4SystemOfUnits.hh>
#include <g4root.hh>

#include "SteppingAction.hh"

//---------------------------------------------------------------------------//
//! Default constructor and destructor
SteppingAction::SteppingAction(EventAction* event_action)
    : G4UserSteppingAction(), event_action_(event_action)
{
}

SteppingAction::~SteppingAction() = default;

//---------------------------------------------------------------------------//
/*!
 * Fetch data at every new simulation step and fill the ROOT trees

 * Step status enum is defined in Geant4/source/track/include/G4StepStatus.hh
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
void SteppingAction::UserSteppingAction(const G4Step* step)
{
    /*
    auto analysis_mgr = G4AnalysisManager::Instance();

    // Filling ntuple 0
    auto track       = step->GetTrack();
    auto secondaries = step->GetNumberOfSecondariesInCurrentStep();
    auto pdg         = track->GetParticleDefinition()->GetPDGEncoding();
    auto process
        = step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName();
    auto track_position = track->GetPosition();
    auto track_dir      = track->GetMomentumDirection();
    auto status         = step->GetPreStepPoint()->GetStepStatus();
    auto global_time    = step->GetPostStepPoint()->GetGlobalTime();

    // Ntuple 0 ID and column ID index
    // Makes it a bit easier to add/reorder ntuple columns
    const int nt_0 = 0;
    int       id_0 = 0;

    analysis_mgr->FillNtupleIColumn(nt_0, id_0, event_action_->event_id_);
    id_0++;
    analysis_mgr->FillNtupleIColumn(nt_0, id_0, track->GetTrackID());
    id_0++;
    analysis_mgr->FillNtupleIColumn(nt_0, id_0, track->GetParentID());
    id_0++;
    analysis_mgr->FillNtupleIColumn(nt_0, id_0, track->GetCurrentStepNumber());
    id_0++;
    analysis_mgr->FillNtupleIColumn(nt_0, id_0, secondaries);
    id_0++;
    analysis_mgr->FillNtupleIColumn(nt_0, id_0, pdg);
    id_0++;
    analysis_mgr->FillNtupleSColumn(nt_0, id_0, process);
    id_0++;
    analysis_mgr->FillNtupleDColumn(nt_0, id_0, step->GetTotalEnergyDeposit());
    id_0++;
    analysis_mgr->FillNtupleDColumn(nt_0, id_0, track->GetKineticEnergy());
    id_0++;
    analysis_mgr->FillNtupleDColumn(nt_0, id_0, step->GetStepLength());
    id_0++;
    analysis_mgr->FillNtupleDColumn(nt_0, id_0, track_position.x() / cm);
    id_0++;
    analysis_mgr->FillNtupleDColumn(nt_0, id_0, track_position.y() / cm);
    id_0++;
    analysis_mgr->FillNtupleDColumn(nt_0, id_0, track_position.z() / cm);
    id_0++;
    analysis_mgr->FillNtupleDColumn(nt_0, id_0, track_dir.x());
    id_0++;
    analysis_mgr->FillNtupleDColumn(nt_0, id_0, track_dir.y());
    id_0++;
    analysis_mgr->FillNtupleDColumn(nt_0, id_0, track_dir.z());
    id_0++;
    analysis_mgr->FillNtupleIColumn(nt_0, id_0, status);
    id_0++;
    analysis_mgr->FillNtupleDColumn(nt_0, id_0, global_time);

    analysis_mgr->AddNtupleRow(nt_0);

    // Filling ntuple 1
    if (track->GetCurrentStepNumber() == 1)
    {
        G4ThreeVector vertex     = track->GetVertexPosition();
        G4ThreeVector vertex_dir = track->GetVertexMomentumDirection();
        G4double      vertex_E_k = step->GetTrack()->GetVertexKineticEnergy();

        // Ntuple 1 column ID index
        const int nt_1 = 1;
        int       id_1 = 0;

        analysis_mgr->FillNtupleIColumn(nt_1, id_1, event_action_->event_id_);
        id_1++;
        analysis_mgr->FillNtupleIColumn(nt_1, id_1, track->GetTrackID());
        id_1++;
        analysis_mgr->FillNtupleIColumn(nt_1, id_1, pdg);
        id_1++;
        analysis_mgr->FillNtupleDColumn(nt_1, id_1, vertex_E_k);
        id_1++;
        analysis_mgr->FillNtupleDColumn(nt_1, id_1, vertex.x() / cm);
        id_1++;
        analysis_mgr->FillNtupleDColumn(nt_1, id_1, vertex.y() / cm);
        id_1++;
        analysis_mgr->FillNtupleDColumn(nt_1, id_1, vertex.z() / cm);
        id_1++;
        analysis_mgr->FillNtupleDColumn(nt_1, id_1, vertex_dir.x());
        id_1++;
        analysis_mgr->FillNtupleDColumn(nt_1, id_1, vertex_dir.y());
        id_1++;
        analysis_mgr->FillNtupleDColumn(nt_1, id_1, vertex_dir.z());

        analysis_mgr->AddNtupleRow(nt_1);
    }
    */
}